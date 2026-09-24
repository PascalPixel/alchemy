/* 2026-09-24: hand-written, 141 differing halfwords. The reference reaches
   0x03001e74 and 0x03001f14 relative to one 0x03001f00 anchor that reload
   rematerialises before each use (ldr =0x03001f00; subs #140); here the
   anchor is kept in a callee-saved register, which shifts every allocation. */

#include "TYPES.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "RESOURCE.H"

typedef void (*ClearFn)(void *dst, s32 size);
typedef void (*DecodeFn)(const void *src, void *dst);

static __inline__ void ClearWords(ClearFn clear, void *dst, s32 size)
{
    clear(dst, size);
}

struct BattleBgState {
    u8 unknown_00[8];
    s32 mode;
    u8 unknown_0c[8];
    DecodeFn decode;
};

struct BattleScreen {
    u8 unknown_000[0x544];
    u16 palette[128];
    s32 brightness;
};

extern u8 Value_00000230;
extern void *Data_03001f00[];

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Graphics_ScaleRgb555Clamped(u16 *src, void *dst, s32 scale, s32 count);
void Graphics_BuildSequentialTileTable(void *dst);
void BattlePresentation_BuildTilemap(void *dst);
void Scheduler_AddOrUpdateCallback(void *callback, s32 order);

void Func_080c08ec(s32 mode, s32 resource, s32 level)
{
    struct BattleBgState *state = Data_03001f00[0];
    u8 *data = GetResource(resource);
    struct BattleScreen *screen = Data_03001f00[-35];
    u32 size = 0x230;
    void *table;

    table = Runtime_AllocateHeapBlock(49, size);
    Dma_Set((void *)0x080b5138, table, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    ((DecodeFn)Data_03001f00[5])(data + 0x100, (void *)0x06008000);
    Runtime_ReleaseHeapBlock(49);
    Dma_Set(data, screen->palette, 0x84000040, (volatile u32 *)0x040000d4);
    if (level >= 0) {
        s32 scale = 0x10000 - level * 1092;
        screen->brightness = scale;
        Graphics_ScaleRgb555Clamped(screen->palette, (void *)0x050000c0, scale, 128);
    }
    Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, (volatile u32 *)0x040000d4);
    *(u16 *)0x050000bc = *(u16 *)0x050001e8;
    Graphics_BuildSequentialTileTable((void *)0x06003800);
    BattlePresentation_BuildTilemap((void *)0x0600f800);
    ClearWords((ClearFn)0x03000164, (void *)0x0600ffc0, 64);
    if (state->mode == 0)
        Scheduler_AddOrUpdateCallback((void *)0x080c0131, 0x4ff);
    state->mode = mode;
    if (mode == 1)
        *(volatile u16 *)0x0400000a = 0x1f83;
}

