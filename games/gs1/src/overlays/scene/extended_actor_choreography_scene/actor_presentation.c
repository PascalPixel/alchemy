#include "types.h"

#define SceneDialogue_RunActorTenFlaggedDialogue Func_020009b4
#define SceneActor_SetFlagBit3ForActors28To35 Func_020037b4
#define RunActorsEightAndNineMapEvent Func_02005ca4
extern u8 Value_00001e9f;
extern u8 Value_0000092b;
extern u8 Value_00000992;
extern u8 Value_00001e7b;
extern u8 Value_0000092a;
extern u8 Value_00000919;
extern u8 Value_00000929;
extern u8 Value_00000937;
extern u8 Value_0000092e;

s32 Func_02006e5c(s32 n);
u8 *Func_02001ca0(void);
void Func_02006e92(void);
void Func_02001b60(u8 *o);
void Func_02006f7e(s32 n);
void Func_02005248(s32 n);
void Func_02006f34(u8 *o, s32 n);
u8 *Func_02006ed2(s32 n);
void Func_02006f14(u8 *o, s32 a, s32 b);
void Func_02006f42(u8 *o);
void Func_02006f54(u8 *o, s32 a, s32 b);
void Func_02006ee0(void);
s32 Func_02006eb8(s32 n);
s32 Func_02006ec8(s32 n);
s32 Func_02006ed8(s32 n);
void Func_0200224a(s32 n, s32 a, s32 b);
void Func_02002256(s32 n, s32 a, s32 b);
u8 *Func_02009ca8();
void Func_0200a554();
void Func_0200a59a();
void Func_0200a5b6();
void Func_0200a5f2();
void Func_0200c170();
void Func_0200c19c();
void Func_0200c1c0();
void Func_0200c256();
void Func_0200c25a();
void Func_0200c2b8(s32, s32, s32);
void Func_0200c2c2();

/*
 * resource_3b1 helper: set bit 3 of the flags byte of actors 28 through 35.
 */

/*
 * The owner at 0x020037b4 is 36 bytes: 34 bytes of code and one alignment
 * halfword; it needs no literal pool.  The index, bit and zero are held in
 * locals initialised in that order.  The zero is an OR identity that folds
 * away unless it has its own local, and initialising the index in a for-init
 * emits it last.  Func_02009ca8 names the in-overlay entry point, not its
 * veneer.
 */

static __inline__ void SetDirection(s32 actor, s32 direction, s32 duration)
{
    Func_0200c2b8(actor, direction, duration);
}

void SceneDialogue_RunActorTenFlaggedDialogue(void)
{
    if (Func_02006e5c(0x300) != 0) {
        u8 *o = Func_02001ca0();
        u8 *p;

        Func_02006e92();
        Func_02001b60(o);
        Func_02006f7e((s32)&Value_00001e9f);
        Func_02005248(10);
        Func_02006f34(o, 2);
        p = Func_02006ed2(0);
        if (p != 0) {
            Func_02006f14(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Func_02006f42(o);
        Func_02006f54(o, 0, 0);
        Func_02006ee0();
    } else if (Func_02006eb8((s32)&Value_0000092b) != 0) {
        Func_0200224a(10, (s32)&Value_00001e7b, (s32)&Value_00000992);
    } else if (Func_02006ec8((s32)&Value_0000092a) != 0) {
        Func_0200224a(10, (s32)&Value_00001e7b, (s32)&Value_00000919);
    } else if (Func_02006ed8((s32)&Value_00000929) != 0) {
        Func_0200224a(10, (s32)&Value_00001e7b, (s32)&Value_00000937);
    } else {
        Func_02002256(10, (s32)&Value_00001e7b, (s32)&Value_0000092e);
    }
}

void SceneActor_SetFlagBit3ForActors28To35(void)
{
    u32 i;
    u32 bit;
    u32 zero;

    i = 28;
    bit = 8;
    zero = 0;
    for (; i <= 35; i++) {
        u8 *obj = Func_02009ca8(i);
        u32 v = obj[0x59];
        obj[0x59] = (u8)((v | bit) | zero);
    }
}

void RunActorsEightAndNineMapEvent(void)
{
    Func_0200c170();
    Func_0200a59a(15, 1, 0);
    Func_0200a554(9, 468, 616, 32768);
    Func_0200a5b6(8, 1, 20);
    Func_0200c256(9, 2);
    Func_0200c19c(20);
    SetDirection(8, 53248, 80);
    Func_0200c2c2(8, 0, 20);
    Func_0200c25a(8, 3);
    Func_0200c1c0(20);
    Func_0200a5f2(9, 21, 0);
}
