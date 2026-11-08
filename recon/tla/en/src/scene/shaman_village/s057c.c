#include "types.h"
#include "call.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/field_effect.h"

/// A field sprite's byte 9: the top of its first tile and its priority (OAM
/// attribute 2), read as bit-fields.
struct SpriteAttr2High {
  u8 unk00[9];
  u8 tileHigh : 2;
  u8 priority : 2;
};

extern s32 gFrameCount;
extern char ResourceId_WhirlwindTiles;
extern char ResourceId_WhirlwindMap;
extern const s32 gData_02004ae0[];

void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void *Resource_GetTableEntry(s32 id);
void Func_0200479c(void *resource, void *destination);
void Actor_SetSpritePriority(u32 id, s32 variant);
void Import_SoundPlay(s32 cue);
struct Actor *Actor_GetById(u32 id);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
u32 Random16Far(void);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Effect_CircleAndBlink(void);
s32 Camera_FollowActor(s32 id, s32 flag);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_020049f4(void);
void Func_020049e4(s32 a, s32 b);
void Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));
void Func_02004864(s32 a, s32 b, s32 c);
void Func_02004844(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200484c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_0200486c(void);
void Func_020047d4(void);
void Func_020047bc(void);
void Func_0200488c(s32 hue, s32 brightness);
void Func_0200482c(void);
void GameFlag_SetBit(s32 flag);
void Func_02000458(void);
void Func_02000378(void);
void Func_02000420(void);
void Func_02000508(union FieldObject *object);

#define OBJ14_BYTE(offset) (((u8 *)Actor_GetById(14))[offset])

void Func_0200057c(void) {
  u16 *buffer;
  u16 *cell;
  struct Actor *spark;
  u32 i;
  u32 j;

  buffer = Heap_Alloc(0x680);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Func_0200479c(Resource_GetTableEntry((s32)&ResourceId_WhirlwindTiles),
                buffer);
  Dma_Set(buffer, (void *)0x0600e800, SPLIT_CONSTANT(0x84000000, 208), REG_DMA3);
  Dma_Set(buffer + 0x1a0, (void *)0x0600ec00, SPLIT_CONSTANT(0x84000000, 208), REG_DMA3);
  Heap_Release(buffer);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_SetSpritePriority(14, 1);
  do {
    s32 control = SPLIT_CONSTANT(0x400, 13);

    REG_BG0CNT = control;
  } while (0);
  REG_BG0CNT &= ~3;
  REG_BG0CNT |= 1;
  Dma_Fill16(16, (void *)0x06002000, 0x500);
  buffer = Heap_Alloc(576);
  Func_0200479c(Resource_GetTableEntry((s32)&ResourceId_WhirlwindMap), buffer);
  i = 0;
  cell = buffer;
mask:
  {
    s32 t = (*cell + 0xff40) & SPLIT_CONSTANT(0xf00, 255);
    *cell = t | 0x1000;
  }
  i++;
  cell++;
  if (i != 288)
    goto mask;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(0x0600200f + (i << 6)), 0x84000008, REG_DMA3);
  i++;
  cell += 16;
  if (i != 18)
    goto row;
  Heap_Release(buffer);
  Import_SoundPlay(246);
  OBJ14_BYTE(98) = 9;
  OBJ14_BYTE(99) = 1;
  Actor_GetById(14)->update = Func_02000458;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct Actor *)Import_ActorCreate(0x2a0, Actor_GetById(14)->x,
                                                 Actor_GetById(14)->y,
                                                 Actor_GetById(14)->z);
      ((struct SpriteAttr2High *)spark->animation)->priority =
          (((s8 *)Actor_GetById(14)->animation)[9] & 12) >> 2;
      spark->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0x18000;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16Far() & SPLIT_CONSTANT(0xff00, 255);
      Import_ActorSetMode((struct FieldActor *)spark, 7);
      Import_ActorSetScript((struct FieldActor *)spark, gData_02004ae0);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetPartAttribute(spark, 2);
      spark->update = Effect_CircleAndBlink;
    }
    FieldEvent_Wait(1);
  }
  Call2(Camera_FollowActor, gMainRoutineWork.currentOwner, 1);
  Func_020049f4();
  Call2(Func_020049e4, 0x10000, 0x2000);
  Call4(Camera_MoveWithinMapBounds, PIXELS(168), -1, PIXELS(424), 1);
  Func_020049f4();
  Task_Add(Func_02000378, 0xc80);
  Import_SoundPlay(207);
  REG_BLDALPHA = 0xf00;
  REG_BLDCNT = 0x3f41;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  Call3(Func_02004864, 0x10000, 0x10000, 0x10000);
  i = 0;
rise:
  REG_BLDALPHA = ((15 - (i >> 2)) << 8) | (i >> 2);
  OBJ14_BYTE(98) = Random16Far() & 7;
  i++;
  FieldEvent_Wait(1);
  if (i != 64)
    goto rise;
  FieldEvent_Wait(60);
  Call3(Func_02004864, 0x30000, 0x30000, 0x10000);
  REG_BLDALPHA = 0x1000;
  REG_BLDCNT = 0x3f42;
  Func_02004844(2, 83, 36, 64, 16, 16);
  Func_02004844(64, 64, 2, 83, 16, 16);
  REG_BG1CNT &= ~3;
  Task_Add(Func_02000420, 0x480);
  i = 0;
swirl:
  REG_BLDALPHA = ((16 - (i >> 2)) << 8) | (i >> 2);
  OBJ14_BYTE(98) = Random16Far() & 7;
  i++;
  FieldEvent_Wait(1);
  if (i != 68)
    goto swirl;
  {
    s32 width = 13;
    s32 height = 11;

    Func_02004844(45, 90, 4, 19, width, height);
  }
  {
    s32 control = 0x602;

    REG_BG2CNT = control;
  }
  /* FAKEMATCH(one-pass): the empty block ends CSE's run here; with no block, 2
   * of 720 instructions differ in tla-en. */
  do {
  } while (0);
  Dma_Set((void *)0x06002800, (void *)0x06003000, 0x84000200, REG_DMA3);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0xa44;
  i = 0;
stage:
  Call6(Func_02004844, (i - (i / 4) * 4) * 14 + 45, (i / 4) * 12 + 90, 4, 19,
        13, 11);
  j = 0;
frame:
  if ((j & 3) == 0) {
    s32 velocity[3];
    struct EffectOptions options;
    s32 *v = velocity;

    options.priority = 1;
    options.update = Func_02000508;
    v[0] = 0;
    v[2] = -0x20000;
    v[1] = 0;
    {
      s32 x = ((Random16Far() & 127) << 16) + (208 << 15);

      Func_020000b8(x, 0, (472 - (Random16Far() & 127)) << 16, v[0], v[1], v[2],
                    SPLIT_CONSTANT(0x1020000, 1), &options);
    }
  }
  REG_BLDALPHA = ((j >> 2) << 8) | (16 - (j >> 2));
  OBJ14_BYTE(98) = Random16Far() & 7;
  wait(1);
  j++;
  if (j != 64)
    goto frame;
  Dma_Set((void *)0x06002800, (void *)0x06003000, 0x84000200, REG_DMA3);
  REG_BLDALPHA = 16;
  i++;
  if (i != 8)
    goto stage;
  {
    s32 control = 0x60a;

    REG_BG2CNT = control;
  }
  REG_DISPCNT &= ~DISPCNT_BG2_ON;
  {
    s32 width = 9;
    s32 height = 11;

    Func_02004844(47, 19, 38, 19, width, height);
  }
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0x3f42;
  i = 0;
clear:
  REG_BLDALPHA = ((i >> 2) << 8) | (16 - (i >> 2));
  OBJ14_BYTE(98) = Random16Far() & 7;
  i++;
  FieldEvent_Wait(1);
  if (i != 68)
    goto clear;
  Task_RemoveFar(Func_02000420);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 1;
  Func_02004844(36, 64, 2, 83, 16, 16);
  REG_BLDCNT = 0x3f41;
  REG_BLDALPHA = 15;
  Call3(Func_02004864, -1, -1, SPLIT_CONSTANT(0xe600, 102));
  Import_SoundPlay(390);
  OBJ14_BYTE(98) = 6;
  i = 0;
fade:
  {
    u32 q = i >> 2;

    REG_BLDALPHA = (q << 8) | (16 - q);
  }
  FieldEvent_Wait(1);
  i++;
  if (i != 65)
    goto fade;
  Task_RemoveFar(Func_02000378);
  {
    s32 zero = 0;

    OBJ14_BYTE(99) = zero;
    wait(30);
    Actor_GetById(14)->update = (void *)zero;
    Actor_SetPartAttribute(Actor_GetById(14), 6);
    Func_0200486c();
    FieldEvent_Wait(60);
    REG_BLDCNT = zero;
  }
  Func_020047d4();
  Func_020047bc();
  Func_0200488c(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags &= ~32;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Actor_GetById(14)->priorityFlags |= 1;
  REG_DISPCNT |= DISPCNT_BG2_ON;
  Func_0200482c();
  wait(1);
  Call2(Camera_FollowActor, gMainRoutineWork.currentOwner, 1);
  Func_020049f4();
  {
    s32 width = 6;
    s32 height = 29;

    Func_0200484c(1, 5, 9, 1, width, height);
  }
  GameFlag_SetBit(SPLIT_CONSTANT(0x900, 76));
}
