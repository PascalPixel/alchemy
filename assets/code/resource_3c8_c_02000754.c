#include "types.h"

struct EffectObject {
    u8 filler00[0x30];
    s32 color;
    u8 filler34[0x38];
    s32 callback;
};

extern struct EffectObject *Func_0200560a(s32);
extern void Func_020055f8(void);
extern void Func_02005766(s32);
extern void Func_02005676(s32, s32);
extern void Func_0200566a(s32, s32, s32);
extern void Func_02005678(s32);
extern void Func_020056c8(s32, s32);
extern s32 Func_02005646(s32);
extern void Func_020055e4(s32, s32);
extern void Func_02005636(s32);
extern void Func_0200577a(void);
extern void Func_02005786(void);
extern void Func_02005764(s16);
extern void Func_02005660(void);

void Func_02000754(void)
{
    struct EffectObject *object;
    u8 *state;

    state = *(u8 **)0x03001ebc;
    object = Func_0200560a(0);
    Func_020055f8();
    Func_02005766(228);
    object->callback = 0x020086a1;
    object->color = 0x3333;
    Func_02005676(0, 2);
    Func_0200566a(0, 0, -6);
    Func_02005678(0);
    Func_020056c8(0, 15);
    Func_020055e4(Func_02005646(0), 0);
    object->callback = 0;
    Func_02005636(30);
    Func_0200577a();
    Func_02005786();
    Func_02005764(*(s16 *)(state + 0x16c));
    Func_02005660();
}
