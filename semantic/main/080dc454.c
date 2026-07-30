#include "types.h"

typedef void (*Transfer_080dc454)(
    void *destination,
    const void *source,
    u32 size);

typedef void (*Renderer_080dc454)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_08002dd8(s32 arg0);
void *Func_08002f40(s32 arg0);
void Func_080030f8(s32 arg0);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(volatile void *address);
void Func_080049ac(void);
void Func_080051d8(void *arg0, void *arg1);
void Func_08005340(void *source, void *destination);
void *Func_080b5098(s32 arg0);
void Func_080cd594(s32 arg0);
void Func_080cdbc0(void);
void Func_080e3944(const s32 *input, s32 *output);
void Func_080e396c(s32 value, s32 *output);
void Func_080ed408(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void Func_080dc454(void)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    void *effect_context = *(void **)0x03001e80;
    Transfer_080dc454 transfer = (Transfer_080dc454)0x03001388;
    Renderer_080dc454 renderer = (Renderer_080dc454)runtime_header[7];
    void *buffer;
    s32 horizontal_offset;
    s32 frame;
    s32 count;
    s32 total_frames;
    s32 i;

    Func_080cd594(1);
    *(volatile u16 *)0x04000020 = 0x100;
    *(volatile u16 *)0x04000052 = 0x1010;

    buffer = Func_08002f40(0xa9);
    transfer((void *)0x05000000, buffer, 0x80);
    Func_08005340((u8 *)buffer + 0x80, runtime);
    Func_080ed408(46, 7, 7, 3, 2);

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    if (*(s32 *)(*(u8 **)(runtime + 0x7828) + 4) != 0)
        horizontal_offset = -112;
    else
        horizontal_offset = 0;

    *(volatile u32 *)0x04000028 = (u32)(horizontal_offset * 0x100);

    for (i = 0; i < 64; i++) {
        s32 *entry = (s32 *)(runtime + 0x7080 + (i * 28));

        entry[0] = 0;
        entry[1] = 0;
        entry[2] = 4;
        entry[3] = 0;
    }

    count = *(s32 *)(*(u8 **)(runtime + 0x7828) + 20);
    total_frames = (count * 16) + 64;
    if (total_frames != 0) {
        frame = 0;
        do {
            for (i = 0; i < count; i++) {
                s32 phase = frame - (i * 16);
                void *object;
                s32 point0[3];
                s32 source_point[3];
                s32 point1[3];
                s32 j;

                if ((u32)phase > 63)
                    continue;

                object = *(void **)Func_080b5098(
                    *(s16 *)(*(u8 **)(runtime + 0x7828) + 36 + (i * 2)));
                Func_080e396c(
                    *(s16 *)(*(u8 **)(runtime + 0x7828) + 36 + (i * 2)),
                    point0);
                point0[0] += horizontal_offset;

                Func_080049ac();
                Func_080051d8(effect_context, (u8 *)effect_context + 12);

                source_point[0] = *(s32 *)((u8 *)object + 8);
                source_point[1] = *(s32 *)((u8 *)object + 12);
                source_point[2] = *(s32 *)((u8 *)object + 16);
                Func_080e3944(source_point, point1);
                point1[0] += horizontal_offset;

                for (j = 0; j < 3; j++) {
                    s32 *entry =
                        (s32 *)(runtime + 0x7080 + (((i * 3) + j) * 28));
                    s32 angle = entry[3] + (j * 0x5555);
                    s32 x = point0[0] +
                        ((s32)((u32)Func_08002322(angle) << 3) >> 16);
                    s32 y = point0[1] +
                        ((s32)((u32)Func_0800231c(angle) << 3) >> 16);

                    entry[3] += 0x200;
                    renderer(
                        render_context,
                        runtime + (j * 0x240),
                        x - 12,
                        y - 28,
                        24,
                        24);
                }
            }

            *(s32 *)(runtime + 0x7824) = 1;
            Func_080030f8(1);
            frame++;
        } while (frame != total_frames);
    }

    Func_08004278((volatile void *)0x04000028);
    Func_08002dd8(46);
    Func_080cdbc0();
}
