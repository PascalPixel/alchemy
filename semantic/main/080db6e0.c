#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

struct Particle_080db6e0 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    u32 age;
};

void Func_080cd594(s32);
void Func_080e0524(s32, void *, s32, s32);
void *Func_08002f40(s32);
void Func_080072f0(s32, s32, s32, s32);
u32 Func_08004458(void);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080041d8(const void *, s32);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_080f9010(s32);
void Func_080b50e8(s32);
void Func_080e3944(void *, s32 *);
void Func_080072f4(s32, const void *, s32, s32, s32, s32);
void Func_080e38b8(void *, s32, s32);
s32 Func_080022ec(s32, s32);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080b5088(s16, s32);
void Func_080e155c(s32, s32);
void Func_080cd52c(void);
void Func_080030f8(s32);
void Func_08004278(const void *);
void Func_08002dd8(s32);
s32 Func_080cdbc0(void);

s32 Func_080db6e0(void *argument, s32 monochrome)
{
    u8 *runtime = *(u8 **)0x03001eec;
    s32 graphics = *(s32 *)0x03001ef0;
    struct Particle_080db6e0 *emitters =
        (struct Particle_080db6e0 *)(runtime + 0x7080);
    struct Particle_080db6e0 *particles =
        (struct Particle_080db6e0 *)0x02010000;
    s32 variant;
    s32 table_index;
    s32 frame;
    s32 i;

    PTR_AT(runtime, 0x7828) = argument;
    Func_080cd594(1);
    Func_080e0524(0xc0, runtime, 1, 0);

    if (monochrome == 1) {
        u16 *palette = (u16 *)0x05000000;
        for (i = 0; i < 0x40; i++) {
            s32 intensity = i / 2;
            palette[i] =
                (intensity << 10) | (intensity << 5) | intensity;
        }
        variant = 1;
    } else {
        Func_080072f0(
            0x05000000, (s32)Func_08002f40(0x96),
            0x80, 0x03001388);
        variant = S32_AT(argument, 0x18);
    }

    for (i = 0; i < 0x20; i++) {
        struct Particle_080db6e0 *emitter = &emitters[i];
        emitter->x =
            S32_AT(argument, 4) == 1 ? 0x320000 : (s32)0xffce0000;
        emitter->y = 0;
        emitter->z = 0;
        emitter->velocity_x =
            ((Func_08004458() & 0x3f) - 0x20) << 13;
        emitter->velocity_y =
            ((Func_08004458() & 0x3f) + 0x10) << 12;
        emitter->velocity_z =
            ((Func_08004458() & 0x3f) - 0x20) << 13;
        emitter->age = 0;
    }

    for (i = 0; i < 0x400; i++) {
        struct Particle_080db6e0 *particle = &particles[i];
        particle->x =
            S32_AT(argument, 4) == 1 ? 0x320000 : (s32)0xffce0000;
        particle->y = 0;
        particle->z = 0;
        particle->velocity_x =
            ((Func_08004458() & 0x3f) - 0x20) << 13;
        particle->velocity_y =
            ((Func_08004458() & 0x1f) + 8) << 13;
        particle->velocity_z =
            ((Func_08004458() & 0x3f) - 0x20) << 13;
        particle->age = 0;
    }

    Func_080ed408(0x2e, 7, 7, 3, 2);
    S32_AT(runtime, 0x7780) = 2;
    S32_AT(runtime, 0x7784) = 0x4b;
    Func_080041d8((const void *)0x080cd261, 0x480);

    table_index = variant * 3;
    for (frame = 0;
         frame < U8_AT((void *)0x080eeae2, table_index + 2);
         frame++) {
        u8 *camera = *(u8 **)0x03001e80;

        Func_080049ac();
        Func_080051d8(camera, camera + 0x0c);
        if (frame == 2)
            Func_080f9010(0x90);
        if (frame ==
            U8_AT((void *)0x080eeae2, table_index + 2) - 0x30)
            Func_080b50e8(0x85);

        for (i = 0;
             i < U8_AT((void *)0x080eeae2, table_index);
             i++) {
            struct Particle_080db6e0 *particle = &particles[i];

            if (particle->y >= 0) {
                s32 projected[3];
                s32 depth;
                s32 size;
                s32 height;
                const void *source;

                Func_080e3944(particle, projected);
                projected[0] =
                    projected[0] / 2 +
                    (S32_AT(argument, 4) << 5) - 0x10;
                if (projected[2] < 0xa0)
                    projected[2] = 0xa0;
                if (projected[2] > 0x31f)
                    projected[2] = 0x31f;
                depth = projected[2] - 0xa0;
                size = 9 - depth / 0x40;
                height = size * 2;
                source =
                    runtime + 0x3200 + (i & 1) * 0x302 +
                    U16_AT((void *)0x080ede48, height - 2);
                Func_080072f4(
                    graphics, source,
                    projected[0] -
                        ((size + (size >> 31)) >> 1),
                    projected[1] - size,
                    size, height);
                Func_080e38b8(particle, 0x40, -0x2000);
            }
        }

        if (frame > 2) {
            for (i = 0;
                 i < U8_AT((void *)0x080eeae2, table_index + 1);
                 i++) {
                struct Particle_080db6e0 *emitter = &emitters[i];

                if (i < frame && emitter->y >= 0) {
                    s32 projected[3];
                    Func_080e3944(emitter, projected);
                    projected[0] =
                        projected[0] / 2 +
                        (S32_AT(argument, 4) << 5) - 0x10;

                    if (emitter->age <= 0x14) {
                        s32 animation =
                            Func_080022ec(emitter->age, 3) * 2;
                        u16 size =
                            U16_AT((void *)0x080eeafa, animation);
                        const void *source =
                            runtime +
                            U16_AT((void *)0x080eeaec, animation);
                        Func_080072f4(
                            graphics, source,
                            projected[0] - (size >> 1),
                            projected[1] - (size >> 1),
                            size, size);
                    }
                    if (emitter->age <= 0x14)
                        emitter->age++;
                    Func_080e38b8(emitter, 0x40, -0x2000);
                }
            }
        }

        if (monochrome == 0) {
            for (i = 0; i < S32_AT(argument, 0x14); i++) {
                if (frame == i + 6) {
                    s16 character =
                        S16_AT(argument, 0x24 + i * 2);
                    Func_080d6888(character, 7, 5, i, 0x0a);
                    Func_080b5088(character, 2);
                }
            }
        } else {
            for (i = 0; i < S32_AT(argument, 0x14); i++) {
                if (frame == i + 6) {
                    Func_080d6888(
                        S16_AT(argument, 0x24 + i * 2),
                        7, 5, i, 0x0a);
                }
            }
        }

        if (frame == 2)
            S32_AT(runtime, 0x77a8) = 6;
        Func_080e155c(0x10, 0x10);
        Func_080cd52c();
        S32_AT(runtime, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(0x2e);
    return Func_080cdbc0();
}
