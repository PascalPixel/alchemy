/// @file
/// Below the decks of the ship across the Karagol Sea: the captain's cabin, the
/// passengers' hold and the rowing deck. After each monster attack the party
/// chooses which passenger rows at the empty seat; the attacks fought off
/// number the seat being filled, and each seat has a group of flags, one for
/// each passenger.

#include "script.h"
#include "flags.h"
#include "songs.h"
#include "macros.h"
#include "game_state.h"
#include "unit/import.h"
#include "unit/scene/tolbi_ship_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "callback_scheduler.h"
#include "scene_ids.h"
#include "inventory.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "field_runtime.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.
#define FLAG_TOLBI_SHIP_ROWER_CHOSEN 0x300    ///< a passenger follows the party
#define FLAG_TOLBI_SHIP_MONSTERS_ABOARD 0x301 ///< the hold's stairs are barred
/// The Kraken is aboard: the deck shakes until it is defeated
/// (tolbi_ship_deck.c reads it too). "There's a huge sea monster on the ship!"
#define FLAG_TOLBI_SHIP_KRAKEN_ABOARD 0x302

/// Operations shared by the cabin, passenger hold and rowing-deck events.
enum ShipSceneCommand {
  SHIP_FACE_PARTY_EAST_AND_WEST,
  SHIP_FACE_PARTY,
  SHIP_PLAY_PARTY_ANIMATION,
  SHIP_SHOW_PARTY_EMOTE,
  SHIP_HIDE_PASSENGERS,
  SHIP_START_ROWING_ANIMATION,
  SHIP_START_MIRRORED_ROWING_ANIMATION,
  SHIP_SET_SELECTED_ROWER_POSE,
  SHIP_OPEN_SCENE,
  SHIP_CLOSE_SCENE,
  SHIP_SETUP_ROWING_PARTY,
  SHIP_RESTORE_ROWING_DECK_POSES,
  SHIP_RESTORE_ROWER_POSE,
  SHIP_HIDE_CABIN_CREW,
  SHIP_HIDE_ACTORS_13_AND_14,
  SHIP_SETUP_CAPTAINS_CABIN,
  SHIP_SETUP_RECRUITMENT_DECK,
  SHIP_HIDE_ACTORS_28_TO_35,
  SHIP_SETUP_PASSENGER_HOLD,
  SHIP_SETUP_MONSTER_ENCOUNTER,
  SHIP_CLEAR_FLAG_RANGE,
  SHIP_CLEAR_ROWING_SELECTIONS,
  SHIP_RESTORE_ACTOR_12_SCRIPT,
  SHIP_HIDE_PARTY_FOLLOWERS,
  SHIP_RESET_CAMERA,
  SHIP_PLACE_FOLLOWERS_AND_MONSTERS
};

/// The warriors bound for Colosso in the hold while the ship waits at Kalay
/// (gTolbiShipInteriors_HoldPlacements): Ouranos and his friend Sean. Ouranos
/// "can't stand water".
enum TolbiShipInteriorsActor {
  HOLD_OURANOS = 13,
  HOLD_SEAN = 14,
};

/// A speaker: the actor, and in the top four bits where the message window
/// opens beside them.
#define SPEAKER(actor, window) ((window) << 12 | (actor))

// Defined below, in the order of the ROM.
static void TolbiShipInteriors_StepBesideLeader(s32 passengerId);
static s32 TolbiShipInteriors_IsSpotFree(s32 bucket);
static s32 TolbiShipInteriors_GetChosenOarsman(void);
static void TolbiShipInteriors_AskToRow(s32 speaker, s32 question,
                                        s32 seatFlag);
static void TolbiShipInteriors_DiscussLaunchingWithoutAnchor(void);
static void TolbiShipInteriors_SetUpCabin(void);
static void TolbiShipInteriors_SetUpHold(void);
static void TolbiShipInteriors_SetUpRowingDeck(void);
static void TolbiShipInteriors_SetSeatActorCollision(void);
static void TolbiShipInteriors_AskForReplacementOarsmen(void);
static void TolbiShipInteriors_CastOff(void);
static void TolbiShipInteriors_AnnounceDeparture(void);
static void TolbiShipInteriors_CallActorSixteenToRow(void);
static void TolbiShipInteriors_CallActorEighteenToRow(void);
static void TolbiShipInteriors_ShowRowing(void);
static void TolbiShipInteriors_AskAboutMonsters(void);
static void TolbiShipInteriors_AnnounceTurn(void);
static void TolbiShipInteriors_StartFirstAttack(void);
static void TolbiShipInteriors_EndFirstAttack(void);
static void TolbiShipInteriors_IntroduceReplacementOarsman(s32 seat);
static struct FieldActor *TolbiShipInteriors_PlaceAnchorCharm(void);
static void TolbiShipInteriors_InstallRowingDeckActors(u32 action);
static void TolbiShipInteriors_Say(s32 speaker);
static void TolbiShipInteriors_Turn(s32 actorId, u16 facing);
static void TolbiShipInteriors_PlaceActor(s32 actorId, s32 x, s32 z,
                                          s32 facing);
static void TolbiShipInteriors_MoveCameraAndWait(s32 x, s32 y, s32 z,
                                                 u32 flags);
static void TolbiShipInteriors_RunSceneCommand(s32 step, u32 arg, u32 opt);
static s32 TolbiShipInteriors_FindFirstSetFlag(u32 group, s32 alternate);
static void TolbiShipInteriors_HideSelectedPassengers(void);
static void TolbiShipInteriors_HideSelectedPassenger(s32 actorId,
                                                     s32 firstFlag);
static void TolbiShipInteriors_StartSecondAttack(void);
static void TolbiShipInteriors_ReportMonstersBelowdecks(void);
static void TolbiShipInteriors_ReactToMonsterAttack(void);
static void TolbiShipInteriors_EndSecondAttack(void);
static void TolbiShipInteriors_StartThirdAttack(void);
static void TolbiShipInteriors_EndThirdAttack(void);
static void TolbiShipInteriors_NoticeShipRocking(void);
static void TolbiShipInteriors_ShakeWhileKrakenAboard(void);
static void TolbiShipInteriors_ShowCabinBeforeKraken(void);
static void TolbiShipInteriors_ReachIsland(void);
static void TolbiShipInteriors_DismissReplacementRowers(void);
static void TolbiShipInteriors_SwapRowerSelections(s32 firstSeatFlags,
                                                   s32 secondSeatFlags);

/// An action-script call: stands the actor still at the origin, with no
/// velocity and no target. Returns 0.
static s32
TolbiShipInteriors_ResetActor(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  Object_SetMode(actor, 1);
  actor->x.fixed = 0;
  actor->y.fixed = 0;
  actor->z.fixed = 0;
  SET_VELOCITY(actor, 0, 0, 0);
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetX = ACTOR_NO_TARGET;
  return 0;
}

/// Bobs the actor between the floor and 8 pixels up by a random amount each
/// frame. Returns 1.
static s32 TolbiShipInteriors_Bob(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  s16 verticalMotionDirection;
  s32 descendingY;
  s32 ascendingY;

  if ((s16)actor->work != 0) {
    descendingY = (actor->y.fixed - ((u32)(Engine_RandomNext() << 15) >> 16)) -
                  PIXELS(1) / 2;
    actor->y.fixed = descendingY;
    if (descendingY < 0) {
      verticalMotionDirection = 0;
      actor->work = verticalMotionDirection;
    }
  } else {
    ascendingY = actor->y.fixed + ((u32)(Engine_RandomNext() << 15) >> 16) +
                 PIXELS(1) / 2;
    actor->y.fixed = ascendingY;
    if (ascendingY > PIXELS(8)) {
      verticalMotionDirection = 1;
      actor->work = verticalMotionDirection;
    }
  }
  return 1;
}

/// Now and then (two chances in 64) turns the actor to face south-southeast or
/// south-southwest. Returns 1.
static s32
TolbiShipInteriors_LookAround(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  u32 choice = (u32)(Engine_RandomNext(actor) << 6) >> 16;

  if (choice == 6) {
    s32 facing = 0x3000;
    actor->facing = facing;
  } else if (choice == 9) {
    s32 facing = 0x5000;
    actor->facing = facing;
  }
  return 1;
}

/// Counts the frames of a pause in riseCounter and moves the pacing actor to
/// its next step after 80 frames.
static void TolbiShipInteriors_WaitThenAdvance(struct FieldActor *actor) {
  u8 *counter = &actor->riseCounter;
  s32 frame = *counter + 1;

  *counter = frame;
  if ((u8)frame > 80) {
    actor->work += 1;
  }
}

/// Counts down the actor's walking time in riseBaseY. Returns 1 once the time
/// is up or the actor has reached its target, else 0.
static s32 TolbiShipInteriors_HasArrived(struct FieldActor *actor) {
  s32 remaining = actor->riseBaseY;

  if (remaining != 0) {
    actor->riseBaseY = remaining - 1;
  } else {
    return 1;
  }
  if (actor->targetX == ACTOR_NO_TARGET && actor->targetY == actor->targetX &&
      actor->targetZ == actor->targetY) {
    return 1;
  }
  return 0;
}

/// The update of a crewman who paces between two spots in the cabin, looking
/// about at each, and signals actor 9 through riseEnabled at three points of
/// the round. While actor 8 is paused he sends no signals. work holds the step.
static void TolbiShipInteriors_PaceCabin(union FieldObject *fieldObject) {
  struct FieldActor *obj = (struct FieldActor *)fieldObject;
  struct FieldActor *leader;

  leader = Object_GetById(8);
  switch ((s16)obj->work) {
  case 0:
    obj->facing = 0xb000;
    obj->work++;
    obj->riseCounter = 0;
    break;
  case 2:
    obj->facing = 0;
    obj->work++;
    obj->riseCounter = 0;
    break;
  case 4:
    Object_SetMode(obj, 2);
    FieldObject_SetPosition(obj, PIXELS(468), PIXELS(32), PIXELS(632));
    obj->riseBaseY = 60;
    obj->work++;
    break;
  case 5:
    if (TolbiShipInteriors_HasArrived(obj) != 0) {
      Object_SetMode(obj, 1);
      obj->riseCounter = 0;
      if (leader->paused == 0) {
        obj->riseEnabled = 1;
      }
      obj->work++;
    }
    break;
  case 7:
    if (leader->paused == 0) {
      Object_SetMode(obj, 3);
      obj->riseEnabled = 2;
    }
    obj->work++;
    obj->riseCounter = 0;
    break;
  case 9:
    Object_SetMode(obj, 2);
    FieldObject_SetPosition(obj, PIXELS(480), PIXELS(32), PIXELS(600));
    obj->riseBaseY = 60;
    obj->work++;
    if (leader->paused == 0) {
      obj->riseEnabled = 3;
    }
    break;
  case 10:
    if (TolbiShipInteriors_HasArrived(obj) != 0) {
      Object_SetMode(obj, 1);
      obj->riseCounter = 0;
      obj->work++;
    }
    break;
  case 1:
  case 3:
  case 6:
  case 8:
  case 11:
    TolbiShipInteriors_WaitThenAdvance(obj);
    break;
  case 12:
    obj->work = 0;
    break;
  }
}

/// The update of the actor who watches the pacing crewman: turns
/// north-northeast at the first signal, shrugs at the second if still turned,
/// and faces east at the third. Does nothing while paused.
static void TolbiShipInteriors_WatchPacer(union FieldObject *fieldObject) {
  struct FieldActor *watcher = (struct FieldActor *)fieldObject;
  struct FieldActor *pacer;

  pacer = Object_GetById(9);
  if (watcher->paused != 0)
    return;
  if (pacer->riseEnabled == 1) {
    watcher->facing = 0xd000;
    watcher->riseCounter = 1;
    pacer->riseEnabled = 0;
  } else if (pacer->riseEnabled == 2) {
    if (watcher->riseCounter != 0)
      Object_SetMode(watcher, 3);
    watcher->riseCounter = 0;
    pacer->riseEnabled = 0;
  } else if (pacer->riseEnabled == 3) {
    watcher->facing = 0;
    pacer->riseEnabled = 0;
  }
}

/// An action-script call: lays the actor flat on the floor, drawn underfoot at
/// priority 1 with its parts hidden. Returns 1.
static s32 TolbiShipInteriors_LayFlat(struct ScriptInterpreter *interpreter) {
  struct FieldActor *work = (struct FieldActor *)interpreter;
  struct AnimationObject *sprite = work->sprite;

  work->collisionFlags = 8;
  Engine_ActorSetSpriteFlags(work, 0);
  sprite->image.priority = 1;
  sprite->shadow.priority = 1;
  work->priorityFlags = (work->priorityFlags & ~1) | 2;
  ObjectGroup_SetChildValue(work, 15);
  return 1;
}

s32 TolbiShipInteriors_SelectEntrances(void) {
  if (gGameState.scene == (s32)&SceneId_TolbiShipInteriors) {
    return (s32)gTolbiShipInteriors_Entrances;
  }
  return (s32)gTolbiShipInteriors_EntrancesOther;
}

s32 TolbiShipInteriors_GetRegions(void) { return 0; }

s32 TolbiShipInteriors_GetExits(void) {
  return (s32)&gTolbiShipInteriors_Exits;
}

/// Picks the placements for the room entered and the voyage's stage. Some
/// stages change who turns to face the party when talked to (talkFacing) in the
/// shared tables.
s32 TolbiShipInteriors_SelectPlacements(void) {
  u8 *placementBytes;

  switch (gGameState.entrance) {
  case 1:
  case 2:
  case 11:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return (s32)gTolbiShipInteriors_HoldPlacements;
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) !=
               0) {
      if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
        // Update talkFacing in the placement rows, preserving their write
        // order.
        placementBytes = (u8 *)gTolbiShipInteriors_HoldPlacementsAtSea;
        placementBytes[22] = 2;
        placementBytes[70] = 2;
        placementBytes[118] = 2;
        placementBytes[142] = 2;
        placementBytes[214] = 2;
        placementBytes[190] = 2;
        placementBytes[166] = 1;
        placementBytes[94] = 2;
      }
      return (s32)gTolbiShipInteriors_HoldPlacementsAtSea;
    } else if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
        gTolbiShipInteriors_HoldPlacements[0].talkFacing = 2;
        gTolbiShipInteriors_HoldPlacements[4].talkFacing = 2;
        gTolbiShipInteriors_HoldPlacements[1].talkFacing = 2;
        gTolbiShipInteriors_HoldPlacements[3].talkFacing = 2;
      }
      return (s32)gTolbiShipInteriors_HoldPlacements;
    } else {
      return (s32)gTolbiShipInteriors_HoldPlacementsEmpty;
    }
    break;
  case 4:
  case 12:
  case 16:
  case 18:
  case 20:
  case 21:
  case 23:
  case 24:
    return (s32)gTolbiShipInteriors_RowingDeckPlacements;
    break;
  case 15:
  case 17:
  case 19:
    placementBytes = (u8 *)gTolbiShipInteriors_RowingDeckPlacements;
    placementBytes[22] = 2;
    placementBytes[46] = 2;
    placementBytes[94] = 1;
    placementBytes[118] = 2;
    placementBytes[142] = 2;
    placementBytes[166] = 2;
    placementBytes[190] = 2;
    placementBytes[214] = 1;
    placementBytes[238] = 2;
    return (s32)placementBytes;
  case 5:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return (s32)gTolbiShipInteriors_CabinPlacementsArrived;
    } else if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
        if (GameFlag_IsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
          gTolbiShipInteriors_CabinPlacementsWarriors[1].talkFacing = 1;
        }
        if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
          if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) == 0) {
            gTolbiShipInteriors_CabinPlacementsWarriors[0].talkFacing = 0;
          }
        }
        return (s32)gTolbiShipInteriors_CabinPlacementsWarriors;
      }
      return (s32)gTolbiShipInteriors_PlacementsNone;
    } else {
      return (s32)gTolbiShipInteriors_CabinPlacements;
    }
    break;
  case 10:
  case 13:
  case 14:
  case 22:
    return (s32)gTolbiShipInteriors_CabinPlacements;
    break;
  default:
    return (s32)gTolbiShipInteriors_PlacementsNone;
    break;
  }
  return (s32)placementBytes;
}

/// Chooses the room's interactions for the current stage of the voyage.
struct SceneEvent *TolbiShipInteriors_SelectEvents(void) {
  s32 entrance = gGameState.entrance;

  switch (entrance) {
  case 1:
  case 2:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      return gTolbiShipInteriors_HoldEventsAtIsland;
    }
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0 &&
        Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
      return gTolbiShipInteriors_HoldEventsChoosingRowers;
    }
    return gTolbiShipInteriors_HoldEvents;
  case 4:
  case 23:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gTolbiShipInteriors_RowingDeckEventsArrived;
    }
    return gTolbiShipInteriors_RowingDeckEvents;
  case 5:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      return gTolbiShipInteriors_CabinEventsAtIsland;
    }
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gTolbiShipInteriors_CabinEventsArrived;
    }
    return gTolbiShipInteriors_CabinEvents;
  case 15:
  case 17:
  case 19:
    return gTolbiShipInteriors_RowingDeckEventsMonstersAboard;
  case 21:
    return gTolbiShipInteriors_RowingDeckEventsKrakenAboard;
  default:
    break;
  }

  return gTolbiShipInteriors_EventsOther;
}

/// Once the warriors want to see the captain, actor 10 asks whether the party
/// wants to join them in forcing the launch.
static void TolbiShipInteriors_TalkToCaptainsCabinActor10(void) {
  Engine_EventBegin();
  Battle_ResetEffectCounter();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL) != 0) {
    Engine_EventSetMessage(MsgTolbiShipIfShipFromTolbiHad);
    Event_ShowMessage(10, 0);
  } else {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
      Engine_EventSetMessage(MsgTolbiShipNowWantSeeCaptainToo);
      Event_OpenMessage(10, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        TolbiShipInteriors_DiscussLaunchingWithoutAnchor();
      } else {
        Engine_ActorStartRepeatedMotion(10, 2);
        Event_ShowMessage(10, 0);
        Actor_FaceDirection(10, 0xd000, 0);
      }
    } else {
      Engine_EventSetMessage(MsgTolbiShipButWeCantSendShip);
      Event_ShowMessage(10, 0);
    }
  }
  Engine_EventEnd();
}

/// Actor 12 in the captain's cabin asks whether the party came to ask the
/// captain to set sail.
static void TolbiShipInteriors_TalkToCaptainsCabinActor12(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiShipYouCameAskCaptainSet);
  Engine_EventAskYesNo(12, 0);
  Engine_EventEnd();
}

/// The captain, by the voyage's stage: too late to hire mercenaries, then his
/// bad luck at losing his lucky anchor, then uncertain what might happen, and
/// under sail the news of an injured oarsman. After finding the Anchor Charm,
/// talking to him also sets the event work's unk172 request.
static void TolbiShipInteriors_TalkToCaptain(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0) {
    Engine_EventSetMessage(MsgTolbiShipOarsmanWasInjured);
    TolbiShipInteriors_Say(8);
    Actor_FaceDirection(8, 0xd000, 60);
    Engine_ActorSetAnimationAndWait(8, 4);
    TolbiShipInteriors_Say(8);
    Engine_ActorSetAnimationAndWait(8, 3);
  } else if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Engine_EventSetMessage(MsgTolbiShipWeDontKnowMightHappen);
    Event_ShowMessage(8, 0);
  } else if (GameFlag_IsSet(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL) != 0) {
    Engine_EventSetMessage(MsgTolbiShipBadLuckLosingMyLucky);
    Event_ShowMessage(8, 0);
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) == 0 &&
        GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND) != 0) {
      gEventWork->unk172 = 1;
    }
  } else {
    Engine_EventSetMessage(MsgTolbiShipItsTooLateHireMercenaries);
    Event_ShowMessage(8, 0);
  }
  Engine_EventEnd();
}

/// A passenger in the hold while the ship waits. Once the party has agreed to
/// protect the ship, she worries the proud warriors won't like being told to
/// row, and asks the party.
static void TolbiShipInteriors_TalkToWaitingPassenger8(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventSetMessage(MsgTolbiShipTheseProudWarriorsNotGoing);
    TolbiShipInteriors_Say(8);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 10);
    Event_OpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(40);
      TolbiShipInteriors_Say(8);
      TolbiShipInteriors_Turn(8, 0x3000);
      Event_ShowMessage(8, 0);
    } else {
      gEventWork->message += 2;
      Event_ShowMessage(8, 0);
      Actor_FaceDirection(8, 0x3000, 0);
    }
  } else {
    Engine_EventSetMessage(MsgTolbiShipLongerWeSitHereMore);
    Event_ShowMessage(8, 0);
  }
  Engine_EventEnd();
}

/// Another waiting passenger, once the party has agreed to protect the ship,
/// asks whether it will row. "Are you going to row?"
static void TolbiShipInteriors_TalkToWaitingPassenger10(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Engine_EventSetMessage(MsgTolbiShipYouGoingRow);
    Engine_EventAskYesNo(10, 0);
  } else {
    Engine_EventSetMessage(MsgTolbiShipTheresWholeBunchReallyMuscular);
    Engine_EventShowMessage(10, 0);
  }
  Engine_EventEnd();
}

/// Walks the rower back to the leader and hides its separate scene actor.
static inline void TolbiShipInteriors_RejoinParty(s32 rowerId) {
  struct FieldActor *leader;

  Engine_ActorSetAnimation(rowerId, 2);
  leader = Object_GetById(0);
  if (leader != 0) {
    Engine_ActorSetDestination(rowerId, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(rowerId);
  Engine_ActorSetPosition(rowerId, 0, 0);
}

/// Passenger 8 under sail, when an oar needs a rower. If someone has already
/// been chosen this round, the chosen passenger steps beside the
/// leader and follows them out; otherwise the party may ask passenger 8 to row
/// at the open seat.
static void TolbiShipInteriors_AskPassenger8ToRow(void) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    s32 rowerId = TolbiShipInteriors_GetChosenOarsman();

    Engine_EventBegin();
    TolbiShipInteriors_StepBesideLeader(rowerId);
    Engine_EventSetMessage(MsgTolbiShipLooksLikeYouveBeenChosen);
    TolbiShipInteriors_Say(8);
    TolbiShipInteriors_RejoinParty(rowerId);
    Engine_EventEnd();
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    TolbiShipInteriors_AskToRow(8, MsgTolbiShipNoooooNotGoing,
                                FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED) != 0) {
    TolbiShipInteriors_AskToRow(8, MsgTolbiShipNoooooNotGoing,
                                FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED) !=
             0) {
    TolbiShipInteriors_AskToRow(8, MsgTolbiShipNoooooNotGoing,
                                FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS);
  } else {
    TolbiShipInteriors_AskToRow(8, MsgTolbiShipNoooooNotGoing,
                                FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS);
  }
}

/// The same for passenger 10.
static void TolbiShipInteriors_AskPassenger10ToRow(void) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    s32 rowerId = TolbiShipInteriors_GetChosenOarsman();

    Engine_EventBegin();
    TolbiShipInteriors_StepBesideLeader(rowerId);
    Engine_EventSetMessage(MsgTolbiShipHaHaHaChosenOarsman);
    TolbiShipInteriors_Say(10);
    TolbiShipInteriors_RejoinParty(rowerId);
    Engine_EventEnd();
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    TolbiShipInteriors_AskToRow(10, MsgTolbiShipJokingWantRow,
                                FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + 2);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED) != 0) {
    TolbiShipInteriors_AskToRow(10, MsgTolbiShipJokingWantRow,
                                FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + 2);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED) !=
             0) {
    TolbiShipInteriors_AskToRow(10, MsgTolbiShipJokingWantRow,
                                FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 2);
  } else {
    TolbiShipInteriors_AskToRow(10, MsgTolbiShipJokingWantRow,
                                FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 2);
  }
}

/// The same for passenger 11, who is too seasick to talk once the ship is at
/// anchor.
static void TolbiShipInteriors_AskPassenger11ToRow(void) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    Engine_EventBegin();
    Engine_ActorSetAttachedEffect(11, 0x102);
    Engine_EventWait(40);
    Engine_EventSetMessage(MsgTolbiShipDontFeelDontTalkMe);
    Engine_EventShowMessage(11, 0);
    Engine_EventEnd();
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    s32 rowerId = TolbiShipInteriors_GetChosenOarsman();

    Engine_EventBegin();
    TolbiShipInteriors_StepBesideLeader(rowerId);
    Engine_EventSetMessage(MsgTolbiShipYouFinallyPickedSomeoneDidnt);
    TolbiShipInteriors_Say(11);
    TolbiShipInteriors_RejoinParty(rowerId);
    Engine_EventEnd();
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    TolbiShipInteriors_AskToRow(11, MsgTolbiShipDontTellGoing,
                                FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + 3);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED) != 0) {
    TolbiShipInteriors_AskToRow(11, MsgTolbiShipDontTellGoing,
                                FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + 3);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED) !=
             0) {
    TolbiShipInteriors_AskToRow(11, MsgTolbiShipDontTellGoing,
                                FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 3);
  } else {
    TolbiShipInteriors_AskToRow(11, MsgTolbiShipDontTellGoing,
                                FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 3);
  }
}

/// The same for passenger 12, written out in full rather than through
/// TolbiShipInteriors_AskToRow.
static void TolbiShipInteriors_AskPassenger12ToRow(void) {
  s32 rowerId;
  struct FieldActor *actor;

  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = TolbiShipInteriors_GetChosenOarsman();
    TolbiShipInteriors_StepBesideLeader(rowerId);
    Engine_EventSetMessage(MsgTolbiShipGivesMeChillsThinkCould);
    TolbiShipInteriors_Say(12);
    TolbiShipInteriors_RejoinParty(rowerId);
  } else {
    Engine_ActorRunRepeatedMotion(12, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgTolbiShipOhhhhNooooGoingMakeMe);
    Event_OpenMessage(12, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      TolbiShipInteriors_Say(12);
      Engine_ActorSetAnimation(12, 2);
      actor = Object_GetById(0);
      if (actor != 0) {
        Actor_SetDestination(12, actor->x.part.pixel, actor->z.part.pixel);
      }
      Engine_ActorWaitForMove(12);
      Actor_SetPosition(12, 0, 0);
      GameFlag_Set(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
      if (GameFlag_IsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + 4);
      } else if (GameFlag_IsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + 4);
      } else if (GameFlag_IsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 4);
      } else {
        GameFlag_Set(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 4);
      }
    } else {
      gEventWork->message++;
      TolbiShipInteriors_Say(12);
    }
  }
  Engine_EventEnd();
}

/// The same for passenger 9, written out in full.
static void TolbiShipInteriors_AskPassenger9ToRow(void) {
  s32 rowerId;
  struct FieldActor *actor;

  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = TolbiShipInteriors_GetChosenOarsman();
    TolbiShipInteriors_StepBesideLeader(rowerId);
    Engine_EventSetMessage(MsgTolbiShipIsaacYouveGotGoodEye);
    TolbiShipInteriors_Say(9);
    TolbiShipInteriors_RejoinParty(rowerId);
  } else {
    Engine_EventSetMessage(MsgTolbiShipHaHaHaRowingFeel);
    Engine_EventShowMessageAndWait(9, 0, 60);
    Engine_ActorRunRepeatedMotion(9, 1);
    Event_OpenMessage(9, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      TolbiShipInteriors_Say(9);
      Engine_ActorSetAnimation(9, 2);
      actor = Object_GetById(0);
      if (actor != 0) {
        Actor_SetDestination(9, actor->x.part.pixel, actor->z.part.pixel);
      }
      Engine_ActorWaitForMove(9);
      Actor_SetPosition(9, 0, 0);
      GameFlag_Set(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
      if (GameFlag_IsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + 1);
      } else if (GameFlag_IsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + 1);
      } else if (GameFlag_IsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED) != 0) {
        GameFlag_Set(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 1);
      } else {
        GameFlag_Set(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 1);
      }
    } else {
      gEventWork->message++;
      TolbiShipInteriors_Say(9);
    }
  }
  Engine_EventEnd();
}

/// The same for passenger 13.
static void TolbiShipInteriors_AskPassenger13ToRow(void) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    s32 rowerId = TolbiShipInteriors_GetChosenOarsman();

    Engine_EventBegin();
    TolbiShipInteriors_StepBesideLeader(rowerId);
    Engine_EventSetMessage(MsgTolbiShipThatsWhoYouPickedOarsman);
    TolbiShipInteriors_Say(13);
    TolbiShipInteriors_RejoinParty(rowerId);
    Engine_EventEnd();
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    TolbiShipInteriors_AskToRow(13, MsgTolbiShipNotThinkingMaking,
                                FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + 5);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED) != 0) {
    TolbiShipInteriors_AskToRow(13, MsgTolbiShipNotThinkingMaking,
                                FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + 5);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED) !=
             0) {
    TolbiShipInteriors_AskToRow(13, MsgTolbiShipNotThinkingMaking,
                                FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 5);
  } else {
    TolbiShipInteriors_AskToRow(13, MsgTolbiShipNotThinkingMaking,
                                FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 5);
  }
}

/// The same for passenger 14.
static void TolbiShipInteriors_AskPassenger14ToRow(void) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    s32 rowerId = TolbiShipInteriors_GetChosenOarsman();

    Engine_EventBegin();
    TolbiShipInteriors_StepBesideLeader(rowerId);
    Engine_EventSetMessage(MsgTolbiShipIfYouveChosenOarsmanLets);
    TolbiShipInteriors_Say(14);
    TolbiShipInteriors_RejoinParty(rowerId);
    Engine_EventEnd();
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    TolbiShipInteriors_AskToRow(14, MsgTolbiShipOarsmanGiveBreak,
                                FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + 6);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED) != 0) {
    TolbiShipInteriors_AskToRow(14, MsgTolbiShipOarsmanGiveBreak,
                                FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + 6);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED) !=
             0) {
    TolbiShipInteriors_AskToRow(14, MsgTolbiShipOarsmanGiveBreak,
                                FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 6);
  } else {
    TolbiShipInteriors_AskToRow(14, MsgTolbiShipOarsmanGiveBreak,
                                FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 6);
  }
}

/// The same for passenger 15.
static void TolbiShipInteriors_AskPassenger15ToRow(void) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    s32 rowerId = TolbiShipInteriors_GetChosenOarsman();

    Engine_EventBegin();
    TolbiShipInteriors_StepBesideLeader(rowerId);
    Engine_EventSetMessage(MsgTolbiShipYouveBeenChosenAsOarsman);
    TolbiShipInteriors_Say(15);
    TolbiShipInteriors_RejoinParty(rowerId);
    Engine_EventEnd();
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    TolbiShipInteriors_AskToRow(15, MsgTolbiShipPoorOarsmanFeel,
                                FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + 7);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED) != 0) {
    TolbiShipInteriors_AskToRow(15, MsgTolbiShipPoorOarsmanFeel,
                                FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + 7);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED) !=
             0) {
    TolbiShipInteriors_AskToRow(15, MsgTolbiShipPoorOarsmanFeel,
                                FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 7);
  } else {
    TolbiShipInteriors_AskToRow(15, MsgTolbiShipPoorOarsmanFeel,
                                FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 7);
  }
}

/// The same for passenger 16.
static void TolbiShipInteriors_AskPassenger16ToRow(void) {
  s32 rowerId;

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = TolbiShipInteriors_GetChosenOarsman();
    Engine_EventBegin();
    TolbiShipInteriors_StepBesideLeader(rowerId);
    Engine_EventSetMessage(MsgTolbiShipHoHoPersonGoingGet);
    TolbiShipInteriors_Say(16);
    TolbiShipInteriors_RejoinParty(rowerId);
    Engine_EventEnd();
  } else {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
      TolbiShipInteriors_AskToRow(16, MsgTolbiShipPeopleAskingFrail,
                                  FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + 8);
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED) !=
               0) {
      TolbiShipInteriors_AskToRow(16, MsgTolbiShipPeopleAskingFrail,
                                  FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + 8);
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED) !=
               0) {
      TolbiShipInteriors_AskToRow(16, MsgTolbiShipPeopleAskingFrail,
                                  FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 8);
    } else {
      TolbiShipInteriors_AskToRow(16, MsgTolbiShipPeopleAskingFrail,
                                  FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 8);
    }
  }
}

/// Finds a placed actor within twelve pixels of the requested floor position.
static struct FieldActor *TolbiShipInteriors_FindActorNear(s32 x, s32 z) {
  struct EventWork *work;
  struct FieldActor **actor;
  struct FieldActor *current;
  u32 i;
  s32 actorX;
  s32 actorZ;
  s32 left;
  s32 top;
  s32 right;
  s32 bottom;

  work = gEventWork;
  i = 8;
  left = x - 12;
  right = x + 12;
  top = z - 12;
  bottom = z + 12;
  actor = work->placedActors;
  while (i <= 65) {
    current = *actor++;
    actorX = current->x.part.pixel;
    actorZ = current->z.part.pixel;
    if (left < actorX && right > actorX && top < actorZ && bottom > actorZ)
      return current;
    i++;
  }
  return 0;
}

/// The chosen oarsman steps out beside the leader, on the leader's right if
/// that spot is free, else on the left, else behind, and turns the way the
/// leader faces.
static void TolbiShipInteriors_StepBesideLeader(s32 passengerId) {
  struct FieldActor *leader;
  struct FieldActor *leaderPosition;
  s32 blocked;
  s32 direction;

  leader = Object_GetById(0);
  blocked = 1;
  Engine_ActorSetSpritePriority(passengerId, 2);
  {
    struct FieldActor *passenger = Object_GetById(passengerId);

    passenger->priorityFlags =
        ACTOR_PRIORITY_AUTOMATIC | passenger->priorityFlags;
  }
  // Try the leader's right, then left, then the space behind.
  direction = (((leader->facing + 0x4000) & 0xf000) >> 12);
  if (TolbiShipInteriors_IsSpotFree(
          (((leader->facing + 0x4000) & 0xf000) >> 12)) != 0) {
    blocked = 0;
  }
  if (blocked != 0) {
    direction = (((leader->facing + -0x4000) & 0xf000) >> 12);
    if (TolbiShipInteriors_IsSpotFree(
            (((leader->facing + -0x4000) & 0xf000) >> 12)) != 0) {
      blocked = 0;
    }
    if (blocked != 0) {
      direction = (((leader->facing + 0x8000) & 0xf000) >> 12);
    }
  }
  leaderPosition = Object_GetById(0);
  if (leaderPosition != 0) {
    Engine_ActorSetPosition(passengerId, leaderPosition->x.fixed,
                            leaderPosition->z.fixed);
  }
  Actor_SetSpeed(passengerId, PIXELS(8) / 5, PIXELS(4) / 5);
  Engine_ActorSetAnimation(passengerId, 2);
  {
    s32 offset = gTolbiShipInteriors_TurnSteps[direction];

    Engine_ActorSetDestinationOffset(passengerId, offset >> 16,
                                     (offset << 16) >> 16);
  }
  Engine_ActorWaitForMove(passengerId);
  Engine_ActorSetAnimation(passengerId, 1);
  TolbiShipInteriors_Turn(passengerId, leader->facing);
}

enum { CabinSpeakerRequest = 0xa01b };

/// Returns 1 when the spot next to the leader in the given sixteenth of a turn
/// holds no actor and no wall, else 0.
static s32 TolbiShipInteriors_IsSpotFree(s32 bucket) {
  struct FieldActor *obj = Object_GetById(0);
  u32 offset = gTolbiShipInteriors_TurnSteps[bucket];
  s32 x = obj->x.part.pixel + ((s32)offset >> 16);
  s32 z = obj->z.part.pixel + (s16)offset;

  if (TolbiShipInteriors_FindActorNear(x, z) != 0) {
    return 0;
  }

  {
    struct FieldPosition point;
    point.x = x << 16;
    point.y = obj->y.fixed;
    point.z = z << 16;

    if (FieldObject_CheckMovementCollision(obj, &point) != 0) {
      return 0;
    }
  }

  return 1;
}

/// Returns the passenger chosen to row at the open seat, which the attacks
/// fought off number.
static s32 TolbiShipInteriors_GetChosenOarsman(void) {
  s32 ret = 0;

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    ret = 3;
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED) != 0) {
    ret = 2;
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED) !=
             0) {
    ret = 1;
  }

  return TolbiShipInteriors_FindFirstSetFlag(ret, 1);
}

/// A passenger at the oars, by the round they were chosen in: Kaja's men are
/// worried, monsters are everywhere, or the ship is starting to list.
static void TolbiShipInteriors_TalkToRower18(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS))
    Engine_EventSetMessage(MsgTolbiShipThingHasKajaHisMen);
  else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS))
    Engine_EventSetMessage(MsgTolbiShipMonstersEverywhereImStuckRowing);
  else
    Engine_EventSetMessage(MsgTolbiShipShipStartingListIfWe);
  Engine_EventShowMessage(0x12, 0);
  Engine_EventEnd();
}

/// The same for rower 19.
static void TolbiShipInteriors_TalkToRower19(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 1))
    Engine_EventSetMessage(MsgTolbiShipThingHasKajaHisMen);
  else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 1))
    Engine_EventSetMessage(MsgTolbiShipMonstersEverywhereImStuckRowing);
  else
    Engine_EventSetMessage(MsgTolbiShipShipStartingListIfWe);
  Engine_EventShowMessage(0x13, 0);
  Engine_EventEnd();
}

/// The same for rower 20.
static void TolbiShipInteriors_TalkToRower20(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 2))
    Engine_EventSetMessage(MsgTolbiShipThingHasKajaHisMen);
  else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 2))
    Engine_EventSetMessage(MsgTolbiShipMonstersEverywhereImStuckRowing);
  else
    Engine_EventSetMessage(MsgTolbiShipShipStartingListIfWe);
  Engine_EventShowMessage(0x14, 0);
  Engine_EventEnd();
}

/// Rower 21 asks how many monsters are out there.
static void TolbiShipInteriors_TalkToRower21(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 3))
    Engine_EventSetMessage(MsgTolbiShipHowManyMonstersOutThere);
  else
    Engine_EventSetMessage(MsgTolbiShipAnotherMonsterIsntFirstClass);
  Engine_EventShowMessage(21, 0);
  Engine_EventEnd();
}

/// The same as rower 18, for rower 22.
static void TolbiShipInteriors_TalkToRower22(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 4))
    Engine_EventSetMessage(MsgTolbiShipThingHasKajaHisMen);
  else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 4))
    Engine_EventSetMessage(MsgTolbiShipMonstersEverywhereImStuckRowing);
  else
    Engine_EventSetMessage(MsgTolbiShipShipStartingListIfWe);
  Engine_EventShowMessage(22, 0);
  Engine_EventEnd();
}

/// The same for rower 23.
static void TolbiShipInteriors_TalkToRower23(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 5))
    Engine_EventSetMessage(MsgTolbiShipThingHasKajaHisMen);
  else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 5))
    Engine_EventSetMessage(MsgTolbiShipMonstersEverywhereImStuckRowing);
  else
    Engine_EventSetMessage(MsgTolbiShipShipStartingListIfWe);
  Engine_EventShowMessage(23, 0);
  Engine_EventEnd();
}

/// The same for rower 24.
static void TolbiShipInteriors_TalkToRower24(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 6))
    Engine_EventSetMessage(MsgTolbiShipThingHasKajaHisMen);
  else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 6))
    Engine_EventSetMessage(MsgTolbiShipMonstersEverywhereImStuckRowing);
  else
    Engine_EventSetMessage(MsgTolbiShipShipStartingListIfWe);
  Engine_EventShowMessage(24, 0);
  Engine_EventEnd();
}

/// As rower 21, for rower 25.
static void TolbiShipInteriors_TalkToRower25(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 7))
    Engine_EventSetMessage(MsgTolbiShipHowManyMonstersOutThere);
  else
    Engine_EventSetMessage(MsgTolbiShipAnotherMonsterIsntFirstClass);
  Engine_EventShowMessage(25, 0);
  Engine_EventEnd();
}

/// Mind Read on a rower, by the round they were chosen in.
/// "Just hurry up and slay that beast!"
static void TolbiShipInteriors_ReadRower18Mind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS))
    Engine_EventSetMessage(MsgTolbiShipDontCareTakesJustHurry);
  else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS))
    Engine_EventSetMessage(MsgTolbiShipIfThoseMonstersComeBack);
  else
    Engine_EventSetMessage(MsgTolbiShipBoatsRockingMuchImCertain);
  Engine_EventShowMessage(18, 0);
  Engine_EventEnd();
}

/// The same for rower 19.
static void TolbiShipInteriors_ReadRower19Mind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 1))
    Engine_EventSetMessage(MsgTolbiShipDontCareTakesJustHurry);
  else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 1))
    Engine_EventSetMessage(MsgTolbiShipIfThoseMonstersComeBack);
  else
    Engine_EventSetMessage(MsgTolbiShipBoatsRockingMuchImCertain);
  Engine_EventShowMessage(19, 0);
  Engine_EventEnd();
}

/// The same for rower 20.
static void TolbiShipInteriors_ReadRower20Mind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 2))
    Engine_EventSetMessage(MsgTolbiShipDontCareTakesJustHurry);
  else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 2))
    Engine_EventSetMessage(MsgTolbiShipIfThoseMonstersComeBack);
  else
    Engine_EventSetMessage(MsgTolbiShipBoatsRockingMuchImCertain);
  Engine_EventShowMessage(20, 0);
  Engine_EventEnd();
}

/// Mind Read on rower 21: there are so many monsters at sea.
static void TolbiShipInteriors_ReadRower21Mind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 3))
    Engine_EventSetMessage(MsgTolbiShipHadIdeaThereWereMany);
  else
    Engine_EventSetMessage(MsgTolbiShipWereSurroundedByMonstersStill);
  Engine_EventShowMessage(21, 0);
  Engine_EventEnd();
}

/// As rower 18, for rower 22.
static void TolbiShipInteriors_ReadRower22Mind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 4))
    Engine_EventSetMessage(MsgTolbiShipDontCareTakesJustHurry);
  else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 4))
    Engine_EventSetMessage(MsgTolbiShipIfThoseMonstersComeBack);
  else
    Engine_EventSetMessage(MsgTolbiShipBoatsRockingMuchImCertain);
  Engine_EventShowMessage(22, 0);
  Engine_EventEnd();
}

/// The same for rower 23.
static void TolbiShipInteriors_ReadRower23Mind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 5))
    Engine_EventSetMessage(MsgTolbiShipDontCareTakesJustHurry);
  else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 5))
    Engine_EventSetMessage(MsgTolbiShipIfThoseMonstersComeBack);
  else
    Engine_EventSetMessage(MsgTolbiShipBoatsRockingMuchImCertain);
  Engine_EventShowMessage(23, 0);
  Engine_EventEnd();
}

/// The same for rower 24.
static void TolbiShipInteriors_ReadRower24Mind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 6))
    Engine_EventSetMessage(MsgTolbiShipDontCareTakesJustHurry);
  else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 6))
    Engine_EventSetMessage(MsgTolbiShipIfThoseMonstersComeBack);
  else
    Engine_EventSetMessage(MsgTolbiShipBoatsRockingMuchImCertain);
  Engine_EventShowMessage(24, 0);
  Engine_EventEnd();
}

/// As rower 21, for rower 25.
static void TolbiShipInteriors_ReadRower25Mind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 7))
    Engine_EventSetMessage(MsgTolbiShipHadIdeaThereWereMany);
  else
    Engine_EventSetMessage(MsgTolbiShipWereSurroundedByMonstersStill);
  Engine_EventShowMessage(25, 0);
  Engine_EventEnd();
}

/// Asks a passenger to row. On yes they walk to the leader and leave with them,
/// and the seat's flag for this passenger (seatFlag) and
/// FLAG_TOLBI_SHIP_ROWER_CHOSEN are set; on no the next message refuses.
static void TolbiShipInteriors_AskToRow(s32 speaker, s32 question,
                                        s32 seatFlag) {
  struct FieldActor *leader;

  Engine_EventBegin();
  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(speaker, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    TolbiShipInteriors_Say(speaker);
    Engine_ActorSetAnimation(speaker, 2);
    leader = Object_GetById(0);
    if (leader != 0) {
      Engine_ActorSetDestination(speaker, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(speaker);
    Engine_ActorSetPosition(speaker, 0, 0);
    Engine_GameFlagSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
    Engine_GameFlagSet(seatFlag);
  } else {
    gEventWork->message++;
    TolbiShipInteriors_Say(speaker);
  }
  Engine_EventEnd();
}

/// The travelling priest. From in front of him (the leader facing roughly east)
/// he opens his sanctum; otherwise he talks by the voyage's stage. Under sail,
/// until the ship is docked, he is actor 17, else actor 15.
static void TolbiShipInteriors_TalkToPriest(void) {
  struct FieldActor *leader;

  leader = Object_GetById(0);
  if ((u16)(leader->facing - 0x2000) > 0xc000) {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0 &&
        Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
      Engine_SanctumOpen(17);
    } else {
      Engine_SanctumOpen(15);
    }
  } else {
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      Engine_EventSetMessage(MsgTolbiShipGoodShipHasArrivedSafely);
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      Engine_EventSetMessage(MsgTolbiShipHateArguing);
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) !=
               0) {
      Engine_EventSetMessage(MsgTolbiShipShipsCrewReadyAnything);
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) !=
               0) {
      Engine_EventSetMessage(MsgTolbiShipShipsCrewReadyForAnything);
    } else {
      Engine_EventSetMessage(MsgTolbiShipImSpreadingGoodwillWhereverTravel);
    }
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0 &&
        Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
      Engine_EventShowMessage(17, 0);
    } else {
      Engine_EventShowMessage(15, 0);
    }
    Engine_EventEnd();
  }
}

/// Talking to one of the monsters in the hold (actors 36 to 38) starts the
/// battle with them: group 62, its variant and the entrance the party comes
/// back to chosen by how far the voyage has gone.
static void TolbiShipInteriors_FightMonstersInHold(void) {

  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Battle_SetStart(3);
  GameFlag_Clear(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) == 0) {
    Event_SetNextDestination((s32)&SceneId_TolbiShipInteriors, 16);
    Field_RequestBattleFromEncounterTable(62, 0);
  } else {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED) == 0) {
      Event_SetNextDestination((s32)&SceneId_TolbiShipInteriors, 18);
      Field_RequestBattleFromEncounterTable(62, 1);
    } else {
      if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED) == 0) {
        Event_SetNextDestination((s32)&SceneId_TolbiShipInteriors, 20);
        Field_RequestBattleFromEncounterTable(62, 2);
      }
    }
  }
  Engine_EventEnd();
}

/// While the monsters are aboard, the leader is walked back from the stairs.
/// "Where are you going, Isaac? Slay the beasts and save my mates!"
static void TolbiShipInteriors_TurnBackFromStairs(void) {
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_MONSTERS_ABOARD) != 0) {
    Engine_EventBegin();
    Ui_SetMessageVoiceFromActor(8);
    UiText_ShowCenteredMessage(MsgTolbiShipWhereGoingSlay, 1, 8);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 408, 134);
    TolbiShipInteriors_Turn(0, 0x4000);
    Engine_EventEnd();
  }
}

/// Once the warriors have gone to the captain, the three crewmen
/// wonder what could have happened to the replacement ship.
static void TolbiShipInteriors_WatchCrewWonderAtDelay(void) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0)
    return;

  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Engine_CameraSetSpeed(0x19999, 0x3333);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(448), -1, PIXELS(638),
                                       0x10000028u);
  Engine_EventSetMessage(MsgTolbiShipWonderCouldHaveHappened);

  TolbiShipInteriors_Say(8);
  TolbiShipInteriors_Say(10);
  TolbiShipInteriors_Turn(8, 0x3000);
  TolbiShipInteriors_Say(8);
  TolbiShipInteriors_Turn(10, 0xd000);
  TolbiShipInteriors_Say(10);
  TolbiShipInteriors_Turn(9, 0x5000);
  TolbiShipInteriors_Say(9);

  Engine_ActorFaceDirection(8, FACING_EAST, 20);
  TolbiShipInteriors_Say(8);
  TolbiShipInteriors_Turn(9, 0x8000);
  TolbiShipInteriors_Say(9);
  TolbiShipInteriors_Say(10);
  TolbiShipInteriors_Say(8);
  TolbiShipInteriors_Turn(10, 0xb000);
  TolbiShipInteriors_Say(8);

  Engine_GameFlagSet(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY);
  Engine_EventEnd();
}

/// Once the party has heard of the delay at Kalay: Ouranos threatens to go
/// ashore if the ship doesn't sail. Actor 12 can't say when it will, so Sean
/// and Ouranos set off to make the captain set sail, and actor 12 ("I don't
/// want any violence!") hurries after them past the party.
static void TolbiShipInteriors_WatchWarriorsStormOff(void) {
  s32 actor12Speaker;
  s32 seanSpeaker;
  const s32 *action;

  if (GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) == 0) {
    // Not yet: the party has not heard of the delay.
  } else {
    Engine_EventBegin();
    Battle_ResetEffectCounter();
    Camera_SetSpeed(0x26666, 0x4ccc);
    TolbiShipInteriors_MoveCameraAndWait(PIXELS(1463), -1, PIXELS(464),
                                         0x10000014);
    Engine_ActorRunRepeatedMotion(HOLD_OURANOS, 1);
    Engine_EventSetMessage(MsgTolbiShipIfWeArentGoingSet);
    TolbiShipInteriors_Say(SPEAKER(HOLD_OURANOS, 2));
    TolbiShipInteriors_Turn(12, 0xd000);
    actor12Speaker = SPEAKER(12, 8);
    Actor_ShowEmote(12, 0x102, 20);
    Engine_ActorStartRepeatedMotion(12, 2);
    TolbiShipInteriors_Say(actor12Speaker);
    Engine_ActorRunRepeatedMotion(HOLD_SEAN, 1);
    Event_ShowMessageAndWait(SPEAKER(HOLD_SEAN, 10), 0, 20);
    TolbiShipInteriors_Turn(12, 0);
    seanSpeaker = SPEAKER(HOLD_SEAN, 10);
    Actor_ShowEmote(12, 0x101, 40);
    Actor_ShowEmote(HOLD_SEAN, 0x103, 40);
    Engine_ActorStartRepeatedMotion(HOLD_SEAN, 3);
    TolbiShipInteriors_Say(seanSpeaker);
    Actor_SetAttachedEffect(12, 0x102);
    Engine_EventWait(40);
    Engine_ActorStartRepeatedMotion(12, 3);
    TolbiShipInteriors_Say(actor12Speaker);
    Engine_ActorRunRepeatedMotion(HOLD_SEAN, 1);
    TolbiShipInteriors_Say(seanSpeaker);
    TolbiShipInteriors_Turn(HOLD_SEAN, 0xb000);
    TolbiShipInteriors_Say(seanSpeaker);
    TolbiShipInteriors_Turn(12, 0xd000);
    Actor_ShowEmote(12, 0x100, 30);
    Engine_ActorStartRepeatedMotion(12, 1);
    TolbiShipInteriors_Say(actor12Speaker);
    Engine_ActorSetAnimationAndWait(HOLD_OURANOS, 4);
    TolbiShipInteriors_Say(SPEAKER(HOLD_OURANOS, 2));
    Engine_ActorStartRepeatedMotion(HOLD_OURANOS, 2);
    TolbiShipInteriors_Say(SPEAKER(HOLD_OURANOS, 2));
    Engine_ActorSetAnimation(12, 4);
    TolbiShipInteriors_Say(actor12Speaker);
    Engine_ActorSetAnimationAndWait(HOLD_SEAN, 4);
    TolbiShipInteriors_Say(seanSpeaker);
    TolbiShipInteriors_Turn(HOLD_SEAN, 0x8000);
    Engine_ActorStartRepeatedMotion(HOLD_SEAN, 2);
    Event_ShowMessageAndWait(seanSpeaker, 0, 20);
    Actor_FaceDirection(12, FACING_EAST, 0);
    Actor_ShowEmote(12, 0x102, 80);
    Event_ShowMessageAndWait(actor12Speaker, 0, 20);
    Actor_ShowEmote(HOLD_SEAN, 0x103, 0);
    Actor_ShowEmote(HOLD_OURANOS, 0x103, 60);
    Engine_ActorStartRepeatedMotion(HOLD_SEAN, 2);
    TolbiShipInteriors_Say(seanSpeaker);
    TolbiShipInteriors_Turn(HOLD_SEAN, 0xb000);
    Engine_ActorRunRepeatedMotion(HOLD_SEAN, 1);
    TolbiShipInteriors_Say(seanSpeaker);
    TolbiShipInteriors_Turn(HOLD_OURANOS, 0x3000);
    Actor_ShowEmote(HOLD_OURANOS, 0x101, 0);
    Actor_ShowEmote(12, 0x101, 60);
    Engine_ActorRunRepeatedMotion(HOLD_OURANOS, 1);
    TolbiShipInteriors_Say(HOLD_OURANOS);
    Actor_ShowEmote(HOLD_SEAN, 0x103, 40);
    Engine_ActorRunRepeatedMotion(HOLD_SEAN, 1);
    TolbiShipInteriors_Say(seanSpeaker);
    Actor_FaceDirection(12, 0xd000, 0);
    Actor_FaceDirection(HOLD_OURANOS, 0x5000, 40);
    Actor_FaceDirection(12, FACING_EAST, 0);
    TolbiShipInteriors_Turn(HOLD_OURANOS, 0x3000);
    Engine_ActorRunRepeatedMotion(12, 2);
    Event_ShowMessageAndWait(actor12Speaker, 0, 20);
    Actor_FaceDirection(HOLD_SEAN, FACING_SOUTH, 40);
    TolbiShipInteriors_Say(seanSpeaker);
    Engine_ActorStartRepeatedMotion(12, 2);
    Engine_ActorRunRepeatedMotion(HOLD_OURANOS, 2);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(HOLD_OURANOS, 1);
    TolbiShipInteriors_Say(HOLD_OURANOS);
    Engine_ActorSetAnimationAndWait(HOLD_SEAN, 3);
    TolbiShipInteriors_Say(seanSpeaker);
    Actor_ShowEmote(12, 0x102, 40);
    Engine_ActorRunRepeatedMotion(12, 2);
    TolbiShipInteriors_Say(actor12Speaker);
    Engine_ActorSetAnimationAndWait(HOLD_OURANOS, 3);
    TolbiShipInteriors_Say(HOLD_OURANOS);
    Actor_FaceDirection(HOLD_SEAN, 0xb000, 40);
    Engine_ActorSetAnimation(HOLD_SEAN, 3);
    Engine_ActorSetAnimationAndWait(HOLD_OURANOS, 3);
    Actor_SetSpeed(HOLD_SEAN, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_SetSpeed(HOLD_OURANOS, PIXELS(8) / 5, PIXELS(4) / 5);
    action = gTolbiShipInteriors_LeaveHold;
    Actor_SetActionScript(HOLD_SEAN, action);
    Actor_SetActionScript(HOLD_OURANOS, action);
    Engine_EventWait(20);
    Actor_FaceDirection(12, FACING_SOUTH, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(12) / 5, PIXELS(6) / 5);
    Object_GetById(0)->steeringFlags &= 254;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 184, 520);
    Engine_EventWait(1);
    {
      struct FieldActor *actor = Object_GetById(0);
      actor->steeringFlags = ACTOR_FACES_MOTION | actor->steeringFlags;
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Engine_ActorJump(12, 4, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
    Engine_ActorStartRepeatedMotion(12, 2);
    TolbiShipInteriors_Say(12);
    Actor_SetSpeed(12, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_SetActionScript(12, action);
    Engine_EventWait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Object_RefreshSelectorById(12);
    GameFlag_Set(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN);
    Engine_EventEnd();
  }
}

/// The passengers pressure the captain to launch despite the missing anchor.
static void TolbiShipInteriors_DiscussLaunchingWithoutAnchor(void) {
  s32 actor12Speech;
  s32 actor11Speech;
  s32 actor8Speech;

  // Gather the cabin's attention around the argument about launching.
  Event_PlayCue(MUS_028);
  Camera_SetSpeed(0x26666, 0x4ccc);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(456), -1, PIXELS(648),
                                       0x10000014);
  Engine_ActorRunRepeatedMotion(9, 1);
  Engine_EventSetMessage(MsgTolbiShipYoureTryingLaunchShip);
  TolbiShipInteriors_Say(9);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
  Actor_FaceDirection(10, 0xd000, 0);
  Actor_FaceDirection(11, FACING_EAST, 0);
  Actor_FaceDirection(12, 0x3000, 0);
  Actor_FaceDirection(13, FACING_WEST, 40);
  Actor_ShowEmote(9, 0x103, 40);
  Engine_ActorStartRepeatedMotion(9, 2);
  TolbiShipInteriors_Say(9);
  Actor_FaceDirection(12, FACING_EAST, 0);
  Actor_FaceDirection(11, 0xd000, 0);
  Actor_FaceDirection(13, 0xd000, 20);
  Engine_ActorRunRepeatedMotion(11, 1);
  TolbiShipInteriors_Say(0x100b);
  Actor_ShowEmote(13, 0x102, 20);
  Engine_ActorStartRepeatedMotion(13, 2);
  TolbiShipInteriors_Say(13);
  Actor_ShowEmote(9, 0x105, 60);
  TolbiShipInteriors_Say(9);
  Actor_ShowEmote(12, 0x104, 20);
  TolbiShipInteriors_Say(0x900c);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_ActorSetAnimation(8, 3);
  TolbiShipInteriors_Say(8);
  TolbiShipInteriors_Turn(12, 0x3000);
  TolbiShipInteriors_Say(0x900c);
  TolbiShipInteriors_Turn(11, 0xb000);
  Engine_ActorSetAnimationAndWait(11, 3);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(13, 1);
  Engine_ActorSetAnimationAndWait(13, 3);
  TolbiShipInteriors_Say(13);
  Actor_FaceDirection(13, FACING_WEST, 0);
  Actor_FaceDirection(12, 0x5000, 0);
  TolbiShipInteriors_Turn(11, 0x5000);
  // Two passengers step aside as the conversation continues.
  Actor_SetSpeed(13, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(12, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(12, 444, 668);
  Actor_WalkToAndWait(13, 472, 668);
  Engine_ActorWaitForMove(12);
  Engine_ActorSetAnimation(12, 1);
  Engine_EventWait(80);
  TolbiShipInteriors_Turn(12, 0xd000);
  Actor_ShowEmote(12, 0x101, 60);
  Engine_ActorRunRepeatedMotion(11, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 11, 0, 40);
  Engine_ActorRunRepeatedMotion(11, 2);
  Actor_FaceDirection(11, 0xd000, 0);
  TolbiShipInteriors_Say(0x100b);
  Actor_FaceDirection(12, 0xd000, 0);
  Actor_ShowEmote(9, 0x101, 60);
  Engine_ActorSetAnimation(11, 4);
  Engine_EventWait(20);
  TolbiShipInteriors_Say(0x100b);
  Engine_ActorSetAnimation(9, 3);
  TolbiShipInteriors_Say(9);
  Actor_FaceDirection(13, 0xd000, 0);
  Actor_SetAttachedEffect(13, 0x102);
  Engine_ActorJump(13, 2, 20);
  TolbiShipInteriors_Say(13);
  Engine_ActorSetAnimationAndWait(9, 3);
  actor12Speech = 0x100c;
  TolbiShipInteriors_Say(9);
  TolbiShipInteriors_Turn(11, 0xd000);
  Engine_ActorRunRepeatedMotion(12, 1);
  TolbiShipInteriors_Say(actor12Speech);
  Actor_ShowEmote(8, 0x105, 40);
  Engine_ActorSetAnimation(8, 3);
  TolbiShipInteriors_Say(8);
  Actor_ShowEmote(13, 0x102, 40);
  Engine_ActorJump(13, 4, 0);
  TolbiShipInteriors_Say(13);
  Engine_ActorSetAnimation(9, 3);
  TolbiShipInteriors_Say(9);
  Engine_ActorRunRepeatedMotion(11, 1);
  TolbiShipInteriors_Say(0x100b);
  Actor_ShowEmote(8, 0x102, 40);
  Event_ShowMessage(8, 0);
  Engine_ActorStartRepeatedMotion(11, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 11, 0, 40);
  Actor_ShowEmote(9, 0x100, 0);
  Actor_FaceDirection(9, 0x5000, 20);
  Engine_ActorStartRepeatedMotion(9, 2);
  Event_ShowMessageAndWait(9, 0, 20);
  Engine_ActorSetAnimation(11, 3);
  Engine_EventWait(20);
  Actor_ShowEmote(12, 0x100, 40);
  Engine_ActorStartRepeatedMotion(12, 2);
  actor11Speech = 0x400b;
  TolbiShipInteriors_Say(actor12Speech);
  Actor_FaceDirection(11, 0x5000, 20);
  TolbiShipInteriors_Say(actor11Speech);
  Engine_ActorStartRepeatedMotion(12, 2);
  TolbiShipInteriors_Say(actor12Speech);
  Engine_ActorSetAnimationAndWait(11, 3);
  Engine_ActorRunRepeatedMotion(11, 1);
  TolbiShipInteriors_Say(actor11Speech);
  Actor_ShowEmote(12, 0x102, 60);
  Engine_ActorRunRepeatedMotion(9, 1);
  TolbiShipInteriors_Say(9);
  Actor_ShowEmote(11, 0x101, 40);
  Actor_FaceDirection(11, 0xd000, 20);
  Engine_ActorSetAnimation(9, 3);
  TolbiShipInteriors_Say(9);
  Actor_ShowEmote(11, 0x103, 20);
  Engine_ActorStartRepeatedMotion(11, 2);
  TolbiShipInteriors_Say(0x100b);
  Actor_ShowEmote(9, 0x108, 40);
  TolbiShipInteriors_Say(9);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_ActorSetAnimation(8, 3);
  TolbiShipInteriors_Say(8);
  Actor_FaceDirection(9, 0xd000, 40);
  TolbiShipInteriors_Say(9);
  Engine_ActorRunRepeatedMotion(12, 1);
  Engine_EventWait(20);
  TolbiShipInteriors_Say(actor12Speech);
  Actor_FaceDirection(11, 0x5000, 0);
  Actor_FaceDirection(9, 0x5000, 0);
  Actor_FaceDirection(13, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(10, 0xb000, 40);
  Engine_ActorRunRepeatedMotion(11, 1);
  TolbiShipInteriors_Say(actor11Speech);
  Engine_ActorSetAnimation(12, 3);
  Event_ShowMessageAndWait(actor12Speech, 0, 20);
  Engine_ActorRunRepeatedMotion(9, 2);
  TolbiShipInteriors_Say(9);
  Actor_ShowEmote(12, 0x108, 40);
  Engine_ActorSetAnimation(12, 3);
  TolbiShipInteriors_Say(actor12Speech);
  Engine_ActorSetAnimationAndWait(8, 3);
  TolbiShipInteriors_Say(8);
  Actor_FaceDirection(8, FACING_WEST, 20);
  Event_PlayCue(MUS_019);
  Engine_ActorStartRepeatedMotion(8, 2);
  actor8Speech = 0x8008;
  Actor_ShowEmote(8, 0x100, 80);
  TolbiShipInteriors_Say(actor8Speech);
  Actor_ShowEmote(12, 0x101, 0);
  Actor_ShowEmote(11, 0x101, 0);
  Actor_ShowEmote(13, 0x101, 0);
  Actor_ShowEmote(10, 0x101, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 40);
  Actor_FaceDirection(12, 0xd000, 0);
  Actor_FaceDirection(11, 0xd000, 0);
  Actor_FaceDirection(13, 0xb000, 0);
  Actor_FaceDirection(10, 0xb000, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Actor_SetAttachedEffect(8, 0x102);
  Engine_ActorJump(8, 4, 40);
  Engine_ActorStartRepeatedMotion(8, 2);
  Event_ShowMessage(actor8Speech, 0);
  Actor_SetSpeed(8, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(8, 475, 598);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Actor_SetSpeed(9, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(9, 462, 618);
  TolbiShipInteriors_Turn(9, 0xb000);
  Actor_ShowEmote(9, 0x100, 40);
  Engine_ActorStartRepeatedMotion(9, 2);
  TolbiShipInteriors_Say(0x8009);
  Actor_ShowEmote(11, 0x101, 60);
  TolbiShipInteriors_Say(11);
  Actor_ShowEmote(12, 0x102, 20);
  TolbiShipInteriors_Say(actor12Speech);
  Actor_ShowEmote(8, 0x103, 20);
  Engine_ActorJump(8, 4, 0);
  Actor_FaceDirection(8, 0x5000, 20);
  TolbiShipInteriors_Say(8);
  Event_PlayCue(MUS_028);
  Engine_ActorStartRepeatedMotion(8, 3);
  TolbiShipInteriors_Say(8);
  Actor_ShowEmote(13, 0x101, 60);
  TolbiShipInteriors_Say(13);
  TolbiShipInteriors_Turn(8, 0x3000);
  Engine_ActorSetAnimationAndWait(8, 4);
  TolbiShipInteriors_Say(8);
  // Settle the passengers into their positions for the next cabin visit.
  Actor_WalkToAndWait(12, 444, 628);
  TolbiShipInteriors_Turn(12, 0xd000);
  TolbiShipInteriors_Say(0x900c);
  Actor_FaceDirection(8, 0x5000, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  TolbiShipInteriors_Say(8);
  Actor_ShowEmote(11, 0x102, 60);
  TolbiShipInteriors_Say(0x100b);
  Actor_ShowEmote(13, 0x107, 40);
  Engine_ActorStartRepeatedMotion(13, 2);
  TolbiShipInteriors_Say(13);
  TolbiShipInteriors_Turn(9, 0x3000);
  Engine_ActorSetAnimationAndWait(9, 4);
  TolbiShipInteriors_Say(0x1009);
  TolbiShipInteriors_Turn(12, 0);
  Engine_ActorRunRepeatedMotion(8, 1);
  TolbiShipInteriors_Say(8);
  Actor_FaceDirection(11, FACING_EAST, 0);
  Actor_ShowEmote(12, 0x105, 0);
  Actor_ShowEmote(9, 0x105, 60);
  Camera_SetSpeed(0x13333, 0x2666);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(464), -1, PIXELS(680),
                                       0x10000000);
  Engine_ActorRunRepeatedMotion(10, 1);
  TolbiShipInteriors_Turn(10, 0);
  TolbiShipInteriors_Say(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_ShowEmote(10, 0x102, 40);
  TolbiShipInteriors_Say(10);
  Actor_FaceDirection(10, FACING_WEST, 20);
  Actor_ShowEmote(10, 0x100, 0);
  Engine_ActorJump(10, 4, 40);
  TolbiShipInteriors_Say(10);
  Engine_ActorRunRepeatedMotion(10, 1);
  TolbiShipInteriors_Say(10);
  Actor_SetSpeed(13, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(13, 438, 659);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Actor_FaceDirection(9, 0xb000, 0);
  Actor_FaceDirection(12, 0x3000, 0);
  Actor_FaceDirection(11, 0xb000, 0);
  Event_PlayCue(MUS_017);
  Actor_SetSpeed(10, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(10, 488, 686);
  Actor_FaceDirection(10, 0xb000, 0);
  Engine_ActorWaitForMove(13);
  Engine_ActorSetAnimation(13, 1);
  Actor_FaceDirection(13, 0xd000, 0);
  Audio_PlayCueFromEventWork();
  GameFlag_Set(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL);
}

/// Returns the captain's lucky anchor and agrees to protect the ship.
static void TolbiShipInteriors_ReturnLuckyAnchor(void) {
  struct FieldActor *actor;
  s32 requestA;
  s32 requestB;
  s32 value;
  s32 requestC;
  const s32 *action;

  // Let the captain explain why the anchor matters and ask the party to help.
  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
  Actor_ShowEmote(8, 0x100, 40);
  requestA = 0x1008;
  Engine_ActorStartRepeatedMotion(8, 3);
  Engine_EventSetMessage(MsgTolbiShipItsMyLuckyAnchor);
  TolbiShipInteriors_Say(requestA);
  Engine_ActorStartRepeatedMotion(9, 1);
  Engine_ActorStartRepeatedMotion(12, 1);
  Engine_ActorStartRepeatedMotion(11, 1);
  Engine_ActorStartRepeatedMotion(13, 1);
  Engine_ActorRunRepeatedMotion(10, 1);
  Actor_FaceDirection(9, 0xd000, 0);
  Actor_FaceDirection(12, 0xd000, 0);
  Actor_FaceDirection(11, 0xd000, 0);
  Actor_FaceDirection(13, 0xd000, 0);
  Actor_FaceDirection(10, 0xb000, 20);
  Engine_ActorRunRepeatedMotion(8, 1);
  Event_OpenMessage(requestA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorRunRepeatedMotion(9, 2);
    TolbiShipInteriors_Say(0x9009);
    Actor_ShowEmote(8, 0x108, 40);
    TolbiShipInteriors_Say(requestA);
    gEventWork->message += 2;
  } else {
    gEventWork->message += 2;
    Engine_ActorRunRepeatedMotion(9, 1);
    TolbiShipInteriors_Say(0x9009);
    Engine_ActorStartRepeatedMotion(8, 2);
    TolbiShipInteriors_Say(0x9008);
  }
  Actor_ShowEmote(13, 0x105, 40);
  Camera_SetSpeed(0xcccc, 0x1999);
  Camera_MoveTo(PIXELS(472), -1, PIXELS(636), 1);
  Actor_SetSpeed(13, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(13, 472, 662);
  TolbiShipInteriors_Turn(13, 0xb000);
  TolbiShipInteriors_Say(13);
  TolbiShipInteriors_Turn(8, 0x5000);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_ActorSetAnimationAndWait(9, 3);
  Actor_FaceDirection(11, FACING_EAST, 0);
  Actor_FaceDirection(13, FACING_WEST, 20);
  Engine_ActorSetAnimation(11, 3);
  Engine_ActorSetAnimationAndWait(13, 3);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(12, 1);
  TolbiShipInteriors_Turn(12, 0x3000);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 12, 0, 20);
  Actor_FaceDirection(11, 0xb000, 20);
  Actor_ShowEmote(11, 0x101, 40);
  TolbiShipInteriors_Say(11);
  requestB = 0x900c;
  TolbiShipInteriors_Turn(12, 0xd000);
  Engine_ActorSetAnimation(12, 4);
  TolbiShipInteriors_Say(requestB);
  TolbiShipInteriors_Turn(13, 0xb000);
  Engine_ActorRunRepeatedMotion(13, 1);
  TolbiShipInteriors_Say(13);
  Actor_ShowEmote(9, 0x100, 20);
  TolbiShipInteriors_Turn(9, 0x3000);
  Engine_ActorRunRepeatedMotion(9, 1);
  TolbiShipInteriors_Say(9);
  Engine_ActorSetAnimationAndWait(12, 3);
  TolbiShipInteriors_Say(requestB);
  Engine_ActorRunRepeatedMotion(8, 2);
  TolbiShipInteriors_Say(8);
  TolbiShipInteriors_Turn(12, 0xd000);
  Engine_ActorSetAnimationAndWait(12, 3);
  TolbiShipInteriors_Say(requestB);
  Engine_ActorRunRepeatedMotion(11, 2);
  TolbiShipInteriors_Turn(11, 0xb000);
  TolbiShipInteriors_Say(11);
  TolbiShipInteriors_Turn(12, 0);
  TolbiShipInteriors_Say(requestB);
  Actor_FaceDirection(8, 0x3000, 0);
  Actor_FaceDirection(9, FACING_EAST, 0);
  Actor_FaceDirection(11, 0xd000, 0);
  Actor_FaceDirection(13, 0xd000, 0);
  // Bring the rest of the party out beside Isaac for the agreement.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 486, 608);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  actor = Object_GetById(0);
  if (actor != 0) {
    Actor_SetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(GARET, 486, 624);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  actor = Object_GetById(1);
  if (actor != 0) {
    Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(IVAN, 486, 640);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  actor = Object_GetById(2);
  if (actor != 0) {
    Actor_SetPosition(MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(MIA, 486, 656);
  Actor_FaceDirection(MIA, FACING_WEST, 20);
  Actor_ShowEmote(12, 0x108, 40);
  TolbiShipInteriors_Say(requestB);
  Engine_ActorRunRepeatedMotion(9, 1);
  TolbiShipInteriors_Say(0x1009);
  Engine_ActorSetAnimationAndWait(8, 3);
  TolbiShipInteriors_Turn(8, 0x5000);
  TolbiShipInteriors_Say(8);
  TolbiShipInteriors_Turn(8, 0x3000);
  Event_OpenMessage(8, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    Engine_ActorStartRepeatedMotion(8, 2);
    TolbiShipInteriors_Say(8);
    Engine_ActorSetAnimationAndWait(12, 3);
    TolbiShipInteriors_Say(requestB);
    Engine_ActorStartRepeatedMotion(9, 1);
    Event_ShowMessageAndWait(0x9009, 0, 40);
    gEventWork->message += 1;
  } else {
    gEventWork->message += 3;
    Engine_ActorStartRepeatedMotion(8, 3);
    Event_ShowMessageAndWait(8, 0, 40);
  }
  Engine_ActorRunRepeatedMotion(13, 1);
  TolbiShipInteriors_Say(13);
  Engine_ActorRunRepeatedMotion(8, 1);
  value = 176;
  TolbiShipInteriors_Turn(8, 0x5000);
  TolbiShipInteriors_Say(8);
  Engine_ActorRunRepeatedMotion(13, 1);
  TolbiShipInteriors_Turn(13, (value << 8));
  Event_ShowMessageAndWait(13, 0, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Actor_SetSpeed(8, PIXELS(4) / 5, PIXELS(2) / 5);
  requestC = 0x4008;
  Actor_WalkToAndWait(8, 472, 632);
  TolbiShipInteriors_Say(requestC);
  Actor_ShowEmote(13, 0x103, 40);
  Engine_ActorStartRepeatedMotion(13, 2);
  TolbiShipInteriors_Say(13);
  Engine_ActorSetAnimation(8, 4);
  Event_ShowMessageAndWait(requestC, 0, 40);
  Engine_ActorRunRepeatedMotion(11, 1);
  TolbiShipInteriors_Turn(11, (value << 8));
  TolbiShipInteriors_Say(0x100b);
  Actor_ShowEmote(10, 0x102, 20);
  Actor_SetSpeed(10, PIXELS(12) / 5, PIXELS(6) / 5);
  Engine_ActorJump(10, 2, 0);
  Actor_WalkToAndWait(10, 462, 674);
  TolbiShipInteriors_Turn(10, (value << 8));
  Engine_ActorStartRepeatedMotion(10, 2);
  TolbiShipInteriors_Say(10);
  TolbiShipInteriors_Turn(9, 0x5000);
  Engine_ActorSetAnimationAndWait(9, 4);
  TolbiShipInteriors_Say(9);
  Engine_ActorSetAnimationAndWait(8, 3);
  TolbiShipInteriors_Say(requestC);
  Actor_ShowEmote(13, 0x102, 40);
  Event_ShowMessageAndWait(13, 0, 40);
  TolbiShipInteriors_Turn(9, 0x3000);
  Engine_ActorStartRepeatedMotion(9, 2);
  TolbiShipInteriors_Say(0x1009);
  TolbiShipInteriors_Turn(12, 0);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Actor_FaceDirection(9, 0x5000, 0);
  Actor_FaceDirection(11, (value << 8), 0);
  Actor_FaceDirection(13, (value << 8), 0);
  Actor_FaceDirection(10, (value << 8), 20);
  Engine_ActorRunRepeatedMotion(12, 1);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 12, 0, 20);
  Actor_ShowEmote(8, 0x101, 40);
  TolbiShipInteriors_Turn(8, 0xd000);
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | 8, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(8, 3);
    TolbiShipInteriors_Say(0x1008);
    gEventWork->message += 1;
  } else {
    gEventWork->message += 1;
    TolbiShipInteriors_Say(0x1008);
  }
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(8, 3);
  TolbiShipInteriors_Say(0x1008);
  TolbiShipInteriors_Turn(8, 0x8000);
  TolbiShipInteriors_Say(0x4008);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLAY_PARTY_ANIMATION, 0, 0);
  Engine_ActorSetAnimation(12, 3);
  Engine_ActorSetAnimation(11, 3);
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorStartRepeatedMotion(10, 2);
  Engine_ActorRunRepeatedMotion(13, 2);
  Engine_EventWait(20);
  // Send the crew away and merge the party's separate scene actors again.
  action = gTolbiShipInteriors_LeaveCabin;
  Actor_SetActionScript(10, action);
  Engine_EventWait(4);
  Actor_SetActionScript(11, action);
  Engine_EventWait(4);
  Actor_SetActionScript(12, action);
  Engine_EventWait(4);
  Actor_SetActionScript(9, action);
  Engine_ActorSetAnimation(MIA, 2);
  actor = Object_GetById(2);
  if (actor != 0) {
    Actor_SetDestination(MIA, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Engine_ActorSetAnimation(IVAN, 2);
  actor = Object_GetById(1);
  if (actor != 0) {
    Actor_SetDestination(IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_ActorSetAnimation(GARET, 2);
  actor = Object_GetById(0);
  if (actor != 0) {
    Actor_SetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetActionScript(13, action);
  Actor_WalkToAndWait(8, 456, 648);
  TolbiShipInteriors_Turn(8, 0);
  PartyInventory_Discard(232);
  GameFlag_Set(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED);
  Engine_EventEnd();
}

/// Restores the room or plays the event selected by this entrance.
s32 TolbiShipInteriors_ApplyEntryState(void) {
  Engine_TaskWait(1);
  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  switch (gGameState.entrance) {
  case 1:
  case 2:
  case 11:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
        Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0) {
      Engine_ActorSetChildValue(9, 2);
    } else if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      Engine_ActorSetChildValue(12, 2);
    }
    break;
  case 4:
  case 12:
  case 15:
  case 16:
  case 17:
  case 18:
  case 19:
  case 20:
  case 21:
  case 23:
  case 24:
    Engine_ActorSetChildValue(19, 2);
    break;
  case 5:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
        Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      Engine_ActorSetChildValue(13, 2);
    }
    break;
  }
  switch (gGameState.entrance) {
  case 1:
  case 2:
    TolbiShipInteriors_SetUpHold();
    break;
  case 4:
    TolbiShipInteriors_SetUpRowingDeck();
    break;
  case 5:
    TolbiShipInteriors_SetUpCabin();
    break;
  case 10:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0) {
      TolbiShipInteriors_AnnounceDeparture();
    } else {
      TolbiShipInteriors_CastOff();
    }
    break;
  case 11:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0) {
      TolbiShipInteriors_CallActorEighteenToRow();
    } else {
      TolbiShipInteriors_CallActorSixteenToRow();
    }
    break;
  case 12:
    TolbiShipInteriors_ShowRowing();
    break;
  case 13:
    TolbiShipInteriors_AskAboutMonsters();
    break;
  case 14:
    TolbiShipInteriors_AnnounceTurn();
    break;
  case 15:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      Engine_EventBegin();
      TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 1,
                                         0);
      TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ACTOR_12_SCRIPT, 0, 0);
      Actor_SetActionScript(36, gTolbiShipInteriors_MonsterBob);
      Actor_SetActionScript(37, gTolbiShipInteriors_MonsterBob);
      Actor_SetActionScript(38, gTolbiShipInteriors_MonsterBob);
      Engine_ActorSetChildValue(36, 3);
      Engine_ActorSetChildValue(37, 3);
      Engine_ActorSetChildValue(38, 3);
      Engine_EventEnd();
    } else {
      TolbiShipInteriors_StartFirstAttack();
    }
    break;
  case 16:
    TolbiShipInteriors_EndFirstAttack();
    break;
  case 17:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      Engine_EventBegin();
      TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 2,
                                         0);
      TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ACTOR_12_SCRIPT, 0, 0);
      Actor_SetActionScript(36, gTolbiShipInteriors_MonsterBob);
      Actor_SetActionScript(37, gTolbiShipInteriors_MonsterBob);
      Engine_EventEnd();
    } else {
      TolbiShipInteriors_StartSecondAttack();
    }
    break;
  case 18:
    TolbiShipInteriors_EndSecondAttack();
    break;
  case 19:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      Engine_EventBegin();
      TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 3,
                                         0);
      TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ACTOR_12_SCRIPT, 0, 0);
      Actor_SetActionScript(36, gTolbiShipInteriors_MonsterBob);
      Actor_SetActionScript(37, gTolbiShipInteriors_MonsterBob);
      Engine_ActorSetChildValue(36, 3);
      Engine_ActorSetChildValue(37, 3);
      Engine_EventEnd();
    } else {
      TolbiShipInteriors_StartThirdAttack();
    }
    break;
  case 20:
    TolbiShipInteriors_EndThirdAttack();
    break;
  case 21:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_ABOARD) != 0) {
        gTolbiShipInteriors_ShakeTimer = 0;
        FieldScheduler_AddCallback(TolbiShipInteriors_ShakeWhileKrakenAboard,
                                   0xc80);
        Engine_ActorSetAnimation(9, 5);
      }
    } else {
      TolbiShipInteriors_NoticeShipRocking();
    }
    break;
  case 22:
    TolbiShipInteriors_ShowCabinBeforeKraken();
    break;
  case 23:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      TolbiShipInteriors_ReachIsland();
    }
    break;
  case 24:
    TolbiShipInteriors_DismissReplacementRowers();
    break;
  case 30:
    TolbiShipInteriors_RunSceneCommand(
        SHIP_CLEAR_FLAG_RANGE, FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT,
        FLAG_TOLBI_SHIP_KRAKEN_DEFEATED);
    TolbiShipInteriors_RunSceneCommand(SHIP_CLEAR_ROWING_SELECTIONS, 0, 0);
    Engine_GameFlagSet(0x902);
    Engine_EventRequestExit(1);
    break;
  }
  return 0;
}

/// Restores the cabin's actors after the anchor and departure conversations.
static void TolbiShipInteriors_SetUpCabin(void) {
  struct FieldActor *actor;
  struct FieldActor *other;
  s32 done;
  s32 x;
  s32 heading;
  u16 zero;

  if (!Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT)) {
    TolbiShipInteriors_PlaceAnchorCharm();
    return;
  }
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED)) {
    TolbiShipInteriors_PlaceAnchorCharm();
  }
  done = Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI);
  if (done != 0) {
    return;
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND)) {
    actor = Object_GetById(9);
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_CABIN_CREW, 0, 0);
    TolbiShipInteriors_PlaceActor(8, 456, 652, 0);
    x = 0x1e0;
    TolbiShipInteriors_PlaceActor(9, x, 600, 0xb000);
    Actor_SetSpeed(9, PIXELS(4) / 5, PIXELS(2) / 5);
    actor->work = done;
    zero = 0;
    actor->riseEnabled = zero;
    actor->collisionFlags |= 128;
    actor->update = TolbiShipInteriors_PaceCabin;
    other = Object_GetById(8);
    other->riseCounter = zero;
    other->update = TolbiShipInteriors_WatchPacer;
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      TolbiShipInteriors_PlaceActor(0, x, 666, 0xa000);
    }
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED)) {
    TolbiShipInteriors_PlaceActor(8, 444, 614, 0xd000);
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_CABIN_CREW, 0, 0);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED)) {
    TolbiShipInteriors_PlaceActor(8, 456, 648, 0);
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_CABIN_CREW, 0, 0);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL)) {
    heading = 0xb000;
    TolbiShipInteriors_PlaceActor(8, 475, 598, 0x8000);
    TolbiShipInteriors_PlaceActor(9, 462, 618, heading);
    Object_GetById(12)->facing = 0x3000;
    Object_GetById(11)->facing = heading;
    TolbiShipInteriors_PlaceActor(13, 438, 659, 0xd000);
    TolbiShipInteriors_PlaceActor(10, 488, 688, heading);
  }
}

/// Restores the passenger hold after recruitment and departure events.
static void TolbiShipInteriors_SetUpHold(void) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    Engine_ActorSetPosition(8, 0, 0);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_ACTORS_13_AND_14, 0, 0);
    return;
  }

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    TolbiShipInteriors_PlaceActor(8, 152, 444, 0x3000);
    Actor_SetActionScript(8, gTolbiShipInteriors_LookAroundScript);
    TolbiShipInteriors_PlaceActor(10, 184, 480, 0xb000);
    TolbiShipInteriors_PlaceActor(12, 170, 488, 0xb000);
    TolbiShipInteriors_PlaceActor(13, 136, 488, 0xd000);
    TolbiShipInteriors_PlaceActor(15, 120, 480, 0xd000);
    TolbiShipInteriors_PlaceActor(14, 184, 526, 0xb000);
    TolbiShipInteriors_PlaceActor(11, 136, 584, 0x8000);
    Actor_SetActionScript(11, gTolbiShipInteriors_Shiver);
    return;
  }

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0) {
    TolbiShipInteriors_HideSelectedPassengers();
    return;
  }

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_PASSENGER_HOLD, 0, 0);
    return;
  }

  if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0 &&
      Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_ACTORS_13_AND_14, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
  }
}

/// Restores the rowing deck and introduces a newly recruited replacement.
static void TolbiShipInteriors_SetUpRowingDeck(void) {
  Engine_TaskWait(1);
  TolbiShipInteriors_SetSeatActorCollision();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_PASSENGERS, 4, 0);
    TolbiShipInteriors_PlaceActor(8, 412, 222, 12288);
    TolbiShipInteriors_PlaceActor(9, 458, 161, 32768);
  } else {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
      Actor_SetPosition(8, PIXELS(472), PIXELS(152));
      Engine_ActorSetAnimation(9, 5);
      TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_PASSENGERS, 4, 0);
    } else {
      if (GameFlag_IsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
        TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_RECRUITMENT_DECK, 0, 0);
        TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_PASSENGERS, 4, 0);
        TolbiShipInteriors_IntroduceReplacementOarsman(3);
      } else {
        if (GameFlag_IsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED) != 0) {
          TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_RECRUITMENT_DECK, 0, 0);
          TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_PASSENGERS, 3, 0);
          TolbiShipInteriors_IntroduceReplacementOarsman(2);
        } else {
          if (GameFlag_IsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED) != 0) {
            TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_RECRUITMENT_DECK, 0,
                                               0);
            TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_PASSENGERS, 2, 0);
            TolbiShipInteriors_IntroduceReplacementOarsman(1);
          } else {
            if (GameFlag_IsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0) {
              TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_RECRUITMENT_DECK, 0,
                                                 0);
              Actor_SetPosition(10, 0, 0);
              TolbiShipInteriors_IntroduceReplacementOarsman(0);
            } else {
              Engine_ActorSetAnimation(9, 5);
              if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0 &&
                  GameFlag_IsSet(FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT) ==
                      0) {
                TolbiShipInteriors_AskForReplacementOarsmen();
              }
            }
          }
        }
      }
    }
  }
}

/// Sets collision bit 3 on the eight actors surrounding the rowing seats.
static void TolbiShipInteriors_SetSeatActorCollision(void) {
  u32 actorId;

  for (actorId = 28; actorId <= 35; actorId++) {
    struct FieldActor *actor = Object_GetById(actorId);
    u32 flags = actor->collisionFlags;
    actor->collisionFlags = flags | 0x08;
  }
}

/// Puts each recruited rower at its seat and hides the original occupant.
static void TolbiShipInteriors_PlaceFoundActors(s32 gated) {
  s32 actor;

  if (gated && !Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED)) {
    return;
  }
  if ((actor = TolbiShipInteriors_FindFirstSetFlag(0, 0)) != 0) {
    TolbiShipInteriors_PlaceActor(actor, 410, 172, 0xd000);
    Engine_ActorSetPosition(10, 0, 0);
  }
  if (gated && !Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED)) {
    return;
  }
  if ((actor = TolbiShipInteriors_FindFirstSetFlag(1, 0)) != 0) {
    TolbiShipInteriors_PlaceActor(actor, 470, 172, 0xb000);
    Engine_ActorSetPosition(11, 0, 0);
  }
  if (gated && !Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED)) {
    return;
  }
  if ((actor = TolbiShipInteriors_FindFirstSetFlag(2, 0)) != 0) {
    TolbiShipInteriors_PlaceActor(actor, 410, 204, 0xd000);
    Engine_ActorSetPosition(12, 0, 0);
  }
  if (gated) {
    return;
  }
  if ((actor = TolbiShipInteriors_FindFirstSetFlag(3, 0)) != 0) {
    TolbiShipInteriors_PlaceActor(actor, 470, 204, 0xb000);
    Engine_ActorSetPosition(13, 0, 0);
  }
}

/// The crew explains the missing replacements and asks the party to find
/// rowers.
static void TolbiShipInteriors_AskForReplacementOarsmen(void) {
  struct FieldActor *actor;
  s32 mode;

  // Bring Isaac to the crewman who explains the shortage of rowers.
  Engine_EventBegin();
  TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(440), -1, PIXELS(168), 0x1000001);
  TolbiShipInteriors_PlaceActor(27, 440, 164, 0x5000);
  TolbiShipInteriors_PlaceActor(8, 428, 190, 0xd000);
  TolbiShipInteriors_PlaceActor(9, 452, 190, 0xb000);
  Engine_ActorSetAnimation(9, 1);
  mode = 128;
  TolbiShipInteriors_PlaceActor(0, 440, 134, 0x8000);
  gEventWork->startTransition = (mode << 1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 408, 134);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 408, 148);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 424, 148);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
  Engine_ActorRunRepeatedMotion(27, 1);
  Engine_EventSetMessage(MsgTolbiShipOurReplacementNeverArrivedBut);
  TolbiShipInteriors_Say(27);
  Engine_ActorRunRepeatedMotion(8, 1);
  TolbiShipInteriors_Say(8);
  Engine_ActorSetAnimationAndWait(27, 3);
  TolbiShipInteriors_Say(27);
  TolbiShipInteriors_Turn(27, 0xd000);
  actor = Object_GetById(0);
  if (actor != 0) {
    Actor_SetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(GARET, 440, 148);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  actor = Object_GetById(1);
  if (actor != 0) {
    Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(IVAN, 456, 148);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  actor = Object_GetById(2);
  if (actor != 0) {
    Actor_SetPosition(MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(MIA, 472, 148);
  Actor_FaceDirection(MIA, FACING_SOUTH, 20);
  // Let the full party respond to the request for replacement oarsmen.
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 60);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY, 0x4000, 20);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  Actor_FaceDirection(27, 0x5000, 20);
  TolbiShipInteriors_Say(27);
  Engine_ActorStartRepeatedMotion(9, 1);
  Actor_ShowEmote(9, (mode << 1), 40);
  TolbiShipInteriors_Say(9);
  Engine_ActorStartRepeatedMotion(GARET, 3);
  Actor_ShowEmote(GARET, 0x103, 60);
  Engine_ActorSetAnimationAndWait(27, 3);
  TolbiShipInteriors_Say(27);
  Engine_ActorRunRepeatedMotion(10, 1);
  Engine_ActorSetAnimation(10, 3);
  TolbiShipInteriors_Say(10);
  Engine_ActorSetAnimation(8, 3);
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorSetAnimation(11, 3);
  Engine_ActorSetAnimation(12, 3);
  Engine_ActorSetAnimationAndWait(13, 3);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 40);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY, 0x4000, 20);
  Engine_ActorSetAnimationAndWait(27, 4);
  TolbiShipInteriors_Say(27);
  Actor_ShowEmote(8, 0x102, 60);
  Engine_ActorStartRepeatedMotion(8, 1);
  TolbiShipInteriors_Say(8);
  Engine_ActorSetAnimationAndWait(27, 3);
  TolbiShipInteriors_Say(27);
  Actor_FaceDirection(8, FACING_EAST, 0);
  Actor_FaceDirection(9, FACING_WEST, 40);
  Actor_ShowEmote(8, 0x102, 0);
  Actor_ShowEmote(8, 0x102, 40);
  Engine_ActorRunRepeatedMotion(27, 1);
  Engine_ActorSetAnimation(27, 3);
  Event_ShowMessageAndWait(27, 0, 20);
  Engine_ActorSetAnimation(8, 3);
  Engine_ActorSetAnimationAndWait(9, 3);
  Engine_EventWait(40);
  Actor_ShowEmote(9, (mode << 1), 20);
  TolbiShipInteriors_Turn(9, 0xb000);
  TolbiShipInteriors_Say(9);
  TolbiShipInteriors_Turn(27, 0x3000);
  Actor_ShowEmote(27, 0x101, 60);
  Event_ShowMessageAndWait(27, 0, 60);
  Actor_ShowEmote(27, 0x106, 20);
  TolbiShipInteriors_Turn(27, 0xb000);
  Engine_ActorSetAnimationAndWait(27, 3);
  TolbiShipInteriors_Say(27);
  TolbiShipInteriors_RunSceneCommand(SHIP_SHOW_PARTY_EMOTE, 2, 80);
  TolbiShipInteriors_Turn(8, 0xd000);
  Engine_ActorStartRepeatedMotion(8, 2);
  TolbiShipInteriors_Say(8);
  Engine_ActorSetAnimationAndWait(9, 3);
  Engine_ActorStartRepeatedMotion(9, 2);
  TolbiShipInteriors_Say(9);
  TolbiShipInteriors_Turn(27, 0x5000);
  Engine_ActorSetAnimationAndWait(27, 3);
  Engine_ActorRunRepeatedMotion(27, 1);
  TolbiShipInteriors_Say(27);
  Actor_SetSpeed(27, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(27, 408, 158);
  Actor_WalkToAndWait(27, 408, 148);
  Actor_FaceDirection(27, FACING_EAST, 20);
  Engine_ActorRunRepeatedMotion(27, 1);
  TolbiShipInteriors_Say(27);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY, 0x8000, 20);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 0);
  Actor_WalkToAndWait(27, 408, 134);
  Actor_WalkTo(27, 440, 134);
  Engine_EventWait(40);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCENE, 10, 0);
  GameFlag_Set(FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT);
}

/// Raises the anchor charm and announces the ship's first departure.
static void TolbiShipInteriors_CastOff(void) {
  struct FieldActor *anchorCharm;

  Engine_EventBegin();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_CAPTAINS_CABIN, 0, 1);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(20);
  Actor_SetSpeed(8, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(8, 468, 614);
  Actor_WalkToAndWait(8, 472, 596);
  Actor_FaceDirection(8, FACING_WEST, 20);
  Engine_ActorJump(8, 4, 20);
  anchorCharm = TolbiShipInteriors_PlaceAnchorCharm();
  Engine_EventWait(20);
  Event_PlayCue(SE_214);
  Engine_ObjectSetScript(anchorCharm, gTolbiShipInteriors_AnchorCharmPulse);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Actor_WalkToAndWait(8, 466, 624);
  TolbiShipInteriors_Turn(8, 0x5000);
  Engine_ActorStartRepeatedMotion(8, 2);
  Engine_EventSetMessage(MsgTolbiShipCastOff);
  Event_ShowMessageAndWait(8, 0, 20);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCENE, 11, 0);
}

/// Announces departure after a replacement rower has joined the crew.
static void TolbiShipInteriors_AnnounceDeparture(void) {
  Engine_EventBegin();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_CAPTAINS_CABIN, 1, 1);
  Actor_FaceDirection(8, 0x5000, 40);
  Engine_ActorStartRepeatedMotion(8, 2);
  Engine_EventSetMessage(MsgTolbiShipWereOff);
  Event_ShowMessageAndWait(8, 0, 20);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCENE, 11, 0);
}

/// Calls the first passenger to the rowing deck.
static void TolbiShipInteriors_CallActorSixteenToRow(void) {
  Engine_EventBegin();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 0, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_PASSENGER_HOLD, 0, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(16, PIXELS(150), PIXELS(586));
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(156), -1, PIXELS(536), 0x1000001);
  TolbiShipInteriors_RunSceneCommand(SHIP_OPEN_SCENE, 0, 0);
  Actor_SetSpeed(16, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(16, 168, 578);
  Actor_WalkToAndWait(16, 168, 554);
  Actor_FaceDirection(16, FACING_WEST, 20);
  Engine_ActorStartRepeatedMotion(16, 2);
  Engine_EventSetMessage(MsgTolbiShipRowThoseOars);
  Event_ShowMessageAndWait(16, 0, 20);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCENE, 12, 0);
}

/// Calls a replacement rower back to the rowing deck.
static void TolbiShipInteriors_CallActorEighteenToRow(void) {
  Engine_EventBegin();
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  TolbiShipInteriors_HideSelectedPassengers();
  Actor_SetPosition(18, PIXELS(150), PIXELS(586));
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(156), -1, PIXELS(536), 0x1000001);
  TolbiShipInteriors_RunSceneCommand(SHIP_OPEN_SCENE, 0, 0);
  Actor_SetSpeed(18, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(18, 168, 578);
  Actor_WalkToAndWait(18, 168, 554);
  Actor_FaceDirection(18, FACING_WEST, 20);
  Engine_ActorStartRepeatedMotion(18, 2);
  Engine_EventSetMessage(MsgTolbiShipRowThoseOars);
  Event_ShowMessageAndWait(18, 0, 20);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCENE, 12, 0);
}

/// Restores the rowing deck for the current set of selected passengers.
static void TolbiShipInteriors_ShowRowing(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(9, 5);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_ACTORS_28_TO_35, 0, 0);
  TolbiShipInteriors_InstallRowingDeckActors(0);
  TolbiShipInteriors_RunSceneCommand(SHIP_OPEN_SCENE, 1, 20);
  Engine_CameraSetSpeed(0x6666, 0xccc);
  Engine_CameraMoveTo(0x1b80000, -1, 0xb00000, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(9, 7);
  Engine_EventWait(30);
  Engine_AudioPlayCue(SE_188);
  Engine_EventWait(30);
  TolbiShipInteriors_InstallRowingDeckActors(16);
  Engine_EventWait(0x50);
  TolbiShipInteriors_InstallRowingDeckActors(0);
  Engine_EventWait(0x3c);
  Engine_ActorSetAnimation(9, 7);
  Engine_EventWait(30);
  Engine_AudioPlayCue(SE_188);
  Engine_EventWait(30);
  TolbiShipInteriors_InstallRowingDeckActors(16);
  Engine_EventWait(0x50);
  TolbiShipInteriors_InstallRowingDeckActors(0);
  Engine_EventWait(0x5a);
  Engine_AudioPlayCue(SE_188);
  Engine_EventWait(30);

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);

  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCENE, 0, 0);

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    Engine_EventRequestExit(20);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED) != 0) {
    Engine_EventRequestExit(18);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED) !=
             0) {
    Engine_EventRequestExit(17);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0) {
    Engine_EventRequestExit(16);
  } else {
    Engine_EventRequestExit(13);
  }
}

/// Asks the crew about the monsters before returning belowdecks.
static void TolbiShipInteriors_AskAboutMonsters(void) {
  Engine_EventBegin();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_CAPTAINS_CABIN, 1, 1);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(10);
  Actor_FaceDirection(8, 0x3000, 20);
  Engine_ActorStartRepeatedMotion(8, 2);
  Engine_EventSetMessage(MsgTolbiShipMonsters);
  Event_ShowMessageAndWait(8, 0, 20);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCENE, 14, 0);
}

/// Announces the ship's next turn and returns control to the rowing deck.
static void TolbiShipInteriors_AnnounceTurn(void) {
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_CAPTAINS_CABIN, 1, 1);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventSetMessage(MsgTolbiShipImTurning);
  TolbiShipInteriors_Say(8);
  Actor_FaceDirection(8, 0xd000, 40);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCENE, 15, 0);
}

/// Stages the first monster encounter with the original rowers.
static void TolbiShipInteriors_StartFirstAttack(void) {
  const s32 *entryScript;

  Engine_EventBegin();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 1, 0);
  TolbiShipInteriors_SetSeatActorCollision();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_MONSTER_ENCOUNTER, 11, 12);
  Engine_ActorSetAnimation(10, 6);
  Actor_SetActionScript(12, gTolbiShipInteriors_Shiver);
  entryScript = gTolbiShipInteriors_MonsterBob;
  Actor_SetActionScript(36, entryScript);
  Actor_SetActionScript(37, entryScript);
  Actor_SetActionScript(38, entryScript);
  Engine_ActorSetChildValue(36, 3);
  Engine_ActorSetChildValue(37, 3);
  Engine_ActorSetChildValue(38, 3);
  TolbiShipInteriors_ReportMonstersBelowdecks();
  Engine_EventEnd();
}

/// Restores the original rowers after the first monster attack.
static void TolbiShipInteriors_EndFirstAttack(void) {
  Engine_EventBegin();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_MONSTER_ENCOUNTER, 11, 12);
  TolbiShipInteriors_ReactToMonsterAttack();
  Engine_GameFlagSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED);
  Engine_EventEnd();
}

/// Introduces the selected passenger and leads them to the open rowing seat.
static void TolbiShipInteriors_IntroduceReplacementOarsman(s32 seat) {
  s32 rowerId;
  struct FieldActor *actor;
  s32 halfStep;
  s32 leftSeat;
  s32 x;

  Engine_EventBegin();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 0, 0);
  x = 0x1b0;
  halfStep = PIXELS(1) / 2;
  TolbiShipInteriors_PlaceActor(0, x, 134, FACING_WEST);
  TolbiShipInteriors_PlaceFoundActors(1);
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(0, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(0, 406, 134);
  Actor_WalkToAndWait(0, 406, 152);
  Actor_WalkToAndWait(0, 421, 152);
  Engine_ActorRunRepeatedMotion(27, 1);
  Engine_EventWait(20);
  Engine_ActorFaceEachOther(27, 0, 10);
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWER_CHOSEN) != 0) {
    rowerId = TolbiShipInteriors_FindFirstSetFlag(seat, 0);
    Engine_ActorRunRepeatedMotion(27, 1);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(27, 0, 10);
    Engine_EventSetMessage(MsgTolbiShipBackBroughtOarsman);
    TolbiShipInteriors_Say(CabinSpeakerRequest);
    Engine_ActorSetAnimationAndWait(0, 3);
    leftSeat = 0;
    Actor_SetSpeed(0, PIXELS(1), halfStep);
    Engine_ActorWalkToAndWait(0, x, 168);
    Engine_ActorFaceDirection(0, FACING_NORTH, 0);
    actor = Object_GetById(0);
    if (actor != 0) {
      Engine_ActorSetPosition(rowerId, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetSpeed(rowerId, PIXELS(1), halfStep);
    Actor_WalkToAndWait(rowerId, 448, 168);
    Actor_FaceDirection(rowerId, 0xb000, 20);
    Actor_FaceDirection(27, 0x3000, 20);
    Engine_ActorSetAnimation(27, 3);
    TolbiShipInteriors_Say(27);
    Actor_ShowEmote(rowerId, 0x102, 60);
    Engine_ActorRunRepeatedMotion(27, 1);
    Engine_ActorSetAnimation(27, 3);
    TolbiShipInteriors_Say(27);
    Engine_ActorSetAnimationAndWait(rowerId, 3);
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
      Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
      Actor_FaceDirection(27, 0x3000, 0);
      Actor_WalkToAndWait(rowerId, 470, 204);
      TolbiShipInteriors_Turn(rowerId, 0xb000);
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED) !=
               0) {
      Actor_WalkToAndWait(0, 422, 154);
      Actor_FaceDirection(0, FACING_SOUTHWEST, 0);
      Actor_FaceDirection(27, 0x5000, 0);
      Actor_WalkToAndWait(rowerId, 410, 204);
      leftSeat = 1;
      TolbiShipInteriors_Turn(rowerId, 0xd000);
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED) !=
               0) {
      Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
      Actor_FaceDirection(27, 0x3000, 0);
      Actor_WalkToAndWait(rowerId, 470, 172);
      TolbiShipInteriors_Turn(rowerId, 0xb000);
    } else {
      Actor_WalkToAndWait(0, 422, 154);
      Actor_FaceDirection(0, FACING_SOUTHWEST, 0);
      Actor_FaceDirection(27, 0x5000, 0);
      Actor_WalkToAndWait(rowerId, 410, 172);
      leftSeat = 1;
      TolbiShipInteriors_Turn(rowerId, 0xd000);
    }
    Engine_ActorFaceEachOther(27, 0, 20);
    TolbiShipInteriors_Say(0x201b);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_ActorSetAnimationAndWait(27, 3);
    Actor_SetSpeed(27, PIXELS(1), PIXELS(1) / 2);
    if (leftSeat != 0) {
      Actor_WalkToAndWait(27, 428, 164);
      Actor_WalkToAndWait(27, 408, 164);
    }
    Actor_WalkToAndWait(27, 408, 134);
    Engine_ActorWalkTo(27, 0x1b8, 134);
    Engine_EventWait(40);
    TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCENE, 10, 0);
  } else {
    Engine_EventSetMessage(MsgTolbiShipBack);
    Event_ShowMessageAndWait(CabinSpeakerRequest, 0, 40);
    Engine_ActorShowEmote(27, 0x101, 60);
    TolbiShipInteriors_Say(CabinSpeakerRequest);
    Engine_ActorSetAttachedEffect(0, 0x102);
    Engine_EventWait(60);
    Actor_ShowEmote(27, 0x103, 40);
    Engine_ActorStartRepeatedMotion(27, 2);
    TolbiShipInteriors_Say(CabinSpeakerRequest);
    Engine_ActorShowEmote(27, 0x105, 40);
    TolbiShipInteriors_Say(CabinSpeakerRequest);
    Engine_ActorSetAnimationAndWait(27, 4);
    TolbiShipInteriors_Say(CabinSpeakerRequest);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Engine_EventRequestExit(4);
  }
}

/// Creates the visible anchor charm once a visit (FLAG_SCENE_TEMP_00 records
/// it) and retains it for departure events.
static struct FieldActor *TolbiShipInteriors_PlaceAnchorCharm(void) {
  s32 set = GameFlag_IsSet(FLAG_SCENE_TEMP_00);
  struct FieldActor *object;
  struct AnimationObject *sprite;
  u8 *buffer;

  if (set != 0) {
    return gTolbiShipInteriors_AnchorObject;
  }
  object = Engine_ObjectCreate(22, 0x1c70000, 0x40000, 0x2200000);
  object->motionFlags = set;
  object->unk5c = 1;
  sprite = object->sprite;
  sprite->count = set;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
  buffer = Engine_HeapAllocate(17, 0x608);
  Engine_ItemLoadIcon(232);
  Engine_VramLoad(sprite->slot, 128, buffer + 0x400);
  Engine_HeapRelease(17);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
  gTolbiShipInteriors_AnchorObject = object;
  return object;
}

/// Restores each replacement rower or the original occupant of that seat.
static void TolbiShipInteriors_InstallRowingDeckActors(u32 action) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0) {
    s32 rowerId = TolbiShipInteriors_FindFirstSetFlag(0, 0);
    Engine_ActorSetPosition(rowerId, PIXELS(410), PIXELS(172));
    TolbiShipInteriors_RunSceneCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId,
                                       action);
    Engine_ActorSetPosition(10, 0, 0);
  } else {
    TolbiShipInteriors_RunSceneCommand(SHIP_START_ROWING_ANIMATION, 10, action);
  }

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED) != 0) {
    s32 rowerId = TolbiShipInteriors_FindFirstSetFlag(1, 0);
    Engine_ActorSetPosition(rowerId, PIXELS(470), PIXELS(172));
    Object_GetById(rowerId)->scaleX = -Q16_ONE;
    TolbiShipInteriors_RunSceneCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId,
                                       action);
    Engine_ActorSetPosition(11, 0, 0);
  } else {
    TolbiShipInteriors_RunSceneCommand(SHIP_START_MIRRORED_ROWING_ANIMATION, 11,
                                       action);
  }

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED) != 0) {
    s32 rowerId = TolbiShipInteriors_FindFirstSetFlag(2, 0);
    Engine_ActorSetPosition(rowerId, PIXELS(410), PIXELS(204));
    TolbiShipInteriors_RunSceneCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId,
                                       action);
    Engine_ActorSetPosition(12, 0, 0);
  } else {
    TolbiShipInteriors_RunSceneCommand(SHIP_START_ROWING_ANIMATION, 12, action);
  }

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    s32 rowerId = TolbiShipInteriors_FindFirstSetFlag(3, 0);
    Engine_ActorSetPosition(rowerId, PIXELS(470), PIXELS(204));
    Object_GetById(rowerId)->scaleX = -Q16_ONE;
    TolbiShipInteriors_RunSceneCommand(SHIP_SET_SELECTED_ROWER_POSE, rowerId,
                                       action);
    Engine_ActorSetPosition(13, 0, 0);
  } else {
    TolbiShipInteriors_RunSceneCommand(SHIP_START_MIRRORED_ROWING_ANIMATION, 13,
                                       action);
  }

  TolbiShipInteriors_RunSceneCommand(SHIP_START_ROWING_ANIMATION, 14, action);
  TolbiShipInteriors_RunSceneCommand(SHIP_START_MIRRORED_ROWING_ANIMATION, 15,
                                     action);
  TolbiShipInteriors_RunSceneCommand(SHIP_START_ROWING_ANIMATION, 16, action);
  TolbiShipInteriors_RunSceneCommand(SHIP_START_MIRRORED_ROWING_ANIMATION, 17,
                                     action);
}

/// Applies the seated pose and rowing action assigned to this rower.
static void TolbiShipInteriors_PoseSceneActor(s32 id) {
  switch (id) {
  case 19:
    Engine_ActorSetAnimation(id, 6);
    Object_SetActionById(id, 8);
    break;
  case 18:
  case 20:
    Engine_ActorSetAnimation(id, 5);
    Object_SetActionById(id, 16);
    break;
  case 22:
  case 23:
    Engine_ActorSetAnimation(id, 5);
    Object_SetActionById(id, 20);
    break;
  case 24:
    Engine_ActorSetAnimation(id, 10);
    Object_SetActionById(id, 8);
    break;
  case 21:
  case 25:
    Engine_ActorSetAnimation(id, 5);
    Object_SetActionById(id, 4);
    break;
  case 26:
    Engine_ActorSetAnimation(id, 9);
    Object_SetActionById(id, 4);
    break;
  }
}

static void TolbiShipInteriors_Say(s32 speaker) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(10);
}

/// Turns an actor to a facing over 10 frames.
static void TolbiShipInteriors_Turn(s32 actorId, u16 facing) {
  Engine_ActorFaceDirection(actorId, facing, 10);
}

/// Places an actor at pixel coordinates and sets its initial heading.
static void TolbiShipInteriors_PlaceActor(s32 actorId, s32 x, s32 z,
                                          s32 facing) {
  Engine_ActorSetPosition(actorId, x << 16, z << 16, facing);
  Object_GetById(actorId)->facing = facing;
}

/// Moves the camera, optionally waits or redraws, then honors the packed delay.
static void TolbiShipInteriors_MoveCameraAndWait(s32 x, s32 y, s32 z,
                                                 u32 flags) {
  u32 selected;

  Engine_CameraMoveTo(x, y, z, ~flags & 1);
  selected = flags & 0x1111;
  if ((flags & 0x10000000) != 0)
    Engine_CameraWaitForMove();
  if ((flags & 0x01000000) != 0)
    Engine_MapRedraw();
  Engine_EventWait(selected);
}

/// Reuses the voyage's actor, camera, transition and selection operations.
/// Each command names how it interprets the two arguments below.
static void TolbiShipInteriors_RunSceneCommand(s32 step, u32 arg, u32 opt) {
  u32 slot;

  switch (step) {
  case SHIP_FACE_PARTY_EAST_AND_WEST:
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(GARET, FACING_WEST, 0);
    Engine_ActorFaceDirection(2, 0, 0);
    Actor_FaceDirection(MIA, FACING_WEST, opt);
    break;
  case SHIP_FACE_PARTY:
    Actor_FaceDirection(ACTOR_PARTY_LEADER, arg, 0);
    Actor_FaceDirection(GARET, arg, 0);
    Actor_FaceDirection(IVAN, arg, 0);
    Actor_FaceDirection(MIA, arg, opt);
    break;
  case SHIP_PLAY_PARTY_ANIMATION:
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GARET, 3);
    Engine_ActorSetAnimation(IVAN, 3);
    Engine_ActorSetAnimation(MIA, 3);
    if (arg != 0) {
      ObjectMotion_WaitForAnimationChange(3);
    }
    if (opt == 0) {
      break;
    }
    Engine_EventWait(opt);
    break;
  case SHIP_SHOW_PARTY_EMOTE:
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GARET, 0x102);
    Actor_SetAttachedEffect(IVAN, 0x102);
    Actor_SetAttachedEffect(MIA, 0x102);
    Engine_EventWait(opt);
    break;
  case SHIP_HIDE_PASSENGERS:
    for (slot = 0; slot < arg; slot++) {
      Actor_SetPosition(slot + 10, 0, 0);
    }
    break;
  case SHIP_START_ROWING_ANIMATION: {
    struct FieldActor *actor;

    actor = Object_GetById(arg);
    actor->facing = 0x5000;
  }
    Engine_ActorSetAnimation(arg, 5);
    Object_SetActionById(arg, opt);
    break;
  case SHIP_START_MIRRORED_ROWING_ANIMATION: {
    struct FieldActor *actor;

    actor = Object_GetById(arg);
    actor->facing = 0x5000;
    actor->scaleX = -Q16_ONE;
  }
    Engine_ActorSetAnimation(arg, 5);
    Object_SetActionById(arg, opt);
    break;
  case SHIP_SET_SELECTED_ROWER_POSE: {
    struct FieldActor *actor;

    actor = Object_GetById(arg);
    actor->facing = 0x5000;
  }
    TolbiShipInteriors_PoseSceneActor(arg);
    if (opt == 0) {
      Object_SetActionById(arg, 0);
    }
    break;
  case SHIP_OPEN_SCENE:
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventOpenScreen();
    if (arg != 0) {
      Engine_EventWaitForScreen();
    }
    Engine_EventWait(0);
    break;
  case SHIP_CLOSE_SCENE:
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    if (arg == 0) {
      break;
    }
    Engine_EventRequestExit(arg);
    break;
  case SHIP_SETUP_ROWING_PARTY:
    TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
    TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
    TolbiShipInteriors_PlaceFoundActors(0);
    TolbiShipInteriors_PlaceActor(0, 432, 168, 0x4000);
    TolbiShipInteriors_PlaceActor(1, 448, 168, 0x4000);
    TolbiShipInteriors_PlaceActor(2, 424, 152, 0x4000);
    TolbiShipInteriors_PlaceActor(3, 458, 152, 0x4000);
    break;
  case SHIP_RESTORE_ROWING_DECK_POSES:
    if (arg != 0) {
      struct FieldActor *actor;

      Engine_ActorSetAnimation(13, 1);
      actor = Object_GetById(13);
      actor->facing = 0x3000;
      actor = Object_GetById(13);
      actor->scaleX = Q16_ONE;
    }
    {
      struct FieldActor *actor;

      Engine_ActorSetAnimation(14, 1);
      actor = Object_GetById(14);
      actor->facing = 0x5000;
      Engine_ActorSetAnimation(15, 1);
      actor = Object_GetById(15);
      actor->facing = 0x3000;
      actor = Object_GetById(15);
      actor->scaleX = Q16_ONE;
      Engine_ActorSetAnimation(16, 1);
      actor = Object_GetById(16);
      actor->facing = 0x5000;
      Engine_ActorSetAnimation(17, 1);
      actor = Object_GetById(17);
      actor->facing = 0x3000;
      actor = Object_GetById(17);
      actor->scaleX = Q16_ONE;
    }
    Actor_SetPosition(28, PIXELS(410), PIXELS(174));
    Actor_SetPosition(29, PIXELS(470), PIXELS(174));
    Actor_SetPosition(30, PIXELS(410), PIXELS(206));
    Actor_SetPosition(31, PIXELS(470), PIXELS(206));
    Actor_SetPosition(32, PIXELS(410), PIXELS(286));
    Actor_SetPosition(33, PIXELS(470), PIXELS(286));
    Actor_SetPosition(34, PIXELS(410), PIXELS(316));
    Actor_SetPosition(35, PIXELS(470), PIXELS(316));
    Engine_TaskWait(1);
    if (arg != 0) {
      Actor_FaceDirection(13, 0xb000, 0);
    }
    Actor_FaceDirection(14, 0xd000, 0);
    Actor_FaceDirection(15, 0xb000, 0);
    Actor_FaceDirection(16, 0xd000, 0);
    TolbiShipInteriors_Turn(17, 0xb000);
    break;
  case SHIP_RESTORE_ROWER_POSE: {
    struct FieldActor *actor;

    actor = Object_GetById(arg);
    Engine_ActorSetAnimation(arg, 1);
    if (opt != 0) {
      actor->facing = 0x3000;
    } else {
      actor->facing = 0x5000;
    }
    actor->scaleX = Q16_ONE;
  } break;
  case SHIP_HIDE_CABIN_CREW:
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(10, 0, 0);
    break;
  case SHIP_HIDE_ACTORS_13_AND_14:
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(13, 0, 0);
    break;
  case SHIP_SETUP_CAPTAINS_CABIN:
    TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    TolbiShipInteriors_PlaceActor(8, 444, 614, 0xd000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    if (arg != 0) {
      TolbiShipInteriors_PlaceAnchorCharm();
    }
    TolbiShipInteriors_MoveCameraAndWait(PIXELS(448), PIXELS(32), PIXELS(624),
                                         0x1000001);
    if (opt == 0) {
      break;
    }
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    break;
  case SHIP_SETUP_RECRUITMENT_DECK:
    Actor_SetPosition(8, 0, 0);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(27, PIXELS(438), PIXELS(152));
    break;
  case SHIP_HIDE_ACTORS_28_TO_35:
    slot = 0;
    do {
      Actor_SetPosition(slot + 28, 0, 0);
      slot++;
    } while (slot <= 7);
    break;
  case SHIP_SETUP_PASSENGER_HOLD:
    TolbiShipInteriors_PlaceActor(12, 152, 532, 0xb000);
    TolbiShipInteriors_PlaceActor(8, 134, 490, 0x3000);
    TolbiShipInteriors_PlaceActor(9, 166, 490, 0x5000);
    TolbiShipInteriors_PlaceActor(10, 182, 504, 0x5000);
    TolbiShipInteriors_PlaceActor(11, 118, 504, 0x3000);
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_ACTORS_13_AND_14, 0, 0);
    break;
  case SHIP_SETUP_MONSTER_ENCOUNTER:
    TolbiShipInteriors_PlaceActor(8, 416, 328, 0);
    TolbiShipInteriors_PlaceActor(9, 448, 352, 0xd000);
    TolbiShipInteriors_PlaceActor(10, 454, 248, 0x3000);
    TolbiShipInteriors_PlaceActor(arg, 408, 290, 0);
    TolbiShipInteriors_PlaceActor(opt, 408, 342, 0);
    TolbiShipInteriors_PlaceActor(13, 420, 356, 0xd000);
    TolbiShipInteriors_PlaceActor(14, 408, 304, 0);
    TolbiShipInteriors_PlaceActor(15, 418, 378, 0xd000);
    TolbiShipInteriors_PlaceActor(16, 440, 262, 0x3000);
    TolbiShipInteriors_PlaceActor(17, 448, 378, 0xd000);
    break;
  case SHIP_CLEAR_FLAG_RANGE:
    for (slot = arg; slot <= opt; slot++) {
      GameFlag_Clear(slot);
    }
    break;
  case SHIP_CLEAR_ROWING_SELECTIONS:
    TolbiShipInteriors_RunSceneCommand(SHIP_CLEAR_FLAG_RANGE,
                                       FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS,
                                       FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS + 8);
    TolbiShipInteriors_RunSceneCommand(SHIP_CLEAR_FLAG_RANGE,
                                       FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS,
                                       FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS + 8);
    TolbiShipInteriors_RunSceneCommand(SHIP_CLEAR_FLAG_RANGE,
                                       FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS,
                                       FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS + 8);
    GameFlag_Clear(FLAG_TOLBI_SHIP_ROWER_CHOSEN);
    GameFlag_Clear(FLAG_TOLBI_SHIP_MONSTERS_ABOARD);
    GameFlag_Clear(FLAG_TOLBI_SHIP_KRAKEN_ABOARD);
    break;
  case SHIP_RESTORE_ACTOR_12_SCRIPT:
    Engine_TaskWait(1);
    TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_PARTY_FOLLOWERS, 0, 0);
    Actor_SetActionScript(12, gTolbiShipInteriors_Shiver);
    break;
  case SHIP_HIDE_PARTY_FOLLOWERS:
    Engine_ActorDestroy(GARET);
    Engine_ActorDestroy(IVAN);
    Engine_ActorDestroy(MIA);
    break;
  case SHIP_RESET_CAMERA:
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    if (arg != 0) {
      Camera_GetEventObject()->motionFlags = 0;
    }
    break;
  case SHIP_PLACE_FOLLOWERS_AND_MONSTERS:
    Event_CallWithLastActiveObjectId(
        gTolbiShipInteriors_PartyFollowerPlacements);
    Engine_TaskWait(1);
    if (arg == 1) {
      Event_CallWithLastActiveObjectId(gTolbiShipInteriors_FirstAttackMonsters);
      Engine_TaskWait(1);
    } else if (arg == 2) {
      Event_CallWithLastActiveObjectId(
          gTolbiShipInteriors_SecondAttackMonsters);
      Engine_TaskWait(1);
    } else if (arg == 3) {
      Event_CallWithLastActiveObjectId(gTolbiShipInteriors_ThirdAttackMonsters);
      Engine_TaskWait(1);
    }
    break;
  }
}

/// Finds the selected passenger or rowing-deck actor for a seat's flag group.
static s32 TolbiShipInteriors_FindFirstSetFlag(u32 group, s32 alternate) {
  s32 flag = 0;
  s32 id = 8;
  u32 i;

  if (alternate == 0) {
    id = 18;
  }
  switch (group) {
  case 0:
    flag = FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS;
    break;
  case 1:
    flag = FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS;
    break;
  case 2:
    flag = FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS;
    break;
  case 3:
    flag = FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS;
    break;
  }
  for (i = 0; i <= 8; i++, flag++, id++) {
    if (Engine_GameFlagIsSet(flag) != 0) {
      return id;
    }
  }
  return 0;
}

/// Hides the four passengers who are already filling rowing seats.
static void TolbiShipInteriors_HideSelectedPassengers(void) {
  TolbiShipInteriors_HideSelectedPassenger(8,
                                           FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS);
  TolbiShipInteriors_HideSelectedPassenger(8,
                                           FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS);
  TolbiShipInteriors_HideSelectedPassenger(8,
                                           FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS);
  TolbiShipInteriors_HideSelectedPassenger(8,
                                           FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS);
}

/// Hides the passenger whose selection flag is set in this seat's flag group.
static void TolbiShipInteriors_HideSelectedPassenger(s32 actorId,
                                                     s32 firstFlag) {
  unsigned int i = 0;

  do {
    if (Engine_GameFlagIsSet(firstFlag) != 0) {
      Engine_ActorSetPosition(actorId, 0, 0);
      break;
    }
    i++;
    actorId++;
    firstFlag++;
  } while (i <= 8);
}

/// Stages the next monster encounter with one replacement rower.
static void TolbiShipInteriors_StartSecondAttack(void) {
  s32 firstRower;
  const s32 *rowerScript;
  const s32 *entryScript;

  firstRower = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  Engine_EventBegin();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 2, 0);
  TolbiShipInteriors_SetSeatActorCollision();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_MONSTER_ENCOUNTER, firstRower,
                                     12);
  Engine_ActorSetAnimation(10, 6);
  rowerScript = gTolbiShipInteriors_Shiver;
  Actor_SetActionScript(firstRower, rowerScript);
  Engine_ActorDestroy(11);
  Actor_SetActionScript(12, rowerScript);
  entryScript = gTolbiShipInteriors_MonsterBob;
  Actor_SetActionScript(36, entryScript);
  Actor_SetActionScript(37, entryScript);
  TolbiShipInteriors_ReportMonstersBelowdecks();
  Engine_EventEnd();
}

/// Gathers the party after the crew reports monsters belowdecks.
static void TolbiShipInteriors_ReportMonstersBelowdecks(void) {
  struct FieldActor *actor;
  s32 message;

  TolbiShipInteriors_MoveCameraAndWait(PIXELS(440), -1, PIXELS(176), 0x1000001);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(440), PIXELS(134));
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 5);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x198, 134);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x198, 152);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x1b0, 166);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  actor = Object_GetById(0);
  if (actor != 0) {
    Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Object_GetById(0);
  if (actor != 0) {
    Actor_SetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Object_GetById(1);
  if (actor != 0) {
    Actor_SetPosition(MIA, actor->x.fixed, actor->z.fixed);
  }
  Engine_TaskWait(1);
  Actor_SetSpeed(IVAN, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkTo(IVAN, 424, 152);
  Actor_SetSpeed(GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkTo(GARET, 448, 168);
  Actor_SetSpeed(MIA, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(MIA, 458, 152);
  Engine_ActorSetAnimation(GARET, 1);
  Engine_ActorSetAnimation(IVAN, 1);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH, 40);
  Ui_SetMessageVoiceFromActor(10);
  message = MsgTolbiShipMonstersBelowdecks;
  UiText_ShowCenteredMessage(message, 1, 10);
  Engine_EventWait(10);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY_EAST_AND_WEST, 0, 40);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY, 0x4000, 20);
  Camera_SetSpeed(0x39999, 0x7333);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(440), -1, PIXELS(320),
                                       0x10000014);
  Engine_ActorRunRepeatedMotion(8, 2);
  TolbiShipInteriors_Turn(8, 0xd000);
  Engine_EventSetMessage(message + 1);
  TolbiShipInteriors_Say(8);
  Actor_FaceDirection(8, FACING_EAST, 20);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(440), -1, PIXELS(134),
                                       0x10000000);
  Actor_SetActionScript(GARET, gTolbiShipInteriors_FollowersRejoinByStairs);
  Actor_SetActionScript(2, gTolbiShipInteriors_FollowersRejoinByStairs);
  Actor_RunActionScriptAndWait(3, gTolbiShipInteriors_FollowersRejoinByStairs);
  Engine_EventWait(40);
  GameFlag_Set(FLAG_TOLBI_SHIP_MONSTERS_ABOARD);
  TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_PARTY_FOLLOWERS, 0, 0);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
}

/// The crew checks on the party after the monster fight, then resumes rowing.
static void TolbiShipInteriors_ReactToMonsterAttack(void) {
  const s32 *action;

  TolbiShipInteriors_PlaceActor(0, 444, 300, 0);
  TolbiShipInteriors_PlaceActor(1, 458, 310, 0);
  TolbiShipInteriors_PlaceActor(2, 444, 330, 0);
  TolbiShipInteriors_PlaceActor(3, 432, 310, 0);
  TolbiShipInteriors_PlaceActor(27, 440, 134, 0x8000);
  TolbiShipInteriors_PlaceActor(10, 454, 248, 0x3000);
  Engine_ActorSetAnimation(10, 6);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(440), -1, PIXELS(308), 0x1000001);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 40);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  Engine_EventSetMessage(MsgTolbiShipHeyAreYouOk);
  TolbiShipInteriors_Say(27);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY, 0xc000, 0);
  Camera_SetSpeed(0x26666, 0x4ccc);
  Camera_MoveTo(PIXELS(440), -1, PIXELS(176), 1);
  Actor_SetSpeed(27, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(27, 408, 134);
  Actor_WalkToAndWait(27, 408, 152);
  Actor_WalkToAndWait(27, 424, 164);
  Camera_SetSpeed(0x19999, 0x3333);
  Camera_MoveTo(PIXELS(440), -1, PIXELS(300), 1);
  Actor_WalkToAndWait(27, 424, 222);
  Actor_WalkToAndWait(27, 424, 262);
  Actor_FaceDirection(27, 0x3000, 20);
  Engine_ActorRunRepeatedMotion(27, 1);
  TolbiShipInteriors_Say(27);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  Engine_ActorSetAnimationAndWait(27, 3);
  Engine_ActorRunRepeatedMotion(27, 1);
  TolbiShipInteriors_Say(27);
  TolbiShipInteriors_RunSceneCommand(SHIP_SHOW_PARTY_EMOTE, 2, 60);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY, 0xe000, 60);
  Actor_FaceDirection(27, FACING_EAST, 40);
  Engine_ActorRunRepeatedMotion(27, 1);
  Engine_ActorSetAnimation(27, 2);
  Actor_MoveToAndWait(27, 0x1b0, 0x10c);
  Actor_MoveToAndWait(27, 0x1c4, 0x10c);
  Engine_ActorSetAnimation(27, 1);
  TolbiShipInteriors_Turn(27, 0xd000);
  Engine_ActorStartRepeatedMotion(27, 2);
  Event_ShowMessageAndWait(27, 0, 20);
  TolbiShipInteriors_RunSceneCommand(SHIP_FACE_PARTY, 0xc000, 20);
  Engine_ActorSetAnimationAndWait(27, 4);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(27, 0, 80);
  Engine_ActorRunRepeatedMotion(27, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(27, 3);
  Engine_EventWait(10);
  TolbiShipInteriors_Turn(27, 0x5000);
  Event_OpenMessage(27, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(27, 3);
    TolbiShipInteriors_Say(27);
  } else {
    Engine_ActorSetAnimationAndWait(27, 4);
    gEventWork->message += 1;
    TolbiShipInteriors_Say(27);
    TolbiShipInteriors_RunSceneCommand(SHIP_SHOW_PARTY_EMOTE, 2, 40);
    Engine_ActorRunRepeatedMotion(27, 1);
    Engine_ActorSetAnimation(27, 3);
    TolbiShipInteriors_Say(27);
  }
  TolbiShipInteriors_RunSceneCommand(SHIP_PLAY_PARTY_ANIMATION, 1, 20);
  action = gTolbiShipInteriors_FollowersRejoinMidDeck;
  Actor_SetActionScript(GARET, action);
  Actor_SetActionScript(IVAN, action);
  Actor_RunActionScriptAndWait(3, action);
  Camera_SetSpeed(0x9999, 0x1333);
  Camera_MoveTo(PIXELS(440), -1, PIXELS(176), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 424, 272);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 424, 164);
  Engine_EventWait(60);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCENE, 0, 0);
  GameFlag_Clear(FLAG_TOLBI_SHIP_MONSTERS_ABOARD);
  GameFlag_Clear(FLAG_TOLBI_SHIP_ROWING);
  Engine_EventRequestExit(4);
}

/// Restores one replacement rower after the next monster attack.
static void TolbiShipInteriors_EndSecondAttack(void) {
  s32 handle = TolbiShipInteriors_FindFirstSetFlag(0, 0);

  Engine_EventBegin();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
  TolbiShipInteriors_PlaceFoundActors(0);
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_MONSTER_ENCOUNTER, handle, 12);
  Engine_ActorSetPosition(11, 0, 0);
  TolbiShipInteriors_ReactToMonsterAttack();
  Engine_GameFlagSet(FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED);
  Engine_EventEnd();
}

/// Stages the next monster encounter with two replacement rowers.
static void TolbiShipInteriors_StartThirdAttack(void) {
  s32 secondRower;
  s32 firstRower;
  const s32 *rowerScript;
  const s32 *entryScript;

  firstRower = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  secondRower = TolbiShipInteriors_FindFirstSetFlag(1, 0);
  Engine_EventBegin();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 3, 0);
  TolbiShipInteriors_SetSeatActorCollision();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_MONSTER_ENCOUNTER, firstRower,
                                     secondRower);
  Engine_ActorSetAnimation(10, 6);
  rowerScript = gTolbiShipInteriors_Shiver;
  Actor_SetActionScript(firstRower, rowerScript);
  Engine_ActorDestroy(11);
  Actor_SetActionScript(secondRower, rowerScript);
  Engine_ActorDestroy(12);
  entryScript = gTolbiShipInteriors_MonsterBob;
  Actor_SetActionScript(36, entryScript);
  Actor_SetActionScript(37, entryScript);
  Engine_ActorSetChildValue(36, 3);
  Engine_ActorSetChildValue(37, 3);
  TolbiShipInteriors_ReportMonstersBelowdecks();
  Engine_EventEnd();
}

/// Restores two replacement rowers after the next monster attack.
static void TolbiShipInteriors_EndThirdAttack(void) {
  s32 handle = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  s32 other = TolbiShipInteriors_FindFirstSetFlag(1, 0);

  Engine_EventBegin();
  TolbiShipInteriors_RunSceneCommand(SHIP_RESET_CAMERA, 1, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_PLACE_FOLLOWERS_AND_MONSTERS, 0, 0);
  TolbiShipInteriors_PlaceFoundActors(0);
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_MONSTER_ENCOUNTER, handle,
                                     other);
  Engine_ActorSetPosition(11, 0, 0);
  Engine_ActorSetPosition(12, 0, 0);
  TolbiShipInteriors_ReactToMonsterAttack();
  Engine_GameFlagSet(FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED);
  Engine_EventEnd();
}

/// The party notices the ship rocking and discusses the rowers' progress.
static void TolbiShipInteriors_NoticeShipRocking(void) {
  s32 secondRower;
  s32 thirdRower;
  s32 firstRower;
  struct FieldActor *actor;
  const s32 *action;

  firstRower = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  secondRower = TolbiShipInteriors_FindFirstSetFlag(1, 0);
  thirdRower = TolbiShipInteriors_FindFirstSetFlag(2, 0);
  // Restore the occupied rowing seats before showing the ship's movement.
  Engine_EventBegin();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_ACTORS_28_TO_35, 0, 0);
  Actor_SetPosition(8, PIXELS(472), PIXELS(152));
  Engine_ActorSetAnimation(9, 5);
  Actor_SetPosition(27, PIXELS(440), PIXELS(134));
  Actor_SetChildValue(27, 15);
  actor = Object_GetById(27);
  Engine_ActorSetSpriteFlags(actor, 0);
  TolbiShipInteriors_InstallRowingDeckActors(16);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(438), -1, PIXELS(174), 0x1000001);
  TolbiShipInteriors_RunSceneCommand(SHIP_OPEN_SCENE, 1, 20);
  Event_PlayCue(MUS_019);
  Event_PlayCue(SE_181);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Engine_EventWait(10);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_EventWait(80);
  Event_PlayCue(SE_181);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Engine_EventWait(10);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Event_PlayCue(MUS_063);
  GameFlag_Set(FLAG_KEEP_MUSIC);
  Actor_SetAttachedEffect(MIA, 0x102);
  Engine_EventWait(40);
  TolbiShipInteriors_Turn(3, 0x6000);
  // The party notices the rocking, then turns to the crew for an explanation.
  Engine_EventSetMessage(MsgTolbiShipWonderWhatsWrongShipShouldnt);
  Event_ShowMessageAndWait(MIA, 0, 40);
  TolbiShipInteriors_Say(27);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_EAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 40);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Actor_ShowEmote(IVAN, 0x100, 60);
  TolbiShipInteriors_Turn(2, 0x2000);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  TolbiShipInteriors_Say(2);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Actor_SetChildValue(27, 0);
  actor = Object_GetById(27);
  Engine_ActorSetSpriteFlags(actor, 1);
  Actor_SetSpeed(27, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(27, 430, 134);
  TolbiShipInteriors_Turn(27, 0x3000);
  Engine_ActorStartRepeatedMotion(27, 2);
  TolbiShipInteriors_Say(27);
  Engine_ActorStartRepeatedMotion(firstRower, 1);
  Engine_ActorStartRepeatedMotion(secondRower, 1);
  Engine_ActorStartRepeatedMotion(thirdRower, 1);
  Engine_ActorRunRepeatedMotion(13, 1);
  Actor_SetAttachedEffect(firstRower, 0x102);
  Actor_SetAttachedEffect(secondRower, 0x102);
  Actor_SetAttachedEffect(thirdRower, 0x102);
  Actor_SetAttachedEffect(13, 0x102);
  Engine_EventWait(40);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWER_POSE, firstRower, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWER_POSE, secondRower, 1);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWER_POSE, thirdRower, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWING_DECK_POSES, 1, 0);
  Actor_FaceDirection(firstRower, 0xd000, 0);
  Actor_FaceDirection(secondRower, 0xb000, 0);
  Actor_FaceDirection(thirdRower, 0xd000, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_EAST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 40);
  Engine_ActorStartRepeatedMotion(27, 2);
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 20);
  Actor_WalkToAndWait(27, 440, 134);
  Actor_SetPosition(27, 0, 0);
  TolbiShipInteriors_Turn(1, 0x8000);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  TolbiShipInteriors_Say(1);
  Actor_FaceDirection(IVAN, FACING_EAST, 0);
  TolbiShipInteriors_Turn(3, 0x8000);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  action = gTolbiShipInteriors_FollowersRejoinBySeats;
  Actor_SetActionScript(GARET, action);
  Actor_SetActionScript(IVAN, action);
  Actor_RunActionScriptAndWait(3, action);
  GameFlag_Set(FLAG_TOLBI_SHIP_KRAKEN_ABOARD);
  gTolbiShipInteriors_ShakeTimer = 0;
  FieldScheduler_AddCallback(TolbiShipInteriors_ShakeWhileKrakenAboard, 0xc80);
  TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_PARTY_FOLLOWERS, 0, 0);
  Engine_ActorDestroy(27);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  GameFlag_Clear(FLAG_TOLBI_SHIP_ROWING);
  Engine_EventEnd();
}

/// Plays the intermittent voyage cue and restores its effect after ten frames.
static void TolbiShipInteriors_ShakeWhileKrakenAboard(void) {
  s32 value;
  s32 *timer = &gTolbiShipInteriors_ShakeTimer;
  s32 remaining;
  if (*timer != 0) {
    remaining = *timer - 1;
    *timer = *timer - 1;
    if (remaining == 70)
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  } else {
    value = Engine_RandomNext();
    if (((u32)(((value << 4) - value) << 3) >> 16) == 0) {
      Engine_AudioPlayCue(SE_181);
      Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
      *timer = 80;
    }
  }
}

/// Plays the short cabin exchange before returning to the voyage.
static void TolbiShipInteriors_ShowCabinBeforeKraken(void) {
  Engine_EventBegin();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_CAPTAINS_CABIN, 1, 0);
  TolbiShipInteriors_PlaceActor(9, 468, 616, 32768);
  TolbiShipInteriors_RunSceneCommand(SHIP_OPEN_SCENE, 1, 20);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(8, 53248, 80);
  Actor_FaceDirection(8, FACING_EAST, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  TolbiShipInteriors_RunSceneCommand(SHIP_CLOSE_SCENE, 21, 0);
}

/// Rests the rowers at the island and plays the selected passengers' responses.
static void TolbiShipInteriors_ReachIsland(void) {
  s32 fourthRower;
  s32 secondRower;
  s32 thirdRower;
  s32 firstRower;
  struct FieldActor *actor;
  s32 reaction = 0;
  const s32 *progressB;
  const s32 *progressC;
  const s32 *progressA;

  firstRower = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  secondRower = TolbiShipInteriors_FindFirstSetFlag(1, 0);
  thirdRower = TolbiShipInteriors_FindFirstSetFlag(2, 0);
  fourthRower = TolbiShipInteriors_FindFirstSetFlag(3, 0);
  // Restore all four selected rowers and announce the island stop.
  Engine_EventBegin();
  TolbiShipInteriors_SetSeatActorCollision();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_ACTORS_28_TO_35, 0, 0);
  Actor_SetPosition(8, PIXELS(472), PIXELS(152));
  Actor_SetPosition(27, PIXELS(440), PIXELS(134));
  Engine_ActorSetChildValue(27, 15);
  actor = Object_GetById(27);
  Engine_ActorSetSpriteFlags(actor, 0);
  TolbiShipInteriors_InstallRowingDeckActors(16);
  Engine_ActorSetAnimation(9, 5);
  TolbiShipInteriors_MoveCameraAndWait(PIXELS(438), -1, PIXELS(174), 0x1000001);
  TolbiShipInteriors_RunSceneCommand(SHIP_OPEN_SCENE, 1, 20);
  Engine_ActorSetChildValue(27, 0);
  actor = Object_GetById(27);
  Engine_ActorSetSpriteFlags(actor, 1);
  Actor_SetSpeed(27, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(27, 408, 132);
  Actor_WalkToAndWait(27, 408, 142);
  Actor_FaceDirection(27, 0x3000, 20);
  Engine_ActorStartRepeatedMotion(27, 2);
  Engine_EventSetMessage(MsgTolbiShipReachedIslandRowers);
  TolbiShipInteriors_Say(27);
  Engine_EventWait(120);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWER_POSE, firstRower, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWER_POSE, secondRower, 1);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWER_POSE, thirdRower, 0);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWER_POSE, fourthRower, 1);
  TolbiShipInteriors_RunSceneCommand(SHIP_RESTORE_ROWING_DECK_POSES, 0, 0);
  Actor_FaceDirection(firstRower, 0xd000, 0);
  Actor_FaceDirection(secondRower, 0xb000, 0);
  Actor_FaceDirection(thirdRower, 0xd000, 0);
  Engine_ActorFaceDirection(fourthRower, 0xb000, 60);
  // The first rower's identity selects the response to the unexpected stop.
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 8)) {
    reaction = 2;
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 7) ||
             Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 3)) {
    reaction = 1;
  }
  Engine_ActorRunRepeatedMotion(firstRower, 1);
  if (reaction == 1) {
    gEventWork->message += 1;
  } else if (reaction == 2) {
    gEventWork->message += 2;
  }
  Engine_ActorStartRepeatedMotion(firstRower, 2);
  TolbiShipInteriors_Say(firstRower);
  Engine_EventSetMessage(MsgTolbiShipShipWentOff);
  Engine_ActorSetAnimationAndWait(27, 4);
  TolbiShipInteriors_Say(27);
  Actor_ShowEmote(firstRower, 0x102, 0);
  Actor_ShowEmote(secondRower, 0x102, 0);
  Actor_ShowEmote(thirdRower, 0x102, 0);
  Engine_ActorShowEmote(fourthRower, 0x102, 60);
  TolbiShipInteriors_Say(27);
  Actor_WalkToAndWait(27, 408, 132);
  Actor_WalkToAndWait(27, 444, 132);
  Engine_ActorDestroy(27);
  Engine_EventWait(40);
  if (reaction == 0 &&
      (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS) ||
       Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS + 1))) {
    reaction = 3;
  }
  if (reaction == 0) {
    gEventWork->message += 1;
  } else if (reaction == 1) {
    gEventWork->message += 2;
  } else if (reaction == 2) {
    gEventWork->message += 3;
  }
  // Send the rowers and party back through their departure scripts.
  TolbiShipInteriors_Turn(firstRower, 0);
  TolbiShipInteriors_Say(firstRower);
  Actor_SetSpeed(firstRower, PIXELS(1), PIXELS(1) / 2);
  progressB = gTolbiShipInteriors_LeaveByStairs;
  Actor_RunActionScriptAndWait(firstRower, progressB);
  Actor_FaceDirection(secondRower, 0x5000, 0);
  Engine_ActorFaceDirection(thirdRower, FACING_EAST, 0);
  Actor_FaceDirection(fourthRower, FACING_WEST, 40);
  Engine_ActorFaceDirection(secondRower, 0xd000, 0);
  Engine_ActorFaceDirection(thirdRower, 0xb000, 0);
  Engine_ActorFaceDirection(fourthRower, 0x5000, 20);
  Actor_SetSpeed(secondRower, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(thirdRower, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(fourthRower, PIXELS(1), PIXELS(1) / 2);
  Actor_SetActionScript(thirdRower, progressB);
  Engine_EventWait(40);
  progressC = gTolbiShipInteriors_StepIntoAisle;
  Actor_RunActionScriptAndWait(secondRower, progressC);
  Actor_SetActionScript(secondRower, progressB);
  Actor_RunActionScriptAndWait(fourthRower, progressC);
  Actor_RunActionScriptAndWait(fourthRower, progressB);
  Actor_SetSpeed(1, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(2, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(3, PIXELS(1), PIXELS(1) / 2);
  progressA = gTolbiShipInteriors_FollowersRejoinByStairs;
  Actor_SetActionScript(1, progressA);
  Actor_SetActionScript(2, progressA);
  Actor_RunActionScriptAndWait(3, progressA);
  TolbiShipInteriors_RunSceneCommand(SHIP_HIDE_PARTY_FOLLOWERS, 0, 0);
  Engine_GameFlagClear(FLAG_TOLBI_SHIP_ROWING);
  Engine_GameFlagSet(FLAG_TOLBI_SHIP_AT_ISLAND);
  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  Engine_EventEnd();
}

/// Returns the replacement rowers to their seats and exchanges their
/// selections.
static void TolbiShipInteriors_DismissReplacementRowers(void) {
  s32 firstRower;
  s32 fourthRower;
  s32 thirdRower;
  s32 secondRower;

  firstRower = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  secondRower = TolbiShipInteriors_FindFirstSetFlag(1, 0);
  thirdRower = TolbiShipInteriors_FindFirstSetFlag(2, 0);
  fourthRower = TolbiShipInteriors_FindFirstSetFlag(3, 0);
  Engine_EventBegin();
  TolbiShipInteriors_RunSceneCommand(SHIP_SETUP_ROWING_PARTY, 0, 0);
  TolbiShipInteriors_PlaceActor(8, 472, 144, 0x5000);
  TolbiShipInteriors_PlaceActor(27, 408, 142, 0x3000);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(27, 1);
  Engine_EventSetMessage(MsgTolbiShipSorryEveryoneButWeNeed);
  TolbiShipInteriors_Say(27);
  Engine_ActorStartRepeatedMotion(firstRower, 2);
  Engine_ActorStartRepeatedMotion(secondRower, 2);
  Engine_ActorStartRepeatedMotion(thirdRower, 2);
  Engine_ActorRunRepeatedMotion(fourthRower, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(firstRower, FACING_EAST, 0);
  Actor_FaceDirection(secondRower, FACING_WEST, 0);
  Actor_FaceDirection(thirdRower, FACING_EAST, 0);
  Actor_FaceDirection(fourthRower, FACING_WEST, 40);
  Actor_SetSpeed(firstRower, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(secondRower, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(thirdRower, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(fourthRower, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(firstRower, 470, 172);
  Actor_WalkTo(secondRower, 410, 172);
  Actor_WalkTo(thirdRower, 470, 204);
  Actor_WalkToAndWait(fourthRower, 410, 204);
  Engine_ActorSetAnimation(firstRower, 1);
  Engine_ActorSetAnimation(secondRower, 1);
  Engine_ActorSetAnimation(thirdRower, 1);
  Actor_FaceDirection(secondRower, 0xd000, 0);
  Actor_FaceDirection(firstRower, 0xb000, 0);
  Actor_FaceDirection(fourthRower, 0xd000, 0);
  Actor_FaceDirection(thirdRower, 0xb000, 20);
  Engine_ActorRunRepeatedMotion(27, 1);
  TolbiShipInteriors_Say(27);
  Engine_ActorSetAnimation(firstRower, 3);
  Engine_ActorSetAnimation(secondRower, 3);
  Engine_ActorSetAnimation(thirdRower, 3);
  Engine_ActorSetAnimationAndWait(fourthRower, 3);
  TolbiShipInteriors_Say(27);
  Engine_ActorSetAnimation(firstRower, 3);
  Engine_ActorSetAnimation(secondRower, 3);
  Engine_ActorSetAnimation(thirdRower, 3);
  Engine_ActorSetAnimationAndWait(fourthRower, 3);
  Actor_FaceDirection(27, FACING_EAST, 0);
  TolbiShipInteriors_Turn(0, 0x8000);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(27, 3);
  Actor_SetSpeed(27, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(27, 408, 132);
  Actor_WalkToAndWait(27, 444, 132);
  Actor_SetPosition(27, 0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  TolbiShipInteriors_SwapRowerSelections(FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS,
                                         FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS);
  TolbiShipInteriors_SwapRowerSelections(FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS,
                                         FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS);
  GameFlag_Clear(FLAG_TOLBI_SHIP_AT_ISLAND);
  Engine_EventRequestExit(10);
}

/// Exchanges the chosen passengers between two rowing seats.
static void TolbiShipInteriors_SwapRowerSelections(s32 firstSeatFlags,
                                                   s32 secondSeatFlags) {
  s32 firstPassenger = 0;
  s32 secondPassenger = 0;
  u32 i;

  for (i = 0; i <= 8; i++) {
    s32 flag = firstSeatFlags + i;
    if (Engine_GameFlagIsSet(flag) != 0) {
      Engine_GameFlagClear(flag);
      break;
    }
    firstPassenger++;
  }

  for (i = 0; i <= 8; i++) {
    s32 flag = secondSeatFlags + i;
    if (Engine_GameFlagIsSet(flag) != 0) {
      Engine_GameFlagClear(flag);
      break;
    }
    secondPassenger++;
  }

  Engine_GameFlagSet(secondSeatFlags + firstPassenger);
  Engine_GameFlagSet(firstSeatFlags + secondPassenger);
}

/// The step beside the leader for each of the 16 facings: the high 16 bits are
/// x, the low 16 bits z.
#define TURN_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gTolbiShipInteriors_TurnSteps[] = {
    TURN_STEP(16, 0),  TURN_STEP(16, 0),  TURN_STEP(16, 0),  TURN_STEP(0, 16),
    TURN_STEP(0, 16),  TURN_STEP(0, 16),  TURN_STEP(0, 16),  TURN_STEP(-16, 0),
    TURN_STEP(-16, 0), TURN_STEP(-16, 0), TURN_STEP(-16, 0), TURN_STEP(0, -16),
    TURN_STEP(0, -16), TURN_STEP(0, -16), TURN_STEP(0, -16), TURN_STEP(16, 0),
};

/// The warriors walk out of the hold.
s32 gTolbiShipInteriors_LeaveHold[] = {
    SC_ARG(2), SC_MOVE(PIXELS(168), 0, PIXELS(500)),
    SC_SYNC,   SC_MOVE(PIXELS(168), 0, PIXELS(584)),
    SC_SYNC,   SC_CALL(TolbiShipInteriors_ResetActor),
    SC_END,
};

/// Those gathered in the cabin walk out.
s32 gTolbiShipInteriors_LeaveCabin[] = {
    SC_SET(SW_SPEED, PIXELS(1)),
    SC_SET(SW_ACCEL, PIXELS(1) / 2),
    SC_ARG(2),
    SC_MOVE(PIXELS(456), 0, PIXELS(674)),
    SC_SYNC,
    SC_MOVE(PIXELS(456), 0, PIXELS(712)),
    SC_SYNC,
    SC_CALL(TolbiShipInteriors_ResetActor),
    SC_END,
};

/// The Anchor Charm swells and shrinks twice as the captain raises it.
s32 gTolbiShipInteriors_AnchorCharmPulse[] = {
    SC_SET(SW_SCALE_X, 11 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 11 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 9 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_LOOP(2, SC_TOP),
    SC_END,
};

/// The party's followers walk into the leader by the rowing deck's stairs.
s32 gTolbiShipInteriors_FollowersRejoinByStairs[] = {
    SC_ARG(2), SC_MOVE(PIXELS(434), 0, PIXELS(166)),
    SC_SYNC,   SC_CALL(TolbiShipInteriors_ResetActor),
    SC_END,
};

/// The same, in the middle of the rowing deck.
s32 gTolbiShipInteriors_FollowersRejoinMidDeck[] = {
    SC_ARG(2), SC_MOVE(PIXELS(438), 0, PIXELS(300)),
    SC_SYNC,   SC_CALL(TolbiShipInteriors_ResetActor),
    SC_END,
};

/// The same, by the first row of seats.
s32 gTolbiShipInteriors_FollowersRejoinBySeats[] = {
    SC_ARG(2), SC_MOVE(PIXELS(432), 0, PIXELS(168)),
    SC_SYNC,   SC_CALL(TolbiShipInteriors_ResetActor),
    SC_END,
};

/// Squashes and stretches the actor over and over: a shiver.
s32 gTolbiShipInteriors_Shiver[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_ADD(SW_SCALE_X, -0x5000),
    SC_ADD(SW_SCALE_Y, 0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2800),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// A monster in the hold bobs in place.
s32 gTolbiShipInteriors_MonsterBob[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(0x24, 0),
    SC_CALL(TolbiShipInteriors_Bob),
};

/// A rower walks up the stairs and is freed.
s32 gTolbiShipInteriors_LeaveByStairs[] = {
    SC_ARG(2), SC_MOVE(PIXELS(408), 0, PIXELS(132)),
    SC_SYNC,   SC_MOVE(PIXELS(444), 0, PIXELS(132)),
    SC_SYNC,   SC_FREE,
};

/// A rower steps into the aisle.
s32 gTolbiShipInteriors_StepIntoAisle[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(416), 0, PIXELS(186)),
    SC_SYNC,
    SC_END,
};

/// A passenger glances about at random.
s32 gTolbiShipInteriors_LookAroundScript[] = {
    SC_CALL(TolbiShipInteriors_LookAround),
};

/// Lies flat on the floor of the hold.
s32 gTolbiShipInteriors_LieFlatScript[] = {
    SC_CALL(TolbiShipInteriors_LayFlat),
};

/// The Anchor Charm's object, once placed this visit.
struct FieldActor *gTolbiShipInteriors_AnchorObject = 0;

/// The other scenes have no entrances.
struct SceneEntrance gTolbiShipInteriors_EntrancesOther[] = {
    {SCENE_TABLE_END},
};

/// The hold (entrances 1, 2, 11 and 30), a side room (3), the rowing deck (4,
/// 12 and 15 to 24 but 22) and the cabin (5, 10, 13, 14 and 22).
struct SceneEntrance gTolbiShipInteriors_Entrances[] = {
    {1, CONDITION_ALWAYS, 152, 0, 584, FACING_NORTH, 0, 30, 380, 270, 620, 0},
    {2, CONDITION_ALWAYS, 160, 0, 552, FACING_EAST, 0, 30, 380, 270, 620, 0},
    {3, CONDITION_ALWAYS, 120, 0, 136, FACING_WEST, 0, 25, 80, 265, 350, 0},
    {4, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {5, CONDITION_ALWAYS, 440, 0, 728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {10, CONDITION_ALWAYS, 440, 0, 728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {11, CONDITION_ALWAYS, 152, 0, 584, FACING_NORTH, 0, 30, 380, 270, 620, 0},
    {12, CONDITION_ALWAYS, 440, 0, 342, FACING_NORTH, 0, 315, 80, 555, 410, 0},
    {13, CONDITION_ALWAYS, 440, 0, 728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {14, CONDITION_ALWAYS, 440, 0, 728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {15, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {16, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {17, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {18, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {19, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {20, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {21, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 305, 60, 565, 430, 0},
    {22, CONDITION_ALWAYS, 444, 0, 632, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {23, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {24, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {30, CONDITION_ALWAYS, 166, 0, 570, FACING_NORTH, 0, 30, 380, 270, 620, 0},
    {SCENE_TABLE_END},
};

u32 gTolbiShipInteriors_Exits[] = {
    SCENE_EXITS(111),        SCENE_EXIT(1, 109, 1),   SCENE_EXIT(2, 111, 4),
    SCENE_EXIT(3, 109, 2),   SCENE_EXIT(4, 111, 2),   SCENE_EXIT(5, 109, 3),
    SCENE_EXIT(10, 109, 10), SCENE_EXIT(11, 111, 11), SCENE_EXIT(12, 111, 12),
    SCENE_EXIT(13, 2, 65),   SCENE_EXIT(14, 109, 11), SCENE_EXIT(15, 109, 12),
    SCENE_EXIT(16, 2, 66),   SCENE_EXIT(17, 2, 67),   SCENE_EXIT(18, 2, 68),
    SCENE_EXIT(19, 109, 15), SCENE_EXIT(20, 2, 69),   SCENE_EXIT(21, 2, 75),
    SCENE_EXITS_END,
};

struct ScenePlacement gTolbiShipInteriors_PlacementsNone[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gTolbiShipInteriors_CabinPlacements[] = {
    { 0x2e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(446), 0, PIXELS(620), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(472), 0, PIXELS(626), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0x91, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(454), 0, PIXELS(640), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

/// The cabin once the warriors have come to see the captain, with the party's
/// followers.
struct ScenePlacement gTolbiShipInteriors_CabinPlacementsWarriors[] = {
    { 0x2e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(482), 0, PIXELS(604), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(474), 0, PIXELS(624), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x91, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(464), 0, PIXELS(690), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x43, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(648), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x42, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(444), 0, PIXELS(628), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(472), 0, PIXELS(660), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

/// The cabin once the ship has reached Tolbi.
struct ScenePlacement gTolbiShipInteriors_CabinPlacementsArrived[] = {
    { 0x2e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(462), 0, PIXELS(612), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

/// The hold before the party hears of the delay at Kalay.
struct ScenePlacement gTolbiShipInteriors_HoldPlacementsEmpty[] = {
    { 0xf4, CONDITION_ALWAYS, (s32)gTolbiShipInteriors_LieFlatScript,
        PIXELS(150), 0, PIXELS(528), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

/// The hold while the ship waits at Kalay, and after it reaches Tolbi.
struct ScenePlacement gTolbiShipInteriors_HoldPlacements[] = {
    { 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(504), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(540), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(540), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(520), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(130), 0, PIXELS(456), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x43, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(142), 0, PIXELS(446), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x42, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(156), 0, PIXELS(466), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(586), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xf4, CONDITION_ALWAYS, (s32)gTolbiShipInteriors_LieFlatScript,
        PIXELS(150), 0, PIXELS(528), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

/// The hold at sea, once the passengers row.
struct ScenePlacement gTolbiShipInteriors_HoldPlacementsAtSea[] = {
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(138), 0, PIXELS(446), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(532), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x84, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(182), 0, PIXELS(492), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x94, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(492), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(480), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(526), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(172), 0, PIXELS(448), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(180), 0, PIXELS(464), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(122), 0, PIXELS(516), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(586), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xf4, CONDITION_ALWAYS, (s32)gTolbiShipInteriors_LieFlatScript,
        PIXELS(150), 0, PIXELS(528), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

/// The rowing deck: the crew (actors 8 and 9), the oarsmen at the seats (10 to
/// 17), the passengers who may take a seat (18 to 26) and eight actors beside
/// the seats (28 to 35).
struct ScenePlacement gTolbiShipInteriors_RowingDeckPlacements[] = {
    { 0x93, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(438), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x93, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(438), 0, PIXELS(374), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(410), 0, PIXELS(172), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(470), 0, PIXELS(172), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(410), 0, PIXELS(204), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(470), 0, PIXELS(204), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(410), 0, PIXELS(284), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(470), 0, PIXELS(284), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(410), 0, PIXELS(314), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(470), 0, PIXELS(314), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x84, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x94, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(410), 0, PIXELS(174), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(470), 0, PIXELS(174), FACING_WEST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(410), 0, PIXELS(206), FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(470), 0, PIXELS(206), FACING_WEST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(410), 0, PIXELS(286), FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(470), 0, PIXELS(286), FACING_WEST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(410), 0, PIXELS(316), FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(470), 0, PIXELS(316), FACING_WEST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

/// The party's followers, placed for a scene.
struct ScenePlacement gTolbiShipInteriors_PartyFollowerPlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

/// The monsters of each attack, beside the seats.
struct ScenePlacement gTolbiShipInteriors_FirstAttackMonsters[] = {
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(474), 0, PIXELS(300), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(458), 0, PIXELS(310), FACING_WEST, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(474), 0, PIXELS(332), FACING_WEST, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipInteriors_SecondAttackMonsters[] = {
    { 0xc4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(474), 0, PIXELS(300), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xc4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(474), 0, PIXELS(332), FACING_WEST, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(458), 0, PIXELS(310), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipInteriors_ThirdAttackMonsters[] = {
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(474), 0, PIXELS(300), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(474), 0, PIXELS(332), FACING_WEST, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(458), 0, PIXELS(310), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

/// The side room, and the cutscenes.
struct SceneEvent gTolbiShipInteriors_EventsOther[] = {
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { SEARCH(SEARCH_BARREL), 100, 0xf92, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

/// The hold while the ship waits at Kalay, and after it reaches Tolbi.
struct SceneEvent gTolbiShipInteriors_HoldEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TOUCH, 11, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, EVENT_SCRIPT(TolbiShipInteriors_WatchWarriorsStormOff) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToWaitingPassenger8) },
    { EVENT_TALK, 9, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsApparentlyWeCantTakeSilkRoad) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAllThatMoneyOnTicketsAnd) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToWaitingPassenger10) },
    { EVENT_TALK, 11, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsYoureGoingToTolbiToSee) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsINeverThoughtIdGetTo) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouAllAgreedToHelpOf) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsMaybeIShouldJustForgetAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfSomethingHappensToOurOarsmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsTheyreJustTakingAdvantageOfUs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsDoTheyHonestlyThinkImGoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsMusclesRipplingEverywhereLooksGreatIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSureItdBeEasierTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsWeLeftTheOldLadyBack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWellShootMaybeWeShouldHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAskingThePassengersToHelpOut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsEvenThoughImOpposedToViolence) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, FLAG_TOLBI_SHIP_REACHED_TOLBI, MSGNO(MsgTolbiShipInteriorsICantRowButIAm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeMadeItToColossoIn) },
    { SEARCH(SEARCH_BARREL), 101, 0xf93, FIND_COINS(23) },
    { SCENE_EVENTS_END },
};

/// The hold at sea: talking to a passenger asks them to row.
struct SceneEvent gTolbiShipInteriors_HoldEventsChoosingRowers[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIsThisCruelOrdealPartOf) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskPassenger8ToRow) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskPassenger10ToRow) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskPassenger11ToRow) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskPassenger12ToRow) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskPassenger9ToRow) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskPassenger13ToRow) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskPassenger14ToRow) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskPassenger15ToRow) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskPassenger16ToRow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouMakeMeNervousJustStanding) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsHeyLooksLikeTheyNeedSomeone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenInAColdSweat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsRowingIsAToughJobIsnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhThisPoorSapLooksSad) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsHeyThisGuysBeenStaringAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhNoNoWhenOurEyes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsOhOhNoHesNotComing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsItsOnlyNaturalThatINot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsPleasePleaseImBeggingJustDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsISnuckByThisTimeBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsWhatsWithYouGoBotherThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWowThisWarriorIsntGonnaLet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsWhyAmIGettingSoWorked) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCanJustMoveRightAlong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_TOLBI_SHIP_ROWER_CHOSEN, MSGNO(MsgTolbiShipInteriorsWeHadEnoughTroubleJustGetting) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCarefulNowWatchOutDontMake) },
    { SEARCH(SEARCH_BARREL), 101, 0xf93, FIND_COINS(23) },
    { SCENE_EVENTS_END },
};

/// The hold while the ship waits off the island.
struct SceneEvent gTolbiShipInteriors_HoldEventsAtIsland[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsInAPerfectWorldThereWouldnt) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCantGoOntoTheIsland) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsButThereWereMonstersAtSea) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeveBeenCoopedUpOnThat) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThereCouldBeMonstersHereSo) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheShipKeepsShakingSheCant) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyShouldWeHaveToWait) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfThereAreMonstersImNot) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontHearThePassengersScreaming) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_AskPassenger11ToRow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatToDoImWorriedAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsButWhyShouldWeStayOnboard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSickOfThisShipIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLandMonstersICanHandleSea) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenWobblingOnThisBoat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsGoAheadTryAndStopMe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsJustDontMakeMeFaceThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSoAnyoneKnowIfThereAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhhThatsCreepy) },
    { SEARCH(SEARCH_BARREL), 101, 0xf93, FIND_COINS(23) },
    { SCENE_EVENTS_END },
};

/// The cabin, from the wait at Kalay until the ship reaches Tolbi. The Anchor
/// Charm is given back to the captain here.
struct SceneEvent gTolbiShipInteriors_CabinEvents[] = {
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToCaptain) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToCaptainsCabinActor10) },
    { EVENT_TALK, 9, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsIWonderIfThereAreAny) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsRightWhenWeFoundSomeWarriors) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsICantBelieveHimRefusingTo) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIHadNoIdeaHeWas) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToCaptainsCabinActor12) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsThisIsAChanceToMake) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipInteriorsTheShipWontLeaveDockUntil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED, MSGNO(MsgTolbiShipInteriorsAsLongAsWeHaveThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeKnewTheRisksWhenWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsSendingOutAShipWhenIts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWaitedTooLongIllGet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL, MSGNO(MsgTolbiShipInteriorsIfWeDontSetSailNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThisIsRidiculousTheShipCant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThatThingCouldntHaveGottenLost) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontThinkAnyoneWouldSteal) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheShipDoesntLeaveSoon) },
    { EVENT_TOUCH, 10, FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY, EVENT_SCRIPT(TolbiShipInteriors_WatchCrewWonderAtDelay) },
    { USE_ITEM_ON_ACTOR(ITEM_ANCHOR_CHARM), 8, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, EVENT_SCRIPT(TolbiShipInteriors_ReturnLuckyAnchor) },
    { 0xc423, 102, 0xf94, FIND_ITEM(ITEM_NUT) },
    { SCENE_EVENTS_END },
};

/// The cabin while the ship waits off the island.
struct SceneEvent gTolbiShipInteriors_CabinEventsAtIsland[] = {
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeMayHaveTakenTheWrong) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereLookingIntoTheShipsPosition) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImReallySurprisedThatThereIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheWindsDirectionAndTheHigh) },
    { SEARCH(SEARCH_JAR), 102, 0xf94, FIND_ITEM(ITEM_NUT) },
    { SCENE_EVENTS_END },
};

/// The cabin once the ship has reached Tolbi.
struct SceneEvent gTolbiShipInteriors_CabinEventsArrived[] = {
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhenTheWatersAreThisDangerous) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSeeOurShipSurvivedAnAttack) },
    { SEARCH(SEARCH_JAR), 102, 0xf94, FIND_ITEM(ITEM_NUT) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gTolbiShipInteriors_RowingDeckEvents[] = {
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_TALK, 8, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsWeCanLeaveAsSoonAs) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenSailingForManyYears) },
    { EVENT_TALK, 9, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsEveryoneRowsInTimeWithThis) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWowIHaveABlisterOn) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsRowingIsAMansJob) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereTheOnesWhoMakeThe) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOnceWeCastOffYouJust) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCanOnlyDoHardWork) },
    { EVENT_TALK, 14, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsHmmmIWonderWhenWereGoing) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfItsOkayTo) },
    { EVENT_TALK, 15, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsICantWaitUntilWeGet) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsStoppingAgainAndAgainLikeThis) },
    { EVENT_TALK, 16, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsIHearTheresAnIsleIn) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLetsSeeIslandsInTheKaragol) },
    { EVENT_TALK, 17, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsFromTheSeaItLooksLike) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhenWereRowingWeCanJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsEnoughIsEnoughLetsSetOff) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsItsCrazyToHaveInexperiencedPassengers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsILoveItWhenTheOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsItWasHardForMeAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsEveryoneDownHereGetsSoCompetitive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheseBigMusclesAreASign) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWellSeeToItThatThey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouveGotToSaveUpLots) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsIDontFeelLikeRowingAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsISureHopeAnotherMonsterDoesnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsSpendYourMoneyWhileYoureYoung) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLooksLikeImGoingToHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsItSeemsLikeThereWouldBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCouldThisBeThatIslandThats) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipInteriorsHaHahIfYouWantTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsMaybeItsTimeForMeTo) },
    { SCENE_EVENTS_END },
};

/// The rowing deck while monsters are aboard: talking to one (actors 36 to 38)
/// fights it.
struct SceneEvent gTolbiShipInteriors_RowingDeckEventsMonstersAboard[] = {
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TurnBackFromStairs) },
    { EVENT_TALK, 36, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_FightMonstersInHold) },
    { EVENT_TALK, 37, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_FightMonstersInHold) },
    { EVENT_TALK, 38, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_FightMonstersInHold) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsMonstersFromTheDeepAreAttacking) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCurseThoseBeastsTheyGotOne) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFightingOffTheMonstersIsYour) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFoulBeastIMayBeTrembling) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfIWerentSoTiredId) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCursesIWantToSmackThat) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhoIsThatThingGoingTo) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfYouDontDoABetter) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeOarsmenMayBeStrongBut) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereSurroundedByDangerOnAll) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAhhhhSeaMonstersRealLiveSea) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhaHeyYouThisIsNo) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAahIHaveToRowThis) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsHHeyIsaacShouldntYouBe) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYaaahhhIImNotAfraidOf) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheOarsmenAreInDireStraights) },
    { EVENT_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIICantDoThisFighting) },
    { EVENT_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontCareWhatHappensNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyArentYouDoingAnythingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfWeLoseAnyMoreOarsmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheSeaMonstersGetUs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImScaredThoseSeaMonstersAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFightingSeaMonstersIsForWarriors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheSeaMonstersAreSoTough) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontCareWhoElseThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsPleaseJustStayHereAndProtect) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeDontEvenKnowIfAttacking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSeaMonstersAreTerriblyFerocious) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfIWereAWarriorI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfThisWarriorHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereSureInAFixSay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSoOutOfMyWits) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheseSeaMonstersLookAwfullyStrong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsPleaseOhPleaseLetMeMake) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyDidSeaMonstersHaveTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIdRatherBeEatenByA) },
    { SCENE_EVENTS_END },
};

/// The rowing deck while the Kraken rocks the ship.
struct SceneEvent gTolbiShipInteriors_RowingDeckEventsKrakenAboard[] = {
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 19 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImagineTheSizeOfThatBeast) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsHowCanTheseSeaMonstersKeep) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsJudgingFromKajasReactionItMust) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsKajaWouldntAskForHelpUnless) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheWholeShipIsRockingThat) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereOkayWithYouHereWe) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheShipSinksWereFinished) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToRower18) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToRower19) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToRower20) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToRower21) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToRower22) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToRower23) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToRower24) },
    { EVENT_TALK, 25, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_TalkToRower25) },
    { EVENT_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOwwwMyHandsAreRawFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyIsHeDownHereShouldnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatTheHowManyMonstersDoes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatKindOfMonsterIsRocking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereRunningLowOnOarsmenAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImTakingALittleBreakUntil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsGoodThingWeHaveSomeStrong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfTheMonsterIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadRower18Mind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadRower19Mind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadRower20Mind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadRower21Mind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadRower22Mind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadRower23Mind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadRower24Mind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_ReadRower25Mind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfTheyllMakeUs) },
    { SCENE_EVENTS_END },
};

/// The rowing deck once the ship has reached Tolbi.
struct SceneEvent gTolbiShipInteriors_RowingDeckEventsArrived[] = {
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontThinkAnyoneWillBe) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouFolksReallyFoughtWell) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsALotOfOarsmenWereInjured) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAreWeSafeItsAllBecause) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenAnOarsmanAllMy) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouGoOnAheadJustWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeCantFindAnyoneToReplace) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImJustGladTheKrakenDidnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSureThePaysGoodButIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIsaacAndHisPalsWereSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIFeelBetterNowIWas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImInNoRushAnywayThis) },
    { SCENE_EVENTS_END },
};
// clang-format on

/// Frames left of the Kraken's current jolt; at 0 another may start.
s32 gTolbiShipInteriors_ShakeTimer;
