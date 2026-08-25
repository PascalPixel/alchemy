#include "types.h"

struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
};

extern u8 *Data_03001ed0;
void Func_0808a338(s32, s32);

#define DMA3 (*(volatile struct DmaTransfer *)0x040000d4)

#define ConfigureBattleEffectPaletteTransfer Func_02000bcc

void ConfigureBattleEffectPaletteTransfer(void)
{
    u8 *buffer = Data_03001ed0;

    DMA3.source = (const void *)0x05000000;
    DMA3.destination = buffer;
    DMA3.control = 0x84000070;

    DMA3.source = (const void *)0x05000200;
    DMA3.destination = buffer + 0x1c0;
    DMA3.control = 0x84000070;

    Func_0808a338(0x10000, 0);
}
