#include "types.h"

/*
 * Owner 080dd9c0 -- one semantic module spanning three manifest rows.
 *
 *   080dd9c0  code 0x58 (+0x24 literal pool at 080dda18)  prologue
 *   080dda3c  code 0xd0 (+0x28 literal pool at 080ddb0c)  continuation
 *   080ddb34  code 0x284 (pool at 080dddb8, filed as an executable gap)
 *
 * The advertised 124-byte "FunctionHead_080dd9c0" is only the prologue: it
 * saves r4-r11/lr, opens a 52-byte frame, and leaves the frame live across
 * `bne sub_080dda3c` / `b sub_080dda3c`.  The continuation at 080dda3c leaves
 * the same frame live across `b sub_080ddb34`, `b sub_080ddcda` and
 * `b sub_080ddd90`, and the row at 080ddb34 branches back to 080ddaba.  There
 * is exactly one return (`pop {r0}; bx r0` at the end of 080ddb34), so all
 * three rows are one function and are reconstructed here as one.
 *
 * This is the sibling of Func_080dd2c4 (same effect family, different variant
 * tables and a per-emitter style table instead of a variant argument).
 */

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

#define REG16_080DD9C0(address) (*(volatile u16 *)(address))
#define REG32_080DD9C0(address) (*(volatile u32 *)(address))

/*
 * The two blit entry points are not direct calls.  The assembly loads them
 * from the runtime header and reaches them through the compiler's inter-mode
 * thunks: `bl sub_080072fc` is _call_via_r6 and `bl sub_080072f4` is
 * _call_via_r4.  Argument order is (source, destination, x, y, width, height),
 * with width/height passed on the stack.
 */
typedef void (*Blit_080dd9c0)(s32 source, void *destination, s32 x, s32 y,
                              s32 width, s32 height);

struct Particle_080dd9c0 {
    s32 x;
    s32 y;
    s32 unknown_08;
    s32 unknown_0c;
    s32 unknown_10;
    s32 unknown_14;
    s32 age;
};

/* 0x03001eec is the runtime header; only words 0, 1, 7 and 8 are used here. */
struct RuntimeHeader_080dd9c0 {
    u8 *base;            /* 0x03001eec */
    s32 graphics_source; /* 0x03001ef0 */
    s32 unknown_08[5];
    Blit_080dd9c0 blit_forward; /* 0x03001f08 */
    Blit_080dd9c0 blit_mirror;  /* 0x03001f0c */
};

/*
 * ROM tables (hyou_b_050/051/052 in assets/code/sentou_kouka_hyou_b.json).
 * 0x080eebb6 holds the emitter count keyed by scene field 0x18; the pillar
 * and spark tables that follow it are addressed as separate bases by the
 * original code, so they are declared that way here.
 */
#define EMITTER_STYLE_080DD9C0 ((const u8 *)0x080eeba6)  /* 8 entries, 0..3  */
#define EMITTER_X_080DD9C0     ((const s8 *)0x080eebae)  /* 8 entries        */
#define EMITTER_COUNT_080DD9C0 ((const u8 *)0x080eebb6)  /* keyed by kind    */
#define SPARK_HALF_W_080DD9C0  ((const s8 *)0x080eebb9)  /* 7 phases         */
#define SPARK_HEIGHT_080DD9C0  ((const s8 *)0x080eebc0)  /* 7 phases         */
#define SPARK_SOURCE_080DD9C0  ((const u16 *)0x080eebc8) /* 7 phases         */

#define PARTICLES_080DD9C0 ((struct Particle_080dd9c0 *)0x02010000)

/* Slots cleared at start; only the first 64 are spawned into or animated. */
#define PARTICLE_CLEAR_COUNT_080DD9C0 0x400
#define PARTICLE_LIVE_COUNT_080DD9C0  64

void Func_080cd594(s32 mode);
void Func_080e0524(s32 resource, void *runtime, s32 arg2, s32 arg3);
void Func_080ed408(s32 resource, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080041d8(const void *handler, s32 size);
void Func_080b50e8(s32 sound);
u32 Func_08004458(void);
void Func_080f9010(s32 sound);
void Func_080d6888(s16 target, s32 arg1, s32 arg2, s32 index, s32 arg4);
void Func_080e155c(s32 x, s32 y);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08004278(const void *handler);
void Func_08002dd8(s32 resource);
s32 Func_080cdbc0(void);

static s32 EmitterCount_080dd9c0(u8 *runtime)
{
    void *scene = PTR_AT(runtime, 0x7828);

    return EMITTER_COUNT_080DD9C0[(u32)S32_AT(scene, 0x18)];
}

/* Spawns one spark below the emitter, or nothing when every slot is busy. */
static void SpawnSpark_080dd9c0(s32 emitter_x)
{
    s32 y = (s32)(Func_08004458() & 31) + 72;
    s32 slot;

    for (slot = 0; slot < PARTICLE_LIVE_COUNT_080DD9C0; slot++) {
        struct Particle_080dd9c0 *particle = &PARTICLES_080DD9C0[slot];
        s32 x;

        if (particle->age != -1)
            continue;

        x = emitter_x + (s32)(Func_08004458() & 31) + 32;
        particle->x = x;
        if (x > 96)
            particle->x = 96;
        particle->y = y;
        particle->age = 0;
        return;
    }
}

static void AnimateSparks_080dd9c0(struct RuntimeHeader_080dd9c0 *header,
                                   u8 *runtime)
{
    s32 slot;

    for (slot = 0; slot < PARTICLE_LIVE_COUNT_080DD9C0; slot++) {
        struct Particle_080dd9c0 *particle = &PARTICLES_080DD9C0[slot];
        s32 phase;
        s32 half_width;
        s32 height;
        void *source;

        if (particle->age < 0)
            continue;

        /* age / 2, rounded toward zero, exactly as the original shifts. */
        phase = (particle->age + (s32)((u32)particle->age >> 31)) >> 1;
        half_width = SPARK_HALF_W_080DD9C0[phase];
        height = SPARK_HEIGHT_080DD9C0[phase];
        source = runtime + SPARK_SOURCE_080DD9C0[phase];

        /* Left half is mirrored, right half is not; both share one row. */
        header->blit_forward(header->graphics_source, source,
                             particle->x - half_width,
                             particle->y - ((height + (height < 0)) >> 1),
                             half_width, height);
        header->blit_mirror(header->graphics_source, source,
                            particle->x,
                            particle->y - ((height + (height < 0)) >> 1),
                            half_width, height);

        particle->age++;
        if (particle->age == 14)
            particle->age = -1;
    }
}

static void DrawEmitter_080dd9c0(struct RuntimeHeader_080dd9c0 *header,
                                 u8 *runtime, s32 style, s32 emitter_x,
                                 s32 elapsed)
{
    Blit_080dd9c0 blit =
        (style & 1) ? header->blit_mirror : header->blit_forward;
    s32 offset;
    s32 height;

    if ((u32)style <= 1) {
        /* Wide pillar: grows sixteen pixels per frame up to eighty. */
        height = elapsed * 16;
        if (height > 80)
            height = 80;
        offset = elapsed * 6;
        if (offset > 30)
            offset = 30;

        blit(header->graphics_source, runtime,
             (style & 1) ? emitter_x - offset : emitter_x + offset,
             108 - height, 48, height);
    } else {
        /* Narrow pillar, drawn from a different tile page. */
        height = elapsed * 8;
        if (height > 64)
            height = 64;
        offset = elapsed;
        if (offset > 8)
            offset = 8;

        blit(header->graphics_source, runtime + 0xf00,
             (style & 1) ? emitter_x - offset : emitter_x + offset,
             108 - height, 32, height);
    }
}

s32 Func_080dd9c0(void *scene)
{
    struct RuntimeHeader_080dd9c0 *header =
        (struct RuntimeHeader_080dd9c0 *)0x03001eec;
    u8 *runtime = header->base;
    s32 graphics_source = header->graphics_source;
    s32 frame_count;
    s32 frame;
    s32 i;

    (void)graphics_source; /* kept live in the frame; read through `header`. */

    PTR_AT(runtime, 0x7828) = scene;
    Func_080cd594(1);
    REG16_080DD9C0(0x04000020) = 0x0100; /* BG2PA */
    REG16_080DD9C0(0x04000050) = 0;      /* BLDCNT */
    REG16_080DD9C0(0x04000052) = 0x1010; /* BLDALPHA */
    Func_080e0524(0x7e, runtime, 1, 1);
    if (S32_AT(scene, 4) == 1)
        REG32_080DD9C0(0x04000028) = 0xffff9000; /* BG2X */

    /* --- continuation 080dda3c ------------------------------------------- */

    Func_080ed408(0x2e, 7, 7, 3, 1);
    Func_080ed408(0x2f, 7, 7, 7, 1);

    S32_AT(runtime, 0x7780) = 1;
    S32_AT(runtime, 0x7784) = 0;
    Func_080041d8((const void *)0x080cd261, 0x480);

    frame_count = EmitterCount_080dd9c0(runtime) * 8 + 56;

    for (i = 0; i < PARTICLE_CLEAR_COUNT_080DD9C0; i++)
        PARTICLES_080DD9C0[i].age = -1;

    frame = 0;
    while (frame_count != 0) {
        s32 emitter;

        if (frame == frame_count - 64)
            Func_080b50e8(132);

        if (frame >= frame_count - 16) {
            REG16_080DD9C0(0x04000050) = 0x3f44;
            REG16_080DD9C0(0x04000052) =
                (u16)((frame_count - frame - 1) | 0x1000);
        }

        /* --- continuation 080ddb34: one pass over the live emitters ------- */

        if (EmitterCount_080dd9c0(runtime) != 0) {
            emitter = 0;
            do {
                s32 start = 8 + emitter * 8;
                s32 emitter_x = EMITTER_X_080DD9C0[emitter];
                void *current;

                if (frame > start) {
                    DrawEmitter_080dd9c0(header, runtime,
                                         EMITTER_STYLE_080DD9C0[emitter],
                                         emitter_x, frame - start);

                    if (frame == start + 1)
                        S32_AT(runtime, 0x77a8) = 3;

                    if (frame < start + 3)
                        SpawnSpark_080dd9c0(emitter_x);
                }

                /*
                 * The cue test below does not depend on the member index, so
                 * every member is cued on the same frame.  That is what the
                 * original does: the compare is hoisted out of nothing and the
                 * loop re-reads the scene pointer on each pass.
                 */
                current = PTR_AT(runtime, 0x7828);
                if (S32_AT(current, 0x14) != 0) {
                    s32 member = 0;
                    do {
                        if (frame == start + 4) {
                            Func_080f9010(132);
                            Func_080d6888(
                                S16_AT(PTR_AT(runtime, 0x7828),
                                       36 + member * 2),
                                7, 5, member, 3);
                        }
                        member++;
                    } while (member != S32_AT(PTR_AT(runtime, 0x7828), 0x14));
                }

                emitter++;
            } while (emitter != EmitterCount_080dd9c0(runtime));
        }

        AnimateSparks_080dd9c0(header, runtime);

        Func_080e155c(8, 8);
        Func_080cd52c();
        S32_AT(runtime, 0x7824) = 1;
        Func_080030f8(1);

        frame++;
        if (frame == frame_count)
            break;
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    return Func_080cdbc0();
}

/*
 * Uncertainties, recorded against the assembly:
 *
 * - The start-up clear covers 0x400 particle records at 0x02010000, but the
 *   spawn search and the animation pass only ever touch the first 64.  The
 *   surplus clear is in the original (0x080dda92 loop bound 0x400); it is kept
 *   rather than narrowed.
 * - Header words 7 and 8 (0x03001f08 / 0x03001f0c) are copied into the frame
 *   once at 080dda50 / 080dda76 and reloaded before each thunked blit.  They
 *   are modelled as function pointers because the call sites use the
 *   _call_via_r4 / _call_via_r6 thunks at 080072f4 / 080072fc, not direct
 *   branches.  The sixth blit argument is a height in every call, but the
 *   fifth is a constant 48/32 for pillars and a table half-width for sparks,
 *   so "width" is the best-supported reading, not a proven one.
 * - Scene fields are addressed by offset: 0x04 selects the BG2X preset, 0x14
 *   is a member count, 0x18 keys the emitter-count table, 0x24.. is an array
 *   of s16 member ids.  No independent struct evidence names them.
 * - Runtime slots 0x7780, 0x7784, 0x77a8 and 0x7824 are written but never read
 *   here; the values (1, 0, 3, 1) are taken verbatim.
 * - The style table at 0x080eeba6 holds 0..3; bit 0 selects the mirrored blit
 *   and values above 1 select the narrow pillar drawn from runtime + 0xf00.
 *   The clamp `if (elapsed > 8) offset = 8` in the narrow case uses the
 *   unclamped elapsed count, matching 080ddbb0.
 */
