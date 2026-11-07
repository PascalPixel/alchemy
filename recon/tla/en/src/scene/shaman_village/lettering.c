#include "../../../../../../games/tla/src/scene/shaman_village/shaman_village.h"
#include "heap_release.h"
#include "resource.h"
#include "dma.h"
#include "io_reg.h"
#include "edition.h"

// The European editions' lettering sheet has one more palette row.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||                      \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define LETTERING_PALETTE_ROWS 6
#else
#define LETTERING_PALETTE_ROWS 5
#endif

extern s16 gData_02005158[];
extern u8 gData_02004bfe[];
extern char ResourceId_LetteringBanner;

void Func_0200479c(const void *source, void *destination);

/// The decoded lettering sheet: its palette rows, then eight graphics of 32
/// tiles each.
struct LetteringSheet {
  u8 palettes[LETTERING_PALETTE_ROWS][32];
  u8 graphics[8][1024];
};

static inline s32 Vram_Load(s32 block, s32 size, const void *data) {
  /* FAKEMATCH(wrapper): forwarded as the Broken Seal's scene layer forwards it,
   * so the size is built after the tile address; as a direct call, 2 of 57
   * instructions differ in tla-en. */
  return VramCache_Load(block, size, data);
}

/// Decodes the lettering sheet, sends one graphic's palette to OBJ palette 15
/// and loads its tiles into a VRAM block of its own. Graphic 8 has a palette of
/// its own and graphic 4's tiles.
void Func_02002fb0(s32 graphic) {
  // BUG: this holds seven graphics of the sheet's eight, so decoding the sheet
  // writes 1024 bytes past the block.
#ifdef BUGFIX
  struct LetteringSheet *sheet = Heap_AllocEwram(sizeof(struct LetteringSheet));
#else
  struct LetteringSheet *sheet =
      Heap_AllocEwram((224 + LETTERING_PALETTE_ROWS) * 32);
#endif
  struct DmaRegisters volatile *dma;
  s32 palette;

  if (gData_02005158[0] == -1)
    gData_02005158[0] = Resource_FindFreeEntry();
  palette = gData_02004bfe[graphic];
  if (graphic == 8)
    graphic = 4;
  Func_0200479c(Resource_GetTableEntry((s32)&ResourceId_LetteringBanner),
                sheet);
  Dma_Set(sheet->palettes[0] + palette, (void *)&OBJ_PLTT_COLOR(15, 0),
          (DMA_ENABLE | DMA_32BIT) << 16 | 32 / 4, REG_DMA3);
  Vram_Load(gData_02005158[0], sizeof(sheet->graphics[0]),
            sheet->graphics[graphic]);
  dma = (struct DmaRegisters volatile *)REG_DMA3;
  while ((s32)dma->control < 0)
    ;
  Heap_Release(sheet);
}
