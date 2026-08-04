#include "types.h"

typedef void (*Transfer_080d05fc)(void *, const void *, u32);
typedef void (*Renderer_080d05fc)(
    void *, const void *, s32, s32, s32, s32);

struct Scene_080d05fc {
    u8 unknown_00[4];
    s32 direction;
    s32 object_argument;
    u8 unknown_0c[8];
    s32 object_count;
    u8 unknown_18[4];
    s32 intro;
    u8 unknown_20[4];
    s16 object_ids[1];
};
struct Object_080d05fc {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};
struct ObjectContext_080d05fc { struct Object_080d05fc *object; };
struct Point_080d05fc { s32 x; s32 y; s32 z; };
struct TrailPoint_080d05fc {
    s32 unknown_00[3];
    s32 x;
    s32 y;
    s32 unknown_14;
    s32 unknown_18;
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
void Func_080049ac(void);
void Func_08004c1c(s32);
void Func_08004c6c(s32);
void Func_08004cf0(void *);
void Func_080051d8(void *, void *);
void Func_08005340(const void *, void *);
struct ObjectContext_080d05fc *Func_080b5098(s16);
void Func_080b50e8(s32);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080cef64(s32, Renderer_080d05fc *);
void Func_080d40ec(s32, s32, s32, s32);
void Func_080de2f8(void *, s32, s32, s32, s32 *, s32 *);
void Func_080e3944(const struct Point_080d05fc *, struct Point_080d05fc *);
void Func_080e396c(s16, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080d05fc(struct Scene_080d05fc *scene)
{
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    u8 *effect_tiles = (u8 *)header[2];
    void *effect_context = *(void **)0x03001e80;
    Transfer_080d05fc transfer = (Transfer_080d05fc)0x03001388;
    struct TrailPoint_080d05fc *trail =
        (struct TrailPoint_080d05fc *)(runtime + 0x7080);
    s32 intro_x = 0;
    s32 intro_y = 0;
    s32 object_position;
    s32 base_x;
    s32 frame;
    u8 *resource;

    *(struct Scene_080d05fc **)(runtime + 0x7828) = scene;
    Func_080cd594(1);
    if (scene->intro == 1)
        Func_080de2f8(
            scene, 3, scene->direction, 0, &intro_x, &intro_y);
    *(volatile u16 *)0x04000020 = 0x100;

    resource = Func_08002f40(0x79);
    transfer((void *)0x05000000, resource, 0x80);
    Func_08005340(resource + 0x80, runtime);
    Func_08005340(Func_08002f40(0x73), effect_tiles);
    Func_08005340(Func_08002f40(0x76), runtime + 0x1000);
    Func_08005340(
        (u8 *)Func_08002f40(0x8f) + 0x80,
        runtime + 0x2000);
    *(s32 *)(runtime + 0x7780) = 3;
    *(s32 *)(runtime + 0x7784) = 0x04040404;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080e396c(scene->object_ids[0], &object_position);
    base_x = 64 - object_position;
    *(volatile s32 *)0x04000028 = base_x << 8;
    Func_080f9010(0x8e);

    for (frame = 0; frame < scene->object_count * 20 + 72; frame++) {
        Renderer_080d05fc renderer0;
        Renderer_080d05fc renderer1;
        struct Object_080d05fc *object;
        struct Point_080d05fc world;
        struct Point_080d05fc projected;
        s32 phase = frame;
        s32 point;

        if (frame == 64)
            Func_080b50e8(0);
        Func_080d40ec(frame, 0xaaab, 0x5555, 0);

        if (scene->intro == 1) {
            Renderer_080d05fc intro_renderers[2];
            s32 angle = frame << 11;
            s32 x = ((Func_08002322(angle) * 20) >> 16)
                + intro_x + base_x - 20;
            s32 y = ((Func_0800231c(angle) * 4) >> 16)
                + intro_y - 24;

            Func_080cef64(scene->direction, intro_renderers);
            if (frame > 32)
                y = y - frame * 2 + 64;
            intro_renderers[0](
                render_context, runtime + 0x2000,
                x, y, 40, 40);
            if (frame <= 3)
                intro_renderers[1](
                    render_context, runtime + 0x2000,
                    x, y, 40, 40);
            Func_08002dd8(47);
            Func_08002dd8(46);
        }

        Func_080ed408(46, 7, 7, 3, 2);
        renderer0 = *(Renderer_080d05fc *)0x03001f08;
        Func_080ed408(47, 7, 7, 7, 2);
        renderer1 = *(Renderer_080d05fc *)0x03001f0c;
        if (frame > 16 && (frame & 15) == 0)
            *(s32 *)(runtime + 0x7784) += 0x01010101;

        object = Func_080b5098(scene->object_ids[0])->object;
        if ((u32)phase <= 95) {
            Func_080049ac();
            Func_080051d8(
                effect_context, (u8 *)effect_context + 12);
            world.x = object->x;
            world.y = object->y;
            world.z = object->z;
            Func_080e3944(&world, &projected);
            projected.x = object_position + base_x;
            projected.y -= 24;

            if (phase <= 67) {
                s32 color = 0x2a000 - frame * 0x600;
                s32 intensity = (64 - phase) << 9;

                for (point = 0; point < 3; point++) {
                    struct Point_080d05fc offset;

                    Func_080049ac();
                    if (phase <= 63) {
                        struct Point_080d05fc colors =
                            { color, color, color };
                        Func_08004cf0(&colors);
                        Func_08004c6c(intensity);
                        Func_08004c1c(intensity);
                    }
                    Func_08004c6c(point * 0x5555);
                    Func_080e3944(
                        (const struct Point_080d05fc *)0x080ee128,
                        &offset);
                    trail[point].x = offset.x + projected.x;
                    trail[point].y = offset.y + projected.y + 16;
                }

                for (point = 0; point < 3; point++) {
                    struct TrailPoint_080d05fc *start = &trail[point];
                    struct TrailPoint_080d05fc *end =
                        &trail[Func_080022fc(point + 1, 3)];
                    s32 size = 5 - phase / 16;
                    s32 step;

                    for (step = 0; step < 24; step++) {
                        s32 x = start->x + Func_080022ec(
                            step * (end->x - start->x), 24);
                        s32 y = start->y + Func_080022ec(
                            step * (end->y - start->y), 24);
                        renderer0(
                            render_context,
                            runtime + 0x1000
                                + *(u16 *)(0x080ede5c + size * 2 - 2),
                            x - size, y - size,
                            size * 2, size * 2);
                    }
                }
            }

            if (phase > 63) {
                renderer0(
                    render_context, runtime,
                    projected.x - 24, projected.y - 24,
                    24, 48);
                renderer1(
                    render_context, runtime,
                    projected.x, projected.y - 24,
                    24, 48);
            }
        }

        Func_08002dd8(47);
        Func_08002dd8(46);
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_080cdbc0();
}
