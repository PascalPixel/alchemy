#include "types.h"

typedef void (*InitializeOwner_080cbc0c)(void *destination, u32 size);
typedef void (*Transfer_080cbc0c)(
    void *destination, const void *source, u32 size);
typedef void (*Fill_080cbc0c)(void *destination, u32 size, u32 value);
typedef void (*Renderer_080cbc0c)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Particle_080cbc0c {
    s32 x;
    s32 y;
    s32 unknown_08;
    s32 velocity_x;
    s32 velocity_y;
    s32 unknown_14;
    s32 age;
};

struct TransferQueueEntry_080cbc0c {
    u32 destination;
    u32 source;
    u32 control;
};

void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void *Func_080048b0(s32, u32);
void Func_080b5028(s32, s32, s32, s32);
void Func_080b5038(s32, u16, s32);
void Func_080b5040(s32, u16, s32);
void Func_080cd508(u32);
void Func_080cd52c(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e3908(struct Particle_080cbc0c *, s32, s32);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

static void QueueTransfer_080cbc0c(
    u32 destination, u32 source, u32 control)
{
    volatile u16 *interrupt_master = (volatile u16 *)0x04000208;
    volatile u16 *count = (volatile u16 *)0x02002090;
    u16 saved = *interrupt_master;

    *interrupt_master = (u16)(u32)interrupt_master;
    if (*count <= 31) {
        struct TransferQueueEntry_080cbc0c *entry =
            (struct TransferQueueEntry_080cbc0c *)
                (0x02002090 + *count * 12);

        (*count)++;
        entry->destination = destination;
        entry->source = source;
        entry->control = control;
    }
    *interrupt_master = saved;
}

static void PlotPixel_080cbc0c(s32 x, s32 y)
{
    volatile u8 *tiles = (volatile u8 *)0x02010000;

    if (x < 0)
        x = 0;
    if (x > 255)
        x = 255;
    if (y < 0)
        y = 0;
    if (y > 119)
        y = 119;
    tiles[(y >> 3) * 0x800 + (y & 7) * 8 +
          (x >> 3) * 0x40 + (x & 7)] = 2;
}

static void PlotSymmetricCircle_080cbc0c(s32 x, s32 y)
{
    PlotPixel_080cbc0c(96 + x, 60 + y);
    PlotPixel_080cbc0c(96 + x, 60 - y);
    PlotPixel_080cbc0c(96 - x, 60 + y);
    PlotPixel_080cbc0c(96 - x, 60 - y);
    PlotPixel_080cbc0c(96 + y, 60 + x);
    PlotPixel_080cbc0c(96 + y, 60 - x);
    PlotPixel_080cbc0c(96 - y, 60 + x);
    PlotPixel_080cbc0c(96 - y, 60 - x);

    PlotPixel_080cbc0c(97 + x, 60 + y);
    PlotPixel_080cbc0c(97 + x, 60 - y);
    PlotPixel_080cbc0c(97 - x, 60 + y);
    PlotPixel_080cbc0c(97 - x, 60 - y);
    PlotPixel_080cbc0c(97 + y, 60 + x);
    PlotPixel_080cbc0c(97 + y, 60 - x);
    PlotPixel_080cbc0c(97 - y, 60 + x);
    PlotPixel_080cbc0c(97 - y, 60 - x);
}

void Func_080cbc0c(void *scene)
{
    u8 *runtime = Func_080048b0(39, 0x782c);
    void *render_context = Func_080048b0(40, 0x4000);
    void *small_graphics = Func_080048b0(41, 0x302);
    u8 *global_runtime = *(u8 **)0x03001e74;
    u32 *callback_state = *(u32 **)0x03001f00;
    InitializeOwner_080cbc0c initialize_owner =
        (InitializeOwner_080cbc0c)0x03000164;
    Transfer_080cbc0c transfer = (Transfer_080cbc0c)0x03001388;
    Fill_080cbc0c fill = (Fill_080cbc0c)0x03000168;
    Renderer_080cbc0c renderers[2];
    struct Particle_080cbc0c *particles =
        (struct Particle_080cbc0c *)(runtime + 0x7080);
    const u8 *initial_x = (const u8 *)0x080ee016;
    const u8 *initial_y = (const u8 *)0x080ee037;
    s32 frame;
    s32 i;

    *(void **)(runtime + 0x7828) = scene;
    Func_080cd508((u32)global_runtime);
    callback_state[3] = 1;
    *(volatile u16 *)0x03001ad6 = 32;
    Func_080b5038(1, *(u16 *)(global_runtime + 0x648), 0);
    *(volatile u16 *)0x0400000c = 0x0784;
    Func_080b5028(0, 0, 0, 100);
    callback_state[3] = 0;
    *(volatile s32 *)0x04000028 = 0;
    *(volatile s32 *)0x0400002c = -0x1000;
    *(volatile u16 *)0x04000020 = 0x80;
    *(volatile u16 *)0x04000022 = 0;
    *(volatile u16 *)0x04000024 = 0;
    *(volatile u16 *)0x04000026 = 0x100;
    *(volatile u16 *)0x04000040 = 0x00f0;
    *(volatile u16 *)0x04000042 = 0x00f0;
    *(volatile u16 *)0x04000044 = 0x1088;
    *(volatile u16 *)0x04000046 = 0x1088;
    *(volatile u16 *)0x04000048 = 0x3537;
    *(volatile u16 *)0x0400004a = 0x3f21;

    for (i = 0; i < 128; i++) {
        s32 row = i >> 3;

        ((volatile u16 *)0x06003800)[i] =
            (u16)((0x100 + row * 0x1000) | (i * 2));
    }

    initialize_owner(render_context, 0x4000);
    initialize_owner((void *)0x06004000, 0x4000);
    QueueTransfer_080cbc0c(0x00007741, 0x04000000, 0x01000000);
    *(volatile u16 *)0x0400000a = 0x1f81;
    *(volatile u16 *)0x04000052 = 0x1010;
    Func_080e0524(0x44, runtime, 1, 1);
    *(s32 *)(runtime + 0x7780) = 1;
    *(s32 *)(runtime + 0x7784) = 0;
    Func_080041d8((const void *)0x080cd261, 0x480);

    Func_080ed408(46, 7, 7, 3, 1);
    renderers[0] = *(Renderer_080cbc0c *)0x03001f08;
    Func_080ed408(47, 7, 7, 3, 2);
    renderers[1] = *(Renderer_080cbc0c *)0x03001f0c;

    for (i = 0; i < 33; i++) {
        particles[i].x = initial_x[i] << 16;
        particles[i].y = initial_y[i] << 16;
        particles[i].velocity_x =
            (particles[i].x - 0x200000) >> 2;
        particles[i].velocity_y =
            (particles[i].y - 0x3c0000) >> 2;
    }

    transfer((void *)0x02010000, (const void *)0x06008000, 0x7800);
    fill((void *)0x02010000, 0x7800, 0x01010101);
    callback_state[4] = 1;
    *(s32 *)(runtime + 0x77a0) =
        *(volatile u16 *)0x03001ad4;
    *(s32 *)(runtime + 0x77a4) =
        *(volatile u16 *)0x03001ad6;
    *(volatile u16 *)0x03001ad4 = 0;
    QueueTransfer_080cbc0c(0x00001f81, 0x0400000a, 0x00010000);
    fill((void *)0x050000c0, 0x100, 0x7fff7fff);
    Func_080f9010(212);
    Func_080d6888(
        *(s16 *)(*(u8 **)(runtime + 0x7828) + 36),
        7, 3, 0, 30);

    for (frame = 0; frame < 128; frame++) {
        if (frame == 2 || frame == 3)
            Func_080f9010(212);
        if (frame == 28)
            Func_080d6888(
                *(s16 *)(*(u8 **)(runtime + 0x7828) + 36),
                -1, 3, 0, 0);
        if (frame == 32)
            Func_080f9010(149);
        if (frame == 5) {
            Func_080f9010(145);
            *(volatile u16 *)0x03001ad4 =
                *(u16 *)(runtime + 0x77a0);
            Func_080b5040(1, *(u16 *)(global_runtime + 0x648), -1);
        }

        if (frame > 7) {
            volatile u16 *palette = (volatile u16 *)0x050000c0;
            u16 *target = (u16 *)(global_runtime + 0x544);

            for (i = 0; i < 128; i++) {
                s32 source = palette[i];
                s32 destination = target[i];
                s32 r = source & 31;
                s32 g = (source >> 5) & 31;
                s32 b = (source >> 10) & 31;
                s32 target_r = destination & 31;
                s32 target_g = (destination >> 5) & 31;
                s32 target_b = (destination >> 10) & 31;

                if (r < target_r) r++;
                else if (r > target_r) r--;
                if (g < target_g) g++;
                else if (g > target_g) g--;
                if (b < target_b) b++;
                else if (b > target_b) b--;
                palette[i] = (u16)(r | (g << 5) | (b << 10));
            }
        }

        if (frame == 4)
            fill((void *)0x06008000, 0x7800, 0x02020202);

        if (frame <= 3) {
            s32 radius;

            *(volatile u16 *)0x05000002 =
                (u16)((frame + 8) * 0x421);
            for (radius = 0; radius < frame * 32; radius++) {
                s32 x = radius;
                s32 y = 0;
                s32 error = radius;

                while (x >= y) {
                    PlotSymmetricCircle_080cbc0c(x, y);
                    error -= y * 2 + 1;
                    if (error < 0) {
                        error += x * 2 - 2;
                        x--;
                    }
                    y++;
                }
            }
            transfer(
                (void *)0x06008000,
                (const void *)0x02010000,
                0x7800);
        }

        if (frame <= 50) {
            for (i = 0; i < 33; i++) {
                const u16 *offsets = (const u16 *)0x080edfd2;

                renderers[0](
                    render_context,
                    runtime + offsets[i],
                    *(s16 *)((u8 *)&particles[i].x + 2) +
                        ((const u8 *)0x080edf90)[i],
                    *(s16 *)((u8 *)&particles[i].y + 2) +
                        ((const u8 *)0x080edfb1)[i],
                    ((const u8 *)0x080edf90)[i],
                    ((const u8 *)0x080edfb1)[i]);
                if (frame > 3)
                    Func_080e3908(&particles[i], 64, 0x4000);
            }
        }

        if ((u32)(frame - 8) <= 42) {
            s32 level = frame - 8;

            if (level > 31)
                level = 31;
            *(volatile u16 *)0x05000002 =
                (u16)(level | (level << 5) | (level << 10));
        }

        if (frame == 51) {
            volatile u16 *palette = (volatile u16 *)0x05000004;

            Func_080e0524(0x7d, runtime, 1, 0);
            for (i = 1; i < 64; i++) {
                s32 red_blue = i / 2;
                s32 green = red_blue / 2;

                palette[i - 1] =
                    (u16)(red_blue | (green << 5) |
                          (red_blue << 10));
            }
            *(volatile u16 *)0x04000050 = 0x3f44;
            for (i = 0; i < 32; i++) {
                struct Particle_080cbc0c *particle = &particles[i];

                particle->x =
                    ((Func_08004458() & 31) + 32) << 16;
                particle->y =
                    ((Func_08004458() & 31) + 80) << 16;
                particle->velocity_x =
                    ((Func_08004458() & 0x1ff) - 0x100) << 12;
                particle->velocity_y = 0;
                particle->age = 0;
            }
            *(s32 *)(runtime + 0x7780) = 2;
            *(s32 *)(runtime + 0x7784) = 50;
        }

        if (frame > 52) {
            for (i = 0; i < 32; i++) {
                struct Particle_080cbc0c *particle = &particles[i];

                if (frame >= i / 4 + 52 && particle->age <= 39) {
                    s32 half_size = particle->age / 4;

                    if (half_size > 5)
                        half_size = 5;
                    renderers[1](
                        render_context,
                        runtime + (half_size << 11),
                        *(s16 *)((u8 *)&particle->x + 2) - 16,
                        *(s16 *)((u8 *)&particle->y + 2) - 32,
                        32,
                        64);
                    Func_080e3908(particle, 60, 0xffffff00);
                    particle->age++;
                }
            }
        }

        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_08004278((const void *)0x080cd261);
    Func_080d6888(
        *(s16 *)(*(u8 **)(runtime + 0x7828) + 36),
        -1, 1, 0, 0);
    *(volatile u16 *)0x03001ad4 =
        *(u16 *)(runtime + 0x77a0);
    *(volatile u16 *)0x03001ad6 = 32;
    Func_080b5038(2, *(u16 *)(global_runtime + 0x648), 0);
    Func_080030f8(1);
    QueueTransfer_080cbc0c(0x00007541, 0x04000000, 0x01000000);
    Func_08002dd8(41);
    Func_08002dd8(40);
    Func_08002dd8(39);
    (void)small_graphics;
}
