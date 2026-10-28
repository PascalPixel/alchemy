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
void WaitFrames(s32 frames);
s32 Func_08020310(s32 x, s32 z, s32 layer);
s32 Func_080dbde8(s32 x, s32 z, s32 layer);
u8 Func_080dbdf4(s32 x, s32 z, s32 layer, s32 value);
void Func_08108148(void);
s32 Inn_RoomPrice(s32 innId);
void Func_0810824c(void);

void Func_080ea8a8(s32 x, s32 z, s32 layer) {
  if (Func_08020310(x, z, layer) == 0)
    Func_080dbdf4(x, z, layer, Func_080dbde8(x, z, layer) | 0x80);
}
