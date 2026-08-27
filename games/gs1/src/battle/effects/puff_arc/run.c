#include "types.h"

/*
 * Effect sequence at 0x080d9fc8.
 *
 * Nine puffs are laid out along one sine/cosine arc across the top of the
 * screen, then animated for eighty frames.  Each puff plays a six-cell
 * sequence, four frames per cell, staggered four frames apart, and is drawn
 * once, twice or three times depending on the layer count the caller set.
 * The rectangle blitter is the kind-46 block Func_080ed408 loads, so every
 * draw goes through the entry cached in Data_03001e50[46].
 *
 * Aggregate names remain provisional; byte offsets into the kind-39 work
 * block are retained where no evidence-backed structure exists yet.
 */

/* Six drawn arguments: destination, source cell, x, y, width, height.
   The reference calls it through the r4 bx bank, so it is an indirect
   call through the cached kind-46 entry rather than a fixed callee. */
typedef void (*DrawRectangle)(
    void *, const void *, s32, s32, s32, s32);

/* Heap-allocation cache: Data_03001e50[kind] holds kind's block address.
   This owner reads kinds 39 (its work block), 40 and 46. */
extern u8 Data_03001e50[];

/* Value_ symbols carry a literal the reference loads from its pool rather
   than materializing with a mov. */
extern u8 Value_000000b4;

void Func_080cd594(s32);
void Func_080e0524(s32, void *, s32, s32);
s32 Func_080ed408(s32, s32, s32, s32, s32);
s32 Func_08002322(s32);
s32 Func_0800231c(s32);
void Func_080041d8(s32, s32);
void Func_080f9010(s32);
void Func_080b50e8(s32);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080cd52c(void);
void Func_080030f8(s32);
void Func_08002dd8(s32);
void Func_08004278(s32);
s32 Func_080cdbc0(void);

/* Six animation cells, one entry each: width, height, vertical bias, and the
   byte offset of the cell inside the kind-39 work block.  The three byte
   tables sit six apart and every cell's width * height matches its source
   span, so the cells are 8bpp and the widths are also the source pitch. */
extern u8 Data_080ede9f[];
extern u8 Data_080edea5[];
extern u8 Data_080edeab[];
extern u16 Data_080edeb2[];

#define CELL_W Data_080ede9f
#define CELL_H Data_080edea5
#define CELL_DY Data_080edeab
#define CELL_SRC Data_080edeb2

/* One 28-byte record; the array starts at work + 0x7080. */
typedef struct Puff {
    s32 x;
    s32 y;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 tick;
} Puff;

/* The caller's effect state, republished at work + 0x7828. */
typedef struct Efx {
    s32 kind;
    s32 side;
    s32 actor;
    s32 unk0C;
    s32 unk10;
    s32 cnt;
    s32 layers;
    s32 unk1C;
    s32 unk20;
    s16 actors[8];
} Efx;

#define WORK_EFX (*(Efx **)(work + 0x7828))

void Func_080d9fc8(Efx *efx)
{
    u32 *cache;
    u32 *entry;
    u8 *work;
    void *dst;
    s32 frame;
    DrawRectangle draw;
    s32 i;
    s32 cell;
    u32 wide;
    s32 sign;
    s32 org;
    s32 ang;
    s32 tick;
    Puff *puff;
    Puff *cur;

    cache = (u32 *)(Data_03001e50 + 39 * 4);
    entry = cache;
    work = (u8 *)*entry++;
    dst = (void *)*entry;
    WORK_EFX = efx;
    Func_080cd594(0);
    *(s16 *)0x04000050 = 0x3F46;
    *(s16 *)0x04000052 = 0x100E;
    Func_080e0524((s32)&Value_000000b4, work, 1, 1);
    Func_080ed408(46, 7, 7, 3, 3);
    draw = (DrawRectangle)cache[46 - 39];
    if (WORK_EFX->actors[0] > 127) {
        org = 0;
        sign = 1;
    } else {
        org = 64;
        sign = -1;
    }
    ang = -0x4000;
    i = 0;
    tick = 0;
    puff = (Puff *)(work + 0x7080);
    do {
        puff->x = ((sign * ((Func_08002322(ang) << 5) >> 16)) + org) + 20;
        puff->y = ((Func_0800231c(ang) << 4) >> 16) + 40;
        puff->tick = tick;
        ang += 0x1000;
        tick -= 4;
        i += 1;
        puff++;
    } while (i != 9);
    *(s32 *)(work + 0x7780) = 2;
    if (WORK_EFX->layers == 2) {
        *(s32 *)(work + 0x7784) = 75;
    } else {
        *(s32 *)(work + 0x7784) = 50;
    }
    Func_080041d8(0x080CD261, 0x480);
    Func_080f9010(0x88);
    frame = 0;
    do {
        if (frame == 24) {
            Func_080b50e8(0x85);
        }
        i = 0;
        cur = (Puff *)(work + 0x7080);
        do {
            /* Negative ticks stagger the puffs; the folded range test is
               what the reference's single unsigned compare came from. */
            if ((cur->tick >= 0) && (cur->tick <= 23)) {
                cell = cur->tick / 4;
                /* wide is loaded once and reused for the centring shift and
                   the width argument, then reloaded per layer because the
                   call clobbers it. */
                draw(dst, work + CELL_SRC[cell],
                    cur->x - ((wide = CELL_W[cell]) >> 1),
                    cur->y + CELL_DY[cell],
                    wide, CELL_H[cell]);
                if (WORK_EFX->layers != 0) {
                    draw(dst, work + CELL_SRC[cell],
                        cur->x - ((wide = CELL_W[cell]) >> 1),
                        (cur->y + CELL_DY[cell]) - 16,
                        wide, CELL_H[cell]);
                }
                if (WORK_EFX->layers == 2) {
                    draw(dst, work + CELL_SRC[cell],
                        cur->x - ((wide = CELL_W[cell]) >> 1),
                        (cur->y + CELL_DY[cell]) - 32,
                        wide, CELL_H[cell]);
                }
            }
            i += 1;
            cur->tick += 1;
            cur++;
        } while (i != 9);
        i = 0;
        while (i != WORK_EFX->cnt) {
            if (frame == (i * 8) + 16) {
                Func_080d6888(WORK_EFX->actors[i], 10, 5, i, 12);
            }
            i += 1;
        }
        Func_080cd52c();
        *(s32 *)(work + 0x7824) = 1;
        Func_080030f8(1);
        frame += 1;
    } while (frame != 80);
    Func_08002dd8(46);
    Func_08004278(0x080CD261);
    Func_080cdbc0();
}
