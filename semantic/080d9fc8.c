#include "types.h"

typedef void (*Renderer_080d9fc8)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Sprite_080d9fc8 {
    s32 x;
    s32 y;
    s32 unused08;
    s32 unused0c;
    s32 unused10;
    s32 unused14;
    s32 phase;
};

s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_08002dd8(s32 arg0);
void Func_080030f8(s32 arg0);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(volatile void *address);
void Func_080b50e8(s32 arg0);
void Func_080cd52c(void);
void Func_080cd594(s32 arg0);
void Func_080cdbc0(void);
void Func_080d6888(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080e0524(s32 arg0, void *runtime, s32 arg2, s32 arg3);
void Func_080ed408(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080f9010(s32 arg0);

void Func_080d9fc8(void *argument)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    Renderer_080d9fc8 renderer;
    struct Sprite_080d9fc8 *sprites =
        (struct Sprite_080d9fc8 *)(runtime + 0x7080);
    s32 center;
    s32 direction;
    s32 frame;
    s32 i;

    *(void **)(runtime + 0x7828) = argument;
    Func_080cd594(0);
    *(volatile u16 *)0x04000050 = 0x3f46;
    *(volatile u16 *)0x04000052 = 0x100e;
    Func_080e0524(0xb4, runtime, 1, 1);
    Func_080ed408(46, 7, 7, 3, 3);
    renderer = (Renderer_080d9fc8)runtime_header[7];

    if (*(s16 *)((u8 *)argument + 36) > 127) {
        center = 0;
        direction = 1;
    } else {
        center = 64;
        direction = -1;
    }

    for (i = 0; i < 9; i++) {
        s32 angle = -0x4000 + (i * 0x1000);
        s32 sine = (s32)((u32)Func_08002322(angle) << 5) >> 16;
        s32 cosine = (s32)((u32)Func_0800231c(angle) << 4) >> 16;

        sprites[i].x = center + (direction * sine) + 20;
        sprites[i].y = cosine + 40;
        sprites[i].phase = -(i * 4);
    }

    *(s32 *)(runtime + 0x7780) = 2;
    if (*(s32 *)((u8 *)argument + 24) == 2)
        *(s32 *)(runtime + 0x7784) = 75;
    else
        *(s32 *)(runtime + 0x7784) = 50;

    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080f9010(136);

    for (frame = 0; frame < 80; frame++) {
        if (frame == 24)
            Func_080b50e8(133);

        for (i = 0; i < 9; i++) {
            struct Sprite_080d9fc8 *sprite = &sprites[i];

            if ((u32)sprite->phase <= 23) {
                s32 index = sprite->phase / 4;
                s32 width = ((const u8 *)0x080ede9f)[index];
                s32 height = ((const u8 *)0x080edea5)[index];
                s32 x = sprite->x - (width / 2);
                s32 y = sprite->y + ((const u8 *)0x080edeab)[index];
                const void *source =
                    runtime + ((const u16 *)0x080edeb2)[index];
                renderer(render_context, source, x, y, width, height);
                if (*(s32 *)(*(u8 **)(runtime + 0x7828) + 24) != 0)
                    renderer(render_context, source, x, y - 16, width, height);
                if (*(s32 *)(*(u8 **)(runtime + 0x7828) + 24) == 2)
                    renderer(render_context, source, x, y - 32, width, height);
            }

            sprite->phase++;
        }

        {
            s32 count = *(s32 *)(*(u8 **)(runtime + 0x7828) + 20);

            for (i = 0; i < count; i++) {
                if (frame == 16 + (i * 8)) {
                    Func_080d6888(
                        *(s16 *)(*(u8 **)(runtime + 0x7828) + 36 + (i * 2)),
                        10,
                        5,
                        i,
                        12);
                }
            }
        }

        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08002dd8(46);
    Func_08004278((volatile void *)0x080cd261);
    Func_080cdbc0();
}
