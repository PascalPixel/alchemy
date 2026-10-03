#include "RUNTIME_MEM.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"
#include "CALLBACK_SCHEDULER.H"
#include "BATTLE_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "HEAP_STATE.H"

extern struct BattleBackgroundView *gTransitionWork;

void BattlePresentation_InitializeWorkAndResetState(void)
{
    struct BattleAffineHdma *work;
    struct BattleBackgroundView *state;
    volatile u32 zero;
    work = Runtime_AllocateBlock(10, sizeof *work);
    state = gTransitionWork;
    zero = 0;
    Dma_Set(&zero, work, 0x850000a8, (volatile u32 *)0x040000d4);
    state->mode = 0;
}

void Runtime_ReleaseHeapBlock10(void)
{
    Runtime_ReleaseHeapBlock(10);
}

typedef void (*BitDecoder)(const u8 *source, void *destination);

/* The length of the tile bit decoder block copied to RAM, DECODE.S through
   BIT_COMMANDS.S, as the linker script measures it. */
extern u8 BitDecoder_Size[];

s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count);
void Graphics_BuildSequentialTileTable(void *destination);
void BattlePresentation_BuildTilemap(void *destination);
void BitDecoder_DecodeImage(void);
void BattlePres_UpdateHBlankScroll(void);

/* Decode a battle background's tiles through the bit decoder copied into
   heap slot 49, keep its palette in the session and show it at the given
   level (a negative level leaves the shown palette alone), then rebuild the
   tile table and the tilemap and start the H-blank scroll on first use. */
void BattleBackground_Load(s32 mode, s32 resource, s32 level)
{
    /* FAKEMATCH: direct heap reads retain the base (320 bytes); named cells
       add pooled addresses and change registers. The typed background-cell
       anchor preserves the original 312-byte register allocation. */
    struct BattleBackgroundView **background = (struct BattleBackgroundView **)
        &((union HeapState *)Ram_WorkSlot)->slots[HEAP_SLOT_BATTLE_BACKGROUND];
    struct BattleBackgroundView *view = *background;
    u8 *data = Resource_GetTableEntry(resource);
    struct BattleSession *session = *(struct BattleSession **)((u8 *)background
        + (HEAP_SLOT_BATTLE - HEAP_SLOT_BATTLE_BACKGROUND) * (s32)sizeof(void *));
    u16 *palette;

    /* FAKEMATCH: the one-pass block keeps the size load after the session load and the decoder call after the copy. */
    do {
        u32 size = (u32)BitDecoder_Size;
        void *decoder = Runtime_AllocateHeapBlock(49, size);

        Dma_Set((void *)BitDecoder_DecodeImage, decoder, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    } while (0);
    {
        u32 offset = 0x100;
        void *vram;

        /* Dma_Set is a volatile statement, which the scheduler counts as one
           more dependent of the tile address: written plainly, the address
           is added before the VRAM address loads, the reverse of the ROM. */
        /* FAKEMATCH: the empty statement keeps the tile offset a load of its own, ahead of the VRAM address. */
        __asm__("" : "+r"(offset));
        vram = (void *)0x06008000;
        (*(BitDecoder *)((u8 *)background
            + (49 - HEAP_SLOT_BATTLE_BACKGROUND) * (s32)sizeof(void *)))(data + offset, vram);
    }
    Runtime_ReleaseHeapBlock(49);
    palette = session->palette;
    Dma_Set(data, palette, 0x84000040, (volatile u32 *)0x040000d4);
    if (level >= 0)
        Graphics_ScaleRgb555Clamped(palette, (u16 *)0x050000c0, session->brightness = 0x10000 - level * 1092, 128);
    Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, (volatile u32 *)0x040000d4);
    *(u16 *)0x050000bc = *(u16 *)0x050001e8;
    Graphics_BuildSequentialTileTable((void *)0x06003800);
    BattlePresentation_BuildTilemap((void *)0x0600f800);
    Iwram_ClearWords((void *)0x0600ffc0, 64);
    if (view->mode == 0)
        Scheduler_AddOrUpdateCallback((s32)BattlePres_UpdateHBlankScroll, 0x4ff);
    view->mode = mode;
    if (mode == 1)
        *(volatile u16 *)0x0400000a = 0x1f83;
}
