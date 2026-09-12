#include "types.h"

#define SceneDialogue_RunActorTwentyThreeByLeaderHeading Func_020003d8
#define SceneActor_SetActorZeroByte35Bit0 Func_020015e0
struct Obj {
    u8 filler00[6];
    u16 f06;
};

struct Obj *Func_02003ffa(s32);
s32 Func_02004168(s32, s32);
void Func_02003fee(void);
void Func_020040bc(s32);
s32 Func_020040e4(s32, s32);
void Func_02004008(void);
s32 Func_02005202(s32);

void SceneDialogue_RunActorTwentyThreeByLeaderHeading(void)
{
    struct Obj *o;
    u32 v;

    o = Func_02003ffa(0);
    v = (o->f06 - 0x6001) << 16;
    if (v <= 0x7ffe0000) {
        Func_02004168(23, 23);
    } else {
        Func_02003fee();
        Func_020040bc(0x1ad1);
        Func_020040e4(23, 0);
        Func_02004008();
    }
}

void SceneActor_SetActorZeroByte35Bit0(void)
{
    u8 *p;
    s32 v;

    p = (u8 *)(Func_02005202(0) + 35);
    v = 1;
    v |= *p;
    *p = v;
}
