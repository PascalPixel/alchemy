/// @file
/// The scene code of overlay 0x660 that its scene tables belong to, in ROM order; the rest of the overlay is in garoh_interiors/.

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
#include "garoh_interiors/garoh_interiors.h"
#include "event_inline.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const u8 *Func_0200004c(void) {
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

  u16 facing = (leader->angle + 0x2000) & ~0x3fff;

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

  u16 facing = (leader->angle + 0x2000) & ~0x3fff;

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

  u16 facing = (leader->angle + 0x2000) & ~0x3fff;

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

  u16 facing = (leader->angle + 0x2000) & ~0x3fff;

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

  facing = ((Actor_GetById(gMainRoutineWork.currentOwner)->angle) + 0x2000) &
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

  Func_020007f4();
  Event_ResolvePendingActions(0);
  message = (s32)MsgGarohInteriors01;
  Event_SetMessageId(message);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Event_SetMessageId(message + 2);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
      Event_ShowMessageAndWait(actor, 0);
    } else {
      ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
      Event_ShowMessageAndWait(actor, 0);
    }
  } else {
    Event_ShowMessageAndWait(actor, 0);
  }
  Func_020007fc();
}

void Func_0200039c(void) {
  Func_020007f4();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x91b))
    Event_SetMessageId((s32)MsgGarohInteriors60);
  else
    Event_SetMessageId((s32)MsgGarohInteriors25);
  Event_ShowMessageAndWait(17, 0);
  Func_020007fc();
}

void Func_020003dc(void) {
  Func_020007f4();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x91b))
    Event_SetMessageId((s32)MsgGarohInteriors61);
  else
    Event_SetMessageId((s32)MsgGarohInteriors26);
  Event_ShowMessageAndWait(18, 0);
  Func_020007fc();
}

void Func_0200041c(void) {
  Func_020007f4();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x91b))
    Event_SetMessageId((s32)MsgGarohInteriors64);
  else
    Event_SetMessageId((s32)MsgGarohInteriors29);
  Event_ShowMessageAndWait(17, 0);
  Func_020007fc();
}

void Func_0200045c(void) {
  Func_020007f4();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x91b))
    Event_SetMessageId((s32)MsgGarohInteriors65);
  else
    Event_SetMessageId((s32)MsgGarohInteriors30);
  Event_ShowMessageAndWait(18, 0);
  Func_020007fc();
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
const u8 *Func_020004f0(void) {
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

void Func_02000710(void);
void Func_020007dc(s32 value);
void Func_0200087c(s32 value);

s32 Func_02000580(void) {
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
      Func_080d3928(17, 0xe000);
      Actor_SetSingleChildFlags(Actor_GetById(17), 0);
      Func_080d3928(18, turn);
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
  Func_020007f4();
  Event_ResolvePendingActions(0);
  Event_Wait(30);
  Func_020007cc();
  Sound_Play(86);
  Func_0803ce64((s32)MsgGarohInteriors33, 0, 0);
  Event_Wait(20);
  Func_020007d4();
  Func_020007bc();
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
  Func_020007fc();
}
