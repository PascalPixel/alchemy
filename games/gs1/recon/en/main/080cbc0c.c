#include "shared-aggregates.h"

#define BattleEffect_RunTileAndPaletteAnimation Func_080cbc0c

/*
 * Blocking battle effect scene at 0x080cbc0c.
 *
 * Allocates three heap blocks (scene work 0x782c, a 0x4000 staging surface and
 * a 0x302 scratch), programs the background/window/blend registers, builds a
 * 16-row by 8-column screen-block tilemap at 0x06003800 (one palette bank per
 * row, tiles from 0x100 up) and loads the effect graphics, then
 * runs 0x80 frames.  Each frame it fades the object palette towards the table
 * at sys+0x544, rasterizes a growing ring into the 8bpp work buffer at
 * 0x02010000 (tile-major, 256 px wide), blits 0x21 sprite pieces through the
 * two routines published by Func_080ed408, and finally releases everything.
 *
 * Still uncertain: the meaning of the 0x1c-byte particle records at
 * work+0x7080 (only offsets 0, 4, 0xc, 0x10 and 0x18 are touched here), the
 * exact roles of the scene flags at work+0x7780/0x7784/0x7824, and the source
 * tables at 0x080ee016 / 0x080ee037 / 0x080edf90 / 0x080edfb1 / 0x080edfd2.
 * Those five tables are spelled as integer addresses because the unit declares
 * no data symbols for them; that costs the reference's reload-and-index form
 * in the sprite loop.  This is a measured draft, not an exact match.
 */

/* Fixed IWRAM helpers reached through a register; the ROM calls them through
   the _call_via_rN veneers, so they are modelled as typed indirect calls. */
typedef void (*ClearFn)(void *dst, s32 size);
typedef void (*FillFn)(void *dst, s32 size, u32 value);
typedef void (*CopyFn)(void *dst, const void *src, s32 size);
/* Rectangle blitter published in absolute_03001e50 by Func_080ed408. */
typedef void (*BlitFn)(void *dst, const void *src, s32 x, s32 y, s32 w, s32 h);

#define IWRAM_CLEAR ((ClearFn)0x03000164)
#define IWRAM_FILL ((FillFn)0x03000168)
#define IWRAM_COPY ((CopyFn)0x03001388)

/* 8bpp tile-major work buffer, 32 tiles across. */
#define WORK_PIXELS 0x02010000

/*
 * A pixel offset inside that surface is (y / 8) * 0x800 + (x / 8) * 0x40
 * + (y & 7) * 8 + (x & 7).  The ring plotter builds the column part once per
 * column and the row part once per row, so the eight mirrored points of a
 * Bresenham step share four subexpressions.
 */
#define PLOT_AT(off) (*(u8 *)(WORK_PIXELS + (off)) = 2)

void *Func_080048b0(s32 id, s32 size);
void Func_08002dd8(s32 id);
void Func_080030f8(s32 arg0);
void Func_080041d8(s32 entry, s32 arg1);
void Func_08004278(s32 entry);
s32 Func_08004458(void);
void Func_080b5028(s32 a, s32 b, s32 c, s32 d);
void Func_080b5038(s32 a, s32 b, s32 c);
void Func_080b5040(s32 a, s32 b, s32 c);
void Func_080cd508(void);
void Func_080cd52c(void);
void Func_080d6888(s32 set, s32 object, s32 group, s32 slot, s32 value);
void Func_080e0524(s32 id, void *dst, s32 arg2, s32 arg3);
void Func_080e3908(void *ent, s32 arg1, s32 arg2);
void Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080f9010(s32 id);

/* Only the m2c spellings this draft actually uses. */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

void BattleEffect_RunTileAndPaletteAnimation(void *arg0) {
    struct M2cAggregate_absolute_02010000 *work;
    struct M2cAggregate_absolute_02010000 *canvas;
    struct M2cAggregate_absolute_02010000 *ent;
    struct M2cAggregate_deref_absolute_03001e74_0 *sys;
    struct M2cAggregate_deref_absolute_03001e74_8c *ctl;
    ClearFn clr;
    FillFn fill;
    BlitFn draw0;
    BlitFn draw1;
    s32 frame;
    s32 rad;
    s32 radMax;
    s32 err;
    s32 px;
    s32 py;
    s32 cnt;
    s32 idx;
    s32 col;
    s32 row;
    s32 pos;
    s32 base;
    s32 ent0;
    s32 ent1;
    s32 sx;
    s32 sy;
    s32 lvl;
    s32 xa;
    s32 xa0;
    s32 xa2;
    s32 xb;
    s32 xb2;
    s32 ya;
    s32 yb;
    s32 ua;
    s32 ua0;
    s32 ua2;
    s32 ub;
    s32 ub2;
    s32 va;
    s32 vb;
    s32 oxa;
    s32 oxb;
    s32 hya;
    s32 hyb;
    s32 tya;
    s32 tyb;
    s32 oua;
    s32 oub;
    s32 hva;
    s32 hvb;
    s32 tva;
    s32 tvb;
    s32 r;
    s32 g;
    s32 b;
    s32 tr;
    s32 tg;
    s32 tb;
    u32 cur;
    u32 tgt;
    u32 span;
    u8 *slot;
    u8 *tblx;
    u8 *tbly;
    u16 *dst;
    u16 *src;
    s16 *pal;
    s32 ime;
    s32 qcnt;

    work = Func_080048b0(0x27, 0x782C);
    canvas = Func_080048b0(0x28, 0x4000);
    Func_080048b0(0x29, 0x302);
    sys = absolute_03001e74.field_0000;
    ctl = absolute_03001e74.field_008c;
    M2C_FIELD(work, void **, 0x7828) = arg0;
    Func_080cd508();
    ctl->field_000c = 1;
    absolute_03001ad0.field_0006 = 0x20;
    Func_080b5038(1, M2C_FIELD(sys, u16 *, 0x648), 0);
    absolute_0400000c.field_0000 = 0x784;
    Func_080b5028(0, 0, 0, 0x64);
    idx = 0;
    ctl->field_000c = 0;
    *(s32 *)0x04000028 = 0;
    *(s32 *)0x0400002C = 0xFFFFF000;
    *(s16 *)0x04000020 = 0x80;
    *(s16 *)0x04000022 = 0;
    *(s16 *)0x04000024 = 0;
    *(s16 *)0x04000026 = 0x100;
    *(s16 *)0x04000040 = 0xF0;
    *(s16 *)0x04000044 = 0x1088;
    *(s16 *)0x04000042 = 0xF0;
    *(s16 *)0x04000046 = 0x1088;
    *(s16 *)0x04000048 = 0x3537;
    *(s16 *)0x0400004A = 0x3F21;

    /* 16 rows of 8 map entries, one screen block at 0x06003800. */
    pos = 0;
    row = 0;
    base = 0;
    do {
        ent0 = base + 0x100;
        col = 0;
        ent1 = idx * 2;
        do {
            col += 1;
            *(s16 *)(0x06003800 + pos) = (s16) (ent0 | ent1);
            ent0 += 0x200;
            ent1 += 2;
            pos += 2;
        } while (col != 8);
        row += 1;
        base += 0x1000;
        idx += 8;
    } while (row != 0x10);

    clr = IWRAM_CLEAR;
    clr(canvas, 0x4000);
    clr((void *)0x06004000, 0x4000);

    ime = *(u16 *)0x04000208;
    *(u16 *)0x04000208 = 0x04000208;
    qcnt = absolute_02002090.field_0000;
    if (qcnt <= 0x1F) {
        slot = (u8 *)&absolute_02002090 + (qcnt * 0xC);
        absolute_02002090.field_0000 = qcnt + 1;
        M2C_FIELD(slot, s32 *, 4) = 0x7741;
        M2C_FIELD(slot, s32 *, 8) = 0x04000000;
        M2C_FIELD(slot, s32 *, 12) = 0x20000;
    }
    *(u16 *)0x04000208 = (u16) ime;

    *(s16 *)0x04000052 = 0x1010;
    *(s16 *)0x04000050 = 0;
    Func_080e0524(0x44, work, 1, 1);
    M2C_FIELD(work, s32 *, 0x7780) = 1;
    M2C_FIELD(work, s32 *, 0x7784) = 0;
    Func_080041d8(0x080CD261, 0x480);
    Func_080ed408(0x2E, 7, 7, 3, 1);
    draw0 = (BlitFn)absolute_03001e50.field_00b8;
    Func_080ed408(0x2F, 7, 7, 3, 2);
    draw1 = (BlitFn)absolute_03001e50.field_00bc;

    /* Seed the 0x21 pieces from the two byte tables of start coordinates. */
    rad = 0;
    tbly = (u8 *)0x080EE037;
    ent = (void *)((u8 *)work + 0x7080);
    tblx = (u8 *)0x080EE016;
    cnt = 0;
    do {
        sx = *tblx << 0x10;
        sy = *tbly << 0x10;
        M2C_FIELD(ent, s32 *, 0) = sx;
        ent->field_0004 = sy;
        cnt += 1;
        ent->field_000c = (u32) ((s32) (sx + 0xFFE00000) >> 2);
        ent->field_0010 = (u32) ((s32) (sy + 0xFFC40000) >> 2);
        tblx += 1;
        tbly += 1;
        ent = (void *)((u8 *)ent + 0x1C);
    } while (cnt != 0x21);

    IWRAM_COPY(&absolute_02010000, (void *)0x06008000, 0x7800);
    fill = IWRAM_FILL;
    fill(&absolute_02010000, 0x7800, 0x01010101);
    ctl->field_0010 = 1;
    M2C_FIELD(work, s32 *, 0x77A0) = (s32) absolute_03001ad0.field_0004;
    M2C_FIELD(work, s32 *, 0x77A4) = (s32) absolute_03001ad0.field_0006;
    absolute_03001ad0.field_0004 = 0;

    ime = *(u16 *)0x04000208;
    *(u16 *)0x04000208 = 0x04000208;
    qcnt = absolute_02002090.field_0000;
    if (qcnt <= 0x1F) {
        slot = (u8 *)&absolute_02002090 + (qcnt * 0xC);
        absolute_02002090.field_0000 = qcnt + 1;
        M2C_FIELD(slot, s32 *, 4) = 0x1F81;
        M2C_FIELD(slot, s32 *, 8) = (u32) &absolute_0400000a;
        M2C_FIELD(slot, s32 *, 12) = 0x20000;
    }
    *(u16 *)0x04000208 = (u16) ime;

    fill((void *)0x050000C0, 0x100, 0x7FFF7FFF);
    Func_080f9010(0xD4);
    Func_080d6888(M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 0x24),
                  7, 3, 0, 0x1E);

    frame = 0;
    do {
        if (frame == 2) {
            Func_080f9010(0xD4);
        }
        if (frame == 3) {
            Func_080f9010(0xD4);
        }
        if (frame == 0x1C) {
            Func_080d6888(
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 0x24),
                -1, 3, -1, 0);
        }
        if (frame == 0x20) {
            Func_080f9010(0x95);
        }
        if (frame == 5) {
            Func_080f9010(0x91);
            absolute_03001ad0.field_0004 = (u16) M2C_FIELD(work, s32 *, 0x77A0);
            Func_080b5040(1, M2C_FIELD(sys, u16 *, 0x648), -1);
        }

        /* Step every object-palette entry one level towards its target. */
        if (frame > 7) {
            dst = (u16 *)0x050000C0;
            cnt = 0;
            src = (u16 *)((u8 *)sys + 0x544);
            do {
                cur = *dst;
                r = cur & 0x1F;
                cur <<= 0x10;
                tgt = *src;
                g = (cur >> 0x15) & 0x1F;
                b = (cur >> 0x1A) & 0x1F;
                tr = tgt & 0x1F;
                tgt <<= 0x10;
                span = tgt;
                tg = (tgt >> 0x15) & 0x1F;
                src += 1;
                tb = (span >> 0x1A) & 0x1F;
                if (r < tr) {
                    r += 1;
                } else if (r > tr) {
                    r -= 1;
                }
                if (g < tg) {
                    g += 1;
                } else if (g > tg) {
                    g -= 1;
                }
                if (b < tb) {
                    b += 1;
                } else if (b > tb) {
                    b -= 1;
                }
                cnt += 1;
                *dst = (b << 0xA) | (g << 5) | r;
                dst += 1;
            } while (cnt != 0x80);
        }

        if (frame == 4) {
            IWRAM_FILL((void *)0x06008000, 0x7800, 0x02020202);
        }

        /* Frames 0..3 rasterize the opening ring into the work buffer. */
        if (frame <= 3) {
            lvl = (frame * 4) + 8;
            radMax = frame << 5;
            *(s16 *)0x05000004 = (lvl << 0xA) | (lvl << 5) | lvl;
            if (rad != radMax) {
                do {
                    px = rad;
                    py = 0;
                    err = rad;
                    if (px >= 0) {
                        do {
                            xa = 0x60 - px;
                            ya = 0x3C - py;
                            xb = px + 0x60;
                            xa0 = xa;
                            yb = py + 0x3C;
                            if (ya < 0) {
                                ya = 0;
                            }
                            if (yb > 0x77) {
                                yb = 0x77;
                            }
                            if (xa < 0) {
                                xa = 0;
                            }
                            if (xb > 0xFF) {
                                xb = 0xFF;
                            }
                            oxb = (xb & 7) + ((xb / 8) << 6);
                            hyb = (yb & 7) * 8;
                            tyb = (yb / 8) << 0xB;
                            PLOT_AT(hyb + oxb + tyb);
                            hya = (ya & 7) * 8;
                            tya = (ya / 8) << 0xB;
                            PLOT_AT(tya + (hya + oxb));
                            oxa = (xa & 7) + ((xa / 8) << 6);
                            PLOT_AT(hyb + oxa + tyb);
                            xa2 = xa0 + 1;
                            PLOT_AT(tya + (hya + oxa));
                            xb2 = px + 0x61;
                            if (xa2 < 0) {
                                xa2 = 0;
                            }
                            if (xb2 > 0xFF) {
                                xb2 = 0xFF;
                            }
                            oxb = (xb2 & 7) + ((xb2 / 8) << 6);
                            PLOT_AT(hyb + oxb + tyb);
                            PLOT_AT(tya + (hya + oxb));
                            oxa = (xa2 & 7) + ((xa2 / 8) << 6);
                            PLOT_AT(hyb + oxa + tyb);
                            PLOT_AT(tya + (hya + oxa));

                            ua = 0x60 - py;
                            ub = py + 0x60;
                            ua0 = ua;
                            vb = px + 0x3C;
                            va = 0x3C - px;
                            if (ua < 0) {
                                ua = 0;
                            }
                            if (ub > 0xFF) {
                                ub = 0xFF;
                            }
                            if (va < 0) {
                                va = 0;
                            }
                            if (vb > 0x77) {
                                vb = 0x77;
                            }
                            oub = (ub & 7) + ((ub / 8) << 6);
                            hvb = (vb & 7) * 8;
                            tvb = (vb / 8) << 0xB;
                            PLOT_AT(hvb + oub + tvb);
                            hva = (va & 7) * 8;
                            tva = (va / 8) << 0xB;
                            PLOT_AT(tva + (hva + oub));
                            oua = (ua & 7) + ((ua / 8) << 6);
                            PLOT_AT(hvb + oua + tvb);
                            ua2 = ua0 + 1;
                            PLOT_AT(tva + (hva + oua));
                            ub2 = py + 0x61;
                            if (ua2 < 0) {
                                ua2 = 0;
                            }
                            if (ub2 > 0xFF) {
                                ub2 = 0xFF;
                            }
                            oub = (ub2 & 7) + ((ub2 / 8) << 6);
                            PLOT_AT(hvb + oub + tvb);
                            PLOT_AT(tva + (hva + oub));
                            oua = (ua2 & 7) + ((ua2 / 8) << 6);
                            PLOT_AT(hvb + oua + tvb);
                            PLOT_AT(tva + (hva + oua));

                            err = (err - (py * 2)) - 1;
                            if (err < 0) {
                                err = (err + (px * 2)) - 2;
                                px -= 1;
                            }
                            py += 1;
                        } while (px >= py);
                    }
                    rad += 1;
                } while (rad != radMax);
            }
            IWRAM_COPY((void *)0x06008000, &absolute_02010000, 0x7800);
        }

        if (frame <= 0x32) {
            cnt = 0;
            ent = (void *)((u8 *)work + 0x7080);
            do {
                draw0(canvas,
                      &work->unknown_0000[*(u16 *)(0x080EDFD2 + (cnt * 2))],
                      M2C_FIELD(ent, s16 *, 2), M2C_FIELD(ent, s16 *, 6),
                      (s32) *(u8 *)(0x080EDF90 + cnt),
                      (s32) *(u8 *)(0x080EDFB1 + cnt));
                if (frame > 3) {
                    Func_080e3908(ent, 0x40, 0x4000);
                }
                cnt += 1;
                ent = (void *)((u8 *)ent + 0x1C);
            } while (cnt != 0x21);
        }

        /* Frames 8..0x32 ramp the backdrop colour up to full white. */
        span = frame - 8;
        if (span <= 0x2AU) {
            lvl = (s32) span;
            if (lvl > 0x1F) {
                lvl = 0x1F;
            }
            *(s16 *)0x05000002 = (lvl << 0xA) | (lvl << 5) | lvl;
        }

        if (frame == 0x33) {
            Func_080e0524(0x7D, work, 1, 0);
            pal = (s16 *)0x05000002;
            cnt = 1;
            do {
                lvl = cnt / 2;
                if (lvl < 0) {
                    lvl = 0;
                }
                cnt += 1;
                *pal = (lvl << 0xA) | ((lvl / 2) << 5) | lvl;
                pal += 1;
            } while (cnt != 0x40);
            absolute_04000050.field_0000 = 0x3F44;
            cnt = 0;
            ent = (void *)((u8 *)work + 0x7080);
            do {
                M2C_FIELD(ent, s32 *, 0) =
                    (s32) (((Func_08004458() & 0x1F) + 0x20) << 0x10);
                ent->field_0004 = ((Func_08004458() & 0x1F) + 0x50) << 0x10;
                cnt += 1;
                ent->field_000c =
                    ((0x1FF & Func_08004458()) + 0xFFFFFF00) << 0xC;
                ent->field_0010 = 0;
                ent->field_0018 = 0;
                ent = (void *)((u8 *)ent + 0x1C);
            } while (cnt != 0x20);
            M2C_FIELD(work, s32 *, 0x7780) = 2;
            M2C_FIELD(work, s32 *, 0x7784) = 0x32;
        }

        /* After frame 0x34 the pieces scatter, one released every four
           frames, each stepping through six 0x800-byte sprite phases. */
        if (frame > 0x34) {
            cnt = 0;
            ent = (void *)((u8 *)work + 0x7080);
            do {
                if (frame >= (cnt / 4) + 0x34) {
                    span = ent->field_0018;
                    if ((s32) span <= 0x27) {
                        lvl = (s32) span / 4;
                        if (lvl > 5) {
                            lvl = 5;
                        }
                        draw1(canvas, (u8 *)work + (lvl << 0xB),
                              M2C_FIELD(ent, s16 *, 2) - 0x10,
                              M2C_FIELD(ent, s16 *, 6) - 0x20, 0x20, 0x40);
                        Func_080e3908(ent, 0x3C, 0xFFFFF000);
                        ent->field_0018 += 1;
                    }
                }
                cnt += 1;
                ent = (void *)((u8 *)ent + 0x1C);
            } while (cnt != 0x20);
        }

        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        frame += 1;
    } while (frame != 0x80);

    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    Func_08004278(0x080CD261);
    Func_080d6888(M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 0x24),
                  -1, 1, -1, 0);
    absolute_03001ad0.field_0004 = (u16) M2C_FIELD(work, s32 *, 0x77A0);
    absolute_03001ad0.field_0006 = 0x20;
    Func_080b5038(2, M2C_FIELD(sys, u16 *, 0x648), 0);
    Func_080030f8(1);

    ime = *(u16 *)0x04000208;
    *(u16 *)0x04000208 = 0x04000208;
    qcnt = absolute_02002090.field_0000;
    if (qcnt <= 0x1F) {
        slot = (u8 *)&absolute_02002090 + (qcnt * 0xC);
        absolute_02002090.field_0000 = qcnt + 1;
        M2C_FIELD(slot, s32 *, 4) = 0x7541;
        M2C_FIELD(slot, s32 *, 8) = 0x04000000;
        M2C_FIELD(slot, s32 *, 12) = 0x20000;
    }
    *(u16 *)0x04000208 = (u16) ime;

    Func_08002dd8(0x29);
    Func_08002dd8(0x28);
    Func_08002dd8(0x27);
}
