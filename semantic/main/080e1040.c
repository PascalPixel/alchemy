#include "layout_guard.h"
#include "types.h"

struct Scene_080e1040 {
    u8 unknown_00[4];
    s32 direction;
    s32 primary_object_id;
    u8 unknown_0c[8];
    s32 object_count;
    u8 unknown_18[4];
    s32 animated;
    u8 unknown_20[4];
    s16 object_ids[1];
};
struct Object_080e1040 {
    u8 unknown_00[8];
    s32 x;
    u8 unknown_0c[4];
    s32 z;
};
struct ObjectContext_080e1040 { struct Object_080e1040 *object; };
struct Particle_080e1040 {
    s32 x, y, z, velocity_x, velocity_y, velocity_z, active;
};
struct Decoration_080e1040 {
    u8 unknown_00[12];
    s32 image;
    s32 flags;
    u8 unknown_14[8];
};
struct Point_080e1040 { s32 x, y, z; };
typedef void (*Renderer_080e1040)(
    void *, const void *, s32, s32, s32, s32);

LAYOUT_OFFSET_GUARD(Scene080e1040_Animated, struct Scene_080e1040, animated, 0x1c);
LAYOUT_OFFSET_GUARD(Scene080e1040_Ids, struct Scene_080e1040, object_ids, 0x24);
LAYOUT_SIZE_GUARD(Particle080e1040, struct Particle_080e1040, 0x1c);
LAYOUT_SIZE_GUARD(Decoration080e1040, struct Decoration_080e1040, 0x1c);

s32 Func_08002304(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
struct ObjectContext_080e1040 *Func_080b5098(s32);
void Func_080b5088(s16, s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080de2f8(void *, s32, s32, s32, u32 *, s32 *);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e3944(const struct Point_080e1040 *, struct Point_080e1040 *);
void Func_080e46f0(s32);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080e1040(struct Scene_080e1040 *scene)
{
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    void *effect_context = *(void **)0x03001e80;
    Renderer_080e1040 renderer46;
    struct Object_080e1040 *primary;
    struct Object_080e1040 *target;
    struct Particle_080e1040 *particles =
        (struct Particle_080e1040 *)(runtime + 0x7080);
    struct Decoration_080e1040 *decorations =
        (struct Decoration_080e1040 *)(runtime + 0x7400);
    u32 center_x;
    s32 center_y;
    s32 frame;
    s32 index;

    *(struct Scene_080e1040 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    Func_080de2f8(
        scene, 0, scene->direction, 2, &center_x, &center_y);
    *(volatile u16 *)0x04000052 = 0x1010;
    if (scene->direction == 1)
        Func_080ed408(46, 7, 7, 7, 2);
    else
        Func_080ed408(46, 7, 7, 3, 2);
    renderer46 = (Renderer_080e1040)header[7];
    Func_080e0524(0xa7, runtime, 1, 0);
    Func_080e0524(0x94, runtime + 0x65c0, 1, 1);
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    primary = Func_080b5098(scene->primary_object_id)->object;
    target = Func_080b5098(scene->object_ids[0])->object;
    for (index = 0; index < 8; index++) {
        struct Particle_080e1040 *p = &particles[index];
        p->x = primary->x;
        p->y = 0x420000;
        p->z = primary->z;
        p->velocity_x = index * 0x500000 >> 5;
        p->velocity_y =
            (((Func_08004458() & 0x7f) - 64) << 16) >> 6;
        p->velocity_z =
            (((Func_08004458() & 0xff) - 127) << 16) >> 5;
        if (p->x > 0)
            p->velocity_x = -p->velocity_x;
        p->active = 1;
    }

    for (frame = 0; frame < 96; frame++) {
        if (frame > 16)
            Func_080e46f0(0xa7);

        if (scene->animated == 1) {
            s32 angle = frame << 11;
            s32 x =
                (-Func_08002322(angle) * 4 >> 16) +
                ((s32)(center_x + ((s32)center_x >> 31)) >> 1) - 10;
            s32 y =
                (Func_0800231c(angle) * 2 >> 16) + center_y - 22;
            Renderer_080e1040 renderer47;

            if (frame > 16)
                y = y - frame * 2 + 32;
            if (scene->direction == 1)
                Func_080ed408(47, 7, 7, 7, 3);
            else
                Func_080ed408(47, 7, 7, 3, 3);
            renderer47 = (Renderer_080e1040)header[8];
            if (frame <= 3)
                renderer47(
                    render_context, runtime + 0x65c0,
                    x, y, 20, 40);
            Func_08002dd8(47);
            renderer46(
                render_context, runtime + 0x65c0,
                x, y, 20, 40);
        }

        if ((frame & 1) == 0) {
            static const u8 values[4] = { 3, 5, 6, 7 };
            for (index = 0; index < 32; index++) {
                decorations[index].image =
                    Func_08002304(Func_08004458(), 6) + 3;
                decorations[index].flags =
                    values[Func_08004458() & 3];
            }
        }

        Func_080049ac();
        Func_080051d8(effect_context, (u8 *)effect_context + 12);
        for (index = 0; index < 6; index++) {
            struct Particle_080e1040 *p = &particles[index];

            if (p->active == 1) {
                s32 start = index * 2;

                if (frame > start) {
                    struct Point_080e1040 screen;
                    s32 x, y;
                    s32 decoration;

                    Func_080e3944(
                        (const struct Point_080e1040 *)p, &screen);
                    x = (screen.x >> 1) - 12;
                    y = screen.y - 24;
                    renderer46(
                        render_context, runtime,
                        x, y, 24, 48);
                    if ((frame & 3) <= 1) {
                        renderer46(
                            render_context,
                            runtime + *(const u16 *)0x080eec88,
                            x + *(const u8 *)0x080eec99,
                            y + *(const u8 *)0x080eeca2,
                            *(const u8 *)0x080eec75,
                            *(const u8 *)0x080eec7e);
                    } else {
                        renderer46(
                            render_context,
                            runtime + *(const u16 *)0x080eec8a,
                            x + *(const u8 *)0x080eec9a,
                            y + *(const u8 *)0x080eeca3,
                            *(const u8 *)0x080eec76,
                            *(const u8 *)0x080eec7f);
                    }

                    for (decoration = 0;
                         decoration < 4;
                         decoration++) {
                        struct Decoration_080e1040 *d =
                            &decorations[index * 4 + decoration];
                        Renderer_080e1040 renderer47;
                        s32 image = d->image;
                        s32 draw_x;
                        s32 draw_y;

                        Func_080ed408(
                            47, 7, 7, d->flags, 2);
                        renderer47 = (Renderer_080e1040)header[8];
                        if (d->flags & 4)
                            draw_x = x -
                                ((const u8 *)0x080eec74)[image] -
                                ((const u8 *)0x080eec98)[image] + 24;
                        else
                            draw_x = x +
                                ((const u8 *)0x080eec98)[image];
                        if (d->flags & 8)
                            draw_y = y -
                                ((const u8 *)0x080eec7d)[image] -
                                ((const u8 *)0x080eeca1)[image] + 48;
                        else
                            draw_y = y +
                                ((const u8 *)0x080eeca1)[image];
                        renderer47(
                            render_context,
                            runtime + ((const u16 *)0x080eec86)[image],
                            draw_x, draw_y,
                            ((const u8 *)0x080eec74)[image],
                            ((const u8 *)0x080eec7d)[image]);
                        Func_08002dd8(47);
                    }

                    p->x += p->velocity_x;
                    p->y += p->velocity_y;
                    p->z += p->velocity_z;
                }

                if (frame > start + 16) {
                    p->velocity_x += (target->x - p->x) >> 8;
                    p->velocity_y += (0x140000 - p->y) >> 8;
                    p->velocity_z += (target->z - p->z) >> 8;
                    if (frame < start + 85) {
                        p->velocity_x = p->velocity_x * 60 / 64;
                        p->velocity_y = p->velocity_y * 60 / 64;
                        p->velocity_z = p->velocity_z * 60 / 64;
                    }
                    if (p->y <= 0x13ffff) {
                        *(s32 *)(runtime + 0x77a8) = 8;
                        p->active = 0;
                        Func_080f9010(0x86);
                        Func_080d6888(
                            scene->object_ids[0], 7, 5, 0, 4);
                        Func_080b5088(scene->object_ids[0], 4);
                    }
                }
            }
        }

        Func_080e155c(16, 16);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
