// Copyright 2021 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_USER_EDUCATION_USER_EDUCATION_SERVICE_FACTORY_H_
#define CHROME_BROWSER_USER_EDUCATION_USER_EDUCATION_SERVICE_FACTORY_H_

#include "base/no_destructor.h"
#include "chrome/browser/profiles/profile_keyed_service_factory.h"
#include "content/public/browser/browser_context.h"

class UserEducationService;

class UserEducationServiceFactory : public ProfileKeyedServiceFactory {
 public:
  // Disallow copy/assign.
  UserEducationServiceFactory(const UserEducationServiceFactory&) = delete;
  void operator=(const UserEducationServiceFactory&) = delete;

  static UserEducationServiceFactory* GetInstance();
  static UserEducationService* GetForBrowserContext(
      content::BrowserContext* context);

 private:
  friend base::NoDestructor<UserEducationServiceFactory>;

  // BrowserContextKeyedServiceFactory overrides.
  std::unique_ptr<KeyedService> BuildServiceInstanceForBrowserContext(
      content::BrowserContext* context) const override;

  UserEducationServiceFactory();
  ~UserEducationServiceFactory() override;
};

#endif  // CHROME_BROWSER_UI_USER_EDUCATION_USER_EDUCATION_SERVICE_FACTORY_H_
