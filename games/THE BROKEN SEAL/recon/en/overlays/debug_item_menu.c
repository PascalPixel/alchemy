#include "FIELD_EVENT.H"
#include "INVENTORY.H"
struct ItemDefinition;
struct ItemDefinition *Engine_DebugGetItem(s32 item);
#include "TEXT_RENDER_RUNTIME.H"

struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
};

extern u8 gDebugItemPrompt[];
extern u8 gDebugItemCapacityLabel[];
extern u8 gDebugItemFullLabel[];
extern volatile u32 gDebugKeysPressed;
extern volatile u32 gDebugKeysRepeated;

struct TextRenderWork *Engine_DebugCreateWindow(s32 x, s32 y, s32 width, s32 height, s32 style);
void Engine_DebugFinalizeWindow(struct TextRenderWork *window, s32 mode);
void Engine_DebugRedrawWindow(struct TextRenderWork *window);
void Engine_DebugClearWindow(struct TextRenderWork *window);
void UiText_DrawStringInWindowFar(u8 *text, struct TextRenderWork *window, s32 x, s32 y);
void UiText_DrawNumberAtOffsetFar(s32 value, s32 format, struct TextRenderWork *window, s32 x, s32 y);
void UiText_DrawCharacterAtOffset(s32 text, struct TextRenderWork *window, s32 x, s32 y);
void Engine_DebugDrawTextResource(s32 text, struct TextRenderWork *window, s32 x, s32 y);
void Engine_DebugDrawItemDetails(struct TextRenderWork *window, s32 item);
s32 Engine_DebugRemainder(s32 value, s32 divisor);

void DebugMenu_SelectItem(void)
{
    struct TextRenderWork *window;
    struct TextRenderWork *details;
    volatile struct DmaTransfer *dma = (volatile struct DmaTransfer *)0x040000d4;
    s32 item;
    s32 redraw;
    s32 index;

    Engine_AudioPlayCue(112);
    window = Engine_DebugCreateWindow(0, 0, 30, 7, 2);
    details = Engine_DebugCreateWindow(0, 8, 13, 10, 2);
    item = 1;
    redraw = 1;
    dma->source = (const void *)0x05000200;
    dma->destination = (void *)0x050001c0;
    dma->control = 0x80000010;
    dma->source = (const void *)0x050001e8;
    dma->destination = (void *)0x050001dc;
    dma->control = 0x80000001;
    for (;;) {
        Engine_TaskWait(1);
        if (redraw) {
            redraw = 0;
            item = Engine_DebugRemainder(item + 270, 270);
            Engine_DebugRedrawWindow(window);
            Engine_DebugClearWindow(window);
            UiText_DrawStringInWindowFar(gDebugItemPrompt, window, 0, 0);
            UiText_DrawNumberAtOffsetFar(item, 0, window, 80, 0);
            if (PartyInventory_HasSpace()) {
                index = item & 0x1ff;
                UiText_DrawStringInWindowFar(gDebugItemCapacityLabel, window, 0, 32);
                Engine_DebugGetItem(index);
                UiText_DrawCharacterAtOffset(index + 0x182, window, 120, 0);
                Engine_DebugDrawTextResource(index + 0x75, window, 0, 16);
                Engine_DebugRedrawWindow(details);
                Engine_DebugDrawItemDetails(details, item);
            } else {
                UiText_DrawStringInWindowFar(gDebugItemFullLabel, window, 0, 32);
            }
        }
        if (gDebugKeysPressed & 1) {
            if (PartyInventory_Add(item) == -1)
                break;
            Engine_AudioPlayCue(175);
        }
        if (gDebugKeysPressed & 2)
            break;
        if (gDebugKeysRepeated & 64) {
            item--;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gDebugKeysRepeated & 128) {
            item++;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gDebugKeysRepeated & 16) {
            item += 10;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gDebugKeysRepeated & 32) {
            item -= 10;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gDebugKeysRepeated & 256) {
            item += 30;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gDebugKeysRepeated & 512) {
            item -= 30;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
    }
    Engine_AudioPlayCue(113);
    Engine_DebugRedrawWindow(window);
    Engine_TaskWait(1);
    Engine_DebugFinalizeWindow(window, 1);
    Engine_DebugFinalizeWindow(details, 1);
}
