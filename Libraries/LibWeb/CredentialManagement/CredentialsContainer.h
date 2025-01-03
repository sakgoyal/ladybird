/*
 * Copyright (c) 2024, Saksham Goyal <sakgoy2001@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/HashTable.h>
#include <AK/Vector.h>
#include <LibWeb/Bindings/CredentialsContainerPrototype.h>
#include <LibWeb/Bindings/PlatformObject.h>
#include <LibWeb/CredentialManagement/Credential.h>
#include <LibWeb/DOM/AbortSignal.h>
#include <LibWeb/HTML/Window.h>
#include <LibWeb/Platform/EventLoopPlugin.h>
#include <LibWeb/WebIDL/Promise.h>

namespace Web::CredentialManagement {
struct CredentialRequestOptions {
    Bindings::CredentialMediationRequirement mediation = Bindings::CredentialMediationRequirement::Optional;
    GC::Ptr<DOM::AbortSignal> signal;
    bool password = false;
};

struct CredentialCreationOptions {
    Bindings::CredentialMediationRequirement mediation = Bindings::CredentialMediationRequirement::Optional;
    GC::Ptr<DOM::AbortSignal> signal;
};
class CredentialsContainer final : public Bindings::PlatformObject {
    WEB_PLATFORM_OBJECT(CredentialsContainer, Bindings::PlatformObject);
    GC_DECLARE_ALLOCATOR(CredentialsContainer);

public:
    static WebIDL::ExceptionOr<GC::Ref<CredentialsContainer>> construct_impl(JS::Realm&);

    [[nodiscard]] JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> get(CredentialRequestOptions const& options);
    [[nodiscard]] JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> store(Credential const& credential);
    [[nodiscard]] JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> create(CredentialCreationOptions const& options);
    [[nodiscard]] JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> prevent_silent_access();

private:
    explicit CredentialsContainer(JS::Realm& realm);

    virtual void initialize(JS::Realm& realm) override;
};

// https://www.w3.org/TR/credential-management-1/#credentialrequestoptions-dictionary
HashTable<Credential> relevant_credential_interface_objects(Variant<CredentialCreationOptions, CredentialRequestOptions> options);

}
