/// @file
/// Scene code of overlay 0x67c, in ROM order, from s1c68's offset up to the next object the overlay links.

#include "scene_tables.h"
#include "local.h"
#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "call.h"
#include "actor.h"
#include "ram_buffer.h"
#include "event_inline.h"

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
    u8 *meta = ResourceMetadata_Register(object->animation, 0x17d);

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
      struct FieldSprite *sprite = child->animation;

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
  object->angle += 0x2000;
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

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

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
