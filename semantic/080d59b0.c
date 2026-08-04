#include "types.h"

typedef void (*Renderer_080d59b0)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Particle_080d59b0 {
    s32 x;
    s32 y;
    s32 speed;
    s32 unused0c;
    s32 velocity;
    s32 unused14;
    s32 phase;
};

void Func_08002dd8(s32 arg0);
void Func_080030f8(s32 arg0);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(volatile void *address);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_08004cb4(s32 *position);
void Func_080051d8(void *arg0, void *arg1);
void *Func_080b5098(s32 arg0);
void Func_080b50e8(s32 arg0);
void Func_080cd594(s32 arg0);
void Func_080cdbc0(void);
void Func_080d6888(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080e0524(volatile void *address, void *runtime, s32 arg2, s32 arg3);
void Func_080ed408(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080f9010(s32 arg0);

void Func_080d59b0(void *argument)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    Renderer_080d59b0 renderers[2];
    struct Particle_080d59b0 *particles =
        (struct Particle_080d59b0 *)0x02010000;
    s32 frame;
    s32 i;

    *(void **)(runtime + 0x7828) = argument;
    Func_080cd594(0);
    *(volatile u16 *)0x04000020 = 0x100;
    *(volatile u16 *)0x04000050 = 0;
    Func_080e0524((volatile void *)0x04000020, runtime, 1, 1);

    Func_080ed408(46, 7, 7, 3, 1);
    renderers[0] = (Renderer_080d59b0)runtime_header[7];
    Func_080ed408(47, 7, 7, 15, 1);
    renderers[1] = (Renderer_080d59b0)runtime_header[8];

    for (i = 0; i < 32; i++) {
        particles[i].x = (s32)((Func_08004458() & 63) + 32) << 16;
        particles[i].y = -0x200000;
        Func_08004458();
        particles[i].velocity = 0;
        particles[i].speed = Func_08004458() & 3;
        particles[i].phase = Func_08004458() & 0xff;
    }

    if (*(s32 *)((u8 *)argument + 4) == 1)
        *(volatile u32 *)0x04000028 = 0xffff9000;

    *(s32 *)(runtime + 0x7780) = 1;
    *(s32 *)(runtime + 0x7784) = 0;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080f9010(142);

    for (frame = 0; frame < 148; frame++) {
        void *effect_context = *(void **)0x03001e80;
        s32 count;

        if (frame == 80)
            Func_080b50e8(0);

        count = *(s32 *)(*(u8 **)(runtime + 0x7828) + 20);
        for (i = 0; i < count; i++) {
            s32 id =
                *(s16 *)(*(u8 **)(runtime + 0x7828) + 36 + (i * 2));
            void *object = *(void **)Func_080b5098(id);
            s32 position[3];

            Func_080049ac();
            Func_080051d8(effect_context, (u8 *)effect_context + 12);

            position[0] = *(s32 *)((u8 *)object + 8);
            position[1] = 0x280000;
            position[2] = *(s32 *)((u8 *)object + 16);
            Func_08004cb4(position);

            if (frame == (i * 16) + 64)
                Func_080d6888(id, 0, 5, -1, 0);
        }

        for (i = 0; i < 12; i++) {
            struct Particle_080d59b0 *particle = &particles[i];

            if (frame > i * 4 && particle->y <= 0x7fffff) {
                s32 phase = (particle->phase / 16) & 7;
                const void *source;

                if (phase <= 3) {
                    source = runtime + (phase * 0x400);
                    renderers[0](
                        render_context,
                        source,
                        (s16)((u32)particle->x >> 16) - 16,
                        (particle->y >> 16) - 16,
                        32,
                        32);
                } else {
                    source = runtime + (phase * 0x400) - 0x1000;
                    renderers[1](
                        render_context,
                        source,
                        (s16)((u32)particle->x >> 16) - 16,
                        (particle->y >> 16) - 16,
                        32,
                        32);
                }

                {
                    s32 velocity = particle->velocity;
                    s32 next_velocity = velocity + 0x4000;

                    particle->y += velocity;
                    particle->velocity = next_velocity;
                    particle->phase += particle->speed;

                    if (particle->y > 0x5c0000 && next_velocity == 0) {
                        particle->speed += 4;
                        particle->y = 0x5c0000;
                        particle->velocity = -(velocity - 0x1000) / 2;
                    }
                }
            }
        }

        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((volatile void *)0x04000028);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
