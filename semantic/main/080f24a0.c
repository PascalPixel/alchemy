#include "layout_guard.h"
#include "types.h"

typedef struct DmaTransfer_080f24a0 {
    const void *source;
    void *destination;
    u32 control;
} DmaTransfer_080f24a0;

typedef struct DisplayRuntime_080f24a0 {
    u32 values[6];
} DisplayRuntime_080f24a0;

typedef struct BackgroundScroll_080f24a0 {
    s16 offsets[8];
} BackgroundScroll_080f24a0;

LAYOUT_SIZE_GUARD(
    DisplayRuntime080f24a0_Size,
    DisplayRuntime_080f24a0,
    0x18);
LAYOUT_SIZE_GUARD(
    BackgroundScroll080f24a0_Size,
    BackgroundScroll_080f24a0,
    0x10);

extern DisplayRuntime_080f24a0 *Data_03001efc;

u8 *Func_08002f40(s32);
void Func_080053e8(const void *, void *);

static void StartDma_080f24a0(
    const void *source,
    void *destination,
    u32 control)
{
    volatile DmaTransfer_080f24a0 *dma =
        (volatile DmaTransfer_080f24a0 *)0x040000d4;

    dma->source = source;
    dma->destination = destination;
    dma->control = control;
}

static u16 *FillTilemapRows_080f24a0(
    u16 *destination,
    s32 row_count,
    u16 first_tile)
{
    s32 row;

    for (row = 0; row < row_count; row++) {
        s32 column;

        for (column = 0; column < 30; column++)
            *destination++ = first_tile++;
        *destination++ = 0x01ff;
        *destination++ = 0x01ff;
    }
    return destination;
}

/* Load the two backgrounds, build both 32x32 maps, and prime display state. */
void Func_080f24a0(void)
{
    volatile u16 *display_control = (volatile u16 *)0x04000000;
    volatile u16 *bg2_control = (volatile u16 *)0x0400000a;
    volatile u16 *window = (volatile u16 *)0x04000040;
    volatile u16 *blend_control = (volatile u16 *)0x04000050;
    volatile u16 *blend_alpha = (volatile u16 *)0x04000052;
    BackgroundScroll_080f24a0 *scroll =
        (BackgroundScroll_080f24a0 *)0x03001ad0;
    DisplayRuntime_080f24a0 *runtime = Data_03001efc;
    u8 *asset;
    u16 *tilemap;
    s32 index;

    *display_control = 0;

    asset = Func_08002f40(0x15);
    StartDma_080f24a0(asset, (void *)0x05000200, 0x84000080);
    *(volatile u16 *)0x05000200 = 0;
    Func_080053e8(asset + 0x200, (void *)0x02010000);
    StartDma_080f24a0(
        (const void *)0x02010000,
        (void *)0x06010000,
        0x80000f00);

    asset = Func_08002f40(0x17);
    StartDma_080f24a0(asset, (void *)0x05000000, 0x84000080);
    *(volatile u16 *)0x05000000 = 0;
    Func_080053e8(asset + 0x200, (void *)0x02010000);
    StartDma_080f24a0(
        (const void *)0x02012940,
        (void *)0x06000000,
        0x80002760);
    StartDma_080f24a0(
        (const void *)0x0201a140,
        (void *)0x06004ec0,
        0x80004ec0);

    tilemap = (u16 *)0x0600f000;
    tilemap = FillTilemapRows_080f24a0(tilemap, 11, 0x0267);
    FillTilemapRows_080f24a0(tilemap, 21, 0x013b);
    tilemap = (u16 *)0x0600f800;
    tilemap = FillTilemapRows_080f24a0(tilemap, 11, 0x012c);
    FillTilemapRows_080f24a0(tilemap, 21, 0);

    bg2_control[0] = 0x1f43;
    bg2_control[1] = 0x1e81;
    window[0] = 0x00f0;
    window[2] = 0x009f;
    window[1] = 0x00f0;
    window[3] = 0x009f;
    window[4] = 0x1616;

    for (index = 0; index < 8; index++)
        scroll->offsets[index] = 0;
    scroll->offsets[3] = 0x60;
    scroll->offsets[5] = 0x60;

    for (index = 0; index < 6; index++)
        runtime->values[index] = 0;

    StartDma_080f24a0(scroll, (void *)0x04000010, 0x84000004);
    *blend_control = 0x3fbf;
    *blend_alpha = 0x1010;
    *blend_control = 0x3f44;
}
