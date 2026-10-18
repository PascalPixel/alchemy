#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "dma.h"
#include "io_reg.h"

struct MenuFlagWork {
  u8 unk000[0x506];
  u8 busy;
};

struct FxGroup {
  u8 unk00[32];
  struct ObjectRuntime *objects[8];
  u16 values[8];
  s32 count;
};

struct DescriptorEntry {
  s16 id;
  u8 unk02[6];
};

extern void *gOverlayArea[];
extern struct DescriptorEntry gData_080eff28[];

void BattleFx_BeginTiledCanvas(s32 mode);
void BattleFx_EndCanvasLayer(void);
void Func_0814ea58(void *effect, s32 mode);
void UiWindow_CreateWithLayoutBounds(s32 flags);
void WaitFrames(s32 frames);
s32 Func_08020310(s32 x, s32 z, s32 layer);
s32 Func_080dbde8(s32 x, s32 z, s32 layer);
u8 Func_080dbdf4(s32 x, s32 z, s32 layer, s32 value);
void Func_08108148(void);
s32 Func_0810b378(s32 value);
void Func_0810824c(void);

void Func_08049a04(u32 source_tile, u32 destination_tile) {
  Dma_Set((u8 *)0x06000000 + (source_tile & 0x3ff) * 32,
          (u8 *)0x06000000 + (destination_tile & 0x3ff) * 32, 0x84000008,
          REG_DMA3);
}
