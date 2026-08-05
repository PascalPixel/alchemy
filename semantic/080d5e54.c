#include "types.h"

typedef void (*Renderer_080d5e54)(
    void *, const void *, s32, s32, s32, s32);
typedef s32 (*SquareRoot_080d5e54)(u32);

struct Scene_080d5e54 {
    u8 unknown_00[4];
    s32 direction;
    s32 target_id;
    u8 unknown_0c[8];
    s32 object_count;
    u8 unknown_18[12];
    s16 object_ids[1];
};

struct Object_080d5e54 {
    u8 unknown_00[8];
    s32 x;
    u8 unknown_0c[4];
    s32 z;
};

struct ObjectContext_080d5e54 {
    struct Object_080d5e54 *object;
};

struct Particle_080d5e54 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 age;
};

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(volatile void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080049e8(void);
void Func_08004a5c(void);
void Func_08004bd4(s32);
void Func_08004c1c(s32);
void Func_08004c6c(s32);
void Func_08004cb4(s32 *);
void Func_080051d8(void *, void *);
struct ObjectContext_080d5e54 *Func_080b5098(s32);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080cef64(s32, Renderer_080d5e54 *);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080e0524(void *, void *, s32, s32);
void Func_080e38b8(struct Particle_080d5e54 *, s32, s32);
void Func_080e3944(const s32 *, s32 *);
void Func_080f9010(s32);

void Func_080d5e54(struct Scene_080d5e54 *scene)
{
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    u8 *graphics = (u8 *)header[2];
    struct Particle_080d5e54 *particles =
        (struct Particle_080d5e54 *)0x02010000;
    Renderer_080d5e54 renderers[2];
    struct Object_080d5e54 *target;
    struct Object_080d5e54 *destination;
    s32 position[3];
    s32 destination_position[3];
    s32 velocity[3];
    u32 frame;
    u32 i;

    *(struct Scene_080d5e54 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    Func_080e0524((void *)0x92, runtime, 1, 1);
    Func_080e0524((void *)0x73, graphics, 0, 0);
    Func_080cef64(scene->direction ^ 1, renderers);

    target = Func_080b5098(scene->target_id)->object;
    destination = Func_080b5098(scene->object_ids[0])->object;

    for (i = 0; i < 64; i++) {
        s32 angle = Func_08004458() & 0xffff;
        s32 speed = (Func_08004458() & 0xff) + 128;

        particles[i].x = 0;
        particles[i].y =
            (s32)((Func_08004458() & 31) + 20) << 16;
        particles[i].z = 0;
        particles[i].velocity_x =
            speed * Func_08002322(angle) >> 5;
        particles[i].velocity_y = 0;
        particles[i].velocity_z =
            speed * Func_0800231c(angle) >> 5;
        particles[i].age = 0;
    }

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    position[0] = target->x;
    position[1] = 0;
    position[2] = target->z;
    destination_position[0] = destination->x;
    destination_position[1] = 0x5a0000;
    destination_position[2] = 0;
    velocity[0] =
        Func_080022ec(destination_position[0] - position[0], 40);
    velocity[1] =
        Func_080022ec(destination_position[1] - position[1], 40);
    velocity[2] =
        Func_080022ec(destination_position[2] - position[2], 40);

    for (frame = 0; frame < 98; frame++) {
        void *effect_context = *(void **)0x03001e80;

        if (frame == 8)
            Func_080f9010(212);
        if (frame == 80)
            Func_080f9010(142);

        Func_080049ac();
        Func_080051d8(effect_context, (u8 *)effect_context + 12);
        if (frame >= 30 && frame <= 69) {
            position[0] += velocity[0];
            position[1] += velocity[1];
            position[2] += velocity[2];
        }
        Func_08004cb4(position);

        if (frame == 0)
            Func_080d6888(scene->target_id, 7, -1, -1, 0);
        if (frame == 24)
            Func_080d6888(scene->object_ids[0], 0, -1, -1, 0);

        for (i = 0; i < 32; i++) {
            struct Particle_080d5e54 *particle = &particles[i];

            if (frame > i && particle->age == 0) {
                s32 projected[3];
                s32 image;
                s32 time = frame * (256 + i * 32);

                Func_080049e8();
                switch (i & 3) {
                case 0:
                    Func_08004c1c(time);
                    break;
                case 1:
                    Func_08004bd4(-time);
                    break;
                case 2:
                    Func_08004c6c(-time);
                    break;
                default:
                    Func_08004bd4(-time);
                    Func_08004c6c(-time);
                    break;
                }

                Func_080e3944(&particle->x, projected);
                projected[0] /= 2;
                Func_08004a5c();
                if (projected[2] < 250)
                    projected[2] = 250;
                if (projected[2] > 634)
                    projected[2] = 634;
                image = 9 - ((projected[2] - 250) / 64);

                renderers[0](
                    render_context,
                    graphics + *(u16 *)(0x080ede48 + (image - 1) * 2),
                    projected[0] - image / 2,
                    projected[1] - image,
                    image, image * 2);

                Func_080e38b8(particle, 60, 0);
                if (frame > i + 30) {
                    particle->velocity_x += -particle->x >> 8;
                    particle->velocity_y += -particle->y >> 8;
                    particle->velocity_z += -particle->z >> 8;
                }
            }
        }

        if (frame > 82) {
            s32 orbit[3];
            s32 projected[3];

            orbit[0] = 0;
            orbit[1] = Func_08002322(frame << 10) << 2;
            orbit[2] = 0;
            Func_080e3944(orbit, projected);
            projected[0] /= 2;
            renderers[1](render_context, runtime,
                         projected[0] - 10, projected[1] - 17,
                         20, 34);
        }

        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    for (i = 0; i < 512; i++) {
        particles[i].x =
            (s32)((Func_08004458() & 0xff) - 127) << 15;
        particles[i].y =
            (s32)((Func_08004458() & 0x7f) + 64) << 15;
        particles[i].z =
            (s32)((Func_08004458() & 0xff) - 127) << 15;
        particles[i].age = 0;
    }
    Func_080e0524((void *)0xba, graphics, 0, 0);

    for (frame = 0; frame < scene->object_count * 8 + 72; frame++) {
        void *effect_context = *(void **)0x03001e80;
        s32 projectile[3];
        s32 projected[3];

        Func_080049ac();
        Func_080051d8(effect_context, (u8 *)effect_context + 12);

        if (frame >= scene->object_count * 8 + 40)
            destination_position[1] += 0x10000;
        projectile[0] = destination_position[0];
        projectile[1] = destination_position[1];
        projectile[2] = destination_position[2] +
            Func_08002322(frame << 11) * 40;
        Func_080e3944(projectile, projected);
        projected[0] /= 2;
        renderers[0](render_context, runtime,
                     projected[0] - 10, projected[1] - 17,
                     20, 34);

        for (i = 0; i < scene->object_count; i++) {
            s32 id = scene->object_ids[i];
            struct Object_080d5e54 *object =
                Func_080b5098(id)->object;
            s32 base_frame = i * 8;
            u32 particle_index;

            Func_080049ac();
            Func_080051d8(effect_context, (u8 *)effect_context + 12);
            position[0] = object->x;
            position[1] = 0x280000;
            position[2] = object->z;
            Func_08004cb4(position);

            if (frame == base_frame + 30)
                Func_080f9010(126);
            if (frame == base_frame + 40)
                Func_080d6888(id, 7, -1, -1, 0);
            if (frame == base_frame + 64)
                Func_080d6888(id, 0, -1, -1, 0);

            if (frame > base_frame) {
                Func_08004c1c((frame - base_frame) << 9);

                for (particle_index = 0;
                     particle_index < 64;
                     particle_index++) {
                    struct Particle_080d5e54 *particle =
                        &particles[i * 64 + particle_index];
                    s32 magnitude;
                    s32 image;

                    if (frame <=
                        base_frame + particle_index / 2)
                        continue;

                    magnitude =
                        ((SquareRoot_080d5e54)0x030001d8)(
                            (particle->x >> 8) *
                                (particle->x >> 8) +
                            (particle->y >> 8) *
                                (particle->y >> 8) +
                            (particle->z >> 8) *
                                (particle->z >> 8)) >> 9;
                    if (magnitude == 0)
                        continue;

                    Func_080e3944(&particle->x, projected);
                    projected[0] /= 2;
                    if (projected[2] <= 313)
                        projected[2] = 314;
                    if (projected[2] > 634)
                        projected[2] = 634;

                    image =
                        Func_080022fc(frame + particle_index * 4, 9);
                    renderers[1](
                        render_context,
                        graphics +
                            *(u16 *)(0x080ede84 + image * 2),
                        projected[0] -
                            *(u8 *)(0x080ede96 + image) / 2,
                        projected[1] -
                            *(u8 *)(0x080ede96 + image) / 2,
                        *(u8 *)(0x080ede96 + image),
                        *(u8 *)(0x080ede96 + image));

                    particle->x -=
                        Func_080022ec(particle->x, magnitude);
                    particle->y -=
                        Func_080022ec(particle->y, magnitude);
                    particle->z -=
                        Func_080022ec(particle->z, magnitude);
                }
            }
        }

        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((volatile void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
