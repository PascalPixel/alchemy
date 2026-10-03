#ifndef ALCHEMY_WINDOW_H
#define ALCHEMY_WINDOW_H

#include "TYPES.H"
#include "TBS_EDITION.H"
#include "LAYOUT_GUARD.H"
#include "RENDER_INPUT.H"

/* One window record, as UiWindow_Create returns it; positions and sizes are
   in tiles. */
struct UiWindow {
    s32 unknown_00;
    s32 unknown_04;
    u16 width;                  /* 0x08 */
    u16 height;                 /* 0x0a */
    u16 x;                      /* 0x0c */
    u16 y;                      /* 0x0e */
    u16 unknown_10;
    u16 unknown_12;
    u16 state;                  /* 0x14 */
    u16 flags;                  /* 0x16 */
    u16 frame;                  /* 0x18 */
    s16 duration;               /* 0x1a */
    u16 previous_x;             /* 0x1c */
    u16 previous_y;             /* 0x1e */
    u16 previous_width;         /* 0x20 */
    u16 previous_height;        /* 0x22 */
};

/* One of the three window text channels. Positions are 8.8 fixed point;
   entry names a built text entry, and colours hold its four palette values. */
struct UiChannelSlot {
    struct UiWindow *work;
    u16 x;
    u16 y;
    u16 colours[4];
    u16 unknown_10;
    u16 entry;
    u16 countdown;
    u16 colour;
    u16 outline;
    u16 line_spacing;
    u16 transition;
    u16 start_x;
    u16 unknown_20;
    u16 unknown_22;
    u16 flags;
    u16 unknown_26;
};

/* Window tilemap, records, text channels and render queues. Japanese text
   uses twelve windows and a 256-entry ring; other editions use eight and 512. */
struct UiRenderWork {
    u16 tilemap[640];
    struct UiWindow windows[WINDOW_COUNT];
    struct UiChannelSlot channels[3];
    struct RenderOutput outputs[64];
    struct RenderOutput *free_head;
    struct RenderOutput *free_tail;
    u8 tile_attributes[256];
    u8 unknown_before_flags[RENDER_ALT_OFS - RENDER_TILE_ATTR_OFS - 256];
    u8 alt;                     /* alternate text-canvas tiles are in use */
    u8 dirty;                   /* rows of tiles to upload, one bit per four */
    u8 mode;
    u8 menu_state;
    u8 menu_busy;               /* holds the upload while a page is redrawn */
    u8 level;                   /* text palette of the next string */
    u16 line_spacing;
    u16 unknown_after_line_spacing;
    u16 outline;
    u16 colour;
    u16 entries[RENDER_ENTRY_MASK + 1];
    u16 unknown_before_count;
    u16 count;
    u16 unknown_after_count;
    u16 glyph_resource;
    u16 glyph_tiles;
    u8 unknown_before_values[RENDER_VALUE_TBL_OFS - RENDER_COUNTER_OFS - 4];
    u32 values[8];
    u16 names[8];
    u16 resource_ids[2];
    u16 output_ids[2];
    u16 result[2];
    u8 unknown_after_result;
    u8 busy;
    u8 spacing_before;
    u8 spacing_after;
};


LAYOUT_SIZE_GUARD(UiWindow_Size, struct UiWindow, 0x24);
LAYOUT_OFFSET_GUARD(UiWindow_Frame, struct UiWindow, frame, 0x18);
LAYOUT_OFFSET_GUARD(UiRenderWork_Windows, struct UiRenderWork, windows, 0x500);
LAYOUT_OFFSET_GUARD(UiRenderWork_Channels, struct UiRenderWork, channels, RENDER_CHANNEL_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_Outputs, struct UiRenderWork, outputs, RENDER_OUTPUT_TBL_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_FreeHead, struct UiRenderWork, free_head, RENDER_FREE_HEAD_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_FreeTail, struct UiRenderWork, free_tail, RENDER_FREE_TAIL_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_TileAttributes, struct UiRenderWork, tile_attributes, RENDER_TILE_ATTR_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_Alt, struct UiRenderWork, alt, RENDER_ALT_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_Dirty, struct UiRenderWork, dirty, RENDER_DIRTY_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_LineSpacing, struct UiRenderWork, line_spacing, RENDER_WORD2_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_Outline, struct UiRenderWork, outline, RENDER_WORD_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_Colour, struct UiRenderWork, colour, RENDER_PARAM_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_Entries, struct UiRenderWork, entries, RENDER_ENTRY_TBL_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_Count, struct UiRenderWork, count, RENDER_ENTRY_COUNT_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_GlyphResource, struct UiRenderWork, glyph_resource, RENDER_COUNTER_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_GlyphTiles, struct UiRenderWork, glyph_tiles, RENDER_COUNTER_OFS + 2);
LAYOUT_OFFSET_GUARD(UiRenderWork_Values, struct UiRenderWork, values, RENDER_VALUE_TBL_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_Names, struct UiRenderWork, names, RENDER_NAME_TBL_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_ResourceIds, struct UiRenderWork, resource_ids, RENDER_SLOT_ID_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_OutputIds, struct UiRenderWork, output_ids, RENDER_SLOT_VALUE_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_Result, struct UiRenderWork, result, RENDER_RESULT_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_Busy, struct UiRenderWork, busy, RENDER_BUSY_OFS);
LAYOUT_OFFSET_GUARD(UiRenderWork_SpacingBefore, struct UiRenderWork, spacing_before, RENDER_BUSY_OFS + 1);
LAYOUT_OFFSET_GUARD(UiRenderWork_SpacingAfter, struct UiRenderWork, spacing_after, RENDER_BUSY_OFS + 2);
LAYOUT_SIZE_GUARD(UiRenderWork_Size, struct UiRenderWork, RENDER_WORK_SIZE);
LAYOUT_SIZE_GUARD(UiChannelSlot_Size, struct UiChannelSlot, 0x28);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Entry, struct UiChannelSlot, entry, 0x12);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Countdown, struct UiChannelSlot, countdown, 0x14);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Transition, struct UiChannelSlot, transition, 0x1c);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Flags, struct UiChannelSlot, flags, 0x24);

extern u8 *gWindowWork[];
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWork_Finalize(struct UiWindow *window, s32 release);

#endif
