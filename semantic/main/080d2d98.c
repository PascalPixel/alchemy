#include "layout_guard.h"
#include "types.h"

#define REG_BLDALPHA_080D2D98 (*(volatile u16 *)0x04000052)
#define MAIN_PARTICLES_080D2D98 ((struct Particle_080d2d98 *)0x02010000)
#define HELD_KEYS_080D2D98 (*(volatile s32 *)0x03001b04)

typedef void (*Renderer_080d2d98)(
    s32 target, void *source, s32 x, s32 y, u32 width, s32 height);
typedef void *(*WordCopy_080d2d98)(
    void *destination, const void *source, s32 size);

#define WORD_COPY_080D2D98 ((WordCopy_080d2d98)0x03001388)

struct Particle_080d2d98 {
    s32 x;
    s32 y;
    s32 reserved08;
    s32 velocity_x;
    s32 velocity_y;
    s32 reserved14;
    s32 timer;
};

struct SceneContext_080d2d98 {
    u8 reserved00[0x14];
    s32 actor_count;
    u8 reserved18[0x0c];
    s16 actor_ids[1];
};

struct SceneWork_080d2d98 {
    u8 reserved0000[0x6e4];
    u8 particle_tiles[0x699c];
    struct Particle_080d2d98 local_particles[0x40];
    s32 scene_phase;
    s32 scene_timer;
    u8 reserved7788[0x50];
    void *temporary_objects[0x0c];
    u8 reserved7808[0x1c];
    s32 frame_ready;
    struct SceneContext_080d2d98 *context;
};

struct RuntimeHeader_080d2d98 {
    struct SceneWork_080d2d98 *work;
    s32 render_target;
    u8 *sprite_sheet;
};

struct ActorEffect_080d2d98 {
    u8 reserved00[0x28];
    s32 vertical_impulse;
    u8 reserved2c[0x1c];
    s32 animation;
};

LAYOUT_SIZE_GUARD(
    Particle080d2d98_Size,
    struct Particle_080d2d98,
    0x1c);
LAYOUT_OFFSET_GUARD(
    Particle080d2d98_VelocityX,
    struct Particle_080d2d98,
    velocity_x,
    0x0c);
LAYOUT_OFFSET_GUARD(
    Particle080d2d98_Timer,
    struct Particle_080d2d98,
    timer,
    0x18);
LAYOUT_OFFSET_GUARD(
    SceneContext080d2d98_ActorCount,
    struct SceneContext_080d2d98,
    actor_count,
    0x14);
LAYOUT_OFFSET_GUARD(
    SceneContext080d2d98_ActorIds,
    struct SceneContext_080d2d98,
    actor_ids,
    0x24);
LAYOUT_OFFSET_GUARD(
    SceneWork080d2d98_LocalParticles,
    struct SceneWork_080d2d98,
    local_particles,
    0x7080);
LAYOUT_OFFSET_GUARD(
    SceneWork080d2d98_ScenePhase,
    struct SceneWork_080d2d98,
    scene_phase,
    0x7780);
LAYOUT_OFFSET_GUARD(
    SceneWork080d2d98_TemporaryObjects,
    struct SceneWork_080d2d98,
    temporary_objects,
    0x77d8);
LAYOUT_OFFSET_GUARD(
    SceneWork080d2d98_FrameReady,
    struct SceneWork_080d2d98,
    frame_ready,
    0x7824);
LAYOUT_OFFSET_GUARD(
    SceneWork080d2d98_Context,
    struct SceneWork_080d2d98,
    context,
    0x7828);
LAYOUT_OFFSET_GUARD(
    ActorEffect080d2d98_VerticalImpulse,
    struct ActorEffect_080d2d98,
    vertical_impulse,
    0x28);
LAYOUT_OFFSET_GUARD(
    ActorEffect080d2d98_Animation,
    struct ActorEffect_080d2d98,
    animation,
    0x48);

static const struct RuntimeHeader_080d2d98 *const Runtime_080d2d98 =
    (const struct RuntimeHeader_080d2d98 *)0x03001eec;
static const u16 *const ParticleTiles_080d2d98 = (const u16 *)0x080ede48;
static const u8 *const StripOrigins_080d2d98 = (const u8 *)0x080ee1ac;
static const s32 *const ParticleGravity_080d2d98 = (const s32 *)0x080ee1b4;

void Func_08002dd8(s32 slot);
void *Func_08002f40(s32 resource);
void Func_080030f8(u32 frames);
s32 Func_080022fc(s32 value, s32 modulus);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
u32 Func_08004458(void);
void Func_080041d8(s32 callback, s32 size);
void Func_08004278(void *callback);
void Func_08005340(s32 source, s32 destination);
void Func_08009038(void *object);
struct ActorEffect_080d2d98 **Func_080b5098(s32 actor);
void Func_080b50e8(s32 sound);
void Func_080cd594(s32 enabled);
void Func_080cdbc0(void);
void Func_080cef64(s32 mode, Renderer_080d2d98 renderers[2]);
void Func_080d6750(struct SceneContext_080d2d98 *context);
void Func_080d6888(s16 actor, s32 a, s32 b, s32 index, s32 duration);
void Func_080dbb24(s32 a, s32 b, u32 c);
void Func_080e6d3c(s32 layer, s32 x, s32 y);
void Func_080e6eac(s32 layer, s32 x, s32 y);
void Func_080f9010(s32 sound);

static void SeedParticle_080d2d98(struct Particle_080d2d98 *particle) {
    s32 radius = Func_08004458() & 0x3f;
    u16 angle = Func_08004458();

    particle->x = (radius * Func_08002322(angle) >> 3) + 0x200000;
    particle->y = (radius * Func_0800231c(angle) >> 2) + 0x600000;
    particle->velocity_x =
        ((Func_08004458() & 0x3f) - 0x20) << 0xe;
    particle->velocity_y =
        (-(s32)(Func_08004458() & 0x3f) - 8) << 0xd;
    particle->timer = 0;
}

static s32 ScaleBy64_080d2d98(s32 value, s32 factor) {
    value *= factor;
    if (value < 0) {
        value += 0x3f;
    }
    return value >> 6;
}

/*
 * Run the long particle scene around a moving anchor. Renderer callbacks are
 * returned as a two-entry out-parameter table; their index follows vertical
 * direction, while all particle storage and lifetimes remain ordinary C.
 */
void Func_080d2d98(struct SceneContext_080d2d98 *context) {
    struct SceneWork_080d2d98 *work = Runtime_080d2d98->work;
    s32 render_target = Runtime_080d2d98->render_target;
    u8 *sprite_sheet = Runtime_080d2d98->sprite_sheet;
    Renderer_080d2d98 renderers[2];
    s32 anchor_x = 0x01000000;
    s32 anchor_y = 0x580000;
    s32 velocity_x = -0x100000;
    s32 velocity_y = -0x40000;
    s32 row_offset = 0;
    s32 frame;
    s32 index;

    work->context = context;
    Func_080cd594(0);
    REG_BLDALPHA_080D2D98 = 0x1010;
    Func_080cef64(0, renderers);

    {
        s32 palette = (s32)Func_08002f40(0x6e);

        WORD_COPY_080D2D98((void *)0x05000000, (void *)palette, 0x80);
        Func_08005340(palette + 0x80, (s32)work);
    }
    Func_08005340(
        (s32)Func_08002f40(0x85) + 0x80,
        (s32)&work->particle_tiles[0]);
    Func_08005340((s32)Func_08002f40(0x73), (s32)sprite_sheet);

    work->scene_phase = 2;
    work->scene_timer = 0x4b;
    Func_080041d8(0x080cd261, 0x480);

    for (index = 0; index < 0x40; index++) {
        work->local_particles[index].timer = -1;
    }
    for (index = 0x18; index < 0x28; index++) {
        struct Particle_080d2d98 *particle = &work->local_particles[index];

        particle->x = Func_08004458() & 0x7f;
        particle->y = (Func_08004458() & 7) + 0x38;
        particle->timer = -(s32)(Func_08004458() & 0x0f);
    }
    for (index = 0; index < 0x400; index++) {
        MAIN_PARTICLES_080D2D98[index].timer = -1;
    }

    Func_080d6750(context);
    Func_080030f8(1);
    Func_080dbb24(0xc, 0x17c, 2);

    for (frame = 0; frame != 0x7c; frame++) {
        u32 spawn_window;

        if ((HELD_KEYS_080D2D98 & 3) && frame > 0x20 && frame <= 0x61) {
            frame = 0x62;
        }
        if (frame == 0x78) {
            Func_080b50e8(0x86);
        }
        if (frame <= 0x0f) {
            row_offset += 2;
        }
        if (frame <= 0x63) {
            anchor_x += velocity_x;
            anchor_y += velocity_y;
            velocity_x = ScaleBy64_080d2d98(velocity_x, 0x3a);
            velocity_y = ScaleBy64_080d2d98(velocity_y, 0x38);
            if (anchor_x <= 0x77ffff) {
                velocity_x += 0x8000;
            }
        }
        Func_080e6d3c(1, anchor_x, anchor_y);

        if (frame == 0x1c) {
            for (index = 0; index < 0x100; index++) {
                if (MAIN_PARTICLES_080D2D98[index].timer == -1) {
                    SeedParticle_080d2d98(&MAIN_PARTICLES_080D2D98[index]);
                }
            }
        }

        spawn_window = frame - 0x20;
        if (spawn_window <= 0x2f) {
            s32 spawned = 0;

            for (index = 0; index < 0x400 && spawned != 0x10; index++) {
                if (MAIN_PARTICLES_080D2D98[index].timer == -1) {
                    SeedParticle_080d2d98(&MAIN_PARTICLES_080D2D98[index]);
                    spawned++;
                }
            }
        }

        if (frame == 0) {
            Func_080f9010(0xa4);
        }
        if (frame == 0x20) {
            Func_080f9010(0x91);
        }
        if (frame == 0x50) {
            Func_080f9010(0x90);
        }

        if (spawn_window <= 0x2f) {
            s32 phase = frame * 0x10 - 0x100;

            for (index = 0; index < 3; index++, phase += 0x19) {
                s32 clipped = Func_080022fc(phase, 0x68);
                s32 x = StripOrigins_080d2d98[index * 2] - 0x11;
                s32 y = StripOrigins_080d2d98[index * 2 + 1] - clipped;

                renderers[0](
                    render_target,
                    &work->particle_tiles[0],
                    x,
                    y - 0x68,
                    0x22,
                    0x68);
                renderers[0](
                    render_target,
                    &work->particle_tiles[0],
                    x,
                    y,
                    0x22,
                    clipped);
            }
        }

        if (frame <= 0x5f) {
            for (index = 0; index < 5; index++) {
                renderers[0](
                    render_target,
                    work,
                    index * 0x20 + ((frame / 4) & 0x1f) - 0x20,
                    0x78 - row_offset,
                    0x20,
                    0x20);
            }
        }

        for (index = 0; index < 0x400; index++) {
            struct Particle_080d2d98 *particle = &MAIN_PARTICLES_080D2D98[index];

            if (particle->timer >= 0) {
                s32 size = Func_080022fc(index, 3) + 2;

                if (particle->velocity_y > 0) {
                    size += 2;
                }
                if (frame > 0x44 && size <= 5) size = 6;
                if (frame > 0x46 && size <= 6) size = 7;
                if (frame > 0x48 && size <= 7) size = 8;
                if (frame > 0x4a && size <= 8) size = 9;
                if (frame > 0x4c) size = 0xa;

                renderers[particle->velocity_y > 0](
                    render_target,
                    sprite_sheet + ParticleTiles_080d2d98[size - 1],
                    (s16)(particle->x >> 16) - size / 2,
                    (s16)(particle->y >> 16) - size,
                    size,
                    size * 2);

                particle->x += particle->velocity_x;
                particle->y += particle->velocity_y;
                if (frame > 0x50) {
                    particle->velocity_y -= 0x8000;
                } else {
                    particle->velocity_y += ParticleGravity_080d2d98[index & 3];
                }
                particle->velocity_x =
                    ScaleBy64_080d2d98(particle->velocity_x, 0x3e);
                particle->velocity_y =
                    ScaleBy64_080d2d98(particle->velocity_y, 0x3e);
                particle->timer++;
                if (particle->velocity_y > 0 &&
                    (s16)(particle->y >> 16) > 0x68) {
                    particle->timer = -1;
                }
            }
        }

        if (frame <= 0x4f) {
            for (index = 0; index < context->actor_count; index++) {
                if (frame > 0x1d) {
                    s32 actor_phase = Func_080022fc(frame, 0xc);
                    s16 actor = context->actor_ids[index];

                    if (actor_phase == 0) {
                        struct ActorEffect_080d2d98 *effect =
                            *Func_080b5098(actor);

                        Func_080d6888(actor, 7, 5, -1, 0);
                        effect->vertical_impulse = 0x48000;
                        effect->animation = 0xab85;
                    }
                    if (actor_phase == 6) {
                        Func_080d6888(actor, 0, 5, -1, 0);
                    }
                }
            }
        }

        work->frame_ready = 1;
        Func_080030f8(1);
    }

    Func_08004278((void *)0x080cd261);
    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    Func_080e6eac(1, anchor_x, anchor_y);
    for (index = 0; index < 0x0c; index++) {
        Func_08009038(work->temporary_objects[index]);
    }
    Func_080cdbc0();
}
