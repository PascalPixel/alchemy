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

/* The known tilemap, windows, text channels, output queues and entry ring.
   Other fields and the remainder of the resident work are not described. */
struct UiRenderWork {
    u8 unknown_before_tilemap[8];
    u16 tilemap[640];
    struct UiWindow windows[UI_WINDOW_COUNT];
    struct UiChannelSlot channels[3];
    struct RenderOutput outputs[UI_OUTPUT_COUNT];
    struct RenderOutputList free_outputs;
    u8 unknown_before_entries[0x108];
    u16 entries[UI_TEXT_ENTRY_COUNT];
    u16 unknown_before_count;
    u16 count;
};

LAYOUT_OFFSET_GUARD(UiRenderWork_Tilemap, struct UiRenderWork, tilemap, 8);
LAYOUT_OFFSET_GUARD(UiRenderWork_Windows, struct UiRenderWork, windows, 0x508);
LAYOUT_OFFSET_GUARD(UiRenderWork_Channels, struct UiRenderWork, channels, 0x6b8);
LAYOUT_OFFSET_GUARD(UiRenderWork_Outputs, struct UiRenderWork, outputs, 0x730);
LAYOUT_OFFSET_GUARD(UiRenderWork_FreeHead, struct UiRenderWork, free_outputs.head, 0xe30);
LAYOUT_OFFSET_GUARD(UiRenderWork_FreeTail, struct UiRenderWork, free_outputs.tail_link, 0xe34);

LAYOUT_OFFSET_GUARD(UiRenderWork_Entries, struct UiRenderWork, entries, 0xf40);
#if EDITION_INTERNATIONAL
LAYOUT_OFFSET_GUARD(UiRenderWork_Count, struct UiRenderWork, count, 0x1342);
#else
LAYOUT_OFFSET_GUARD(UiRenderWork_Count, struct UiRenderWork, count, 0x1142);
#endif

void UiWork_InitFreeList(void);

#endif
