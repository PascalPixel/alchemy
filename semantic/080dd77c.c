#include "types.h"

typedef void (*Renderer_080dd77c)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

s32 Func_080022fc(s32 value, s32 modulus);
void Func_08002dd8(s32 arg0);
void Func_080030f8(s32 arg0);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(volatile void *address);
void Func_080cd52c(void);
void Func_080cd594(s32 arg0);
void Func_080cdbc0(void);
void Func_080d6888(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080de2f8(
    void *object,
    s32 arg1,
    u32 arg2,
    s32 arg3,
    void *output0,
    void *output1);
void Func_080e0524(volatile void *address, void *runtime, s32 arg2, s32 arg3);
void Func_080e155c(s32 arg0, s32 arg1);
void Func_080e396c(s32 value, s32 *output);
void Func_080ed408(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080f9010(s32 arg0);

void Func_080dd77c(void *object)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    Renderer_080dd77c renderers[2];
    s32 output0;
    s32 output1;
    s32 point0[3];
    s32 point1[3];
    s32 center;
    u32 frame;
    u32 i;

    *(void **)(runtime + 0x7828) = object;
    Func_080de2f8(
        object,
        4,
        *(u32 *)((u8 *)object + 4),
        4,
        &output0,
        &output1);
    Func_080cd594(1);

    *(volatile u16 *)0x04000020 = 0x100;
    *(volatile u16 *)0x04000050 = 0;
    Func_080e0524((volatile void *)0x04000020, runtime, 1, 1);

    Func_080e396c(*(s16 *)((u8 *)object + 36), point0);
    Func_080e396c(
        *(s16 *)((u8 *)object + 34 + (*(s32 *)((u8 *)object + 20) * 2)),
        point1);

    center = point0[0] + ((point1[0] - point0[0]) / 2);
    point0[0] = center;
    *(volatile u32 *)0x04000028 = (u32)((64 - center) * 0x100);

    Func_080ed408(46, 7, 7, 3, 1);
    renderers[0] = (Renderer_080dd77c)runtime_header[7];
    Func_080ed408(47, 7, 7, 7, 1);
    renderers[1] = (Renderer_080dd77c)runtime_header[8];

    for (i = 0; i < 16; i++) {
        *(s32 *)(runtime + 0x7080 + (i * 28)) =
            (s8)((const u8 *)0x080eeb96)[i] + 64;
    }
    *(s32 *)(runtime + 0x7780) = 1;
    *(s32 *)(runtime + 0x7784) = 0;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (frame = 0; frame < 70; frame++) {
        if (frame == 32) {
            s32 count;

            Func_080f9010(143);
            count = *(s32 *)(*(u8 **)(runtime + 0x7828) + 20);
            for (i = 0; i < count; i++) {
                void *saved_object = *(void **)(runtime + 0x7828);
                Func_080d6888(
                    *(s16 *)((u8 *)saved_object + 36 + (i * 2)),
                    7,
                    5,
                    i,
                    16);
            }
        }

        for (i = 0; i < 16; i++) {
            s32 width;

            if (frame == (i * 4) + 5)
                *(s32 *)(runtime + 0x77a8) = 2;

            if (frame <= (i * 2) + 4)
                continue;

            {
                s32 tile = frame / 4 + i;
                s32 source_offset;

                source_offset = Func_080022fc(tile, 5);

                if (frame < (i * 2) + 32) {
                    width = ((frame - (i * 2)) * 4) - 16;
                    if (width > 32)
                        width = 32;
                } else {
                    width = 160 - ((frame - (i * 2)) * 4);
                }

                if (width > 0) {
                    renderers[i & 1](
                        render_context,
                        runtime + (source_offset * 0x400),
                        *(s32 *)(runtime + 0x7080 + (i * 28)) - 16,
                        104 + (i & 7) - width,
                        32,
                        width);
                }
            }
        }

        Func_080e155c(4, 4);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((volatile void *)0x04000028);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
