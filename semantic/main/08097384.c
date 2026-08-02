#include "types.h"

struct DmaTransfer_08097384 {
    const void *source;
    void *destination;
    u32 control;
};

struct RuntimeState_08097384 {
    u8 *scene;
    u8 padding04[0x10];
    u8 *tiles;
};

extern s32 Data_03001e40;
extern const s32 Data_080a0108[8];

s32 Func_080770c0(s32);
void Func_08091200(s32, s32);
void Func_08091254(s32);

/*
 * Install the scene's tile data and choose its rendering preset.  The eight
 * event flags are checked in order, so the highest enabled flag wins; when
 * none is enabled the low three bits of the global scene index are used.
 */
void Func_08097384(void)
{
    struct RuntimeState_08097384 *runtime =
        (struct RuntimeState_08097384 *)0x03001ebc;
    volatile struct DmaTransfer_08097384 *dma =
        (volatile struct DmaTransfer_08097384 *)0x040000d4;
    u8 *base = runtime->scene;
    u8 *tiles = runtime->tiles;
    s32 preset = Data_03001e40 & 7;
    s32 flag;

    dma->source = tiles + 0x1340;
    dma->destination = base + 0x0776;
    dma->control = 0x84000150;

    if (*(s16 *)(base + 0x0cb8) == 0) {
        dma->source = tiles + 0x0e00;
        dma->destination = base + 0x0236;
        dma->control = 0x84000150;
    }

    dma->source = tiles + 0x0e00;
    dma->destination = tiles + 0x0380;
    dma->control = 0x840002a0;

    for (flag = 0; flag < 8; flag++) {
        if (Func_080770c0(0x0148 + flag) != 0)
            preset = flag;
    }

    Func_08091200(Data_080a0108[preset], 1);
    Func_08091254(8);
}
