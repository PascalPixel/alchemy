#include "types.h"

typedef void (*Transfer_080d4ce8)(void *, const void *, u32);
typedef void (*Renderer_080d4ce8)(
    void *, const void *, s32, s32, s32, s32);

struct Scene_080d4ce8 {
    s32 unknown_00;
    s32 direction;
    u8 unknown_08[12];
    s32 object_count;
    s32 variant;
    u8 unknown_1c[8];
    s16 object_ids[1];
};

struct Point_080d4ce8 {
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_0c[16];
};

struct Object_080d4ce8 {
    u8 unknown_00[8];
    s32 x;
};

struct ObjectContext_080d4ce8 {
    struct Object_080d4ce8 *object;
};

struct Particle_080d4ce8 {
    s32 x;
    s32 y;
    s32 velocity_x;
    s32 unknown_0c;
    s32 velocity_y;
    s32 unknown_14;
    s32 timer;
};

s32 Func_080022fc(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_080b5088(s32, s32);
struct ObjectContext_080d4ce8 *Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(void *, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e3944(const s32 *, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080d4ce8(struct Scene_080d4ce8 *scene)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    u8 *effect_tiles = (u8 *)runtime_header[2];
    Renderer_080d4ce8 renderers[2];
    Transfer_080d4ce8 transfer = (Transfer_080d4ce8)0x03001388;
    struct Particle_080d4ce8 *particles =
        (struct Particle_080d4ce8 *)0x02010000;
    struct Point_080d4ce8 *points =
        (struct Point_080d4ce8 *)(runtime + 0x7080);
    s32 frame;
    s32 item_count;
    s32 index;

    *(struct Scene_080d4ce8 **)(runtime + 0x7828) = scene;
    Func_080cd594(1);
    *(volatile u16 *)0x04000052 = 0x1010;
    Func_080e0524((void *)0x85, runtime, 1, 1);
    Func_080e0524((void *)0x73, effect_tiles, 0, 0);

    if (scene->variant == 0) {
        transfer((void *)0x05000000, Func_08002f40(0x86), 0x80);
    } else if (scene->variant == 2) {
        transfer((void *)0x05000000, Func_08002f40(0x87), 0x80);
    }

    Func_080ed408(46, 7, 7, 3, 2);
    renderers[0] = (Renderer_080d4ce8)runtime_header[7];
    Func_080ed408(47, 7, 7, 7, 2);
    renderers[1] = (Renderer_080d4ce8)runtime_header[8];

    for (index = 0; index < 1024; index++)
        particles[index].timer = 0;

    {
        struct Object_080d4ce8 *object =
            Func_080b5098(scene->object_ids[0])->object;

        for (index = 0; index < 4; index++) {
            points[index].x =
                ((Func_08004458() & 15) + 72) << 16;
            points[index].y = 0;
            points[index].z =
                *(s8 *)(0x080ee29d + scene->variant * 4 + index) << 16;
            if (object->x < 0)
                points[index].x = -points[index].x;
        }
    }

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 50;
    Func_080041d8((const void *)0x080cd261, 0x480);

    item_count = *(u8 *)(0x080ee29a + scene->variant);
    for (frame = 0; frame < 96; frame++) {
        void *effect_context = *(void **)0x03001e80;

        if (scene->variant == 2 && frame <= 63) {
            volatile u16 *angle =
                (volatile u16 *)((u8 *)effect_context + 0x36);
            if (scene->direction == 0)
                *angle += 0xc0;
            else
                *angle -= 0xc0;
        }
        if (frame == 16)
            Func_080b50e8(0x86);

        Func_080049ac();
        Func_080051d8(effect_context, (u8 *)effect_context + 12);

        for (index = 0; index < item_count && frame <= 63; index++) {
            s32 projected[3];
            s32 timing = *(u8 *)(0x080ee2a9 + index);
            s32 created = 0;
            s32 slot;

            Func_080e3944(&points[index].x, projected);
            projected[0] /= 2;
            projected[1] -= 8;

            if (frame == timing)
                Func_080f9010(0x91);

            if (frame >= timing + 4) {
                s32 progress =
                    Func_080022fc(frame * 16 + index * 25, 104);
                const void *side_tiles =
                    runtime + ((frame & 1) ? 0x0dd0 : 0x10b4);

                renderers[frame & 1](render_context, runtime,
                         projected[0] - 17,
                         projected[1] - progress - 104,
                         34, 104);
                renderers[frame & 1](render_context, runtime,
                         projected[0] - 17,
                         projected[1] - progress,
                         34, progress);
                renderers[0](render_context, side_tiles,
                         projected[0] - 20, projected[1] - 24,
                         20, 37);
                renderers[1](render_context, side_tiles,
                         projected[0], projected[1] - 24,
                         20, 37);
            }

            if (frame == timing || frame >= timing + 16) {
                s32 wanted = frame == timing ? 200 : 4;

                for (slot = 0; slot < 1024 && created < wanted; slot++) {
                    struct Particle_080d4ce8 *particle = &particles[slot];

                    if (particle->timer == 0) {
                        s32 radius = (Func_08004458() & 0x3ff) + 32;
                        s32 particle_angle =
                            (Func_08004458() & 0x7fff) - 0x4000;

                        particle->x = projected[0] << 8;
                        particle->y = (projected[1] << 8) + 0x1000;
                        particle->velocity_x =
                            radius * Func_08002322(particle_angle) >> 15;
                        particle->velocity_y =
                            -(radius * Func_0800231c(particle_angle) * 2)
                            >> 15;
                        particle->timer =
                            (Func_08004458() & 7) +
                            (frame == timing ? 48 : 24);
                        created++;
                    }
                }
            }

            if (frame == timing) {
                s32 object_index;

                *(s32 *)(runtime + 0x77a8) = 2;
                for (object_index = 0;
                     object_index < scene->object_count;
                     object_index++) {
                    Func_080d6888(scene->object_ids[object_index],
                                  10, 5, object_index, 8);
                    Func_080b5088(scene->object_ids[object_index], 1);
                }
            }
        }

        for (index = 0; index < 1024; index++) {
            struct Particle_080d4ce8 *particle = &particles[index];

            if (particle->timer > 0) {
                s32 old_timer = particle->timer;
                s32 screen_x;
                s32 screen_y;

                particle->timer--;
                particle->x += particle->velocity_x;
                particle->y += particle->velocity_y;
                particle->velocity_x =
                    particle->velocity_x * 60 / 64;
                particle->velocity_y =
                    particle->velocity_y * 60 / 64 - 16;
                screen_y = particle->y / 256;

                if (screen_y > 120) {
                    particle->velocity_y = -particle->velocity_y / 2;
                } else if (particle->x >= 0 &&
                           (screen_x = particle->x >> 8) <= 126 &&
                           particle->y >= 0) {
                    s32 size = (old_timer - 17) / 8;
                    s32 height;
                    const void *source;

                    if (size <= 0)
                        size = 1;
                    height = size * 2;
                    source = effect_tiles +
                        *(u16 *)(0x080ede48 + height - 2);
                    renderers[index & 1](
                        render_context,
                        source,
                        screen_x - size / 2,
                        screen_y - size,
                        size,
                        height);
                }
            }
        }

        Func_080e155c(16, 16);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_08004278((const void *)0x080cd261);
    Func_080cdbc0();
}
