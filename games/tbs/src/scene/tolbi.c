// Tolbi, the town of the Colosso festival. The town has three moods, chosen
// by story flags: the festival during the trials, finals day, and the quiet
// after Colosso has ended. Each mood has its own event table; the actors and
// their scripts are shared.

#include "unit/scene/tolbi.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
#include "tolbi.h"
#include "iwram_call.h"
#include "call.h"
#include "field_event.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "edition.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"

static void Tolbi_FreezePuddle(void);

// Talking to Ember, the Mars Djinni hiding in town, makes it join the party.
// Its index among the Mars Djinn is 4, the fifth Mars name in the game text.
static void Tolbi_JoinEmber(void) {
  Djinn_JoinParty(TOLBI_EMBER, 2, 4);
}

#include "common/object_distance.h"

// Stops a running child when the target is close by and ahead of it, or
// always when force is set; otherwise starts the child running again.
// The child stops when the target is within one facing step of where it
// looks; for the party leader, within two steps either side.
// Returns 1 when the child stopped.
static s32 Tolbi_KidStopForTarget(struct TolbiKid *kid, struct TolbiKid *target,
                                  s32 range, s32 force) {
  s32 stopped = 0;
  s32 *targetPos = &target->x;
  s32 *kidPos = &kid->x;

  if (FixedPoint_Distance(targetPos, kidPos) < range || force != 0) {
    // The heading to the target, and the eight-way directions around it.
    u32 heading = (u16)ArcTan2(target->z - kid->z, *targetPos - *kidPos);
    u32 farLeft = (heading - 2 * FACING_STEP) & 0xf000;
    u32 farRight = (heading + 2 * FACING_STEP) & 0xf000;
    u32 left = (heading - FACING_STEP) & 0xf000;
    u32 right = (heading + FACING_STEP) & 0xf000;
    u32 ahead = heading & 0xf000;
    u32 facing = kid->facing & 0xf000;

    if (ahead == facing || right == facing || left == facing || force != 0) {
      kid->paused = 1;
      Object_SetMode(kid, ANIM_STAND);
      stopped = 1;
    }
    if (target == Object_GetById(ACTOR_PARTY_LEADER) &&
        (farRight == facing || farLeft == facing)) {
      kid->paused = 1;
      Object_SetMode(kid, ANIM_STAND);
      stopped = 1;
    }
  } else {
    kid->paused = 0;
    Object_SetMode(kid, ANIM_WALK);
  }
  return stopped;
}

// The update of the two children running around the plaza. Each stops when
// it runs up behind the other, and otherwise when the party leader is in its
// way. While a window is up (the range widens to 26 pixels) the child the
// party is talking to stops whatever the direction.
static s32 Tolbi_UpdateKid(struct TolbiKid *self) {
  u8 **workAreas = (u8 **)gWindowWork;
  struct TolbiUiWork *ui = (struct TolbiUiWork *)workAreas[0];
  u8 *work12 = workAreas[12];
  u16 *kidFlags = &self->kidFlags;
  s32 force = 0;
  s32 range = 18;
  struct TolbiKid *other;
  struct TolbiKid *leader;

  if ((*kidFlags & KID_CHASES_YOUNGER) != 0) {
    other = Object_GetById(TOLBI_YOUNGER_SIBLING);
  } else {
    other = Object_GetById(TOLBI_OLDER_BROTHER);
  }
  if (Tolbi_KidStopForTarget(self, other, 32, 0) != 0) {
    return 0;
  }

  leader = Object_GetById(ACTOR_PARTY_LEADER);

  // The halfword at 376 in work area 12 is another window state.
  if (*(s16 *)(work12 + 376) != 0 || ui->renderMode != 0) {
    range = 26;
    if ((*kidFlags & KID_TALKING) != 0) {
      force = 1;
    }
  }

  Tolbi_KidStopForTarget(self, leader, range, force);
  return 0;
}

// The scene's entrances.
u8 *Tolbi_GetEntrances(void) { return (u8 *)gTolbiEntrances; }

// Tolbi has no regions.
s32 Tolbi_GetRegions(void) { return 0; }

// The scene's exits.
u8 *Tolbi_GetExits(void) { return (u8 *)gTolbiExits; }

// The scene's actor placements.
u8 *Tolbi_GetPlacements(void) { return (u8 *)gTolbiPlacements; }

// Growth on the sprout: the grown plant no longer blocks the party, and its
// map cell becomes climbable.
static void Tolbi_GrowSprout(void) {
  struct FieldActor *sprout;

  sprout = Object_GetById(TOLBI_SPROUT);
  if ((s32)sprout != 0) {
    sprout->collisionFlags = 0;
  }
  sprout = Object_GetById(TOLBI_SPROUT);
  Engine_ActorSetSpriteFlags((s32)sprout, 0);
  Field_SetMapCellCollision(0, PIXELS(544), PIXELS(288), 253);
  Engine_GameFlagSet(FLAG_TOLBI_SPROUT_GROWN);
}

// Reveal uncovers the hidden chest: its map cell is drawn in and the chest
// moves into place.
static void Tolbi_RevealHiddenChest(void) {
  Map_CopyCellAttributes(3, 32, 1, 1, 3, 26);
  MapObject_SetPosition(TOLBI_HIDDEN_CHEST, PIXELS(56), PIXELS(424));
}

// When Reveal wears off, the chest's cell is covered again and the chest
// moves out of sight.
static void Tolbi_HideRevealedChest(void) {
  Map_CopyCellAttributes(2, 25, 1, 1, 3, 26);
  MapObject_SetPosition(TOLBI_HIDDEN_CHEST, -1, -1);
}

// The first sign at Tolbi Spring: face away and toss a coin in.
static void Tolbi_ReadSpringSign(void) {
  Engine_EventSetMessage(MsgTolbiFaceAwayTolbi);
  Engine_EventShowMessage(-1, 0);
}

// The second sign at Tolbi Spring: a Lucky Medal wins better prizes.
static void Tolbi_ReadLuckyMedalSign(void) {
  Engine_EventSetMessage(MsgTolbiTossLuckyMedal);
  Engine_EventShowMessage(-1, 0);
}

// Talking to the older brother, or reading his mind. He stops running while
// he speaks, then turns back the way he was going.
static void Tolbi_TalkToOlderBrother(void) {
  struct EventWork *work = gEventWork;
  struct TolbiKid *kid;
  s16 facing;
  s32 msg;

  kid = Object_GetById(TOLBI_OLDER_BROTHER);
  facing = kid->facing;
  Engine_EventBegin();
  kid->kidFlags |= KID_TALKING;
  if (work->psynergyRequest == 0) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_ENDED) != 0) {
      msg = MsgTolbiYayEasyRun;
    } else if (Engine_GameFlagIsSet(FLAG_COLOSSO_FINALS_DAY) != 0) {
      msg = MsgTolbiHoorayFinalsSeven;
    } else {
      msg = MsgTolbiWheeFestivalColosso;
    }
  } else {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_ENDED) != 0) {
      msg = MsgTolbiMainStreetTolbi;
    } else if (Engine_GameFlagIsSet(FLAG_COLOSSO_FINALS_DAY) != 0) {
      msg = MsgTolbiWinBigUsed;
    } else {
      msg = MsgTolbiOldestShouldntShare;
    }
  }
  Engine_EventSetMessage(msg);
  Engine_ActorSetAnimation(TOLBI_OLDER_BROTHER, 0);
  Engine_ActorFaceEachOther(TOLBI_OLDER_BROTHER, ACTOR_PARTY_LEADER, 2);
  Engine_EventShowMessageAndWait(TOLBI_OLDER_BROTHER, 0, 10);
  kid->facing = facing;
  Engine_TaskWait(1);
  kid->kidFlags &= KID_CHASES_YOUNGER;
  Engine_EventEnd();
}

// Talking to the younger sibling, or reading the child's mind, as above.
static void Tolbi_TalkToYoungerSibling(void) {
  struct EventWork *work = gEventWork;
  struct TolbiKid *kid;
  s16 facing;
  s32 msg;

  kid = Object_GetById(TOLBI_YOUNGER_SIBLING);
  facing = kid->facing;
  Engine_EventBegin();
  kid->kidFlags |= KID_TALKING;
  if (work->psynergyRequest == 0) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_ENDED) != 0) {
      msg = MsgTolbiHaHaHa;
    } else if (Engine_GameFlagIsSet(FLAG_COLOSSO_FINALS_DAY) != 0) {
      msg = MsgTolbiWaahBigBrother;
    } else {
      msg = MsgTolbiWaahSaidMoney;
    }
  } else {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_ENDED) != 0) {
      msg = MsgTolbiBrotherMeanDuring;
    } else if (Engine_GameFlagIsSet(FLAG_COLOSSO_FINALS_DAY) != 0) {
      msg = MsgTolbiMamaWhyListen;
    } else {
      msg = MsgTolbiMamaToldShare;
    }
  }
  Engine_EventSetMessage(msg);
  Engine_ActorSetAnimation(TOLBI_YOUNGER_SIBLING, 0);
  Engine_ActorFaceEachOther(TOLBI_YOUNGER_SIBLING, ACTOR_PARTY_LEADER, 2);
  Engine_EventShowMessageAndWait(TOLBI_YOUNGER_SIBLING, 0, 10);
  kid->facing = facing;
  Engine_TaskWait(1);
  kid->kidFlags &= KID_CHASES_YOUNGER;
  Engine_EventEnd();
}

// Picks the event table for Tolbi's current mood.
u8 *Tolbi_SelectEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_ENDED) != 0) {
    return (u8 *)gTolbiEventsAfterColosso;
  }
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_FINALS_DAY) != 0) {
    return (u8 *)gTolbiEventsFinalsDay;
  }
  return (u8 *)gTolbiEventsFestival;
}

// A townsperson asks whether someone can live to be more than 100.
static void Tolbi_TalkToActor15(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiSomeoneLiveMore);
  Engine_EventAskYesNo(15, 0);
  Engine_EventEnd();
}

// A parent asks whether children really do love souvenirs.
static void Tolbi_TalkToActor24(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiChildrenLoveSouvenirs);
  Engine_EventAskYesNo(24, 0);
  Engine_EventEnd();
}

// A parent answers the party, then turns to the crying child: "Be patient,
// little guy."
static void Tolbi_TalkToActor25(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiPatientLittleGuy);
  Actor_FaceDirection(25, FACING_NORTH, 0);
  Engine_EventShowMessage(25, 0);
  Actor_FaceDirection(25, FACING_WEST, 0);
  Engine_EventShowMessage(25, 0);
  Engine_EventEnd();
}

// The child begging for sweets turns and stamps twice.
static void Tolbi_TalkToActor26(void) {
  Engine_EventBegin();
  Actor_FaceDirection(26, FACING_SOUTH, 0);
  Engine_ActorStartRepeatedMotion(26, 2);
  Engine_EventSetMessage(MsgTolbiWaahBuySweets);
  Engine_EventShowMessage(26, 0);
  Engine_EventEnd();
}

// The traveller sleeping under the archway because the inns are full.
static void Tolbi_TalkToActor27(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiInnsFullStaying);
  Engine_EventShowMessage(27, 0);
  Engine_EventEnd();
}

// After Colosso, a man asks whether the party was in the finals. The question
// is anchored to actor 24, who has left town by then, not to actor 12.
static void Tolbi_TalkToActor12(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiFinalsWerent);
  Engine_EventAskYesNo(24, 0);
  Engine_EventEnd();
}

// After Colosso, the snack vendor gives the party her leftover corn, once.
static void Tolbi_TalkToSnackVendor(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_TOLBI_CORN_GIVEN) == 0) {
    Engine_GameFlagSet(FLAG_TOLBI_CORN_GIVEN);
    Engine_EventSetMessage(MsgTolbiLeftovers);
    Engine_EventShowMessage(TOLBI_SNACK_VENDOR, 0);
    Engine_ItemShowFound(233, 3); // corn
    Engine_EventShowMessage(TOLBI_SNACK_VENDOR, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Engine_PartyGiveItem(233, 0);
  } else {
    Engine_EventSetMessage(MsgTolbiFestivalLongerUsual);
    Engine_EventShowMessage(TOLBI_SNACK_VENDOR, 0);
  }
  Engine_EventEnd();
}

// Walking into one of the town's doors: the door opens, the leader steps in
// (except at the seventh door) and the scene changes to the door's exit.
static void Tolbi_EnterDoor(void) {
  struct EventWork *work = gEventWork;
  u32 actorId;
  s32 door;

  Engine_EventBegin();
  for (actorId = ACTOR_FIRST_PLACED; actorId <= 65; actorId++) {
    struct FieldActor *actor = Object_GetById(actorId);
    if (actor != NULL) {
      actor->motionFlags = 0;
    }
  }
  door = work->touchedTrigger - 1;
  Engine_AudioPlayCue(158);
  Map_AnimateCells((s32)gTolbiDoors[door].steps, gTolbiDoors[door].x,
                   gTolbiDoors[door].y);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (door != 6) {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
    Engine_EventWait(10);
  }
  Engine_EventRequestExit(work->touchedTrigger);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

// The update of actor 14: gives it the leader's sprite priority every frame,
// for both of its sprite parts.
static void Tolbi_CopyLeaderPriority(union FieldObject *object) {
  s32 priority;

  if (object != NULL) {
    priority = Object_GetById(ACTOR_PARTY_LEADER)->sprite->priority;
    object->actor.priorityFlags = 0;
    object->actor.sprite->priority = priority;
    ((struct TolbiSpritePriorityByte *)object->actor.sprite)->secondPriority =
        priority;
  }
}

// Sets up the scene on entry: the running children, actor 14's update, and
// everything the flags have already changed.
s32 Tolbi_ApplyEntryState(s32 unused) {
  struct FieldActor *actor;
  void (*kidUpdate)(union FieldObject *);
  s32 clearFlags;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  // The two children run their loops around the plaza.
  Actor_SetPosition(TOLBI_OLDER_BROTHER, PIXELS(352), PIXELS(352));
  Actor_SetActionScript(TOLBI_OLDER_BROTHER, gTolbiOlderBrotherRun);
  actor = Object_GetById(TOLBI_OLDER_BROTHER);
  kidUpdate = (void (*)(union FieldObject *))Tolbi_UpdateKid;
  ((struct TolbiKid *)actor)->kidFlags = KID_CHASES_YOUNGER;
  actor->update = kidUpdate;
  clearFlags = 0;
  Engine_ActorSetPosition(TOLBI_YOUNGER_SIBLING, PIXELS(368), PIXELS(320));
  Actor_SetActionScript(TOLBI_YOUNGER_SIBLING, gTolbiYoungerSiblingRun);
  actor = Object_GetById(TOLBI_YOUNGER_SIBLING);
  ((struct TolbiKid *)actor)->kidFlags = clearFlags;
  actor->update = kidUpdate;
  actor = Object_GetById(TOLBI_PLAYER_COPY);
  actor->update = Tolbi_CopyLeaderPriority;

  // What the party has already done here.
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SPRING_OPENED) != 0) {
    Actor_SetPosition(TOLBI_GAMBLER, PIXELS(316), PIXELS(328));
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_PUDDLE_FROZEN) != 0) {
    Tolbi_FreezePuddle();
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SPROUT_GROWN) != 0) {
    Tolbi_GrowSprout();
    Engine_ActorSetAnimation(TOLBI_SPROUT, 4);
  }

  // After Colosso the crowd has gone home: actors 20 to 27 (but 23) are
  // moved off the map. On finals day actor 27 waits by the gate.
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_ENDED) != 0) {
    Actor_SetPosition(20, PIXELS(520), PIXELS(560));
    Actor_SetPosition(21, PIXELS(520), PIXELS(560));
    Actor_SetPosition(22, PIXELS(520), PIXELS(560));
    Actor_SetPosition(24, PIXELS(520), PIXELS(560));
    Actor_SetPosition(25, PIXELS(520), PIXELS(560));
    Actor_SetPosition(26, PIXELS(520), PIXELS(560));
    Actor_SetPosition(27, PIXELS(520), PIXELS(560));
  } else {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_FINALS_DAY) != 0) {
      Actor_SetPosition(27, PIXELS(280), PIXELS(80));
      Actor_FaceDirection(27, FACING_SOUTHEAST, 0);
      Engine_ActorSetAnimation(27, ANIM_STAND);
    }
  }
  return 0;
}

// The gambler at the spring's gate asks whether the party wants to test its
// luck. On yes he steps aside for good and the way to Tolbi Spring opens.
static void Tolbi_TalkToGambler(void) {
  Engine_EventBegin();
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 304, 312);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_ActorFaceDirection(TOLBI_GAMBLER, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgTolbiWantTestLuck);
  Event_OpenMessage(TOLBI_GAMBLER, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    // Yes: "I'll give you the chance to go first."
    gEventWork->message++;
    Engine_EventShowMessage(TOLBI_GAMBLER, 0);
    Actor_SetSpeed(TOLBI_GAMBLER, 0x10000, 0x8000);
    Actor_WalkToAndWait(TOLBI_GAMBLER, 320, 304);
    Actor_WalkToAndWait(TOLBI_GAMBLER, 316, 328);
    Engine_ActorFaceDirection(TOLBI_GAMBLER, FACING_NORTHWEST, 0);
    Engine_GameFlagSet(FLAG_TOLBI_SPRING_OPENED);
  } else {
    Engine_EventShowMessage(TOLBI_GAMBLER, 0);
  }
  Engine_EventEnd();
}

// The gate to Tolbi Spring, once the gambler has stepped aside.
static void Tolbi_EnterSpring(void) {
  Engine_EventRequestExit(30);
  Engine_EventEnd();
}

// Leaves by the exit numbered as the trigger the party walked into.
static void Tolbi_RequestTouchedExit(void) {
  s16 *trigger = &gEventWork->touchedTrigger;

  Engine_EventRequestExit(*trigger);
}

// During the festival, two soldiers searching for the missing Babi stop the
// party and show his picture. They leave either way the party answers.
static void Tolbi_MeetSearchingSoldiers(void) {
  s32 firstMsg;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Actor_SetSpeed(TOLBI_SOLDIER_A, 0x10000, 0x8000);
  Engine_ActorSetSpeed(TOLBI_SOLDIER_B, 0x10000, 0x8000);
  firstMsg = MsgTolbiSeenAnyoneWho;
  Engine_EventSetMessage(firstMsg);

  // The soldiers come up to the party; the poster waits off to the side.
  Actor_SetPosition(TOLBI_SOLDIER_A, PIXELS(72), PIXELS(208));
  Actor_SetPosition(TOLBI_SOLDIER_B, PIXELS(56), PIXELS(208));
  Engine_ActorSetChildValue(TOLBI_MISSING_PERSON_POSTER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(TOLBI_MISSING_PERSON_POSTER), 0);
  Actor_SetPosition(TOLBI_MISSING_PERSON_POSTER, PIXELS(95), PIXELS(40));
  Engine_ActorWalkTo(TOLBI_SOLDIER_A, 72, 248);
  Engine_ActorWalkTo(TOLBI_SOLDIER_B, 56, 248);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 64, 264);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_ActorWaitForMove(TOLBI_SOLDIER_A);
  Engine_ActorSetAnimation(TOLBI_SOLDIER_A, ANIM_STAND);
  Engine_ActorSetAnimation(TOLBI_SOLDIER_B, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_ActorFaceActor(TOLBI_SOLDIER_A, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceActor(TOLBI_SOLDIER_B, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(TOLBI_SOLDIER_A, 0x102);
  Actor_SetAttachedEffect(TOLBI_SOLDIER_B, 0x102);
  Engine_ActorStartRepeatedMotion(TOLBI_SOLDIER_A, 2);
  Engine_ActorRunRepeatedMotion(TOLBI_SOLDIER_B, 2);
  Engine_EventWait(20);

  // "Have you seen anyone who looks like this?", under a "Missing Person"
  // window showing the poster.
  Event_OpenMessage(TOLBI_SOLDIER_A, 0);
  Engine_EventWait(25);
  UiWindow_CreateWithSideObject(52, 0, 12, 7);
  UiText_OpenMessageWindow((firstMsg + 3), 11, 12, 2);
  EVENT_WORK_1F4 = TOLBI_MISSING_PERSON_POSTER;
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    // Yes: "Babi must be nearby!!!" They nod and run east.
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(TOLBI_SOLDIER_B, 2);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(TOLBI_SOLDIER_B, FACING_EAST, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(TOLBI_SOLDIER_A, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(TOLBI_SOLDIER_A, FACING_EAST, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(TOLBI_SOLDIER_A, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(TOLBI_SOLDIER_A, FACING_SOUTH, 0);
    Engine_ActorFaceDirection(TOLBI_SOLDIER_B, FACING_SOUTH, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(TOLBI_SOLDIER_A, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(TOLBI_SOLDIER_B, ANIM_NOD);
    Engine_EventWait(20);
    Actor_SetSpeed(TOLBI_SOLDIER_A, 0x1cccc, 0xe666);
    Actor_SetSpeed(TOLBI_SOLDIER_B, 0x1cccc, 0xe666);
    Actor_WalkTo(TOLBI_SOLDIER_A, 232, 248);
    Engine_EventWait(2);
    Actor_WalkTo(TOLBI_SOLDIER_B, 232, 248);
    Engine_ActorWaitForMove(TOLBI_SOLDIER_A);
    Engine_ActorWalkTo(TOLBI_SOLDIER_A, 248, 248);
    Engine_ActorWalkToAndWait(TOLBI_SOLDIER_B, 248, 248);
  } else {
    // No: "He doesn't seem to be here. Let's look over there." They shake
    // their heads and go back north.
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(TOLBI_SOLDIER_B, 2);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(TOLBI_SOLDIER_B, FACING_EAST, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(TOLBI_SOLDIER_A, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(TOLBI_SOLDIER_A, FACING_EAST, 0);
    Engine_EventWait(30);
    gEventWork->message += 1;
    Engine_EventShowMessage(TOLBI_SOLDIER_A, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(TOLBI_SOLDIER_A, FACING_SOUTH, 0);
    Actor_FaceDirection(TOLBI_SOLDIER_B, FACING_SOUTH, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(TOLBI_SOLDIER_A, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(TOLBI_SOLDIER_B, ANIM_NOD);
    Engine_EventWait(20);
    Actor_SetSpeed(TOLBI_SOLDIER_A, 0x19999, 0xcccc);
    Actor_SetSpeed(TOLBI_SOLDIER_B, 0x19999, 0xcccc);
    Engine_ActorWalkTo(TOLBI_SOLDIER_A, 72, 184);
    Engine_ActorWalkToAndWait(TOLBI_SOLDIER_B, 56, 184);
  }
  Engine_ActorSetPosition(TOLBI_SOLDIER_A, 0, 0);
  Engine_ActorSetPosition(TOLBI_SOLDIER_B, 0, 0);
  Engine_ActorSetPosition(TOLBI_MISSING_PERSON_POSTER, 0, 0);
  Engine_GameFlagSet(FLAG_TOLBI_SEARCHERS_MET);
  Engine_EventEnd();
}

// Frost on the puddle: the frozen pillar is drawn underfoot and stops
// following the terrain, and its map cell becomes solid.
static void Tolbi_FreezePuddle(void) {
  struct FieldActor *puddle = Object_GetById(TOLBI_PUDDLE);
  if (puddle != 0) {
    Engine_ActorSetSpriteFlags(puddle, 0);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    puddle->motionFlags = 0;
  }
  Engine_ActorSetAnimation(TOLBI_PUDDLE, 5);
  {
    Map_CopyCellAttributes(36, 16, 1, 1, 34, 16);
  }
  Engine_GameFlagSet(FLAG_TOLBI_PUDDLE_FROZEN);
}

// The action script of a mirrored actor: flips its sprite and passes 7 on.
s32 gTolbiMirroredActor[] = {
    SC_SET(SW_SCALE_X, -0x10000),
    SC_ARG(7),
    SC_END,
};

// The older brother runs a loop of eight points around the plaza.
s32 gTolbiOlderBrotherRun[] = {
    SC_ARG(2),
    SC_SET(SW_SPEED, 0xcccc),
    SC_SET(SW_ACCEL, 0x3333),
    SC_LABEL(1),
    SC_MOVE(PIXELS(352), 0, PIXELS(352)),
    SC_SYNC,
    SC_MOVE(PIXELS(368), 0, PIXELS(352)),
    SC_SYNC,
    SC_MOVE(PIXELS(376), 0, PIXELS(344)),
    SC_SYNC,
    SC_MOVE(PIXELS(376), 0, PIXELS(328)),
    SC_SYNC,
    SC_MOVE(PIXELS(368), 0, PIXELS(320)),
    SC_SYNC,
    SC_MOVE(PIXELS(352), 0, PIXELS(320)),
    SC_SYNC,
    SC_MOVE(PIXELS(344), 0, PIXELS(328)),
    SC_SYNC,
    SC_MOVE(PIXELS(344), 0, PIXELS(344)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

// The younger sibling runs the same loop half a lap ahead.
s32 gTolbiYoungerSiblingRun[] = {
    SC_ARG(2),
    SC_SET(SW_SPEED, 0xcccc),
    SC_SET(SW_ACCEL, 0x3333),
    SC_LABEL(1),
    SC_MOVE(PIXELS(368), 0, PIXELS(320)),
    SC_SYNC,
    SC_MOVE(PIXELS(352), 0, PIXELS(320)),
    SC_SYNC,
    SC_MOVE(PIXELS(344), 0, PIXELS(328)),
    SC_SYNC,
    SC_MOVE(PIXELS(344), 0, PIXELS(344)),
    SC_SYNC,
    SC_MOVE(PIXELS(352), 0, PIXELS(352)),
    SC_SYNC,
    SC_MOVE(PIXELS(368), 0, PIXELS(352)),
    SC_SYNC,
    SC_MOVE(PIXELS(376), 0, PIXELS(344)),
    SC_SYNC,
    SC_MOVE(PIXELS(376), 0, PIXELS(328)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct SceneEntrance gTolbiEntrances[] = {
    {0, CONDITION_ALWAYS, 296, 0, 232, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 304, 0, 448, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 168, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 408, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 536, 0, 232, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 536, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 456, 0, 376, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 368, 0, 416, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 200, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 200, 0, 424, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 88, 0, 392, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 64, 0, 112, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 304, 0, 64, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {13, CONDITION_ALWAYS, 424, 0, 224, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {20, CONDITION_ALWAYS, 304, 0, 304, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {30, CONDITION_ALWAYS, 568, 0, 296, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gTolbiExits[] = {
    SCENE_EXITS(135),      SCENE_EXIT(1, 136, 4),   SCENE_EXIT(2, 136, 2),
    SCENE_EXIT(3, 136, 9), SCENE_EXIT(4, 137, 11),  SCENE_EXIT(5, 136, 1),
    SCENE_EXIT(6, 136, 3), SCENE_EXIT(7, 136, 6),   SCENE_EXIT(8, 136, 5),
    SCENE_EXIT(9, 136, 8), SCENE_EXIT(10, 139, 1),  SCENE_EXIT(11, 142, 1),
    SCENE_EXIT(12, 2, 23), SCENE_EXIT(13, 137, 10), SCENE_EXIT(30, 189, 1),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gTolbiPlacements[] = {
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(552), 0, PIXELS(288), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(552), 0, PIXELS(264), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(264), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(208), 0, PIXELS(312), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(224), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(216), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(152), 0, PIXELS(304), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(24), 0, PIXELS(456), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6b, CONDITION_ALWAYS, (s32)gTolbiOlderBrotherRun,
        PIXELS(344), 0, PIXELS(328), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x66, CONDITION_ALWAYS, (s32)gTolbiYoungerSiblingRun,
        PIXELS(376), 0, PIXELS(328), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(200), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x82, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(400), 0, PIXELS(432), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x82, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x9d, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(184), 0, PIXELS(472), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(256), 0, PIXELS(168), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(184), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(168), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x74, CONDITION_ALWAYS, (s32)gTolbiMirroredActor,
        PIXELS(312), 0, PIXELS(120), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(302), 0, PIXELS(296), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-9), 0, PIXELS(488), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-9), 0, PIXELS(488), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x5c, 0x5c, ACTOR_STAND,
        PIXELS(584), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x34, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-9), 0, PIXELS(488), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

// During the festival, before the finals.
struct SceneEvent gTolbiEventsFestival[] = {
    { PSYNERGY_ON_ACTOR(0xc /* Growth */), TOLBI_SPROUT, FLAG_TOLBI_SPROUT_GROWN, EVENT_SCRIPT(Tolbi_GrowSprout) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_TOUCH, 20, FLAG_TOLBI_SEARCHERS_MET, EVENT_SCRIPT(Tolbi_MeetSearchingSoldiers) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 239, IF_FLAG_SET(FLAG_TOLBI_SPRING_OPENED), EVENT_SCRIPT(Tolbi_EnterSpring) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTolbiTolbisAlwaysReallyBustlingDuringColosso) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTolbiColossoIsHeldInTheColosseum) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTolbiTryoutsStartedAtTheColosseumToday) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiColossoShouldHaveStartedTodayWhat) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiBabiWasAlreadyPrettyOldBack) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToActor15) },
    { EVENT_TALK, TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { EVENT_TALK, TOLBI_YOUNGER_SIBLING, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSibling) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTolbiGetYourColossoSouvenirsTheyMake) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgTolbiGetYourSnacksAndDrinksBefore) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTolbiIveDecidedNotToFightAt) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgTolbiWowTolbisFestivalIsHumongousJust) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiRatsWeOversleptAndNowThe) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToActor24) },
    { EVENT_TALK, 25, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToActor25) },
    { EVENT_TALK, 26, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToActor26) },
    { EVENT_TALK, 27, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToActor27) },
    { EVENT_TALK, TOLBI_GAMBLER, FLAG_TOLBI_SPRING_OPENED, EVENT_SCRIPT(Tolbi_TalkToGambler) },
    { EVENT_TALK, TOLBI_GAMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiAhIGuessIJustDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiTouristsArentTheOnlyOnesWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiTheTrialsAreStartingAtThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiTheTrialsAreOkayButThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiItsNotLikeThereHaveBeen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiEveryYearIGetOlderAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiWhatIfBabiIsActuallyA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_YOUNGER_SIBLING, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSibling) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgTolbiWeHaveToSellAllOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgTolbiTheyreSellingTheseAtTheColosseum) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTolbiISawThePeopleWhoAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTolbiTolbiIsNotLikeXianIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTolbiICantSayAnythingCauseIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgTolbiTheStuffTheyreSellingAtThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgTolbiWhyDoKidsWantSuchAwful) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTolbiThoseSweetsSmellGreatImNot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgTolbiIveBeenGuardingOurSpotBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgTolbiIKeepLosingCoinsButI) },
    { EVENT_TALK, TOLBI_EMBER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_JoinEmber) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadSpringSign) },
    { SEARCH(SEARCH_UNNAMED), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadLuckyMedalSign) },
    { SEARCH(SEARCH_BARREL), TOLBI_BARREL, 0xf97, FIND_COINS(9) },
// The international editions find the jar only when the party faces north.
#if !EDITION_INTERNATIONAL
    { SEARCH(SEARCH_JAR), TOLBI_JAR, 0xf98, FIND_ITEM(ITEM_LUCKY_MEDAL) },
#else
    { SEARCH(SEARCH_JAR) | EVENT_TOUCH_FACING | FACING_NORTH, TOLBI_JAR, 0xf98, FIND_ITEM(ITEM_LUCKY_MEDAL) },
#endif
    { SEARCH(SEARCH_CHEST), TOLBI_HIDDEN_CHEST, 0xf99, FIND_ITEM(0xbf) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), TOLBI_PUDDLE, FLAG_TOLBI_PUDDLE_FROZEN, EVENT_SCRIPT(Tolbi_FreezePuddle) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RevealHiddenChest) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_HideRevealedChest) },
    { SCENE_EVENTS_END },
};

// On finals day.
struct SceneEvent gTolbiEventsFinalsDay[] = {
    { PSYNERGY_ON_ACTOR(0xc /* Growth */), TOLBI_SPROUT, FLAG_TOLBI_SPROUT_GROWN, EVENT_SCRIPT(Tolbi_GrowSprout) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 239, IF_FLAG_SET(FLAG_TOLBI_SPRING_OPENED), EVENT_SCRIPT(Tolbi_EnterSpring) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTolbiColossoWillReachItsApexAny) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTolbiAreYouPlanningToWatchThe) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTolbiMyFamilySavedMeASeat) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiIThoughtSomethingHappenedToBabi) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiTheColosseumWasSoFullThat) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTolbiNobodyElseHasNoticedButThis) },
    { EVENT_TALK, TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { EVENT_TALK, TOLBI_YOUNGER_SIBLING, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSibling) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTolbiHowAboutSomeColossoSouvenirsThis) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgTolbiTheFinalsAreSoLongIts) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTolbiTheTrialsWereSoRivetingThe) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgTolbiIHaveSeatsToTheFinals) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiIOversleptAgainSoICant) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToActor24) },
    { EVENT_TALK, 25, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToActor25) },
    { EVENT_TALK, 26, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToActor26) },
    { EVENT_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgTolbiTheFinalsStartTodayIWonder) },
    { EVENT_TALK, TOLBI_GAMBLER, FLAG_TOLBI_SPRING_OPENED, EVENT_SCRIPT(Tolbi_TalkToGambler) },
    { EVENT_TALK, TOLBI_GAMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiAhIGuessIJustDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiIveGotToGetToThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiABattleToTheDeathWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiTheFinalsStartOnBabisSignal) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiSoWhyWereTheSoldiersAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiICouldStillBuySeatsFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiNoMatterHowIAddIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_YOUNGER_SIBLING, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSibling) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgTolbiWeCantSellAnythingOnceThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgTolbiThisIsTheFinalPushAnything) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTolbiFightingInColossoRequiresALot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTolbiTheyreSellingFinalsSeatsRightAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTolbiItsSupposedToBeFreeHaving) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgTolbiOnceColossoEndsOurLivesCan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgTolbiFestivalsAreSoExpensiveMyWallets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTolbiMomYouSaidYouWereAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgTolbiTodayIsABigDayFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgTolbiIKeepLosingCoinsButI) },
    { EVENT_TALK, TOLBI_EMBER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_JoinEmber) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadSpringSign) },
    { SEARCH(SEARCH_UNNAMED), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadLuckyMedalSign) },
    { SEARCH(SEARCH_BARREL), TOLBI_BARREL, 0xf97, FIND_COINS(9) },
    { SEARCH(SEARCH_JAR), TOLBI_JAR, 0xf98, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_CHEST), TOLBI_HIDDEN_CHEST, 0xf99, FIND_ITEM(0xbf) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), TOLBI_PUDDLE, FLAG_TOLBI_PUDDLE_FROZEN, EVENT_SCRIPT(Tolbi_FreezePuddle) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RevealHiddenChest) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_HideRevealedChest) },
    { SCENE_EVENTS_END },
};

// After Colosso has ended.
struct SceneEvent gTolbiEventsAfterColosso[] = {
    { PSYNERGY_ON_ACTOR(0xc /* Growth */), TOLBI_SPROUT, FLAG_TOLBI_SPROUT_GROWN, EVENT_SCRIPT(Tolbi_GrowSprout) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 239, IF_FLAG_SET(FLAG_TOLBI_SPRING_OPENED), EVENT_SCRIPT(Tolbi_EnterSpring) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTolbiWellThisYearsColossoHasEnded) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTolbiItsBackToOurNormalQuiet) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToActor12) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiWithAllThoseRevelersGoneThe) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiIHadNoLuckInThe) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTolbiItsSoQuietTheHustleAnd) },
    { EVENT_TALK, TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { EVENT_TALK, TOLBI_YOUNGER_SIBLING, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSibling) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTolbiIllBeBackToDoBusiness) },
    { EVENT_TALK, TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToSnackVendor) },
    { EVENT_TALK, TOLBI_GAMBLER, FLAG_TOLBI_SPRING_OPENED, EVENT_SCRIPT(Tolbi_TalkToGambler) },
    { EVENT_TALK, TOLBI_GAMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiAhIGuessIJustDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiTheSpectatorsHaveAllLeftSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiThinkHowFunItWouldBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiAllTheWarriorsWhoGainFame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiComeToThinkOfItNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiHmmSinceColossoEndedSoDramatically) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiTheCalmAfterTheStormIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_YOUNGER_SIBLING, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSibling) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgTolbiEveryYearITryToSell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgTolbiHowCanWeGetHomeIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgTolbiIKeepLosingCoinsButI) },
    { EVENT_TALK, TOLBI_EMBER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_JoinEmber) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadSpringSign) },
    { SEARCH(SEARCH_UNNAMED), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadLuckyMedalSign) },
    { SEARCH(SEARCH_BARREL), TOLBI_BARREL, 0xf97, FIND_COINS(9) },
    { SEARCH(SEARCH_JAR), TOLBI_JAR, 0xf98, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_CHEST), TOLBI_HIDDEN_CHEST, 0xf99, FIND_ITEM(0xbf) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), TOLBI_PUDDLE, FLAG_TOLBI_PUDDLE_FROZEN, EVENT_SCRIPT(Tolbi_FreezePuddle) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RevealHiddenChest) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_HideRevealedChest) },
    { SCENE_EVENTS_END },
};
// clang-format on

// How a door opens: two frames of cells copied over the closed door.
u16 gTolbiDoorOpening[] = {
    MAP_CELL_STEP(39, 32, 1, 2, 6),
    MAP_CELL_STEP(40, 32, 1, 2, 6),
    MAP_CELL_STEPS_END,
};

// The doors, in the order of their triggers 1 to 9, by map cell.
struct MapCellAnim gTolbiDoors[] = {
    {gTolbiDoorOpening, 48, 6},  {gTolbiDoorOpening, 63, 4},
    {gTolbiDoorOpening, 71, 8},  {gTolbiDoorOpening, 71, 4},
    {gTolbiDoorOpening, 66, 21}, {gTolbiDoorOpening, 61, 24},
    {gTolbiDoorOpening, 50, 26}, {gTolbiDoorOpening, 50, 22},
    {gTolbiDoorOpening, 43, 20},
};
