// Draft, differ 215 of 720, skeleton gap 73 (tla-en, 2026-11-01)
// A Shaman Village scene around object 14, in the idiom of Air's Rock's
// Func_02001f7c (games/tla/src/scene/airs_rock/s1f7c.c): loads the whirlwind's pictures into background
// 1, sets the leader and object 14 on their action variants, sound 246 while
// for 45 frames object 14 sheds circling, blinking objects (type 672) every
// fourth frame; then the camera to (168, 424), sound 207 with the screen
// shaking and a blend over 64 frames, the map's cells redrawn in stages
// (Func_02004844) while effects (Func_020000b8) rise and object 14 flickers,
// eight rounds of the background's halves swapped, sound 390, the blend
// cleared, the palette restored (Func_0200488c from the options' hue and
// brightness) and flag 2380 set.
//
// Left: the loops are goto loops as in that twin: the native hoists nothing
// and CSE carries REG_BLDALPHA's address and 16 past them.
// - The blocker is the effect spawn's allocation, and it looks like the
//   compiler's reference weight, not source. The native has the velocity
//   pointer r6, 127 r8, the options pointer r10 and j r9, with r4 free and a
//   72-byte frame. Here the frame loop sits at flow depth 2, so refs weigh 3:
//   127 has 9 refs across 1 call, the options pointer 12 across 2, and
//   CALLER_SAVE_PROFITABLE (4 * calls < refs) sends a pseudo to r4 before
//   r8. That spills sp+60's address to sp+16 and makes the frame 76. Only
//   weight 1 explains the native, and it also orders the velocity pointer
//   above 127. Proof: a real branch entering the stage and frame loops past
//   their headers makes both irreducible (depth 0), and the spawn then
//   allocates as the native, frame 72, reload rotation right from the top.
//   With the three devices below, only one order line differed before those
//   branches. No loop form reaches that without the extra edge: a condition
//   that folds away after flow but before local-alloc was tried ((i << 1) & 1,
//   j & 3 & 4, a u8 shift: CSE folds them first; a test on a pseudo set in an
//   earlier block stays as code and takes r11). This is a compiler question
//   (K1, K3), out of scope here: Pascal's lead is whether the native's flow
//   weighs refs by loop notes, which goto loops lack, where alchemy-gcc weighs
//   CFG natural loops.
// - Three devices are kept out because they cannot reach differ 0 until that
//   is settled. Measured alone from this draft: Call2 and Call3 on
//   Func_020049e4 and Func_02004864 (wrapper), differ 177, gap 45; cell =
//   spark->animation as the twin does, SpriteAttr2High read through cell
//   (temporary), differ 167, gap 65, which also fixes the reload rotation
//   before the spawn; do { } while (0) round BG0CNT's control (one-pass),
//   differ 211, gap 69. All three: differ 125, gap 33.
// - The zero before byte 99 is shared with update and BLDCNT in the native; a
//   local for it, at any scope or type, makes the frame 80 (the first
//   Actor_GetById copy loses r6) and lost 41 on top of the devices.
// - Tried without gain: an options pointer (the pointer form is the native's,
//   but it lands in r8 here and adds 11), velocity set later, a separate z,
//   six forms of the tile mask, a set-once count local (cprop folds it).
// ResourceId_WhirlwindTiles and ResourceId_WhirlwindMap (ids 0x1be, 0x1bf,
// Field_WhirlwindTiles and Field_WhirlwindMap) need names in
// resource_directory.s on adoption: the ids load from the pool, as symbols.
// The stage loop's i - (i / 4) * 4 is the native's form; i % 4 emits a mask.
#include "types.h"
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
/// An object's priority flags read as a whole-byte bit-field.
struct ObjectPriorityByte {
  u8 unk00[35];
  u8 priorityFlags : 8;
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
  Dma_Set(buffer, (void *)0x0600e800, 0x840000d0, REG_DMA3);
  Dma_Set(buffer + 0x1a0, (void *)0x0600ec00, 0x840000d0, REG_DMA3);
  Heap_Release(buffer);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_SetSpritePriority(14, 1);
  {
    s32 control = 0x40d;

    REG_BG0CNT = control;
  }
  REG_BG0CNT &= ~3;
  REG_BG0CNT |= 1;
  Dma_Fill16(16, (void *)0x06002000, 0x500);
  buffer = Heap_Alloc(576);
  Func_0200479c(Resource_GetTableEntry((s32)&ResourceId_WhirlwindMap), buffer);
  i = 0;
  cell = buffer;
mask: {
  s32 t = (*cell + 0xff40) & 0xfff;
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
                                 Actor_GetById(14)->y, Actor_GetById(14)->z);
      {
        s32 priority = ((s8 *)Actor_GetById(14)->animation)[9] & 12;
        struct SpriteAttr2High *sprite = spark->animation;

        sprite->priority = priority >> 2;
      }
      ((struct ObjectPriorityByte *)spark)->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0x18000;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16Far() & 0xffff;
      Import_ActorSetMode((struct FieldActor *)spark, 7);
      Import_ActorSetScript((struct FieldActor *)spark, gData_02004ae0);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetPartAttribute(spark, 2);
      spark->update = Effect_CircleAndBlink;
    }
    FieldEvent_Wait(1);
  }
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_020049f4();
  Func_020049e4(0x10000, 0x2000);
  Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(424), 1);
  Func_020049f4();
  Task_Add(Func_02000378, 0xc80);
  Import_SoundPlay(207);
  REG_BLDALPHA = 0xf00;
  REG_BLDCNT = 0x3f41;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  Func_02004864(0x10000, 0x10000, 0x10000);
  i = 0;
rise:
  REG_BLDALPHA = ((15 - (i >> 2)) << 8) | (i >> 2);
  OBJ14_BYTE(98) = Random16Far() & 7;
  i++;
  FieldEvent_Wait(1);
  if (i != 64)
    goto rise;
  FieldEvent_Wait(60);
  Func_02004864(0x30000, 0x30000, 0x10000);
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
  Dma_Set((void *)0x06002800, (void *)0x06003000, 0x84000200, REG_DMA3);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0xa44;
  i = 0;
stage:
  Func_02004844((i - (i / 4) * 4) * 14 + 45, (i / 4) * 12 + 90, 4, 19, 13,
                11);
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

      Func_020000b8(x, 0, (472 - (Random16Far() & 127)) << 16, v[0],
                    v[1], v[2], 0x1020001, &options);
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
  Func_02004864(-1, -1, 0xe666);
  Import_SoundPlay(390);
  OBJ14_BYTE(98) = 6;
  i = 0;
fade:
  REG_BLDALPHA = ((i >> 2) << 8) | (16 - (i >> 2));
  i++;
  FieldEvent_Wait(1);
  if (i != 65)
    goto fade;
  Task_RemoveFar(Func_02000378);
  OBJ14_BYTE(99) = 0;
  wait(30);
  Actor_GetById(14)->update = 0;
  Actor_SetPartAttribute(Actor_GetById(14), 6);
  Func_0200486c();
  FieldEvent_Wait(60);
  REG_BLDCNT = 0;
  Func_020047d4();
  Func_020047bc();
  Func_0200488c(gMainRoutineWork.paletteGlow[0], gMainRoutineWork.paletteGlow[1]);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags &= ~32;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Actor_GetById(14)->priorityFlags |= 1;
  REG_DISPCNT |= DISPCNT_BG2_ON;
  Func_0200482c();
  wait(1);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_020049f4();
  {
    s32 width = 6;
    s32 height = 29;

    Func_0200484c(1, 5, 9, 1, width, height);
  }
  GameFlag_SetBit(2380);
}
