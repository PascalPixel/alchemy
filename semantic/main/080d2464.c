#include "types.h"

typedef void (*Transfer_080d2464)(void *, const void *, u32);
typedef void (*Renderer_080d2464)(
    void *, const void *, s32, s32, s32, s32);

struct Scene_080d2464 {
    u8 unknown_00[4];
    s32 direction;
    s32 target_id;
    u8 unknown_0c[8];
    s32 object_count;
    u8 unknown_18[12];
    s16 object_ids[1];
};

struct Particle_080d2464 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 age;
};

void *Func_08002f40(s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(volatile void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_08005340(const void *, void *);
void Func_08009020(void *, s32);
void Func_08009038(void *);
void Func_080b5088(s32, s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6750(void *);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080dbb24(s32, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e396c(s32, s32 *);
void Func_080e6d3c(s32, s32, s32);
void Func_080e6eac(s32, s32, s32);
void Func_080e727c(s32, s32, s32);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

static void Spawn_080d2464(
    struct Particle_080d2464 *particles,
    s32 count,
    s32 mode,
    s32 origin_x,
    s32 origin_y,
    s32 direction,
    s32 fast)
{
    s32 i;

    for (i = 0; i < count; i++) {
        struct Particle_080d2464 *particle = &particles[i];

        if (particle->age < 0) {
            s32 angle = (Func_08004458() & 0x1fff) + 0x4e20;
            s32 speed = (Func_08004458() & 0xff) + 128;

            if (mode == 0) {
                if (fast) {
                    particle->x = 68 << 16;
                    particle->y = 64 << 16;
                } else {
                    particle->x =
                        ((Func_08004458() & 7) + 78) << 16;
                    particle->y = 70 << 16;
                }
            } else {
                particle->x = fast
                    ? origin_x << 16
                    : ((Func_08004458() & 7) + origin_x - 8) << 16;
                particle->y = origin_y << 16;
            }
            particle->velocity_x =
                speed * Func_08002322(angle) >> (fast ? 6 : 9);
            particle->velocity_y =
                speed * Func_0800231c(angle) >> (fast ? 6 : 9);
            particle->age = 0;
            (void)direction;
            return;
        }
    }
}

void Func_080d2464(struct Scene_080d2464 *scene, s32 mode)
{
    u8 *runtime = *(u8 **)0x03001eec;
    void *render_context = *(void **)0x03001ef0;
    u8 *graphics = *(u8 **)0x03001ef4;
    void *effect_context = *(void **)0x03001e80;
    Renderer_080d2464 renderer0;
    Renderer_080d2464 renderer1;
    Transfer_080d2464 transfer = (Transfer_080d2464)0x03001388;
    struct Particle_080d2464 *small =
        (struct Particle_080d2464 *)(runtime + 0x7320);
    struct Particle_080d2464 *sparks =
        (struct Particle_080d2464 *)(runtime + 0x74e0);
    struct Particle_080d2464 *large =
        (struct Particle_080d2464 *)0x02010000;
    struct Particle_080d2464 *trails =
        (struct Particle_080d2464 *)(runtime + 0x7080);
    s32 direction = 1;
    s32 origin[3] = { 0, 0, 0 };
    s32 beam_x = 212 << 16;
    s32 beam_y = (s32)0xffc40000;
    s32 frame;
    s32 i;

    *(struct Scene_080d2464 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    *(volatile u16 *)0x04000052 = 0x1010;
    *(volatile u16 *)0x0400000c = 0x0784;

    Func_080ed408(46, 7, 7, 3, 2);
    renderer0 = *(Renderer_080d2464 *)0x03001f08;
    Func_080ed408(47, 7, 7, 3, 3);
    renderer1 = *(Renderer_080d2464 *)0x03001f0c;

    {
        u8 *source = Func_08002f40(0x7d);
        transfer((void *)0x05000000, source, 0x80);
        Func_08005340(source + 0x80, runtime);
    }
    {
        u8 *source = Func_08002f40(0xb4);
        Func_08005340(source + 0x80, runtime + 0x3000);
    }
    Func_08005340(Func_08002f40(0x73), graphics);
    if (mode == 1)
        transfer((void *)0x05000000, Func_08002f40(0xc4), 0x80);

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    if (mode != 0) {
        direction = -1;
        if (scene->direction != 1)
            direction = 1;
    }
    if (mode == 1) {
        Func_080e396c(scene->target_id, origin);
        origin[0] /= 2;
        origin[1] = 66;
        origin[0] = scene->direction == 1 ? 76 : 44;
    }

    for (i = 0; i < 64; i++)
        ((struct Particle_080d2464 *)(runtime + 0x7098))[i].age = -1;
    for (i = 0; i < 16; i++) {
        small[i].x = direction == 1
            ? (Func_08004458() & 0x7f) + 128
            : (Func_08004458() & 0x7f) - 128;
        small[i].y = (Func_08004458() & 7) - 72;
        small[i].age = -(s32)(Func_08004458() & 31);
    }
    for (i = 0; i < 512; i++)
        large[i].age = -1;

    if (mode == 0) {
        Func_080d6750(scene);
        Func_080030f8(1);
        Func_080dbb24(8, (s32)0x03001b04, 2);
    }

    for (frame = 0; frame < 208; frame++) {
        if ((*(u32 *)0x03001b04 & 3) != 0 &&
            frame > 48 && frame <= 159) {
            if (mode == 0) {
                Func_08009020(*(void **)(runtime + 0x77d8), 8);
                Func_08009020(*(void **)(runtime + 0x77dc), 9);
                Func_08009020(*(void **)(runtime + 0x77e4), 10);
                Func_08009020(*(void **)(runtime + 0x77e8), 11);
            }
            for (i = 0; i < scene->object_count; i++) {
                Func_080d6888(scene->object_ids[i], 10, 5, -1, 0);
                Func_080b5088(scene->object_ids[i], 4);
            }
            frame = 160;
        }

        Func_080049ac();
        Func_080051d8(effect_context, (u8 *)effect_context + 12);

        if (frame == 178)
            Func_080b50e8(134);
        if (frame == 128)
            *(s32 *)(runtime + 0x7784) = 50;

        if (frame == 176) {
            *(s32 *)(runtime + 0x7780) = 3;
            *(u32 *)(runtime + 0x77a8) = 0x02020202;
            transfer((void *)0x05000000, Func_08002f40(0xc4), 0x80);
        } else if (frame >= 160 && frame <= 175) {
            *(s32 *)(runtime + 0x7780) = 1;
            *(u32 *)(runtime + 0x77a8) =
                frame <= 173 ? 0x10101010 : 0x3f3f3f3f;
            Func_080e727c(2, 2, 2);
        }

        if (frame >= 33 && frame <= 175) {
            s32 wanted = frame <= 103 ? 1 : 8;
            s32 made = 0;

            for (i = 0; i < 512 && made < wanted; i++) {
                if (large[i].age < 0) {
                    s32 lane = i & 3;
                    large[i].x =
                        ((Func_08004458() & 0xff) - 32) << 16;
                    large[i].y = 112 << 16;
                    large[i].velocity_x =
                        ((Func_08004458() & 0x7f) +
                         *(u8 *)(0x080ee184 + lane)) << 9;
                    large[i].velocity_y =
                        -(((Func_08004458() & 0x7f) +
                           *(u8 *)(0x080ee184 + lane)) << 11);
                    large[i].age = 0;
                    made++;
                }
            }
        }

        if (frame >= 41 && frame <= 127 && (frame & 1))
            Spawn_080d2464(sparks, 24, mode,
                           origin[0], origin[1], direction, 0);
        if (frame >= 129 && frame <= 175)
            Spawn_080d2464(trails, 24, mode,
                           origin[0], origin[1], direction, 1);

        if (frame == 48)
            Func_080f9010(141);
        if (frame == 128)
            Func_080f9010(145);

        if (frame <= 175) {
            for (i = 0; i < 24; i++) {
                struct Particle_080d2464 *particle = &trails[i];
                if (particle->age >= 0) {
                    renderer0(
                        render_context,
                        runtime + ((particle->age / 4) << 11),
                        (s16)(particle->x >> 16) - 16,
                        (s16)(particle->y >> 16) - 32,
                        32, 64);
                    particle->x += direction * particle->velocity_x;
                    particle->y += particle->velocity_y;
                    if (++particle->age == 24)
                        particle->age = -1;
                }
            }
        }

        for (i = 0; i < 24; i++) {
            struct Particle_080d2464 *particle = &sparks[i];
            if (particle->age >= 0) {
                renderer1(
                    render_context, (const void *)0x080ee188,
                    (s16)(particle->x >> 16),
                    (s16)(particle->y >> 16) - 1,
                    1, 2);
                particle->x += direction * particle->velocity_x;
                particle->y += particle->velocity_y;
                particle->velocity_y -= 0x400;
                if (++particle->age == 48)
                    particle->age = -1;
            }
        }

        if (frame <= 175) {
            for (i = 0; i < 512; i++) {
                struct Particle_080d2464 *particle = &large[i];
                if (particle->age >= 0) {
                    s32 image = *(u8 *)(0x080ee18a + (i & 3));
                    renderer0(
                        render_context,
                        graphics +
                            *(u16 *)(0x080ede48 + (image - 1) * 2),
                        (s16)(particle->x >> 16) - image / 2,
                        (s16)(particle->y >> 16) - image,
                        image, image * 2);
                    particle->x += direction * particle->velocity_x;
                    particle->y += particle->velocity_y;
                    if (frame > 128) {
                        particle->velocity_x +=
                            (i & 1) ? -0x8000 : -0x2000;
                    } else {
                        particle->velocity_y -= 0x8000;
                    }
                    if (++particle->age == 256)
                        particle->age = -1;
                }
            }
        }

        if (frame == 128)
            *(s32 *)(runtime + 0x77a8) = 48;
        if (mode == 0 && frame == 48)
            *(s32 *)(runtime + 0x77a8) = 8;

        if (frame >= 40 && frame <= 47) {
            beam_x += (s32)0xfff80000;
            beam_y += 0x100000;
        }

        if (mode == 0) {
            if (frame == 128) {
                Func_08009020(*(void **)(runtime + 0x77d8), 8);
                Func_08009020(*(void **)(runtime + 0x77dc), 9);
                Func_08009020(*(void **)(runtime + 0x77e4), 10);
                Func_08009020(*(void **)(runtime + 0x77e8), 11);
            }
            if (frame == 176) {
                Func_08009020(*(void **)(runtime + 0x77d8), 0);
                Func_08009020(*(void **)(runtime + 0x77dc), 1);
                Func_08009020(*(void **)(runtime + 0x77e4), 3);
                Func_08009020(*(void **)(runtime + 0x77e8), 4);
            }
            Func_080e6d3c(3, beam_x, beam_y);
        }

        if (frame == 138) {
            for (i = 0; i < scene->object_count; i++) {
                Func_080d6888(scene->object_ids[i], 10, 5, -1, 0);
                Func_080b5088(scene->object_ids[i], 4);
            }
        }

        if (frame <= 175) {
            for (i = 0; i < 16; i++) {
                struct Particle_080d2464 *particle = &small[i];

                if (particle->y > 55 && particle->age <= 11) {
                    s32 image = particle->age / 2;
                    renderer0(
                        render_context,
                        runtime + 0x3000 +
                            *(u16 *)(0x080ee1a0 + image * 2),
                        particle->x -
                            *(u8 *)(0x080ee18e + image) / 2,
                        particle->y +
                            *(u8 *)(0x080ee19a + image),
                        *(u8 *)(0x080ee18e + image),
                        *(u8 *)(0x080ee194 + image));
                    particle->age++;
                    if (particle->age == 12)
                        particle->age = 0;
                } else if (particle->age == 0) {
                    particle->x -= direction * 6;
                    particle->y += 6;
                    if (frame <= 47 && particle->y > 55)
                        Func_080f9010(136);
                    renderer0(render_context,
                              graphics + *(u16 *)(0x080ede48 + 18),
                              particle->x - 5, particle->y + 30,
                              10, 20);
                } else {
                    particle->age++;
                }
            }
        }

        Func_080e155c(8, 8);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((volatile void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    if (mode == 0) {
        Func_080e6eac(3, beam_x, beam_y);
        for (i = 0; i < 8; i++)
            Func_08009038(*(void **)(runtime + 0x77d8 + i * 4));
    }
    Func_080cdbc0();
}
