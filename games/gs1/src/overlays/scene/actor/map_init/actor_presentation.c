#include "types.h"

#define SceneActor_SetActor14Field98ByFlag200 Func_020014ac
#define ActorPresentation_ConfigureActorTwentyAndFlag200 Func_0200170c
s32 Func_0200458e_a(s32);
s32 Func_0200458e_b(s32);
u8 *Func_020045d0(s32);
u8 *Func_020045dc(s32);
u8 *Func_020045ec(s32);
u8 *Func_020045f8(s32);
void Func_02004858(s32, s32);
void Func_02004878(s32, s32);
void Func_02004868(s32, s32);
u8 *Func_02004836(s32);
void Func_02004818(s32);

void SceneActor_SetActor14Field98ByFlag200(void)
{
    u8 *p;
    u8 val;

    Func_0200458e_a(0x201);
    if (Func_0200458e_b(0x200) != 0) {
        Func_020045d0(14)[98] = 0;
        Func_020045dc(14)[89] &= (u8)0xf7;
    } else {
        Func_020045ec(14)[98] = 1;
        p = Func_020045f8(14);
        p += 89;
        val = 8;
        val |= *p;
        *p = val;
    }
}

void ActorPresentation_ConfigureActorTwentyAndFlag200(void)
{
    u8 *flags;

    Func_02004858(20, 1);
    Func_02004878(20, 0);
    Func_02004868(20, 2);
    flags = Func_02004836(20) + 35;
    *flags &= 0xFD;
    Func_02004818(0x200);
}
