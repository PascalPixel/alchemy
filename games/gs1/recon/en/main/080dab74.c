#include "types.h"

/*
 * Battle-presentation sub-effect at 0x080dab74 (1776 bytes).  It belongs to
 * the same 0x03001eec "battle work" family as the already-exact
 * games/gs1/src/battle/effects/member_orbit/run.c (main:080ce85c) and the
 * measured drafts games/gs1/recon/en/main/080da6cc.c and
 * games/gs1/recon/en/main/080db264.c; the work-block layout (64 twenty-eight
 * byte particle records at 0x7080, display mode/value at 0x7780/0x7784,
 * frame_ready at 0x7824 and the republished effect argument at 0x7828) and
 * the DrawRectangleFn calling convention (an indirect call through the r4
 * slot of the _call_via_rN trampoline at games/gs1/asm/080072e4.s) are taken
 * from those owners.  The blitter pair comes from the exact
 * games/gs1/src/battle/effects/work/fetch_rectangle_blitters.c.
 *
 * Behaviour: sixty-four Mote records in the work block are seeded with a
 * random ring angle, radius and (negative) height.  Each animation frame the
 * owner rebuilds the affine view, then
 *   - draws and advances the first `count` motes (count is the per-variant
 *     byte Data_080eea88[variant*3] while the frame is still below the
 *     variant's timing base Data_080eea88[variant*3+2], and 16 afterwards).
 *     Before the timing base a mote spirals inwards and downwards; from the
 *     timing base on it climbs, and the climbing extent is tracked in
 *     ymin/ymax;
 *   - draws up to Data_080eea88[variant*3+1] fixed "column" sprites whose
 *     screen slots come from the byte pairs at Data_080eea62, using the
 *     second blitter once the frame reaches the timing base minus seven;
 *   - on the timing-base frame exactly, seeds thirty-two Ember records at
 *     0x02010000 and re-seeds the motes as a falling shower spread over the
 *     recorded ymin..ymax band;
 *   - from the timing base on, draws twenty-four Embers and runs the motes'
 *     twelve-step (frame 17..0) shower sprite sequence, re-arming a mote at
 *     frame 17 until the run is thirty-five frames past the timing base.
 * The run lasts Data_080eea88[variant*3+2] + 75 frames.
 *
 * Uncertain, and left as read from the reference:
 *   - Ember.unk_08 is seeded but never read, while Ember.vx (offset 0x0c) is
 *     added to Ember.x every frame without ever being seeded.  This looks
 *     like an original off-by-one-field slip; it is reproduced as written.
 *   - Mote.unk_14 (offset 0x14) is never touched at all.
 *   - The roles of Data_080eea62/91/99/a2/b2/b8/bb/c3/cc are only known from
 *     their use here (screen slot pairs, widths, heights and sprite offsets),
 *     so the names stay address-derived.
 */

#define BattleEffect_RunRisingMotes Func_080dab74

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *destination,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct EffectArgument {
    u8 unknown_00[4];
    s32 direction;           /* 0x04 */
    s32 source_id;           /* 0x08 */
    u8 unknown_0c[8];
    s32 member_count;        /* 0x14 */
    s32 variant;             /* 0x18 */
    u8 unknown_1c[8];
    s16 member_ids[8];       /* 0x24 */
};

typedef struct Mote {
    s32 angle;               /* 0x00 */
    s32 height;              /* 0x04 */
    s32 radius;              /* 0x08 */
    s32 x;                   /* 0x0c */
    s32 y;                   /* 0x10 */
    s32 unk_14;              /* 0x14 */
    s32 frame;               /* 0x18 */
} Mote;

typedef struct Ember {
    s32 x;                   /* 0x00 */
    s32 y;                   /* 0x04 */
    s32 unk_08;              /* 0x08 */
    s32 vx;                  /* 0x0c */
    s32 vy;                  /* 0x10 */
    s32 unk_14;              /* 0x14 */
    s32 life;                /* 0x18 */
} Ember;

struct EffectRuntime {
    u8 unknown_0000[0x7080];
    Mote motes[64];          /* 0x7080 */
    s32 display_mode;        /* 0x7780 */
    s32 display_value;       /* 0x7784 */
    u8 unknown_7788[0x9c];
    s32 frame_ready;         /* 0x7824 */
    struct EffectArgument *argument;   /* 0x7828 */
};

struct RuntimeCells {
    struct EffectRuntime *runtime;
    void *draw_destination;
    u8 *graphics;
};

extern u8 Data_080ede84[];
extern u8 Data_080ede96[];
extern u8 Data_080eea62[];
extern u8 Data_080eea88[];
extern u8 Data_080eea91[];
extern u8 Data_080eea99[];
extern u16 Data_080eeaa2[];
extern u16 Data_080eeab2[];
extern u8 Data_080eeab8[];
extern u8 Data_080eeabb[];
extern u8 Data_080eeac3[];
extern u16 Data_080eeacc[];
extern char Value_000000b8;
extern char Value_000000ba;

void Func_080cd594(s32 mode);
void Func_080e0524(void *resource_id, void *destination, s32 upper, s32 palette);
void BattleFx_FetchRectangleBlitters(s32 alternate, u32 *output);
s32 Func_08004458(void);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_080b50e8(s32 value);
void Func_080049ac(void);
void Func_08004cb4(s32 *position);
void Func_080f9010(s32 value);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080022fc(s32 numerator, s32 denominator);
s32 Func_08002304(s32 value, s32 range);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080e3944(void *source, void *screen);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 resource_id);
void Func_080cdbc0(void);

void BattleEffect_RunRisingMotes(struct EffectArgument *object)
{
    struct RuntimeCells *cells;
    void **cursor;
    struct EffectRuntime *work;
    void *canvas;
    u8 *graphics;
    void *rectangle[2];
    void **rectangle_slot;
    s32 view[3];
    s32 record[3];
    s32 screen[3];
    Mote *mote;
    s32 frame;
    s32 phase;
    s32 count;
    s32 ymin;
    s32 ymax;
    s32 i;

    cells = (struct RuntimeCells *)0x03001EEC;
    cursor = (void **)cells;
    work = *cursor++;
    canvas = *cursor;
    graphics = cells->graphics;
    work->argument = object;
    Func_080cd594(1);
    *(u16 *)0x04000020 = 0x100;
    Func_080e0524(&Value_000000b8, work, 1, 1);
    Func_080e0524(&Value_000000ba, graphics, 0, 0);
    rectangle_slot = rectangle;
    BattleFx_FetchRectangleBlitters(0, (u32 *)rectangle_slot);

    mote = work->motes;
    for (i = 0; i != 64; i++) {
        mote->angle = Func_08004458() & 0xffff;
        mote->radius = (Func_08004458() & 63) + 56;
        mote->height = ((Func_08004458() & 31) - 64) << 16;
        mote++;
    }

    work->display_mode = 2;
    work->display_value = 50;
    Func_080041d8((void *)0x080CD261, 0x480);
    if (work->argument->direction == 1)
        *(u32 *)0x04000028 = 0xffff9000;

    for (frame = 0;
            frame != Data_080eea88[work->argument->variant * 3 + 2] + 75;
            frame++) {
        s32 j;
        s32 k;
        s32 m;
        s32 limit;

        ymin = 0x780000;
        ymax = 0;

        if (frame == Data_080eea88[work->argument->variant * 3 + 2] + 11)
            Func_080b50e8(132);

        view[0] = 0;
        view[1] = 0;
        view[2] = 0x2000000;
        Func_080049ac();
        Func_08004cb4(view);

        phase = frame - 36;
        if ((u32)phase <= 27 && (frame & 3) == 0)
            Func_080f9010(115);
        if (frame == 85)
            Func_080f9010(136);

        if (work->argument->member_count != 0) {
            i = 0;
            do {
                if (frame == i * 4 + 40) {
                    Func_080d6888(
                        work->argument->member_ids[i], 9, 5, -1, 0);
                }
                i++;
            } while (i != work->argument->member_count);
        }

        count = 16;
        if (frame < Data_080eea88[work->argument->variant * 3 + 2])
            count = Data_080eea88[work->argument->variant * 3];

        if (frame < Data_080eea88[work->argument->variant * 3 + 2] + 35) {
            mote = work->motes;
            for (j = 0; j != count; j++) {
                if (frame > j) {
                    s32 slot;
                    s32 ceiling;

                    slot = j % 8;
                    ceiling = (48 - j / 2) << 16;
                    if (mote->height < ceiling
                            && mote->height > -0x300000) {
                        s32 width;
                        s32 height;

                        record[0] = mote->radius * Func_08002322(mote->angle);
                        record[1] = mote->height;
                        record[2] = mote->radius * Func_0800231c(mote->angle);
                        Func_080e3944(record, screen);
                        screen[0] = (screen[0] >> 17) + 64;
                        screen[1] = M2C_FIELD(screen, s16 *, 6) + 60;
                        width = Data_080eea91[slot];
                        height = Data_080eea99[slot];
                        ((DrawRectangleFn)rectangle_slot[1])(
                            canvas,
                            (u8 *)work + Data_080eeaa2[slot],
                            screen[0] - ((u32)width >> 1),
                            screen[1] - ((u32)height >> 1),
                            width,
                            height);
                    }

                    if (frame
                            < Data_080eea88[
                                work->argument->variant * 3 + 2]) {
                        if (frame > j + 16) {
                            if (mote->radius > 4)
                                mote->radius -= 2;
                            if (mote->height <= 0x2fffff)
                                mote->height += 0x50000;
                            mote->angle += 512;
                        }
                    } else {
                        mote->radius += 8;
                        mote->height -= (Func_080022fc(j, 5) + 2) << 16;
                        if (ymin > mote->height)
                            ymin = mote->height;
                        if (ymax < mote->height)
                            ymax = mote->height;
                    }
                }
                mote++;
            }
        }

        ymin += 0x400000;
        ymax += 0x400000;

        if (frame < Data_080eea88[work->argument->variant * 3 + 2]) {
            for (k = 0;
                    k != Data_080eea88[work->argument->variant * 3 + 1];
                    k++) {
                if (k < Func_080022ec(phase, 3)) {
                    s32 image;
                    s32 height;

                    image = Func_080022fc(k, 3);
                    if (frame
                            >= Data_080eea88[
                                work->argument->variant * 3 + 2] - 7) {
                        height = Data_080eeab8[image];
                        ((DrawRectangleFn)rectangle_slot[1])(
                            canvas,
                            (u8 *)work + Data_080eeab2[image],
                            Data_080eea62[k * 2],
                            Data_080eea62[k * 2 + 1] - height,
                            32,
                            height);
                    } else {
                        height = Data_080eeab8[image];
                        ((DrawRectangleFn)rectangle[0])(
                            canvas,
                            (u8 *)work + Data_080eeab2[image],
                            Data_080eea62[k * 2],
                            Data_080eea62[k * 2 + 1] - height,
                            32,
                            height);
                    }
                }
            }
        }

        if (frame == Data_080eea88[work->argument->variant * 3 + 2]) {
            Ember *ember;

            ember = (Ember *)0x02010000;
            for (m = 0; m != 32; m++) {
                ember->x = (Func_08004458() & 127) << 16;
                ember->y = ((Func_08004458() & 15) + 80) << 16;
                ember->unk_08 = ((Func_08004458() & 63) - 32) << 12;
                ember->vy = ((-Func_08004458() & 15) - 16) << 13;
                ember->life = (Func_08004458() & 15) + 16;
                ember++;
            }
        }

        if (frame >= Data_080eea88[work->argument->variant * 3 + 2]) {
            Ember *ember;

            ember = (Ember *)0x02010000;
            for (m = 0; m != 24; m++) {
                if (ember->life >= 0) {
                    s32 slot;

                    slot = m % 8;
                    ((DrawRectangleFn)rectangle_slot[1])(
                        canvas,
                        (u8 *)work + Data_080eeacc[slot],
                        M2C_FIELD(ember, s16 *, 2),
                        M2C_FIELD(ember, s16 *, 6),
                        Data_080eeabb[slot],
                        Data_080eeac3[slot]);
                    ember->x += ember->vx;
                    ember->y += ember->vy;
                    ember->life--;
                }
                if (ymin > ember->y)
                    ymin = ember->y;
                if (ymax < ember->y)
                    ymax = ember->y;
                ember++;
            }
        }

        ymin >>= 16;
        ymax >>= 16;
        if (ymax <= ymin)
            ymax = ymin + 1;

        if (frame == Data_080eea88[work->argument->variant * 3 + 2]) {
            mote = work->motes;
            for (m = 0; m != 32; m++) {
                mote->x = (Func_08004458() & 127) << 16;
                if (ymax == ymin) {
                    mote->y = ymin << 16;
                } else {
                    mote->y =
                        (Func_08002304(Func_08004458(), ymax - ymin) + ymin)
                        << 16;
                }
                mote->frame = (Func_08004458() & 15) + 20;
                mote++;
            }
        }

        if (frame >= Data_080eea88[work->argument->variant * 3 + 2]) {
            s32 drop;

            drop = (frame - Data_080eea88[work->argument->variant * 3 + 2])
                / 2;
            mote = work->motes;
            for (m = 0; m != 32; m++) {
                if ((u32)mote->frame <= 17) {
                    s32 image;
                    s32 size;
                    s32 half;

                    image = (17 - mote->frame) / 2;
                    size = Data_080ede96[image];
                    half = (u32)size >> 1;
                    ((DrawRectangleFn)rectangle_slot[1])(
                        canvas,
                        graphics + Data_080ede84[image],
                        M2C_FIELD(mote, s16 *, 14) - half,
                        (M2C_FIELD(mote, s16 *, 18) - half) - drop,
                        size,
                        size);
                }
                mote->frame--;
                if (mote->frame == -1 || mote->frame == 17) {
                    if (frame
                            < Data_080eea88[
                                work->argument->variant * 3 + 2] + 35) {
                        mote->frame = 17;
                        mote->x = (Func_08004458() & 127) << 16;
                        mote->y =
                            (Func_08002304(Func_08004458(), ymax - ymin)
                                + ymin) << 16;
                    }
                }
                mote++;
            }
        }

        limit = 1;
        work->frame_ready = limit;
        Func_080030f8(1);
    }

    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_08004278((void *)0x080CD261);
    Func_080cdbc0();
}
