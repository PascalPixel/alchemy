#include "types.h"

typedef void (*Transfer_080d85d0)(
    void *destination,
    const void *source,
    u32 size);

typedef void (*Renderer_080d85d0)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Particle_080d85d0 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 active;
};

void Func_08002dd8(s32 arg0);
void *Func_08002f40(s32 arg0);
void Func_080030f8(s32 arg0);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(volatile void *address);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(void *arg0, void *arg1);
void *Func_080b5098(s32 arg0);
s32 Func_080b5070(s32 arg0);
void Func_080b50e8(s32 arg0);
void Func_080cd52c(void);
void Func_080cd594(s32 arg0);
void Func_080cdbc0(void);
void Func_080d6888(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080e0524(s32 arg0, void *arg1, s32 arg2, s32 arg3);
void Func_080e38b8(struct Particle_080d85d0 *particle, s32 arg1, s32 arg2);
void Func_080e3944(const struct Particle_080d85d0 *input, s32 *output);
void Func_080ed408(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080f9010(s32 arg0);

void Func_080d85d0(void *argument)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    void *graphics = (void *)runtime_header[2];
    void *effect_context = *(void **)0x03001e80;
    Transfer_080d85d0 transfer = (Transfer_080d85d0)0x03001388;
    Renderer_080d85d0 renderer;
    struct Particle_080d85d0 *particles =
        (struct Particle_080d85d0 *)0x02010000;
    void *buffer;
    s32 count;
    s32 frame;
    s32 total_frames;
    s32 i;

    *(void **)(runtime + 0x7828) = argument;
    Func_080cd594(1);
    Func_080e0524(0x73, graphics, 0, 0);

    buffer = Func_08002f40(*(s32 *)((u8 *)argument + 24) == 0 ? 0xb9 : 0xc0);
    transfer((void *)0x05000000, buffer, 0x80);

    for (i = 0; i < 1024; i++)
        particles[i].active = -1;

    count = *(s32 *)(*(u8 **)(runtime + 0x7828) + 20);
    for (i = 0; i < count; i++) {
        s32 id =
            *(s16 *)(*(u8 **)(runtime + 0x7828) + 36 + (i * 2));
        void *object = *(void **)Func_080b5098(id);
        s32 vertical_offset = Func_080b5070(id) / 2;
        s32 j;

        for (j = 0; j < 128; j++) {
            struct Particle_080d85d0 *particle = &particles[(i * 128) + j];

            particle->x = *(s32 *)((u8 *)object + 8);
            particle->y = *(s32 *)((u8 *)object + 12) + vertical_offset;
            particle->z = *(s32 *)((u8 *)object + 16);
            particle->velocity_x =
                ((s32)(Func_08004458() & 0xff) - 128) * 0x400;
            particle->velocity_y =
                ((s32)(Func_08004458() & 0xff) - 128) * 0x400;
            particle->velocity_z =
                ((s32)(Func_08004458() & 0xff) - 128) * 0x400;
            particle->active = 0;
        }
    }

    Func_080ed408(46, 7, 7, 3, 2);
    renderer = (Renderer_080d85d0)runtime_header[7];
    *(s32 *)(runtime + 0x7780) = 3;
    *(s32 *)(runtime + 0x7784) = 0x04040404;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080f9010(142);

    count = *(s32 *)(*(u8 **)(runtime + 0x7828) + 20);
    total_frames = (count * 20) + 72;
    if (total_frames != 0) {
        frame = 0;
        do {
            s32 primary_id =
                *(s32 *)(*(u8 **)(runtime + 0x7828) + 8);
            void *primary_object = *(void **)Func_080b5098(primary_id);
            s32 vertical_offset = Func_080b5070(primary_id) / 2;

            if (frame == 64)
                Func_080b50e8(133);

            Func_080049ac();
            Func_080051d8(effect_context, (u8 *)effect_context + 12);

            if (frame == 40)
                Func_080d6888(primary_id, 7, -1, -1, 0);

            count = *(s32 *)(*(u8 **)(runtime + 0x7828) + 20);
            if (frame == (count * 20) + 52)
                Func_080d6888(primary_id, 0, -1, -1, 0);

            for (i = 0; i < count; i++) {
                s32 start_frame = i * 20;
                s32 id =
                    *(s16 *)(*(u8 **)(runtime + 0x7828) + 36 + (i * 2));
                s32 j;

                if (frame == start_frame)
                    Func_080d6888(id, 7, 5, i, 42);

                if (frame <= start_frame)
                    continue;

                for (j = 0; j < 32; j++) {
                    struct Particle_080d85d0 *particle =
                        &particles[(i * 128) + j];

                    if (particle->active < 0)
                        continue;

                    {
                        s32 projected[3];
                        s32 delta_x;
                        s32 delta_y;
                        s32 delta_z;

                        Func_080e3944(particle, projected);
                        projected[0] >>= 1;
                        renderer(
                            render_context,
                            (u8 *)graphics +
                                *(const u16 *)(0x080ede48 + 10),
                            projected[0] - 3,
                            projected[1] - 6,
                            6,
                            12);
                        Func_080e38b8(particle, 62, 0);

                        if (frame <= start_frame + j + 10)
                            continue;

                        delta_x =
                            (*(s32 *)((u8 *)primary_object + 8) - particle->x) >>
                            8;
                        delta_y =
                            (*(s32 *)((u8 *)primary_object + 12) +
                             vertical_offset - particle->y) >> 8;
                        delta_z =
                            (*(s32 *)((u8 *)primary_object + 16) - particle->z) >>
                            8;
                        particle->velocity_x += delta_x;
                        particle->velocity_y += delta_y;
                        particle->velocity_z += delta_z;

                        if ((u32)(delta_x + 0xfff) <= 0x1ffe &&
                            (u32)(delta_z + 0xfff) <= 0x1ffe) {
                            particle->active = -1;
                        }
                    }
                }
            }

            Func_080cd52c();
            *(s32 *)(runtime + 0x7824) = 1;
            Func_080030f8(1);
            frame++;
        } while (
            frame !=
            (*(s32 *)(*(u8 **)(runtime + 0x7828) + 20) * 20) + 72);
    }

    Func_08004278((volatile void *)0x04000028);
    Func_08002dd8(46);
    Func_080cdbc0();
}
