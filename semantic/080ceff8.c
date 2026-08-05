#include "types.h"

typedef void (*Transfer_080ceff8)(
    void *destination,
    const void *source,
    u32 size);

typedef void (*Renderer_080ceff8)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Scene_080ceff8 {
    u8 unknown_00[36];
    s16 object_ids[1];
};

s32 Func_080022ec(s32, s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(u32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
void Func_08005340(const void *, void *);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e155c(s32, u32);
void Func_080e3980(s16, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080ceff8(struct Scene_080ceff8 *scene, s32 mode)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    Transfer_080ceff8 transfer = (Transfer_080ceff8)0x03001388;
    Renderer_080ceff8 renderers[2];
    u8 *resource;
    s32 position[2];
    u32 frame;

    *(struct Scene_080ceff8 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);

    resource = Func_08002f40(0x7b);
    transfer((void *)0x05000000, resource, 0x80);
    Func_08005340(resource + 0x80, runtime);

    resource = Func_08002f40(0x8d);
    transfer((void *)0x05000000, resource, 0x80);
    if (mode == 2) {
        resource = Func_08002f40(0x68);
        transfer((void *)0x05000000, resource, 0x80);
    }

    Func_080e3980(scene->object_ids[0], position);
    if (mode == 0) {
        *(volatile u16 *)0x04000020 = 0x100;
        *(volatile s32 *)0x04000028 = (64 - position[0]) << 8;
    } else {
        *(volatile u16 *)0x04000020 = 0xcc;
        *(volatile s32 *)0x04000028 =
            (Func_080022ec(-position[0] * 4, 5) + 64) << 8;
    }

    Func_080ed408(46, 7, 7, 3, 2);
    renderers[0] = *(Renderer_080ceff8 *)0x03001f08;
    Func_080ed408(47, 7, 7, 7, 2);
    renderers[1] = *(Renderer_080ceff8 *)0x03001f0c;
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 50;
    Func_080041d8((const void *)0x080cd261, 0x480);

    if (mode == 2 || mode == 1) {
        *(s32 *)(runtime + 0x77a8) = mode == 2 ? 0 : 8;
        Func_080f9010(0xd4);
    } else {
        *(s32 *)(runtime + 0x77a8) = 32;
    }

    for (frame = 0; frame < 48; frame++) {
        if (frame == 0) {
            if (mode == 2)
                Func_080d6888(
                    scene->object_ids[0], 7, -1, 0, 32);
            else
                Func_080d6888(
                    scene->object_ids[0], 10, -1, 0, 32);
        }
        if (frame == 24)
            Func_080b50e8(0);
        if (frame == 8 && mode == 0)
            Func_080f9010(0x7e);

        if (frame <= 27) {
            s32 animation = frame / 4;
            s32 width;
            s32 height;
            const void *source;

            if (animation > 2)
                animation = (animation & 1) + 1;
            width = *(u8 *)(0x080ee09c + animation);
            height = *(u8 *)(0x080ee09f + animation);
            source = runtime +
                *(u16 *)(0x080ee096 + animation * 2);
            renderers[0](
                render_context,
                source,
                64 - width,
                position[1] - height + 8,
                width,
                height);
            renderers[1](
                render_context,
                source,
                64,
                position[1] - height + 8,
                width,
                height);
        }

        if (mode == 0)
            Func_080e155c(2, 2);
        else
            Func_080e155c(16, 16);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
