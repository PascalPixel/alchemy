#include "types.h"

struct Particle_080d33c0 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 timer;
};

typedef void (*Transfer_080d33c0)(
    void *destination,
    const void *source,
    u32 size);
typedef void (*Renderer_080d33c0)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

void *Func_08002f40(s32 resource);
s32 Func_080022fc(s32 value, s32 divisor);
void Func_08002dd8(s32 resource);
void Func_080030f8(s32 frames);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(const void *source);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(void *object, void *position);
void Func_08005340(const void *source, void *destination);
s32 Func_080b5070(s32 id);
void Func_080b5088(s32 id, s32 arg1);
u8 **Func_080b5098(s32 id);
void Func_080b50e8(s32 sound);
void Func_080cd52c(void);
void Func_080cd594(s32 mode);
void Func_080cdbc0(void);
void Func_080d6888(s32 id, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080e155c(s32 arg0, s32 arg1);
void Func_080e3944(struct Particle_080d33c0 *particle, s32 *point);
void Func_080ed408(s32 resource, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080f9010(s32 sound);

void Func_080d33c0(void *argument)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    Transfer_080d33c0 transfer = (Transfer_080d33c0)0x03001388;
    Renderer_080d33c0 renderers[2];
    struct Particle_080d33c0 *particles =
        (struct Particle_080d33c0 *)(runtime + 0x7080);
    u8 *finished = (u8 *)0x02010000;
    const u8 *effect_timing = (const u8 *)0x080ee1c4;
    s32 effect = *(s32 *)((u8 *)argument + 24);
    s32 point[3];
    s32 frame;
    s32 index;
    void *palette;

    *(void **)(runtime + 0x7828) = argument;
    Func_080cd594(1);
    palette = Func_08002f40(0xcd);
    transfer((void *)0x05000000, palette, 0x80);
    Func_08005340((u8 *)palette + 0x80, runtime);

    Func_080ed408(46, 7, 7, 3, 2);
    renderers[0] = (Renderer_080d33c0)runtime_header[7];
    Func_080ed408(47, 7, 7, 15, 2);
    renderers[1] = (Renderer_080d33c0)runtime_header[8];
    *(volatile u16 *)0x04000052 = 0x0f0f;

    {
        s32 id = *(s32 *)((u8 *)argument + 8);
        u8 *actor = *Func_080b5098(id);
        s32 initial_y = *(s32 *)(actor + 12) + Func_080b5070(id);

        for (index = 0; index < 30; index++) {
            struct Particle_080d33c0 *particle = &particles[index];

            particle->x = *(s32 *)(actor + 8);
            particle->y = initial_y;
            particle->z = *(s32 *)(actor + 16);
            particle->velocity_x =
                (s32)((u32)((Func_08004458() & 255) - 127) << 11);
            particle->velocity_y =
                (s32)((u32)((Func_08004458() & 127) - 16) << 10);
            particle->velocity_z =
                (s32)((u32)((Func_08004458() & 255) - 127) << 11);
            particle->timer = -1;
            finished[index] = 0;
        }
    }

    {
        s32 count = *(s32 *)((u8 *)argument + 20);
        struct Particle_080d33c0 *targets =
            (struct Particle_080d33c0 *)(runtime + 0x7400);

        for (index = 0; index < count; index++) {
            s32 id = *(s16 *)((u8 *)argument + 36 + index * 2);
            u8 *object = *Func_080b5098(id);

            targets[index].x = *(s32 *)(object + 8);
            targets[index].y = 0;
            targets[index].z = *(s32 *)(object + 16);
        }
    }

    *(s32 *)(runtime + 0x77ac) = 0;
    *(s32 *)(runtime + 0x77b0) = 0;
    Func_080041d8((const void *)0x080d6505, 0x480);
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080f9010(164);

    for (frame = 0; frame < effect_timing[effect * 2 + 1]; frame++) {
        void *camera = *(void **)0x03001e80;
        s32 particle_count = effect_timing[effect * 2];

        *(s32 *)(runtime + 0x77ac) =
            (u32)(frame - 17) <= 46 ? 384 : 0;

        if (frame == effect_timing[effect * 2 + 1] - 16)
            Func_080b50e8(132);

        Func_080049ac();
        Func_080051d8(camera, (u8 *)camera + 12);

        for (index = 0; index < particle_count; index++) {
            struct Particle_080d33c0 *particle = &particles[index];
            s32 delay = index * 2;

            if (frame > delay && finished[index] == 0) {
                Func_080e3944(particle, point);
                point[0] /= 2;
                if (point[2] <= 159)
                    point[2] = 160;
                if (point[2] > 0x31f)
                    point[2] = 0x31f;

                renderers[0](
                    render_context,
                    runtime + 0xc00,
                    point[0] - 6,
                    point[1] - 12,
                    12,
                    24);
                particle->x += particle->velocity_x;
                particle->y += particle->velocity_y;
                particle->z += particle->velocity_z;
            }

            if (frame > delay + 48 && finished[index] == 0) {
                s32 count = *(s32 *)((u8 *)argument + 20);
                s32 target_index = Func_080022fc(index, count);
                struct Particle_080d33c0 *target =
                    &((struct Particle_080d33c0 *)(runtime + 0x7400))[
                        target_index];

                particle->velocity_x +=
                    (target->x - particle->x) >> 9;
                particle->velocity_y +=
                    (target->y - particle->y) >> 9;
                particle->velocity_z +=
                    (target->z - particle->z) >> 9;

                if (frame < delay + 85) {
                    particle->velocity_x =
                        particle->velocity_x * 60 / 64;
                    particle->velocity_y =
                        particle->velocity_y * 60 / 64;
                    particle->velocity_z =
                        particle->velocity_z * 60 / 64;
                }

                if (particle->y < 0) {
                    s32 id;

                    finished[index] = 1;
                    particle->timer = 0;
                    particle->x = point[0];
                    particle->y =
                        point[1] + (Func_08004458() & 31) - 16;
                    target_index = Func_080022fc(index, count);
                    id = *(s16 *)(
                        (u8 *)argument + 36 + target_index * 2);
                    Func_080d6888(id, 7, 5, target_index, 4);
                    target_index = Func_080022fc(index, count);
                    id = *(s16 *)(
                        (u8 *)argument + 36 + target_index * 2);
                    Func_080b5088(id, 0);
                    *(s32 *)(runtime + 0x77a8) = 4;
                    Func_080f9010(132);
                }
            }

            if ((u32)particle->timer <= 15) {
                s32 tile =
                    Func_080022fc(particle->timer / 2, 3);
                const void *source = runtime + (tile << 10);

                renderers[0](
                    render_context,
                    source,
                    particle->x - 16,
                    particle->y - 56,
                    16,
                    64);
                tile = Func_080022fc(particle->timer / 2, 3);
                source = runtime + (tile << 10);
                renderers[1](
                    render_context,
                    source,
                    particle->x,
                    particle->y - 56,
                    64,
                    16);
                particle->timer++;
            }
        }

        Func_080e155c(effect * 2 + 2, effect * 2 + 2);
        if (*(s32 *)(runtime + 0x77b0) == 0)
            *(s32 *)(runtime + 0x77b0) = 1;
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080d6505);
    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
