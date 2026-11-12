#include "shaman_village.h"

extern u32 gData_02005410[];
extern u32 gData_02005428[];
extern s16 gData_02005590[];
struct ShamanVillageSceneBlock {
  u8 unk00[22];
  u8 field16;
};

extern struct ShamanVillageSceneBlock gData_020056b0;
extern u32 gData_02005860[];
extern u32 gData_02005998[];
extern u32 gData_02005a40[];

void *Func_020002dc(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShamanVillage1)
    return gData_02005428;
  if (scene == (s32)&SceneId_ShamanVillage3)
    return gData_02005590;
  if (scene == (s32)&SceneId_ShamanVillage2) {
    if (GameFlag_Test(0x9fe))
      gData_020056b0.field16 = 1;
    return &gData_020056b0;
  }
  if (scene == (s32)&SceneId_ShamanVillage4)
    return gData_02005860;
  if (scene == (s32)&SceneId_ShamanVillage5)
    return gData_02005998;
  if (scene == (s32)&SceneId_ShamanVillage6)
    return gData_02005a40;
  return gData_02005410;
}

#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"

extern s16 gData_02006350[];

/// Fills BG0's line scroll table with a jittered sine wave and streams it
/// to BG0HOFS by HBlank DMA 0.
void Func_02000378(void) {
  s16 *line = gData_02006350;
  s32 i;
  s16 *source;
  volatile u16 *scroll;
  volatile u16 *dma;

  for (i = 0; i != 160; i++, line++) {
    s32 range = (160 - i) >> 2;

    *line = (Iwram_MulQ16(Math_Sine((i + *(u8 *)&gFrameCount) << 8),
                          (144 - i) << 10) >>
             11) -
            4;
    *line += Random16Far() % (range * 2) - range;
    if (*line > 56)
      *line -= 56;
    else if (*line < -64)
      *line += 64;
  }
  line = gData_02006350;
  scroll = (volatile u16 *)REG_BG0HOFS;
  *scroll = *line;
  dma = REG_DMA0;
  dma[5] &= 0xc5ff; // stop the start timing and the repeat
  dma[5] &= 0x7fff; // stop the channel
  source = line + 1;
  (void)dma[5];
  Dma_Set(source, scroll, 0xa2600001, (volatile u32 *)dma);
}

void Func_02000420(void) {
  s32 *view = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 376);

  view[2] -= 0x20000;

  view[3] += 0x20000;
  if (view[3] > 0x4100000) {
    view[2] = 0;
    view[3] = 0x4000000;
  }
}

void Func_02000458(struct ShamanVillageObject *object) {
  s32 bit = (((u32)gFrameCount & 3) >> *((u8 *)object + 99)) & 1;

  Actor_SetPartAttribute(object, (u8)(bit * *((u8 *)object + 98)));
}
