#include "types.h"

typedef void (*Transfer_080d1350)(
    void *destination,
    const void *source,
    u32 size);

typedef void (*Renderer_080d1350)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Particle_080d1350 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 active;
};

#define MUL_SHIFT_080D1350(left, right, shift) \
    ((s32)((u32)(left) * (u32)(right)) >> (shift))

s32 Func_080022ec(s32 dividend, s32 divisor);
s32 Func_080022fc(s32 value, s32 modulus);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_08002dd8(s32 arg0);
void *Func_08002f40(s32 arg0);
void Func_080030f8(s32 arg0);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(const void *address);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(void *arg0, void *arg1);
void Func_08005340(void *source, void *destination);
void *Func_080b5098(s32 arg0);
void Func_080b50e8(s32 arg0);
void Func_080cd52c(void);
void Func_080cd594(s32 arg0);
void Func_080cdbc0(void);
void Func_080d40ec(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_080d6888(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080e155c(s32 arg0, s32 arg1);
void Func_080e3944(const struct Particle_080d1350 *input, s32 *output);
void Func_080ed408(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080f9010(s32 arg0);

void Func_080d1350(void *argument)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    void *graphics = (void *)runtime_header[2];
    void *effect_context = *(void **)0x03001e80;
    Transfer_080d1350 transfer = (Transfer_080d1350)0x03001388;
    Renderer_080d1350 renderer;
    struct Particle_080d1350 *particles =
        (struct Particle_080d1350 *)(runtime + 0x7080);
    struct Particle_080d1350 *nodes =
        (struct Particle_080d1350 *)0x02010000;
    void *primary;
    void *secondary;
    void *resource;
    s32 primary_id;
    s32 secondary_id;
    s32 frame;
    s32 i;

    *(void **)(runtime + 0x7828) = argument;
    Func_080cd594(1);

    resource = Func_08002f40(0x79);
    transfer((void *)0x05000000, resource, 0x80);
    resource = Func_08002f40(0x73);
    Func_08005340(resource, graphics);

    Func_080ed408(46, 7, 7, 3, 2);
    renderer = (Renderer_080d1350)runtime_header[7];
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 50;
    Func_080041d8((const void *)0x080cd261, 0x480);

    primary_id = *(s32 *)(*(u8 **)(runtime + 0x7828) + 8);
    secondary_id = *(s16 *)(*(u8 **)(runtime + 0x7828) + 36);
    primary = *(void **)Func_080b5098(primary_id);
    secondary = *(void **)Func_080b5098(secondary_id);

    for (i = 0; i < 8; i++) {
        s32 random_x;

        particles[i].x = *(s32 *)((u8 *)primary + 8) / 2;
        particles[i].y = *(s32 *)((u8 *)primary + 12) + 0x780000;
        particles[i].z = *(s32 *)((u8 *)primary + 16);
        random_x = (s32)(Func_08004458() & 0x7f) - 64;
        particles[i].velocity_x = Func_080022ec(
            *(s32 *)((u8 *)secondary + 8) +
                (random_x * 0x10000) -
                particles[i].x,
            12);
        particles[i].velocity_y = Func_080022ec(
            *(s32 *)((u8 *)secondary + 12) -
                particles[i].y +
                0x140000,
            12);
        particles[i].velocity_z = Func_080022ec(
            *(s32 *)((u8 *)secondary + 16) - particles[i].z,
            12);
        particles[i].active = (Func_08004458() & 15) + (i * 8);
    }

    for (frame = 0; frame < 128; frame++) {
        Func_080d40ec(frame, 0xaaab, 0x5555, 0);
        if (frame == 96)
            Func_080b50e8(134);

        for (i = 0; i < 8; i++) {
            struct Particle_080d1350 *particle = &particles[i];
            s32 node_base = i * 10;

            if (frame >= particle->active) {
                s32 projected[3];

                Func_080049ac();
                Func_080051d8(
                    effect_context,
                    (u8 *)effect_context + 12);
                Func_080e3944(particle, projected);
                projected[0] >>= 1;

                if ((u32)(projected[0] + 8) <= 135 &&
                    projected[1] <= 127 &&
                    projected[1] >= -8) {
                    s32 j;

                    for (j = 0; j < 10; j++) {
                        struct Particle_080d1350 *node =
                            &nodes[node_base + j];
                        s32 angle =
                            (j * 0xaaab) -
                            ((frame - particle->active) << 11);
                        s32 amplitude =
                            ((const u8 *)0x080ee158)[j & 1];
                        s32 x_product = (s32)(
                            (u32)amplitude *
                            (u32)Func_08002322(angle));
                        s32 y_product = (s32)(
                            (u32)amplitude *
                            (u32)Func_0800231c(angle));

                        node->velocity_x =
                            projected[0] +
                            ((s32)(
                                (u32)x_product +
                                ((u32)x_product >> 31)) >>
                             17);
                        node->velocity_y =
                            projected[1] - (y_product >> 16);
                    }

                    for (j = 0; j < 10; j++) {
                        s32 current_index = node_base + j;
                        s32 next_index =
                            node_base + Func_080022fc(j + 1, 10);
                        struct Particle_080d1350 *current =
                            &nodes[current_index];
                        struct Particle_080d1350 *next =
                            &nodes[next_index];
                        s32 step;

                        for (step = 0; step < 12; step++) {
                            s32 x = current->velocity_x +
                                Func_080022ec(
                                    (s32)(
                                        (u32)step *
                                        ((u32)next->velocity_x -
                                         (u32)current->velocity_x)),
                                    12);
                            s32 y = current->velocity_y +
                                Func_080022ec(
                                    (s32)(
                                        (u32)step *
                                        ((u32)next->velocity_y -
                                         (u32)current->velocity_y)),
                                    12);

                            renderer(
                                render_context,
                                (u8 *)graphics +
                                    ((const u16 *)0x080ede48)[1],
                                x - 1,
                                y - 2,
                                2,
                                4);
                        }
                    }
                }

                if (particle->y <= 0x199a) {
                    s32 count;
                    s32 object_index;

                    particle->velocity_y = -particle->velocity_y;
                    particle->velocity_x /= 2;
                    particle->velocity_z /= 2;
                    *(s32 *)(runtime + 0x77a8) = 4;
                    Func_080f9010(134);

                    count = *(s32 *)(*(u8 **)(runtime + 0x7828) + 20);
                    for (object_index = 0; object_index < count;
                         object_index++) {
                        Func_080d6888(
                            *(s16 *)(
                                *(u8 **)(runtime + 0x7828) +
                                36 +
                                (object_index * 2)),
                            7,
                            5,
                            object_index,
                            8);
                    }
                }

                particle->x += particle->velocity_x;
                particle->y += particle->velocity_y;
                particle->z += particle->velocity_z;
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
