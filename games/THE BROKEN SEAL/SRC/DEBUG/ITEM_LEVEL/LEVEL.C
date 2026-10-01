/* The item and level debug room: its scene tables, actor 13's steps and the
 * record counts it adjusts. */
#include "LEVEL.H"
#include "CALL.H"
#include "PARTY_STATE.H"
#include "TYPES.H"
#include "INVENTORY.H"
#include "DMA.H"
#include "TEXT_RENDER_RUNTIME.H"
#include "ITEM.H"

extern u8 MsgDebugWontStopYou[];
extern u8 MsgDebugRaiseEveryonesLevel[];
extern volatile u32 gKeyState;

extern u8 MsgItemName[];
extern u8 MsgItemPlainName[];
struct ItemDefinition *Engine_DebugGetItem(s32 item);
void Engine_AudioPlayCue(s32 cue);
void Engine_TaskWait(s32 frames);
extern u8 gItemLevelItemPrompt[];
extern u8 gItemLevelItemHelp[];
extern u8 gItemLevelItemFull[];
extern volatile u32 gKeysRepeat;
void UiWork_Finalize(struct TextRenderWork *window, s32 mode);
void RenderOutput_RedrawSavedRect(struct TextRenderWork *window);
void Engine_DebugClearWindow(struct TextRenderWork *window);
void UiText_DrawStringInWindow(u8 *text, struct TextRenderWork *window, s32 x, s32 y);
void UiText_DrawNumberAtOffset(s32 value, s32 format, struct TextRenderWork *window, s32 x, s32 y);
void UiText_DrawCharacterAtOffset(s32 text, struct TextRenderWork *window, s32 x, s32 y);
void Engine_DebugDrawItemDetails(struct TextRenderWork *window, s32 item);
#define ITEM_COUNT 270
extern u8 MsgAbilityName[];
extern u8 MsgAbilityDescription[];
u8 *Engine_DebugGetAbility(s32 ability);
extern u8 gItemLevelPsyPrompt[];
extern u8 gItemLevelPsyHelp[];
void UiText_DrawResource(s32 text, struct TextRenderWork *window, s32 x, s32 y);
#define ABILITY_COUNT 270

u8 *ItemLevel_GetEntrances(void)
{
    return gItemLevelEntrances;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *ItemLevel_GetExits(void)
{
    return gItemLevelExits;
}

u8 *ItemLevel_GetPlacements(void)
{
    return gItemLevelPlacements;
}

void FieldScene_RunActor13Mode102Step(void)
{
    Engine_EventSetMessage((s32)MsgDebugWontStopYou);
    Call3(Engine_ActorShowEmote, 13, 0x102, 0);
    Engine_EventShowMessage(13, 0);
}

void FieldScene_RunActor13Mode105Step(void)
{
    Engine_ActorShowEmote(13, 0x105, 0);
    Engine_EventSetMessage((s32)MsgDebugWontStopYou);
    Engine_EventShowMessage(13, 0);
}

u8 *ItemLevel_GetEvents(void)
{
    return gItemLevelEvents;
}

void SceneState_AddToRecordCount(s32 id, s32 amount)
{
    u8 *entry = Owner_GetState(id);

    Party_AdvanceOwnerCountToTarget(id, entry[15] + amount);
    Owner_RecalculateStats(id);
}

void Scene_AddToListedRecordCounts(s32 amount)
{
    u16 list[16];
    u16 *p;
    s32 n;

    n = Value1((s32 (*)(u16 *))Party_ListActiveOwners, (s32)(list));
    if (n > 0) {
        s32 count;
        p = list;
        count = n;
        do {
            SceneState_AddToRecordCount(*p++, amount);
        } while (--count != 0);
    }
}

/* The item and level debug room's level panel: a 30x9 window with three
 * caption lines and the current owner's name and level, looping on the
 * button latch until B closes it. Select and Start raise every listed owner
 * by five and A by one. */
void FieldScene_RunCountAdjustPanel(void)
{
    u8 *record;
    /* FAKEMATCH: reading the held keys through a volatile pointer loads
     * them once per test, where the reference does not merge the tests. */
    volatile u32 *key;
    s32 win;
    s32 flag;
    s32 msg;

    record = Owner_GetState(gGameState.selected_actor);
    win = UiWindow_Create(0, 0, 30, 9, 2);

    msg = (s32)MsgDebugRaiseEveryonesLevel;
    UiText_DrawResource(msg, win, 0, 0);
    UiText_DrawResource(msg + 1, win, 0, 16);
    msg += 2;
    flag = 1;
    UiText_DrawResource(msg, win, 0, 32);

    for (;;) {
        if (flag != 0) {
            RenderOutput_RedrawSavedRect(win);
            UiText_DrawStringAtOffset(record, win, 0, 48);
            UiText_DrawStringInWindow(gItemLevelLvLabel, win, 48, 48);
            flag = 0;
            UiText_DrawNumberInWindow(record[15], 0, win, 72, 48);
        }

        key = &gKeyState;

        if ((*key & 8) != 0 || (*key & 4) != 0) {
            Scene_AddToListedRecordCounts(5);
            Engine_AudioPlayCue(93);
            flag = 1;
        }

        if ((*key & 1) != 0) {
            Scene_AddToListedRecordCounts(1);
            Engine_AudioPlayCue(91);
            flag = 1;
        }

        if ((*key & 2) != 0) {
            Engine_AudioPlayCue(113);
            RenderOutput_RedrawSavedRect(win);
            Engine_TaskWait(1);
            UiWork_Finalize(win, 1);

            /* The refresh order 0, 1, 3, 2 is deliberate; do not sort it. */
            Owner_RecalculateStats(0);
            Owner_RecalculateStats(1);
            Owner_RecalculateStats(3);
            Owner_RecalculateStats(2);
            return;
        }

        Engine_TaskWait(1);
    }
}

/* The item and level debug room's item browser: pick an item number, see its
 * name and details, and A adds it to the party's bag. Stepping skips the
 * numbers with no item icon. */
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
            item = (item + ITEM_COUNT) % count;
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
            item = (item + ITEM_COUNT) % count;
            goto check_back;
        back:
            item = (item + ITEM_COUNT - 1) % count;
        check_back:
            if (Engine_DebugGetItem(item & 0x1ff)->icon == 0)
                goto back;
        }
        if (step == 1) {
            item = (item + ITEM_COUNT) % count;
            goto check_ahead;
        ahead:
            item = (item + ITEM_COUNT + 1) % count;
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
            ability = (ability + ABILITY_COUNT) % count;
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
            ability = (ability + ABILITY_COUNT) % count;
            goto check_back;
        back:
            ability = (ability + ABILITY_COUNT - 1) % count;
        check_back:
            if (Engine_DebugGetAbility(ability & 0x3fff)[4] == 0)
                goto back;
        }
        if (step == 1) {
            ability = (ability + ABILITY_COUNT) % count;
            goto check_ahead;
        ahead:
            ability = (ability + ABILITY_COUNT + 1) % count;
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

/* The item and level debug room: its entry setup, the glyph caption window
 * and the far debug browsers. */
void SceneState_SetWorkWords1c0And1c8(void)
{
    *(s32 *)((*(u8 **)&gEventWork) + 0x1c0) = 0x201;
    *(s32 *)((*(u8 **)&gEventWork) + 0x1c8) = 24;
    Engine_EventRequestExit();
}

/* The overlay's entry driver, the target of the first entry veneer. Actor
 * 11 is fetched twice, once per store, and must not be folded into one
 * local. */
s32 FieldScene_RunEntrySetup(void)
{
    *(s32 *)((*(u8 **)&gEventWork) + 448) = 516;
    *(s32 *)((*(u8 **)&gEventWork) + 456) = 24;
    *(s32 *)(Object_GetById(11) + 28) = 0x19999;
    *(s32 *)(Object_GetById(11) + 24) = 0x19999;
    Engine_ActorSetAnimation(13, 5);
    Engine_ActorSetAnimation(14, 2);
    return 0;
}

void FieldScene_DrawThreeCaptionWindow(void)
{
    /*
     * The frame is 36 bytes: 4 for the stacked fifth argument, plus a 32-byte
     * local that no instruction reads or writes.  Only its size is known, not
     * its element type, so the declaration must stay at 32 bytes.
     */
    u8 buf[32];
    s32 handle = UiWindow_Create(0, 13, 30, 6, 2);

    UiText_DrawStringInWindow(gItemLevelGlyphsUpper, handle, 0, 0);
    UiText_DrawStringInWindow(gItemLevelGlyphsLower, handle, 0, 8);
    UiText_DrawStringInWindow(gItemLevelGlyphsMarks, handle, 0, 16);
}

/* Set the flag byte at +53 of the record the effect work pointer holds. */
void SceneState_SetRecordFlag53(void)
{
    u8 *record = *(u8 **)gEffectWork;

    record[53] = 1;
}

/*
 * The return address is popped into r1, not r0, so r0 is live at return and
 * the wrapper hands its callee's result back; declaring the pair void would
 * compile pop {r0} / bx r0 instead.
 */
int SceneState_GetFarResult100c(void)
{
    return DebugMenu_BrowseIcons();
}

int SceneState_GetFarResult1020(void)
{
    return DebugMenu_BrowseEntryGlyphs();
}
