#include "layout_guard.h"
#include "types.h"

struct Scene_080e01e4 {
    u8 unknown_00[0x14];
    s32 object_count;
    u8 unknown_18[0x0c];
    s16 object_ids[1];
};

struct Emitter_080e01e4 {
    s32 x;
    s32 y;
    u8 unknown_08[0x10];
    s32 unused_timer;
};

struct Particle_080e01e4 {
    s32 x;
    s32 y;
    s32 unknown_08;
    s32 velocity_x;
    s32 velocity_y;
    s32 unknown_14;
    s32 timer;
};

typedef void (*Renderer_080e01e4)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

LAYOUT_OFFSET_GUARD(
    Scene080e01e4_ObjectCount,
    struct Scene_080e01e4,
    object_count,
    0x14);
LAYOUT_OFFSET_GUARD(
    Scene080e01e4_ObjectIds,
    struct Scene_080e01e4,
    object_ids,
    0x24);
LAYOUT_SIZE_GUARD(
    Emitter080e01e4,
    struct Emitter_080e01e4,
    0x1c);
LAYOUT_SIZE_GUARD(
    Particle080e01e4,
    struct Particle_080e01e4,
    0x1c);

s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080b5088(s16, s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080cef64(s32, Renderer_080e01e4 *);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e3908(struct Particle_080e01e4 *, s32, s32);
void Func_080f9010(s32);

void Func_080e01e4(struct Scene_080e01e4 *scene)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    u8 *effect_tiles = (u8 *)runtime_header[2];
    Renderer_080e01e4 renderers[2];
    struct Emitter_080e01e4 *emitters =
        (struct Emitter_080e01e4 *)(runtime + 0x7080);
    struct Particle_080e01e4 *particles =
        (struct Particle_080e01e4 *)0x02010000;
    s32 angle = 0x8000;
    u32 frame;
    u32 index;

    *(struct Scene_080e01e4 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    *(volatile u16 *)0x04000052 = 0x1010;
    Func_080cef64(0, renderers);
    Func_080e0524(0x73, effect_tiles, 0, 0);
    Func_080e0524(0x90, runtime, 1, 1);
    Func_080e0524(0x89, runtime + 0x320, 1, 0);

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (index = 0; index < 32; index++) {
        emitters[index].x = (Func_08004458() & 0x3f) + 64;
        emitters[index].y = (Func_08004458() & 0x3f) - 80;
    }

    for (index = 0; index < 512; index++)
        particles[index].timer = -1;

    Func_080f9010(0xab);

    for (frame = 0; frame < 96; frame++, angle -= 0x800) {
        if (frame == 56)
            Func_080b50e8(0x85);

        if (frame <= 95) {
            s32 radius = 64 - frame * 2;

            renderers[0](
                render_context,
                runtime,
                (radius * Func_08002322(angle) >> 17) + 86,
                (radius * Func_0800231c(angle) >> 16) + 28,
                20,
                40);
        }

        for (index = 0; index < 8; index++) {
            struct Emitter_080e01e4 *emitter = &emitters[index];

            if (frame >= index * 4 + 8 && emitter->y <= 95) {
                u32 particle_index;

                renderers[0](
                    render_context,
                    runtime + 0x320,
                    emitter->x - 20,
                    emitter->y - 32,
                    40,
                    64);
                emitter->x -= 6;
                emitter->y += 12;

                if (emitter->y > 95) {
                    struct Particle_080e01e4 *group =
                        &particles[index * 32];

                    for (particle_index = 0;
                         particle_index < 32;
                         particle_index++) {
                        struct Particle_080e01e4 *particle =
                            &group[particle_index];
                        s32 particle_angle =
                            Func_08004458() & 0xffff;
                        s32 magnitude =
                            (Func_08004458() & 0x1ff) + 0x100;

                        particle->x = emitter->x << 16;
                        particle->y = emitter->y << 16;
                        particle->velocity_x =
                            magnitude *
                            Func_08002322(particle_angle) >> 7;
                        particle->velocity_y =
                            magnitude *
                            Func_0800231c(particle_angle) >> 6;
                        particle->timer =
                            (Func_08004458() & 0x0f) + 32;
                    }

                    Func_080f9010(0x85);
                    *(s32 *)(runtime + 0x77a8) = 4;
                    for (particle_index = 0;
                         particle_index < scene->object_count;
                         particle_index++) {
                        s16 object_id =
                            scene->object_ids[particle_index];

                        Func_080d6888(
                            object_id,
                            7,
                            5,
                            particle_index,
                            6);
                        Func_080b5088(object_id, 6);
                    }
                }
            }
        }

        for (index = 0; index < 512; index++) {
            struct Particle_080e01e4 *particle = &particles[index];

            if (particle->timer != -1) {
                s32 life = particle->timer;
                s32 size;

                if (life < 0)
                    life += 15;
                size = (life >> 4) + 1;
                renderers[0](
                    render_context,
                    effect_tiles +
                        ((const u16 *)0x080ede48)[size - 1],
                    (s16)(particle->x >> 16) -
                        ((size + ((u32)size >> 31)) >> 1),
                    (s16)(particle->y >> 16) - size,
                    size,
                    size * 2);
                Func_080e3908(particle, 62, 0x2000);
                particle->timer--;
            }
        }

        Func_080e155c(4, 4);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
