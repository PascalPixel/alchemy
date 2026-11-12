/// @file
/// Scene code of overlay 0x655, in ROM order, from s3284's offset up to the next object the overlay links.

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"
#include "task.h"
#include "event_inline.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "event_work.h"
#include "field_effect.h"

extern const u8 gData_02003f80[];
extern const u8 gData_02003fc4[];
extern const u8 gData_02003fc8[];
extern s32 gData_02005678;
extern s32 gData_0200567c;

void Func_020001d0(const void *data);
void Func_02000c64(const void *data);
void Func_020013b0(void);
void Func_0200172c(void);
void Func_02001930(void);
void Func_02001ee8(void);
void Func_02001fc4(void);
void Func_02003cc0(s32 id);
void Func_02003d90(void);
s32 Func_02003d98(void);
s32 Func_02003da0(s32 a, s32 b, s32 c);
void Func_02003da8(s32 a, s32 b, s32 c, s32 d);
s32 Func_02003db0(void);
void Func_02003db8(const void *data);

// The room entry written in the scene layer of Func_02003164 and Func_0200390c:
// sets actors' variants, links, cells and flags for the room.
void Func_02003284(void) {
  struct Actor *object;
  s32 zero, value;

  if (GameFlag_Test(265) == 0) {
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  }
  Actor_SetSpritePriority(8, 1);
  Actor_SetSpritePriority(9, 1);
  Actor_SetSpritePriority(10, 1);
  Actor_SetSpritePriority(11, 1);
  /* FAKEMATCH(dead): an empty once-only block ends the argument loads' tie; as
   * no block, 2 of 276 instructions differ in tla-en. */
  do {
  } while (0);
  Func_02003d98();
  Func_02003da0(0, 8, 9);
  Func_02003da0(1, 10, 11);
  Actor_SetSpritePriority(12, 1);
  Actor_SetSpritePriority(13, 1);
  Func_02003d90();
  Func_02003da8(0, 520, 12, 13);
  Func_02003db8(gData_02003fc4);
  if (Actor_GetById(19)->y > (s32)0xfff80000)
    Actor_SetSpritePriority(19, 1);
  else
    Actor_SetSpritePriority(19, 2);
  Func_02003cc0(20);
  Actor_GetById(20)->priorityFlags |= 32;
  Actor_SetSpritePriority(20, 2);
  if (GameFlag_Test(0x841) != 0)
    Func_0200172c();
  else
    /* FAKEMATCH(wrapper): forwarded, so each call builds its own constants; as direct
     * calls, 8 of 276 instructions differ in tla-en. */
    MapGrid_CopyCellAttributes(38, 32, 1, 1, 38, 33);
  Actor_GetById(14)->priorityFlags &= ~1;
  Actor_GetById(15)->priorityFlags &= ~1;
  Actor_GetById(16)->priorityFlags &= ~1;
  Actor_GetById(17)->priorityFlags &= ~1;
  Func_02000c64(gData_02003fc8);
  value = Actor_GetById(17)->x;
  zero = 0;
  if ((value >> 20) == 54) {
    struct MapCell *cell = Ram_HeapSlots->mapWork->layers[0].cells;

    cell += Func_02003db0();
    cell->attributeB &= 0x7f;
    cell->collisionCode = 11;
  }
  Actor_SetSpritePriority(14, 1);
  Actor_SetSpritePriority(15, 1);
  Func_02001930();
  if (GameFlag_Test(0x86e) != 0) {
    s32 angle = 0x4000;

    /* FAKEMATCH(wrapper): forwarded, so the angle loads ahead of the coordinates; as
     * direct calls, 4 of 276 instructions differ in tla-en. */
    Actor_SetPositionAndFacing(25, 212 << 17, 208 << 15, angle);
  }
  Actor_SetModeById(18, 3);
  if (GameFlag_Test(0x86f) != 0) {
    object = Actor_GetById(18);
    MapGrid_CopyCellAttributes(27, 8, 1, 1, 27, 9);
    Actor_SetModeById(18, 4);
    object->priorityFlags |= 2;
    *((u8 *)object + 89) &= ~1;
    GameFlag_SetBit(517);
    GameFlag_SetBit(577);
    Map_CopyCellPictureIndicesRect(69, 51, 92, 11, 1, 1);
    {
      s32 angle = 0x4000;

      Actor_SetPositionAndFacing(25, 154 << 18, 140 << 17, angle);
    }
  }
  if (GameFlag_Test(0x870) != 0) {
    GameFlag_SetBit(528);
    GameFlag_SetBit(534);
    Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
  }
  Func_020001d0(gData_02003f80);
  Func_020013b0();
  Func_02001fc4();
  {
    struct Actor *last = Actor_GetById(27);

    *((u8 *)last + 85) = zero;
    last->y = zero;
  }
  gData_0200567c = zero;
  gData_02005678 = zero;
  Task_Add(Func_02001ee8, TASK_PASS_VBLANK);
}

extern const u16 gData_02004276[];

void Func_0200353c(void) {
  /* FAKEMATCH(pin): flag in r5, which leaves r6 to the entry; as a plain
   * local, 61 of 79 instructions differ in tla-en. */
  register s32 flag asm("r5") = 800;
  s32 offset = 0;

  while (1) {
    if (GameFlag_Test(flag)) {
      u16 *entry = (u16 *)((u8 *)gData_02004276 + offset);
      s32 y;
      s32 x;
      s32 one;

      flag -= 800;
      x = *entry++;
      y = *entry++;
      one = 1;
      MapGrid_CopyCellsTo(3, 8, x, y, one, one);
      x = entry[0];
      y = entry[1];
      MapGrid_CopyCellsTo(67, 3, x, y, one, 5);
      if (flag == 1)
        MapGrid_CopyCellsTo(67, 4, 69, 3, one, one);
      x -= 64;
      y += 66;
      MapGrid_CopyCellsTo(3, 69, x, y, one, 4);
      return;
    }
    if (flag == 863)
      return;
    offset += 8;
    flag++;
  }
}

void Func_020035ec(void) {
  /* FAKEMATCH(pin): flag in r5, which leaves r6 to the entry; as a plain
   * local, 61 of 79 instructions differ in tla-en. */
  register s32 flag asm("r5") = 800;
  s32 offset = 0;

  while (1) {
    if (GameFlag_Test(flag)) {
      u16 *entry = (u16 *)((u8 *)gData_02004276 + offset);
      s32 y;
      s32 x;
      s32 one;

      flag -= 800;
      x = *entry++;
      y = *entry++;
      one = 1;
      MapGrid_CopyCellsTo(13, 5, x, y, one, one);
      x = entry[0];
      y = entry[1];
      MapGrid_CopyCellsTo(77, 0, x, y, one, 5);
      if (flag == 1)
        MapGrid_CopyCellsTo(67, 4, 69, 3, one, one);
      x -= 64;
      y += 65;
      MapGrid_CopyCellsTo(13, 65, x, y, one, 4);
      return;
    }
    if (flag == 863)
      return;
    offset += 8;
    flag++;
  }
}

void Func_02001804(void);
void Func_02001fdc(void);
void Func_02003764(void);

void Func_0200369c(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Func_02001fdc();
  if (GameFlag_Test(0x86c) != 0) {
    Map_CopyCellPictureIndicesRect(3, 8, 30, 33, 1, 1);
    Map_CopyCellPictureIndicesRect(67, 3, 94, 28, 1, 5);
    Map_CopyCellPictureIndicesRect(3, 69, 30, 93, 1, 4);
  }
  {
    s32 entrance = *(u16 *)&gMainRoutineWork.entrance;

    if ((u16)(entrance - 11) <= 28) {
      Func_02001804();
      entrance <<= 16;
      entrance >>= 16;
      entrance += 789;
      GameFlag_SetBit(entrance);
    }
  }
  Func_0200353c();
  if (GameFlag_Test(265) == 0) {
    if (gMainRoutineWork.entrance > 10) {
      if (gMainRoutineWork.entrance <= 39)
        Func_02003764();
    }
  }
}

extern u8 gMapShapeGrid[];

s32 Math_Cosine(s32 angle);
void ObjectVisual_CopyAttributes(s32 id, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200056c(s32 id);
void Func_020006ec(struct Actor *object, struct Actor *other);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000c4c(s32 level, s32 layer);
void Func_02003ba0(struct Actor *object, s32 mode);
void Func_02003ba8(struct Actor *object, const void *script);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003c78(s32 id, s32 target);
s32 Func_02003d20(void);
s32 Func_02003d80(void *sprite, s32 frame);

s32 Func_02003dd0(s32 layer, s32 shape, s32 height);
struct Actor *Func_02003dd8(s32 *position, s32 mode);
void Effect_AdvanceMotionAtLeaderPriority(struct Actor *object);

/// The effect fields of an Actor, as field_effect.h names them.
struct EffectView {
  u8 unk00[0x30];
  s32 scaleRateX;
  s32 scaleRateY;
  u8 unk38[0x0c];
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  struct FieldSprite *sprite;
  u8 unk54[0x10];
  u16 spin;
  u8 unk66[6];
  void (*update)(struct Actor *object);
};

void Func_02000fc4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_02003bc8(void);

void Func_02003764(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  u32 i;
  struct EffectOptions *opts;
  s32 spawn;

  Func_02003c28();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02003bc8();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(204);
  leader->motionFlags = 3;
  Event_Wait(24);
  opts = &options;
  opts->palette = 7;
  opts->update = Effect_AdvanceMotion;
  opts->startScaleX = 0xa3d7;
  opts->startScaleY = 0xa3d7;
  opts = &options;
  spawn = 1;
  i = 0;
loop:
  {
    s32 angle;
    s32 velocity[3];
    s32 cosine;

    angle = i << 12;
    cosine = Math_Cosine(angle);
    if (spawn) {
      velocity[0] = cosine;
      velocity[1] = 0;
      velocity[2] = Math_Sine(angle);
      velocity[0] += velocity[0] >> 1;
      velocity[0] += ((Random16Far() * 3) << 14) >> 16;
      velocity[0] += -0x6000;
      velocity[2] += (((Random16Far() * 3) << 13) >> 16) + -0x3000;
      Func_02000fc4(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                    velocity[2], 0x01090001, opts);
    }
    i++;
  }
  if (i <= 16)
    goto loop;
  Sound_Play(188);
  Func_080d489c(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  Map_WaitForShakeToSettle();
  Func_080d489c(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  Event_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Func_02003c30();
}
