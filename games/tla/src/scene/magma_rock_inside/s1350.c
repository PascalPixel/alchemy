#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "map.h"
#include "event_inline.h"

extern char SceneId_Ov692Room2;
extern char SceneId_Ov692Room3;
extern char SceneId_Ov692Room4;
extern const s32 gData_02005050[];
extern const s32 gData_0200502c[];

void Func_02004cc0(void);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_020034bc(void);
void Func_020034f0(void);
void Func_02003548(void);

static __inline__ void Func_02004d10_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): Func_02004d10_ forwarded inline; as direct calls to
   * Map_SetScreenShake, 110 of 537 instructions differ in tla-en. */
  Map_SetScreenShake(a, b, c);
}

static __inline__ void Func_02004cd8_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH(wrapper): Func_02004cd8_ forwarded inline; as direct calls to
   * Map_CopyCellPictureIndicesRect, 20 of 537 instructions differ in tla-en. */
  Map_CopyCellPictureIndicesRect(a, b, c, d, e, f);
}

void Func_02001350(void) {
  s32 srcX;
  s32 srcZ;
  s32 baseX = 0;
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  struct EffectOptions options;
  struct EffectOptions *effectOptions;
  struct EffectOptions *lastOptions;
  s32 position[3];
  s32 *burstPosition;
  s32 *sparkPosition;
  s32 *roomPosition;
  u32 t;
  s32 scene;
  s32 closing;
  typeof(gMainRoutineWork) *work;
  s32 riseX;
  u32 burstRandom, sparkRandom;
  u32 mask;
  u32 i;
  u32 row;

  /* FAKEMATCH(dead): srcX and srcZ start from a zero only combine sees; as srcX
   * = 0 and srcZ = 0, 138 of 537 instructions differ in tla-en. */
  srcX = ((u32)layer & 3) >> 2;
  srcZ = ((u32)layer & 7) >> 3;
  /* FAKEMATCH(temporary): t holds 212 for a set flag, then the frame count, in
   * r10; as two variables, 197 of 537 instructions differ in tla-en. */
  t = 0;
  effectOptions = &options;
  effectOptions->type = 403;
  effectOptions->priority = 1;
  effectOptions->script = gData_02005050;
  effectOptions->startScaleX = 0x1cccc;
  effectOptions->startScaleY = 0x1cccc;
  scene = gMainRoutineWork.scene;
  roomPosition = position;
  if (scene == (s32)&SceneId_Ov692Room2) {
    baseX = 164 << 17;
    roomPosition[1] = 0x200000;
    roomPosition[2] = 200 << 16;
    srcX = 20;
    srcZ = 69;
    if (GameFlag_Test(768))
      t = 212;
  } else if (scene == (s32)&SceneId_Ov692Room3) {
    baseX = 232 << 16;
    roomPosition[1] = 0x200000;
    roomPosition[2] = 248 << 16;
    srcX = 14;
    srcZ = 72;
    if (GameFlag_Test(769))
      t = 212;
  } else if (scene == (s32)&SceneId_Ov692Room4) {
    baseX = 196 << 17;
    roomPosition[1] = 0x200000;
    roomPosition[2] = 134 << 18;
    srcX = 24;
    srcZ = 90;
    if (GameFlag_Test(770))
      t = 212;
  }
  // The loops are written with goto: they carry no loop notes, so the masks,
  // the shake constants and the counters' shifts stay in each pass, as in the
  // native; as for loops, 431 of 537 instructions differ in tla-en.
  i = 0;
burst:
  /* FAKEMATCH(temporary): burstRandom holding the first call's value; as the
   * call inside the sum, 4 of 537 instructions differ in tla-en. */
  burstRandom = Random16Far();
  mask = 31;
  /* FAKEMATCH(barrier): the empty asm clobbering sfp; as no asm, 230 of 537
   * instructions differ in tla-en. */
  asm("" : : : "sfp");
  burstPosition = position;
  burstPosition[0] = baseX + ((burstRandom & mask) << 16);
  Sound_Play(145);
  Func_0200015c(burstPosition[0], burstPosition[1], burstPosition[2], 0,
                (Random16Far() & mask) << 12, 0, 0x3a0001, effectOptions);
  Func_02004d10_(0x50000, 0x50000, 0x10000);
  Func_02004d10_(-1, -1, 0xe666);
  i++;
  Event_Wait(8);
  if (i != 16)
    goto burst;
  Sound_Play(148);
  effectOptions->priority = 1;
  effectOptions->startScaleX = 0x20000;
  effectOptions->startScaleY = 0x20000;
  effectOptions->targetScaleX = 0x8000;
  effectOptions->targetScaleY = 0x8000;
  baseX -= 0x80000;
  i = 0;
  riseX = baseX;
rise:
  burstPosition[0] = riseX;
  Func_0200015c(burstPosition[0], burstPosition[1], burstPosition[2], 0, 0,
                (Random16Far() & 31) << 10, 0xe0001, effectOptions);
  i++;
  riseX += 0x60000;
  if (i != 8)
    goto rise;
  Func_02004cd8_(68, 70, srcX, srcZ, 3, 6);
  Func_02004d10_(0x50000, 0x70000, 0x10000);
  Func_02004d10_(-1, -1, 0xe666);
  Event_Wait(30);
  Map_CopyCellPictureIndicesRect(71, 70, srcX, srcZ, 3, 6);
  effectOptions->type = 403;
  effectOptions->priority = 1;
  effectOptions->palette = 10;
  effectOptions->script = gData_0200502c;
  if (t != 0) {
    Map_CopyCellPictureIndicesRect(68, 70, srcX, srcZ, 3, 6);
    Event_Wait(30);
    burstPosition[2] -= 0x280000;
  } else {
    Func_02004d10_(0x20000, 0x10000, 0x10000);
    Sound_Play(107);
    row = 0;
  row:
    i = 0;
  column:
    burstPosition[0] = baseX + ((Random16Far() % 48) << 16);
    Func_0200015c(burstPosition[0], burstPosition[1],
                  burstPosition[2] + (row << 18) + 0x80000, 0, 0x10000, 0,
                  0x330000, effectOptions);
    i++;
    if (i != 6)
      goto column;
    wait(3);
    Map_CopyCellPictureIndicesRect(74, 70, srcX, srcZ, 3, (row >> 2) + 6);
    row++;
    if (row != 12)
      goto row;
    Map_CopyCellPictureIndicesRect(74, 70, srcX, srcZ, 3, 8);
    t = 0;
  frame:
    if ((t & 3) == 0) {
      i = 0;
    spark:
      /* FAKEMATCH(temporary): sparkRandom holding the call's value; as the call
       * inside the sum, 2 of 537 instructions differ in tla-en. */
      sparkRandom = Random16Far();
      sparkPosition = position;
      sparkPosition[0] = baseX + ((sparkRandom % 48) << 16);
      Func_0200015c(sparkPosition[0], sparkPosition[1],
                    sparkPosition[2] + 0x300000, 0, 0x10000, 0, 0x330000,
                    effectOptions);
      i++;
      if (i != 4)
        goto spark;
    }
    if (t == 0) {
      *(s32 *)&layer->unk08[4] -= 0x200000;
      Func_02004cc0();
      Event_Wait(1);
    } else if (t <= 63) {
      REG_BLDALPHA = ((16 - (t >> 2)) << 8) | (t >> 2);
      REG_BLDCNT = 0x3f44;
    } else if (t == 64) {
      REG_BLDCNT = 0;
    } else if (t <= 127) {
      *(s32 *)&layer->unk08[4] += 0x8000;
      roomPosition = position;
      roomPosition[2] -= 0x8000;
      Func_02004cd8_(71, 76, srcX, srcZ - ((t - 47) >> 5) + 8, 3, 1);
    } else if (t == 128) {
      Func_02004d10_(-1, -1, 0xe666);
    }
    t++;
    wait(1);
    if (t != 160)
      goto frame;
    MapGrid_WaitForShakeToSettle();
  }
  Sound_Play(288);
  Sound_Play(136);
  lastOptions = &options;
  lastOptions->startScaleY = 0x1cccc;
  lastOptions->startScaleX = 0x1cccc;
  i = 0;
last:
  /* FAKEMATCH(temporary): sparkRandom holding the call's value; as the call
   * inside the sum, 2 of 537 instructions differ in tla-en. */
  sparkRandom = Random16Far();
  sparkPosition = position;
  sparkPosition[0] = baseX + ((sparkRandom % 48) << 16);
  i++;
  Func_0200015c(sparkPosition[0], sparkPosition[1], sparkPosition[2] + 0x300000,
                0, 0, 0, 0x3b0000, lastOptions);
  if (i != 6)
    goto last;
  MapGrid_SetShake(0x50000, 0x70000, 0x10000);
  Func_02004d10_(-1, -1, 0xe666);
  Map_CopyCellPictureIndicesRect(68, 64, srcX, srcZ, 3, 6);
  MapGrid_WaitForShakeToSettle();
  Event_Wait(30);
  work = &gMainRoutineWork;
  *(s32 *)&layer->unk08[4] = 0;
  closing = work->scene;
  if (closing == (s32)&SceneId_Ov692Room2)
    Func_020034bc();
  else if (closing == (s32)&SceneId_Ov692Room3)
    Func_020034f0();
  else if (closing == (s32)&SceneId_Ov692Room4)
    Func_02003548();
}
