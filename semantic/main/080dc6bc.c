#include "types.h"

typedef void (*Transfer_080dc6bc)(
    void *destination,
    const void *source,
    u32 size);

typedef void (*Renderer_080dc6bc)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Scene_080dc6bc {
    u8 unknown_00[20];
    s32 object_count;
    u8 unknown_18[12];
    s16 object_ids[1];
};

struct Particle_080dc6bc {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 unknown_14;
    s32 phase;
};

struct Object_080dc6bc {
    u8 unknown_00[8];
    s32 x;
    s32 unknown_0c;
    s32 z;
};

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(u32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_08004cb4(void *);
void Func_080051d8(void *, void *);
struct Object_080dc6bc **Func_080b5098(s32);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e38b8(struct Particle_080dc6bc *, s32, s32);
void Func_080e3944(const struct Particle_080dc6bc *, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);

void Func_080dc6bc(struct Scene_080dc6bc *scene)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    Renderer_080dc6bc renderers[2];
    Transfer_080dc6bc transfer = (Transfer_080dc6bc)0x03001388;
    struct Particle_080dc6bc *particles =
        (struct Particle_080dc6bc *)(runtime + 0x7080);
    void *projection_context = *(void **)0x03001e80;
    s32 frame;
    s32 i;

    *(struct Scene_080dc6bc **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    Func_080e0524(0x9e, runtime, 1, 1);
    Func_080e0524(0x6c, runtime + 0x1b00, 0, 0);
    transfer((void *)0x05000000, Func_08002f40(0xbb), 0x80);

    Func_080ed408(46, 7, 7, 3, 3);
    renderers[0] = (Renderer_080dc6bc)runtime_header[7];
    Func_080ed408(47, 7, 7, 3, 2);
    renderers[1] = (Renderer_080dc6bc)runtime_header[8];

    for (i = 0; i < 64; i++) {
        s32 angle = Func_08004458() & 0xffff;
        s32 speed = Func_08004458() & 0xff;

        particles[i].x = 0;
        particles[i].y = 0;
        particles[i].z = 0;
        if (Func_080022fc(i, 6) == 5) {
            particles[i].velocity_x = 0;
            particles[i].velocity_y = 0;
        } else {
            particles[i].velocity_x =
                (speed * Func_08002322(angle)) >> 7;
            particles[i].velocity_y =
                (speed * Func_0800231c(angle)) >> 9;
        }
        particles[i].unknown_14 = 0;
        particles[i].phase = 0;
    }

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (frame = 0; frame < 96; frame++) {
        for (i = 0; i < scene->object_count; i++) {
            struct Object_080dc6bc *object =
                *Func_080b5098(scene->object_ids[i]);
            s32 projection_input[3];
            s32 particle_index;

            Func_080049ac();
            Func_080051d8(
                projection_context,
                (u8 *)projection_context + 12);
            projection_input[0] = object->x;
            projection_input[1] = 0x140000;
            projection_input[2] = object->z;
            Func_08004cb4(projection_input);

            if (frame < i * 8 || frame >= i * 8 + 40)
                continue;

            for (particle_index = 0; particle_index < 6;
                 particle_index++) {
                struct Particle_080dc6bc *particle =
                    &particles[i * 6 + particle_index];
                s32 animation = Func_080022ec(particle->phase, 6);
                s32 projected[2];

                if (animation > 5)
                    animation = 5;
                Func_080e3944(particle, projected);
                projected[0] >>= 1;

                if (particle_index == 5) {
                    Func_080e38b8(particle, 62, 0x800);
                    renderers[0](
                        render_context,
                        runtime + 0x1b00 + animation * 0x480,
                        projected[0] - 12,
                        projected[1] - 36,
                        24,
                        48);
                } else {
                    Func_080e38b8(particle, 60, 0x200);
                    renderers[1](
                        render_context,
                        runtime + animation * 0x480,
                        projected[0] - 12,
                        projected[1] - 36,
                        24,
                        48);
                }
                particle->phase++;
            }
        }

        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
