/* The item and level debug room's level panel: a 30x9 window with three
 * caption lines and the current owner's name and level, looping on the
 * button latch until B closes it. Select and Start raise every listed owner
 * by five and A by one. */
#include "LEVEL.H"
#include "PARTY_STATE.H"
extern u8 MsgDebugRaiseEveryonesLevel[];

extern u8 gKeyState[];

void FieldScene_RunCountAdjustPanel(void)
{
    u8 *record;
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
