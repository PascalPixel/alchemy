/* The item and level debug room: its scene tables, actor 13's steps and the
 * record counts it adjusts. */
#include "LEVEL.H"
#include "CALL.H"
#include "PARTY_STATE.H"

extern u8 MsgDebugWontStopYou[];

extern u8 MsgDebugRaiseEveryonesLevel[];
extern u8 gKeyState[];

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

    record = Owner_GetState(gGameState.current_owner);
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

        key = (volatile u32 *)gKeyState;

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
