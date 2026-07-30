#include "types.h"

struct DmaChannel_0800679c {
    const void *source;
    void *destination;
    u32 control;
};

void Func_080f9010(s32 sound);
void Func_08005d10(void);
void Func_08006864(u32 *state, void *tiles, void *palette);
void Func_08006384(s32 mode);
void Func_08006408(void *buffer);
void Func_080063bc(s32 source, s32 size);
void Func_08006798(void);
void Func_080030f8(u32 frames);

void Func_0800679c(void)
{
    volatile struct DmaChannel_0800679c *dma =
        (volatile struct DmaChannel_0800679c *)0x040000d4;
    volatile u16 *tilemap = (volatile u16 *)0x06002426;
    u32 state = 0;
    s32 index;

    Func_080f9010(3);
    Func_08005d10();
    for (index = 0; index < 20; index++)
        tilemap[-index] = 0xf093 - index;

    Func_08006864(&state, (void *)0x02010000, (void *)0x05000100);
    Func_08006384(3);

    for (;;) {
        Func_08006408((void *)0x02010000);
        for (;;) {
            s32 keys = *(s32 *)0x03001ae8;

            if (keys & 1)
                Func_080063bc(0x08000000, 0x280);
            if (keys & 2)
                Func_080063bc(0x08001000, 0x280);
            if (keys & 8) {
                for (index = 0; index < 10000; index++)
                    Func_08006798();
            }

            if (*(s32 *)0x020023ac == 0) {
                dma->source = (const void *)0x02010000;
                dma->destination = (void *)0x06001000;
                dma->control = 0x840000a0;
                break;
            }
            Func_080030f8(1);
        }
    }
}
