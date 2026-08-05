#include "types.h"

typedef void (*Transfer_080cf2b8)(void *, const void *, u32);
typedef void (*Renderer_080cf2b8)(
    void *, const void *, s32, s32, s32, s32);

struct Scene_080cf2b8 {
    s32 unknown_00;
    s32 direction;
    s32 unknown_08;
    u8 unknown_0c[8];
    s32 object_count;
    u8 unknown_18[4];
    s32 intro;
    u8 unknown_20[4];
    s16 object_ids[1];
};

struct Object_080cf2b8 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct ObjectContext_080cf2b8 {
    struct Object_080cf2b8 *object;
};

struct Point_080cf2b8 {
    s32 x;
    s32 y;
    s32 z;
};

struct Particle_080cf2b8 {
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_0c;
    s32 unknown_10;
    s32 unknown_14;
    s32 timer;
};

s32 Func_080022ec(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_08004cb4(struct Point_080cf2b8 *);
void Func_080051d8(void *, void *);
void Func_08005340(const void *, void *);
struct ObjectContext_080cf2b8 *Func_080b5098(s16);
void Func_080b50e8(s32);
void Func_080cd594(s32);
s32 Func_080cdbc0(void);
void Func_080cef64(s32, Renderer_080cf2b8 *);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080de2f8(
    void *, s32, s32, s32, s32 *, s32 *);
void Func_080e3944(
    const struct Point_080cf2b8 *, struct Point_080cf2b8 *);
void Func_080e396c(s16, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

s32 Func_080cf2b8(struct Scene_080cf2b8 *scene, s32 variant)
{
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    void *effect_context;
    Transfer_080cf2b8 transfer =
        (Transfer_080cf2b8)0x03001388;
    struct Particle_080cf2b8 *particles =
        (struct Particle_080cf2b8 *)0x02010000;
    Renderer_080cf2b8 intro_renderers[2];
    s32 intro_x = 0;
    s32 intro_y = 0;
    s32 base_x;
    s32 frame;
    u32 particle_index;
    void *resource;

    *(struct Scene_080cf2b8 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    if (scene->intro == 1) {
        Func_080de2f8(
            scene, variant, scene->direction ^ 1, 0,
            &intro_x, &intro_y);
    }

    resource = Func_08002f40(0x7b);
    transfer((void *)0x05000000, resource, 0x80);
    Func_08005340((u8 *)resource + 0x80, runtime);

    resource = Func_08002f40(0xb1);
    transfer((void *)0x05000000, resource, 0x80);
    Func_08005340((u8 *)resource + 0x80, runtime + 0x2710);

    if (variant == 0)
        resource = Func_08002f40(0x93);
    else
        resource = Func_08002f40(0x91);
    transfer((void *)0x05000000, resource, 0x80);
    Func_08005340((u8 *)resource + 0x80, runtime - 0x10000);

    for (particle_index = 0; particle_index < 512;
         particle_index++) {
        particles[particle_index].x = 0;
        particles[particle_index].y = 0x500000;
        particles[particle_index].z =
            (s32)(Func_08004458() | 0xffffffe0) << 14;
        particles[particle_index].timer =
            Func_08004458() & 0xff;
    }

    *(volatile u16 *)0x04000020 = 0x100;
    if (scene->intro == 1) {
        s32 object_x;

        Func_080e396c(scene->object_ids[0], &object_x);
        base_x = 64 - object_x;
    } else if (scene->direction == 1) {
        base_x = -112;
    } else {
        base_x = 0;
    }

    *(volatile s32 *)0x04000028 = base_x << 8;
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 50;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (frame = 0;
         frame <
             (*(struct Scene_080cf2b8 **)(runtime + 0x65c0))
                     ->object_count *
                 16 +
             116;
         frame++) {
        struct Scene_080cf2b8 *active_scene;
        Renderer_080cf2b8 renderers[2];
        s32 object_index;

        effect_context = *(void **)0x03001e80;
        if (frame == 64)
            Func_080f9010(0xd4);
        if (frame == 80)
            Func_080b50e8(0);

        if (scene->intro == 1) {
            s32 angle = frame << 11;
            s32 x =
                ((-Func_08002322(angle) * 20) >> 16)
                + intro_x + base_x - 20;
            s32 y =
                ((Func_0800231c(angle) * 4) >> 16)
                + intro_y - 24;

            Func_080cef64(
                scene->direction ^ 1, intro_renderers);
            if (frame > 32)
                y = y - frame * 2 + 64;
            intro_renderers[1](
                render_context, runtime + 0x2710,
                x, y, 40, 40);
            if (frame <= 3)
                intro_renderers[1](
                    render_context, runtime + 0x2710,
                    x, y, 40, 40);
            Func_08002dd8(47);
            Func_08002dd8(46);
        }

        Func_080ed408(46, 7, 7, 3, 2);
        renderers[0] = *(Renderer_080cf2b8 *)0x03001f08;
        Func_080ed408(47, 7, 7, 7, 2);
        renderers[1] = *(Renderer_080cf2b8 *)0x03001f0c;
        active_scene =
            *(struct Scene_080cf2b8 **)(runtime + 0x65c0);

        for (object_index = 0;
             object_index < active_scene->object_count;
             object_index++) {
            struct Object_080cf2b8 *object =
                Func_080b5098(
                    active_scene->object_ids[object_index])
                    ->object;
            struct Point_080cf2b8 projected;
            s32 delay = object_index * 16;

            Func_080049ac();
            Func_080051d8(
                effect_context, (u8 *)effect_context + 12);
            projected.x = object->x;
            projected.y = object->y;
            projected.z = object->z;
            Func_08004cb4(&projected);

            if (frame > delay) {
                for (particle_index = 0;
                     particle_index < 8;
                     particle_index++) {
                    struct Particle_080cf2b8 *particle =
                        &particles[object_index * 64
                                   + particle_index];

                    if (frame > delay + particle_index * 8 &&
                        particle->y > 0x80000) {
                        struct Point_080cf2b8 particle_point;
                        s32 tile;

                        Func_080e3944(
                            (const struct Point_080cf2b8 *)
                                particle,
                            &particle_point);
                        particle_point.x += base_x;
                        particle_point.x +=
                            (Func_08002322(
                                 particle->timer << 10)
                             * 16) >> 16;
                        tile =
                            ((particle->timer < 0
                                  ? particle->timer + 7
                                  : particle->timer) >>
                             3) & 7;
                        renderers[particle_index & 1](
                            render_context,
                            runtime + 0x2710
                                + ((const u8 *)0x080ee0a2)[tile]
                                      * 576,
                            particle_point.x - 12,
                            particle_point.y - 12,
                            24, 24);
                        particle->y -= 0x10000;
                        particle->timer++;
                    }
                }
            }
        }

        for (object_index = 0;
             object_index < active_scene->object_count;
             object_index++) {
            s32 start = object_index * 16 + 72;

            if (frame >= start) {
                struct Object_080cf2b8 *object =
                    Func_080b5098(
                        active_scene->object_ids[object_index])
                        ->object;
                struct Point_080cf2b8 world;
                struct Point_080cf2b8 projected;
                s32 size_index;
                s32 strip_width = 6;

                Func_080049ac();
                Func_080051d8(
                    effect_context, (u8 *)effect_context + 12);
                if (frame == start)
                    Func_080d6888(
                        active_scene->object_ids[object_index],
                        1, -1, -1, 0);
                if (frame == start + 16)
                    Func_080d6888(
                        active_scene->object_ids[object_index],
                        0, -1, -1, 0);

                world.x = object->x;
                world.y = object->y;
                world.z = object->z;
                Func_080e3944(&world, &projected);
                projected.x += base_x;

                if (frame < object_index * 16 + 104) {
                    s32 adjusted_frame =
                        frame < 0 ? frame + 3 : frame;

                    size_index = adjusted_frame >> 2;
                    if (frame > start) {
                        strip_width = 6 - Func_080022ec(
                            frame - object_index * 16 - 88,
                            3);
                    }
                    if (size_index > 2)
                        size_index = (size_index & 1) + 1;

                    if (frame < object_index * 16 + 100) {
                        s32 width =
                            ((const u8 *)0x080ee0a2)[size_index];
                        s32 height =
                            ((const u8 *)0x080ee0b0)[size_index];

                        renderers[0](
                            render_context,
                            runtime
                                + ((const u16 *)0x080ee0aa)
                                      [size_index],
                            projected.x - width,
                            projected.y - height + 8,
                            width, height);
                        width =
                            ((const u8 *)0x080ee0b3)[size_index];
                        height =
                            ((const u8 *)0x080ee0b0)[size_index];
                        renderers[1](
                            render_context,
                            runtime
                                + ((const u16 *)0x080ee0aa)
                                      [size_index],
                            projected.x,
                            projected.y - width + 8,
                            height, width);
                    }

                    if (projected.y != 0) {
                        particle_index = 0;
                        do {
                            renderers[0](
                                render_context, runtime + 5,
                                projected.x - strip_width,
                                particle_index, strip_width, 1);
                            renderers[1](
                                render_context, runtime + 5,
                                projected.x, particle_index,
                                strip_width, 1);
                            particle_index++;
                        } while (particle_index != projected.y);
                    }
                }
            }
        }

        Func_08002dd8(47);
        Func_08002dd8(46);
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    return Func_080cdbc0();
}
