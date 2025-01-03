/*
 * Copyright (c) 2024, Saksham Goyal <sakgoy2001@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/HashTable.h>
#include <AK/Vector.h>
#include <LibWeb/Bindings/PasswordCredentialPrototype.h>
#include <LibWeb/Bindings/PlatformObject.h>
#include <LibWeb/CredentialManagement/Credential.h>
#include <LibWeb/DOM/AbortSignal.h>
#include <LibWeb/HTML/HTMLFormElement.h>
#include <LibWeb/HTML/Window.h>
#include <LibWeb/Platform/EventLoopPlugin.h>
#include <LibWeb/WebIDL/Promise.h>

namespace Web::CredentialManagement {

struct PasswordCredentialData : CredentialData {
    Optional<String> name;
    Optional<String> icon_url;
    String origin;
    String password;
};

using PasswordCredentialInit = Variant<GC::Root<PasswordCredentialData>, GC::Root<HTML::HTMLFormElement>>;

class PasswordCredential final : public Bindings::PlatformObject {
    WEB_PLATFORM_OBJECT(PasswordCredential, Bindings::PlatformObject);
    GC_DECLARE_ALLOCATOR(PasswordCredential);

public:
    static WebIDL::ExceptionOr<GC::Ref<PasswordCredential>> construct_impl(JS::Realm&, PasswordCredentialData const&);
    static WebIDL::ExceptionOr<GC::Ref<PasswordCredential>> construct_impl(JS::Realm&, GC::Ptr<HTML::HTMLFormElement> const&);

    auto password() { return m_password; }
    auto name() { return m_name; }
    auto icon_url() { return m_icon_url; }

private:
    PasswordCredential(JS::Realm&, PasswordCredentialData const&);
    PasswordCredential(JS::Realm&, GC::Ptr<HTML::HTMLFormElement> const&);

    virtual void initialize(JS::Realm& realm) override;

    String m_id;
    String m_name;
    String m_icon_url;
    String m_origin;
    String m_password;
};

}
