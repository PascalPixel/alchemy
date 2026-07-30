#include "types.h"

typedef void (*Renderer_080ccebc)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

void Func_08002dd8(s32 resource);
void Func_080030f8(s32 frames);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(const void *source);
void Func_080b50e8(s32 sound);
void Func_080cd52c(void);
void Func_080cd594(s32 mode);
void Func_080cdbc0(void);
void Func_080d6888(s32 id, s32 arg1, s32 arg2, s32 index, s32 arg4);
void Func_080e0524(s32 resource, void *destination, s32 arg2, s32 arg3);
void Func_080e396c(s32 value, s32 *point);
void Func_080ed408(s32 resource, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080f9010(s32 sound);

void Func_080ccebc(void *argument)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    Renderer_080ccebc renderer;
    s32 first_point[3];
    s32 second_point[3];
    s32 frame;

    *(void **)(runtime + 0x7828) = argument;
    Func_080cd594(2);
    *(volatile u16 *)0x04000052 = 0x1000;
    *(volatile u16 *)0x04000020 = 0x0100;

    Func_080e396c(*(s16 *)((u8 *)argument + 36), first_point);
    Func_080e396c(
        *(s16 *)((u8 *)argument + 34 + (*(s32 *)((u8 *)argument + 20) * 2)),
        second_point);

    first_point[0] += (second_point[0] - first_point[0]) / 2;
    *(s32 *)0x04000028 =
        (s32)((u32)(64 - first_point[0]) << 8);

    Func_080e0524(0x59, runtime, 1, 1);
    *(s32 *)(runtime + 0x7780) = 1;
    *(s32 *)(runtime + 0x7784) = 0;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080f9010(143);

    for (frame = 0; frame < 63; frame++) {
        s32 object_index;

        if (frame <= 8)
            *(volatile u16 *)0x04000052 = 0x1000 | (frame * 2);
        if (frame > 53)
            *(volatile u16 *)0x04000052 =
                0x1000 | (0x7c - (frame * 2));

        Func_080ed408(46, 7, 7, 3, 1);
        renderer = (Renderer_080ccebc)runtime_header[7];
        renderer(render_context, runtime, 33, 41, 32, 32);
        Func_08002dd8(46);

        Func_080ed408(46, 7, 7, 7, 1);
        renderer = (Renderer_080ccebc)runtime_header[7];
        renderer(render_context, runtime, 64, 41, 32, 32);
        Func_08002dd8(46);

        Func_080ed408(46, 7, 7, 11, 1);
        renderer = (Renderer_080ccebc)runtime_header[7];
        renderer(render_context, runtime, 33, 72, 32, 32);
        Func_08002dd8(46);

        Func_080ed408(46, 7, 7, 15, 1);
        renderer = (Renderer_080ccebc)runtime_header[7];
        renderer(render_context, runtime, 64, 72, 32, 32);
        Func_08002dd8(46);

        if (frame == 32)
            Func_080b50e8(143);

        if (frame == 10) {
            s32 count = *(s32 *)((u8 *)argument + 20);

            for (object_index = 0; object_index < count; object_index++) {
                s32 id = *(s16 *)(
                    (u8 *)argument + 36 + (object_index * 2));
                Func_080d6888(id, 7, -1, object_index, 8);
            }
        }

        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_080cdbc0();
}
