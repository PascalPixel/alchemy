/// @file
/// Madra, the walled port town of eastern Gondowan where Felix first arrives
/// after the plateaus: guards inspect the party at the gate after Champa's
/// pirates raided the town for Briggs, Piers sits in its jail, and its elder
/// grants passage over the drawbridge to Osenia. A hole in the north-east
/// corner, kept by a worker until the inspection is over, leads down into the
/// catacombs. When Felix returns with Piers the hole is sealed, Karst and
/// Agatio arrive (Madra_MeetKarst), and the town takes other placements and
/// triggers.

#include "shared/actors.h"
#include "flags.h"
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
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "field/event.h"
#include "event_work.h"
#include "field_event.h"
#include "flag_call.h"
#include "module/game/game.h"
#include "module/scene/scene.h"
#include "module/system/system.h"
#include "scene_call.h"
#include "module/window/window.h"
#include "field_sprite.h"

extern struct ActorPlacement gMadra_Placements[];
extern struct ActorPlacement gMadra_PlacementsOnReturn[];

void Shop_Run(s32 a, s32 b);

void Madra_GrantPassageToOsenia(void);

void Madra_MeetKarst(void);
extern struct Trigger gMadra_TriggersBeforeRaid[];
extern struct Trigger gMadra_TriggersAfterRaid[];
extern struct Trigger gMadra_TriggersOnReturn[];

void Madra_AskComingThroughHole(void);

void Madra_AskComingThroughSealedHole(void);
void Madra_MindReadKarst(void);

void Madra_KarstCallsForMenardi(void);

void Madra_WorkerBlocksSealedHole(void);

void Madra_AskAboutBriggs(void);

void Madra_MindReadBriggsFan(void);

void Madra_InspectPartyAtGate(void);
extern s16 gMadra_DoorOpening[];
extern s16 gMadra_CavernHomeDoorOpening[];

void FieldCamera_WaitForArrival(void);

#include "text/msg_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (Event_GetFlag(0x8ff))
    return gMadra_PlacementsOnReturn;
  return gMadra_Placements;
}

extern struct MadraPlacement gMadra_Doors[];

/// The party leader walks into one of Madra's doors. Everyone stops, the door
/// opens with its cell animation (gMadra_Doors), and the leader walks in and
/// leaves by the exit of the door's number.
void Madra_EnterDoor(void) {
  struct MadraEventWork *work;
  struct Actor *object;
  s16 *trigger;
  u32 id;
  s32 index;

  work = Ram_HeapSlots->eventWork;
  Event_Start();
  Event_ResolvePendingActions(0);
  id = 8;
next:
  object = Actor_GetById(id);
  if (object != NULL) {
    object->motionFlags = 0;
  }
  id++;
  if (id < 64)
    goto next;
  trigger = &work->value154;
  index = *trigger - 1;
  Event_PlaySound(SE_DOOR_OPEN);
  {
    s32 x = gMadra_Doors[index].x;
    s32 y = gMadra_Doors[index].y;

    MapGrid_AnimateCells(gMadra_Doors[index].data, x, y);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(*trigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Madra's weapon merchant sells across the counter (stock row 4). Spoken to
/// from elsewhere, she talks of the raid, then of the Kibombo's weapons.
void Madra_TalkToWeaponMerchant(s32 arg) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Shop_Run(4, arg);
    return;
  }
  if (Event_GetFlag(0x8ff))
    Event_SetMessageId((s32)&MsgItLooksLikeThe);
  else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    Event_SetMessageId((s32)&MsgTheArmorerAnd);
  else
    Event_SetMessageId((s32)&MsgAfterTheChampa);
  Event_ShowMessage(29, 0);
}

/// Madra's armor merchant sells across the counter (stock row 5), and otherwise
/// talks of the Champa and the Kibombo.
void Madra_TalkToArmorMerchant(s32 arg) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Shop_Run(5, arg);
    return;
  }
  if (Event_GetFlag(0x8ff))
    Event_SetMessageId((s32)&MsgPeopleAreSaying);
  else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    Event_SetMessageId((s32)&MsgIKnewThere);
  else
    Event_SetMessageId((s32)&MsgOwOwOw);
  Event_ShowMessage(30, 0);
}

/// Madra's item merchant sells across the counter (stock row 6), and otherwise
/// talks of her sister, hurt in the Champa's attack, and of Shin.
void Madra_TalkToItemMerchant(s32 arg) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Shop_Run(6, arg);
    return;
  }
  if (Event_GetFlag(0x8ff))
    Event_SetMessageId((s32)&MsgMyBoyfriendShin);
  else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    Event_SetMessageId((s32)&MsgAllMyInjuries);
  else
    Event_SetMessageId((s32)&MsgMySisterWas);
  Event_ShowMessage(arg, 0);
}

/// Karst, told the party does not know her sister, sends it off.
/// "If you don't know my sister, then I have nothing to say to you."
void Madra_KarstSendsPartyAway(void) {
  Event_SetMessageId((s32)&MsgIfYouDon);
  Actor_SetAnimationAndWait(33, 4);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
}

/// Karst, waiting near the mayor's house, asks twice whether the party has seen
/// her sister.
/// "Have you by chance seen a girl named Menardi?"
void Madra_KarstAsksAboutMenardi(void) {
  Event_SetMessageId(MsgHaveYouBy);
  Event_OpenMessage(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldActor_ShowEmote(33, 0x103, 30);
    Event_OpenMessage(33, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      FieldActor_ShowEmote(33, 0x103, 30);
      Event_ShowMessageThenWait(33, 0, 10);
    } else {
      Madra_KarstSendsPartyAway();
    }
  } else {
    Madra_KarstSendsPartyAway();
  }
}

struct Actor *Actor_GetById(u32 objectId);
void Actor_SetDestinationOffsetById(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 id, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void MapGrid_PlayMapEdit(const void *data, s32 a, s32 b);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 a, s32 b);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 y,
                                                    s32 z);

/// Karst feels the party's Mind Read.
/// "Using Psynergy on me? Are you so dense that you don't know when to be
/// afraid?"
void Madra_MindReadKarst(void) {
  Event_SetMessageId((s32)MsgUsingPsynergyOn);
  Actor_RunRepeatedMotion(33, 2);
  Event_Wait(20);
  FieldActor_ShowEmote(33, 0x108, 30);
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_SetAnimationAndWait(33, 4);
  Event_Wait(10);
  Event_ShowMessageThenWait(33, 0, 10);
}

/// Stepping near the mayor's house the first time on the return with Piers, the
/// party hears Karst call for her sister.
/// "Where could Menardi have gone? Where are you, my sister?"
void Madra_KarstCallsForMenardi(void) {
  if (Event_GetFlag(FLAG_SCENE_TEMP_0F) == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldActor_ShowEmote(33, 0x105, 30);
    Event_SetMessageId((s32)&MsgWhereCouldMenardi);
    Event_ShowMessageThenWait(33, 0, 10);
    Event_End();
    Event_SetFlag(FLAG_SCENE_TEMP_0F);
  }
}

/// "Everyone here in Madra is afraid of the Kibombo now. Do they scare you,
/// too?"
void Madra_AskAfraidOfKibombo(s32 actor) {
  s32 msg = (s32)&MsgEveryoneHereIn;

  Event_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

/// The worker at the hole into the catacombs asks whether the party is coming
/// through, and sends it back while they work.
/// "You comin' through here?"
void Madra_AskComingThroughHole(void) {
  s32 msg;

  Actor_FaceActor(26, 4, 0);
  if (Event_GetFlag(FLAG_SCENE_TEMP_81)) {
    Event_SetMessageId((s32)&MsgYeahWellWe);
  } else {
    msg = (s32)&MsgYouCominThrough;
    Event_SetMessageId(msg);
    Event_OpenMessage(26, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetFlag(FLAG_SCENE_TEMP_81);
      Event_Wait(10);
      FieldActor_ShowEmote(26, 0x102, 0);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Actor_SetAnimationAndWait(26, 3);
      Event_SetMessageId(msg + 2);
    }
  }
  Event_ShowMessage(26, 0);
}

/// After the raid the worker is sealing the hole, and asks again.
/// "Oh, uh... You coming through?"
void Madra_AskComingThroughSealedHole(void) {
  s32 msg;

  Actor_FaceActor(26, 4, 0);
  if (Event_GetFlag(FLAG_SCENE_TEMP_81)) {
    Event_SetMessageId((s32)&MsgYeahUhWe);
  } else {
    msg = (s32)&MsgOhUhYou;
    Event_SetMessageId(msg);
    Event_OpenMessage(26, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetFlag(FLAG_SCENE_TEMP_81);
      Event_Wait(10);
      FieldActor_ShowEmote(26, 0x102, 0);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Actor_SetAnimationAndWait(26, 3);
      Event_SetMessageId(msg + 2);
    }
  }
  Event_ShowMessage(26, 0);
}

/// Places an actor facing an angle, the angle taken as the u16 an actor's
/// facing is; the hole's four placements, named by us.
static inline void Madra_PlaceFacing(s32 actorId, s32 x, s32 z, u16 facing) {
  Actor_SetPositionAndFacing(actorId, x, z, facing);
}

/// The worker pops up out of the hole the first time the party steps on it,
/// until the gate inspection is over (flag 0x910); afterwards the hole leads
/// down into the catacombs.
void Madra_WorkerBlocksHole(void) {
  if (Event_GetFlag(0x910)) {
    Event_PlaySound(SE_MAP_EXIT);
    Import_EventSetExitRequest(16);
  } else if (!Event_GetFlag(FLAG_SCENE_TEMP_80)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Madra_PlaceFacing(26, PIXELS(744), PIXELS(257), FACING_SOUTH);
    Actor_GetById(4)->velocityY = PIXELS(5);
    Actor_SetSpeed(4, 0x20000, 0x10000);
    Actor_WalkToAndWait(4, 720, 244);
    Actor_FaceDirectionAndWait(4, FACING_SOUTHEAST);
    Madra_PlaceFacing(4, PIXELS(720), PIXELS(244), FACING_SOUTHEAST);
    Event_SetFlag(FLAG_SCENE_TEMP_80);
    Madra_AskComingThroughHole();
    Event_End();
  }
}

/// After the raid, the worker pops up out of the hole he is sealing.
void Madra_WorkerBlocksSealedHole(void) {
  if (!Event_GetFlag(FLAG_SCENE_TEMP_80)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Madra_PlaceFacing(26, PIXELS(744), PIXELS(256), FACING_SOUTH);
    Actor_GetById(4)->velocityY = PIXELS(5);
    Actor_SetSpeed(4, 0x20000, 0x10000);
    Actor_WalkToAndWait(4, 720, 244);
    Actor_FaceDirectionAndWait(4, FACING_SOUTHEAST);
    Madra_PlaceFacing(4, PIXELS(720), PIXELS(244), FACING_SOUTHEAST);
    Event_SetFlag(FLAG_SCENE_TEMP_80);
    Madra_AskComingThroughSealedHole();
    Event_End();
  }
}

const struct Trigger *Scene_GetEvents(void) {
  if (Event_GetFlag(0x8ff))
    return gMadra_TriggersOnReturn;
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    return gMadra_TriggersAfterRaid;
  return gMadra_TriggersBeforeRaid;
}

void Madra_SealHoleCells(void) {
  MapGrid_CopyCellAttributes(109, 14, 3, 3, 45, 14);
}

/// On the return with Piers the hole is sealed: its picture and its cells are
/// drawn closed.
void Madra_SealHole(void) {
  MapGrid_CopyCellsLayered(51, 10, 3, 3, 45, 10);
  Madra_SealHoleCells();
}

/// Madra's prepare step: sets up actor 26 (larger, its sprite flags set)
/// and places actors 26 to 28, 33 and 66 by the story flags, then runs
/// Madra_GrantPassageToOsenia on entrance 9 and Madra_MeetKarst once on
/// entrance 7.
s32 Scene_Initialize(void) {
  struct Actor *object;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  if (Event_GetFlag(0x8ff)) {
    Madra_SealHole();
  } else {
    Actor_SetSpriteFlags(Actor_GetById(26), 0);
    object = Actor_GetById(26);
    object->radius = Actor_GetById(26)->radius * 4 / 3;
    ((struct FieldSprite *)Actor_GetById(26)->sprite)->nextFrame |= 8;
    {
      struct FieldSprite *sprite = Actor_GetById(26)->sprite;

      sprite->unk11 = (sprite->unk11 & 3) | 32;
    }
    ((struct FieldSprite *)Actor_GetById(26)->sprite)->unk19 = 1;
    if (Event_GetFlag(0x8ff) == 0 && Event_GetFlag(FLAG_BRIGGS_CAUGHT) != 0) {
      Actor_SetPosition(27, 0, 0);
      Actor_SetPosition(28, 0, 0);
      if (Event_GetFlag(FLAG_SCENE_TEMP_80) == 0)
        Actor_SetPosition(26, 0, 0);
    } else if (Event_GetFlag(0x8ff) != 0 || Event_GetFlag(0x910) != 0 ||
               Event_GetFlag(FLAG_SCENE_TEMP_80) == 0) {
      Actor_SetPosition(26, 0, 0);
    }
  }
  if (gMainRoutineWork.entrance == 9 && Event_GetFlag(0x913) == 0)
    Madra_GrantPassageToOsenia();
  if (Event_GetFlag(0x8ff) != 0 && Event_GetFlag(0x989) != 0) {
    if (gMainRoutineWork.entrance == 7 && Event_GetFlag(0x98e) == 0) {
      Madra_MeetKarst();
      Event_SetFlag(0x98e);
    }
    if (Event_GetFlag(0x98e))
      Actor_SetPosition(33, 0, 0);
  } else {
    Actor_SetPosition(66, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// "Was I just dreaming when that shaft of ice appeared?"
void Madra_AskAboutIceShaft(s32 actor) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)&MsgWasIJust;
  Event_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
  Event_End();
}

/// A boy asks whether Felix is the one who beat up Briggs.
/// "Hey, mister! Are you the guy who beat up Briggs?"
void Madra_AskAboutBriggs(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    msg = (s32)&MsgHeyMisterAre;
    Event_SetMessageId(msg);
    Event_OpenMessage(8, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(8, 0);
  } else {
    if (Event_GetFlag(FLAG_AREA_TEMP_00) == 0)
      Actor_FaceActor(14, 4, 0);
    msg = (s32)&MsgDidYouCome;
    Event_SetMessageId(msg);
    Event_OpenMessage(8, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(8, 0);
  }
  Actor_FaceDirection(14, FACING_SOUTH, 0);
  Event_SetFlag(FLAG_AREA_TEMP_00);
  Event_End();
}

void Madra_MindReadBriggsFan(void) {
  s32 msg, x, z;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    msg = (s32)&MsgHeyMisterAre;
    Event_SetMessageId(msg);
    Event_OpenMessage(8, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(8, 0);
  } else {
    if (Event_GetFlag(FLAG_AREA_TEMP_00) == 0) {
      x = 0x100;
      z = 30;
      FieldActor_ShowEmote(14, x, z);
      Actor_FaceActor(14, 4, 0);
    }
    msg = (s32)&MsgDidYouCome;
    Event_SetMessageId(msg);
    Event_OpenMessage(8, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(8, 0);
  }
  Actor_FaceDirection(14, FACING_SOUTH, 0);
  Event_SetFlag(FLAG_AREA_TEMP_00);
  Event_End();
}

/// "Is there by any chance a girl named Jenna with you?"
void Madra_AskAboutJenna(void) {
  s32 flags;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActor(11, 4, 0);
  Event_SetMessageId((s32)&MsgIsThereBy);
  flags = 0;
  Event_OpenMessage(11, flags);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_SetFlag(FLAG_AREA_TEMP_01);
    Event_ShowMessage(11, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessage(11, 0);
  }
  Event_End();
}
#undef PLACE

struct Actor *Actor_GetById(u32 objectId);
void Actor_WaitForMoveById(s32 id);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 id, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Actor_WalkToById(s32 id, s32 x, s32 z);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 a, s32 b);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_LaunchFromLeader(s32 id, s32 a, s32 b, s32 c);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z,
                                                    s32 angle);
void ItemDisplay_ShowItem(s32 item, s32 a);
s32 PartyInventory_GiveItem(s32 item, s32 count);
void FieldMessage_Show(s32 message, s32 position);
void FieldCamera_FollowActor(s32 id, s32 mode);
void Actor_SetSpriteColor(struct Actor *object, s32 attribute);

/// Actors 24 and 25 talk with 32 and 4, 25 is moved about with its priority
/// bit cleared and set, and 32 is placed at the current owner before flag
/// 0x910 is set. It opens with "You there, hold".
void Madra_InspectPartyAtGate(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgYouThereHold);
  Event_ShowMessageThenWait(24, 0, 2);
  FieldCamera_MoveTo(238 << 17, -1, 210 << 17, 1);
  Actor_WalkBy(25, 3, 8);
  Actor_WalkByAndWait(24, -3, 8);
  Actor_WaitForMoveById(25);
  Actor_WalkToAndWait(4, 472, 426);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  FieldActor_LaunchFromLeader(32, 16, 1, FACING_NORTH);
  FieldCamera_WaitForArrival();
  Actor_WaitForMoveById(32);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(32, FACING_WEST, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(32, FACING_NORTH, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_RunRepeatedMotion(32, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(32, 0, 2);
  Event_Wait(10);
  Actor_SetAnimationAndWait(24, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(25, 0, 2);
  Event_Wait(10);
  Actor_FaceActor(24, 25, 0);
  Event_Wait(10);
  Actor_FaceActor(25, 24, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(24, 3);
  Event_Wait(30);
  Actor_FaceActor(25, 24, 0);
  Event_Wait(10);
  Actor_FaceActor(25, 32, 0);
  Actor_SetAnimationAndWait(25, 3);
  Event_Wait(30);
  Actor_FaceDirection(24, FACING_SOUTH + FACING_STEP, 0);
  Actor_GetById(25)->priorityFlags &= 0xfe;
  Actor_SetSpritePriority(25, 3);
  Actor_WalkToAndWait(25, 457, 402);
  Actor_WalkByAndWait(25, 16, 0);
  Actor_WalkToAndWait(25, 472, 420);
  Event_Wait(30);
  Actor_WalkToAndWait(25, 484, 420);
  Actor_GetById(25)->priorityFlags |= 1;
  Actor_FaceDirection(25, FACING_SOUTH, 0);
  Event_Wait(60);
  Event_Wait(10);
  Actor_FaceDirection(25, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessageThenWait(25, 0, 2);
  Event_Wait(10);
  Actor_SetAnimationAndWait(24, 3);
  Event_ShowMessageThenWait(24, 0, 2);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH);
  Event_Wait(10);
  Actor_SetAnimationAndWait(25, 3);
  Event_Wait(5);
  Event_ShowMessageThenWait(25, 0, 2);
  Actor_GetById(25)->priorityFlags &= 0xfe;
  Actor_SetSpritePriority(25, 3);
  Actor_WalkByAndWait(25, 0, -16);
  Actor_WalkToAndWait(25, 475, 400);
  Actor_WalkByAndWait(25, -16, 0);
  Actor_GetById(25)->priorityFlags |= 1;
  FieldCamera_MoveTo(238 << 17, -1, 198 << 17, 1);
  Actor_WalkToAndWait(25, 459, 416);
  Actor_FaceDirection(25, FACING_SOUTHEAST, 0);
  Event_Wait(10);
  FieldActor_ShowEmote(32, 0x101, 50);
  Event_ShowMessageThenWait(32, 0, 2);
  Event_Wait(10);
  Actor_RunRepeatedMotion(24, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(24, 0, 2);
  Event_Wait(10);
  FieldActor_ShowEmote(32, 0x100, 0);
  FieldActor_ShowEmote(4, 0x100, 50);
  Event_Wait(20);
  Actor_SetAnimationAndWait(25, 4);
  Actor_FaceDirection(32, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessageThenWait(25, 0, 2);
  Event_Wait(10);
  Actor_FaceEachOther(4, 32, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(32, FACING_NORTH, 0);
  FieldActor_ShowEmote(25, 0x109, 50);
  Event_ShowMessageThenWait(24, 0, 2);
  FieldActor_ShowEmote(25, 0x103, 20);
  Actor_FaceDirection(32, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessageThenWait(25, 0, 2);
  Event_Wait(10);
  Actor_SetAnimationAndWait(24, 3);
  Event_Wait(30);
  Actor_FaceDirection(32, FACING_NORTH + FACING_STEP, 0);
  Event_ShowMessageThenWait(24, 0, 2);
  Actor_SetSpeed(24, 0x4ccc, 0x2666);
  Actor_SetSpeed(25, 0x4ccc, 0x2666);
  Actor_WalkBy(24, 3, -8);
  Actor_WalkBy(25, -3, -8);
  Actor_WaitForMoveById(24);
  Actor_WaitForMoveById(25);
  Actor_FaceDirection(25, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(24, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_SetAnimation(32, 3);
  Actor_SetAnimationAndWait(4, 3);
  Event_Wait(30);
  Actor_SetAnimation(32, 2);
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object != NULL)
    Actor_SetDestination(32, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(32);
  Actor_SetPosition(32, 0, 0);
  Event_SetFlag(0x910);
  Actor_SetPosition(26, 0, 0);
  Event_End();
}

/// Once flag 0x911 is set: actors 33 and 34 talk with 4, the prompt's answer
/// is counted in the event work either way, and flag 0x913 is set. The talk
/// includes "... has escaped".
void Madra_GrantPassageToOsenia(void) {
  if (!GameFlag_Get(0x911))
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveWithinMapBounds(PIXELS(124), -1, PIXELS(420), 1);
  FieldCamera_WaitForArrival();
  Actor_SetSpeed(33, 0xcccc, 0x6666);
  Actor_SetSpeed(34, 0xcccc, 0x6666);
  Event_StartScreenTransition();
  Actor_SetPosition(33, PIXELS(172), PIXELS(332));
  Actor_SetHorizontalPositionWithTerrain(34, PIXELS(172), PIXELS(324));
  Event_SetMessageId(MsgHasEscaped2);
  Event_WaitTransitionFrames();
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_WalkTo(33, 156, 372);
  Event_Wait(20);
  Actor_WalkTo(34, 156, 372);
  Actor_WalkTo(4, 76, 388);
  Actor_WaitForMoveById(33);
  Actor_WalkTo(33, 108, 380);
  Actor_WaitForMoveById(34);
  Actor_WalkTo(34, 124, 380);
  Actor_WaitForMoveById(4);
  Actor_ArmCallback(4, FACING_SOUTHEAST, 0);
  Actor_WaitForMoveById(33);
  Actor_WaitForMoveById(34);
  Actor_FaceDirection(33, FACING_NORTH, 0);
  Actor_FaceDirection(34, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_FaceDirection(33, FACING_NORTH, 0);
  Actor_FaceDirection(34, FACING_NORTH, 0);
  Actor_SetAttachedEffect(34, 258);
  Actor_SetAttachedEffect(33, 258);
  Event_Wait(50);
  Motion_SetModeAndWaitAnimation(34, 3);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  Actor_WalkTo(33, 102, 340);
  Event_Wait(20);
  Actor_WalkTo(34, 102, 340);
  Actor_WaitForMoveById(33);
  Actor_SetPosition(33, PIXELS(4), PIXELS(4));
  Actor_WaitForMoveById(34);
  Actor_SetPosition(34, PIXELS(4), PIXELS(4));
  Actor_ArmCallback(4, FACING_NORTHEAST, 0);
  Event_Wait(40);
  FieldActor_ShowEmote(4, 258, 30);
  Event_Wait(40);
  Event_Wait(120);
  Actor_SetPosition(33, PIXELS(102), PIXELS(340));
  Actor_SetPosition(34, PIXELS(102), PIXELS(340));
  Actor_WalkToAndWait(34, 100, 364);
  Actor_WalkTo(34, 124, 380);
  Actor_WalkToAndWait(33, 100, 380);
  Actor_WaitForMoveById(34);
  Actor_WalkTo(34, 132, 396);
  Actor_WalkToAndWait(33, 108, 396);
  Actor_WaitForMoveById(34);
  Actor_FaceActorById(4, 33, 0);
  Actor_FaceDirection(34, FACING_NORTH, 0);
  Actor_FaceDirection(33, FACING_NORTH, 0);
  FieldActor_ShowEmote(33, 258, 30);
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_FaceActorById(34, 33, 0);
  Motion_SetModeAndWaitAnimation(34, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  Actor_FaceActorById(33, 34, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Motion_SetModeAndWaitAnimation(34, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  FieldActor_ShowEmote(33, 258, 30);
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_FaceDirection(33, FACING_NORTH, 0);
  Actor_ArmCallback(34, FACING_NORTH, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(34, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Event_Wait(20);
  Actor_ArmCallback(33, FACING_WEST, 0);
  Event_Wait(20);
  FieldActor_ShowEmote(33, 257, 30);
  Event_ShowMessageThenWait(33, 0, 10);
  Event_ShowMessageThenWait(33, 0, 10);
  Motion_SetVarCbAndRefresh(34, 2);
  Actor_FaceActorById(34, 33, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  Actor_FaceActorById(33, 34, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(33, 4);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_FaceActorById(33, 4, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(33, 0, 10);
  FieldActor_ShowEmote(4, 261, 30);
  Motion_SetVarCbAndRefresh(34, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  Event_Wait(20);
  FieldActor_ShowEmote(33, 262, 30);
  FieldMessage_ShowAtActor(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_ShowMessageThenWait(33, 0, 10);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(33, 0, 10);
  }
  Actor_FaceActorById(33, 34, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Motion_SetModeAndWaitAnimation(34, 3);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  FieldActor_ShowEmote(34, 258, 30);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Motion_SetVarCbAndRefresh(34, 2);
  Event_Wait(60);
  Motion_SetModeAndWaitAnimation(34, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_WalkByAndWaitById(34, 6, 0);
  Actor_WalkToAndWait(34, 164, 316);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(33, FACING_WEST);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_SetHorizontalPositionWithTerrain(34, 0, 0);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  GameFlag_Set(0x913);
  Actor_WalkByAndWaitById(33, 30, 0);
  Actor_WalkToAndWait(33, 164, 316);
  Event_Wait(20);
  Actor_SetHorizontalPositionWithTerrain(33, 0, 0);
  Event_Wait(20);
  Event_End();
}

/// Actors 24, 25 and 32 question 4, whose answers are counted in the event
/// work; after flag 0x98f 32 asks again. It opens with "Hey, you lot"; 32's
/// second question is "It was them". Then 32 is placed at the current owner and
/// flag 0x914 is set.
void Madra_StopPartyAfterRaid(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgHeyYouLot);
  Event_ShowMessageThenWait(24, 0, 2);
  FieldCamera_MoveTo(PIXELS(476), -1, PIXELS(400), 1);
  Actor_WalkByAndWait(24, -3, 8);
  Actor_WalkToAndWait(4, 472, 426);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  Actor_LaunchFromLeader(32, 16, 1, FACING_NORTH);
  FieldCamera_WaitForArrival();
  Actor_WaitForMoveById(32);
  Actor_WalkByAndWait(25, 3, 8);
  Actor_FaceDirectionAndWait(25, FACING_EAST);
  Actor_SetAnimation(25, 4);
  Event_Wait(20);
  Event_ShowMessageThenWait(25, 0, 10);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(32, FACING_WEST, 0);
  Event_Wait(20);
  FieldActor_ShowEmote(32, 0x101, 30);
  Event_ShowMessage(32, 0);
  Actor_SetAnimationAndWait(4, 4);
  Actor_FaceDirectionAndWait(24, FACING_SOUTH + FACING_STEP);
  Actor_SetAnimationAndWait(24, 4);
  Event_ShowMessageThenWait(24, 0, 10);
  Actor_FaceDirection(32, FACING_NORTH + FACING_STEP, 0);
  FieldActor_ShowEmote(32, 0x103, 0);
  Actor_Jump(32, 6, 15);
  Actor_Jump(32, 6, 23);
  Event_ShowMessage(32, 0);
  FieldActor_ShowEmote(4, 0x100, 30);
  Actor_FaceActor(4, 24, 0);
  Event_Wait(20);
  FieldActor_ShowEmote(32, 0x100, 30);
  Actor_FaceDirectionAndWait(32, FACING_NORTH + FACING_STEP);
  Event_ShowMessage(32, 0);
  Actor_FaceDirectionAndWait(25, FACING_SOUTHEAST);
  Actor_SetAnimationAndWait(25, 3);
  Actor_FaceDirection(32, FACING_NORTHWEST + FACING_STEP, 0);
  Event_Wait(10);
  Event_OpenMessage(25, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTHWEST + FACING_STEP);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(25);
    Actor_FaceDirection(32, FACING_NORTHWEST + FACING_STEP, 0);
    Event_ShowMessageThenWait(25, 0, 10);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    Event_Wait(25);
    Actor_FaceDirection(32, FACING_NORTH + FACING_STEP, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(25, 0, 10);
  }
  Actor_RunRepeatedMotion(32, 2);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(32, FACING_NORTHWEST + FACING_STEP);
  Event_ShowMessage(32, 0);
  FieldActor_ShowEmote(25, 0x102, 30);
  Event_ShowMessageThenWait(25, 0, 10);
  Actor_RunRepeatedMotion(24, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(24, 0, 10);
  if (Event_GetFlag(0x98f)) {
    Event_SetMessageId(MsgItWasThem);
    Actor_FaceDirection(32, FACING_WEST, 0);
    Event_Wait(20);
    Event_OpenMessage(32, 0);
    Actor_FaceDirection(4, FACING_EAST, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      Event_Wait(20);
      Actor_SetAnimationAndWait(32, 3);
      Event_ShowMessage(32, 0);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    } else {
      Event_Wait(20);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      Actor_SetAnimationAndWait(32, 4);
      Event_ShowMessage(32, 0);
    }
  }
  Event_SetMessageId(MsgWasAnythingStolen);
  Actor_FaceDirection(32, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(4, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(20);
  Event_ShowMessage(32, 0);
  Event_Wait(40);
  FieldActor_ShowEmote(24, 0x101, 30);
  Actor_FaceDirection(24, FACING_WEST, 0);
  Event_Wait(20);
  Actor_FaceDirection(32, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(4, FACING_NORTHWEST + FACING_STEP, 0);
  FieldActor_ShowEmote(25, 0x102, 30);
  Actor_SetAnimationAndWait(25, 4);
  Event_Wait(10);
  Actor_FaceDirection(25, FACING_SOUTHEAST, 0);
  Event_ShowMessageThenWait(25, 0, 10);
  FieldActor_ShowEmote(32, 0x101, 0);
  FieldActor_ShowEmote(4, 0x101, 30);
  Event_ShowMessage(32, 0);
  Actor_FaceDirectionAndWait(24, FACING_SOUTH + FACING_STEP);
  Actor_SetAnimationAndWait(24, 4);
  Event_Wait(10);
  Event_ShowMessageThenWait(24, 0, 10);
  Actor_SetAnimationAndWait(25, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(25, 0, 10);
  Actor_SetSpeed(24, 0x4ccc, 0x2666);
  Actor_SetSpeed(25, 0x4ccc, 0x2666);
  Actor_WalkBy(24, 3, -8);
  Actor_WalkBy(25, -3, -8);
  Actor_WaitForMoveById(24);
  Actor_WaitForMoveById(25);
  Actor_FaceDirection(25, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(24, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(20);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(32, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimation(32, 3);
  Actor_SetAnimationAndWait(4, 3);
  Event_Wait(10);
  Event_SetFlag(0x914);
  Actor_SetAnimation(32, 2);
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object != NULL)
    Actor_SetDestination(32, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(32);
  Actor_SetPosition(32, 0, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  Event_End();
}

#define SET_MODE(id, mode)                                                     \
  do {                                                                         \
    Actor_SetAnimation(id, mode);                                              \
  } while (0)

/// Actor 34 questions 4 and the others; the answers are counted in the event
/// work. It opens with "Wait". With room for item 210 it is given and flag
/// 0x918 is set, otherwise the mayor's two lines are shown. 33 then talks with
/// the group, and they all gather at 4 before flags 0x980 and 0x98e are set.
void Madra_MeetKarst(void) {
  struct Actor *object;
  s32 x = 153 << 16;
  s32 z;

  z = 0x01030000;
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  if (GameFlag_Get(0x825) || GameFlag_Get(0x826))
    GameFlag_Set(0xa76);
  Event_SetMessageId(MsgWait3);
  Event_ShowMessageThenWait(34, 0, 0);
  Actor_WalkToAndWait(4, 150, 280);
  Actor_WalkByById(4, 16, 0);
  FieldActor_LaunchFromLeader(5, 8, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(32, 0, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(6, -8, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(7, -16, 0, FACING_NORTH);
  Actor_WaitForMoveById(4);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Event_Wait(20);
  MapGrid_AnimateCells(gMadra_Doors[6].data, gMadra_Doors[6].x,
                       gMadra_Doors[6].y);
  Actor_SetHorizontalPositionAndAngleWithTerrain(34, x, z, FACING_SOUTH);
  Actor_WalkByAndWaitById(34, 0, 4);
  SET_MODE(34, 3);
  /* FAKEMATCH(dead): this wait in a once-only block, which starts its own
   * scheduling region. */
  do {
    Event_Wait(20);
  } while (0);
  FieldMessage_ShowAtActor(34, 0);
  Actor_FaceActorById(32, 4, 0);
  Actor_FaceActorById(5, 4, 0);
  Actor_FaceActorById(7, 4, 0);
  Actor_FaceActorById(6, 4, 0);
  Actor_ArmCallback(34, FACING_SOUTH, 0);
  Event_Wait(20);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Actor_FaceDirection(32, FACING_NORTH, 0);
    Actor_FaceDirection(5, FACING_NORTH, 0);
    Actor_FaceDirection(7, FACING_NORTH, 0);
    Actor_FaceDirection(6, FACING_NORTH, 0);
    Actor_ArmCallback(34, FACING_SOUTH, 0);
    Event_Wait(20);
    SET_MODE(34, 4);
    Event_Wait(33);
    Event_Wait(20);
    Event_ShowMessageAndWait(34, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    Actor_FaceDirection(32, FACING_NORTH, 0);
    Actor_FaceDirection(5, FACING_NORTH, 0);
    Actor_FaceDirection(7, FACING_NORTH, 0);
    Actor_FaceDirection(6, FACING_NORTH, 0);
    Actor_ArmCallback(34, FACING_SOUTH, 0);
    Event_Wait(20);
    SET_MODE(34, 4);
    Event_Wait(33);
    Event_Wait(20);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(34, 0, 0);
  }
  FieldActor_ShowEmote(34, 0x102, 30);
  Event_ShowMessageThenWait(34, 0, 0);
  SET_MODE(34, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 0);
  FieldActor_ShowEmote(6, 0x102, 30);
  Actor_FaceActorById(6, 5, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceActorById(5, 6, 0);
  Event_Wait(20);
  SET_MODE(5, 4);
  Event_Wait(33);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  FieldActor_ShowEmote(34, 0x100, 30);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 0);
  Actor_WalkByAndWaitById(34, 16, 8);
  Actor_FaceDirection(34, FACING_SOUTH, 0);
  if (PartyInventory_FindRoom(210) >= 0) {
    ItemDisplay_ShowItem(210, 3);
    PartyInventory_GiveItem(210, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Actor_ArmCallback(4, FACING_NORTH, 0);
    GameFlag_Set(0x918);
  } else {
    s32 count = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) + 1;

    Event_SetMessageId(MsgTheMayorTried);
    MessageArgument_Push(210, 2);
    FieldMessage_Show((*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++, 5);
    Motion_SetVarCbAndRefresh(34, 2);
    Event_ShowMessageAndWait(34, 0);
    Actor_WalkByAndWaitById(34, 16, 0);
    MessageArgument_Push(210, 2);
    FieldMessage_Show((*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++, 5);
    Actor_WalkByAndWait(34, -16, 0);
    Actor_FaceDirectionAndWait(34, FACING_SOUTH);
    Actor_SetPosition(66, PIXELS(192), PIXELS(256));
    *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) = count;
  }
  Actor_WalkByAndWait(34, -16, -8);
  Actor_FaceDirectionAndWait(34, FACING_SOUTH);
  FieldActor_ShowEmote(34, 0x105, 30);
  Event_ShowMessageThenWait(34, 0, 0);
  SET_MODE(34, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Event_ShowMessageThenWait(34, 0, 0);
  Actor_SetModeById(32, 3);
  SET_MODE(4, 3);
  SET_MODE(5, 3);
  SET_MODE(6, 3);
  SET_MODE(7, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(34, FACING_NORTH);
  Actor_WalkToAndWait(34, 153, 259);
  Actor_SetHorizontalPositionWithTerrain(34, 0, 0);
  SET_MODE(32, 4);
  Event_Wait(33);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  Event_ShowMessageAndWait(33, 0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  FieldActor_ShowEmote(4, 0x101, 0);
  FieldActor_ShowEmote(5, 0x101, 0);
  FieldActor_ShowEmote(7, 0x101, 0);
  FieldActor_ShowEmote(6, 0x101, 0);
  FieldActor_ShowEmote(32, 0x101, 30);
  Actor_FaceActorById(4, 33, 0);
  Actor_FaceActorById(7, 33, 0);
  Actor_FaceActorById(6, 33, 0);
  Actor_FaceActorById(5, 33, 0);
  Actor_FaceActorById(32, 33, 0);
  Event_Wait(20);
  FieldCamera_FollowActor(33, 1);
  Event_Wait(60);
  Sound_Play(MUS_036);
  FieldActor_ShowEmote(33, 0x107, 30);
  Event_ShowMessageAndWait(33, 0);
  Actor_FaceDirection(33, FACING_WEST, 0);
  Actor_TurnToFaceEachOther(6, 5, 0);
  Actor_TurnToFaceEachOther(7, 32, 0);
  Event_Wait(40);
  FieldActor_ShowEmote(4, 0x102, 30);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Actor_FaceActorById(4, 33, 0);
  Actor_FaceActorById(7, 33, 0);
  Actor_FaceActorById(6, 33, 0);
  Actor_FaceActorById(5, 33, 0);
  Actor_FaceActorById(32, 33, 0);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(33, FACING_WEST);
  Actor_FaceDirectionById(33, FACING_EAST);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  FieldActor_ShowEmote(6, 0x102, 30);
  Actor_FaceActorById(6, 5, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(6, 0);
  FieldActor_ShowEmote(5, 0x100, 30);
  Event_ShowMessageAndWait(5, 0);
  FieldActor_ShowEmote(33, 0x107, 30);
  Event_ShowMessageAndWait(33, 0);
  Actor_FaceActorById(33, 4, 0);
  Event_Wait(20);
  FieldMessage_ShowAtActor(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) != 0) {
    Event_ShowMessageAndWait(33, 0);
    FieldActor_ShowEmote(4, 0x102, 30);
    Event_Wait(40);
    SET_MODE(33, 3);
    Event_Wait(20);
    Event_Wait(20);
    Event_ShowMessageAndWait(33, 0);
    Actor_TurnToFaceEachOther(6, 5, 0);
    Event_Wait(40);
    Actor_FaceActorById(6, 33, 0);
    Actor_FaceActorById(5, 33, 0);
    Event_Wait(40);
    Actor_SetModeById(5, 4);
    SET_MODE(6, 4);
    Event_Wait(33);
    Event_Wait(20);
    FieldActor_ShowEmote(33, 0x109, 30);
    Event_ShowMessageAndWait(33, 0);
  } else {
    Event_SetMessageId(MsgICanTBelieveIt);
    Event_ShowMessageAndWait(33, 0);
    Actor_TurnToFaceEachOther(6, 5, 0);
    Event_Wait(40);
    Actor_FaceActorById(6, 33, 0);
    Actor_FaceActorById(5, 33, 0);
    Event_Wait(40);
    Actor_SetModeById(5, 4);
    SET_MODE(6, 4);
    Event_Wait(33);
    Event_Wait(20);
    FieldActor_ShowEmote(6, 0x109, 30);
    Event_ShowMessageAndWait(33, 0);
  }
  Event_SetMessageId(MsgWhoIsThis);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  SET_MODE(32, 4);
  SET_MODE(4, 4);
  SET_MODE(7, 4);
  SET_MODE(6, 4);
  SET_MODE(5, 4);
  Event_Wait(33);
  Event_Wait(20);
  SET_MODE(33, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  FieldActor_ShowEmote(4, 0x109, 0);
  FieldActor_ShowEmote(5, 0x109, 0);
  FieldActor_ShowEmote(6, 0x109, 0);
  FieldActor_ShowEmote(32, 0x109, 30);
  Event_Wait(60);
  FieldActor_ShowEmote(33, 0x106, 30);
  Event_ShowMessageThenWait(33, 0, 0);
  FieldActor_ShowEmote(4, 0x102, 0);
  FieldActor_ShowEmote(5, 0x102, 0);
  FieldActor_ShowEmote(6, 0x102, 0);
  FieldActor_ShowEmote(32, 0x102, 30);
  SET_MODE(33, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Actor_ArmCallback(33, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  FieldActor_ShowEmote(32, 0x100, 30);
  Event_ShowMessageAndWait(32, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  SET_MODE(33, 4);
  Event_Wait(33);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  FieldActor_ShowEmote(4, 0x103, 0);
  FieldActor_ShowEmote(6, 0x103, 0);
  FieldActor_ShowEmote(5, 0x103, 0);
  FieldActor_ShowEmote(7, 0x103, 30);
  Actor_FaceDirection(33, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  FieldActor_ShowEmote(33, 0x108, 30);
  Event_ShowMessageAndWait(33, 0);
  Motion_SetVarCbAndRefresh(4, 2);
  Event_Wait(20);
  SET_MODE(33, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  SET_MODE(33, 4);
  Event_Wait(33);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  FieldActor_ShowEmote(4, 0x100, 0);
  FieldActor_ShowEmote(7, 0x100, 0);
  FieldActor_ShowEmote(5, 0x100, 0);
  FieldActor_ShowEmote(6, 0x100, 0);
  FieldActor_ShowEmote(4, 0x100, 30);
  FieldActor_ShowEmote(33, 0x101, 30);
  Event_ShowMessageAndWait(33, 0);
  Actor_FaceDirectionById(33, FACING_EAST);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(33, FACING_WEST);
  Event_Wait(20);
  Actor_FaceDirectionById(33, FACING_EAST);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Actor_ArmCallback(33, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  SET_MODE(33, 3);
  Event_Wait(20);
  Event_Wait(20);
  FieldMessage_ShowAtActor(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldActor_ShowEmote(33, 0x108, 30);
    Event_ShowMessageThenWait(33, 0, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    FieldActor_ShowEmote(33, 0x103, 30);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(33, 0, 0);
  }
  FieldActor_ShowEmote(32, 0x102, 30);
  SET_MODE(32, 4);
  Event_Wait(33);
  SET_MODE(32, 4);
  Event_Wait(33);
  Event_ShowMessageAndWait(32, 0);
  SET_MODE(33, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Actor_ArmCallback(33, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_WalkByAndWaitById(33, 24, 24);
  Actor_WalkByAndWaitById(33, 132, 0);
  Actor_SetHorizontalPositionWithTerrain(33, 0, 0);
  Scene_PlayMusic();
  FieldCamera_FollowActor(4, 1);
  FieldActor_ShowEmote(6, 0x102, 30);
  Event_ShowMessageAndWait(6, 0);
  SET_MODE(5, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  FieldActor_ShowEmote(7, 0x101, 30);
  Actor_FaceActorById(7, 4, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(7, 0, 0);
  SET_MODE(32, 3);
  Event_Wait(20);
  Event_Wait(10);
  Event_ShowMessageAndWait(32, 0);
  Motion_SetVarCbAndRefresh(6, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceActorById(5, 6, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  SET_MODE(32, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_FaceDirection(32, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  FieldActor_ShowEmote(7, 0x101, 20);
  Actor_FaceActorById(7, 32, 0);
  Actor_FaceActorById(4, 32, 0);
  Actor_FaceActorById(6, 32, 0);
  Actor_FaceActorById(5, 32, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(7, 0, 0);
  Actor_FaceActorById(32, 7, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  Actor_FaceActorById(6, 5, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetSpriteColor(Actor_GetById(5), 10);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Actor_TurnToFaceEachOther(4, 7, 0);
  Event_Wait(40);
  Motion_SetVarCbAndRefresh(32, 2);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(32, FACING_SOUTH);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetSpriteColor(Actor_GetById(5), 0);
  Event_Wait(20);
  FieldActor_ShowEmote(5, 0x100, 30);
  Actor_FaceActorById(5, 32, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  FieldActor_ShowEmote(6, 0x108, 30);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetSpriteColor(Actor_GetById(5), 10);
  Actor_FaceActorById(5, 6, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Motion_SetVarCbAndRefresh(32, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetSpriteColor(Actor_GetById(5), 0);
  Actor_FaceActorById(4, 32, 0);
  Actor_FaceActorById(5, 32, 0);
  Actor_FaceActorById(6, 32, 0);
  Actor_FaceActorById(7, 32, 0);
  Event_Wait(40);
  SET_MODE(32, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  SET_MODE(4, 3);
  SET_MODE(5, 3);
  SET_MODE(6, 3);
  SET_MODE(7, 3);
  Event_Wait(20);
  Event_Wait(20);
  SET_MODE(32, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  SET_MODE(5, 3);
  Event_Wait(20);
  Event_Wait(20);
  FieldActor_ShowEmote(32, 0x108, 30);
  Event_ShowMessageAndWait(32, 0);
  SET_MODE(6, 3);
  SET_MODE(7, 3);
  Event_Wait(20);
  Event_Wait(20);
  SET_MODE(32, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_FaceDirectionById(32, FACING_EAST);
  Event_ShowMessageAndWait(32, 0);
  SET_MODE(4, 3);
  Event_Wait(20);
  Event_Wait(20);
  SET_MODE(6, 3);
  SET_MODE(5, 3);
  Event_Wait(20);
  SET_MODE(4, 3);
  Event_Wait(20);
  SET_MODE(32, 3);
  /* FAKEMATCH(dead): this wait in a once-only block, which starts its own
   * scheduling region. */
  do {
    Event_Wait(20);
  } while (0);
  Actor_SetSpeed(32, 0x00013333, 0x9999);
  SET_MODE(32, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_SetDestinationById(32, ((s16 *)&object->x)[1],
                             ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(32);
  Actor_SetHorizontalPositionWithTerrain(32, 0, 0);
  Actor_SetSpeed(5, 0x00013333, 0x9999);
  SET_MODE(5, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_SetDestinationById(5, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(5);
  Actor_SetHorizontalPositionWithTerrain(5, 0, 0);
  Actor_SetSpeed(6, 0x00013333, 0x9999);
  SET_MODE(6, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_SetDestinationById(6, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(6);
  Actor_SetHorizontalPositionWithTerrain(6, 0, 0);
  Actor_SetSpeed(7, 0x00013333, 0x9999);
  SET_MODE(7, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_SetDestinationById(7, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(7);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  GameFlag_Set(0x980);
  GameFlag_Set(0x98e);
  Event_End();
}
#undef SET_MODE

void Madra_SkipGateInspection(void) { GameFlag_Set(0x910); }

void Madra_SkipRaidQuestioning(void) { GameFlag_Set(0x914); }

/// The leader, already beside a ladder top, turns north, steps onto the rungs
/// and climbs down out of sight, then leaves by exit 16, or 15 from the door
/// numbers above 34.
void Madra_ClimbDownLadder(void) {
  s32 next;

  next = 16;
  if (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154) > 34)
    next = 15;
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(4)->actionFlags &= 0xfe;
  Actor_SetDestinationOffset(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSpriteFlags(Actor_GetById(4), 0);
  Actor_SetAnimation(4, 13);
  Actor_SetDestinationOffset(4, 0, 16);
  Actor_WaitForMoveById(4);
  Actor_SetAnimation(4, 10);
  Event_Wait(10);
  Event_PlaySound(SE_MAP_EXIT);
  Import_EventSetExitRequest(next);
  Event_End();
}

/// Unused: the leader steps up to a ladder top from its west side and climbs
/// down; no trigger runs it.
UNUSED void Madra_StepOntoLadderFacingEast(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -14);
  Actor_WalkByAndWait(4, 16, 0);
  Madra_ClimbDownLadder();
}

/// Unused: the leader steps up to a ladder top from its east side and climbs
/// down; no trigger runs it.
UNUSED void Madra_StepOntoLadderFacingWest(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, -2, -6);
  Actor_WalkByAndWait(4, -14, -8);
  Madra_ClimbDownLadder();
}

/// Unused: the leader turns north on the spot and climbs down; no trigger runs
/// it.
UNUSED DEFINE_SCENE_TURN_ACTOR_AND_RUN(Madra_TurnAndClimbDownLadder,
                                       Madra_ClimbDownLadder)

    void Madra_EnterDoor(void);
void Madra_KarstAsksAboutMenardi(void);
void Madra_WorkerBlocksHole(void);
void Madra_AskAboutJenna(void);
void Madra_StopPartyAfterRaid(void);

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(44),
    SCENE_EXIT_IF(1, 46, 1, -1),
    SCENE_EXIT_IF(2, 45, 1, -1),
    SCENE_EXIT_IF(3, 45, 2, -1),
    SCENE_EXIT_IF(4, 45, 8, -1),
    SCENE_EXIT_IF(5, 45, 10, -1),
    SCENE_EXIT_IF(6, 45, 15, -1),
    SCENE_EXIT_IF(7, 45, 5, -1),
    SCENE_EXIT_IF(8, 45, 9, -1),
    SCENE_EXIT_IF(9, 45, 20, -1),
    SCENE_EXIT_IF(10, 46, 2, -1),
    SCENE_EXIT_IF(11, 2, 9, -1),
    SCENE_EXIT_IF(15, 48, 1, -1),
    SCENE_EXIT_IF(16, 48, 3, -1),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gMadra_Placements[] = {
    {71, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {72, CONDITION_ALWAYS, 1, PIXELS(744), 0, PIXELS(392), FACING_NORTH, 1, 0},
    {73, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(360), FACING_SOUTH, 1, 0},
    {74, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(440), FACING_NORTHWEST, 1, 0},
    {75, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(280), FACING_SOUTH, 1, 0},
    {76, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(384), FACING_NORTH, 1, 0},
    {77, CONDITION_ALWAYS, 1, PIXELS(274), 0, PIXELS(510), FACING_SOUTH, 1, 0},
    {78, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(376), FACING_NORTH, 1, 0},
    {75, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(264), FACING_SOUTHEAST, 1, 0},
    {73, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(232), FACING_SOUTH, 0, 0},
    {224, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(296), FACING_EAST, 1, 0},
    {78, CONDITION_ALWAYS, 1, PIXELS(552), 0, PIXELS(259), FACING_SOUTH, 1, 0},
    {71, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(472), FACING_NORTHEAST, 1, 0},
    {72, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(440), FACING_NORTH, 1, 0},
    {187, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(360), FACING_SOUTH + FACING_STEP, 1, 0},
    {187, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(360), FACING_SOUTH + FACING_STEP, 1, 0},
    {71, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(408), FACING_SOUTH, 1, 0},
    {71, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(408), FACING_SOUTH, 1, 0},
    {71, CONDITION_ALWAYS, 1, PIXELS(744), 0, PIXELS(248), FACING_NORTH, 0, 0},
    {187, 0x1911, 1, PIXELS(112), 0, PIXELS(440), FACING_NORTH, 1, 0},
    {192, 0x1911, 1, PIXELS(88), 0, PIXELS(440), FACING_NORTH, 1, 0},
    {205, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(272), FACING_SOUTH, 1, 0},
    {206, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(256), FACING_SOUTH, 1, 0},
    {198, 0x18ab, 1, PIXELS(280), 0, PIXELS(256), FACING_SOUTH, 1, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {200, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {88, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {186, 0x8ab, 1, PIXELS(280), 0, PIXELS(256), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMadra_PlacementsOnReturn[] = {
    {71, CONDITION_ALWAYS, 1, PIXELS(150), 0, PIXELS(344), FACING_SOUTH + FACING_STEP, 1, 0},
    {72, CONDITION_ALWAYS, 1, PIXELS(744), 0, PIXELS(392), FACING_NORTH, 1, 0},
    {73, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(360), FACING_SOUTH, 1, 0},
    {74, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(440), FACING_NORTHWEST, 1, 0},
    {75, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(280), FACING_SOUTH, 1, 0},
    {76, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(392), FACING_NORTH, 1, 0},
    {77, CONDITION_ALWAYS, 1, PIXELS(274), 0, PIXELS(510), FACING_SOUTH, 1, 0},
    {78, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(376), FACING_NORTH, 1, 0},
    {75, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(264), FACING_SOUTH, 1, 0},
    {73, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(232), FACING_SOUTH, 0, 0},
    {224, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(296), FACING_SOUTH + FACING_STEP, 1, 0},
    {78, CONDITION_ALWAYS, 1, PIXELS(552), 0, PIXELS(259), FACING_SOUTH, 1, 0},
    {71, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(472), FACING_NORTHEAST, 1, 0},
    {72, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(440), FACING_NORTHWEST, 1, 0},
    {187, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH + FACING_STEP, 1, 0},
    {187, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH + FACING_STEP, 1, 0},
    {71, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(408), FACING_SOUTH, 1, 0},
    {71, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(408), FACING_SOUTH, 1, 0},
    {71, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 0, 0},
    {187, 0x1911, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {192, 0x1911, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {205, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(272), FACING_SOUTH, 1, 0},
    {206, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(256), FACING_SOUTH, 1, 0},
    {198, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(256), FACING_SOUTH, 1, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {23, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(276), FACING_SOUTHEAST, 1, 0},
    {20, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {238, CONDITION_ALWAYS, 2, PIXELS(112), 0, PIXELS(393), FACING_SOUTH, 1, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 0, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 0, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 0, 0},
    {SCENE_TABLE_END},
};

/// A door opening: the one cell animation step of most of the town's doors.
s16 gMadra_DoorOpening[] = {
    1, 39, 1, 2, 6, -1,
};

/// The door opening of the left cavern home's two doors, from other cells.
s16 gMadra_CavernHomeDoorOpening[] = {
    3, 39, 1, 2, 6, -1,
};
// clang-format on

struct MadraPlacement gMadra_Doors[] = {
    {gMadra_CavernHomeDoorOpening, 20, 3},
    {gMadra_DoorOpening, 13, 23},
    {gMadra_DoorOpening, 17, 17},
    {gMadra_DoorOpening, 23, 18},
    {gMadra_DoorOpening, 28, 10},
    {gMadra_DoorOpening, 28, 6},
    {gMadra_DoorOpening, 9, 6},
    {gMadra_DoorOpening, 19, 24},
    {gMadra_DoorOpening, 0, 0},
    {gMadra_CavernHomeDoorOpening, 17, 3},
};

struct Trigger gMadra_TriggersBeforeRaid[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0xc602, 10, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(Madra_WorkerBlocksHole)},
    {TRIGGER_STEP, 19, 0x910, EVENT(Madra_SkipGateInspection)},
    {TRIGGER_STEP, 20, 0x910, EVENT(Madra_InspectPartyAtGate)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTheTownElder)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgYouPickedThe)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgThatBigWave)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgLetMeGuess)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgMaybeWeLl)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgIfICould)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(Madra_AskAboutBriggs)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgItSLike)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTheChampaWere)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgThoseChampaCurs)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgIDoubtThe)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgIGotSomeone)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgIDonTCare)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgWeTookThat)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgMyFriendWas)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgMyPartnerWas)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgThereSNoWay)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgAllOfIndra)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, EVENT(Madra_AskComingThroughHole)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, EVENT(Madra_AskAboutIceShaft)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgEvenIfWe)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, EVENT(Madra_TalkToWeaponMerchant)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, EVENT(Madra_TalkToArmorMerchant)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, EVENT(Madra_TalkToItemMerchant)},
    {TRIGGER_TALK, 35, CONDITION_ALWAYS, EVENT(Madra_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgEveryoneThinksBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgTheTownElderIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgIfABig)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgItSABit)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgThatChampaIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgIVeAlways)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1038, 0x300,
     EVENT(Madra_MindReadBriggsFan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgWhoWasIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgNowMadraIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgWhenISaw)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgTheChampaMust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgItSNot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgIMNotSure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgILlNever)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgILookedInto)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgIWonderHowMany)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgIMissMy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgItSSoWeird)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS,
     MSGNO(MsgTheseAreRough)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS,
     MSGNO(MsgTheChampaDug)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS,
     MSGNO(MsgItWasAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS,
     MSGNO(MsgMySoreBottom)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS,
     MSGNO(MsgTheItemVendor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS,
     MSGNO(MsgThoseLousyChampa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 31, CONDITION_ALWAYS,
     MSGNO(MsgBriggsAndHis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 35, CONDITION_ALWAYS,
     MSGNO(MsgBriggsAndHis)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMadra_TriggersAfterRaid[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0xc602, 10, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(Madra_WorkerBlocksSealedHole)},
    {TRIGGER_STEP, 19, 0x914, EVENT(Madra_SkipRaidQuestioning)},
    {TRIGGER_STEP, 20, 0x914, EVENT(Madra_StopPartyAfterRaid)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgWeThinkThe)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSoTheKibombo)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTheKibomboCame)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgIHeardThatThe)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgIGuessThat)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgGondowanSoundsLike)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(Madra_AskAboutBriggs)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgICanTBelieveSomeone)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgNowThatWe)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgThereAreStill)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgBriggsDidnT)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgTheseCavesWere)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgThatGuyWasn)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgOhWeReleased)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgEveryoneYelledAt)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgYouShouldaSeen)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgFirstPiratesRaid)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgMyGuessIs)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS,
     EVENT(Madra_AskComingThroughSealedHole)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, EVENT(Madra_TalkToWeaponMerchant)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, EVENT(Madra_TalkToArmorMerchant)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, EVENT(Madra_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgIfItWerenT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgIfGondowanIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgItSStrangeIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgTheMayorIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgHeToldUs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgInGondowanThey)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgIHeardThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgIfTheyWere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgItWasSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgIWonderWhoDug)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgWaitIfBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgIVeNeverSeenAny)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgDidnTSeem)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgWasSoHappy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgIDidnT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgICanTBelieveNobody)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgTheWayThings)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS,
     MSGNO(MsgIVeBeenStanding)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS,
     MSGNO(MsgWeSealedUp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS,
     MSGNO(MsgTheChampaWeapons)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS,
     MSGNO(MsgTheGroupThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 31, CONDITION_ALWAYS,
     MSGNO(MsgThatShinIs)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMadra_TriggersOnReturn[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0xc602, 10, CONDITION_ALWAYS, EVENT(Madra_EnterDoor)},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_STEP, 21, 0x98e, EVENT(Madra_KarstCallsForMenardi)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgIDonTThink)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgWeGoTo)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgIHeardTheBoat)},
    {TRIGGER_TALK, 11, 0x301, EVENT(Madra_AskAboutJenna)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgThoseTravelersLooked)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgIWonderIfThe)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgIDonTKnowWhatWill)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAGroupOfSharp)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgIfYouHaveA)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgIWasAt)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgItSNotThat)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTheMayorAsked)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgItSeemsNo)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgSayThereWas)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgIThoughtShin)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgWhenAndHis)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgUhhhnnnUhhhnnn2)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgTheMayorFinally)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgHeyItS)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgYeahWellWe)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, EVENT(Madra_TalkToWeaponMerchant)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, EVENT(Madra_TalkToArmorMerchant)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, EVENT(Madra_TalkToItemMerchant)},
    {TRIGGER_TALK, 33, CONDITION_ALWAYS, EVENT(Madra_KarstAsksAboutMenardi)},
    {TRIGGER_TALK, 35, CONDITION_ALWAYS, EVENT(Madra_AskAfraidOfKibombo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgWhyWouldAnyoneBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgTheyVeBeenHaving)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgEveryoneGetsThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgIWonderWhy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgWhatWasThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgThePeopleOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgThatIslandThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgHowBigWould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgTheseNewBoats)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgIfABuilding)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgSailorsAreSupposed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgOurMayorS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgIsTurningWater)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgMadraIsAlways)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgSoThisFellow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgIKnewI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgHeWasGone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS,
     MSGNO(MsgHowMuchLongerDo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS,
     MSGNO(MsgThePiratesAnd)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS,
     MSGNO(MsgThatNightThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 31, CONDITION_ALWAYS,
     MSGNO(MsgShinSeemsTerrified)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 33, CONDITION_ALWAYS,
     EVENT(Madra_MindReadKarst)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 35, CONDITION_ALWAYS,
     MSGNO(MsgWeReStill)},
    {TRIGGER_TABLE_END},
};
