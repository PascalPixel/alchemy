#include "types.h"

extern u8 Data_02000000;
extern u8 Value_0000000a;
extern u8 Value_0000000c;
extern u8 Value_0000000d;
extern u8 Value_00000019;

void Func_08005c68(void);
volatile u32 Func_08005cf8(void);
s32 Func_080056cc(void);
s32 Func_08020244(s16, s32);
u32 Func_08005a78(s32, void *);
u32 Func_0801f704(void);
u32 Func_08005920(s32, void *);
void Func_0801776c(s32, s32);

s32 Func_0801fba8(void)
{
    u32 found;
    s32 value;
    s32 result;

    result = 0;
    found = Func_080056cc();
    if (found != 0) {
        Func_0801776c((s32)&Value_0000000a, 1);
        result = -9;
    } else {
        Func_08005c68();
        value = Func_08020244(0, 2);
        if (value == -1) {
            result = value;
        } else {
            void *lower = &Data_02000000;
            void *upper;

            found = Func_08005a78(value, lower);
            upper = (u8 *)lower + 0x1000;
            found |= Func_08005a78(value + 3, upper);
            if (found != 0) {
                Func_0801776c((s32)&Value_0000000c, 1);
                result = -2;
            } else {
                value = Func_0801f704();
                if (value == 999) {
                    Func_0801776c((s32)&Value_0000000d, 1);
                    result = -5;
                } else {
                    found = Func_08005920(value, lower);
                    found |= Func_08005920(value + 3, upper);
                    if (found != 0) {
                        Func_0801776c((s32)&Value_0000000d, 1);
                        result = -3;
                    } else {
                        Func_0801776c((s32)&Value_00000019, 1);
                    }
                }
            }
        }
    }

    Func_08005cf8();
    return result;
}
