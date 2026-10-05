#ifndef ALCHEMY_WINDOW_H
#define ALCHEMY_WINDOW_H

#include "EDITION.H"
#include "OUTPUT.H"
#include "UIFLOW.H"
#include "LAYOUT_GUARD.H"

enum { UI_WINDOW_COUNT = 12, UI_OUTPUT_COUNT = 64 };

#if EDITION_INTERNATIONAL
enum { UI_TEXT_ENTRY_COUNT = 512 };
#else
enum { UI_TEXT_ENTRY_COUNT = 256 };
#endif

/* The known tilemap, windows, text channels, output queues, entry ring and named values.
   Other fields and the remainder of the resident work are not described. */
struct UiRenderWork {
    u8 unknown_00[3];
    /* Set when the tilemap needs copying to VRAM. */
    u8 dirty;
    u8 unknown_04[4];
    /* Twenty rows of 32 cells: the screen's 30 columns and two spare. */
    u16 tilemap[20][32];
    struct UiWindow windows[UI_WINDOW_COUNT];
    struct UiChannelSlot channels[3];
    struct RenderOutput outputs[UI_OUTPUT_COUNT];
    struct RenderOutputList free_outputs;
    u8 unknown_before_entries[0x100];
    /* The text pen: line spacing, outline and colour. */
    u16 line_spacing;
    u16 unknown_after_line_spacing;
    u16 outline;
    u16 colour;
    u16 entries[UI_TEXT_ENTRY_COUNT];
    /* How far the pen moves right after each character, as line_spacing
       moves it down after each line. */
    u16 glyph_spacing;
    u16 count;
    u8 unknown_before_values[8];
    u32 values[8];
    u16 names[8];
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
#if EDITION_INTERNATIONAL
LAYOUT_OFFSET_GUARD(UiRenderWork_Count, struct UiRenderWork, count, 0x1342);
LAYOUT_OFFSET_GUARD(UiRenderWork_Values, struct UiRenderWork, values, 0x134c);
LAYOUT_OFFSET_GUARD(UiRenderWork_Names, struct UiRenderWork, names, 0x136c);
#else
LAYOUT_OFFSET_GUARD(UiRenderWork_Count, struct UiRenderWork, count, 0x1142);
LAYOUT_OFFSET_GUARD(UiRenderWork_Values, struct UiRenderWork, values, 0x114c);
LAYOUT_OFFSET_GUARD(UiRenderWork_Names, struct UiRenderWork, names, 0x116c);
#endif

void UiWork_InitFreeList(void);

#endif
