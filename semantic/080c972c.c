#include "types.h"

struct Particle_080c972c {
    s32 x;
    s32 y;
    s32 unused08;
    s32 velocity_x;
    s32 velocity_y;
    s32 unused14;
    s32 timer;
};

typedef void (*Renderer_080c972c)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

void Func_08002dd8(s32 resource);
void Func_080030f8(s32 frames);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(const void *source);
u32 Func_08004458(void);
void Func_080b50e8(s32 sound);
void Func_080c9048(void);
void Func_080cd52c(void);
void Func_080cd594(s32 mode);
void Func_080cdbc0(void);
void Func_080d6888(s32 id, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080e0524(s32 resource, void *destination, s32 arg2, s32 arg3);
void Func_080e3908(struct Particle_080c972c *particle, s32 arg1, s32 arg2);
void Func_080ed408(s32 resource, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080f9010(s32 sound);

void Func_080c972c(void *argument)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    u8 *graphics = (u8 *)runtime_header[2];
    Renderer_080c972c renderers[2];
    struct Particle_080c972c *particles =
        (struct Particle_080c972c *)(runtime + 0x7080);
    struct Particle_080c972c *fragments =
        (struct Particle_080c972c *)0x02010140;
    const u8 *config = (const u8 *)0x080ededc;
    s32 effect = *(s32 *)((u8 *)argument + 24);
    s32 frame;
    s32 index;

    *(void **)(runtime + 0x7828) = argument;
    Func_080cd594(0x2001);
    *(volatile u16 *)0x04000020 = 0x0100;
    Func_080e0524(0xcc, runtime + 0x604, 1, 1);
    Func_080e0524(0x76, runtime, 0, 0);
    Func_080c9048();
    *(volatile u16 *)0x04000050 = 0x3f44;
    *(volatile u16 *)0x04000048 = 0x3337;

    Func_080ed408(46, 7, 7, 2, 2);
    renderers[0] = (Renderer_080c972c)runtime_header[7];
    Func_080ed408(47, 7, 7, 2, 3);
    renderers[1] = (Renderer_080c972c)runtime_header[8];

    for (index = 0; index < 512; index++)
        fragments[index].timer = -1;

    for (index = 0; index < 64; index++) {
        s32 random = Func_08004458() & 63;
        s32 y = -(index * config[effect * 4 + 2] + 16);
        s32 x;

        if (*(s32 *)((u8 *)argument + 4) == 1)
            x = random + y / 2 - 48;
        else
            x = random - y / 2 + 72;
        particles[index].x = x * 8;
        particles[index].y = y * 8;
        particles[index].timer = -1;
    }

    for (index = 0; index < 160; index++) {
        u16 value;

        if (*(s32 *)((u8 *)argument + 4) == 0) {
            if ((u32)(index - 8) <= 95)
                value =
                    ((52 - index / 2) << 8) |
                    (180 - index / 2);
            else if (index <= 135)
                value = 0x0080;
            else
                value = 0x0100;
        } else {
            if ((u32)(index - 8) <= 95)
                value =
                    ((index / 2 + 60) << 8) |
                    (index / 2 + 188);
            else if (index <= 135)
                value = 0x70f0;
            else
                value = 0x0100;
        }
        ((u16 *)0x02010000)[index] = value;
    }

    Func_080041d8((const void *)0x080c91a5, 0x480);
    if (effect == 0) {
        *(s32 *)(runtime + 0x7780) = 1;
        *(s32 *)(runtime + 0x7784) = 0;
    } else if (effect == 1) {
        *(s32 *)(runtime + 0x7780) = 2;
        *(s32 *)(runtime + 0x7784) = 50;
    } else {
        *(s32 *)(runtime + 0x7780) = 2;
        *(s32 *)(runtime + 0x7784) = 75;
    }
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (frame = 0; frame < config[effect * 4 + 3]; frame++) {
        if (frame == config[effect * 4 + 3] - 64)
            Func_080b50e8(132);

        for (index = 0; index < config[effect * 4]; index++) {
            struct Particle_080c972c *particle = &particles[index];
            s32 x = particle->x / 8;
            s32 y = particle->y / 8;

            if (particle->timer == -1) {
                renderers[0](
                    render_context,
                    runtime + 0x604,
                    x,
                    y,
                    24,
                    24);
                if (particle->y <= 0x27f) {
                    if (*(s32 *)((u8 *)argument + 4) == 0)
                        particle->x -= 32;
                    else
                        particle->x += 32;
                    particle->y += 64;
                } else {
                    s32 fragment_count = config[effect * 4 + 1];
                    s32 fragment_index;

                    particle->timer = 0;
                    for (fragment_index = 0;
                         fragment_index < fragment_count;
                         fragment_index++) {
                        struct Particle_080c972c *fragment =
                            &fragments[
                                index * fragment_count + fragment_index];

                        fragment->x = (x + 12) << 16;
                        fragment->y = y << 16;
                        fragment->velocity_x =
                            (s32)((u32)(
                                (Func_08004458() & 255) - 128) << 9);
                        if (effect == 2) {
                            fragment->velocity_y =
                                (s32)((u32)(
                                    (Func_08004458() & 0x1ff) -
                                    0x180) << 10);
                        } else {
                            fragment->velocity_y =
                                (s32)((u32)(
                                    (Func_08004458() & 255) -
                                    255) << 10);
                        }
                        fragment->timer =
                            (Func_08004458() & 15) + 16;
                    }

                    if ((index & 3) == 0)
                        Func_080f9010(132);

                    {
                        s32 object_count =
                            *(s32 *)((u8 *)argument + 20);
                        s32 object_index;

                        for (object_index = 0;
                             object_index < object_count;
                             object_index++) {
                            s32 id = *(s16 *)(
                                (u8 *)argument +
                                36 +
                                object_index * 2);
                            Func_080d6888(
                                id,
                                7,
                                5,
                                object_index,
                                2);
                        }
                    }
                }
            } else if (particle->timer <= 7) {
                const void *source;
                s32 render_x;
                s32 render_y;
                s32 size;

                if ((u32)particle->timer <= 3) {
                    source = runtime + 0x604;
                    render_x = x;
                    render_y = y;
                    size = 24;
                } else {
                    source = runtime + 0x844;
                    render_x = x - 9;
                    render_y = y - 9;
                    size = 42;
                }
                renderers[0](
                    render_context,
                    source,
                    render_x,
                    render_y,
                    size,
                    size);
            }

            if ((u32)particle->timer <= 14)
                particle->timer++;
        }

        for (index = 0; index < 512; index++) {
            struct Particle_080c972c *fragment = &fragments[index];

            if (fragment->timer != -1) {
                s32 size = fragment->timer + 1;
                const void *source;

                if (size > 6)
                    size = 6;
                source =
                    graphics +
                    ((const u16 *)0x080ede5c)[size - 1];
                renderers[1](
                    render_context,
                    source,
                    *(s16 *)((u8 *)fragment + 2) - size,
                    *(s16 *)((u8 *)fragment + 6) - size,
                    size * 2,
                    size * 2);
                Func_080e3908(fragment, 60, 0x2000);
                fragment->timer--;
            }
        }

        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080c91a5);
    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
    Func_080c9048();
}
