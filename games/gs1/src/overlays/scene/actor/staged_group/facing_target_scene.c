#include "types.h"

#define SceneState_BranchOnSlotZeroFacing Func_02000394
#define FieldScene_RunActorTwentyFourAngleDialogue Func_02000420
struct Obj {
    u8 filler00[6];
    u16 f06;
};

struct Obj *Func_02003fb6(s32);
s32 Func_02004126(s32, s32);
void Func_02003fac(void);
void Func_0200407a(s32);
void Func_02004092(s32, s32);
void Func_02003fc6(void);
struct Obj *Func_02004042(s32);
s32 Func_020041b2(s32, s32);
void Func_02004038(void);
void Func_02004106(s32);
void Func_0200411e(s32, s32);
void Func_02004052(void);

void SceneState_BranchOnSlotZeroFacing(void)
{
    struct Obj *o;
    u32 v;

    o = Func_02003fb6(0);
    v = (o->f06 - 0x2000) << 16;
    if (v > 0x80000000) {
        Func_02004126(22, 22);
    } else {
        Func_02003fac();
        Func_0200407a(0x1acf);
        Func_02004092(22, 0);
        Func_02003fc6();
    }
}

void FieldScene_RunActorTwentyFourAngleDialogue(void)
{
    struct Obj *o;
    u32 v;

    o = Func_02004042(0);
    v = (o->f06 - 0x2000) << 16;
    if (v > 0xC0000000) {
        Func_020041b2(24, 24);
    } else {
        Func_02004038();
        Func_02004106(0x1ad5);
        Func_0200411e(24, 0);
        Func_02004052();
    }
}
