#include "TYPES.H"
#include "SCENE.H"
#include "RUNTIME_INTERFACES.H"
#include "TBS_EDITION.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"

/* save/state/confirm_and_process_selected_slot.c */
/* save/state/state_confirm_and_process_selected_slot.c */
/* save/state/confirm_and_process_selected_slot.c */
extern u8 Value_00000014;
extern u8 Value_00000017;

s32 UiWork_IsComplete(void);

void UiWork_FinalizePendingCore(void);
void Audio_PlayCue(s32);

s32 SaveState_ConfirmAndProcessSelectedSlot(void)
{
    s32 result;

    UiText_OpenMessageWindow((s32)&Value_00000014, 8, 12, 2);
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
            UiText_ShowPositionedMessageAndWait((s32)&Value_00000017, 1);
        }
    }
    return result;
}

/* save/state/copy_slot_pair.c */
extern u8 gSaveBuffer;
extern u8 MsgNoBackupMemory;
extern u8 Value_0000000c;
extern u8 Value_0000000d;
extern u8 Value_00000019;

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
                UiText_ShowPositionedMessageAndWait((s32)&Value_0000000c, 1);
                result = -2;
            } else {
                value = SaveState_FindFreeSummarySlot();
                if (value == 999) {
                    UiText_ShowPositionedMessageAndWait((s32)&Value_0000000d, 1);
                    result = -5;
                } else {
                    found = SaveState_WriteRecord(value, lower);
                    found |= SaveState_WriteRecord(value + 3, upper);
                    if (found != 0) {
                        UiText_ShowPositionedMessageAndWait((s32)&Value_0000000d, 1);
                        result = -3;
                    } else {
                        UiText_ShowPositionedMessageAndWait((s32)&Value_00000019, 1);
                    }
                }
            }
        }
    }

    SaveState_ReleaseWorkspace();
    return result;
}

/* save/state/delete_selected_slot.c */
extern u8 Value_00000016;
extern u8 Value_00000018;

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
            UiText_OpenMessageWindow((s32)&Value_00000016, 8, 1, 2);
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
                    UiText_ShowPositionedMessageAndWait((s32)&Value_0000000d, 1);
                    result = -4;
                } else {
                    UiText_ShowPositionedMessageAndWait((s32)&Value_00000018, 1);
                }
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return result;
}
