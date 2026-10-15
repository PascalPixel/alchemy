#ifndef ALCHEMY_WINDOW_H
#define ALCHEMY_WINDOW_H

#include "edition.h"
#include "output.h"
#include "uiflow.h"
#include "layout_guard.h"

enum { UI_WINDOW_COUNT = 12, UI_OUTPUT_COUNT = 64 };

#if EDITION_INTERNATIONAL
enum { UI_TEXT_ENTRY_COUNT = 512 };
#else
enum { UI_TEXT_ENTRY_COUNT = 256 };
#endif

struct UiRenderWork {
    u8 unknown_00[3];
    u8 dirty;
    u8 mode;
    u8 menu_state;
    u8 menu_busy;
    u8 level;
    u16 tilemap[20][32];
    struct UiWindow windows[UI_WINDOW_COUNT];
    struct UiChannelSlot channels[3];
    struct RenderOutput outputs[UI_OUTPUT_COUNT];
    struct RenderOutputList free_outputs;
    u8 tile_attributes[0x100];
    u16 line_spacing;
    u16 unknown_after_line_spacing;
    u16 outline;
    u16 colour;
    u16 entries[UI_TEXT_ENTRY_COUNT];
    u16 glyph_spacing;
    u16 count;
    u16 unknown_after_count;
    u16 glyph_resource;
    u16 glyph_tiles;
    u8 unknown_before_values[2];
    u32 values[8];
    u16 names[8];
    u16 resource_ids[2];
    u16 output_ids[2];
    u16 result[2];
    u8 unknown_1388;
    u8 busy;
    u8 unknown_138a[2];
    s8 text_mode;
    u8 unknown_138d[3];
};

LAYOUT_OFFSET_GUARD(UiRenderWork_Tilemap, struct UiRenderWork, tilemap, 8);
LAYOUT_OFFSET_GUARD(UiRenderWork_Windows, struct UiRenderWork, windows, 0x508);
LAYOUT_OFFSET_GUARD(UiRenderWork_Channels, struct UiRenderWork, channels, 0x6b8);
LAYOUT_OFFSET_GUARD(UiRenderWork_Outputs, struct UiRenderWork, outputs, 0x730);
LAYOUT_OFFSET_GUARD(UiRenderWork_FreeHead, struct UiRenderWork, free_outputs.head, 0xe30);
LAYOUT_OFFSET_GUARD(UiRenderWork_FreeTail, struct UiRenderWork, free_outputs.tail_link, 0xe34);

LAYOUT_OFFSET_GUARD(UiRenderWork_LineSpacing, struct UiRenderWork, line_spacing, 0xf38);
LAYOUT_OFFSET_GUARD(UiRenderWork_Colour, struct UiRenderWork, colour, 0xf3e);
LAYOUT_OFFSET_GUARD(UiRenderWork_Entries, struct UiRenderWork, entries, 0xf40);
LAYOUT_OFFSET_GUARD(UiRenderWork_MenuState, struct UiRenderWork, menu_state, 5);
LAYOUT_OFFSET_GUARD(UiRenderWork_Level, struct UiRenderWork, level, 7);
LAYOUT_OFFSET_GUARD(UiRenderWork_TileAttributes, struct UiRenderWork, tile_attributes, 0xe38);
#if EDITION_INTERNATIONAL
LAYOUT_OFFSET_GUARD(UiRenderWork_Count, struct UiRenderWork, count, 0x1342);
LAYOUT_OFFSET_GUARD(UiRenderWork_GlyphResource, struct UiRenderWork, glyph_resource, 0x1346);
LAYOUT_OFFSET_GUARD(UiRenderWork_Values, struct UiRenderWork, values, 0x134c);
LAYOUT_OFFSET_GUARD(UiRenderWork_Names, struct UiRenderWork, names, 0x136c);
LAYOUT_SIZE_GUARD(UiRenderWork_Size, struct UiRenderWork, 0x1390);
#else
LAYOUT_OFFSET_GUARD(UiRenderWork_Count, struct UiRenderWork, count, 0x1142);
LAYOUT_OFFSET_GUARD(UiRenderWork_GlyphResource, struct UiRenderWork, glyph_resource, 0x1146);
LAYOUT_OFFSET_GUARD(UiRenderWork_Values, struct UiRenderWork, values, 0x114c);
LAYOUT_OFFSET_GUARD(UiRenderWork_Names, struct UiRenderWork, names, 0x116c);
LAYOUT_SIZE_GUARD(UiRenderWork_Size, struct UiRenderWork, 0x1190);
#endif

void UiWork_InitFreeList(void);

#endif
