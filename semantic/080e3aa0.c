#include "types.h"

typedef void (*Transfer_080e3aa0)(void *, const void *, u32);
typedef void (*Renderer_080e3aa0)(
    void *, const void *, s32, s32, s32, s32);

struct Scene_080e3aa0 {
    s32 mode;
    s32 direction;
    s32 focus_id;
    u8 unknown_0c[24];
    s16 object_ids[1];
};

struct Object_080e3aa0 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x114];
    u8 status128;
};

struct Particle_080e3aa0 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 life;
};

s32 Func_080022ec(s32, s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(u32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_08005268(const struct Particle_080e3aa0 *, s32 *);
u8 *Func_08077008(s32);
s32 Func_080b5058(u8);
u32 Func_080b5070(s16);
void Func_080b5088(s16, s32);
struct Object_080e3aa0 **Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080c9048(void);
void Func_080cdd14(void);
void Func_080cdd58(void);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e38b8(struct Particle_080e3aa0 *, s32, s32);
void Func_080e3958(s16, s32 *);
void Func_080e396c(s32, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);

static s32 HalfTowardZero_080e3aa0(s32 value)
{
    return (value + ((u32)value >> 31)) >> 1;
}

void Func_080e3aa0(struct Scene_080e3aa0 *scene)
{
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    u8 *graphics = (u8 *)header[2];
    void *projection = *(void **)0x03001e80;
    Transfer_080e3aa0 transfer = (Transfer_080e3aa0)0x03001388;
    Renderer_080e3aa0 renderers[2];
    struct Particle_080e3aa0 *particles =
        (struct Particle_080e3aa0 *)(runtime + 0x7080);
    struct Object_080e3aa0 *object;
    u8 *battle_state;
    s32 anchor[3];
    s32 position[2];
    s32 camera_offset;
    u32 frame;
    u32 i;

    *(struct Scene_080e3aa0 **)(runtime + 0x7828) = scene;
    battle_state = Func_08077008(scene->focus_id);
    Func_080030f8(1);
    Func_080c9048();
    Func_080cdd58();
    *(volatile u16 *)0x0400000a = 0x1f80;
    Func_080030f8(1);

    if (scene->mode == 5) {
        if (scene->direction == 0) {
            Func_080ed408(46, 7, 7, 11, 3);
            Func_080ed408(47, 7, 7, 11, 2);
        } else {
            Func_080ed408(46, 7, 7, 15, 3);
            Func_080ed408(47, 7, 7, 15, 2);
        }
    } else {
        if (scene->direction == 0) {
            Func_080ed408(46, 7, 7, 3, 3);
            Func_080ed408(47, 7, 7, 3, 2);
        } else {
            Func_080ed408(46, 7, 7, 7, 3);
            Func_080ed408(47, 7, 7, 7, 2);
        }
    }
    renderers[0] = (Renderer_080e3aa0)header[7];
    renderers[1] = (Renderer_080e3aa0)header[8];
    Func_080030f8(1);

    if (scene->mode == 4 || (scene->mode != 2 && scene->mode != 3))
        Func_080e0524(scene->mode == 4 ? 0x6b : 0xb5, runtime, 1, 1);
    else if (scene->mode == 3)
        Func_080e0524(0xc5, runtime, 0, 0);
    else
        Func_080e0524(0xb6, runtime, 1, 1);

    if (scene->focus_id > 7)
        transfer(
            (void *)0x05000000, Func_08002f40(0x8e), 0x80);
    else
        transfer(
            (void *)0x05000000, Func_08002f40(0x4a), 0x80);
    Func_080030f8(1);
    Func_080e0524(0x76, graphics, 0, 0);
    Func_080e0524(0x99, (void *)0x02010000, 1, 0);
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 50;
    Func_080041d8((const void *)0x080cd261, 0x480);
    *(volatile u16 *)0x0400000a = 0x1f81;

    Func_080e3958(scene->object_ids[0], anchor);
    camera_offset =
        (scene->direction == 0 ? 96 : 32) - anchor[0];
    if (camera_offset > 0)
        camera_offset = 0;
    if (camera_offset < -128)
        camera_offset = -128;
    anchor[0] += camera_offset;
    *(volatile u16 *)0x03001ad4 = camera_offset;
    *(volatile s16 *)0x03001ad6 = 80;
    Func_080030f8(1);

    object = *Func_080b5098(scene->object_ids[0]);
    {
        s32 y_offset =
            HalfTowardZero_080e3aa0(Func_080b5070(scene->object_ids[0]));

        for (i = 0; i < 64; i++) {
            particles[i].x = object->x;
            particles[i].y = object->y + y_offset;
            particles[i].z = object->z;
            particles[i].velocity_x =
                (Func_08004458() & 0xff) << 10;
            particles[i].velocity_y =
                ((Func_08004458() & 0xff) - 32) << 10;
            particles[i].velocity_z =
                ((Func_08004458() & 0xff) - 127) << 10;
            if (particles[i].x > 0)
                particles[i].velocity_x = -particles[i].velocity_x;
            particles[i].velocity_x = -particles[i].velocity_x;
            particles[i].life = i + 16;
        }
    }

    for (frame = 0; frame < 32; frame++) {
        s32 original_y;
        const void *main_source = 0;
        s32 main_x = 0;
        s32 main_y = 0;
        s32 main_width = 0;
        s32 main_height = 0;
        s32 render_main_left = 0;

        if (frame == 5) {
            if (Func_080b5058(battle_state[0x128]))
                Func_080b50e8(0x86);
            else
                Func_080b50e8(0x85);
        }
        if (frame == 4)
            Func_080b5088(scene->object_ids[0], 0);

        Func_080e396c(scene->focus_id, position);
        original_y = position[1];
        position[1] += 16;

        if (scene->mode == 4 && frame <= 11) {
            main_source = runtime + (5 - frame / 2) * 0x300;
            main_x = position[0] + camera_offset
                - (scene->direction == 0 ? 48 : 0);
            main_y = original_y + 8;
            main_width = 48;
            main_height = 16;
            render_main_left = 1;
        } else if ((u32)scene->mode <= 2 || scene->mode == 5) {
            if (frame <= 11) {
                main_source = runtime + (frame / 2) * 0xd80;
                main_x = position[0] + camera_offset
                    - (scene->direction == 0 ? 48 : 0);
                main_y = original_y - 24;
                main_width = 48;
                main_height = 72;
                render_main_left = 1;
            }
        } else if (frame <= 17) {
            s32 stage = Func_080022ec(frame, 3);
            s32 width = *(u8 *)(0x080eedb2 + stage);
            s32 height = *(u8 *)(0x080eedb8 + stage);
            const void *source =
                runtime + *(u16 *)(0x080eedbe + stage * 2);

            if (scene->direction == 0)
            {
                main_source = source;
                main_x = position[0]
                    + *(u8 *)(0x080eedca + stage)
                    + camera_offset - 58;
                main_y = position[1] - (height >> 1);
                main_width = width;
                main_height = height;
                render_main_left = 1;
            }
            else
                renderers[1](
                    render_context, source,
                    position[0]
                        - *(u8 *)(0x080eedca + stage)
                        + camera_offset - width + 58,
                    position[1] - (height >> 1),
                    width, height);
        }
        if (render_main_left)
            renderers[0](
                render_context, main_source,
                main_x, main_y, main_width, main_height);

        if ((u32)(frame - 4) <= 11)
            renderers[0](
                render_context,
                (u8 *)0x02010000 + ((frame - 4) / 2) * 0x780,
                anchor[0] - 16,
                anchor[1] - 24,
                40, 48);

        Func_080049ac();
        Func_080051d8(projection, (u8 *)projection + 12);
        if ((u32)(frame - 4) <= 27) {
            u32 j;

            for (j = 0; j < 64; j++) {
                struct Particle_080e3aa0 *particle = &particles[j / 2];

                if (particle->life > 0) {
                    s32 projected[2];
                    s32 size = (particle->life >> 3) + 2;
                    s32 diameter = size * 2;

                    Func_08005268(particle, projected);
                    projected[0] += camera_offset;
                    renderers[(j / 2) & 1](
                        render_context,
                        graphics
                            + *(u16 *)(0x080ede5c + diameter - 2),
                        projected[0] - size,
                        projected[1] - size,
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
    *(volatile s16 *)0x03001ad6 = 32;
    Func_080cdd14();
}
