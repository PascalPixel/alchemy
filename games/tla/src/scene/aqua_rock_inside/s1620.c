/// @file
/// Scene code of overlay 0x67c, in ROM order, from s1620's offset up to the
/// next object the overlay links.

#include "types.h"
#include "system.h"
#include "actor.h"
#include "system/io_reg.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "main_routine_work.h"

union ActorView {
  struct Actor actor;
  struct {
    u8 unk00[0x62];
    u8 unk62;
    u8 unk63;
  } bytes;
};
extern const u8 gData_02004034[], gData_02003fe8[], gData_02004004[],
    gData_02004058[], gData_02004014[], gData_0200407c[], gData_020040ac[];
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Func_02003cc4(struct Actor *object, s32 mode);
void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);
void Animation_SetStateFlags(struct Actor *object, u32 value);
void Func_02003ccc(struct Actor *object, const void *data);
void Func_02001598(void);
void Func_02003ebc(void);

void Func_02001620(s32 firstId, s32 secondId) {
  struct Actor *first = Actor_GetById(firstId);
  struct Actor *second = Actor_GetById(secondId);
  struct Actor *object;
  struct Actor *spark;
  struct Actor *riser;
  s32 i;
  u16 noMotion;
  s32 probe;
  const u8 *script;
  // Unused: gives the stack frame its 12 bytes.
  s32 unused[3];

  script = gData_02004058;
  Sound_Play(78);
  Event_Wait(30);
  FieldCamera_SetSpeed(0x8000, 0x1000);
  Camera_MoveWithinMapBounds((first->x >> 16) << 16, (first->y >> 16) << 16,
                             (first->z >> 16) << 16, 1);
  Sound_Play(141);
  /* FAKEMATCH(wrapper): forwarded, so each argument builds its own 0x20000; as
   * a direct call, 17 of 402 instructions differ in tla-en. */
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  BattleEffect_StartBufferInterpolation(60);
  first->motionFlags = 0;
  first->acceleration = 0x3333;
  first->speedLimit = 0x3333;
  Actor_SetMovementTypeById(firstId, gData_02004034);
  Sound_Play(194);
  Event_Wait(45);
  Actor_ConfigureSpriteColor(firstId, 256);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  i = 0;
burst:
  object = ((void *)Import_ActorCreate(672, first->x, first->y, first->z));
  Sound_Play(246);
  object->x += ((Random16Far() & 31) - 16) << 16;
  object->y += ((Random16Far() & 15) - 8) << 16;
  object->speedLimit = 0x80000;
  object->acceleration = 0x3333;
  object->motionFlags = 0;
  Actor_SetPartAttribute(object, 2);
  Scene_SetActorSpritePriority(object, 1);
  Func_02003cc4(object, 7);
  Actor_SetSingleChildFlags(object, 0);
  Animation_SetStateFlags(object, 1);
  Func_02003ccc(object, gData_02003fe8);
  REG_BLDALPHA = 0x1004;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  REG_BLDALPHA = 0x1010;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  i++;
  if (i != 16)
    goto burst;
  REG_BLDALPHA = 0x1008;
  wait(30);
  i = 0;
  /* FAKEMATCH(dead): the never-taken jump into the spiral keeps its refs at
   * weight 1; as the spiral alone, 174 of 402 instructions differ in tla-en. */
  probe = second->unk57;
  if (probe & 0x100) {
    wait(1);
    goto tail;
  }
spiral:
  /* FAKEMATCH(wrapper): forwarded, so second->z takes r0 for its reload; as a
   * direct call, 105 of 402 instructions differ in tla-en. */
  spark = FieldActor_Create(672, second->x, second->y, second->z);
  Sound_Play(195);
  spark->timer = (Random16Far() & 0x7fff) + 0x8000;
  spark->unk66 = (((i & 1) << 1) - 1) << ((Random16Far() & 3) + 9);
  noMotion = 0;
  spark->motionFlags = noMotion;
  spark->unk4c = 0x7a0000;
  spark->watchedObject = second;
  ((union ActorView *)spark)->bytes.unk62 = ((16 - i) << 3) + 15;
  Actor_SetPartAttribute(spark, 2);
  Scene_SetActorSpritePriority(spark, 1);
  Func_02003cc4(spark, 7);
  Actor_SetSingleChildFlags(spark, 0);
  Animation_SetStateFlags(spark, 1);
  /* FAKEMATCH(wrapper): forwarded for its argument order; as a direct call, 2
   * of 402 instructions differ in tla-en. */
  FieldVector_AddPolarOffset(spark->unk4c, spark->timer, &spark->x);
  Func_02003ccc(spark, gData_02004004);
  if (i == 3) {
    /* FAKEMATCH(wrapper): forwarded for its argument order; as a direct call, 2
     * of 402 instructions differ in tla-en. */
    Actor_SetSpriteColorById(secondId, 256);
    second->motionFlags = noMotion;
    second->acceleration = 0x3333;
    second->speedLimit = 0x3333;
    Actor_SetMovementTypeById(secondId, script);
  }
tail:
  wait(8);
  i++;
  if (i != 16)
    goto spiral;
  Sound_Play(220);
  wait(16);
  i = 2;
column:
  /* FAKEMATCH(wrapper): forwarded, so second->z takes r0 for its reload; as a
   * direct call, 116 of 402 instructions differ in tla-en. */
  riser = FieldActor_Create(672, second->x, second->y + i * 0x10000 - 0x80000,
                            second->z);
  riser->timer = Random16Far();
  riser->unk66 = 0x800;
  ((union ActorView *)riser)->bytes.unk62 = i;
  ((union ActorView *)riser)->bytes.unk63 = 1;
  riser->bounce = riser->x;
  riser->unk4c = 0;
  riser->gravity = riser->z;
  riser->watchedObject = second;
  riser->motionFlags = (u16)0;
  riser->priorityFlags = 1;
  Actor_SetPartAttribute(riser, 2);
  Func_02003cc4(riser, 7);
  Actor_SetSingleChildFlags(riser, 0);
  Animation_SetStateFlags(riser, 1);
  Func_02003ccc(riser, gData_02004014);
  i += 2;
  if (i != 32)
    goto column;
  Sound_Play(220);
  wait(50);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_ConfigureSpriteColor(firstId, 0);
  Actor_ConfigureSpriteColor(secondId, 0);
  Sound_Play(574);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(80);
  Actor_SetMovementTypeById(firstId, gData_0200407c);
  Actor_SetMovementTypeById(secondId, gData_020040ac);
  second->update = Func_02001598;
  wait(120);
  second->update = 0;
  Sound_Play(390);
  Func_02003ebc();
  wait(1);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_0200124c();
void Func_02003d7c();
void Func_02003d84();
void Func_02003de4();
void Party_LearnAbility();
extern u8 MsgCheckedTheTabletIt[];
extern u8 MsgWhatWentWrong[];

void Func_02001a1c(void) {
  s32 t4, t59;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)&MsgCheckedTheTabletIt, 1);
  t4 = Event_GetFlag(0x9ae);
  if (t4 == 0)
    goto L_02009a46;
  Func_02003d84();
  goto L_02009c56;
L_02009a46:
  FieldEvent_SetMessageId((s32)&MsgCheckedTheTabletIt);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), s16, 0) =
      (FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) + 1);
  Func_02003de4(7, gMainRoutineWork.currentOwner);
  FieldEvent_Wait(1);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 256, 184);
  Actor_WalkTo(7, 274, 184);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  FieldActor_WaitForMoveById(7);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  FieldEvent_Wait(5);
  Event_ShowMessage(7, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(30);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 184);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(7, 3);
  FieldEvent_Wait(30);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_WalkToAndWait(7, 264, 168);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Message_ShowCentered(((s32)&MsgCheckedTheTabletIt + 2), 1);
  Func_02001620(14, 7);
  Party_LearnAbility(7, 138);
  Event_SetFlag(0x9ae);
  Event_PlaySound(107);
  FieldEvent_Wait(30);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  Actor_ShowEmote(7, 0x100, 0);
  Func_0200124c();
  Event_PlaySound(0x186);
  FieldEvent_Wait(30);
  FieldEvent_SetMessageId((s32)&MsgWhatWentWrong);
  Actor_ShowEmote(7, 0x105, 20);
  Actor_FaceEachOther(gMainRoutineWork.currentOwner, 7, 0);
  FieldEvent_Wait(10);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(7, 0x100, 20);
  Event_ShowMessage(7, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetAnimation(7, 2);
  t59 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if (t59 == 0)
    goto L_02009c42;
  Actor_SetDestination(7, FIELD_AT(t59, s16, 10), FIELD_AT(t59, s16, 18));
L_02009c42:
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
  Func_02003d84();
L_02009c56:
}

#include "local.h"
#include "scene_ids.h"
#include "call.h"

void Func_02001c68(void) {
  if (Event_GetFlag(0x9ae)) {
    Func_02003d7c();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgCheckedTheTabletIt, 1);
    Func_02003d84();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 152);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 184);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 184);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02001a1c();
  }
}

void Func_02001cd8(void) {
  if (Event_GetFlag(0x9ae)) {
    Func_02003d7c();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgCheckedTheTabletIt, 1);
    Func_02003d84();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 280, 184);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 184);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02001a1c();
  }
}

void Func_02001d40(void) {
  if (Event_GetFlag(0x9ae)) {
    Func_02003d7c();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgCheckedTheTabletIt, 1);
    Func_02003d84();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 184);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 184);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02001a1c();
  }
}

extern const u8 gData_02004c70[];
extern const u8 gData_02004c7c[];
extern const u8 gData_02004d88[];
extern const u8 gData_02004e48[];
extern const u8 gData_02004ed8[];
extern const u8 gData_02004fb0[];
extern const u8 gData_0200507c[];
extern const u8 gData_020051b4[];
extern const u8 gData_020051fc[];
extern const u8 gData_02005268[];
extern const u8 gData_020052f8[];

const u8 *Func_02001da8(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov67cRoom1)
    return gData_02004c7c;
  if (scene == (s32)&SceneId_Ov67cRoom2)
    return gData_02004d88;
  if (scene == (s32)&SceneId_Ov67cRoom3)
    return gData_02004e48;
  if (scene == (s32)&SceneId_Ov67cRoom4)
    return gData_02004ed8;
  if (scene == (s32)&SceneId_Ov67cRoom5)
    return gData_02004fb0;
  if (scene == (s32)&SceneId_Ov67cRoom6)
    return gData_0200507c;
  if (scene == (s32)&SceneId_Ov67cRoom7)
    return gData_020051b4;
  if (scene == (s32)&SceneId_Ov67cRoom8)
    return gData_020051fc;
  if (scene == (s32)&SceneId_Ov67cRoom9)
    return gData_02005268;
  if (scene == (s32)&SceneId_Ov67cRoom10)
    return gData_020052f8;
  return gData_02004c70;
}

void Func_02001e78(s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object != 0) {
    u8 *meta = ResourceMetadata_Register(object->sprite, 0x17d);

    meta[5] = 0;
    meta[6] = 0;
    Func_02003cc4(object, 2);
  }
}

void Func_02001ea0(struct Actor *object) {
  s32 phase = gFrameCount & 15;

  if (phase == 0) {
    struct Actor *child =
        Import_ActorCreate_(0x10d, object->x, object->y, object->z);

    if (child != 0) {
      struct FieldSprite *sprite = child->sprite;

      Func_02003ccc(child, gData_020040dc);
      child->motionFlags = 0;
      child->terrainId = 1;
      if (sprite != 0) {
        Animation_ApplyChildArgument(sprite, 2);
        sprite->nextFrame = 0;
        sprite->rotation = 0x4000;
        sprite->priority = 2;
      }
    }
  }
}

void Func_02001f04(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u8 *flags = &object->motionFlags;
  s32 saved = *flags;
  s32 speed;
  s32 i;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Animation_ApplyChildValues(object, 9);
  *flags = 0;
  object->y = 0x500000;
  speed = 0x30000;
  // A goto loop: the ROM rebuilds the turn step on every pass, which a for loop
  // hoists (differ 43).
  i = 31;
fall:
  object->y -= speed;
  object->facing += 0x2000;
  speed += -0xccc;
  Actor_SetSingleChildFlags(object, 0);
  System_Wait(1);
  i--;
  if (i >= 0)
    goto fall;
  Func_02002bd4(object);
  Animation_ApplyChildValues(object, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
  *flags = saved;
  object->velocityY = 0x80000;
  Event_PlaySound(215);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 168, 200);
  Func_02003d84();
}

void Func_02001fcc(void) {
  s32 counter = Func_02003bcc();

  switch (counter) {
  case 0:
    Func_02003bb8(0, 1);
    Func_02003bb8(1, 1);
    Func_02003ecc(170);
    break;
  case 0x34bbf:
    Func_02003bb8(0, 0);
    Func_02003bb8(1, 0);
    Func_02003ecc(-1);
    break;
  }
  Func_02003bd8(0x34bc0);
}

extern const u8 gData_020040f4[];
extern const u8 gData_02004102[];
extern const u8 gData_02004106[];
extern const u8 gData_02004114[];

void Func_02002ffc(void);
// Returns what its closing Task_Add call leaves in r0.
s32 Func_02003ab0(const u8 *a, const u8 *b, void (*handler)(void));
void Func_0200107c(s32 a, s32 b);

/// Aqua Rock's entry setup: in each room it shows the statues already filled by
/// their flags (or dims those still empty), opens what the story has opened and
/// arms the room's water and cell handlers by entrance.
s32 Func_02002020(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *object;

  FIELD_AT(Ram_HeapSlots->eventWork, s32, 428) = 0x204;
  if (scene == (s32)&SceneId_Ov67cRoom1) {
    if (GameFlag_Test(0x230)) {
      Call1((void (*)())Func_02001e78, 8);
      Actor_SetAnimation(8, 3);
    } else {
      Actor_SetSpriteColor(Actor_GetById(8), 6);
    }
    if (GameFlag_Test(0x231)) {
      Call1((void (*)())Func_02001e78, 9);
      Actor_SetAnimation(9, 3);
    } else {
      Actor_SetSpriteColor(Actor_GetById(9), 6);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom2) {
    if (GameFlag_Test(0x240)) {
      MapGrid_CopyCellsLayered(118, 15, 5, 32, 110, 15);
      MapGrid_CopyCellAttributes(59, 16, 5, 32, 46, 16);
      Actor_SetSpriteColor(Actor_GetById(8), 6);
    } else {
      Call1((void (*)())Func_02001e78, 8);
      Actor_SetAnimation(8, 3);
    }
    object = Actor_GetById(9);
    object->motionFlags = 0;
    if (GameFlag_Test(0x95c)) {
      Actor_SetPosition(9, PIXELS(632), PIXELS(152));
      MapGrid_CopyCellAttributes(41, 9, 1, 1, 39, 9);
      object->y = 0;
      object->terrainHeight = 0;
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom3) {
    if (entrance == 7)
      Func_02002ffc();
  } else if (scene == (s32)&SceneId_Ov67cRoom4) {
    Call1((void (*)())Func_02001e78, 10);
    Actor_SetAnimation(10, 3);
    Call1((void (*)())Func_02001e78, 11);
    Actor_SetAnimation(11, 3);
    if ((u32)(entrance - 6) <= 1) {
      if (GameFlag_Test(0x249)) {
        Actor_SetSpriteColor(Actor_GetById(9), 5);
        Call1((void (*)())Func_02001e78, 9);
        Actor_SetAnimation(9, 3);
        Func_02000b5c();
      } else {
        Actor_SetSpriteColor(Actor_GetById(9), 6);
      }
    }
    FIELD_AT(Actor_GetById(13), u8, 98) = 1;
    FIELD_AT(Actor_GetById(14), u8, 98) = 1;
    object = Actor_GetById(64);
    if (object != 0) {
      object->motionFlags = 0;
      object->terrainHeight = -0x200000;
      object->y = -0x200000;
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom5) {
    if ((u32)(entrance - 5) <= 1) {
      Func_02003ab0(gData_020040f4, gData_02004102, Func_02001fcc);
      Func_02003ecc(170);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom6) {
    if (entrance == 5 || entrance == 7) {
      Func_02003ab0(gData_02004106, gData_02004114, Func_02001fcc);
      Func_02003ecc(170);
      Func_0200107c(0, 10);
    }
    if (!GameFlag_Test(0x238)) {
      Func_02001e78(9);
      Actor_SetAnimation(9, 3);
    }
    object = Actor_GetById(12);
    Actor_SetSingleChildFlags(object, 0);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    object->update = (void (*)(void))Func_02001ea0;
    if (!GameFlag_Test(0x109) && entrance == 13)
      Func_02001f04();
  } else if (scene == (s32)&SceneId_Ov67cRoom9) {
    if (GameFlag_Test(0x24b)) {
      Actor_SetSpriteColor(Actor_GetById(9), 5);
      Func_02001e78(9);
      Actor_SetAnimation(9, 3);
    } else {
      Actor_SetSpriteColor(Actor_GetById(9), 6);
    }
    FIELD_AT(Actor_GetById(10), u8, 98) = 1;
    FIELD_AT(Actor_GetById(11), u8, 98) = 1;
    FIELD_AT(Actor_GetById(14), u8, 98) = 1;
    FIELD_AT(Actor_GetById(16), u8, 98) = 1;
    FIELD_AT(Actor_GetById(17), u8, 98) = 1;
    FIELD_AT(Actor_GetById(18), u8, 98) = 1;
  }
  return 0;
}

#include "iwram_call.h"
#include "text/msg_ids.h"
#include "field.h"

struct MapBits {
  u8 unk00[23];
  u8 low : 2;
  u8 mode : 2;
};

// The original keeps the masked BG1CNT in a stack slot it never reads again:
// control is volatile, and the register is written in a once-only block.
#define BG1_SET_PRIORITY_1()                                                   \
  {                                                                            \
    u16 value;                                                                 \
                                                                               \
    value = (control = REG_BG1CNT & 0xfffc);                                   \
    do {                                                                       \
      REG_BG1CNT = value | 1;                                                  \
    } while (0);                                                               \
  }

void Func_02003278(void);
void Func_020032d4(u32 first, s32 level, s32 baseLevel);
void Func_0200349c(u32 index, s32 height);
void Func_02003ea4();
void Func_020007a8(void);
void Func_02003420(s32 id, s32 kind, s32 height);
void Func_02000b84(void);
void Func_02000ad8(void);
void Func_02003edc();
void Func_02003ee4();

/// Aqua Rock's map preparation: for each room and entrance it sets the water
/// level, places the statues and boulders the flags have moved, opens or
/// closes the cells around them, and in the rooms whose water drains it shows
/// the drained state once its flag is set. Room 6 clears its actors' x and z
/// where the other rooms clear terrainHeight and y.
s32 Func_02002340(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  volatile u16 control;
  // i1, i3, i4, i6, i7, i12, i14, i17 and b each hold one constant for their
  // whole life, i2 holds a flag test and the others each count one loop.
  s32 i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, i16,
      i17;
  struct Actor *actor;
  s32 a, b;
  u8 *map;

  ((struct MapBits *)Ram_HeapSlots->mapWork)->mode = 1;
  Func_02003278();
  if (scene == (s32)&SceneId_Ov67cRoom1) {
    if ((u32)(entrance - 3) <= 1 || entrance == 6 || entrance == 8) {
      Func_020032d4(33, 0, -2);
    } else if (entrance == 5 || entrance == 7) {
      actor = Actor_GetById(12);
      i1 = 0;
      actor->motionFlags = i1;
      actor->terrainHeight = i1;
      actor->y = i1;
      Call2((void (*)())Func_02003cc4, (s32)actor, 0);
      actor = Actor_GetById(13);
      actor->motionFlags = i1;
      actor->terrainHeight = i1;
      actor->y = i1;
      i2 = GameFlag_Test(0x230);
      if (i2 == 0) {
        MapGrid_CopyCellsLayered(104, 80, 16, 10, 104, 40);
        MapGrid_CopyCellsLayered(110, 116, 18, 12, 39, 105);
        // i2 is the flag test's 0.
        MapGrid_CopyCellAttributes(64, 32, 64, 32, i2, 32);
        Func_020032d4(33, 0, 0);
      } else {
        Call3((void (*)())Func_020032d4, 33, 4, 0);
        actor = Actor_GetById(12);
        Call2((void (*)())Func_0200349c, (actor->x >> 20) - 42, 0);
      }
      if (GameFlag_Test(0x250)) {
        MapGrid_CopyCellAttributes(41, 42, 1, 1, 43, 44);
      }
    } else if ((u32)(entrance - 9) <= 1) {
      a = GameFlag_Test(0x231);
      if (a == 0) {
        i3 = 32;
        MapGrid_CopyCellsLayered(77, 80, 23, 20, 77, i3);
        MapGrid_CopyCellsLayered(112, 96, 16, 20, 12, 98);
        // a is the flag test's 0.
        MapGrid_CopyCellAttributes(64, 32, 64, 32, a, i3);
        Func_020032d4(33, 0, 0);
      } else {
        Func_020032d4(33, 4, 0);
      }
    }
    if (GameFlag_Test(0x95a)) {
      Actor_SetPosition(10, PIXELS(184), PIXELS(200));
      i4 = 12;
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 9, i4);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 13, i4);
    }
    if (GameFlag_Test(0x95b)) {
      Actor_SetPosition(11, PIXELS(824), PIXELS(216));
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom2) {
    if ((u32)(entrance - 1) <= 1) {
      Func_020032d4(33, 0, -2);
    }
    actor = Actor_GetById(8);
    actor->motionFlags = 0;
    actor->terrainHeight = 0;
    actor->y = 0;
  } else if (scene == (s32)&SceneId_Ov67cRoom3) {
    Call1((void (*)())Func_02003ea4, 0);
    Call2((void (*)())Actor_SetSingleChildFlags, (s32)Actor_GetById(9), 0);
    Call2((void (*)())Actor_SetSingleChildFlags, (s32)Actor_GetById(10), 0);
    Func_020032d4(33, 8, -2);
    Call3((void (*)())Func_020032d4, 40, 8, 8);
    if (GameFlag_Test(0xa66)) {
      Func_02003628();
      Value4((s32 (*)())Func_02003688, 16, 4, 16, 2);
      Value4((s32 (*)())Func_02003688, 22, 4, 16, 0);
      for (i5 = 0; i5 < 6; i5++)
        Func_020036b8(33, 55);
    }
    if (GameFlag_Test(0x95d)) {
      MapGrid_CopyCellsLayered(75, 96, 7, 13, 80, 27);
      i6 = 16;
      b = 31;
      MapGrid_CopyCellsLayered(91, 96, 7, 10, i6, b);
      MapGrid_CopyCellsLayered(99, 96, 7, 14, i6, 91);
      MapGrid_CopyCellAttributes(43, 0, 5, 9, 17, b);
      Func_020007a8();
    } else {
      MapGrid_CopyCellAttributes(17, 32, 5, 1, 17, 31);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom4) {
    Call1((void (*)())Func_02003ea4, 0);
    if ((u32)(entrance - 6) <= 1)
      goto statues;
    goto no_statues;
  statues:
    {
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
      Value3((s32 (*)())Func_02003420, 12, 5, 4);
      Value3((s32 (*)())Func_02003420, 13, 3, 4);
      Value3((s32 (*)())Func_02003420, 14, 4, 4);
      Func_020032d4(40, 4, 4);
      if (!GameFlag_Test(0x249) && !Event_GetFlag(0xa64)) {
        map = (u8 *)Ram_HeapSlots->mapWork;
        i7 = 39;
        MapGrid_CopyCellsLayered(64, 64, 16, 13, 71, i7);
        MapGrid_CopyCellAttributes(71, 39, 16, 15, 7, i7);
        Func_02000b84();
        map[262] = 1;
        BG1_SET_PRIORITY_1();
      } else if (Event_GetFlag(0xa64) || GameFlag_Test(0x248)) {
        if (Event_GetFlag(0xa64)) {
          Actor_SetPosition(12, PIXELS(280), PIXELS(712));
        }
        Func_02000b24(12, 38);
        Func_02003628();
        Value4((s32 (*)())Func_02003688, 12, 2, 45, 1);
        for (i8 = 0; i8 < 8; i8++)
          Func_020036b8(33, 55);
        Event_SetFlag(0x249);
      }
      i9 = 12;
      do {
        actor = Actor_GetById(i9);
        actor->motionFlags = 0;
        actor->terrainHeight = 0;
        actor->y = 0;
        i9++;
      } while (i9 <= 14);
      goto statues_cleared;
    }
  no_statues:
    Func_020032d4(33, 0, -4);
  statues_cleared:
    if (GameFlag_Test(0xa6a)) {
      Func_02000ad8();
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom5) {
    Func_020032d4(33, 0, -2);
    Call3((void (*)())Func_020032d4, 40, 0, 0);
    Call1((void (*)())Func_02003ea4, 0);
    if (((u32)(entrance - 1) <= 3 || entrance == 7) && GameFlag_Test(0xa67)) {
      Func_02003628();
      Value4((s32 (*)())Func_02003688, 44, 0, 34, 2);
      for (i10 = 0; i10 <= 4; i10++)
        Func_020036b8(33, 55);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom6) {
    Call1((void (*)())Func_02003ea4, 0);
    if (entrance == 1 || entrance == 13) {
      Func_020032d4(33, 0, -2);
    } else if (entrance == 9 || entrance == 11 || entrance == 12) {
      Func_020032d4(33, 0, -2);
    }
    if (!GameFlag_Test(0x238)) {
      i11 = 13;
      do {
        actor = Actor_GetById(i11);
        actor->motionFlags = 0;
        actor->x = 0;
        actor->z = 0;
        i11++;
      } while (i11 <= 15);
      goto done;
    } else {
      MapGrid_CopyCellsLayered(114, 39, 1, 2, 113, 39);
      i12 = 41;
      MapGrid_CopyCellAttributes(105, 41, 12, 13, i12, i12);
      MapGrid_CopyCellsLayered(105, 105, 12, 13, 105, i12);
      for (i13 = 13; i13 <= 15; i13++) {
        actor = Actor_GetById(i13);
        if (GameFlag_Test(i13 + 580)) {
          MapGrid_CopyCellAttributes(47, 45, 1, 1, actor->x >> 20,
                                     actor->z >> 20);
        }
      }
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom7) {
    Func_020032d4(33, 0, -2);
  } else if (scene == (s32)&SceneId_Ov67cRoom8) {
    Call1((void (*)())Func_02003ea4, 0);
    Func_020032d4(33, 0, -2);
    Func_02003edc();
    Call3((void (*)())Func_02003ee4, 0, 8, 9);
  } else if (scene == (s32)&SceneId_Ov67cRoom9) {
    if ((u32)(entrance - 3) <= 1) {
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
      Value3((s32 (*)())Func_02003420, 10, 6, 4);
      Value3((s32 (*)())Func_02003420, 11, 3, 4);
      Value3((s32 (*)())Func_02003420, 12, 5, 4);
      Value3((s32 (*)())Func_02003420, 13, 6, 4);
      Value3((s32 (*)())Func_02003420, 14, 5, 4);
      Value3((s32 (*)())Func_02003420, 15, 4, 4);
      Value3((s32 (*)())Func_02003420, 16, 4, 4);
      Value3((s32 (*)())Func_02003420, 17, 3, 4);
      Value3((s32 (*)())Func_02003420, 18, 6, 4);
      Func_020032d4(40, 4, 4);
      if (!GameFlag_Test(0x24b) && !Event_GetFlag(0xa65)) {
        map = (u8 *)Ram_HeapSlots->mapWork;
        i14 = 13;
        MapGrid_CopyCellsLayered(64, 64, 19, 17, 100, i14);
        MapGrid_CopyCellsLayered(35, 105, 23, 24, 35, 73);
        MapGrid_CopyCellAttributes(100, 13, 19, 19, 36, i14);
        map[262] = 1;
        BG1_SET_PRIORITY_1();
      } else if (Event_GetFlag(0xa65) || GameFlag_Test(0x24a)) {
        if (Event_GetFlag(0xa65)) {
          Actor_SetPosition(12, PIXELS(808), PIXELS(296));
          Actor_SetPosition(13, PIXELS(680), PIXELS(360));
          Actor_SetPosition(15, PIXELS(744), PIXELS(424));
        }
        Call2((void (*)())Func_02000b24, 12, 38);
        Call2((void (*)())Func_02000b24, 13, 39);
        Func_02000b24(15, 37);
        Func_02003628();
        Value4((s32 (*)())Func_02003688, 38, 2, 19, 1);
        for (i15 = 0; i15 < 8; i15++)
          Func_020036b8(33, 55);
        Event_SetFlag(0x24b);
      }
    }
    i16 = 10;
  clear_pillars:
    actor = Actor_GetById(i16);
    actor->motionFlags = 0;
    actor->terrainHeight = 0;
    actor->y = 0;
    i16++;
    if (i16 <= 20)
      goto clear_pillars;
    actor = Actor_GetById(19);
    MapGrid_CopyCellAttributes(23, 39, 1, 1, actor->x >> 20, 39);
    if ((u32)(entrance - 1) <= 1) {
      Call1((void (*)())Func_02003ea4, 0);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom10) {
    Call1((void (*)())Func_02003ea4, 0);
    Func_020032d4(33, 0, -2);
    Event_SetFlag(0x144);
    if (GameFlag_Test(0x349)) {
      MapGrid_CopyCellAttributes(38, 9, 7, 6, 13, 9);
    } else {
      i17 = 8;
      MapGrid_CopyCellsLayered(77, 8, 7, 6, 50, i17);
      MapGrid_CopyCellsLayered(70, 8, 7, 6, 77, i17);
      MapGrid_CopyCellAttributes(38, 17, 7, 6, 13, 9);
    }
  }
done:
  return 0;
}

#include "system/frame.h"

void Func_02002bd4(struct Actor *object) {
  struct Actor *child = FieldActor_Create(14, object->x, object->y + 0x40000,
                                          object->z + 0x40000);
  struct FieldSprite *source = object->sprite;

  if (child != 0) {
    struct FieldSprite *sprite = child->sprite;

    child->terrainHeight = object->terrainHeight;
    Func_02003ccc(child, gData_0200411c);
    child->motionFlags = 0;
    if (sprite != 0) {
      Animation_ApplyChildArgument(sprite, 1);
      sprite->nextFrame = 0;
      sprite->priority = source->priority;
    }
  }
}

void Func_02002c3c(struct Actor *object) {
  s32 phase = gFrameCount & 15;

  if (phase == 0) {
    struct Actor *child =
        Import_ActorCreate_(0x10d, object->x, object->y, object->z);

    if (child != 0) {
      struct FieldSprite *sprite = child->sprite;

      Func_02003ccc(child, gData_02004128);
      child->motionFlags = 0;
      child->y = -0x78000;
      child->terrainId = 1;
      if (sprite != 0) {
        Animation_ApplyChildArgument(sprite, 2);
        sprite->nextFrame = 0;
        sprite->priority = 2;
      }
    }
  }
}

struct SlideWork {
  u8 unk000[0x1cc];
  struct Actor *camera;
};

s32 Func_02003d2c(struct Actor *object, s32 *position);

void Func_02002ca4(void) {
  struct Actor *center;
  s32 id;
  struct Actor *actor;
  s32 position[3];
  s32 hit;

  {
    struct SlideWork *work = Ram_HeapSlots->eventWork;

    id = gMainRoutineWork.currentOwner;
    center = work->camera;
  }
  actor = Actor_Get(id);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67bRoom1 &&
      actor->terrainHeight == 0)
    return;
  position[0] = actor->x;
  position[1] = actor->y;
  position[2] = actor->z + 0x20000;
  hit = Func_02003d2c(actor, position);
  if ((gFrameCount & 4) == 0)
    Func_02002bd4(actor);
  if (hit < 0) {
    /* FAKEMATCH(wrapper): Call2 kept; as a direct call, 2 of 151 instructions
     * differ in tla-en. */
    Actor_SetAttachedEffect(id, 258);
    FieldActor_SetMoveTarget(actor, actor->x, actor->y, actor->z + 0x80000);
    Func_02003cc4(actor, 49);
    Actor_WaitForMove(actor);
    do {
      System_Wait(1);
    } while (actor->y != actor->terrainHeight);
    Func_02002bd4(actor);
    Func_02003cc4(actor, 49);
    System_Wait(3);
    return;
  }
  position[0] = actor->x;
  position[1] = actor->y;
  position[2] = actor->z + 0x80000;
  hit = Func_02003d2c(actor, position);
  if (hit > 0)
    return;
  position[0] = actor->x + 0x5b333;
  position[1] = actor->y;
  position[2] = actor->z + 0x5b333;
  hit = Func_02003d2c(actor, position);
  if (hit > 0)
    return;
  position[0] = actor->x - 0x5b333;
  position[1] = actor->y;
  position[2] = actor->z + 0x5b333;
  hit = Func_02003d2c(actor, position);
  if (hit > 0)
    return;
  center->z += 0x20000;
  actor->z += 0x20000;
  actor->facing = 0x4000;
}

void Func_02002e04(s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object != 0) {
    u8 *meta = ResourceMetadata_Register(object->sprite, 0x17d);

    meta[5] = 0;
    meta[6] = 0;
    Func_02003cc4(object, 0);
    Func_02003cc4(object, 2);
  }
}

void Func_02002e34(s32 entrance) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 i;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  leader->motionFlags = 0;
  leader->priorityFlags |= 0x20;
  Event_PlaySound(215);
  Func_02003cc4(leader, 18);
  Event_PlaySound(612);
  for (i = 0; i < 60; i++) {
    s32 angle;

    if (i == 30)
      Event_FinishScreenTransition();
    leader->y -= 0x4000;
    angle = leader->facing;
    leader->facing = angle + 0x1000;
    if ((i & 7) == 0)
      Func_02002bd4(Actor_Get(15));
    System_Wait(1);
  }
  Func_02003d84();
  FieldEvent_SetExitRequest(entrance);
}

void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 flags,
                   s32 *params);

/// On odd frames, spawns a drifting effect above the object, swaying with the
/// frame count; Func_02002030 of Aqua Rock is the same function.
s32 Func_02002ed4(struct Actor *object) {
  s32 params[10];

  if (gFrameCount & 1) {
    s32 drift;
    s32 x;
    s32 y;

    params[1] = 7;
    if ((gFrameCount & 2) == 0)
      params[1] = 5;
    params[2] = 0xcccc;
    params[3] = 0xcccc;
    params[0] = 0;
    drift = ((u32)(Random16Far() << 3) >> 16) * 0x3333;
    x = object->x;
    y = object->y;
    Func_0200015c(x + ((8 - (gFrameCount & 15)) << 16), y + 0x1a0000, object->z,
                  0, -drift, 0, 0xb0000, params);
  }
  return 0;
}

void Func_02002f60(s32 entrance) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 *owner = &gMainRoutineWork.currentOwner;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(228);
  leader->update = (void (*)(void))Func_02002ed4;
  leader->motionFlags = 0;
  leader->speedLimit = 0x3333;
  Actor_SetAnimation(*owner, 2);
  Actor_SetDestinationOffset(*owner, 0, -8);
  FieldActor_WaitForMoveById(*owner);
  Actor_SetSpriteColor(Actor_Get(*owner), 9);
  Actor_SetSingleChildFlags(Actor_Get(*owner), 0);
  leader->update = 0;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(entrance);
  Func_02003d84();
}

void Func_02002ffc(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 flag = Event_GetFlag(0x109);
  s32 *owner = &gMainRoutineWork.currentOwner;
  s32 offset;

  if (flag == 0) {
    Func_02003d7c();
    FieldEvent_ResolvePendingActions(0);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    leader->motionFlags = flag;
    Actor_SetPosition(*owner, ((s16 *)&leader->x)[1] << 16,
                      (((s16 *)&leader->z)[1] << 16) - 0x100000);
    Actor_SetSpriteColor(Actor_Get(*owner), 9);
    Actor_SetSingleChildFlags(Actor_Get(*owner), 0);
    Event_StartScreenTransition();
    Event_PlaySound(228);
    leader->update = (void (*)(void))Func_02002ed4;
    Actor_SetSpeed(*owner, 0x6666, 0x3333);
    Actor_WalkByAndWait(*owner, 0, 8);
    Actor_SetSpriteColor(Actor_Get(*owner), 0);
    Actor_SetSingleChildFlags(Actor_Get(*owner), 1);
    ((struct FieldSprite *)leader->sprite)->priority = 1;
    offset = 10;
    Actor_WalkByAndWait(*owner, 0, offset);
    leader->motionFlags = 3;
    leader->update = (void (*)(void))flag;
    Func_02003ed4();
    Event_WaitTransitionFrames();
    Func_02003d84();
  }
}

#include "map.h"
#include "vram_tab.h"

void Func_02003c94(u32 *sprite, s32 depth);

/// Draws the scene's pillars: a top sprite for each active one and, when it
/// stands above its base, a column sprite as tall as its height.
void Func_020030f4(void) {
  s32 *view = &Ram_HeapSlots->mapWork->viewX;
  struct PillarWork *work = &gData_0200536c;
  s32 viewX = view[0];
  struct PillarEntry *entry = work->entries;
  s32 viewZ;
  s32 originY;
  s32 depth;
  s32 i;
  u32 tile;

  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = Ram_HeapSlots->mapWork->origin[1];
  tile = gResourceTableEntries[work->vramEntry].offset >> 5;

  for (i = 0; i < 32; i++) {
    if (entry->state != 0) {
      s32 *position = entry->position;
      s32 x;
      s32 y;
      s32 z;
      s32 screenY;
      s32 attributes;

      if (position != 0) {
        x = position[0] - viewX;
        y = position[1] + entry->height - originY;
        z = position[2] - viewZ - originY;
        screenY = z - y;
      } else {
        x = entry->x - viewX;
        y = entry->height - originY;
        z = entry->z - viewZ - originY;
        screenY = z - y;
      }
      if (entry->height != entry->base || entry->state == 2)
        attributes = 0x800;
      else
        attributes = 0xc00;
      x = (x >> 16) - 8;
      screenY = (screenY >> 16) - 8;
      depth = ((y + z) >> 16) + 50;
      if ((u32)(x + 15) <= 270) {
        s32 rise;

        if ((u32)(screenY + 47) <= 238) {
          x &= 511;
          screenY &= 255;
          rise = entry->kind * 8;
          entry->top[0] = 0;
          screenY |= x << 16;
          screenY |= 0x40002000;
          entry->top[1] = screenY;
          entry->top[2] = (tile + rise) | attributes;
          Func_02003c94(entry->top, depth);
        }
        screenY = z - y;
        screenY = (screenY >> 16) + 8;
        if ((u32)(screenY + 47) <= 238) {
          rise = (entry->height - entry->base) >> 16;
          if (rise != 0) {
            u32 *column;
            rise--;
            screenY &= 255;
            column = entry->column;
            if ((u32)rise > 7)
              rise = 7;
            rise = rise * 4 + 64;
            column[0] = 0;
            screenY |= x << 16;
            screenY |= 0x6000;
            entry->column[1] = screenY;
            entry->column[2] = attributes | (tile + rise);
            Func_02003c94(column, depth);
          }
        }
      }
    }
    entry++;
  }
}

void Func_02003278(void) {
  void *buffer;
  s32 priority;

  Iwram_ClearWords(&gData_0200536c, 0x604);
  buffer = MemAlloc_(0xc80);
  Func_02003c7c(gData_0200414c, buffer);
  gData_0200536c.vramEntry = Resource_FindFreeEntry();
  Vram_Load(gData_0200536c.vramEntry, 0xc80, buffer);
  Heap_Release(buffer);
  priority = 0x41c;
  FieldTask_AddFrame(Func_020030f4, priority);
}

extern u8 Data_0202c001[];

#define LAYER_S32(layer, off) (*(s32 *)((layer)->unk08 + (off) - 8))

// Adds a water spout on every map cell of the given layer whose spout kind lies
// in [first, first + 8), at the given heights, and marks those cells as spout
// cells (120). The loops are built from labels: both 0x80000 loads stay in the
// body and neither counter is strength-reduced.
void Func_020032d4(u32 first, s32 level, s32 baseLevel) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayer *layer = map->layers;
  struct MapCell *cells = layer->cells;
  s32 originX = LAYER_S32(layer, 8) + map->minX;
  s32 originZ = LAYER_S32(layer, 12) + map->minZ;
  struct PillarWork *work = &gData_0200536c;
  s32 width = (map->maxX - map->minX) >> 20;
  s32 depth = (map->maxZ - map->minZ) >> 20;
  struct PillarEntry *spout = &work->entries[work->count];
  s32 corner = ((map->minZ >> 20) << 7) + (map->minX >> 20);
  s16 row;

  *(s32 *)(Data_0202c001 + 0x1df) = level << 8;
  cells += corner;
  row = 0;
  if (row < depth) {
    struct MapCell *cell;
    s16 column;
  rowTop:
    cell = cells + ((u16)row << 7);
    column = 0;
    if (column < width) {
    columnTop:
      {
        u32 kind = cell->collisionCode;

        if (kind != 0 && kind >= first && kind < first + 8) {
          spout->x = ((u16)column << 20) + originX + 0x80000;
          spout->z = ((u16)row << 20) + originZ + 0x80000;
          spout->base = baseLevel << 19;
          spout->height = level << 19;
          spout->kind = kind - first;
          spout->position = 0;
          spout->state = 1;
          spout++;
          work->count++;
          {
            u8 *terrain = map->layers[0].terrainIds + corner;

            terrain[(u16)column + ((u16)row << 7)] = 120;
          }
        }
      }
      {
        s32 next = (column << 16) + 0x10000;
        column = next >> 16;
        cell++;
        if ((s32)((u32)next >> 16) < width)
          goto columnTop;
      }
    }
    {
      s32 next = (row << 16) + 0x10000;
      row = next >> 16;
      if ((s32)((u32)next >> 16) < depth)
        goto rowTop;
    }
  }
}

void Func_02003420(s32 id, s32 kind, s32 height) {
  struct Actor *object = Actor_Get(id);
  struct PillarWork *work = &gData_0200536c;
  struct PillarEntry *entry = &work->entries[work->count];

  if (object != 0) {
    entry->x = 0;
    entry->z = 0;
    entry->base = height << 19;
    entry->height = height << 19;
    entry->state = 2;
    entry->kind = kind;
    entry->position = &object->x;
    work->count++;
  }
}

void Func_02003460(s32 height) {
  struct PillarWork *work = &gData_0200536c;
  struct PillarEntry *entry;
  s32 i;

  *(s32 *)(Ram_MapCollision + 0x1e0) = ((height / 0x80000) & 0xff) << 8;
  entry = work->entries;
  for (i = 31; i >= 0; i--) {
    if (entry->x != 0)
      entry->height = height;
    entry++;
  }
}

void Func_0200349c(u32 index, s32 height) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct PillarWork *work = &gData_0200536c;

  if (index <= 31) {
    struct PillarEntry *entry = &work->entries[index];

    if (entry->x != 0) {
      *(s32 *)(Ram_MapCollision + 0x1e4) = ((height / 0x80000) & 0xff) << 8;
      {
        u8 *ids = map->layers[0].terrainIds;
        s32 at = (entry->x >> 20) + (entry->z >> 20) * 128;
        s32 v = 121;
        ids[at] = v;
      }
      entry->height = height;
    }
  }
}

/// Draws thirty-two sprites that each shake by a random few pixels a frame.
void Func_020034fc(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  struct BeamWork *splashes = &gData_02005970;
  s32 viewX = view[0];
  s32 viewZ;
  s32 originY;
  struct Splash *splash;
  s32 i;
  u32 tile;
  // The sprite word's priority bits (priority 2): a local that holds this one
  // value.
  s32 priority = 0x800;

  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = map->origin[1];
  tile = gResourceTableEntries[splashes->vramEntry].offset >> 5;
  splash = splashes->splashes;
  i = 31;

  do {
    if (splash->active != 0) {
      s32 x = splash->x - viewX;
      s32 y = splash->y - originY;
      s32 z = splash->z;
      s32 screenY, rawX, rawY;
      z -= viewZ;
      z -= originY;
      screenY = z - y;
      rawX = (x >> 16) + ((Random16Far() * 10) >> 16);

      x = rawX - 10;
      rawY = (screenY >> 16) + ((Random16Far() * 10) >> 16);
      screenY = rawY - 10;
      z = ((y + z) >> 16) + 50;

      if ((u32)(x + 15) <= 270 && (u32)(screenY + 47) <= 238) {
        u32 *frame = &splash->sprite[2];
        s32 column = x & 511;

        screenY &= 255;
        splash->sprite[0] = 0;
        splash->sprite[1] = screenY | (column << 16) | 0x40002000;
        *frame = (tile + (((Random16Far() * 3) >> 16) << 3)) | priority;
        Func_02003c94(splash->sprite, z);
      }
    }
    i--;
    splash++;
  } while (i >= 0);
}

#include "macros.h"

void Func_02003628(void) {
  void *buffer;
  s32 priority;

  Iwram_ClearWords(&gData_02005970, 0x3a4);
  buffer = MemAlloc_(0x400);
  Func_02003c7c(gData_02004351, buffer);
  gData_02005970.vramEntry = Resource_FindFreeEntry();
  Vram_Load(gData_02005970.vramEntry, 0x400, buffer);
  Heap_Release(buffer);
  priority = 0x41c;
  FieldTask_AddFrame(Func_020034fc, priority);
  Event_PlaySound(220);
}

s32 Func_02003688(s32 x, s32 y, s32 z, s32 kind) {
  struct BeamWork *work = &gData_02005970;
  struct BeamEntry *entry = work->entries;

  if (entry->x != 0)
    entry++;
  SET_POSITION(entry, (x << 20) + 0x80000, y << 20, (z << 20) + 0x80000);
  entry->direction = kind;
}

extern const s8 gData_0200534c[];

// Advances Aqua Rock's two water streams one cell along the map: each stream
// follows the turn its cell's kind gives, leaves a splash on every open cell
// (255) it crosses, stops on a wall kind and counts the cells of the given kind
// it reaches; then it clears the splashes past the new count and opens the
// cells under those left.
s32 Func_020036b8(s32 first, s32 target) {
  s32 reached = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct BeamWork *state = &gData_02005970;
  s32 count = state->count;
  s32 turn;
  s32 again = 0;
  s32 i;
  struct BeamEntry *stream = state->entries;
  struct Splash *splash;

  for (i = 0; i < 2; i++) {
    if (stream->x != 0) {
      splash = &state->splashes[state->count];
      do {
        struct MapCell *cells = map->layers[0].cells;
        struct MapCell *cell =
            &cells[(stream->x >> 20) + (stream->z >> 20) * 128];
        s32 kind = cell->collisionCode;

        if (kind == 255) {
          splash->x = stream->x;
          splash->z = stream->z;
          splash->y = stream->y;
          splash->active = 1;
          splash++;
          state->count++;
          turn = 0;
          again = 1;
        } else if (kind >= first && kind <= first + 7) {
          turn = gData_0200534c[(stream->direction << 3) + kind - first];
          if (turn != 8)
            count = state->count;
          again = 0;
        } else if (kind == target) {
          turn = 8;
          count = state->count;
          reached++;
          again = 0;
        } else if (kind == 0) {
          turn = 8;
          again = 0;
        }
        if (turn != 8) {
          // The unsigned % 4: as & 3 the mask narrows to a halfword and pools.
          stream->direction = (u32)(stream->direction + turn) % 4;
          FieldVector_AddPolarOffset(0x100000, stream->direction << 14,
                                     &stream->x);
        }
      } while (again != 0);
      state->count = count;
    }
    stream++;
  }
  splash = &state->splashes[count];
  if (count < 32) {
    i = 32 - count;
  clear:
    if (splash->active != 0)
      splash->active = 0;
    splash++;
    if (--i != 0)
      goto clear;
  }
  splash = state->splashes;
  if (count != 0) {
    i = count;
  open:
    if (splash->active != 0) {
      struct MapCell *cells = map->layers[0].cells;
      struct MapCell *cell =
          &cells[(splash->x >> 20) + (splash->z >> 20) * 128];

      cell->collisionCode = 0;
      cell->attributeB |= 0x80;
    }
    splash++;
    if (--i != 0)
      goto open;
  }
  return reached;
}
