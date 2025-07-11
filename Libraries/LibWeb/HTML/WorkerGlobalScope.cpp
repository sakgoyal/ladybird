/*
 * Copyright (c) 2022, Andrew Kaster <akaster@serenityos.org>
 * Copyright (c) 2025, Luke Wilde <luke@ladybird.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Vector.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/Bindings/WorkerGlobalScopePrototype.h>
#include <LibWeb/CSS/FontFaceSet.h>
#include <LibWeb/ContentSecurityPolicy/Directives/Directive.h>
#include <LibWeb/ContentSecurityPolicy/Policy.h>
#include <LibWeb/ContentSecurityPolicy/PolicyList.h>
#include <LibWeb/Fetch/Infrastructure/URL.h>
#include <LibWeb/HTML/EventHandler.h>
#include <LibWeb/HTML/EventNames.h>
#include <LibWeb/HTML/MessageEvent.h>
#include <LibWeb/HTML/MessagePort.h>
#include <LibWeb/HTML/Scripting/ClassicScript.h>
#include <LibWeb/HTML/StructuredSerialize.h>
#include <LibWeb/HTML/WorkerGlobalScope.h>
#include <LibWeb/HTML/WorkerLocation.h>
#include <LibWeb/HTML/WorkerNavigator.h>
#include <LibWeb/Page/Page.h>

namespace Web::HTML {

GC_DEFINE_ALLOCATOR(WorkerGlobalScope);

WorkerGlobalScope::WorkerGlobalScope(JS::Realm& realm, GC::Ref<Web::Page> page)
    : DOM::EventTarget(realm)
    , m_page(page)
{
}

WorkerGlobalScope::~WorkerGlobalScope() = default;

void WorkerGlobalScope::initialize_web_interfaces_impl()
{
    auto& realm = this->realm();
    Base::initialize(realm);

    WindowOrWorkerGlobalScopeMixin::initialize(realm);

    m_navigator = WorkerNavigator::create(*this);
}

void WorkerGlobalScope::visit_edges(Cell::Visitor& visitor)
{
    Base::visit_edges(visitor);
    WindowOrWorkerGlobalScopeMixin::visit_edges(visitor);
    UniversalGlobalScopeMixin::visit_edges(visitor);

    visitor.visit(m_location);
    visitor.visit(m_navigator);
    visitor.visit(m_internal_port);
    visitor.visit(m_page);
    visitor.visit(m_fonts);
    visitor.visit(m_policy_container);
}

void WorkerGlobalScope::finalize()
{
    Base::finalize();
    WindowOrWorkerGlobalScopeMixin::finalize();
}

void WorkerGlobalScope::set_internal_port(GC::Ref<MessagePort> port)
{
    m_internal_port = port;
    m_internal_port->set_worker_event_target(*this);
}

// https://html.spec.whatwg.org/multipage/workers.html#close-a-worker
void WorkerGlobalScope::close_a_worker()
{
    // 1. Discard any tasks that have been added to workerGlobal's relevant agent's event loop's task queues.
    relevant_settings_object(*this).responsible_event_loop().task_queue().remove_tasks_matching([](HTML::Task const& task) {
        // NOTE: We don't discard tasks with the PostedMessage source, as the spec expects PostMessage() to act as if it is invoked immediately
        return task.source() != HTML::Task::Source::PostedMessage;
    });

    // 2. Set workerGlobal's closing flag to true. (This prevents any further tasks from being queued.)
    m_closing = true;
}

// https://html.spec.whatwg.org/multipage/workers.html#importing-scripts-and-libraries
// https://whatpr.org/html/9893/workers.html#importing-scripts-and-libraries
WebIDL::ExceptionOr<void> WorkerGlobalScope::import_scripts(Vector<String> const& urls, PerformTheFetchHook perform_fetch)
{
    // The algorithm may optionally be customized by supplying custom perform the fetch hooks,
    // which if provided will be used when invoking fetch a classic worker-imported script.
    // NOTE: Service Workers is an example of a specification that runs this algorithm with its own options for the perform the fetch hook.

    // FIXME: 1. If worker global scope's type is "module", throw a TypeError exception.

    // 2. Let settings object be the current principal settings object.
    auto& settings_object = HTML::current_principal_settings_object();

    // 3. If urls is empty, return.
    if (urls.is_empty())
        return {};

    // 4. Let urlRecords be « ».
    Vector<URL::URL> url_records;
    url_records.ensure_capacity(urls.size());

    // 5. For each url of urls:
    for (auto const& url : urls) {
        // 1. Let urlRecord be the result of encoding-parsing a URL given url, relative to settings object.
        auto url_record = settings_object.encoding_parse_url(url);

        // 2. If urlRecord is failure, then throw a "SyntaxError" DOMException.
        if (!url_record.has_value())
            return WebIDL::SyntaxError::create(realm(), "Invalid URL"_string);

        // 3. Append urlRecord to urlRecords.
        url_records.unchecked_append(url_record.release_value());
    }

    // 6. For each urlRecord of urlRecords:
    for (auto const& url_record : url_records) {
        // 1. Fetch a classic worker-imported script given urlRecord and settings object, passing along performFetch if provided.
        //    If this succeeds, let script be the result. Otherwise, rethrow the exception.
        auto classic_script = TRY(HTML::fetch_a_classic_worker_imported_script(url_record, settings_object, perform_fetch));

        // 2. Run the classic script script, with rethrow errors set to true.
        // NOTE: script will run until it either returns, fails to parse, fails to catch an exception,
        //       or gets prematurely aborted by the terminate a worker algorithm defined above.
        // If an exception was thrown or if the script was prematurely aborted, then abort all these steps,
        // letting the exception or aborting continue to be processed by the calling script.
        TRY(classic_script->run(ClassicScript::RethrowErrors::Yes));
    }

    return {};
}

// https://html.spec.whatwg.org/multipage/workers.html#dom-workerglobalscope-location
GC::Ref<WorkerLocation> WorkerGlobalScope::location() const
{
    // The location attribute must return the WorkerLocation object whose associated WorkerGlobalScope object is the WorkerGlobalScope object.
    return *m_location;
}

// https://html.spec.whatwg.org/multipage/workers.html#dom-worker-navigator
GC::Ref<WorkerNavigator> WorkerGlobalScope::navigator() const
{
    // The navigator attribute of the WorkerGlobalScope interface must return an instance of the WorkerNavigator interface,
    // which represents the identity and state of the user agent (the client).
    return *m_navigator;
}

#undef __ENUMERATE
#define __ENUMERATE(attribute_name, event_name)                               \
    void WorkerGlobalScope::set_##attribute_name(WebIDL::CallbackType* value) \
    {                                                                         \
        set_event_handler_attribute(event_name, move(value));                 \
    }                                                                         \
    WebIDL::CallbackType* WorkerGlobalScope::attribute_name()                 \
    {                                                                         \
        return event_handler_attribute(event_name);                           \
    }
ENUMERATE_WORKER_GLOBAL_SCOPE_EVENT_HANDLERS(__ENUMERATE)
#undef __ENUMERATE

GC::Ref<CSS::FontFaceSet> WorkerGlobalScope::fonts()
{
    if (!m_fonts)
        m_fonts = CSS::FontFaceSet::create(realm());
    return *m_fonts;
}

GC::Ref<PolicyContainer> WorkerGlobalScope::policy_container() const
{
    auto& heap = this->heap();
    if (!m_policy_container) {
        m_policy_container = heap.allocate<PolicyContainer>(heap);
    }
    return *m_policy_container;
}

// https://html.spec.whatwg.org/multipage/browsers.html#initialize-worker-policy-container
void WorkerGlobalScope::initialize_policy_container(GC::Ref<Fetch::Infrastructure::Response const> response, GC::Ref<EnvironmentSettingsObject> environment)
{
    auto& realm = this->realm();

    // 1. If workerGlobalScope's url is local but its scheme is not "blob":
    if (m_url.has_value() && Fetch::Infrastructure::is_local_url(m_url.value()) && m_url->scheme() != "blob"sv) {
        // FIXME: 1. Assert: workerGlobalScope's owner set's size is 1.
        // FIXME: 2. Set workerGlobalScope's policy container to a clone of workerGlobalScope's owner set[0]'s relevant settings object's policy container.
        dbgln("FIXME: WorkerGlobalScope::initialize_policy_container: Clone owner's policy container for local, non-blob URL.");
        return;
    }

    // 2. Otherwise, set workerGlobalScope's policy container to the result of creating a policy container from a fetch
    //    response given response and environment.
    m_policy_container = create_a_policy_container_from_a_fetch_response(realm.heap(), response, environment);
}

// https://w3c.github.io/webappsec-csp/#run-global-object-csp-initialization
ContentSecurityPolicy::Directives::Directive::Result WorkerGlobalScope::run_csp_initialization() const
{
    // 1. Let result be "Allowed".
    auto result = ContentSecurityPolicy::Directives::Directive::Result::Allowed;

    // 2. For each policy of global’s CSP list:
    for (auto policy : policy_container()->csp_list->policies()) {
        // 1. For each directive of policy:
        for (auto directive : policy->directives()) {
            // 1. Execute directive’s initialization algorithm on global. If its returned value is "Blocked", then set
            //    result to "Blocked".
            auto directive_result = directive->initialization(GC::Ref { *this }, policy);
            if (directive_result == ContentSecurityPolicy::Directives::Directive::Result::Blocked) {
                result = ContentSecurityPolicy::Directives::Directive::Result::Blocked;
            }
        }
    }

    // 3. Return result.
    return result;
}

}
