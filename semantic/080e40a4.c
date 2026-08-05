#include "types.h"

typedef void (*Transfer_080e40a4)(void *, const void *, u32);
typedef void (*OwnerInit_080e40a4)(void *, u32);
typedef void (*Renderer_080e40a4)(
    void *, const void *, s32, s32, s32, s32);

struct Scene_080e40a4 {
    s32 mode;
    s32 direction;
    s32 focus_id;
    s32 focus_argument;
    u8 unknown_10[0x14];
    s16 object_ids[1];
};

struct Object_080e40a4 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[16];
    s32 saved_24;
    s32 saved_28;
    s32 saved_2c;
    u8 unknown_30[4];
    s32 saved_34;
    u8 unknown_38[16];
    s32 saved_48;
};

struct ObjectContext_080e40a4 {
    struct Object_080e40a4 *object;
};

struct Point_080e40a4 {
    s32 x;
    s32 y;
    s32 z;
};

struct Particle_080e40a4 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 life;
};

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
void Func_080051d8(void *, void *);
void Func_08009088(struct Object_080e40a4 *, s32);
void Func_080b5030(s32, s32, s32);
void Func_080b5048(u16, s32);
u32 Func_080b5070(s16);
void Func_080b5088(s16, s32);
struct ObjectContext_080e40a4 *Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cdd14(void);
void Func_080cdd58(void);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e38b8(struct Particle_080e40a4 *, s32, s32);
void Func_080e3944(
    const struct Point_080e40a4 *, struct Point_080e40a4 *);
void Func_080e396c(s32, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080e40a4(struct Scene_080e40a4 *scene)
{
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    u8 *effect_tiles = (u8 *)header[2];
    void *projection = *(void **)0x03001e80;
    Transfer_080e40a4 transfer =
        (Transfer_080e40a4)0x03001388;
    OwnerInit_080e40a4 initialize_owner =
        (OwnerInit_080e40a4)0x03000164;
    Renderer_080e40a4 renderers[2];
    struct Particle_080e40a4 *particles =
        (struct Particle_080e40a4 *)(runtime + 0x7080);
    s32 special = scene->mode > 199;
    s32 focus_position[2];
    s32 camera_offset;
    u32 frame;
    u32 i;

    *(struct Scene_080e40a4 **)(runtime + 0x7828) = scene;
    Func_080b5030(scene->focus_id, scene->focus_argument, 130);
    Func_080030f8(1);
    Func_080cdd58();
    *(volatile u16 *)0x0400000a = 0x1f80;

    if (scene->direction == 0) {
        Func_080ed408(46, 7, 7, 3, 3);
        Func_080ed408(47, 7, 7, 3, 2);
    } else {
        Func_080ed408(46, 7, 7, 7, 3);
        Func_080ed408(47, 7, 7, 7, 2);
    }
    renderers[0] = *(Renderer_080e40a4 *)0x03001f08;
    renderers[1] = *(Renderer_080e40a4 *)0x03001f0c;

    Func_080b5030(scene->focus_id, scene->focus_argument, 130);
    Func_080030f8(1);
    Func_080e0524(0x49, runtime, 1, 0);
    Func_080b5030(scene->focus_id, scene->focus_argument, 130);
    Func_080030f8(1);
    Func_080e0524(0x4a, (void *)0x02010000, 1, 1);
    if (scene->focus_id > 7)
        transfer(
            (void *)0x05000000, Func_08002f40(0x8e), 0x80);
    Func_080b5030(scene->focus_id, scene->focus_argument, 130);
    Func_080030f8(1);
    Func_080e0524(0x76, effect_tiles, 0, 0);
    Func_080b5030(scene->focus_id, scene->focus_argument, 130);
    Func_080030f8(1);

    *(s32 *)(runtime + 0x7780) = 1;
    *(s32 *)(runtime + 0x7784) = 0;
    Func_080041d8((const void *)0x080cd261, 0x480);

    if (special) {
        struct Object_080e40a4 *object =
            Func_080b5098(scene->focus_id)->object;
        s32 saved_24;
        s32 saved_28;
        s32 saved_2c;
        s32 saved_34;
        s32 saved_48;

        for (i = 0; i < 64; i++) {
            particles[i].x = (Func_08004458() & 0x3f) + 16;
            particles[i].y = 0;
            particles[i].z = 0;
            particles[i].velocity_x =
                Func_08004458() & 0xffff;
            particles[i].velocity_y =
                Func_08004458() & 0xffff;
            particles[i].velocity_z =
                Func_08004458() & 0xffff;
        }

        Func_08009088(object, 0);
        saved_24 = object->saved_24;
        saved_28 = object->saved_28;
        saved_2c = object->saved_2c;
        saved_34 = object->saved_34;
        saved_48 = object->saved_48;
        object->saved_24 = 0;
        object->saved_28 = 0;
        object->saved_2c = 0;
        object->saved_34 = 0;
        object->saved_48 = 0;

        Func_080e396c(scene->focus_id, focus_position);
        camera_offset = 64 - focus_position[0];
        *(volatile u16 *)0x03001ad4 = camera_offset;
        *(volatile u16 *)0x03001ad6 = 80;
        *(s32 *)(runtime + 0x77b4) = 24;
        *(s32 *)(runtime + 0x77b8) = 0;
        Func_080041d8((const void *)0x080cd4b5, 2000);
        Func_080f9010(0xd4);

        for (frame = 0; frame < 32; frame++) {
            Func_080b5030(
                scene->focus_id, scene->focus_argument, 130);
            for (i = 0; i < 64; i++) {
                struct Particle_080e40a4 *particle =
                    &particles[i];

                if (particle->x >= 0 &&
                    frame >= (i < 0 ? i + 3 : i) / 4) {
                    struct Point_080e40a4 projected;
                    s32 size = (i & 1) + 5;
                    s32 diameter = size * 2;

                    Func_080049ac();
                    Func_08004c6c(particle->velocity_z);
                    Func_08004bd4(particle->velocity_x);
                    Func_08004c1c(particle->velocity_y);
                    Func_080e3944(
                        (const struct Point_080e40a4 *)particle,
                        &projected);
                    projected.x += 64;
                    projected.y += focus_position[1] + 24;
                    if (projected.z < -60)
                        projected.z = -60;
                    if (projected.z > 60)
                        projected.z = 60;
                    projected.z += 60;
                    renderers[0](
                        render_context,
                        effect_tiles
                            + ((const u16 *)0x080ede5c)
                                  [size - 1],
                        projected.x - size,
                        projected.y - size,
                        diameter, diameter);
                    particle->x -= 4;
                }
            }
            *(s32 *)(runtime + 0x7824) = 1;
            Func_080030f8(1);
        }

        Func_08004278((const void *)0x080cd4b5);
        Func_08009088(object, 16);
        object->saved_24 = saved_24;
        object->saved_28 = saved_28;
        object->saved_2c = saved_2c;
        object->saved_34 = saved_34;
        object->saved_48 = saved_48;
    }

    initialize_owner(render_context, 0x4000);
    initialize_owner((void *)0x06004000, 0x4000);
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    *(volatile u16 *)0x0400000a = 0x1f81;

    Func_080e396c(scene->object_ids[0], focus_position);
    camera_offset =
        (scene->direction == 0 ? 32 : 96) - focus_position[0];
    if (camera_offset > 0)
        camera_offset = 0;
    if (camera_offset < -128)
        camera_offset = -128;
    focus_position[0] += camera_offset;
    *(volatile u16 *)0x03001ad4 = camera_offset;
    *(volatile u16 *)0x03001ad6 = 80;

    {
        struct Object_080e40a4 *object =
            Func_080b5098(scene->object_ids[0])->object;
        s32 object_height = Func_080b5070(scene->object_ids[0]);

        object_height =
            (object_height + ((u32)object_height >> 31)) >> 1;
        for (i = 0; i < 64; i++) {
            particles[i].x = object->x;
            particles[i].y = object->y + object_height;
            particles[i].z = object->z;
            particles[i].velocity_x =
                (Func_08004458() & 0xff) << 10;
            particles[i].velocity_y =
                (Func_08004458() & 0xff) << 10;
            particles[i].velocity_z =
                ((Func_08004458() & 0xff) - 127) << 10;
            if (particles[i].x > 0)
                particles[i].velocity_x =
                    -particles[i].velocity_x;
            particles[i].life = i + 16;
        }
    }

    for (frame = 0; frame < 32; frame++) {
        s32 projected_focus[2];

        if (frame == 5)
            Func_080b50e8(0x86);
        if (frame == 4)
            Func_080b5088(scene->object_ids[0], 0);

        Func_080e396c(scene->focus_id, projected_focus);
        projected_focus[1] += 16;
        if (frame <= 1) {
            renderers[0](
                render_context, runtime,
                0, 0, 120, 120);
        } else if (frame <= 3) {
            renderers[0](
                render_context, runtime + 0x3840,
                0, 0, 120, 120);
        } else if (frame <= 5) {
            renderers[0](
                render_context, (const void *)0x02010000,
                0, 0, 120, 120);
        } else if (frame <= 7) {
            renderers[0](
                render_context, (const void *)0x02013840,
                0, 0, 120, 120);
        }

        Func_080049ac();
        Func_080051d8(projection, (u8 *)projection + 12);
        if ((u32)(frame - 4) <= 27) {
            for (i = 0; i < 64; i++) {
                struct Particle_080e40a4 *particle =
                    &particles[i / 2];

                if (particle->life > 0) {
                    struct Point_080e40a4 projected;
                    s32 size = (particle->life >> 3) + 2;
                    s32 diameter = size * 2;

                    Func_080e3944(
                        (const struct Point_080e40a4 *)particle,
                        &projected);
                    projected.x += camera_offset;
                    projected.y += 16;
                    renderers[(i / 2) & 1](
                        render_context,
                        effect_tiles
                            + ((const u16 *)0x080ede5c)
                                  [size - 1],
                        projected.x - size,
                        projected.y - size,
                        diameter, diameter);
                    Func_080e38b8(particle, 60, -0x400);
                    particle->life--;
                }
            }
        }

        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    *(volatile u16 *)0x03001ad6 = 32;

    {
        const u16 *entry =
            (const u16 *)(*(u8 **)0x03001e74 + 0x648);

        for (i = 0; i < 7; i++) {
            Func_080b5048(*entry, 6 - i);
            Func_080030f8(1);
        }
    }
    Func_080cdd14();
}
