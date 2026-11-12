#include "tundaria_tower.h"
#include "call.h"
#include "scene_tables.h"
#include "types.h"
#include "event_inline.h"
#include "io_reg.h"
#include "actor.h"
#include "main_routine_work.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02003178(void);

/// Off layer 2's terrain height, the event's object runs cue 188 on terrain 2
/// with motion flags 3. In room 6, column 27 sets flag 0x947, and column 25
/// with that flag set sets 0x946.
void Func_02000d2c(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *object;

  object = *(struct Actor **)(event + 0xcbc);

  Func_02003048();
  Event_ResolvePendingActions(0);
  if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
    object->terrainId = 2;
    object->motionFlags = 3;
    OverlayObject_WaitUntilIdle(object);
    Sound_Play(188);
    OverlayObject_WaitUntilIdle(object);
    object->motionFlags = 0;
    object->terrainId = 0;
  }
  Func_02003178();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom6) {
    /* FAKEMATCH(temporary): the copy makes each column test reload X; as
     * object->x, 10 of 76 instructions differ in tla-en. */
    struct Actor *o = object;

    if (o->x >> 19 == 27)
      GameFlag_SetBit(0x947);
    if (o->x >> 19 == 25 && GameFlag_Test(0x947))
      GameFlag_SetBit(0x946);
  }
  Func_02003050();
}

void Func_02002fc8(void);

void Func_02000de4(void) {
  Func_02002fc8();
  GameFlag_ClearBit(0x212);
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    Map_CopyCellPictureIndicesRect(39, 100, 53, 100, 3, 3);
    Call6((void (*)())Map_CopyCellAttributeRect, 43, 41, 3, 3, 53, 37);
    if (Actor_GetById(gMainRoutineWork.currentOwner)->z >> 20 == 38) {
      s16 angle = Actor_GetById(gMainRoutineWork.currentOwner)->facing;
      s32 owner = gMainRoutineWork.currentOwner;

      if (angle < 0)
        Actor_SetHorizontalPositionWithTerrain(owner, PIXELS(872), PIXELS(600));
      else
        Actor_SetHorizontalPositionWithTerrain(owner, PIXELS(872), PIXELS(632));
    }
  }
}

void Func_02002fd0(void);

void Func_02000e78(void) {
  Func_02002fd0();
  GameFlag_SetBit(0x212);
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    Map_CopyCellPictureIndicesRect(43, 100, 53, 100, 3, 3);
    Call6((void (*)())Map_CopyCellAttributeRect, 43, 37, 3, 3, 53, 37);
    if (Actor_GetById(gMainRoutineWork.currentOwner)->z >> 20 > 38)
      MapCell_SetAttributes(0, 54, 37, 0x1f00);
    else
      MapCell_SetAttributes(0, 54, 39, 0x1f00);
  }
}

void Func_02000f00(void) { Event_ResolvePendingActions(0); }

void Func_02000f0c(s32 step) {
  struct TundariaTowerPosition position;

  switch (step) {
  case 1:
    Map_CopyCellPictureIndicesRect(18, 97, 6, 97, 3, 4);
    Map_CopyCellPictureIndicesRect(82, 34, 70, 34, 3, 2);
    Call6((void (*)())Map_CopyCellAttributeRect, 7, 36, 1, 1, 7, 35);
    GameFlag_SetBit(0x94b);
    break;
  case 2:
    position.x = 0x780000;
    position.y = 0;
    position.z = 0x2380000;
    Func_02000708(&position, 1, 0xcccc);
    break;
  case 3:
    position.x = 0x780000;
    position.y = 0;
    position.z = 0x2380000;
    Func_02000708(&position, 8, 0x18000);
    break;
  }
}

void Func_02000fac(void) { Event_ResolvePendingActions(0); }

void Func_02000fb8(s32 mode, s32 cue) {
  if (cue == 0) {
    Func_02002e00();
    Task_RemoveFar(Func_0200137c);
  }
  if (mode == 1) {
    Map_CopyCellPictureIndicesRect(13, 26, 13, 16, 3, 3);
    Call6((void (*)())Map_CopyCellAttributeRect, 13, 26, 3, 3, 13, 16);
    GameFlag_SetBit(0x948);
  }
  if (mode == 2)
    Func_02002d44(0xe80000, 0x100000, 0x1180000, 2);
  if (mode == 3)
    Func_02002d44(0xe80000, 0x100000, 0x1180000, 30);
  if (cue == 0x3e7)
    Func_02002ebc();
}

const struct Trigger *Func_02001044(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom1)
    return (const struct Trigger *)gData_020037f8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom2)
    return (const struct Trigger *)gData_0200381c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom3)
    return (const struct Trigger *)gData_0200393c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom4)
    return (const struct Trigger *)gData_020039e4;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom5)
    return (const struct Trigger *)gData_02003a5c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom6)
    return (const struct Trigger *)gData_02003b28;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom7)
    return (const struct Trigger *)gData_02003c90;
  return (const struct Trigger *)gData_0200381c;
}

struct ReflectionSprite {
  u8 unk00[2];
  u16 unk02;
  u8 unk04[12];
  s32 unk10;
  u8 unk14;
};

struct ReflectionAnimation {
  u8 unk00[0x28];
  struct ReflectionSprite *sprite;
};

void Func_020010d4(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct ReflectionAnimation *leaderAnimation = leader->sprite;
  struct ReflectionAnimation *animation = object->sprite;

  if (leader->z <= object->unk4c) {
    object->z = 0;
    object->x = 0;
    REG_BG2CNT &= 0xfffc;
    REG_BG2CNT |= 1;
  } else {
    REG_BG2CNT &= 0xfffc;
    REG_BG2CNT |= 2;
    if (GameFlag_Test(0x212)) {
      object->z = 0;
      object->x = 0;
    } else {
      struct ReflectionSprite *sprite = animation->sprite;
      struct ReflectionSprite *source;

      object->x = leader->x;
      object->y = leader->y;
      object->z = object->unk4c - (leader->z - object->unk4c);
      object->facing = ~leader->facing;
      source = leaderAnimation->sprite;
      sprite->unk10 = source->unk10;
      sprite->unk02 = source->unk02;
      sprite->unk14 = source->unk14;
    }
  }
}
