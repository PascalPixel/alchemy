#include "types.h"

extern u8 *Data_03001ebc;
extern void Func_080000c0();
extern void Func_080091c0();
extern s32 Func_080770c0();
extern void Func_080770c8();
extern void Func_0808a010();
extern void Func_0808a018();
extern void Func_0808a020();
extern u8 *Func_0808a080();
extern void Func_0808a090();
extern void Func_0808a098();
extern void Func_0808a0c8();
extern void Func_0808a0d0();
extern void Func_0808a0e0();
extern void Func_0808a0e8();
extern void Func_0808a0f0();
extern void Func_0808a100();
extern void Func_0808a130();
extern void Func_0808a138();
extern void Func_0808a148();
extern void Func_0808a170();
extern void Func_0808a180();
extern void Func_0808a188();
extern void Func_0808a1b8();
extern void Func_0808a1e0();
extern void Func_0808a1e8();
extern void Func_0808a5e8();
extern void Func_080f9010();

/*
 * resource_3a0 owner at 0x020005f4, 856 bytes through the eleven-word pool
 * before 0x0200094c. This is the complete actor-18 encounter, movement,
 * presentation, and cleanup controller. All 74 call sites remain explicit.
 */
void Func_020005f4(void)
{
    u8 *scene = Func_0808a080(20);
    u8 *actor;
    u8 *leader;
    s32 specialSide = 0;
    s32 x;
    s32 z;

    Func_0808a018();
    actor = Func_0808a080(18);
    *(s32 *)(actor + 108) = 0;

    if (Func_080770c0(0x200) != 0 ||
        (*(s32 *)(Func_0808a080(18) + 8) >> 20) <= 19) {
        u16 facing;
        u8 *state;

        actor = Func_0808a080(18);
        facing = *(u16 *)(actor + 6);
        Func_0808a148(18, 0, 0);
        Func_0808a010(10);
        Func_0808a170(0x17fb);

        if (Func_080770c0(0x200) == 0) {
            state = *(u8 **)Data_03001ebc;
            (*(u16 *)(state + 472))++;
            Func_0808a180(18, 0);
            *(u16 *)(Func_0808a080(18) + 0x64) = 0;
            *(u16 *)(Func_0808a080(18) + 6) = facing;
        } else {
            Func_0808a180(18, 0);
            Func_0808a1b8(18, 0x8000, 20);
        }

        *(s32 *)(Func_0808a080(18) + 108) = 0x02008501;
        Func_0808a020();
        return;
    }

    if ((*(s32 *)(Func_0808a080(0) + 16) >> 19) > 27 &&
        (*(s32 *)(Func_0808a080(0) + 16) >> 19) <= 29 &&
        (*(s32 *)(Func_0808a080(0) + 8) >> 20) != 26) {
        u8 *actorZero;
        u8 *actorEighteen;

        Func_0808a090(0, 0x8000, 0x4000);
        Func_0808a148(0, 18, 0);
        Func_0808a010(5);
        actorZero = Func_0808a080(0);
        actorEighteen = Func_0808a080(18);
        if (*(s32 *)(actorZero + 8) < *(s32 *)(actorEighteen + 8)) {
            Func_0808a080(0)[0x5a] &= (u8)~1;
            x = (*(s32 *)(Func_0808a080(18) + 8) >> 20) * 16 - 8;
            Func_0808a0c8(0, x, 232);
            specialSide = 1;
        } else {
            Func_0808a080(0)[0x5a] &= (u8)~1;
            x = (*(s32 *)(Func_0808a080(18) + 8) >> 20) * 16 + 24;
            Func_0808a0c8(0, x, 232);
        }
        Func_0808a0e8(0);
    }

    actor = Func_0808a080(18);
    *(s32 *)(actor + 0x38) = (s32)0x80000000;
    actor = Func_0808a080(18);
    *(s32 *)(actor + 0x3c) = (s32)0x80000000;
    actor = Func_0808a080(18);
    *(s32 *)(actor + 0x40) = (s32)0x80000000;
    Func_0808a098(18, 1);
    Func_0808a100(18, 1);
    Func_0808a138(18, 2);
    Func_0808a010(10);
    Func_080f9010(228);

    *(s32 *)(scene + 24) = 0x4ccc;
    *(s32 *)(scene + 28) = 0x4ccc;
    x = *(s32 *)(Func_0808a080(18) + 8);
    z = *(s32 *)(Func_0808a080(18) + 16);
    Func_0808a0f0(20, x, z);
    x = (x >> 20 << 20) + 0x80000;
    z = (z >> 20 << 20) + 0x80000;

    x = *(s32 *)(Func_0808a080(18) + 8) >> 20;
    z = *(s32 *)(Func_0808a080(18) + 16) >> 20;
    Func_080091c0(16, 16, 1, 1, x, z);
    Func_0808a1e0(20, 2);
    scene[0x23] |= 2;

    do {
        Func_080000c0(3);
        *(s32 *)(scene + 24) += 0x1999;
        *(s32 *)(scene + 28) += 0x1999;
    } while (*(s32 *)(scene + 24) <= 0xffff);

    Func_0808a1e8(18, 0x105, 70);
    Func_0808a148(18, 0, 0);
    Func_0808a010(20);
    Func_0808a1e8(18, 0x103, 0);
    Func_0808a130(18, 2);
    Func_0808a010(70);
    Func_0808a170(0x17fa);
    Func_0808a188(18, 0, 20);
    Func_0808a5e8();

    if ((*(s32 *)(Func_0808a080(0) + 8) >> 20) == 26 &&
        (*(s32 *)(Func_0808a080(0) + 16) >> 20) > 13) {
        Func_0808a090(0, 0xcccc, 0x6666);
        Func_0808a1b8(0, 0xc000, 10);
        Func_0808a080(0)[0x5a] &= (u8)~1;
        Func_0808a100(0, 2);
        Func_0808a0e0(0, 0, 16);
        Func_0808a0e8(0);
        Func_0808a100(0, 1);
    }

    Func_0808a090(18, 0xcccc, 0x6666);
    if ((*(s32 *)(Func_0808a080(18) + 16) >> 20) != 14) {
        actor = Func_0808a080(18);
        Func_0808a0d0(18, *(s16 *)(actor + 10), 232);
    }
    Func_0808a0d0(18, 280, 232);
    Func_080770c8(0x200);
    Func_0808a080(0)[0x5a] |= 1;
    Func_0808a020();
    (void)specialSide;
}
