#include "local.h"
#include "macros.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

struct MagmaRockInsideWork {
  s32 unk00;
  s32 x;
  s32 y;
  s32 z;
  s32 unk10;
  struct Actor *target;
};

void Func_02000ed0(struct Actor *object);
void Func_02004a5c(s32 x, s32 y, s32 z, s32 kind);
void Func_02004b18(void);
void Func_02004bd4(void);
void Func_02004d40(void);
void Func_02004d48(void);

void Func_02001148(void) {
  struct MagmaRockInsideWork *work = Ram_HeapSlots->effectWork;
  struct Actor *object;

  Func_02004d40();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(10);
  object = work->target;
  object->update = NULL;
  Actor_SetSpriteColor(object, 7);
  FieldEvent_Wait(3);
  Actor_SetSpriteColor(work->target, 0);
  FieldEvent_Wait(1);
  Func_02000ed0(work->target);
  Func_02004d48();
}

void Func_02001190(s32 mode, s32 cue) {
  struct MagmaRockInsideWork *work = Ram_HeapSlots->effectWork;

  if (cue == 0)
    Func_02004b18();
  if (mode == 2) {
    struct Actor *object = work->target;

    Func_02004a5c(object->x, object->y + 0xf0000, object->z, 1);
  }
  if (mode == 3) {
    struct Actor *object = work->target;
    struct Actor *target;

    Func_02004a5c(object->x, object->y + 0xf0000, object->z, 8);
    target = work->target;
    SET_POSITION(target, 0, 0, 0);
  }
  if (cue == 0x3e7)
    Func_02004bd4();
  if (cue == 178)
    Func_02004c98(work->target, 3);
}

struct Actor *Actor_FindInCell(s32 *position, struct Actor *self);

void Func_020011fc(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *target;
  s32 position[3];
  s32 stepX;
  s32 stepZ;
  s32 i;
  s32 angle = (leader->angle + 0x2000) & 0xc000;

  position[0] = (leader->x & 0xfff00000) + 0x80000;
  position[1] = leader->y - 0x100000;
  position[2] = (leader->z & 0xfff00000) + 0x80000;
  FieldVector_AddPolarOffset(0x200000, angle, position);
  target = Actor_FindInCell(position, leader);
  if (target == 0)
    return;
  Func_02004d40();
  FieldEvent_ResolvePendingActions(0);
  Func_02004c98(leader, 6);
  System_Wait(6);
  Event_PlaySound(152);
  Func_02004c98(leader, 7);
  leader->speedLimit = 0x30000;
  leader->acceleration = 0x20000;
  leader->velocityY = 0x4cccc;
  leader->motionFlags &= 0x7e;
  Actor_SetSingleChildFlags(leader, 0);
  stepX = (position[0] - leader->x) / 12;
  stepZ = (position[2] - leader->z) / 12;
  i = 0;
  do {
    leader->x += target->x - position[0] + stepX;
    leader->z += target->z - position[2] + stepZ;
    position[0] = target->x;
    position[2] = target->z;
    System_Wait(1);
    i++;
  } while (i != 12);
  leader->x = target->x;
  leader->y = leader->terrainHeight = target->y + 0x100000;
  leader->z = target->z;
  Func_02004c98(leader, 6);
  Actor_SetSingleChildFlags(leader, 1);
  leader->motionFlags |= 3;
  Func_02004d48();
}

#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "map.h"
#include "event_inline.h"

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
  asm("" : : : SFP_CLOBBER);
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

#include "actor.h"

void Func_02000694(s32 layer, s32 x, s32 z, s32 mask);

/// A Magma Rock room's opening event: the leader is centred on its cell, the
/// screen shakes, and by room (2, 3 or 5, the last by entrance) it picks the
/// cells to open, the flag that was set (768 to 770, cleared here) and where
/// the effects play. The cells are copied open; if that flag was set, ten rows
/// of six effects rise along the opening while it widens and the blend fades
/// over 64 steps. Then the cells close again and the leader steps back a cell.
void Func_02001828(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  u32 facing = (leader->angle + 0x2000) & 0xc000;
  s32 position[3];
  struct EffectOptions options;
  s32 baseX = 0;
  s32 dstX = 0;
  s32 dstZ = 0;
  s32 open = 0;
  s32 srcX = 0;
  u8 *flags;
  u32 i = 0;
  u32 j;
  s32 *p;
  s32 rowAbove;

  /* FAKEMATCH(wrapper): the calls that repeat a constant, the flag tests and
   * the cell copies go through call.h's forwarding inlines, so that each call
   * builds its own constants and stack arguments; as direct calls, 163 of 496
   * instructions differ in tla-en. */
  Func_02004d40();
  FieldEvent_ResolvePendingActions(0);
  // One local reused for the position's address, the loop counters and the
  // owner's address: the native keeps all of them in r6.
  j = (u32)position;
  ((s32 *)j)[0] = (leader->x & 0xfff00000) + 0x80000;
  ((s32 *)j)[2] = (leader->z & 0xfff00000) + 0x80000;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, ((s32 *)j)[0] >> 16,
                      ((s32 *)j)[2] >> 16);
  FieldEvent_Wait(20);
  Event_PlaySound(584);
  flags = &leader->priorityFlags;
  *flags &= ~64;
  FieldEvent_Wait(20);
  MapGrid_SetShake(0x10000, 0x20000, 0x10000);
  Event_PlaySound(107);
  FieldEvent_Wait(30);
  // Each test reads the scene again: the native reloads gMainRoutineWork for
  // room 3's test and reuses that load for room 5's.
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room2) {
    srcX = 20;
    i = 89;
    dstX = 84;
    dstZ = 26;
    baseX = 160 << 17;
    ((s32 *)j)[1] = 0;
    ((s32 *)j)[2] = 212 << 17;
    if (Event_GetFlag(768))
      open = 212;
    Event_ClearFlag(768);
    facing = 0;
    ((s32 (*)())Func_02000694)(0, 15, 13, 64);
    FieldActor_SetSpritePriority(15, 3);
    for (j = 0; j != 7; j++)
      Actor_SetAnimation(j + 9, 1);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room3) {
    srcX = 28;
    i = 83;
    dstX = 92;
    dstZ = 21;
    baseX = 224 << 17;
    ((s32 *)j)[1] = 0;
    ((s32 *)j)[2] = 172 << 17;
    if (Event_GetFlag(769))
      open = 212;
    Event_ClearFlag(769);
    ((s32 (*)())Func_02000694)(0, 24, 15, 64);
    ((s32 (*)())Func_02000694)(0, 9, 16, 64);
    Actor_SetAnimation(11, 4);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room5) {
    // The native tests and clears flag 770 in both arms.
    if (gMainRoutineWork.entrance <= 8) {
      srcX = 49;
      /* FAKEMATCH(dead): j set again to the position's address, which it
       * already holds; as without it, 25 of 496 instructions differ in tla-en.
       */
      j = (u32)position;
      i = 75;
      dstX = 113;
      dstZ = 13;
      baseX = 198 << 18;
      ((s32 *)j)[1] = 0;
      ((s32 *)j)[2] = 216 << 16;
      if (Event_GetFlag(770))
        open = 212;
      Event_ClearFlag(770);
    } else {
      srcX = 51;
      i = 104;
      dstX = 115;
      dstZ = 42;
      baseX = 206 << 18;
      ((s32 *)j)[1] = 0;
      ((s32 *)j)[2] = 170 << 18;
      if (Event_GetFlag(770))
        open = 212;
      Event_ClearFlag(770);
    }
  }
  Event_PlaySound(181);
  MapGrid_SetShake(0x50000, 0x70000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  rowAbove = i - 1;
  MapGrid_CopyCellsTo(srcX, i, srcX, rowAbove, 3, 4);
  if (open == 0) {
    MapGrid_WaitForShakeToSettle();
    FieldEvent_Wait(20);
  } else {
    MapGrid_SetShake(0x20000, 0x10000, 0x10000);
    options.type = 403;
    options.priority = 2;
    options.palette = 10;
    options.script = gData_0200502c;
    for (i = 0; i != 10; i++) {
      for (j = 0; j != 6; j++) {
        u32 random = Random16Far();

        p = position;
        p[0] = baseX + ((random % 48) << 16);
        Func_0200015c(p[0], p[1], p[2] + (i << 18) + 0x170000, 0, 0x10000, 0,
                      0x330000, &options);
      }
      System_Wait(3);
      MapGrid_CopyCellsTo(75, 64, dstX, dstZ, 3, (i >> 1) + 1);
    }
    for (j = 0; j != 64; j++) {
      REG_BLDALPHA = ((j >> 2) << 8) | (16 - (j >> 2));
      REG_BLDCNT = 0x3f44;
      System_Wait(3);
    }
    MapGrid_CopyCellsTo(78, 64, dstX, dstZ, 3, 5);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room2)
    Actor_Get(15)->priorityFlags |= 1;
  Event_PlaySound(288);
  Event_PlaySound(181);
  MapGrid_SetShake(0x50000, 0x70000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_CopyCellsTo(72, 63, srcX, rowAbove, 3, 4);
  MapGrid_WaitForShakeToSettle();
  Event_PlaySound(584);
  {
    s32 bit = 64;
    bit |= *flags;
    *flags = bit;
  }
  FieldEvent_Wait(20);
  j = (u32)&gMainRoutineWork.currentOwner;
  Actor_Get(*(s32 *)j)->actionFlags &= ~1;
  /* FAKEMATCH(temporary): the position's address held in a pointer, so that
   * the two halfword reads are taken from it as the native does (movs r2, #2;
   * ldrsh r1, [r5, r2]); as position[] and ((s16 *)position)[1], 37 of 496
   * instructions differ in tla-en. */
  p = position;
  p[0] = (leader->x & 0xfff00000) + 0x80000;
  p[1] = leader->y;
  p[2] = (leader->z & 0xfff00000) + 0x80000;
  FieldVector_AddPolarOffset(0x100000, (s32)facing ^ 0x8000, p);
  leader->angle = facing;
  Actor_WalkToAndWait(*(s32 *)j, ((s16 *)p)[1], ((s16 *)p)[5]);
  Actor_Get(*(s32 *)j)->actionFlags |= 1;
  Func_02004d48();
}
