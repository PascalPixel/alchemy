#include "types.h"

typedef void (*Transfer_080dc968)(void *, const void *, u32);
typedef void (*Renderer_080dc968)(
    void *, const void *, s32, s32, s32, s32);

struct Scene_080dc968 {
    u8 unknown_00[20];
    s32 object_count;
    u8 unknown_18[12];
    s16 object_ids[1];
};

struct Particle_080dc968 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 life;
};

struct Point4_080dc968 {
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_0c;
};

s32 Func_080022fc(s32, s32);
s32 Func_08002304(s32, s32);
void *Func_08002f40(s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_08004a28(void *);
void Func_08004a44(void *);
void Func_08004bd4(s32);
void Func_08004c1c(s32);
void Func_08004c6c(s32);
void Func_080051d8(void *, void *);
void Func_08009008(
    u32, const struct Point4_080dc968 *, void *, s32);
void Func_08009038(u32);
void Func_080b5040(s32, s32, s32);
void Func_080b5088(s16, s32);
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
void Func_080e3944(
    const struct Point4_080dc968 *, struct Point4_080dc968 *);
void Func_080e396c(s16, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

static void Render_080dc968(
    Renderer_080dc968 renderer, void *context, u8 *runtime,
    s32 source, s32 x, s32 y, s32 width, s32 height)
{
    renderer(
        context, runtime + source, x, y, width, height);
}

void Func_080dc968(struct Scene_080dc968 *scene)
{
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    u8 *effect_tiles = (u8 *)header[2];
    u32 *battle_handles = (u32 *)0x03001f00;
    Transfer_080dc968 transfer =
        (Transfer_080dc968)0x03001388;
    Renderer_080dc968 renderers[2];
    struct Particle_080dc968 *anchors =
        (struct Particle_080dc968 *)(runtime + 0x7080);
    struct Particle_080dc968 *particles =
        (struct Particle_080dc968 *)0x02010000;
    u8 *matrices = (u8 *)0x02013800;
    const u8 *motion_script;
    u16 saved_affine = *(volatile u16 *)0x03001ad4;
    s32 script_x = 0;
    s32 script_y = 0;
    u32 frame;
    u32 group;
    u32 i;

    *(struct Scene_080dc968 **)(runtime + 0x7828) = scene;
    Func_080cd594(0x2000);
    *(volatile u16 *)0x04000020 = 0x100;
    Func_080c9048();
    *(volatile u16 *)0x05000002 = 0;
    *(volatile u16 *)0x04000052 = 0;
    *(s32 *)(runtime + 0x7780) = 0;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080cd104(0, 0);
    Func_080d6750(scene);
    Func_080dbb24(9, 370, 1);
    Func_080e0524(0x6a, runtime, 1, 1);
    transfer(
        (void *)0x05000000, Func_08002f40(0xa0), 0x80);
    Func_080e0524(0x73, effect_tiles, 0, 0);
    motion_script = Func_08002f40(0xd2);

    Func_080ed408(46, 7, 7, 3, 2);
    Func_080ed408(47, 7, 7, 3, 3);
    renderers[0] = (Renderer_080dc968)battle_handles[2];
    renderers[1] = (Renderer_080dc968)battle_handles[3];
    *(volatile s32 *)(0x03001ce0 + 16) = 240;
    Func_080030f8(1);
    Func_080b5040(1, 59, 0);
    *(s32 *)(runtime + 0x7790) = 0;
    *(s32 *)(runtime + 0x7794) = 4;
    *(s32 *)(runtime + 0x7798) = -1;
    *(s32 *)(runtime + 0x779c) = 0;
    Func_080041d8((const void *)0x080c90e5, 0x480);
    *(s32 *)(*(u8 **)0x03001f00 + 16) = 1;
    Func_080cd104(0, 1);
    *(volatile u16 *)0x04000000 = 0x7741;
    *(volatile u16 *)0x04000020 = 0x80;
    *(volatile u16 *)0x04000052 = 0x1010;
    *(volatile u16 *)0x04000050 = 0x3f44;

    for (group = 0; group < 16; group++) {
        struct Particle_080dc968 *anchor = &anchors[group];

        anchor->x =
            (Func_08002304(Func_08004458(), 96) + 12) << 16;
        anchor->y =
            ((Func_08004458() & 0x3f) + 32) << 16;
        anchor->velocity_x = 0;
        anchor->velocity_y = 0;
        anchor->life = 0;
        for (i = 0; i < 24; i++) {
            struct Particle_080dc968 *particle =
                &particles[group * 24 + i];
            void *matrix = matrices + (group * 24 + i) * 48;

            particle->x = (Func_08004458() & 0xf) + 48;
            Func_080049ac();
            Func_08004c6c(Func_08004458() & 0xffff);
            Func_08004bd4(Func_08004458() & 0xffff);
            Func_08004c1c(Func_08004458() & 0xffff);
            Func_08004a28(matrix);
        }
    }

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 50;
    *(volatile u16 *)0x0400000c = 0;

    if ((*(u32 *)0x03001b04 & 3) == 0) {
        struct Point4_080dc968 projection_input;

        for (frame = 0;
             frame < 220 && ((*(u32 *)0x03001b04 & 3) == 0);
             frame++) {
            if (frame <= 209) {
                if (frame == 0) {
                    script_x =
                        ((s8)motion_script[0] << 8)
                        + motion_script[1];
                    script_y =
                        ((s8)motion_script[2] << 8)
                        + motion_script[3];
                    motion_script += 4;
                } else {
                    script_x += (s8)motion_script[0];
                    script_y += (s8)motion_script[1];
                    motion_script += 2;
                }

                projection_input.unknown_0c = 0;
                projection_input.y = 0xff0000;
                for (i = 0; i < 3; i++) {
                    u32 column;

                    projection_input.z =
                        0x400000 - (script_y << 16)
                        + i * 0x200000;
                    for (column = 0; column < 3; column++) {
                        projection_input.x =
                            (script_x << 16) + 0x500000
                            + column * 0x200000;
                        Func_08009008(
                            *(u32 *)(runtime + 0x77d8
                                     + (i * 3 + column) * 4),
                            &projection_input,
                            (void *)0x02010000, 0);
                    }
                }
            }

            projection_input.y = 0;
            projection_input.z = 0;
            if (frame == 48) {
                *(s32 *)(runtime + 0x77b4) = 24;
                *(s32 *)(runtime + 0x77b8) = 0;
            }

            for (group = 0; group < 16; group++) {
                struct Particle_080dc968 *anchor = &anchors[group];
                s32 anchor_x = (s16)(anchor->x >> 16);
                s32 anchor_y = (s16)(anchor->y >> 16);
                s32 threshold = group * 8 + 64;

                if (frame < threshold)
                    continue;
                if (frame == group * 8 + 84)
                    Func_080f9010(0xd4);

                if (frame >= group * 8 + 85) {
                    anchor->x += anchor->velocity_x;
                    anchor->y += anchor->velocity_y;
                    anchor->velocity_x -= 0x10000;
                    anchor->velocity_y += 0x400000;
                    Render_080dc968(
                        renderers[0], render_context, runtime,
                        0x16ac, anchor_x + 4, anchor_y - 40,
                        16, 21);
                    Render_080dc968(
                        renderers[0], render_context, runtime,
                        0x17fc, anchor_x - 16, anchor_y - 19,
                        29, 35);
                    Render_080dc968(
                        renderers[0], render_context, runtime,
                        0x1bf3, anchor_x - 20, anchor_y + 16,
                        21, 24);
                } else if (frame >= group * 8 + 80) {
                    switch (frame - threshold - 16) {
                    case 0:
                        Render_080dc968(
                            renderers[0], render_context, runtime,
                            0, anchor_x - 7, anchor_y - 14,
                            14, 28);
                        break;
                    case 1:
                        Render_080dc968(
                            renderers[0], render_context, runtime,
                            0x188, anchor_x - 11, anchor_y - 22,
                            23, 44);
                        break;
                    case 2:
                        Render_080dc968(
                            renderers[0], render_context, runtime,
                            0x57c, anchor_x - 4, anchor_y - 31,
                            20, 30);
                        Render_080dc968(
                            renderers[0], render_context, runtime,
                            0x7d4, anchor_x - 16, anchor_y - 1,
                            22, 33);
                        break;
                    case 3:
                        Render_080dc968(
                            renderers[0], render_context, runtime,
                            0xaaa, anchor_x + 1, anchor_y - 38,
                            18, 27);
                        Render_080dc968(
                            renderers[0], render_context, runtime,
                            0xe74, anchor_x - 11, anchor_y - 11,
                            22, 22);
                        Render_080dc968(
                            renderers[0], render_context, runtime,
                            0x1088, anchor_x - 19, anchor_y + 11,
                            19, 28);
                        break;
                    default:
                        Render_080dc968(
                            renderers[0], render_context, runtime,
                            0x11f8, anchor_x + 4, anchor_y - 40,
                            16, 23);
                        Render_080dc968(
                            renderers[0], render_context, runtime,
                            0x147c, anchor_x - 10, anchor_y - 17,
                            23, 28);
                        Render_080dc968(
                            renderers[0], render_context, runtime,
                            0x16ac, anchor_x - 20, anchor_y + 11,
                            20, 28);
                        break;
                    }
                } else {
                    for (i = 0; i < 24; i++) {
                        struct Particle_080dc968 *particle =
                            &particles[group * 24 + i];
                        void *matrix =
                            matrices + (group * 24 + i) * 48;

                        if (particle->x > 0) {
                            struct Point4_080dc968 projected;

                            Func_08004a44(matrix);
                            projection_input.x = particle->x;
                            Func_080e3944(
                                &projection_input, &projected);
                            projected.x =
                                (projected.x >> 1) + anchor_x;
                            projected.y += anchor_y + 16;
                            particle->x -= 4;
                            renderers[1](
                                render_context,
                                effect_tiles
                                    + ((const u16 *)0x080ede48)[4],
                                projected.x - 2,
                                projected.y - 5,
                                5, 10);
                        }
                    }
                }
            }

            *(s32 *)(runtime + 0x7824) = 1;
            Func_080030f8(1);
        }
    }

    Func_08004278((const void *)0x080c90e5);
    *(s32 *)(*(u8 **)0x03001f00 + 16) = 0;
    *(volatile u16 *)0x03001ad4 = saved_affine;
    Func_080d67dc();
    for (i = 0; i < 9; i++)
        Func_08009038(*(u32 *)(runtime + 0x77d8 + i * 4));
    *(volatile u16 *)0x04000020 = 0x80;
    *(volatile u16 *)0x04000000 = 0x7741;
    Func_080e0524(0xb4, (void *)0x02010000, 1, 0);

    for (i = 0; i < 32; i++) {
        s32 target = Func_080022fc(i, 6);
        struct Particle_080dc968 *particle = &anchors[i];

        if (target < scene->object_count) {
            s32 position[2];
            s32 y;

            Func_080e396c(scene->object_ids[target], position);
            y = -((Func_08004458() & 0x1f) + 40);
            particle->y = y;
            particle->x =
                ((position[0]
                  + ((u32)position[0] >> 31)) >> 1)
                + ((80 - y + ((u32)(80 - y) >> 31)) >> 1);
        } else {
            particle->x =
                (Func_08004458() & 0x3f) + 80;
            particle->y =
                -((Func_08004458() & 0x1f) + 40);
        }
        particle->life = -1;
    }

    for (frame = 0; frame < 88; frame++) {
        for (i = 0; i < 24; i++) {
            struct Particle_080dc968 *particle = &anchors[i];

            if (frame < i * 2 && frame <= 40)
                continue;
            if (particle->life >= 0) {
                s32 stage;
                s32 width;
                s32 height;

                if (particle->life <= 23) {
                    stage = particle->life / 4;
                    width = ((const u8 *)0x080ede9f)[stage];
                    height = ((const u8 *)0x080edea5)[stage];
                    renderers[i & 1](
                        render_context,
                        (u8 *)0x02010000
                            + ((const u16 *)0x080edeb2)[stage],
                        particle->x - (width >> 1) - 8,
                        particle->y
                            + ((const u8 *)0x080edeab)[stage]
                            - 40,
                        width, height);
                    if (particle->life <= 11) {
                        Render_080dc968(
                            renderers[0], render_context, runtime,
                            0x16ac, particle->x + 4,
                            particle->y - 40, 16, 21);
                        Render_080dc968(
                            renderers[0], render_context, runtime,
                            0x17fc, particle->x - 16,
                            particle->y - 19, 29, 35);
                    }
                }
                particle->life++;
            } else {
                s32 clipped_height = 24;

                if (particle->y > 56)
                    clipped_height = 80 - particle->y;
                Render_080dc968(
                    renderers[0], render_context, runtime,
                    0x16ac, particle->x + 4,
                    particle->y - 40, 16, 21);
                Render_080dc968(
                    renderers[0], render_context, runtime,
                    0x17fc, particle->x - 16,
                    particle->y - 19, 29, 35);
                if (clipped_height > 0)
                    Render_080dc968(
                        renderers[0], render_context, runtime,
                        0x1bf3, particle->x - 20,
                        particle->y + 16, 21,
                        clipped_height);
                particle->x -= 6;
                particle->y += 12;
                if (particle->y > 79) {
                    s32 target;

                    particle->life = 0;
                    *(s32 *)(runtime + 0x77a8) = 2;
                    Func_080f9010(0x86);
                    target = Func_080022fc(i, 6);
                    if (target < scene->object_count) {
                        Func_080d6888(
                            scene->object_ids[target],
                            7, 5, target, 8);
                        Func_080b5088(
                            scene->object_ids[target], 1);
                    }
                }
            }
        }

        Func_080e155c(4, 8);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
