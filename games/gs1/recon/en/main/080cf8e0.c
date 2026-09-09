#include "types.h"

/*
 * Battle-presentation sub-effect at 0x080cf8e0 (1556 bytes).  It belongs to
 * the same 0x03001eec "battle work" family as the exact
 * games/gs1/src/battle/effects/member_orbit/run.c (main:080ce85c) and
 * games/gs1/src/battle/effects/puff_arc/run.c (main:080d9fc8), and to the
 * measured draft games/gs1/recon/en/main/080dab74.c.  The work-block layout
 * (display mode/value at 0x7780/0x7784, frame_ready at 0x7824, the
 * republished effect argument at 0x7828), the effect-argument layout
 * (direction 0x04, source id 0x08, member count 0x14, member ids 0x24) and
 * the DrawRectangleFn convention (an indirect call through the _call_via_rN
 * trampolines at games/gs1/asm/080072e4.s: 0x080072f0 = r3, 0x080072f4 = r4,
 * 0x080072f8 = r5) are taken from those owners.
 *
 * Behaviour: the owner takes the effect argument and a small variant index.
 * It loads a fixed palette plus its 16000-byte sprite bank, a second palette
 * plus the bank that follows it, and a third palette chosen from the variant
 * (a six-way if chain over the resource-id constants 0x9f, 0x59, 0xa0, 0xbf,
 * 0x8d and the shared default 0x77).  Data_080ee0b6[variant * 2] is the
 * style flag; it selects the second rectangle blitter's shape, the run
 * length, the spark count, the spark velocity spread, the spark sprite bank
 * and the gravity sign, so the two styles are two different presentations of
 * the same sequence.
 *
 * The 1024 twenty-eight byte records at 0x02010000 are cleared (frame = -1),
 * then sixteen consecutive records per party member are seeded with the
 * caster's position and a velocity that reaches that member's position in
 * sixteen steps.  Only record [member * 16] is handled here; the other
 * fifteen and the whole seeded set are advanced by the 0x080CD261 callback
 * this owner installs.  The 256 records at 0x02011c00 - which is
 * &record[256] of the same table - are the sparks, also cleared to -1.
 *
 * Each of member_count * 8 + 72 (or + 56) frames:
 *   - rebuilds the transfer state;
 *   - for every member whose window is open (frame >= member * 8), draws its
 *     lead record with sprite cell min((frame - member * 8) / 3, 9) taken
 *     from work + cell * 768, either 32x24 or 24x32; on the frame numbered
 *     member * 8 + 17 it runs update_members and plays cue 133; on the
 *     window's first frame it starts the record falling, and when the record
 *     passes y = 0 it lands and scatters four or sixteen sparks with random
 *     velocities into 0x02011c00 + (member * 32 + k) * 28;
 *   - draws and advances every spark whose frame counter is in 0..44, using
 *     cell frame / 8 of the second bank for style 0 and the width/height/
 *     offset tables Data_080ee0c4 / Data_080ee0d6 / Data_080ee0e8 for the
 *     other style, mirrored by the argument direction and the sign of the
 *     spark's x velocity;
 *   - shakes the camera by (2, 2), ticks the object-group timers, publishes
 *     the frame and waits one frame.
 *
 * Uncertain, and left as read from the reference:
 *   - the two clear loops overlap: the first clears 1024 records from
 *     0x02010000, the second re-clears records 256..511 through the separate
 *     spelling 0x02011c00.  Both write -1, so the overlap is harmless; it is
 *     reproduced as written rather than folded into one loop.
 *   - the lead records' gravity call is gated on frame == 0, so it runs once
 *     per record; every later position update comes from the 0x080CD261
 *     callback.
 *   - Data_080ee0b6 is fourteen bytes (seven variants, stride two) and only
 *     the first byte of each pair is read here, so the pair's second byte and
 *     the roles of Data_080ee0c4 / Data_080ee0d6 / Data_080ee0e8 beyond
 *     width, height and source offset stay address-derived.
 *
 * Measured residual (drafted, NOT exact, and not close to exact).  Scored
 * with the approved route: candidate 1560 bytes against 1556 reference bytes
 * (100.3%), 739 differing halfwords, 643 wrong instructions, and the scorer
 * still reports topology=different.  The aligned comparison is 666 candidate
 * instructions against 670 reference instructions with 277 identical, 305
 * substituted and 152/151 unpaired, so the block sequence does track the
 * reference end to end - but "tracks" is as strong as the claim gets; this is
 * a readable draft of the right program, not a near match.  Every reference
 * branch, loop, switch arm, call, store and access width was checked against
 * the disassembly and is represented.  The residual is register allocation:
 *   - the reference keeps the work-block pointer in fp for the whole owner,
 *     so each of its sixteen uses is one `add rN, fp`; this source leaves it
 *     in a stack slot, which costs an extra load at each use and renames the
 *     registers downstream.  GCC here spends fp on the short-lived spark
 *     count instead.  Making `variant` die early (a `style` index local,
 *     matching the reference's spilled `variant * 2`) was tried and made the
 *     match worse; the emitted code already spills `variant * 2` to one slot
 *     and reads it at all seven style sites exactly as the reference does,
 *     so `variant` is not what is holding the work pointer out of fp.
 *   - the reference recomputes the spark slot address from the member index
 *     inside the scatter loop; this source lets the compiler strength-reduce
 *     it into a member-loop induction variable, which is four instructions
 *     shorter and adds one more long-lived value.
 */

#define BattleEffect_RunImpactBurst Func_080cf8e0

typedef void (*DrawRectangleFn)(
    void *destination,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

typedef s32 (*WordCopyFn)(void *destination, const void *source, s32 words);

struct EffectArgument {
    u8 unknown_00[4];
    s32 direction;           /* 0x04 */
    s32 source_id;           /* 0x08 */
    u8 unknown_0c[8];
    s32 member_count;        /* 0x14 */
    u8 unknown_18[12];
    s16 member_ids[8];       /* 0x24 */
};

/* One twenty-eight byte motion record.  The same shape is used for the
   member shards at 0x02010000 and the sparks at 0x02011c00. */
typedef struct Shard {
    s32 x;                   /* 0x00 */
    s32 y;                   /* 0x04 */
    s32 z;                   /* 0x08 */
    s32 vx;                  /* 0x0c */
    s32 vy;                  /* 0x10 */
    s32 vz;                  /* 0x14 */
    s32 frame;               /* 0x18 */
} Shard;

/* The projected object Func_080b5098 resolves; only the two position words
   this owner reads are named. */
struct ActorObject {
    u8 unknown_00[8];
    s32 x;                   /* 0x08 */
    u8 unknown_0c[4];
    s32 z;                   /* 0x10 */
};

struct EffectRuntime {
    u8 sprites[0x7780];      /* 0x0000 */
    s32 display_mode;        /* 0x7780 */
    s32 display_value;       /* 0x7784 */
    u8 unknown_7788[0x9c];
    s32 frame_ready;         /* 0x7824 */
    struct EffectArgument *argument;   /* 0x7828 */
};

/* Heap-allocation cache: Data_03001e50[kind] holds kind's block address.
   This owner reads kinds 46 and 47 through it. */
extern u8 Data_03001e50[];

/* Value_ symbols carry a resource id the reference loads from its literal
   pool rather than materializing with a mov. */
extern u8 Value_00000059;
extern u8 Value_00000077;
extern u8 Value_0000008d;
extern u8 Value_0000009e;
extern u8 Value_0000009f;
extern u8 Value_000000a0;
extern u8 Value_000000bf;

extern s8 Data_080ee0b6[];
extern u8 Data_080ee0c4[];
extern u8 Data_080ee0d6[];
extern u16 Data_080ee0e8[];

/* Names the project already resolves for this owner's callees; the four
   lowercase spellings are static in their own units, so they are aliased
   here the way games/gs1/recon/en/main/08027114.c aliases random_16. */
#define update_members Func_080d6888
#define apply_base_and_y_offset Func_080e3944
#define random_16 Func_08004458
#define advance_with_gravity_3d Func_080e38b8

void Func_080cd594(s32 mode);
void *Func_08002f40(s32 resource_id);   /* "get" in alchemy inspect */
u32 Func_08005340(const void *source, void *destination);
struct ActorObject **Func_080b5098(s32 member_id);
s32 Func_080ed408(s32 resource_id, s32 a, s32 b, s32 c, s32 d);
void Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
void Scheduler_RemoveCallback(void *callback);
void Func_080f9010(s32 cue);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void update_members(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080b50e8(s32 cue);
s32 Func_080022ec(s32 numerator, s32 denominator);
void apply_base_and_y_offset(void *source, void *screen);
void advance_with_gravity_3d(void *record, s32 mode, s32 gravity);
s32 random_16(void);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void Func_080030f8(s32 frames);
void Runtime_ReleaseHeapBlock(s32 resource_id);
void Func_080cdbc0(void);

void BattleEffect_RunImpactBurst(struct EffectArgument *object, s32 variant)
{
    void **cursor;
    struct EffectRuntime *work;
    void *canvas;
    s32 facing;
    void *rectangle[2];
    void **rectangle_slot;
    struct ActorObject *source;
    struct ActorObject *target;
    Shard *shard;
    Shard *spark;
    u8 *palette;
    u8 *state;
    s32 resource_id;
    s32 total;
    s32 frame;
    s32 member;
    s32 start;
    s32 count;
    s32 cell;
    s32 i;
    s32 screen[3];

    cursor = (void **)0x03001EEC;
    work = (struct EffectRuntime *)*cursor++;
    canvas = *cursor;
    facing = *(s32 *)0x03001E80;
    work->argument = object;
    Func_080cd594(1);
    *(u16 *)0x04000052 = 0x1010;

    palette = (u8 *)Func_08002f40((s32)&Value_000000bf);
    ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    Func_08005340(palette + 128, work);
    palette = (u8 *)Func_08002f40((s32)&Value_0000009e);
    ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    Func_08005340(palette + 128, work->sprites + 16000);

    /* An if chain, not a switch: the reference tests 0, 1, 2, 3, 4 and 6 in
       order and cross-jumps the variant-3 arm onto the default arm, which is
       what this shape produces and a jump table does not. */
    if (variant == 0)
        resource_id = (s32)&Value_0000009f;
    else if (variant == 1)
        resource_id = (s32)&Value_00000059;
    else if (variant == 2)
        resource_id = (s32)&Value_000000a0;
    else if (variant == 3)
        resource_id = (s32)&Value_00000077;
    else if (variant == 4)
        resource_id = (s32)&Value_000000bf;
    else if (variant == 6)
        resource_id = (s32)&Value_0000008d;
    else
        resource_id = (s32)&Value_00000077;
    palette = (u8 *)Func_08002f40(resource_id);
    ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);

    for (i = 0; i != 1024; i++)
        ((Shard *)0x02010000)[i].frame = -1;

    source = *Func_080b5098(work->argument->source_id);
    member = 0;
    if (work->argument->member_count != 0) {
        do {
            target = *Func_080b5098(work->argument->member_ids[member]);
            shard = &((Shard *)0x02010000)[member * 16];
            for (i = 0; i != 16; i++) {
                shard->x = source->x;
                shard->y = 0x40000;
                shard->z = source->z;
                shard->vx = (target->x - source->x) >> 4;
                shard->vy = 0x40000;
                shard->vz = (target->z - source->z) >> 4;
                shard->frame = 0;
                shard++;
            }
            member++;
        } while (member != work->argument->member_count);
    }

    for (i = 0; i != 256; i++)
        ((Shard *)0x02011C00)[i].frame = -1;

    if (object->direction == 0) {
        Func_080ed408(46, 7, 7, 3, 2);
        state = Data_03001e50;
        rectangle[0] = *(void **)(state + 184);
        if (Data_080ee0b6[variant * 2] == 0)
            Func_080ed408(47, 7, 7, 3, 3);
        else
            Func_080ed408(47, 7, 7, 7, 2);
    } else {
        Func_080ed408(46, 7, 7, 7, 2);
        state = Data_03001e50;
        rectangle[0] = *(void **)(state + 184);
        if (Data_080ee0b6[variant * 2] == 0)
            Func_080ed408(47, 7, 7, 3, 3);
        else
            Func_080ed408(47, 7, 7, 3, 2);
    }
    rectangle_slot = rectangle;
    rectangle_slot[1] = *(void **)(state + 188);

    work->display_mode = 2;
    work->display_value = 75;
    Scheduler_AddOrUpdateCallback((void *)0x080CD261, 0x480);

    if (Data_080ee0b6[variant * 2] == 0)
        total = (work->argument->member_count * 8) + 72;
    else
        total = (work->argument->member_count * 8) + 56;

    Func_080f9010(103);

    for (frame = 0; frame != total; frame++) {
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, facing + 12);

        member = 0;
        if (work->argument->member_count != 0) {
            shard = (Shard *)0x02010000;
            start = 0;
            do {
                if (frame >= start) {
                    if (frame == start + 17) {
                        update_members(
                            work->argument->member_ids[member], 7, 5, member, 16);
                        Func_080b50e8(133);
                    }
                    if (shard->frame >= 0) {
                        cell = Func_080022ec(frame - start, 3);
                        if (cell > 9)
                            cell = 9;
                        apply_base_and_y_offset(shard, screen);
                        screen[0] >>= 1;
                        if (cell > 4) {
                            ((DrawRectangleFn)rectangle[0])(
                                canvas,
                                work->sprites + (cell * 768),
                                screen[0] - 16,
                                screen[1] - 12,
                                32,
                                24);
                        } else {
                            ((DrawRectangleFn)rectangle[0])(
                                canvas,
                                work->sprites + (cell * 768),
                                screen[0] - 12,
                                screen[1] - 16,
                                24,
                                32);
                        }
                        if (shard->frame == 0)
                            advance_with_gravity_3d(shard, 63, -0x8000);
                        if (shard->y < 0) {
                            shard->y = 0;
                            shard->frame = 1;
                            if (Data_080ee0b6[variant * 2] == 0)
                                count = 4;
                            else
                                count = 16;
                            for (i = 0; i != count; i++) {
                                spark = &((Shard *)0x02011C00)[
                                    (member * 32) + i];
                                spark->x = shard->x;
                                spark->y = shard->y;
                                spark->z = shard->z;
                                if (Data_080ee0b6[variant * 2] == 0) {
                                    spark->vx =
                                        ((random_16() & 63) - 32) << 11;
                                    spark->vy = 0;
                                    spark->vz =
                                        ((random_16() & 63) - 32) << 11;
                                } else {
                                    spark->vx =
                                        ((random_16() & 63) - 32) << 13;
                                    spark->vy =
                                        ((random_16() & 31) + 32) << 12;
                                    spark->vz =
                                        ((random_16() & 63) - 32) << 13;
                                }
                                spark->frame = 0;
                            }
                        }
                    }
                }
                shard += 16;
                start += 8;
                member++;
            } while (member != work->argument->member_count);
        }

        spark = (Shard *)0x02011C00;
        for (i = 0; i != 256; i++) {
            /* The folded range test is what the reference's single unsigned
               compare came from: an unstarted spark keeps frame = -1. */
            if ((spark->frame >= 0) && (spark->frame <= 44)
                    && (spark->y >= 0)) {
                apply_base_and_y_offset(spark, screen);
                screen[0] >>= 1;
                if (Data_080ee0b6[variant * 2] == 0) {
                    ((DrawRectangleFn)rectangle_slot[1])(
                        canvas,
                        work->sprites + ((spark->frame / 8) * 1152) + 16000,
                        screen[0] - 12,
                        screen[1] - 24,
                        24,
                        48);
                } else {
                    s32 index;
                    s32 side;
                    s32 wide;
                    s32 high;

                    index = Func_080022ec(spark->frame, 5);
                    if ((i & 1) != 0)
                        index += 9;
                    side = object->direction;
                    if (spark->vx > 0)
                        side ^= 1;
                    /* Each table entry is read once and reused for both the
                       centring shift and the size argument, as in the exact
                       games/gs1/src/battle/effects/puff_arc/run.c. */
                    wide = Data_080ee0c4[index];
                    high = Data_080ee0d6[index];
                    ((DrawRectangleFn)rectangle_slot[side])(
                        canvas,
                        work->sprites + Data_080ee0e8[index] + 7680,
                        screen[0] - (wide >> 1),
                        screen[1] - (high >> 1),
                        wide,
                        high);
                }
                if (Data_080ee0b6[variant * 2] == 0)
                    advance_with_gravity_3d(spark, 62, 0x800);
                else
                    advance_with_gravity_3d(spark, 62, -0x8000);
                spark->frame++;
            }
            spark++;
        }

        Camera_ApplyShake(2, 2);
        ObjectGroup_TickMemberTimers();
        work->frame_ready = 1;
        Func_080030f8(1);
    }

    Scheduler_RemoveCallback((void *)0x080CD261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Func_080cdbc0();
}
