/*
 * Copyright (c) 2024, Saksham Goyal <sakgoy2001@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWeb/Bindings/CredentialPrototype.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/CredentialManagement/Credential.h>
#include <LibWeb/WebIDL/ExceptionOr.h>

namespace Web::CredentialManagement {
GC_DEFINE_ALLOCATOR(Credential);

WebIDL::ExceptionOr<GC::Ref<Credential>> Credential::construct_impl(JS::Realm& realm)
{
    return realm.create<Credential>(realm);
}

// https://www.w3.org/TR/credential-management-1/#dom-credential-isconditionalmediationavailable
JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> Credential::is_conditional_mediation_available(JS::VM& vm)
{
    auto* realm = vm.current_realm();
    return WebIDL::create_rejected_promise(*realm, JS::PrimitiveString::create(vm, "Not implemented"sv));
}

// https://www.w3.org/TR/credential-management-1/#dom-credential-willrequestconditionalcreation
JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> Credential::will_request_conditional_creation(JS::VM& vm)
{
    auto* realm = vm.current_realm();
    return WebIDL::create_rejected_promise(*realm, JS::PrimitiveString::create(vm, "Not implemented"sv));
}

void Credential::initialize(JS::Realm& realm)
{
    Base::initialize(realm);
    WEB_SET_PROTOTYPE_FOR_INTERFACE(Credential);
}

}
