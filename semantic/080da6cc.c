#include "types.h"

typedef void (*Renderer_080da6cc)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Particle_080da6cc {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 active;
};

#define MUL_SHIFT_080DA6CC(left, right, shift) \
    ((s32)((u32)(left) * (u32)(right)) >> (shift))

#define DAMP_080DA6CC(value) \
    ((s32)((u32)(value) * 60) / 64)

s32 Func_080022fc(s32 value, s32 modulus);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_08002dd8(s32 arg0);
void Func_080030f8(s32 arg0);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(const void *address);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(void *arg0, void *arg1);
void Func_080b5028(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 Func_080b5070(s32 arg0);
void *Func_080b5098(s32 arg0);
void Func_080b50e8(s32 arg0);
void Func_080cd52c(void);
void Func_080cd594(s32 arg0);
void Func_080cdbc0(void);
void Func_080d6888(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080e0524(s32 arg0, void *destination, s32 arg2, s32 arg3);
void Func_080e155c(s32 arg0, s32 arg1);
void Func_080e3944(const struct Particle_080da6cc *input, s32 *output);
void Func_080ed408(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080f9010(s32 arg0);

void Func_080da6cc(void *argument)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    void *graphics = (void *)runtime_header[2];
    void *effect_context = *(void **)0x03001e80;
    Renderer_080da6cc renderers[2];
    struct Particle_080da6cc *particles =
        (struct Particle_080da6cc *)0x02010000;
    struct Particle_080da6cc *sprites =
        (struct Particle_080da6cc *)(runtime + 0x7080);
    void *primary;
    s32 primary_id;
    s32 center_y;
    s32 frame;
    u32 i;

    *(void **)(runtime + 0x7828) = argument;
    if (*(s32 *)((u8 *)argument + 4) == 1)
        Func_080cd594(1);
    else
        Func_080cd594(0);

    Func_080e0524(0xb4, runtime, 1, 1);
    Func_080e0524(0x73, graphics, 0, 0);
    Func_080ed408(46, 7, 7, 3, 3);
    renderers[0] = (Renderer_080da6cc)runtime_header[7];
    Func_080ed408(47, 7, 7, 3, 2);
    renderers[1] = (Renderer_080da6cc)runtime_header[8];
    *(volatile u16 *)0x04000052 = 0x1010;

    primary_id = *(s32 *)(*(u8 **)(runtime + 0x7828) + 8);
    primary = *(void **)Func_080b5098(primary_id);
    center_y =
        *(s32 *)((u8 *)primary + 12) + Func_080b5070(primary_id);

    for (i = 0; i < 64; i++) {
        s32 angle = Func_08004458();
        s32 speed = (Func_08004458() & 0x7f) + 127;

        particles[i].velocity_x = MUL_SHIFT_080DA6CC(
            speed,
            Func_08002322(angle),
            6);
        particles[i].velocity_y =
            ((s32)(Func_08004458() & 0x7f) - 16) * 0x400;
        particles[i].velocity_z = MUL_SHIFT_080DA6CC(
            speed,
            Func_0800231c(angle),
            6);
        particles[i].x = *(s32 *)((u8 *)primary + 8);
        particles[i].y = center_y;
        particles[i].z = *(s32 *)((u8 *)primary + 16);
        particles[i].active = -1;
    }

    *(s32 *)(runtime + 0x77ac) = 0;
    *(s32 *)(runtime + 0x77b0) = 0;
    Func_080041d8((const void *)0x080d6505, 0x480);
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    frame = 0;
    do {
        s32 variant = *(s32 *)(*(u8 **)(runtime + 0x7828) + 24);
        s32 count = ((const u8 *)0x080eea41)[variant];

        if ((u32)(frame - 17) <= 62)
            *(s32 *)(runtime + 0x77ac) = 0x100;
        else
            *(s32 *)(runtime + 0x77ac) = 0;

        if (frame == (count / 2) + 108)
            Func_080b50e8(133);

        Func_080b5028(0, 0, 0, 100);
        Func_080049ac();
        Func_080051d8(effect_context, (u8 *)effect_context + 12);

        for (
            i = 0;
            i < ((const u8 *)0x080eea41)[
                *(s32 *)(*(u8 **)(runtime + 0x7828) + 24)];
            i++) {
            struct Particle_080da6cc *particle = &particles[i];
            s32 start_frame = i / 2;
            s32 end_frame = start_frame + 48;
            s32 projected[3];

            if (frame > start_frame && particle->active == -1) {
                s32 depth;
                s32 size;
                const void *source;
                Renderer_080da6cc renderer;

                Func_080e3944(particle, projected);
                projected[0] >>= 1;
                if (projected[2] < 160)
                    projected[2] = 160;
                if (projected[2] > 0x31f)
                    projected[2] = 0x31f;

                depth = (projected[2] - 160) / 64;
                size = 10 - depth;
                source = (u8 *)graphics +
                    ((const u16 *)0x080ede48)[size - 1];
                renderer = frame < end_frame ? renderers[1] : renderers[0];
                renderer(
                    render_context,
                    source,
                    projected[0] - (size / 2),
                    projected[1] - size,
                    size,
                    size * 2);

                particle->x += particle->velocity_x;
                particle->y += particle->velocity_y;
                particle->z += particle->velocity_z;
            }

            if (frame > end_frame && particle->active == -1) {
                s32 scene_count =
                    *(s32 *)(*(u8 **)(runtime + 0x7828) + 20);
                s32 object_index = Func_080022fc(i, scene_count);
                s32 object_id = *(s16 *)(
                    *(u8 **)(runtime + 0x7828) +
                    36 +
                    (object_index * 2));
                void *target = *(void **)Func_080b5098(object_id);

                particle->velocity_x +=
                    (*(s32 *)((u8 *)target + 8) - particle->x) >> 9;
                particle->velocity_y +=
                    (*(s32 *)((u8 *)target + 12) - particle->y) >> 9;
                particle->velocity_z +=
                    (*(s32 *)((u8 *)target + 16) - particle->z) >> 9;

                if (frame < start_frame + 85) {
                    particle->velocity_x =
                        DAMP_080DA6CC(particle->velocity_x);
                    particle->velocity_y =
                        DAMP_080DA6CC(particle->velocity_y);
                    particle->velocity_z =
                        DAMP_080DA6CC(particle->velocity_z);
                }

                if (particle->y < 0) {
                    particle->active = 0;
                    particle->x = projected[0];
                    particle->y = projected[1];
                    Func_080f9010(136);

                    scene_count =
                        *(s32 *)(*(u8 **)(runtime + 0x7828) + 20);
                    object_index = Func_080022fc(i, scene_count);
                    object_id = *(s16 *)(
                        *(u8 **)(runtime + 0x7828) +
                        36 +
                        (object_index * 2));
                    Func_080d6888(object_id, 10, 5, object_index, 4);
                    *(s32 *)(runtime + 0x77a8) = 2;
                }
            }
        }

        for (
            i = 0;
            i < ((const u8 *)0x080eea41)[
                *(s32 *)(*(u8 **)(runtime + 0x7828) + 24)];
            i++) {
            struct Particle_080da6cc *sprite = &sprites[i];

            if ((u32)sprite->active <= 11) {
                s32 phase = sprite->active / 2;
                s32 width = ((const u8 *)0x080eea44)[phase];
                s32 height = ((const u8 *)0x080eea4a)[phase];
                const void *source =
                    runtime + ((const u16 *)0x080eea56)[phase];

                renderers[1](
                    render_context,
                    source,
                    sprite->x - (width / 2),
                    sprite->y + ((const u8 *)0x080eea50)[phase] - 56,
                    width,
                    height);
                sprite->active++;
            }
        }

        if (*(s32 *)(runtime + 0x77b0) == 0)
            *(s32 *)(runtime + 0x77b0) = 1;

        Func_080e155c(8, 8);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
        frame++;
    } while (
        frame !=
        (((const u8 *)0x080eea41)[
             *(s32 *)(*(u8 **)(runtime + 0x7828) + 24)] /
         2) +
            132);

    Func_08004278((const void *)0x080cd261);
    Func_08004278((const void *)0x080d6505);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
