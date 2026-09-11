#include "types.h"
#include "scene.h"
#include "abi/save/write_selected_slot.h"

extern u8 Value_0000000a[];
extern u8 Value_0000000b[];
extern u8 Value_00000014[];
extern u8 Value_00000017[];
extern u8 Value_0000001a[];
extern s16 gOv;
extern u8 gOv2[];
extern u8 *gIw;

void WaitFrames(s32 frames);

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
        slot = Sys_Apply2(gOv, 0);
        if (slot == -1) {
            result = slot;
        } else {
            s32 off = (slot << 6) + 0x105c;
            if (base[off] != 0) {
                Sys_Apply((s32)Value_00000014, 13);
                while (Sys_Check2() == 0) {
                    WaitFrames(1);
                }
                if (Sys_SetMode(1, 0, 0, 1) != 0) {
                    Sys_Run2();
                    goto skip;
                }
                Sys_Run2();
            }
            gOv = slot;
            Sys_Do(85);
            Sys_Apply((s32)Value_0000001a, 13);
            while (Sys_Check2() == 0) {
                WaitFrames(1);
            }
            Sys_Run3();
            Sys_Run4();
            flag = Sys_Apply3(slot, gOv2);
            flag |= Sys_Apply3(slot + 3, gOv2 + 0x1000);
            Sys_Run2();
            if (flag != 0) {
                Sys_Apply((s32)Value_0000000b, 1);
                result = -3;
            } else {
                Sys_Apply((s32)Value_00000017, 9);
            }
        }
    }
skip:
    Sys_Run5();
    return result;
}
