#include "types.h"

#define SceneActor_CopyActor8PositionWithFixedY Func_02000098
#define SceneActor_UpdateSlot10ByTileX Func_0200040c
#define SceneActor_RaiseSlot9StepA Func_0200050c
#define SceneActor_RaiseSlot9StepB Func_0200055c
#define SceneActor_RaiseSlot11AndSetFlag201 Func_020005fc
#define SceneActor_ClearCollisionFlagAndPlaceMarker Func_02003460
struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Frame *Func_02003c38(s32);
s32 *Func_02003fac(s32);
void Func_02003f56(s32, s32, s32, s32, s32, s32);
void Func_02003f6e(s32, s32, s32, s32, s32, s32);
void Func_02003f80(s32, s32, s32, s32, s32, s32);
void Func_02003f90(s32 *, s32);
void Func_02003f5c(void);
void Func_02003eea(s32);
s32 *Func_020040ac(s32);
void Func_02004050(s32, s32, s32, s32, s32, s32);
void Func_02004064(s32 *, s32);
void Func_020040a4(s32);
s32 *Func_020040fc(s32);
void Func_020040a2(s32, s32, s32, s32, s32, s32);
void Func_020040b6(s32 *, s32);
void Func_020040f6(s32);
s32 *Func_0200419c(s32);
void Func_02004142(s32, s32, s32, s32, s32, s32);
void Func_02004156(s32 *, s32);
void Func_02004194(s32);

/*
 * resource_3a4 owner at 0x02003460, 34 bytes with no literal pool: clear
 * bit 0 of the scene record's flags byte at +89, then place a marker at the
 * record's x and z at level 255. Both callees are reached through veneers.
 */

u8 *Func_02006ffc();           /* Scene record accessor. */

void Func_02006fbc();          /* Place a marker: kind, x, z, level. */

s32 SceneActor_CopyActor8PositionWithFixedY(struct Frame *dst)
{
    struct Frame *src = Func_02003c38(8);

    dst->f08 = src->f08;
    dst->f0c = 0xFFF40000;
    dst->f10 = src->f10;
    return 0;
}

void SceneActor_UpdateSlot10ByTileX(void)
{
    s32 *a = Func_02003fac(10);

    if (a != 0) {
        s32 x = 24;
        s32 y = 26;
        s32 t;

        Func_02003f56(x, 27, 2, 1, x, y);
        t = a[2] >> 20;
        if (t == 25) {
            Func_02003f6e(0, 0, 1, 1, t, y);
        } else {
            Func_02003f80(0, 0, 1, 1, x, y);
        }
        Func_02003f90(a, 0);
        ((u8 *)a)[0x55] = 0;
        Func_02003f5c();
        Func_02003eea(1);
    }
}

void SceneActor_RaiseSlot9StepA(void)
{
    s32 *p;
    s32 s0;

    p = Func_020040ac(9);
    s0 = 26;
    Func_02004050(0, 0, 1, 1, s0, s0);
    if (p != 0) {
        Func_02004064(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_020040a4(0x200);
}

void SceneActor_RaiseSlot9StepB(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Func_020040fc(9);
    s0 = 25;
    s1 = 13;
    Func_020040a2(23, 13, 1, 1, s0, s1);
    if (p != 0) {
        Func_020040b6(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_020040f6(0x200);
}

void SceneActor_RaiseSlot11AndSetFlag201(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Func_0200419c(11);
    s0 = 17;
    s1 = 10;
    Func_02004142(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Func_02004156(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_02004194(0x201);
}

void SceneActor_ClearCollisionFlagAndPlaceMarker(s32 no)
{
    u8 *record;

    record = Func_02006ffc(no);
    record[89] &= 0xfe;

    Func_02006fbc(0, *(s32 *)(record + 8), *(s32 *)(record + 16), 255);
}
