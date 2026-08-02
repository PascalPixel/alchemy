#include "layout_guard.h"
#include "types.h"

#define REG_BLDALPHA_080D4604 (*(volatile u16 *)0x04000052)
#define MAIN_PARTICLES_080D4604 ((struct Particle_080d4604 *)0x02010000)
#define CAMERA_080D4604 (*(struct Camera_080d4604 **)0x03001e80)

enum {
    TRAIL_GROUP_CAPACITY_080D4604 = 4,
    TRAIL_SEED_COUNT_080D4604 = 16,
    TRAIL_DRAW_COUNT_080D4604 = 12
};

typedef void *(*WordCopy_080d4604)(
    void *destination,
    const void *source,
    s32 size);
typedef void (*ArmFill_080d4604)(
    void *destination,
    u32 size,
    u32 value);
typedef void (*Renderer_080d4604)(
    s32 target,
    void *source,
    s32 x,
    s32 y,
    u32 width,
    s32 height);

#define WORD_COPY_080D4604 ((WordCopy_080d4604)0x03001388)
#define ARM_FILL_080D4604 ((ArmFill_080d4604)0x03000168)
#define RENDERER_A_080D4604 (*(Renderer_080d4604 *)0x03001f08)
#define RENDERER_B_080D4604 (*(Renderer_080d4604 *)0x03001f0c)

struct Particle_080d4604 {
    s32 x;
    s32 y;
    s32 reserved08;
    s32 velocity_x;
    s32 velocity_y;
    s32 reserved14;
    s32 timer;
};

struct SceneContext_080d4604 {
    u8 reserved00[4];
    s32 side;
    s32 bounds_source;
    u8 reserved0c[8];
    s32 actor_count;
    s32 variant;
    u8 reserved1c[8];
    s16 actor_ids[1];
};

struct SceneWork_080d4604 {
    u8 reserved0000[0x7080];
    struct Particle_080d4604
        trail_groups[TRAIL_GROUP_CAPACITY_080D4604][TRAIL_SEED_COUNT_080D4604];
    s32 scene_phase;
    s32 scene_timer;
    u8 reserved7788[0x20];
    s32 burst_phase;
    u8 reserved77ac[0x78];
    s32 frame_ready;
    struct SceneContext_080d4604 *context;
};

struct RuntimeHeader_080d4604 {
    struct SceneWork_080d4604 *work;
    s32 render_target;
    u8 *sprite_sheet;
};

struct Camera_080d4604 {
    u8 reserved00[0x36];
    u16 horizontal_offset;
};

LAYOUT_SIZE_GUARD(
    Particle080d4604_Size,
    struct Particle_080d4604,
    0x1c);
LAYOUT_OFFSET_GUARD(
    Particle080d4604_VelocityX,
    struct Particle_080d4604,
    velocity_x,
    0x0c);
LAYOUT_OFFSET_GUARD(
    Particle080d4604_Timer,
    struct Particle_080d4604,
    timer,
    0x18);
LAYOUT_OFFSET_GUARD(
    SceneContext080d4604_ActorCount,
    struct SceneContext_080d4604,
    actor_count,
    0x14);
LAYOUT_OFFSET_GUARD(
    SceneContext080d4604_Variant,
    struct SceneContext_080d4604,
    variant,
    0x18);
LAYOUT_OFFSET_GUARD(
    SceneContext080d4604_ActorIds,
    struct SceneContext_080d4604,
    actor_ids,
    0x24);
LAYOUT_OFFSET_GUARD(
    SceneWork080d4604_TrailGroups,
    struct SceneWork_080d4604,
    trail_groups,
    0x7080);
LAYOUT_OFFSET_GUARD(
    SceneWork080d4604_ScenePhase,
    struct SceneWork_080d4604,
    scene_phase,
    0x7780);
LAYOUT_OFFSET_GUARD(
    SceneWork080d4604_BurstPhase,
    struct SceneWork_080d4604,
    burst_phase,
    0x77a8);
LAYOUT_OFFSET_GUARD(
    SceneWork080d4604_FrameReady,
    struct SceneWork_080d4604,
    frame_ready,
    0x7824);
LAYOUT_OFFSET_GUARD(
    SceneWork080d4604_Context,
    struct SceneWork_080d4604,
    context,
    0x7828);
LAYOUT_OFFSET_GUARD(
    Camera080d4604_HorizontalOffset,
    struct Camera_080d4604,
    horizontal_offset,
    0x36);

static const struct RuntimeHeader_080d4604 *const Runtime_080d4604 =
    (const struct RuntimeHeader_080d4604 *)0x03001eec;
/* Five u16 values per variant: particle count, group count, three offsets. */
static const u16 *const LayoutWords_080d4604 = (const u16 *)0x080ee262;
static const u16 *const ParticleTiles_080d4604 = (const u16 *)0x080ede48;
static const u8 *const TrailFrames_080d4604 = (const u8 *)0x080ee294;

void Func_08002dd8(s32 slot);
void *Func_08002f40(s32 resource);
void Func_080030f8(u32 frames);
s32 Func_080022ec(s32 value, s32 divisor);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
u32 Func_08004458(void);
void Func_080041d8(s32 callback, s32 size);
void Func_08004278(void *callback);
void Func_080b5088(s16 actor, s32 action);
void Func_080b50e8(s32 sound);
void Func_080cd52c(void);
void Func_080cd594(s32 enabled);
s32 Func_080cdbc0(void);
void Func_080d6888(s16 actor, s32 a, s32 b, s32 index, s32 duration);
void Func_080e0524(void *resource, void *destination, s32 a, s32 b);
void Func_080e155c(s32 x, u32 y);
void Func_080e3908(struct Particle_080d4604 *particle, s32 gravity, s32 wind);
void Func_080e396c(s32 source, s32 *bounds);
void Func_080ed408(s32 slot, s32 a, s32 b, s32 c, s32 d);
void Func_080f9010(s32 sound);

/*
 * Run a staged battle-transition scene. Each group owns a short-lived spray
 * in scene work and a variable-size particle run in shared EWRAM.
 *
 * The renderer slots are populated once by Func_080ed408 and cached locally.
 * The IWRAM copy/fill targets are ordinary three-argument routines.
 */
s32 Func_080d4604(struct SceneContext_080d4604 *context, s32 mode) {
    struct SceneWork_080d4604 *work = Runtime_080d4604->work;
    s32 render_target = Runtime_080d4604->render_target;
    u8 *sprite_sheet = Runtime_080d4604->sprite_sheet;
    Renderer_080d4604 renderers[2];
    const u16 *layout;
    u16 particles_per_group;
    u16 group_count;
    s32 origin_x;
    s32 origin_y;
    s32 group;
    s32 frame;

    work->context = context;
    layout = &LayoutWords_080d4604[context->variant * 5];
    particles_per_group = layout[0];
    group_count = layout[1];

    if (mode == 0) {
        Func_080cd594(1);
        origin_x = 0x3c;
        origin_y = 0x30;
    } else if (mode == 1) {
        Func_080cd594(0);
        origin_x = 0x3c;
        origin_y = 0x40;
    } else {
        s32 bounds[2];
        u32 first_bound;

        Func_080cd594(0);
        Func_080e396c(context->bounds_source, bounds);
        first_bound = bounds[0];
        origin_x = (s32)(first_bound + (first_bound >> 31)) >> 1;
        origin_y = bounds[1] + 0x30;
    }

    REG_BLDALPHA_080D4604 = 0x1010;
    Func_080ed408(0x2e, 7, 7, 3, 2);
    renderers[0] = RENDERER_A_080D4604;
    Func_080ed408(0x2f, 7, 7, 3, 3);
    renderers[1] = RENDERER_B_080D4604;
    Func_080e0524((void *)0x7d, work, 1, 1);
    Func_080e0524((void *)0x73, sprite_sheet, 0, 0);

    if (mode == 1) {
        WORD_COPY_080D4604(
            (void *)0x05000000,
            Func_08002f40(0x87),
            0x80);
    } else if (mode == 2) {
        WORD_COPY_080D4604(
            (void *)0x05000000,
            Func_08002f40(0xc4),
            0x80);
    }

    for (group = 0; group < group_count; group++) {
        struct Particle_080d4604 *trail = work->trail_groups[group];
        s32 particle_index;

        for (particle_index = 0;
             particle_index < TRAIL_SEED_COUNT_080D4604;
             particle_index++, trail++) {
            s32 speed = particle_index * 2;
            u16 angle = Func_08004458();

            trail->x = speed * Func_08002322(angle);
            trail->y = -(speed * Func_0800231c(angle));
            trail->timer = particle_index / 2 + 0x19;
        }

        for (particle_index = 0;
             particle_index < particles_per_group;
             particle_index++) {
            struct Particle_080d4604 *particle =
                &MAIN_PARTICLES_080D4604[
                    particles_per_group * group + particle_index];
            s32 speed = (Func_08004458() & 0x3ff) + 0x20;
            u16 angle = Func_08004458();

            if (context->side == 1) {
                particle->x =
                    (origin_x - layout[group + 2] + 0x1c) << 16;
            } else {
                particle->x =
                    (origin_x + layout[group + 2] - 0x1c) << 16;
            }
            particle->y = origin_y << 16;
            particle->velocity_x =
                speed * Func_08002322(angle) >> 6;
            particle->velocity_y =
                -(speed * Func_0800231c(angle) * 2) >> 6;
            particle->timer = (Func_08004458() & 7) + 0x20;
        }
    }

    work->scene_phase = 2;
    work->scene_timer = 0x4b;
    Func_080041d8(0x080cd261, 0x480);

    /* The ROM compares the u16 group count with this unreachable sentinel. */
    if (group_count != 0x1ffffff9) {
        u32 sound_mode = mode - 1;

        for (frame = 0; frame != group_count * 8 + 0x38; frame++) {
            if (context->variant == 2 && frame <= 0x33) {
                if (context->side == 0) {
                    CAMERA_080D4604->horizontal_offset += 0x100;
                } else {
                    CAMERA_080D4604->horizontal_offset -= 0x100;
                }
            }

            if (context->variant == 3 && frame == 4) {
                ARM_FILL_080D4604(
                    (void *)render_target,
                    0x4000,
                    0x3f3f3f3f);
            }

            if (sound_mode <= 1) {
                if (frame == 2) {
                    Func_080b50e8(0x91);
                }
            } else {
                if (frame == 2) {
                    Func_080f9010(0x91);
                }
                if (frame == 0x18) {
                    Func_080b50e8(0x86);
                }
            }

            for (group = 0; group < group_count; group++) {
                s32 start_frame = group * 8;
                s32 particle_index;

                if (frame == start_frame) {
                    work->burst_phase = 0xc;
                }

                if (frame >= start_frame && frame < start_frame + 2) {
                    s32 x;

                    if (context->side == 1) {
                        x = origin_x - layout[group + 2] + 0xc;
                    } else {
                        x = origin_x + layout[group + 2] - 0x2c;
                    }
                    renderers[0](
                        render_target,
                        work,
                        x,
                        origin_y - 0x20,
                        0x20,
                        0x40);
                }

                if (frame >= start_frame) {
                    struct Particle_080d4604 *trail = work->trail_groups[group];

                    for (particle_index = 0;
                         particle_index < TRAIL_DRAW_COUNT_080D4604;
                         particle_index++, trail++) {
                        s32 x;
                        s32 y = (s16)(trail->y >> 16) + origin_y;

                        if (context->side == 1) {
                            x = (s16)(trail->x >> 16) + origin_x -
                                layout[group + 2] + 0x1c;
                        } else {
                            x = (s16)(trail->x >> 16) + origin_x +
                                layout[group + 2] - 0x1c;
                        }

                        if ((u32)trail->timer <= 0x11) {
                            renderers[0](
                                render_target,
                                (u8 *)work +
                                    (TrailFrames_080d4604[
                                         Func_080022ec(trail->timer, 3)]
                                     << 11),
                                x - 0x10,
                                y - 0x20,
                                0x20,
                                0x40);
                        }

                        if (trail->timer > 0) {
                            trail->timer--;
                        } else {
                            trail->timer = -1;
                        }
                    }
                }

                if (frame > start_frame + 5) {
                    s32 wind = mode == 2 ? -0x1000 : 0x1000;

                    for (particle_index = 0;
                         particle_index < particles_per_group;
                         particle_index++) {
                        struct Particle_080d4604 *particle =
                            &MAIN_PARTICLES_080D4604[
                                particles_per_group * group +
                                particle_index];

                        if (particle->timer > 0) {
                            s32 size;

                            Func_080e3908(particle, 0x3c, wind);
                            particle->timer--;

                            if (particle->y > 0x6c0000) {
                                s32 reflected = -particle->velocity_y;

                                particle->velocity_y =
                                    (s32)((u32)reflected +
                                          ((u32)reflected >> 31)) >> 1;
                            } else if ((u32)particle->x <= 0x7effff &&
                                       particle->y >= 0) {
                                size = Func_080022ec(particle->timer, 5) + 1;
                                renderers[particle_index & 1](
                                    render_target,
                                    sprite_sheet +
                                        ParticleTiles_080d4604[size - 1],
                                    (particle->x >> 16) - size / 2,
                                    (particle->y >> 16) - size,
                                    size,
                                    size * 2);
                            }
                        }
                    }
                }

                for (particle_index = 0;
                     particle_index < context->actor_count;
                     particle_index++) {
                    if (frame == start_frame + 6) {
                        s16 actor = context->actor_ids[particle_index];

                        Func_080d6888(actor, 7, 5, particle_index, 0xa);
                        Func_080b5088(actor, 4);
                    }
                }
            }

            Func_080e155c(0x10, 0x10);
            Func_080cd52c();
            work->frame_ready = 1;
            Func_080030f8(1);
        }
    }

    Func_08004278((void *)0x080cd261);
    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    return Func_080cdbc0();
}
