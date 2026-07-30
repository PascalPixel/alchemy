#include "layout_guard.h"
#include "types.h"

struct Particle_080e2538 {
    s32 x;
    s32 y;
    s32 scale;
    s32 velocity_x;
    s32 velocity_y;
    s32 unknown_14;
    s32 phase;
};

typedef void (*Renderer_080e2538)(
    void *,
    const void *,
    s32,
    s32,
    s32,
    s32);

LAYOUT_SIZE_GUARD(Particle080e2538, struct Particle_080e2538, 0x1c);

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
s32 Func_08002304(s32, s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(volatile void *);
u32 Func_08004458(void);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e3908(void *, s32, s32);
void Func_080e396c(s16, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080e2538(void *object)
{
    static const u8 *const point_pairs = (const u8 *)0x080eecb2;
    static const s8 *const group_x = (const s8 *)0x080eecf2;
    static const u8 *const start_times = (const u8 *)0x080eecf7;
    static const u8 *const group_counts = (const u8 *)0x080eecfc;
    static const u8 *const widths = (const u8 *)0x080eecff;
    static const u8 *const heights = (const u8 *)0x080eed0e;
    static const u16 *const image_offsets = (const u16 *)0x080eed1e;
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    Renderer_080e2538 render;
    struct Particle_080e2538 *anchors =
        (struct Particle_080e2538 *)(runtime + 0x7080);
    struct Particle_080e2538 *particles =
        (struct Particle_080e2538 *)0x02010000;
    s32 first_point[3];
    s32 last_point[3];
    s32 group_count;
    s32 frame;
    s32 group;

    *(void **)(runtime + 0x7828) = object;
    Func_080cd594(1);
    *(volatile u16 *)0x04000020 = 0x0100;
    *(volatile u16 *)0x04000050 = 0;
    Func_080e0524(0x8a, runtime, 1, 1);

    Func_080ed408(46, 7, 7, 3, 1);
    render = (Renderer_080e2538)runtime_header[7];

    Func_080e396c(*(s16 *)((u8 *)object + 36), first_point);
    Func_080e396c(
        *(s16 *)((u8 *)object + 34 +
                 *(s32 *)((u8 *)object + 20) * 2),
        last_point);
    first_point[0] += (last_point[0] - first_point[0]) / 2;
    *(volatile s32 *)0x04000028 = (64 - first_point[0]) << 8;

    *(s32 *)(runtime + 0x7780) = 1;
    *(s32 *)(runtime + 0x7784) = 0;
    Func_080041d8((const void *)0x080cd261, 0x480);

    group_count =
        group_counts[*(s32 *)((u8 *)object + 24)];
    for (group = 0; group < group_count; group++) {
        s32 index;

        anchors[group].y = -0x400000;
        anchors[group].velocity_y = 0;
        for (index = 0; index < 21; index++) {
            struct Particle_080e2538 *particle =
                &particles[group * 21 + index];

            particle->x =
                (point_pairs[index * 2] + group_x[group]) * 0x10000;
            particle->y = point_pairs[index * 2 + 1] << 16;
            particle->velocity_x =
                (Func_08002304(Func_08004458(), 96) - 48) *
                0x400;
            particle->velocity_y =
                -((Func_08004458() & 127) + 32) * 0x800;
            particle->scale = 32;
            particle->phase = 0;
        }
    }

    for (frame = 0;
         frame != start_times[group_count - 1] + 80;
         frame++) {
        if (frame == start_times[group_count - 1] + 48)
            Func_080b50e8(132);

        for (group = 0; group < group_count; group++) {
            s32 start = start_times[group];

            if (frame == start + 18) {
                Func_080f9010(134);
                *(s32 *)(runtime + 0x77a8) = 4;
            }

            if (frame >= start + 18) {
                s32 index;

                for (index = 0; index < 21; index++) {
                    struct Particle_080e2538 *particle =
                        &particles[group * 21 + index];
                    s32 image =
                        Func_080022fc(index, 5) * 3 +
                        Func_080022fc(
                            Func_080022ec(particle->phase, 96),
                            3);
                    s32 width = widths[image];
                    s32 height = heights[image];

                    render(
                        render_context,
                        runtime + 0x83c + image_offsets[image],
                        (s16)(particle->x >> 16) - (width >> 1),
                        (s16)(particle->y >> 16) - (height >> 1),
                        width,
                        height);
                    Func_080e3908(particle, 64, 0x4000);
                    particle->phase += particle->scale;
                    if (particle->scale > 1 && (frame & 1))
                        particle->scale--;
                }
            } else {
                struct Particle_080e2538 *anchor = &anchors[group];

                if (frame >= start) {
                    render(
                        render_context,
                        runtime,
                        group_x[group] + 47,
                        (s16)(anchor->y >> 16),
                        34,
                        62);
                }
                anchor->y += anchor->velocity_y;
                if (frame > start)
                    anchor->velocity_y += 0x10000;
                if (anchor->y > 0x320000)
                    anchor->y = 0x320000;
            }

            if (frame == start + 18) {
                s32 count = *(s32 *)((u8 *)object + 20);
                s32 index;

                for (index = 0; index < count; index++) {
                    Func_080d6888(
                        *(s16 *)((u8 *)object + 36 + index * 2),
                        7,
                        5,
                        index,
                        8);
                }
            }
        }

        Func_080e155c(2, 4);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((volatile void *)0x080cd261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
