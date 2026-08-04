#include "types.h"

typedef void (*Transfer_080ce85c)(
    void *destination,
    const void *source,
    u32 size);

typedef void (*Renderer_080ce85c)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Scene_080ce85c {
    s32 unknown_00;
    s32 mode;
    u8 unknown_08[12];
    s32 count;
    u8 unknown_18[12];
    s16 object_ids[1];
};

struct Object_080ce85c {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct ObjectContext_080ce85c {
    struct Object_080ce85c *object;
};

void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_08005340(const void *, void *);
struct ObjectContext_080ce85c *Func_080b5098(s32);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e3944(const s32 *, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);

void Func_080ce85c(struct Scene_080ce85c *scene)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    Transfer_080ce85c transfer = (Transfer_080ce85c)0x03001388;
    Renderer_080ce85c renderers[2];
    void *effect_context;
    void *resource;
    s32 horizontal_offset;
    s32 frame;
    s32 total_frames;
    s32 index;

    *(struct Scene_080ce85c **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    *(volatile u16 *)0x04000020 = 0x100;

    resource = Func_08002f40(0xaf);
    transfer((void *)0x05000000, resource, 0x80);
    Func_08005340((u8 *)resource + 0x80, runtime);
    Func_080ed408(46, 7, 7, 3, 2);
    renderers[0] = (Renderer_080ce85c)runtime_header[7];
    Func_080ed408(47, 7, 7, 15, 2);
    renderers[1] = (Renderer_080ce85c)runtime_header[8];

    Func_080041d8((const void *)0x080dbb9d, 0x480);
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 50;
    Func_080041d8((const void *)0x080cd261, 0x480);

    if (scene->mode == 1) {
        *(volatile s32 *)0x04000028 = -0x6800;
        horizontal_offset = -112;
    } else {
        horizontal_offset = 0;
    }

    frame = 0;
    total_frames = scene->count * 16 + 48;
    if (total_frames != 0) {
        do {
            s32 *wave = (s32 *)(runtime + 0x6980);

            effect_context = *(void **)0x03001e80;
            if (scene->mode == 0) {
                for (index = 0; index < 160; index++) {
                    wave[index] =
                        (0x80000 - Func_08002322(frame * 0x400 +
                                                index * 0x400) * 8) >> 10;
                }
            } else {
                for (index = 0; index < 160; index++) {
                    wave[index] =
                        (Func_08002322(frame * 0x400 +
                                      index * 0x400) * 8 >> 10) - 0x7000;
                }
            }

            Func_080049ac();
            Func_080051d8(effect_context, (u8 *)effect_context + 12);

            for (index = 0; index < scene->count; index++) {
                s32 start = index * 16;
                struct Object_080ce85c *object =
                    Func_080b5098(scene->object_ids[index])->object;

                if (frame > start && frame < start + 60) {
                    s32 source_point[3];
                    s32 projected[3];
                    s32 angle;
                    s32 quadrant;
                    s32 point;

                    if (frame == start + 32) {
                        Func_080d6888(scene->object_ids[index],
                                      0, 5, -1, 0);
                    }

                    source_point[0] = object->x;
                    source_point[1] = 0x280000;
                    source_point[2] = object->z;
                    Func_080e3944(source_point, projected);

                    angle = frame << 9;
                    quadrant = (frame >> 2) - ((frame >> 4) * 4);
                    for (point = 0; point < 4; point++, angle += 0x4000) {
                        s32 x = projected[0] +
                            ((Func_08002322(angle) * 16) >> 16) +
                            horizontal_offset;
                        s32 y = projected[1] +
                            ((Func_0800231c(angle) * 16) >> 16);

                        renderers[(frame >> 4) & 1](
                            render_context,
                            runtime + quadrant * 0x400,
                            x - 16,
                            y - 16,
                            32,
                            32);
                    }
                }
            }

            *(s32 *)(runtime + 0x7824) = 1;
            Func_080030f8(1);
            frame++;
        } while (frame != total_frames);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08004278((const void *)0x080dbb9d);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
