/* SaveState_ProcessSelectedSlot: rewrite the selected save slot with the
   current save stamp, reporting a missing backup chip or a failed read or
   write as a negative code. */
#include "TYPES.H"
#include "SAVE_STATE.H"
#include "IWRAM_CALL.H"
#include "RUNTIME_MEM.H"
#include "SCENE.H"
#include "RUNTIME_INTERFACES.H"
#include "TBS_EDITION.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);
extern s16 gSaveSlot;
extern u8 gSaveBuffer[];
extern u8 gSaveStamp[];
extern u8 MsgNoBackupMemory;
extern u8 MsgSaveFailed;
void UiText_ShowPositionedMessageAndWait(s32 message, s32 mode);

s32 UiText_OpenMessageWindow(s32, s32, s32, s32);
s32 SaveMenu_SelectSlot(s16, s32);
s32 Menu_RunConfirmSelection(s32, s32, s32, s32);
void UiText_ShowPositionedMessageAndWait(s32, s32);

/* save/state/confirm_and_process_selected_slot.c */
extern u8 MsgOverwriteConfirm;
extern u8 MsgGameSaved;
s32 UiWork_IsComplete(void);
void UiWork_FinalizePendingCore(void);
void Audio_PlayCue(s32);
extern u8 MsgLoadFailed;
extern u8 MsgEraseFailed;
extern u8 MsgFileCopied;

/* save/state/delete_selected_slot.c */
extern u8 MsgEraseConfirm;
extern u8 MsgFileErased;

s32 SaveState_ProcessSelectedSlot(void)
{
    void *buffer;
    s32 value;
    s32 result;
    s32 found;
    /* FAKEMATCH: keeps the error code in r7 */
    register s32 error asm("r7");

    buffer = Runtime_BumpAllocateAlternatePool(0x1000);
    result = 0;
    value = gSaveSlot;
    if (value != -1) {
        found = SaveState_InitializeWorkspace();
        if (found != 0) {
            error = 9;
            UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
            goto negate;

        } else {
            u8 *dst;

            found = SaveState_ReadRecordPayload(gSaveSlot, buffer);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)&MsgSaveFailed, 1);
                result = -2;
            }
            dst = (u8 *)buffer + (s32)gSaveStamp;
            dst -= (s32)gSaveBuffer;
            ((WordCopyFn)(IwramIrqMain + Iwram_CopyWordsOffset))(dst, gSaveStamp, 16);
            found = SaveState_WriteRecord(gSaveSlot, buffer);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)&MsgSaveFailed, 1);
                error = 3;
negate:
                result = -error;
            }
        }
        SaveState_ReleaseWorkspace();
        Sys_Free(buffer);
        value = result;
    }
    return value;
}

/* save/state/copy_slot_pair.c */
/* save/state/confirm_and_process_selected_slot.c */
/* save/state/state_confirm_and_process_selected_slot.c */
s32 SaveState_ConfirmAndProcessSelectedSlot(void)
{
    s32 result;

    UiText_OpenMessageWindow((s32)&MsgOverwriteConfirm, 8, 12, 2);
    while (UiWork_IsComplete() == 0) {
        WaitFrames(1);
    }
    if (Menu_RunConfirmSelection(1, 0, 0, 1)) {
        UiWork_FinalizePendingCore();
    } else {
        UiWork_FinalizePendingCore();
        Audio_PlayCue(85);
        result = SaveState_ProcessSelectedSlot();
        if (result >= 0) {
            UiText_ShowPositionedMessageAndWait((s32)&MsgGameSaved, 1);
        }
    }
    return result;
}

s32 SaveState_CopySlotPair(void)
{
    u32 found;
    s32 value;
    s32 result;

    result = 0;
    found = SaveState_InitializeWorkspace();
    if (found != 0) {
        UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
        result = -9;
    } else {
        SaveState_LoadSummaryRecords();
        value = SaveMenu_SelectSlot(0, 2);
        if (value == -1) {
            result = value;
        } else {
            void *lower = &gSaveBuffer;
            void *upper;

            found = SaveState_ReadRecordPayload(value, lower);
            upper = (u8 *)lower + 0x1000;
            found |= SaveState_ReadRecordPayload(value + 3, upper);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)&MsgLoadFailed, 1);
                result = -2;
            } else {
                value = SaveState_FindFreeSummarySlot();
                if (value == 999) {
                    UiText_ShowPositionedMessageAndWait((s32)&MsgEraseFailed, 1);
                    result = -5;
                } else {
                    found = SaveState_WriteRecord(value, lower);
                    found |= SaveState_WriteRecord(value + 3, upper);
                    if (found != 0) {
                        UiText_ShowPositionedMessageAndWait((s32)&MsgEraseFailed, 1);
                        result = -3;
                    } else {
                        UiText_ShowPositionedMessageAndWait((s32)&MsgFileCopied, 1);
                    }
                }
            }
        }
    }

    SaveState_ReleaseWorkspace();
    return result;
}

s32 SaveState_DeleteSelectedSlot(void)
{
    s32 found;
    s32 value;
    s32 result = 0;

    found = SaveState_InitializeWorkspace();
    if (found != 0) {
        UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
        result = -9;
    } else {
        SaveState_LoadSummaryRecords();
        value = SaveMenu_SelectSlot(0, 3);
        if (value == -1) {
            result = value;
        } else {
            UiText_OpenMessageWindow((s32)&MsgEraseConfirm, 8, 1, 2);
            while (UiWork_IsComplete() == 0) {
                WaitFrames(1);
            }
            if (Menu_RunConfirmSelection(1, 0, 3, 1) != 0) {
                UiWork_FinalizePendingCore();
            } else {
                UiWork_FinalizePendingCore();
                found = SaveState_DeleteRecord(value);
                found |= SaveState_DeleteRecord(value + 3);
                if (found != 0) {
                    UiText_ShowPositionedMessageAndWait((s32)&MsgEraseFailed, 1);
                    result = -4;
                } else {
                    UiText_ShowPositionedMessageAndWait((s32)&MsgFileErased, 1);
                }
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return result;
}
