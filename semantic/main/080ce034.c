#include "types.h"

typedef void (*Transfer_080ce034)(void *, const void *, u32);
typedef void (*Renderer_080ce034)(
    void *, const void *, s32, s32, s32, s32);

struct Scene_080ce034 {
    u8 unknown_00[4];
    s32 direction;
    s32 focus_id;
    u8 unknown_0c[24];
    s16 object_ids[1];
};

struct Object_080ce034 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[16];
    s32 state24;
    s32 state28;
    u8 unknown_2c[4];
    s32 state30;
    s32 state34;
    u8 unknown_38[16];
    s32 state48;
    u8 unknown_4c[12];
    u8 flag58;
    u8 unknown_59;
    u8 flag5a;
};

struct Particle_080ce034 {
    s32 x;
    s32 y;
    s32 velocity_x;
    s32 unknown_0c;
    s32 velocity_y;
    s32 unknown_14;
    s32 life;
};

s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(u32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_08005340(const void *, void *);
void Func_08009080(struct Object_080ce034 *, s32);
void Func_08009140(struct Object_080ce034 *);
void Func_08009150(struct Object_080ce034 *, s32, s32, s32);
void Func_080b5088(s16, s32);
struct Object_080ce034 **Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e155c(s32, u32);
void Func_080e3980(s32, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

static s32 HalfTowardZero_080ce034(s32 value)
{
    return (value + ((u32)value >> 31)) >> 1;
}

void Func_080ce034(struct Scene_080ce034 *scene)
{
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    u8 *graphics = (u8 *)header[2];
    void *projection = *(void **)0x03001e80;
    Transfer_080ce034 transfer = (Transfer_080ce034)0x03001388;
    Renderer_080ce034 renderers[2];
    struct Particle_080ce034 *particles =
        (struct Particle_080ce034 *)0x02010000;
    struct Object_080ce034 *focus;
    struct Object_080ce034 *target;
    s32 position[2];
    s32 horizontal_step;
    s32 frame;
    s32 i;
    void *resource;

    *(struct Scene_080ce034 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    Func_08005340(Func_08002f40(0x73), graphics);
    resource = Func_08002f40(0x7d);
    transfer((void *)0x05000000, resource, 0x80);
    Func_08005340((u8 *)resource + 0x80, runtime);

    Func_080ed408(46, 7, 7, 3, 2);
    renderers[0] = (Renderer_080ce034)header[7];
    Func_080ed408(47, 7, 7, 7, 2);
    renderers[1] = (Renderer_080ce034)header[8];
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (i = 0; i < 1024; i++)
        particles[i].life = 0;

    focus = *Func_080b5098(scene->focus_id);
    target = *Func_080b5098(scene->object_ids[0]);
    horizontal_step = focus->x > 0 ? -0xf0000 : 0xf0000;

    for (frame = 0; frame < 88; frame++) {
        Func_080049ac();
        Func_080051d8(projection, (u8 *)projection + 12);

        if (frame > 17 || frame == 0) {
            Func_080e3980(scene->focus_id, position);
            position[0] = HalfTowardZero_080ce034(position[0]);
        }

        if ((u32)(frame - 2) <= 1)
            renderers[0](
                render_context, runtime,
                position[0] - 16, position[1] - 64, 32, 64);

        if ((u32)(frame - 4) <= 11) {
            s32 animation = (frame - 4) / 2;

            for (i = 0; i < 16; i++) {
                s32 angle = i << 12;
                s32 x = position[0]
                    + ((frame * Func_08002322(angle)) >> 16);
                s32 y = position[1]
                    + ((frame * Func_0800231c(angle)) >> 16);
                renderers[0](
                    render_context,
                    runtime + animation * 0x800,
                    x - 16,
                    y - frame - 64,
                    32,
                    64);
            }
        }

        if (frame == 4) {
            focus->state28 = 0x140000;
            focus->state34 = 0x10000;
            focus->state30 = 0x30000;
            focus->state48 = 0xab85;
            focus->flag5a = 0;
            focus->flag58 = 0;
            Func_08009150(focus, focus->x * 3, 0, focus->z);
            Func_08009080(focus, 2);
            *(s32 *)(runtime + 0x77a8) = 4;
            Func_080f9010(0x88);
        }

        if (frame == 16) {
            resource = Func_08002f40(0x89);
            transfer((void *)0x05000000, resource, 0x80);
            Func_08005340((u8 *)resource + 0x80, runtime);
            focus->state48 = 0;
            focus->state24 = 0;
            focus->state28 = 0;
            focus->z = target->z;
            Func_08009140(focus);
        }

        if (frame > 17) {
            s32 vertical = focus->y;

            if (vertical > 0) {
                focus->x += horizontal_step;
                focus->y = vertical - 0x80000;
                if (scene->direction == 0) {
                    renderers[0](
                        render_context, runtime,
                        position[0] - 20,
                        position[1] - 52,
                        40, 64);
                    position[0] -= 8;
                } else {
                    renderers[1](
                        render_context, runtime,
                        position[0] - 26,
                        position[1] - 52,
                        40, 64);
                    position[1] += 8;
                }
                vertical = focus->y;
            }

            if (vertical < 0) {
                focus->y = 0;
                for (i = 0; i < 256; i++) {
                    s32 speed = (Func_08004458() & 0x3ff) + 32;
                    s32 angle = Func_08004458() & 0xffff;

                    particles[i].x = position[0] << 16;
                    particles[i].y = (position[1] - 24) << 16;
                    particles[i].velocity_x =
                        (speed * Func_08002322(angle)) >> 6;
                    particles[i].velocity_y =
                        -(speed * Func_0800231c(angle) * 2) >> 6;
                    particles[i].life =
                        (Func_08004458() & 7) + 32;
                }
                *(s32 *)(runtime + 0x77a8) = 8;
                Func_080b50e8(0x91);
                Func_080b5088(scene->object_ids[0], 4);
                Func_080d6888(scene->object_ids[0], 7, 5, 0, 8);
            }
        }

        for (i = 0; i < 256; i++) {
            struct Particle_080ce034 *particle = &particles[i];

            if (particle->life > 0) {
                s32 old_life = particle->life;
                s32 width;
                s32 height;

                particle->x += particle->velocity_x;
                particle->y += particle->velocity_y;
                particle->life--;
                particle->velocity_x =
                    (particle->velocity_x * 56) / 64;
                particle->velocity_y =
                    (particle->velocity_y * 56) / 64 + 0x2000;
                if (particle->y > 0x700000) {
                    particle->velocity_y =
                        HalfTowardZero_080ce034(-particle->velocity_y);
                } else if ((u32)particle->x <= 0x7effff
                    && particle->y >= 0) {
                    width = ((old_life - 1) / 8) + 1;
                    height = width * 2;
                    renderers[i & 1](
                        render_context,
                        graphics
                            + *(u16 *)(0x080ede48 + height - 2),
                        (particle->x >> 16) - width / 2,
                        (particle->y >> 16) - width,
                        width,
                        height);
                }
            }
        }

        Func_080e155c(16, 16);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
