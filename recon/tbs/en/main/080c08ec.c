/* BattleBackground_Load: decode a battle background's tiles through the bit
   decoder copied into heap slot 49, keep its palette in the session and show
   it at the given level, then rebuild the tile table and tilemap.

   2026-10-01 slice-6: 1 instruction off (was 36). Every register, the frame
   and the pool now agree; the work-slot pointers hang off the one
   gTransitionWork symbol, whose spilled base reload reloads before each use.
   Remaining: before the decoder call the reference loads r1 (the VRAM
   address) and then adds data into r0; this draft adds first. The two are
   tied in the scheduler (same priority, same count of dependents), so
   original insn order decides, and the argument add always precedes the
   second argument's load. Not solved by a temporary for either argument, an
   indexed or member address, or pinning r0 and r1; the permuter found
   nothing in 100 s. The reference's order needs the add emitted after r1's
   load, or the palette copy's asm not being a volatile barrier (it then
   counts one dependent fewer for the add).
   BitDecoder_Size is linked in recon/tbs/en/MAIN.LD only; the other five
   editions need the same line when this is adopted. */

#include "TYPES.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "IWRAM_CALL.H"
#include "CALLBACK_SCHEDULER.H"
#include "BATTLE_WORK.H"

typedef void (*DecodeFn)(const void *src, void *dst);

struct BattleView {
    u8 padding00[8];
    s32 mode;
};

/* The length of the tile bit decoder block copied to RAM (DECODE.S through
   BIT_COMMANDS.S), linked in MAIN.LD. */
extern u8 BitDecoder_Size;
extern u8 gTransitionWork[];

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count);
void Graphics_BuildSequentialTileTable(void *dst);
void BattlePresentation_BuildTilemap(void *dst);
void BitDecoder_DecodeImage(void);
void BattlePres_UpdateHBlankScroll(void);

void BattleBackground_Load(s32 mode, s32 resource, s32 level)
{
    /* FAKEMATCH: the session and the installed decoder are addressed from
       the view pointer's symbol, as the reference derives them from its one
       pool entry. */
    struct BattleView *view = *(struct BattleView **)gTransitionWork;
    u8 *data = Resource_GetTableEntry(resource);
    struct BattleSession *session = *(struct BattleSession **)(gTransitionWork - 140);
    u16 *palette;

    /* FAKEMATCH: the one-pass block keeps the size load after the session
       load and the decoder call's arguments after the copy. */
    do {
        u32 size = (u32)&BitDecoder_Size;
        void *decoder = Runtime_AllocateHeapBlock(49, size);

        Dma_Set((void *)BitDecoder_DecodeImage, decoder, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    } while (0);
    (*(DecodeFn *)(gTransitionWork + 20))(data + 0x100, (void *)0x06008000);
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
