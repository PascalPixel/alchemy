#include "layout_guard.h"
#include "types.h"

struct Scene_080d9ae8 {
    u8 unknown_00[4];
    s32 direction;
    u8 unknown_08[12];
    s32 object_count;
    u8 unknown_18[12];
    s16 object_ids[1];
};
struct Object_080d9ae8 {
    u8 unknown_00[8];
    s32 x;
    u8 unknown_0c[4];
    s32 z;
};
struct ObjectContext_080d9ae8 { struct Object_080d9ae8 *object; };
struct Particle_080d9ae8 {
    s32 x, y, z, velocity_x, velocity_y, velocity_z, timer;
};
struct Point_080d9ae8 { s32 x, y, z; };

typedef void (*Transfer_080d9ae8)(void *, const void *, s32);
typedef void (*Renderer_080d9ae8)(
    void *, const void *, s32, s32, s32, s32);

LAYOUT_OFFSET_GUARD(Scene080d9ae8_Count, struct Scene_080d9ae8, object_count, 0x14);
LAYOUT_OFFSET_GUARD(Scene080d9ae8_Ids, struct Scene_080d9ae8, object_ids, 0x24);
LAYOUT_SIZE_GUARD(Particle080d9ae8, struct Particle_080d9ae8, 0x1c);

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
void Func_08004cb4(void *);
void Func_080051d8(void *, void *);
struct ObjectContext_080d9ae8 *Func_080b5098(s16);
void Func_080b50e8(s32);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080cef64(s32, Renderer_080d9ae8 *);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e38b8(struct Particle_080d9ae8 *, s32, s32);
void Func_080e3944(const struct Point_080d9ae8 *, struct Point_080d9ae8 *);
void Func_080f9010(s32);

void Func_080d9ae8(struct Scene_080d9ae8 *scene, s32 mode)
{
    static const u16 *const image_offsets =
        (const u16 *)0x080eea08;
    static const u8 *const widths = (const u8 *)0x080eea20;
    static const u8 *const heights = (const u8 *)0x080eea2c;
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    Renderer_080d9ae8 renderers[2];
    Transfer_080d9ae8 transfer =
        (Transfer_080d9ae8)0x03001388;
    struct Particle_080d9ae8 *particles =
        (struct Particle_080d9ae8 *)0x02010000;
    s32 horizontal_offset;
    s32 frame;
    s32 index;

    *(struct Scene_080d9ae8 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    *(volatile u16 *)0x04000020 = 0x100;
    if (mode == 0)
        Func_080e0524(0x9c, runtime, 1, 1);
    else
        Func_080e0524(0x9b, runtime, 1, 1);
    transfer(
        (void *)0x05000000,
        Func_08002f40(mode == 1 ? 0xb7 : 0xbb),
        0x80);
    Func_080e0524(0x9d, runtime + 0x2580, 0, 0);

    for (index = 1; index < 8; index++) {
        s32 pixel;
        u8 *source = runtime + 0x2580;
        u8 *destination = source + index * 0x2b8;
        s32 maximum = 64 - index * 7;

        for (pixel = 0; pixel < 0x2b8; pixel++) {
            u8 value = source[pixel];
            destination[pixel] = value > maximum ? maximum : value;
        }
    }

    if (scene->direction == 1) {
        *(volatile s32 *)0x04000028 = -0x7000;
        horizontal_offset = -112;
    } else {
        *(volatile s32 *)0x04000028 = 0;
        horizontal_offset = 0;
    }

    for (index = 0; index < 512; index++) {
        struct Particle_080d9ae8 *particle = &particles[index];
        s32 angle = Func_08004458() & 0xffff;

        particle->x = 0;
        particle->y =
            ((index & 31) / 4) * 0x60000 - 0xa0000;
        particle->z = mode == 0
            ? (index % 4) * 0x20000 - 0x20000
            : (index % 4) * 0x80000 - 0x100000;
        particle->velocity_x =
            scene->direction == 1 ? 0x20000 : -0x20000;
        particle->velocity_y =
            (192 * Func_0800231c(angle) >> 6) + 0x10000;
        particle->velocity_z =
            192 * Func_08002322(angle) >> 6;
        particle->timer = Func_08004458() & 0xff;
    }

    Func_080cef64(scene->direction, renderers);
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 50;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (frame = 0; frame < scene->object_count * 4 + 64; frame++) {
        void *effect_context = *(void **)0x03001e80;
        s32 object_index;

        if (frame == 72)
            Func_080b50e8(0);

        for (object_index = 0;
             object_index < scene->object_count;
             object_index++) {
            s32 object_frame = frame - object_index * 4;
            struct Object_080d9ae8 *object =
                Func_080b5098(scene->object_ids[object_index])->object;

            if (object_frame > 0) {
                struct Point_080d9ae8 world;
                struct Point_080d9ae8 zero = { 0, 0, 0 };
                struct Point_080d9ae8 projected;
                s32 center_x;
                s32 center_y;

                Func_080049ac();
                Func_080051d8(effect_context, (u8 *)effect_context + 12);
                world.x = object->x;
                world.y = 0x140000;
                world.z = object->z;
                Func_080049ac();
                Func_080051d8(effect_context, (u8 *)effect_context + 12);
                Func_08004cb4(&world);
                Func_080e3944(&zero, &projected);
                center_x = projected.x + horizontal_offset;
                center_y = projected.y;

                if (mode == 0) {
                    if (object_frame <= 26) {
                        s32 image =
                            Func_080022fc(object_frame / 4, 7);
                        renderers[0](
                            render_context,
                            runtime + image * 0x3c0,
                            center_x - 12,
                            center_y - 20,
                            24,
                            40);
                    }
                } else if (object_frame <= 23) {
                    s32 image =
                        Func_080022fc(object_frame / 4, 6);
                    renderers[0](
                        render_context,
                        runtime + image * 0x640,
                        center_x - 20,
                        center_y - 20,
                        40,
                        40);
                }

                if (object_frame == 24)
                    Func_080f9010(0x8f);

                if ((u32)(frame - 24 - object_index * 4) <= 36) {
                    s32 sheet = 0;
                    struct Particle_080d9ae8 *group =
                        &particles[object_index * 32];

                    if (object_frame > 28) {
                        sheet = (frame - 24 - object_index * 4) / 4;
                        if (sheet > 7)
                            sheet = 7;
                    }
                    for (index = 0; index < 24; index++) {
                        struct Particle_080d9ae8 *particle = &group[index];
                        struct Point_080d9ae8 screen;
                        s32 animation =
                            Func_080022fc(
                                (particle->timer + object_frame) / 8,
                                3);
                        s32 image =
                            (index % 4) * 3 + animation;

                        Func_080e3944(
                            (const struct Point_080d9ae8 *)particle,
                            &screen);
                        renderers[0](
                            render_context,
                            runtime + 0x2580 + sheet * 0x2b8 +
                                image_offsets[image],
                            screen.x + horizontal_offset,
                            screen.y,
                            widths[image],
                            heights[image]);
                        Func_080e38b8(particle, 60, 0);
                    }
                }
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
