#include "TYPES.H"
#include "SYSTEM.H"

extern u8 Value_0000000a[];
extern u8 Value_0000000b[];
extern u8 Value_00000014[];
extern u8 Value_00000017[];
extern u8 Value_0000001a[];
extern s16 Data_02002004;
extern u8 Data_02000000[];
extern u8 *Data_03001f1c;

s32 SaveState_InitializeWorkspace(void);
void SaveState_LoadSummaryRecords(void);
s32 SaveMenu_SelectSlot(s16 a, s32 b);
void UiText_ShowPositionedMessageAndWait(s32 msg, s32 mode);
s32 UiWork_IsComplete(void);
s32 Menu_RunConfirmSelection(s32 a, s32 b, s32 c, s32 d);
void UiWork_FinalizePendingCore(void);
void Audio_PlayCue(u8 mode);
void SaveState_BuildSummaryHeader(void);
void SaveState_CaptureObjectTableFar(void);
s32 SaveState_WriteRecord(s32 a, void *b);
void SaveState_ReleaseWorkspace(void);

s32 Save_WriteSelectedSlot(void)
{
    s32 result;
    s32 slot;
    s32 flag;
    u8 *base;

    result = 0;
    flag = SaveState_InitializeWorkspace();
    if (flag != 0) {
        UiText_ShowPositionedMessageAndWait((s32)Value_0000000a, 1);
        result = -9;
    } else {
        SaveState_LoadSummaryRecords();
        base = Data_03001f1c;
        slot = SaveMenu_SelectSlot(Data_02002004, 0);
        if (slot == -1) {
            result = slot;
        } else {
            s32 off = (slot << 6) + 0x105c;
            if (base[off] != 0) {
                UiText_ShowPositionedMessageAndWait((s32)Value_00000014, 13);
                while (UiWork_IsComplete() == 0) {
                    WaitFrames(1);
                }
                if (Menu_RunConfirmSelection(1, 0, 0, 1) != 0) {
                    UiWork_FinalizePendingCore();
                    goto skip;
                }
                UiWork_FinalizePendingCore();
            }
            Data_02002004 = slot;
            Audio_PlayCue(85);
            UiText_ShowPositionedMessageAndWait((s32)Value_0000001a, 13);
            while (UiWork_IsComplete() == 0) {
                WaitFrames(1);
            }
            SaveState_BuildSummaryHeader();
            SaveState_CaptureObjectTableFar();
            flag = SaveState_WriteRecord(slot, Data_02000000);
            flag |= SaveState_WriteRecord(slot + 3, Data_02000000 + 0x1000);
            UiWork_FinalizePendingCore();
            if (flag != 0) {
                UiText_ShowPositionedMessageAndWait((s32)Value_0000000b, 1);
                result = -3;
            } else {
                UiText_ShowPositionedMessageAndWait((s32)Value_00000017, 9);
            }
        }
    }
skip:
    SaveState_ReleaseWorkspace();
    return result;
}
