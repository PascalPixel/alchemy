#include "types.h"

typedef void (*Transfer_080dbc30)(
    void *destination,
    const void *source,
    u32 size);

typedef void (*Renderer_080dbc30)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Scene_080dbc30 {
    u8 unknown_00[4];
    s32 direction;
    s32 focus_id;
    u8 unknown_0c[8];
    s32 object_count;
    u8 unknown_18[12];
    s16 object_ids[1];
};

struct Particle_080dbc30 {
    s32 x;
    s32 y;
    s32 unknown_08;
    s32 velocity_x;
    s32 velocity_y;
    s32 unknown_14;
    s32 phase;
};

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(u32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080b5088(s16, s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080cef64(s32, Renderer_080dbc30 *);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, u32);
void Func_080e3958(s32, s32 *);
void Func_080e396c(s16, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080dbc30(struct Scene_080dbc30 *scene, s32 mode)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    void *graphics = (void *)runtime_header[2];
    Transfer_080dbc30 transfer = (Transfer_080dbc30)0x03001388;
    Renderer_080dbc30 renderers[2];
    struct Particle_080dbc30 *particles =
        (struct Particle_080dbc30 *)(runtime + 0x7080);
    s32 anchor[3];
    s32 object_positions[8][3];
    s32 palette_resource = 0;
    u32 frame;
    u32 i;

    *(struct Scene_080dbc30 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    *(volatile u16 *)0x04000052 = 0x1010;

    if (mode == 7) {
        Func_080ed408(46, 7, 7, 3, 2);
        renderers[0] = (Renderer_080dbc30)runtime_header[7];
        Func_080ed408(47, 7, 7, 7, 2);
        renderers[1] = (Renderer_080dbc30)runtime_header[8];
    } else {
        Func_080cef64(scene->direction, renderers);
    }

    Func_080e0524(0xce, runtime, 1, 0);
    if (mode == 5 || mode == 7) {
        Func_080e0524(
            mode == 5 ? 0x5a : 0x54,
            runtime + 0xc56,
            1,
            1);
    } else {
        Func_080e0524(0x7d, runtime + 0xc56, 1, 1);
        Func_080e0524(0x73, graphics, 0, 0);

        if (mode == 6) {
            volatile u16 *palette = (volatile u16 *)0x05000000;

            for (i = 0; i < 64; i++) {
                s32 level = i / 4;
                palette[i] =
                    (level << 10) | (level << 5) | level;
            }
            *(volatile u16 *)0x04000050 = 0;
        } else {
            switch (mode) {
            case 0:
                palette_resource = 0x7d;
                break;
            case 1:
                palette_resource = 0xb9;
                break;
            case 2:
                palette_resource = 0x6e;
                break;
            case 3:
                palette_resource = 0xa1;
                break;
            default:
                palette_resource = 0x8d;
                break;
            }
            transfer(
                (void *)0x05000000,
                Func_08002f40(palette_resource),
                0x80);
        }
    }

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = mode == 7 ? 50 : 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (i = 0; i < 64; i++)
        particles[i].phase = -1;

    Func_080e3958(scene->focus_id, anchor);
    if (mode == 3)
        anchor[1] -= 16;
    if (mode == 4)
        anchor[0] += scene->direction == 1 ? 28 : -28;
    if (mode == 7)
        anchor[0] += scene->direction == 1 ? 16 : -16;
    if (mode == 5) {
        anchor[0] = Func_080022ec(anchor[0], 3);
        *(volatile u16 *)0x04000020 = 0x55;
    }

    for (i = 0; i < scene->object_count; i++)
        Func_080e396c(scene->object_ids[i], object_positions[i]);

    for (frame = 0; frame < 64; frame++) {
        s32 selected = Func_080022fc(frame, scene->object_count);

        if (frame == 4)
            Func_080f9010(0x88);
        if (mode != 6) {
            if (frame == 24)
                Func_080b50e8(0x86);
        } else if (frame == 60) {
            Func_080b50e8(0x86);
        }

        if (mode == 5) {
            const void *source;
            s32 x;

            if (scene->direction == 1) {
                s32 animation =
                    Func_080022fc(Func_080022ec(frame, 3), 3);
                source = runtime + 0xc56 + animation * 0x1200;
                x = anchor[0] - 2;
            } else {
                s32 animation =
                    Func_080022fc(Func_080022ec(frame, 3), 3);
                source = runtime + 0xc56 + animation * 0x1200;
                x = anchor[0] - 70;
            }
            renderers[0](
                render_context,
                source,
                x,
                anchor[1] - 32,
                72,
                62);
        } else {
            s32 target_x =
                object_positions[selected][0]
                    + (Func_08004458() & 31) - 16;
            s32 target_y =
                object_positions[selected][1]
                    + (Func_08004458() & 63) - 16;

            if (frame <= 47) {
                struct Particle_080dbc30 *particle = &particles[frame];

                particle->x = anchor[0] << 15;
                particle->y = anchor[1] << 16;
                particle->velocity_x =
                    (target_x - anchor[0]) << 11;
                particle->velocity_y =
                    (target_y - anchor[1]) << 11;
                particle->phase = 0;
            }
        }

        for (i = 0; i < 64; i++) {
            struct Particle_080dbc30 *particle = &particles[i];

            if (particle->phase >= 0) {
                if (mode == 7) {
                    if (particle->phase > 5)
                        renderers[scene->direction](
                            render_context,
                            runtime + 0xc56,
                            (s16)(particle->x >> 16) - 16,
                            (s16)(particle->y >> 16) - 32,
                            32,
                            64);
                } else if (mode == 4) {
                    if (particle->phase > 5)
                        renderers[0](
                            render_context,
                            runtime + 0xc56
                                + (particle->phase / 4) * 0x800,
                            (s16)(particle->x >> 16) - 16,
                            (s16)(particle->y >> 16) - 32,
                            32,
                            64);
                } else if (mode != 5 && particle->phase > 1) {
                    renderers[0](
                        render_context,
                        runtime + 0xc56
                            + (particle->phase / 4) * 0x800,
                        (s16)(particle->x >> 16) - 16,
                        (s16)(particle->y >> 16) - 32,
                        32,
                        64);
                }

                particle->x += particle->velocity_x;
                particle->y += particle->velocity_y;
                particle->phase++;
                if (particle->phase == 24)
                    particle->phase = -1;
            }
        }

        if (mode == 5) {
            for (i = 0; i < scene->object_count; i++) {
                if (frame >= i * 4 + 2 && (frame & 7) == i) {
                    *(s32 *)(runtime + 0x77a8) = 8;
                    Func_080d6888(
                        scene->object_ids[i], 7, 5, i, 4);
                }
            }
        } else {
            for (i = 0; i < scene->object_count; i++) {
                if (frame >= i * 4 + 16 && (frame & 7) == i) {
                    *(s32 *)(runtime + 0x77a8) = 8;
                    if (mode == 6)
                        Func_080d6888(
                            scene->object_ids[i], 14, 5, i, 4);
                    else
                        Func_080d6888(
                            scene->object_ids[i], 7, 5, i, 4);
                    Func_080b5088(scene->object_ids[i], 4);
                }
            }
        }

        Func_080e155c(4, 4);
        if (mode != 6)
            Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
