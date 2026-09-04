#include "types.h"

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define Resource397_EmptyHook Func_020001d0

#include "resource_397.h"

struct SceneObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 settled_y;
    u8 unknown_18[10];
    u8 layer;
};

extern s32 Data_02000240[];
extern u8 *Data_03001ebc;
extern s32 Data_03001ae8;
extern s32 Data_03001b04;
extern u16 Data_04000006;
extern s32 Data_02008610;
extern u16 Data_02008614;
extern u16 Data_02008616;
extern u16 Data_0400001c;

void Func_0200037c(s32, s32);
u8 *Func_020003ac();
s32 Func_020003b4();
s32 Func_020003da();
void Func_0200013c(s32, s32);
s32 Func_0200014e(s32, s32);
s32 Func_0200016e(s32, s32);
void Func_0200017c(s32, s32);
s32 Func_0200018e(s32, s32);
void Func_02000496(s32);
void Func_0200048c(s32);
void Func_020004ae();
void Func_020004e4(void);
void Func_020004f0(void);
void Func_020004fc(u16 *, u8 *);
void Func_02000560();
void Func_0200051e();
void Func_020005f6(s32, s32, s32);
s32 Func_020005f8(s32, s32);

/*
 * resource_397 owner at 0x02000040, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000044 holding 0x200835c.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000040 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x200835c is image offset
 * 0x35c under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_397 owner at 0x02000048, 4 bytes: `movs r0, #0 / bx lr`.
 *
 * LEAF RESIDUE. Published at image offset 0x2c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 * A leaf never does -- it saves no register and returns with `bx lr`.
 *
 * Complete owner: both instructions. No prologue, no stack frame, no
 * literal pool, no callees, no argument read.
 *
 * One of the 70 rows sharing this exact body across the tree. The body is
 * shared; the identity is not -- this row is bounded by ITS overlay's
 * neighbours and published from ITS overlay's table.
 */

/*
 * resource_397 owner at 0x0200004c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000050 holding 0x200844c.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200004c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x200844c is image offset
 * 0x44c under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_397 owner at 0x02000054, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000058 holding 0x2008474.
 *
 * LEAF RESIDUE. Published at image offset 0x1c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000054 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008474 is image offset
 * 0x474 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/* The two tracked scene objects share this coordinate/terrain prefix. */

/*
 * resource_397 owner at 0x020001b4, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x20001b8 holding 0x20084a4.
 *
 * LEAF RESIDUE. Published at image offset 0x24; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x020001b4 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20084a4 is image offset
 * 0x4a4 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_397 owner at 0x020001d0, 2 bytes: `bx lr` alone.
 *
 * SWEEP-D RESIDUE, and the reason sweep D exists. This row is in NO sweep:
 * it is not published anywhere in the image (the full spelling
 * 0x02000000 + 0x1d0 + 0x8000 + Thumb bit = 0x20081d1 appears in no word),
 * no `bl` in the image reaches it, and it has no `push` for a shape scan
 * to key on. It offers no key to sweeps A, B or C. It was found only as
 * unaccounted bytes between two exact-C owners -- 0x020001bc ends at
 * 0x020001ca and 0x020001d4 begins the next -- and the ten bytes in between
 * read as pad, stub, pad, stub, pad.
 *
 * An EMPTY HOOK, the same body resource_3bb's 0x02003228 has and the same
 * one this cohort's two-byte class is made of. It saves nothing, reads
 * nothing, writes nothing and returns at once. Nothing here establishes
 * what it would have done; only that it does nothing.
 *
 * SPAN 2, NOT 4. The `0x0000` halfword before it aligns the entry to four
 * bytes and the one after aligns the next, and neither is part of this
 * body. Recording 4 would claim alignment as code and put a phantom
 * overlap in the next reader's arithmetic.
 */

void Func_02000030(void) {
    Func_0200037c(8, 0x3D);
}

u8 *Func_02000040(void)
{
    return (u8 *)0x0200835c;   /* image offset 0x35c */
}

s32 Func_02000048(void)
{
    return 0;
}

u8 *Func_0200004c(void)
{
    return (u8 *)0x0200844c;   /* image offset 0x44c */
}

u8 *Func_02000054(void)
{
    return (u8 *)0x02008474;   /* image offset 0x474 */
}

void Func_0200005c(s32 block_x, s32 block_z)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    struct SceneObject *object;
    s32 dx = block_x;
    s32 dz = block_z;
    s32 height;

    /* Block coordinates become 16.16 fixed-point shifts of sixteen tiles. */
    object = (struct SceneObject *)Func_020003ac(Data_02000240[125]);
    dx <<= 20;
    dz <<= 20;

    if (object != 0) {
        object->x += dx;
        object->z += dz;
        height = Func_020003b4((s32)object->layer, object->x, object->z);
        object->y = height;
        object->settled_y = height;
    }

    /* Apply the same shift to the workspace's independently optional object. */
    object = *(struct SceneObject **)(workspace + 480);
    if (object != 0) {
        object->x += dx;
        object->z += dz;
        height = Func_020003da((s32)object->layer, object->x, object->z);
        object->y = height;
        object->settled_y = height;
    }
}

void Func_020000d8(void)
{
    Func_0200013c(0, 5);
}

void Func_020000e8(void)
{
    Func_0200014e(0, -5);
}

void Func_020000f8(void)
{
    void Func_0200015c(s32, s32);

    Func_0200015c(0, 5);
}

void Func_02000108(void)
{
    Func_0200016e(0, -5);
}

void Func_02000118(void)
{
    Func_0200017c(0, 6);
}

void Func_02000128(void)
{
    Func_0200018e(0, -6);
}

void Func_02000138(void)
{
    u8 *state = Data_03001ebc;
    s16 *counter;

    Func_02000496(0x7B);
    counter = (s16 *)(state + 0x16C);
    Func_0200048c(*counter);
}

void Func_0200015c(void)
{
    u8 *display;

    /* Start the scene, then configure alpha blending for its display state. */
    Func_020004ae(9);

    *(volatile u16 *)0x04000050 = 0x3f42;
    *(volatile u16 *)0x04000052 = 0x0c04;

    display = *(u8 **)0x03001ecc;
    {
        u16 *slot = (u16 *)(display + 0x534);
        int value = 0x3f3f;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(display + 0x536);
        int value = 31;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(display + 0x52a);
        int value = 10;
        *slot = value;
    }
}

u8 *Func_020001b4(void)
{
    return (u8 *)0x020084a4;   /* image offset 0x4a4 */
}

void Func_020001bc(void)
{
    Func_020004e4();
    Func_020004f0();
}

void Resource397_EmptyHook(void)
{
}

void Func_020001d4(void)
{
    u16 *header = (u16 *)0x02008590;

    header[0] = Data_03001ae8;
    header[1] = Data_03001b04;
    Func_020004fc(header, (u8 *)0x020085B0);
}

s32 Func_02000200(void)
{
    u8 **base = (u8 **)0x03001ebc;
    u8 *workspace;
    u8 *display;
    s32 offset = 224;
    s32 *scene;

    /* Reset the scene word at workspace + 448 before entering scene 9. */
    offset <<= 1;
    workspace = *base;
    scene = (s32 *)(workspace + offset);
    offset -= 192;
    *scene = offset;

    Func_02000560(9);

    *(volatile u16 *)0x04000050 = 0x3f42;
    *(volatile u16 *)0x04000052 = 0x0c04;

    display = base[4];
    {
        u16 *slot = (u16 *)(display + 0x534);
        int value = 0x3f3f;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(display + 0x536);
        int value = 31;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(display + 0x52a);
        int value = 10;
        *slot = value;
    }

    Func_0200051e();
    return 0;
}

void Func_0200026c(void)
{
    u16 *source;
    u32 value;

    if (Data_04000006 >= Data_02008610) {
        source = &Data_02008614;
    } else {
        source = &Data_02008616;
    }
    value = *source;
    Data_0400001c = value;
}

void Func_020002e0(void)
{
    Func_020005f6(1, 0, 0x0200826D);
    Func_020005f8(0x020082A1, 0xC80);
}
