#include "types.h"

typedef void (*Transfer_080d82b0)(
    void *destination,
    const void *source,
    u32 size);

typedef void (*Renderer_080d82b0)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Scene_080d82b0 {
    s32 unknown_00;
    s32 unknown_04;
    u8 unknown_08[12];
    s32 count;
    u8 unknown_18[12];
    s16 object_ids[1];
};

struct Object_080d82b0 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct ObjectContext_080d82b0 {
    struct Object_080d82b0 *object;
};

struct Particle_080d82b0 {
    s32 x;
    s32 y;
    s32 unknown_08;
    s32 velocity_x;
    s32 velocity_y;
    s32 unknown_14;
    s32 age;
};

s32 Func_080022fc(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
struct ObjectContext_080d82b0 *Func_080b5098(s32);
s32 Func_080b5070(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(void *, void *, s32, s32);
void Func_080e3908(struct Particle_080d82b0 *, s32, s32);
void Func_080e3944(const s32 *, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080d82b0(struct Scene_080d82b0 *scene)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    u8 *effect_tiles = (u8 *)runtime_header[2];
    Renderer_080d82b0 renderer;
    Transfer_080d82b0 transfer = (Transfer_080d82b0)0x03001388;
    struct Particle_080d82b0 *particles =
        (struct Particle_080d82b0 *)0x02010000;
    void *effect_context = *(void **)0x03001e80;
    s32 frame;
    s32 total_frames;
    s32 object_index;
    s32 particle_index;

    *(struct Scene_080d82b0 **)(runtime + 0x7828) = scene;
    Func_080cd594(1);
    Func_080e0524((void *)0x73, effect_tiles, 0, 0);
    transfer((void *)0x05000000, Func_08002f40(0xb9), 0x80);
    Func_080ed408(46, 7, 7, 3, 2);
    renderer = (Renderer_080d82b0)runtime_header[7];

    for (particle_index = 0; particle_index < 1024; particle_index++)
        particles[particle_index].age = -1;

    Func_080049ac();
    Func_080051d8(effect_context, (u8 *)effect_context + 12);

    for (object_index = 0;
         object_index < scene->count;
         object_index++) {
        s32 object_id = scene->object_ids[object_index];
        struct Object_080d82b0 *object =
            Func_080b5098(object_id)->object;
        s32 source_point[3];
        s32 projected[3];

        source_point[0] = object->x;
        source_point[1] = Func_080b5070(object_id) / 2;
        source_point[2] = object->z;
        Func_080e3944(source_point, projected);
        projected[0] >>= 1;

        for (particle_index = 0; particle_index < 128; particle_index++) {
            struct Particle_080d82b0 *particle =
                &particles[object_index * 128 + particle_index];
            s32 radius = Func_08004458() & 0xff;
            s32 angle = Func_08004458() & 0xffff;

            particle->x =
                (radius * Func_08002322(angle) >> 7) +
                (projected[0] << 16);
            particle->y =
                (radius * Func_0800231c(angle) >> 3) +
                (projected[1] << 16);
            particle->velocity_x =
                (128 - (s32)(Func_08004458() & 0xff)) << 9;
            particle->velocity_y =
                (-(s32)(Func_08004458() & 0xff) - 128) << 10;
            particle->age = 0;
        }
    }

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 50;
    Func_080041d8((const void *)0x080cd261, 0x480);

    frame = 0;
    total_frames = scene->count * 20 + 56;
    if (total_frames != 0) {
        do {
            if (frame == 32)
                Func_080b50e8(0);

            for (object_index = 0;
                 object_index < scene->count;
                 object_index++) {
                s32 start = object_index * 20;

                if (frame == start) {
                    Func_080f9010(0x8f);
                    Func_080d6888(scene->object_ids[object_index],
                                  7, -1, object_index, 20);
                }

                if (frame > start) {
                    for (particle_index = 0;
                         particle_index < 128;
                         particle_index++) {
                        struct Particle_080d82b0 *particle =
                            &particles[object_index * 128 + particle_index];

                        if (particle->age >= 0) {
                            s32 size = Func_080022fc(particle_index, 3) + 1;
                            s32 height = size * 2;
                            const void *source = effect_tiles +
                                *(u16 *)(0x080ede48 + height - 2);

                            renderer(
                                render_context,
                                source,
                                *(s16 *)((u8 *)particle + 2) - size / 2,
                                *(s16 *)((u8 *)particle + 6) - size,
                                size,
                                height);
                            Func_080e3908(
                                particle,
                                62,
                                *(s32 *)(0x080ee9f8 +
                                        ((particle_index & 3) * 4)));
                            particle->age++;
                            if (particle->velocity_y > 0 &&
                                *(s16 *)((u8 *)particle + 6) > 112) {
                                particle->age = -1;
                            }
                        }
                    }
                }
            }

            Func_080cd52c();
            *(s32 *)(runtime + 0x7824) = 1;
            Func_080030f8(1);
            frame++;
        } while (frame != total_frames);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
