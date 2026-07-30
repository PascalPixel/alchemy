#include "types.h"

/*
 * Func_080ec100 - complete multi-region owner.
 *
 * Executable ranges (embedded literal pools excluded):
 *   080ec100..080ec16f   (0x70 bytes)   prologue, pool 080ec170..080ec18f
 *   080ec190..080ec1d3   (0x44 bytes)   pool 080ec1d4..080ec1e7
 *   080ec1e8..080ec23b   (0x54 bytes)   pool 080ec23c..080ec263
 *   080ec264..080ec2e1   (0x7e bytes)   pool 080ec2e2..080ec2f7
 *   080ec2f8..080ec44b   (0x154 bytes)  pool 080ec44c..080ec473
 *   080ec474..080ec6f3   (0x280 bytes)  pool 080ec6f4..080ec727
 *   080ec728..080ecab9   (0x392 bytes)  pool 080ecaba..080ecae7
 *   080ecae8..080ecbb7   (0xd0 bytes)   pool 080ecbb8..080ecbd7
 *   080ecbd8..080ece51   (0x27a bytes)  pool 080ece52..080ece7b (outside owner rows)
 *
 * The manifest rows 080ec100/190/264/474/6f4/728/aba/ae8 are one function.  The
 * prologue at 080ec100 opens a 116-byte frame and saves r8-r11; the single
 * epilogue is at 080ece3e.  Every inter-row transfer is an unconditional or
 * conditional `b` (080ec16c, 080ec23a, 080ec448, 080ec444, 080ec6ee, 080ec6f2,
 * 080ec750, 080ec6ba, 080eca7e, 080eca8c, 080ecab8) reusing live sp, r8-r11 -
 * they are ordinary intra-function control flow and appear here as C flow, not
 * as calls.  All 87 `bl` sites in the owner are genuine external calls; none is
 * an internal edge.
 *
 * Note: the row map classifies 080ecaba (46 bytes) as code.  It is not.  It
 * disassembles to eleven literal words that are the pc-relative pool for
 * 080ec758..080eca90 (0x080ede5c, 0x4e20, 0xf0, 0x02010000, 0x080ede48,
 * 0x080eef78, 0x7824, 0x03001b04, 0x03001ad0, 0x080ec0e1, 0x04000040) plus one
 * halfword of alignment.  No branch or fallthrough reaches it: the instruction
 * at 080ecab8 is an unconditional `b 080ecae8`.  Counting every pool listed
 * above, the nine executable subranges total 3126 bytes and the eight embedded
 * pools total 284 bytes; 3126 + 284 = 3410 = the eight manifest rows.  (The
 * scope tool reports executable=3358 / pool=52 because it only recognises the
 * two rows that are pure data.)  The trailing pool at 080ece52..080ece7b is
 * this function's as well but lies outside the owner rows.
 *
 * Behavior: this is the full-screen "summon"/finisher cutscene driver.  It
 * takes over the display, seeds three particle systems from the shared RNG,
 * runs a 244-frame flight/impact sequence and then a 144-frame aftermath
 * sequence, and restores the caller's display state before returning.
 */

/* ------------------------------------------------------------------ */
/* Shared runtime layout                                              */
/* ------------------------------------------------------------------ */

/*
 * 0x03001eec is the runtime header used across this family.  Only the slots
 * this owner touches are named.
 */
struct RuntimeHeader_080ec100 {
    u8 *runtime;            /* +0x00 : work RAM base, held in r11 all along  */
    void *render_context;   /* +0x04 : first argument of the blit helpers    */
    u8 *graphics;           /* +0x08 : base of the packed tile source        */
    u32 unused_0c;
    u32 unused_10;
    void *display;          /* +0x14 : object whose +16 word is a line count */
    u32 unused_18;
    void *palette_a;        /* +0x1c                                         */
    void *palette_b;        /* +0x20                                         */
};

/*
 * 28-byte particle/sprite record.  The same shape backs three different
 * pools, and the fields are reused with slightly different meanings between
 * the phases; the names follow the dominant use.
 *
 * Uncertainty: in the frame-222 confetti pool (080ec6bc..080ec751) `velocity_y`
 * holds the *initial* y instead of a velocity, and the update adds it to y
 * every frame (an accelerating fall that snaps back to the spawn point when it
 * wraps negative).  That is what the assembly does; it is reproduced verbatim.
 */
struct Particle_080ec100 {
    s32 x;          /* +0x00 : 16.16 in the RAM pool, plain pixels in confetti */
    s32 y;          /* +0x04 */
    s32 reset_x;    /* +0x08 */
    s32 velocity_x; /* +0x0c */
    s32 velocity_y; /* +0x10 */
    s32 unused_14;  /* +0x14 : never written by this owner */
    s32 state;      /* +0x18 : -1 = free, >=0 = live countdown/age */
};

#define REG16_080EC100(address) (*(volatile u16 *)(address))

/* Work-RAM offsets relative to RuntimeHeader_080ec100::runtime. */
#define OFF_SPRITES_080EC100    0x7080  /* struct Particle_080ec100[64]  */
#define OFF_BEAM_TILES_080EC100 0x4e20  /* base of the sized dot tiles   */
#define OFF_MODE_080EC100       0x7780
#define OFF_SUBMODE_080EC100    0x7784
#define OFF_SHAKE_080EC100      0x77a8
#define OFF_FLASH_A_080EC100    0x77b4
#define OFF_FLASH_B_080EC100    0x77b8
#define OFF_OBJECTS_080EC100    0x77d8  /* void *[8] */
#define OFF_PRESENT_080EC100    0x7824
#define OFF_TARGET_080EC100     0x7828  /* the argument, republished here */

/*
 * The 1024-entry particle pool lives in EWRAM at 0x02010000.  During frames
 * 0..15 the first 130 bytes of the same buffer are reused as the scanline
 * distortion table consumed by the 0x080d66cd raster handler; the particles
 * are not drawn until frame 28, so the two uses do not overlap in time.
 */
#define PARTICLE_POOL_080EC100  ((struct Particle_080ec100 *)0x02010000)
#define PARTICLE_COUNT_080EC100 1024
#define WAVE_SCROLL_080EC100    ((volatile u16 *)0x02010000)
#define WAVE_TABLE_080EC100     ((volatile u8 *)0x02010002)

/* ------------------------------------------------------------------ */
/* External entry points                                              */
/* ------------------------------------------------------------------ */

s32 Func_080022fc(s32 value, s32 modulus);
s32 Func_0800231c(s32 angle);            /* cosine, 16.16-ish scale */
s32 Func_08002322(s32 angle);            /* sine */
void Func_08002dd8(s32 resource);
void Func_0800307c(s32 channel, s32 delay, void *callback);
void Func_080030f8(s32 frames);
void Func_080041d8(void *handler, s32 parameter);
void Func_08004278(void *handler);
s32 Func_08004458(void);                 /* pseudo-random word */

/*
 * Sized blit.  r0-r3 carry context/source/x/y, [sp+0] and [sp+4] carry the
 * two extents, and r4 carries the palette handle.  The r4 operand is loaded
 * immediately before every one of the fourteen call sites and is never live
 * afterwards, so it is a genuine seventh operand of a hand-written routine
 * rather than a saved register.  Uncertainty: the exact declared order in the
 * original source is unknown; sibling semantic files spell this prototype
 * differently.  Here it is (context, source, x, y, width, height, palette).
 */
void Func_080072f4(void *context, const void *source, s32 x, s32 y,
                   s32 width, s32 height, void *palette);
void Func_08007308(void *context, const void *source, s32 x, s32 y,
                   s32 width, s32 height);

void Func_08009008(void *object, s32 *placement, s32 *transform, s32 flags);
void Func_08009038(void *object);
void Func_080b5040(s32 a, s32 b, s32 c);
void Func_080b5088(s16 actor, s32 mode);
void Func_080b50e8(s32 sound);
void Func_080c9048(void);
void Func_080cd104(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080cd594(s32 mode);
void Func_080cdbc0(void);
void Func_080cde90(s32 x, s32 mode, s32 y, s32 span, s32 length);
void Func_080d6750(void *target);
void Func_080d67dc(void);
void Func_080d6888(s16 actor, s32 a, s32 b, s32 c, s32 d);
void Func_080dbb24(s32 a, s32 b, s32 c);
void Func_080e0524(s32 resource, void *destination, s32 a, s32 b);
void Func_080e155c(s32 a, s32 b);
void Func_080e3908(struct Particle_080ec100 *particle, s32 damping, s32 gravity);
void Func_080e396c(s16 actor, s32 *out);
void Func_080ed408(s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_080f9010(s32 sound);

/* ------------------------------------------------------------------ */
/* Read-only tables in ROM                                            */
/* ------------------------------------------------------------------ */

#define HALO_OFFSET_X_080EC100 ((const u8 *)0x080eef68)   /* 8 entries */
#define HALO_OFFSET_Y_080EC100 ((const u8 *)0x080eef70)   /* 8 entries */
#define HALO_TRANSFORM_080EC100 ((const s32 *)0x080edae8) /* 2 words   */
#define DOT_TILES_080EC100 ((const u16 *)0x080ede5c)      /* by width  */
#define SPARK_TILES_080EC100 ((const u16 *)0x080ede48)    /* by width  */
#define SPARK_GRAVITY_080EC100 ((const s32 *)0x080eef78)  /* 4 entries */

/* ------------------------------------------------------------------ */
/* Local helpers                                                      */
/* ------------------------------------------------------------------ */

static s32 Clamp_080ec100(s32 value, s32 low, s32 high)
{
    if (value < low)
        value = low;
    if (value > high)
        value = high;
    return value;
}

/* The original inlines a signed divide-by-power-of-two everywhere; plain C
 * division already rounds toward zero, so it is written out directly. */

/*
 * Seed one entry of the EWRAM particle pool with a random outward burst.
 * Used at 080ec34c (initial fill) and 080ec5a2 (frame 76 refill); the two
 * differ only in the vertical origin and in how the drift is derived.
 */
static void SeedBurst_080ec100(struct Particle_080ec100 *particle, s32 origin_x,
                               s32 refill)
{
    s32 magnitude = Func_08004458() & 0xff;
    s32 angle = Func_08004458() & 0xffff;

    particle->x = ((magnitude * Func_08002322(angle)) >> 3) + origin_x;
    particle->y = ((magnitude * Func_0800231c(angle)) >> 4) + 0x00600000;

    if (refill) {
        particle->velocity_x = (32 - (Func_08004458() & 63)) << 12;
        particle->velocity_y = (-(Func_08004458() & 31) - 16) << 13;
    } else {
        particle->velocity_x =
            (s32)(0xfffff800u - (u32)(Func_08004458() & 0x1fff)) << 5;
        particle->velocity_y =
            (s32)(0xffffe800u - (u32)(Func_08004458() & 0x0fff)) << 5;
    }

    particle->state = 0;
}

/* ------------------------------------------------------------------ */
/* Owner                                                              */
/* ------------------------------------------------------------------ */

void Func_080ec100(void *target)
{
    struct RuntimeHeader_080ec100 *header =
        (struct RuntimeHeader_080ec100 *)0x03001eec;
    u8 *runtime = header->runtime;
    void *render_context = header->render_context;
    u8 *graphics = header->graphics;
    void *display;
    void *palettes[2];

    struct Particle_080ec100 *sprites =
        (struct Particle_080ec100 *)(runtime + OFF_SPRITES_080EC100);
    struct Particle_080ec100 *particles = PARTICLE_POOL_080EC100;

    s32 frame;
    s32 i;
    s32 saved_line;
    s32 scroll_step;

    /* Flight path state, kept in the frame across the whole first loop. */
    s32 path_x;
    s32 path_y;
    s32 path_step_x;
    s32 path_step_y;

    s32 transform[2];
    s32 placement[4];

    /*
     * The caller's target descriptor is republished into work RAM so the
     * helpers below can reach it without another argument.
     */
    *(void **)(runtime + OFF_TARGET_080EC100) = target;

    Func_080cd594(0);
    Func_080c9048();

    /* Blank the two backdrop palette entries before taking over. */
    REG16_080EC100(0x05000000) = 0;
    REG16_080EC100(0x05000002) = 0;

    Func_080041d8((void *)0x080ec0e1, 1152);
    Func_0800307c(2, 0, (void *)0x080ec0f1);
    *(s32 *)(runtime + OFF_MODE_080EC100) = 0;
    Func_080041d8((void *)0x080cd261, 1152);

    REG16_080EC100(0x04000048) = 0x2137;   /* WININ  */
    Func_080cd104(1, 0);
    REG16_080EC100(0x04000040) = 0xf0f0;   /* WIN0H  */

    Func_080d6750(*(void **)(runtime + OFF_TARGET_080EC100));
    Func_080dbb24(8, 378, 1);
    *(s32 *)(0x03001ce0 + 16) = 240;
    Func_080030f8(1);
    Func_080b5040(1, 0x3d, 0);

    Func_080e0524(0x73, graphics, 0, 0);
    Func_080e0524(0x6e, runtime, 1, 1);
    Func_080e0524(0x76, runtime + OFF_BEAM_TILES_080EC100, 0, 0);

    REG16_080EC100(0x04000000) = 0x7741;   /* DISPCNT  */
    REG16_080EC100(0x04000020) = 0x0080;   /* BG2PA    */
    REG16_080EC100(0x04000052) = 0x1010;   /* BLDALPHA */
    REG16_080EC100(0x04000050) = 0x3f44;   /* BLDCNT   */

    scroll_step = 1;
    path_x = 140 << 17;      /* 0x01180000 */
    path_y = 200 << 16;      /* 0x00c80000 */
    path_step_x = (s32)0xfffc0000;
    path_step_y = (s32)0xfff80000;

    /* Preserve the window line count the caller had installed. */
    saved_line = *(u16 *)(0x03001ad0 + 4);

    display = header->display;
    *(s32 *)(runtime + OFF_MODE_080EC100) = 2;
    *(s32 *)(runtime + OFF_SUBMODE_080EC100) = 75;
    *(s32 *)((u8 *)display + 16) = scroll_step;

    Func_080ed408(46, 7, 7, 3, 3);
    palettes[0] = header->palette_a;
    Func_080ed408(47, 7, 7, 3, 2);
    palettes[1] = header->palette_b;

    REG16_080EC100(0x0400000c) = 0x0784;   /* BG3CNT */

    /* Free every slot of the EWRAM particle pool. */
    for (i = 0; i < PARTICLE_COUNT_080EC100; i++)
        particles[i].state = -1;

    /* 32 slow orbiting motes around the caster. */
    for (i = 0; i < 32; i++) {
        s32 angle = (Func_08004458() & 0x3fff) + 0x8000;
        s32 radius = (Func_08004458() & 0x7f) + 255;
        s32 x = (radius * Func_08002322(angle)) >> 2;
        s32 y = (radius * Func_0800231c(angle)) >> 2;

        sprites[i].x = x;
        sprites[i].y = y;
        sprites[i].velocity_x = (-x) / 32;
        sprites[i].velocity_y = (-y) / 32;
        sprites[i].state = 0;
    }

    /* 712 of the 1024 pool entries get an initial outward burst. */
    for (i = 0; i < 712; i++)
        SeedBurst_080ec100(&particles[i], 0x00800000, 0);

    /* ---------------------------------------------------------------- */
    /* Phase 1: 244 frames of approach, impact and confetti.            */
    /* ---------------------------------------------------------------- */

    frame = 0;
    for (;;) {
        if (frame == 24)
            Func_080f9010(162);
        if (frame == 76)
            Func_080f9010(164);
        if (frame == 154)
            Func_080f9010(142);
        if (frame == 222)
            Func_080f9010(145);

        /* Frames 0..15: raster distortion ramp. */
        if ((u32)frame <= 15) {
            if (frame == 1) {
                s32 j;

                for (j = 0; j < 128; j++)
                    WAVE_TABLE_080EC100[j] = (u8)(Func_08004458() & 63);
                *WAVE_SCROLL_080EC100 = 0;
                Func_080041d8((void *)0x080d66cd, 1152);
            }
            *WAVE_SCROLL_080EC100 =
                (u16)(*WAVE_SCROLL_080EC100 + scroll_step);
            scroll_step += 3;
            if (frame == 15)
                Func_08004278((void *)0x080d66cd);
        }

        /*
         * Frames 22.. : advance the flight path and place the eight halo
         * objects along it.  Before frame 22 both the path integration and
         * the halo placement are skipped (the original branches straight to
         * 080ec596), so `transform` stays untouched.
         */
        if (frame > 21) {
            transform[0] = HALO_TRANSFORM_080EC100[0];
            transform[1] = HALO_TRANSFORM_080EC100[1];

            if (frame <= 83) {
                if (frame > 48)
                    path_step_x = (path_step_x * 60) / 64;
                path_step_y = (path_step_y * 60) / 64;
                if (path_y > (132 << 16))
                    path_step_y += (s32)0xffffe000;
                else
                    path_step_y += 0x2000;
                path_x += path_step_x;
                path_y += path_step_y;
            } else if (frame <= 170) {
                if (frame <= 107)
                    path_step_x += 0x2000;
                path_step_x = (path_step_x * 60) / 64;
                path_x += path_step_x;
                if (frame <= 117) {
                    if (path_y > (138 << 16))
                        path_step_y += (s32)0xffffe000;
                    else
                        path_step_y += 0x2000;
                    path_step_y = (path_step_y * 56) / 64;
                    path_y += path_step_y;
                }
            }

            placement[1] = 0;
            placement[3] = 0;
            for (i = 0; i < 8; i++) {
                placement[0] =
                    (((HALO_OFFSET_X_080EC100[i] * 3) / 2) << 16) + path_x +
                    (s32)0xffe00000;
                placement[2] =
                    (((HALO_OFFSET_Y_080EC100[i] * 3) / 2) << 16) + path_y +
                    (s32)0xffe80000;
                Func_08009008(((void **)(runtime + OFF_OBJECTS_080EC100))[i],
                              placement, transform, 0);
            }
        }

        /* Impact: refill every retired pool slot with a fresh burst. */
        if (frame == 76) {
            for (i = 0; i < PARTICLE_COUNT_080EC100; i++) {
                if (particles[i].state == -1)
                    SeedBurst_080ec100(&particles[i], 0x00400000, 1);
            }
        }

        if (frame == 152) {
            Func_0800307c(2, 96, (void *)0x080ec0f1);
            *(s32 *)(runtime + OFF_FLASH_A_080EC100) = 24;
            *(s32 *)(runtime + OFF_FLASH_B_080EC100) = 0;
        }

        /* Frames 152..239: stagger the 32 orbiting motes outward. */
        if ((u32)(frame - 152) <= 87) {
            for (i = 0; i < 32; i++) {
                s32 start = 152 + i / 4;

                if (frame >= start && frame < start + 32) {
                    s32 size = (i & 3) + 5;
                    s32 extent = size * 2;
                    const u8 *source = runtime +
                        DOT_TILES_080EC100[size - 1] + OFF_BEAM_TILES_080EC100;

                    Func_080072f4(render_context, source,
                                  (s32)(s16)(sprites[i].x >> 16) + 112 - size,
                                  (s32)(s16)(sprites[i].y >> 16) + 62 - size,
                                  extent, extent, palettes[0]);
                    sprites[i].x += sprites[i].velocity_x;
                    sprites[i].y += sprites[i].velocity_y;
                }
            }
        }

        /* Frame 222: re-seed the same array as 64 falling confetti. */
        if (frame == 222) {
            for (i = 0; i < 64; i++) {
                s32 x = (Func_08004458() & 15) - 8;
                s32 y = (Func_08004458() & 15) - 8;

                if (x < 0)
                    x -= 4;
                else
                    x += 4;
                if (y < 0)
                    y -= 4;
                else
                    y += 4;

                x += 100;
                y += 52;
                sprites[i].velocity_x = -6;
                sprites[i].x = x;
                sprites[i].y = y;
                sprites[i].reset_x = x;
                sprites[i].velocity_y = y;
                sprites[i].state = 0;
            }
        }

        if (frame > 221) {
            for (i = 0; i < 64; i++) {
                if (frame >= 222 + i / 2) {
                    const u8 *source = runtime + DOT_TILES_080EC100[0] +
                        OFF_BEAM_TILES_080EC100;

                    Func_080072f4(render_context, source, sprites[i].x - 1,
                                  sprites[i].y - 1, 2, 2, palettes[0]);
                    sprites[i].x += sprites[i].velocity_x;
                    sprites[i].y += sprites[i].velocity_y;
                    if (sprites[i].y < 0) {
                        sprites[i].y = sprites[i].velocity_y;
                        sprites[i].x = sprites[i].reset_x;
                    }
                }
            }
        }

        /* Radial shockwave lines. */
        if (frame > 175) {
            s32 count = 5;

            if (frame > 221) {
                if ((u32)(frame - 223) <= 1)
                    count = 60;
                else
                    count = 40;
            }
            if (count > 60)
                count = 60;

            if (count != 0) {
                s32 span = count * 2;

                for (i = 0; i < count; i++) {
                    s32 step = (i + 1) / 2;
                    s32 length = Clamp_080ec100(span - step * 4, 12, 63);

                    if (i & 1)
                        step = -step;
                    Func_080cde90(step + 48, 0, step / 2 + 111,
                                  63 - (step + 1) / 2, length);
                }
            }
        }

        /* Growing core flash, three overlapping ramps. */
        if ((u32)(frame - 152) <= 39) {
            s32 size = (frame - 152) / 2 + 1;
            s32 extent;
            const u8 *source;

            if (size > 4)
                size = 4;
            extent = size * 2;
            source = runtime + DOT_TILES_080EC100[size - 1] +
                OFF_BEAM_TILES_080EC100;
            Func_080072f4(render_context, source, 108 - size, 60 - size,
                          extent, extent, palettes[1]);
        }

        if ((u32)(frame - 192) <= 7) {
            s32 size = (frame - 192) / 2 + 3;
            s32 extent;
            const u8 *source;

            if (size > 6)
                size = 6;
            extent = size * 2;
            source = runtime + DOT_TILES_080EC100[size - 1] +
                OFF_BEAM_TILES_080EC100;
            Func_080072f4(render_context, source, 108 - size, 60 - size,
                          extent, extent, palettes[1]);
        }

        if (frame > 199) {
            s32 quarter = (frame - 200) / 4;
            s32 size = quarter + 5;
            s32 extent;
            const u8 *source;

            if (size > 8)
                size = 8;
            extent = size * 2;
            source = runtime + DOT_TILES_080EC100[size - 1] +
                OFF_BEAM_TILES_080EC100;
            Func_080072f4(render_context, source, 108 - size, 60 - size,
                          extent, extent, palettes[1]);

            if (frame <= 213) {
                s32 inner = quarter + 1;

                if (inner > 4)
                    inner = 4;
                extent = inner * 2;
                source = runtime + DOT_TILES_080EC100[inner - 1] +
                    OFF_BEAM_TILES_080EC100;
                Func_080072f4(render_context, source, 100 - inner, 52 - inner,
                              extent, extent, palettes[1]);
            }
        }

        /* Three-frame impact sprite, then the four-frame settle sprite. */
        if ((u32)(frame - 214) <= 7) {
            s32 index = (frame - 214) / 4;

            if (index > 2)
                index = 2;
            Func_080072f4(render_context,
                          runtime + ((index * 9) << 8) + 1024,
                          63, 18, 48, 48, palettes[1]);
        }

        if (frame > 221) {
            s32 half = (frame - 222) / 2;
            s32 index = half % 4 + 3;

            Func_080072f4(render_context,
                          runtime + ((index * 9) << 8) + 1024,
                          72, 30, 48, 48, palettes[0]);
            if (index == 5)
                Func_080072f4(render_context, runtime + (176 << 5),
                              66, 22, 48, 48, palettes[0]);
        }

        /* From frame 28 the EWRAM particles are integrated and drawn. */
        if (frame > 27) {
            for (i = 0; i < PARTICLE_COUNT_080EC100; i++) {
                struct Particle_080ec100 *particle = &particles[i];
                s32 size;
                const u8 *source;

                if (particle->state < 0)
                    continue;

                size = Func_080022fc(i, 3) + 2;
                source = graphics + SPARK_TILES_080EC100[size - 1];
                Func_08007308(render_context, source,
                              (s32)(s16)(particle->x >> 16) - size / 2,
                              (s32)(s16)(particle->y >> 16) - size,
                              size, size * 2);
                Func_080e3908(particle, 62, SPARK_GRAVITY_080EC100[i & 3]);
                particle->state++;
                if (particle->velocity_y > 0 &&
                    (s32)(s16)(particle->y >> 16) > 104)
                    particle->state = -1;
            }
        }

        /* Scrolling ground strip: five 32-pixel tiles. */
        for (i = 0; i < 5; i++) {
            Func_080072f4(render_context, runtime,
                          i * 32 + ((frame / 4) & 31) - 32, 88,
                          32, 32, palettes[1]);
        }

        *(s32 *)(runtime + OFF_PRESENT_080EC100) = 1;
        Func_080030f8(1);

        frame++;
        if (frame == 244)
            break;
        /*
         * After frame 16 the player may abort with the button mask held at
         * 0x03001b04; the first sixteen frames are unskippable.
         */
        if (frame > 16 && (*(u32 *)0x03001b04 & 3) != 0)
            break;
    }

    /* ---------------------------------------------------------------- */
    /* Hand-off between the two sequences.                              */
    /* ---------------------------------------------------------------- */

    *(u16 *)(0x03001ad0 + 4) = (u16)saved_line;
    *(s32 *)((u8 *)display + 16) = 0;
    Func_0800307c(2, 0, (void *)0x080ec0f1);
    Func_080d67dc();
    Func_08004278((void *)0x080ec0e1);
    REG16_080EC100(0x04000040) = 0x00f0;   /* WIN0H back to full width */

    *(s32 *)(runtime + OFF_MODE_080EC100) = 2;
    *(s32 *)(runtime + OFF_SUBMODE_080EC100) = 75;

    for (i = 0; i < PARTICLE_COUNT_080EC100; i++)
        particles[i].state = 0;

    /* Sixteen falling curtains, one per party slot. */
    for (i = 0; i < 16; i++) {
        sprites[i].x = (Func_08004458() & 31) + 32;
        sprites[i].y = 0;
        sprites[i].state = 0;
    }

    {
        void *actors = *(void **)(runtime + OFF_TARGET_080EC100);

        if (*(s32 *)((u8 *)actors + 20) != 0) {
            i = 0;
            do {
                s32 column;

                Func_080e396c(*(s16 *)((u8 *)(*(void **)(runtime +
                                  OFF_TARGET_080EC100)) + 36 + i * 2),
                              &column);
                sprites[i].x = column / 2;
                i++;
            } while (i != *(s32 *)((u8 *)(*(void **)(runtime +
                         OFF_TARGET_080EC100)) + 20));
        }
    }

    Func_080e0524(0x6f, runtime, 1, 1);
    Func_080f9010(0x121);
    REG16_080EC100(0x04000020) = 0x0080;   /* BG2PA    */
    REG16_080EC100(0x04000052) = 0x1010;   /* BLDALPHA */

    /* ---------------------------------------------------------------- */
    /* Phase 2: 144 frames of falling curtains and rising sparks.       */
    /* ---------------------------------------------------------------- */

    frame = 0;
    for (;;) {
        s32 band;

        if (frame == 96)
            Func_080b50e8(134);

        for (band = 0; band < 5; band++) {
            struct Particle_080ec100 *curtain = &sprites[band];
            s32 start = band * 16;

            if (frame == start + 7)
                Func_080f9010(154);

            if (frame < start || curtain->state > 31)
                continue;

            {
                s32 split = Func_080022fc(frame * 16, 104);

                Func_080072f4(render_context, runtime, curtain->x - 8,
                              curtain->y + split - 216, 17, 104, palettes[1]);
                Func_080072f4(render_context, runtime, curtain->x - 8,
                              curtain->y + split - 112, 17, 104 - split,
                              palettes[1]);
                Func_080072f4(render_context, runtime + 1768,
                              curtain->x - 17, curtain->y - 65, 34, 65,
                              palettes[1]);
            }

            if (curtain->y <= 111)
                curtain->y += 16;
            else
                curtain->state++;

            if (curtain->y > 111) {
                s32 spawned = 0;
                s32 column = curtain->x;

                if (curtain->state <= 7)
                    *(s32 *)(runtime + OFF_SHAKE_080EC100) = 4;

                for (i = 0; i < PARTICLE_COUNT_080EC100; i++) {
                    struct Particle_080ec100 *particle = &particles[i];
                    s32 speed;
                    s32 angle;

                    if (particle->state != 0)
                        continue;

                    speed = (Func_08004458() & 0x3ff) + 32;
                    angle = (Func_08004458() & 0x7fff) + (s32)0xffffc000;
                    particle->x = column << 16;
                    particle->y = 112 << 16;
                    particle->velocity_x =
                        (speed * Func_08002322(angle)) >> 7;
                    particle->velocity_y =
                        (-((speed * Func_0800231c(angle)) << 1)) >> 7;
                    particle->state = (Func_08004458() & 7) + 32;

                    spawned++;
                    if (spawned == 4)
                        break;
                }
            }
        }

        /* Stagger the per-actor reaction cues sixteen frames apart. */
        {
            void *actors = *(void **)(runtime + OFF_TARGET_080EC100);

            if (*(s32 *)((u8 *)actors + 20) != 0) {
                i = 0;
                do {
                    if (frame == 8 + i * 16) {
                        s16 actor = *(s16 *)((u8 *)(*(void **)(runtime +
                                        OFF_TARGET_080EC100)) + 36 + i * 2);

                        Func_080d6888(actor, 7, 5, i, 8);
                        Func_080b5088(actor, 2);
                    }
                    i++;
                } while (i != *(s32 *)((u8 *)(*(void **)(runtime +
                             OFF_TARGET_080EC100)) + 20));
            }
        }

        for (i = 0; i < PARTICLE_COUNT_080EC100; i++) {
            struct Particle_080ec100 *particle = &particles[i];
            s32 line;

            if (particle->state <= 0)
                continue;

            particle->state--;
            Func_080e3908(particle, 60, 0x4000);

            line = particle->y >> 16;
            if (line > 120) {
                /* Bounce off the floor with half the remaining speed. */
                particle->velocity_y = (-particle->velocity_y) / 2;
            } else if (particle->x >= 0 && (particle->x >> 16) <= 126 &&
                       particle->y >= 0) {
                s32 size = particle->state / 8 + 3;
                const u8 *source = graphics + SPARK_TILES_080EC100[size - 1];

                Func_080072f4(render_context, source,
                              (particle->x >> 16) - size / 2, line - size,
                              size, size * 2, palettes[i & 1]);
            }
        }

        Func_080e155c(4, 8);
        Func_080cd52c();
        *(s32 *)(runtime + OFF_PRESENT_080EC100) = 1;
        Func_080030f8(1);

        frame++;
        if (frame == 144)
            break;
    }

    /* ---------------------------------------------------------------- */
    /* Tear-down                                                        */
    /* ---------------------------------------------------------------- */

    for (i = 0; i < 8; i++)
        Func_08009038(((void **)(runtime + OFF_OBJECTS_080EC100))[i]);

    Func_08004278((void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
