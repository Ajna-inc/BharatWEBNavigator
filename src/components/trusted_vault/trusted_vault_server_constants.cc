// Copyright 2021 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/trusted_vault/trusted_vault_server_constants.h"

#include "base/base64url.h"
#include "base/containers/contains.h"
#include "base/containers/fixed_flat_map.h"
#include "base/strings/string_piece.h"
#include "net/base/url_util.h"

namespace trusted_vault {

std::vector<uint8_t> GetConstantTrustedVaultKey() {
  return std::vector<uint8_t>(16, 0);
}

std::string GetGetSecurityDomainMemberURLPathAndQuery(
    base::span<const uint8_t> public_key) {
  std::string encoded_public_key;
  base::Base64UrlEncode(std::string(public_key.begin(), public_key.end()),
                        base::Base64UrlEncodePolicy::OMIT_PADDING,
                        &encoded_public_key);
  return kSecurityDomainMemberNamePrefix + encoded_public_key + "?view=2" +
         "&request_header.force_master_read=true";
}

GURL GetFullJoinSecurityDomainsURLForTesting(const GURL& server_url) {
  return net::AppendQueryParameter(
      /*url=*/GURL(server_url.spec() + kJoinSecurityDomainsURLPath),
      kQueryParameterAlternateOutputKey, kQueryParameterAlternateOutputProto);
}

GURL GetFullGetSecurityDomainMemberURLForTesting(
    const GURL& server_url,
    base::span<const uint8_t> public_key) {
  return net::AppendQueryParameter(
      /*url=*/GURL(server_url.spec() +
                   GetGetSecurityDomainMemberURLPathAndQuery(public_key)),
      kQueryParameterAlternateOutputKey, kQueryParameterAlternateOutputProto);
}

GURL GetFullGetSecurityDomainURLForTesting(const GURL& server_url) {
  return net::AppendQueryParameter(
      /*url=*/GURL(server_url.spec() + kGetSecurityDomainURLPathAndQuery),
      kQueryParameterAlternateOutputKey, kQueryParameterAlternateOutputProto);
}

std::string GetSecurityDomainName(SecurityDomainId domain) {
  switch (domain) {
    case SecurityDomainId::kChromeSync:
      return kSyncSecurityDomainName;
  }
}

absl::optional<SecurityDomainId> GetSecurityDomainByName(
    base::StringPiece name) {
  static_assert(static_cast<int>(SecurityDomainId::kMaxValue) == 0,
                "Update GetSecurityDomainByName when adding SecurityDomainId "
                "enum values");
  static constexpr auto kSecurityDomainNames =
      base::MakeFixedFlatMap<base::StringPiece, SecurityDomainId>({
          {kSyncSecurityDomainName, SecurityDomainId::kChromeSync},
      });
  return base::Contains(kSecurityDomainNames, name)
             ? absl::make_optional(kSecurityDomainNames.at(name))
             : absl::nullopt;
}

}  // namespace trusted_vault