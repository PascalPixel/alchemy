/* Draft of FieldScene_RunCountAdjustPanel, resource_3cd at 0x020080ec (was
 * part of DEBUG/ITEM_LEVEL/RECORD_COUNT.C).
 * Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines (UiText_DrawResource,
 * RenderOutput_RedrawSavedRect, UiText_DrawStringAtOffset,
 * UiText_DrawNumberInWindow, Engine_AudioPlayCue, Engine_TaskWait, ...).
 * The listing keeps these rows. */
#include "../../../../../games/THE BROKEN SEAL/SRC/DEBUG/ITEM_LEVEL/LEVEL.H"
extern u8 MsgDebugRaiseEveryonesLevel[];

extern s32 gGameStateWords[];
extern u8 gItemLevelIcon[];
extern u8 gKeyState[];
void Engine_TaskWait();
void UiWork_Finalize();
void UiText_DrawResource();
void UiText_DrawStringAtOffset();
void UiText_DrawNumberInWindow();
void RenderOutput_RedrawSavedRect();
void Engine_AudioPlayCue();

/*
 * An interactive panel: open a 30x9 window, draw three caption lines plus the
 * current record's icon and count, then loop on the button latch until B
 * closes it.  Up and down (Select and Start) take five from every listed
 * record, A adds one.
 */
void FieldScene_RunCountAdjustPanel(void)
{
    u8 *record;
    s32 *work;
    volatile u32 *key;
    s32 win;
    s32 flag;
    s32 msg;

    work = gGameStateWords;
    record = Owner_GetState(work[125]);
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
            UiText_DrawStringInWindow(gItemLevelIcon, win, 48, 48);
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
