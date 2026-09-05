#include "types.h"

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    u8 filler10[4];
    s32 unk14;
    u8 filler18[61];
    u8 unk55;
} Struct_1644;

extern Struct_1644 *Func_020064f8();
extern void Func_020064e6(void);
extern void Func_020064ec(s32);
extern void Func_02006662(s32);
extern void Func_020064ac(s32, s32, s32, s32, s32, s32);
extern void Func_02006530(void);

void SceneState_RunActor13AtColumn42Setup(void)
{
    Struct_1644 *obj;
    s32 val;
    s32 a;
    s32 b;

    obj = Func_020064f8(13);
    Func_020064e6();
    if (obj->unk8 >> 20 == 42) {
        Func_020064ec(30);
        Func_02006662(188);
        obj->unk55 = 0;
        val = 0xfffe0000;
        obj->unk14 = val;
        obj->unkC = val;
        Func_020064f8(0x200);
        a = 3;
        b = 5;
        Func_020064ac(44, 117, 41, 117, a, b);
    }
    Func_02006530();
}
