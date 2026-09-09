#include "types.h"

/*
 * Draft for the battle-presentation sub-effect at 0x080d6970.
 *
 * Same 0x03001eec "battle work" subsystem as the recovered sibling
 * games/gs1/src/battle/effects/member_orbit/run.c and the drafts
 * games/gs1/recon/en/main/080e08c0.c, 080d82b0.c, 080e01e4.c and
 * 080e7404.c; the field offsets, the heap_cache/cursor prologue, the
 * Value_XXXXXXXX effect-id idiom and the Func_080072f4/Func_080072f0
 * trampoline calls all follow those files.  Func_080072f4 and
 * Func_080072f0 are not real callees: they are the r4 and r3 entries of
 * the _call_via_rN bundle at games/gs1/asm/080072e4.s, so every
 * Func_080072f4 site here is an indirect call through one of the two
 * generated rectangle-blit routines (heap kinds 46 and 47).
 *
 * The owner is one function.  The reference's `bl 0x080d6b76` at
 * 0x080d75de is a long unconditional jump, not a call: the 366-frame
 * loop body is 2664 bytes, past the +/-2KB reach of a Thumb `b`, so the
 * back edge has to be emitted as a BL pair.  That is what the current
 * intra_function_call_module classification is describing.
 *
 * Sequence: seed and shuffle an 8 x 128 byte dither permutation at
 * 0x02010000, spawn eight effect objects, load the effect graphics, seed
 * three particle banks (work + 0x7080, + 0x7240, + 0x7400), then run 366
 * frames.  Each frame draws whichever banks its frame window enables,
 * greys the effect palette across frames 224..247, punches dither holes
 * into the canvas after frame 232, reloads a second graphics set at frame
 * 302, and shakes the camera.
 *
 * Uncertain, stated rather than guessed:
 *   - the third argument of the 0x03000164 fill at frame 191 is the
 *     constant 3 the reference leaves in r2 from the `& 3` mask above it;
 *     no other evidence fixes that value.
 *   - the 31 masks in the palette-grey loop are spelled as plain
 *     literals; the reference loads one of the three from its literal
 *     pool, which an ordinary 31 does not produce.
 *   - Data_080ee958 / Data_080ee966 are read at the fixed byte offset 12
 *     (element 6); the reference keeps that offset in a register, so the
 *     original may have indexed them with a variable not recovered here.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*WordCopyFn)(void *dest, s32 size, s32 value);
typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, u32 w, u32 h);

void Func_080dbb24(s32 count, s32 kind, s32 variant);
u32 Func_08004458(void);
void Func_080cd594(s32 mode);
void Func_080e0524(s32 effect_id, void *target, s32 flag_a, s32 flag_b);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_080d6750(void *object);
void Func_080072f0(void *dest, s32 size, s32 value, WordCopyFn fill);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void Func_080f9010(s32 id);
void Func_080b5088(s32 member_id, s32 unk);
void Func_080e6d3c(s32 channel, s32 a, s32 b);
void Func_080e6eac(s32 channel, s32 a, s32 b);
s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080022fc(s32 numerator, s32 denominator);
s32 Func_08002304(s32 numerator, s32 denominator);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_08009020(void *object, s32 value);
void Func_08009038(void *object);
void Func_080e3908(void *particle, s32 count, s32 flags);
void Func_080e3944(void *source, void *screen);
void **Func_080b5098(s32 member_id);
void *Func_080b50d8(void *object, s32 index);
void Func_080b50e8(s32 id);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
s32 Func_080cdbc0(void);

extern u8 Data_080ee910[];
extern u16 Data_080ee916[];
extern u8 Data_080ee920[];
extern u8 Data_080ee925[];
extern u16 Data_080ee92a[];
extern u8 Data_080ee930[];
extern u16 Data_080ee934[];
extern u8 Data_080ee93e[];
extern u8 Data_080ee943[];
extern u16 Data_080ee948[];
extern u8 Data_080ee952[];
extern u16 Data_080ee958[];
extern u16 Data_080ee966[];

extern u8 Value_00000000;
extern u8 Value_00000098;
extern u8 Value_000000b2;
extern u8 Value_000000c0;
extern u8 Value_00000100;
extern u8 Value_00000177;

void Func_080d6970(void *object)
{
    s32 record[3];
    s32 screen[3];
    DrawRectangleFn rect[2];
    void *canvas;
    s32 t;
    s32 fade;
    s32 fade_step;
    s32 phase;
    s32 limit;
    s32 scroll;
    DrawRectangleFn *rect_ptr;
    void *work;

    {
        void **cursor;

        cursor = (void **)0x03001EEC;
        work = *cursor++;
        canvas = *cursor;
    }
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080dbb24(8, (s32) &Value_00000177, 1);

    {
        u8 *buf;
        s32 i;

        buf = (u8 *)0x02010000;
        i = 0;
        do {
            *buf = (u8)(i & 0x7F);
            i++;
            buf++;
        } while (i != 1024);
    }
    {
        s32 row;
        s32 base;

        row = 0;
        base = 0;
        do {
            s32 i;

            i = 0;
            do {
                u8 *tbl;
                s32 a;
                s32 b;
                u8 tmp;

                tbl = (u8 *)0x02010000;
                a = Func_08004458() & 0x7F;
                b = Func_08004458() & 0x7F;
                tmp = tbl[base + b];
                tbl[base + b] = tbl[base + a];
                tbl[base + a] = tmp;
                i++;
            } while (i != 128);
            row++;
            base += 128;
        } while (row != 8);
    }

    Func_080cd594(0);
    M2C_FIELD((void *)0x04000020, s16 *, 0) = (s16)(s32) &Value_00000100;
    M2C_FIELD((void *)0x04000020, s16 *, 0x30) = (s16)(s32) &Value_00000000;

    Func_080e0524((s32) &Value_000000b2, work, 1, 1);
    {
        s32 status;

        status = Func_080ed408(46, 7, 7, 3, 1);
        rect[0] = (DrawRectangleFn)((void **)0x03001E50)[46];
        status = Func_080ed408(47, 7, 7, 7, 1);
        rect_ptr = rect;
        rect_ptr[1] = (DrawRectangleFn)((void **)0x03001E50)[47];
    }

    M2C_FIELD(work, s32 *, 0x7780) = 1;
    M2C_FIELD(work, s32 *, 0x7784) = 0;
    Func_080041d8((void *)0x080CD261, 0x480);
    M2C_FIELD((void *)0x04000028, s32 *, 0) = -0x2000;

    {
        s32 *ring;
        s32 i;
        s32 pos;
        s32 vel;

        ring = (s32 *)((u8 *)work + 0x7080);
        i = 0;
        pos = 0;
        vel = -90;
        do {
            if (i <= 4) {
                ring[0] = pos;
                ring[1] = (Func_08004458() & 7) + 104;
            } else {
                ring[0] = vel;
                ring[1] = (Func_08004458() & 7) + 108;
            }
            ring[4] = (Func_08004458() & 7) + 4;
            ring[6] = (Func_08004458() & 15) + 16;
            i++;
            vel += 20;
            pos += 20;
            ring += 7;
        } while (i != 16);
    }
    {
        s32 *drop;
        s32 i;

        drop = (s32 *)((u8 *)work + 0x7240);
        i = 0;
        do {
            s32 tmp;

            drop[0] = (Func_08004458() & 63) + 32;
            drop[1] = (Func_08004458() & 31) + 64;
            tmp = Func_08004458() & 7;
            i++;
            drop[6] = -tmp - 8;
            drop += 7;
        } while (i != 16);
    }
    {
        s32 *spark;
        s32 i;
        s32 zero;

        spark = (s32 *)((u8 *)work + 0x7400);
        i = 0;
        zero = 0;
        do {
            spark[0] = 0x800000;
            spark[1] = 0x400000;
            spark[3] = -((s32)(Func_08004458() & 255) + 200) << 9;
            spark[4] = zero;
            spark[6] = zero;
            i++;
            spark += 7;
        } while (i != 16);
    }

    Func_080d6750(M2C_FIELD(work, void **, 0x7828));

    fade = -0x400000;
    fade_step = 0;
    t = 0;
    do {
        s32 facing;

        facing = *(s32 *)0x03001E80;
        if ((*(s32 *)0x03001B04 & 3) != 0 && t > 190 && t <= 285) {
            Func_080072f0(canvas, 0x4000, 3, (WordCopyFn)0x03000164);
            t = 286;
        }
        if (t == 224) {
            M2C_FIELD(work, s32 *, 0x7780) = 0;
        }
        Func_080049ac();
        Func_080051d8(facing, facing + 12);

        if (t == 31) {
            s32 member;

            M2C_FIELD(work, s32 *, 0x77A8) = 8;
            Func_080f9010(157);
            member = 0;
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20) != 0) {
                s32 id_ofs;

                id_ofs = 36;
                do {
                    Func_080b5088(
                        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *,
                            id_ofs), 6);
                    member++;
                    id_ofs += 2;
                } while (member
                    != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20));
            }
        }
        if (t == 72) {
            Func_080f9010(136);
        }
        if (t == 140) {
            Func_080f9010(156);
        }

        fade_step += 0x4000;
        fade += fade_step;
        if (fade > 0x400000) {
            fade = 0x400000;
        }
        Func_080e6d3c(2, 0x800000, fade);

        if ((u32)(t - 48) <= 48U) {
            s32 cell;

            cell = Func_080022fc(Func_080022ec(t - 48, 24), 3);
            Func_08009020(M2C_FIELD(work, void **, 0x77E4),
                Data_080ee910[cell * 2]);
            Func_08009020(M2C_FIELD(work, void **, 0x77E8),
                Data_080ee910[cell * 2 + 1]);
        }

        if ((u32)(t - 72) <= 55U) {
            s32 *spark;
            s32 i;

            spark = (s32 *)((u8 *)work + 0x7400);
            i = 0;
            do {
                if (t >= i + 72) {
                    s32 y;

                    y = spark[1];
                    if (y <= 0x67FFFF) {
                        s32 cell;
                        u32 w;
                        u32 h;

                        cell = Func_080022fc((t + i) / 4, 5);
                        w = Data_080ee920[cell];
                        h = Data_080ee925[cell];
                        rect[0](canvas,
                            (u8 *)work + Data_080ee916[cell],
                            M2C_FIELD(spark, s16 *, 2) - (w >> 1),
                            (y >> 16) - (h >> 1), w, h);
                    }
                    Func_080e3908(spark, 64, 0x1000);
                }
                i++;
                spark += 7;
            } while (i != 16);
        }

        if (t == 128) {
            s32 *drop;
            s32 i;

            drop = (s32 *)((u8 *)work + 0x7240);
            i = 0;
            do {
                drop[0] = Func_08002304(Func_08004458(), 96) << 16;
                drop[1] = ((Func_08004458() & 7) + 88) << 16;
                drop[3] = ((s32)(Func_08004458() & 255) - 128) << 11;
                drop[4] = -(s32)(Func_08004458() & 255) << 11;
                drop[6] = -(s32)(Func_08004458() & 15) - 16;
                i++;
                drop += 7;
            } while (i != 48);
            M2C_FIELD((void *)0x04000028, s32 *, 0) = 0;
        }

        phase = t - 128;
        if ((u32)phase <= 96U) {
            s32 *ring;
            s32 k;

            limit = phase;
            if (phase > 80) {
                limit = 80;
            } else {
                M2C_FIELD(work, s32 *, 0x77A8) = 2;
            }
            ring = (s32 *)((u8 *)work + 0x7080);
            k = 0;
            do {
                if (limit > ring[6]) {
                    void *sheet;
                    s32 cnt;
                    s32 j;

                    sheet = work;
                    if (k > 5) {
                        sheet = (u8 *)work + 0x6C0;
                    }
                    scroll = (limit - ring[6]) * ring[4];
                    cnt = scroll;
                    while (cnt > 184) {
                        cnt -= 64;
                    }
                    if (cnt <= 119) {
                        rect_ptr[k & 1](canvas, sheet, ring[0],
                            (ring[1] - cnt) - 8, 24, 8);
                    }
                    j = 0;
                    do {
                        s32 y;
                        s32 h;
                        s32 src;

                        y = (ring[1] - cnt) + (j << 6);
                        h = 64;
                        src = 0;
                        if (y >= -64) {
                            if (y < 0) {
                                src = -y * 24;
                                h = y + 64;
                                y = 0;
                            }
                            if (y + h > ring[1]) {
                                h -= (y + h) - ring[1];
                            }
                            rect_ptr[k & 1](canvas,
                                ((u8 *)sheet + src) + 192, ring[0], y, 24, h);
                        }
                        j++;
                    } while (j != 3);
                    if ((k & 1) != 0) {
                        s32 y;
                        s32 cell;
                        s32 w;
                        s32 h;

                        y = ((ring[1] - scroll) & 0x7F) - 16;
                        cell = Func_080022fc(k, 3);
                        w = Data_080ee930[cell];
                        h = w;
                        if (y + h > ring[1]) {
                            h -= (y + h) - ring[1];
                        }
                        if (h > 0) {
                            rect_ptr[k & 1](canvas,
                                (u8 *)work + Data_080ee92a[cell],
                                ring[0] + 8, y, (u32)w, (u32)h);
                        }
                    }
                }
                k++;
                ring += 7;
            } while (k != 10);
        }

        if ((u32)phase <= 95U) {
            s32 *spark;
            s32 i;

            spark = (s32 *)((u8 *)work + 0x7400);
            i = 0;
            do {
                if (spark[6] >= 0) {
                    s32 cell;
                    s32 y;
                    u32 w;
                    u32 h;

                    cell = Func_080022fc(i, 5);
                    w = Data_080ee93e[cell];
                    h = Data_080ee943[cell];
                    rect[0](canvas, (u8 *)work + Data_080ee934[cell],
                        M2C_FIELD(spark, s16 *, 2),
                        M2C_FIELD(spark, s16 *, 6), w, h);
                    spark[0] = spark[0] + spark[3];
                    y = spark[1] + spark[4];
                    spark[1] = y;
                    spark[4] = spark[4] + 0x4000;
                    if ((u32)y > 0x780000U && t <= 159) {
                        spark[0] = Func_08002304(Func_08004458(), 96) << 16;
                        spark[1] = ((Func_08004458() & 7) + 88) << 16;
                        spark[3] = ((s32)(Func_08004458() & 255) - 128) << 11;
                        spark[4] = -(s32)(Func_08004458() & 255) << 11;
                    }
                }
                spark[6] = spark[6] + 1;
                i++;
                spark += 7;
            } while (i != 32);
        }

        if ((u32)(t - 224) <= 23U && (t & 3) == 0) {
            u16 *pal;
            s32 i;

            pal = (u16 *)0x05000000;
            i = 0;
            do {
                u16 colour;
                s32 r;
                s32 g;
                s32 b;
                s32 grey;

                colour = *pal;
                r = colour & 31;
                g = (colour >> 5) & 31;
                b = (colour >> 10) & 31;
                grey = Func_080022ec(r + g + b, 3);
                if (r > grey) {
                    r--;
                }
                if (r < grey) {
                    r++;
                }
                if (g > grey) {
                    g--;
                }
                if (g < grey) {
                    g++;
                }
                if (b > grey) {
                    b--;
                }
                if (b < grey) {
                    b++;
                }
                i++;
                *pal = (u16)((b << 10) | (g << 5) | r);
                pal++;
            } while (i != 64);
        }

        if ((u32)phase <= 172U) {
            s32 member;

            member = 0;
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20) != 0) {
                s32 *screen_ptr;
                s32 *record_ptr;
                s32 slot;
                s32 id_ofs;

                screen_ptr = screen;
                record_ptr = record;
                slot = 0;
                id_ofs = 36;
                do {
                    void *mp;
                    s32 *drop;
                    s32 i;

                    mp = *Func_080b5098(
                        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *,
                            id_ofs));
                    record_ptr[0] = M2C_FIELD(mp, s32 *, 8);
                    record_ptr[1] = M2C_FIELD(mp, s32 *, 12);
                    record_ptr[2] = M2C_FIELD(mp, s32 *, 16);
                    Func_080e3944(record_ptr, screen_ptr);
                    drop = (s32 *)((u8 *)work + 0x7240 + slot * 28);
                    i = 0;
                    do {
                        s32 age;

                        age = drop[6];
                        if (age == 0) {
                            drop[0] = (screen_ptr[0]
                                + (s32)(Func_08004458() & 15)) - 8;
                            drop[1] = (screen_ptr[1]
                                + (s32)(Func_08004458() & 15)) - 40;
                            age = drop[6];
                        }
                        if ((u32)age <= 4U) {
                            u32 w;

                            w = Data_080ee952[age];
                            rect[0](canvas,
                                (u8 *)work + Data_080ee948[age],
                                drop[0] - (w >> 1), drop[1] - (w >> 1), w, w);
                            age = drop[6];
                        }
                        age++;
                        drop[6] = age;
                        if (t <= 199 && age == 5) {
                            drop[6] = -(s32)(Func_08004458() & 7);
                        }
                        i++;
                        drop += 7;
                    } while (i != 6);
                    member++;
                    id_ofs += 2;
                    slot += 6;
                } while (member
                    != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20));
            }
        }

        if (t > 232) {
            s32 base;
            s32 x;
            s32 row;

            base = t * 2 - 496;
            row = 0;
            x = base;
            do {
                s32 j;

                j = 0;
                do {
                    if ((u32)x <= 127U) {
                        s32 lo;
                        s32 v;

                        lo = x & 7;
                        v = ((u8 *)0x02010000)[(((lo << 5) + row) << 2) + j];
                        M2C_FIELD(canvas, u8 *,
                            (((((x / 8) << 4) + (v / 8)) << 3) + lo) * 8
                                + (v & 7)) = (u8)0;
                    }
                    j++;
                } while (j != 4);
                row++;
                x++;
            } while (row != 32);

            row = 0;
            x = base + 1;
            do {
                s32 j;

                j = 0;
                do {
                    if ((u32)x <= 127U) {
                        s32 lo;
                        s32 v;

                        lo = x & 7;
                        v = ((u8 *)0x02010000)[(((lo << 5) + row) << 2) + j];
                        M2C_FIELD(canvas, u8 *,
                            (((((x / 8) << 4) + (v / 8)) << 3) + lo) * 8
                                + (v & 7)) = (u8)0;
                    }
                    j++;
                } while (j != 4);
                row++;
                x++;
            } while (row != 32);
        }

        if ((u32)(t - 161) <= 62U) {
            s32 member;

            member = 0;
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20) != 0) {
                do {
                    if (t > member * 8 + 160) {
                        void **holder;
                        void *body;
                        void *sub;
                        s32 i;

                        holder = Func_080b5098(
                            M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *,
                                36 + member * 2));
                        body = holder[0];
                        M2C_FIELD(body, s32 *, 12) =
                            M2C_FIELD(body, s32 *, 12) + 0x80000;
                        if (M2C_FIELD(body, s32 *, 12) > 0x800000) {
                            M2C_FIELD(body, s32 *, 12) = 0x800000;
                        }
                        M2C_FIELD(body, s32 *, 72) = 0;
                        i = 0;
                        while ((sub = Func_080b50d8(holder[0], i)) != 0) {
                            Func_08009020(sub, 5);
                            i++;
                        }
                    }
                    member++;
                } while (member
                    != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20));
            }
        }

        {
            s32 member;

            member = 0;
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20) != 0) {
                s32 cnt;
                s32 id_ofs;

                cnt = 286;
                id_ofs = 36;
                do {
                    if (t == cnt) {
                        void *mp;

                        mp = *Func_080b5098(
                            M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *,
                                id_ofs));
                        M2C_FIELD(mp, s32 *, 12) = 0x600000;
                        M2C_FIELD(mp, s32 *, 72) = 0xAB85;
                    }
                    if (t == cnt + 16) {
                        Func_080d6888(
                            M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *,
                                id_ofs), 7, -1, member, 8);
                        Func_080f9010(134);
                        M2C_FIELD(work, s32 *, 0x77A8) = 8;
                    }
                    member++;
                    id_ofs += 2;
                    cnt += 5;
                } while (member
                    != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20));
            }
        }

        if (t == 302) {
            s32 status;
            s32 member;

            Func_08002dd8(47);
            Func_08002dd8(46);
            Func_080e0524((s32) &Value_00000098, work, 1, 0);
            Func_080e0524((s32) &Value_000000c0, (u8 *)work + 0x1680, 1, 1);
            status = Func_080ed408(46, 7, 7, 3, 2);
            rect[0] = (DrawRectangleFn)((void **)0x03001E50)[46];
            status = Func_080ed408(47, 7, 7, 7, 2);
            rect_ptr[1] = (DrawRectangleFn)((void **)0x03001E50)[47];
            M2C_FIELD((void *)0x04000050, s16 *, 0) = 0x3F46;
            M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x80;
            M2C_FIELD((void *)0x04000028, s32 *, 0) = 0;
            M2C_FIELD(work, s32 *, 0x7780) = 2;
            M2C_FIELD(work, s32 *, 0x7784) = 75;
            Func_080041d8((void *)0x080CD261, 0x480);
            member = 0;
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20) != 0) {
                do {
                    void *mp;
                    s32 *ring;
                    s32 i;

                    mp = *Func_080b5098(
                        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *,
                            36 + member * 2));
                    ring = (s32 *)((u8 *)work + 0x7080 + member * 280);
                    i = 0;
                    do {
                        s32 angle;

                        ring[0] = M2C_FIELD(mp, s32 *, 8);
                        ring[1] = 0x140000;
                        ring[2] = M2C_FIELD(mp, s32 *, 16);
                        angle = i * 13108;
                        ring[3] = Func_08002322(angle) << 2;
                        ring[4] = (s32)(Func_08004458() & 0x7FFF) + 0x10000;
                        ring[5] = Func_0800231c(angle) << 2;
                        ring[6] = 0;
                        i++;
                        ring += 7;
                    } while (i != 10);
                    member++;
                } while (member
                    != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20));
            }
        }

        if (t > 301) {
            s32 member;

            member = 0;
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20) != 0) {
                s32 *record_ptr;
                s32 *screen_ptr;
                s32 span;
                s32 start;
                s32 age;

                record_ptr = record;
                screen_ptr = screen;
                span = t - 302;
                start = 302;
                age = 0;
                do {
                    if (t >= start && t < age + 314) {
                        void *mp;
                        s32 frame;

                        frame = span / 2;
                        mp = *Func_080b5098(
                            M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *,
                                36 + member * 2));
                        record_ptr[0] = M2C_FIELD(mp, s32 *, 8);
                        record_ptr[1] = 0;
                        record_ptr[2] = M2C_FIELD(mp, s32 *, 16);
                        Func_080e3944(record_ptr, screen_ptr);
                        screen_ptr[0] = screen_ptr[0] >> 1;
                        rect[0](canvas, (u8 *)work + frame * 480,
                            screen_ptr[0] - 20, screen_ptr[1] - 24, 20, 24);
                        rect_ptr[1](canvas, (u8 *)work + frame * 480,
                            screen_ptr[0], screen_ptr[1] - 24, 20, 24);
                    }
                    if (t >= start + 6) {
                        s32 *ring;
                        s32 i;

                        ring = (s32 *)((u8 *)work + 0x7080
                            + (age + member) * 56);
                        i = 0;
                        do {
                            Func_080e3944(ring, screen_ptr);
                            screen_ptr[0] = screen_ptr[0] >> 1;
                            if ((u32)ring[6] <= 26U) {
                                u32 w;

                                w = Data_080ee966[6];
                                rect[0](canvas,
                                    (u8 *)work + Data_080ee958[6],
                                    screen_ptr[0] - (w >> 1),
                                    screen_ptr[1] - (w >> 1), w, w);
                            }
                            Func_080e3908(ring, 60, 0x1000);
                            ring[6] = ring[6] + 1;
                            i++;
                            ring += 7;
                        } while (i != 5);
                    }
                    span -= 4;
                    start += 4;
                    age += 4;
                    member++;
                } while (member
                    != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20));
            }
        }

        if (t <= 127) {
            Func_080e155c(4, 16);
        } else if (t <= 301) {
            Func_080e155c(2, 2);
        } else {
            Func_080e155c(4, 8);
        }
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        t++;
    } while (t != 366);

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080b50e8(134);
    Func_080e6eac(2, 0x800000, fade);
    {
        void **cursor;
        s32 i;

        cursor = (void **)((u8 *)work + 0x77D8);
        i = 0;
        do {
            Func_08009038(*cursor++);
            i++;
        } while (i != 8);
    }
    Func_080cdbc0();
}
