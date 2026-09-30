/* Whole owner [080c08ec, 080c0a24), 312 bytes including 22 pool words.
   2026-09-26 H1: pool-loaded 0x230 decoder size and in-place size >>= 2
   before Dma_Set. The DMA setup now has the exact size/control instructions,
   but the pre-existing slot-anchor lifetime retains another saved register:
   candidate 320/312, 147 differing halfwords, 62 aligned edits, versus the
   baseline 308/312, 141 halfwords, 48 edits. No new DONE bytes. Stop here at
   checkpoint rather than permute that old anchor residual. Caller 080b63c8
   supplies mode 1, work+0x648 resource and level 0; the far-call table also
   exposes this loader. Audited callees establish ScaleRgb555Clamped returns
   s32 (this older draft still says void); the copied 560-byte decoder is the
   maintained DECODE/BIT_DISPATCH/BIT_COMMANDS library block, not game C.
   2026-09-30: the size is the decoder block's own length, BIT_DECODER_SIZE,
   replacing the Value_00000230 symbol (an address-named answer): 100/103
   instructions, 45 differing lines (was 51).
   2026-09-24: hand-written, 141 differing halfwords. The reference reaches
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

/* The RAM copy of the tile bit decoder: DECODE.S, BIT_DISPATCH.S, their jump
   tables and BIT_COMMANDS.S in one 560-byte block. */
#define BIT_DECODER_SIZE 0x230
extern void *gTransitionWork[];

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Graphics_ScaleRgb555Clamped(u16 *src, void *dst, s32 scale, s32 count);
void Graphics_BuildSequentialTileTable(void *dst);
void BattlePresentation_BuildTilemap(void *dst);
void Func_080b5138(void);
void BattlePres_UpdateHBlankScroll(void);
void Scheduler_AddOrUpdateCallback(void *callback, s32 order);

void BattleBackground_Load(s32 mode, s32 resource, s32 level)
{
    struct BattleBgState *state = gTransitionWork[0];
    u8 *data = GetResource(resource);
    struct BattleScreen *screen = gTransitionWork[-35];
    u32 size = BIT_DECODER_SIZE;
    void *table;

    table = Runtime_AllocateHeapBlock(49, size);
    size >>= 2;
    Dma_Set((void *)Func_080b5138, table, 0x84000000 | size, (volatile u32 *)0x040000d4);
    ((DecodeFn)gTransitionWork[5])(data + 0x100, (void *)0x06008000);
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
        Scheduler_AddOrUpdateCallback((void *)BattlePres_UpdateHBlankScroll, 0x4ff);
    state->mode = mode;
    if (mode == 1)
        *(volatile u16 *)0x0400000a = 0x1f83;
}
