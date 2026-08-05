#include "layout_guard.h"
#include "types.h"

#define REG_BLDALPHA_080DE2F8 (*(volatile u16 *)0x04000052)
#define MAIN_PARTICLES_080DE2F8 ((struct Particle_080de2f8 *)0x02010000)
#define CAMERA_080DE2F8 (*(void **)0x03001e80)

typedef void (*Renderer_080de2f8)(s32, void *, s32, s32, u32, s32);
typedef void *(*WordCopy_080de2f8)(void *, const void *, s32);
typedef s32 (*IwramClear_080de2f8)(void *, u32);
#define WORD_COPY_080DE2F8 ((WordCopy_080de2f8)0x03001388)
#define IWRAM_CLEAR_080DE2F8 ((IwramClear_080de2f8)0x03000164)
#define RENDERER_A_080DE2F8 (*(Renderer_080de2f8 *)0x03001f08)
#define RENDERER_B_080DE2F8 (*(Renderer_080de2f8 *)0x03001f0c)

struct Particle_080de2f8 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 timer;
};

struct ActorEffect_080de2f8 {
    u8 reserved00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct SceneContext_080de2f8 {
    u8 reserved00[8];
    s32 target_actor;
    u8 reserved0c[0x18];
    s16 actor_id;
};

struct SceneWork_080de2f8 {
    u8 graphics[0x7080];
    struct Particle_080de2f8 burst_particles[0x40];
    s32 scene_phase;
    s32 scene_timer;
    u8 reserved7788[0x2c];
    s32 display_span;
    s32 display_offset;
    u8 reserved77bc[0x68];
    s32 frame_ready;
    struct SceneContext_080de2f8 *context;
};

struct RuntimeHeader_080de2f8 {
    struct SceneWork_080de2f8 *work;
    s32 render_target;
    u8 *sprite_sheet;
};

LAYOUT_SIZE_GUARD(Particle080de2f8_Size, struct Particle_080de2f8, 0x1c);
LAYOUT_OFFSET_GUARD(Particle080de2f8_Timer, struct Particle_080de2f8, timer, 0x18);
LAYOUT_OFFSET_GUARD(SceneContext080de2f8_Target, struct SceneContext_080de2f8, target_actor, 0x08);
LAYOUT_OFFSET_GUARD(SceneContext080de2f8_Actor, struct SceneContext_080de2f8, actor_id, 0x24);
LAYOUT_OFFSET_GUARD(SceneWork080de2f8_Burst, struct SceneWork_080de2f8, burst_particles, 0x7080);
LAYOUT_OFFSET_GUARD(SceneWork080de2f8_DisplaySpan, struct SceneWork_080de2f8, display_span, 0x77b4);
LAYOUT_OFFSET_GUARD(SceneWork080de2f8_FrameReady, struct SceneWork_080de2f8, frame_ready, 0x7824);
LAYOUT_OFFSET_GUARD(SceneWork080de2f8_Context, struct SceneWork_080de2f8, context, 0x7828);

static const struct RuntimeHeader_080de2f8 *const Runtime_080de2f8 =
    (const struct RuntimeHeader_080de2f8 *)0x03001eec;
static const u16 *const ParticleTiles_080de2f8 = (const u16 *)0x080ede48;

void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(u32);
s32 Func_080022ec(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
u32 Func_08004458(void);
void Func_080041d8(s32, s32);
void Func_08004278(void *);
void Func_080049ac(void);
void Func_080049e8(void);
void Func_08004a5c(void);
void Func_08004bd4(s32);
void Func_08004c1c(s32);
void Func_08004c6c(s32);
void Func_08004cb4(s32 *);
void Func_080051d8(void *, void *);
void Func_08005340(s32, s32);
struct ActorEffect_080de2f8 **Func_080b5098(s32);
void Func_080cd594(s32);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080e0524(void *, void *, s32, s32);
void Func_080e38b8(struct Particle_080de2f8 *, s32, s32);
void Func_080e3908(struct Particle_080de2f8 *, s32, s32);
void Func_080e3944(s32, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

static s32 ScaleBy64_080de2f8(s32 value) {
    value *= 0x3e;
    if (value < 0) value += 0x3f;
    return value >> 6;
}

/* Configure and run the shared projectile-orbit effect. */
s32 Func_080de2f8(
    struct SceneContext_080de2f8 *context,
    s32 palette_mode,
    s32 mirrored,
    s32 target_mode,
    s32 *marker_x,
    s32 *marker_y
) {
    struct SceneWork_080de2f8 *work = Runtime_080de2f8->work;
    s32 render_target = Runtime_080de2f8->render_target;
    u8 *sprite_sheet = Runtime_080de2f8->sprite_sheet;
    Renderer_080de2f8 render_a;
    Renderer_080de2f8 render_b;
    struct ActorEffect_080de2f8 *target;
    struct ActorEffect_080de2f8 *actor;
    s32 current[3];
    s32 destination[3];
    s32 delta[3];
    s32 projected[3];
    s32 point[3];
    s32 duration;
    s32 resource;
    s32 frame;
    u32 index;
    s32 result;

    work->context = context;
    Func_080cd594(0);
    work->display_span = 0x18;
    work->display_offset = 0;
    if (palette_mode > 3) {
        palette_mode -= 4;
        duration = 0x54;
    } else {
        duration = 0x40;
    }
    switch (palette_mode) {
    case 0: resource = 0x94; break;
    case 1: resource = 0x92; break;
    case 2: resource = 0x8e; break;
    default: resource = 0x90; break;
    }

    result = (s32)Func_08002f40(resource);
    WORD_COPY_080DE2F8((void *)0x05000000, (void *)result, 0x80);
    Func_08005340(result + 0x80, (s32)work);
    Func_080e0524((void *)0x73, sprite_sheet, 0, 0);
    if (mirrored == 1) {
        Func_080ed408(0x2e, 7, 7, 7, 3);
        Func_080ed408(0x2f, 7, 7, 7, 2);
    } else {
        Func_080ed408(0x2e, 7, 7, 3, 3);
        Func_080ed408(0x2f, 7, 7, 3, 2);
    }
    render_b = RENDERER_B_080DE2F8;
    render_a = RENDERER_A_080DE2F8;

    target = *Func_080b5098(context->target_actor);
    actor = *Func_080b5098(context->actor_id);
    for (index = 0; index < 0x40; index++) {
        struct Particle_080de2f8 *particle = &MAIN_PARTICLES_080DE2F8[index];
        u16 angle = Func_08004458();
        s32 speed = (Func_08004458() & 0xff) + 0x80;

        particle->x = 0;
        particle->y = ((Func_08004458() & 0x1f) + 0x14) << 16;
        particle->z = 0;
        particle->velocity_x = speed * Func_08002322(angle) >> 5;
        particle->velocity_y = 0;
        particle->velocity_z = speed * Func_0800231c(angle) >> 5;
        particle->timer = 0;
    }

    work->scene_phase = 2;
    work->scene_timer = 0x4b;
    Func_080041d8(0x080cd261, 0x480);
    current[0] = target->x;
    current[1] = 0;
    current[2] = target->z;
    destination[1] = 0x3c0000;
    switch (target_mode) {
    case 0:
        destination[0] = actor->x;
        destination[2] = actor->z;
        break;
    case 1:
        destination[0] = actor->x;
        destination[2] = 0;
        break;
    case 2:
        destination[0] = target->x;
        destination[2] = target->z;
        break;
    case 3:
        destination[0] = target->x;
        destination[2] = 0;
        break;
    case 4:
        destination[0] = 0;
        destination[2] = 0;
        break;
    default:
        destination[0] = current[0];
        destination[2] = current[2];
        break;
    }
    for (index = 0; index < 3; index++) {
        delta[index] = Func_080022ec(destination[index] - current[index], 0x28);
    }

    for (frame = 0; frame < duration; frame++) {
        void *camera = CAMERA_080DE2F8;

        if (frame > 0x4b) REG_BLDALPHA_080DE2F8 = (0xa8 - frame * 2) | 0x1000;
        if (frame == 8) Func_080f9010(0xd4);
        Func_080049ac();
        Func_080051d8(camera, (u8 *)camera + 0x0c);
        if ((u32)(frame - 6) <= 0x27) {
            for (index = 0; index < 3; index++) current[index] += delta[index];
        }
        Func_08004cb4(current);
        if (frame == 0) Func_080d6888(context->target_actor, 7, -1, -1, 0);
        if (frame == 0x18) Func_080d6888(context->target_actor, 0, -1, -1, 0);

        for (index = 0; index < 0x20; index++) {
            struct Particle_080de2f8 *particle = &MAIN_PARTICLES_080DE2F8[index];

            if (frame >= index / 8 && particle->timer == 0) {
                s32 rotation = (frame << 8) + index * (frame << 5);
                s32 reverse = -rotation;
                s32 depth;
                s32 size;

                Func_080049e8();
                switch (index & 3) {
                case 0: Func_08004c1c(rotation); break;
                case 1: Func_08004bd4(reverse); break;
                case 2: Func_08004c6c(reverse); break;
                case 3: Func_08004bd4(reverse); Func_08004c6c(reverse); break;
                }
                Func_080e3944((s32)particle, projected);
                projected[0] >>= 1;
                Func_08004a5c();
                depth = projected[2];
                if (depth <= 0xf9) depth = 0xfa;
                if (depth > 0x27a) depth = 0x27a;
                projected[2] = depth;
                size = 8 - (depth - 0xfa) / 0x40;
                render_b(render_target,
                         sprite_sheet + ParticleTiles_080de2f8[size - 1],
                         projected[0] - size / 2,
                         projected[1] - size,
                         size,
                         size * 2);
                Func_080e38b8(particle, 0x3c, 0);

                if (frame >= index / 8 + 0x18) {
                    s32 pull_x = -particle->x >> 7;
                    s32 pull_z = -particle->z >> 7;

                    particle->velocity_x += pull_x;
                    particle->velocity_y += -particle->y >> 7;
                    particle->velocity_z += pull_z;
                    particle->velocity_x = ScaleBy64_080de2f8(particle->velocity_x);
                    particle->velocity_y = ScaleBy64_080de2f8(particle->velocity_y);
                    particle->velocity_z = ScaleBy64_080de2f8(particle->velocity_z);
                    if ((u32)(pull_x + 0x7ff) <= 0xffe &&
                        (u32)(pull_z + 0x7ff) <= 0xffe) {
                        particle->timer = -1;
                    }
                }
            }
        }

        if ((u32)(frame - 0x36) <= 0x0f) {
            point[0] = Func_08002322(frame << 10) * 4;
            point[1] = 0;
            point[2] = 0;
            Func_080e3944((s32)point, projected);
            *marker_x = projected[0];
            *marker_y = projected[1];
            projected[0] >>= 1;
            render_a(render_target, work,
                     projected[0] - 0x0a, projected[1] - 0x14,
                     0x14, 0x28);
        }

        if (frame == 0x40) {
            for (index = 0; index < 0x40; index++) {
                struct Particle_080de2f8 *particle = &work->burst_particles[index];
                u16 angle = Func_08004458();
                s32 speed = (Func_08004458() & 0xff) + 0x80;

                particle->x = *marker_x << 15;
                particle->y = *marker_y << 16;
                particle->velocity_x = speed * Func_08002322(angle) >> 6;
                particle->velocity_y = speed * Func_0800231c(angle) >> 5;
                particle->timer = (Func_08004458() & 0x0f) + 8;
            }
        }
        if (frame > 0x3f) {
            for (index = 0; index < 0x40; index++) {
                struct Particle_080de2f8 *particle = &work->burst_particles[index];

                if (particle->timer >= 0) {
                    s32 size = particle->timer / 8 + 2;

                    render_a(render_target,
                             sprite_sheet + ParticleTiles_080de2f8[size - 1],
                             (s16)(particle->x >> 16) - size / 2,
                             (s16)(particle->y >> 16) - size,
                             size,
                             size * 2);
                    Func_080e3908(particle, 0x3c, 0);
                    particle->timer--;
                }
            }
        }

        work->frame_ready = 1;
        Func_080030f8(1);
    }

    Func_08004278((void *)0x080cd261);
    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    Func_08004278((void *)0x080cd4b5);
    IWRAM_CLEAR_080DE2F8((void *)0x06004000, 0x4000);
    result = IWRAM_CLEAR_080DE2F8((void *)render_target, 0x4000);
    REG_BLDALPHA_080DE2F8 = 0x1010;
    return result;
}
