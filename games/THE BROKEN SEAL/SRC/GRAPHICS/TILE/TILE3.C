#include "DMA.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"

extern const u8 SentouKouka_Tenkai[];
extern void *Data_03001e50[];
extern u8 SentouKouka_TenkaiCodeSize[];
void Graphics_ClearCharacterBlockAndPalette(s32 alternate);
u8 *Resource_GetTableEntry(u32 resource);
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);

/* The decoder returns a value this caller ignores. */
typedef s32 (*PackedDecoder)(u8 *source, u32 destination, u32 fill);

typedef struct {
    u16 unused[4];
    u16 first;
    u16 padding;
    u16 second;
} State;

extern u32 gFrameTick;
extern State gBgScroll;

/* Clears the background (or, when alternate, the second) character block to
   its fill pattern and the matching palette bank to zero. */
void Graphics_ClearCharacterBlockAndPalette(s32 alternate)
{
    u32 value;
    u32 vram;
    u32 palette;
    volatile u32 fill;

    if (alternate == 0) {
        value = 0x01010101;
        vram = 0x06000000;
        palette = 0x05000000;
    } else {
        value = 0x81818181;
        vram = 0x06008000;
        palette = 0x05000100;
    }
    fill = value;
    Dma_Set((const void *)&fill, (void *)vram, 0x85001e00, (volatile u32 *)0x040000d4);
    fill = 0;
    Dma_Set((const void *)&fill, (void *)palette, 0x85000040, (volatile u32 *)0x040000d4);
}

/* Load character set `resource` into the background (or, when alternate, the
   second) character block and its palette into the matching bank; resource
   0 clears them instead. The packed decoder is copied from ROM into heap
   block 49 and run there; the palette load is queued for the next frame.

   FAKEMATCH: the queued palette load is QueueIoWriteDelay-style inline code
   with the odd constructs of SYSTEM/IO_WRITE_QUEUE.C (a one-pass loop around
   the IME read, and the count stored through an explicit u16 pointer). */
void Graphics_LoadCharacterBlockAndPalette(u32 resource, s32 alternate)
{
    u8 *data;
    u32 palette;
    u32 fill;
    u32 vram;
    void *decoder;
    u32 size;

    if (resource == 0) {
        Graphics_ClearCharacterBlockAndPalette(alternate);
        return;
    }
    data = Resource_GetTableEntry(resource);
    if (alternate == 0) {
        fill = 0;
        vram = 0x06000000;
        palette = 0x05000000;
    } else {
        fill = 0x80808080;
        vram = 0x06008000;
        palette = 0x05000100;
    }
    size = (u32)SentouKouka_TenkaiCodeSize;
    decoder = Runtime_AllocateHeapBlock(49, size);
    Dma_Set((void *)SentouKouka_Tenkai, decoder, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    ((PackedDecoder)Data_03001e50[49])(data + 256, vram, fill);
    Runtime_ReleaseHeapBlock(49);

    {
        volatile u16 *ime;
        struct IoWriteQueue *q;
        u32 saved;
        s32 count;

        q = &gIoWriteQueue;
        /* FAKEMATCH: removing this one-pass block changes instruction scheduling; see its retained draft. */
        do {
            ime = &REG_IME;
            saved = *ime;
        } while (0);
        *ime = (u16)ime;
        count = q->count;
        if (count <= 31) {
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);
            *(u16 *)&q->count = count + 1;
            *destination++ = (u32)data;
            *destination++ = palette;
            *destination = 0x84000040;
        }
        *ime = saved;
    }
}

void DisplayScroll_BuildHblankWordTable(u32 *arg0)
{
    s32 count;
    u32 value = 0x01FF01FF;
    u32 step = 0x10000;

    count = 31;
    do {
        count--;
        *arg0++ = value;
    } while (count >= 0);
    count = 239;
    do {
        count--;
        *arg0++ = step;
        step += 0x20002;
    } while (count >= 0);
    count = 47;
    do {
        count--;
        *arg0++ = value;
    } while (count >= 0);
    step = 0;
    count = 191;
    do {
        count--;
        *arg0++ = step;
    } while (count >= 0);
}

void DisplayScroll_StepPositionEveryFourFrames(void)
{
    if ((gFrameTick & 3) == 0) {
        State *state = &gBgScroll;
        u32 decrement = 0xffff; /* one step back in the 16-bit positions */
        state->first += decrement;
        state->second += decrement;
    }
}

extern u8 gOamCopyEnabled;
extern u8 Data_03001f58;
extern u8 Data_03001ac4;
extern u8 gOptionMirror;
extern const u32 DisplayScroll_SlideResources[];

void DisplayScroll_InitObjectTable(void);
void Ui_LoadWindowGraphics(void);
void Bg0_ClearTilemap(void);
void WaitFrames(s32 frames);

/* FAKEMATCH: the write goes through its own int, which loads each value as a
   word, and the do/while (0) ends a scheduling region after it; together
   they give the reference's register order and single literal pool. */
#define Io_Write16(reg, value) \
    do { \
        /* FAKEMATCH: the one-pass register-write boundary preserves measured instruction scheduling. */ \
        /* FAKEMATCH: the register-write word temporary preserves measured value allocation. */ \
        s32 value_ = (value); \
        (reg) = value_; \
    } while (0)

/* Shows the 33 slides, crossfading each into the other background over 16
   steps of four frames and holding it 267 frames, then restores the menu
   display. */
s32 DisplayScroll_RunSlideshow(void)
{
    u32 slide;
    s32 alpha;

    gOamCopyEnabled = 0;
    Data_03001f58 = 0;
    Data_03001ac4 = 0;
    gOptionMirror = 0;
    Scheduler_ResetTaskTable();
    Scheduler_AddOrUpdateCallback((s32)DisplayScroll_StepPositionEveryFourFrames, 0x480);
    Io_Write16(REG_DISPCNT, 0x40);
    DisplayScroll_BuildHblankWordTable((u32 *)0x06007800);
    DisplayScroll_BuildHblankWordTable((u32 *)0x0600f800);
    Graphics_ClearCharacterBlockAndPalette(0);
    Graphics_ClearCharacterBlockAndPalette(1);
    Io_Write16(REG_BG2CNT, 0x1f8a);
    Io_Write16(*(volatile u16 *)0x0400000e, 0x0f83);
    Io_Write16(REG_DISPCNT, 0x1c40);
    Io_Write16(REG_BLDCNT, 0x2844);
    DisplayScroll_InitObjectTable();
    WaitFrames(300);
    for (slide = 0; slide <= 32; slide++) {
        Graphics_LoadCharacterBlockAndPalette(DisplayScroll_SlideResources[slide], (slide & 1) ^ 1);
        for (alpha = 1; alpha <= 16; alpha++) {
            if (slide & 1)
                Io_Write16(REG_BLDALPHA, (alpha << 8) | (16 - alpha));
            else
                Io_Write16(REG_BLDALPHA, ((16 - alpha) << 8) | alpha);
            WaitFrames(4);
        }
        WaitFrames(267);
    }
    Io_Write16(REG_BLDCNT, 0);
    Io_Write16(REG_DISPCNT, 0x1040);
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    gOamCopyEnabled = 1;
    return 0;
}

#undef Io_Write16
