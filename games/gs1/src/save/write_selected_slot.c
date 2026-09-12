#include "types.h"
#include "scene.h"
#include "runtime_interfaces.h"

/* save/write_selected_slot.c */
extern u8 Value_0000000a[];
extern u8 Value_0000000b[];
extern u8 Value_00000014[];
extern u8 Value_00000017[];
extern u8 Value_0000001a[];
extern s16 RomBytes_02002004;
extern u8 RomBytes_02000000[];
extern u8 *gIw;

void WaitFrames(s32);

s32 Save_WriteSelectedSlot(void)
{
    s32 result;
    s32 slot;
    s32 flag;
    u8 *base;

    result = 0;
    flag = Sys_Check();
    if (flag != 0) {
        Sys_Apply((s32)Value_0000000a, 1);
        result = -9;
    } else {
        Sys_Run();
        base = gIw;
        slot = Sys_Apply2(RomBytes_02002004, 0);
        if (slot == -1) {
            result = slot;
        } else {
            s32 off = (slot << 6) + 0x105c;
            if (base[off] != 0) {
                Sys_Apply((s32)Value_00000014, 13);
                while (UiWork_IsComplete() == 0) {
                    WaitFrames(1);
                }
                if (Sys_SetMode(1, 0, 0, 1) != 0) {
                    UiWork_FinalizePendingCore();
                    goto skip;
                }
                UiWork_FinalizePendingCore();
            }
            RomBytes_02002004 = slot;
            Audio_PlayCue(85);
            Sys_Apply((s32)Value_0000001a, 13);
            while (UiWork_IsComplete() == 0) {
                WaitFrames(1);
            }
            Save_CapturePartySnapshot();
            Save_CaptureObjectTable();
            flag = Sys_Apply3(slot, RomBytes_02000000);
            flag |= Sys_Apply3(slot + 3, RomBytes_02000000 + 0x1000);
            UiWork_FinalizePendingCore();
            if (flag != 0) {
                Sys_Apply((s32)Value_0000000b, 1);
                result = -3;
            } else {
                Sys_Apply((s32)Value_00000017, 9);
            }
        }
    }
skip:
    SaveState_ReleaseWorkspace();
    return result;
}
