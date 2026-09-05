#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

struct TileRun {
    s16 id;
    s16 x;
    s16 y;
    s16 vertical;
    s16 unused08;
    s16 unused0a;
};

struct Cell {
    u8 unk0;
    u8 unk1;
    u8 kind;
    u8 type;
};

extern s16 *Data_020092c0;
extern s16 *Data_020092c8;
extern u16 *Data_020092c4;
extern u8 Data_0202c000[];

void Func_020001cc();
s32 Func_02000bb4(s32, s32);
void Func_02000c8a(s32);
void Func_020010e4(s32, s32, s32, s32, s32, s32);
void Func_0200118c(void);
void Func_020011e4(s32, s32, s32, s32, s32, s32);
void Func_020011fa(s32, s32, s32, s32, s32, s32);
void Func_0200122a();
void Func_0200123e();
void Func_02001254();
void Func_02001272();
void Func_02001284();
void Func_0200129c();
void Func_020012a6();
void Func_020012b8();
void Func_020012d0();
void Func_020012b6();
void Func_02001324();
void Func_02001340();
void Func_02001312();
void Func_02001378();
void Func_0200139e();
void Func_02000e70(s16 *records, s32 value);
void Func_020013bc();
void Func_02000e8e(s16 *records, s32 value);
void Func_020004a6(void);
void Func_02001952();
void Func_02001958();
void Func_02001982();
void Func_0200198a();
void Func_0200199a();
void Func_020019a4();
void Func_020019b4();
void Func_020019ba();
void Func_020019cc();
void Func_020019ce();
void Func_020019d2();
void Func_020019de();
void Func_020019ea();
void Func_020019f4();
void Func_02001a02();
void Func_02001a08();
void Func_02001a42();
void Func_02001a48();
void Func_02001d9e(void);
s32 Func_02001d64(s32, s32);
void Func_02001e22(void);
void Func_02001e06(void);
void Func_02001fc0(s32, s32);

/*
 * resource_394 owner at 0x02000030, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000034 holding 0x2009170.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000030 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2009170 is image offset
 * 0x1170 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_394 owner at 0x0200003c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000040 holding 0x20091d0.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200003c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20091d0 is image offset
 * 0x11d0 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_394 owner at 0x02000044, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000048 holding 0x20091e0.
 *
 * LEAF RESIDUE. Published at image offset 0x1c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000044 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20091e0 is image offset
 * 0x11e0 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_394 owner at 0x0200004c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000050 holding 0x2009240.
 *
 * LEAF RESIDUE. Published at image offset 0x24; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200004c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2009240 is image offset
 * 0x1240 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * Resource 394 board repaint at 0x02000194.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus `mov r7,r8 / push {r7}` at
 * 0x02000194, and the matching `add sp,#8 / pop {r3} / mov r8,r3 /
 * pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02000358.  472-byte row: 464 bytes
 * of code plus the two pool words 0x020092c0 and 0x020092c8 at
 * 0x02000364-0x0200036b.  Control-flow walk: every branch target is
 * 0x02000358 or below and the next owner's prologue is at 0x0200036c.  The
 * return address is popped into r0, so the owner is `void`.
 *
 * Called from the root 0x02000980 and reached again from 0x02000980's own
 * chain; it in turn calls 0x02000b3c twice and 0x02000150 once.
 *
 * 0x020092c0 and 0x020092c8 are pointer CELLS, not values — the body always
 * loads through them twice.  tracked byte-exact games/gs1/assets/code/
 * resource_394_c_02000054.c reads `*(s32 *)0x020092C0` the same way, and
 * games/gs1/assets/code/resource_394_c_020008b0.c declares the neighbouring cell
 * `extern u16 *Data_020092c4`.
 *
 * The layout selector at `*Data_020092c8` is re-read four times rather
 * than cached, and each read is spelled out: the first two arms test it
 * against zero and the last against one, so folding them into one local would
 * not be faithful to the polarity changes.
 *
 * The two cells are declared as extern pointers rather than spelled as
 * literal addresses: 0x020092c8 is 0x020092c0 + 8, so the literal form lets
 * CSE derive the second pool word with `adds r3, #8' while the reference keeps
 * two distinct pool words.  The zero stored into piece[85] and into the else
 * arm's piece+12 is a function-scope local, which is what buys the reference's
 * fourth callee-saved register (r7).  The record pointer advances once, in the
 * loop's common tail, so both arms read the record fields unadvanced.
 *
 * The record list is the same twelve-byte-stride, -1-terminated array that
 * 0x02000a90 fills and 0x02000b3c stamps: id at +0, column at +2, row at +4,
 * orientation at +6 and the spawned object pointer at +8.
 *
 * Nineteen call sites, matching the row's advertised count.  Per-target,
 * assembly = C: Func_080091b8 10, Func_080091c0 4, Object_SetMode 2,
 * SceneState_StampRecordCells 2, SceneState_ApplyRectByLayoutSelector 1.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */

  /* place a fixture, first bank: (x, y, w, h, sx, sy) */

  /* place a fixture, second bank */

  /* set object motion state */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* Resolved whole-owner call targets. */

/* One symbol PER CALL SITE, named at the site's PC-relative-decoded address
   (see resource_382:3ac for the rule, tools/bl-site-symbols to derive
   them). All three reach the same ARM-mode IWRAM helper that scales a
   channel by the adjustment, and each still needs its own name. */

/*
 * Apply the resource's asymmetric RGB555 colour adjustment.
 *
 * Owner span: 0x02000ecc-0x02000f34.  Control jumps over the mask literal at
 * 0x02000f14 and rejoins at 0x02000f18 before the common return.
 */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 Func_02001ee2();   /* 0x02000eec */

s32 Func_02001ef0();   /* 0x02000efa */

s32 Func_02001efe();   /* 0x02000f08 */

u8 *SceneData_GetTable9170(void)
{
    return (u8 *)0x02009170;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable91d0(void)
{
    return (u8 *)0x020091d0;
}

u8 *SceneData_GetTable91e0(void)
{
    return (u8 *)0x020091e0;
}

u8 *SceneData_GetTable9240(void)
{
    return (u8 *)0x02009240;
}

void SceneState_ApplyRecordTable92c0(void) {
    Func_02000c8a(*(s32 *)0x020092C0);
    Func_020010e4(0, 0x40, 0x20, 0x20, 0, 0);
    Func_02000bb4(*(s32 *)0x020092C0, 0xFF);
    Func_020001cc();
}

void FieldScene_CallHelper118c(void)
{
    Func_0200118c();
}

void SceneState_ApplyRectByLayoutSelector(void)
{
    if (**(s16 **)0x020092c4 == 1) {
        s32 fifth = 4;
        s32 sixth = 9;
        Func_020011e4(0, 0, 1, 4, fifth, sixth);
    } else {
        s32 fifth = 6;
        s32 sixth = 9;
        Func_020011fa(0, 0, 1, 4, fifth, sixth);
    }
}

void FieldScene_RepaintBoardRecords(void)
{
    s32 zero;
    s16 *record = Data_020092c0;

    if (*Data_020092c8 != 0) {
        { s32 f1 = 79; s32 g1 = 29; Func_0200122a(65, 53, 2, 1,  f1, g1); }
        { s32 f2 = 15; s32 g2 = 28; Func_0200123e(65, 40, 2, 4,  f2, g2); }
    } else {
        { s32 f3 = 79; s32 g3 = 25; Func_02001254(65, 50, 2, 5,  f3, g3); }
    }

    if (*Data_020092c8 != 0) {
        { s32 f4 = 32; s32 g4 = 0; Func_02001272(0, 32, 32, 32,  f4, g4); }
        { s32 f5 = 64; s32 g5 = 0; Func_02001284(32, 32, 32, 32,  f5, g5); }
        { s32 f6 = 0; s32 g6 = 0; Func_0200129c(0, 32, 32, 32,  f6, g6); }
    } else {
        { s32 f7 = 32; s32 g7 = 0; Func_020012a6(0, 64, 32, 32,  f7, g7); }
        { s32 f8 = 64; s32 g8 = 0; Func_020012b8(32, 64, 32, 32,  f8, g8); }
        { s32 f9 = 0; s32 g9 = 0; Func_020012d0(0, 64, 32, 32,  f9, g9); }
    }

    if (record[0] != -1) {
        zero = 0;
        do {
            u8 *piece = *(u8 **)(record + 4);

            if (*Data_020092c8 == 1) {
                Func_020012b6(piece, 4);
                piece[35] = 3;
                piece[85] = zero;
                *(s32 *)(piece + 12) = 0x1a0000;

                if (record[3] != 0) {
                    s32 col = record[1];
                    s32 row = record[2];
                    Func_02001324(68, 40, 1, 4, col + 32, row);
                } else {
                    s32 col = record[1];
                    s32 row = record[2];
                    Func_02001340(70, 40, 4, 1, col + 32, row);
                }
            } else {
                Func_02001312(piece, 1);
                piece[35] = 1;
                piece[85] = 2;
                *(s32 *)(piece + 12) = zero;
            }
            record += 6;
        } while (record[0] != -1);
    }

    { s32 f10 = 10; s32 g10 = 50; Func_02001378(70, 42, 1, 1,  f10, g10); }

    if (*Data_020092c8 == 1) {
        { s32 f11 = 0; s32 g11 = 0; Func_0200139e(0, 32, 32, 32,  f11, g11); }
        Func_02000e70(Data_020092c0, 254);
    } else {
        { s32 f12 = 0; s32 g12 = 0; Func_020013bc(0, 64, 32, 32,  f12, g12); }
        Func_02000e8e(Data_020092c0, 255);
    }

    Func_020004a6();
}

void SceneState_CopyPresetA0d0WithOffsetB0(void)
{
    u32 *dst;
    const u32 *src;
    u16 *p;

    src = (const u32 *)0x03001ad4;
    dst = (u32 *)0x0200a0d0;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst = *src;
    p = (u16 *)0x0200a0d0;
    p[1] += 0xb0;
    p[3] += 0xb0;
    p[5] += 0xb0;
}

void FieldScene_RunActorEightSequence(void)
{
    Func_02001952();
    Func_0200199a(0, 8);
    Func_02001958(6);
    Func_020019de(239);
    Call3(Func_0200198a, 8, 0x8000, 0x3333);
    Func_020019ba(8, 2);
    Func_020019a4(8, 104, 176);
    Func_02001982(6);
    Func_020019d2(0, 2);
    Call3(Func_020019b4, 0, 0x4ccc, 0x3333);
    Func_020019ce(0, 8, 0);
    Func_020019a4(24);
    Func_020019f4(0, 1);
    Func_020019ea(8);
    Func_02001a02(8, 1);
    Call1(Func_02001a42, 0x120);
    Func_02001a48(213);
    Call6(Func_020019ba, 5, 9, 1, 4, 4, 9);
    Call6(Func_020019cc, 0, 0, 1, 4, 6, 9);
    *Data_020092c4 = 0;
    Func_02001a08();
}

void SceneState_StampRecordCells(s16 *records, s32 value)
{
    extern u8 Data_02010000[];

    s16 *record = records;
    if (record[0] == -1) return;
    do {
        s32 column = record[1];
        s32 row = record[2];
        s32 along = record[3];
        s32 i;
        for (i = 3; i >= 0; i--) {
            u8 *cell;
            cell = Data_02010000 + ((column + (row << 7)) << 2);
            cell[2] = (u8)value;
            if (along == 0) column++;
            else row++;
        }
        record += 6;
    } while (record[0] != -1);
}

const struct TileRun *SceneData_FindTileRunAt(
    const struct TileRun *run,
    s32 x,
    s32 y)
{
    while (run->id != -1) {
        s32 x0 = run->x;
        s32 x1 = x0;
        s32 y0 = run->y;
        s32 y1 = y0;

        if (run->vertical == 0)
            x1 += 3;
        else
            y1 += 3;

        if (x >= x0 && x <= x1 &&
            y >= y0 && y <= y1)
            return run;
        run++;
    }
    return 0;
}

s32 SceneState_CheckFourCellRun(s32 x, s32 z, s32 mode)
{
    extern struct Cell Data_02010000[];

    s32 i;

    for (i = 0; i <= 3; i++) {
        struct Cell *cell = &Data_02010000[x + (z << 7)];

        if (cell->kind == 0xff || *(u8 *)((cell->type << 2) + (s32)Data_0202c000) != 0) {
            return -1;
        }
        if (mode == 0) {
            x++;
        } else {
            z++;
        }
    }
    return 0;
}

void SceneEffect_AdjustPaletteColors(s32 a)
{
    u32 x;

    Func_02001d9e();
    x = 0;
    do {
        u32 idx = x >> 16;
        if (x + 0xffef0000 > 0x60000 && (idx + 0xff3f) << 16 > 0x70000) {
            u16 *pal = (u16 *)(0x5000000 + idx * 2);
            *pal = Func_02001d64(*pal, a);
        }
        {
            u32 nx = x + 0x10000;
            x = nx;
            if (nx > 0xdf0000) {
                break;
            }
        }
    } while (1);
    Func_02001e22();
    Func_02001e06();
    Func_02001fc0(0x10000, 0);
}

u16 SceneEffect_AdjustColorChannels(u16 color, s32 adj)
{
    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Func_02001ee2(
        red,
        (s32)((u32)adj << 2)
    ));
    green = (s16)(green - Func_02001ef0(green, adj));
    blue = (s16)(blue - Func_02001efe(blue, adj));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
