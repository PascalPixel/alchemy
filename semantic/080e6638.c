#include "types.h"

struct DmaChannel_080e6638 {
    const void *source;
    void *destination;
    u32 control;
};

struct DeferredTransfer_080e6638 {
    u16 count;
    u16 unknown_02;
    struct {
        const void *source;
        void *destination;
        u32 control;
    } entry[32];
};

typedef s32 (*SquareRoot_080e6638)(s32);
typedef void (*Renderer_080e6638)(
    void *,
    void *,
    s32,
    s32,
    s32,
    s32);

void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(volatile void *);
s32 Func_08002322(s32);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080cef64(s32, u32 *);
void Func_080de2f8(void *, s32, s32, s32, s32 *, s32 *);

static s32 Clamp_080e6638(s32 value, s32 low, s32 high)
{
    if (value < low)
        value = low;
    if (value > high)
        value = high;
    return value;
}

void Func_080e6638(void *object)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    SquareRoot_080e6638 square_root =
        (SquareRoot_080e6638)0x030001d8;
    Renderer_080e6638 renderers[2];
    s32 output0;
    s32 output1;
    s32 row;
    s32 column;
    s32 frame;

    *(void **)(runtime + 0x7828) = object;
    Func_080cd594(0x2000);
    Func_080de2f8(
        object,
        6,
        *(s32 *)((u8 *)object + 4),
        2,
        &output0,
        &output1);

    *(volatile u16 *)0x0400000c = 0x2784;
    *(volatile u16 *)0x04000052 = 0x1000;
    *(volatile u16 *)0x04000020 = 0x00aa;

    Func_080cef64(
        *(s32 *)(*(u8 **)(runtime + 0x7828) + 4),
        (u32 *)renderers);
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (row = 0; row < 64; row++) {
        s32 horizontal = row - (row / 8 + 64);

        for (column = 0; column < 64; column++) {
            s32 vertical = column - 64;
            s32 distance =
                square_root(
                    horizontal * horizontal + vertical * vertical) /
                2;

            distance = Clamp_080e6638(distance, 1, 63);
            runtime[row * 128 + column] = distance;
            runtime[row * 128 + 127 - column] = distance;
            runtime[(127 - row) * 128 + column] = distance;
            runtime[(127 - row) * 128 + 127 - column] = distance;
        }
    }

    for (column = 1; column < 64; column++) {
        s32 shade = column > 31 ? 64 - column : column;
        s32 red = Clamp_080e6638(shade * 9, 0, 255);
        s32 green = Clamp_080e6638(shade * 7 - 42, 0, 255);
        s32 blue = Clamp_080e6638(shade * 7 - 56, 0, 250);
        u16 color =
            ((blue >> 3) << 10) | ((green >> 3) << 5) | (red >> 3);

        *(volatile u16 *)(0x05000000 + column * 2) = color;
        *(u16 *)(0x02010000 + column * 2) = color;
    }

    renderers[0](render_context, runtime, 0, 0, 128, 128);
    *(s32 *)(runtime + 0x7824) = 1;
    Func_080041d8((const void *)0x080dbb9d, 0x480);

    for (frame = 0; frame < 96; frame++) {
        s32 scroll = frame * 2;
        s32 angle = -(scroll << 9);
        s32 *affine = (s32 *)(runtime + 0x6980);

        if (frame <= 8)
            *(volatile u16 *)0x04000052 = scroll | 0x1000;
        if (frame > 88)
            *(volatile u16 *)0x04000052 =
                (0xc0 - scroll) | 0x1000;

        for (column = 0; column < 160; column++) {
            affine[column] =
                ((column << 18) -
                     (Func_08002322(angle + column * 0x200) << 7) +
                     0x40000) >>
                10;
        }

        if (frame > 127) {
            *(s32 *)(runtime + 0x7824) = 1;
        } else {
            volatile struct DmaChannel_080e6638 *dma =
                (volatile struct DmaChannel_080e6638 *)0x040000d4;
            volatile struct DeferredTransfer_080e6638 *transfers =
                (volatile struct DeferredTransfer_080e6638 *)0x02002090;
            u16 interrupt_state;

            *(u16 *)0x02010002 = *(u16 *)0x0201007e;
            dma->source = (const void *)0x0201007c;
            dma->destination = (void *)0x0201007e;
            dma->control = 0x80a0003e;

            interrupt_state = *(volatile u16 *)0x04000208;
            *(volatile u16 *)0x04000208 = 0x0208;
            if (transfers->count <= 31) {
                u16 transfer = transfers->count++;

                transfers->entry[transfer].source =
                    (const void *)0x02010002;
                transfers->entry[transfer].destination =
                    (void *)0x05000002;
                transfers->entry[transfer].control = 0x8000003f;
            }
            *(volatile u16 *)0x04000208 = interrupt_state;
        }

        Func_080030f8(1);
    }

    Func_08004278((volatile void *)0x080dbb9d);
    Func_08004278((volatile void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
