#include "types.h"

#define ConfigureSceneActor11 Func_02001508
#define ConfigureSceneActor12 Func_02001548
#define ConfigureSceneActor13 Func_0200158c
#define ConfigureSceneActor14 Func_020015d0
u8 *Func_02004598(s32 arg0);
s32 Func_020045aa(s32 arg0);
void Func_02004560(s32 arg0, s32 arg1);
void Func_02004580(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_020045a0(s32 arg0);
u8 *Func_020045d8(s32 arg0);
s32 Func_020045ea(s32 arg0);
void Func_020045a0_a(s32 arg0, s32 arg1);
void Func_020045c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_020045de(s32 arg0);
u8 *Func_0200461c(s32 arg0);
s32 Func_0200462e(s32 arg0);
void Func_020045e4(s32 arg0, s32 arg1);
void Func_02004604(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02004622(s32 arg0);
u8 *Func_02004660(s32 arg0);
s32 Func_02004672(s32 arg0);
void Func_02004628(s32 arg0, s32 arg1);
void Func_02004648(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02004658(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02004676(s32 arg0);

void ConfigureSceneActor11(s32 actor_id)
{
    s32 a = 0x1300000;
    s32 b = 0x1700000;
    u8 *p = Func_02004598(11);

    if (p != 0) {
        p[89] = 0;
    }
    Func_02004560(Func_020045aa(actor_id), 0);
    Func_02004580(0, a, b, 253);
    Func_020045a0(576);
}

void ConfigureSceneActor12(s32 actor_id)
{
    s32 a = 0x500000;
    s32 b = 0x1700000;
    u8 *p = Func_020045d8(12);

    if (p != 0) {
        p[89] = 0;
    }
    Func_020045a0_a(Func_020045ea(actor_id), 0);
    Func_020045c0(0, a, b, 253);
    Func_020045de(577);
}

void ConfigureSceneActor13(s32 actor_id)
{
    s32 a = 0x600000;
    s32 b = 0x1500000;
    u8 *p = Func_0200461c(13);

    if (p != 0) {
        p[89] = 0;
    }
    Func_020045e4(Func_0200462e(actor_id), 0);
    Func_02004604(0, a, b, 253);
    Func_02004622(578);
}

void ConfigureSceneActor14(s32 actor_id)
{
    s32 a = 0x900000;
    s32 b = 0x1400000;
    s32 c = 0x2f00000;
    s32 d = 0x1400000;
    u8 *p = Func_02004660(14);

    if (p != 0) {
        p[89] = 0;
    }
    Func_02004628(Func_02004672(actor_id), 0);
    Func_02004648(0, a, b, 253);
    Func_02004658(0, c, d, 253);
    Func_02004676(579);
}
