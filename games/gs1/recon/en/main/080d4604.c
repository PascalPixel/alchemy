#include "types.h"

#define BattleEffect_RunSparkGroups Func_080d4604

/*
 * Draft for the battle-presentation sub-effect at 0x080d4604 (1764 bytes).
 *
 * Same 0x03001eec "battle work" subsystem family as
 * games/gs1/src/battle/effects/member_orbit/run.c (Func_080ce85c, exact) and
 * the measured drafts games/gs1/recon/en/main/080dfe2c.c and 080d4ce8.c: the
 * heap_cache/cursor prologue, the M2C_FIELD field-offset idiom, the
 * DrawRectangleFn typedef, the work+0x7828 effect-state republication, the
 * work+0x7780 / 0x7784 / 0x7824 / 0x77a8 stores, the Func_080041d8 /
 * Func_08004278 0x080CD261 callback pair and the Func_080e155c /
 * Func_080cd52c / Func_080030f8 frame tail are all shared with them.  Every
 * constant, offset and branch below was read from this owner's own reference
 * disassembly (games/gs1/asm/080d4604.s), not carried over from a template.
 *
 * Behaviour: the owner takes the effect-state object plus a small `kind`
 * selector.  kind 0 and 1 use fixed anchor coordinates; any other kind
 * projects the state's field_08 through Func_080e396c and derives them.  It
 * then sets a BG control word, prepares the two generated rectangle-blit
 * routines (heap kinds 46 and 47), loads two resources, optionally copies a
 * kind-specific palette, and seeds a per-group ring of sixteen 28-byte
 * records at work + 0x7080 + group * 0x1c0 plus one burst of the shared
 * 28-byte particle pool at 0x02010000.  The number of groups and the number
 * of particles per group come from a 10-byte-stride table at 0x080ee262
 * indexed by the state's field_18; that same table row supplies a per-group
 * horizontal offset.  The frame loop then runs groups * 8 + 56 frames,
 * drawing the ring records, ageing them, and advancing and drawing the
 * particles with Func_080e3908 gravity.
 *
 * Uncertain / unresolved:
 *   - Names of the 0x080ee262 row fields are behavioural, not recovered.
 *   - Data_080ee294 (u8) and Data_080ede48 (u16) are indexed lookup tables
 *     whose contents were not decoded here; only their access widths and
 *     index expressions are evidenced.
 *   - Spark fields 8 and 20 are never touched by this owner; they are named
 *     unk08 / unk14 and only reserve the 28-byte stride the reference proves.
 *   - The unsigned range tests ((u32)x <= 0x7effff, (u32)timer <= 17) are
 *     written in the form the reference's `bhi` proves; whether the original
 *     spelled a two-sided signed range is not decidable from the bytes.
 */

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, s32 w, s32 h);
typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);
typedef s32 (*FillWordsFn)(void *dest, s32 bytes, s32 value);

/* Small absolute link-time constants: every retained Func_08002f40 /
   Func_080e0524 call site loads its resource id from a literal pool rather
   than an immediate, which an ordinary integer literal cannot produce. */
extern u8 Value_0000007d;
extern u8 Value_00000073;
extern u8 Value_00000087;
extern u8 Value_000000c4;

/* Heap-block address cache; Data_03001e50[kind] holds that kind's block. */
extern void *Data_03001e50[];

extern const u16 Data_080ede48[];
extern const u8 Data_080ee294[];

/* 10-byte rows at 0x080ee262, indexed by the effect state's field_18. */
typedef struct SparkPattern {
    u16 count;      /* particles seeded and stepped per group */
    u16 groups;     /* number of groups, and the frame-count multiplier */
    u16 offset[3];  /* per-group horizontal offset from the anchor */
} SparkPattern;

extern const SparkPattern Data_080ee262[];

/* 28-byte record, shared by the work + 0x7080 rings and the 0x02010000
   particle pool.  Fields 0/4 are 16.16 fixed-point position, 12/16 are the
   matching velocity, 24 is a countdown. */
typedef struct Spark {
    s32 x;
    s32 y;
    s32 unk08;
    s32 vx;
    s32 vy;
    s32 unk14;
    s32 timer;
} Spark;

extern Spark Data_02010000[];

typedef struct Efx {
    s32 unk00;
    s32 side;    /* +4  */
    s32 unk08;   /* +8  */
    s32 unk0C;
    s32 unk10;
    s32 members; /* +20 */
    s32 index;   /* +24, row index into Data_080ee262 */
    s32 unk1C;
    s32 unk20;
    s16 actors[8]; /* +36 */
} Efx;

#define WORK_EFX (*(Efx **)((s8 *)work + 0x7828))

void Func_080cd594(s32 mode);
void Func_080e396c(s32 source, s32 *out);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080e0524(s32 resource, void *target, s32 flag_a, s32 flag_b);
void *Func_08002f40(s32 id);
s32 Func_08004458(void);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
s32 Func_080022ec(s32 numerator, s32 denominator);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_080b50e8(s32 id);
void Func_080f9010(s32 id);
void Func_080e3908(void *particle, s32 step, s32 gravity);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080b5088(s32 member_id, s32 b);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

void BattleEffect_RunSparkGroups(void *object, s32 kind)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    void *extra;
    void *palette;
    s32 status;
    s32 pos[3];
    s32 base_x;
    s32 base_y;
    DrawRectangleFn rectangle[2];
    DrawRectangleFn *rectangle_slot;
    DrawRectangleFn second;
    s8 *ring_base;
    s32 group;
    s32 i;
    s32 frame;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    extra = heap_cache[2];
    WORK_EFX = (Efx *)object;

    if (kind == 0) {
        Func_080cd594(1);
        base_x = 60;
        base_y = 48;
    } else if (kind == 1) {
        Func_080cd594(0);
        base_x = 60;
        base_y = 64;
    } else {
        Func_080cd594(0);
        Func_080e396c(WORK_EFX->unk08, pos);
        base_x = pos[0] / 2;
        base_y = pos[1] + 48;
    }

    M2C_FIELD((void *)0x04000052, u16 *, 0) = 0x1010;
    status = Func_080ed408(46, 7, 7, 3, 2);
    rectangle[0] = (DrawRectangleFn)Data_03001e50[46];
    status = Func_080ed408(47, 7, 7, 3, 3);
    second = (DrawRectangleFn)Data_03001e50[47];
    rectangle_slot = rectangle;
    rectangle_slot[1] = second;

    Func_080e0524((s32)&Value_0000007d, work, 1, 1);
    Func_080e0524((s32)&Value_00000073, extra, 0, 0);

    if (kind == 1) {
        palette = Func_08002f40((s32)&Value_00000087);
        status = ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    } else if (kind == 2) {
        palette = Func_08002f40((s32)&Value_000000c4);
        status = ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    }
    (void)status;

    group = 0;
    if (Data_080ee262[WORK_EFX->index].groups != 0) {
        ring_base = (s8 *)work;
        do {
            Spark *ring;

            ring = (Spark *)(ring_base + 0x7080);
            for (i = 0; i != 16; i++) {
                s32 angle;

                angle = Func_08004458() & 0xFFFF;
                ring->x = (i * 2) * Func_08002322(angle);
                ring->y = -((i * 2) * Func_0800231c(angle));
                ring->timer = (i / 2) + 25;
                ring++;
            }

            i = 0;
            if (Data_080ee262[WORK_EFX->index].count != 0) {
                s32 y_fixed;

                y_fixed = base_y << 16;
                do {
                    Efx *efx;
                    Spark *spark;
                    s32 magnitude;
                    s32 angle;
                    s32 offset;
                    s32 x;

                    efx = WORK_EFX;
                    spark = &Data_02010000[
                        Data_080ee262[efx->index].count * group + i];
                    magnitude = (Func_08004458() & 0x3FF) + 32;
                    angle = Func_08004458() & 0xFFFF;
                    efx = WORK_EFX;
                    offset = Data_080ee262[efx->index].offset[group];
                    if (efx->side == 1) {
                        x = (base_x - offset) + 28;
                    } else {
                        x = (base_x + offset) - 28;
                    }
                    spark->x = x << 16;
                    spark->y = y_fixed;
                    spark->vx = (magnitude * Func_08002322(angle)) >> 6;
                    spark->vy = -((magnitude * Func_0800231c(angle)) << 1) >> 6;
                    i++;
                    spark->timer = (Func_08004458() & 7) + 32;
                } while (i != Data_080ee262[WORK_EFX->index].count);
            }

            ring_base += 0x1C0;
            group++;
        } while (group != Data_080ee262[WORK_EFX->index].groups);
    }

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Func_080041d8((void *)0x080CD261, 0x480);

    for (frame = 0;
            frame != (Data_080ee262[WORK_EFX->index].groups << 3) + 56;
            frame++) {
        void *screen;
        Efx *efx;

        screen = *(void **)0x03001E80;
        efx = WORK_EFX;
        if (efx->index == 2 && frame <= 51) {
            if (efx->side == 0) {
                M2C_FIELD(screen, u16 *, 54) += 256;
            } else {
                M2C_FIELD(screen, u16 *, 54) -= 256;
            }
        }

        if (WORK_EFX->index == 3 && frame == 4) {
            status = ((FillWordsFn)0x03000168)(canvas, 0x4000, 0x3F3F3F3F);
        }

        if (kind == 1 || kind == 2) {
            if (frame == 2) {
                Func_080b50e8(145);
            }
        } else {
            if (frame == 2) {
                Func_080f9010(145);
            }
            if (frame == 24) {
                Func_080b50e8(134);
            }
        }

        group = 0;
        if (Data_080ee262[WORK_EFX->index].groups != 0) {
            ring_base = (s8 *)work;
            do {
                s32 start;

                start = group << 3;
                if (frame == start) {
                    M2C_FIELD(work, s32 *, 0x77A8) = 12;
                }

                if (frame >= start && frame < start + 2) {
                    s32 offset;

                    efx = WORK_EFX;
                    offset = Data_080ee262[efx->index].offset[group];
                    if (efx->side == 1) {
                        rectangle[0](canvas, work, (base_x - offset) + 12,
                            base_y - 32, 32, 64);
                    } else {
                        rectangle[0](canvas, work, (base_x + offset) - 44,
                            base_y - 32, 32, 64);
                    }
                }

                if (frame >= start) {
                    Spark *ring;

                    ring = (Spark *)(ring_base + 0x7080);
                    for (i = 0; i != 12; i++) {
                        s32 x;
                        s32 y;
                        s32 offset;
                        s32 timer;

                        y = M2C_FIELD(ring, s16 *, 6) + base_y;
                        efx = WORK_EFX;
                        offset = Data_080ee262[efx->index].offset[group];
                        if (efx->side == 1) {
                            x = ((M2C_FIELD(ring, s16 *, 2) + base_x)
                                - offset) + 28;
                        } else {
                            x = ((M2C_FIELD(ring, s16 *, 2) + base_x)
                                + offset) - 28;
                        }
                        timer = ring->timer;
                        if ((u32)timer <= 17) {
                            rectangle[0](canvas,
                                (s8 *)work
                                    + (Data_080ee294[Func_080022ec(timer, 3)]
                                        << 11),
                                x - 16, y - 32, 32, 64);
                            timer = ring->timer;
                        }
                        if (timer > 0) {
                            ring->timer = timer - 1;
                        } else {
                            ring->timer = -1;
                        }
                        ring++;
                    }
                }

                if (frame > start + 5) {
                    s32 gravity;

                    gravity = 0x1000;
                    if (kind == 2) {
                        gravity = -0x1000;
                    }
                    i = 0;
                    if (Data_080ee262[WORK_EFX->index].count != 0) {
                        do {
                            Spark *spark;

                            spark = &Data_02010000[
                                Data_080ee262[WORK_EFX->index].count * group
                                    + i];
                            if (spark->timer > 0) {
                                s32 timer;
                                s32 x;
                                s32 y;

                                Func_080e3908(spark, 60, gravity);
                                timer = spark->timer - 1;
                                y = spark->y;
                                spark->timer = timer;
                                if (y > 0x6C0000) {
                                    spark->vy = -spark->vy / 2;
                                } else {
                                    x = spark->x;
                                    if ((u32)x <= 0x7EFFFF && y >= 0) {
                                        s32 size;

                                        size = Func_080022ec(timer, 5) + 1;
                                        rectangle_slot[i & 1](canvas,
                                            (s8 *)extra
                                                + Data_080ede48[size - 1],
                                            (x >> 16) - (size / 2),
                                            (y >> 16) - size,
                                            size, size * 2);
                                    }
                                }
                            }
                            i++;
                        } while (i != Data_080ee262[WORK_EFX->index].count);
                    }
                }

                i = 0;
                if (WORK_EFX->members != 0) {
                    do {
                        if (frame == start + 6) {
                            Func_080d6888(WORK_EFX->actors[i], 7, 5, i, 10);
                            Func_080b5088(WORK_EFX->actors[i], 4);
                        }
                        i++;
                    } while (i != WORK_EFX->members);
                }

                ring_base += 0x1C0;
                group++;
            } while (group != Data_080ee262[WORK_EFX->index].groups);
        }

        Func_080e155c(16, 16);
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
