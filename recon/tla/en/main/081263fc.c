// Draft of Func_081263fc, The Broken Seal's BattleBackground_Load
// (games/tbs/src/battle/battle.c): differ 4 of 137 (tla-en, 2026-10-15).
// The Lost Age copies the decoder into a heap block of its own (RamCode_Load
// and MemFree) instead of work slot 49, scales the palette with
// ColorBuffer_Scale, and skips the backdrop's palette copy while the battle
// work's byte at 0x867 is set (summons 4, 6 and 24 set it).

// Left: two halfword copies whose address the native starts around its
// load (the 0x050001e8 colour, ldrh between movs and lsls of 0x050000bc; the
// BG1CNT value, ldr between movs and lsls of 0x0400000a); here the address
// is finished first. sched2 issues the pooled load one slot later than the
// native, the residue the Djinn drafts record. Tried: the colour through a
// local (differ 27).

#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "ram_buffer.h"
#include "ramcode.h"

struct BattlePresentationTransition {
  u8 unk00[8];
  s32 mode;
};

// The parts of the battle work the backdrop uses.
struct BattleBackdropWork {
  u8 unk000[0x56c];
  u16 palette[128];
  s32 brightness;
  u8 unk670[0x1f7];
  u8 keepBackdropPalette;
};

typedef void (*ModelDecoder)(const void *source, void *destination);

extern const u8 Model_DecodeCode[];
extern u8 Model_DecodeCodeSize[];

u8 *Resource_GetTableEntry(s32 id);
void ColorBuffer_Scale(u16 *source, u16 *destination, s32 scale, s32 count);
void Graphics_BuildSequentialTileTable(void *destination);
void Func_08125bb8(void *destination);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void BattlePres_UpdateHBlankScroll(void);

// Loads a battle backdrop: its picture decoded into BG VRAM, its palette
// (darkened by level steps of 1/60 when level is not negative), the tile
// table and tilemap, and the HBlank scroll the first time.
void Func_081263fc(s32 mode, s32 resource, s32 level) {
  struct BattlePresentationTransition *view = Ram_HeapSlots->transitionWork;
  u8 *data = Resource_GetTableEntry(resource);
  struct BattleBackdropWork *session =
      (struct BattleBackdropWork *)Ram_HeapSlots->battleWork;
  ModelDecoder decode;
  u16 *palette;

  RamCode_Load(decode, Model_DecodeCode, Model_DecodeCodeSize);
  {
    u32 offset = 0x100;
    void *vram;

    /* FAKEMATCH: as in The Broken Seal, conditional cancelling updates end
     * CSE before the decoder's arguments, so the VRAM address loads before
     * the data add (plainly, or through a vram local: differ 6; a one-pass
     * block before the call: 6). */
    if (session) {
      offset++;
      offset--;
    }
    vram = (void *)0x06008000;
    decode(data + offset, vram);
  }
  MemFree(decode);
  palette = session->palette;
  Dma_Set(data, palette, 0x84000040, REG_DMA3);
  if (level >= 0)
    ColorBuffer_Scale(palette, (u16 *)0x050000c0,
                      session->brightness = 0x10000 - level * 1092, 128);
  if (session->keepBackdropPalette == 0) {
    Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
    *(u16 *)0x050000bc = *(u16 *)0x050001e8;
  }
  Graphics_BuildSequentialTileTable((void *)0x06003800);
  Func_08125bb8((void *)0x0600f800);
  Iwram_ClearWords((void *)0x0600ffc0, 64);
  if (view->mode == 0)
    Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0xcff);
  view->mode = mode;
  if (mode == 1)
    REG_BG1CNT = 0x1f83;
}
