#ifndef ALCHEMY_WINDOW_H
#define ALCHEMY_WINDOW_H

#include "OUTPUT.H"
#include "UIFLOW.H"
#include "LAYOUT_GUARD.H"

enum { UI_WINDOW_COUNT = 12, UI_OUTPUT_COUNT = 64 };

/* The known windows, text channels and output queues in the resident work.
   The prefix and the remainder of the work are not described. */
struct UiRenderWork {
    u8 unknown_before_windows[0x508];
    struct UiWindow windows[UI_WINDOW_COUNT];
    struct UiChannelSlot channels[3];
    struct RenderOutput outputs[UI_OUTPUT_COUNT];
    struct RenderOutputList free_outputs;
};

LAYOUT_OFFSET_GUARD(UiRenderWork_Windows, struct UiRenderWork, windows, 0x508);
LAYOUT_OFFSET_GUARD(UiRenderWork_Channels, struct UiRenderWork, channels, 0x6b8);
LAYOUT_OFFSET_GUARD(UiRenderWork_Outputs, struct UiRenderWork, outputs, 0x730);
LAYOUT_OFFSET_GUARD(UiRenderWork_FreeHead, struct UiRenderWork, free_outputs.head, 0xe30);
LAYOUT_OFFSET_GUARD(UiRenderWork_FreeTail, struct UiRenderWork, free_outputs.tail_link, 0xe34);

void UiWork_InitFreeList(void);

#endif
