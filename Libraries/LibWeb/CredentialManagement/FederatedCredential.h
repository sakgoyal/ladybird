/*
 * Copyright (c) 2024, Saksham Goyal <sakgoy2001@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/HashTable.h>
#include <AK/Vector.h>
#include <LibWeb/Bindings/FederatedCredentialPrototype.h>
#include <LibWeb/Bindings/PlatformObject.h>
#include <LibWeb/CredentialManagement/Credential.h>
#include <LibWeb/DOM/AbortSignal.h>
#include <LibWeb/HTML/Window.h>
#include <LibWeb/Platform/EventLoopPlugin.h>
#include <LibWeb/WebIDL/Promise.h>

namespace Web::CredentialManagement {

struct FederatedCredentialInit : CredentialData {
    Optional<String> name;
    Optional<String> icon_url;
    String origin;
    String provider;
    Optional<String> protocol;
};

struct FederatedCredentialRequestOptions {
    Vector<String> providers;
    Vector<String> protocols;
};

class FederatedCredential final : public Bindings::PlatformObject {
    WEB_PLATFORM_OBJECT(FederatedCredential, Bindings::PlatformObject);
    GC_DECLARE_ALLOCATOR(FederatedCredential);

public:
    static WebIDL::ExceptionOr<GC::Ref<FederatedCredential>> construct_impl(JS::Realm&, FederatedCredentialInit const&);

    auto provider() { return m_provider; }
    auto protocol() { return m_protocol; }
    auto name() { return m_name; }
    auto icon_url() { return m_icon_url; }

private:
    FederatedCredential(JS::Realm&, FederatedCredentialInit const&);

    virtual void initialize(JS::Realm& realm) override;

    String m_provider;
    String m_name;
    String m_icon_url;
    Optional<String> m_protocol;
};

}
