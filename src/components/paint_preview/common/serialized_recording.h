// Copyright 2020 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_PAINT_PREVIEW_COMMON_SERIALIZED_RECORDING_H_
#define COMPONENTS_PAINT_PREVIEW_COMMON_SERIALIZED_RECORDING_H_

#include "base/files/file.h"
#include "base/gtest_prod_util.h"
#include "base/unguessable_token.h"
#include "components/paint_preview/common/mojom/paint_preview_types.mojom-shared.h"
#include "components/paint_preview/common/serial_utils.h"
#include "mojo/public/cpp/base/big_buffer.h"
#include "mojo/public/cpp/bindings/union_traits.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "third_party/skia/include/core/SkRefCnt.h"

class SkPicture;

namespace paint_preview {

class PaintPreviewTracker;

// Enumeration of strategies to store artifacts (i.e. frame recordings) of a
// paint preview capture.
enum class RecordingPersistence {
  // Store artifacts in the file system. This strategy can be robust to browser
  // restarts, since the lifetime of the files can outlive the browser process.
  kFileSystem,

  // Store artifacts in memory buffers returned from or passed to capture or
  // compositing methods. On low-memory devices, there is a higher risk of
  // out-of-memory issues.
  kMemoryBuffer,
};

// Struct for containing an SkPicture and its deserialization context.
struct SkpResult {
  SkpResult();
  ~SkpResult();

  SkpResult(const SkpResult& other) = delete;
  SkpResult& operator=(const SkpResult& rhs) = delete;

  SkpResult(SkpResult&& other);
  SkpResult& operator=(SkpResult&& rhs);

  sk_sp<SkPicture> skp;
  DeserializationContext ctx;
};

// This is an object that contains the serialized |SkPicture| that results from
// a rendering frame recording. It will be backed by a value corresponding to
// its |persistence_| value.
//
// The contents of a |SerializedRecording| must be generated by |RecordToFile|
// or |RecordToBuffer| to be safely deserialized.
class SerializedRecording {
 public:
  // Create an invalid variant, which doesn't have a valid file or buffer.
  SerializedRecording();

  // Create a file variant. Will open a file at the given path for reading.
  explicit SerializedRecording(base::FilePath);

  // Create a memory buffer variant.
  explicit SerializedRecording(mojo_base::BigBuffer);

  SerializedRecording(SerializedRecording&&);
  SerializedRecording& operator=(SerializedRecording&&);
  ~SerializedRecording();

  // Returns true if this contains a valid value for its variant.
  bool IsValid() const;

 private:
  friend struct mojo::UnionTraits<
      paint_preview::mojom::SerializedRecordingDataView,
      paint_preview::SerializedRecording>;

  // Expose |Deserialize| only to places that will use it safely (i.e. not on
  // the browser process).
  friend class PaintPreviewCompositorImpl;
  friend class PaintPreviewBrowserTest;
  FRIEND_TEST_ALL_PREFIXES(PaintPreviewSerializedRecordingTest,
                           RoundtripWithFileBacking);
  FRIEND_TEST_ALL_PREFIXES(PaintPreviewSerializedRecordingTest,
                           RoundtripWithMemoryBufferBacking);
  FRIEND_TEST_ALL_PREFIXES(PaintPreviewSerializedRecordingTest,
                           RoundtripHasEmbeddedContent);
  FRIEND_TEST_ALL_PREFIXES(PaintPreviewSerializedRecordingTest,
                           RecordingMapFromCaptureResultSingleFrame);
  FRIEND_TEST_ALL_PREFIXES(PaintPreviewSerializedRecordingTest,
                           RecordingMapFromPaintPreviewProtoSingleFrame);
  FRIEND_TEST_ALL_PREFIXES(PaintPreviewRecorderUtilsSerializeAsSkPictureTest,
                           Roundtrip);
  FRIEND_TEST_ALL_PREFIXES(PaintPreviewRecorderUtilsSerializeAsSkPictureTest,
                           RoundtripWithImage);
  FRIEND_TEST_ALL_PREFIXES(PaintPreviewRecorderUtilsSerializeAsSkPictureTest,
                           RoundtripWithLazyImage);
  FRIEND_TEST_ALL_PREFIXES(PaintPreviewRecorderUtilsSerializeAsSkPictureTest,
                           RoundtripWithPaintWorklet);
  FRIEND_TEST_ALL_PREFIXES(PaintPreviewRecorderUtilsSerializeAsSkPictureTest,
                           RoundtripWithTexture);
  FRIEND_TEST_ALL_PREFIXES(PaintPreviewRecorderUtilsSerializeAsSkPictureTest,
                           RoundtripWithLazyTexture);

  // Deserialize into an |SkPicture|. The result will not include any embedded
  // subframes.
  //
  // This is not safe to call in the browser process.
  absl::optional<SkpResult> Deserialize() &&;

  // Deserialize into an |SkPicture|. |ctx| should contain entries for any
  // subframes that should be included in the output.
  //
  // This is not safe to call in the browser process.
  sk_sp<SkPicture> DeserializeWithContext(LoadedFramesDeserialContext* ctx) &&;

  // Create a file variant from an existing file. File must have been created
  // with the flags: base::File::FLAG_OPEN | base::File::FLAG_READ.
  explicit SerializedRecording(base::File);

  bool is_file() const {
    return persistence_ == RecordingPersistence::kFileSystem;
  }

  bool is_buffer() const {
    return persistence_ == RecordingPersistence::kMemoryBuffer;
  }

  RecordingPersistence persistence_;

  base::File file_;
  absl::optional<mojo_base::BigBuffer> buffer_;
};

// Serialize and write |skp| to |file|.
//
// If |max_size| is set, it is a limit on the total serialized size, else the
// capture will be unrestricted in size. If |max_size| is exceeded the
// serialization will fail. |serialized_size| will contain the size of the
// serialized output.
//
// Returns |true| on success.
bool RecordToFile(base::File file,
                  sk_sp<const SkPicture> skp,
                  PaintPreviewTracker* tracker,
                  absl::optional<size_t> max_capture_size,
                  size_t* serialized_size);

// Serialize and write |recording| to a memory buffer.
//
// If |max_size| is set, it is a limit on the total serialized size, else the
// capture will be unrestricted in size. If |max_size| is exceeded the
// serialization will fail. |serialized_size| will contain the size of the
// serialized output.
//
// Returns the memory buffer on success.
absl::optional<mojo_base::BigBuffer> RecordToBuffer(
    sk_sp<const SkPicture> skp,
    PaintPreviewTracker* tracker,
    absl::optional<size_t> max_capture_size,
    size_t* serialized_size);

}  // namespace paint_preview

#endif  // COMPONENTS_PAINT_PREVIEW_COMMON_SERIALIZED_RECORDING_H_