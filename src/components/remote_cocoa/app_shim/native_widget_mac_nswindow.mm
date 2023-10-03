// Copyright 2014 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "components/remote_cocoa/app_shim/native_widget_mac_nswindow.h"

#include "base/apple/foundation_util.h"
#include "base/auto_reset.h"
#include "base/debug/dump_without_crashing.h"
#include "base/mac/mac_util.h"
#include "base/memory/raw_ptr_exclusion.h"
#include "base/trace_event/trace_event.h"
#import "components/remote_cocoa/app_shim/browser_native_widget_window_mac.h"
#import "components/remote_cocoa/app_shim/native_widget_ns_window_bridge.h"
#include "components/remote_cocoa/app_shim/native_widget_ns_window_host_helper.h"
#import "components/remote_cocoa/app_shim/views_nswindow_delegate.h"
#import "components/remote_cocoa/app_shim/window_touch_bar_delegate.h"
#include "components/remote_cocoa/common/native_widget_ns_window_host.mojom.h"
#import "ui/base/cocoa/user_interface_item_command_handler.h"
#import "ui/base/cocoa/window_size_constants.h"

namespace {

bool AreWindowShadowsDisabled() {
  // When:
  // 1) Shadows are being generated by the window server
  // 2) The window with the shadow has a layer (all of Chrome's do)
  // 3) Software compositing is in use (it is in most test configs, which
  //    run in VMs)
  // 4) There are many windows in use at once (they are when running
  //    test in parallel)
  // The window server seems to crash with distressing frequency. To hopefully
  // mitigate that, disable window shadows when running on a bot.
  // For context on this see:
  //   https://crbug.com/899286
  //   https://crbug.com/828031
  //   https://crbug.com/515627, especially #63 and #67
  static bool is_headless = getenv("CHROME_HEADLESS") != nullptr;
  return is_headless;
}

// AppKit quirk: -[NSWindow orderWindow] does not handle reordering for children
// windows. Their order is fixed to the attachment order (the last attached
// window is on the top). Therefore, work around it by re-parenting in our
// desired order.
void OrderChildWindow(NSWindow* child_window,
                      NSWindow* other_window,
                      NSWindowOrderingMode ordering_mode) {
  NSWindow* parent = [child_window parentWindow];
  DCHECK(parent);

  // `ordered_children` sorts children windows back to front.
  NSArray<NSWindow*>* children = [[child_window parentWindow] childWindows];
  std::vector<std::pair<NSInteger, NSWindow*>> ordered_children;
  for (NSWindow* child in children) {
    ordered_children.emplace_back([child orderedIndex], child);
  }
  std::sort(ordered_children.begin(), ordered_children.end(), std::greater<>());

  // If `other_window` is nullptr, place `child_window` in front of (or behind)
  // all other children windows.
  if (other_window == nullptr) {
    other_window = ordering_mode == NSWindowAbove
                       ? ordered_children.back().second
                       : parent;
  }

  if (child_window == other_window) {
    return;
  }

  const bool relative_to_parent = parent == other_window;
  DCHECK(ordering_mode != NSWindowBelow || !relative_to_parent)
      << "Placing a child window behind its parent is not supported.";

  for (NSWindow* child in children) {
    [parent removeChildWindow:child];
  }

  // If `relative_to_parent` is true, `child_window` is the first child of its
  // parent.
  if (relative_to_parent) {
    [parent addChildWindow:child_window ordered:NSWindowAbove];
  }

  // Re-parent children windows in the desired order.
  for (auto [ordered_index, child] : ordered_children) {
    if (child != child_window && child != other_window) {
      [parent addChildWindow:child ordered:NSWindowAbove];
    } else if (child == other_window && !relative_to_parent) {
      if (ordering_mode == NSWindowAbove) {
        [parent addChildWindow:other_window ordered:NSWindowAbove];
        [parent addChildWindow:child_window ordered:NSWindowAbove];
      } else {
        [parent addChildWindow:child_window ordered:NSWindowAbove];
        [parent addChildWindow:other_window ordered:NSWindowAbove];
      }
    }
  }
}

// A struct to record child window ordering commands.
struct ChildWindowOrderingCommand {
  NSWindowOrderingMode windowOrderingMode;
  NSInteger otherWindowNumber;

  bool operator!=(const ChildWindowOrderingCommand& other) const {
    return this->windowOrderingMode != other.windowOrderingMode ||
           this->otherWindowNumber != other.otherWindowNumber;
  }
};

}  // namespace

@interface NSWindow (Private)
+ (Class)frameViewClassForStyleMask:(NSWindowStyleMask)windowStyle;
- (BOOL)hasKeyAppearance;
- (long long)_resizeDirectionForMouseLocation:(CGPoint)location;
- (BOOL)_isConsideredOpenForPersistentState;
- (void)_zoomToScreenEdge:(NSUInteger)edge;
@end

// Private API as of at least macOS 13.
@interface NSWindow (NSWindow_Theme)
- (void)_regularMinimizeToDock;
@end

@interface NativeWidgetMacNSWindow () <NSKeyedArchiverDelegate>
- (ViewsNSWindowDelegate*)viewsNSWindowDelegate;
- (BOOL)hasViewsMenuActive;
- (id<NSAccessibility>)rootAccessibilityObject;

// Private API on NSWindow, determines whether the title is drawn on the title
// bar. The title is still visible in menus, Expose, etc.
- (BOOL)_isTitleHidden;

// Completes the processing of child windows whose removal or ordering was
// deferred while we were fullscreen and not in the active space.
- (void)processDeferredChildWindowOperations;

// Executes any window ordering commands that were requested while the child
// window was not on the active space. We collect them rather than execute
// them to avoid triggering a Space change.
- (void)processChildWindowOrderingCommands;

@end

// Use this category to implement mouseDown: on multiple frame view classes
// with different superclasses.
@interface NSView (CRFrameViewAdditions)
- (void)cr_mouseDownOnFrameView:(NSEvent*)event;
@end

@implementation NSView (CRFrameViewAdditions)
// If a mouseDown: falls through to the frame view, turn it into a window drag.
- (void)cr_mouseDownOnFrameView:(NSEvent*)event {
  if ([self.window _resizeDirectionForMouseLocation:event.locationInWindow] !=
      -1)
    return;
  [self.window performWindowDragWithEvent:event];
}
@end

@implementation NativeWidgetMacNSWindowTitledFrame
- (void)mouseDown:(NSEvent*)event {
  if (self.window.isMovable)
    [self cr_mouseDownOnFrameView:event];
  [super mouseDown:event];
}
- (BOOL)usesCustomDrawing {
  return NO;
}
// The base implementation just tests [self class] == [NSThemeFrame class].
- (BOOL)_shouldFlipTrafficLightsForRTL {
  return [[self window] windowTitlebarLayoutDirection] ==
         NSUserInterfaceLayoutDirectionRightToLeft;
}
@end

@implementation NativeWidgetMacNSWindowBorderlessFrame
- (void)mouseDown:(NSEvent*)event {
  [self cr_mouseDownOnFrameView:event];
  [super mouseDown:event];
}
- (BOOL)usesCustomDrawing {
  return NO;
}
@end

@implementation NativeWidgetMacNSWindow {
 @private
  CommandDispatcher* __strong _commandDispatcher;
  id<UserInterfaceItemCommandHandler> __strong _commandHandler;
  id<WindowTouchBarDelegate> __weak _touchBarDelegate;
  NSMutableArray<NSWindow*>* _childWindowsToRemove;
  std::vector<ChildWindowOrderingCommand> _windowOrderingCommands;
  uint64_t _bridgedNativeWidgetId;
  // This field is not a raw_ptr<> because it requires @property rewrite.
  RAW_PTR_EXCLUSION remote_cocoa::NativeWidgetNSWindowBridge* _bridge;
  BOOL _willUpdateRestorableState;
  BOOL _isEnforcingNeverMadeVisible;
  BOOL _preventKeyWindow;
  BOOL _isTooltip;
  BOOL _isHeadless;
  BOOL _isShufflingForOrdering;
  BOOL _miniaturizationInProgress;
  BOOL _isOrderingOut;
}
@synthesize bridgedNativeWidgetId = _bridgedNativeWidgetId;
@synthesize bridge = _bridge;
@synthesize isTooltip = _isTooltip;
@synthesize isHeadless = _isHeadless;
@synthesize isShufflingForOrdering = _isShufflingForOrdering;
@synthesize childWindowAddedHandler = _childWindowAddedHandler;
@synthesize childWindowRemovedHandler = _childWindowRemovedHandler;
@synthesize commandDispatchParentOverride = _commandDispatchParentOverride;

- (instancetype)initWithContentRect:(NSRect)contentRect
                          styleMask:(NSUInteger)windowStyle
                            backing:(NSBackingStoreType)bufferingType
                              defer:(BOOL)deferCreation {
  DCHECK(NSEqualRects(contentRect, ui::kWindowSizeDeterminedLater));
  if ((self = [super initWithContentRect:ui::kWindowSizeDeterminedLater
                               styleMask:windowStyle
                                 backing:bufferingType
                                   defer:deferCreation])) {
    _commandDispatcher = [[CommandDispatcher alloc] initWithOwner:self];
    self.releasedWhenClosed = NO;
  }
  return self;
}

// This is called by the "Move Window to {Left/Right} Side of Screen"
// Window menu alternate items (must press Option to see).
// Without this, selecting these items will move child windows like
// bubbles and the find bar, but these should not be movable.
// Instead, let's push this up to the parent window which should be
// the browser.
- (void)_zoomToScreenEdge:(NSUInteger)edge {
  if (self.parentWindow) {
    [self.parentWindow _zoomToScreenEdge:edge];
  } else {
    [super _zoomToScreenEdge:edge];
  }
}

// This override helps diagnose lifetime issues in crash stacktraces by
// inserting a symbol on NativeWidgetMacNSWindow and should be kept even if it
// does nothing.
- (void)dealloc {
  if (_isEnforcingNeverMadeVisible) {
    [self removeObserver:self forKeyPath:@"visible"];
  }
  _willUpdateRestorableState = YES;
  [NSObject cancelPreviousPerformRequestsWithTarget:self];
}

- (void)addChildWindow:(NSWindow*)childWin ordered:(NSWindowOrderingMode)place {
  // Attaching a window to be a child window resets the window level, so
  // restore the window level afterwards.
  NSInteger level = childWin.level;
  [super addChildWindow:childWin ordered:place];
  childWin.level = level;
  if (self.childWindowAddedHandler) {
    self.childWindowAddedHandler(childWin);
  }
}

// Overridden to ensure that removing a child window does not trigger a Space
// change, and to perform post-removal operations.
- (void)removeChildWindow:(NSWindow*)childWindow {
  if (self != childWindow.parentWindow) {
    return;
  }

  // For any non-Chrome windows (i.e. those created by the frameworks),
  // remove as usual. Also continue as usual if we're on the active space,
  // or we happen to be a child of another window.
  if (![childWindow isKindOfClass:[NativeWidgetMacNSWindow class]] ||
      [self isOnActiveSpace] || [self parentWindow] != nil) {
    [super removeChildWindow:childWindow];
  } else {
    // Defer removal to avoid triggering a space change.
    [self removeChildWindowOnActivation:childWindow];
  }

  // If there's a windowRemoved handler, we'll call it even if we've deferred
  // the actual NSWindow removal via -removeChildWindowOnActivation:. As far as
  // Chrome is concerned, the child window no longer exists (for example, it's
  // no longer in self.ordered_children). The removeChildWindow: that finally
  // removes the child will happen at some future date.
  if (self.childWindowRemovedHandler) {
    self.childWindowRemovedHandler(childWindow);
  }
}

- (void)enforceNeverMadeVisible {
  if (_isEnforcingNeverMadeVisible)
    return;
  _isEnforcingNeverMadeVisible = YES;
  [self addObserver:self
         forKeyPath:@"visible"
            options:NSKeyValueObservingOptionNew
            context:nil];
}

- (void)observeValueForKeyPath:(NSString*)keyPath
                      ofObject:(id)object
                        change:(NSDictionary*)change
                       context:(void*)context {
  if ([keyPath isEqual:@"visible"]) {
    DCHECK(_isEnforcingNeverMadeVisible);
    DCHECK_EQ(object, self);
    DCHECK_EQ(context, nil);
    if ([change[NSKeyValueChangeNewKey] boolValue])
      base::debug::DumpWithoutCrashing();
  }
  [super observeValueForKeyPath:keyPath
                       ofObject:object
                         change:change
                        context:context];
}

// Public methods.

- (void)setHasShadow:(BOOL)flag {
  [super setHasShadow:flag && !AreWindowShadowsDisabled()];
}

- (void)setCommandDispatcherDelegate:(id<CommandDispatcherDelegate>)delegate {
  [_commandDispatcher setDelegate:delegate];
}

- (void)setWindowTouchBarDelegate:(id<WindowTouchBarDelegate>)delegate {
  _touchBarDelegate = delegate;
}

- (void)orderFrontKeepWindowKeyState {
  _miniaturizationInProgress = NO;

  if ([self isOnActiveSpace]) {
    [self orderWindow:NSWindowAbove relativeTo:0];
    return;
  }
  // The OS will activate the window if it causes a space switch.
  // Temporarily prevent the window from becoming the key window until after
  // the space change completes.
  _preventKeyWindow = ![self isKeyWindow];
  __block id observer = [NSWorkspace.sharedWorkspace.notificationCenter
      addObserverForName:NSWorkspaceActiveSpaceDidChangeNotification
                  object:[NSWorkspace sharedWorkspace]
                   queue:[NSOperationQueue mainQueue]
              usingBlock:^(NSNotification* notification) {
                self->_preventKeyWindow = NO;
                [NSWorkspace.sharedWorkspace.notificationCenter
                    removeObserver:observer];
              }];
  [self orderWindow:NSWindowAbove relativeTo:0];
}

- (NSRect)constrainFrameRect:(NSRect)frameRect toScreen:(NSScreen*)screen {
  // Headless windows should not be constrained within the physical screen.
  if (_isHeadless) {
    return frameRect;
  }

  return [super constrainFrameRect:frameRect toScreen:screen];
}

// Private methods.

- (ViewsNSWindowDelegate*)viewsNSWindowDelegate {
  return base::apple::ObjCCastStrict<ViewsNSWindowDelegate>([self delegate]);
}

- (BOOL)hasViewsMenuActive {
  bool hasMenuController = false;
  if (_bridge)
    _bridge->host()->GetHasMenuController(&hasMenuController);
  return hasMenuController;
}

- (id<NSAccessibility>)rootAccessibilityObject {
  id<NSAccessibility> obj =
      _bridge ? _bridge->host_helper()->GetNativeViewAccessible() : nil;
  // We should like to DCHECK that the object returned implements the
  // NSAccessibility protocol, but the NSAccessibilityRemoteUIElement interface
  // does not conform.
  // TODO(https://crbug.com/944698): Create a sub-class that does.
  return obj;
}

- (NSAccessibilityRole)accessibilityRole {
  return _isTooltip ? NSAccessibilityHelpTagRole : [super accessibilityRole];
}

// NSWindow overrides.

+ (Class)frameViewClassForStyleMask:(NSWindowStyleMask)windowStyle {
  if (windowStyle & NSWindowStyleMaskTitled) {
    if (Class customFrame = [NativeWidgetMacNSWindowTitledFrame class])
      return customFrame;
  } else if (Class customFrame =
                 [NativeWidgetMacNSWindowBorderlessFrame class]) {
    return customFrame;
  }
  return [super frameViewClassForStyleMask:windowStyle];
}

- (BOOL)_isTitleHidden {
  bool shouldShowWindowTitle = YES;
  if (_bridge)
    _bridge->host()->GetShouldShowWindowTitle(&shouldShowWindowTitle);
  return !shouldShowWindowTitle;
}

// The base implementation returns YES if the window's frame view is a custom
// class, which causes undesirable changes in behavior. AppKit NSWindow
// subclasses are known to override it and return NO.
- (BOOL)_usesCustomDrawing {
  return NO;
}

// Ignore [super canBecome{Key,Main}Window]. The default is NO for windows with
// NSWindowStyleMaskBorderless, which is not the desired behavior.
// Note these can be called via -[NSWindow close] while the widget is being torn
// down, so check for a delegate.
- (BOOL)canBecomeKeyWindow {
  if (_preventKeyWindow)
    return NO;
  bool canBecomeKey = NO;
  if (_bridge)
    _bridge->host()->GetCanWindowBecomeKey(&canBecomeKey);
  return canBecomeKey;
}

- (BOOL)canBecomeMainWindow {
  if (!_bridge)
    return NO;

  // Dialogs and bubbles shouldn't take large shadows away from their parent.
  if (_bridge->parent())
    return NO;

  bool canBecomeKey = NO;
  if (_bridge)
    _bridge->host()->GetCanWindowBecomeKey(&canBecomeKey);
  return canBecomeKey;
}

// Lets the traffic light buttons on the parent window keep their active state.
- (BOOL)hasKeyAppearance {
  // Note that this function is called off of the main thread. In such cases,
  // it is not safe to access the mojo interface or the ui::Widget, as they are
  // not reentrant.
  // https://crbug.com/941506.
  if (![NSThread isMainThread])
    return [super hasKeyAppearance];
  if (_bridge) {
    bool isAlwaysRenderWindowAsKey = NO;
    _bridge->host()->GetAlwaysRenderWindowAsKey(&isAlwaysRenderWindowAsKey);
    if (isAlwaysRenderWindowAsKey)
      return YES;
  }
  return [super hasKeyAppearance];
}

// Override sendEvent to intercept window drag events and allow key events to be
// forwarded to a toolkit-views menu while it is active, and while still
// allowing any native subview to retain firstResponder status.
- (void)sendEvent:(NSEvent*)event {
  // TODO(bokan): Tracing added temporarily to diagnose crbug.com/1039833.
  TRACE_EVENT1("browser", "NSWindow::sendEvent", "WindowNum",
               [self windowNumber]);

  // Let CommandDispatcher check if this is a redispatched event.
  if ([_commandDispatcher preSendEvent:event]) {
    TRACE_EVENT_INSTANT0("browser", "StopSendEvent", TRACE_EVENT_SCOPE_THREAD);
    return;
  }

  NSEventType type = [event type];

  // Draggable regions only respond to left-click dragging, but the system will
  // still suppress right-clicks in a draggable region. Forwarding right-clicks
  // and ctrl+left-clicks allows the underlying views to respond to right-click
  // to potentially bring up a frame context menu.
  if (type == NSEventTypeRightMouseDown ||
      (type == NSEventTypeLeftMouseDown &&
       ([event modifierFlags] & NSEventModifierFlagControl))) {
    if ([[self contentView] hitTest:event.locationInWindow] == nil) {
      [[self contentView] rightMouseDown:event];
      return;
    }
  } else if (type == NSEventTypeRightMouseUp) {
    if ([[self contentView] hitTest:event.locationInWindow] == nil) {
      [[self contentView] rightMouseUp:event];
      return;
    }
  } else if ([self hasViewsMenuActive]) {
    // Send to the menu, after converting the event into an action message using
    // the content view.
    if (type == NSEventTypeKeyDown) {
      [[self contentView] keyDown:event];
      return;
    } else if (type == NSEventTypeKeyUp) {
      [[self contentView] keyUp:event];
      return;
    }
  }

  [super sendEvent:event];
}

- (void)orderWindowByShuffling:(NSWindowOrderingMode)orderingMode
                    relativeTo:(NSInteger)otherWindowNumber {
  NativeWidgetMacNSWindow* parent =
      static_cast<NativeWidgetMacNSWindow*>([self parentWindow]);

  // This is not a child window. No need to patch.
  if (!parent) {
    [self orderWindow:orderingMode relativeTo:otherWindowNumber];
    return;
  }

  // Calling OrderChildWindow() when we're not on the active Space will
  // will trigger a Space switch. Instead, save the window ordering command
  // until we're on the active space.
  if (![self isOnActiveSpace]) {
    ChildWindowOrderingCommand newCommand = {orderingMode, otherWindowNumber};

    // Add the command, but ignore any repeats of the last command in the list.
    if (_windowOrderingCommands.empty() ||
        _windowOrderingCommands.back() != newCommand) {
      _windowOrderingCommands.push_back(newCommand);
    }

    return;
  }

  base::AutoReset<BOOL> shuffling(&_isShufflingForOrdering, YES);

  // `otherWindow` is nil if `otherWindowNumber` is 0. In this case, place
  // `self` at the top / bottom, depending on `orderingMode`.
  NSWindow* otherWindow = [NSApp windowWithWindowNumber:otherWindowNumber];
  if (otherWindow == nullptr || parent == [otherWindow parentWindow] ||
      parent == otherWindow) {
    OrderChildWindow(self, otherWindow, orderingMode);
  }

  [[self viewsNSWindowDelegate] onWindowOrderChanged:nil];
}

// Override window order functions to intercept other visibility changes. This
// is needed in addition to the -[NSWindow display] override because Cocoa
// hardly ever calls display, and reports -[NSWindow isVisible] incorrectly
// when ordering in a window for the first time.
// Note that this methods has no effect for children windows. Use
// -orderWindowByShuffling:relativeTo: instead.
- (void)orderWindow:(NSWindowOrderingMode)orderingMode
         relativeTo:(NSInteger)otherWindowNumber {
  [super orderWindow:orderingMode relativeTo:otherWindowNumber];
  [[self viewsNSWindowDelegate] onWindowOrderChanged:nil];
}

- (void)miniaturize:(id)sender {
  static const BOOL isMacOS13OrHigher = base::mac::MacOSMajorVersion() >= 13;
  // On macOS 13, the miniaturize operation appears to no longer be "atomic"
  // because of non-blocking roundtrip IPC with the Dock. We want to note here
  // that miniaturization is in progress. The process completes when we
  // reach -_regularMinimizeToDock:.
  _miniaturizationInProgress = isMacOS13OrHigher;

  [super miniaturize:sender];
}

- (void)_regularMinimizeToDock {
  // On macOS 13, a call to -miniaturize: kicks of an async round-trip IPC with
  // the Dock that ends up in this method. Unfortunately, it appears that if we
  // immediately follow a call to -miniaturize: with -makeKeyAndOrderFront:,
  // the AppKit doesn't cancel the in-flight round-trip IPC. As a result,
  // _regularMinimizeToDock gets called sometime after -makeKeyAndOrderFront:
  // and miniaturizes the window anyway. This is  a potential problem in
  // session restore where we might restart with a single browser window
  // sitting Dock. In that case, Session Restore creates the window,
  // miniaturizes to the dock, and then brings it back out. With this new macOS
  // 13 behavior (which seems like a bug), the browser window may not be
  // restored from the Dock.
  //
  // To get around this problem, if we arrive here and
  // _miniaturizationInProgress is NO, the miniaturization process was
  // cancelled by a call to -makeKeyAndOrderFront:. In that case, we don't want
  // to proceed with miniaturization.
  static const BOOL isMacOS13OrHigher = base::mac::MacOSMajorVersion() >= 13;
  if (isMacOS13OrHigher && !_miniaturizationInProgress) {
    return;
  }

  _miniaturizationInProgress = NO;
  [super _regularMinimizeToDock];
}

- (void)makeKeyAndOrderFront:(id)sender {
  _miniaturizationInProgress = NO;
  [super makeKeyAndOrderFront:sender];
}

- (void)orderOut:(id)sender {
  _miniaturizationInProgress = NO;
  _isOrderingOut = YES;

  // If we're a child window and our parent is not on the active space,
  // arrange for our removal after our parent becomes the active window
  // to avoid triggering a Space switch.
  NativeWidgetMacNSWindow* parentWindow =
      base::apple::ObjCCast<NativeWidgetMacNSWindow>([self parentWindow]);
  if (parentWindow != nil && ![parentWindow isOnActiveSpace]) {
    [parentWindow removeChildWindowOnActivation:self];
  } else {
    [self processDeferredChildWindowOperations];

    // Throw away our own ordering commands (if we have any).
    _windowOrderingCommands.clear();

    [super orderOut:sender];
  }

  _isOrderingOut = NO;
}

// NSResponder implementation.

- (BOOL)performKeyEquivalent:(NSEvent*)event {
  // TODO(bokan): Tracing added temporarily to diagnose crbug.com/1039833.
  TRACE_EVENT1("browser", "NSWindow::performKeyEquivalent", "WindowNum",
               [self windowNumber]);
  return [_commandDispatcher performKeyEquivalent:event];
}

- (void)cursorUpdate:(NSEvent*)theEvent {
  // The cursor provided by the delegate should only be applied within the
  // content area. This is because we rely on the contentView to track the
  // mouse cursor and forward cursorUpdate: messages up the responder chain.
  // The cursorUpdate: isn't handled in BridgedContentView because views-style
  // SetCapture() conflicts with the way tracking events are processed for
  // the view during a drag. Since the NSWindow is still in the responder chain
  // overriding cursorUpdate: here handles both cases.
  if (!NSPointInRect([theEvent locationInWindow], [[self contentView] frame])) {
    [super cursorUpdate:theEvent];
    return;
  }

  NSCursor* cursor = [[self viewsNSWindowDelegate] cursor];
  if (cursor)
    [cursor set];
  else
    [super cursorUpdate:theEvent];
}

- (NSTouchBar*)makeTouchBar {
  return _touchBarDelegate ? [_touchBarDelegate makeTouchBar] : nil;
}

// Called when the window is the delegate of the archiver passed to
// |-encodeRestorableStateWithCoder:|, below. It prevents the archiver from
// trying to encode the window or an NSView, say, to represent the first
// responder. When AppKit calls |-encodeRestorableStateWithCoder:|, it
// accomplishes the same thing by passing a custom coder.
- (id)archiver:(NSKeyedArchiver*)archiver willEncodeObject:(id)object {
  if (object == self)
    return nil;
  if ([object isKindOfClass:[NSView class]])
    return nil;
  return object;
}

- (void)saveRestorableState {
  if (!_bridge)
    return;
  if (![self _isConsideredOpenForPersistentState])
    return;

  // On macOS 12+, create restorable state archives with secure encoding. See
  // the article at
  // https://sector7.computest.nl/post/2022-08-process-injection-breaking-all-macos-security-layers-with-a-single-vulnerability/
  // for more details.
  NSKeyedArchiver* encoder = [[NSKeyedArchiver alloc]
      initRequiringSecureCoding:base::mac::MacOSMajorVersion() >= 12];
  encoder.delegate = self;
  [self encodeRestorableStateWithCoder:encoder];
  [encoder finishEncoding];
  NSData* restorableStateData = encoder.encodedData;

  auto* bytes = static_cast<uint8_t const*>(restorableStateData.bytes);
  _bridge->host()->OnWindowStateRestorationDataChanged(
      std::vector<uint8_t>(bytes, bytes + restorableStateData.length));
  _willUpdateRestorableState = NO;
}

// AppKit calls -invalidateRestorableState when a property of the window which
// affects its restorable state changes.
- (void)invalidateRestorableState {
  [super invalidateRestorableState];
  if ([self _isConsideredOpenForPersistentState]) {
    if (_willUpdateRestorableState)
      return;
    _willUpdateRestorableState = YES;
    [self performSelectorOnMainThread:@selector(saveRestorableState)
                           withObject:nil
                        waitUntilDone:NO
                                modes:@[ NSDefaultRunLoopMode ]];
  } else if (_willUpdateRestorableState) {
    _willUpdateRestorableState = NO;
    [NSObject cancelPreviousPerformRequestsWithTarget:self];
  }
}

// On newer SDKs, _canMiniaturize respects NSWindowStyleMaskMiniaturizable in
// the window's styleMask. Views assumes that Widgets can always be minimized,
// regardless of their window style, so override that behavior here.
- (BOOL)_canMiniaturize {
  return YES;
}

- (BOOL)respondsToSelector:(SEL)aSelector {
  // If this window or its parent does not handle commands, remove it from the
  // chain.
  bool isCommandDispatch =
      aSelector == @selector(commandDispatch:) ||
      aSelector == @selector(commandDispatchUsingKeyModifiers:);
  if (isCommandDispatch && _commandHandler == nil &&
      self.commandDispatchParent == nil) {
    return NO;
  }

  return [super respondsToSelector:aSelector];
}

// CommandDispatchingWindow implementation.

- (void)setCommandHandler:(id<UserInterfaceItemCommandHandler>)commandHandler {
  _commandHandler = commandHandler;
}

- (CommandDispatcher*)commandDispatcher {
  return _commandDispatcher;
}

- (BOOL)defaultPerformKeyEquivalent:(NSEvent*)event {
  // TODO(bokan): Tracing added temporarily to diagnose crbug.com/1039833.
  TRACE_EVENT1("browser", "NSWindow::defaultPerformKeyEquivalent", "WindowNum",
               [self windowNumber]);
  return [super performKeyEquivalent:event];
}

- (BOOL)defaultValidateUserInterfaceItem:
    (id<NSValidatedUserInterfaceItem>)item {
  return [super validateUserInterfaceItem:item];
}

- (void)commandDispatch:(id)sender {
  [_commandDispatcher dispatch:sender forHandler:_commandHandler];
}

- (void)commandDispatchUsingKeyModifiers:(id)sender {
  [_commandDispatcher dispatchUsingKeyModifiers:sender
                                     forHandler:_commandHandler];
}

// NSWindow overrides (NSUserInterfaceItemValidations implementation)

- (BOOL)validateUserInterfaceItem:(id<NSValidatedUserInterfaceItem>)item {
  return [_commandDispatcher validateUserInterfaceItem:item
                                            forHandler:_commandHandler];
}

// NSWindow overrides (NSAccessibility informal protocol implementation).

- (id)accessibilityFocusedUIElement {
  if (![self delegate])
    return [super accessibilityFocusedUIElement];

  // The SDK documents this as "The deepest descendant of the accessibility
  // hierarchy that has the focus" and says "if a child element does not have
  // the focus, either return self or, if available, invoke the superclass's
  // implementation."
  // The behavior of NSWindow is usually to return null, except when the window
  // is first shown, when it returns self. But in the second case, we can
  // provide richer a11y information by reporting the views::RootView instead.
  // Additionally, if we don't do this, VoiceOver reads out the partial a11y
  // properties on the NSWindow and repeats them when focusing an item in the
  // RootView's a11y group. See http://crbug.com/748221.
  id superFocus = [super accessibilityFocusedUIElement];
  if (!_bridge || superFocus != self)
    return superFocus;

  return _bridge->host_helper()->GetNativeViewAccessible();
}

- (NSString*)accessibilityTitle {
  // Check when NSWindow is asked for its title to provide the title given by
  // the views::RootView (and WidgetDelegate::GetAccessibleWindowTitle()). For
  // all other attributes, use what NSWindow provides by default since diverging
  // from NSWindow's behavior can easily break VoiceOver integration.
  NSString* viewsValue = self.rootAccessibilityObject.accessibilityTitle;
  return viewsValue ? viewsValue : [super accessibilityTitle];
}

- (NSWindow<CommandDispatchingWindow>*)commandDispatchParent {
  if (_commandDispatchParentOverride) {
    return _commandDispatchParentOverride;
  }
  NSWindow* parent = self.parentWindow;
  if (parent && [parent hasKeyAppearance] &&
      [parent conformsToProtocol:@protocol(CommandDispatchingWindow)]) {
    return static_cast<NSWindow<CommandDispatchingWindow>*>(parent);
  }
  return nil;
}

- (BOOL)isFullScreen {
  return (self.styleMask & NSWindowStyleMaskFullScreen) ==
         NSWindowStyleMaskFullScreen;
}

- (void)removeChildWindowOnActivation:(NSWindow*)childWindow {
  if (_childWindowsToRemove == nil) {
    _childWindowsToRemove = [[NSMutableArray alloc] init];
  }

  // Ignore if a duplicate request.
  if ([_childWindowsToRemove containsObject:childWindow]) {
    return;
  }

  // Hide `childWindow` by making it transparent and schedule it for deferred
  // removal.
  childWindow.alphaValue = 0.0;
  [_childWindowsToRemove addObject:childWindow];
}

- (BOOL)willRemoveChildWindowOnActivation:(NSWindow*)aWindow {
  return [_childWindowsToRemove containsObject:aWindow];
}

- (BOOL)hasDeferredChildWindowRemovalsForTesting {
  return _childWindowsToRemove.count > 0;
}

- (BOOL)hasDeferredChildWindowOrderingCommandsForTesting {
  return !_windowOrderingCommands.empty();
}

- (void)processDeferredChildWindowOperations {
  // Remove any child windows where removal was pending.
  for (NSWindow* childWindow in _childWindowsToRemove) {
    [super removeChildWindow:childWindow];
  }
  [_childWindowsToRemove removeAllObjects];

  // Process any child window ordering commands, unless we're ordering out.
  if (_isOrderingOut) {
    return;
  }

  for (NSWindow* childWindow in self.childWindows) {
    NativeWidgetMacNSWindow* nativeWidgetMacNSWindow =
        base::apple::ObjCCast<NativeWidgetMacNSWindow>(childWindow);

    [nativeWidgetMacNSWindow
        performSelector:@selector(processChildWindowOrderingCommands)
             withObject:nil];
  }
}

- (void)processChildWindowOrderingCommands {
  for (const auto& command : _windowOrderingCommands) {
    [self orderWindowByShuffling:command.windowOrderingMode
                      relativeTo:command.otherWindowNumber];
  }
  _windowOrderingCommands.clear();
}

- (void)becomeMainWindow {
  [super becomeMainWindow];

  [self processDeferredChildWindowOperations];
}

- (void)toggleFullScreen:(id)sender {
  [super toggleFullScreen:sender];

  // We're either entering fullscreen or exiting - either way, process the
  // deferred child window operations.
  [self processDeferredChildWindowOperations];
}

@end