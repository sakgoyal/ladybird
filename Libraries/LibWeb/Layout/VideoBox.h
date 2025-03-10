/*
 * Copyright (c) 2023, Tim Flynn <trflynn89@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibWeb/DOM/Document.h>
#include <LibWeb/Forward.h>
#include <LibWeb/Layout/ReplacedBox.h>

namespace Web::Layout {

class VideoBox final
    : public ReplacedBox
    , public DOM::Document::ViewportClient {
    GC_CELL(VideoBox, ReplacedBox);
    GC_DECLARE_ALLOCATOR(VideoBox);

public:
    virtual void prepare_for_replaced_layout() override;

    HTML::HTMLVideoElement& dom_node();
    HTML::HTMLVideoElement const& dom_node() const;

    virtual GC::Ptr<Painting::Paintable> create_paintable() const override;

private:
    VideoBox(DOM::Document&, DOM::Element&, GC::Ref<CSS::ComputedProperties>);

    // ^Document::ViewportClient
    virtual void did_set_viewport_rect(CSSPixelRect const&) final;

    // ^JS::Cell
    virtual void finalize() override;
};

}
