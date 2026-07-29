#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

struct Particle_080dd2c4 {
    s32 x;
    s32 y;
    s32 unknown_08;
    s32 unknown_0c;
    s32 limit;
    s32 unknown_14;
    s32 age;
};

void Func_080cd594(s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080ed408(s32, s32, s32, s32, s32);
u32 Func_08004458(void);
s32 Func_080022fc(s32, s32);
void Func_080041d8(const void *, s32);
void Func_080b50e8(s32);
void Func_080072f4();
void Func_080f9010(s32);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e155c(s32, u32);
void Func_080cd52c(void);
void Func_080030f8(s32);
void Func_08004278(const void *);
void Func_08002dd8(s32);
s32 Func_080cdbc0(void);

s32 Func_080dd2c4(void *scene, s32 variant)
{
    u8 *runtime = *(u8 **)0x03001eec;
    s32 graphics_source = *(s32 *)0x03001ef0;
    struct Particle_080dd2c4 *particles =
        (struct Particle_080dd2c4 *)0x02010000;
    struct Particle_080dd2c4 *emitters =
        (struct Particle_080dd2c4 *)(runtime + 0x7080);
    s32 emitter_count;
    s32 frame_count;
    s32 frame;
    s32 i;

    PTR_AT(runtime, 0x7828) = scene;
    Func_080cd594(1);
    *(volatile u16 *)0x04000020 = 0x0100;
    *(volatile u16 *)0x04000050 = 0;

    if (variant == 1)
        Func_080e0524(0x83, runtime, 1, 1);
    else
        Func_080e0524(0x84, runtime, 1, 1);
    if (S32_AT(scene, 4) == 1)
        *(volatile s32 *)0x04000028 = (s32)0xffff9000;

    Func_080ed408(0x2e, 7, 7, 3, 1);
    Func_080ed408(0x2f, 7, 7, 7, 1);

    emitter_count = U8_AT((void *)0x080eeb5e, (u32)PTR_AT(scene, 0x18));
    frame_count = emitter_count * 4 + 0x38;

    for (i = 0; i < 0x400; i++)
        particles[i].age = -1;

    for (i = 0; i < 16; i++) {
        s32 limit;
        s32 table;
        emitters[i].x =
            S8_AT((void *)0x080eeb61, i) + (Func_08004458() & 7) - 4;
        emitters[i].y = (i + (i < 0)) / 2 + 0x6c;
        limit = (Func_08004458() & 0x3f) + 0x37;
        emitters[i].limit = limit;
        table = Func_080022fc(i, 3);
        if (U8_AT((void *)0x080eeb4b, table) < limit)
            emitters[i].limit = U8_AT((void *)0x080eeb4b, table);
        emitters[i].age = i * 4 + 8;
    }

    S32_AT(runtime, 0x7780) = 1;
    S32_AT(runtime, 0x7784) = 0;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (frame = 0; frame < frame_count; frame++) {
        if (frame == frame_count - 0x40)
            Func_080b50e8(0x84);

        if (frame < frame_count - 4) {
            if (frame >= frame_count - 0x14) {
                *(volatile u16 *)0x04000050 = 0x3f44;
                *(volatile u16 *)0x04000052 =
                    (frame_count - frame - 5) | 0x1000;
            }

            for (i = 0; i < emitter_count; i++) {
                struct Particle_080dd2c4 *emitter = &emitters[i];
                s32 start = i * 4 + 8;

                if (frame == i * 4 + 9)
                    S32_AT(runtime, 0x77a8) = 2;

                if (frame > start) {
                    s32 height = (frame - start) * 8;
                    s32 table = Func_080022fc(i, 3);
                    if (height > emitter->limit)
                        height = emitter->limit;

                    if (variant == 0) {
                        u8 width = U8_AT((void *)0x080eeb48, table);
                        Func_080072f4(
                            graphics_source,
                            runtime + U16_AT((void *)0x080eeb4e, table * 2),
                            emitter->x - (width >> 1), emitter->y - height,
                            width, height);
                    } else {
                        s32 maximum = S8_AT((void *)0x080eeb71, i & 7);
                        u8 width = U8_AT((void *)0x080eeb54, table);
                        if (height > maximum)
                            height = maximum;
                        Func_080072f4(
                            graphics_source,
                            runtime + U16_AT((void *)0x080eeb58, table * 2),
                            emitter->x - (width >> 1), emitter->y - height,
                            width, height);
                    }
                }

                if (S32_AT(scene, 0x14) != 0) {
                    s32 member;
                    for (member = 0; member < S32_AT(scene, 0x14); member++) {
                        if (frame == start + 4) {
                            if (!(i & 1))
                                Func_080f9010(0x85);
                            Func_080d6888(
                                S16_AT(scene, 0x24 + member * 2),
                                7, 5, member, 3);
                        }
                    }
                }

                if (frame == start + 4 || frame == start + 8) {
                    s32 slot;
                    for (slot = 0; slot < 0x200; slot++) {
                        if (particles[slot].age == -1) {
                            particles[slot].x =
                                emitter->x + (Func_08004458() & 15) - 8;
                            particles[slot].y =
                                (Func_08004458() & 15) + 0x50;
                            particles[slot].age = 0;
                            break;
                        }
                    }
                }
            }
        }

        for (i = 0; i < 0x200; i++) {
            struct Particle_080dd2c4 *particle = &particles[i];
            if (particle->age >= 0) {
                s32 phase = (particle->age + ((u32)particle->age >> 31)) >> 1;
                s32 base = variant ? 0x0aff : 0x1e59;
                s8 half_width = S8_AT((void *)0x080eeb79, phase);
                s8 height = S8_AT((void *)0x080eeb80, phase);
                s32 source = base + U16_AT((void *)0x080eeb88, phase * 2);

                Func_080072f4(
                    graphics_source, runtime + source,
                    particle->x - half_width,
                    particle->y - ((height + (height < 0)) >> 1),
                    half_width, height);
                Func_080072f4(
                    graphics_source, runtime + source,
                    particle->x,
                    particle->y - ((height + (height < 0)) >> 1),
                    S8_AT((void *)0x080eeb79, phase), height);
                particle->age++;
                if (particle->age == 14)
                    particle->age = -1;
            }
        }

        Func_080e155c(4, 4);
        Func_080cd52c();
        S32_AT(runtime, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    return Func_080cdbc0();
}
