#include "types.h"

typedef void (*Renderer_080e0564)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Object_080e0564 {
    s32 x;
    s32 y;
    s32 unused08;
    s32 velocity_x;
    s32 velocity_y;
    s32 unused14;
    s32 active;
};

#define MUL_SHIFT_080E0564(left, right, shift) \
    ((s32)((u32)(left) * (u32)(right)) >> (shift))

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
void Func_080d6888(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080e0524(s32 arg0, void *destination, s32 arg2, s32 arg3);
void Func_080e155c(s32 arg0, s32 arg1);
void Func_080e3908(struct Object_080e0564 *object, s32 arg1, s32 arg2);
void Func_080ed408(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080f9010(s32 arg0);

void Func_080e0564(void *argument)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    void *graphics = (void *)runtime_header[2];
    Renderer_080e0564 renderers[2];
    struct Object_080e0564 *points =
        (struct Object_080e0564 *)(runtime + 0x7080);
    struct Object_080e0564 *particles =
        (struct Object_080e0564 *)0x02010000;
    s32 frame;
    s32 orbit_angle;
    s32 i;

    *(void **)(runtime + 0x7828) = argument;
    Func_080cd594(0);
    *(volatile u16 *)0x04000052 = 0x1010;

    Func_080ed408(46, 7, 7, 11, 2);
    renderers[0] = (Renderer_080e0564)runtime_header[7];
    Func_080ed408(47, 7, 7, 3, 3);
    renderers[1] = (Renderer_080e0564)runtime_header[8];

    Func_080e0524(0x73, graphics, 0, 0);
    Func_080e0524(0x94, runtime, 1, 1);
    Func_080e0524(0x6f, runtime + 0x2f8, 1, 0);

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (i = 0; i < 32; i++) {
        points[i].x = Func_08004458() & 63;
        points[i].y = 104;
    }
    for (i = 0; i < 512; i++)
        particles[i].active = -1;

    Func_080f9010(141);
    orbit_angle = 0x8000;

    for (frame = 0; frame < 96; frame++, orbit_angle -= 0x800) {
        if (frame <= 79) {
            s32 x = MUL_SHIFT_080E0564(
                24,
                Func_08002322(orbit_angle),
                16) + 22;
            s32 y = MUL_SHIFT_080E0564(
                64 - (frame * 2),
                Func_0800231c(orbit_angle),
                16) + 29;

            renderers[1](
                render_context,
                runtime,
                x,
                y,
                20,
                38);
        }

        if (frame == 56)
            Func_080b50e8(133);

        for (i = 0; i < 10; i++) {
            struct Object_080e0564 *point = &points[i];
            s32 start_frame = 16 + (i * 4);

            if (frame >= start_frame) {
                renderers[0](
                    render_context,
                    runtime + 0x9e0,
                    point->x - 17,
                    point->y - 32,
                    34,
                    65);

                if (frame == start_frame) {
                    struct Object_080e0564 *group = &particles[i * 32];
                    s32 j;

                    for (j = 0; j < 16; j++) {
                        s32 angle =
                            (Func_08004458() & 0x7fff) + 0x4000;
                        s32 speed =
                            (Func_08004458() & 0x1ff) + 0x100;

                        group[j].x = (s32)((u32)point->x << 16);
                        group[j].y =
                            (s32)((u32)(point->y + 16) << 16);
                        group[j].velocity_x = MUL_SHIFT_080E0564(
                            speed,
                            Func_08002322(angle),
                            7);
                        group[j].velocity_y = MUL_SHIFT_080E0564(
                            speed,
                            Func_0800231c(angle),
                            6);
                        group[j].active =
                            (Func_08004458() & 15) + 32;
                    }

                    if ((i & 1) != 0)
                        Func_080f9010(133);

                    *(s32 *)(runtime + 0x77a8) = 4;
                    {
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

                point->y -= 12;
            }
        }

        for (i = 0; i < 512; i++) {
            struct Object_080e0564 *particle = &particles[i];

            if (particle->active != -1) {
                s32 phase = particle->active / 16 + 2;
                const void *source =
                    (u8 *)graphics +
                    ((const u16 *)0x080ede48)[phase - 1];

                renderers[1](
                    render_context,
                    source,
                    (s16)((u32)particle->x >> 16) - (phase / 2),
                    (s16)((u32)particle->y >> 16) - phase,
                    phase,
                    phase * 2);
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
