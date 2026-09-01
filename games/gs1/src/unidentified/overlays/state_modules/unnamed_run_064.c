/* Contiguous unnamed state-owner run for resource_383. */

#include "types.h"

/*
 * resource_383 owner at 0x02000030, 8 bytes: `ldr r0, [pc, #0] / bx lr`
 * plus the one-word literal pool at 0x02000034 holding 0x200d8f8.
 *
 * HEAD EXPORT STUB. Published from this overlay's own header word at image
 * offset 0xc. It lies BEFORE the first recorded owner, in the region
 * `gapsBetween` never read until 2026-08-01, and it is visible to the head
 * sweep only because the veneer predicate compares registers: `ldr r0,=X /
 * bx lr` is a LEAF, not a veneer, and the old predicate masked every one of
 * these as structure.
 *
 * SPAN IS 8 BYTES, NOT 4. The pool word sits PAST the `bx lr` and is read by
 * this row's own `ldr r0, [pc, #0]` -- pc 0x02000034 after alignment, plus 0 --
 * so it belongs to this owner. Recording 4 would orphan a word and
 * manufacture a phantom gap.
 *
 * The pool word 0x200d8f8 is an ADDRESS, loaded and returned WITHOUT being
 * dereferenced, so the row is a getter for an in-image table. One of the 29
 * head getters on ten overlays, and 28 of the 29 pool words are distinct.
 * The one repeat is 0x2009c34, returned by BOTH resource_395 0x48 and
 * resource_3ad 0x30 -- and that is a coincidence of ADDRESS, not shared
 * identity: only one overlay is resident at 0x02000000 at a time, so the
 * same VA designates different bytes depending on which is loaded.
 * Identical bytes are not identical semantics.
 *
 * 0 of 0 callees.
 */


u8 *Func_02000030(void)
{
    return (u8 *)0x0200d8f8;
}

#include "types.h"

/*
 * resource_383 owner at 0x02000038, 8 bytes: `ldr r0, [pc, #0] / bx lr`
 * plus the one-word literal pool at 0x0200003c holding 0x200da60.
 *
 * HEAD EXPORT STUB. Published from this overlay's own header word at image
 * offset 0x2c. It lies BEFORE the first recorded owner, in the region
 * `gapsBetween` never read until 2026-08-01, and it is visible to the head
 * sweep only because the veneer predicate compares registers: `ldr r0,=X /
 * bx lr` is a LEAF, not a veneer, and the old predicate masked every one of
 * these as structure.
 *
 * SPAN IS 8 BYTES, NOT 4. The pool word sits PAST the `bx lr` and is read by
 * this row's own `ldr r0, [pc, #0]` -- pc 0x0200003c after alignment, plus 0 --
 * so it belongs to this owner. Recording 4 would orphan a word and
 * manufacture a phantom gap.
 *
 * The pool word 0x200da60 is an ADDRESS, loaded and returned WITHOUT being
 * dereferenced, so the row is a getter for an in-image table. One of the 29
 * head getters on ten overlays, and 28 of the 29 pool words are distinct.
 * The one repeat is 0x2009c34, returned by BOTH resource_395 0x48 and
 * resource_3ad 0x30 -- and that is a coincidence of ADDRESS, not shared
 * identity: only one overlay is resident at 0x02000000 at a time, so the
 * same VA designates different bytes depending on which is loaded.
 * Identical bytes are not identical semantics.
 *
 * 0 of 0 callees.
 */


u8 *Func_02000038(void)
{
    return (u8 *)0x0200da60;
}

#include "types.h"

/*
 * resource_383 owner at 0x02000040, 8 bytes: `ldr r0, [pc, #0] / bx lr`
 * plus the one-word literal pool at 0x02000044 holding 0x200da80.
 *
 * HEAD EXPORT STUB. Published from this overlay's own header word at image
 * offset 0x14. It lies BEFORE the first recorded owner, in the region
 * `gapsBetween` never read until 2026-08-01, and it is visible to the head
 * sweep only because the veneer predicate compares registers: `ldr r0,=X /
 * bx lr` is a LEAF, not a veneer, and the old predicate masked every one of
 * these as structure.
 *
 * SPAN IS 8 BYTES, NOT 4. The pool word sits PAST the `bx lr` and is read by
 * this row's own `ldr r0, [pc, #0]` -- pc 0x02000044 after alignment, plus 0 --
 * so it belongs to this owner. Recording 4 would orphan a word and
 * manufacture a phantom gap.
 *
 * The pool word 0x200da80 is an ADDRESS, loaded and returned WITHOUT being
 * dereferenced, so the row is a getter for an in-image table. One of the 29
 * head getters on ten overlays, and 28 of the 29 pool words are distinct.
 * The one repeat is 0x2009c34, returned by BOTH resource_395 0x48 and
 * resource_3ad 0x30 -- and that is a coincidence of ADDRESS, not shared
 * identity: only one overlay is resident at 0x02000000 at a time, so the
 * same VA designates different bytes depending on which is loaded.
 * Identical bytes are not identical semantics.
 *
 * 0 of 0 callees.
 */


u8 *Func_02000040(void)
{
    return (u8 *)0x0200da80;
}

#include "types.h"

extern s16 Data_02000240[];
extern u8 Data_0200dcc8[];
extern u8 Data_0200dab8[];
extern void Func_02004dbc(u8 *);

u8 *Func_02000048(void)
{
    s16 *table = Data_02000240;
    u8 *result;
    s32 value = table[225];
    s32 low = 15;

    if (value <= 17) {
        if (value >= low) {
            result = Data_0200dcc8;
        } else {
            result = Data_0200dab8;
        }
    } else {
        result = Data_0200dab8;
    }
    Func_02004dbc(result);
    return result;
}

#include "types.h"

extern u8 *Func_02004e04(s32);
extern u8 *Func_02004e14(s32);
extern u8 *Func_02004e3e(s32);
extern u8 *Func_02004e4e(s32);
extern void Func_02004f32(void);
extern void Func_02004d9e(s32, s32, s32, s32, s32, s32);
extern void Func_02004f6c(void);
extern void Func_02004dd8(s32, s32, s32, s32, s32, s32);

void Func_02000080(void)
{
    s32 x;
    s32 z;

    if (*(u16 *)(Func_02004e04(0) + 6) >= 0xa000
        && *(u16 *)(Func_02004e14(0) + 6) <= 0xe000) {
        Func_02004f32();
        x = 42;
        z = 85;
        Func_02004d9e(41, 85, 1, 1, x, z);
    } else if (*(u16 *)(Func_02004e3e(0) + 6) >= 0x2000
               && *(u16 *)(Func_02004e4e(0) + 6) <= 0x6000) {
        Func_02004f6c();
        x = 42;
        z = 85;
        Func_02004dd8(43, 85, 1, 1, x, z);
    }
}

#include "types.h"

struct Object {
    u8 filler00[8];
    s32 x;
};

extern void Func_02004e46(void);
extern void Func_02004e46_b(s32);
extern struct Object *Func_02004e84(s32);
extern void Func_02004e08(s32, s32, s32, s32, s32, s32);
extern void Func_02004e7a(void);

void Func_020000fc(void)
{
    struct Object *object;
    s32 x;
    s32 z;

    Func_02004e46();
    object = Func_02004e84(26);
    if ((object->x >> 20) == 42) {
        x = 41;
        z = 24;
        Func_02004e08(101, 24, 3, 4, x, z);
        Func_02004e46_b(0x859);
    }
    Func_02004e7a();
}

#include "types.h"

extern u8 Value_00001280;

extern u8 *Func_02004ebe(s32);
extern void Func_02004e8c(void);
extern void Func_02005066(s32, s32);
extern s32 Func_02004e76(s32);
extern void Func_02004f80(s32);
extern void Func_02004f88(s32);
extern void Func_02004fa0(s32, s32);
extern void Func_02004ecc(void);

void Func_0200013c(void)
{
    s32 value = *(u16 *)(Func_02004ebe(0) + 6);

    Func_02004e8c();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_02005066(4, 19);
    } else {
        if (Func_02004e76(0x855) == 0) {
            Func_02004f80((s32)&Value_00001280);
        } else {
            Func_02004f88(0x1370);
        }
        Func_02004fa0(19, 0);
    }
    Func_02004ecc();
}

#include "types.h"

extern u8 *Func_02004f1e(s32);
extern void Func_02004eec(void);
extern void Func_020050c6(s32, s32);
extern s32 Func_02004ed6(s32);
extern void Func_02004fe0(s32);
extern void Func_02004fe8(s32);
extern void Func_02005000(s32, s32);
extern void Func_02004f2c(void);

void Func_0200019c(void)
{
    s32 value = *(u16 *)(Func_02004f1e(0) + 6);

    Func_02004eec();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_020050c6(5, 20);
    } else {
        if (Func_02004ed6(0x855) == 0) {
            Func_02004fe0(0x1282);
        } else {
            Func_02004fe8(0x1372);
        }
        Func_02005000(20, 0);
    }
    Func_02004f2c();
}

#include "types.h"

extern u8 *Func_02004f7e(s32);
extern void Func_02004f4c(void);
extern void Func_0200512e(s32, s32);
extern s32 Func_02004f36(s32);
extern void Func_02005040(s32);
extern void Func_02005048(s32);
extern void Func_02005060(s32, s32);
extern void Func_02004f8c(void);

void Func_020001fc(void)
{
    s32 value = *(u16 *)(Func_02004f7e(0) + 6);

    Func_02004f4c();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_0200512e(1, 23);
    } else {
        if (Func_02004f36(0x855) == 0) {
            Func_02005040(0x128d);
        } else {
            Func_02005048(0x137b);
        }
        Func_02005060(23, 0);
    }
    Func_02004f8c();
}

