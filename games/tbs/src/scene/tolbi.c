/// @file
/// SceneId_Tolbi, the town of Babi, its ruler, where Colosso, a festival
/// of warriors, is held each year in the colosseum.
///
/// The party comes to Tolbi across the Karagol Sea from Kalay while the
/// festival is on, and the town's talk follows the Colosso story in three
/// stages. Tolbi_SelectEvents gives each stage its own event table; one
/// placement table serves all three, and Tolbi_ApplyEntryState moves the
/// actors whose places change:
/// - The festival (neither story flag set). The trials have begun at the
///   colosseum, but Babi is not there, and his soldiers are searching the
///   town. The first time the party steps on cell code 20, two of them stop
///   it to show his picture, `Have you seen anyone who looks like this?`, and
///   go their way whatever it answers (Tolbi_MeetSearchingSoldiers,
///   FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI).
/// - Finals day (FLAG_BABI_SAVED), once the party has saved Babi in
///   Altmiller Cave: he has shown himself at the finals, the town hurries to
///   the colosseum, and the scalper who slept under the archway has moved,
///   sending people to his buddy for seats.
/// - After Colosso (FLAG_COLOSSO_OVER): the visitors have left, so the family
///   seen at the festival and four others are moved off the map, and those
///   who stay talk of the quiet and of the ships that no longer sail the
///   Karagol. The snack vendor gives away his leftover corn, once
///   (FLAG_TOLBI_CORN_GIVEN).
///
/// In every stage two children run a loop around the plaza
/// (gTolbiOlderBrotherRun, gTolbiYoungerSisterRun), each stopping when it runs
/// up behind the other or finds the leader in its way (Tolbi_UpdateKid), and
/// quarrel over their allowance. A gambler stands at the gate of Tolbi Spring;
/// once the party agrees to test its luck he steps aside for good
/// (Tolbi_TalkToGambler, FLAG_TOLBI_SPRING_OPENED), and the gate leads into
/// Tolbi Spring (TOLBI_TO_TOLBI_SPRING). Ember, a Mars Djinni, joins the
/// party when spoken to. Growth raises a sprout and Frost freezes a puddle,
/// each a scene flag kept only through a return that keeps the party's
/// position; Reveal uncovers a chest with Power Bread, and a barrel and a jar
/// hold 9 coins and a Lucky Medal (during the festival, the international
/// editions find the jar only when the party faces north).
///
/// The doors from TOLBI_TO_HOUSE_SOUTH to TOLBI_TO_SANCTUM, each numbered as
/// its trigger, lead into Tolbi's houses, inns and shops and into Tolbi Sanctum
/// (all SceneId_TolbiInteriors), but for TOLBI_TO_LUCKY_WHEELS, which with
/// TOLBI_TO_LUCKY_DICE leads into the Lucky Dice and Lucky Wheels rooms
/// (SceneId_TolbiGameRooms, whose area name is also Tolbi).
/// TOLBI_TO_BABI_PALACE_ENTRANCE leads to Babi's Palace,
/// TOLBI_TO_COLOSSEUM_ENTRANCE to the Colosseum Entrance and TOLBI_TO_WORLD_MAP
/// out onto the world map.

#include "unit/scene/tolbi.h"
#include "djinn_ids.h"
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
#include "songs.h"

#if TBS_PATCH_1
#define TOLBI_JAR_SEARCH_KIND                                                  \
  (SEARCH(SEARCH_JAR) | EVENT_TOUCH_FACING | FACING_NORTH)
#else
#define TOLBI_JAR_SEARCH_KIND SEARCH(SEARCH_JAR)
#endif

static void Tolbi_FreezePuddle(void);

/// Talking to Ember, a Mars Djinni, makes it join the party. Its index among
/// the Mars Djinn is 4, the fifth Mars name in the game text.
static void Tolbi_JoinEmber(void) {
  Djinn_JoinParty(TOLBI_EMBER, ELEMENT_MARS, DJINNI_INDEX(DJINNI_EMBER));
}

#include "common/object_distance.h"

/// Stops a running child when the target is close by and ahead of it, or
/// always when force is set; otherwise starts the child running again.
/// The child stops when the target is within one facing step of where it
/// looks; for the party leader, within two steps either side.
/// Returns 1 when the child stopped.
static s32 Tolbi_KidStopForTarget(struct FieldActor *kid,
                                  struct FieldActor *target, s32 range,
                                  s32 force) {
  s32 stopped = 0;
  const u8 *targetPos = (const u8 *)target + offsetof(struct FieldActor, x);
  const u8 *kidPos = (const u8 *)kid + offsetof(struct FieldActor, x);

  if (FixedPoint_Distance(targetPos, kidPos) < range || force != 0) {
    // The heading to the target, and the eight-way directions around it.
    u32 heading = (u16)ArcTan2(target->z.fixed - kid->z.fixed,
                               *(const s32 *)targetPos - *(const s32 *)kidPos);
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
// way. While the party talks to someone or Mind Read shows thoughts (the range
// widens to 26 pixels), the child being talked to stops whatever the
// direction.
// UB: calling this s32 function through a void update hook is incompatible.
// Plain void removes its zero result; its aligned extent stays 152 bytes.
#ifdef UBFIX
static void Tolbi_UpdateKid(union FieldObject *fieldObject) {
#else
static s32 Tolbi_UpdateKid(union FieldObject *fieldObject) {
#endif
  struct FieldActor *self = &fieldObject->actor;
  u8 **workAreas = gWindowWork;
  struct UiRenderWork *ui = (struct UiRenderWork *)workAreas[0];
  struct FieldWork *field =
      (struct FieldWork *)workAreas[HEAP_SLOT_EVENT - HEAP_SLOT_WINDOW];
  u16 *kidFlags = (u16 *)&self->timer;
  s32 force = 0;
  s32 range = 18;
  struct FieldActor *other;
  struct FieldActor *leader;

  if ((*kidFlags & KID_CHASES_YOUNGER) != 0) {
    other = Object_GetById(TOLBI_YOUNGER_SISTER);
  } else {
    other = Object_GetById(TOLBI_OLDER_BROTHER);
  }
  if (Tolbi_KidStopForTarget(self, other, 32, 0) != 0) {
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  }

  leader = Object_GetById(ACTOR_PARTY_LEADER);

  if ((s16)field->descriptor != 0 || ui->mode != 0) {
    range = 26;
    if ((*kidFlags & KID_TALKING) != 0) {
      force = 1;
    }
  }

  Tolbi_KidStopForTarget(self, leader, range, force);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

const struct SceneEntrance *Tolbi_GetEntrances(void) { return gTolbiEntrances; }

const struct SceneRegion *Tolbi_GetRegions(void) { return 0; }

const u32 *Tolbi_GetExits(void) { return gTolbiExits; }

const struct ScenePlacement *Tolbi_GetPlacements(void) {
  return gTolbiPlacements;
}

/// Growth on the sprout: the grown plant no longer blocks the party, and its
/// map cell becomes climbable.
static void Tolbi_GrowSprout(void) {
  struct FieldActor *sprout;

  sprout = Object_GetById(TOLBI_SPROUT);
  if ((s32)sprout != 0) {
    sprout->collisionFlags = 0;
  }
  sprout = Object_GetById(TOLBI_SPROUT);
  Engine_ActorSetSpriteFlags(sprout, 0);
  Field_SetMapCellCollision(0, PIXELS(544), PIXELS(288), 253);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
}

/// Reveal uncovers the hidden chest: its map cell is drawn in and the chest
/// moves into place.
static void Tolbi_RevealHiddenChest(void) {
  Map_CopyCellAttributes(3, 32, 1, 1, 3, 26);
  MapObject_SetPosition(TOLBI_HIDDEN_CHEST, PIXELS(56), PIXELS(424));
}

/// When Reveal wears off, the chest's cell is covered again and the chest
/// moves out of sight.
static void Tolbi_HideRevealedChest(void) {
  Map_CopyCellAttributes(2, 25, 1, 1, 3, 26);
  MapObject_SetPosition(TOLBI_HIDDEN_CHEST, -1, -1);
}

/// The first sign at Tolbi Spring: face away and toss a coin in.
static void Tolbi_ReadSpringSign(void) {
  Engine_EventSetMessage(MsgTolbiFaceAwayTolbi);
  Engine_EventShowMessage(-1, 0);
}

/// The second sign at Tolbi Spring: a Lucky Medal wins better prizes.
static void Tolbi_ReadLuckyMedalSign(void) {
  Engine_EventSetMessage(MsgTolbiTossLuckyMedal);
  Engine_EventShowMessage(-1, 0);
}

/// Talking to the older brother, or reading his mind. He stops running while
/// he speaks, then turns back the way he was going.
static void Tolbi_TalkToOlderBrother(void) {
  struct EventWork *work = gEventWork;
  struct FieldActor *kid;
  s16 facing;
  s32 msg;

  kid = Object_GetById(TOLBI_OLDER_BROTHER);
  facing = kid->facing;
  Engine_EventBegin();
  kid->timer |= KID_TALKING;
  if (work->psynergyRequest == 0) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      msg = MsgTolbiYayEasyRun;
    } else if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      msg = MsgTolbiHoorayFinalsSeven;
    } else {
      msg = MsgTolbiWheeFestivalColosso;
    }
  } else {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      msg = MsgTolbiMainStreetTolbi;
    } else if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
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
  kid->timer &= KID_CHASES_YOUNGER;
  Engine_EventEnd();
}

/// Talking to the younger sister, or reading her mind, as above.
static void Tolbi_TalkToYoungerSister(void) {
  struct EventWork *work = gEventWork;
  struct FieldActor *kid;
  s16 facing;
  s32 msg;

  kid = Object_GetById(TOLBI_YOUNGER_SISTER);
  facing = kid->facing;
  Engine_EventBegin();
  kid->timer |= KID_TALKING;
  if (work->psynergyRequest == 0) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      msg = MsgTolbiHaHaHa;
    } else if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      msg = MsgTolbiWaahBigBrother;
    } else {
      msg = MsgTolbiWaahSaidMoney;
    }
  } else {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      msg = MsgTolbiBrotherMeanDuring;
    } else if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      msg = MsgTolbiMamaWhyListen;
    } else {
      msg = MsgTolbiMamaToldShare;
    }
  }
  Engine_EventSetMessage(msg);
  Engine_ActorSetAnimation(TOLBI_YOUNGER_SISTER, 0);
  Engine_ActorFaceEachOther(TOLBI_YOUNGER_SISTER, ACTOR_PARTY_LEADER, 2);
  Engine_EventShowMessageAndWait(TOLBI_YOUNGER_SISTER, 0, 10);
  kid->facing = facing;
  Engine_TaskWait(1);
  kid->timer &= KID_CHASES_YOUNGER;
  Engine_EventEnd();
}

/// Picks the event table for Tolbi's current mood.
const struct SceneEvent *Tolbi_SelectEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    return gTolbiEventsAfterColosso;
  }
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
    return gTolbiEventsFinalsDay;
  }
  return gTolbiEventsFestival;
}

/// A suspicious old woman asks whether anyone can live past 100 years old.
/// She's TOLBI_SUSPICIOUS_OLD_WOMAN, in Tolbi during the festival, and asks
/// whether someone can live to be more than 100 years old; either way the talk
/// turns to Babi, who is older still.
static void Tolbi_AskAboutLivingPastHundred(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiSomeoneLiveMore);
  Engine_EventAskYesNo(TOLBI_SUSPICIOUS_OLD_WOMAN, 0);
  Engine_EventEnd();
}

/// The father asks whether children really do love souvenirs, and owns up to
/// loving them himself.
static void Tolbi_TalkToFather(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiChildrenLoveSouvenirs);
  Engine_EventAskYesNo(TOLBI_FATHER, 0);
  Engine_EventEnd();
}

/// The mother turns to her crying boy, then back to promise better sweets
/// later on. "Be patient, little guy."
static void Tolbi_TalkToMother(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiPatientLittleGuy);
  Actor_FaceDirection(TOLBI_MOTHER, FACING_NORTH, 0);
  Engine_EventShowMessage(TOLBI_MOTHER, 0);
  Actor_FaceDirection(TOLBI_MOTHER, FACING_WEST, 0);
  Engine_EventShowMessage(TOLBI_MOTHER, 0);
  Engine_EventEnd();
}

/// The crying boy turns, stamps twice and wails for sweets.
static void Tolbi_TalkToCryingBoy(void) {
  Engine_EventBegin();
  Actor_FaceDirection(TOLBI_CRYING_BOY, FACING_SOUTH, 0);
  Engine_ActorStartRepeatedMotion(TOLBI_CRYING_BOY, 2);
  Engine_EventSetMessage(MsgTolbiWaahBuySweets);
  Engine_EventShowMessage(TOLBI_CRYING_BOY, 0);
  Engine_EventEnd();
}

/// During the festival the scalper sleeps under the archway: the inns are
/// full.
static void Tolbi_TalkToSleepingScalper(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiInnsFullStaying);
  Engine_EventShowMessage(TOLBI_SCALPER, 0);
  Engine_EventEnd();
}

/// After Colosso, TOLBI_FINALS_FAN asks whether the party was in the finals.
/// The question is anchored to the father, who has left town by then.
static void Tolbi_AskWhetherPartyWasInFinals(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiFinalsWerent);
  Engine_EventAskYesNo(TOLBI_FATHER, 0);
  Engine_EventEnd();
}

/// After Colosso, the snack vendor gives the party his leftover corn, once.
static void Tolbi_TalkToSnackVendor(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_TOLBI_CORN_GIVEN) == 0) {
    Engine_GameFlagSet(FLAG_TOLBI_CORN_GIVEN);
    Engine_EventSetMessage(MsgTolbiLeftovers);
    Engine_EventShowMessage(TOLBI_SNACK_VENDOR, 0);
    Engine_ItemShowFound(ITEM_CORN, 3);
    Engine_EventShowMessage(TOLBI_SNACK_VENDOR, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Engine_PartyGiveItem(ITEM_CORN, 0);
  } else {
    Engine_EventSetMessage(MsgTolbiFestivalLongerUsual);
    Engine_EventShowMessage(TOLBI_SNACK_VENDOR, 0);
  }
  Engine_EventEnd();
}

/// Walking into one of the town's doors: the door opens, the leader steps in
/// (except at TOLBI_TO_WEAPON_AND_ARMOR_SHOP) and the scene changes to the
/// door's exit, numbered as its trigger.
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
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gTolbiDoors[door].steps, gTolbiDoors[door].x,
                   gTolbiDoors[door].y);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (door != TOLBI_TO_WEAPON_AND_ARMOR_SHOP - 1) {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
    Engine_EventWait(10);
  }
  Engine_EventRequestExit(work->touchedTrigger);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

/// The update of TOLBI_OLD_MAN, who during the festival says he is 65 and that
/// Babi was already old when he was born: gives him the leader's sprite
/// priority every frame, for both of his sprite parts.
static void Tolbi_CopyLeaderPriority(union FieldObject *object) {
  s32 priority;

  if (object != NULL) {
    priority = Object_GetById(ACTOR_PARTY_LEADER)->sprite->image.priority;
    object->actor.priorityFlags = 0;
    object->actor.sprite->image.priority = priority;
    object->actor.sprite->shadow.priority = priority;
  }
}

/// Sets up the scene on entry: the running children, TOLBI_OLD_MAN's update,
/// and everything the flags have already changed.
s32 Tolbi_ApplyEntryState(s32 unused) {
  struct FieldActor *actor;
  void (*kidUpdate)(union FieldObject *);
  s32 clearFlags;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  // The two children run their loops around the plaza.
  Actor_SetPosition(TOLBI_OLDER_BROTHER, PIXELS(352), PIXELS(352));
  Actor_SetActionScript(TOLBI_OLDER_BROTHER, gTolbiOlderBrotherRun);
  actor = Object_GetById(TOLBI_OLDER_BROTHER);
#ifdef UBFIX
  kidUpdate = Tolbi_UpdateKid;
#else
  kidUpdate = (void (*)(union FieldObject *))Tolbi_UpdateKid;
#endif
  actor->timer = KID_CHASES_YOUNGER;
  actor->update = kidUpdate;
  clearFlags = 0;
  Engine_ActorSetPosition(TOLBI_YOUNGER_SISTER, PIXELS(368), PIXELS(320));
  Actor_SetActionScript(TOLBI_YOUNGER_SISTER, gTolbiYoungerSisterRun);
  actor = Object_GetById(TOLBI_YOUNGER_SISTER);
  actor->timer = clearFlags;
  actor->update = kidUpdate;
  actor = Object_GetById(TOLBI_OLD_MAN);
  actor->update = Tolbi_CopyLeaderPriority;

  // What the party has already done here.
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SPRING_OPENED) != 0) {
    Actor_SetPosition(TOLBI_GAMBLER, PIXELS(316), PIXELS(328));
  }
  // The puddle is frozen.
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01) != 0) {
    Tolbi_FreezePuddle();
  }
  // The sprout has grown.
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00) != 0) {
    Tolbi_GrowSprout();
    Engine_ActorSetAnimation(TOLBI_SPROUT, 4);
  }

  // After Colosso the crowd has gone home and is moved off the map. On finals
  // day the scalper waits by the gate.
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    Actor_SetPosition(TOLBI_WOULD_BE_FIGHTER, PIXELS(520), PIXELS(560));
    Actor_SetPosition(TOLBI_XIAN_VISITOR, PIXELS(520), PIXELS(560));
    Actor_SetPosition(TOLBI_OVERSLEEPER, PIXELS(520), PIXELS(560));
    Actor_SetPosition(TOLBI_FATHER, PIXELS(520), PIXELS(560));
    Actor_SetPosition(TOLBI_MOTHER, PIXELS(520), PIXELS(560));
    Actor_SetPosition(TOLBI_CRYING_BOY, PIXELS(520), PIXELS(560));
    Actor_SetPosition(TOLBI_SCALPER, PIXELS(520), PIXELS(560));
  } else {
    if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      Actor_SetPosition(TOLBI_SCALPER, PIXELS(280), PIXELS(80));
      Actor_FaceDirection(TOLBI_SCALPER, FACING_SOUTHEAST, 0);
      Engine_ActorSetAnimation(TOLBI_SCALPER, ANIM_STAND);
    }
  }
  return 0;
}

/// The gambler at the spring's gate asks whether the party wants to test its
/// luck. On yes he steps aside for good and the way to Tolbi Spring opens.
static void Tolbi_TalkToGambler(void) {
  Engine_EventBegin();
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 304, 312);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_ActorFaceDirection(TOLBI_GAMBLER, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgTolbiWantTestLuck);
  Event_OpenMessage(TOLBI_GAMBLER, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    // Yes: "I'll give you the chance to go first."
    gEventWork->message++;
    Engine_EventShowMessage(TOLBI_GAMBLER, 0);
    Actor_SetSpeed(TOLBI_GAMBLER, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(TOLBI_GAMBLER, 320, 304);
    Actor_WalkToAndWait(TOLBI_GAMBLER, 316, 328);
    Engine_ActorFaceDirection(TOLBI_GAMBLER, FACING_NORTHWEST, 0);
    Engine_GameFlagSet(FLAG_TOLBI_SPRING_OPENED);
  } else {
    Engine_EventShowMessage(TOLBI_GAMBLER, 0);
  }
  Engine_EventEnd();
}

/// The gate to Tolbi Spring (cell code 239), once the gambler has stepped
/// aside.
static void Tolbi_EnterSpring(void) {
  Engine_EventRequestExit(TOLBI_TO_TOLBI_SPRING);
  Engine_EventEnd();
}

/// Leaves by the exit numbered as the trigger the party walked into.
static void Tolbi_RequestTouchedExit(void) {
  s16 *trigger = &gEventWork->touchedTrigger;

  Engine_EventRequestExit(*trigger);
}

/// Two soldiers searching for the missing Babi stop the party and show his
/// picture. They do so during the festival, the first time the party steps on
/// cell code 20. "Have you seen anyone who looks like this?" They leave either
/// way the party answers. Sets FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI.
static void Tolbi_MeetSearchingSoldiers(void) {
  s32 firstMsg;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Actor_SetSpeed(TOLBI_SOLDIER_A, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetSpeed(TOLBI_SOLDIER_B, PIXELS(1), PIXELS(1) / 2);
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
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
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
    Actor_SetSpeed(TOLBI_SOLDIER_A, PIXELS(9) / 5, PIXELS(9) / 10);
    Actor_SetSpeed(TOLBI_SOLDIER_B, PIXELS(9) / 5, PIXELS(9) / 10);
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
    Actor_SetSpeed(TOLBI_SOLDIER_A, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_SetSpeed(TOLBI_SOLDIER_B, PIXELS(8) / 5, PIXELS(4) / 5);
    Engine_ActorWalkTo(TOLBI_SOLDIER_A, 72, 184);
    Engine_ActorWalkToAndWait(TOLBI_SOLDIER_B, 56, 184);
  }
  Engine_ActorSetPosition(TOLBI_SOLDIER_A, 0, 0);
  Engine_ActorSetPosition(TOLBI_SOLDIER_B, 0, 0);
  Engine_ActorSetPosition(TOLBI_MISSING_PERSON_POSTER, 0, 0);
  Engine_GameFlagSet(FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI);
  Engine_EventEnd();
}

/// Frost on the puddle: the frozen pillar is drawn underfoot and stops
/// following the terrain, and its map cell becomes solid.
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
  Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
}

/// The action script of a mirrored actor: flips its sprite and passes 7 on.
s32 gTolbiMirroredActor[] = {
    SC_SET(SW_SCALE_X, -0x10000),
    SC_ARG(7),
    SC_END,
};

/// The older brother runs a loop of eight points around the plaza.
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

/// The younger sister runs the same loop half a lap ahead.
s32 gTolbiYoungerSisterRun[] = {
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
    {TOLBI_FROM_HOUSE_SOUTH, CONDITION_ALWAYS, 304, 0, 448, FACING_NORTH, 0, -1,
     -1, -1, -1, 0},
    {TOLBI_FROM_HOUSE_NORTHWEST, CONDITION_ALWAYS, 168, 0, 200, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {TOLBI_FROM_INN, CONDITION_ALWAYS, 408, 0, 160, FACING_SOUTH, 0, -1, -1, -1,
     -1, 0},
    {TOLBI_FROM_TOLBI_INTERIORS_EAST, CONDITION_ALWAYS, 536, 0, 232,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_FROM_HOUSE_NORTHEAST, CONDITION_ALWAYS, 536, 0, 200, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {TOLBI_FROM_HOUSE_EAST, CONDITION_ALWAYS, 456, 0, 376, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {TOLBI_FROM_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 368, 0, 416,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_FROM_HOUSE_SOUTHWEST, CONDITION_ALWAYS, 200, 0, 456, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {TOLBI_FROM_SANCTUM, CONDITION_ALWAYS, 200, 0, 424, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {TOLBI_FROM_TOLBI_INTERIORS_WEST, CONDITION_ALWAYS, 88, 0, 392,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_FROM_BABI_PALACE_ENTRANCE, CONDITION_ALWAYS, 64, 0, 112,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_FROM_COLOSSEUM_ENTRANCE, CONDITION_ALWAYS, 304, 0, 64, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {TOLBI_FROM_LUCKY_DICE, CONDITION_ALWAYS, 424, 0, 224, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {TOLBI_FROM_TOLBI_SPRING, CONDITION_ALWAYS, 304, 0, 304, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {30, CONDITION_ALWAYS, 568, 0, 296, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gTolbiExits[] = {
    SCENE_EXITS(Tolbi),
    SCENE_EXIT(TOLBI_TO_HOUSE_SOUTH, TolbiInteriors,
               TOLBI_INTERIORS_HOUSE_SOUTH),
    SCENE_EXIT(TOLBI_TO_HOUSE_NORTHWEST, TolbiInteriors,
               TOLBI_INTERIORS_HOUSE_NORTHWEST),
    SCENE_EXIT(TOLBI_TO_INN, TolbiInteriors, TOLBI_INTERIORS_INN),
    SCENE_EXIT(TOLBI_TO_LUCKY_WHEELS, TolbiGameRooms,
               TOLBI_GAME_ROOMS_LUCKY_WHEELS_FROM_TOLBI),
    SCENE_EXIT(TOLBI_TO_HOUSE_NORTHEAST, TolbiInteriors,
               TOLBI_INTERIORS_HOUSE_NORTHEAST),
    SCENE_EXIT(TOLBI_TO_HOUSE_EAST, TolbiInteriors, TOLBI_INTERIORS_HOUSE_EAST),
    SCENE_EXIT(TOLBI_TO_WEAPON_AND_ARMOR_SHOP, TolbiInteriors,
               TOLBI_INTERIORS_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(TOLBI_TO_HOUSE_SOUTHWEST, TolbiInteriors,
               TOLBI_INTERIORS_HOUSE_SOUTHWEST),
    SCENE_EXIT(TOLBI_TO_SANCTUM, TolbiInteriors, TOLBI_INTERIORS_SANCTUM),
    SCENE_EXIT(TOLBI_TO_BABI_PALACE_ENTRANCE, BabiPalaceEntrance,
               BABI_PALACE_ENTRANCE_FROM_TOLBI),
    SCENE_EXIT(TOLBI_TO_COLOSSEUM_ENTRANCE, ColosseumEntrance,
               COLOSSEUM_ENTRANCE_FROM_TOLBI),
    SCENE_EXIT(TOLBI_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_TOLBI),
    SCENE_EXIT(TOLBI_TO_LUCKY_DICE, TolbiGameRooms,
               TOLBI_GAME_ROOMS_LUCKY_DICE_FROM_TOLBI),
    SCENE_EXIT(TOLBI_TO_TOLBI_SPRING, TolbiSpring, TOLBI_SPRING_FROM_TOLBI),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gTolbiPlacements[] = {
    { SPRITE_SPROUT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(552), 0, PIXELS(288), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(552), 0, PIXELS(264), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(264), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(208), 0, PIXELS(312), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(224), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(216), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(152), 0, PIXELS(304), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(24), 0, PIXELS(456), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (s32)gTolbiOlderBrotherRun,
        PIXELS(344), 0, PIXELS(328), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (s32)gTolbiYoungerSisterRun,
        PIXELS(376), 0, PIXELS(328), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(200), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUTH_WITH_HEADBAND, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(400), 0, PIXELS(432), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUTH_WITH_HEADBAND, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(184), 0, PIXELS(472), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(256), 0, PIXELS(168), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(184), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(168), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, (s32)gTolbiMirroredActor,
        PIXELS(312), 0, PIXELS(120), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(302), 0, PIXELS(296), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-9), 0, PIXELS(488), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-9), 0, PIXELS(488), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    // Ember, until its Djinni flag is set: twenty flags to an element.
    { MARS_DJINNI, FLAG_DJINNI_0 + DJINNI_EMBER, ACTOR_STAND,
        PIXELS(584), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BABI, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-9), 0, PIXELS(488), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

/// During the festival, before the finals.
struct SceneEvent gTolbiEventsFestival[] = {
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), TOLBI_SPROUT, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Tolbi_GrowSprout) },
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
    { EVENT_EXIT, 12, CONDITION_ALWAYS, TOLBI_TO_WORLD_MAP },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_TOUCH, 20, FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI, EVENT_SCRIPT(Tolbi_MeetSearchingSoldiers) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 239, IF_FLAG_SET(FLAG_TOLBI_SPRING_OPENED), EVENT_SCRIPT(Tolbi_EnterSpring) },
    { EVENT_TALK, TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTolbisAlwaysReallyBustlingDuringColosso) },
    { EVENT_TALK, TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiColossoIsHeldInTheColosseum) },
    { EVENT_TALK, TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTryoutsStartedAtTheColosseumToday) },
    { EVENT_TALK, TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiColossoShouldHaveStartedTodayWhat) },
    { EVENT_TALK, TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiBabiWasAlreadyPrettyOldBack) },
    { EVENT_TALK, TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_AskAboutLivingPastHundred) },
    { EVENT_TALK, TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { EVENT_TALK, TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSister) },
    { EVENT_TALK, TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiGetYourColossoSouvenirsTheyMake) },
    { EVENT_TALK, TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiGetYourSnacksAndDrinksBefore) },
    { EVENT_TALK, TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiIveDecidedNotToFightAt) },
    { EVENT_TALK, TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiWowTolbisFestivalIsHumongousJust) },
    { EVENT_TALK, TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiRatsWeOversleptAndNowThe) },
    { EVENT_TALK, TOLBI_FATHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToFather) },
    { EVENT_TALK, TOLBI_MOTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToMother) },
    { EVENT_TALK, TOLBI_CRYING_BOY, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToCryingBoy) },
    { EVENT_TALK, TOLBI_SCALPER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToSleepingScalper) },
    { EVENT_TALK, TOLBI_GAMBLER, FLAG_TOLBI_SPRING_OPENED, EVENT_SCRIPT(Tolbi_TalkToGambler) },
    { EVENT_TALK, TOLBI_GAMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiAhIGuessIJustDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTouristsArentTheOnlyOnesWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheTrialsAreStartingAtThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheTrialsAreOkayButThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiItsNotLikeThereHaveBeen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiEveryYearIGetOlderAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiWhatIfBabiIsActuallyA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiWeHaveToSellAllOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTheyreSellingTheseAtTheColosseum) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiISawThePeopleWhoAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTolbiIsNotLikeXianIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiICantSayAnythingCauseIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_FATHER, CONDITION_ALWAYS, MSGNO(MsgTolbiTheStuffTheyreSellingAtThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_MOTHER, CONDITION_ALWAYS, MSGNO(MsgTolbiWhyDoKidsWantSuchAwful) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiThoseSweetsSmellGreatImNot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SCALPER, CONDITION_ALWAYS, MSGNO(MsgTolbiIveBeenGuardingOurSpotBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_GAMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiIKeepLosingCoinsButI) },
    { EVENT_TALK, TOLBI_EMBER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_JoinEmber) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadSpringSign) },
    { SEARCH(SEARCH_UNNAMED), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadLuckyMedalSign) },
    { SEARCH(SEARCH_BARREL), TOLBI_BARREL, 0xf97, FIND_COINS(9) },
// The international editions find the jar only when the party faces north.
    { TOLBI_JAR_SEARCH_KIND, TOLBI_JAR, 0xf98, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_CHEST), TOLBI_HIDDEN_CHEST, 0xf99, FIND_ITEM(ITEM_POWER_BREAD) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), TOLBI_PUDDLE, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Tolbi_FreezePuddle) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RevealHiddenChest) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_HideRevealedChest) },
    { SCENE_EVENTS_END },
};

/// On finals day.
struct SceneEvent gTolbiEventsFinalsDay[] = {
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), TOLBI_SPROUT, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Tolbi_GrowSprout) },
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
    { EVENT_EXIT, 12, CONDITION_ALWAYS, TOLBI_TO_WORLD_MAP },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 239, IF_FLAG_SET(FLAG_TOLBI_SPRING_OPENED), EVENT_SCRIPT(Tolbi_EnterSpring) },
    { EVENT_TALK, TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiColossoWillReachItsApexAny) },
    { EVENT_TALK, TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiAreYouPlanningToWatchThe) },
    { EVENT_TALK, TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiMyFamilySavedMeASeat) },
    { EVENT_TALK, TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiIThoughtSomethingHappenedToBabi) },
    { EVENT_TALK, TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheColosseumWasSoFullThat) },
    { EVENT_TALK, TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiNobodyElseHasNoticedButThis) },
    { EVENT_TALK, TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { EVENT_TALK, TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSister) },
    { EVENT_TALK, TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiHowAboutSomeColossoSouvenirsThis) },
    { EVENT_TALK, TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTheFinalsAreSoLongIts) },
    { EVENT_TALK, TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiTheTrialsWereSoRivetingThe) },
    { EVENT_TALK, TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiIHaveSeatsToTheFinals) },
    { EVENT_TALK, TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiIOversleptAgainSoICant) },
    // XXX: likely the family below had finals-day lines of its own, the four
    // messages after MsgTolbiSpringOhThereAreAlwaysScalpersAt (`The finals are
    // about to start.` and the rest), because those speak of waiting for
    // Colosso to end and of buying once prices drop, and no row or code shows
    // them; the family repeats its festival lines instead.
    { EVENT_TALK, TOLBI_FATHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToFather) },
    { EVENT_TALK, TOLBI_MOTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToMother) },
    { EVENT_TALK, TOLBI_CRYING_BOY, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToCryingBoy) },
    { EVENT_TALK, TOLBI_SCALPER, CONDITION_ALWAYS, MSGNO(MsgTolbiTheFinalsStartTodayIWonder) },
    { EVENT_TALK, TOLBI_GAMBLER, FLAG_TOLBI_SPRING_OPENED, EVENT_SCRIPT(Tolbi_TalkToGambler) },
    { EVENT_TALK, TOLBI_GAMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiAhIGuessIJustDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiIveGotToGetToThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiABattleToTheDeathWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheFinalsStartOnBabisSignal) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiSoWhyWereTheSoldiersAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiICouldStillBuySeatsFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiNoMatterHowIAddIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiWeCantSellAnythingOnceThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiThisIsTheFinalPushAnything) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiFightingInColossoRequiresALot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTheyreSellingFinalsSeatsRightAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiItsSupposedToBeFreeHaving) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_FATHER, CONDITION_ALWAYS, MSGNO(MsgTolbiOnceColossoEndsOurLivesCan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_MOTHER, CONDITION_ALWAYS, MSGNO(MsgTolbiFestivalsAreSoExpensiveMyWallets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiMomYouSaidYouWereAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SCALPER, CONDITION_ALWAYS, MSGNO(MsgTolbiTodayIsABigDayFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_GAMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiIKeepLosingCoinsButI) },
    { EVENT_TALK, TOLBI_EMBER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_JoinEmber) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadSpringSign) },
    { SEARCH(SEARCH_UNNAMED), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadLuckyMedalSign) },
    { SEARCH(SEARCH_BARREL), TOLBI_BARREL, 0xf97, FIND_COINS(9) },
    { SEARCH(SEARCH_JAR), TOLBI_JAR, 0xf98, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_CHEST), TOLBI_HIDDEN_CHEST, 0xf99, FIND_ITEM(ITEM_POWER_BREAD) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), TOLBI_PUDDLE, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Tolbi_FreezePuddle) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RevealHiddenChest) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_HideRevealedChest) },
    { SCENE_EVENTS_END },
};

/// After Colosso has ended.
struct SceneEvent gTolbiEventsAfterColosso[] = {
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), TOLBI_SPROUT, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Tolbi_GrowSprout) },
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
    { EVENT_EXIT, 12, CONDITION_ALWAYS, TOLBI_TO_WORLD_MAP },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedExit) },
    { EVENT_TOUCH | EVENT_TOUCH_FACING | FACING_NORTH, 239, IF_FLAG_SET(FLAG_TOLBI_SPRING_OPENED), EVENT_SCRIPT(Tolbi_EnterSpring) },
    { EVENT_TALK, TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiWellThisYearsColossoHasEnded) },
    { EVENT_TALK, TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiItsBackToOurNormalQuiet) },
    { EVENT_TALK, TOLBI_FINALS_FAN, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_AskWhetherPartyWasInFinals) },
    { EVENT_TALK, TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiWithAllThoseRevelersGoneThe) },
    { EVENT_TALK, TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiIHadNoLuckInThe) },
    { EVENT_TALK, TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiItsSoQuietTheHustleAnd) },
    { EVENT_TALK, TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { EVENT_TALK, TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSister) },
    { EVENT_TALK, TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiIllBeBackToDoBusiness) },
    { EVENT_TALK, TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToSnackVendor) },
    { EVENT_TALK, TOLBI_GAMBLER, FLAG_TOLBI_SPRING_OPENED, EVENT_SCRIPT(Tolbi_TalkToGambler) },
    { EVENT_TALK, TOLBI_GAMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiAhIGuessIJustDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheSpectatorsHaveAllLeftSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiThinkHowFunItWouldBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiAllTheWarriorsWhoGainFame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiComeToThinkOfItNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiHmmSinceColossoEndedSoDramatically) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheCalmAfterTheStormIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiEveryYearITryToSell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiHowCanWeGetHomeIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_GAMBLER, CONDITION_ALWAYS, MSGNO(MsgTolbiIKeepLosingCoinsButI) },
    { EVENT_TALK, TOLBI_EMBER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_JoinEmber) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadSpringSign) },
    { SEARCH(SEARCH_UNNAMED), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadLuckyMedalSign) },
    { SEARCH(SEARCH_BARREL), TOLBI_BARREL, 0xf97, FIND_COINS(9) },
    { SEARCH(SEARCH_JAR), TOLBI_JAR, 0xf98, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_CHEST), TOLBI_HIDDEN_CHEST, 0xf99, FIND_ITEM(ITEM_POWER_BREAD) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), TOLBI_PUDDLE, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Tolbi_FreezePuddle) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RevealHiddenChest) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_HideRevealedChest) },
    { SCENE_EVENTS_END },
};
// clang-format on

/// How a door opens: two frames of cells copied over the closed door.
u16 gTolbiDoorOpening[] = {
    MAP_CELL_STEP(39, 32, 1, 2, 6),
    MAP_CELL_STEP(40, 32, 1, 2, 6),
    MAP_CELL_STEPS_END,
};

/// The doors, in the order of their triggers 1 to 9, by map cell.
struct MapCellAnim gTolbiDoors[] = {
    {gTolbiDoorOpening, 48, 6},  {gTolbiDoorOpening, 63, 4},
    {gTolbiDoorOpening, 71, 8},  {gTolbiDoorOpening, 71, 4},
    {gTolbiDoorOpening, 66, 21}, {gTolbiDoorOpening, 61, 24},
    {gTolbiDoorOpening, 50, 26}, {gTolbiDoorOpening, 50, 22},
    {gTolbiDoorOpening, 43, 20},
};
