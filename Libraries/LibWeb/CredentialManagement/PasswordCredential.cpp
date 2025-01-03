/*
 * Copyright (c) 2024, Saksham Goyal <sakgoy2001@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/String.h>
#include <AK/Vector.h>
#include <LibWeb/Bindings/PasswordCredentialPrototype.h>
#include <LibWeb/CredentialManagement/Credential.h>
#include <LibWeb/CredentialManagement/PasswordCredential.h>
#include <LibWeb/HTML/Window.h>
#include <LibWeb/Platform/EventLoopPlugin.h>

namespace Web::CredentialManagement {
GC_DEFINE_ALLOCATOR(PasswordCredential);

WebIDL::ExceptionOr<GC::Ref<PasswordCredential>> PasswordCredential::construct_impl(JS::Realm& realm, GC::Ptr<HTML::HTMLFormElement> const& cred)
{
    return realm.create<PasswordCredential>(realm, cred);
}

WebIDL::ExceptionOr<GC::Ref<PasswordCredential>> PasswordCredential::construct_impl(JS::Realm& realm, PasswordCredentialData const& cred)
{
    return realm.create<PasswordCredential>(realm, cred);
}

PasswordCredential::PasswordCredential(JS::Realm& realm, PasswordCredentialData const&)
    : PlatformObject(realm)
{
    // FIXME
}

PasswordCredential::PasswordCredential(JS::Realm& realm, GC::Ptr<HTML::HTMLFormElement> const&)
    : PlatformObject(realm)
{
    // FIXME
}

void PasswordCredential::initialize(JS::Realm& realm)
{
    Base::initialize(realm);
    WEB_SET_PROTOTYPE_FOR_INTERFACE(PasswordCredential);
}

}
