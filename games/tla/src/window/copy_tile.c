#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "dma.h"
#include "io_reg.h"

struct MenuFlagWork {
  u8 unk000[0x506];
  u8 busy;
};

struct FxGroup {
  u8 unk00[32];
  struct Actor *objects[8];
  u16 values[8];
  s32 count;
};

struct DescriptorEntry {
  s16 id;
  u8 unk02[6];
};

extern void *gOverlayArea[];
extern struct DescriptorEntry gData_080eff28[];

void BattleEffect_StartTiledCanvas(s32 mode);
void BattleEffect_EndCanvasLayer(void);
void Func_0814ea58(void *effect, s32 mode);
void PartyStatus_CreateWindow(s32 flags);
void wait(s32 frames);
s32 Func_08020310(s32 x, s32 z, s32 layer);
s32 Func_080dbde8(s32 x, s32 z, s32 layer);
u8 Func_080dbdf4(s32 x, s32 z, s32 layer, s32 value);
void Func_08108148(void);
s32 Inn_RoomPrice(s32 innId);
void Func_0810824c(void);

/// Copies one 4bpp tile of BG character block 0 to another, each given as a
/// tilemap entry whose palette and flip bits are ignored. The battle lists copy
/// tiles 24 and 25 (as entries `0xf018` and `0xf019`) to tiles 512, 513, 528
/// and 529 as they open.
void Vram_CopyTile(u32 sourceTile, u32 destinationTile) {
  Dma_Set(BG_CHAR_BLOCK(0) + (sourceTile & 0x3ff) * TILE_SIZE_4BPP,
          BG_CHAR_BLOCK(0) + (destinationTile & 0x3ff) * TILE_SIZE_4BPP,
          0x84000008, REG_DMA3);
}
