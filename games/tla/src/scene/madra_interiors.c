/// @file
/// Madra's houses: the inn, the shops, the elder's home with the Black Crystal
/// on its mantel, the jail where Piers is questioned (entrance 20), and, as a
/// second scene, the left cavern home. The old couple trade Char the Mars
/// Djinni for the Healing Fungus, and on the return with Piers the elder's
/// household welcomes him home.

#include "shared/tables.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "module/scene/scene.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "version.h"
#include "field_sprite.h"
#include "event_work.h"
#include "objcmd.h"

/// What games/tla/src/scene/ declares for the files that use it, written once.

extern s32 gMadraInteriors_InterrogatorLifted[];
extern s32 gMadraInteriors_InterrogatorTopples[];
void MadraInteriors_SayGoodbyeAgain(void);

void MadraInteriors_AskToStayLonger(void);

extern u32 gMadraInteriors_OldManWalksToBackRoom[];
extern u32 gMadraInteriors_WifeComesBack[];
extern u32 gMadraInteriors_OldManComesBack[];

extern u32 gMadraInteriors_CharFollows[];

void Func_02001ef8();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern struct ActorPlacement gMadraInteriors_Placements[];
extern struct ActorPlacement gMadraInteriors_PlacementsOnReturn[];
extern struct ActorPlacement gMadraInteriors_JailPlacements[];
extern struct ActorPlacement gMadraInteriors_SecondRoomsPlacements[];
extern struct Trigger gMadraInteriors_TriggersBeforeRaid[];
extern struct Trigger gMadraInteriors_JailTriggers[];
extern struct Trigger gMadraInteriors_SecondRoomsTriggers[];
extern struct Trigger gMadraInteriors_TriggersAfterRaid[];
extern struct Trigger gMadraInteriors_TriggersOnReturn[];

void Event_End(void);
void Sanctum_Run(s32 actor);
void Inn_CheckIn(s32 slot, s32 actor);
void MadraInteriors_InterrogatePiers(void);

#include "text/msg_ids.h"

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov659Room2)
    return gMadraInteriors_SecondRoomsPlacements;
  if (gMainRoutineWork.entrance == 20)
    return gMadraInteriors_JailPlacements;
  if (Event_GetFlag(0x8ff))
    return gMadraInteriors_PlacementsOnReturn;
  return gMadraInteriors_Placements;
}

/// "Indra is in direct contact with southern Gondowan... Will we be safe?"
void MadraInteriors_AskWillWeBeSafe(void) {
  s32 msg;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  msg = (s32)MsgMadraInteriors06;
  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(9, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(9, 0);
  Event_End();
}

/// Madra's innkeeper takes the party in across the counter (inn 1), and
/// otherwise talks of the week's troubles, the Kibombo, and later the dead fish
/// on the shore, which from patch 1 is a question.
void MadraInteriors_TalkToInnkeeper(void) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Inn_CheckIn(1, 22);
  } else {
#if !TLA_PATCH_1
    // Before patch 1 there is no choice prompt.
    if (Event_GetFlag(0x8ff))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors155);
    else if (Event_GetFlag(0x8ab))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors87);
    else
      FieldEvent_SetMessageId((s32)MsgMadraInteriors39);
    Event_ShowMessage(22, 0);
#else
    if (Event_GetFlag(0x8ff)) {
      s32 msg = (s32)MsgMadraInteriors155;

      FieldEvent_SetMessageId(msg);
      Event_OpenMessage(22, 0);
      if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(),
                                                0) == 0) {
        FieldEvent_Wait(10);
        FieldEvent_SetMessageId(msg + 1);
      } else {
        FieldEvent_Wait(20);
        FieldEvent_SetMessageId(msg + 2);
      }
      Event_ShowMessage(22, 0);
    } else {
      if (Event_GetFlag(0x8ab))
        FieldEvent_SetMessageId((s32)MsgMadraInteriors87);
      else
        FieldEvent_SetMessageId((s32)MsgMadraInteriors39);
      Event_ShowMessage(22, 0);
    }
#endif
  }
}

/// Madra's sanctum healer heals across the counter, and otherwise talks of the
/// town's trials, the Kibombo, and later the warming sea.
void MadraInteriors_TalkToHealer(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Sanctum_Run(actor);
  } else {
    if (Event_GetFlag(0x8ff))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors149);
    else if (Event_GetFlag(0x8ab))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors81);
    else
      FieldEvent_SetMessageId((s32)MsgMadraInteriors33);
    Event_ShowMessage(actor, 0);
  }
}

/// "Don't touch that gem! The elder gave us strict orders to protect it!"
void MadraInteriors_GuardElderGem(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(14, 4, 0);
  FieldActor_Jump(14, 6, 20);
  Actor_FaceActor(4, 14, 0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors18);
  Event_ShowMessage(14, 0);
  Actor_FaceDirection(14, FACING_SOUTHEAST, 0);
  Event_End();
}

/// The old man who loves the rare mushrooms of the Gondowan Cliffs, before and
/// after the party brings him one.
void MadraInteriors_TalkToMushroomLover(void) {
  s32 msg;

  if (Event_GetFlag(0x98a)) {
    if (Event_GetFlag(0x310))
      msg = (s32)MsgMadraInteriors111;
    else
      msg = (s32)MsgMadraInteriors118;
    FieldEvent_SetMessageId(msg);
    Event_ShowMessage(10, 0);
  } else {
    if (Event_GetFlag(0x210)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors116);
      Event_ShowMessage(10, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors106);
      Event_ShowMessage(10, 0);
    }
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 346) = 1;
  }
}

/// His wife, who cooks them.
void MadraInteriors_TalkToMushroomCook(void) {
  if (Event_GetFlag(0x98a)) {
    if (Event_GetFlag(0x310)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors112);
      Event_ShowMessage(11, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors119);
      Event_ShowMessage(11, 0);
    }
  } else if (Event_GetFlag(0x210)) {
    FieldEvent_SetMessageId((s32)MsgMadraInteriors117);
    Event_ShowMessage(11, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgMadraInteriors107);
    Event_ShowMessage(11, 0);
  }
}

void MadraInteriors_MindReadMushroomLover(void) {
  if (Event_GetFlag(0x98a)) {
    if (Event_GetFlag(0x310)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors113);
      Event_ShowMessage(10, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors120);
      Event_ShowMessage(10, 0);
    }
  } else {
    s32 flag = Event_GetFlag(0x210);
    s32 msg = (s32)MsgMadraInteriors108;

    if (flag) {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(10, 0);
    } else {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(10, 0);
    }
  }
}

void MadraInteriors_MindReadMushroomCook(void) {
  if (Event_GetFlag(0x98a)) {
    if (Event_GetFlag(0x310)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors114);
      Event_ShowMessage(11, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors121);
      Event_ShowMessage(11, 0);
    }
  } else {
    s32 flag = Event_GetFlag(0x210);
    s32 msg = (s32)MsgMadraInteriors109;

    if (flag) {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(11, 0);
    } else {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(11, 0);
    }
  }
}

/// The ill man in the cavern home groans, the first time, and thinks of the ice
/// he saw made by thinking.
/// "Uhhhnnn... UHHHNNN!"
void MadraInteriors_TalkToIllMan(void) {
  if (!Event_GetFlag(0x211)) {
    Event_SetFlag(0x211);
    FieldEvent_SetMessageId((s32)MsgUhhhnnnUhhhnnn2);
    Actor_WalkByAndWait(14, -12, 0);
    FieldEvent_Wait(20);
    Actor_RunRepeatedMotion(14, 2);
    FieldEvent_Wait(20);
    Event_ShowMessage(14, 0);
    FieldActor_ShowEmote(14, 265, 30);
    FieldEvent_Wait(20);
    Actor_WalkByAndWait(14, 12, 0);
    Actor_FaceActor(14, 4, 0);
    Event_ShowMessage(14, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgMadraInteriors97);
    Actor_FaceActor(14, 4, 0);
    Event_ShowMessage(14, 0);
  }
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov659Room2)
    return gMadraInteriors_SecondRoomsTriggers;
  if (gMainRoutineWork.entrance == 20)
    return gMadraInteriors_JailTriggers;
  if (Event_GetFlag(0x8ff))
    return gMadraInteriors_TriggersOnReturn;
  if (Event_GetFlag(0x8ab))
    return gMadraInteriors_TriggersAfterRaid;
  return gMadraInteriors_TriggersBeforeRaid;
}

/// The Black Crystal (item 242) sits on the elder's mantel until it is stolen
/// (flag 0x8ab), after which its actor is put out of sight.
void MadraInteriors_PlaceBlackCrystal(void) {
  if (Event_GetFlag(0x8ab)) {
    Actor_SetPosition(26, PIXELS(888), PIXELS(8));
  } else {
    Actor_SetItemIcon(Actor_Get(26), 242);
    Actor_SetSpriteFlags(Actor_Get(26), 0);
  }
}

/// Places an actor facing an angle, the angle taken as the u16 an actor's
/// facing is; the jail's four placements, named by us.
static inline void MadraInteriors_PlaceFacing(s32 actorId, s32 x, s32 z,
                                              u16 facing) {
  Actor_SetPositionAndFacing(actorId, x, z, facing);
}

/// The jail on entrance 20: Piers' interrogation the first time, then his
/// interrogators by the story flags.
void MadraInteriors_SetUpJail(void) {
  if (!Event_GetFlag(0x8ab)) {
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 0xd4c) = 3;
    if (!Event_GetFlag(0x911)) {
      MadraInteriors_InterrogatePiers();
    } else {
      Actor_SetPosition(9, 0, 0);
      Actor_SetPosition(10, 0, 0);
    }
  } else if (Event_GetFlag(0x8ff)) {
    MadraInteriors_PlaceFacing(9, PIXELS(515), PIXELS(841), FACING_NORTHEAST);
    MadraInteriors_PlaceFacing(10, PIXELS(472), PIXELS(877), FACING_NORTH);
    Actor_SetPosition(8, 0, 0);
  } else {
    MadraInteriors_PlaceFacing(9, PIXELS(515), PIXELS(841), FACING_NORTHWEST);
    MadraInteriors_PlaceFacing(10, PIXELS(472), PIXELS(877), FACING_NORTH);
    Actor_SetPosition(8, 0, 0);
  }
}

extern u32 gMadraInteriors_InterrogatorsLeave[];
void FieldCamera_WaitForArrival();
void BattleEffect_LoadActionEffectResources();
void FieldPsynergy_SetupActorPair();
void BattleEffect_StopMainActor();

/// The rooms' setup: the leader's sprite priority, the two rooms' actors and
/// the sprite of actor 10, by the story flags.
s32 Scene_Initialize(void) {
  struct FieldSprite *sprite;
  void *event;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov659Room1) {
    event = Ram_HeapSlots->eventWork;
    FIELD_AT(event, u16, 0xd4c) = 3;
    if (!Event_GetFlag(0x821)) {
      Actor_SetPosition(29, 0, 0);
      Actor_SetPosition(30, 0, 0);
    } else {
      FieldActor_SetSpriteColor(Actor_Get(29), 3);
      FieldActor_SetSpriteColor(Actor_Get(30), 5);
    }
    if (Event_GetFlag(0x8ab)) {
      MapGrid_CopyCellsLayered(2, 110, 16, 10, 16, 64);
      MapGrid_CopyCellAttributes(2, 46, 16, 10, 16, 0);
    }
  }
  if (gMainRoutineWork.entrance == 20)
    MadraInteriors_SetUpJail();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov659Room2) {
    if (Event_GetFlag(0x8ab)) {
      Actor_SetPosition(10, 0, 0);
    } else {
      Actor_SetSpriteRotationById(10, 0xc000);
      Actor_SetSpriteFlags(Actor_Get(10), 0);
      ((struct FieldSprite *)Actor_Get(10)->sprite)->nextFrame |= 8;
      sprite = Actor_Get(10)->sprite;
      sprite->unk11 = (sprite->unk11 & 3) | 32;
      ((struct FieldSprite *)Actor_Get(10)->sprite)->unk19 = 1;
    }
  }
  MadraInteriors_PlaceBlackCrystal();
  if (Event_GetFlag(0x8ff))
    FieldActor_SetSpriteColor(Actor_Get(33), 2);
  else
    FieldActor_SetSpriteColor(Actor_Get(28), 2);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Tips an actor's sprite a thirty-second of a turn, a frame of the
/// interrogator toppling over (gMadraInteriors_InterrogatorTopples).
s32 MadraInteriors_TiltSprite(struct Actor *object) {
  struct FieldSprite *sprite;

  sprite = object->sprite;
  if (object != NULL && sprite != NULL)
    sprite->rotation -= 0x800;
  return 0;
}

/// Unused: stands an actor's sprite upright again; no script calls it.
UNUSED s32 MadraInteriors_StraightenSprite(struct Actor *object) {
  struct FieldSprite *sprite;

  sprite = object->sprite;
  if (object != NULL && sprite != NULL)
    sprite->rotation = 0;
  return 0;
}

/// The party enters the jail as Piers is questioned, and sees him drive off an
/// interrogator with Psynergy.
/// "Admit it! You're one of the Champa, aren't you, Piers?"
void MadraInteriors_InterrogatePiers(void) {
  s32 i;
  struct Actor *object0;
  struct Actor *object1;
  struct Actor *object2;
  struct Actor *object3;
  struct Actor *object4;
  struct Actor *object5;
  struct Actor *object6;
  struct Actor *object7;
  struct Actor *object8;
  struct Actor *object9;
  struct Actor *object10;
  struct Actor *object11;
  struct Actor *object12;
  s32 result13;
  struct Actor *object14;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(8, 1);
  Actor_SetPosition(4, PIXELS(400), PIXELS(888));
  FieldCamera_MoveTo(PIXELS(408), -1, PIXELS(848), 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors28);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_WalkToAndWait(4, 396, 869);
  FieldActor_LaunchFromLeader(11, 25, 19, FACING_NORTH);
  FieldActor_RefreshSelectorById(11);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(11, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_FaceDirection(11, FACING_NORTHEAST, 0);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(PIXELS(488), -1, PIXELS(820), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(20);
  Actor_ShowEmote(9, 0x107, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  Actor_WalkByAndWait(7, 0, -16);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldActor_ShowEmote(9, 0x103, 0);
  object0 = Actor_Get(9);
  object0->actionFlags &= 254;
  for (i = 2; i >= 0; i--) {
    Actor_WalkByAndWait(9, 0, -8);
    Event_PlaySound(134);
    Actor_WalkByAndWait(9, 0, 8);
  }
  object1 = Actor_Get(9);
  object1->actionFlags |= 1;
  FieldEvent_Wait(30);
  Actor_ShowEmote(7, 0x102, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(7, 0x107, 30);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(9, 0x108, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_FaceActor(10, 9, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_FaceActor(9, 10, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldActor_ShowEmote(9, 0x107, 20);
  Actor_FaceDirectionAndWait(9, FACING_NORTH);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(10, 0x102, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(7, 0, 16);
  BattleEffect_LoadActionEffectResources(24, 1);
  FieldPsynergy_SetupActorPair(7, 8);
  EventObject_Initialize();
  Actor_ShowEmote(9, 0x101, 30);
  Actor_FaceActor(9, 7, 0);
  Actor_FaceActor(10, 7, 0);
  object2 = Actor_Get(9);
  Actor_SetSpriteFlags(object2, 0);
  Actor_SetScript(9, gMadraInteriors_InterrogatorLifted);
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  BattleEffect_CleanupSceneObjects();
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  FieldActor_Stop(9);
  object3 = Actor_Get(9);
  object3->scaleX = 0x10000;
  object4 = Actor_Get(9);
  object4->scaleY = 0x10000;
  FieldEvent_Wait(4);
  object5 = Actor_Get(9);
  object5->motionFlags = 2;
  object6 = Actor_Get(9);
  object6->actionFlags &= 254;
  Actor_SetDestinationOffset(9, -8, 0);
  Actor_SetScript(9, gMadraInteriors_InterrogatorTopples);
  FieldActor_RefreshSelectorById(9);
  Event_PlaySound(127);
  FieldEvent_Wait(60);
  object7 = Actor_Get(9);
  object7->actionFlags = (object7->actionFlags | 1);
  FieldActor_Jump(9, 6, 0);
  FieldEvent_Wait(3);
  Actor_SetSpriteRotationById(9, 0);
  object8 = Actor_Get(9);
  Actor_SetSpriteFlags(object8, 1);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(10, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(10, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(10, 0x100, 30);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldActor_Jump(9, 6, 20);
  FieldEvent_Wait(20);
  Actor_FaceActor(9, 7, 0);
  FieldEvent_Wait(20);
  object9 = Actor_Get(10);
  object9->actionFlags &= 254;
  object10 = Actor_Get(9);
  object10->actionFlags &= 254;
  Actor_WalkByAndWait(9, -16, 16);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldActor_Jump(9, 6, 20);
  FieldEvent_Wait(20);
  object11 = Actor_Get(9);
  object11->actionFlags = (object11->actionFlags | 1);
  Actor_SetScript(9, gMadraInteriors_InterrogatorsLeave);
  FieldActor_WatchAndSetScript(4, 9);
  FieldActor_WatchAndSetScript(11, 9);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(20);
  Actor_FaceActor(10, 7, 0);
  FieldActor_WatchAndSetScript(4, 10);
  FieldActor_WatchAndSetScript(11, 10);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(7, 0x105, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(10, 0x102, 30);
  Actor_WalkByAndWait(10, -16, 16);
  FieldEvent_ShowMessageThenWait(10, 0, 1);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, -16, 8);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  object12 = Actor_Get(10);
  object12->actionFlags |= 1;
  Actor_SetScript(10, gMadraInteriors_InterrogatorsLeave);
  FieldEvent_Wait(40);
  FieldActor_Stop(4);
  FieldActor_Stop(11);
  FieldActor_RefreshSelectorById(10);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, 0, 0);
  Actor_SetAnimation(8, 6);
  Actor_SetSpriteColorById(8, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_WaitForAnimationChange(8);
  EventCamera_FollowActor(4, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceEachOther(11, 4, 0);
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(20);
  Event_OpenMessage(11, 0);
  result13 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result13 != 0)
    goto answeredNo;
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  goto answered;
answeredNo:
  Actor_SetAnimationAndWait(11, 4);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  FieldEvent_ShowMessageThenWait(11, 0, 10);
answered:
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimationAndWait(11, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimation(11, 2);
  object14 = Actor_Get(4);
  if (object14 == 0)
    goto piersLeft;
  Actor_SetDestination(11, *(s16 *)((u8 *)object14 + 10),
                       *(s16 *)((u8 *)object14 + 18));
piersLeft:
  FieldActor_WaitForMoveById(11);
  Actor_SetPosition(11, 0, 0);
  Event_SetFlag(0x911);
  Event_End();
  return;
}

/// "Ah... When will that kindly elder return and end my imprisonment?"
void MadraInteriors_TalkToPiers(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors29);
  Actor_FaceDirection(11, FACING_SOUTH, 0);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Event_End();
  return;
}

/// Piers feels the Mind Read.
/// "Why did I use my Psynergy in anger? I must maintain control..."
void MadraInteriors_MindReadPiers(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors30);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldEvent_Wait(10);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, FACING_EAST, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(7, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Event_End();
  return;
}

/// Unused: Piers' Psynergy at actor 8 alone, as the interrogation plays it;
/// nothing calls it.
UNUSED void MadraInteriors_PiersUsesPsynergy(void) {
  BattleEffect_LoadActionEffectResources(24, 1);
  FieldPsynergy_SetupActorPair(7, 8);
  EventObject_Initialize();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  BattleEffect_CleanupSceneObjects();
  return;
}

extern u32 gMadraInteriors_WifeWalksToBackRoom[];
extern u32 gMadraInteriors_LeaderStepsAside[];

/// The couple are given the Healing Fungus from the Gondowan Cliffs, his
/// favourite, and trade the party Char the Mars Djinni for it.
/// "Oh, my... Is... Is that..."
void MadraInteriors_GiveHealingFungus(void) {
  struct Actor *object;

  Event_SetFlag(0x98a);
  Event_SetFlag(0x310);
  PartyInventory_Remove(0x1c0);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(11, 0x6666, 0x3333);
  Actor_WalkTo(11, 410, 109);
  object = Actor_Get(4);
  if (object->z > 0x5f0000)
    Actor_WalkToAndWait(4, object->x / 0x10000, 95);
  Actor_WalkToAndWait(4, 362, 95);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldActor_WaitForMoveById(11);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors110);
  Actor_FaceActor(10, 4, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(10, 0x102, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(11, 0x100, 3);
  Actor_WalkToAndWait(11, 378, 109);
  Actor_FaceDirection(11, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Actor_FaceEachOther(10, 11, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(10, 4, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(10, 0x108, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_ShowEmote(11, 0x102, 30);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Actor_FaceActor(10, 11, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(10, 3);
  FieldEvent_Wait(30);
  Actor_ShowEmote(10, 0x101, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Actor_FaceDirectionAndWait(11, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(11, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(11, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(11, 0x106, 30);
  FieldActor_Stop(25);
  Actor_SetSpeed(10, 0x8000, 0x4000);
  Actor_SetSpeed(11, 0x8000, 0x4000);
  Actor_SetSpeed(25, 0x8000, 0x4000);
  Actor_SetScript(11, (s32)gMadraInteriors_WifeWalksToBackRoom);
  Actor_SetScript(4, (s32)gMadraInteriors_LeaderStepsAside);
  FieldEvent_Wait(60);
  Actor_SetScript(10, (s32)gMadraInteriors_OldManWalksToBackRoom);
  FieldActor_RefreshSelectorById(10);
  FieldEvent_Wait(120);
  Actor_SetScript(11, (s32)gMadraInteriors_WifeComesBack);
  FieldActor_WatchAndSetScript(4, 11);
  FieldEvent_Wait(60);
  Actor_SetScript(10, (s32)gMadraInteriors_OldManComesBack);
  Actor_SetScript(25, (s32)gMadraInteriors_CharFollows);
  FieldActor_RefreshSelectorById(10);
  FieldActor_RefreshSelectorById(11);
  FieldActor_RefreshSelectorById(25);
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Djinn_JoinParty(25, 2, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Actor_SetAnimation(4, 1);
  Actor_FaceDirection(10, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(10, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_SetAnimationAndWait(11, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Event_End();
}

/// Given the Laughing Fungus, the old man tells his wife it only looks like the
/// right one.
/// "No, dear... They look similar, but these aren't quite right."
void MadraInteriors_GiveLaughingFungus(void) {
  struct Actor *object0;

  Event_SetFlag(0x210);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object0 = Actor_Get(4);
  if (object0->z > 0x5f0000)
    Actor_WalkToAndWait(4, object0->x / 0x10000, 95);
  Actor_WalkToAndWait(4, 362, 95);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors110);
  Actor_FaceActor(10, 4, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(10, 0x102, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 5);
  Actor_ShowEmote(11, 0x100, 3);
  Actor_FaceActor(11, 4, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(11, 0, 5);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors115);
  Actor_ShowEmote(10, 0x100, 30);
  Actor_FaceDirectionAndWait(10, FACING_EAST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 5);
  Actor_FaceEachOther(10, 11, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(10, 4, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(10, 0x105, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 5);
  Actor_FaceActor(11, 4, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(11, 0, 5);
  Event_End();
  return;
}

/// A party member runs into the leader and vanishes, as the party gathers up
/// to leave; the four sites of the welcome-home event, named by us.
static inline void MadraInteriors_WalkIntoLeader(s32 actorId) {
  struct Actor *leader;

  Actor_SetSpeed(actorId, 0x13333, 0x9999);
  Actor_SetAnimation(actorId, 2);
  leader = Actor_Get(4);
  if (leader != NULL)
    Actor_SetDestination(actorId, ((s16 *)&leader->x)[1],
                         ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(actorId);
  Actor_SetPosition(actorId, 0, 0);
}

/// On the return with Piers, the elder's household welcomes him home.
/// "Felix! You're back!"
void MadraInteriors_WelcomePiersHome(void) {
  s32 result0;
  s32 result1;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors126);
  Actor_ShowEmote(28, 0x100, 30);
  Actor_FaceActor(28, 4, 0);
  FieldEvent_ShowMessageThenWait(28, 0, 10);
  Actor_FaceActor(15, 4, 0);
  Actor_FaceDirection(17, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceActor(18, 4, 0);
  Actor_FaceActor(14, 4, 0);
  Actor_FaceActor(15, 4, 0);
  Actor_FaceActor(16, 4, 0);
  FieldEvent_Wait(20);
  Actor_WalkToAndWait(4, 701, 93);
  Actor_WalkBy(4, -32, 0);
  FieldActor_LaunchFromLeader(5, -8, 32, FACING_WEST);
  FieldActor_LaunchFromLeader(31, -16, 0, FACING_SOUTH + FACING_STEP);
  FieldActor_LaunchFromLeader(6, -8, 16, FACING_SOUTHWEST);
  FieldActor_LaunchFromLeader(7, -24, 16, FACING_SOUTHWEST);
  FieldEvent_Wait(30);
  Actor_ShowEmote(15, 0x100, 30);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_ShowEmote(27, 0x100, 30);
  FieldEvent_Wait(20);
  Actor_FaceDirection(27, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(27, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  Actor_ShowEmote(27, 0x109, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_FaceEachOther(7, 4, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_FaceDirection(4, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldActor_Jump(18, 6, 20);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHEAST + FACING_STEP);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(28, 0x102, 30);
  Event_OpenMessage(28, 0);
  result0 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result0 != 0)
    goto answeredNo;
  Actor_ShowEmote(27, 0x108, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(15, 0x108, 30);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 3;
  goto answered;
answeredNo:
  Actor_ShowEmote(15, 0x102, 30);
  Func_02001ef8();
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 3;
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldActor_StartRepeatedMotion(15, 3);
  Actor_SetAttachedEffect(15, 0x102);
  FieldEvent_Wait(50);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
answered:
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(31, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(31, 0, 10);
  result1 = Event_GetFlag(0x981);
  if (result1 == 0)
    goto firstGoodbye;
  MadraInteriors_SayGoodbyeAgain();
  goto saidGoodbye;
firstGoodbye:
  MadraInteriors_AskToStayLonger();
saidGoodbye:
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldEvent_Wait(20);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(40);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_FaceDirection(31, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(4, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(7, FACING_EAST, 0);
  FieldEvent_Wait(40);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(31, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  MadraInteriors_WalkIntoLeader(31);
  MadraInteriors_WalkIntoLeader(6);
  MadraInteriors_WalkIntoLeader(7);
  MadraInteriors_WalkIntoLeader(5);
  Event_SetFlag(0x989);
  Event_End();
  return;
}

/// Leaving a second time, the household asks whether the party must go so soon.
/// "Ah, so you'll be leaving again, will you?"
void MadraInteriors_SayGoodbyeAgain(void) {
  FieldEvent_SetMessageId((s32)MsgMadraInteriors127);
  FieldEvent_ShowMessageThenWait(28, 0, 10);
  Actor_FaceEachOther(4, 31, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(4, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(31, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_ShowEmote(15, 0x100, 30);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(27, 0x102, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(31, 0, 10);
  Actor_ShowEmote(5, 0x108, 30);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_SetAnimationAndWait(17, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(17, 0, 10);
  Actor_FaceDirectionAndWait(5, FACING_SOUTH);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(18, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  return;
}

/// The first farewell: the household asks the party to stay, and Piers explains
/// why he cannot.
/// "If you've already recovered your jewel, why don't you stay here for a bit
/// longer?"
void MadraInteriors_AskToStayLonger(void) {
  FieldEvent_SetMessageId((s32)MsgMadraInteriors128);
  Actor_SetAnimationAndWait(17, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(17, 0, 10);
  Actor_FaceDirectionAndWait(5, FACING_SOUTH);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_FaceActor(6, 5, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(6, 0, 10);
  Actor_FaceDirectionAndWait(5, FACING_NORTH);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_ShowEmote(17, 0x101, 30);
  Actor_FaceActor(7, 17, 0);
  Actor_ShowEmote(7, 0x108, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(31, 0x102, 30);
  Actor_FaceActor(31, 7, 0);
  FieldEvent_ShowMessageThenWait(31, 0, 10);
  Actor_FaceActor(7, 31, 0);
  FieldEvent_Wait(60);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceActor(4, 7, 0);
  Actor_FaceActor(6, 7, 0);
  Actor_FaceActor(5, 7, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(6, 0x101, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 10);
  Actor_FaceActor(7, 6, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(4, 0x101, 30);
  Actor_FaceActor(7, 4, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(5, 0x109, 30);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHEAST);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_FaceActor(7, 31, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(20);
  Actor_FaceActor(7, 4, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Event_SetFlag(0x981);
  Actor_ShowEmote(15, 0x100, 30);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(4, FACING_SOUTHWEST, 0);
  Actor_ShowEmote(27, 0x102, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(31, 0, 10);
  Actor_ShowEmote(5, 0x108, 30);
  Actor_FaceDirection(5, FACING_WEST, 0);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_SetAnimationAndWait(17, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(17, 0, 10);
  Actor_FaceDirectionAndWait(5, FACING_SOUTH);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(18, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  return;
}

// clang-format off
/// The old couple's walks into their back room and out again, Char hopping
/// after them, as they fetch the Djinni for the Healing Fungus.
u32 gMadraInteriors_WifeWalksToBackRoom[] = {
    SC_ARG(2),
    SC_MOVE_XZ(PIXELS(356), PIXELS(72)),
    SC_SYNC,
    SC_MOVE(PIXELS(324), 0, PIXELS(73)),
    SC_SYNC,
    SC_CMD_03,
    SC_END,
};

u32 gMadraInteriors_LeaderStepsAside[] = {
    SC_ARG(2),
    SC_MOVE_XZ(PIXELS(339), PIXELS(111)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

u32 gMadraInteriors_OldManWalksToBackRoom[] = {
    SC_ARG(2),
    SC_MOVE_XZ(PIXELS(356), PIXELS(72)),
    SC_SYNC,
    SC_MOVE(PIXELS(324), 0, PIXELS(73)),
    SC_SYNC,
    SC_CMD_03,
    SC_END,
};

DEFINE_G_DATA_02001FC4(gMadraInteriors_WifeComesBack, 109)

DEFINE_G_DATA_02001FC4(gMadraInteriors_OldManComesBack, 97)

u32 gMadraInteriors_CharFollows[] = {
    SC_ARG(2),
    SC_POSITION(PIXELS(327), PIXELS(24), PIXELS(71)),
    SC_SET(SW_FACING, FACING_EAST),
    SC_MOVE(PIXELS(352), 0, PIXELS(72)),
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(373), PIXELS(97)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(45),
    SCENE_EXIT_IF(1, 44, 2, -1),
    SCENE_EXIT_IF(2, 44, 3, -1),
    SCENE_EXIT_IF(3, 45, 4, -1),
    SCENE_EXIT_IF(4, 45, 3, -1),
    SCENE_EXIT_IF(5, 44, 7, -1),
    SCENE_EXIT_IF(6, 45, 7, -1),
    SCENE_EXIT_IF(7, 45, 6, -1),
    SCENE_EXIT_IF(8, 44, 4, -1),
    SCENE_EXIT_IF(9, 44, 8, -1),
    SCENE_EXIT_IF(10, 44, 5, -1),
    SCENE_EXIT_IF(11, 45, 12, -1),
    SCENE_EXIT_IF(12, 45, 11, -1),
    SCENE_EXIT_IF(13, 45, 14, -1),
    SCENE_EXIT_IF(14, 45, 13, -1),
    SCENE_EXIT_IF(15, 44, 6, -1),
    SCENE_EXIT_IF(20, 44, 9, -1),
    SCENE_EXITS(46),
    SCENE_EXIT_IF(1, 44, 1, -1),
    SCENE_EXIT_IF(2, 44, 10, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gMadraInteriors_Placements[] = {
    {73, -1, 3, PIXELS(184), PIXELS(0), PIXELS(120), 0x4000, 1, 0},
    {74, -1, 1, PIXELS(72), PIXELS(0), PIXELS(112), 0x2000, 1, 0},
    {75, -1, 1, PIXELS(360), PIXELS(0), PIXELS(109), 0x4000, 1, 0},
    {76, -1, 1, PIXELS(440), PIXELS(0), PIXELS(96), 0x6000, 1, 0},
    {224, -1, 1, PIXELS(120), PIXELS(0), PIXELS(296), 0xa000, 1, 0},
    {225, -1, 1, PIXELS(72), PIXELS(0), PIXELS(384), 0x4000, 1, 0},
    {74, -1, 1, PIXELS(616), PIXELS(0), PIXELS(112), 0x2000, 1, 0},
    {200, -1, 1, PIXELS(640), PIXELS(0), PIXELS(152), 0xa000, 1, 0},
    {76, -1, 1, PIXELS(615), PIXELS(0), PIXELS(311), 0x5000, 1, 0},
    {191, -1, 1, PIXELS(728), PIXELS(0), PIXELS(364), 0xc000, 1, 0},
    {186, -1, 1, PIXELS(744), PIXELS(0), PIXELS(348), 0x4000, 1, 0},
    {202, -1, 1, PIXELS(160), PIXELS(0), PIXELS(556), 0x4000, 1, 0},
    {74, -1, 1, PIXELS(120), PIXELS(0), PIXELS(624), 0xe000, 1, 0},
    {76, -1, 1, PIXELS(184), PIXELS(0), PIXELS(648), 0xa000, 1, 0},
    {199, -1, 1, PIXELS(604), PIXELS(0), PIXELS(548), 0x2000, 1, 0},
    {209, -1, 3, PIXELS(432), PIXELS(0), PIXELS(616), 0xe000, 1, 0},
    {72, -1, 1, PIXELS(568), PIXELS(0), PIXELS(518), 0xe000, 1, 0},
    {245, 98, 2, PIXELS(368), PIXELS(0), PIXELS(320), 0x4000, 0, 0},
    {0x1e9, -1, 1, PIXELS(600), PIXELS(0), PIXELS(56), 0x4000, 2, 0},
    {238, -1, 1, PIXELS(676), PIXELS(0), PIXELS(762), 0x8000, 1, 0},
    {238, -1, 1, PIXELS(730), PIXELS(0), PIXELS(712), 0x4000, 1, 0},
    {239, 0x8ab, 1, PIXELS(847), PIXELS(0), PIXELS(711), 0x2000, 1, 0},
    {239, 0x8ab, 1, PIXELS(906), PIXELS(0), PIXELS(760), 0, 1, 0},
    {-1, 0, 0, PIXELS(0), PIXELS(0), PIXELS(0), 0, 0, 0},
};

struct ActorPlacement gMadraInteriors_PlacementsOnReturn[] = {
    {73, CONDITION_ALWAYS, 3, PIXELS(184), 0, PIXELS(120), FACING_SOUTH, 1, 0},
    {74, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(112), FACING_SOUTHEAST, 1, 0},
    {75, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(109), FACING_SOUTH, 1, 0},
    {76, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(96), FACING_SOUTHWEST, 1, 0},
    {224, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(296), FACING_NORTHWEST, 1, 0},
    {225, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(384), FACING_SOUTH, 1, 0},
    {74, CONDITION_ALWAYS, 1, PIXELS(584), 0, PIXELS(88), FACING_NORTHEAST, 0, 0},
    {200, CONDITION_ALWAYS, 1, PIXELS(640), 0, PIXELS(152), FACING_NORTHWEST, 0, 0},
    {76, CONDITION_ALWAYS, 1, PIXELS(620), 0, PIXELS(152), FACING_NORTHEAST, 0, 0},
    {191, CONDITION_ALWAYS, 1, PIXELS(680), 0, PIXELS(152), FACING_NORTH, 0, 0},
    {186, CONDITION_ALWAYS, 1, PIXELS(696), 0, PIXELS(136), FACING_SOUTH, 0, 0},
    {202, CONDITION_ALWAYS, 1, PIXELS(160), 0, PIXELS(552), FACING_SOUTH, 1, 0},
    {74, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(624), FACING_NORTHEAST, 1, 0},
    {76, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(648), FACING_NORTHWEST, 1, 0},
    {199, CONDITION_ALWAYS, 1, PIXELS(604), 0, PIXELS(548), FACING_SOUTHEAST, 1, 0},
    {209, CONDITION_ALWAYS, 3, PIXELS(432), 0, PIXELS(616), FACING_NORTHEAST, 1, 0},
    {72, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(518), FACING_NORTHEAST, 1, 0},
    {245, 0x62, 2, PIXELS(368), 0, PIXELS(320), FACING_SOUTH, 0, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(56), FACING_SOUTH, 2, 0},
    {19, CONDITION_ALWAYS, 1, PIXELS(614), 0, PIXELS(111), FACING_SOUTHEAST, 0, 0},
    {20, CONDITION_ALWAYS, 1, PIXELS(636), 0, PIXELS(111), FACING_SOUTH + FACING_STEP, 0, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(1288), 0, PIXELS(8), FACING_WEST, 0, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(1288), 0, PIXELS(8), FACING_WEST, 0, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(1288), 0, PIXELS(8), FACING_WEST, 0, 0},
    {7, CONDITION_ALWAYS, 1, PIXELS(1288), 0, PIXELS(8), FACING_NORTH, 0, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 0, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 0, 0},
    {238, CONDITION_ALWAYS, 1, PIXELS(676), 0, PIXELS(762), FACING_WEST, 1, 0},
    {238, CONDITION_ALWAYS, 1, PIXELS(730), 0, PIXELS(712), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMadraInteriors_JailPlacements[] = {
    {290, CONDITION_ALWAYS, 1, PIXELS(528), 0, PIXELS(840), FACING_NORTH, 1, 0},
    {192, CONDITION_ALWAYS, 1, PIXELS(522), 0, PIXELS(840), FACING_NORTH, 1, 0},
    {187, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(840), FACING_NORTH, 1, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_WEST, 0, 0},
    {7, 0x8ab, 1, PIXELS(504), 0, PIXELS(808), FACING_NORTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(362), 0, PIXELS(847), FACING_NORTH, 2, 0},
    {187, 0x18ff, 1, PIXELS(394), 0, PIXELS(863), FACING_NORTHWEST, 1, 0},
    {187, 0x18ff, 1, PIXELS(396), 0, PIXELS(848), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMadraInteriors_SecondRoomsPlacements[] = {
    {71, CONDITION_ALWAYS, 1, PIXELS(192), 0, PIXELS(128), FACING_EAST, 1, 0},
    {72, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(192), FACING_SOUTH, 1, 0},
    {198, CONDITION_ALWAYS, 1, PIXELS(356), 0, PIXELS(109), FACING_SOUTH, 2, 0},
    {186, 0x18ab, 1, PIXELS(357), 0, PIXELS(131), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gMadraInteriors_TriggersBeforeRaid[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors05)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(MadraInteriors_AskWillWeBeSafe)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors09)},
    {0xc400, 11, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors10)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors13)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors14)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors17)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors19)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors20)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors21)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors22)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToHealer)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors34)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors35)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors40)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors41)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, EVENT(MadraInteriors_GuardElderGem)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors42)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors43)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors165)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors166)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors07)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors08)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors11)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors12)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors16)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors23)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors25)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors26)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors27)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors36)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors37)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors38)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors44)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors45)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors46)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors47)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors48)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors167)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors168)},
    {0x173, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors169))},
    {0x173, 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors170))},
    {0x173, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors171))},
    {0x173, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors172))},
    {0x173, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors173))},
    {TRIGGER_TABLE_END},
};

struct Trigger gMadraInteriors_JailTriggers[] = {
    {TRIGGER_EXIT, 20, CONDITION_ALWAYS, 20},
    {TRIGGER_TALK, 9, 0x18ff, MSGNO(MsgMadraInteriors143)},
    {TRIGGER_TALK, 10, 0x18ff, MSGNO(MsgMadraInteriors144)},
    {0xc400, 13, 0x18ff, MSGNO(MsgWhenAndHis)},
    {0xa400, 13, 0x18ff, MSGNO(MsgWhenAndHis)},
    {0x8400, 13, 0x18ff, MSGNO(MsgWhenAndHis)},
    {TRIGGER_TALK, 14, 0x18ff, EVENT(MadraInteriors_TalkToIllMan)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors75)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors76)},
    {TRIGGER_TALK, 7, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToPiers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x18ff, MSGNO(MsgMadraInteriors145)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x18ff, MSGNO(MsgMadraInteriors146)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x18ff, MSGNO(MsgSoThisFellow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x1211, MSGNO(MsgIKnewI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1038, 0x18ff, EVENT(MadraInteriors_TalkToIllMan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors77)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors78)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 7, CONDITION_ALWAYS, EVENT(MadraInteriors_MindReadPiers)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMadraInteriors_SecondRoomsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TALK, 8, 0x8ab, MSGNO(MsgMadraInteriors01)},
    {TRIGGER_TALK, 8, 0x8ff, MSGNO(MsgMadraInteriors49)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors98)},
    {TRIGGER_TALK, 9, 0x8ab, MSGNO(MsgMadraInteriors02)},
    {TRIGGER_TALK, 9, 0x8ff, MSGNO(MsgMadraInteriors50)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors99)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors31)},
    {TRIGGER_TALK, 11, 0x8ff, MSGNO(MsgMadraInteriors79)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors147)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x8ab, MSGNO(MsgMadraInteriors03)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x8ff, MSGNO(MsgMadraInteriors51)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors100)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x8ab, MSGNO(MsgMadraInteriors04)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x8ff, MSGNO(MsgMadraInteriors52)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors101)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors32)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x8ff, MSGNO(MsgMadraInteriors80)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors148)},
    {0x173, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors174))},
    {TRIGGER_TABLE_END},
};

struct Trigger gMadraInteriors_TriggersAfterRaid[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors53)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors54)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors57)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors58)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors61)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors62)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors65)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors66)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors67)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors68)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors69)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToHealer)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors82)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors83)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors88)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors89)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, EVENT(MadraInteriors_GuardElderGem)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors90)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors91)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors55)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors56)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors59)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors60)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors63)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors64)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors70)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors71)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors72)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors73)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors74)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors84)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors85)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors86)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors92)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors93)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors94)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors95)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors96)},
    {0x173, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors169))},
    {0x173, 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors170))},
    {0x173, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors171))},
    {0x173, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors172))},
    {0x173, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors173))},
    {TRIGGER_TABLE_END},
};

struct Trigger gMadraInteriors_TriggersOnReturn[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, 30, 0x989, EVENT(MadraInteriors_WelcomePiersHome)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors102)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors103)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToMushroomLover)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToMushroomCook)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors122)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors123)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors129)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors130)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors131)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors132)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors133)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors134)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors135)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToHealer)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors150)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors151)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors156)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors157)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, EVENT(MadraInteriors_GuardElderGem)},
    {TRIGGER_TALK, 32, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors158)},
    {TRIGGER_TALK, 33, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors159)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors104)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors105)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, EVENT(MadraInteriors_MindReadMushroomLover)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT(MadraInteriors_MindReadMushroomCook)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors124)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors125)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors136)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors137)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors138)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors139)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors140)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors141)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors142)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors152)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors153)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors154)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors160)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors161)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors162)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 32, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors163)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 33, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors164)},
    {0x1c014, 10, 0x98a, EVENT(MadraInteriors_GiveHealingFungus)},
    {0x1c114, 10, 0x98a, EVENT(MadraInteriors_GiveLaughingFungus)},
    {0x173, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors169))},
    {0x173, 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors170))},
    {0x173, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors171))},
    {0x173, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors172))},
    {0x173, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors173))},
    {TRIGGER_TABLE_END},
};

/// The interrogators walk out of the jail once Piers has been questioned.
DEFINE_G_DATA_020033C8(gMadraInteriors_InterrogatorsLeave, 408, 872, 904, 408, 920)

/// The interrogator topples over, a quarter turn in eight frames.
s32 gMadraInteriors_InterrogatorTopples[] = {
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_END,
};

/// Piers' Psynergy lifts the interrogator and holds him bobbing in the air.
s32 gMadraInteriors_InterrogatorLifted[] = {
    SC_SET(SW_MOTION, 0),
    SC_WAIT(100),
    SC_WAIT(36),
    SC_ADD(SW_Y, PIXELS(3)),
    SC_WAIT(9),
    SC_ADD(SW_Y, PIXELS(5)),
    SC_ADD(SW_X, PIXELS(1)),
    SC_WAIT(9),
    SC_ADD(SW_Y, PIXELS(4)),
    SC_WAIT(9),
    SC_SET(SW_FACING, FACING_WEST),
    SC_ADD(SW_X, PIXELS(1)),
    SC_ADD(SW_Y, PIXELS(5)),
    SC_WAIT(9),
    SC_ADD(SW_Y, PIXELS(4)),
    SC_WAIT(9),
    SC_ADD(SW_Y, PIXELS(4)),
    SC_ADD(SW_X, PIXELS(1)),
    SC_WAIT(9),
    SC_ADD(SW_Y, PIXELS(4)),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ADD(SW_X, PIXELS(1)),
    SC_LABEL(1),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_ADD(SW_Y, PIXELS(-1)),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_Y, PIXELS(1)),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_ADD(SW_Y, PIXELS(-1)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_Y, PIXELS(1)),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};
// clang-format on
