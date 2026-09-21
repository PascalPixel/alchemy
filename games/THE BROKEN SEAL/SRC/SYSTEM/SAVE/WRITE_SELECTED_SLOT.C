#include "TYPES.H"

extern u8 Value_0000000a[];
extern u8 Value_0000000b[];
extern u8 Value_00000014[];
extern u8 Value_00000017[];
extern u8 Value_0000001a[];
extern s16 Data_02002004;
extern u8 Data_02000000[];
extern u8 *Data_03001f1c;

s32 Func_080056cc(void);
#define SaveState_InitializeWorkspace Func_080056cc
void Func_08005c68(void);
#define SaveState_LoadSummaryRecords Func_08005c68
s32 Func_08020244(s16 a, s32 b);
void Func_0801776c(s32 msg, s32 mode);
#define UiText_ShowPositionedMessageAndWait Func_0801776c
s32 Func_08017364(void);
#define UiWork_IsComplete Func_08017364
void WaitFrames(s32 frames);
s32 Func_08028df4(s32 a, s32 b, s32 c, s32 d);
#define Menu_RunConfirmSelection Func_08028df4
void Func_08019a54(void);
#define UiWork_FinalizePendingCore Func_08019a54
void Func_080f9010(u8 mode);
#define Audio_PlayCue Func_080f9010
void Func_0801f818(void);
void Func_0808a5b0(void);
s32 Func_08005920(s32 a, void *b);
#define SaveState_WriteRecord Func_08005920
void Func_08005cf8(void);
#define SaveState_ReleaseWorkspace Func_08005cf8

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
        slot = Func_08020244(Data_02002004, 0);
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
            Func_0801f818();
            Func_0808a5b0();
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
