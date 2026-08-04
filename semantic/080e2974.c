#include "layout_guard.h"
#include "types.h"

#define REG_BLDALPHA_080E2974 (*(volatile u16 *)0x04000052)
#define PARTICLES_080E2974 ((struct Particle_080e2974 *)0x02010000)
#define CAMERA_080E2974 (*(void **)0x03001e80)

typedef void *(*WordCopy_080e2974)(void *, const void *, s32);
typedef void (*ArmFill_080e2974)(void *, u32, u32);
typedef void (*Renderer_080e2974)(s32, void *, s32, s32, u32, s32);

#define WORD_COPY_080E2974 ((WordCopy_080e2974)0x03001388)
#define ARM_FILL_080E2974 ((ArmFill_080e2974)0x03000168)
#define RENDERER_A_080E2974 (*(Renderer_080e2974 *)0x03001f08)
#define RENDERER_B_080E2974 (*(Renderer_080e2974 *)0x03001f0c)

struct Particle_080e2974 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 timer;
};

struct ActorEffect_080e2974 {
    u8 reserved00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct SceneContext_080e2974 {
    u8 reserved00[4];
    s32 side;
    s32 target_source;
    u8 reserved0c[0x10];
    s32 special_layout;
    u8 reserved20[4];
    s16 actor_id;
};

struct SceneWork_080e2974 {
    u8 source_graphics[0x5100];
    u8 strike_frames[0x2680];
    s32 scene_phase;
    s32 scene_timer;
    u8 reserved7788[0x20];
    s32 burst_phase;
    u8 reserved77ac[0x78];
    s32 frame_ready;
    struct SceneContext_080e2974 *context;
};

struct RuntimeHeader_080e2974 {
    struct SceneWork_080e2974 *work;
    s32 render_target;
    u8 *sprite_sheet;
};

LAYOUT_SIZE_GUARD(Particle080e2974_Size, struct Particle_080e2974, 0x1c);
LAYOUT_OFFSET_GUARD(Particle080e2974_Timer, struct Particle_080e2974, timer, 0x18);
LAYOUT_OFFSET_GUARD(SceneContext080e2974_Side, struct SceneContext_080e2974, side, 0x04);
LAYOUT_OFFSET_GUARD(SceneContext080e2974_Target, struct SceneContext_080e2974, target_source, 0x08);
LAYOUT_OFFSET_GUARD(SceneContext080e2974_Special, struct SceneContext_080e2974, special_layout, 0x1c);
LAYOUT_OFFSET_GUARD(SceneContext080e2974_Actor, struct SceneContext_080e2974, actor_id, 0x24);
LAYOUT_OFFSET_GUARD(SceneWork080e2974_StrikeFrames, struct SceneWork_080e2974, strike_frames, 0x5100);
LAYOUT_OFFSET_GUARD(SceneWork080e2974_ScenePhase, struct SceneWork_080e2974, scene_phase, 0x7780);
LAYOUT_OFFSET_GUARD(SceneWork080e2974_BurstPhase, struct SceneWork_080e2974, burst_phase, 0x77a8);
LAYOUT_OFFSET_GUARD(SceneWork080e2974_FrameReady, struct SceneWork_080e2974, frame_ready, 0x7824);
LAYOUT_OFFSET_GUARD(SceneWork080e2974_Context, struct SceneWork_080e2974, context, 0x7828);

static const struct RuntimeHeader_080e2974 *const Runtime_080e2974 =
    (const struct RuntimeHeader_080e2974 *)0x03001eec;
static const u8 *const SceneConfigs_080e2974 = (const u8 *)0x080eed3e;
static const u16 *const ParticleTiles_080e2974 = (const u16 *)0x080ede48;

void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(u32);
u32 Func_08004458(void);
void Func_080041d8(s32, s32);
void Func_08004278(void *);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_080b5078(s32, s16, s32, s32);
void Func_080b5088(s16, s32);
struct ActorEffect_080e2974 **Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
s32 Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080de2f8(void *, s32, s32, s32, s32 *, s32 *);
void Func_080e0524(void *, void *, s32, s32);
void Func_080e155c(s32, u32);
void Func_080e38b8(struct Particle_080e2974 *, s32, s32);
void Func_080e3944(s32, s32 *);
void Func_080e396c(s32, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

static s32 HalfSigned_080e2974(s32 value) {
    u32 bits = value;
    return (s32)(bits + (bits >> 31)) >> 1;
}

/* Run one table-configured battle strike and its trailing particle burst. */
s32 Func_080e2974(struct SceneContext_080e2974 *context, s32 scene) {
    struct SceneWork_080e2974 *work = Runtime_080e2974->work;
    s32 render_target = Runtime_080e2974->render_target;
    u8 *sprite_sheet = Runtime_080e2974->sprite_sheet;
    void *camera = CAMERA_080E2974;
    const u8 *config = &SceneConfigs_080e2974[scene * 7];
    Renderer_080e2974 renderers[2];
    s32 primary_position[2];
    s32 target_position[2];
    s32 particle_position[3];
    s32 scratch_a;
    s32 scratch_b;
    s32 index;
    s32 frame;

    work->context = context;
    Func_080cd594(0);
    REG_BLDALPHA_080E2974 = 0x1010;
    if (context->special_layout == 1) {
        Func_080de2f8(context, 7, context->side, 2, &scratch_a, &scratch_b);
    }

    Func_080e0524((void *)0x73, sprite_sheet, 0, 0);
    Func_080e0524((void *)0x99, work, 1, 0);
    for (index = 0; index < 0x120; index++) {
        s32 byte;
        u8 *source = &work->source_graphics[index * 0x28];
        u8 *destination = &work->strike_frames[index * 0x14];

        for (byte = 0; byte < 0x28; byte++) {
            destination[byte / 2] = source[byte];
        }
    }

    Func_080e0524((void *)(config[0] == 0 ? 0xb5 : 0xb6), work, 1, 1);
    Func_080e0524((void *)0x6b, (void *)0x02015e00, 1, 0);
    switch (config[1]) {
    case 0: index = 0x8d; break;
    case 1: index = 0xa0; break;
    case 2: index = 0xb6; break;
    default: index = 0xb4; break;
    }
    WORD_COPY_080E2974((void *)0x05000000, Func_08002f40(index), 0x80);
    Func_080030f8(1);

    Func_080e396c(context->actor_id, primary_position);
    work->scene_phase = 2;
    work->scene_timer = 0x4b;
    Func_080041d8(0x080cd261, 0x480);

    {
        struct ActorEffect_080e2974 *actor = *Func_080b5098(context->actor_id);

        for (index = 0; index < 0x300; index++) {
            struct Particle_080e2974 *particle = &PARTICLES_080E2974[index];

            particle->x = actor->x;
            particle->y = actor->y + 0x190000;
            particle->z = actor->z;
            particle->velocity_x = (u8)Func_08004458() << 12;
            particle->velocity_y = ((u8)Func_08004458() - 0x7f) << 12;
            particle->velocity_z = ((u8)Func_08004458() - 0x7f) << 12;
            if (particle->x > 0) {
                particle->velocity_x = -particle->velocity_x;
            }
            particle->timer = -1;
        }
    }
    Func_080b5078(context->target_source, context->actor_id, 4, 0);

    for (frame = 0; frame < config[5]; frame++) {
        s32 stage;

        Func_080e396c(context->target_source, target_position);
        target_position[0] = HalfSigned_080e2974(target_position[0]);

        if (context->side == 0) {
            Func_080ed408(0x2e, 7, 7, 3, 2);
            Func_080ed408(0x2f, 7, 7, 0xb, 2);
        } else {
            Func_080ed408(0x2e, 7, 7, 7, 2);
            Func_080ed408(0x2f, 7, 7, 0xf, 2);
        }
        renderers[0] = RENDERER_A_080E2974;
        renderers[1] = RENDERER_B_080E2974;

        for (stage = 0; stage < config[2]; stage++) {
            s32 start = stage * config[4];

            if (frame >= start && frame < start + 6) {
                s32 elapsed = frame - start;
                void *source;
                s32 x;
                s32 y;

                if ((stage & 3) <= 1 || config[0] == 1) {
                    source = (u8 *)work + elapsed * 0xd80;
                    x = HalfSigned_080e2974(primary_position[0]) -
                        (context->side == 0 ? 0x10 : 0x20);
                    y = primary_position[1] - 0x28;
                } else {
                    source = (u8 *)0x02015e00 + elapsed * 0x300;
                    x = HalfSigned_080e2974(primary_position[0]) -
                        (context->side == 0 ? 0x10 : 0x20);
                    y = target_position[1] - 8;
                }
                renderers[stage & 1](render_target, source, x, y, 0x30, 0x10);
            }

            if (frame == start + 2) {
                if (config[6] == 1) {
                    ARM_FILL_080E2974((void *)render_target, 0x4000, 0x2f2f2f2f);
                }
                Func_080d6888(context->actor_id, 7, 5, 0, 4);
                if (stage == config[2] - 1) {
                    Func_080b5088(context->actor_id, 4);
                    work->burst_phase = 8;
                    Func_080b50e8(0x86);
                } else {
                    if (stage & 1) Func_080b5088(context->actor_id, 7);
                    work->burst_phase = 4;
                    Func_080f9010(0x86);
                }
                for (index = 0; index < config[3]; index++) {
                    PARTICLES_080E2974[stage * 0x20 + index].timer =
                        (Func_08004458() & 7) + 0x0f;
                }
            }

            if (frame >= start + 2 && frame < start + 0x0e) {
                s32 animation = HalfSigned_080e2974(frame - start - 2);

                renderers[0](
                    render_target,
                    &work->strike_frames[animation * 0x3c0],
                    HalfSigned_080e2974(primary_position[0]) - 0x0a,
                    primary_position[1] - 0x18,
                    0x14,
                    0x30);
            }
        }

        Func_08002dd8(0x2f);
        Func_08002dd8(0x2e);
        Func_080049ac();
        Func_080051d8(camera, (u8 *)camera + 0x0c);
        Func_080ed408(0x2e, 7, 7, 3, 3);
        Func_080ed408(0x2f, 7, 7, 3, 2);
        renderers[0] = RENDERER_A_080E2974;
        renderers[1] = RENDERER_B_080E2974;

        for (index = 0; index < 0x200; index++) {
            struct Particle_080e2974 *particle = &PARTICLES_080E2974[index];

            if (particle->timer > 0) {
                s32 size;
                s32 diameter;

                Func_080e3944((s32)particle, particle_position);
                particle_position[0] = HalfSigned_080e2974(particle_position[0]);
                size = particle->timer / 8 + 1;
                diameter = size * 2;
                renderers[(index / 2) & 1](
                    render_target,
                    sprite_sheet + ParticleTiles_080e2974[size - 1],
                    particle_position[0] - size / 2,
                    particle_position[1] - size,
                    size,
                    diameter);
                Func_080e38b8(particle, 0x3c, -0x400);
                if (particle->y <= 0x7ffff) {
                    s32 reflected = -particle->velocity_y;
                    particle->velocity_y = HalfSigned_080e2974(reflected);
                }
                particle->timer--;
            }
        }

        Func_08002dd8(0x2f);
        Func_08002dd8(0x2e);
        Func_080e155c(8, 8);
        Func_080cd52c();
        work->frame_ready = 1;
        Func_080030f8(1);
    }

    Func_08004278((void *)0x080cd261);
    return Func_080cdbc0();
}
