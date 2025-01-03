/*
 * Copyright (c) 2024, Saksham Goyal <sakgoy2001@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Vector.h>
#include <LibWeb/Bindings/FederatedCredentialPrototype.h>
#include <LibWeb/CredentialManagement/Credential.h>
#include <LibWeb/CredentialManagement/FederatedCredential.h>
#include <LibWeb/HTML/Window.h>
#include <LibWeb/Platform/EventLoopPlugin.h>

namespace Web::CredentialManagement {
GC_DEFINE_ALLOCATOR(FederatedCredential);

WebIDL::ExceptionOr<GC::Ref<FederatedCredential>> FederatedCredential::construct_impl(JS::Realm& realm, FederatedCredentialInit const& cred)
{
    return realm.create<FederatedCredential>(realm, cred);
}

FederatedCredential::FederatedCredential(JS::Realm& realm, FederatedCredentialInit const& cred)
    : PlatformObject(realm)
{
    // FIXME: make sure provider and protocol exist
    this->m_provider = cred.provider;
    this->m_protocol = cred.protocol;
}

void FederatedCredential::initialize(JS::Realm& realm)
{
    Base::initialize(realm);
    WEB_SET_PROTOTYPE_FOR_INTERFACE(FederatedCredential);
}

}
