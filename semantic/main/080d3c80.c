#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

struct Particle_080d3c80 {
    s32 x;
    s32 y;
    s32 state;
    s32 velocity_x;
    s32 velocity_y;
    s32 unknown_14;
    s32 timer;
};

void Func_080cd594(s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080cef64(s32, u32 *);
void Func_080041d8(const void *, s32);
u32 Func_08004458(void);
void Func_080b50e8(s32);
s32 Func_080022fc(s32, s32);
s32 Func_080022ec(s32, s32);
void Func_080072f4(s32, const void *, s32, s32, s32, s32);
void Func_080e3908(void *, s32, s32);
void Func_080f9010(s32);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e155c(s32, s32);
void Func_080cd52c(void);
void Func_080030f8(s32);
void Func_08004278(const void *);
void Func_08002dd8(s32);
void Func_080cdbc0(void);

void Func_080d3c80(void *argument)
{
    u8 *runtime = *(u8 **)0x03001eec;
    s32 graphics = *(s32 *)0x03001ef0;
    u32 layer_sources[2];
    struct Particle_080d3c80 *emitters =
        (struct Particle_080d3c80 *)(runtime + 0x7080);
    u8 variant;
    u8 emitter_count;
    u8 duration;
    s32 frame;
    s32 emitter_index;

    PTR_AT(runtime, 0x7828) = argument;
    Func_080cd594(0);
    *(volatile u16 *)0x04000052 = 0x1010;
    Func_080e0524(0xcf, runtime, 1, 1);
    Func_080cef64(S32_AT(argument, 4), layer_sources);
    S32_AT(runtime, 0x7780) = 2;
    S32_AT(runtime, 0x7784) = 0x32;
    Func_080041d8((const void *)0x080cd261, 0x480);

    variant = U8_AT(argument, 0x18);
    emitter_count = U8_AT((void *)0x080ee1f5, variant * 2);
    duration = U8_AT((void *)0x080ee1f5, variant * 2 + 1);

    for (emitter_index = 0; emitter_index < emitter_count; emitter_index++) {
        struct Particle_080d3c80 *emitter = &emitters[emitter_index];
        s32 horizontal;
        s32 velocity;

        Func_08004458();
        emitter->y = (s32)0xffc00000;
        if (S32_AT(argument, 4) == 1) {
            horizontal = ((Func_08004458() & 0x1f) + 0x50) << 16;
            velocity = Func_08004458() & 0x3f;
        } else {
            horizontal = ((Func_08004458() & 0x1f) + 8) << 16;
            velocity = -(s32)(Func_08004458() & 0x3f);
        }
        emitter->velocity_x = velocity << 12;
        emitter->x = horizontal - emitter->velocity_x * 0x12;
        emitter->velocity_y = 0;
        emitter->state = 0;
        emitter->timer = emitter_index * 8;
    }

    for (frame = 0; frame < duration; frame++) {
        if (variant == 2 && frame <= 0x67) {
            u8 *object = *(u8 **)0x03001e80;
            s32 shift = frame <= 0x5f ? 0xc0 : 0x9c0 - frame * 0x18;
            if (S32_AT(argument, 4) == 0)
                U16_AT(object, 0x36) -= shift;
            else
                U16_AT(object, 0x36) += shift;
        }

        if (frame == duration - 0x50)
            Func_080b50e8(0x86);
        if (frame == duration - 8) {
            S32_AT(runtime, 0x7780) = 3;
            S32_AT(runtime, 0x7784) = 0x06060606;
        }

        if (frame <= duration - 8) {
            for (emitter_index = 0;
                 emitter_index < emitter_count;
                 emitter_index++) {
                struct Particle_080d3c80 *emitter =
                    &emitters[emitter_index];

                if (emitter->state == 1) {
                    struct Particle_080d3c80 *particle =
                        (struct Particle_080d3c80 *)
                            (0x02010000 + emitter_index * 0x1c0);
                    s32 particle_index;

                    for (particle_index = 0;
                         particle_index < 16;
                         particle_index++, particle++) {
                        s32 tile = Func_080022fc(particle_index, 5) * 3;
                        s32 animation = Func_080022fc(
                            Func_080022ec(particle->timer, 0x60), 3);
                        s32 image = tile + animation;
                        u8 width = U8_AT((void *)0x080ee1fb, image);
                        u8 height = U8_AT((void *)0x080ee207, image);
                        const void *source =
                            (u8 *)runtime + 0x800 +
                            S32_AT((void *)0x080ee214, image * 4);

                        Func_080072f4(
                            graphics, source,
                            (particle->x >> 16) - (width >> 1),
                            (particle->y >> 16) - (height >> 1),
                            width, height);
                        Func_080e3908(particle, 0x40, 0x2000);
                        particle->timer += particle->state;
                        if (particle->state > 1 && (frame & 1))
                            particle->state--;
                    }
                } else if (frame >= emitter->timer) {
                    Func_080072f4(
                        graphics,
                        (u8 *)runtime + (emitter_index & 1) * 4,
                        (emitter->x >> 16) - 0x10,
                        emitter->y >> 16,
                        0x20, 0x40);
                    Func_080e3908(emitter, 0x40, 0x10000);

                    if (emitter->y > 0x380000) {
                        struct Particle_080d3c80 *particle =
                            (struct Particle_080d3c80 *)
                                (0x02010000 + emitter_index * 0x1c0);
                        const u8 *position = (const u8 *)0x080ee1d3;
                        s32 particle_index;

                        emitter->state = 1;
                        emitter->y = 0x380000;
                        for (particle_index = 0;
                             particle_index < 16;
                             particle_index++, particle++, position += 2) {
                            s32 random_x;
                            s32 random_y;

                            particle->x =
                                ((position[0] - 0x28) << 16) + emitter->x;
                            particle->y = position[1] << 16;
                            random_x = (Func_08004458() & 0x7f) - 0x40;
                            random_y = -(s32)(Func_08004458() & 0x7f);
                            particle->velocity_x = random_x << 11;
                            particle->velocity_y = random_y << 11;
                            if (emitter_index & 1) {
                                particle->velocity_x *= 2;
                                particle->velocity_y = random_y << 12;
                            }
                            particle->state = 0x20;
                            particle->timer = 0;
                        }

                        S32_AT(runtime, 0x77a8) = 8;
                        Func_080f9010(0x90);
                        for (particle_index = 0;
                             particle_index < S32_AT(argument, 0x14);
                             particle_index++) {
                            Func_080d6888(
                                S16_AT(argument, 0x24 + particle_index * 2),
                                7, 5, particle_index, 4);
                        }
                    }
                }
            }
        }

        Func_080e155c(variant * 2 + 4, variant * 4 + 8);
        Func_080cd52c();
        S32_AT(runtime, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    Func_080cdbc0();
}
