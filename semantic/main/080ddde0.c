/*
 * 0x080ddde0 -- the largest gray item in the core band.  One function,
 * span 0x080ddde0-0x080de2c2, with its OWN literal pool sitting inside it
 * at 0x080de08c/72 (the entry `ldr r5, [pc, #668]` at 0x080dddee reads
 * from it, and the `b.n 0x080de0da` at 0x080de08a jumps over it), plus a
 * two-byte `movs r0, r0` alignment pad at 0x080de2c2 and a 52-byte trailing
 * pool at 0x080de2c4.  Executable code is therefore two ranges,
 * 0x080ddde0/684 and 0x080de0d4/494 -- NOT the 548 the manifest region
 * length suggests, because that region length runs to 0x080de2f8 and so
 * swallows the pad and the trailing pool.
 *
 * The whole shape was recovered from the sibling family that shares this
 * function's callee set and its runtime-header idiom: semantic/main/
 * 080dfa48.c, semantic/main/080e01e4.c, semantic/main/080dfe2c.c and the
 * exact sources src/080cef64.c, src/080e3908.c, src/080e3980.c,
 * src/080e0524.c, src/080e155c.c.  Nothing below is a guess about the
 * calling idiom; the uncertain parts are flagged in place.
 *
 * Two decoding notes that mattered here:
 *
 *  - `bl 0x080072f4` and `bl 0x080072f8` are NOT calls to functions at
 *    those addresses.  0x080072e4 begins the GCC `__call_via_rN` veneer
 *    table (`bx rN; nop`, four bytes apiece), so 0x080072f4 is
 *    `__call_via_r4` and 0x080072f8 is `__call_via_r5`.  Every such site
 *    is an INDIRECT call through the register loaded immediately before
 *    it -- which is exactly why r4 is reloaded from a pooled global right
 *    before each blit and never read afterwards.  semantic/main/
 *    080c1ffc.c already recorded the r5 half of this.
 *
 *  - src/080cef64.c is the exact source for the resource-46/47 renderer
 *    idiom this function inlines by hand: Func_080ed408(46, 7, 7, ...)
 *    publishes a renderer pointer at Data_03001e50 + 184 = 0x03001f08 and
 *    Func_080ed408(47, 7, 7, ...) publishes one at + 188 = 0x03001f0c.
 *    Those are the two pooled addresses this function reads.
 */

#include "effect_step.h"
#include "layout_guard.h"
#include "types.h"

/*
 * The argument.  object_count at 0x14 and the s16 object_ids array at 0x24
 * are the same layout semantic/main/080e01e4.c reconstructed for its own
 * scene argument, reached here through the identical
 * `*(void **)(runtime + 0x7828)` slot.  `variant` at 0x18 selects a row of
 * the effect table at 0x080eebd6; the reference reloads it through the
 * runtime slot on nearly every use, which is why it is read from
 * `*scene_slot` rather than cached.
 */
struct Scene_080ddde0 {
    u8 unknown_00[0x14];
    s32 object_count;
    s32 variant;
    u8 unknown_1c[0x08];
    s16 object_ids[1];
};

LAYOUT_OFFSET_GUARD(
    Scene080ddde0_ObjectCount,
    struct Scene_080ddde0,
    object_count,
    0x14);
LAYOUT_OFFSET_GUARD(
    Scene080ddde0_Variant,
    struct Scene_080ddde0,
    variant,
    0x18);
LAYOUT_OFFSET_GUARD(
    Scene080ddde0_ObjectIds,
    struct Scene_080ddde0,
    object_ids,
    0x24);

typedef void (*Renderer_080ddde0)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

/*
 * The resident routine at 0x03000168, reached through __call_via_r5.  Its
 * body lives in IWRAM and is not part of this ROM region, so only the
 * argument shape is asserted here: this function always calls it with the
 * render context, 0x4000 and 0x10101010.  Uncertainty: the role of the
 * two constants is not established -- 0x4000 is a plausible byte count and
 * 0x10101010 a plausible four-pixel fill pattern, but neither is proven
 * from this function alone, so the parameters stay unnamed.
 */
typedef void (*Resident_080ddde0)(void *context, s32 arg1, s32 arg2);

/*
 * Effect table at 0x080eebd6, four u8 columns per `variant`, three rows
 * (0x080eebd6..0x080eebe2, where the next table begins):
 *
 *     variant 0: { 16, 0, 15, 1 }
 *     variant 1: { 32, 3, 31, 1 }
 *     variant 2: { 128, 5, 48, 3 }
 *
 * Column roles are read off the three use sites below: [0] is the burst
 * particle count, [1] the sparkle count, [2] the sparkle scatter span,
 * [3] the beam repeat count.  The monotonic progression across rows is
 * consistent with variant being an intensity tier, but that reading is not
 * load-bearing for the transcription.
 */
#define EFFECT_TABLE_080DDDE0 ((const u8 *)0x080eebd6)
#define BURST_COUNT_080DDDE0 0
#define SPARKLE_COUNT_080DDDE0 1
#define SPARKLE_SPAN_080DDDE0 2
#define BEAM_REPEAT_080DDDE0 3

/* Sparkle palette step, indexed by two random bits: { 0, 4, 8, 12 }. */
#define SPARKLE_PALETTE_080DDDE0 ((const u8 *)0x080eebe2)
/* Per-variant argument to the resource-47 load: { 2, 2, 3 }. */
#define SPARKLE_TONE_080DDDE0 ((const u8 *)0x080eebe6)

/*
 * The four sparkle shapes.  These three tables verify each other: the
 * source offsets are a running sum of width * height, so
 * offset[n + 1] - offset[n] == width[n] * height[n] holds for every n
 * (589 = 19*31, 713 = 23*31, 713 = 23*31, 399 = 19*21).  That is what
 * pins width to 0x080edeca and height to 0x080eded0 rather than the other
 * way round.
 */
#define SPARKLE_SOURCE_080DDDE0 ((const u16 *)0x080edebe)
#define SPARKLE_WIDTH_080DDDE0 ((const u8 *)0x080edeca)
#define SPARKLE_HEIGHT_080DDDE0 ((const u8 *)0x080eded0)

/*
 * Spark tile offsets by size, { 0, 2, 10, 28, 60, 110, 182, 280 } -- the
 * running sum of 2 * n * n, matching the size-by-size-times-two blit
 * below.  semantic/main/080dfa48.c and semantic/main/080e01e4.c use this
 * same table identically.
 */
#define SPARK_TILE_080DDDE0 ((const u16 *)0x080ede48)

/* The 1024 shared spark slots, eight groups of 128. */
#define SPARK_POOL_080DDDE0 ((struct EffectStep *)0x02010000)
#define SPARK_GROUP_STRIDE_080DDDE0 0x0e00
#define SPARK_POOL_COUNT_080DDDE0 1024

s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
s32 Func_08002304(s32 value, s32 limit);
void Func_08002dd8(s32 resource);
void Func_080030f8(s32 frames);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(const void *source);
u32 Func_08004458(void);
void Func_080b5088(s32 object_id, s32 arg1);
void Func_080b50e8(s32 sound);
void Func_080cd52c(void);
void Func_080cd594(s32 mode);
void Func_080cdbc0(void);
void Func_080d6888(s32 object_id, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080e0524(s32 resource, void *destination, s32 arg2, s32 arg3);
void Func_080e155c(s32 arg0, s32 arg1);
void Func_080e3908(struct EffectStep *step, s32 damping, s32 gravity);
void Func_080e3980(s32 object_id, struct EffectPosition *position);
void Func_080ed408(s32 resource, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080f9010(s32 sound);

void Func_080ddde0(struct Scene_080ddde0 *scene)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    u8 *effect_tiles = (u8 *)runtime_header[2];
    Resident_080ddde0 resident = (Resident_080ddde0)0x03000168;
    struct Scene_080ddde0 **scene_slot =
        (struct Scene_080ddde0 **)(runtime + 0x7828);
    struct EffectStep *local_sparks =
        (struct EffectStep *)(runtime + 0x7080);
    Renderer_080ddde0 beam_renderer;
    struct EffectPosition position;
    s32 frame;
    s32 index;
    s32 step;
    s32 id_offset;
    s32 group_offset;

    *scene_slot = scene;
    Func_080cd594(1);
    Func_080ed408(46, 7, 7, 3, 2);
    beam_renderer = (Renderer_080ddde0)*(u32 *)0x03001f08;

    Func_080e0524(0xce, runtime, 1, 0);
    Func_080e0524(0xc4, runtime + 0xc56, 1, 1);
    Func_080e0524(0x73, effect_tiles, 0, 0);

    for (index = 0; index < SPARK_POOL_COUNT_080DDDE0; index++)
        SPARK_POOL_080DDDE0[index].variant = 0;
    for (index = 0; index < 64; index++)
        local_sparks[index].variant = -1;

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080f9010(138);

    /*
     * The frame bound is re-read from the scene through the runtime slot
     * on every iteration, and the reference guards the whole loop with a
     * separate zero-trip test (`(object_count << 3) == -40`), which is the
     * compiler's guard for a `!=` termination test.  Written as `<` here;
     * for any non-negative object_count the two agree.
     */
    for (frame = 0; frame < (*scene_slot)->object_count * 8 + 40; frame++) {
        if (frame == 24)
            Func_080b50e8(133);

        for (index = 0; index < (*scene_slot)->object_count; index++) {
            if (frame == index * 8)
                resident(render_context, 0x4000, 0x10101010);
        }

        id_offset = 0x24;
        group_offset = 0;

        for (index = 0; index < (*scene_slot)->object_count; index++) {
            struct EffectStep *group =
                (struct EffectStep *)((u8 *)SPARK_POOL_080DDDE0 +
                                      group_offset);
            s32 base = index * 8;
            s16 object_id =
                *(s16 *)((u8 *)*scene_slot + id_offset);
            s32 count;

            Func_080e3980(object_id, &position);
            position.x /= 2;

            if (frame == base + 1)
                *(s32 *)(runtime + 0x77a8) = 4;

            if (frame == base + 4) {
                object_id = *(s16 *)((u8 *)*scene_slot + id_offset);
                Func_080d6888(object_id, 7, 5, index, 6);
                Func_080b5088(
                    *(s16 *)((u8 *)*scene_slot + id_offset), 6);
            }

            /*
             * The rising beam column: sixteen frames from this object's
             * own start, a 24-wide strip whose height grows 64 pixels a
             * frame and caps at 104.  Each repeat picks one of four
             * 2880-byte source frames (24 * 120) at runtime + 0xc56,
             * cycling on (index + frame + step) / 2.
             */
            if (frame >= base && frame < base + 16) {
                s32 height = (frame - base) << 6;

                if (height > 104)
                    height = 104;

                count = EFFECT_TABLE_080DDDE0[
                    (*scene_slot)->variant * 4 + BEAM_REPEAT_080DDDE0];
                for (step = 0; step < count; step++) {
                    s32 phase = ((index + frame + step) / 2) & 3;

                    beam_renderer(
                        render_context,
                        runtime + 0xc56 + phase * 45 * 64,
                        position.x - 12,
                        0,
                        24,
                        height);
                    count = EFFECT_TABLE_080DDDE0[
                        (*scene_slot)->variant * 4 +
                        BEAM_REPEAT_080DDDE0];
                }

                /*
                 * On the beam's third frame the object's spark group is
                 * seeded: every particle starts at the beam's foot,
                 * y = 0x680000 in 16.16, thrown along a random angle in
                 * [-0x4000, 0x4000).
                 */
                if (frame == base + 2) {
                    count = EFFECT_TABLE_080DDDE0[
                        (*scene_slot)->variant * 4 +
                        BURST_COUNT_080DDDE0];
                    for (step = 0; step < count; step++) {
                        struct EffectStep *spark = &group[step];
                        s32 magnitude =
                            (s32)(Func_08004458() & 0x1ff);
                        s32 angle =
                            (s32)(Func_08004458() & 0x7fff) - 0x4000;

                        magnitude += 64;
                        spark->x = position.x << 16;
                        spark->y = 0x680000;
                        spark->velocity_x =
                            magnitude * Func_08002322(angle) >> 5;
                        spark->velocity_y =
                            -(magnitude * Func_0800231c(angle)) >> 6;
                        spark->variant =
                            (s32)(Func_08004458() & 7) + 32;
                        count = EFFECT_TABLE_080DDDE0[
                            (*scene_slot)->variant * 4 +
                            BURST_COUNT_080DDDE0];
                    }
                }
            }

            /*
             * The sparkle shower, frames base+2 through base+23.  Each
             * sparkle lands at a random point under the object: the
             * vertical offset is uniform over the variant's span, and the
             * horizontal spread is what is LEFT of the span after that
             * (span - dy + 1), so the scatter is widest at the top and
             * narrows downward.  Resource 47 is re-loaded per sparkle with
             * a random palette step and released again immediately, which
             * is why the renderer pointer at 0x03001f0c is re-read inside
             * the loop rather than hoisted.
             */
            if (frame >= base + 2 && frame < base + 24) {
                count = EFFECT_TABLE_080DDDE0[
                    (*scene_slot)->variant * 4 +
                    SPARKLE_COUNT_080DDDE0];
                for (step = 0; step < count; step++) {
                    s32 shape = step & 3;
                    s32 span;
                    s32 offset;
                    s32 x;
                    s32 y;
                    Renderer_080ddde0 sparkle_renderer;

                    span = EFFECT_TABLE_080DDDE0[
                        (*scene_slot)->variant * 4 +
                        SPARKLE_SPAN_080DDDE0];
                    offset = Func_08002304((s32)Func_08004458(), span);
                    y = position.y - offset;
                    y -= SPARKLE_HEIGHT_080DDDE0[shape] >> 1;
                    y += 8;

                    span = span - offset + 1;
                    x = position.x +
                        Func_08002304((s32)Func_08004458(), span);
                    x -= span / 2;
                    x -= SPARKLE_WIDTH_080DDDE0[shape] >> 1;

                    Func_080ed408(
                        47,
                        7,
                        7,
                        3 | SPARKLE_PALETTE_080DDDE0[
                                Func_08004458() & 3],
                        SPARKLE_TONE_080DDDE0[(*scene_slot)->variant]);
                    sparkle_renderer =
                        (Renderer_080ddde0)*(u32 *)0x03001f0c;
                    sparkle_renderer(
                        render_context,
                        runtime + SPARKLE_SOURCE_080DDDE0[shape],
                        x,
                        y,
                        SPARKLE_WIDTH_080DDDE0[shape],
                        SPARKLE_HEIGHT_080DDDE0[shape]);
                    Func_08002dd8(47);

                    count = EFFECT_TABLE_080DDDE0[
                        (*scene_slot)->variant * 4 +
                        SPARKLE_COUNT_080DDDE0];
                }
            }

            id_offset += 2;
            group_offset += SPARK_GROUP_STRIDE_080DDDE0;
        }

        /*
         * The shared spark sweep.  All 1024 slots every frame, whichever
         * group they belong to.  `variant` is the remaining-frames
         * countdown here -- include/effect_step.h names the field for its
         * general role, and this function, like 080dfa48 and 080e01e4,
         * uses it as a lifetime.  A spark whose y has fallen past the
         * floor at 0x680000 bounces with half its vertical speed and is
         * not drawn that frame; otherwise it is drawn if it is still
         * on-screen, at a size that shrinks by one every sixteen frames.
         */
        for (index = 0; index < SPARK_POOL_COUNT_080DDDE0; index++) {
            struct EffectStep *spark = &SPARK_POOL_080DDDE0[index];

            if (spark->variant > 0) {
                spark->variant--;
                Func_080e3908(spark, 60, 0x1000);

                if (spark->y > 0x680000) {
                    spark->velocity_y = -spark->velocity_y / 2;
                } else if (spark->x <= 0x007effff && spark->y >= 0) {
                    s32 life = spark->variant;
                    s32 size;

                    if (life < 0)
                        life += 15;
                    size = (life >> 4) + 1;
                    beam_renderer(
                        render_context,
                        effect_tiles + SPARK_TILE_080DDDE0[size - 1],
                        (spark->x >> 16) - size / 2,
                        (spark->y >> 16) - size,
                        size,
                        size * 2);
                }
            }
        }

        Func_080e155c(2, 8);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
