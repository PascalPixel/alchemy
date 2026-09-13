#include "types.h"
#include "scene.h"

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

extern s16 *gOv;
extern s16 *gOv2;
extern u16 *gOv3;
extern u8 gUnk[];

/*
 * The eight-byte owner at 0x02000030 includes its one pool word, which holds
 * the returned table address 0x02009170.
 */

/*
 * The eight-byte owner at 0x0200003c includes its one pool word, which holds
 * the returned table address 0x020091d0.
 */

/*
 * The eight-byte owner at 0x02000044 includes its one pool word, which holds
 * the returned table address 0x020091e0.
 */

/*
 * The eight-byte owner at 0x0200004c includes its one pool word, which holds
 * the returned table address 0x02009240.
 */

/*
 * Repaint the board records.  The owner at 0x02000194 includes its two pool
 * words; 0x020092c0 and 0x020092c8 are pointer cells, declared extern rather
 * than as literal addresses so that neither pool word derives the other.  The
 * layout selector is re-read at every test and must not be folded into one
 * local; the zero stored into piece[85] and piece + 12 is a function-scope
 * local; the record pointer advances only in the loop's common tail.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */

  /* Place a fixture, first bank: (x, y, w, h, sx, sy). */

  /* Place a fixture, second bank. */

  /* Set object motion state. */

/*
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, not a runtime address.
 */

/*
 * Call sites spelled through these wrappers pass their constants straight into
 * the argument registers; a direct call precomputes a costly constant into a
 * pseudo shared with later uses in the block.  A value-returning call also
 * sets r0 last of its arguments.
 */

/*
 * One symbol per call site, named at the site's pc-relative-decoded address.
 * All three reach the same ARM-mode IWRAM helper that scales a channel by the
 * adjustment, and each still needs its own name.
 */

/*
 * Apply the resource's asymmetric RGB555 color adjustment.  The owner spans
 * 0x02000ecc-0x02000f34; control jumps over the mask literal at 0x02000f14 and
 * rejoins at 0x02000f18 before the common return.
 */

s32 SceneData_Run();   /* 0x02000eec */

s32 SceneData_unk2_4();   /* 0x02000efa */

s32 SceneData_unk3_4();   /* 0x02000f08 */

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

void State_ApplyRecordTable92c0(void)
{
    SceneData_unk2_2(*(s32 *)0x020092C0);
    SceneData_unk3_5(0, 0x40, 0x20, 0x20, 0, 0);
    SceneData_Apply(*(s32 *)0x020092C0, 0xFF);
    SceneData_unk4_4();
}

void Scene_CallHelper(void)
{
    SceneData_unk5_4();
}

void State_ApplyRectByLayoutSelector(void)
{
    if (**(s16 **)0x020092c4 == 1) {
        s32 fifth = 4;
        s32 sixth = 9;
        SceneData_unk4_5(0, 0, 1, 4, fifth, sixth);
    } else {
        s32 fifth = 6;
        s32 sixth = 9;
        SceneData_unk5_5(0, 0, 1, 4, fifth, sixth);
    }
}

void Scene_RepaintBoardRecords(void)
{
    s32 zero;
    s16 *record = gOv;

    if (*gOv2 != 0) {
        { s32 f1 = 79; s32 g1 = 29; SceneData_unk6_3(65, 53, 2, 1,  f1, g1); }
        { s32 f2 = 15; s32 g2 = 28; SceneData_unk7_3(65, 40, 2, 4,  f2, g2); }
    } else {
        { s32 f3 = 79; s32 g3 = 25; SceneData_unk8_3(65, 50, 2, 5,  f3, g3); }
    }

    if (*gOv2 != 0) {
        { s32 f4 = 32; s32 g4 = 0; SceneData_unk9_3(0, 32, 32, 32,  f4, g4); }
        { s32 f5 = 64; s32 g5 = 0; SceneData_unk10_3(32, 32, 32, 32,  f5, g5); }
        { s32 f6 = 0; s32 g6 = 0; SceneData_unk11_3(0, 32, 32, 32,  f6, g6); }
    } else {
        { s32 f7 = 32; s32 g7 = 0; SceneData_unk12_3(0, 64, 32, 32,  f7, g7); }
        { s32 f8 = 64; s32 g8 = 0; SceneData_unk13_3(32, 64, 32, 32,  f8, g8); }
        { s32 f9 = 0; s32 g9 = 0; SceneData_unk14_3(0, 64, 32, 32,  f9, g9); }
    }

    if (record[0] != -1) {
        zero = 0;
        do {
            u8 *piece = *(u8 **)(record + 4);

            if (*gOv2 == 1) {
                SceneData_unk15_3(piece, 4);
                piece[35] = 3;
                piece[85] = zero;
                *(s32 *)(piece + 12) = 0x1a0000;

                if (record[3] != 0) {
                    s32 col = record[1];
                    s32 row = record[2];
                    SceneData_unk16_3(68, 40, 1, 4, col + 32, row);
                } else {
                    s32 col = record[1];
                    s32 row = record[2];
                    SceneData_unk17_3(70, 40, 4, 1, col + 32, row);
                }
            } else {
                SceneData_unk18_3(piece, 1);
                piece[35] = 1;
                piece[85] = 2;
                *(s32 *)(piece + 12) = zero;
            }
            record += 6;
        } while (record[0] != -1);
    }

    { s32 f10 = 10; s32 g10 = 50; SceneData_unk19_3(70, 42, 1, 1,  f10, g10); }

    if (*gOv2 == 1) {
        { s32 f11 = 0; s32 g11 = 0; SceneData_unk20_3(0, 32, 32, 32,  f11, g11); }
        SceneData_Apply2(gOv, 254);
    } else {
        { s32 f12 = 0; s32 g12 = 0; SceneData_unk21_3(0, 64, 32, 32,  f12, g12); }
        SceneData_Apply3(gOv, 255);
    }

    SceneData_unk22_3();
}

void State_CopyPresetA0d0WithOffsetB0(void)
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

void Scene_RunActorEightSequence(void)
{
    SceneData_unk23_3();
    SceneData_unk24_3(0, 8);
    SceneData_unk25_3(6);
    SceneData_unk26_2(239);
    SceneData_Place(8, 0x8000, 0x3333);
    SceneData_unk27_2(8, 2);
    SceneData_unk28_2(8, 104, 176);
    SceneData_unk29_2(6);
    SceneData_unk30_2(0, 2);
    SceneData_unk2_3(0, 0x4ccc, 0x3333);
    SceneData_unk31_2(0, 8, 0);
    SceneData_unk32_2(24);
    SceneData_unk33_2(0, 1);
    SceneData_unk34_2(8);
    SceneData_unk35_2(8, 1);
    SceneData_Do(0x120);
    SceneData_unk36_2(213);
    SceneData_SetRect(5, 9, 1, 4, 4, 9);
    SceneData_unk2_5(0, 0, 1, 4, 6, 9);
    *gOv3 = 0;
    SceneData_unk37_2();
}

void State_StampRecordCells(s16 *records, s32 value)
{
    extern u8 gUnk2[];

    s16 *record = records;
    if (record[0] == -1) return;
    do {
        s32 column = record[1];
        s32 row = record[2];
        s32 along = record[3];
        s32 i;
        for (i = 3; i >= 0; i--) {
            u8 *cell;
            cell = gUnk2 + ((column + (row << 7)) << 2);
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

s32 State_CheckFourCellRun(s32 x, s32 z, s32 mode)
{
    extern struct Cell gUnk2[];

    s32 i;

    for (i = 0; i <= 3; i++) {
        struct Cell *cell = &gUnk2[x + (z << 7)];

        if (cell->kind == 0xff || *(u8 *)((cell->type << 2) + (s32)gUnk) != 0) {
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

void Effect_AdjustPaletteColors(s32 a)
{
    u32 x;

    SceneData_unk38_2();
    x = 0;
    do {
        u32 idx = x >> 16;
        if (x + 0xffef0000 > 0x60000 && (idx + 0xff3f) << 16 > 0x70000) {
            u16 *pal = (u16 *)(0x5000000 + idx * 2);
            *pal = SceneData_Apply4(*pal, a);
        }
        {
            u32 nx = x + 0x10000;
            x = nx;
            if (nx > 0xdf0000) {
                break;
            }
        }
    } while (1);
    SceneData_unk39_2();
    SceneData_unk40_2();
    SceneData_Apply5(0x10000, 0);
}

u16 Effect_AdjustColorChannels(u16 color, s32 adj)
{
    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + SceneData_Run(
        red,
        (s32)((u32)adj << 2)
    ));
    green = (s16)(green - SceneData_unk2_4(green, adj));
    blue = (s16)(blue - SceneData_unk3_4(blue, adj));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
