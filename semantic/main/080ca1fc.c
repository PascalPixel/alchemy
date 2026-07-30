#include "layout_guard.h"
#include "types.h"

struct Scene_080ca1fc {
    u8 unknown_00[8];
    s32 primary_object_id;
    u8 unknown_0c[8];
    s32 object_count;
    u8 unknown_18[12];
    s16 object_ids[1];
};

struct Object_080ca1fc {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct ObjectContext_080ca1fc {
    struct Object_080ca1fc *object;
};

struct Particle_080ca1fc {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 vertical_offset;
    s32 velocity_y;
    s32 timer;
};

struct TransformPoint_080ca1fc {
    s32 x;
    s32 y;
    s32 z;
};

typedef void (*Transfer_080ca1fc)(
    void *destination,
    const void *source,
    s32 size);
typedef void (*Renderer_080ca1fc)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

LAYOUT_OFFSET_GUARD(
    Scene080ca1fc_PrimaryObjectId,
    struct Scene_080ca1fc,
    primary_object_id,
    8);
LAYOUT_OFFSET_GUARD(
    Scene080ca1fc_ObjectCount,
    struct Scene_080ca1fc,
    object_count,
    0x14);
LAYOUT_OFFSET_GUARD(
    Scene080ca1fc_ObjectIds,
    struct Scene_080ca1fc,
    object_ids,
    0x24);
LAYOUT_SIZE_GUARD(
    Particle080ca1fc,
    struct Particle_080ca1fc,
    0x1c);

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
struct ObjectContext_080ca1fc *Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e38b8(struct Particle_080ca1fc *, s32, s32);
void Func_080e3944(
    const struct TransformPoint_080ca1fc *,
    struct TransformPoint_080ca1fc *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080ca1fc(struct Scene_080ca1fc *scene, s32 variant)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    u8 *effect_tiles = (u8 *)runtime_header[2];
    void *effect_context = *(void **)0x03001e80;
    Transfer_080ca1fc transfer =
        (Transfer_080ca1fc)0x03001388;
    Renderer_080ca1fc renderer;
    struct Object_080ca1fc *object;
    struct Particle_080ca1fc *particles =
        (struct Particle_080ca1fc *)0x02010000;
    s32 frame;
    s32 index;

    *(struct Scene_080ca1fc **)(runtime + 0x7828) = scene;
    if (variant == 0)
        Func_080cd594(0);
    else
        Func_080cd594(1);

    Func_080ed408(46, 7, 7, 3, 2);
    renderer = *(Renderer_080ca1fc *)0x03001f08;
    Func_080ed408(47, 7, 7, 11, 2);

    Func_080e0524(0x73, effect_tiles, 0, 0);
    transfer(
        (void *)0x05000000,
        Func_08002f40(variant == 0 ? 0x7c : 0x7b),
        0x80);

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    object = Func_080b5098(scene->primary_object_id)->object;
    for (index = 0; index < 256; index++) {
        struct Particle_080ca1fc *particle = &particles[index];
        s32 magnitude = (Func_08004458() & 0x3ff) + 32;
        s32 angle = Func_08004458() & 0xffff;

        particle->x = object->x;
        particle->y = object->y + 0x50000;
        particle->z = object->z;
        particle->velocity_x =
            magnitude * Func_08002322(angle) >> 8;
        particle->vertical_offset =
            ((Func_08004458() & 0xff) - 32) << 9;
        particle->velocity_y =
            -(magnitude * Func_0800231c(angle) * 2) >> 8;
        particle->timer = (Func_08004458() & 0x1f) + 48;

        if (variant == 0) {
            particle->velocity_x =
                (particle->velocity_x +
                 ((u32)particle->velocity_x >> 31)) >> 1;
            particle->velocity_y =
                (particle->velocity_y +
                 ((u32)particle->velocity_y >> 31)) >> 1;
        }
    }

    for (frame = 0; frame < 128; frame++) {
        Func_080049ac();
        Func_080051d8(effect_context, (u8 *)effect_context + 12);

        for (index = 0; index < 128; index++) {
            struct Particle_080ca1fc *particle = &particles[index];

            if (frame >= (index >> 5) * 8 &&
                particle->timer >= 0) {
                struct TransformPoint_080ca1fc world;
                struct TransformPoint_080ca1fc projected;
                s32 size;

                world.x = particle->x +
                    (Func_08002322(
                         (index * 4 + particle->timer) << 10) << 4);
                world.y = particle->y;
                world.z = particle->z;
                Func_080e3944(&world, &projected);
                projected.x >>= 1;

                if (projected.z <= 0x139)
                    projected.z = 0x13a;
                if (projected.z > 0x27a)
                    projected.z = 0x27a;

                size = 6 - ((projected.z - 0x13a) >> 6);
                renderer(
                    render_context,
                    effect_tiles +
                        ((const u16 *)0x080ede48)[size - 1],
                    projected.x -
                        ((size + ((u32)size >> 31)) >> 1),
                    projected.y - size,
                    size,
                    size * 2);
                Func_080e38b8(particle, 62, 0x400);

                if (variant == 1) {
                    if (object->x < 0)
                        particle->velocity_x += 0x2000;
                    else
                        particle->velocity_x -= 0x2000;
                }
                particle->timer--;
            }
        }

        for (index = 0; index < scene->object_count; index++) {
            if (frame == 48 + index * 8) {
                if (variant == 1) {
                    Func_080b50e8(-1);
                    Func_080d6888(
                        scene->object_ids[index],
                        7,
                        5,
                        index,
                        8);
                } else {
                    Func_080f9010(126);
                    Func_080b50e8(-1);
                    Func_080d6888(
                        scene->object_ids[index],
                        7,
                        -1,
                        index,
                        8);
                }
            }
        }

        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
