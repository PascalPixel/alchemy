#include "layout_guard.h"
#include "types.h"

#define REG_BLDALPHA_080CF8E0 (*(volatile u16 *)0x04000052)
#define PRIMARY_PARTICLES_080CF8E0 ((struct Particle_080cf8e0 *)0x02010000)
#define IMPACT_PARTICLES_080CF8E0 ((struct Particle_080cf8e0 *)0x02011c00)
#define CAMERA_080CF8E0 (*(void **)0x03001e80)

typedef void *(*WordCopy_080cf8e0)(void *, const void *, s32);
typedef void (*Renderer_080cf8e0)(s32, void *, s32, s32, s32, s32);
#define WORD_COPY_080CF8E0 ((WordCopy_080cf8e0)0x03001388)
#define RENDERER_A_080CF8E0 (*(Renderer_080cf8e0 *)0x03001f08)
#define RENDERER_B_080CF8E0 (*(Renderer_080cf8e0 *)0x03001f0c)

struct Particle_080cf8e0 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 timer;
};

struct ActorEffect_080cf8e0 {
    u8 reserved00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct SceneContext_080cf8e0 {
    u8 reserved00[4];
    s32 side;
    s32 target_actor;
    u8 reserved0c[8];
    s32 actor_count;
    u8 reserved18[0x0c];
    s16 actor_ids[1];
};

struct SceneWork_080cf8e0 {
    u8 projectile_graphics[0x3e80];
    u8 impact_graphics[0x3900];
    s32 scene_phase;
    s32 scene_timer;
    u8 reserved7788[0x9c];
    s32 frame_ready;
    struct SceneContext_080cf8e0 *context;
};

struct RuntimeHeader_080cf8e0 {
    struct SceneWork_080cf8e0 *work;
    s32 render_target;
};

LAYOUT_SIZE_GUARD(Particle080cf8e0_Size, struct Particle_080cf8e0, 0x1c);
LAYOUT_OFFSET_GUARD(Particle080cf8e0_Timer, struct Particle_080cf8e0, timer, 0x18);
LAYOUT_OFFSET_GUARD(SceneContext080cf8e0_Side, struct SceneContext_080cf8e0, side, 0x04);
LAYOUT_OFFSET_GUARD(SceneContext080cf8e0_Target, struct SceneContext_080cf8e0, target_actor, 0x08);
LAYOUT_OFFSET_GUARD(SceneContext080cf8e0_Count, struct SceneContext_080cf8e0, actor_count, 0x14);
LAYOUT_OFFSET_GUARD(SceneContext080cf8e0_Ids, struct SceneContext_080cf8e0, actor_ids, 0x24);
LAYOUT_OFFSET_GUARD(SceneWork080cf8e0_ImpactGraphics, struct SceneWork_080cf8e0, impact_graphics, 0x3e80);
LAYOUT_OFFSET_GUARD(SceneWork080cf8e0_Phase, struct SceneWork_080cf8e0, scene_phase, 0x7780);
LAYOUT_OFFSET_GUARD(SceneWork080cf8e0_FrameReady, struct SceneWork_080cf8e0, frame_ready, 0x7824);
LAYOUT_OFFSET_GUARD(SceneWork080cf8e0_Context, struct SceneWork_080cf8e0, context, 0x7828);

static const struct RuntimeHeader_080cf8e0 *const Runtime_080cf8e0 =
    (const struct RuntimeHeader_080cf8e0 *)0x03001eec;
static const s8 *const VariantFlags_080cf8e0 = (const s8 *)0x080ee0b6;
static const u8 *const ImpactWidths_080cf8e0 = (const u8 *)0x080ee0c4;
static const u8 *const ImpactHeights_080cf8e0 = (const u8 *)0x080ee0d6;
static const u16 *const ImpactOffsets_080cf8e0 = (const u16 *)0x080ee0e8;

void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(u32);
s32 Func_080022ec(s32, s32);
u32 Func_08004458(void);
void Func_080041d8(s32, s32);
void Func_08004278(void *);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_08005340(s32, s32);
struct ActorEffect_080cf8e0 **Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
s32 Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e155c(s32, u32);
void Func_080e38b8(struct Particle_080cf8e0 *, s32, s32);
void Func_080e3944(s32, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

static s32 PaletteResource_080cf8e0(s32 variant) {
    switch (variant) {
    case 0: return 0x9f;
    case 1: return 0x59;
    case 2: return 0xa0;
    case 4: return 0xbf;
    case 6: return 0x8d;
    default: return 0x77;
    }
}

/* Run the per-character projectile streams and their secondary impacts. */
s32 Func_080cf8e0(struct SceneContext_080cf8e0 *context, s32 variant) {
    struct SceneWork_080cf8e0 *work = Runtime_080cf8e0->work;
    s32 render_target = Runtime_080cf8e0->render_target;
    void *camera = CAMERA_080CF8E0;
    s32 alternate = VariantFlags_080cf8e0[variant * 2];
    Renderer_080cf8e0 renderers[2];
    struct ActorEffect_080cf8e0 *target;
    s32 position[3];
    s32 actor;
    s32 index;
    s32 frame;
    s32 palette;

    work->context = context;
    Func_080cd594(1);
    REG_BLDALPHA_080CF8E0 = 0x1010;

    palette = (s32)Func_08002f40(0xbf);
    WORD_COPY_080CF8E0((void *)0x05000000, (void *)palette, 0x80);
    Func_08005340(palette + 0x80, (s32)work);
    palette = (s32)Func_08002f40(0x9e);
    WORD_COPY_080CF8E0((void *)0x05000000, (void *)palette, 0x80);
    Func_08005340(palette + 0x80, (s32)&work->impact_graphics[0]);
    WORD_COPY_080CF8E0(
        (void *)0x05000000,
        Func_08002f40(PaletteResource_080cf8e0(variant)),
        0x80);

    for (index = 0; index < 0x400; index++) {
        PRIMARY_PARTICLES_080CF8E0[index].timer = -1;
    }
    target = *Func_080b5098(context->target_actor);
    for (actor = 0; actor < context->actor_count; actor++) {
        struct ActorEffect_080cf8e0 *source =
            *Func_080b5098(context->actor_ids[actor]);

        for (index = 0; index < 0x10; index++) {
            struct Particle_080cf8e0 *particle =
                &PRIMARY_PARTICLES_080CF8E0[actor * 0x10 + index];

            particle->x = target->x;
            particle->y = 0x40000;
            particle->z = target->z;
            particle->velocity_x = (source->x - target->x) >> 4;
            particle->velocity_y = 0x40000;
            particle->velocity_z = (source->z - target->z) >> 4;
            particle->timer = 0;
        }
    }
    for (index = 0; index < 0x100; index++) {
        IMPACT_PARTICLES_080CF8E0[index].timer = -1;
    }

    if (context->side == 0) {
        Func_080ed408(0x2e, 7, 7, 3, 2);
        renderers[0] = RENDERER_A_080CF8E0;
        if (alternate == 0) {
            Func_080ed408(0x2f, 7, 7, 3, 2);
        } else {
            Func_080ed408(0x2f, 7, 7, 7, 2);
        }
    } else {
        Func_080ed408(0x2e, 7, 7, 7, 2);
        renderers[0] = RENDERER_A_080CF8E0;
        if (alternate == 0) {
            Func_080ed408(0x2f, 7, 7, 3, 3);
        } else {
            Func_080ed408(0x2f, 7, 7, 3, 2);
        }
    }
    renderers[1] = RENDERER_B_080CF8E0;

    work->scene_phase = 2;
    work->scene_timer = 0x4b;
    Func_080041d8(0x080cd261, 0x480);
    Func_080f9010(0x67);

    for (frame = 0;
         frame < context->actor_count * 8 + (alternate == 0 ? 0x48 : 0x38);
         frame++) {
        Func_080049ac();
        Func_080051d8(camera, (u8 *)camera + 0x0c);

        for (actor = 0; actor < context->actor_count; actor++) {
            s32 start = actor * 8;
            struct Particle_080cf8e0 *projectile =
                &PRIMARY_PARTICLES_080CF8E0[actor * 0x10];

            if (frame >= start) {
                if (frame == start + 0x11) {
                    Func_080d6888(context->actor_ids[actor], 7, 5, actor, 0x10);
                    Func_080b50e8(0x85);
                }
                if (projectile->timer >= 0) {
                    s32 image = Func_080022ec(frame - start, 3);

                    if (image > 9) image = 9;
                    Func_080e3944((s32)projectile, position);
                    position[0] >>= 1;
                    if (image > 4) {
                        renderers[0](render_target, (u8 *)work + image * 0x300,
                                     position[0] - 0x10, position[1] - 0x0c,
                                     0x20, 0x18);
                    } else {
                        renderers[0](render_target, (u8 *)work + image * 0x300,
                                     position[0] - 0x0c, position[1] - 0x10,
                                     0x18, 0x20);
                    }
                    if (projectile->timer == 0) {
                        Func_080e38b8(projectile, 0x3f, -0x8000);
                    }
                    if (projectile->y < 0) {
                        s32 impact_count = alternate == 0 ? 4 : 0x10;

                        projectile->y = 0;
                        projectile->timer = 1;
                        for (index = 0; index < impact_count; index++) {
                            struct Particle_080cf8e0 *impact =
                                &IMPACT_PARTICLES_080CF8E0[actor * 0x20 + index];

                            impact->x = projectile->x;
                            impact->y = projectile->y;
                            impact->z = projectile->z;
                            if (alternate == 0) {
                                impact->velocity_x =
                                    ((Func_08004458() & 0x3f) - 0x20) << 11;
                                impact->velocity_y = 0;
                                impact->velocity_z =
                                    ((Func_08004458() & 0x3f) - 0x20) << 11;
                            } else {
                                impact->velocity_x =
                                    ((Func_08004458() & 0x3f) - 0x20) << 13;
                                impact->velocity_y =
                                    ((Func_08004458() & 0x1f) + 0x20) << 12;
                                impact->velocity_z =
                                    ((Func_08004458() & 0x3f) - 0x20) << 13;
                            }
                            impact->timer = 0;
                        }
                    }
                }
            }
        }

        for (index = 0; index < 0x100; index++) {
            struct Particle_080cf8e0 *impact = &IMPACT_PARTICLES_080CF8E0[index];

            if ((u32)impact->timer <= 0x2c && impact->y >= 0) {
                Func_080e3944((s32)impact, position);
                position[0] >>= 1;
                if (alternate == 0) {
                    s32 image = impact->timer;
                    if (image < 0) image += 7;
                    renderers[1](render_target,
                                 &work->impact_graphics[(image >> 3) * 0x480],
                                 position[0] - 0x0c, position[1] - 0x18,
                                 0x18, 0x30);
                } else {
                    s32 image = Func_080022ec(impact->timer, 5);
                    s32 renderer = context->side;
                    u8 width;
                    u8 height;

                    if (index & 1) image += 9;
                    if (impact->velocity_x > 0) renderer ^= 1;
                    width = ImpactWidths_080cf8e0[image];
                    height = ImpactHeights_080cf8e0[image];
                    renderers[renderer](
                        render_target,
                        &work->projectile_graphics[0x1e00 +
                            ImpactOffsets_080cf8e0[image]],
                        position[0] - width / 2,
                        position[1] - height / 2,
                        width,
                        height);
                }
                Func_080e38b8(impact, 0x3e, alternate == 0 ? 0x800 : -0x8000);
                impact->timer++;
            }
        }

        Func_080e155c(2, 2);
        Func_080cd52c();
        work->frame_ready = 1;
        Func_080030f8(1);
    }

    Func_08004278((void *)0x080cd261);
    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    return Func_080cdbc0();
}
