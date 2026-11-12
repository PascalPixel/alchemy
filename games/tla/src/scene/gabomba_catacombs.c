/// @file
/// The scene code of overlay 0x673, in ROM order.

#include "gabomba_catacombs/local.h"

#include "event_inline.h"

#define LEADER_HOP()                                                           \
  do {                                                                         \
    Func_0200119c();                                                           \
    Event_ResolvePendingActions(0);                                            \
    Func_080d489c(gMainRoutineWork.currentOwner, 0x102);                       \
  } while (0)

void Func_02000038(void) { Func_080d2b4c(8, 10); }

void Func_02000044(void) { Func_02001274_(7, 63, 1, 16, 16, 57, 63); }

void Func_02000060(void) { Func_02001274_(22, 15, 0, 5, 5, 63, 52); }

void Func_02000080(void) { Func_02001274_(7, 22, 0, 3, 3, 56, 58); }

void Func_020000a0(void) { Func_02001274_(44, 25, 0, 4, 3, 66, 57); }

void Func_020000c0(void) { Func_02001274_(16, 13, 0, 4, 8, 61, 52); }

void Func_020000e0(void) { Func_02001274_(21, 9, 0, 2, 2, 57, 52); }

void Func_02000100(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001274_(22, 15, 0, 5, 5, 63, 52);
  if (GameFlag_Test(0xf53) == 0) {
    Actor_SetPosition(64, PIXELS(392), PIXELS(344));
    if (leader->x >> 20 == 24 && leader->z >> 20 == 21) {
      LEADER_HOP();
      Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = PIXELS(6);
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_020011a4();
    }
  }
}

void Func_0200018c(void) {
  MapGrid_CopyCellsLayered(51, 83, 3, 1, 9, 65);
  Import_EventSetExitRequest(3);
}

void Func_020001b0(void) {
  MapGrid_CopyCellsLayered(51, 83, 3, 1, 18, 66);
  Import_EventSetExitRequest(4);
}

void Func_020001d4(void) { Import_EventSetExitRequest(5); }

void Func_020001e0(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001274_(7, 22, 0, 3, 3, 56, 58);
  Actor_SetPosition(8, PIXELS(136), PIXELS(376));
  if (leader->x >> 20 == 8 && leader->z >> 20 == 23) {
    LEADER_HOP();
    Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 16, 0);
    leader->velocityY = PIXELS(6);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Func_020011a4();
  }
}

void Func_02000260(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001274_(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(9, PIXELS(136), PIXELS(248));
  if (leader->x >> 20 == 8 && leader->z >> 20 == 15) {
    LEADER_HOP();
    Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 16, 0);
    leader->velocityY = PIXELS(6);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Func_020011a4();
  }
}

void Func_020002e0(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001274_(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(10, PIXELS(152), PIXELS(264));
  if (leader->x >> 20 == 9 && leader->z >> 20 == 16) {
    Func_0200119c();
    Event_ResolvePendingActions(0);
    Func_080d489c(gMainRoutineWork.currentOwner, 0x102);
    Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, -16, 0);
    leader->velocityY = PIXELS(6);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Func_020011a4();
  }
}

void Func_02000364(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001274_(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(11, PIXELS(136), PIXELS(280));
  if (leader->x >> 20 == 8 && leader->z >> 20 == 17) {
    LEADER_HOP();
    Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 16, 0);
    leader->velocityY = PIXELS(6);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Func_020011a4();
  }
}

void Func_020003e4(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001274_(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(12, PIXELS(104), PIXELS(280));
  if (leader->x >> 20 == 6 && leader->z >> 20 == 17) {
    LEADER_HOP();
    Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 16, 0);
    leader->velocityY = PIXELS(6);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Func_020011a4();
  }
}

void Func_02000464(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001274_(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(13, PIXELS(88), PIXELS(264));
  if (leader->x >> 20 == 5 && leader->z >> 20 == 16) {
    LEADER_HOP();
    Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 16, 0);
    leader->velocityY = PIXELS(6);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Func_020011a4();
  }
}

void Func_020004e4(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001274_(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(14, PIXELS(104), PIXELS(248));
  if (leader->x >> 20 == 6 && leader->z >> 20 == 15) {
    LEADER_HOP();
    Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 16, 0);
    leader->velocityY = PIXELS(6);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Func_020011a4();
  }
}

void Func_02000564(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001274_(16, 13, 0, 4, 8, 61, 52);
  Actor_SetPosition(8, PIXELS(280), PIXELS(296));
  if (leader->x >> 20 == 17 && leader->z >> 20 == 18) {
    Func_0200119c();
    Event_ResolvePendingActions(0);
    Func_080d489c(gMainRoutineWork.currentOwner, 0x102);
    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -16);
    leader->velocityY = PIXELS(6);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Func_020011a4();
  }
}

void Func_020005e8(void) { Func_02001274_(7, 23, 0, 4, 5, 60, 56); }

void Func_02000608(void) {
  Func_02001274_(7, 23, 0, 4, 5, 60, 56);
  if (GameFlag_Test(0x9b5) == 0) {
    Func_0200127c_(249, 168, 424);
    ((struct GabombaCatacombsParty *)&gMainRoutineWork)->value254 = 0x19b5;
    gMainRoutineWork.battleRule24b = 2;
    Func_02001224(106, 0);
  }
}

void Func_0200066c(void) {
  Func_0802b450(0, 0, 1, 2, 7, 11);
  Map_CopyCellAttributeRect(2, 4, 1, 1, 7, 14);
}

void Func_0200069c(void) {
  Func_0802b450(1, 0, 1, 2, 7, 11);
  Map_CopyCellAttributeRect(2, 5, 1, 1, 7, 14);
}

void Func_020006cc(void) {
  struct Actor *object = Actor_GetById(8);

  GameFlag_SetBit(0x200);
  object->motionFlags = 0;
  object->priorityFlags = 0;
  MapGrid_CopyCellAttributes(6, 23, 1, 1, 8, 23);
}

void Func_02000704(void) {
  struct Actor *object = Actor_GetById(9);

  GameFlag_SetBit(0x201);
  object->motionFlags = 0;
  object->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 15);
}

void Func_02000740(void) {
  struct Actor *object = Actor_GetById(10);

  GameFlag_SetBit(0x202);
  object->motionFlags = 0;
  object->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 9, 16);
}

void Func_0200077c(void) {
  struct Actor *object = Actor_GetById(11);

  GameFlag_SetBit(0x203);
  object->motionFlags = 0;
  object->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 17);
}

void Func_020007b8(void) {
  struct Actor *object = Actor_GetById(12);

  GameFlag_SetBit(0x204);
  object->motionFlags = 0;
  object->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 17);
}

void Func_020007f0(void) {
  struct Actor *object = Actor_GetById(13);

  GameFlag_SetBit(0x205);
  object->motionFlags = 0;
  object->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 5, 16);
}

void Func_0200082c(void) {
  struct Actor *object = Actor_GetById(14);

  GameFlag_SetBit(0x206);
  object->motionFlags = 0;
  object->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 15);
}

void Func_02000868(void) {
  struct Actor *object = Actor_GetById(8);

  GameFlag_SetBit(0x200);
  object->motionFlags = 0;
  object->priorityFlags = 0;
  MapGrid_CopyCellAttributes(15, 18, 1, 1, 17, 18);
}

void Func_020008a0(struct Actor *object, s32 dz) {
  struct GabombaCatacombsFollower *follower;
  struct GabombaCatacombsVec position;

  follower =
      ((struct GabombaCatacombsEventWork *)Ram_HeapSlots->eventWork)->follower;

  position.x = object->x;
  position.y = object->y;
  position.z = object->z + dz;
  Func_02001174(object, &position);
  position.x = object->x;
  position.y = object->y;
  position.z = object->z + 0x80000;
  if (Func_02001174(object, &position) <= 0) {
    position.x = object->x + 0x5b333;
    position.y = object->y;
    position.z = object->z + 0x5b333;
    if (Func_02001174(object, &position) <= 0) {
      position.x = object->x - 0x5b333;
      position.y = object->y;
      position.z = object->z + 0x5b333;
      if (Func_02001174(object, &position) <= 0) {
        follower->z += dz;
        object->z += dz;
      }
    }
  }
}

void Func_020008a0(struct Actor *object, s32 dz);

void Func_0200094c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *steam;

  if ((gFrameCount & 7) == 0) {
    steam = ((struct Actor *)Import_ActorCreate(
        14, leader->x, leader->y + 0x60000, leader->z));
    if (steam) {
      Actor_SetSingleChildFlags(steam, 0);
      Func_0200114c(steam, 1);
      Func_02001154(steam, gData_0200128c);
    }
    Sound_Play(0x26f);
  }
  Func_020008a0(leader, 0xd999);
}

s32 Func_020009bc(struct Actor *object) {
  object->scaleX -= 3276;
  object->scaleY -= 3276;
  return 0;
}

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

extern const u8 MsgByReachingThis[];
extern const u8 gData_020012a4[];
void Func_0200119c(void);
void Func_020011a4(void);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_020011d4(s32 actor, s32 target);
s32 Func_02001264(void);
void Func_02001214(s32 actor, s32 a);

union EventLine {
  s32 word;
  u16 line;
};

#define TALK()                                                                 \
  FieldMessage_ShowCentered((s16)((union EventLine *)work)->line++, 0, -3)

void Func_020009d4(void) {
  struct Actor *follower = Actor_GetById(9);
  struct Actor *statue = Actor_GetById(0x40);
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;
  struct Actor *glow, *leader;
  s32 speed;

  Func_0200119c();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x403108, 1);
  BattleEffect_StartBufferInterpolation(60);
  FieldCamera_MoveTo(0xa80000, -1, 0x9e0000, 1);
  Sound_Play(26);
  Event_Wait(60);
  Actor_SetModeById(8, 2);
  Sound_Play(0x240);
  Event_Wait(60);
  Event_SetMessageId((s32)MsgByReachingThis);
  work += 0x1c4;
  TALK();
  Actor_SetPosition(0x40, 0xa80000, 0x980000);
  statue->motionFlags = 6;
  statue->terrainHeight += 0x100000;
  Sound_Play(220);
  glow = ((struct Actor *)Import_ActorCreate(412, 0, 0, 0));
  Func_02001154(glow, gData_020012a4);
  Animation_ApplyChildValues(glow, 1);
  glow->motionFlags = 0;
  glow->x = statue->x;
  glow->y = statue->y + 0x80000;
  glow->z = statue->z;
  glow->terrainHeight = statue->terrainHeight;
  glow->update = Func_020009bc;
  Event_Wait(15);
  glow->x = 0;
  glow->y = 0;
  glow->z = 0;
  glow->update = 0;
  TALK();
  Actor_ResetAndSetPosition(gMainRoutineWork.currentOwner, 166, 188);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Func_020011d4(9, gMainRoutineWork.currentOwner);
  speed = 0x10000;
  follower->speedLimit = speed;
  Actor_ResetAndSetPosition(9, 184, 188);
  Actor_WaitForMoveById(9);
  Actor_FaceDirection(9, 0x8000, 0);
  Event_ShowMessageAndWait(9, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 0);
  TALK();
  Func_080d47b4(9, 258, 0);
  Actor_FaceDirection(9, 0xb000, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(9, 0);
  TALK();
  Actor_ShowEmote(9, 261, 0);
  Event_Wait(30);
  TALK();
  Func_080d47b4(9, 265, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(9, 0);
  TALK();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(9, 0x8000, 0);
  Event_Wait(10);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 257, 0);
  Func_080d47b4(9, 257, 0);
  Event_Wait(60);
  TALK();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
  Actor_FaceDirection(9, 0xb000, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 261, 0);
  Func_080d47b4(9, 261, 0);
  Event_Wait(60);
  TALK();
  Event_Wait(40);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Event_Wait(40);
  Actor_SetModeById(9, 3);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetModeById(8, 2);
  TALK();
  Event_Wait(30);
  Actor_SetModeById(8, 4);
  Actor_SetModeById(9, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(9, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(9);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Actor_WaitForMoveById(9);
  GameFlag_SetBit(0x9ce);
  Func_02001264();
  PaletteFade_ApplyTarget(speed, 0);
  BattleEffect_StartBufferInterpolation(60);
  Func_02001214(gMainRoutineWork.currentOwner, 1);
  Event_Wait(60);
  Func_020011a4();
}

void Func_02000d90(void) {
  struct Actor *object = Actor_GetById(64);

  if (object != NULL && object->x != 0)
    Func_02001234(0x1040);
}

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern const u8 gData_02001310[];

const struct SceneRegion *Func_02000db4(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room1)
    return (const struct SceneRegion *)gData_02001310;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_02001378[];
extern const u8 gData_02001390[];
extern const u8 gData_020013a8[];
extern const u8 gData_020013d8[];
extern const u8 gData_02001498[];
extern const u8 gData_02001510[];
extern const u8 gData_02001540[];
extern const u8 gData_0200154c[];
extern const u8 gData_02001594[];
extern const u8 gData_020015c4[];
extern const u8 gData_020016cc[];
extern const u8 gData_02001714[];

const struct ActorPlacement *Func_02000de4(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room1)
    return (const struct ActorPlacement *)gData_02001390;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room2)
    return (const struct ActorPlacement *)gData_020013a8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room3)
    return (const struct ActorPlacement *)gData_020013d8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room4)
    return (const struct ActorPlacement *)gData_02001498;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room5)
    return (const struct ActorPlacement *)gData_02001510;
  return (const struct ActorPlacement *)gData_02001378;
}

const struct Trigger *Func_02000e58(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room1)
    return (const struct Trigger *)gData_0200154c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room2)
    return (const struct Trigger *)gData_02001594;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room3)
    return (const struct Trigger *)gData_020015c4;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room4)
    return (const struct Trigger *)gData_020016cc;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room5)
    return (const struct Trigger *)gData_02001714;
  return (const struct Trigger *)gData_02001540;
}

#include "version.h"

#if !TLA_PATCH_1
#define CATACOMBS_SCREEN_EFFECT 513
#else
#define CATACOMBS_SCREEN_EFFECT 516
#endif

extern char SceneId_Ov671Room7, SceneId_Ov672Room1;
void Func_02001254(s32 a);
void Func_0200126c(s32 id);

s32 Func_02000ecc(void) {
  s32 entrance;
  struct Actor *statue;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = CATACOMBS_SCREEN_EFFECT;
  Func_02001254(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room1 &&
      (entrance = gMainRoutineWork.entrance) == 1) {
    if (GameFlag_Test(0xa27)) {
      gMainRoutineWork.retreatScene = (s32)&SceneId_Ov671Room7;
      gMainRoutineWork.retreatEntrance = entrance;
    } else if (GameFlag_Test(0xa28)) {
      gMainRoutineWork.retreatScene = (s32)&SceneId_Ov672Room1;
      gMainRoutineWork.retreatEntrance = 3;
    }
  }
  if (!GameFlag_Test(265) &&
      gMainRoutineWork.scene == (s32)&SceneId_Ov673Room2 &&
      gMainRoutineWork.entrance > 2)
    Func_0200126c(gMainRoutineWork.currentOwner);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room3) {
    if (GameFlag_Test(0x200))
      MapGrid_CopyCellAttributes(6, 23, 1, 1, 8, 23);
    if (GameFlag_Test(513))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 15);
    if (GameFlag_Test(514))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 9, 0x10);
    if (GameFlag_Test(515))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 17);
    if (GameFlag_Test(516))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 17);
    if (GameFlag_Test(517))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 5, 0x10);
    if (GameFlag_Test(518))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 15);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room5 &&
      GameFlag_Test(0x200))
    MapGrid_CopyCellAttributes(15, 18, 1, 1, 17, 18);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room4 &&
      GameFlag_Test(0x9ce) && !GameFlag_Test(0xf93) && !GameFlag_Test(265)) {
    statue = Actor_GetById(0x40);
    Actor_SetPosition(0x40, 0xa80000, 0x980000);
    statue->motionFlags = 6;
    statue->terrainHeight += 0x100000;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

