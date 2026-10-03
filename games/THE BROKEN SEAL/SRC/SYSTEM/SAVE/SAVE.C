#include "TYPES.H"
#include "SYSTEM.H"
#include "SAVE_STATE.H"
#include "GAME_STATE.H"
#include "RUNTIME_INTERFACES.H"

extern u8 MsgNoBackupMemory[];
extern u8 MsgSaveFailed[];
extern u8 MsgOverwriteConfirm[];
extern u8 MsgGameSaved[];
extern u8 MsgSaving[];
extern s16 gSaveSlot;
extern u8 gSaveBuffer[];
s32 SaveMenu_SelectSlot(s16 a, s32 b);
void UiText_ShowPositionedMessageAndWait(s32 msg, s32 mode);
s32 UiWork_IsComplete(void);
s32 Menu_RunConfirmSelection(s32 a, s32 b, s32 c, s32 d);
void UiWork_FinalizePendingCore(void);
void Audio_PlayCue(u8 mode);
void SaveState_CaptureObjectTableFar(void);

extern char MsgLoadFailed;
extern s32 gLoadedStateWord;
extern u8 gOptionMirror;
extern s16 gPostLoadCounter;

s32 Save_WriteSelectedSlot(void)
{
    s32 result;
    s32 slot;
    s32 flag;
    struct SaveWorkspace *work;

    result = 0;
    flag = SaveState_InitializeWorkspace();
    if (flag != 0) {
        UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
        result = -9;
    } else {
        SaveState_LoadSummaryRecords();
        work = gSaveWorkspace;
        slot = SaveMenu_SelectSlot(gSaveSlot, 0);
        if (slot == -1) {
            result = slot;
        } else {
            if (work->summary[slot].level != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgOverwriteConfirm, 13);
                while (UiWork_IsComplete() == 0) {
                    WaitFrames(1);
                }
                if (Menu_RunConfirmSelection(1, 0, 0, 1) != 0) {
                    UiWork_FinalizePendingCore();
                    goto skip;
                }
                UiWork_FinalizePendingCore();
            }
            gSaveSlot = slot;
            Audio_PlayCue(85);
            UiText_ShowPositionedMessageAndWait((s32)MsgSaving, 13);
            while (UiWork_IsComplete() == 0) {
                WaitFrames(1);
            }
            SaveState_BuildSummaryHeader();
            SaveState_CaptureObjectTableFar();
            flag = SaveState_WriteRecord(slot, gSaveBuffer);
            flag |= SaveState_WriteRecord(slot + 3, gSaveBuffer + 0x1000);
            UiWork_FinalizePendingCore();
            if (flag != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
                result = -3;
            } else {
                UiText_ShowPositionedMessageAndWait((s32)MsgGameSaved, 9);
            }
        }
    }
skip:
    SaveState_ReleaseWorkspace();
    return result;
}

s32 SaveState_LoadRecordIntoWork(s32 arg)
{
    s32 ret = 0;
    s32 err = SaveState_InitializeWorkspace();

    if (err != 0) {
        UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
        ret = -9;
    } else {
        s32 value;

        SaveState_LoadSummaryRecords();
        value = SaveMenu_SelectSlot(gSaveSlot, arg);
        if (value == -1) {
            ret = value;
        } else {
            void *base = &gSaveBuffer;

            err = SaveState_ReadRecordPayload(value, base);
            base = (char *)base + 0x1000;
            err |= SaveState_ReadRecordPayload(value + 3, base);
            if (err != 0) {
                UiText_ShowPositionedMessageAndWait((s32)&MsgLoadFailed, 1);
                ret = -2;
            } else {
                gLoadedStateWord = gGameState.play_time;
                gOptionMirror = gGameState.saved_options;
                gPostLoadCounter = 0;
                gSaveSlot = value;
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return ret;
}
