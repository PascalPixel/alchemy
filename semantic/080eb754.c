#include "types.h"

typedef void (*Renderer_080eb754)(
    void *, const void *, s32, s32, s32, s32);

struct Scene_080eb754 {
    u8 unknown_00[20];
    s32 object_count;
    u8 unknown_18[12];
    s16 object_ids[1];
};

struct Particle_080eb754 {
    s32 x;
    s32 y;
    s32 unknown_08;
    s32 velocity_x;
    s32 velocity_y;
    s32 unknown_14;
    s32 timer;
};

s32 Func_080022fc(s32, s32);
s32 Func_08002304(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_08009008(void *, s32 *, s32 *, s32);
void Func_08009038(void *);
void Func_080b5040(s32, s32, s32);
void Func_080b5088(s16, s32);
void Func_080b50e8(s32);
void Func_080c9048(void);
void Func_080cd104(s32, s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6750(void *);
void Func_080d67dc(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080dbb24(s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e3980(s16, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080eb754(struct Scene_080eb754 *scene)
{
    u8 *runtime = *(u8 **)0x03001eec;
    void *render_context = *(void **)0x03001ef0;
    u8 *graphics = *(u8 **)0x03001ef4;
    u32 *callback_state = *(u32 **)0x03001f00;
    Renderer_080eb754 renderer;
    struct Particle_080eb754 *primary =
        (struct Particle_080eb754 *)(runtime + 0x7080);
    struct Particle_080eb754 *bursts =
        (struct Particle_080eb754 *)0x02010000;
    void **projection_objects = (void **)(runtime + 0x77d8);
    const u16 *particle_offsets = (const u16 *)0x080ede48;
    const u8 *object_x = (const u8 *)0x080eef56;
    const u8 *object_z = (const u8 *)0x080eef5f;
    s32 saved_scroll = *(volatile u16 *)0x03001ad4;
    s32 projection[2];
    s32 point[4];
    s32 horizontal_offset = 0;
    s32 scroll = 0;
    s32 ramp = 0;
    s32 wave = 0;
    s32 frame;
    s32 i;

    *(struct Scene_080eb754 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    Func_080c9048();
    *(volatile u16 *)0x0400000c = 0x0784;
    *(volatile u16 *)0x05000000 = 0;
    *(volatile u16 *)0x05000002 = 0;
    *(s32 *)(runtime + 0x7780) = 0;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080cd104(1, 0);
    Func_080dbb24(9, 0x175, 1);
    *(s32 *)(runtime + 0x2737 + 16) = 240;
    Func_080d6750(scene);
    *(volatile u16 *)0x04000048 = 0xca;
    *(volatile u16 *)0x04000040 = 0x3a;
    Func_080030f8(1);
    Func_080b5040(1, 0x3a, 0);
    Func_080cd104(1, 1);
    Func_080e0524(0x73, graphics, 0, 0);
    Func_080e0524(0x95, runtime, 1, 1);
    *(volatile u16 *)0x04000000 = 0x7741;
    *(volatile u16 *)0x04000020 = 0x80;
    *(volatile u16 *)0x04000052 = 0x100e;
    *(volatile u16 *)0x04000050 = 0x3f44;

    callback_state[4] = 1;
    *(s32 *)(runtime + 0x7780) = 1;
    *(s32 *)(runtime + 0x7784) = 0;
    *(s32 *)(runtime + 0x7824) = 0;
    *(s32 *)(runtime + 0x77a8) = 0;

    for (i = 0; i < 64; i++) {
        primary[i].x = (Func_08004458() & 31) + 16;
        primary[i].y =
            ((Func_08004458() & 31) + 48) << 16;
        primary[i].velocity_y =
            ((Func_08004458() & 31) - 16) << 16;
        primary[i].timer =
            Func_08002304(Func_08004458(), 48) + 2;
    }

    Func_080ed408(46, 7, 7, 3, 3);
    renderer = *(Renderer_080eb754 *)0x03001f08;
    *(volatile u16 *)0x0400000c = 0x0786;

    projection[0] = *(const s32 *)0x080edae0;
    projection[1] = *(const s32 *)0x080edae4;

    for (frame = 0;
         frame < 120 &&
         ((*(volatile u32 *)0x03001b04 & 3) == 0 ||
          frame <= 16);
         frame++) {
        if ((u32)(frame - 24) <= 31)
            ramp++;
        if (ramp > 24)
            ramp = 24;
        if (frame <= 135) {
            *(volatile u16 *)0x03001ad4 -= (u16)ramp;
            scroll += ramp;
        }

        if (frame <= 149) {
            s32 object_wave = 0;

            if (frame > 103)
                horizontal_offset = frame * 16 - 0x680;
            if ((u32)(frame - 8) <= 23)
                horizontal_offset -= ramp - 8;
            if (frame > 7) {
                s32 amplitude = frame <= 104 ? 32 : 96;
                s32 angle = ((frame << 10) - 0x2000) & 0xffff;

                object_wave =
                    (amplitude * Func_08002322(angle)) >> 16;
                wave = object_wave;
                if ((frame & 31) == 8)
                    *(s32 *)(runtime + 0x77a8) = 4;
            }

            point[1] = 0x00ff0000;
            point[3] = 0;
            for (i = 0; i < 9; i++) {
                point[0] =
                    (scroll + object_x[i] - horizontal_offset)
                        * 0x10000 + 0x00e00000;
                point[2] =
                    (object_z[i] - object_wave) * 0x10000 +
                    0x00480000;
                Func_08009008(
                    projection_objects[i],
                    point,
                    projection,
                    0);
            }
        }

        if (frame <= 26) {
            s32 radius = scroll + 4;
            s32 count = frame * 8;

            if (radius > 10) radius = 10;
            if (count > 64) count = 64;
            for (i = 0; i < count; i++) {
                s32 angle = i << 10;
                s32 x =
                    scroll +
                    (((scroll * 2 + 8) *
                      Func_08002322(angle)) >> 16);
                s32 y =
                    (((scroll * 12 + 48) *
                      Func_0800231c(angle)) >> 16);
                s32 diameter = radius * 2;

                renderer(
                    render_context,
                    graphics +
                        particle_offsets[radius - 1],
                    x + 96 - radius / 2,
                    y + 64 - radius,
                    radius,
                    diameter);
            }
        }

        if (frame == 24) {
            *(s32 *)(runtime + 0x7780) = 2;
            *(s32 *)(runtime + 0x7784) = 50;
        }
        if (frame == 28)
            *(volatile u16 *)0x0400000c = 0x0784;

        if (frame > 17) {
            for (i = 0; i < 48; i++) {
                struct Particle_080eb754 *particle = &primary[i];

                if (particle->timer == 0) {
                    s32 size = Func_080022fc(i, 3) + 1;

                    renderer(
                        render_context,
                        graphics + particle_offsets[size - 1],
                        particle->x,
                        *(s16 *)((u8 *)&particle->y + 2) -
                            size,
                        size,
                        size * 2);
                    particle->x += 2;
                    particle->y += particle->velocity_y;
                    particle->velocity_y =
                        (particle->velocity_y * 3) / 4;
                } else {
                    particle->timer--;
                }

                if (particle->x > 128 ||
                    particle->timer == 1) {
                    particle->x =
                        (Func_08004458() & 31) +
                        horizontal_offset + 172;
                    particle->y =
                        ((Func_08004458() & 31) -
                         wave + 56) << 16;
                    particle->velocity_y =
                        ((Func_08004458() & 31) - 16)
                        << 15;
                }
            }
        }

        if (frame > 31) {
            s32 drop = (frame - 32) / 2;

            if (drop > 40)
                drop = 40;
            for (i = 0; i < 6; i++) {
                s32 image = Func_08004458() & 3;

                renderer(
                    render_context,
                    runtime + image * 0x600,
                    120 - drop,
                    i * 18,
                    48,
                    32);
            }
        }

        if (*(s32 *)(runtime + 0x77a8) > 0) {
            *(s32 *)(runtime + 0x77a8) -= 1;
            *(volatile u16 *)0x03001ad6 =
                (Func_08004458() & 7) + 28;
        } else {
            *(volatile u16 *)0x03001ad6 = 32;
        }

        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
        if (frame == 0)
            Func_080f9010(136);
        if (frame == 26)
            Func_080f9010(141);
        if (frame == 40 || frame == 72 || frame == 104)
            Func_080f9010(154);
    }

    *(volatile u16 *)0x03001ad4 = (u16)saved_scroll;
    callback_state[4] = 0;
    Func_080d67dc();
    *(volatile u16 *)0x04000040 = 0x00f0;
    for (i = 0; i < 9; i++)
        ((u8 *)projection_objects[i])[9] |= 12;

    {
        u8 actor_triggered[14];
        u8 actor_x[16];
        s32 x_limit = 224;

        for (i = 0; i < 14; i++)
            actor_triggered[i] = 0;
        for (i = 0; i < 16; i++)
            actor_x[i] = Func_08004458() & 31;
        for (i = 0; i < 320; i++)
            bursts[i].timer = 0;

        *(s32 *)(runtime + 0x7780) = 2;
        *(s32 *)(runtime + 0x7784) = 75;
        *(volatile u16 *)0x0400000c = 0x0784;
        *(volatile u16 *)0x04000052 = 0x1010;

        for (frame = 0; frame < 96; frame++) {
            s32 object_wave;

            if (frame <= 23)
                x_limit -= 16;
            if (frame <= 8) {
                object_wave =
                    Func_08002322(
                        ((frame << 11) + 0x4000) &
                        0xffff) >> 10;
            } else if (frame <= 23) {
                object_wave =
                    Func_08002322(
                        ((frame << 11) + 0x4000) &
                        0xffff) >> 11;
            } else {
                object_wave = 0;
            }

            if (frame <= 23) {
                point[1] = 0x00ff0000;
                point[3] = 0;
                for (i = 0; i < 9; i++) {
                    point[0] =
                        (x_limit + object_x[i]) << 16;
                    point[2] =
                        (object_z[i] - object_wave) << 16;
                    point[2] += 0x00480000;
                    Func_08009008(
                        projection_objects[i],
                        point,
                        projection,
                        0);
                }
            }

            if (frame == 8) {
                *(s32 *)(runtime + 0x77a8) = 8;
                Func_080f9010(145);
            }
            if (frame == 11)
                Func_080f9010(145);
            if (frame == 46)
                Func_080f9010(137);

            for (i = 0;
                 i < scene->object_count;
                 i++) {
                if (!actor_triggered[i]) {
                    s32 position[2];

                    Func_080e3980(
                        scene->object_ids[i], position);
                    if (position[0] > x_limit) {
                        s32 j;
                        struct Particle_080eb754 *spawn =
                            &bursts[i * 32];

                        actor_triggered[i] = 1;
                        for (j = 0; j < 32; j++, spawn++) {
                            s32 vx =
                                (Func_08004458() & 255) -
                                128;
                            s32 vy =
                                (Func_08004458() & 255) -
                                192;

                            spawn->x = position[0] << 15;
                            spawn->y =
                                (position[1] - 16) << 16;
                            spawn->velocity_x = vx << 10;
                            spawn->velocity_y = vy << 11;
                            spawn->x +=
                                (vx << 10) * 4;
                            spawn->y += vy << 13;
                            spawn->timer =
                                (Func_08004458() & 15) + 8;
                        }
                        Func_080b5088(
                            scene->object_ids[i], 1);
                        Func_080f9010(134);
                    }
                }
            }

            for (i = 0; i < 192; i++) {
                struct Particle_080eb754 *particle =
                    &bursts[i];

                if (particle->timer > 0) {
                    renderer(
                        render_context,
                        graphics +
                            *(volatile u16 *)0x03001ad4,
                        *(s16 *)((u8 *)&particle->x + 2)
                            - 1,
                        *(s16 *)((u8 *)&particle->y + 2)
                            - 3,
                        3,
                        6);
                    particle->x += particle->velocity_x;
                    particle->y += particle->velocity_y;
                    particle->timer--;
                }
            }

            if (frame == 48)
                Func_080f9010(136);

            if (frame > 40) {
                *(s32 *)(runtime + 0x7780) = 0;
                *(s32 *)(runtime + 0x7784) = 75;
                for (i = 0; i < 16; i++) {
                    s32 image = Func_08004458() & 3;

                    renderer(
                        render_context,
                        runtime + image * 0x600,
                        actor_x[i] + 600 - frame * 12,
                        i * 8 - 8,
                        48,
                        32);
                }
            }
            if (frame > 64)
                *(s32 *)(runtime + 0x7780) = 2;

            if (frame == 58) {
                for (i = 0; i < scene->object_count; i++)
                    Func_080d6888(
                        scene->object_ids[i],
                        14, 5, -1, 0);
            }

            Func_080e155c(8, 8);
            Func_080cd52c();
            *(s32 *)(runtime + 0x7824) = 1;
            Func_080030f8(1);
        }
    }

    Func_080b50e8(134);
    for (i = 0; i < 9; i++)
        Func_08009038(projection_objects[i]);
    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
