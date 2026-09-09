#include "types.h"

/* Per-frame driver for the five-reel symbol minigame.
 *
 * One call advances the whole screen: it samples the pad, runs the small
 * state machine that owns betting, spinning, holding and paying out, then
 * rebuilds the complete 128-entry object buffer at work + 200 and hands it
 * to DMA3 for the next vblank.  Each reel is a 21-symbol ring scrolled
 * sixteen units per cell; the seven "lines" tested once every reel has
 * settled are the five columns plus the two diagonals, and how many of them
 * count is gated by the current bet.
 *
 * Uncertain and left neutral: the meaning of item 228 (only its count is
 * read and consumed), the roles of the heap cells at 0x7780, 0x7784 and
 * 0x778c, and everything in the work block this function does not touch.
 * The duplicated "state = 20" in both arms of the coin test at the end of
 * the payout branch is reproduced as the reference emits it.
 *
 * Residual: every block here follows the reference, but the reference holds
 * the work pointer in r7 while this source makes the register allocator put
 * it in r8.  That one difference costs a register move at nearly every field
 * access, stops the object writes from using the [work, offset] register
 * form, and lets the compiler tail-merge the five-way stores in the last
 * loop that the reference keeps apart with one induction variable each.  It
 * is an allocation outcome, not a control-flow or type disagreement; no
 * evidenced source spelling found so far moves the pointer back to a low
 * register. */

#define ReelGame_RunFrame Func_080f6440

u32 Func_08004458(void);
s32 Func_080022fc(s32 value, s32 modulus);
s32 Func_08002322(s32 angle);
s32 Func_08015010(s32 x, s32 y, s32 w, s32 h, s32 flags);
void Func_08015018(s32 window, s32 flags);
void Func_08015080(s32 message, s32 window, s32 x, s32 y);
void Func_080150b0(s32 value, s32 digits, s32 window, s32 x, s32 y);
void Func_08077048(s32 item);
s32 Func_080772e0(s32 item);
void Func_080f9010(s32 cue);

extern u8 Data_0200024c[];
extern u8 Data_080f870c[];
extern u8 Data_080f8712[];
extern u8 Data_080f871a[];
extern u8 Data_080f8728[];

typedef struct {
    s32 pos;      /* +0   ring position, sixteen units per symbol, wraps at 336 */
    u8 cell[21];  /* +4   symbol ring */
    u8 held;      /* +25  row is held by the player */
    u8 stop;      /* +26  frames left before the row settles, 0xff while free */
    u8 unk_1b;    /* +27 */
} ReelRow;

typedef struct {
    ReelRow row[5];   /* +0    */
    s32 state;        /* +140  */
    s32 cursor;       /* +144  0..4 pick a row, 5 picks the confirm slot */
    s32 spins;        /* +148  */
    s32 bet;          /* +152  */
    u16 keys;         /* +156  pad seen last frame */
    u16 dir;          /* +158  direction bits, cleared while the repeat runs */
    u16 pressed;      /* +160  newly pressed bits */
    u16 repeat;       /* +162  */
    u8 unk_a4[4];     /* +164  */
    s32 timer;        /* +168  */
    s32 line[7];      /* +172  per-line win flags */
    u32 obj[128][2];  /* +200  object attribute buffer */
    s32 win;          /* +1224 */
    s32 sub_win;      /* +1228 */
    u8 unk_4d0[8];    /* +1232 */
    u32 fade[80];     /* +1240 per-scanline source for the HBlank transfer */
    s32 phase;        /* +1560 which prompt the window currently shows */
} ReelWork;

typedef union {
    s32 v;
    struct {
        u16 frac;
        s16 whole;
    } h;
} ReelFixed;

typedef struct {
    ReelFixed x;   /* +0  */
    ReelFixed y;   /* +4  */
    s32 unk_08;
    s32 unk_0c;
    s32 vel;       /* +16 */
    s32 unk_14;
    s32 bounce;    /* +24 */
} ReelSpark;

void ReelGame_RunFrame(void)
{
    ReelWork *work;
    u8 *heap;
    u32 *dst;
    volatile u16 *reg;
    u32 pad;
    s32 blend;
    s32 oam;
    s32 coins;
    s32 all;
    s32 cnt;
    s32 hits;
    s32 found;
    s32 mixed;
    s32 col;
    s32 v;
    s32 x;
    s32 y;
    u32 i;
    u32 j;

    work = *(ReelWork **)0x03001f04;
    heap = *(u8 **)0x03001eec;
    blend = 0x400;
    oam = 0;

    Func_08004458();

    reg = (volatile u16 *)0x040000b0;
    reg[5] &= 0xc5ff;
    reg[5] &= 0x7fff;
    reg[5];

    dst = (u32 *)reg;
    dst[0] = (u32)work->fade;
    dst[1] = 0x04000054;
    dst[2] = 0xa2600001;

    pad = *(volatile u32 *)0x03001ae8;
    work->pressed = (u16)pad & ~work->keys;
    work->dir = (u16)(*(volatile u32 *)0x03001ae8 & 0xf0);
    if ((work->keys & 0xf0) == work->dir) {
        if (work->repeat > 12)
            work->repeat = 12;
        if (work->repeat == 0) {
            work->repeat = 4;
        } else {
            work->repeat -= 1;
            work->dir = 0;
        }
    } else {
        work->repeat = 12;
    }
    work->keys = (u16)pad;

    if (*(u8 *)0x03001d20 != 0)
        goto build_objects;

    if (work->state == 0) {
        coins = Func_080772e0(228);
        Func_080150b0(coins - work->bet, 2, work->sub_win, 64, 0);
        Func_080150b0(work->bet, 2, work->sub_win, 64, 8);
        if ((work->pressed & 2) != 0) {
            work->state = 10;
            Data_0200024c[0x120] = 254;
            Func_08015018(work->win, 1);
            goto build_objects;
        }
        if ((work->pressed & 0x40) != 0) {
            if (work->bet <= 3 && coins > work->bet) {
                work->bet += 1;
                Func_080f9010(111);
            } else {
                Func_080f9010(113);
            }
        }
        if ((work->pressed & 0x80) != 0) {
            if (work->bet > 1) {
                work->bet -= 1;
                Func_080f9010(111);
            } else {
                Func_080f9010(113);
            }
        }
        reg = (u16 *)0x04000050;
        *reg++ = 0x3fd0;
        *reg = 0x0010;
        if ((work->pressed & 1) == 0)
            goto build_objects;
        work->state = 1;
        *(s32 *)(heap + 0x778c) = 0;
        Func_08015018(work->win, 1);
        for (i = 0; i < (u32)work->bet; i++)
            Func_08077048(228);
        Func_08015018(work->sub_win, 1);
        Func_080f9010(0x130);
        goto build_objects;
    }

    if (work->state == 5) {
        work->timer += 1;
        all = 0;
        for (i = 0; i < 5; i++) {
            if (work->row[i].held == 0)
                break;
        }
        if (i == 5)
            all = 1;
        if ((work->pressed & 1) != 0) {
            work->timer = 0;
            *(s32 *)(heap + 0x778c) = 0;
            if (work->spins == 4) {
                work->spins = 0;
                work->cursor = 0;
                work->state = 0;
                for (i = 0; i < 5; i++) {
                    work->row[i].held = 0;
                    work->row[i].stop |= 255;
                }
            } else if (work->cursor <= 4) {
                Func_080f9010(0x131);
                work->row[work->cursor].held ^= 1;
            } else if (all == 0) {
                Func_080f9010(0x130);
                work->state = 1;
                work->cursor = 0;
                for (i = 0; i < 5; i++)
                    work->row[i].stop |= 255;
                work->spins += 1;
            } else {
                Func_080f9010(113);
            }
        } else {
            if ((work->dir & 16) != 0) {
                work->cursor = Func_080022fc(work->cursor + 1, 6);
                Func_080f9010(111);
            }
            if ((work->dir & 32) != 0) {
                work->cursor = Func_080022fc(work->cursor + 5, 6);
                Func_080f9010(111);
            }
        }

        if (work->state == 5) {
            if (work->cursor == 5) {
                if (all != 0) {
                    if ((u32)(work->phase - 1) > 1) {
                        Func_08015018(work->win, 1);
                        work->win = Func_08015010(11, 0, 19, 4, 6);
                        Func_08015080(0x912, work->win, 0, 0);
                        work->phase = 1;
                    } else if (work->phase == 1) {
                        Func_08015080(0x913, work->win, 0, 8);
                        work->phase = 2;
                    }
                } else {
                    if (work->phase != 3) {
                        Func_08015018(work->win, 1);
                        work->win = Func_08015010(16, 0, 14, 3, 6);
                        Func_08015080(0x90f, work->win, 0, 0);
                    }
                    work->phase = 3;
                }
            } else if (work->row[work->cursor].held == 0) {
                if (work->phase != 4) {
                    Func_08015018(work->win, 1);
                    work->win = Func_08015010(23, 0, 7, 3, 6);
                    Func_08015080(0x90d, work->win, 0, 0);
                }
                work->phase = 4;
            } else {
                if (work->phase != 5) {
                    Func_08015018(work->win, 1);
                    work->win = Func_08015010(23, 0, 7, 3, 6);
                    Func_08015080(0x90e, work->win, 0, 0);
                }
                work->phase = 5;
            }
        } else {
            Func_08015018(work->win, 1);
        }
        goto build_objects;
    }

    if (work->state == 2) {
        work->timer += 1;
        blend = 0;
        if (work->timer != 60)
            goto build_objects;
        work->state = 3;
        Func_080f9010(93);
        work->timer = 0;
        reg = (u16 *)0x04000050;
        *reg++ = 0x3f44;
        *reg = 0x1010;
        *(s32 *)(heap + 0x7780) = 2;
        *(s32 *)(heap + 0x7784) = 75;
        goto build_objects;
    }

    if (work->state == 3) {
        work->timer += 1;
        blend = 0;
        if ((work->pressed & 1) == 0)
            goto build_objects;
        work->state = 10;
        Func_080f9010(112);
        goto build_objects;
    }

    if (work->state == 11) {
        if (work->phase == 0) {
            work->phase = 1;
            Func_08015080(0x90c, work->win, 0, 8);
        }
        if ((work->pressed & 1) == 0)
            goto build_objects;
        work->state = 5;
        work->phase = 0;
        Func_080f9010(112);
        Func_08015018(work->win, 1);
        goto build_objects;
    }

    if (work->state == 20) {
        work->timer += 1;
        if (work->timer != 45)
            goto build_objects;
        work->state = 10;
        goto build_objects;
    }

    if (work->state == 10)
        goto build_objects;

    /* Reels are turning. */
    if (work->timer == 4) {
        work->win = Func_08015010(18, 17, 12, 3, 6);
        Func_08015080(0x90a, work->win, 0, 0);
    }
    if (work->timer == 16)
        Func_080f9010(0x132);
    if (work->timer > 56) {
        if (*(s32 *)(heap + 0x778c) > 31 || (work->pressed & 0x100) != 0) {
            *(s32 *)(heap + 0x778c) = 0;
            for (i = 0; i < 5; i++) {
                if (work->row[i].held == 0 && (s8)work->row[i].stop == -1) {
                    work->row[i].stop = (Func_08004458() & 3) + 4;
                    Func_080f9010(0x133);
                    break;
                }
            }
        }
    }

    for (i = 0; i < 5; i++) {
        if ((s8)work->row[i].stop > 0)
            work->row[i].stop -= 1;
    }

    cnt = 0;
    for (i = 0; i < 5; i++) {
        if (work->row[i].held == 1 ||
            ((s8)work->row[i].stop == 0 && (work->row[i].pos & 15) == 8))
            cnt += 1;
    }

    if (cnt == 5) {
        hits = 0;
        for (col = 0; col < 7; col++) {
            work->line[col] = 0;
            mixed = 0;
            found = -1;
            if (col > 3 - work->bet && col < work->bet + 3) {
                for (i = 0; i < 5; i++) {
                    if (col == 0)
                        v = i - (work->row[i].pos / 16) + 22;
                    else if (col == 6)
                        v = -i - (work->row[i].pos / 16) + 26;
                    else
                        v = col - (work->row[i].pos / 16) + 21;
                    v = work->row[i].cell[Func_080022fc(v, 21)];
                    if (v != 5) {
                        if (found == -1)
                            found = v;
                        else if (found != v)
                            mixed = 1;
                    }
                }
                if (mixed == 0) {
                    work->line[col] = 1;
                    Data_0200024c[0x120 + hits] = Data_080f870c[found];
                    hits += 1;
                }
            }
        }
        work->timer = 0;
        if (hits != 0) {
            Data_0200024c[0x120 + hits] = 0xff;
            work->state = 2;
            Func_080f9010(171);
            *(s32 *)(heap + 0x7780) = 1;
            *(s32 *)(heap + 0x7784) = 0;
            *(u16 *)0x04000050 = 0;
            Func_08015018(work->win, 1);
        } else {
            work->state = 11;
            work->phase = 0;
            Func_08015018(work->win, 1);
            work->win = Func_08015010(3, 16, 24, 4, 6);
            Func_08015080(0x90b, work->win, 0, 0);
            if (work->spins == 4) {
                coins = Func_080772e0(228);
                if (coins > 0)
                    work->state = 20;
                else
                    work->state = 20;
                if (work->bet > coins)
                    work->bet = coins;
                work->spins = 0;
                work->cursor = 0;
                for (i = 0; i < 5; i++) {
                    work->row[i].held = 0;
                    work->row[i].stop |= 255;
                }
                *(s32 *)(heap + 0x7780) = 1;
                *(s32 *)(heap + 0x7784) = 0;
                *(u16 *)0x04000050 = 0;
                work->sub_win = Func_08015010(18, 0, 12, 4, 6);
                Func_08015080(0x905, work->sub_win, 0, 8);
                Func_08015080(0x904, work->sub_win, 0, 0);
            }
        }
    }

    if (work->state == 1) {
        for (i = 0; i < 5; i++) {
            if (work->row[i].held == 0) {
                if ((s8)work->row[i].stop != 0 || (work->row[i].pos & 15) != 8) {
                    v = work->row[i].pos + 8;
                    work->row[i].pos = v;
                } else {
                    v = work->row[i].pos;
                }
                if (v == 336)
                    work->row[i].pos = 0;
            }
        }
    }
    *(s32 *)(heap + 0x778c) += 1;
    work->timer += 1;

build_objects:
    if (work->state == 5) {
        x = work->cursor * 36 + 36;
        y = 128;
        v = 0;
        if ((work->timer & 15) <= 7)
            v = 1;
        if (work->cursor == 5) {
            x = 208;
            y = 32;
        }
        work->obj[0][0] = (((x - 12) << 16) | blend | (y + 8)) | 0x80006000;
        work->obj[0][1] = (v << 4) + 0x2b0;
        work->obj[1][0] = (((x + 12) << 16) | blend | (y + 8)) | 0x90006000;
        work->obj[1][1] = (v << 4) + 0x2b0;
        work->obj[2][0] = ((x << 16) | blend | y) | 0x80002000;
        work->obj[2][1] = 0x1f0;
        oam = 3;
    }

    if (work->state == 3) {
        for (i = 0; i < 8; i++) {
            ReelSpark *p = (ReelSpark *)(heap + 0x7080) + i;

            work->obj[oam][0] = (((p->x.h.whole << 16) | blend) |
                                 ((p->y.h.whole + 256) & 255)) | 0x80000000;
            work->obj[oam][1] = ((Data_080f8712[i] << 4) + 0x370) | 0xf000;
            p->y.v += p->vel;
            p->vel += 0x4000;
            if (work->timer % 256 == (s32)(i * 4 + 200)) {
                p->vel = 0x60000;
                p->bounce = 0;
            }
            if (p->y.v > 0x400000) {
                p->y.v = 0x400000;
                if (p->bounce <= 1)
                    p->vel = -p->vel / 2;
                p->bounce += 1;
            }
            oam += 1;
        }
    }

    for (i = 0; i < 14; i++) {
        work->obj[oam][0] =
            ((Data_080f871a[i] << 16) | blend | Data_080f8728[i]) | 0x80006000;
        if (i <= 3)
            work->obj[oam][1] = 0x4e0;
        else
            work->obj[oam][1] = 0x4e8;
        oam += 1;
    }

    x = 0x200000;
    for (i = 0; i < 5; i++) {
        work->obj[oam][0] = (blend | x) | 0x8000207c;
        if (work->row[i].held == 0)
            work->obj[oam][1] = 0x460;
        else
            work->obj[oam][1] = 0x480;
        oam += 1;
        x += 0x240000;
    }

    x = 0x200000;
    for (i = 0; i < 5; i++) {
        work->obj[oam][0] = (blend | x) | 0x80006003;
        if (i == (u32)work->spins)
            work->obj[oam][1] = (i * 32 + 0x210) | 0x400;
        else
            work->obj[oam][1] = (i * 32 + 0x220) | 0x400;
        oam += 1;
        x += 0x100000;
    }

    for (i = 0; i < 7; i++) {
        work->obj[oam][0] = (((((0x204 - ((i & 1) << 3)) & 0x1ff) << 16) |
                              blend) | (i * 16 + 5)) | 0x80002000;
        if ((s32)i > 3 - work->bet && (s32)i < work->bet + 3)
            work->obj[oam][1] = 0x5d0;
        else
            work->obj[oam][1] = 0x510;
        oam += 1;
    }

    x = 0x280000;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 7; j++) {
            work->obj[oam][0] =
                ((j * 16 + (work->row[i].pos % 16) + 4) | x) | 0x80006000;
            v = Func_080022fc(j - (work->row[i].pos / 16) + 21, 21);
            work->obj[oam][1] = (work->row[i].cell[v] << 4) | 0x800;
            oam += 1;
        }
        x += 0x200000;
    }

    y = 40;
    if (work->state == 1) {
        if (work->timer <= 47)
            y = ((Func_08002322(work->timer * 682) << 6) >> 16) + 40;
        else if (work->timer <= 55)
            y = ((Func_08002322((work->timer << 12) - 0x30000) << 2) >> 16) + 40;
    }
    work->obj[oam][0] = (y | blend) | 0x80d06000;
    work->obj[oam][1] = 0x500;
    oam += 1;

    for (i = 0; i < 8; i++) {
        work->obj[oam][0] = (blend | (i * 16 + 12)) | 0x80ce6000;
        if (i == 0)
            work->obj[oam][1] = 0x540;
        else if (i == 1)
            work->obj[oam][1] = 0x550;
        else if (i == 6)
            work->obj[oam][1] = 0x570;
        else if (i == 7)
            work->obj[oam][1] = 0x580;
        else
            work->obj[oam][1] = 0x560;
        oam += 1;
    }

    while (oam != 128) {
        work->obj[oam][0] = 0x40f02000;
        work->obj[oam][1] = 0;
        oam += 1;
    }

    dst = (u32 *)0x040000d4;
    dst[0] = (u32)work->obj;
    dst[1] = 0x07000000;
    dst[2] = (oam * 2) | 0x84000000;
}
