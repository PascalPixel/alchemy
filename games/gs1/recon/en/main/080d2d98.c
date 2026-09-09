#include "types.h"

/*
 * Battle-presentation sub-effect at 0x080d2d98 (1576 bytes).  It belongs to
 * the same 0x03001eec "battle work" family as the exact
 * games/gs1/src/battle/effects/member_orbit/run.c (main:080ce85c) and
 * games/gs1/src/battle/presentation/prepare_scene.c (main:080ccaec), and to
 * the measured draft games/gs1/recon/en/main/080dab74.c.  The kind-39 work
 * block (0x782c bytes: sixty-four twenty-eight byte particle records at
 * 0x7080, display mode/value at 0x7780/0x7784, the twelve spawned-object
 * pointers at 0x77d8 that games/gs1/src/battle/effects/common/spawn_objects.c
 * fills, frame_ready at 0x7824 and the republished effect argument at
 * 0x7828), the DrawRectangleFn convention through the _call_via_r4 thunk at
 * games/gs1/asm/080072e4.s, the IWRAM word-copy kernel at 0x03001388 and the
 * Value_ spelling for a pool-loaded resource id all come from those owners.
 *
 * Behaviour: three graphics resources are streamed in (palette plus scrolling
 * sky tiles at work+0, column tiles at work+0x6e4, and the ember sprite sheet
 * into the third runtime cell), the sixty-four work-block particle records
 * are disarmed, records 24..39 are given a random screen slot and a short
 * negative delay for the co-scheduled object callback at 0x080cd261, the
 * 1024-entry ember pool at 0x02010000 is disarmed, and twelve kind-380
 * objects are spawned.  The run is then 124 frames long; pressing a button
 * (bits 0..1 of the pad word at 0x03001b04) fast-forwards frames 33..97 to
 * frame 98.  Each frame:
 *   - a damped focus point is integrated and published through Func_080e6d3c;
 *     x decays toward 120<<16 with a restoring push, y just decays;
 *   - frame 28 seeds up to 256 embers on a ring around (32<<16, 96<<16), and
 *     frames 32..79 top the pool up by sixteen embers per frame;
 *   - one-shot sounds fire on frames 0, 32 and 80 (Func_080f9010) and on
 *     frame 120 (Func_080b50e8);
 *   - frames 32..79 draw three growing columns from the byte pairs at
 *     Data_080ee1ac: a fixed 104-tall body and a span-tall foot whose height
 *     is (frame*16 - 256 + 25*i)/104;
 *   - up to frame 95 five 32x32 sky tiles scroll horizontally by frame/4 and
 *     rise by two pixels per frame over the first sixteen frames;
 *   - every live ember is drawn with a size that grows with its index, with
 *     an upward velocity and with the frame number, then integrated with
 *     gravity (per-lane from Data_080ee1b4 until frame 80, a fixed -0x8000
 *     afterwards) and 62/64 drag, and retired once it falls past y=104;
 *   - from frame 30, while frame <= 79, every listed party member is driven
 *     through Func_080d6888 on the frames where frame/12 is 0 or 6.
 * Afterwards the callback is removed, heap kinds 46/47 are released, the
 * focus point is handed to Func_080e6eac and the twelve spawned objects are
 * released through Func_08009038.
 *
 * Uncertain, and left as read from the reference:
 *   - the sixteen particle records seeded at work+0x7320 are never read here;
 *     they are consumed by the 0x080cd261 frame callback or by the spawned
 *     objects, so their field roles (x, y, life) follow the ember record
 *     layout rather than independent evidence.  The base is exactly
 *     particles[24], which is why the array is indexed rather than given its
 *     own name.
 *   - Particle.unk_08 and Particle.unk_14 are never touched by this owner.
 *   - Data_080ede48 (sprite offsets by size), Data_080ee1ac (column screen
 *     slots) and Data_080ee1b4 (four gravity lanes) are only known from their
 *     use here, so the names stay address-derived.
 *   - the two constants written into the member object at offsets 40 and 72
 *     (0x48000 and 0xab85) are opaque.
 *
 * Measured residual against the reference (1580 candidate bytes against 1576,
 * 622 differing halfwords, 479 wrong instructions).  Control flow, calls,
 * stores, aggregate offsets and the spill-slot layout of the frame all agree;
 * what remains is register naming inside the big loops plus three scheduling
 * differences that no evidenced source change reproduced:
 *   - the reference derives the -1 particle sentinel as 0 - 1 from the shared
 *     zero constant, while an ordinary -1 literal materialises 1 and negates;
 *   - the reference reloads rectangle_slot from its spill slot for the
 *     FetchRectangleBlitters argument instead of keeping it in a register;
 *   - the reference reaches work + 0x77d8 in the epilogue by subtracting 76
 *     from the still-live 0x7824 constant instead of loading a second pool
 *     word, which is the whole of the four-byte size difference.
 * Writing the blitter selector as an if-statement instead of the conditional
 * expression, and interleaving the focus-point integration with its damping,
 * were both tried and were measurably worse; they are not the answer.  So was
 * assigning the selector to its own local first (s32 slot = vy > 0 ? 1 : 0;
 * then rectangle_slot[slot]): that spelling costs four more bytes and scores
 * 702/499 instead of 622/479, so the reference's register-offset load is not
 * reached by naming the index either.
 */

#define BattleEffect_RunEmberShower Func_080d2d98

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);
typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

struct EffectArgument {
    u8 unknown_00[20];
    s32 member_count;                /* 0x14 */
    u8 unknown_18[12];
    s16 member_ids[8];               /* 0x24 */
};

typedef struct Particle {
    s32 x;                           /* 0x00 */
    s32 y;                           /* 0x04 */
    s32 unk_08;                      /* 0x08 */
    s32 vx;                          /* 0x0c */
    s32 vy;                          /* 0x10 */
    s32 unk_14;                      /* 0x14 */
    s32 life;                        /* 0x18 */
} Particle;

struct EffectRuntime {
    u8 sky_tiles[0x6e4];             /* 0x0000 */
    u8 column_tiles[0x699c];         /* 0x06e4 */
    Particle particles[64];          /* 0x7080 */
    s32 display_mode;                /* 0x7780 */
    s32 display_value;               /* 0x7784 */
    u8 unknown_7788[0x50];
    void *objects[12];               /* 0x77d8 */
    u8 unknown_7808[0x1c];
    s32 frame_ready;                 /* 0x7824 */
    struct EffectArgument *argument; /* 0x7828 */
};

extern u8 Value_0000006e;
extern u8 Value_00000073;
extern u8 Value_00000085;

extern u16 Data_080ede48[];
extern u8 Data_080ee1ac[];
extern s32 Data_080ee1b4[];

void Func_080cd594(s32 mode);
void BattleEffect_FetchRectangleBlitters(s32 alternate, u32 *output);
void *Func_08002f40(s32 id);
u32 Func_08005340(const void *source, void *destination);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
void Scheduler_RemoveCallback(void *callback);
s32 Func_08004458(void);
void BattleEffect_SelectLivingTargets(void *argument);
void Func_080030f8(s32 frames);
void BattleEffect_SpawnObjects(s32 entry_count, s32 kind, s32 variant);
void Func_080b50e8(s32 id);
void Func_080f9010(s32 id);
void Func_080e6d3c(s32 channel, s32 x, s32 y);
void Func_080e6eac(s32 channel, s32 x, s32 y);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
s32 Func_080022fc(s32 numerator, s32 denominator);
void **Func_080b5098(s32 member_id);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Runtime_ReleaseHeapBlock(s32 id);
void Func_08009038(void *object);
void Func_080cdbc0(void);

void BattleEffect_RunEmberShower(struct EffectArgument *object)
{
    void *canvas;
    struct EffectRuntime *work;
    u8 *graphics;
    u8 *palette;
    void *rectangle[2];
    void **rectangle_slot;
    s32 pos_y;
    s32 pos_x;
    s32 vel_y;
    s32 vel_x;
    s32 rise;
    s32 phase;
    Particle *particle;
    void **cursor;
    u8 *tbl;
    s32 frame;
    s32 seeded;
    s32 span;
    s32 i;

    cursor = (void **)0x03001EEC + 1;
    canvas = cursor[0];
    work = (struct EffectRuntime *)cursor[-1];
    graphics = (u8 *)cursor[1];
    work->argument = object;
    Func_080cd594(0);
    *(s16 *)0x04000052 = 0x1010;
    rectangle_slot = rectangle;
    BattleEffect_FetchRectangleBlitters(0, (u32 *)rectangle_slot);

    palette = (u8 *)Func_08002f40((s32)&Value_0000006e);
    ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    palette += 128;
    Func_08005340(palette, work);
    palette = (u8 *)Func_08002f40((s32)&Value_00000085);
    palette += 128;
    Func_08005340(palette, work->column_tiles);
    Func_08005340(Func_08002f40((s32)&Value_00000073), graphics);

    work->display_mode = 2;
    work->display_value = 75;
    Scheduler_AddOrUpdateCallback((void *)0x080CD261, 0x480);

    pos_y = 0x580000;
    pos_x = 0x1000000;
    vel_y = -0x40000;
    vel_x = -0x100000;
    rise = 0;

    for (i = 0; i != 64; i++)
        work->particles[i].life = -1;

    for (i = 0; i != 16; i++) {
        work->particles[i + 24].x = Func_08004458() & 127;
        work->particles[i + 24].y = (Func_08004458() & 7) + 56;
        work->particles[i + 24].life = -(Func_08004458() & 15);
    }

    for (i = 0; i != 1024; i++)
        ((Particle *)0x02010000)[i].life = -1;

    BattleEffect_SelectLivingTargets(work->argument);
    Func_080030f8(1);
    BattleEffect_SpawnObjects(12, 380, 2);

    for (frame = 0; frame != 124; frame++) {
        if ((*(s32 *)0x03001B04 & 3) != 0) {
            if (frame > 32) {
                if (frame <= 97)
                    frame = 98;
            }
        }
        if (frame == 120)
            Func_080b50e8(134);
        if (frame <= 15)
            rise += 2;
        if (frame <= 99) {
            pos_x = vel_x + pos_x;
            pos_y = vel_y + pos_y;
            vel_x = (vel_x * 58) / 64;
            vel_y = (vel_y * 56) / 64;
            if (pos_x <= 0x77FFFF)
                vel_x += 0x8000;
        }
        Func_080e6d3c(1, pos_x, pos_y);

        if (frame == 28) {
            particle = (Particle *)0x02010000;
            for (i = 0; i != 256; i++) {
                if (particle->life == -1) {
                    s32 radius;
                    s32 angle;

                    radius = Func_08004458() & 63;
                    angle = Func_08004458() & 0xFFFF;
                    particle->x =
                        ((Func_08002322(angle) * radius) >> 3) + 0x200000;
                    particle->y =
                        ((Func_0800231c(angle) * radius) >> 2) + 0x600000;
                    particle->vx = ((Func_08004458() & 63) - 32) << 14;
                    particle->vy = (-(Func_08004458() & 63) - 8) << 13;
                    particle->life = 0;
                }
                particle++;
            }
        }

        phase = frame - 32;
        if ((u32)phase <= 47) {
            seeded = 0;
            particle = (Particle *)0x02010000;
            for (i = 0; i != 1024; i++) {
                if (particle->life == -1) {
                    s32 radius;
                    s32 angle;

                    radius = Func_08004458() & 63;
                    angle = Func_08004458() & 0xFFFF;
                    particle->x =
                        ((Func_08002322(angle) * radius) >> 3) + 0x200000;
                    particle->y =
                        ((Func_0800231c(angle) * radius) >> 2) + 0x600000;
                    particle->vx = ((Func_08004458() & 63) - 32) << 14;
                    particle->vy = (-(Func_08004458() & 63) - 8) << 13;
                    particle->life = 0;
                    seeded++;
                    if (seeded == 16)
                        break;
                }
                particle++;
            }
        }

        if (frame == 0)
            Func_080f9010(164);
        if (frame == 32)
            Func_080f9010(145);
        if (frame == 80)
            Func_080f9010(144);

        if ((u32)phase <= 47) {
            tbl = Data_080ee1ac;
            span = (frame * 16) - 256;
            for (i = 0; i != 3; i++) {
                s32 foot;

                foot = Func_080022fc(span, 104);
                ((DrawRectangleFn)rectangle[0])(
                    canvas, work->column_tiles,
                    tbl[0] - 17, (tbl[1] - foot) - 104, 34, 104);
                ((DrawRectangleFn)rectangle[0])(
                    canvas, work->column_tiles,
                    tbl[0] - 17, tbl[1] - foot, 34, foot);
                tbl += 2;
                span += 25;
            }
        }

        if (frame <= 95) {
            for (i = 0; i != 5; i++) {
                ((DrawRectangleFn)rectangle[0])(
                    canvas, work,
                    ((i << 5) + ((frame / 4) & 31)) - 32, 120 - rise, 32, 32);
            }
        }

        particle = (Particle *)0x02010000;
        for (i = 0; i != 1024; i++) {
            if (particle->life >= 0) {
                s32 size;

                size = Func_080022fc(i, 3) + 2;
                if (particle->vy > 0)
                    size += 2;
                if (frame > 68) {
                    if (size <= 5)
                        size = 6;
                }
                if (frame > 70) {
                    if (size <= 6)
                        size = 7;
                }
                if (frame > 72) {
                    if (size <= 7)
                        size = 8;
                }
                if (frame > 74) {
                    if (size <= 8)
                        size = 9;
                }
                if (frame > 76)
                    size = 10;
                ((DrawRectangleFn)rectangle_slot[particle->vy > 0 ? 1 : 0])(
                    canvas, graphics + Data_080ede48[size - 1],
                    M2C_FIELD(particle, s16 *, 2) - (size / 2),
                    M2C_FIELD(particle, s16 *, 6) - size,
                    size, size * 2);
                particle->x += particle->vx;
                particle->y += particle->vy;
                if (frame > 80)
                    particle->vy = particle->vy - 0x8000;
                else
                    particle->vy = particle->vy + Data_080ee1b4[i & 3];
                particle->vx = (particle->vx * 62) / 64;
                particle->vy = (particle->vy * 62) / 64;
                particle->life++;
                if (particle->vy > 0) {
                    if (M2C_FIELD(particle, s16 *, 6) > 104)
                        particle->life = -1;
                }
            }
            particle++;
        }

        if (frame <= 79) {
            if (work->argument->member_count != 0) {
                s32 id_ofs;

                id_ofs = 36;
                i = 0;
                do {
                    if (frame > 29) {
                        s32 step;

                        step = Func_080022fc(frame, 12);
                        if (step == 0) {
                            void *member;

                            member = *Func_080b5098(
                                M2C_FIELD(work->argument, s16 *, id_ofs));
                            Func_080d6888(
                                M2C_FIELD(work->argument, s16 *, id_ofs),
                                7, 5, -1, step);
                            M2C_FIELD(member, s32 *, 40) = 0x48000;
                            M2C_FIELD(member, s32 *, 72) = 0xAB85;
                        }
                        if (step == 6) {
                            Func_080d6888(
                                M2C_FIELD(work->argument, s16 *, id_ofs),
                                0, 5, -1, 0);
                        }
                    }
                    id_ofs += 2;
                    i++;
                } while (i != work->argument->member_count);
            }
        }

        work->frame_ready = 1;
        Func_080030f8(1);
    }

    Scheduler_RemoveCallback((void *)0x080CD261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Func_080e6eac(1, pos_x, pos_y);
    cursor = work->objects;
    for (i = 0; i != 12; i++) {
        Func_08009038(*cursor++);
    }
    Func_080cdbc0();
}
