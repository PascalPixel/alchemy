#include "types.h"

typedef void (*Renderer_080dfe2c)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

#define MUL_SHIFT_080DFE2C(left, right, shift) \
    ((s32)((u32)(left) * (u32)(right)) >> (shift))

struct Object_080dfe2c {
    s32 x;
    s32 y;
    s32 unused08;
    s32 velocity_x;
    s32 velocity_y;
    s32 unused14;
    s32 active;
};

s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_08002dd8(s32 arg0);
void Func_080030f8(s32 arg0);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(const void *address);
u32 Func_08004458(void);
void Func_080b5088(s32 arg0, s32 arg1);
void Func_080b50e8(s32 arg0);
void Func_080cd52c(void);
void Func_080cd594(s32 arg0);
void Func_080cdbc0(void);
void Func_080cef64(s32 arg0, Renderer_080dfe2c *renderers);
void Func_080d6888(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080dfddc(void *destination, void *source, s32 width, s32 height);
void Func_080e0524(s32 arg0, void *destination, s32 arg2, s32 arg3);
void Func_080e155c(s32 arg0, s32 arg1);
void Func_080e3908(struct Object_080dfe2c *object, s32 arg1, s32 arg2);
void Func_080f9010(s32 arg0);

void Func_080dfe2c(void *argument)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    void *graphics = (void *)runtime_header[2];
    Renderer_080dfe2c renderers[2];
    struct Object_080dfe2c *sprites =
        (struct Object_080dfe2c *)(runtime + 0x7080);
    struct Object_080dfe2c *particles =
        (struct Object_080dfe2c *)0x02010000;
    u32 frame;
    u32 i;

    *(void **)(runtime + 0x7828) = argument;
    Func_080cd594(0);
    Func_080cef64(0, renderers);
    Func_080e0524(0x73, graphics, 0, 0);
    Func_080e0524(0x92, runtime, 1, 0);
    Func_080e0524(0x6f, particles, 1, 1);
    Func_080dfddc(particles, runtime + 0x2a8, 17, 104);
    Func_080dfddc((u8 *)particles + 0x6e8, runtime + 0x990, 34, 65);

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (i = 0; i < 8; i++)
        sprites[i].active = -1;
    for (i = 0; i < 512; i++)
        particles[i].active = -1;

    Func_080f9010(162);

    for (frame = 0; frame < 96; frame++) {
        if (frame == 56)
            Func_080b50e8(133);

        for (i = 0; i < 5; i++) {
            struct Object_080dfe2c *sprite = &sprites[i];

            if (sprite->active == -1)
                continue;

            renderers[0](
                render_context,
                runtime + 0x990,
                sprite->x - 16,
                sprite->y - 17,
                65,
                34);
            sprite->x -= 12;
            sprite->active++;

            if (sprite->active == 5) {
                struct Object_080dfe2c *group = &particles[i * 32];
                u32 j;

                Func_080f9010(133);
                *(s32 *)(runtime + 0x77a8) = 4;

                for (j = 0; j < 32; j++) {
                    s32 angle = Func_08004458() & 0xffff;
                    s32 speed = (Func_08004458() & 0x1ff) + 0x100;

                    group[j].x = (s32)((u32)sprite->x << 16);
                    group[j].y = (s32)((u32)sprite->y << 16);
                    group[j].velocity_x = MUL_SHIFT_080DFE2C(
                        speed,
                        Func_08002322(angle),
                        8);
                    group[j].velocity_y = MUL_SHIFT_080DFE2C(
                        speed,
                        Func_0800231c(angle),
                        7);
                    group[j].active = (Func_08004458() & 15) + 32;
                }
            }
        }

        {
            s32 angle = frame << 11;
            s32 amplitude = 64 - (frame * 2);
            s32 x_offset = MUL_SHIFT_080DFE2C(
                amplitude,
                Func_08002322(angle),
                17);
            s32 y_offset = MUL_SHIFT_080DFE2C(
                amplitude,
                Func_0800231c(angle),
                16);
            s32 center_x = 96 + x_offset;
            s32 center_y = 60 + y_offset;
            const u8 *schedule = (const u8 *)0x080eec5a;

            renderers[1](
                render_context,
                runtime,
                x_offset + 86,
                y_offset + 43,
                20,
                34);

            for (i = 0; i < 5; i++) {
                if (frame == schedule[i] && sprites[i].active == -1) {
                    sprites[i].x = center_x - 8;
                    sprites[i].y = center_y;
                    sprites[i].velocity_x = center_x;
                    sprites[i].active = 0;
                    break;
                }

                if (frame == schedule[i] + 6) {
                    s32 count =
                        *(s32 *)(*(u8 **)(runtime + 0x7828) + 20);
                    s32 object_index;

                    for (object_index = 0; object_index < count;
                         object_index++) {
                        s32 id = *(s16 *)(
                            *(u8 **)(runtime + 0x7828) +
                            36 +
                            (object_index * 2));
                        Func_080d6888(id, 7, 5, object_index, 6);
                        Func_080b5088(id, 6);
                    }
                }
            }
        }

        for (i = 0; i < 512; i++) {
            struct Object_080dfe2c *particle = &particles[i];

            if (particle->active != -1) {
                s32 phase = particle->active / 16 + 2;
                s32 width = phase;
                s32 height = phase * 2;
                const void *source =
                    (u8 *)graphics + ((const u16 *)0x080ede48)[phase - 1];

                renderers[1](
                    render_context,
                    source,
                    (s16)((u32)particle->x >> 16) - (phase / 2),
                    (s16)((u32)particle->y >> 16) - phase,
                    width,
                    height);
                Func_080e3908(particle, 62, 0x4000);
                particle->active--;
            }
        }

        Func_080e155c(4, 4);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
