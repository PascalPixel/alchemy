#include "types.h"

typedef void (*Transfer_080d89ac)(void *, const void *, u32);
typedef void (*Renderer_080d89ac)(
    void *, const void *, s32, s32, s32, s32);
typedef s32 (*SquareRoot_080d89ac)(s32);

struct Scene_080d89ac {
    u8 unknown_00[4];
    s32 direction;
    u8 unknown_08[12];
    s32 object_count;
    s32 style;
    s32 special;
    s32 layout;
    u8 unknown_20[4];
    s16 object_ids[1];
};

struct Object_080d89ac {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct ObjectContext_080d89ac {
    struct Object_080d89ac *object;
};

struct Particle_080d89ac {
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_0c;
    s32 speed;
    s32 unknown_14;
    s32 age;
};

s32 Func_080022ec(s32, s32);
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
void Func_08004bd4(s32);
void Func_08004c1c(s32);
void Func_08004c6c(s32);
void Func_08004cb4(s32 *);
void Func_080051d8(void *, void *);
void Func_08005340(const void *, void *);
void Func_080b50e8(s32);
struct ObjectContext_080d89ac *Func_080b5098(s16);
void Func_080cdbc0(void);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cef64(s32, Renderer_080d89ac *);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080de2f8(
    void *, s32, s32, s32, s32 *, s32 *);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e3944(s32 *, s32 *);
void Func_080e396c(s16, s32 *);
void Func_080f9010(s32);

void Func_080d89ac(
    struct Scene_080d89ac *scene, s32 mode)
{
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    u8 *graphics = (u8 *)header[2];
    Transfer_080d89ac transfer =
        (Transfer_080d89ac)0x03001388;
    SquareRoot_080d89ac square_root =
        (SquareRoot_080d89ac)0x030001d8;
    Renderer_080d89ac renderers[2];
    struct Particle_080d89ac *particles =
        (struct Particle_080d89ac *)0x02010000;
    void *effect_context = *(void **)0x03001e80;
    s32 camera_offset = 0;
    s32 camera_mode;
    s32 anchor_x = 0;
    s32 anchor_y = 0;
    s32 particle_count;
    s32 total_frames;
    s32 frame;
    u32 i;

    *(struct Scene_080d89ac **)(runtime + 0x7828) = scene;
    Func_080cd594(0);

    if (scene->layout == 1) {
        s32 direction =
            2 - (mode == 6 ? 0 : 1);

        Func_080de2f8(
            scene,
            direction,
            scene->direction,
            (mode == 0 || mode == 6) ? 0 : 1,
            &anchor_x,
            &anchor_y);
        *(s32 *)(*(u8 **)(runtime + 0x7828) + 24) = 0;
    }

    if (mode == 0) {
        s32 position[3];
        struct Scene_080d89ac *owner =
            *(struct Scene_080d89ac **)(runtime + 0x7828);

        Func_080e396c(owner->object_ids[0], position);
        camera_offset = 64 - position[0];
        *(volatile s32 *)0x04000028 =
            camera_offset << 8;
        *(volatile u16 *)0x04000020 = 0x0100;
        camera_mode = 0;
    } else {
        camera_mode = 1;
    }

    Func_080e0524(0x73, graphics, 0, 0);
    Func_080e0524(0xba, runtime, 0, 0);

    {
        s32 palette;

        if (mode <= 1 || mode == 3 ||
            mode == 4 || mode == 5) {
            palette = scene->style == 0 ? 0xb3 : 0xb9;
        } else if (mode == 6) {
            palette = 0x8d;
        } else {
            palette = 0xc0;
        }
        transfer(
            (void *)0x05000000,
            Func_08002f40(palette),
            0x80);
    }

    {
        s32 resource;
        u8 *source;

        if (camera_mode == 0)
            resource = mode == 6 ? 0x8d : 0x91;
        else
            resource = mode == 6 ? 0x8e : 0x92;
        source = (u8 *)Func_08002f40(resource);
        Func_08005340(source + 0x80, runtime + 0x1000);
    }

    Func_080cef64(scene->direction, renderers);

    for (i = 0; i < 512; i++) {
        struct Particle_080d89ac *particle = &particles[i];

        if (mode == 0 || mode == 6) {
            particle->x =
                ((Func_08004458() & 255) - 127) << 15;
            particle->y =
                ((Func_08004458() & 127) + 64) << 15;
            particle->z =
                ((Func_08004458() & 255) - 127) << 15;
        } else if (mode == 1) {
            particle->x =
                ((Func_08004458() & 255) - 127) << 15;
            particle->y =
                ((Func_08004458() & 255) - 127) << 15;
            particle->z =
                ((Func_08004458() & 255) - 127) << 15;
        } else if (mode == 2) {
            s32 angle = Func_08004458() & 0xffff;
            s32 radius = (Func_08004458() & 63) + 32;

            particle->x = radius * Func_08002322(angle);
            particle->y = (s32)0xffce0000;
            particle->z = radius * Func_0800231c(angle);
            particle->speed =
                ((Func_08004458() & 31) + 32) << 13;
        } else {
            particle->x =
                ((Func_08004458() & 255) - 127) << 15;
            particle->y =
                ((Func_08004458() & 255) - 127) <<
                (mode == 3 ? 14 : 15);
            particle->z =
                ((Func_08004458() & 255) - 127) << 15;
        }
        particle->age = 0;
    }

    total_frames =
        scene->object_count * 8 +
        ((mode == 3 || mode == 4 || mode == 5) ? 72 : 88);
    particle_count = 64;
    if (scene->style == 0)
        particle_count = 32;
    else if (scene->style == 2)
        particle_count = 128;

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (frame = 0; frame < total_frames; frame++) {
        if (frame == 40)
            Func_080b50e8(0);

        if (scene->layout == 1) {
            s32 angle = frame * 0x800;
            s32 x;
            s32 y =
                anchor_y +
                (Func_0800231c(angle) * 4 >> 16) - 24;

            if (camera_mode == 0) {
                x = anchor_x + camera_offset +
                    (Func_08002322(angle) * 20 >> 16) -
                    20;
                if (frame > 32)
                    y += 64 - frame * 2;
                renderers[0](
                    render_context,
                    runtime + 0x1000,
                    x, y, 40, 40);
                if (frame <= 3)
                    renderers[1](
                        render_context,
                        runtime + 0x1000,
                        x, y, 40, 40);
            } else {
                x = anchor_x / 2 +
                    (Func_08002322(angle) * 10 >> 16) -
                    10;
                if (frame > 32)
                    y += 64 - frame * 2;
                renderers[0](
                    render_context,
                    runtime + 0x1000,
                    x, y, 20, 40);
                if (frame <= 3)
                    renderers[1](
                        render_context,
                        runtime + 0x1000,
                        x, y, 20, 40);
            }
        }

        for (i = 0; i < scene->object_count; i++) {
            s32 start = i * 8;
            s32 object_id = scene->object_ids[i];
            struct Object_080d89ac *object =
                Func_080b5098(object_id)->object;
            s32 source_point[3];
            s32 projected[3];
            s32 j;

            Func_080049ac();
            Func_080051d8(
                effect_context, (u8 *)effect_context + 12);
            source_point[0] = object->x;
            source_point[1] = 0x2800000;
            source_point[2] = object->z;
            Func_08004cb4(source_point);

            if (frame == start + 20)
                Func_080f9010(126);
            if (frame == start + 36)
                Func_080d6888(
                    object_id, 7, -1, i, 28);
            if (frame <= start)
                continue;

            if (mode == 1) {
                Func_08004bd4(frame << 9);
                Func_08004c6c(frame << 9);
            } else if (mode == 2) {
                Func_08004c1c((frame - i * 40) << 9);
            } else if (mode == 0 ||
                       mode == 3 || mode == 6) {
                Func_08004c1c(frame << 9);
            } else {
                Func_08004c1c(frame << 9);
                Func_08004bd4(frame << 9);
            }

            for (j = 0; j < particle_count; j++) {
                struct Particle_080d89ac *particle =
                    &particles[i * 64 + j];
                s32 last_frame =
                    (mode >= 3 && mode <= 5)
                        ? start + 32 + j / 2
                        : 0x10000;
                s32 release = start + j / 4;
                s32 magnitude;
                s32 size;

                if (frame <= release || frame >= last_frame)
                    continue;
                magnitude = square_root(
                    (particle->x >> 8) *
                        (particle->x >> 8) +
                    (particle->y >> 8) *
                        (particle->y >> 8) +
                    (particle->z >> 8) *
                        (particle->z >> 8));
                magnitude >>= 9;
                if (magnitude == 0)
                    continue;

                Func_080e3944(
                    (s32 *)particle, projected);
                if (mode == 0)
                    projected[0] += camera_offset;
                else
                    projected[0] >>= 1;
                projected[1] += 16;
                if (projected[2] < 314)
                    projected[2] = 314;
                if (projected[2] > 634)
                    projected[2] = 634;

                size = 3 -
                    ((projected[2] - 314) / 128);
                if (mode == 0)
                    size = Func_080022fc(j * 4 + frame, 9);

                if (mode == 0 ||
                    (mode >= 3 && mode <= 5)) {
                    s32 width =
                        ((const u8 *)0x080ede96)[size];

                    renderers[1](
                        render_context,
                        runtime +
                            ((const u16 *)0x080ede84)[size],
                        projected[0] - (width >> 1),
                        projected[1] - (width >> 1),
                        width,
                        width);
                } else {
                    const u16 *offsets =
                        (const u16 *)0x080ede48;

                    renderers[1](
                        render_context,
                        graphics + offsets[size - 1],
                        projected[0] - (size >> 1),
                        projected[1] - size,
                        size,
                        size * 2);
                }

                if (mode <= 2 || mode == 6) {
                    particle->x -=
                        Func_080022ec(
                            particle->x, magnitude);
                    particle->y -=
                        Func_080022ec(
                            particle->y, magnitude);
                    particle->z -=
                        Func_080022ec(
                            particle->z, magnitude);
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
