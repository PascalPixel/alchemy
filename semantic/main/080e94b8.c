#include "layout_guard.h"
#include "types.h"

struct Scene_080e94b8 {
    u8 unknown_00[4];
    s32 direction;
    s32 object_id;
    u8 unknown_0c[8];
    s32 object_count;
    s32 active;
    u8 unknown_1c[8];
    s16 object_ids[1];
};
struct ObjectContext_080e94b8 { void *object; };
struct Particle_080e94b8 {
    s32 x, y, z, velocity_x, velocity_y, unknown_14, timer;
};
typedef void (*Renderer_080e94b8)(
    void *, const void *, s32, s32, s32, s32);

LAYOUT_OFFSET_GUARD(Scene080e94b8_Count, struct Scene_080e94b8, object_count, 0x14);
LAYOUT_OFFSET_GUARD(Scene080e94b8_Active, struct Scene_080e94b8, active, 0x18);
LAYOUT_OFFSET_GUARD(Scene080e94b8_Ids, struct Scene_080e94b8, object_ids, 0x24);
LAYOUT_SIZE_GUARD(Particle080e94b8, struct Particle_080e94b8, 0x1c);

s32 Func_080022ec(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_08009080(void *, s32);
void Func_08009088(void *, s32);
void Func_080b5088(s16, s32);
struct ObjectContext_080e94b8 *Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080cef64(s32, Renderer_080e94b8 *);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e3908(struct Particle_080e94b8 *, s32, s32);
void Func_080e46f0(s32);
void Func_080f9010(s32);

void Func_080e94b8(struct Scene_080e94b8 *scene)
{
    static const u8 *const x_positions = (const u8 *)0x080eef06;
    static const u8 *const frames = (const u8 *)0x080eef0c;
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    u8 *effect_tiles = (u8 *)header[2];
    void *object = Func_080b5098(scene->object_id)->object;
    Renderer_080e94b8 renderers[2];
    struct Particle_080e94b8 *orbit =
        (struct Particle_080e94b8 *)(runtime + 0x7080);
    struct Particle_080e94b8 *particles =
        (struct Particle_080e94b8 *)0x02010000;
    s32 group, index, frame;

    scene->active = 1;
    *(struct Scene_080e94b8 **)(runtime + 0x7828) = scene;
    Func_080cd594(1);
    *(volatile u16 *)0x04000052 = 0x1010;
    Func_080cef64(scene->direction, renderers);
    Func_08009080(object, 2);
    Func_08009088(object, 48);
    Func_080e0524(0x55, runtime, 1, 1);
    Func_080e0524(0x7d, runtime + 0x2000, 1, 0);
    Func_080e0524(0x73, effect_tiles, 0, 0);

    for (group = 0; group < 3; group++) {
        for (index = 0; index < 16; index++) {
            s32 angle = Func_08004458() & 0xffff;
            orbit[group * 16 + index].x =
                index * 2 * Func_08002322(angle);
            orbit[group * 16 + index].y =
                -(index * 2 * Func_0800231c(angle));
            orbit[group * 16 + index].timer = index / 2 + 25;
        }
        for (index = 0; index < 340; index++) {
            struct Particle_080e94b8 *p = &particles[group * 340 + index];
            s32 magnitude = (Func_08004458() & 0x1ff) + 32;
            s32 angle = Func_08004458() & 0xffff;

            p->x = x_positions[scene->direction * 3 + group] << 16;
            p->y = 0x580000;
            p->velocity_x = magnitude * Func_08002322(angle) >> 6;
            p->velocity_y = -(magnitude * Func_0800231c(angle) * 2) >> 6;
            p->timer = (Func_08004458() & 7) + 32;
        }
    }

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (frame = 0; frame < 80; frame++) {
        if (frame == 4) Func_080f9010(0xd4);
        if (frame == 8) *(s32 *)(runtime + 0x77a8) = 8;
        if (frame == 18) Func_080f9010(0x91);
        if (frame == 40) Func_080b50e8(0x86);

        if (frame <= 39) {
            s32 x, y, height = 128;

            if (scene->direction == 1) {
                if (frame <= 9) { x = frame * 10 - 8; y = frame * 16 - 128; }
                else if (frame > 20) { x = frame + 62; y = frame * 2 - 24; }
                else { x = 82; y = 16; }
            } else {
                if (frame <= 9) { x = 128 - frame * 10; y = frame * 16 - 128; }
                else if (frame > 20) { x = 58 - frame; y = frame * 2 - 24; }
                else { x = 38; y = 16; }
            }
            if (y + 128 > 104)
                height = 128 - y - 24;
            if (height > 0)
                renderers[0](render_context, runtime, x - 32, y, 64, height);
        }
        if (frame > 16)
            Func_080e46f0(0xc0);

        for (group = 0; group < 2; group++) {
            s32 start = 16 + group * 8;
            s32 sound_time = 22 + group * 8;

            if (frame == start)
                *(s32 *)(runtime + 0x77a8) = 12;
            if (frame >= start && frame < group * 8 + 18) {
                renderers[0](
                    render_context,
                    runtime + 0x2000,
                    x_positions[scene->direction * 3 + group] - 16,
                    56, 32, 64);
            }
            if (frame >= start) {
                for (index = 0; index < 12; index++) {
                    struct Particle_080e94b8 *p =
                        &orbit[group * 16 + index];
                    s32 x = (s16)(p->x >> 16) +
                        x_positions[scene->direction * 3 + group];
                    s32 y = (s16)(p->y >> 16);

                    if ((u32)p->timer <= 17) {
                        renderers[0](
                            render_context,
                            runtime + 0x2000 + frames[Func_080022ec(p->timer, 3)] * 0x800,
                            x - 16, y + 56, 32, 64);
                    }
                    p->timer = p->timer > 0 ? p->timer - 1 : -1;
                }
            }

            if (frame > start + 5) {
                struct Particle_080e94b8 *group_particles =
                    &particles[group * 340];

                for (index = 0; index < 256; index++) {
                    struct Particle_080e94b8 *p = &group_particles[index];

                    if (p->timer > 0) {
                        s32 size;
                        Func_080e3908(p, 64, 0x1000);
                        p->timer--;
                        if (p->y > 0x6c0000) {
                            s32 value = -p->velocity_y;
                            p->velocity_y =
                                (value + ((u32)value >> 31)) >> 1;
                        } else if ((u32)p->x <= 0x7effff && p->y >= 0) {
                            size = Func_080022ec(p->timer, 5) + 1;
                            renderers[0](
                                render_context,
                                effect_tiles +
                                    ((const u16 *)0x080ede48)[size - 1],
                                (s16)(p->x >> 16) -
                                    ((size + ((u32)size >> 31)) >> 1),
                                (s16)(p->y >> 16) - size,
                                size, size * 2);
                        }
                    }
                }
            }

            for (index = 0; index < scene->object_count; index++) {
                if (frame == sound_time) {
                    Func_080d6888(scene->object_ids[index], 7, 5, index, 10);
                    Func_080b5088(scene->object_ids[index], 4);
                }
            }
        }

        Func_080e155c(16, 16);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08009088(object, 16);
    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
