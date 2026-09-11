#include "types.h"
#include "scene.h"
#include "abi/save/state/load_record_into_work.h"
#include "runtime_interfaces.h"

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
    State_Run2();
    return ret;
}
