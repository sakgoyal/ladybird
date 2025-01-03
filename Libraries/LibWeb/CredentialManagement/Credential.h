/*
 * Copyright (c) 2024, Saksham Goyal <sakgoy2001@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibGC/Ptr.h>
#include <LibWeb/WebIDL/ExceptionOr.h>
#include <LibWeb/WebIDL/Promise.h>

namespace Web::CredentialManagement {
struct CredentialData {
    String id;
};

class Credential : public Bindings::PlatformObject {
    WEB_PLATFORM_OBJECT(Credential, Bindings::PlatformObject);
    GC_DECLARE_ALLOCATOR(Credential);

public:
    virtual ~Credential() override = default;

    static WebIDL::ExceptionOr<GC::Ref<Credential>> construct_impl(JS::Realm&);

    auto const& id() { return m_id; }
    auto const& name() { return m_name; }
    auto const& icon_url() { return m_icon_url; }
    auto const& type() const { return m_type; }

    static JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> is_conditional_mediation_available(JS::VM&);
    static JS::ThrowCompletionOr<GC::Ref<WebIDL::Promise>> will_request_conditional_creation(JS::VM&);

protected:
    explicit Credential(JS::Realm& realm)
        : PlatformObject(realm)
    {
    }

    virtual void initialize(JS::Realm& realm) override;

    String m_id;
    String m_name;
    String m_icon_url;
    String m_type;
};

}
