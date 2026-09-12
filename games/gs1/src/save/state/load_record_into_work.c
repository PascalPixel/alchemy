#include "types.h"
#include "runtime_interfaces.h"

struct State_080208e4 {
    u8 padding0[4];
    s32 value;
    u8 padding8[0x222];
    u8 flag;
};

s32 Func_080056cc(void);
void Func_08005c68(void);
void Func_0801776c(s32, s32);
s32 Func_08020244(s16, s32);
s32 Func_08005a78(s32, void *);

extern char Value_0000000a;
extern char Value_0000000c;
extern char Data_02000000;
extern s16 Data_02002004;
extern volatile struct State_080208e4 Data_02000240;
extern s32 Data_03001c9c;
extern volatile u8 Data_03001d08;
extern s16 Data_03001d24;

s32 SaveState_LoadRecordIntoWork(s32 arg)
{
    s32 ret = 0;
    s32 err = Func_080056cc();

    if (err != 0) {
        Func_0801776c((s32)&Value_0000000a, 1);
        ret = -9;
    } else {
        s32 value;

        Func_08005c68();
        value = Func_08020244(Data_02002004, arg);
        if (value == -1) {
            ret = value;
        } else {
            void *base = &Data_02000000;

            err = Func_08005a78(value, base);
            base = (char *)base + 0x1000;
            err |= Func_08005a78(value + 3, base);
            if (err != 0) {
                Func_0801776c((s32)&Value_0000000c, 1);
                ret = -2;
            } else {
                Data_03001c9c = Data_02000240.value;
                {
                    volatile u8 *state = (volatile u8 *)&Data_02000240;

                    Data_03001d08 = state[0x22a];
                }
                Data_03001d24 = 0;
                Data_02002004 = value;
            }
        }
    }
    Func_08005cf8();
    return ret;
}
