#include "types.h"

/*
 * Func_080d765c -- the Venus Lighthouse "beam / eruption" cutscene driver.
 *
 * This is ONE owner spread over seven executable ranges of the manifest,
 * because the 148-byte head allocates a 332-byte frame and then falls through
 * direct `b` edges into every continuation row without ever restoring it.  The
 * single epilogue is at 080d829a.  Reconstructed ranges (literal pools and
 * alignment excluded):
 *
 *     080d765c..080d76d0   116   prologue, video/window setup
 *     080d76f0..080d778c   156   scene registration, DISPCNT/BLD setup
 *     080d77b4..080d7862   174   spark seeding, slot table, phase-1 loop head
 *     080d787c..080d7bd2   854   phase-1 body
 *     080d7c08..080d7e5c   596   phase-1 sprays + phase-2 seeding
 *     080d7ea0..080d821a   890   phase-2 body
 *     080d8258..080d82a8    80   phase-2 tail and the sole epilogue
 *                        = 2866 executable bytes
 *
 * Excluded literal pools: 080d76d0(32) 080d778c(40) 080d7862(26) 080d7bd2(54)
 * 080d7e5c(68) 080d821a(62) 080d82a8(8).  Every pc-relative `ldr` in the owner
 * was resolved and lands inside exactly these seven pools; no branch anywhere
 * in the owner targets an address inside them.  The rows the sizing tool
 * flagged `POOL?` (080d7bd2, 080d821a) are confirmed data, and so is the
 * un-flagged 68-byte row 080d7e5c, which decodes to plausible `lsrs`/`movs`
 * pairs but is entirely 0x080ee9xx / 0x080edexx pointers and is loaded from
 * by `ldr r2,[pc,#60]` at 080d7e1e.
 *
 * Call accounting -- 91 `bl` sites, all placed:
 *   81 ordinary calls, of which 34 are the RNG Func_08004458, 9 Func_080f9010,
 *   7 Func_080022fc, 3 Func_080030f8, 2 each of Func_080ed408 / Func_080e155c /
 *   Func_080e0524 / Func_08004278 / Func_080041d8 / Func_08002dd8, and one each
 *   of Func_08002304, Func_080049ac, Func_08009008, Func_08009020,
 *   Func_08009038, Func_080b5040, Func_080b50e8, Func_080c9048, Func_080cd104,
 *   Func_080cd594, Func_080cdbc0, Func_080d6750, Func_080d67dc, Func_080d6888,
 *   Func_080dbb24, Func_080e3908.
 *   10 thunk-bank indirect calls: 7x `bl Func_080072f4` (call via r4),
 *   1x `bl Func_080072f8` (via r5), 2x `bl Func_08007300` (via r7).  The via
 *   register is loaded from the frame slot at sp+36 or sp+32 immediately
 *   before every one of them, and those slots are filled once during setup
 *   from *(0x03001f08) and *(0x03001f0c).  They are therefore two blitter
 *   function pointers published by the scene, called with the established
 *   six-argument renderer ABI (r0-r3 plus two stack words), and are modelled
 *   here as function pointers -- NOT as routines named Func_080072f4.
 *   0 internal control edges are emitted as `bl`; every intra-owner edge in
 *   this owner is a `b`/`b<cc>`.
 *
 * Uncertainties are recorded inline with the tag "uncertain:".
 */

/* uncertain: the two trailing words are most likely a source rectangle's
   width/height; the order below matches the argument slots, not proven names. */
typedef void (*Blit6_080d765c)(void *target, const void *source, s32 x, s32 y,
                               s32 width, s32 height);

/* 28-byte particle record.  Three distinct pools of these are used:
   0x02010000 (64 or 128 live entries depending on the phase), 0x02010380 and
   0x02010e00.  Phase 1 keeps 16.16 fixed-point positions in x/y; phase 2's
   second pool at 0x02010000 keeps plain integer positions. */
struct Particle_080d765c {
    s32 x;          /* +0  */
    s32 y;          /* +4  */
    s32 field_08;   /* +8  */
    s32 vx;         /* +12 */
    s32 vy;         /* +16 */
    s32 field_20;   /* +20 */
    s32 timer;      /* +24 */
};

/* The 8-byte records built at sp+76 from the byte table at 0x080ee974. */
struct SpriteSlot_080d765c {
    u32 x;
    u32 y;
};

/* The object handed to Func_080d765c and parked at scene+0x7828. */
struct Owner_080d765c {
    u8 unknown_00[20];
    s32 entry_count;        /* +20 */
    u8 unknown_18[12];
    s16 entry_ids[1];       /* +36, entry_ids[i] at +36+2*i */
};

/*
 * The globals at 0x03001eec..0x03001f0c, read once through r5 = 0x03001ef0.
 * Only the five words the owner touches are named.
 */
#define SCENE_BASE_080D765C   (*(u8 **)0x03001eec)
#define SURFACE_A_080D765C    (*(void **)0x03001ef0)
#define SURFACE_B_080D765C    (*(u8 **)0x03001ef4)
#define BLIT_A_080D765C       (*(Blit6_080d765c *)0x03001f08)
#define BLIT_B_080D765C       (*(Blit6_080d765c *)0x03001f0c)

#define REG16_080D765C(address) (*(volatile u16 *)(address))
#define MEM32_080D765C(address) (*(volatile u32 *)(address))

/* Mirror of the 332-byte stack frame the head allocates. */
struct Stage_080d765c {
    u8 *scene;                              /* sp+44 */
    void *surface_a;                        /* sp+48 */
    u8 *assets_b;                           /* sp+28, second graphics blob */
    Blit6_080d765c blit_a;                  /* sp+36, from 0x03001f08 */
    Blit6_080d765c blit_b;                  /* sp+32, from 0x03001f0c */
    s32 frame;                              /* sp+40 */
    s32 palette_step;                       /* sp+24 */
    u32 matrix_a[2];                        /* sp+52 */
    u32 matrix_b[4];                        /* sp+60 */
    struct SpriteSlot_080d765c slots[32];   /* sp+76 .. sp+331 */
};

void Func_080041d8(void *entry, u32 size);
void Func_08002dd8(s32 resource);
s32 Func_080022fc(s32 value, s32 modulus);
s32 Func_08002304(s32 value, s32 modulus);
void Func_080030f8(u32 frames);
void Func_08004278(void *entry);
s32 Func_08004458(void);
void Func_080049ac(void);
void Func_08009008(void *object, const u32 *placement, const u32 *scale,
                   s32 flags);
void Func_08009020(void *object, s32 variant);
void Func_08009038(void *object);
void Func_080b5040(s32 channel, s32 track, s32 flags);
void Func_080b50e8(s32 sound);
void Func_080c9048(void);
void Func_080cd104(s32 mode, s32 flags);
void Func_080cd594(s32 mode);
void Func_080cdbc0(void);
void Func_080d6750(struct Owner_080d765c *owner);
void Func_080d67dc(void);
void Func_080d6888(s32 entry, s32 a, s32 b, s32 c, s32 d);
void Func_080dbb24(s32 a, s32 b, s32 c);
void Func_080e0524(s32 id, void *surface, s32 a, s32 b);
void Func_080e155c(s32 a, s32 b);
void Func_080e3908(struct Particle_080d765c *particle, s32 a, s32 b);
void Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080f9010(s32 effect);

/* ------------------------------------------------------------------ */
/* Read-only tables.  All of these are ROM addresses reached only through
   pc-relative loads out of this owner's literal pools. */

#define SEQ_080D765C      ((const u8 *)0x080ee974)   /* interleaved x,y pairs */
#define GRID_TILT_080D765C ((const s8 *)0x080ee994)  /* 4 entries, frames 176..179 */

/* Phase-1 spark artwork: 3 variants. */
#define P1_SRC_080D765C   ((const u16 *)0x080ee998)
#define P1_H_080D765C     ((const u8 *)0x080ee99e)
#define P1_W_080D765C     ((const u8 *)0x080ee9a1)
#define P1_VARIANT_080D765C ((const u8 *)0x080ee9a4) /* 4 entries */

/* Phase-1 trail artwork: up to 11 variants. */
#define TRAIL_SRC_080D765C ((const u16 *)0x080ee9be)
#define TRAIL_W_080D765C   ((const u8 *)0x080ee9a8)
#define TRAIL_H_080D765C   ((const u8 *)0x080ee9b3)

/* Phase-1 rain artwork: 3 variants. */
#define RAIN_SRC_080D765C ((const u16 *)0x080ee9da)
#define RAIN_W_080D765C   ((const u8 *)0x080ee9d4)
#define RAIN_H_080D765C   ((const u8 *)0x080ee9d7)

/* Spray artwork: 9 animation frames, shared by both phases. */
#define SPRAY_SRC_080D765C ((const u16 *)0x080ede84)
#define SPRAY_SZ_080D765C  ((const u8 *)0x080ede96)

/* Phase-2 spark artwork: 3 variants. */
#define P2_SRC_080D765C   ((const u16 *)0x080ee9e0)
#define P2_H_080D765C     ((const u8 *)0x080ee9e6)
#define P2_W_080D765C     ((const u8 *)0x080ee9e9)

/* Phase-2 fall artwork: 3 variants. */
#define FALL_SRC_080D765C ((const u16 *)0x080ee9f2)
#define FALL_H_080D765C   ((const u8 *)0x080ee9ec)
#define FALL_W_080D765C   ((const u8 *)0x080ee9ef)

/* Particle pools. */
#define POOL_A_080D765C ((struct Particle_080d765c *)0x02010000)
#define POOL_B_080D765C ((struct Particle_080d765c *)0x02010380)
#define POOL_C_080D765C ((struct Particle_080d765c *)0x02010e00)

/* Scene-relative offsets.  The odd ones are byte offsets into a packed
   graphics blob, which is why the scene pointer is kept as u8 *. */
#define SCENE_BEAM_080D765C     0x0c46
#define SCENE_PILLAR_080D765C   0x14f9
#define SCENE_MODE_080D765C     0x7780
#define SCENE_TIMER_080D765C    0x7784
#define SCENE_FLAG_A8_080D765C  0x77a8
#define SCENE_OBJECTS_080D765C  0x77d8   /* 9 object pointers */
#define SCENE_TARGET_080D765C   0x77e0
#define SCENE_PRESENT_080D765C  0x7824
#define SCENE_OWNER_080D765C    0x7828
#define SCENE_SPARKS_080D765C   0x7080   /* 225 << 7 */

#define SCENE_U32_080D765C(scene, off) (*(u32 *)((scene) + (off)))
#define SCENE_PTR_080D765C(scene, off) (*(void **)((scene) + (off)))

/* ------------------------------------------------------------------ */

/* 080d77b4: seed 64 sparks with a scattered start and a disarmed timer. */
static void SeedSparks_080d765c(void)
{
    struct Particle_080d765c *particle = POOL_A_080D765C;
    s32 i;

    for (i = 0; i < 64; i++) {
        particle->x = (Func_08004458() & 63) + 32;
        particle->y = (Func_08004458() & 31) + 120;
        particle->timer = -1;
        particle++;
    }
}

/* 080d77e6: expand the packed byte pairs at 0x080ee974 into the frame. */
static void LoadSlots_080d765c(struct Stage_080d765c *stage)
{
    const u8 *source = SEQ_080D765C;
    s32 i;

    for (i = 0; i < 32; i++) {
        stage->slots[i].x = source[0];
        stage->slots[i].y = source[1];
        source += 2;
    }
}

/* 080d78fe: the spark row drawn each phase-1 frame, tilted by the grid shake. */
static void DrawSparkRow_080d765c(struct Stage_080d765c *stage, s32 count,
                                  s32 tilt_x, s32 tilt_y)
{
    const u8 *seq = SEQ_080D765C;
    s32 i;

    for (i = 0; i < count; i++) {
        s32 variant = Func_080022fc(i, 3);
        s32 width = P1_W_080D765C[variant];

        stage->blit_a(stage->surface_a,
                      stage->scene + P1_SRC_080D765C[variant],
                      seq[2 * i] - tilt_x,
                      seq[2 * i + 1] - width - tilt_y,
                      P1_H_080D765C[variant],
                      width);
    }
}

/* 080d7990..080d7a0c: place the nine lighthouse objects as a 3x3 grid. */
static void PlaceObjectGrid_080d765c(struct Stage_080d765c *stage, s32 tilt_x,
                                     s32 tilt_y)
{
    s32 depth;
    s32 column;

    stage->matrix_a[0] = 0x00010000;
    stage->matrix_a[1] = 0x00010000;
    stage->matrix_b[3] = 0;
    stage->matrix_b[1] = 0x00ff0000;

    /*
     * Note: 080d793a loads *(0x080eda80) and *(0x080eda84) into matrix_a here
     * every frame, but both words are overwritten by the two stores above
     * before any use.  The loads are dead and are not reproduced.
     */
    depth = 0x004c0000 - (tilt_y << 16);

    for (column = 0; column < 3; column++) {
        void **objects =
            (void **)(stage->scene + SCENE_OBJECTS_080D765C + column * 12);
        s32 height = 0x00900000 - (tilt_x << 16);
        s32 row;

        for (row = 0; row < 3; row++) {
            stage->matrix_b[0] = (u32)height;
            stage->matrix_b[2] = (u32)depth;
            Func_08009008(*objects++, stage->matrix_b, stage->matrix_a, 0);
            height += 0x00200000;
        }

        depth += 0x00200000;
    }
}

/* 080d7aa4: the frame-32 burst. */
static void SeedBurstAt32_080d765c(void)
{
    struct Particle_080d765c *particle = POOL_A_080D765C;
    s32 i;

    for (i = 0; i < 32; i++) {
        particle->x = ((Func_08004458() & 31) + 68) << 16;
        particle->y = ((Func_08004458() & 31) + 8) << 16;
        particle->vx = ((Func_08004458() & 127) - 63) << 11;
        particle->vy = (((-Func_08004458()) & 127) - 64) << 11;
        particle->timer = (Func_08004458() & 15) + 32;
        particle++;
    }
}

/* 080d7b00: the frame-64 burst, in the second pool. */
static void SeedBurstAt64_080d765c(void)
{
    struct Particle_080d765c *particle = POOL_B_080D765C;
    s32 i;

    for (i = 0; i < 32; i++) {
        /* uncertain: Func_08002304 sits eight bytes after Func_080022fc and is
           used the same way; both are read here as a remainder helper. */
        particle->x = (Func_08002304(Func_08004458(), 48) + 60) << 16;
        particle->y = ((Func_08004458() & 31) + 52) << 16;
        particle->vx = ((Func_08004458() & 127) - 63) << 12;
        particle->vy = (((-Func_08004458()) & 31) - 32) << 13;
        particle->timer = (Func_08004458() & 15) + 32;
        particle++;
    }
}

/* 080d7b5e: the frame-104 burst. */
static void SeedBurstAt104_080d765c(void)
{
    struct Particle_080d765c *particle = POOL_A_080D765C;
    s32 i;

    for (i = 0; i < 32; i++) {
        particle->x = ((Func_08004458() & 63) + 52) << 16;
        particle->y = ((Func_08004458() & 31) + 72) << 16;
        particle->vx = ((Func_08004458() & 127) - 63) << 11;
        particle->vy = (((-Func_08004458()) & 31) - 32) << 13;
        particle->timer = (Func_08004458() & 15) + 32;
        particle++;
    }
}

/* 080d7bba..080d7c58: draw and advance the 64 armed sparks. */
static void StepSparks_080d765c(struct Stage_080d765c *stage)
{
    struct Particle_080d765c *particle = POOL_A_080D765C;
    s32 i;

    for (i = 0; i < 64; i++) {
        if (particle->timer >= 0) {
            s32 variant;

            if (stage->frame > 191) {
                variant = Func_080022fc(i, 7) + 4;
            } else {
                variant = i & 3;
            }

            stage->blit_b(stage->surface_a,
                          stage->scene + TRAIL_SRC_080D765C[variant],
                          (s32)(s16)(particle->x >> 16),
                          (s32)(s16)(particle->y >> 16),
                          TRAIL_W_080D765C[variant],
                          TRAIL_H_080D765C[variant]);

            particle->x += particle->vx;
            particle->y += particle->vy;
            particle->vy += 128 << 6;
        }

        particle++;
    }
}

/* 080d7c66: the frame-224 downpour, 128 entries. */
static void SeedRain_080d765c(void)
{
    struct Particle_080d765c *particle = POOL_A_080D765C;
    s32 i;

    for (i = 0; i < 128; i++) {
        particle->x = 144 << 15;
        particle->y = 224 << 14;
        particle->vx = ((-(Func_08004458() & 127)) - 64) << 11;
        particle->vy = ((Func_08004458() & 127) + 16) << 11;
        particle->timer = Func_08004458();
        particle++;
    }
}

/* 080d7caa: draw and advance the downpour, staggering entries into life. */
static void StepRain_080d765c(struct Stage_080d765c *stage)
{
    struct Particle_080d765c *particle = POOL_A_080D765C;
    s32 i;

    for (i = 0; i < 128; i++) {
        if (stage->frame >= (i / 4) + 224) {
            s32 variant = Func_080022fc(i, 3);

            if ((i & 1) == 0) {
                stage->blit_a(stage->surface_a,
                              stage->scene + RAIN_SRC_080D765C[variant],
                              (s32)(s16)(particle->x >> 16),
                              (s32)(s16)(particle->y >> 16),
                              RAIN_W_080D765C[variant],
                              RAIN_H_080D765C[variant]);
            }

            particle->x += particle->vx;
            particle->y += particle->vy;

            if ((particle->x >> 16) < -16 || (particle->y >> 16) > 120) {
                particle->x = 144 << 15;
                particle->y = 224 << 14;
            }

            particle->timer++;
        }

        particle++;
    }
}

/* 080d7d44: snapshot the downpour into the spray pool at frame 228. */
static void SnapshotSpray_080d765c(void)
{
    struct Particle_080d765c *source = POOL_A_080D765C;
    struct Particle_080d765c *target = POOL_C_080D765C;
    s32 i;

    for (i = 0; i < 128; i++) {
        target->x = source->x;
        target->y = source->y;
        target->timer = 0;
        source++;
        target++;
    }
}

/* 080d7d64: the nine-frame splash animation, restarted from the live rain. */
static void StepSpray_080d765c(struct Stage_080d765c *stage)
{
    struct Particle_080d765c *spray = POOL_C_080D765C;
    struct Particle_080d765c *rain = POOL_A_080D765C;
    s32 i;

    for (i = 0; i < 128; i++) {
        if (stage->frame >= i + 228) {
            s32 step = Func_080022fc(spray->timer / 2, 9);
            s32 size = SPRAY_SZ_080D765C[step];
            s32 half = (s32)((u32)size >> 1);

            stage->blit_a(stage->surface_a,
                          stage->assets_b + SPRAY_SRC_080D765C[step],
                          (s32)(s16)(spray->x >> 16) - half,
                          (s32)(s16)(spray->y >> 16) - half,
                          size, size);

            spray->timer++;
            if (spray->timer == 18) {
                spray->x = rain->x;
                spray->y = rain->y;
                spray->timer = 0;
            }
        }

        spray++;
        rain++;
    }
}

/* ------------------------------------------------------------------ */

/* 080d7814..080d7dfc: one frame of the eruption. */
static void RunPhase1Frame_080d765c(struct Stage_080d765c *stage)
{
    s32 frame = stage->frame;
    s32 spark_count = 0;
    s32 tilt_x;
    s32 tilt_y;
    s32 pillar;
    volatile s16 *shake = (volatile s16 *)0x03001ad0;

    if ((u32)frame <= 15) {
        if (frame == 1) {
            /* Re-purpose the head of the particle pool as a 128-byte noise
               buffer for the dissolve, then hand the routine at 0x080d66cd
               1152 bytes of workspace. */
            u8 *noise = (u8 *)0x02010002;
            s32 i;

            for (i = 0; i < 128; i++) {
                noise[i] = (u8)(Func_08004458() & 63);
            }

            REG16_080D765C(0x02010000) = 0;
            Func_080041d8((void *)0x080d66cd, 144 << 3);
        }

        REG16_080D765C(0x02010000) =
            (u16)(REG16_080D765C(0x02010000) + stage->palette_step);
        stage->palette_step += 3;

        if (frame == 15) {
            Func_08004278((void *)0x080d66cd);
        }
    }

    if (frame > 103) {
        spark_count = 0;
    } else if (frame > 63) {
        spark_count = 6;
    } else if (frame > 31) {
        spark_count = 10;
    }

    if (frame > 167) {
        tilt_x = 0;
        tilt_y = 0;
        shake[2] = 0;
        shake[3] = 32;
    } else {
        tilt_x = (Func_08004458() & 3) - 1;
        tilt_y = (Func_08004458() & 3) - 1;
        shake[2] = (s16)tilt_x;
        shake[3] = (s16)(tilt_y + 32);
    }

    if ((u32)(frame - 176) <= 3) {
        s32 offset = GRID_TILT_080D765C[frame - 176];

        tilt_x = -offset;
        tilt_y = offset;
        shake[2] = (s16)tilt_x;
        shake[3] = (s16)(offset + 32);
    }

    if (spark_count != 0) {
        DrawSparkRow_080d765c(stage, spark_count, tilt_x, tilt_y);
    }

    if (frame == 174) {
        void **objects =
            (void **)(stage->scene + SCENE_OBJECTS_080D765C);
        s32 i;

        for (i = 0; i < 9; i++) {
            u8 *object = (u8 *)*objects++;

            object[9] = (u8)(object[9] & ~12);
        }
    }

    if (frame > 208) {
        Func_08009020(SCENE_PTR_080D765C(stage->scene, SCENE_TARGET_080D765C),
                      P1_VARIANT_080D765C[(frame / 4) & 3]);
    }

    PlaceObjectGrid_080d765c(stage, tilt_x, tilt_y);

    if ((u32)(frame - 160) <= 157) {
        s32 width = 80;
        s32 height = 8;

        if (frame <= 175) {
            width = 96 - (frame - 160);
            height = ((frame - 160) << 2) - 56;
        } else if (frame > 208) {
            s32 span = frame - 208;

            if (span < 0) {
                span = frame - 205;
            }
            height = (span >> 2) + 8;
        }

        stage->blit_b(stage->surface_a,
                      stage->scene + SCENE_BEAM_080D765C,
                      width, height, 24, 48);
    }

    if (frame == 32 || frame == 64 || frame == 104 || frame == 176) {
        Func_080f9010(134);
    }
    if (frame == 226) {
        Func_080f9010(145);
    }

    Func_080049ac();

    if (frame == 32) {
        SeedBurstAt32_080d765c();
    }
    if (frame == 64) {
        SeedBurstAt64_080d765c();
    }
    if (frame == 104) {
        SeedBurstAt104_080d765c();
    }

    if ((u32)(frame - 32) <= 175) {
        StepSparks_080d765c(stage);
    }

    if (frame > 223) {
        if (frame == 224) {
            SeedRain_080d765c();
        }

        StepRain_080d765c(stage);

        if (frame == 228) {
            SnapshotSpray_080d765c();
        }

        StepSpray_080d765c(stage);
    }

    pillar = 1;
    SCENE_U32_080D765C(stage->scene, SCENE_PRESENT_080D765C) = (u32)pillar;
    Func_080030f8(1);
}

/* 080d7e28..080d7ec0: seed both phase-2 pools. */
static void SeedPhase2_080d765c(struct Stage_080d765c *stage)
{
    struct Particle_080d765c *particle =
        (struct Particle_080d765c *)(stage->scene + SCENE_SPARKS_080D765C);
    struct Particle_080d765c *droplet = POOL_A_080D765C;
    s32 i;

    for (i = 0; i < 64; i++) {
        particle->field_08 = (s32)0xfff00000;
        particle->x = ((Func_08004458() & 127) + 64) << 16;
        particle->y = (((-(Func_08004458() & 127))) - 64) << 16;
        particle->vx = ((-(Func_08004458() & 63)) - 127) << 12;
        particle->vy = ((Func_08004458() & 63) + 127) << 12;
        particle->timer = 0;
        particle++;
    }

    for (i = 0; i < 64; i++) {
        droplet->x = Func_08004458() & 127;
        droplet->y = (Func_08004458() & 63) + i / 2;
        droplet->timer = (-i) / 2;
        droplet++;
    }
}

/* 080d7fa2: the phase-2 spark row. */
static void DrawPhase2SparkRow_080d765c(struct Stage_080d765c *stage,
                                        s32 count)
{
    const u8 *seq = SEQ_080D765C;
    s32 i;

    for (i = 0; i < count; i++) {
        s32 variant = Func_080022fc(i, 3);
        s32 width = P2_W_080D765C[variant];

        stage->blit_a(stage->surface_a,
                      stage->scene + P2_SRC_080D765C[variant],
                      seq[2 * i] - 56,
                      seq[2 * i + 1] - width,
                      P2_H_080D765C[variant],
                      width);
    }
}

/* 080d7fec: at frame 72 the sparks snap back onto the packed layout. */
static void ReseedPhase2Sparks_080d765c(struct Stage_080d765c *stage)
{
    struct Particle_080d765c *particle =
        (struct Particle_080d765c *)(stage->scene + SCENE_SPARKS_080D765C);
    const u8 *seq = SEQ_080D765C;
    s32 i;

    for (i = 0; i < 64; i++) {
        s32 slot = (i & 15) * 2;

        particle->x = (seq[slot] - 56) << 16;
        particle->y = seq[slot + 1] << 16;
        particle->vx = ((Func_08004458() & 127) - 63) << 13;
        particle->vy = ((-(Func_08004458() & 31)) - 16) << 14;
        particle++;
    }
}

/* 080d8062: draw the falling sparks and bounce them off the water line. */
static void StepFallingSparks_080d765c(struct Stage_080d765c *stage, s32 count)
{
    struct Particle_080d765c *particle =
        (struct Particle_080d765c *)(stage->scene + SCENE_SPARKS_080D765C);
    s32 i;

    for (i = 0; i < count; i++) {
        s32 y = (s32)(s16)(particle->y >> 16);

        if (y <= 135) {
            s32 variant = Func_080022fc(i, 3);
            s32 width = FALL_W_080D765C[variant];

            stage->blit_a(stage->surface_a,
                          stage->scene + FALL_SRC_080D765C[variant],
                          (s32)(s16)(particle->x >> 16),
                          y - width,
                          FALL_H_080D765C[variant],
                          width);

            Func_080e3908(particle, 64, 128 << 9);

            if ((s32)(s16)(particle->y >> 16) > 120 &&
                particle->vy > (128 << 12)) {
                particle->vy = (-particle->vy) / 4;
                particle->y = 240 << 15;
                SCENE_U32_080D765C(stage->scene, SCENE_FLAG_A8_080D765C) = 1;
            }
        }

        particle++;
    }
}

/* 080d80ec: before frame 72 the sparks are drawn through the second blitter. */
static void DrawRisingSparks_080d765c(struct Stage_080d765c *stage)
{
    struct Particle_080d765c *particle =
        (struct Particle_080d765c *)(stage->scene + SCENE_SPARKS_080D765C);
    s32 i;

    for (i = 0; i < 64; i++) {
        stage->blit_a(stage->surface_a, stage->scene,
                      (s32)(s16)(particle->x >> 16) - 12,
                      (s32)(s16)(particle->y >> 16) - 24,
                      24, 48);

        particle->x += particle->vx;
        particle->y += particle->vy;

        if ((particle->y >> 16) > 120 && stage->frame <= 47) {
            particle->x = ((Func_08004458() & 127) + 64) << 16;
            particle->y = (s32)0xfff00000;
        }

        particle++;
    }
}

/* 080d8144: fire the scripted cues carried by the owner object. */
static void RunOwnerCues_080d765c(struct Stage_080d765c *stage)
{
    struct Owner_080d765c *owner =
        (struct Owner_080d765c *)SCENE_PTR_080D765C(stage->scene,
                                                    SCENE_OWNER_080D765C);
    s32 i = 0;

    if (owner->entry_count == 0) {
        return;
    }

    do {
        if (stage->frame == 32 + i * 8) {
            Func_080d6888(owner->entry_ids[i], 9, 5, -1, 0);
        }
        i++;
        owner = (struct Owner_080d765c *)SCENE_PTR_080D765C(
            stage->scene, SCENE_OWNER_080D765C);
    } while (i != owner->entry_count);
}

/* 080d8196: the phase-2 splash animation over the 64 droplets. */
static void StepPhase2Spray_080d765c(struct Stage_080d765c *stage)
{
    struct Particle_080d765c *droplet = POOL_A_080D765C;
    s32 spread = stage->frame - 54;
    s32 i;

    for (i = 0; i < 64; i++) {
        s32 timer = droplet->timer;

        if ((u32)timer <= 17) {
            s32 step = Func_080022fc(timer / 2, 9);
            s32 size = SPRAY_SZ_080D765C[step];
            s32 half = (s32)((u32)size >> 1);

            /* Phase 2's droplet pool keeps plain integer coordinates, so no
               16.16 shift is applied here -- unlike StepSpray_080d765c. */
            stage->blit_a(stage->surface_a,
                          stage->assets_b + SPRAY_SRC_080D765C[step],
                          droplet->x - half,
                          droplet->y - half,
                          size, size);
            timer = droplet->timer;
        }

        timer++;
        droplet->timer = timer;

        if (timer == 18 && stage->frame <= 127) {
            droplet->x = Func_08004458() & 127;
            droplet->y = (Func_08004458() & 63) + spread / 2;
            droplet->timer = 0;
        }

        droplet++;
    }
}

/* 080d7f08..080d8296: one frame of the aftermath. */
static void RunPhase2Frame_080d765c(struct Stage_080d765c *stage, s32 sweep)
{
    s32 frame = stage->frame;
    s32 spark_count = 0;

    if (frame == 96) {
        Func_080b50e8(134);
    }

    if (frame == 16) {
        SCENE_U32_080D765C(stage->scene, SCENE_FLAG_A8_080D765C) = 32;
    }

    if (frame > 16) {
        spark_count = (frame - 16) / 2;
        if (spark_count > 16) {
            spark_count = 16;
        }
    }

    if ((u32)(frame - 9) <= 62 && (frame & 3) == 0) {
        Func_080f9010(132);
    }

    if (frame == 72) {
        Func_080f9010(145);
    }

    if (frame > 64) {
        stage->blit_a(stage->surface_a,
                      stage->scene + SCENE_PILLAR_080D765C,
                      88 - (frame - 64) * 7, sweep, 40, 80);
    }

    if (frame <= 71 && spark_count != 0) {
        DrawPhase2SparkRow_080d765c(stage, spark_count);
    }

    if (frame == 72) {
        ReseedPhase2Sparks_080d765c(stage);
        SCENE_U32_080D765C(stage->scene, SCENE_FLAG_A8_080D765C) = 4;
    }

    if (frame > 71) {
        StepFallingSparks_080d765c(stage, frame == 72 ? 32 : 64);
    } else {
        DrawRisingSparks_080d765c(stage);
    }

    RunOwnerCues_080d765c(stage);

    if (frame > 72) {
        StepPhase2Spray_080d765c(stage);
    }

    if ((u32)(frame - 72) <= 7) {
        Func_080e155c(8, 8);
    } else {
        Func_080e155c(2, 2);
    }

    SCENE_U32_080D765C(stage->scene, SCENE_PRESENT_080D765C) = 1;
    Func_080030f8(1);
}

void Func_080d765c(struct Owner_080d765c *owner)
{
    struct Stage_080d765c stage;
    s32 sweep;

    stage.surface_a = SURFACE_A_080D765C;
    stage.scene = SCENE_BASE_080D765C;
    stage.assets_b = SURFACE_B_080D765C;
    stage.palette_step = 1;

    SCENE_PTR_080D765C(stage.scene, SCENE_OWNER_080D765C) = owner;

    Func_080cd594(0);
    Func_080c9048();

    REG16_080D765C(0x05000000) = 0;
    REG16_080D765C(0x05000002) = 0;

    SCENE_U32_080D765C(stage.scene, SCENE_MODE_080D765C) = 0;
    Func_080041d8((void *)0x080cd261, 144 << 3);

    REG16_080D765C(0x04000048) = 0x2137;    /* WININ  */
    Func_080cd104(1, 0);
    REG16_080D765C(0x04000040) = 0xf0f0;    /* WIN0H  */

    /* -- 080d76f0 -------------------------------------------------- */

    Func_080e0524(0xb9, stage.scene, 1, 1);
    Func_080e0524(0xba, stage.assets_b, 0, 0);

    Func_080ed408(46, 7, 7, 3, 2);
    stage.blit_a = BLIT_A_080D765C;
    Func_080ed408(47, 7, 7, 3, stage.palette_step);
    stage.blit_b = BLIT_B_080D765C;

    MEM32_080D765C(0x03001ce0 + 16) = 240;

    Func_080d6750((struct Owner_080d765c *)SCENE_PTR_080D765C(
        stage.scene, SCENE_OWNER_080D765C));
    Func_080030f8(1);
    Func_080b5040(1, 0x3b, 0);
    Func_080dbb24(9, 186 << 1, 1);

    REG16_080D765C(0x04000000) = 0x7741;    /* DISPCNT  */
    REG16_080D765C(0x04000020) = 0x0080;    /* BG2PA    */
    REG16_080D765C(0x04000052) = 0x1010;    /* BLDALPHA */
    REG16_080D765C(0x04000050) = 0x3f44;    /* BLDCNT   */

    SCENE_U32_080D765C(stage.scene, SCENE_MODE_080D765C) = 2;
    SCENE_U32_080D765C(stage.scene, SCENE_TIMER_080D765C) = 50;

    /* -- 080d77b4 -------------------------------------------------- */

    SeedSparks_080d765c();
    LoadSlots_080d765c(&stage);
    Func_080f9010(141);

    for (stage.frame = 0; stage.frame < 288; stage.frame++) {
        RunPhase1Frame_080d765c(&stage);

        /* 080d7de0: after the sixteen-frame fade-in the scene may be skipped. */
        if ((MEM32_080D765C(0x03001b04) & 3) != 0 && stage.frame + 1 > 16) {
            stage.frame++;
            break;
        }
    }

    /* -- 080d7dfe: tear the grid down and switch to the aftermath -- */
    {
        void **objects = (void **)(stage.scene + SCENE_OBJECTS_080D765C);
        s32 i;

        for (i = 0; i < 9; i++) {
            Func_08009038(*objects++);
        }
    }

    Func_080d67dc();
    REG16_080D765C(0x04000052) = 0x1010;
    Func_080f9010(0x121);

    SeedPhase2_080d765c(&stage);

    sweep = -992;
    for (stage.frame = 0; stage.frame < 146; stage.frame++) {
        RunPhase2Frame_080d765c(&stage, sweep);
        sweep += 14;
    }

    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_08004278((void *)0x080cd261);
    Func_080cdbc0();
}
