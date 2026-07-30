#include "types.h"

struct DmaChannel_080030f8 {
    const void *source;
    void *destination;
    u32 control;
};

typedef void (*ResetEntry_080030f8)(void);

#define REG8_080030F8(address)  (*(volatile u8 *)(address))
#define REG16_080030F8(address) (*(volatile u16 *)(address))
#define REG32_080030F8(address) (*(volatile u32 *)(address))

void Func_08004420(s32 request);
void *Func_080048b0(s32 resource, u32 size);
void Func_08003e10(s32 request);
void Func_08003538(void);
void Func_08002dd8(s32 resource);
void Func_08003d04(void);
s32 Func_08005fcc(void);
void Func_08006868(void);
void Func_08006870(void);

static void WaitForInterrupt_080030f8(void)
{
    volatile u16 *interrupt_flags = (volatile u16 *)0x03001d28;

    *interrupt_flags &= 0xfffe;
    while ((*interrupt_flags & 1) == 0) {
    }
}

static void ResetGame_080030f8(void)
{
    ResetEntry_080030f8 reset = (ResetEntry_080030f8)0x08000000;

    reset();
}

static void StopUntilResetChord_080030f8(void)
{
    while ((REG16_080030F8(0x04000130) & 0x0300) != 0) {
    }
}

void Func_080030f8(u32 frames)
{
    struct DmaChannel_080030f8 *dma3 =
        (struct DmaChannel_080030f8 *)0x040000d4;
    u32 frame;
    u32 relocated_bytes = 0;
    void *original_stack = &frame;

    /*
     * Keep the active frame out of the interrupt stack's path.  The original
     * changes sp to 0x03007a00 after this copy and reverses the copy below.
     * The semantic build records the same transfer and lifetime explicitly.
     */
    if ((u32)original_stack <= 0x030079ff) {
        relocated_bytes = 0x03007a00 - (u32)original_stack;
        REG32_080030F8(0x03001804) = relocated_bytes;
        dma3->source = original_stack;
        dma3->destination = (void *)0x020023b0;
        dma3->control = 0x84000000 | (relocated_bytes >> 2);
    }

    for (frame = 0; frame < frames; frame++) {
        u32 pending;

        REG8_080030F8(0x03001a10) = 1;
        Func_08004420(0xc80);
        REG8_080030F8(0x03001a10) = 0;
        Func_08003e10((s32)Func_080048b0(52, 0x400));
        REG8_080030F8(0x03001e44) = 1;

        if (REG8_080030F8(0x03001f58) != 0) {
            u32 scanline = REG16_080030F8(0x04000006);
            u32 deadline;

            if (scanline > 159)
                scanline -= 160;
            else
                scanline += 68;
            deadline = scanline +
                ((REG16_080030F8(0x03001ccc) - 1) << 8);

            pending = REG32_080030F8(0x03001af0);
            if (pending == 0)
                REG32_080030F8(0x03001ca4) = 0;
            else
                REG32_080030F8(0x03001af0) = pending - 1;

            if (REG32_080030F8(0x03001ca4) < deadline) {
                REG32_080030F8(0x03001ca4) = deadline;
                REG32_080030F8(0x03001af0) = 30;
            }
        }

        if (REG8_080030F8(0x03001ca0) == 0) {
            if (REG8_080030F8(0x03001d08) != 0) {
                if (REG32_080030F8(0x03001ae8) != 0) {
                    REG16_080030F8(0x03001d24) = 0;
                } else {
                    REG16_080030F8(0x03001d24)++;
                    if (REG16_080030F8(0x03001d24) > 0x2a30)
                        REG8_080030F8(0x03001cc8) = 1;
                }
            }

            if (REG32_080030F8(0x03001ae8) == 0x300) {
                REG16_080030F8(0x03001f5c)++;
                if (REG16_080030F8(0x03001f5c) > 179) {
                    REG16_080030F8(0x03001f5c) = 0;
                    REG8_080030F8(0x03001cc8) = 1;
                }
            } else {
                REG16_080030F8(0x03001f5c) = 0;
            }
        }

        if (REG8_080030F8(0x03001f54) != 0) {
            for (;;) {
                u8 ready = REG8_080030F8(0x03001d20);

                if (ready == 0) {
                    if (REG32_080030F8(0x03001ae8) != 12)
                        break;
                    REG8_080030F8(0x03001d20) = 1;
                    ready = 1;
                }

                if ((REG32_080030F8(0x03001b04) & 7) != 0 ||
                    (REG32_080030F8(0x03001ae8) & 0xf0) != 0) {
                    break;
                }
                if ((REG32_080030F8(0x03001b04) & 8) != 0) {
                    REG8_080030F8(0x03001d20) = 0;
                    break;
                }

                WaitForInterrupt_080030f8();
                Func_08003538();
                if (REG8_080030F8(0x03001cb8) != 0) {
                    REG8_080030F8(0x03001cb8) = 0;
                    REG32_080030F8(0x03007800) = 0x19670704;
                    REG16_080030F8(0x04000208) = 0;
                    ResetGame_080030f8();
                }
            }
        }

        REG16_080030F8(0x03001cd0) =
            REG16_080030F8(0x03001ccc);
        REG16_080030F8(0x03001ccc) = 0;
        WaitForInterrupt_080030f8();
        Func_08002dd8(52);
        Func_08003d04();
        REG32_080030F8(0x03001e40)++;
        REG32_080030F8(0x03001c9c)++;
        Func_08003538();

        if (REG16_080030F8(0x03001cb0) != 0) {
            Func_08005fcc();
            if (REG8_080030F8(0x02002240) != 0)
                REG8_080030F8(0x02002248) = 1;
        }

        if (REG8_080030F8(0x03001cc8) != 0 &&
            REG8_080030F8(0x03001ca0) == 0) {
            s16 display_control = (s16)REG16_080030F8(0x04000000);
            s16 backdrop_color = (s16)REG16_080030F8(0x05000000);

            if (REG8_080030F8(0x03001cc8) == 1) {
                s32 delay;

                REG16_080030F8(0x04000000) = 0;
                REG16_080030F8(0x05000000) = 0x7fff;
                for (delay = 0; delay <= 59; delay++)
                    WaitForInterrupt_080030f8();

                REG16_080030F8(0x02002000) = 1;
                REG16_080030F8(0x04000132) = 0xc300;
                Func_08006868();
                StopUntilResetChord_080030f8();
                Func_08006870();
                REG16_080030F8(0x04000132) = 0xc00f;
                REG16_080030F8(0x02002000) = 0;
                REG16_080030F8(0x04000000) = (u16)display_control;
                REG16_080030F8(0x05000000) = (u16)backdrop_color;

                for (delay = 0; delay <= 9; delay++)
                    WaitForInterrupt_080030f8();
                REG8_080030F8(0x03001cc8) = 0;
                REG16_080030F8(0x03001d24) = 0;
            } else {
                REG8_080030F8(0x03001cc8)--;
            }
        }

        if (REG8_080030F8(0x03001cb8) != 0) {
            REG8_080030F8(0x03001cb8) = 0;
            REG32_080030F8(0x03007800) = 0x19670704;
            REG16_080030F8(0x04000208) = 0;
            ResetGame_080030f8();
        }
    }

    if (relocated_bytes != 0) {
        dma3->source = (const void *)0x020023b0;
        dma3->destination = original_stack;
        dma3->control = 0x84000000 | (relocated_bytes >> 2);
        while ((dma3->control & 0x80000000) != 0) {
        }
        REG32_080030F8(0x03001804) = 0;
    }
}
