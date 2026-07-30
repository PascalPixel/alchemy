#include "types.h"

typedef void (*Transfer_080d52c8)(void *, const void *, u32);
typedef void (*Renderer_080d52c8)(
    void *, const void *, s32, s32, s32, s32);
typedef s32 (*SquareRoot_080d52c8)(u32);

struct Scene_080d52c8 {
    u8 unknown_00[20];
    s32 object_count;
    u8 unknown_18[12];
    s16 object_ids[1];
};

struct Object_080d52c8 {
    u8 unknown_00[8];
    s32 x;
    u8 unknown_0c[4];
    s32 z;
};

struct ObjectContext_080d52c8 {
    struct Object_080d52c8 *object;
};

struct Particle_080d52c8 {
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_0c;
    s32 unknown_10;
    s32 unknown_14;
    s32 age;
};

s32 Func_080022ec(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(volatile void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_08004bd4(s32);
void Func_08004c1c(s32);
void Func_08004cb4(s32 *);
void Func_080051d8(void *, void *);
void Func_080b5078(s32, s32, s32, s32);
void Func_080b5088(s32, s32);
struct ObjectContext_080d52c8 *Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080e0524(void *, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e3944(const s32 *, s32 *);
void Func_080e396c(s32, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080d52c8(struct Scene_080d52c8 *scene, s32 variant)
{
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    struct Particle_080d52c8 *particles =
        (struct Particle_080d52c8 *)0x02010000;
    Transfer_080d52c8 transfer = (Transfer_080d52c8)0x03001388;
    Renderer_080d52c8 renderers[2];
    s32 particle_count = 16;
    s32 frame_count;
    s32 frame;
    s32 object_index;
    s32 i;
    s32 resource;

    *(struct Scene_080d52c8 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    Func_080e0524((void *)0x9e, runtime, 1, 1);

    switch (variant) {
    case 0: resource = 0xa0; break;
    case 1: resource = 0xbb; break;
    case 2: resource = 0xa3; break;
    case 3: resource = 0xc0; break;
    case 4: resource = 0xb7; break;
    case 5:
        resource = 0x8d;
        Func_080e0524((void *)resource, runtime, 1, 0);
        break;
    case 7:
        particle_count = 24;
        Func_080e0524((void *)0xcd, runtime, 1, 0);
        resource = 0xce;
        break;
    default:
        particle_count = 32;
        resource = 0xaa;
        Func_080e0524((void *)resource, runtime, 1, 0);
        break;
    }

    transfer((void *)0x05000000, Func_08002f40(resource), 0x80);
    if (variant == 4)
        Func_080e0524((void *)0xce, runtime, 1, 1);
    if (variant == 3)
        Func_080e0524((void *)0xaa, runtime + 0x9600, 1, 0);

    for (i = 0; i < 512; i++) {
        if (variant <= 1 || (variant >= 4 && variant <= 7)) {
            particles[i].x =
                (s32)((Func_08004458() & 0xff) - 127) << 15;
            particles[i].y =
                (s32)((Func_08004458() & 0xff) - 127) << 14;
            particles[i].z =
                (s32)((Func_08004458() & 0xff) - 127) << 15;
        } else {
            particles[i].x =
                (s32)((Func_08004458() & 0xff) - 127) << 13;
            particles[i].y =
                (s32)((Func_08004458() & 0xff) - 255) << 13;
            particles[i].z =
                (s32)((Func_08004458() & 0xff) - 127) << 13;
        }
        particles[i].age = 0;
    }

    if (variant <= 1 || (variant >= 4 && variant <= 7))
        frame_count = scene->object_count * 8 + 64;
    else
        frame_count = scene->object_count * 8 + 32;

    if (variant > 1 && variant != 3)
        Func_080041d8((const void *)0x080dbb9d, 0x480);

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080f9010(142);

    for (frame = 0; frame < frame_count; frame++) {
        void *effect_context = *(void **)0x03001e80;

        if (variant == 7 && frame == frame_count - 46) {
            struct Object_080d52c8 *object =
                Func_080b5098(scene->object_ids[0])->object;
            Func_080b5078(object->x, scene->object_ids[0], 16, 0);
        }
        if (frame == frame_count - 32) {
            if (variant == 7) {
                Func_080b50e8(134);
                Func_080b5088(scene->object_ids[0], 4);
                *(s32 *)(runtime + 0x77a8) = 8;
            } else {
                Func_080b50e8(133);
            }
        }

        for (i = 0; i < 160; i++) {
            s32 angle = (frame << 12) + (i << 11);
            *(s32 *)(runtime + 0x6980 + i * 4) =
                (0x10000 - (Func_08002322(angle) << 2)) >> 10;
        }

        for (object_index = 0;
             object_index < scene->object_count;
             object_index++) {
            s32 id = scene->object_ids[object_index];
            struct Object_080d52c8 *object =
                Func_080b5098(id)->object;
            s32 object_delay = object_index * 8;
            s32 position[3];

            if (variant == 3 &&
                frame > object_delay && frame < object_delay + 32) {
                s32 center[3];
                s32 which = frame & variant;

                Func_080e396c(id, center);
                for (i = 0; i < 2; i++) {
                    s32 angle = Func_08004458() & 0xffff;
                    s32 radius = (Func_08004458() & 31) + 4;
                    s32 x = center[0] / 2 +
                        radius * Func_08002322(angle) / 0x20000 -
                        *(u8 *)(0x080edebe + which) / 2;
                    s32 y = center[1] -
                        radius * Func_0800231c(angle) / 0x10000 -
                        *(u8 *)(0x080edeca + which) / 2;

                    Func_080ed408(47, 7, 7,
                                  *(u8 *)(0x080eded0 +
                                         (Func_08004458() & 3)) | 3,
                                  2);
                    ((Renderer_080d52c8)*(u32 *)0x03001f0c)(
                        render_context,
                        runtime + 0x9600 +
                            *(u16 *)(0x080ee2ae + which * 2),
                        x, y + 16,
                        *(u8 *)(0x080edebe + which),
                        *(u8 *)(0x080edeca + which));
                    Func_08002dd8(47);
                }
            }

            Func_080ed408(46, 7, 7, 3, 3);
            renderers[0] = *(Renderer_080d52c8 *)0x03001f08;
            Func_080ed408(47, 7, 7, 3, 2);
            renderers[1] = *(Renderer_080d52c8 *)0x03001f0c;

            Func_080049ac();
            Func_080051d8(effect_context, (u8 *)effect_context + 12);
            position[0] = object->x;
            position[1] = 0x140000;
            position[2] = object->z;
            Func_08004cb4(position);

            if (frame > object_delay) {
                s32 time = frame << 9;

                Func_08004c1c(time);
                if (variant <= 1 || variant == 4)
                    Func_08004bd4(time);

                for (i = 0; i < particle_count; i++) {
                    struct Particle_080d52c8 *particle =
                        &particles[object_index * 64 + i];
                    s32 projected[3];
                    s32 magnitude;

                    if (frame <= object_delay + i)
                        continue;
                    magnitude =
                        ((SquareRoot_080d52c8)0x030001d8)(
                            (particle->x >> 8) * (particle->x >> 8) +
                            (particle->y >> 8) * (particle->y >> 8) +
                            (particle->z >> 8) * (particle->z >> 8)) >> 9;
                    if (magnitude == 0 || particle->age > 23)
                        continue;

                    Func_080e3944(&particle->x, projected);
                    projected[0] /= 2;
                    if (variant == 5 || variant == 7) {
                        s32 image = particle->age / 4;
                        renderers[1](
                            render_context,
                            runtime + (((image * 3) * 8 + image) << 6),
                            projected[0] - 20, projected[1] - 20,
                            40, 40);
                    } else if (variant == 6) {
                        renderers[1](
                            render_context, runtime + 0xc00,
                            projected[0] - 6, projected[1] - 12,
                            12, 24);
                    } else if (variant == 4) {
                        renderers[1](
                            render_context, runtime,
                            projected[0] - 11, projected[1] - 21,
                            22, 42);
                    } else {
                        s32 image = (i & 3) != 0;
                        s32 age_image = (particle->age / 4);
                        renderers[image](
                            render_context,
                            runtime + ((age_image * 9) << 7),
                            projected[0] - 12, projected[1] - 24,
                            24, 48);
                    }

                    if (variant <= 1 ||
                        (variant >= 4 && variant <= 6)) {
                        particle->x -=
                            Func_080022ec(particle->x, magnitude);
                        particle->y -=
                            Func_080022ec(particle->y, magnitude);
                        particle->z -=
                            Func_080022ec(particle->z, magnitude);
                    } else {
                        particle->y += 0x10000;
                    }

                    particle->age++;
                    if (particle->age == 24) {
                        if (variant <= 1 ||
                            (variant >= 4 && variant <= 6)) {
                            particle->age = 0;
                        } else {
                            particle->x =
                                (s32)((Func_08004458() & 0xff) - 127)
                                << 13;
                            particle->y =
                                (s32)((Func_08004458() & 0xff) - 255)
                                << 12;
                            particle->z =
                                (s32)((Func_08004458() & 0xff) - 127)
                                << 13;
                        }
                    }
                }
            }

            Func_08002dd8(47);
            Func_08002dd8(46);
            if (frame == object_delay + 16) {
                s32 remaining = frame_count - frame;
                if (remaining > 31)
                    remaining = 31;
                Func_080d6888(id, 7, 5, object_index, remaining);
            }
        }

        Func_080e155c(16, 16);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((volatile void *)0x080cd261);
    if (variant > 1 && variant != 3)
        Func_08004278((volatile void *)0x080dbb9d);
    Func_080cdbc0();
}
