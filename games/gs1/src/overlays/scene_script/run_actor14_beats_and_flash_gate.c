#include "types.h"

u8 *Func_02001522(s32);
void Func_02001522_a();
void Func_02001506(s32);
void Func_020015fe();
void Func_020015ac(s32);
void Func_0200152a(s32);
void Func_020015de(s32, s32, s32);
void Func_020015b8(s32, s32, s32);
void Func_020015e0(s32, s32);
void Func_02001558(s32);
void Func_020015c0(s32, s32);
void Func_02001566(s32);
void Func_02001606(s32, s32);
void Func_02001574(s32);
void Func_020015ec(s32, s32);
void Func_02001582(s32);
void Func_02001622(s32, s32);
void Func_02001590(s32);
void Func_020015f8(s32, s32);
void Func_0200159e(s32);
void Func_02001640(s32, s32);
s32 Func_020016ae(s32, s32, s32);
void Func_0200166a(s32, s32, s32);
void Func_020015d0(s32);
s32 Func_02001608(s32, s32);
void Func_020015ec_a();

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunActor14BeatsAndFlashGate(void)
{
    u8 *rec;
    s16 gate;

    rec = Func_02001522(0);
    gate = (s16)((*(u16 *)(rec + 6) + 0x2000) & -0x4000);
    Func_02001506(0x300);
    Func_02001522_a();
    Func_020015fe();
    Func_020015ac(0x26ec);
    Func_0200152a(50);
    Call3(Func_020015de, 14, 0x102, 50);
    Func_020015b8(14, 0, 20);
    Func_020015e0(14, 0);
    Func_02001558(10);
    Func_020015c0(14, 4);
    Func_02001566(30);
    Func_02001606(14, 0);
    Func_02001574(10);
    Func_020015ec(14, 2);
    Func_02001582(30);
    Func_02001622(14, 0);
    Func_02001590(10);
    Func_020015f8(14, 3);
    Func_0200159e(20);
    Func_02001640(14, 0);
    if (((s32)gate << 16) == (s32)0x80000000) {
        Call3((void (*)())Func_020016ae, 0, 0, 16);
        Func_0200166a(0, 0xc000, 0);
        Func_020015d0(20);
    }
    Func_02001608(14, 0x02009314);
    Func_020015ec_a();
}
