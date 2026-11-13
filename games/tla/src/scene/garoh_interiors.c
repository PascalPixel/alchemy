/// @file
/// The scene code of overlay 0x660, in ROM order.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "event_inline.h"

struct EventWork1c4 {
  u8 unk000[0x1c4];
  u16 count;
};

extern struct ActorPlacement gData_0200094c[];
extern struct ActorPlacement gData_02000964[];
extern struct ActorPlacement gData_02000ab4[];
extern struct ActorPlacement gData_02000b2c[];
extern struct Trigger gData_02000b5c[];
extern struct Trigger gData_02000b80[];
extern struct Trigger gData_02000d18[];
extern struct Trigger gData_02000de4[];
extern struct Trigger gData_02000e44[];
extern struct Trigger gData_02000fe8[];

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Map_CenterOnFollowedPoint(void);
void Func_020007cc(void);
void Func_020007d4(void);
void Event_End(void);
void Func_0200089c(s32 slot, s32 actor);
void Func_020008a4(s32 id);
void Func_020008ac(s32 slot, s32 actor);
#include "event_inline.h"
#include "module/scene/garoh_interiors.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov660Room1)
    return gData_02000964;
  if (scene == (s32)&SceneId_Ov660Room2)
    return gData_02000ab4;
  if (scene == (s32)&SceneId_Ov660Room3)
    return gData_02000b2c;
  return gData_0200094c;
}

void Func_0200009c(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->facing + 0x2000) & ~0x3fff;

  if (facing == 0xc000 && GameFlag_Test(0x91b)) {
    Func_0200089c(10, actor);
  } else {
    if (GameFlag_Test(0x91b))
      Event_SetMessageId((s32)MsgGarohInteriors52);
    else
      Event_SetMessageId((s32)MsgGarohInteriors17);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_02000114(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->facing + 0x2000) & ~0x3fff;

  if (facing == 0xc000 && GameFlag_Test(0x91b)) {
    Func_0200089c(11, actor);
  } else {
    if (GameFlag_Test(0x91b))
      Event_SetMessageId((s32)MsgGarohInteriors54);
    else
      Event_SetMessageId((s32)MsgGarohInteriors19);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_0200018c(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->facing + 0x2000) & ~0x3fff;

  if (facing == 0xc000) {
    Func_0200089c(12, actor);
  } else if (GameFlag_Test(0x91b)) {
    Event_SetMessageId((s32)MsgGarohInteriors56);
    Event_ShowMessageAndWait(actor, 0);
  } else {
    s32 message = (s32)MsgGarohInteriors21;

    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(message + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(message + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_02000230(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->facing + 0x2000) & ~0x3fff;

  if (facing == 0xc000) {
    Func_020008ac(3, actor);
  } else {
    if (GameFlag_Test(0x91b))
      Event_SetMessageId((s32)MsgGarohInteriors58);
    else
      Event_SetMessageId((s32)MsgGarohInteriors23);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_02000298(void) {
  s16 facing;

  facing = ((Actor_GetById(gMainRoutineWork.currentOwner)->facing) + 0x2000) &
           ~0x3fff;
  Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
  if ((u16)facing == 0xc000) {
    Func_020008a4(8);
  } else {
    if (GameFlag_Test(0x91b))
      Event_SetMessageId((s32)MsgGarohInteriors66);
    else
      Event_SetMessageId((s32)MsgGarohInteriors31);
    Event_ShowMessageAndWait(8, 0);
  }
  Actor_FaceDirection(8, FACING_SOUTH, 0);
}

void Func_02000318(s32 actor) {
  s32 message;

  Event_Start();
  Event_ResolvePendingActions(0);
  message = (s32)MsgGarohInteriors01;
  Event_SetMessageId(message);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Event_SetMessageId(message + 2);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Event_ShowMessageAndWait(actor, 0);
    } else {
      ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
      Event_ShowMessageAndWait(actor, 0);
    }
  } else {
    Event_ShowMessageAndWait(actor, 0);
  }
  Event_End();
}

void Func_0200039c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x91b))
    Event_SetMessageId((s32)MsgGarohInteriors60);
  else
    Event_SetMessageId((s32)MsgGarohInteriors25);
  Event_ShowMessageAndWait(17, 0);
  Event_End();
}

void Func_020003dc(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x91b))
    Event_SetMessageId((s32)MsgGarohInteriors61);
  else
    Event_SetMessageId((s32)MsgGarohInteriors26);
  Event_ShowMessageAndWait(18, 0);
  Event_End();
}

void Func_0200041c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x91b))
    Event_SetMessageId((s32)MsgGarohInteriors64);
  else
    Event_SetMessageId((s32)MsgGarohInteriors29);
  Event_ShowMessageAndWait(17, 0);
  Event_End();
}

void Func_0200045c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x91b))
    Event_SetMessageId((s32)MsgGarohInteriors65);
  else
    Event_SetMessageId((s32)MsgGarohInteriors30);
  Event_ShowMessageAndWait(18, 0);
  Event_End();
}

void Func_0200049c(void) {
  s32 message = (s32)MsgGarohInteriors47;

  Event_SetMessageId(message);
  FieldMessage_ShowAtActor(11, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(message + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(message + 2);
  }
  Event_ShowMessageAndWait(11, 0);
  GameFlag_SetBit(0x91a);
}

/// Returns the room's table, chosen by flag 0x91B, the scene and, in the
/// first room, the entrance.
const struct Trigger *Scene_GetEvents(void) {
  s32 moved = GameFlag_Test(0x91b);
  struct MainRoutineWork *party = &gMainRoutineWork;
  s32 room1 = (s32)&SceneId_Ov660Room1;
  s32 scene;

  if (moved) {
    scene = party->scene;
    if (scene == room1)
      return gData_02000e44;
    if (scene == (s32)&SceneId_Ov660Room2)
      return gData_02000fe8;
  } else {
    scene = party->scene;
    if (scene == room1) {
      if (party->entrance == 99)
        return gData_02000e44;
      return gData_02000b80;
    }
    if (scene == (s32)&SceneId_Ov660Room2)
      return gData_02000d18;
  }
  if (scene == (s32)&SceneId_Ov660Room3)
    return gData_02000de4;
  return gData_02000b5c;
}

s32 Scene_Initialize(void) {
  s32 turn;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Func_0200087c(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov660Room1) {
    if (GameFlag_Test(0x91b) || gMainRoutineWork.entrance == 99) {
      Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
      Actor_SetMovementTypeById(18, 3);
    } else {
      {
        s32 x;
        s32 z;
        s32 id;

        turn = 0x2000;
        x = 153;
        id = 17;
        /* FAKEMATCH(barrier): the forced id keeps its load right after that
         * half; as no asm, 2 of 158 instructions differ in tla-en. */
        asm volatile("" : "+l"(id));
        x <<= 17;
        z = 335 << 16;
        Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, turn);
      }
      {
        s32 angle = 0x6000;
        s32 z = 336 << 16;

        Actor_SetHorizontalPositionAndAngleWithTerrain(18, 345 << 16, z, angle);
      }
      Actor_SetSingleChildFlags(Actor_GetById(19), 0);
      Actor_SetSingleChildFlags(Actor_GetById(20), 0);
      Actor_GetById(17)->motionFlags = 0;
      Actor_GetById(17)->y = 0;
      Actor_GetById(18)->motionFlags = 0;
      Actor_GetById(18)->y = 0;
      Actor_GetById(19)->motionFlags = 0;
      Actor_GetById(19)->y = 0;
      Actor_GetById(20)->motionFlags = 0;
      Actor_GetById(20)->y = 0;
      Actor_SetSpriteRotationById(17, 0xe000);
      Actor_SetSingleChildFlags(Actor_GetById(17), 0);
      Actor_SetSpriteRotationById(18, turn);
      Actor_SetSingleChildFlags(Actor_GetById(18), 0);
    }
    if (gMainRoutineWork.entrance == 99 && !GameFlag_Test(0x91b))
      Func_02000710();
  }
  if (GameFlag_Test(0x91b))
    GameFlag_ClearBit(0x310);
  if (GameFlag_Test(0x310))
    Func_020007dc(0xa00);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02000710(void) {
  GameFlag_SetBit(0x91b);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_Wait(30);
  Func_020007cc();
  Sound_Play(86);
  FieldMessage_ShowCentered((s32)MsgGarohInteriors33, 0, 0);
  Event_Wait(20);
  Func_020007d4();
  Map_CenterOnFollowedPoint();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId((s32)MsgGarohInteriors34);
  Event_ShowMessageThenWait(16, 0, 10);
  Motion_SetVarCbAndRefresh(15, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(15, 0, 10);
  Motion_SetModeAndWaitAnimation(15, 3);
  Event_Wait(30);
  Event_ShowMessageThenWait(15, 0, 10);
  Event_End();
}

#include "text/msg_ids.h"

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(64),
    SCENE_EXIT_IF(1, 59, 1, -1),
    SCENE_EXIT_IF(3, 59, 3, -1),
    SCENE_EXIT_IF(4, 59, 4, -1),
    SCENE_EXIT_IF(5, 59, 5, -1),
    SCENE_EXIT_IF(6, 62, 7, -1),
    SCENE_EXIT_IF(7, 62, 8, -1),
    SCENE_EXITS(65),
    SCENE_EXIT_IF(3, 62, 5, -1),
    SCENE_EXIT_IF(4, 59, 2, -1),
    SCENE_EXIT_IF(5, 62, 6, -1),
    SCENE_EXITS(66),
    SCENE_EXIT_IF(2, 59, 6, -1),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gData_0200094c[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02000964[] = {
    {192, CONDITION_ALWAYS, 2, PIXELS(336), 0, PIXELS(120), FACING_SOUTH, 0, 0},
    {190, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(104), FACING_WEST, 0, 0},
    {191, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(136), FACING_NORTH, 1, 0},
    {188, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(136), FACING_SOUTHEAST, 0, 0},
    {194, CONDITION_ALWAYS, 1, PIXELS(592), 0, PIXELS(96), FACING_SOUTH, 1, 0},
    {205, CONDITION_ALWAYS, 1, PIXELS(134), 0, PIXELS(120), FACING_SOUTH, 1, 0},
    {105, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(120), FACING_SOUTH, 1, 0},
    {199, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(424), FACING_SOUTH, 1, 0},
    {209, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(420), FACING_WEST, 0, 0},
    {239, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(344), FACING_NORTHEAST, 1, 0},
    {239, CONDITION_ALWAYS, 1, PIXELS(330), 0, PIXELS(376), FACING_SOUTHWEST, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(336), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(352), 0, PIXELS(344), FACING_SOUTHWEST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02000ab4[] = {
    {185, CONDITION_ALWAYS, 1, PIXELS(372), 0, PIXELS(406), FACING_SOUTHEAST, 1, 0},
    {196, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(424), FACING_NORTHWEST, 0, 0},
    {186, CONDITION_ALWAYS, 1, PIXELS(352), 0, PIXELS(456), FACING_SOUTH, 1, 0},
    {198, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(412), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02000b2c[] = {
    {202, CONDITION_ALWAYS, 1, PIXELS(448), 0, PIXELS(152), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_02000b5c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gData_02000b80[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {0x31, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Func_02000318)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors02)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors03)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors04)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors05)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors06)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors13)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors14)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors16)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(Func_0200009c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors18)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(Func_02000114)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors20)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(Func_02000230)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors27)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors28)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(Func_0200039c)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(Func_020003dc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     EVENT(Func_0200041c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     EVENT(Func_0200045c)},
    {0x173, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors68))},
    {0x173, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors69))},
    {0x173, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors71))},
    {0x173, 213, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors72))},
    {TRIGGERS_END},
};

struct Trigger gData_02000d18[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {0x31, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors07)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors08)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors09)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors10)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors11)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors12)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(Func_0200018c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors22)},
    {0x173, 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors70))},
    {TRIGGERS_END},
};

struct Trigger gData_02000de4[] = {
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Func_02000298)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x91b,
     MSGNO(MsgGarohInteriors32)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors67)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Func_02000298)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x91b,
     MSGNO(MsgGarohInteriors32)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors67)},
    {TRIGGERS_END},
};

struct Trigger gData_02000e44[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors35)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors36)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors37)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors38)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors39)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors40)},
    {TRIGGER_TALK, 11, 0x91a, EVENT(Func_0200049c)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors48)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors49)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors50)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors51)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(Func_0200009c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors53)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(Func_02000114)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors55)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(Func_02000230)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors59)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors62)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors63)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(Func_0200039c)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Func_020003dc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     EVENT(Func_0200041c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     EVENT(Func_0200045c)},
    {0x173, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors68))},
    {0x173, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors69))},
    {0x173, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors71))},
    {0x173, 213, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors72))},
    {TRIGGERS_END},
};

struct Trigger gData_02000fe8[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {0x31, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors41)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors42)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgGarohInteriors43)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors44)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors45)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors46)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(Func_0200018c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors57)},
    {0x173, 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors70))},
    {TRIGGERS_END},
};
