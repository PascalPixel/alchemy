/* The item and level debug room's item browser: pick an item number, see its
 * name and details, and A adds it to the party's bag. Stepping skips the
 * numbers with no item icon. */
#include "TYPES.H"
#include "INVENTORY.H"
#include "DMA.H"
#include "TEXT_RENDER_RUNTIME.H"
#include "ITEM.H"

extern u8 MsgItemName[];
extern u8 MsgItemPlainName[];
struct ItemDefinition *Engine_DebugGetItem(s32 item);
void Engine_AudioPlayCue(s32 cue);
void Engine_TaskWait(s32 frames);
extern u8 gItemLevelItemPrompt[];
extern u8 gItemLevelItemHelp[];
extern u8 gItemLevelItemFull[];
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;
struct TextRenderWork *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWork_Finalize(struct TextRenderWork *window, s32 mode);
void RenderOutput_RedrawSavedRect(struct TextRenderWork *window);
void Engine_DebugClearWindow(struct TextRenderWork *window);
void UiText_DrawStringInWindow(u8 *text, struct TextRenderWork *window, s32 x, s32 y);
void UiText_DrawNumberAtOffset(s32 value, s32 format, struct TextRenderWork *window, s32 x, s32 y);
void UiText_DrawCharacterAtOffset(s32 text, struct TextRenderWork *window, s32 x, s32 y);
void Engine_DebugDrawItemDetails(struct TextRenderWork *window, s32 item);
s32 Engine_DebugRemainder(s32 value, s32 divisor);
#define ITEM_COUNT 270

extern u8 MsgAbilityName[];
extern u8 MsgAbilityDescription[];
u8 *Engine_DebugGetAbility(s32 ability);
extern u8 gItemLevelPsyPrompt[];
extern u8 gItemLevelPsyHelp[];
void UiText_DrawResource(s32 text, struct TextRenderWork *window, s32 x, s32 y);
#define ABILITY_COUNT 270

void ItemLevel_SelectItem(void)
{
    struct TextRenderWork *window;
    struct TextRenderWork *details;
    s32 item;
    s32 redraw;
    s32 index;
    s32 count;
    s8 step;

    count = ITEM_COUNT;
    step = 0;
    item = 1;
    Engine_AudioPlayCue(112);
    redraw = 1;
    window = UiWindow_Create(0, 0, 30, 7, 2);
    details = UiWindow_Create(0, 8, 28, 10, 2);
    Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010,
            (volatile u32 *)0x040000d4);
    Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001,
            (volatile u32 *)0x040000d4);
    Engine_TaskWait(1);
    for (;;) {
        if (redraw) {
            redraw = 0;
            item = Engine_DebugRemainder(item + ITEM_COUNT, count);
            RenderOutput_RedrawSavedRect(window);
            Engine_DebugClearWindow(window);
            UiText_DrawStringInWindow(gItemLevelItemPrompt, window, 0, 0);
            UiText_DrawNumberAtOffset(item, 0, window, 80, 0);
            if (PartyInventory_HasSpace()) {
                UiText_DrawStringInWindow(gItemLevelItemHelp, window, 0, 32);
                index = item & 0x1ff;
                Engine_DebugGetItem(index);
                UiText_DrawCharacterAtOffset(index + (s32)MsgItemName, window, 120, 0);
                UiText_DrawCharacterAtOffset(index + (s32)MsgItemPlainName, window, 0, 16);
                RenderOutput_RedrawSavedRect(details);
                Engine_DebugDrawItemDetails(details, item);
            } else {
                UiText_DrawStringInWindow(gItemLevelItemFull, window, 0, 32);
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
            step = -1;
            item--;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 128) {
            step = 1;
            item++;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 16) {
            step = 1;
            item += 10;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 32) {
            step = -1;
            item -= 10;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 256) {
            step = 1;
            item += 30;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 512) {
            step = -1;
            item -= 30;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (step == -1) {
            item = Engine_DebugRemainder(item + ITEM_COUNT, count);
            goto check_back;
        back:
            item = Engine_DebugRemainder(item + ITEM_COUNT - 1, count);
        check_back:
            if (Engine_DebugGetItem(item & 0x1ff)->icon == 0)
                goto back;
        }
        if (step == 1) {
            item = Engine_DebugRemainder(item + ITEM_COUNT, count);
            goto check_ahead;
        ahead:
            item = Engine_DebugRemainder(item + ITEM_COUNT + 1, count);
        check_ahead:
            if (Engine_DebugGetItem(item & 0x1ff)->icon == 0)
                goto ahead;
        }
        step = 0;
        Engine_TaskWait(1);
    }
done:
    RenderOutput_RedrawSavedRect(window);
    Engine_TaskWait(1);
    UiWork_Finalize(window, 1);
    UiWork_Finalize(details, 1);
}

/* The item and level debug room's Psynergy browser: pick an ability number
 * and see its name and description. Stepping skips the numbers whose
 * ability has no target range. It closes a details window it never opened,
 * as the item browser it was copied from does open one. */
void ItemLevel_SelectAbility(void)
{
    struct TextRenderWork *window;
    struct TextRenderWork *details;
    s32 ability;
    s32 redraw;
    s32 index;
    s32 count;
    s8 step;

    count = ABILITY_COUNT;
    step = 0;
    ability = 1;
    Engine_AudioPlayCue(112);
    redraw = 1;
    window = UiWindow_Create(0, 0, 30, 12, 2);
    Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010,
            (volatile u32 *)0x040000d4);
    Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001,
            (volatile u32 *)0x040000d4);
    Engine_TaskWait(1);
    for (;;) {
        if (redraw) {
            redraw = 0;
            ability = Engine_DebugRemainder(ability + ABILITY_COUNT, count);
            RenderOutput_RedrawSavedRect(window);
            Engine_DebugClearWindow(window);
            UiText_DrawStringInWindow(gItemLevelPsyPrompt, window, 0, 0);
            UiText_DrawNumberAtOffset(ability, 0, window, 80, 0);
            UiText_DrawStringInWindow(gItemLevelPsyHelp, window, 0, 72);
            index = ability & 0x3fff;
            UiText_DrawCharacterAtOffset(index + (s32)MsgAbilityName, window, 120, 0);
            index += (s32)MsgAbilityDescription;
            UiText_DrawCharacterAtOffset(index, window, 0, 24);
            UiText_DrawResource(index, window, 0, 48);
        }
        if (gKeyState & 2) {
            Engine_AudioPlayCue(113);
            goto done;
        }
        if (gKeysRepeat & 64) {
            step = -1;
            ability--;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 128) {
            step = 1;
            ability++;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 16) {
            step = 1;
            ability += 10;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 32) {
            step = -1;
            ability -= 10;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 256) {
            step = 1;
            ability += 30;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gKeysRepeat & 512) {
            step = -1;
            ability -= 30;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (step == -1) {
            ability = Engine_DebugRemainder(ability + ABILITY_COUNT, count);
            goto check_back;
        back:
            ability = Engine_DebugRemainder(ability + ABILITY_COUNT - 1, count);
        check_back:
            if (Engine_DebugGetAbility(ability & 0x3fff)[4] == 0)
                goto back;
        }
        if (step == 1) {
            ability = Engine_DebugRemainder(ability + ABILITY_COUNT, count);
            goto check_ahead;
        ahead:
            ability = Engine_DebugRemainder(ability + ABILITY_COUNT + 1, count);
        check_ahead:
            if (Engine_DebugGetAbility(ability & 0x3fff)[4] == 0)
                goto ahead;
        }
        step = 0;
        Engine_TaskWait(1);
    }
done:
    RenderOutput_RedrawSavedRect(window);
    Engine_TaskWait(1);
    UiWork_Finalize(window, 1);
    UiWork_Finalize(details, 1);
}
