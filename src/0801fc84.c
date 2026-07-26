#include "types.h"

extern u8 Value_0000000a;
extern u8 Value_0000000d;
extern u8 Value_00000016;
extern u8 Value_00000018;

s32 Func_080056cc(void);
void Func_08005c68(void);
volatile u32 Func_08005cf8(void);
u32 Func_08005ac0(s32);
void Func_0801776c(s32, s32);
s32 Func_08020244(s16, s32);
s32 Func_08017658(s32, s32, s32, s32);
s32 Func_08017364(void);
s32 Func_080030f8(s32);
s32 Func_08028df4(s32, s32, s32, s32);
void Func_08019a54(void);

s32 Func_0801fc84(void)
{
    s32 found;
    s32 value;
    s32 result = 0;

    found = Func_080056cc();
    if (found != 0) {
        Func_0801776c((s32)&Value_0000000a, 1);
        result = -9;
    } else {
        Func_08005c68();
        value = Func_08020244(0, 3);
        if (value == -1) {
            result = value;
        } else {
            Func_08017658((s32)&Value_00000016, 8, 1, 2);
            while (Func_08017364() == 0) {
                Func_080030f8(1);
            }
            if (Func_08028df4(1, 0, 3, 1) != 0) {
                Func_08019a54();
            } else {
                Func_08019a54();
                found = Func_08005ac0(value);
                found |= Func_08005ac0(value + 3);
                if (found != 0) {
                    Func_0801776c((s32)&Value_0000000d, 1);
                    result = -4;
                } else {
                    Func_0801776c((s32)&Value_00000018, 1);
                }
            }
        }
    }
    Func_08005cf8();
    return result;
}
