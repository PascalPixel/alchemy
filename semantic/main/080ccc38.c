#include "layout_guard.h"
#include "types.h"

struct Scene_080ccc38 {
    u8 unknown_00[4];
    s32 direction;
    u8 unknown_08[0x1c];
    s16 object_id;
};

struct Object_080ccc38 {
    u8 unknown_00[0x28];
    s32 displacement;
    u8 unknown_2c[0x18];
    s32 scale;
    s32 state;
};

struct ObjectContext_080ccc38 {
    struct Object_080ccc38 *object;
};

typedef void (*Transfer_080ccc38)(
    void *destination,
    const void *source,
    s32 size);
typedef void (*Renderer_080ccc38)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

LAYOUT_OFFSET_GUARD(
    Scene080ccc38_Direction,
    struct Scene_080ccc38,
    direction,
    4);
LAYOUT_OFFSET_GUARD(
    Scene080ccc38_ObjectId,
    struct Scene_080ccc38,
    object_id,
    0x24);
LAYOUT_OFFSET_GUARD(
    Object080ccc38_Displacement,
    struct Object_080ccc38,
    displacement,
    0x28);
LAYOUT_OFFSET_GUARD(
    Object080ccc38_Scale,
    struct Object_080ccc38,
    scale,
    0x44);
LAYOUT_OFFSET_GUARD(
    Object080ccc38_State,
    struct Object_080ccc38,
    state,
    0x48);

void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
void Func_080b5088(s16, s32);
struct ObjectContext_080ccc38 *Func_080b5098(s16);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e396c(s16, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080ccc38(struct Scene_080ccc38 *scene, s32 mode)
{
    static const u8 *const widths = (const u8 *)0x080ee064;
    static const u8 *const heights = (const u8 *)0x080ee06a;
    static const u16 *const source_offsets =
        (const u16 *)0x080ee070;
    static const u8 *const x_positions =
        (const u8 *)0x080ee07c;
    static const s8 *const y_positions =
        (const s8 *)0x080ee088;
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    Renderer_080ccc38 renderer;
    Transfer_080ccc38 transfer =
        (Transfer_080ccc38)0x03001388;
    struct Object_080ccc38 *object;
    s32 object_position;
    s32 duration = mode == 1 ? 74 : 48;
    s32 frame;

    *(struct Scene_080ccc38 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    *(volatile u16 *)0x04000020 = 0x100;

    Func_080ed408(
        46,
        7,
        7,
        scene->direction == 1 ? 3 : 7,
        1);
    renderer = (Renderer_080ccc38)runtime_header[7];
    Func_080e0524(0x71, runtime, 1, 1);
    Func_080e0524(0x72, (void *)0x02010000, 1, 0);

    if (mode == 0) {
        transfer(
            (void *)0x05000000,
            Func_08002f40(0xa0),
            0x80);
    }

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    object = Func_080b5098(scene->object_id)->object;
    Func_080e396c(scene->object_id, &object_position);
    *(volatile s32 *)0x04000028 =
        ((scene->direction == 0 ? 16 : 112) -
         object_position) << 8;

    for (frame = 0; frame < duration; frame++) {
        s32 group = frame / 4;

        if (group <= 5) {
            if (group <= 3) {
                renderer(
                    render_context,
                    runtime + source_offsets[group],
                    x_positions[scene->direction * 6 + group],
                    y_positions[group] + 32,
                    widths[group],
                    heights[group]);
            } else {
                renderer(
                    render_context,
                    (u8 *)0x02010000 + source_offsets[group],
                    x_positions[scene->direction * 6 + group],
                    y_positions[group] + 32,
                    widths[group],
                    heights[group]);
            }
        }

        if (frame == 8) {
            if (mode == 0) {
                Func_080b50e8(0x85);
                Func_080b5088(scene->object_id, 1);
            } else {
                Func_080f9010(0x86);
                Func_080d6888(
                    scene->object_id,
                    7,
                    5,
                    0,
                    4);
            }
            *(s32 *)(runtime + 0x77a8) = 8;
        }

        if (mode == 1) {
            if (frame == 13) {
                object->displacement = 0xc0000;
                object->state = 0x7851;
                object->scale = 0x4000;
            }
            if (frame == 65) {
                *(s32 *)(runtime + 0x77a8) = 4;
                Func_080b50e8(0x86);
            }
        }

        Func_080e155c(8, 8);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
