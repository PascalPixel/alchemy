#include "types.h"
#include "scene.h"
#include "runtime_interfaces.h"

/* save/write_selected_slot.c */
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
                while (UiWork_IsComplete() == 0) {
                    WaitFrames(1);
                }
                if (Sys_SetMode(1, 0, 0, 1) != 0) {
                    UiWork_FinalizePendingCore();
                    goto skip;
                }
                UiWork_FinalizePendingCore();
            }
            gOv = slot;
            Sys_Do(85);
            Sys_Apply((s32)Value_0000001a, 13);
            while (UiWork_IsComplete() == 0) {
                WaitFrames(1);
            }
            Sys_unk3_4();
            Sys_unk4();
            flag = Sys_Apply3(slot, gOv2);
            flag |= Sys_Apply3(slot + 3, gOv2 + 0x1000);
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

/* save/state/load_record_into_work.c */
struct State_080208e4 {
    u8 padding0[4];
    s32 value;
    u8 padding8[0x222];
    u8 flag;
};

extern char Value_0000000a;
extern char Value_0000000c;
extern char gOv;
extern s16 gOv2;
extern volatile struct State_080208e4 gCell;
extern s32 gIw;
extern volatile u8 gIw2;
extern s16 gIw3;

s32 SaveState_LoadRecordIntoWork(s32 arg)
{
    s32 ret = 0;
    s32 err = State_Check();

    if (err != 0) {
        State_Apply((s32)&Value_0000000a, 1);
        ret = -9;
    } else {
        s32 value;

        State_Run();
        value = State_Apply2(gOv2, arg);
        if (value == -1) {
            ret = value;
        } else {
            void *base = &gOv;

            err = State_Apply3(value, base);
            base = (char *)base + 0x1000;
            err |= State_Apply3(value + 3, base);
            if (err != 0) {
                State_Apply((s32)&Value_0000000c, 1);
                ret = -2;
            } else {
                gIw = gCell.value;
                {
                    volatile u8 *state = (volatile u8 *)&gCell;

                    gIw2 = state[0x22a];
                }
                gIw3 = 0;
                gOv2 = value;
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return ret;
}
