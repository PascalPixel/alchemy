#include "types.h"

typedef void (*Renderer_080ecef4)(
    void *,
    const void *,
    s32,
    s32,
    s32,
    s32);
typedef void (*Fill_080ecef4)(void *, s32, s32);

void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(volatile void *);
void Func_080b5078(s32, s16, s32, s32);
void Func_080b5088(s16, s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080ecef4(void *object, s32 mode)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    Renderer_080ecef4 render =
        *(Renderer_080ecef4 *)(0x03001e50 + 0xb8);
    Fill_080ecef4 fill = (Fill_080ecef4)0x03000168;
    s32 alternate = mode == 1;
    s32 second_resource;
    s32 frame;

    *(void **)(runtime + 0x7828) = object;
    Func_080cd594(0);
    *(volatile u16 *)0x04000050 = 0;

    if (mode == 0) {
        Func_080e0524(0x4f, runtime, 1, 0);
        second_resource = 0x50;
    } else if (mode == 1) {
        Func_080e0524(0x4d, runtime, 1, 0);
        second_resource = 0x4e;
    } else {
        Func_080e0524(0x4b, runtime, 1, 0);
        Func_080e0524(0x4c, (void *)0x02010000, 1, 1);
        second_resource = -1;
    }
    if (second_resource >= 0)
        Func_080e0524(
            second_resource,
            (void *)0x02010000,
            1,
            1);

    *(s32 *)(runtime + 0x7780) = 1;
    *(s32 *)(runtime + 0x7784) = 0;
    Func_080041d8((const void *)0x080cd261, 0x480);

    if (alternate) {
        Func_080b5078(
            *(s32 *)((u8 *)object + 8),
            *(s16 *)((u8 *)object + 36),
            16,
            0x80000);
    } else {
        Func_080b5078(
            *(s32 *)((u8 *)object + 8),
            *(s16 *)((u8 *)object + 36),
            16,
            0);
    }
    Func_080030f8(16);

    if (*(s32 *)((u8 *)object + 4) == 1)
        Func_080ed408(46, 7, 7, 7, 0);
    else
        Func_080ed408(46, 7, 7, 3, 0);

    Func_080f9010(212);

    for (frame = 0; frame < 21; frame++) {
        if (frame <= 11) {
            const void *source;

            if (frame <= 3)
                source = runtime;
            else if (frame <= 7)
                source = runtime + 0x3840;
            else
                source = (const void *)0x02010000;
            render(render_context, source, 0, 0, 120, 120);
        } else if (frame <= 15) {
            render(
                render_context,
                (const void *)0x02013840,
                0,
                0,
                120,
                120);
        }

        if ((u32)(frame - 16) <= 3)
            fill(render_context, 0x4000, 0x3f3f3f3f);
        if (frame == 18)
            Func_080b50e8(134);
        if (frame == 20) {
            *(s32 *)(runtime + 0x77a8) = 8;
            Func_080b5088(
                *(s16 *)(*(u8 **)(runtime + 0x7828) + 36),
                4);
        }

        Func_080e155c(16, 16);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08002dd8(46);
    Func_08004278((volatile void *)0x080cd261);
    Func_080cdbc0();
}
