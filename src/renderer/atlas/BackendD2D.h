// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include <til/flat_set.h>

#include "Backend.h"

namespace Microsoft::Console::Render::Atlas
{
    struct BackendD2D : IBackend
    {
        BackendD2D(wil::com_ptr<ID3D11Device2> device, wil::com_ptr<ID3D11DeviceContext2> deviceContext) noexcept;

        void Render(RenderingPayload& payload) override;
        bool RequiresContinuousRedraw() noexcept override;
        void WaitUntilCanRender() noexcept override;

        struct CachedBrush
        {
            wil::com_ptr<ID2D1SolidColorBrush> brush;
            u32 color = 0;

            constexpr bool operator==(u32 key) const noexcept
            {
                return color == key;
            }

            operator bool() const noexcept
            {
                return static_cast<bool>(brush);
            }

            constexpr CachedBrush& operator=(u32 key) noexcept
            {
                color = key;
                return *this;
            }
        };

    private:
        ATLAS_ATTR_COLD void _handleSettingsUpdate(const RenderingPayload& p);
        void _drawBackground(const RenderingPayload& p) noexcept;
        void _drawText(RenderingPayload& p);
        f32 _drawTextPrepareLineRendition(const RenderingPayload& p, f32 baselineY, LineRendition lineRendition) const noexcept;
        void _drawTextResetLineRendition() const noexcept;
        ATLAS_ATTR_COLD f32r _getGlyphRunDesignBounds(const DWRITE_GLYPH_RUN& glyphRun, f32 baselineX, f32 baselineY);
        void _drawGridlines(const RenderingPayload& p);
        void _drawGridlineRow(const RenderingPayload& p, const ShapedRow* row, u16 y);
        void _drawCursorWithColor(const RenderingPayload& p);
        void _drawCursorPart1(const RenderingPayload& p);
        void _drawCursorPart2(const RenderingPayload& p);
        static void _drawCursor(const RenderingPayload& p, ID2D1RenderTarget* renderTarget, D2D1_RECT_F rect, ID2D1Brush* brush) noexcept;
        void _resizeCursorBitmap(const RenderingPayload& p, til::size newSize);
        void _drawSelection(const RenderingPayload& p);
        void _debugShowDirty(const RenderingPayload& p);
        void _debugDumpRenderTarget(const RenderingPayload& p);
        ID2D1Brush* _brushWithColor(u32 color);
        ATLAS_ATTR_COLD void _clearBrushes() const noexcept;
        void _fillRectangle(const D2D1_RECT_F& rect, u32 color);

        SwapChainManager _swapChainManager;

        wil::com_ptr<ID3D11Device2> _device;
        wil::com_ptr<ID3D11DeviceContext2> _deviceContext;

        wil::com_ptr<ID2D1DeviceContext> _renderTarget;
        wil::com_ptr<ID2D1DeviceContext4> _renderTarget4; // Optional. Supported since Windows 10 14393.
        wil::com_ptr<ID2D1StrokeStyle> _dottedStrokeStyle;
        wil::com_ptr<ID2D1Bitmap> _backgroundBitmap;
        wil::com_ptr<ID2D1BitmapBrush> _backgroundBrush;
        til::generation_t _backgroundBitmapGeneration;

        wil::com_ptr<ID2D1Bitmap> _cursorBitmap;
        til::size _cursorBitmapSize; // in columns/rows

        til::linear_flat_set<CachedBrush> _brushes;

        Buffer<DWRITE_GLYPH_METRICS> _glyphMetrics;

        til::generation_t _generation;
        til::generation_t _fontGeneration;
        til::generation_t _cursorGeneration;
        u16x2 _cellCount{};

#if ATLAS_DEBUG_SHOW_DIRTY
        til::rect _presentRects[9]{};
        size_t _presentRectsPos = 0;
#endif

#if ATLAS_DEBUG_DUMP_RENDER_TARGET
        wchar_t _dumpRenderTargetBasePath[MAX_PATH]{};
        size_t _dumpRenderTargetCounter = 0;
#endif
    };
}
