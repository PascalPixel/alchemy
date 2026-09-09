#include "types.h"

/*
 * Battle-presentation full-screen weather scene at 0x080e302c.  The single
 * argument is the effect state pointer; like every other member of the
 * 0x03001eec "battle work" subsystem family it is parked at work + 0x7828
 * (see games/gs1/recon/en/main/080e2538.c, 080e7404.c and 080e01e4.c for the
 * shared prologue, the heap_cache cursor pair and the callee evidence this
 * owner reuses).
 *
 * The state's s32 at 0x18 selects one of three intensity variants and drives
 * BG2PA, the horizontal bias pair, the sprite row picked out of
 * Data_080eeda6, and the spark respawn spread near the end.
 *
 * Setup: decompress resource 0x7b into 0x02010000, DMA palette resource 0x7c
 * into 0x05000000 through the IWRAM copy routine at 0x03001388, darken all
 * 63 foreground palette entries by 8 per channel, then repack the decompressed
 * tiles into work as four runs (a 9-row strip table driven by Data_080eed90,
 * a 32 x 3 block run, a flat 0x3f0-byte run, and a 3-entry width x height run
 * driven by Data_080eeda0 / Data_080eeda3).  Decompress resource 0x73 over
 * heap_cache[2], create eleven kind-0x186 objects into work + 0x77d8,
 * register the two blit routines 0x2e and 0x2f and keep their entry points,
 * program BLDCNT/BLDALPHA and BG3CNT, seed eleven sparks at work + 0x7198 and
 * 256 drops at 0x02010000, then run exactly 192 frames.
 *
 * Each frame cues audio at frames 0, 40, 96 and 120, redraws the two curtain
 * halves while fp <= 0x51, scrolls a four-row band in (fp - 0xc <= 0x4b) and
 * back out again (fp - 0xa0 <= 0x17), draws the two foreground panels while
 * fp - 0x58 <= 0x47, steps and draws the drops once past fp 0x57, spawns and
 * steps the sparks through Func_08009008, pokes every real party member,
 * applies the camera shake, ticks the object group and waits one frame.
 *
 * Neither `Func_080072f0` nor `Func_080072f4`, `Func_080072fc`,
 * `Func_08007304` and `Func_0800730c` is a real function symbol: they are
 * entries 3, 4, 6, 8 and 10 of the `_call_via_rN` trampoline bundle at
 * games/gs1/asm/080072e4.s (base 0x080072e4, four bytes per register), i.e.
 * indirect calls through whatever pointer the compiler kept in that register.
 * Here they are exactly three distinct pointers: the IWRAM copy routine at
 * 0x03001388, and the two blit routines Func_080ed408 publishes into
 * heap_cache[7] / heap_cache[8] (0x03001f08 / 0x03001f0c, reached in the
 * reference through the 0x03001e50 base with +184 / +188, so they are spelled
 * that way here).  Modelling them as direct calls is what cost the previous
 * draft its coverage: every call site lost the pointer load that feeds it.
 *
 * The resource ids 0x73, 0x7b and 0x7c reach their calls through the literal
 * pool rather than `movs`, which the project's external `Value_xxxxxxxx`
 * spelling reproduces; the original source form of those three ids is not
 * recovered.  The Data_ tables are deliberately not `const`: the reference
 * reloads every one of them after an intervening call, which a read-only
 * qualifier would let the compiler hoist away.
 *
 * Still uncertain: the roles of the spark and drop fields beyond the ones
 * this owner reads, the meaning of the 0x7780 / 0x7784 / 0x77a8 / 0x7824
 * words, and whether the six-entry Data_080eedac run really addresses sparks
 * 6..11 of an eleven-entry table or a separate twelve-entry one.  The drop
 * table is seeded with 0x100 entries but the per-frame step and draw only
 * walks the first 0x40 of them; the reference is unambiguous about both
 * counts, so the remaining 0xc0 entries are either used by another owner in
 * this family or simply over-allocated here.
 *
 * Residual: the emitted extent now equals the owner's 2188 bytes and the
 * block topology follows the reference, but register assignment and spill
 * slot numbering still differ throughout -- the reference keeps the frame
 * counter in fp and orders its frame slots 16 row_y / 20 col_x / 24 bias /
 * 28 sprite_src / 32 blit_a / 36 blit_b, where this draft lands on r9 and a
 * different slot order.  Five small constants (0xcc, 0xaa, 0x3f46, 0x1010,
 * 0x785) are materialised by the reference as 32-bit pool words and by this
 * draft as 16-bit ones; spelling them through `Value_xxxxxxxx` symbols made
 * the whole function worse and was reverted.
 */

#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

/* The state pointer this owner parks in, and reloads from, work + 0x7828. */
#define STATE (*(void **)(work + 0x7828))

typedef void (*BlitFn)(
    void *dest, const void *src, s32 x, s32 y, s32 w, s32 h);
typedef void (*CopyFn)(void *dest, const void *src, s32 count);

/* Two adjacent s32 words copied as a unit out of Data_080edab0. */
struct Pair {
    s32 field_0000;
    s32 field_0004;
};

/* Four s32 words handed to Func_08009008 as the spawn transform. */
struct Transform {
    s32 field_0000;
    s32 field_0004;
    s32 field_0008;
    s32 field_000c;
};

/* Eleven entries at work + 0x7198. */
struct Spark {
    s32 pos;
    s32 height;
    s32 fall;
    s32 phase;
    s32 rate;
    s32 unk_14;
    s32 timer;
};

/* 0x100 entries at 0x02010000; pos/height are 16.16. */
struct Drop {
    s32 pos;
    s32 height;
    s32 unk_08;
    s32 unk_0c;
    s32 rate;
    s32 unk_14;
    s32 timer;
};

extern u8 Value_00000073;
extern u8 Value_0000007b;
extern u8 Value_0000007c;

/* heap_cache[7] and heap_cache[8] reached through the 0x03001e50 base. */
extern void *Data_03001e50[];
extern void *Data_03001eec[];

extern struct Pair Data_080edab0;
extern u16 Data_080ede48[];
extern u16 Data_080eed7e[];
extern u8 Data_080eed90[];
extern u16 Data_080eed9a[];
extern u8 Data_080eeda0[];
extern u8 Data_080eeda3[];
extern s8 Data_080eeda6[];
extern u8 Data_080eedac[];

void Func_080cd594(s32 mode);
void Func_080e0524(s32 id, void *target, s32 flag_a, s32 flag_b);
void *Func_08002f40(s32 id);
void *Func_08009030(s32 kind);
void Func_08009020(void *object, s32 index);
void Func_08009008(
    void *object, struct Transform *transform, struct Pair *pair, s32 mode);
void Func_08009038(void *object);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
u32 Func_08004458(void);
s32 Func_08002304(s32 value, s32 range);
s32 Func_080022ec(s32 value, s32 shift);
s32 Func_080022fc(s32 value, s32 divisor);
void Func_080f9010(s32 id);
void Func_080b50e8(s32 id);
void Func_080b5088(s32 member, s32 kind);
void Func_080d6888(s32 member, s32 a, s32 b, s32 index, s32 e);
void Func_080e155c(s32 x, s32 y);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
void Func_080cdbc0(void);

void Func_080e302c(void *object)
{
    void **heap;
    void **cursor;
    u8 *work;
    void *draw_target;
    void *sprite_src;
    BlitFn blit_a;
    BlitFn blit_b;
    struct Spark *sparks;
    struct Spark *spark;
    struct Drop *drops;
    struct Drop *drop;
    const u8 *strip;
    const u8 *w_tbl;
    const u8 *h_tbl;
    const u8 *tbl_end;
    const u8 *src;
    u8 *dst;
    u16 *pal;
    u16 *reg;
    struct Pair pair;
    struct Transform transform;
    s32 bias;
    s32 row_y;
    s32 col_x;
    s32 off;
    s32 slot;
    s32 cnt;
    s32 num;
    s32 i;
    s32 j;
    s32 fp;
    s32 idx;
    s32 spread;
    s32 timer;
    s32 y;
    s32 w;
    s32 h;
    s32 pos;
    s32 mode;
    s32 flags;
    s32 red;
    s32 green;
    s32 blue;
    u16 entry;
    u32 packed;
    void *node;

    heap = Data_03001eec;
    cursor = heap;
    work = (u8 *)*cursor++;
    draw_target = *cursor;
    sprite_src = heap[2];
    STATE = object;
    Func_080cd594(0);

    mode = M2C_FIELD(STATE, s32 *, 0x18);
    if (mode == 0) {
        *(u16 *)0x04000020 = 0xCC;
    } else if (mode == 1) {
        *(u16 *)0x04000020 = 0xAA;
    }

    if (M2C_FIELD(STATE, s32 *, 4) == 1) {
        bias = 8;
        mode = M2C_FIELD(STATE, s32 *, 0x18);
        col_x = 0x28;
        if (mode != 0) {
            col_x = 0x24;
            if (mode != 1) {
                col_x = 0x28;
            }
        }
    } else {
        bias = -0x10;
        col_x = -0xC;
    }

    *(s32 *)0x04000028 =
        Data_080eeda6[M2C_FIELD(STATE, s32 *, 0x18)
            + (M2C_FIELD(STATE, s32 *, 4) * 3)]
        << 8;

    Func_080e0524((s32)&Value_0000007b, (void *)0x02010000, 1, 0);
    ((CopyFn)0x03001388)(
        (void *)0x05000000, Func_08002f40((s32)&Value_0000007c), 0x80);

    /* Darken every foreground palette entry by eight per channel. */
    pal = (u16 *)0x05000002;
    i = 0;
    do {
        entry = *pal;
        packed = (u32)entry << 16;
        red = (s32)((packed >> 0x1A) & 0x1F) - 8;
        green = (s32)((packed >> 0x15) & 0x1F) - 8;
        blue = (s32)(0x1F & entry) - 8;
        if (red < 0) {
            red = 0;
        }
        if (green < 0) {
            green = 0;
        }
        if (blue < 0) {
            blue = 0;
        }
        i++;
        *pal = (u16)((red << 0xA) | (green << 5) | blue);
        pal++;
    } while (i != 0x3F);

    /* Repack the decompressed tiles into work as four consecutive runs. */
    strip = Data_080eed90;
    src = (const u8 *)0x02010000;
    off = 0;
    i = 0;
    do {
        w = *strip;
        j = 0;
        do {
            cnt = 0;
            if (w != 0) {
                const u8 *run;

                dst = work + off;
                run = src;
                do {
                    *dst = *run;
                    cnt++;
                    run++;
                    dst++;
                    off++;
                } while (cnt != Data_080eed90[i]);
            }
            j++;
        } while (j != 0x20);
        i++;
        src += *strip;
        strip++;
    } while (i != 9);

    i = 0;
    do {
        const u8 *end;

        end = src + 0x30;
        j = 0;
        do {
            const u8 *run;

            dst = work + off;
            run = src;
            do {
                *dst = *run;
                run++;
                off++;
                dst++;
            } while (run != end);
            j++;
        } while (j != 3);
        i++;
        src += 0x30;
    } while (i != 0x20);

    cnt = 0;
    dst = work + off;
    do {
        *dst = *src;
        cnt++;
        src++;
        dst++;
        off++;
    } while (cnt != 0x3F0);

    w_tbl = Data_080eeda0;
    h_tbl = Data_080eeda3;
    tbl_end = w_tbl + 3;
    do {
        cnt = 0;
        num = *h_tbl * *w_tbl;
        if (num != 0) {
            dst = work + off;
            do {
                *dst = *src;
                cnt++;
                dst++;
                src++;
                off++;
            } while (cnt != num);
        }
        w_tbl++;
        h_tbl++;
    } while (w_tbl != tbl_end);

    Func_080e0524((s32)&Value_00000073, sprite_src, 0, 0);

    /* Eleven scene objects, kept in work + 0x77d8. */
    slot = 0x77D8;
    i = 0;
    do {
        node = Func_08009030(0x186);
        M2C_FIELD(work, void **, slot) = node;
        if (node != NULL) {
            M2C_FIELD(node, u8 *, 0x26) = 0;
            idx = i;
            if (i < 0) {
                idx = i + 3;
            }
            Func_08009020(node, idx >> 2);
            node = M2C_FIELD(work, void **, slot);
            flags = M2C_FIELD(node, u8 *, 9);
            M2C_FIELD(node, u8 *, 9) = (flags & ~0xC) | 4;
        }
        i++;
        slot += 4;
    } while (i != 0xB);

    Func_080ed408(0x2E, 7, 7, 3, 2);
    blit_a = (BlitFn)Data_03001e50[46];
    Func_080ed408(0x2F, 7, 7, 7, 2);

    reg = (u16 *)0x04000050;
    *reg++ = 0x3F46;
    *reg = 0x1010;

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    blit_b = (BlitFn)Data_03001e50[47];
    M2C_FIELD(work, s32 *, 0x7784) = 0x4B;
    Func_080041d8((void *)0x080CD261, 0x480);

    sparks = (struct Spark *)(work + 0x7198);
    spark = sparks;
    i = 0;
    do {
        pos = (s32)(Func_08004458() & 0xF) + 0x58;
        spark->height = 0x80;
        spark->pos = pos;
        y = (s32)(Func_08004458() & 0xF);
        spark->rate = 1;
        spark->unk_14 = 0x8000;
        spark->phase = i;
        timer = 0x2C - (i * 4);
        i++;
        spark->fall = y + 2;
        spark->timer = timer;
        spark++;
    } while (i != 0xB);

    spark = (struct Spark *)(work + 0x7240);
    i = 0;
    do {
        spark->pos = Data_080eedac[i];
        i++;
        spark++;
    } while (i != 6);

    drops = (struct Drop *)0x02010000;
    drop = drops;
    i = 0;
    do {
        drop->pos = (s32)(((Func_08004458() & 0x3F) + bias + 0x20) << 0x10);
        drop->height = (s32)(((7 & Func_08004458()) + 0x60) << 0x10);
        drop->rate = (s32)(((Func_08004458() & 0x3F) + 0x20) << 0xD);
        i++;
        drop->timer = (s32)(0x1F & Func_08004458());
        drop++;
    } while (i != 0x100);

    *(u16 *)0x0400000c = 0x785;
    M2C_FIELD(work, s32 *, 0x77A8) = 0xFA;

    row_y = bias + 0x40;
    fp = 0;
    do {
        if (fp == 0) {
            Func_080f9010(0xD4);
        }
        if (fp == 0x28) {
            Func_080f9010(0x8D);
        }
        if (fp == 0x60) {
            Func_080f9010(0x91);
        }
        if (fp == 0x78) {
            Func_080b50e8(0x86);
        }

        /* The two curtain halves, until they have finished parting. */
        if (fp <= 0x51) {
            idx = fp;
            if (fp < 0) {
                idx = fp + 3;
            }
            idx >>= 2;
            if (idx > 2) {
                idx = (1 & idx) + 1;
            }
            w = Data_080eeda0[idx];
            h = Data_080eeda3[idx];
            blit_a(draw_target, work + Data_080eed9a[idx],
                (bias - w) + 0x40, 0x74 - h, w, h);
            h = Data_080eeda3[idx];
            blit_b(draw_target, work + Data_080eed9a[idx], row_y, 0x74 - h,
                Data_080eeda0[idx], h);
        }

        /* Four band rows scrolling in, then the same rows scrolling out. */
        if ((u32)(fp - 0xC) <= 0x4BU) {
            idx = Func_080022ec(fp - 0x40, 3);
            if (idx < 0) {
                idx = 0;
            }
            if (idx > 7) {
                idx = 7;
            }
            j = 0;
            y = -0xC;
            do {
                w = Data_080eed90[idx];
                blit_a(draw_target, work + Data_080eed7e[idx],
                    (bias - w) + 0x40, y, w, 0x20);
                blit_b(draw_target, work + Data_080eed7e[idx], row_y, y,
                    Data_080eed90[idx], 0x20);
                j++;
                y += 0x20;
            } while (j != 4);
        }

        if ((u32)(fp - 0xA0) <= 0x17U) {
            idx = 7 - Func_080022ec(fp - 0xA0, 3);
            if (idx < 0) {
                idx = 0;
            }
            if (idx > 7) {
                idx = 7;
            }
            j = 0;
            y = -0xC;
            do {
                w = Data_080eed90[idx];
                blit_a(draw_target, work + Data_080eed7e[idx],
                    (bias - w) + 0x40, y, w, 0x20);
                blit_b(draw_target, work + Data_080eed7e[idx], row_y, y,
                    Data_080eed90[idx], 0x20);
                j++;
                y += 0x20;
            } while (j != 4);
        }

        /* The two foreground panels. */
        if ((u32)(fp - 0x58) <= 0x47U) {
            void *panel;

            panel = work + 0x13C0;
            blit_a(draw_target, panel, bias + 0x10, 0, 0x30, 0x60);
            blit_b(draw_target, panel, row_y, 0, 0x30, 0x60);
            panel = work + 0x25C0;
            blit_a(draw_target, panel, bias + 0x10, 0x60, 0x30, 0x15);
            blit_b(draw_target, panel, row_y, 0x60, 0x30, 0x15);
        }

        /* Step and draw the drops. */
        if (fp > 0x57) {
            drop = drops;
            i = 0;
            do {
                timer = drop->timer;
                if (timer == 0) {
                    u32 size;

                    size = (3 & i) + 5;
                    h = size * 2;
                    blit_a(draw_target,
                        (u8 *)sprite_src
                            + *(u16 *)((u8 *)Data_080ede48 + (h - 2)),
                        M2C_FIELD(drop, s16 *, 2) - (size >> 1),
                        M2C_FIELD(drop, s16 *, 6) - size, size, h);
                    pos = drop->height - drop->rate;
                    drop->height = pos;
                    if (pos < 0 && fp <= 0x9F) {
                        drop->height = 0x600000;
                    }
                } else {
                    drop->timer = timer - 1;
                }
                i++;
                drop++;
            } while (i != 0x40);
        }

        /* Spawn and step the sparks. */
        if (fp > 4) {
            num = 6;
            if (fp > 0x47) {
                num = 0xB;
            }
            i = 0;
            if (num != 0) {
                spark = sparks;
                do {
                    timer = spark->timer;
                    if (timer != 0) {
                        spark->timer = timer - 1;
                    } else {
                        s32 base;

                        pair = Data_080edab0;
                        if (fp > 0x47) {
                            base = (i << 0xC) + 0x8000;
                            /*
                             * The reference really does store the spread
                             * twice here: once before the state term is
                             * folded in and once after, so the first store
                             * is kept even though it is dead.
                             */
                            pair.field_0000 = base;
                            base =
                                (M2C_FIELD(STATE, s32 *, 0x18) << 0xE) + base;
                        } else {
                            base = 0x8000;
                        }
                        pair.field_0000 = base;
                        pair.field_0004 = pair.field_0000;

                        transform.field_000c = 0;
                        transform.field_0000 =
                            (s32)((spark->pos + (col_x * 2)) << 0x10);
                        transform.field_0004 =
                            (s32)(0x02000000 - (spark->height << 0x10));
                        transform.field_0008 = 0x02000000;

                        idx = Func_080022fc((fp / 2) + i, 0xB);
                        timer = -1;
                        if (idx != -1) {
                            Func_08009008(
                                M2C_FIELD(work, void **, (idx * 4) + 0x77D8),
                                &transform, &pair, 0);
                        }

                        spark->height -= spark->fall;
                        pos = spark->phase + spark->rate;
                        spark->phase = pos;
                        if (pos > 0xC) {
                            spark->phase = pos - 0xC;
                        }
                        if (spark->height < 0) {
                            if (fp > 0x9F) {
                                spark->timer = timer;
                            } else {
                                if (fp > 0x57) {
                                    spark->fall =
                                        (s32)((7 & Func_08004458()) + 8);
                                    spread = M2C_FIELD(STATE, s32 *, 0x18);
                                    if (spread == 0) {
                                        pos = Func_08002304(
                                            Func_08004458(), 0x60) + 0x2A;
                                    } else if (spread == 1) {
                                        pos = Func_08002304(
                                            Func_08004458(), 0x70) + 0x22;
                                    } else {
                                        pos = Func_08002304(
                                            Func_08004458(), 0xA0) + 0xA;
                                    }
                                    spark->pos = pos;
                                }
                                spark->height = 0x80;
                                spark->timer = 8;
                            }
                        }
                    }
                    i++;
                    spark++;
                } while (i != num);
            }
        }

        /* Poke every real party member the state carries. */
        if (fp <= 0x9E) {
            j = 0;
            if (M2C_FIELD(STATE, s32 *, 0x14) != 0) {
                off = 0x24;
                do {
                    if (fp > 0x55) {
                        if (Func_080022fc(fp, 0xC) == 0) {
                            Func_080d6888(
                                M2C_FIELD(STATE, s16 *, off), 7, 5, j, 6);
                        }
                        if (!(fp & 3)) {
                            Func_080b5088(M2C_FIELD(STATE, s16 *, off), 5);
                        }
                    }
                    j++;
                    off += 2;
                } while (j != M2C_FIELD(STATE, s32 *, 0x14));
            }
        }

        if ((u32)(fp - 0x5A) > 0x46U) {
            Func_080e155c(2, 2);
        } else {
            Func_080e155c(8, 8);
        }

        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        fp++;
    } while (fp != 0xC0);

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);

    cursor = (void **)(work + 0x77D8);
    i = 0;
    do {
        node = *cursor++;
        i++;
        Func_08009038(node);
    } while (i != 0xB);

    Func_080cdbc0();
}
