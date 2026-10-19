#include "script.h"
#include "flags.h"
#include "songs.h"
#include "version.h"
// The deck of the ship across the Karagol Sea, from Kalay to Tolbi: the wait
// at the Kalay dock while the replacement ship fails to come, setting sail with
// the passengers at the oars, the sea monsters and the Kraken, the arrival at
// Tolbi, and the stop at anchor off the island. Entrances 10 to 19 chain the
// voyage's cutscenes, each leaving by an exit that comes back to the next.

#include "game_state.h"
#include "unit/scene/tolbi_ship_deck.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "call.h"
#include "field_service.h"
#include "global_cells.h"
#include "edition.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.

/// "There's a huge sea monster on the ship!": the deck shakes until the
/// Kraken is defeated (set in tolbi_ship_interiors.c).
#define FLAG_TOLBI_SHIP_KRAKEN_ABOARD 0x302

// Defined below, in the order of the ROM.
static void TolbiShipDeck_ConfrontCrewman(void);
static void TolbiShipDeck_ApplyEntryState(void);
static void TolbiShipDeck_PlaceWaitingPassengers(void);
static void TolbiShipDeck_PlacePassengersAgreedToRow(void);
static void TolbiShipDeck_SetUpUnderSail(void);
static void TolbiShipDeck_SetUpAfterKraken(void);
static void TolbiShipDeck_SetUpAtAnchor(void);
static void TolbiShipDeck_DepartFromKalay(void);
static void TolbiShipDeck_DepartWithParty(void);
static void TolbiShipDeck_AnnouncePreparationsReady(s32 a0, s32 a1);
static void TolbiShipDeck_SightSeaMonsters(void);
static void TolbiShipDeck_MonstersBoard(void);
static void TolbiShipDeck_MonstersAttackCrew(void);
static void TolbiShipDeck_MonsterLandsOnDeck(void);
static void TolbiShipDeck_JumpIntoKrakenBattle(void);
static void TolbiShipDeck_ThankForKraken(void);
static void TolbiShipDeck_SightLand(void);
static void TolbiShipDeck_RowersDisembark(void);
static s32 TolbiShipDeck_GetRowerAtSeat(u32 seat);
static void TolbiShipDeck_RockDeck(void);
static void TolbiShipDeck_SetUpOars(void);
static void TolbiShipDeck_ShowOar(s32 a);
static void TolbiShipDeck_ChooseOarStrokes(void);
static s32 TolbiShipDeck_GetSeatStroke(u32 seat);
static void TolbiShipDeck_RowOars(void);
static void TolbiShipDeck_AdvanceStroke(s32 actor, s32 slot);
static void TolbiShipDeck_SwingOar(s32 id, s32 slot, s32 flags);
static void TolbiShipDeck_RestOars(void);
static void TolbiShipDeck_CopyUnderwayCells(void);
static void TolbiShipDeck_CopyAnchoredCells(void);
static void TolbiShipDeck_Say(s32 a);
static void TolbiShipDeck_Turn(s32 a, s32 b);
static void TolbiShipDeck_ArriveAtTolbi(void);
static void TolbiShipDeck_ReturnFromIsland(void);
static void TolbiShipDeck_PlaceLeaderOnDeck(void);

s32 Math_RemainderUnsigned(s32, s16);

/// A speaker: the actor, and in the top four bits where the message window
/// opens beside them (4, SPEAKER_WINDOW_ABOVE, is above).
#define SPEAKER(actor, window) ((window) << 12 | (actor))

/// Who is who on deck. At Kalay (gTolbiShipDeckPlacementsWaiting) the captain
/// asks Kaja, the deck's fighter, how the oarsmen are ("Say, Kaja, how are the
/// oarsmen doing?"). At sea gTolbiShipDeck_CrewScript places Kaja and the
/// warriors Ouranos and Sean, who help him guard the deck ("That'll be me,
/// Ouranos, and Sean on deck").
enum TolbiShipDeckActor {
  KALAY_CREWMAN = 20, ///< denies everything. "I didn't do anything..."
  KALAY_CAPTAIN = 22,
  KALAY_KAJA = 23,
  SEA_KAJA = 20,
  SEA_OURANOS = 21,
  SEA_SEAN = 22,
};

// Shared code, included where it sits in the ROM.
#include "common/object_pulse.h"

/// An action-script call: shows the actor's sprite. Returns 0 to end the call.
static s32 TolbiShipDeck_ShowSprite(struct ScriptInterpreter *interpreter) {
  struct FieldActor *a = (struct FieldActor *)interpreter;
  Engine_ActorSetSpriteFlags(a, 1);
  return 0;
}

/// A diving monster keeps its state in spare words of its field actor.
struct DivingMonster {
  u8 fieldActor[0x4c];
  s32 drift; ///< sideways drift per frame
  u8 unknown50[0x12];
  u8 step;      ///< 0 hovering, 1 to 8 diving and returning
  u8 rightSide; ///< returns to the right-hand spot
  s16 driftingLeft;
  s16 sinking;
};

#define DIVER(obj) ((struct DivingMonster *)(obj))

/// The update of a sea monster hovering by Ouranos's side of the deck while the
/// monsters attack. It drifts and bobs at random; one frame in a hundred it
/// dives at Ouranos, who turns and jumps (sometimes with an emote), then it
/// returns to its side. DivingMonster.rightSide picks which side it returns to.
/// Returns 1 to keep running.
static s32
TolbiShipDeck_HoverAndDiveAtOuranos(struct ScriptInterpreter *interpreter) {
  struct FieldActor *obj = (struct FieldActor *)interpreter;
  if (DIVER(obj)->step != 0) {
    switch (DIVER(obj)->step) {
    case 1:
      obj->speed = 0x40000;
      obj->acceleration = 0x20000;
      FieldObject_SetPosition((s32)obj, PIXELS(268), PIXELS(20), PIXELS(692));
      DIVER(obj)->step++;
      break;
    case 2:
      DIVER(obj)->step++;
      break;
    case 3:
      if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
          obj->targetZ == obj->targetY) {
        DIVER(obj)->step++;
        Event_PlayCue(SE_146);
        if (DIVER(obj)->rightSide != 0)
          Actor_FaceDirection(SEA_OURANOS, 0xd000, 0);
        else
          Actor_FaceDirection(SEA_OURANOS, 0xb000, 0);
        if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
          Actor_Get(SEA_OURANOS)->velocityY = 0x20000;
        } else {
          Actor_ShowEmote(SEA_OURANOS, 0x103, 0);
          Actor_Get(SEA_OURANOS)->velocityY = 0x60000;
        }
      }
      break;
    case 4:
      DIVER(obj)->step++;
      break;
    case 5:
      if (DIVER(obj)->rightSide != 0)
        FieldObject_SetPosition((s32)obj, PIXELS(282), 0, PIXELS(658));
      else
        FieldObject_SetPosition((s32)obj, PIXELS(254), 0, PIXELS(668));
      DIVER(obj)->step++;
      break;
    case 6:
      DIVER(obj)->step++;
      break;
    case 7:
      if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
          obj->targetZ == obj->targetY) {
        obj->speed = 0x20000;
        obj->acceleration = 0x10000;
        DIVER(obj)->driftingLeft = 0;
        DIVER(obj)->sinking = 0;
        DIVER(obj)->step++;
        DIVER(obj)->drift = 0;
      }
      break;
    case 8:
      DIVER(obj)->step = 0;
      break;
    }
  } else {
    if (DIVER(obj)->driftingLeft != 0) {
      DIVER(obj)->drift -= ((u32)Engine_RandomNext() << 12) >> 16;
      if (DIVER(obj)->drift < -0x4000)
        DIVER(obj)->driftingLeft = 0;
    } else {
      DIVER(obj)->drift += ((u32)Engine_RandomNext() << 12) >> 16;
      if (DIVER(obj)->drift > 0x4000)
        DIVER(obj)->driftingLeft = 1;
    }
    if (obj->x.fixed > 0xf80000 && obj->x.fixed < 0x1240000)
      obj->x.fixed += DIVER(obj)->drift;
    if (DIVER(obj)->sinking != 0) {
      obj->y.fixed =
          obj->y.fixed - (((u32)Engine_RandomNext() << 15) >> 16) - 0x8000;
      if (obj->y.fixed < 0)
        DIVER(obj)->sinking = 0;
    } else {
      obj->y.fixed =
          obj->y.fixed + (((u32)Engine_RandomNext() << 15) >> 16) + 0x8000;
      if (obj->y.fixed > 0x80000)
        DIVER(obj)->sinking = 1;
    }
  }
  if (((u32)Engine_RandomNext() * 100) >> 16 == 0)
    DIVER(obj)->step = 1;
  return 1;
}

/// The same for the monster that dives at Sean.
static s32
TolbiShipDeck_HoverAndDiveAtSean(struct ScriptInterpreter *interpreter) {
  struct FieldActor *obj = (struct FieldActor *)interpreter;
  if (DIVER(obj)->step != 0) {
    switch (DIVER(obj)->step) {
    case 1:
      obj->speed = 0x40000;
      obj->acceleration = 0x20000;
      FieldObject_SetPosition((s32)obj, PIXELS(256), PIXELS(20), PIXELS(640));
      DIVER(obj)->step++;
      break;
    case 2:
      DIVER(obj)->step++;
      break;
    case 3:
      if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
          obj->targetZ == obj->targetY) {
        DIVER(obj)->step++;
        Event_PlayCue(SE_146);
        if (DIVER(obj)->rightSide != 0)
          Actor_FaceDirection(SEA_SEAN, 0xd000, 0);
        else
          Actor_FaceDirection(SEA_SEAN, 0xb000, 0);
        if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
          Actor_Get(SEA_SEAN)->velocityY = 0x20000;
        } else {
          Actor_ShowEmote(SEA_SEAN, 0x103, 0);
          Actor_Get(SEA_SEAN)->velocityY = 0x60000;
        }
      }
      break;
    case 4:
      DIVER(obj)->step++;
      break;
    case 5:
      if (DIVER(obj)->rightSide != 0)
        FieldObject_SetPosition((s32)obj, PIXELS(264), 0, PIXELS(600));
      else
        FieldObject_SetPosition((s32)obj, PIXELS(242), 0, PIXELS(604));
      DIVER(obj)->step++;
      break;
    case 6:
      DIVER(obj)->step++;
      break;
    case 7:
      if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
          obj->targetZ == obj->targetY) {
        obj->speed = 0x20000;
        obj->acceleration = 0x10000;
        DIVER(obj)->driftingLeft = 0;
        DIVER(obj)->sinking = 0;
        DIVER(obj)->step++;
        DIVER(obj)->drift = 0;
      }
      break;
    case 8:
      DIVER(obj)->step = 0;
      break;
    }
  } else {
    if (DIVER(obj)->driftingLeft != 0) {
      DIVER(obj)->drift -= ((u32)Engine_RandomNext() << 12) >> 16;
      if (DIVER(obj)->drift < -0x4000)
        DIVER(obj)->driftingLeft = 0;
    } else {
      DIVER(obj)->drift += ((u32)Engine_RandomNext() << 12) >> 16;
      if (DIVER(obj)->drift > 0x4000)
        DIVER(obj)->driftingLeft = 1;
    }
    if (obj->x.fixed > 0xe80000 && obj->x.fixed < 0x1100000)
      obj->x.fixed += DIVER(obj)->drift;
    if (DIVER(obj)->sinking != 0) {
      obj->y.fixed =
          obj->y.fixed - (((u32)Engine_RandomNext() << 15) >> 16) - 0x8000;
      if (obj->y.fixed < 0)
        DIVER(obj)->sinking = 0;
    } else {
      obj->y.fixed =
          obj->y.fixed + (((u32)Engine_RandomNext() << 15) >> 16) + 0x8000;
      if (obj->y.fixed > 0x80000)
        DIVER(obj)->sinking = 1;
    }
  }
  if (((u32)Engine_RandomNext() * 100) >> 16 == 0)
    DIVER(obj)->step = 1;
  return 1;
}

struct MapLayer {
  u8 unk00[8];
  s32 x;
  s32 y;
  u8 unk10[32];
};

struct MapWork {
  s32 *camera;
  u8 unk04[16];
  struct MapLayer layers[8];
};

static u16 sStrokePhase[4];

static s32 sSwayAngleY;
static s32 sUnused0; ///< never used; keeps the statics' layout

static u16 sOarAngle[8];

static s32 sSeaScroll[2];

static s32 sSwayAngleX;
static s32 sUnused1; ///< never used; keeps the statics' layout

static s32 sSeaSpeed[2];

// The ROM places these after the file's other statics, the strokes first, on
// the 16-byte boundary the assembler gives a 32-byte common symbol.
static u32 sOarStroke[8] __attribute__((aligned(16)));
static s32 sOarDepth[8];

/// Bobs the actor between 4 and 12 pixels high by a random amount each frame;
/// the actor's work halfword says which way it is going. Returns 1.
static s32 TolbiShipDeck_Bob(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  s16 *flag = (s16 *)&actor->work;
  s32 val;
  s32 tmp;

  if (*flag != 0) {
    val = actor->y.fixed - (((u32)(Engine_RandomNext() << 15)) >> 16) - 0x8000;
    actor->y.fixed = val;
    if (val >= 0x40000)
      goto done;
    tmp = 0;
  } else {
    val = actor->y.fixed + (((u32)(Engine_RandomNext() << 15)) >> 16) + 0x8000;
    actor->y.fixed = val;
    if (val <= 0xC0000)
      goto done;
    tmp = 1;
  }
  *flag = tmp;
done:
  return 1;
}

/// Now and then (two chances in 32) turns the actor to look north-northeast or
/// north-northwest. Returns 1.
static s32 TolbiShipDeck_LookAround(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  u32 roll = ((u32)(Engine_RandomNext() << 5)) >> 16;

  if (roll == 6)
    actor->facing = 0xd000;
  else if (roll == 9)
    actor->facing = 0xb000;
  return 1;
}

/// The update of a monster on deck that, at random, leaps at Ouranos: it jumps
/// to him, he turns and jumps back (sometimes with an emote), and it lands on
/// one of two spots chosen by riseEnabled. The actor's timer holds the step.
/// Returns 1.
static s32 TolbiShipDeck_LeapAtOuranos(struct ScriptInterpreter *interpreter) {
  struct FieldActor *obj = (struct FieldActor *)interpreter;
  switch (obj->timer) {
  case 0:
    if ((((u32)Engine_RandomNext() * 40) >> 16) == 0) {
      obj->timer++;
    }
    break;
  case 1:
    obj->timer++;
    break;
  case 2:
    obj->velocityY = 0x40000;
    obj->speed = 0x40000;
    obj->acceleration = 0x20000;
    FieldObject_SetPosition((s32)obj, PIXELS(264), 0, PIXELS(662));
    obj->timer++;
    break;
  case 3:
    obj->timer++;
    break;
  case 4:
    if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
        obj->targetZ == obj->targetY) {
      obj->timer++;
      Engine_AudioPlayCue(SE_JUMP);
      if (obj->riseEnabled != 0) {
        Actor_FaceDirection(SEA_OURANOS, 0xb000, 0);
      } else {
        Actor_FaceDirection(SEA_OURANOS, 0x5000, 0);
      }
      if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
        Object_GetById(SEA_OURANOS)->velocityY = 0x20000;
      } else {
        Actor_ShowEmote(SEA_OURANOS, 0x103, 0);
        Object_GetById(SEA_OURANOS)->velocityY = 0x60000;
      }
    }
    break;
  case 5:
    obj->timer++;
    break;
  case 6:
    obj->timer++;
    obj->velocityY = 0x40000;
    obj->speed = 0x20000;
    obj->acceleration = 0x10000;
    if (obj->riseEnabled != 0) {
      FieldObject_SetPosition((s32)obj, PIXELS(252), 0, PIXELS(646));
    } else {
      FieldObject_SetPosition((s32)obj, PIXELS(256), 0, PIXELS(686));
    }
    break;
  case 7:
    obj->timer++;
    break;
  case 8:
    if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
        obj->targetZ == obj->targetY) {
      obj->timer++;
    }
    break;
  case 9:
    obj->timer = 0;
    break;
  }
  return 1;
}

/// The same for the monster that leaps at Sean.
static s32 TolbiShipDeck_LeapAtSean(struct ScriptInterpreter *interpreter) {
  struct FieldActor *obj = (struct FieldActor *)interpreter;
  switch (obj->timer) {
  case 0:
    if ((((u32)Engine_RandomNext() * 40) >> 16) == 0) {
      obj->timer++;
    }
    break;
  case 1:
    obj->timer++;
    break;
  case 2:
    obj->velocityY = 0x40000;
    obj->speed = 0x40000;
    obj->acceleration = 0x20000;
    FieldObject_SetPosition((s32)obj, PIXELS(176), 0, PIXELS(696));
    obj->timer++;
    break;
  case 3:
    obj->timer++;
    break;
  case 4:
    if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
        obj->targetZ == obj->targetY) {
      obj->timer++;
      Engine_AudioPlayCue(SE_JUMP);
      if (obj->riseEnabled != 0) {
        Actor_FaceDirection(SEA_SEAN, 0xd000, 0);
      } else {
        Engine_ActorFaceDirection(SEA_SEAN, FACING_EAST, 0);
      }
      if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
        Object_GetById(SEA_SEAN)->velocityY = 0x20000;
      } else {
        Actor_ShowEmote(SEA_SEAN, 0x103, 0);
        Object_GetById(SEA_SEAN)->velocityY = 0x60000;
      }
    }
    break;
  case 5:
    obj->timer++;
    break;
  case 6:
    obj->timer++;
    obj->velocityY = 0x40000;
    obj->speed = 0x20000;
    obj->acceleration = 0x10000;
    if (obj->riseEnabled != 0) {
      FieldObject_SetPosition((s32)obj, PIXELS(184), 0, PIXELS(672));
    } else {
      FieldObject_SetPosition((s32)obj, PIXELS(202), 0, PIXELS(692));
    }
    break;
  case 7:
    obj->timer++;
    break;
  case 8:
    if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX &&
        obj->targetZ == obj->targetY) {
      obj->timer++;
    }
    break;
  case 9:
    obj->timer = 0;
    break;
  }
  return 1;
}

/// Makes the actor hop at random, one frame in 40 on average. Returns 1.
static s32
TolbiShipDeck_RandomlyStartActorHop(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  if ((((u32)(Engine_RandomNext() * 40)) >> 16) == 0)
    actor->velocityY = 0x40000;
  return 1;
}

/// The update that shrinks an enlarged actor by 1/32 a frame until it is back
/// to normal size.
static void TolbiShipDeck_ShrinkToNormalSize(struct FieldActor *actor) {
  if (actor->scaleX > 0x10000) {
    actor->scaleX += 0xFFFFF800;
    actor->scaleY += 0xFFFFF800;
  }
}

/// Turns the actor east, northeast-by-north or southeast-by-south at random
/// intervals of 80 to 160 frames. riseCounter counts down the wait. Returns 1.
static s32 TolbiShipDeck_IdleTurn(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  if (actor->riseCounter != 0) {
    actor->riseCounter--;
  } else {
    u32 roll = (u32)(Engine_RandomNext() * 300) >> 16;

    if (roll > 200) {
      actor->facing = 0xd000;
    } else if (roll > 100) {
      actor->facing = 0x5000;
    } else {
      actor->facing = 0;
    }
    actor->riseCounter = ((u32)(Engine_RandomNext() * 80) >> 16) + 80;
  }
  return 1;
}

u8 *TolbiShipDeck_GetEntrances(void) { return gTolbiShipDeckEntrances; }

u8 *TolbiShipDeck_GetRegions(void) { return gTolbiShipDeckRegions; }

u8 *TolbiShipDeck_GetExits(void) { return gTolbiShipDeckExits; }

u8 *TolbiShipDeck_GetPlacements(void) {
  s32 afterFirstAttack;

  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI))
    return gTolbiShipDeckPlacementsArrived;
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWING))
    return gTolbiShipDeckPlacementsRowing;
  afterFirstAttack =
      Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED);
  if (afterFirstAttack != 0)
    return gTolbiShipDeckPlacementsAtSea;
  if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT)) {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED)) {
      STORE_CONST(gTolbiShipDeckPlacementsWaiting[13].talkFacing) =
          afterFirstAttack;
      STORE_CONST(gTolbiShipDeckPlacementsWaiting[17].talkFacing) = 2;
      STORE_CONST(gTolbiShipDeckPlacementsWaiting[18].talkFacing) = 2;
    } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN)) {
      STORE_CONST(gTolbiShipDeckPlacementsWaiting[17].talkFacing) = 1;
      STORE_CONST(gTolbiShipDeckPlacementsWaiting[18].talkFacing) = 1;
    }
    return gTolbiShipDeckPlacementsWaiting;
  }
  return gTolbiShipDeckPlacements;
}

u8 *TolbiShipDeck_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI))
    return gTolbiShipDeckEventsArrived;
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND))
    return gTolbiShipDeckEventsAtIsland;
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED))
    return gTolbiShipDeckEventsAtSea;
  return gTolbiShipDeckEvents;
}

/// A passenger at Kalay. What he says follows the delay: impatient at first,
/// alarmed once the warriors storm off to the captain, relieved
/// once the party has agreed to protect the ship. While alarmed he
/// paces at random intervals.
static void TolbiShipDeck_TalkToWaitingPassenger21(void) {
  struct FieldActor *passenger;
  s32 random;
  s32 wait;

  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Engine_EventSetMessage(MsgTolbiShipNowWeHaveProtectShip);
    Event_ShowMessage(21, 0);
  } else {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
      Engine_ActorRunRepeatedMotion(21, 2);
      Engine_EventSetMessage(MsgTolbiShipSomebodyStopThem);
      Event_ShowMessage(21, 0);
      passenger = Object_GetById(21);
      random = Random_Next();
      wait = ((u32)(90 * random) >> 16) + 60;
      passenger->timer = wait;
      Actor_SetActionScript(21, gTolbiShipDeck_RandomActorActions);
    } else {
      Actor_ShowEmote(21, 0x103, 0);
      Engine_ActorStartRepeatedMotion(21, 3);
      Engine_EventSetMessage(MsgTolbiShipToldWereLeavingSoonSet);
      Event_ShowMessage(21, 0);
    }
  }
  Engine_EventEnd();
}

/// Another waiting passenger, with the same three moods as actor 21.
static void TolbiShipDeck_TalkToWaitingPassenger24(void) {
  struct FieldActor *passenger;
  s32 random;
  s32 wait;

  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    Engine_EventSetMessage(MsgTolbiShipHaveMakeThemPromiseHelp);
    Event_ShowMessage(24, 0);
  } else {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
      Engine_ActorRunRepeatedMotion(24, 2);
      Engine_EventSetMessage(MsgTolbiShipTheyCantPlanningMutiny);
      Event_ShowMessage(24, 0);
      passenger = Object_GetById(24);
      random = Random_Next();
      wait = ((u32)(90 * random) >> 16) + 60;
      passenger->timer = wait;
      Actor_SetActionScript(24, gTolbiShipDeck_RandomActorActions);
    } else {
      Actor_ShowEmote(24, 0x103, 0);
      Engine_ActorStartRepeatedMotion(24, 3);
      Engine_EventSetMessage(MsgTolbiShipIfWeDontLeaveSoon);
      Event_ShowMessage(24, 0);
    }
  }
  Engine_EventEnd();
}

/// A passenger asks whether the party is headed for Colosso too.
static void TolbiShipDeck_AskHeadedForColosso(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiShipHeadedColosso);
  Engine_EventAskYesNo(21, 0);
  Engine_EventEnd();
}

/// Opens the first door to below deck, once a visit.
static void TolbiShipDeck_OpenFirstDoor(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_71) == 0) {
    Event_PlayCue(SE_DOOR_OPEN);
    Map_CopyCellsTo(30, 94, 13, 94, 1, 3);
    GameFlag_Set(FLAG_SCENE_TEMP_71);
  }
}

/// Opens the second door to below deck, once a visit.
static void TolbiShipDeck_OpenSecondDoor(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_72) == 0) {
    Event_PlayCue(SE_DOOR_OPEN);
    Map_CopyCellsTo(30, 108, 13, 108, 1, 2);
    GameFlag_Set(FLAG_SCENE_TEMP_72);
  }
}

/// Walking into one of the ways below deck (triggers 1 to 3). The doors at 1
/// and 3 open and the leader steps through; the hatch at 2 just plays its
/// sound. Leaves by the exit numbered as the trigger.
static void TolbiShipDeck_GoBelow(void) {

  s32 throughDoor;
  struct EventWork *work;

  work = gEventWork;
  Engine_EventBegin();
  throughDoor = 0;
  switch (work->touchedTrigger) {
  case 1:
    throughDoor = 1;
    TolbiShipDeck_OpenFirstDoor();
    break;
  case 3:
    throughDoor = 1;
    TolbiShipDeck_OpenSecondDoor();
    break;
  }
  if (throughDoor != 0) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    FieldObjectMotion_CenterOnColumn(0, 1, -10);
    Engine_EventWait(10);
  } else {
    Engine_AudioPlayCue(SE_MAP_EXIT);
  }
  Engine_EventRequestExit(work->touchedTrigger);
}

#if TBS_PATCH_2
/// In the German, Spanish, French and Italian editions trigger 2 is a plain
/// touch: the leader goes below only while facing west, after walking onto the
/// hatch.
static void TolbiShipDeck_GoBelowWhileFacingWest(void) {
  if ((u16)(Object_GetById(ACTOR_PARTY_LEADER)->facing - 0x6001) <= 0x3ffe) {
    Engine_EventBegin();
    Actor_WalkTo(ACTOR_PARTY_LEADER, 214, 664);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    TolbiShipDeck_GoBelow();
  }
}
#endif

/// At Kalay, once the party has heard of the delay (flag 0x911): the crew
/// wonder what happened to the replacement ship and decide to set sail anyway;
/// the crewman runs off through the second door. Plays once.
static void TolbiShipDeck_WatchCrewDecideToSail(void) {
  struct FieldActor *record;

  if (GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    Engine_EventBegin();
    Battle_ResetEffectCounter();
    Actor_FaceActor(ACTOR_PARTY_LEADER, KALAY_CREWMAN, 10);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(PIXELS(190), -1, PIXELS(708), 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(KALAY_CAPTAIN, 1);
    Engine_EventSetMessage(MsgTolbiShipWonderCouldHaveHappened);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    Actor_ShowEmote(KALAY_CREWMAN, 0x102, 60);
    Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(KALAY_CREWMAN);
    Engine_ActorRunRepeatedMotion(KALAY_CAPTAIN, 1);
    Actor_FaceDirection(KALAY_CAPTAIN, 0x5000, 0);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    Engine_ActorRunRepeatedMotion(KALAY_CREWMAN, 1);
    TolbiShipDeck_Turn(KALAY_CREWMAN, 0xb000);
    TolbiShipDeck_Say(KALAY_CREWMAN);
    TolbiShipDeck_Turn(KALAY_KAJA, 0x3000);
    Engine_ActorSetAnimation(KALAY_KAJA, 3);
    TolbiShipDeck_Say(SPEAKER(KALAY_KAJA, 4));
    Actor_ShowEmote(KALAY_CAPTAIN, 0x101, 40);
    Actor_FaceDirection(KALAY_CAPTAIN, FACING_WEST, 20);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    TolbiShipDeck_Turn(KALAY_KAJA, FACING_EAST);
    Engine_ActorSetAnimationAndWait(KALAY_KAJA, 4);
    TolbiShipDeck_Say(SPEAKER(KALAY_KAJA, 4));
    Actor_ShowEmote(KALAY_CREWMAN, 0x100, 40);
    Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(KALAY_CREWMAN);
    Engine_ActorSetAnimationAndWait(KALAY_CAPTAIN, 3);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    TolbiShipDeck_Turn(KALAY_CREWMAN, 0xd000);
    Engine_ActorSetAnimation(KALAY_KAJA, 3);
    Engine_ActorSetAnimationAndWait(KALAY_CREWMAN, 3);
    Engine_EventWait(60);
    Actor_ShowEmote(KALAY_CAPTAIN, 0x106, 40);
    TolbiShipDeck_Turn(KALAY_CAPTAIN, 0x5000);
    Engine_EventSetMessage(MsgTolbiShipTheresNothingWeCanDo);
    Engine_ActorStartRepeatedMotion(KALAY_CAPTAIN, 1);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    Actor_ShowEmote(KALAY_CREWMAN, 0x101, 40);
    Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(KALAY_CREWMAN);
    Actor_ShowEmote(KALAY_CAPTAIN, 0x108, 20);
    Event_ShowMessageAndWait(SPEAKER(KALAY_CAPTAIN, 4), 0, 20);
    Actor_ShowEmote(KALAY_KAJA, 0x102, 60);
    TolbiShipDeck_Say(SPEAKER(KALAY_KAJA, 4));
    TolbiShipDeck_Turn(KALAY_CAPTAIN, FACING_WEST);
    Engine_ActorSetAnimationAndWait(KALAY_CAPTAIN, 3);
    Event_ShowMessageAndWait(SPEAKER(KALAY_CAPTAIN, 4), 0, 20);
    Actor_ShowEmote(KALAY_CREWMAN, 0x102, 40);
    Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
    TolbiShipDeck_Say(KALAY_CREWMAN);
    TolbiShipDeck_Turn(KALAY_CAPTAIN, 0x5000);
    Engine_ActorSetAnimation(KALAY_CAPTAIN, 4);
    TolbiShipDeck_Say(KALAY_CAPTAIN);
    Actor_FaceDirection(KALAY_CREWMAN, 0xb000, 0);
    Actor_FaceDirection(KALAY_KAJA, 0x3000, 40);
    Actor_FaceDirection(KALAY_KAJA, FACING_EAST, 0);
    Actor_FaceDirection(KALAY_CREWMAN, 0xd000, 20);
    Engine_ActorRunRepeatedMotion(KALAY_CAPTAIN, 2);
    Engine_EventWait(20);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    Actor_SetAttachedEffect(KALAY_KAJA, 0x102);
    Actor_SetAttachedEffect(KALAY_CREWMAN, 0x102);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(KALAY_CAPTAIN, 3);
    TolbiShipDeck_Say(SPEAKER(KALAY_CAPTAIN, 4));
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(PIXELS(182), -1, PIXELS(760), 1);
    Actor_SetSpeed(KALAY_KAJA, 0xcccc, 0x6666);
    Actor_SetActionScript(KALAY_KAJA, gTolbiShipDeck_PresentationActionsA);
    Actor_SetSpeed(KALAY_CAPTAIN, 0xcccc, 0x6666);
    Actor_SetActionScript(KALAY_CAPTAIN, gTolbiShipDeck_PresentationActionsB);
    Actor_SetSpeed(KALAY_CREWMAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(KALAY_CREWMAN, 182, 0x2f8);
    Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
    Actor_ShowEmote(KALAY_CREWMAN, 0x100, 60);
    TolbiShipDeck_Turn(KALAY_CREWMAN, 0xd000);
    Event_ShowMessageAndWait(KALAY_CREWMAN, 0, 20);
    Engine_ActorSetAnimationAndWait(KALAY_CREWMAN, 3);
    Engine_ActorJump(KALAY_CREWMAN, 4, 0);
    Actor_FaceDirection(KALAY_CREWMAN, 0x3000, 40);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(PIXELS(216), -1, PIXELS(790), 1);
    {
      struct FieldActor *record = Object_GetById(KALAY_CREWMAN);
      record->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | record->priorityFlags;
    }
    Actor_SetSpeed(KALAY_CREWMAN, 0x13333, 0x9999);
    Actor_WalkToAndWait(KALAY_CREWMAN, 182, 0x30e);
    Actor_WalkToAndWait(KALAY_CREWMAN, 192, 0x328);
    Actor_WalkToAndWait(KALAY_CREWMAN, 216, 0x328);
    TolbiShipDeck_Turn(KALAY_CREWMAN, 0xd000);
    Engine_ActorRunRepeatedMotion(KALAY_CREWMAN, 2);
    TolbiShipDeck_OpenSecondDoor();
    Actor_WalkToAndWait(KALAY_CREWMAN, 216, 0x31e);
    Actor_SetPosition(KALAY_CREWMAN, 0, 0);
    record = Object_GetById(KALAY_CREWMAN);
    record->scaleX = 0x10000;
    record = Object_GetById(KALAY_CREWMAN);
    record->scaleY = 0x10000;
    GameFlag_Set(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY);
    Engine_EventEnd();
  }
}

/// Approaching from the east side of the door: the crewman comes out, protests
/// and slips away round the party to the bow. "I didn't do anything..." Runs
/// until the party has confronted the crewman.
static void TolbiShipDeck_CatchCrewmanFromEast(void) {
  u8 bits;

  if (GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0) {
      Engine_EventBegin();
      Battle_ResetEffectCounter();
      TolbiShipDeck_ConfrontCrewman();
      Actor_SetSpeed(KALAY_CREWMAN, 0x6666, 0x3333);
      Object_GetById(KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(KALAY_CREWMAN, 232, 0x330);
      Engine_EventWait(1);
      bits = 1;
      {
        u8 *record = (s32)Object_GetById(KALAY_CREWMAN);
        u8 value = record[90];

        record[90] = value | bits;
      }
      Engine_EventWait(20);
      Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
      TolbiShipDeck_Say(KALAY_CREWMAN);
      Actor_SetSpeed(KALAY_CREWMAN, 0x13333, 0x9999);
      Object_GetById(KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(KALAY_CREWMAN, 244, 0x324);
      Engine_EventWait(1);
      {
        struct FieldActor *record = Object_GetById(KALAY_CREWMAN);

        bits |= record->steeringFlags;
        record->steeringFlags = bits;
      }
      Engine_EventWait(20);
      Actor_SetSpeed(KALAY_CREWMAN, 0x33333, 0x19999);
      Actor_WalkToAndWait(KALAY_CREWMAN, 248, 0x30a);
      Actor_WalkToAndWait(KALAY_CREWMAN, 248, 0x2bc);
      Actor_SetPosition(KALAY_CREWMAN, PIXELS(246), PIXELS(512));
      Actor_FaceDirection(KALAY_CREWMAN, FACING_EAST, 0);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
      Engine_EventEnd();
    }
  }
}

/// The same, approaching from the west side.
static void TolbiShipDeck_CatchCrewmanFromWest(void) {
  u8 bits;

  if (GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    if (GameFlag_IsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) == 0) {
      Engine_EventBegin();
      Battle_ResetEffectCounter();
      TolbiShipDeck_ConfrontCrewman();
      Actor_SetSpeed(KALAY_CREWMAN, 0x6666, 0x3333);
      Object_GetById(KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(KALAY_CREWMAN, 202, 0x330);
      Engine_EventWait(1);
      bits = 1;
      {
        u8 *record = (s32)Object_GetById(KALAY_CREWMAN);
        u8 value = record[90];

        record[90] = value | bits;
      }
      Engine_EventWait(20);
      Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
      TolbiShipDeck_Say(KALAY_CREWMAN);
      Actor_SetSpeed(KALAY_CREWMAN, 0x13333, 0x9999);
      Object_GetById(KALAY_CREWMAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(KALAY_CREWMAN, 192, 0x324);
      Engine_EventWait(1);
      {
        struct FieldActor *record = Object_GetById(KALAY_CREWMAN);

        bits |= record->steeringFlags;
        record->steeringFlags = bits;
      }
      Engine_EventWait(20);
      Actor_SetSpeed(KALAY_CREWMAN, 0x33333, 0x19999);
      Actor_WalkToAndWait(KALAY_CREWMAN, 180, 0x30a);
      Actor_WalkToAndWait(KALAY_CREWMAN, 180, 0x2bc);
      Actor_SetPosition(KALAY_CREWMAN, PIXELS(246), PIXELS(512));
      Actor_FaceDirection(KALAY_CREWMAN, FACING_EAST, 0);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
      Engine_EventEnd();
    }
  }
}

/// The start both of those share: the camera pans to the door, which opens, and
/// the crewman steps out and denies everything. Records the confrontation.
static void TolbiShipDeck_ConfrontCrewman(void) {
  Camera_SetSpeed(0x19999, 0x3333);
  Camera_MoveTo(PIXELS(216), -1, PIXELS(824), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  TolbiShipDeck_OpenSecondDoor();
  Map_CopyCellsTo(30, 108, 13, 108, 1, 2);
  Engine_EventWait(10);
  Actor_SetPosition(KALAY_CREWMAN, PIXELS(216), PIXELS(800));
  Actor_SetSpeed(KALAY_CREWMAN, 0x13333, 0x9999);
  Actor_WalkToAndWait(KALAY_CREWMAN, 216, 0x32e);
  Actor_FaceActor(ACTOR_PARTY_LEADER, KALAY_CREWMAN, 10);
  Engine_ActorSetAnimationAndWait(KALAY_CREWMAN, 4);
  Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
  Actor_ShowEmote(KALAY_CREWMAN, 0x100, 20);
  Actor_FaceActor(KALAY_CREWMAN, ACTOR_PARTY_LEADER, 20);
  Engine_ActorStartRepeatedMotion(KALAY_CREWMAN, 2);
  Engine_EventSetMessage(MsgTolbiShipDidntDoAnything);
  Event_ShowMessageAndWait(KALAY_CREWMAN, 0, 20);
  Engine_ActorShowEmote(KALAY_CREWMAN, 0x102, 0);
  GameFlag_Set(FLAG_TOLBI_SHIP_CREWMAN_CONFRONTED);
}

/// Sets up the scene on entry: turns off flag 0x11c, sets the map's transition,
/// starts the deck's sway at a random angle and stops the sea's scroll, redraws
/// the map and applies the voyage's state.
s32 TolbiShipDeck_Initialize(void) {
  u8 **base = ((u8 **)gMapWork);
  u8 *map = base[0] + 0x104;

  Engine_GameFlagClear(FLAG_NOT_ON_MAP);
  *(s32 *)(base[19] + 0x1c0) = 0x209;
  *(s32 *)(map + 0x1c) = 0;
  sSwayAngleX = (u16)Engine_RandomNext();
  sSwayAngleY = (u16)Engine_RandomNext();
  sSeaScroll[0] = 0;
  sSeaScroll[1] = 0;
  sSeaSpeed[0] = 0;
  Engine_MapRedraw();
  Engine_TaskWait(1);
  TolbiShipDeck_ApplyEntryState();
  return 0;
}

/// Applies the voyage's state: which cells and oars are drawn, whether the deck
/// rocks and the sea scrolls, and which cutscene the entrance starts (entrances
/// 10 to 19 are the voyage's cutscenes, chained by their exits).
static void TolbiShipDeck_ApplyEntryState(void) {
  s32 flag;

  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    Engine_GameFlagClear(FLAG_SCENE_TEMP_71);
    Engine_GameFlagClear(FLAG_SCENE_TEMP_72);
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    TolbiShipDeck_CopyAnchoredCells();
    Engine_ActorSetChildValue(24, 2);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    TolbiShipDeck_CopyAnchoredCells();
  } else if ((flag = Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWING)) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    TolbiShipDeck_SetUpOars();
    Task_AddCallback(TolbiShipDeck_RowOars, 0xc80);
    sSeaScroll[1] = 0x200000;
    sSeaSpeed[1] = 0x13333;
    FieldScheduler_AddCallback(TolbiShipDeck_RockDeck, 0xc80);
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0) {
    TolbiShipDeck_CopyUnderwayCells();
    sSeaScroll[1] = flag;
    sSeaSpeed[1] = flag;
    FieldScheduler_AddCallback(TolbiShipDeck_RockDeck, 0xc80);
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWING) == 0) {
    TolbiShipDeck_RestOars();
  }
  switch (gGameState.entrance) {
  case 4:
    Object_GetById(0)->sprite->image.priority = 1;
    break;
  case 10:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0) {
      TolbiShipDeck_DepartWithParty();
    } else {
      TolbiShipDeck_DepartFromKalay();
    }
    return;
  case 11:
    TolbiShipDeck_SightSeaMonsters();
    return;
  case 12:
    TolbiShipDeck_MonstersBoard();
    return;
  case 13:
    TolbiShipDeck_MonstersAttackCrew();
    return;
  case 14:
    TolbiShipDeck_MonsterLandsOnDeck();
    return;
  case 15:
    TolbiShipDeck_JumpIntoKrakenBattle();
    return;
  case 16:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      TolbiShipDeck_SetUpAfterKraken();
    } else {
      TolbiShipDeck_ThankForKraken();
    }
    return;
  case 17:
    TolbiShipDeck_SightLand();
    return;
  case 18:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      TolbiShipDeck_RowersDisembark();
    }
    return;
  case 19:
    TolbiShipDeck_ArriveAtTolbi();
    return;
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
    *(s32 *)(((u8 *)gMapWork[0]) + 236) = 0x410000;
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    TolbiShipDeck_SetUpAtAnchor();
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED) != 0) {
    TolbiShipDeck_SetUpAfterKraken();
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0) {
    TolbiShipDeck_SetUpUnderSail();
  } else if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0) {
    TolbiShipDeck_PlacePassengersAgreedToRow();
  } else if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    TolbiShipDeck_PlaceWaitingPassengers();
  }
}

/// At Kalay while the ship waits: places the passengers by what has happened so
/// far.
static void TolbiShipDeck_PlaceWaitingPassengers(void) {
  struct FieldActor *actor;
  s32 flag;

  Engine_ActorSetSpritePriority(27, 1);
  Engine_ActorSetSpritePriority(23, 1);
  Engine_ActorSetSpritePriority(22, 1);
  Engine_ActorSetSpritePriority(26, 1);
  Engine_ActorSetSpritePriority(24, 1);
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY) != 0) {
    Engine_ActorSetPosition(22, PIXELS(162), PIXELS(666));
    Object_GetById(22)->facing = FACING_WEST;
    Engine_ActorSetPosition(23, 0, 0);
    Engine_ActorSetPosition(20, 0, 0);
  }
  flag = Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN);
  if (flag != 0) {
    Engine_ActorSetPosition(21, PIXELS(264), PIXELS(702));
    Object_GetById(21)->facing = 0x5000;
    actor = Object_GetById(21);
    actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    Actor_SetActionScript(21, gTolbiShipDeck_RandomActorActions);
    Engine_ActorSetPosition(24, PIXELS(248), PIXELS(680));
    actor = Actor_Get(24);
    actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
    Actor_SetActionScript(24, gTolbiShipDeck_RandomActorActions);
    Engine_ActorSetPosition(22, 0, 0);
  } else {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_CREWMAN_CONFRONTED) != 0) {
      Actor_SetPosition(20, PIXELS(246), PIXELS(512));
      Object_GetById(20)->facing = flag;
    }
  }
}

/// At Kalay once the party has agreed to protect the ship: places the
/// passengers who promised to row.
static void TolbiShipDeck_PlacePassengersAgreedToRow(void) {
  Engine_ActorSetPosition(21, PIXELS(262), PIXELS(706));
  Object_GetById(21)->facing = 0x5000;
  Engine_ActorSetPosition(24, PIXELS(164), PIXELS(648));
  Object_GetById(24)->facing = 0;
  Engine_ActorSetSpritePriority(24, 1);
  Engine_ActorSetPosition(25, PIXELS(198), PIXELS(665));
  Object_GetById(25)->facing = FACING_WEST;
  Engine_ActorSetSpritePriority(25, 1);
  Engine_ActorSetPosition(26, PIXELS(188), PIXELS(678));
  Object_GetById(26)->facing = 0xb000;
  Engine_ActorSetPosition(27, PIXELS(186), PIXELS(635));
  Object_GetById(27)->facing = 0x5000;
  Engine_ActorSetPosition(22, 0, 0);
  Engine_ActorSetPosition(23, 0, 0);
  Engine_ActorSetPosition(20, 0, 0);
}

/// Under sail: the crew take their places, the two lookouts start their rounds
/// and, when the party keeps its position, the leader stands on
/// deck.
static void TolbiShipDeck_SetUpUnderSail(void) {
  struct FieldActor *record;
  u8 bits;

  Engine_EventBegin();
  Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Actor_SetPosition(20, 0, 0);
  Actor_SetPosition(23, PIXELS(238), PIXELS(626));
  Actor_SetPosition(22, PIXELS(204), PIXELS(521));
  record = Object_GetById(22);
  record->y.fixed = PIXELS(16);
  bits = 128;
  {
    struct FieldActor *record = Object_GetById(22);
    u8 value = record->collisionFlags;

    record->collisionFlags = value | bits;
  }
  Actor_SetSpeed(22, 0x9999, 0x4ccc);
  Actor_SetActionScript(22, gTolbiShipDeck_LeadActionsA);
  {
    struct FieldActor *record = Object_GetById(21);

    bits |= record->collisionFlags;
    record->collisionFlags = bits;
  }
  Actor_SetSpeed(21, 0xcccc, 0x6666);
  Actor_SetActionScript(21, gTolbiShipDeck_LeadActionsB);
  if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    TolbiShipDeck_PlaceLeaderOnDeck();
  }
  Engine_EventEnd();
}

/// After the Kraken: as under sail, with Sean at the helm.
static void TolbiShipDeck_SetUpAfterKraken(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Actor_SetPosition(20, 0, 0);
  Actor_SetPosition(23, PIXELS(238), PIXELS(626));
  Actor_SetPosition(22, PIXELS(268), PIXELS(678));
  record = Object_GetById(22);
  record->facing = 0;
  Actor_SetActionScript(22, gTolbiShipDeck_LeadActionsC);
  {
    struct FieldActor *record = Object_GetById(21);
    record->collisionFlags = 128 | record->collisionFlags;
  }
  Actor_SetSpeed(21, 0xcccc, 0x6666);
  Actor_SetActionScript(21, gTolbiShipDeck_LeadActionsB);
  if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    TolbiShipDeck_PlaceLeaderOnDeck();
  }
  Engine_EventEnd();
}

/// At anchor off the island: the crew stand where they were left, by whether
/// the party has agreed to scout the island, and entering by entrance 6 means
/// the party is back from it.
static void TolbiShipDeck_SetUpAtAnchor(void) {
  struct FieldActor *record;
  s32 facing;

  *(s32 *)((s32)gMapWork[0] + 236) = 0x410000;
  Engine_EventBegin();
  Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScriptE);
  Engine_TaskWait(1);
  Engine_ActorDestroy(24);
  Actor_SetPosition(23, PIXELS(238), PIXELS(626));
  facing = 192; // 192 << 6 is 0x3000, south-southeast
  record = Object_GetById(23);
  record->facing = (facing << 6);
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED) != 0) {
    Actor_SetPosition(22, PIXELS(162), PIXELS(634));
    record = Object_GetById(22);
    record->facing = (facing << 6);
    Actor_SetPosition(21, PIXELS(162), PIXELS(676));
    record = Object_GetById(21);
    record->facing = 0xd000;
  } else {
    Engine_ActorSetPosition(22, PIXELS(160), PIXELS(652));
    record = Object_GetById(22);
    record->facing = (facing << 6);
    Actor_SetPosition(21, PIXELS(166), PIXELS(668));
    record = Object_GetById(21);
    record->facing = 0xb000;
  }
  if (gGameState.entrance == 6) {
    TolbiShipDeck_ReturnFromIsland();
  }
  Engine_EventEnd();
}

/// Entrance 10, setting sail before the party is on deck: actor 23 brings word
/// to actor 21 that all is ready.
static void TolbiShipDeck_DepartFromKalay(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(20, 0, 0);
  Actor_SetPosition(22, 0, 0);
  Actor_SetPosition(24, 0, 0);
  Actor_SetPosition(25, 0, 0);
  Actor_SetPosition(26, 0, 0);
  Actor_SetPosition(27, 0, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(23, 0, 0);
  record = Object_GetById(23);
  record->facing = 0x3000;
  Actor_SetPosition(21, PIXELS(232), PIXELS(650));
  record = Object_GetById(21);
  record->facing = 0xb000;
  Camera_MoveTo(PIXELS(232), -1, PIXELS(636), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  TolbiShipDeck_AnnouncePreparationsReady(23, 21);
}

/// Entrance 10 with the party aboard: Kaja brings word to actor 23.
static void TolbiShipDeck_DepartWithParty(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(232), PIXELS(636));
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  record = Object_GetById(0);
  Engine_ActorSetSpriteFlags(record, 0);
  Engine_TaskWait(1);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_ActorStop(22);
  Engine_ActorStop(21);
  Engine_TaskWait(1);
  Actor_SetPosition(22, 0, 0);
  Actor_SetPosition(21, 0, 0);
  Actor_SetPosition(20, 0, 0);
  record = Object_GetById(20);
  record->facing = 0x3000;
  Actor_SetPosition(23, PIXELS(232), PIXELS(650));
  record = Object_GetById(23);
  record->facing = 0xb000;
  Engine_TaskWait(1);
  TolbiShipDeck_AnnouncePreparationsReady(20, 23);
}

/// The messenger comes up through the first door to report, the officer calls
/// the ship ready, and the scene moves on to the next stage of the voyage.
/// "Preparations... Ready!"
static void TolbiShipDeck_AnnouncePreparationsReady(s32 a0, s32 a1) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  TolbiShipDeck_OpenFirstDoor();
  Actor_SetPosition(a0, PIXELS(216), PIXELS(588));
  Actor_SetSpeed(a0, 0xcccc, 0x6666);
  Actor_WalkToAndWait(a0, 216, 0x258);
  Actor_WalkToAndWait(a0, 218, 0x25c);
  Actor_WalkToAndWait(a0, 234, 0x25c);
  Actor_WalkToAndWait(a0, 236, 0x26a);
  Actor_FaceDirection(a0, 0x5000, 20);
  Engine_ActorSetAnimationAndWait(a0, 3);
  Engine_EventWait(20);
  TolbiShipDeck_Turn(a1, 0x5000);
  Engine_ActorJump(a1, 4, 40);
  Engine_ActorStartRepeatedMotion(a1, 2);
  Engine_EventSetMessage(MsgTolbiShipPreparationsReady);
  Event_ShowMessageAndWait(a1, 0, 20);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(10);
}

/// Entrance 11: sea monsters are sighted. "Aye, Captain! Sea monsters!"
static void TolbiShipDeck_SightSeaMonsters(void) {
  Engine_EventBegin();
  Event_CallWithLastActiveObjectId((s32)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(20, 1);
  Engine_EventSetMessage(MsgTolbiShipAyeCaptainSeaMonsters);
  Engine_EventShowMessageAndWait(20, 0, 10);
  TolbiShipDeck_Turn(22, 0x5000);
  Engine_ActorJump(22, 4, 20);
  Engine_ActorStartRepeatedMotion(22, 2);
  Engine_EventShowMessageAndWait(SPEAKER(22, 6), 0, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(11);
}

/// Entrance 12: the monsters leap aboard and someone cries out. "It's no use.
/// It's too late!"
static void TolbiShipDeck_MonstersBoard(void) {
  s32 zero;
  const s32 *shared;

  Engine_EventBegin();
  Event_CallWithLastActiveObjectId((s32)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Event_CallWithLastActiveObjectId((s32)gTolbiShipDeck_CrewScriptB);
  Engine_TaskWait(1);
  Actor_SetPosition(21, PIXELS(248), PIXELS(728));
  Engine_ActorSetChildValue(0, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventOpenScreen();
  Actor_SetSpeed(21, 104857, 52428);
  Actor_WalkToAndWait(21, 242, 692);
  Actor_WalkToAndWait(21, 196, 678);
  Actor_WalkToAndWait(21, 182, 654);
  Engine_ActorRunRepeatedMotion(21, 2);
  Engine_EventSetMessage(MsgTolbiShipNoUseLate);
  TolbiShipDeck_Say(SPEAKER(21, 10));
  Actor_SetSpeed(0, 157286, 78643);
  FieldObjectMotion_ResetAndSetPosition(0, 154, 609);
  Engine_AudioPlayCue(SE_146);
  zero = 0;
  Object_GetById(24)->timer = zero;
  Object_GetById(25)->timer = zero;
  Object_GetById(26)->timer = zero;
  Actor_SetPosition(24, PIXELS(32), PIXELS(484));
  Actor_SetPosition(25, PIXELS(84), PIXELS(496));
  Actor_SetPosition(26, PIXELS(16), PIXELS(596));
  Actor_SetSpeed(24, 157286, 78643);
  Actor_SetSpeed(25, 157286, 78643);
  Actor_SetSpeed(26, 157286, 78643);
  shared = gTolbiShipDeck_DeckEventActions;
  Actor_SetActionScript(24, shared);
  Actor_SetActionScript(25, shared);
  Actor_SetActionScript(26, shared);
  Engine_ActorSetChildValue(24, 3);
  Engine_ActorSetChildValue(25, 3);
  Engine_ActorSetChildValue(26, 3);
  do {
    Engine_TaskWait(1);
  } while (Object_GetById(24)->timer == 0);
  TolbiShipDeck_OpenFirstDoor();
  /* FAKEMATCH: the do/while sets r0 = 21 first, straight after the call. Plain
   * call: differ 2; Engine_ActorWalkTo directly: differ 4. Measured 2026-10-13:
   * Actor_WalkTo as a do/while statement macro at every use changes 111
   * functions in the six editions, so the block is this site's alone. */
  do {
    Actor_WalkTo(21, 196, 612);
  } while (0);
  Object_RefreshSelectorById(24);
  Engine_EventWait(10);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(10);
  Graphics_EnableObjLayerAndCallbacks();
  Ui_SetMessageVoiceFromActor(21);
  UiText_ShowCenteredMessage(MsgTolbiShipArrgh, 1, 0);
  ObjectDispatch_StopCallbacksAndHideLayers();
  Engine_EventRequestExit(12);
}

/// Entrance 13: the monsters harry the crew while more of them climb aboard.
static void TolbiShipDeck_MonstersAttackCrew(void) {

  Engine_EventBegin();
  Engine_ActorSetChildValue(0, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
  Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScriptC);
  Engine_TaskWait(1);
  Engine_ActorSetAnimation(31, 0);
  Engine_ActorSetSpriteFlags(Object_GetById(24), 1);
  Engine_ActorSetSpriteFlags(Object_GetById(25), 1);
  Engine_ActorSetSpriteFlags(Object_GetById(26), 1);
  Engine_ActorSetSpriteFlags(Object_GetById(27), 1);
  Engine_ActorSetSpriteFlags(Object_GetById(28), 1);
  Engine_ActorSetSpriteFlags(Object_GetById(29), 1);
  Actor_SetPosition(22, PIXELS(256), PIXELS(640));
  Actor_SetActionScript(22, gTolbiShipDeck_CrewActionsD);
  Actor_SetPosition(21, PIXELS(268), PIXELS(692));
  Actor_SetActionScript(22, gTolbiShipDeck_CrewActionsD);
  Actor_SetPosition(24, PIXELS(242), PIXELS(604));
  Actor_SetPosition(25, PIXELS(264), PIXELS(600));
  Actor_SetPosition(26, PIXELS(254), PIXELS(668));
  Engine_ActorSetPosition(27, PIXELS(282), PIXELS(658));
  Object_GetById(24)->riseEnabled = 0;
  Object_GetById(25)->riseEnabled = 1;
  Object_GetById(26)->riseEnabled = 0;
  Object_GetById(27)->riseEnabled = 1;
  Actor_SetActionScript(24, gTolbiShipDeck_CrewActionsB);
  Actor_SetActionScript(25, gTolbiShipDeck_CrewActionsB);
  Actor_SetActionScript(26, gTolbiShipDeck_CrewActionsA);
  Actor_SetActionScript(27, gTolbiShipDeck_CrewActionsA);
  Engine_ActorSetPosition(20, 0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(0x190);
  Actor_SetPosition(28, PIXELS(254), PIXELS(740));
  Actor_SetPosition(29, PIXELS(24), PIXELS(586));
  Actor_SetSpeed(28, 0x19999, 0xcccc);
  Actor_SetSpeed(29, 0x19999, 0xcccc);
  Actor_SetDestination(29, 172, 0x284);
  FieldObject_MotionSetPositionAndCommit(28, 200, 0x294);
  Actor_SetSpeed(0, 0x40000, 0x20000);
  Actor_SetDestination(0, 174, 0x26c);
  Engine_ObjectMotionSetPositionAndCommit(28, 180, 0x244);
  Engine_AudioPlayCue(SE_146);
  Actor_SetActionScript(28, gTolbiShipDeck_CrewActionsC);
  Actor_SetActionScript(29, gTolbiShipDeck_CrewActionsC);
  Engine_AudioPlayCue(SE_240);
  Actor_SetPosition(31, PIXELS(134), PIXELS(594));
  Actor_SetActionScript(31, gTolbiShipDeck_CrewActionsE);
  Engine_EventWait(10);
  Actor_SetPosition(30, PIXELS(134), PIXELS(584));
  Actor_SetSpeed(30, 0x40000, 0x20000);
  Object_GetById(30)->velocityY = 0x80000;
  Engine_ObjectMotionSetPositionAndCommit(30, 186, 0x264);
  Engine_ActorSetSpriteFlags(Object_GetById(30), 1);
  Engine_EventWait(10);
  Actor_SetSpeed(30, 0x20000, 0x10000);
  Engine_ObjectMotionSetPositionAndCommit(30, 216, 0x258);
  TolbiShipDeck_Turn(30, FACING_NORTH);
  TolbiShipDeck_OpenFirstDoor();
  Engine_EventWait(10);
  Actor_SetActionScript(30, gTolbiShipDeck_SailorActions);
  Engine_EventWait(10);
  Actor_SetActionScript(28, gTolbiShipDeck_SailorActions);
  Engine_EventWait(10);
  Actor_RunActionScriptAndWait(29, gTolbiShipDeck_SailorActions);
  Engine_EventWait(20);
  Engine_AudioPlayCue(SE_147);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_ActorStop(24);
  Engine_ActorStop(25);
  Engine_ActorStop(26);
  Engine_ActorStop(27);
  Engine_EventWait(10);
  Graphics_EnableObjLayerAndCallbacks();
  Ui_SetMessageVoiceFromActor(21);
  UiText_ShowCenteredMessage(MsgTolbiShipArrgh, 1, 0);
  ObjectDispatch_StopCallbacksAndHideLayers();
  Engine_EventRequestExit(13);
}

/// Entrance 14: a great shape rises from the sea and a monster drops onto the
/// deck.
static void TolbiShipDeck_MonsterLandsOnDeck(void) {
  struct FieldActor *monster;
  struct FieldActor *record;
  s32 frame;
  const s32 *sailorActions;

  Engine_EventBegin();
  Engine_ActorSetChildValue(0, 15);
  record = Object_GetById(0);
  Engine_ActorSetSpriteFlags(record, 0);
  Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScriptD);
  Engine_TaskWait(1);
  Engine_ActorSetPosition(22, PIXELS(176), PIXELS(696));
  record = Object_GetById(22);
  record->facing = 0xd000;
  Engine_ActorSetPosition(21, PIXELS(264), PIXELS(662));
  record = Object_GetById(21);
  record->facing = 0xb000;
  Actor_SetPosition(24, PIXELS(184), PIXELS(672));
  Actor_SetPosition(25, PIXELS(202), PIXELS(692));
  Actor_SetPosition(26, PIXELS(252), PIXELS(646));
  Actor_SetPosition(27, PIXELS(256), PIXELS(686));
  Actor_SetPosition(28, PIXELS(172), PIXELS(632));
  Engine_ActorSetPosition(29, PIXELS(256), PIXELS(622));
  Object_GetById(24)->riseEnabled = 0;
  Object_GetById(25)->riseEnabled = 1;
  Object_GetById(26)->riseEnabled = 0;
  Object_GetById(27)->riseEnabled = 2;
  Engine_ActorSetPosition(20, 0, 0);
  Actor_SetActionScript(24, gTolbiShipDeck_DeckActionsA);
  Actor_SetActionScript(25, gTolbiShipDeck_DeckActionsA);
  Actor_SetActionScript(26, gTolbiShipDeck_DeckActionsB);
  Actor_SetActionScript(27, gTolbiShipDeck_DeckActionsB);
  Actor_SetActionScript(28, gTolbiShipDeck_DeckActionsC);
  Actor_SetActionScript(29, gTolbiShipDeck_DeckActionsC);
  Engine_ActorSetChildValue(24, 3);
  Engine_ActorSetChildValue(25, 3);
  Engine_ActorSetChildValue(26, 3);
  Engine_ActorSetChildValue(27, 3);
  Engine_ActorSetChildValue(28, 3);
  Engine_ActorSetChildValue(29, 3);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(80);
  Engine_AudioPlayCue(SE_147);
  monster = Object_GetById(31);
  monster->scaleX = 0x1999;
  monster->scaleY = 0x1999;
  monster->x.fixed = PIXELS(194);
  monster->z.fixed = PIXELS(642);
  for (frame = 0; (u32)frame < 16; frame++) {
    monster->scaleX += 0xf5c;
    monster->scaleY += 0xf5c;
    Engine_TaskWait(1);
  }
  monster = Object_GetById(30);
  monster->scaleX = 0x11999;
  monster->scaleY = 0x11999;
  monster->x.fixed = PIXELS(194);
  monster->y.fixed = PIXELS(80);
  monster->z.fixed = PIXELS(642);
  monster->facing = 0x5000;
  monster->bounce = 0x6666;
  monster->gravity = 0x20000;
  Engine_EventWait(80);
  Engine_AudioPlayCue(SE_147);
  Engine_ActorSetPosition(31, 0, 0);
  record = Object_GetById(30);
  Engine_ActorSetSpriteFlags(record, 1);
  Actor_SetSpeed(0, 0x19999, 0xcccc);
  monster = Object_GetById(0);
  monster->motionFlags = 0;
  Actor_SetDestination(0, 216, 0x264);
  Actor_SetSpeed(30, 0x19999, 0xcccc);
  FieldObject_MotionSetPositionAndCommit(30, 196, 0x258);
  FieldObject_MotionSetPositionAndCommit(30, 216, 0x258);
  Engine_ActorStop(28);
  Engine_TaskWait(1);
  Actor_SetSpeed(28, 0x19999, 0xcccc);
  sailorActions = gTolbiShipDeck_SailorActions;
  Actor_SetActionScript(28, sailorActions);
  TolbiShipDeck_Turn(30, 0xd000);
  TolbiShipDeck_OpenFirstDoor();
  Engine_EventWait(10);
  Actor_SetActionScript(30, sailorActions);
  Engine_ActorStop(29);
  Engine_TaskWait(1);
  Actor_SetSpeed(29, 0x19999, 0xcccc);
  Actor_RunActionScriptAndWait(29, sailorActions);
  Engine_EventWait(20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_ActorStop(24);
  Engine_ActorStop(25);
  Engine_ActorStop(26);
  Engine_ActorStop(27);
  Engine_ActorStop(28);
  Engine_ActorStop(29);
  Engine_EventWait(10);
  Graphics_EnableObjLayerAndCallbacks();
  Ui_SetMessageVoiceFromActor(21);
  UiText_ShowCenteredMessage(MsgTolbiShipArrgh, 1, 0);
  ObjectDispatch_StopCallbacksAndHideLayers();
  Engine_EventRequestExit(14);
}

/// Entrance 15: the party jumps down to the deck, gathers and charges; the
/// battle with the Kraken begins (battle 62), returning to entrance 16.
static void TolbiShipDeck_JumpIntoKrakenBattle(void) {
  struct FieldActor *actor;

  Engine_EventBegin();
  Event_CallWithLastActiveObjectId((s32)gTolbiShipDeck_CrewScriptE);
  Engine_TaskWait(1);
  Engine_CameraFollowActor(25, 1);
  Engine_TaskWait(1);
  Engine_ActorSetAnimation(21, 5);
  Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->facing = FACING_SOUTH;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  TolbiShipDeck_OpenFirstDoor();
  Engine_EventWait(10);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(216), PIXELS(586));
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 598);
  Engine_EventWait(20);
  TolbiShipDeck_Turn(0, FACING_SOUTHWEST);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 194, 624);
  Engine_AudioPlayCue(SE_181);
  Map_SetShake(0x40000, 0x40000, 0x10000);
  Engine_EventWait(20);
  Map_SetShake(-1, -1, 0xe666);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Object_GetById(25)->motionFlags = 0;
  Actor_SetSpeed(25, 0x20000, 0x10000);
  Engine_ActorSetDestination(25, 216, 612);
  Engine_AudioPlayCue(SE_149);
  Engine_ActorSetSpritePriority(22, 2);
  Engine_ActorSetAnimation(22, 5);
  actor = Object_GetById(22);
  actor->velocityY = 0x80000;
  actor->gravity = 0xb333;
  actor->scaleX = 0x1a000;
  actor->scaleY = 0x1a000;
  actor->update =
      (void (*)(union FieldObject *))TolbiShipDeck_ShrinkToNormalSize;
  actor->bounce = 0x8000;
  Actor_SetSpeed(22, 0x60000, 0x30000);
  Engine_ObjectMotionSetPositionAndCommit(22, 182, 0x26a);
  Engine_ActorSetSpriteFlags(Object_GetById(22), 0);
  TolbiShipDeck_Turn(0, FACING_NORTHWEST);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 80);
  Actor_SetDestination(25, 232, 564);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 204, 610);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 208, 598);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 598);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetPosition(MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(GARET, 0x10000, 0x8000);
  Actor_SetSpeed(IVAN, 0x10000, 0x8000);
  Actor_SetSpeed(MIA, 0x10000, 0x8000);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 250, 584);
  Actor_WalkTo(GARET, 240, 600);
  Actor_WalkTo(IVAN, 254, 600);
  Engine_ActorWalkToAndWait(MIA, 248, 616);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetAnimation(GARET, 1);
  Engine_ActorSetAnimation(IVAN, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_ActorFaceDirection(MIA, FACING_NORTH, 20);
  Engine_AudioPlayCue(SE_149);
  Engine_EventWait(40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Engine_ActorSetAttachedEffect(MIA, 0x102);
  Engine_EventWait(60);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Actor_SetSpeed(GARET, 0xcccc, 0x6666);
  Actor_SetSpeed(IVAN, 0xcccc, 0x6666);
  Actor_SetSpeed(MIA, 0xcccc, 0x6666);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 564);
  Actor_WalkTo(GARET, 248, 564);
  Engine_EventWait(20);
  Actor_WalkTo(IVAN, 248, 564);
  Actor_WalkTo(MIA, 248, 564);
  Engine_EventWait(20);
  gGameState.savedScene = (s32)&SceneId_TolbiShipInteriors;
  gGameState.savedEntrance = 30;
  Battle_SetStart(3);
  Event_SetNextDestination((s32)&SceneId_TolbiShipDeck, 16);
  Field_RequestBattleFromEncounterTable(62, 3);
  Engine_EventEnd();
}

/// Entrance 16: Isaac is thanked for the Kraken.
/// "Thank you, Isaac. You did good against that Kraken."
static void TolbiShipDeck_ThankForKraken(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Actor_SetPosition(SEA_KAJA, PIXELS(182), PIXELS(618));
  Actor_SetPosition(23, PIXELS(238), PIXELS(626));
  Actor_SetPosition(SEA_SEAN, PIXELS(268), PIXELS(678));
  record = Object_GetById(SEA_SEAN);
  record->facing = 0;
  Actor_SetActionScript(SEA_SEAN, gTolbiShipDeck_LeadActionsC);
  {
    struct FieldActor *record = Object_GetById(SEA_OURANOS);
    record->collisionFlags = 128 | record->collisionFlags;
  }
  Actor_SetSpeed(SEA_OURANOS, 0xcccc, 0x6666);
  Actor_SetActionScript(SEA_OURANOS, gTolbiShipDeck_LeadActionsB);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_SetSpeed(SEA_KAJA, 0x19999, 0xcccc);
  Actor_WalkToAndWait(SEA_KAJA, 182, 0x224);
  TolbiShipDeck_Turn(SEA_KAJA, FACING_EAST);
  TolbiShipDeck_Turn(0, FACING_WEST);
  Engine_ActorRunRepeatedMotion(SEA_KAJA, 1);
  Engine_EventSetMessage(MsgTolbiShipThankIsaacDidGoodAgainst);
  TolbiShipDeck_Say(SEA_KAJA);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(40);
  Actor_FaceDirection(SEA_KAJA, 0x5000, 20);
  Actor_ShowEmote(SEA_KAJA, 0x105, 60);
  Event_ShowMessageAndWait(SEA_KAJA, 0, 40);
  TolbiShipDeck_Turn(SEA_KAJA, FACING_EAST);
  TolbiShipDeck_Say(SEA_KAJA);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(SEA_KAJA, 3);
  Actor_WalkToAndWait(SEA_KAJA, 182, 0x258);
  Actor_WalkToAndWait(SEA_KAJA, 216, 0x258);
  TolbiShipDeck_Turn(SEA_KAJA, FACING_NORTH);
  TolbiShipDeck_OpenFirstDoor();
  Engine_EventWait(10);
  Actor_WalkToAndWait(SEA_KAJA, 216, 0x244);
  Actor_SetPosition(SEA_KAJA, 0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  GameFlag_Set(FLAG_TOLBI_SHIP_KRAKEN_DEFEATED);
  GameFlag_Clear(FLAG_TOLBI_SHIP_KRAKEN_ABOARD);
  Engine_EventEnd();
}

/// Entrance 17: land is sighted. "Hey, I can see land!"
static void TolbiShipDeck_SightLand(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  record = Object_GetById(0);
  Engine_ActorSetSpriteFlags(record, 0);
  Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Actor_SetPosition(20, PIXELS(196), PIXELS(502));
  record = Object_GetById(20);
  record->facing = FACING_NORTHWEST;
  Actor_SetPosition(22, PIXELS(184), PIXELS(524));
  record = Object_GetById(22);
  record->facing = 0xb000;
  Engine_ActorSetSpritePriority(21, 1);
  Actor_SetPosition(21, PIXELS(184), PIXELS(632));
  record = Object_GetById(21);
  record->facing = 0xb000;
  /* FAKEMATCH: needed to match: the work pointer is read as a word, which may
   * alias the facing stores, so they stay ahead of its load.
   * gEventWork->startTransition: differ 2; the records held as union
   * FieldObject: differ 2. Measured 2026-10-13: with gEventWork declared a word
   * (u32, cast at each use) the plain store matches and, in all six editions,
   * only ValeHome_RunActorPairEffectScene changes (differ 2); this word view on
   * every startTransition store changes 49 functions. */
  ((struct EventWork *)*(u32 *)&gEventWork)->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_ActorJump(22, 4, 10);
  Engine_ActorJump(22, 6, 20);
  Engine_EventSetMessage(MsgTolbiShipCanSeeLand);
  TolbiShipDeck_Say(22);
  Engine_ActorSetAnimationAndWait(20, 3);
  Actor_SetSpeed(21, 0x30000, 0x18000);
  Actor_WalkToAndWait(21, 180, 0x222);
  Engine_ActorFaceDirection(21, 0xb000, 40);
  Engine_ActorRunRepeatedMotion(21, 1);
  TolbiShipDeck_Say(21);
  Engine_EventRequestExit(15);
}

/// Entrance 18, at Tolbi: the four passengers who rowed come up from below in
/// turn and each says something, then the leader comes up and is thanked for
/// the hard work. Who rowed is read from the four seats' flag groups, the
/// passengers (actors 23 to 31) in a fixed order of preference.
static void TolbiShipDeck_RowersDisembark(void) {
  s32 roster[4];
  s32 matched[4];
  s32 category[4];
  u32 index;
  s32 found;
  s32 actor;
  s32 member;
  struct FieldActor *record;

  found = 0;
  for (index = 0; index <= 3; index++) {
    roster[index] = TolbiShipDeck_GetRowerAtSeat(index);
    category[index] = 0;
  }

  member = 23;
  for (index = 0; index <= 3; index++) {
    if (roster[index] == member) {
      matched[found] = member;
      category[found] = found;
      found++;
      break;
    }
  }
  member = 24;
  for (index = 0; index <= 3; index++) {
    if (roster[index] == member) {
      matched[found] = member;
      category[found] = found;
      found++;
      break;
    }
  }
  member = 25;
  for (index = 0; index <= 3; index++) {
    if (roster[index] == member) {
      matched[found] = member;
      category[found] = found;
      found++;
      break;
    }
  }
  member = 27;
  for (index = 0; index <= 3; index++) {
    if (roster[index] == member) {
      matched[found] = member;
      category[found] = found;
      found++;
      break;
    }
  }
  if (found != 4) {
    member = 28;
    for (index = 0; index <= 3; index++) {
      if (roster[index] == member) {
        matched[found] = member;
        category[found] = found;
        found++;
        break;
      }
    }
    if (found != 4) {
      member = 29;
      for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
          matched[found] = member;
          category[found] = found;
          found++;
          break;
        }
      }
      if (found != 4) {
        member = 26;
        for (index = 0; index <= 3; index++) {
          if (roster[index] == member) {
            matched[found] = member;
            category[found] = 10;
            found++;
            break;
          }
        }
        if (found != 4) {
          member = 30;
          for (index = 0; index <= 3; index++) {
            if (roster[index] == member) {
              matched[found] = member;
              category[found] = 11;
              found++;
              break;
            }
          }
          if (found != 4) {
            member = 31;
            for (index = 0; index <= 3; index++) {
              if (roster[index] == member) {
                matched[found] = member;
                category[found] = 20;
                break;
              }
            }
          }
        }
      }
    }
  }

  Engine_EventBegin();
  Engine_ActorSetChildValue(0, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
  record = Object_GetById(0);
  if (record != 0) {
    Engine_ActorSetPosition(32, record->x.fixed, record->z.fixed);
  }
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_CameraFollowActor(32, 1);

  TolbiShipDeck_OpenFirstDoor();

  Engine_EventWait(10);

  for (index = 0; index <= 3; index++) {
    actor = matched[index];
    Actor_SetPosition(actor, 216 << 16, 146 << 18);
    if (category[index] == 20) {
      Actor_SetSpeed(actor, 0xcccc, 0x6666);
    } else {
      Actor_SetSpeed(actor, 128 << 9, 128 << 8);
    }
    Actor_WalkToAndWait(actor, 216, 150 << 2);
    Actor_WalkToAndWait(actor, 192, 0x26a);
    Engine_ActorWalkToAndWait(actor, 192, 164 << 2);

    switch (category[index]) {
    case 0:
      Engine_ActorShowEmote(actor, 129 << 1, 60);
      Engine_EventSetMessage(MsgTolbiShipMadeFinallyLucky);
      break;
    case 1:
      TolbiShipDeck_Turn(actor, 208 << 8);
      Engine_ActorShowEmote(actor, 129 << 1, 60);
      Engine_EventSetMessage(MsgTolbiShipOtherPassengersAlready);
      break;
    case 2:
      Engine_ActorShowEmote(actor, 0x105, 60);
      Engine_EventSetMessage(MsgTolbiShipDontRowAnymore);
      break;
    case 3:
      Engine_ActorRunRepeatedMotion(actor, 1);
      Engine_EventSetMessage(MsgTolbiShipOhhhHaventWorkout);
      break;
    case 10:
      Engine_ActorSetAnimationAndWait(actor, 3);
      Engine_EventSetMessage(MsgTolbiShipRowingShipMore);
      break;
    case 11:
      Engine_ActorSetAnimation(actor, 4);
      Engine_EventSetMessage(MsgTolbiShipTotallyLostOcean);
      break;
    case 20:
      Engine_ActorSetAnimationAndWait(actor, 4);
      Engine_ActorShowEmote(actor, 0x107, 40);
      Engine_EventSetMessage(MsgTolbiShipMakeOldMan);
      break;
    default:
      break;
    }

    TolbiShipDeck_Say(actor);
    Actor_SetActionScript(actor, gTolbiShipDeck_RosterActions);
  }

  Object_RefreshSelectorById(actor);
  Engine_EventWait(40);
  Engine_ActorSetPosition(0, 216 << 16, 146 << 18);
  Engine_TaskWait(1);
  Engine_ActorSetChildValue(0, 0);
  Engine_ActorSetSpriteFlags(Object_GetById(0), 1);
  Actor_SetSpeed(0, 0xcccc, 0x6666);
  Actor_WalkToAndWait(0, 216, 150 << 2);
  Actor_WalkToAndWait(0, 190, 153 << 2);
  Engine_EventSetMessage(MsgTolbiShipThanksHardWork);
  TolbiShipDeck_Say(20);
  Engine_ActorFaceDirection(0, 192 << 8, 0);

  Object_GetById(32)->motionFlags = 0;
  Actor_SetSpeed(32, 128 << 10, 128 << 9);
  Actor_SetDestination(32, 196, 141 << 2);

  Actor_SetSpeed(20, 0xcccc, 0x6666);
  Actor_WalkToAndWait(20, 182, 0x22b);
  Actor_FaceDirection(20, 192 << 6, 20);
  Engine_ActorRunRepeatedMotion(20, 1);
  TolbiShipDeck_Say(20);
  Engine_ActorSetAnimationAndWait(20, 3);
  TolbiShipDeck_Say(20);
  Engine_ActorFaceDirection(20, 128 << 8, 40);
  TolbiShipDeck_Say(20);
  TolbiShipDeck_Turn(20, 192 << 6);
  TolbiShipDeck_Say(20);
  Engine_ActorSetAnimationAndWait(20, 3);
  Engine_CameraFollowActor(0, 1);
  Actor_WalkToAndWait(20, 188, 128 << 2);
  Engine_ActorFaceDirection(20, 192 << 6, 0);
  Engine_ActorSetPosition(32, 0, 0);

  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  Engine_EventEnd();
}

/// Returns the passenger (actor 23 to 31) who promised to row at a seat (0 to
/// 3), or 0 if the seat is empty. Each seat has a group of nine flags, one for
/// each passenger.
static s32 TolbiShipDeck_GetRowerAtSeat(u32 seat) {
  s32 flag = 0;
  s32 passenger = 23;
  u32 i;

  switch (seat) {
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
  for (i = 0; i < 9; i++) {
    if (Engine_GameFlagIsSet(flag) != 0)
      return passenger;
    flag++;
    passenger++;
  }
  return 0;
}

/// The callback that rocks the deck while under sail: sways the camera on two
/// random-walking angles and scrolls the sea layer, wrapping at 512 pixels.
static void TolbiShipDeck_RockDeck(void) {
  struct MapWork *map = gMapWork[0];
  s32 *camera = map->camera;
  s32 dx = Engine_MathCos(sSwayAngleX);
  s32 dy = Engine_MathSin(sSwayAngleY);
  struct MapLayer *layer;

  *camera++ += dx >> 1;
  *camera += dy;
  sSwayAngleX += (u32)(Engine_RandomNext() * 3 << 7) >> 16;
  sSwayAngleY += (u32)(Engine_RandomNext() << 9) >> 16;
  sSwayAngleX &= 0xffff;
  sSwayAngleY &= 0xffff;
  layer = &map->layers[5];
  layer->x = sSeaScroll[0];
  sSeaScroll[0] -= sSeaSpeed[0];
  if (sSeaScroll[0] < 0) {
    sSeaScroll[0] += 0x200000;
  }
  if (sSeaScroll[0] > 0x200000) {
    sSeaScroll[0] -= 0x200000;
  }
  layer->y = sSeaScroll[1];
  sSeaScroll[1] -= sSeaSpeed[1];
  if (sSeaScroll[1] < 0) {
    sSeaScroll[1] += 0x200000;
  }
}

/// Under sail: shows the rowers (8 to 11) and the oars (12 to 19, the
/// right-hand ones mirrored), records each oar's resting depth and chooses
/// their strokes.
static void TolbiShipDeck_SetUpOars(void) {
  Object_GetById(8)->collisionFlags = 0;
  Object_GetById(9)->collisionFlags = 0;
  Object_GetById(10)->collisionFlags = 0;
  Object_GetById(11)->collisionFlags = 0;
  TolbiShipDeck_ShowOar(8);
  TolbiShipDeck_ShowOar(9);
  TolbiShipDeck_ShowOar(10);
  TolbiShipDeck_ShowOar(11);
  TolbiShipDeck_ShowOar(12);
  TolbiShipDeck_ShowOar(13);
  TolbiShipDeck_ShowOar(14);
  TolbiShipDeck_ShowOar(15);
  sOarDepth[0] = Object_GetById(12)->z.fixed;
  sOarDepth[1] = Object_GetById(13)->z.fixed;
  sOarDepth[2] = Object_GetById(14)->z.fixed;
  sOarDepth[3] = Object_GetById(15)->z.fixed;
  TolbiShipDeck_ShowOar(16);
  TolbiShipDeck_ShowOar(17);
  TolbiShipDeck_ShowOar(18);
  TolbiShipDeck_ShowOar(19);
  Object_GetById(16)->scaleX = 0xffff0000;
  Object_GetById(17)->scaleX = 0xffff0000;
  Object_GetById(18)->scaleX = 0xffff0000;
  Object_GetById(19)->scaleX = 0xffff0000;
  sOarDepth[4] = Object_GetById(16)->z.fixed;
  sOarDepth[5] = Object_GetById(17)->z.fixed;
  sOarDepth[6] = Object_GetById(18)->z.fixed;
  sOarDepth[7] = Object_GetById(19)->z.fixed;
  TolbiShipDeck_ChooseOarStrokes();
}

/// Shows an oar or rower drawn under the deck, with no collision.
static void TolbiShipDeck_ShowOar(s32 a) {
  struct FieldActor *p = Object_GetById(a);

  if (p != 0) {
    Engine_ActorSetSpritePriority(a, 3);
    Engine_ActorSetSpriteFlags(p, 0);
    p->collisionFlags = 0;
    {
      s32 c;
      c = 2 | p->priorityFlags;
      p->priorityFlags = c;
    }
  }
}

struct OarSprite {
  s32 unk00[7];
  s16 unk1c;
  s16 tilt;
};

/// Gives each oar the stroke of its seat's rower; empty seats take stroke 3.
/// The first loop's stores are all overwritten below.
static void TolbiShipDeck_ChooseOarStrokes(void) {
  u32 *mode = sOarStroke;
  u32 i;

  for (i = 0; i <= 3; i++) {
    if (TolbiShipDeck_GetRowerAtSeat(i) != 0)
      *mode = TolbiShipDeck_GetSeatStroke(i);
    else
      *mode = 3;
    mode++;
  }
  if (TolbiShipDeck_GetRowerAtSeat(0) != 0)
    sOarStroke[0] = TolbiShipDeck_GetSeatStroke(0);
  else
    sOarStroke[0] = 3;
  if (TolbiShipDeck_GetRowerAtSeat(2) != 0)
    sOarStroke[1] = TolbiShipDeck_GetSeatStroke(2);
  else
    sOarStroke[1] = 3;
  sOarStroke[2] = 3;
  sOarStroke[3] = 3;
  if (TolbiShipDeck_GetRowerAtSeat(1) != 0)
    sOarStroke[4] = TolbiShipDeck_GetSeatStroke(1);
  else
    sOarStroke[4] = 3;
  if (TolbiShipDeck_GetRowerAtSeat(3) != 0)
    sOarStroke[5] = TolbiShipDeck_GetSeatStroke(3);
  else
    sOarStroke[5] = 3;
  sOarStroke[6] = 3;
  sOarStroke[7] = 3;
}

/// Returns the stroke (1 to 4) of the rower at a seat, or 0 if the seat is
/// empty.
static s32 TolbiShipDeck_GetSeatStroke(u32 seat) {
  s32 base = 0;
  u32 i;

  switch (seat) {
  case 0:
    base = FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS;
    break;
  case 1:
    base = FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS;
    break;
  case 2:
    base = FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS;
    break;
  case 3:
    base = FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS;
    break;
  }
  for (i = 0; i < 9; i++) {
    if (Engine_GameFlagIsSet(base + i) != 0)
      return gTolbiShipDeckRowerStrokes[i];
  }
  return 0;
}

/// The task that rows: advances the four rowers' strokes and swings the eight
/// oars, each following its rower's phase.
static void TolbiShipDeck_RowOars(void) {
  TolbiShipDeck_AdvanceStroke(8, 0);
  sStrokePhase[0] = 0;
  TolbiShipDeck_AdvanceStroke(9, 1);
  sStrokePhase[1] += 0x80;
  TolbiShipDeck_AdvanceStroke(10, 2);
  sStrokePhase[2] += 0x100;
  TolbiShipDeck_AdvanceStroke(11, 3);
  sStrokePhase[3] += 0x200;
  TolbiShipDeck_SwingOar(12, 0, 0);
  TolbiShipDeck_SwingOar(13, 1, 0);
  TolbiShipDeck_SwingOar(14, 2, 0);
  TolbiShipDeck_SwingOar(15, 3, 0);
  TolbiShipDeck_SwingOar(16, 4, 1);
  TolbiShipDeck_SwingOar(17, 5, 1);
  TolbiShipDeck_SwingOar(18, 6, 1);
  TolbiShipDeck_SwingOar(19, 7, 1);
}

/// Advances one rower's stroke phase: slowly through the catch and the finish
/// of the stroke, quicker through the drive, quickest on the return; the
/// animation follows.
static void TolbiShipDeck_AdvanceStroke(s32 actor, s32 slot) {
  u16 value = sOarAngle[slot];

  if (value >= 0x6801 && value <= 0x6fff) {
    sStrokePhase[slot] += 0x70;
    Engine_ActorSetAnimation(actor, 3);
  } else if (value >= 0xe801 && value <= 0xefff) {
    sStrokePhase[slot] += 0xe0;
    Engine_ActorSetAnimation(actor, 3);
  } else if (value >= 0x7001 && value <= 0xefff) {
    sStrokePhase[slot] += 0x1c0;
    Engine_ActorSetAnimation(actor, 2);
  } else {
    sStrokePhase[slot] += 0x300;
    Engine_ActorSetAnimation(actor, 1);
  }
}

/// Swings one oar to its angle: unless flags bit 1 is set, the angle follows
/// the rower whose stroke the oar takes; bit 0 marks a right-hand oar. The
/// oar's tilt and depth follow the angle.
static void TolbiShipDeck_SwingOar(s32 id, s32 slot, s32 flags) {
  struct FieldActor *actor = Object_GetById(id);
  struct OarSprite *sprite = (struct OarSprite *)actor->sprite;
  s32 sine;

  if ((flags & 2) == 0) {
    switch (sOarStroke[slot]) {
    case 1:
      sOarAngle[slot] = sStrokePhase[0];
      ObjectVisual_CopyAttributes(id, 8);
      break;
    case 2:
      sOarAngle[slot] = sStrokePhase[1];
      ObjectVisual_CopyAttributes(id, 9);
      break;
    case 3:
      sOarAngle[slot] = sStrokePhase[2];
      ObjectVisual_CopyAttributes(id, 10);
      break;
    case 4:
      sOarAngle[slot] = sStrokePhase[3];
      ObjectVisual_CopyAttributes(id, 11);
      break;
    }
  }
  if (flags & 1) {
    sine = Engine_MathSin(sOarAngle[slot]);
    sprite->tilt = Engine_MathSin(sOarAngle[slot] + 0x8000) >> 5;
    actor->z.fixed = sOarDepth[slot] - (sine << 2) - (sine << 1);
  } else {
    sine = Engine_MathSin(sOarAngle[slot] + 0x8000);
    sprite->tilt = Engine_MathSin(sOarAngle[slot]) >> 5;
    actor->z.fixed = sOarDepth[slot] + (sine << 2) + (sine << 1);
  }
}

/// When not under sail: the oars lie at rest, all following the first rower's
/// placeholder, which stands where the leader is.
static void TolbiShipDeck_RestOars(void) {
  struct FieldActor *actor;
  u32 i;

  for (i = 0; i <= 7; i++)
    sOarAngle[i] = 0xc000;
  TolbiShipDeck_ShowOar(8);
  Engine_ActorSetPosition(9, 0, 0);
  Engine_ActorSetPosition(10, 0, 0);
  Engine_ActorSetPosition(11, 0, 0);
  Engine_ActorSetPosition(12, 0, 0);
  TolbiShipDeck_ShowOar(13);
  TolbiShipDeck_ShowOar(14);
  TolbiShipDeck_ShowOar(15);
  sOarStroke[0] = 0;
  sOarStroke[1] = 0;
  sOarStroke[2] = 0;
  sOarStroke[3] = 0;
  sOarDepth[0] = Object_GetById(8)->z.fixed;
  sOarDepth[1] = Object_GetById(13)->z.fixed;
  sOarDepth[2] = Object_GetById(14)->z.fixed;
  sOarDepth[3] = Object_GetById(15)->z.fixed;
  TolbiShipDeck_ShowOar(16);
  TolbiShipDeck_ShowOar(17);
  TolbiShipDeck_ShowOar(18);
  TolbiShipDeck_ShowOar(19);
  Object_GetById(16)->scaleX = 0xffff0000;
  Object_GetById(17)->scaleX = 0xffff0000;
  Object_GetById(18)->scaleX = 0xffff0000;
  Object_GetById(19)->scaleX = 0xffff0000;
  sOarStroke[4] = 0;
  sOarStroke[5] = 0;
  sOarStroke[6] = 0;
  sOarStroke[7] = 0;
  sOarDepth[4] = Object_GetById(16)->z.fixed;
  sOarDepth[5] = Object_GetById(17)->z.fixed;
  sOarDepth[6] = Object_GetById(18)->z.fixed;
  sOarDepth[7] = Object_GetById(19)->z.fixed;
  actor = Object_GetById(0);
  if (actor != NULL)
    Engine_ActorSetPosition(8, actor->x.fixed, actor->z.fixed);
  Engine_TaskWait(1);
  ObjectVisual_CopyAttributes(13, 8);
  ObjectVisual_CopyAttributes(14, 8);
  ObjectVisual_CopyAttributes(15, 8);
  ObjectVisual_CopyAttributes(16, 8);
  ObjectVisual_CopyAttributes(17, 8);
  ObjectVisual_CopyAttributes(18, 8);
  ObjectVisual_CopyAttributes(19, 8);
  Object_GetById(8)->unk5c = 1;
  Object_GetById(13)->unk5c = 1;
  Object_GetById(14)->unk5c = 1;
  Object_GetById(15)->unk5c = 1;
  Object_GetById(16)->unk5c = 1;
  Object_GetById(17)->unk5c = 1;
  Object_GetById(18)->unk5c = 1;
  Object_GetById(19)->unk5c = 1;
  Engine_TaskWait(1);
  Engine_ActorSetPosition(8, PIXELS(132), PIXELS(632));
  Engine_TaskWait(1);
  TolbiShipDeck_SwingOar(8, 0, 2);
  TolbiShipDeck_SwingOar(13, 1, 2);
  TolbiShipDeck_SwingOar(14, 2, 2);
  TolbiShipDeck_SwingOar(15, 3, 2);
  TolbiShipDeck_SwingOar(16, 4, 3);
  TolbiShipDeck_SwingOar(17, 5, 3);
  TolbiShipDeck_SwingOar(18, 6, 3);
  TolbiShipDeck_SwingOar(19, 7, 3);
}

/// Redraws a block of the deck's cells and attributes, as it is whenever the
/// ship has left the Kalay dock.
static void TolbiShipDeck_CopyUnderwayCells(void) {
  Map_CopyCellsTo(78, 39, 78, 40, 5, 1);
  Map_CopyCellsTo(78, 39, 78, 41, 5, 1);
  Map_CopyCellsTo(78, 39, 79, 42, 4, 1);
  Map_CopyCellsTo(78, 39, 82, 43, 1, 1);
  {

    Map_CopyCellAttributes(17, 38, 5, 2, 17, 40);
  }
}

/// Redraws two more blocks of cells, as they are at anchor.
static void TolbiShipDeck_CopyAnchoredCells(void) {
  Map_CopyCellsTo(66, 61, 64, 40, 5, 4);
  Map_CopyCellAttributes(0, 0, 5, 4, 5, 39);
}

/// Shows the current message from a speaker and waits 10 frames.
static void TolbiShipDeck_Say(s32 a) {
  Engine_EventShowMessage(a, 0);
  Engine_EventWait(10);
}

/// Turns an actor to a facing over 10 frames.
static void TolbiShipDeck_Turn(s32 a, s32 b) {
  Engine_ActorFaceDirection(a, b, 10);
}

/// Entrance 19: the ship reaches Tolbi and the crew talk it over before Kaja
/// goes below.
static void TolbiShipDeck_ArriveAtTolbi(void) {
  Engine_EventBegin();
  sSeaScroll[0] = 0x40000;
  sSeaSpeed[0] = -0x8000;
  Event_CallWithLastActiveObjectId(gTolbiShipDeck_CrewScript);
  Engine_TaskWait(1);
  Actor_SetPosition(21, PIXELS(182), PIXELS(618));
  Actor_Get(21)->facing = FACING_NORTH;
  Actor_SetPosition(20, PIXELS(218), PIXELS(516));
  Actor_Get(20)->facing = 0xb000;
  Actor_SetPosition(22, PIXELS(204), PIXELS(526));
  Actor_Get(22)->facing = 0xb000;
  Actor_SetPosition(23, 0, 0);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Actor_SetSpeed(21, 0xcccc, 0x6666);
  Actor_WalkToAndWait(21, 182, 0x214);
  Actor_FaceDirection(21, 0xb000, 40);
  Engine_EventSetMessage(MsgTolbiShipFinallyReachedTolbi);
  TolbiShipDeck_Say(21);
  Engine_ActorRunRepeatedMotion(20, 2);
  Engine_ActorSetAnimation(20, 4);
  TolbiShipDeck_Say(SPEAKER(20, 6));
  Actor_FaceDirection(21, 0xd000, 0);
  Actor_FaceDirection(22, 0xd000, 0);
  Actor_ShowEmote(21, 0x101, 0);
  Actor_ShowEmote(22, 0x101, 60);
  Engine_ActorSetAnimation(20, 3);
  TolbiShipDeck_Say(SPEAKER(20, 6));
  Actor_SetAttachedEffect(21, 0x102);
  Actor_SetAttachedEffect(22, 0x102);
  Engine_EventWait(80);
  Actor_ShowEmote(21, 0x100, 20);
  Actor_SetSpeed(21, 0x19999, 0xcccc);
  Actor_WalkToAndWait(21, 194, 0x1f4);
  Actor_FaceDirection(21, 0xb000, 20);
  Actor_SetSpeed(22, 0xcccc, 0x6666);
  Actor_WalkToAndWait(22, 192, 0x206);
  Actor_FaceDirection(22, 0xb000, 0);
  Actor_SetSpeed(20, 0x10000, 0x8000);
  Actor_WalkToAndWait(20, 210, 0x1fc);
  TolbiShipDeck_Turn(20, 0xb000);
  Engine_ActorRunRepeatedMotion(21, 1);
  TolbiShipDeck_Say(SPEAKER(21, 5));
  Engine_ActorSetAnimationAndWait(20, 3);
  TolbiShipDeck_Turn(22, 0xd000);
  TolbiShipDeck_Say(SPEAKER(22, 9));
  Actor_FaceDirection(20, FACING_WEST, 20);
  Engine_ActorSetAnimation(20, 4);
  TolbiShipDeck_Say(SPEAKER(20, 10));
  Actor_WalkToAndWait(20, 204, 0x218);
  Actor_FaceDirection(22, 0xb000, 0);
  Actor_WalkToAndWait(20, 182, 0x224);
  Actor_WalkToAndWait(20, 182, 0x250);
  Actor_WalkTo(20, 182, 0x298);
  Engine_EventWait(40);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(16);
}

/// At anchor: the party is asked to scout the island; Garet and Ivan may argue
/// before the party agrees.
static void TolbiShipDeck_OfferToScoutIsland(void) {
  struct FieldActor *leader;
  u8 bits;

  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 180, 0x28e);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x10000, 0x8000);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Actor_WalkTo(GARET, 194, 0x280);
  Actor_WalkTo(IVAN, 198, 0x28e);
  Actor_WalkToAndWait(MIA, 194, 0x2a0);
  Engine_ActorSetAnimation(GARET, 1);
  Engine_ActorSetAnimation(IVAN, 1);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  TolbiShipDeck_Turn(3, FACING_WEST);
  TolbiShipDeck_Turn(22, FACING_EAST);
  Engine_EventSetMessage(MsgTolbiShipIsaacTalkedPassengersDidntTour);
  TolbiShipDeck_Say(22);
  TolbiShipDeck_Turn(21, 0xd000);
  Event_ShowMessageAndWait(21, 0, 40);
  Actor_ShowEmote(22, 0x100, 20);
  Engine_ActorRunRepeatedMotion(22, 1);
  Event_OpenMessage(22, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    Engine_ActorSetAnimationAndWait(IVAN, 4);
    TolbiShipDeck_Say(2);
    TolbiShipDeck_Turn(3, FACING_NORTHWEST);
    Engine_ActorSetAnimation(MIA, 3);
    TolbiShipDeck_Say(3);
    TolbiShipDeck_Turn(1, FACING_SOUTHWEST);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Event_OpenMessage(GARET, 0);
    for (;;) {
      if (Engine_EventChooseYesNo(0, 0) == 1) {
        Engine_ActorRunRepeatedMotion(IVAN, 1);
        Engine_EventSetMessage(MsgTolbiShipIsaacDontTalkLikeShouldnt);
        Event_OpenMessage(IVAN, 0);
      } else
        break;
    }
  }
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(22, 3);
  Engine_EventSetMessage(MsgTolbiShipSeeYoureGoingGoFor);
  TolbiShipDeck_Say(22);
  Actor_SetSpeed(22, 0x10000, 0x8000);
  Actor_SetSpeed(21, 0x10000, 0x8000);
  Object_GetById(22)->steeringFlags &= 254;
  Actor_WalkToAndWait(22, 162, 0x27a);
  Engine_EventWait(1);
  bits = ACTOR_FACES_MOTION;
  {
    struct FieldActor *actor = Object_GetById(22);
    u8 value = actor->steeringFlags;

    actor->steeringFlags = value | bits;
  }
  Object_GetById(21)->steeringFlags &= 254;
  Actor_WalkToAndWait(21, 162, 0x2a4);
  Engine_EventWait(1);
  {
    struct FieldActor *actor = Object_GetById(21);

    bits |= actor->steeringFlags;
    actor->steeringFlags = bits;
  }
  Actor_FaceDirection(22, 0x3000, 0);
  TolbiShipDeck_Turn(21, 0xd000);
  TolbiShipDeck_Say(22);
  Actor_WalkTo(GARET, 180, 0x28e);
  Actor_WalkTo(IVAN, 180, 0x28e);
  Actor_WalkToAndWait(MIA, 180, 0x28e);
  Engine_ActorDestroy(GARET);
  Engine_ActorDestroy(IVAN);
  Engine_ActorDestroy(MIA);
  GameFlag_Set(FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED);
  Engine_EventEnd();
}

/// Back on deck from the island: on yes, the crew make ready to sail and the
/// scene moves on. "How was it, Isaac? Did you explore the island
/// thoroughly?"
static void TolbiShipDeck_ReturnFromIsland(void) {
  Engine_EventBegin();
  Event_CallWithLastActiveObjectId(gTolbiShipDeck_ClosingCrewScript);
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 148, 0x290);
  Actor_ShowEmote(22, 0x100, 0);
  Engine_ActorRunRepeatedMotion(22, 1);
  TolbiShipDeck_Turn(22, 0x5000);
  Engine_EventSetMessage(MsgTolbiShipHowWasIsaacDidExplore);
  Event_OpenMessage(SPEAKER(22, 2), 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    TolbiShipDeck_Say(SPEAKER(22, 2));
    Engine_EventEnd();
    return;
  }
  gEventWork->message++;
  Event_AskYesNo(SPEAKER(22, 2), 0);
  TolbiShipDeck_OpenFirstDoor();
  Actor_SetPosition(26, PIXELS(216), PIXELS(588));
  Actor_SetSpeed(26, 0x13333, 0x9999);
  Actor_WalkToAndWait(26, 216, 0x254);
  Actor_WalkToAndWait(26, 188, 0x268);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(21, 0xd000, 0);
  Actor_FaceDirection(22, 0xd000, 0);
  TolbiShipDeck_Turn(26, 0x5000);
  Engine_ActorJump(26, 2, 0);
  Engine_ActorSetAnimation(26, 4);
  Event_ShowMessage(26, 0);
  Actor_SetPosition(20, PIXELS(180), PIXELS(777));
  Actor_SetSpeed(20, 0x10000, 0x8000);
  Actor_WalkToAndWait(20, 180, 0x298);
  Actor_FaceDirection(20, 0xd000, 0);
  TolbiShipDeck_Say(SPEAKER(20, 2));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(22, 0x3000, 0);
  Actor_ShowEmote(26, 0x101, 60);
  Engine_ActorRunRepeatedMotion(20, 1);
  TolbiShipDeck_Say(SPEAKER(20, 2));
  Engine_ActorStartRepeatedMotion(21, 2);
  TolbiShipDeck_Say(21);
  Actor_FaceDirection(20, 0x5000, 20);
  Engine_ActorSetAnimation(20, 3);
  TolbiShipDeck_Say(SPEAKER(20, 6));
  Engine_ActorJump(26, 2, 20);
  Engine_ActorSetAnimation(26, 4);
  TolbiShipDeck_Say(26);
  Actor_WalkToAndWait(20, 182, 0x280);
  Actor_FaceDirection(20, 0xd000, 0);
  TolbiShipDeck_Say(SPEAKER(20, 8));
  Actor_ShowEmote(26, 0x100, 20);
  Engine_ActorStartRepeatedMotion(26, 2);
  TolbiShipDeck_Say(26);
  Engine_ActorSetAnimationAndWait(20, 3);
  TolbiShipDeck_Turn(22, FACING_EAST);
  Engine_ActorRunRepeatedMotion(22, 1);
  TolbiShipDeck_Say(22);
  Actor_SetSpeed(22, 0x19999, 0xcccc);
  Actor_SetActionScript(22, gTolbiShipDeck_LeaveActions);
  Actor_SetSpeed(21, 0x19999, 0xcccc);
  Actor_WalkToAndWait(21, 168, 0x278);
  Actor_SetActionScript(21, gTolbiShipDeck_LeaveActions);
  Engine_EventWait(80);
  Actor_SetActionScript(26, gTolbiShipDeck_LeaveActions);
  Engine_EventWait(40);
  TolbiShipDeck_Turn(20, FACING_WEST);
  TolbiShipDeck_Say(SPEAKER(20, 2));
  TolbiShipDeck_Turn(ACTOR_PARTY_LEADER, FACING_NORTHEAST);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(20, 3);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(17);
}

/// Places the leader on the deck facing south, with the camera on them.
static void TolbiShipDeck_PlaceLeaderOnDeck(void) {
  struct FieldActor *record;

  Camera_MoveTo(PIXELS(232), -1, PIXELS(676), 0);
  Engine_MapRedraw();
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(232), PIXELS(676));
  record = Object_GetById(0);
  record->facing = FACING_SOUTH;
  Engine_TaskWait(1);
}

const s32 gTolbiShipDeck_PresentationActionsA[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(202), 0, PIXELS(600)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_END,
};

const s32 gTolbiShipDeck_PresentationActionsB[] = {
    SC_WAIT(20), SC_ARG(2), SC_MOVE(PIXELS(162), 0, PIXELS(666)),
    SC_SYNC,     SC_ARG(1), SC_SET(SW_FACING, FACING_WEST),
    SC_END,
};

const s32 gTolbiShipDeck_RandomActorActions[] = {
    SC_CALL(Scene_SquashAndStretch),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gTolbiShipDeck_DeckEventActions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(140), 0, PIXELS(586)),
    SC_SYNC,
    SC_SE(153),
    SC_SET(SW_VELOCITY_Y, 0x80000),
    SC_MOVE(PIXELS(184), 0, PIXELS(598)),
    SC_SYNC,
    SC_SET(0x23, 1),
    SC_SET(SW_VELOCITY_Y, 0x40000),
    SC_CALL(TolbiShipDeck_ShowSprite),
    SC_MOVE(PIXELS(216), 0, PIXELS(596)),
    SC_SYNC,
    SC_MOVE(PIXELS(216), 0, PIXELS(586)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_END,
};

const s32 gTolbiShipDeck_LeadActionsA[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(228), 0, PIXELS(521)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xd000),
    SC_WAIT(40),
    SC_ARG(4),
    SC_WAIT(40),
    SC_ARG(2),
    SC_MOVE(PIXELS(204), 0, PIXELS(521)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xb000),
    SC_WAIT(40),
    SC_ARG(4),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gTolbiShipDeck_LeadActionsB[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(672)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(70),
    SC_SET(SW_FACING, 0xb000),
    SC_WAIT(50),
    SC_ARG(2),
    SC_MOVE(PIXELS(196), 0, PIXELS(672)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xd000),
    SC_WAIT(30),
    SC_SET(SW_FACING, 0x3000),
    SC_WAIT(70),
    SC_ARG(2),
    SC_MOVE(PIXELS(196), 0, PIXELS(636)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(50),
    SC_SET(SW_FACING, 0x5000),
    SC_WAIT(30),
    SC_ARG(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(636)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xb000),
    SC_WAIT(70),
    SC_SET(SW_FACING, 0x3000),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gTolbiShipDeck_CrewActionsA[] = {
    SC_SET(SW_MOTION, 0), SC_SET(0x23, 0),
    SC_SET(0x24, 0),      SC_SET(0x27, 0),
    SC_SET(0x17, 0),      SC_CALL(TolbiShipDeck_HoverAndDiveAtOuranos),
};

const s32 gTolbiShipDeck_CrewActionsB[] = {
    SC_SET(SW_MOTION, 0), SC_SET(0x23, 0),
    SC_SET(0x24, 0),      SC_SET(0x27, 0),
    SC_SET(0x17, 0),      SC_CALL(TolbiShipDeck_HoverAndDiveAtSean),
};

const s32 gTolbiShipDeck_CrewActionsC[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(0x24, 0),
    SC_CALL(TolbiShipDeck_Bob),
};

const s32 gTolbiShipDeck_CrewActionsD[] = {
    SC_CALL(TolbiShipDeck_LookAround),
};

const s32 gTolbiShipDeck_CrewActionsE[] = {
    SC_ARG(1),
    SC_SET(SW_SCALE_X, 0xb333),
    SC_SET(SW_SCALE_Y, 0xb333),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 3932),
    SC_ADD(SW_SCALE_Y, 0x147a),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

const s32 gTolbiShipDeck_SailorActions[] = {
    SC_MOVE(PIXELS(216), 0, PIXELS(586)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_END,
};

const s32 gTolbiShipDeck_DeckActionsB[] = {
    SC_SET(0x23, 0),
    SC_CALL(TolbiShipDeck_LeapAtOuranos),
};

const s32 gTolbiShipDeck_DeckActionsA[] = {
    SC_SET(0x23, 0),
    SC_CALL(TolbiShipDeck_LeapAtSean),
};

const s32 gTolbiShipDeck_DeckActionsC[] = {
    SC_CALL(TolbiShipDeck_RandomlyStartActorHop),
};

const s32 gTolbiShipDeck_RosterActions[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(82), 0, PIXELS(656)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_END,
};

const s32 gTolbiShipDeck_LeaveActions[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(186), 0, PIXELS(604)),
    SC_SYNC,         SC_MOVE(PIXELS(216), 0, PIXELS(604)),
    SC_SYNC,         SC_MOVE(PIXELS(216), 0, PIXELS(586)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_WAIT(1),
    SC_END,
};

const s32 gTolbiShipDeck_LeadActionsC[] = {
    SC_SET(0x27, 0),
    SC_CALL(TolbiShipDeck_IdleTurn),
};

const struct SceneEntrance gTolbiShipDeckEntrances[] = {
    {0, CONDITION_ALWAYS, 179, 0, 653, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 216, 0, 600, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 232, 0, 664, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 216, 0, 808, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 216, 4, 648, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 312, -4, 656, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 120, -4, 656, FACING_EAST, 0, 65, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 232, 0, 664, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 212, 0, 521, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 192, 0, 650, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {13, CONDITION_ALWAYS, 232, 0, 654, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {14, CONDITION_ALWAYS, 216, 0, 657, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {15, CONDITION_ALWAYS, 216, 0, 640, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {16, CONDITION_ALWAYS, 216, 0, 540, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {17, CONDITION_ALWAYS, 216, 0, 540, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {18, CONDITION_ALWAYS, 196, 0, 652, FACING_SOUTH, 0, 65, -1, -1, -1, 0},
    {19, CONDITION_ALWAYS, 212, 0, 521, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
const s16 gTolbiShipDeckRegions[] = {
    208, 24, 634, 224, 40, 650, -1, 5,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

const u32 gTolbiShipDeckExits[] = {
    SCENE_EXITS(109),
    SCENE_EXIT(1, 111, 1),
    SCENE_EXIT(2, 111, 3),
    SCENE_EXIT(3, 111, 5),
    SCENE_EXIT(4, 107, 2),
    SCENE_EXIT(5, 110, 1),
    SCENE_EXIT(6, 112, 1),
    SCENE_EXIT(7, 2, 74),
    SCENE_EXIT(10, 110, 10),
    SCENE_EXIT(11, 111, 14),
    SCENE_EXIT(12, 111, 15),
    SCENE_EXIT(13, 111, 17),
    SCENE_EXIT(14, 111, 19),
    SCENE_EXIT(15, 2, 70),
    SCENE_EXIT(16, 111, 22),
    SCENE_EXIT(17, 111, 24),
    SCENE_EXITS_END,
};

const struct ScenePlacement gTolbiShipDeckPlacements[] = {
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x91, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(266), 0, PIXELS(703), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeckPlacementsWaiting[] = {
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x91, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(183), 0, PIXELS(720), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x92, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(256), 0, PIXELS(712), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x2e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(198), 0, PIXELS(706), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(172), 0, PIXELS(706), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x84, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(230), 0, PIXELS(602), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x94, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(236), 0, PIXELS(621), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(182), 0, PIXELS(556), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeckPlacementsRowing[] = {
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeckPlacementsAtSea[] = {
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_CrewScript[] = {
    { 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(512), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x43, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(634), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x42, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(212), 0, PIXELS(521), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x92, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_ClosingCrewScript[] = {
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_CrewScriptB[] = {
    { 0xbd, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbd, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbd, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_CrewScriptC[] = {
    { 0xc4, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc4, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc4, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc4, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc4, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc4, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xc6, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd6, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_CrewScriptD[] = {
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc5, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_CrewScriptE[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x43, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(178), 0, PIXELS(642), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x42, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(182), 0, PIXELS(534), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x92, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc2, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(540), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(640), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeckPlacementsArrived[] = {
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(188), 0, PIXELS(512), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x91, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(162), 0, PIXELS(634), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x92, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(162), 0, PIXELS(676), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
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
    { 0x16, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

const struct SceneEvent gTolbiShipDeckEvents[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { TOUCH_WALKING_INTO(FACING_WEST), 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#else
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_TALK, 20, FLAG_TOLBI_SHIP_CREWMAN_CONFRONTED, MSGNO(MsgTolbiShipDeckThereAreALotOfPassengers) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckOhAtTheRiskOfSounding) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, FLAG_TOLBI_SHIP_CREWMAN_CONFRONTED, MSGNO(MsgTolbiShipDeckTheReplacementShipFromTolbiShould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIHidTheAnchorCharmNo) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeApologizeForAnyTroubleThat) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_TalkToWaitingPassenger21) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_TalkToWaitingPassenger24) },
    { EVENT_TALK, 25, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckIWonderWhatsGoingOnWeve) },
    { EVENT_TALK, 25, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckThoseWarriorsGotTiredOfWaiting) },
    { EVENT_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIfWeJustPromiseToHelp) },
    { EVENT_TALK, 26, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckIWonderIfImTheOnly) },
    { EVENT_TALK, 26, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckIWonderIfThoseTwoAre) },
    { EVENT_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIllAgreeToAnythingEvenRowing) },
    { EVENT_TALK, 27, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckTheKaragolIsLikeAHumongous) },
    { EVENT_TALK, 27, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckHaHahThoseMercenariesAreGoing) },
    { EVENT_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThatSeaDogKajaIsA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThereIsNothingToFearAs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckWhenAreWeGoingWhenAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckUsingForceToLaunchTheShip) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckAreYouSeriousAboutSettingSail) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckHmmmThisIsSoFrustratingWhy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckTheyWereJustAboutToLaunch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckItProbablyWontMakeAnyDifference) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckTravelingByShipIsFasterThan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckThoseThugsAreTooImpatientTrying) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUltimatelyTheyreJustBeingThreatenedInto) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckMostOfThePeopleGoingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckHeeHeeIfWeCanGet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckBesidesRowingIsAMansJob) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN, MSGNO(MsgTolbiShipDeckTheKaragolSeaIsPrettyFoggy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED, MSGNO(MsgTolbiShipDeckThoseGuysSeemPrettySureOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUsHelpWithTheRowingGet) },
    { EVENT_TOUCH, 10, FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY, EVENT_SCRIPT(TolbiShipDeck_WatchCrewDecideToSail) },
    { EVENT_TOUCH, 11, FLAG_TOLBI_SHIP_CREWMAN_CONFRONTED, EVENT_SCRIPT(TolbiShipDeck_CatchCrewmanFromEast) },
    { EVENT_TOUCH, 12, FLAG_TOLBI_SHIP_CREWMAN_CONFRONTED, EVENT_SCRIPT(TolbiShipDeck_CatchCrewmanFromWest) },
    { SCENE_EVENTS_END },
};

const struct SceneEvent gTolbiShipDeckEventsAtSea[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { TOUCH_WALKING_INTO(FACING_WEST), 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#else
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckYouMustFeelAwfullyImportantOrdering) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckItLooksLikeAnotherOarsmanWas) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIFeelSorryForTheOarsmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckAnOarsmanWasInjuredByA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeHaveNoIdeaWhenThose) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUhnnnWhenILookAtThe) },
    { SCENE_EVENTS_END },
};

const struct SceneEvent gTolbiShipDeckEventsArrived[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { TOUCH_WALKING_INTO(FACING_WEST), 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#else
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckYouLookinForSeanAndOuranos) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_AskHeadedForColosso) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThisGroupThatCameOnOur) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWantSeanAndOuranosTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckImJustGoingToHangOut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWonderWhatHappenedToThat) },
    { SCENE_EVENTS_END },
};

const struct SceneEvent gTolbiShipDeckEventsAtIsland[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#if !TBS_PATCH_2
    { TOUCH_WALKING_INTO(FACING_WEST), 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
#else
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelowWhileFacingWest) },
#endif
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_GoBelow) },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 7 },
    { EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckKajaAndTheCaptainAreProfessionals) },
    { EVENT_TALK, 22, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, EVENT_SCRIPT(TolbiShipDeck_OfferToScoutIsland) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckTheresNoTellingWhatKindOf) },
    { EVENT_TALK, 21, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, EVENT_SCRIPT(TolbiShipDeck_OfferToScoutIsland) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeveDoneAGoodJobOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWantToBeASailor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, MSGNO(MsgTolbiShipDeckIGiveUpWhatAreWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThisIslandGivesMeTheCreeps) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED, MSGNO(MsgTolbiShipDeckThatsEnoughBickering) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWhatWeCanDoNowIs) },
    { SCENE_EVENTS_END },
};
// clang-format on

/// How hard each passenger rows, for actors 23 to 31: 1 leaves the oar still,
/// 2 to 4 row ever faster.
const s32 gTolbiShipDeckRowerStrokes[] = {3, 2, 3, 1, 4, 4, 2, 1, 1};
