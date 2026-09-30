#include "TYPES.H"
#include "SCENE.H"
#include "RUNTIME_INTERFACES.H"
#include "SYSTEM.H"
s32 UiText_OpenMessageWindow(s32, s32, s32, s32);
s32 SaveMenu_SelectSlot(s16, s32);
s32 Menu_RunConfirmSelection(s32, s32, s32, s32);
s32 SaveState_ProcessSelectedSlot(void);
void UiText_ShowPositionedMessageAndWait(s32, s32);
s32 SaveState_InitializeWorkspace(void);
void SaveState_LoadSummaryRecords(void);
u32 SaveState_ReadRecordPayload(s32, void *);
u32 SaveState_FindFreeSummarySlot(void);
u32 SaveState_WriteRecord(s32, void *);
u32 SaveState_ReleaseWorkspace(void);
u32 SaveState_DeleteRecord(s32);

/* save/state/confirm_and_process_selected_slot.c */
/* save/state/state_confirm_and_process_selected_slot.c */
/* save/state/confirm_and_process_selected_slot.c */
extern u8 MsgOverwriteConfirm;
extern u8 MsgGameSaved;

s32 UiWork_IsComplete(void);

void UiWork_FinalizePendingCore(void);
void Audio_PlayCue(s32);

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
