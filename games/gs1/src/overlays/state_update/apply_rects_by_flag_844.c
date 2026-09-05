#include "types.h"

extern s32 Func_02002a2c(s32 flag);
extern void Func_02001884(void);
extern void Func_02002992(s32 frames);
extern s32 Func_02002a40(s32 flag);
extern void Func_02002a20(s32, s32, s32, s32, s32, s32);
extern void Func_02002a34(s32, s32, s32, s32, s32, s32);
extern void Func_02002a4e(s32, s32, s32, s32, s32, s32);
extern void Func_02002a60(s32, s32, s32, s32, s32, s32);
extern void Func_02002a6a(s32, s32, s32, s32, s32, s32);
extern void Func_02002a88(s32, s32, s32, s32, s32, s32);
void SceneState_ApplyRectsByFlag844(s32 flag)
{
    if (flag != 0 && Func_02002a2c(0x109) == 0)
        Func_02001884();

    Func_02002992(1);
    if (Func_02002a40(0x844) != 0) {
        s32 w1 = 10;
        Func_02002a20(121, 34, 3, 1, 93, w1);
        {
            s32 w2 = 30;
            Func_02002a34(46, 38, 1, 1, w2, 43);
            Func_02002a4e(0, 0, 1, 2, w2, 9);
        }
        Func_02002a60(26, 3, 1, 2, w1, 8);
        Func_02002a6a(26, 35, 1, 4, w1, 40);
    } else {
        s32 w1 = 10;
        s32 w2 = 8;
        Func_02002a88(11, 8, 1, 2, w1, w2);
    }
}
