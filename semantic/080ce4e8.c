#include "layout_guard.h"
#include "types.h"

struct Scene_080ce4e8 {
    u8 unknown_00[4];
    s32 direction;
    u8 unknown_08[12];
    s32 object_count;
    u8 unknown_18[12];
    s16 object_ids[1];
};
struct Object_080ce4e8 {
    u8 unknown_00[8];
    s32 x;
    s32 unknown_0c;
    s32 z;
};
struct ObjectContext_080ce4e8 {
    struct Object_080ce4e8 *object;
};
struct Particle_080ce4e8 {
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_0c;
    s32 unknown_10;
    s32 unknown_14;
    s32 age;
};
struct Point_080ce4e8 {
    s32 x;
    s32 y;
    s32 z;
};

typedef void (*Transfer_080ce4e8)(void *, const void *, s32);
typedef void (*Renderer_080ce4e8)(
    void *, const void *, s32, s32, s32, s32);
typedef s32 (*Magnitude_080ce4e8)(s32);

LAYOUT_OFFSET_GUARD(Scene080ce4e8_Count, struct Scene_080ce4e8, object_count, 0x14);
LAYOUT_OFFSET_GUARD(Scene080ce4e8_Ids, struct Scene_080ce4e8, object_ids, 0x24);
LAYOUT_SIZE_GUARD(Particle080ce4e8, struct Particle_080ce4e8, 0x1c);

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_08004bd4(s32);
void Func_08004cb4(void *);
void Func_080051d8(void *, void *);
void Func_08005340(const void *, void *);
struct ObjectContext_080ce4e8 *Func_080b5098(s16);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e3944(const struct Point_080ce4e8 *, struct Point_080ce4e8 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080ce4e8(struct Scene_080ce4e8 *scene)
{
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    Transfer_080ce4e8 transfer = (Transfer_080ce4e8)0x03001388;
    Magnitude_080ce4e8 magnitude = (Magnitude_080ce4e8)0x030001d8;
    Renderer_080ce4e8 renderer;
    struct Particle_080ce4e8 *particles =
        (struct Particle_080ce4e8 *)0x02010000;
    void *effect_context;
    s32 frame;
    u32 index;

    *(struct Scene_080ce4e8 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    {
        u8 *palette = Func_08002f40(0xa9);
        transfer((void *)0x05000000, palette, 0x80);
        Func_08005340(palette + 0x80, runtime);
    }
    transfer((void *)0x05000000, Func_08002f40(0xbb), 0x80);

    Func_080ed408(46, 7, 7, 3, 2);
    renderer = (Renderer_080ce4e8)header[7];
    Func_080ed408(47, 7, 7, 3, 3);
    Func_080041d8((const void *)0x080dbb9d, 0x480);
    *(s32 *)(runtime + 0x7780) = 3;
    *(s32 *)(runtime + 0x7784) = 0x04040404;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (index = 0; index < 512; index++) {
        particles[index].x = ((Func_08004458() & 0xff) - 127) << 15;
        particles[index].y = ((Func_08004458() & 0xff) - 127) << 15;
        particles[index].z = ((Func_08004458() & 0xff) - 127) << 15;
    }
    Func_080f9010(0x8e);

    for (frame = 0; frame != scene->object_count * 32 + 96; frame++) {
        s32 *wave = (s32 *)(runtime + 0x6980);
        s32 object_index;

        effect_context = *(void **)0x03001e80;
        if (frame == 96)
            Func_080b50e8(0);

        if (scene->direction == 0) {
            for (index = 0; index < 160; index++) {
                s32 value =
                    Func_08002322((frame + index) << 11) * 6;
                wave[index] = (0x60000 - value) >> 10;
            }
        } else {
            for (index = 0; index < 160; index++) {
                s32 value =
                    Func_08002322((frame + index) << 11) * 6;
                wave[index] = value >> 10;
            }
        }

        for (object_index = 0;
             object_index < scene->object_count;
             object_index++) {
            struct Object_080ce4e8 *object =
                Func_080b5098(scene->object_ids[object_index])->object;
            struct Point_080ce4e8 world;
            struct Particle_080ce4e8 *group =
                &particles[object_index * 64];

            Func_080049ac();
            Func_080051d8(effect_context, (u8 *)effect_context + 12);
            world.x = object->x;
            world.y = 0x140000;
            world.z = object->z;
            Func_08004cb4(&world);

            if (frame > object_index * 32) {
                Func_08004bd4(frame << 9);
                if (frame == object_index * 32 + 32)
                    Func_080d6888(
                        scene->object_ids[object_index],
                        7, 5, object_index, 32);

                for (index = 0; index < 8; index++) {
                    struct Particle_080ce4e8 *particle = &group[index];

                    if (frame > (object_index * 8 + index) * 4) {
                        s32 x = particle->x >> 8;
                        s32 y = particle->y >> 8;
                        s32 z = particle->z >> 8;
                        s32 divisor = magnitude(x * x + y * y + z * z) >> 8;

                        if (divisor != 0) {
                            struct Point_080ce4e8 projected;

                            Func_080e3944(
                                (const struct Point_080ce4e8 *)particle,
                                &projected);
                            projected.x >>= 1;
                            renderer(
                                render_context,
                                runtime + Func_080022fc(index, 3) * 0x240,
                                projected.x - 12,
                                projected.y - 12,
                                24,
                                24);
                            particle->x -= Func_080022ec(particle->x, divisor);
                            particle->y -= Func_080022ec(particle->y, divisor);
                            particle->z -= Func_080022ec(particle->z, divisor);
                            particle->age++;
                        }
                    }
                }
            }
        }

        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08004278((const void *)0x080dbb9d);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
