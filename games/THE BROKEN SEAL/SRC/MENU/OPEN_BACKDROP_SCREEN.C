#include "TYPES.H"
#include "DMA.H"
#include "SYSTEM.H"

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);
typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

struct BackdropSave {
    u8 unknown_000[0xa8];
    u8 tiles[0x2000];
    u16 palette[64];
};

struct MenuWork {
    u8 unknown_000[0x30];
    s32 window;
    u8 unknown_034[0x150];
    struct BackdropSave *backdrop;
};

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
    return copy(dst, src, size);
}

static __inline__ s32 FillWords(WordFillFn fill, void *dst, s32 size, u32 value)
{
    return fill(dst, size, value);
}

s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void Func_080153d8(void *);
void *Runtime_GetLowTableAddress(void);
void Func_080aac84(s32);
s32 Func_080aafb8(struct BackdropSave *);

/* Opens the full-width menu window: saves BG character block 1 and
   palettes 4-7 into the backdrop buffer, blanks them with fill patterns,
   loads the frame tile and palettes, then runs the backdrop screen. */
s32 Menu_OpenBackdropScreen(void)
{
    struct MenuWork *work = *(struct MenuWork **)0x03001f2c;
    struct BackdropSave *backdrop = work->backdrop;

    UiWindow_UpdateOrCreate(&work->window, 0, 5, 30, 15, 2);
    WaitFrames(1);
    CopyWords((WordCopyFn)0x03001388, backdrop->tiles, (void *)0x06004000, 0x2000);
    CopyWords((WordCopyFn)0x03001388, backdrop->palette, (void *)0x05000080, 128);
    FillWords((WordFillFn)0x03000168, (void *)0x06004000, 0x2000, 0x33333333);
    FillWords((WordFillFn)0x03000168, (void *)0x05000080, 128, 0x55555555);
    Func_080153d8((void *)0x06005000);
    CopyWords((WordCopyFn)0x03001388, (void *)0x060052c0, (void *)0x080af26c, 32);
    Dma_Set(Runtime_GetLowTableAddress(), (void *)0x050000a0, 0x80000010,
            (volatile u32 *)0x040000d4);
    *(volatile u16 *)0x050000bc = *(volatile u16 *)0x050001e8;
    Dma_Set((void *)0x050001e0, (void *)0x050000e0, 0x80000010, (volatile u32 *)0x040000d4);
    Func_080aac84(8);
    *(volatile u16 *)0x050000e8 = *(volatile u16 *)0x050001e8;
    *(volatile u16 *)0x050000c8 = *(volatile u16 *)0x050001e8;
    return Func_080aafb8(backdrop);
}
