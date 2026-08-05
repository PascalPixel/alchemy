#include "layout_guard.h"
#include "types.h"

#define NULL ((void *)0)
#define REG_BG2PA_080DAB74 (*(volatile u16 *)0x04000020)
#define REG_BG2X_080DAB74 (*(volatile s32 *)0x04000028)
#define BURST_PARTICLES_080DAB74 ((struct Particle_080dab74 *)0x02010000)

typedef void (*Renderer_080dab74)(s32, void *, u8, s16, s32, s32);

struct Particle_080dab74 {
    s32 angle_or_x;
    s32 y;
    s32 radius_or_z;
    s32 velocity_x;
    s32 velocity_y;
    s32 reserved14;
    s32 timer;
};

struct SceneContext_080dab74 {
    u8 reserved00[4];
    s32 side;
    u8 reserved08[0x0c];
    s32 actor_count;
    s32 variant;
    u8 reserved1c[8];
    s16 actor_ids[1];
};

struct SceneWork_080dab74 {
    u8 graphics[0x7080];
    struct Particle_080dab74 particles[0x40];
    s32 scene_phase;
    s32 scene_timer;
    u8 reserved7788[0x9c];
    s32 frame_ready;
    struct SceneContext_080dab74 *context;
};

struct RuntimeHeader_080dab74 {
    struct SceneWork_080dab74 *work;
    s32 render_target;
    u8 *sprite_sheet;
};

LAYOUT_SIZE_GUARD(Particle080dab74_Size, struct Particle_080dab74, 0x1c);
LAYOUT_OFFSET_GUARD(Particle080dab74_Timer, struct Particle_080dab74, timer, 0x18);
LAYOUT_OFFSET_GUARD(SceneContext080dab74_Count, struct SceneContext_080dab74, actor_count, 0x14);
LAYOUT_OFFSET_GUARD(SceneContext080dab74_Variant, struct SceneContext_080dab74, variant, 0x18);
LAYOUT_OFFSET_GUARD(SceneContext080dab74_Ids, struct SceneContext_080dab74, actor_ids, 0x24);
LAYOUT_OFFSET_GUARD(SceneWork080dab74_Particles, struct SceneWork_080dab74, particles, 0x7080);
LAYOUT_OFFSET_GUARD(SceneWork080dab74_Phase, struct SceneWork_080dab74, scene_phase, 0x7780);
LAYOUT_OFFSET_GUARD(SceneWork080dab74_FrameReady, struct SceneWork_080dab74, frame_ready, 0x7824);
LAYOUT_OFFSET_GUARD(SceneWork080dab74_Context, struct SceneWork_080dab74, context, 0x7828);

static const struct RuntimeHeader_080dab74 *const Runtime_080dab74 =
    (const struct RuntimeHeader_080dab74 *)0x03001eec;
static const u8 *const SceneConfigs_080dab74 = (const u8 *)0x080eea88;
static const u8 *const OrbitWidths_080dab74 = (const u8 *)0x080eea91;
static const u8 *const OrbitHeights_080dab74 = (const u8 *)0x080eea99;
static const u16 *const OrbitOffsets_080dab74 = (const u16 *)0x080eeaa2;
static const u8 *const CurtainPositions_080dab74 = (const u8 *)0x080eea62;
static const u16 *const CurtainOffsets_080dab74 = (const u16 *)0x080eeab2;
static const u8 *const CurtainHeights_080dab74 = (const u8 *)0x080eeab8;
static const u8 *const BurstWidths_080dab74 = (const u8 *)0x080eeabb;
static const u8 *const BurstHeights_080dab74 = (const u8 *)0x080eeac3;
static const u16 *const BurstOffsets_080dab74 = (const u16 *)0x080eeacc;
static const u16 *const SparkOffsets_080dab74 = (const u16 *)0x080ede84;
static const u8 *const SparkSizes_080dab74 = (const u8 *)0x080ede96;

void Func_08002dd8(s32);
s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
u16 Func_08002304(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
u32 Func_08004458(void);
void Func_080041d8(s32, s32);
void Func_08004278(void *);
void Func_080049ac(void);
void Func_08004cb4(s32 *);
void Func_080030f8(u32);
void Func_080b50e8(s32);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080cef64(s32, Renderer_080dab74 renderers[2]);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(void *, void *, s32, s32);
void Func_080e3944(s32, s32 *);
void Func_080f9010(s32);

static s32 HalfSigned_080dab74(s32 value) {
    u32 bits = value;
    return (s32)(bits + (bits >> 31)) >> 1;
}

/* Run the orbit, curtain, burst, and recycled-spark layers of the scene. */
void Func_080dab74(struct SceneContext_080dab74 *context) {
    struct SceneWork_080dab74 *work = Runtime_080dab74->work;
    s32 render_target = Runtime_080dab74->render_target;
    u8 *sprite_sheet = Runtime_080dab74->sprite_sheet;
    const u8 *config = &SceneConfigs_080dab74[context->variant * 3];
    Renderer_080dab74 renderers[2];
    s32 display_state[3];
    s32 source_position[3];
    s32 projected[3];
    s32 frame;
    s32 index;

    work->context = context;
    Func_080cd594(1);
    REG_BG2PA_080DAB74 = 0x100;
    Func_080e0524((void *)0xb8, work, 1, 1);
    Func_080e0524((void *)0xba, sprite_sheet, 0, 0);
    Func_080cef64(0, renderers);

    for (index = 0; index < 0x40; index++) {
        struct Particle_080dab74 *particle = &work->particles[index];

        particle->angle_or_x = Func_08004458() & 0xffff;
        particle->radius_or_z = (Func_08004458() & 0x3f) + 0x38;
        particle->y = ((Func_08004458() & 0x1f) - 0x40) << 16;
    }
    work->scene_phase = 2;
    work->scene_timer = 0x32;
    Func_080041d8(0x080cd261, 0x480);
    if (context->side == 1) REG_BG2X_080DAB74 = 0xffff9000;

    for (frame = 0; frame != config[2] + 0x4b; frame++) {
        s32 lower = 0x780000;
        s32 upper = 0;
        s32 orbit_count = frame < config[2] ? config[0] : 0x10;
        u32 sound_window = frame - 0x24;

        if (frame == config[2] + 0x0b) Func_080b50e8(0x84);
        display_state[0] = 0;
        display_state[1] = 0;
        display_state[2] = 0x02000000;
        Func_080049ac();
        Func_08004cb4(display_state);
        if (sound_window <= 0x1b && (frame & 3) == 0) Func_080f9010(0x73);
        if (frame == 0x55) Func_080f9010(0x88);

        for (index = 0; index < context->actor_count; index++) {
            if (frame == 0x28 + index * 4) {
                Func_080d6888(context->actor_ids[index], 9, 5, -1, 0);
            }
        }

        if (frame < config[2] + 0x23) {
            for (index = 0; index < orbit_count; index++) {
                struct Particle_080dab74 *particle = &work->particles[index];

                if (frame > index) {
                    s32 image = index & 7;

                    if (particle->y < (0x30 - index / 2) * 0x10000 &&
                        particle->y > -0x300000) {
                        u8 width;
                        u8 height;

                        source_position[0] =
                            particle->radius_or_z * Func_08002322(particle->angle_or_x);
                        source_position[1] = particle->y;
                        source_position[2] =
                            particle->radius_or_z * Func_0800231c(particle->angle_or_x);
                        Func_080e3944((s32)source_position, projected);
                        projected[0] = (projected[0] >> 17) + 0x40;
                        projected[1] = (s16)(projected[1] >> 16) + 0x3c;
                        width = OrbitWidths_080dab74[image];
                        height = OrbitHeights_080dab74[image];
                        renderers[1](render_target,
                                     &work->graphics[OrbitOffsets_080dab74[image]],
                                     projected[0] - width / 2,
                                     projected[1] - height / 2,
                                     width, height);
                    }

                    if (frame < config[2]) {
                        if (frame > index + 0x10) {
                            if (particle->radius_or_z > 4) particle->radius_or_z -= 2;
                            if (particle->y <= 0x2fffff) particle->y += 0x50000;
                            particle->angle_or_x += 0x200;
                        }
                    } else {
                        particle->radius_or_z += 8;
                        particle->y -= (Func_080022fc(index, 5) + 2) << 16;
                        if (lower > particle->y) lower = particle->y;
                        if (upper < particle->y) upper = particle->y;
                    }
                }
            }
        }

        lower += 0x400000;
        upper += 0x400000;

        if (frame < config[2]) {
            for (index = 0; index < config[1]; index++) {
                if (index < Func_080022ec((s32)sound_window, 3)) {
                    s32 image = Func_080022fc(index, 3);
                    u8 height = CurtainHeights_080dab74[image];
                    s32 x = CurtainPositions_080dab74[index * 2];
                    s32 y = CurtainPositions_080dab74[index * 2 + 1] - height;

                    renderers[frame >= config[2] - 7 ? 1 : 0](
                        render_target,
                        &work->graphics[CurtainOffsets_080dab74[image]],
                        x, y, 0x20, height);
                }
            }
        }

        if (frame == config[2]) {
            for (index = 0; index < 0x20; index++) {
                struct Particle_080dab74 *particle = &BURST_PARTICLES_080DAB74[index];

                particle->angle_or_x = (Func_08004458() & 0x7f) << 16;
                particle->y = ((Func_08004458() & 0x0f) + 0x50) << 16;
                particle->radius_or_z = ((Func_08004458() & 0x3f) - 0x20) << 12;
                particle->velocity_y = (((-Func_08004458()) & 0x0f) - 0x10) << 13;
                particle->timer = (Func_08004458() & 0x0f) + 0x10;
            }
        }

        if (frame >= config[2]) {
            for (index = 0; index < 0x18; index++) {
                struct Particle_080dab74 *particle = &BURST_PARTICLES_080DAB74[index];

                if (particle->timer >= 0) {
                    s32 image = index & 7;
                    renderers[1](render_target,
                                 &work->graphics[BurstOffsets_080dab74[image]],
                                 (u8)(particle->angle_or_x >> 16),
                                 (s16)(particle->y >> 16),
                                 BurstWidths_080dab74[image],
                                 BurstHeights_080dab74[image]);
                    particle->angle_or_x += particle->velocity_x;
                    particle->y += particle->velocity_y;
                    particle->timer--;
                }
                if (lower > particle->y) lower = particle->y;
                if (upper < particle->y) upper = particle->y;
            }
        }

        lower >>= 16;
        upper >>= 16;
        if (upper <= lower) upper = lower + 1;

        if (frame == config[2]) {
            for (index = 0; index < 0x20; index++) {
                struct Particle_080dab74 *particle = &work->particles[index];

                particle->velocity_x = (Func_08004458() & 0x7f) << 16;
                if (upper == lower) {
                    particle->velocity_y = lower << 16;
                } else {
                    particle->velocity_y =
                        (Func_08002304(Func_08004458(), upper - lower) + lower) << 16;
                }
                particle->timer = (Func_08004458() & 0x0f) + 0x14;
            }
        }

        if (frame >= config[2]) {
            u32 age = frame - config[2];

            for (index = 0; index < 0x20; index++) {
                struct Particle_080dab74 *particle = &work->particles[index];

                if ((u32)particle->timer <= 0x11) {
                    s32 image = HalfSigned_080dab74(0x11 - particle->timer);
                    u8 size = SparkSizes_080dab74[image];

                    renderers[1](render_target,
                                 sprite_sheet + SparkOffsets_080dab74[image],
                                 (s16)(particle->velocity_x >> 16) - size / 2,
                                 (s16)(particle->velocity_y >> 16) - size / 2 -
                                     HalfSigned_080dab74(age),
                                 size, size);
                }
                particle->timer--;
                if ((particle->timer == -1 || particle->timer == 0x11) &&
                    frame < config[2] + 0x23) {
                    particle->timer = 0x11;
                    particle->velocity_x = (Func_08004458() & 0x7f) << 16;
                    particle->velocity_y =
                        (Func_08002304(Func_08004458(), upper - lower) + lower) << 16;
                }
            }
        }

        work->frame_ready = 1;
        Func_080030f8(1);
    }

    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    Func_08004278((void *)0x080cd261);
    Func_080cdbc0();
}
