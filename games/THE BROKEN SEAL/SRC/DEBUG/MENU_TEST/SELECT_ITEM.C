#include "TYPES.H"
#include "INVENTORY.H"
#include "DMA.H"
#include "TEXT_RENDER_RUNTIME.H"
#include "ITEM.H"

extern u8 MsgItemName[];
extern u8 MsgItemPlainName[];
struct ItemDefinition *Engine_DebugGetItem(s32 item);
void Engine_AudioPlayCue(s32 cue);

extern u8 gDebugItemPrompt[];
extern u8 gDebugItemCapacityLabel[];
extern u8 gDebugItemFullLabel[];
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;

struct TextRenderWork *Engine_DebugCreateWindow(s32 x, s32 y, s32 width, s32 height, s32 style);
void Engine_DebugFinalizeWindow(struct TextRenderWork *window, s32 mode);
void Engine_DebugRedrawWindow(struct TextRenderWork *window);
void Engine_DebugClearWindow(struct TextRenderWork *window);
void UiText_DrawStringInWindow(u8 *text, struct TextRenderWork *window, s32 x, s32 y);
void UiText_DrawNumberAtOffset(s32 value, s32 format, struct TextRenderWork *window, s32 x, s32 y);
void UiText_DrawCharacterAtOffset(s32 text, struct TextRenderWork *window, s32 x, s32 y);
void UiText_DrawResource(s32 text, struct TextRenderWork *window, s32 x, s32 y);
void Engine_DebugDrawItemDetails(struct TextRenderWork *window, s32 item);
s32 Engine_DebugRemainder(s32 value, s32 divisor);

void DebugMenu_SelectItem(void)
{
    struct TextRenderWork *window;
    struct TextRenderWork *details;
    s32 item;
    s32 redraw;
    s32 index;
    s32 count;

    count = 270;
    Engine_AudioPlayCue(112);
    window = Engine_DebugCreateWindow(0, 0, 30, 7, 2);
    details = Engine_DebugCreateWindow(0, 8, 13, 10, 2);
    item = 1;
    redraw = 1;
    Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010,
            (volatile u32 *)0x040000d4);
    Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001,
            (volatile u32 *)0x040000d4);
    Engine_TaskWait(1);
    for (;;) {
        if (redraw) {
            redraw = 0;
            item = Engine_DebugRemainder(item + 270, count);
            Engine_DebugRedrawWindow(window);
            Engine_DebugClearWindow(window);
            UiText_DrawStringInWindow(gDebugItemPrompt, window, 0, 0);
            UiText_DrawNumberAtOffset(item, 0, window, 80, 0);
            if (PartyInventory_HasSpace()) {
                UiText_DrawStringInWindow(gDebugItemCapacityLabel, window, 0, 32);
                index = item & 0x1ff;
                Engine_DebugGetItem(index);
                UiText_DrawCharacterAtOffset(index + (s32)MsgItemName, window, 120, 0);
                UiText_DrawResource(index + (s32)MsgItemPlainName, window, 0, 16);
                Engine_DebugRedrawWindow(details);
                Engine_DebugDrawItemDetails(details, item);
            } else {
                UiText_DrawStringInWindow(gDebugItemFullLabel, window, 0, 32);
            }
        }
        if (gKeyState & 1) {
            if (PartyInventory_Add(item) == -1) {
                Engine_AudioPlayCue(113);
                goto done;
            }
            Engine_AudioPlayCue(175);
        }
        if (gKeyState & 2) {
            Engine_AudioPlayCue(113);
            goto done;
        }
        if (gKeysRepeat & 64) {
            item--;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 128) {
            item++;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 16) {
            item += 10;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 32) {
            item -= 10;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 256) {
            item += 30;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 512) {
            item -= 30;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        Engine_TaskWait(1);
    }
done:
    Engine_DebugRedrawWindow(window);
    Engine_TaskWait(1);
    Engine_DebugFinalizeWindow(window, 1);
    Engine_DebugFinalizeWindow(details, 1);
}
