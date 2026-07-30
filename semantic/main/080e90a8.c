#include "types.h"

typedef void (*Renderer_080e90a8)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Scene_080e90a8 {
    u8 unknown_00[4];
    s32 direction;
    s32 focus_id;
    u8 unknown_0c[12];
    s32 alternate;
    u8 unknown_1c[8];
    s16 object_ids[1];
};

struct Particle_080e90a8 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 phase;
};

struct Object_080e90a8 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Dma_080e90a8 {
    const void *source;
    void *destination;
    u32 control;
};

void Func_08002dd8(s32);
void Func_080030f8(u32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_080b5078(s32, s16, s32, s32);
void Func_080b5088(s16, s32);
struct Object_080e90a8 **Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080cef64(s32, Renderer_080e90a8 *);
void Func_080d655c(s32);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, u32);
void Func_080e38b8(struct Particle_080e90a8 *, s32, s32);
void Func_080e3944(const struct Particle_080e90a8 *, s32 *);
void Func_080e396c(s32, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

static s32 HalfTowardZero_080e90a8(s32 value)
{
    return (value + ((u32)value >> 31)) >> 1;
}

void Func_080e90a8(struct Scene_080e90a8 *scene)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    u8 *graphics = (u8 *)runtime_header[2];
    void *effect_context = *(void **)0x03001e80;
    u8 *portrait_frames = (u8 *)0x02010000;
    struct Particle_080e90a8 *particles =
        (struct Particle_080e90a8 *)(runtime + 0x7080);
    struct Object_080e90a8 *object;
    Renderer_080e90a8 renderer;
    s32 object_position[2];
    s32 focus_position[2];
    s32 frame;
    s32 i;

    *(struct Scene_080e90a8 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    Func_080e0524(0x96, runtime, 1, 1);
    Func_080e0524(0x63, portrait_frames, 1, 1);
    Func_080e0524(0x73, graphics, 0, 0);
    Func_080b5078(scene->focus_id, scene->object_ids[0], 4, 0);
    Func_080030f8(1);

    object = *Func_080b5098(scene->object_ids[0]);
    for (i = 0; i < 64; i++) {
        particles[i].x = object->x;
        particles[i].y = object->y;
        particles[i].z = object->z;
        particles[i].velocity_x = (Func_08004458() & 0xff) << 11;
        particles[i].velocity_y =
            ((Func_08004458() & 0xff) - 127) << 12;
        particles[i].velocity_z =
            ((Func_08004458() & 0xff) - 127) << 12;
        if (particles[i].x > 0)
            particles[i].velocity_x = -particles[i].velocity_x;
        particles[i].phase = (i / 4) * 2 + 16;
    }

    Func_080e396c(scene->object_ids[0], object_position);
    Func_080041d8((const void *)0x080cd261, 0x480);
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;

    for (frame = 0; frame < 64; frame++) {
        u8 *large_frame = runtime - 0x5100 + frame * 0xd80;

        if (frame == 8)
            Func_080b50e8(0x86);
        if (scene->alternate != 0 && frame == 8)
            Func_080f9010(0xd4);

        Func_080e396c(scene->focus_id, focus_position);
        if ((u32)(frame - 6) <= 5) {
            if (scene->direction == 0) {
                Func_080ed408(46, 7, 7, 3, 3);
                renderer = *(Renderer_080e90a8 *)0x03001f08;
                renderer(
                    render_context,
                    large_frame,
                    HalfTowardZero_080e90a8(focus_position[0]) - 24,
                    focus_position[1] - 24,
                    48,
                    72);
            } else {
                Func_080ed408(46, 7, 7, 7, 3);
                renderer = *(Renderer_080e90a8 *)0x03001f08;
                renderer(
                    render_context,
                    large_frame,
                    HalfTowardZero_080e90a8(focus_position[0]),
                    focus_position[1] - 24,
                    48,
                    72);
            }
            Func_08002dd8(46);
        }

        if ((u32)(frame - 16) <= 31) {
            s32 animation = (frame - 16) / 2;
            s32 bank = scene->alternate != 0 ? 0x2580 : 0;

            Func_080ed408(46, 7, 7, 3, 2);
            renderer = *(Renderer_080e90a8 *)0x03001f08;
            if (animation > 2)
                animation = 2;
            renderer(
                render_context,
                portrait_frames + bank + animation * 0xc80,
                HalfTowardZero_080e90a8(object_position[0]) - 20,
                object_position[1] - 48,
                40,
                80);
            Func_080d655c(0x2710);
            Func_08002dd8(46);
        }

        if (frame == 8) {
            volatile struct Dma_080e90a8 *dma =
                (volatile struct Dma_080e90a8 *)0x040000d4;
            static const u32 fill = 0x3f3f3f3f;

            dma->source = &fill;
            dma->destination = render_context;
            dma->control = 0x85001000;
        }

        Func_080049ac();
        Func_080051d8(effect_context, (u8 *)effect_context + 12);
        if (frame > 3) {
            Renderer_080e90a8 renderers[2];

            Func_080cef64(scene->direction, renderers);
            for (i = 0; i < 64; i++) {
                struct Particle_080e90a8 *particle = &particles[i];

                if (particle->phase > 0) {
                    s32 projected[2];
                    s32 width = (particle->phase >> 3) + 2;
                    s32 height = width * 2;

                    Func_080e3944(particle, projected);
                    projected[0] >>= 1;
                    projected[1] += object_position[1] - 112;
                    renderers[(i / 2) & 1](
                        render_context,
                        graphics + *(u16 *)(0x080ede48 + height - 2),
                        projected[0] - width / 2,
                        projected[1] - width,
                        width,
                        height);
                    Func_080e38b8(particle, 60, -0x400);
                    particle->phase--;
                }
            }
            Func_08002dd8(47);
            Func_08002dd8(46);
        }

        if (frame == 8) {
            Func_080b5088(scene->object_ids[0], 4);
            *(s32 *)(runtime + 0x77a8) = 4;
        }
        if (frame == 6)
            Func_080d6888(scene->object_ids[0], 10, -1, -1, 0);
        if (frame == 14)
            Func_080d6888(scene->object_ids[0], 10, -1, -1, 0);

        Func_080e155c(16, 16);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_080cdbc0();
}
