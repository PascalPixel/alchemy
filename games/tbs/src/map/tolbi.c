/// @file
/// Tolbi (MapId_Tolbi), the town Babi rules, where Colosso, a festival of
/// warriors, is held each year in the colosseum. The party comes to Tolbi
/// across the Karagol Sea from Kalay while the festival is on, and the town's
/// talk follows the Colosso story in three stages. Tolbi_SelectEvents gives
/// each stage its own event table; one object event template table serves all
/// three, and Tolbi_ApplyEntryState moves the object events whose places
/// change:
///
/// - The festival (neither story flag set). The trials have begun at the
///   colosseum, but Babi is not there, and his soldiers are searching the
///   town. The first time the party steps on TOLBI_CELL_SOLDIERS, two of them
///   stop it to show his picture, `Have you seen anyone who looks like
///   this?`, and go their way whatever it answers
///   (Tolbi_MeetSearchingSoldiers, FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI).
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
/// In every stage two children run a small loop south-east of the fountain
/// (gTolbiOlderBrotherRun, gTolbiYoungerSisterRun), each stopping when it runs
/// up behind the other or finds the leader in its way (Tolbi_UpdateKid), and
/// quarrel over their allowance; the recording of talking to the brother
/// shows them running it
/// (https://alchemy.pascalpixel.com/places/tolbi/events/Tolbi_TalkToOlderBrother.gif).
/// A visitor testing his luck stands at the gate of Tolbi Spring; once the
/// party agrees to go first he steps aside for good (Tolbi_TalkToSpringVisitor,
/// FLAG_TOLBI_SPRING_OPENED), and the gate (TOLBI_CELL_SPRING_GATE) leads into
/// Tolbi Spring (TOLBI_TO_TOLBI_SPRING). Ember, a Mars Djinni, joins the party
/// when spoken to. Growth raises a sprout and Frost freezes a puddle, each a
/// map flag kept only through a return that keeps the party's position;
/// Reveal uncovers a chest with Power Bread, and a barrel and a jar hold 9
/// coins and a Lucky Medal (during the festival, the editions from TBS_PATCH_1
/// on find the jar only when the party faces north).
///
/// The doors from TOLBI_TO_HOUSE_SOUTH to TOLBI_TO_SANCTUM, whose cells carry
/// their warps' numbers, lead into Tolbi's houses, inns and shops and into
/// Tolbi Sanctum (all MapId_TolbiInteriors), but for TOLBI_TO_LUCKY_WHEELS,
/// which with TOLBI_TO_LUCKY_DICE leads into the Lucky Dice and Lucky Wheels
/// rooms (MapId_TolbiGameRooms, whose area name is also Tolbi).
/// TOLBI_TO_BABI_PALACE_ENTRANCE leads to Babi's Palace,
/// TOLBI_TO_COLOSSEUM_ENTRANCE to the Colosseum Entrance and TOLBI_TO_WORLD_MAP
/// out onto the world map.
///
/// Open questions:
/// - XXX (no evidence): whether the four messages after
///   MsgTolbiSpringOhThereAreAlwaysScalpersAt (`The finals are about to
///   start.` and the rest) were the festival family's finals-day lines; no
///   row or code shows them, and the family repeats its festival lines.

#include "unit/map/tolbi.h"
#include "djinn_ids.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "unit/runtime/runtime.h"
#include "tolbi.h"
#include "iwram_call.h"
#include "call.h"
#include "script_layer.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "edition.h"
#include "map_tables.h"
#include "item_ids.h"
#include "objcmd.h"
#include "songs.h"
#include "fixed_math.h"

#if TBS_PATCH_1
#define TOLBI_JAR_SEARCH_KIND                                                  \
  (BG_EVENT(BG_EVENT_JAR) | MAP_EVENT_FLAG_FACING | FACING_NORTH)
#else
#define TOLBI_JAR_SEARCH_KIND BG_EVENT(BG_EVENT_JAR)
#endif

static void Tolbi_FreezePuddle(void);

/// Talking to Ember, a Mars Djinni, makes it join the party. Its index among
/// the Mars Djinn is 4, the fifth Mars name in the game text.
static void Tolbi_JoinEmber(void) {
  Djinn_JoinParty(LOCALID_TOLBI_EMBER, ELEMENT_MARS,
                  DJINNI_INDEX(DJINNI_EMBER));
}

#include "common/object_distance.h"

/// Stops a running child when the target is close by and ahead of it, or
/// always when force is set; otherwise starts the child running again.
/// The child stops when the target is within one facing step of where it
/// looks; for the party leader, within two steps either side.
/// Returns 1 when the child stopped.
static s32 Tolbi_KidStopForTarget(struct ObjectEvent *kid,
                                  struct ObjectEvent *target, s32 range,
                                  s32 force) {
  s32 stopped = 0;
  const u8 *targetPos = (const u8 *)target + offsetof(struct ObjectEvent, x);
  const u8 *kidPos = (const u8 *)kid + offsetof(struct ObjectEvent, x);

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
      ObjectEvent_SetMode(kid, ANIMATION_STAND);
      stopped = 1;
    }
    if (target == ObjectEvent_GetById(LOCALID_PARTY_LEADER) &&
        (farRight == facing || farLeft == facing)) {
      kid->paused = 1;
      ObjectEvent_SetMode(kid, ANIMATION_STAND);
      stopped = 1;
    }
  } else {
    kid->paused = 0;
    ObjectEvent_SetMode(kid, ANIMATION_WALK);
  }
  return stopped;
}

// The update of the two children running their loop. Each stops when
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
  struct ObjectEvent *self = &fieldObject->objectEvent;
  u8 **workAreas = gWindowWork;
  struct WindowWork *ui = (struct WindowWork *)workAreas[0];
  struct FieldWork *work =
      (struct FieldWork *)workAreas[HEAP_SLOT_FIELD - HEAP_SLOT_WINDOW];
  u16 *kidFlags = (u16 *)&self->timer;
  s32 force = 0;
  s32 range = 18;
  struct ObjectEvent *other;
  struct ObjectEvent *leader;

  if ((*kidFlags & KID_CHASES_YOUNGER) != 0) {
    other = ObjectEvent_GetById(LOCALID_TOLBI_YOUNGER_SISTER);
  } else {
    other = ObjectEvent_GetById(LOCALID_TOLBI_OLDER_BROTHER);
  }
  if (Tolbi_KidStopForTarget(self, other, 32, 0) != 0) {
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  }

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  if ((s16)work->talk != 0 || ui->mindRead != 0) {
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

const struct MapWarpDest *Tolbi_GetWarpDests(void) { return gTolbiWarpDests; }

const struct MapWarpRegion *Tolbi_GetRegions(void) { return 0; }

const u32 *Tolbi_GetWarps(void) { return gTolbiWarps; }

const struct ObjectEventTemplate *Tolbi_GetObjectEventTemplates(void) {
  return gTolbiObjectEventTemplates;
}

/// Growth on the sprout: the grown plant no longer blocks the party, and its
/// map cell becomes climbable.
static void Tolbi_GrowSprout(void) {
  struct ObjectEvent *sprout;

  sprout = ObjectEvent_GetById(LOCALID_TOLBI_SPROUT);
  if ((s32)sprout != 0) {
    sprout->collisionFlags = 0;
  }
  sprout = ObjectEvent_GetById(LOCALID_TOLBI_SPROUT);
  Engine_ObjectEventSetSpriteFlags(sprout, 0);
  Field_SetMetatileBehaviorAt(0, PIXELS(544), PIXELS(288), 253);
  Engine_GameFlagSet(FLAG_MAP_TEMP_00);
}

/// Reveal uncovers the hidden chest: its map cell is drawn in and the chest
/// moves into place.
static void Tolbi_RevealHiddenChest(void) {
  MapGrid_CopyCellAttributes(3, 32, 1, 1, 3, 26);
  TreasureSpot_SetPosition(TOLBI_HIDDEN_CHEST, PIXELS(56), PIXELS(424));
}

/// When Reveal wears off, the chest's cell is covered again and the chest
/// moves out of sight.
static void Tolbi_HideRevealedChest(void) {
  MapGrid_CopyCellAttributes(2, 25, 1, 1, 3, 26);
  TreasureSpot_SetPosition(TOLBI_HIDDEN_CHEST, -1, -1);
}

/// The first sign at Tolbi Spring: face away and toss a coin in.
static void Tolbi_ReadSpringSign(void) {
  Engine_ScriptSetMessage(MsgTolbiFaceAwayTolbi);
  Engine_ScriptShowMessage(-1, 0);
}

/// The second sign at Tolbi Spring: a Lucky Medal wins better prizes.
static void Tolbi_ReadLuckyMedalSign(void) {
  Engine_ScriptSetMessage(MsgTolbiTossLuckyMedal);
  Engine_ScriptShowMessage(-1, 0);
}

/// Talking to the older brother, or reading his mind. He stops running while
/// he speaks, then turns back the way he was going.
static void Tolbi_TalkToOlderBrother(void) {
  struct EventWork *work = gFieldWork;
  struct ObjectEvent *kid;
  s16 facing;
  s32 msg;

  kid = ObjectEvent_GetById(LOCALID_TOLBI_OLDER_BROTHER);
  facing = kid->facing;
  Engine_ScriptStart();
  kid->timer |= KID_TALKING;
  if (work->psynergy == 0) {
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
  Engine_ScriptSetMessage(msg);
  Engine_ObjectEventSetAnimation(LOCALID_TOLBI_OLDER_BROTHER, 0);
  Engine_ObjectEventFaceEachOther(LOCALID_TOLBI_OLDER_BROTHER,
                                  LOCALID_PARTY_LEADER, 2);
  Engine_ScriptShowMessageAndWait(LOCALID_TOLBI_OLDER_BROTHER, 0, 10);
  kid->facing = facing;
  Engine_TaskWait(1);
  kid->timer &= KID_CHASES_YOUNGER;
  Engine_ScriptEnd();
}

/// Talking to the younger sister, or reading her mind, as above.
static void Tolbi_TalkToYoungerSister(void) {
  struct EventWork *work = gFieldWork;
  struct ObjectEvent *kid;
  s16 facing;
  s32 msg;

  kid = ObjectEvent_GetById(LOCALID_TOLBI_YOUNGER_SISTER);
  facing = kid->facing;
  Engine_ScriptStart();
  kid->timer |= KID_TALKING;
  if (work->psynergy == 0) {
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
  Engine_ScriptSetMessage(msg);
  Engine_ObjectEventSetAnimation(LOCALID_TOLBI_YOUNGER_SISTER, 0);
  Engine_ObjectEventFaceEachOther(LOCALID_TOLBI_YOUNGER_SISTER,
                                  LOCALID_PARTY_LEADER, 2);
  Engine_ScriptShowMessageAndWait(LOCALID_TOLBI_YOUNGER_SISTER, 0, 10);
  kid->facing = facing;
  Engine_TaskWait(1);
  kid->timer &= KID_CHASES_YOUNGER;
  Engine_ScriptEnd();
}

/// Picks the event table for Tolbi's current mood.
const struct MapEvent *Tolbi_SelectEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    return gTolbiEventsAfterColosso;
  }
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
    return gTolbiEventsFinalsDay;
  }
  return gTolbiEventsFestival;
}

/// A suspicious old woman asks whether anyone can live past 100 years old.
/// She's LOCALID_TOLBI_SUSPICIOUS_OLD_WOMAN, in Tolbi during the festival, and
/// asks whether someone can live to be more than 100 years old; either way the
/// talk turns to Babi, who is older still.
static void Tolbi_AskAboutLivingPastHundred(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgTolbiSomeoneLiveMore);
  Engine_ScriptAskYesNo(LOCALID_TOLBI_SUSPICIOUS_OLD_WOMAN, 0);
  Engine_ScriptEnd();
}

/// The father asks whether children really do love souvenirs, and owns up to
/// loving them himself.
static void Tolbi_TalkToFather(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgTolbiChildrenLoveSouvenirs);
  Engine_ScriptAskYesNo(LOCALID_TOLBI_FATHER, 0);
  Engine_ScriptEnd();
}

/// The mother turns to her crying boy, then back to promise better sweets
/// later on. "Be patient, little guy."
static void Tolbi_TalkToMother(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgTolbiPatientLittleGuy);
  ObjectEvent_FaceDirection(LOCALID_TOLBI_MOTHER, FACING_NORTH, 0);
  Engine_ScriptShowMessage(LOCALID_TOLBI_MOTHER, 0);
  ObjectEvent_FaceDirection(LOCALID_TOLBI_MOTHER, FACING_WEST, 0);
  Engine_ScriptShowMessage(LOCALID_TOLBI_MOTHER, 0);
  Engine_ScriptEnd();
}

/// The crying boy turns, stamps twice and wails for sweets.
static void Tolbi_TalkToCryingBoy(void) {
  Engine_ScriptStart();
  ObjectEvent_FaceDirection(LOCALID_TOLBI_CRYING_BOY, FACING_SOUTH, 0);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_TOLBI_CRYING_BOY, 2);
  Engine_ScriptSetMessage(MsgTolbiWaahBuySweets);
  Engine_ScriptShowMessage(LOCALID_TOLBI_CRYING_BOY, 0);
  Engine_ScriptEnd();
}

/// During the festival the scalper sleeps under the archway: the inns are
/// full.
static void Tolbi_TalkToSleepingScalper(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgTolbiInnsFullStaying);
  Engine_ScriptShowMessage(LOCALID_TOLBI_SCALPER, 0);
  Engine_ScriptEnd();
}

/// After Colosso, LOCALID_TOLBI_FINALS_FAN asks whether the party was in the
/// finals. The question is anchored to the father, who has left town by then.
static void Tolbi_AskWhetherPartyWasInFinals(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgTolbiFinalsWerent);
  Engine_ScriptAskYesNo(LOCALID_TOLBI_FATHER, 0);
  Engine_ScriptEnd();
}

/// After Colosso, the snack vendor gives the party his leftover corn, once.
static void Tolbi_TalkToSnackVendor(void) {
  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_TOLBI_CORN_GIVEN) == 0) {
    Engine_GameFlagSet(FLAG_TOLBI_CORN_GIVEN);
    Engine_ScriptSetMessage(MsgTolbiLeftovers);
    Engine_ScriptShowMessage(LOCALID_TOLBI_SNACK_VENDOR, 0);
    Engine_ItemShowFound(ITEM_CORN, 3);
    Engine_ScriptShowMessage(LOCALID_TOLBI_SNACK_VENDOR, 0);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
    Engine_PartyGiveItem(ITEM_CORN, 0);
  } else {
    Engine_ScriptSetMessage(MsgTolbiFestivalLongerUsual);
    Engine_ScriptShowMessage(LOCALID_TOLBI_SNACK_VENDOR, 0);
  }
  Engine_ScriptEnd();
}

/// Walking into one of the town's doors: the door opens, the leader steps in
/// (except at TOLBI_TO_WEAPON_AND_ARMOR_SHOP) and the map changes through the
/// door's warp, numbered as its metatile behavior.
static void Tolbi_EnterDoor(void) {
  struct EventWork *work = gFieldWork;
  u32 localId;
  s32 door;

  Engine_ScriptStart();
  for (localId = LOCALID_FIRST_PLACED; localId <= 65; localId++) {
    struct ObjectEvent *objectEvent = ObjectEvent_GetById(localId);
    if (objectEvent != NULL) {
      objectEvent->motionFlags = 0;
    }
  }
  door = work->coordEvent - 1;
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gTolbiDoors[door].steps, gTolbiDoors[door].x,
                       gTolbiDoors[door].y);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 0;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_WALK);
  if (door != TOLBI_TO_WEAPON_AND_ARMOR_SHOP - 1) {
    ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 2, -8);
    Engine_ScriptWait(10);
  }
  Engine_ScriptRequestWarp(work->coordEvent);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptEnd();
}

/// The update of LOCALID_TOLBI_OLD_MAN, who during the festival says he is 65
/// and that Babi was already old when he was born: gives him the leader's
/// sprite priority every frame, for both of his sprite parts.
static void Tolbi_CopyLeaderPriority(union FieldObject *fieldObject) {
  s32 priority;

  if (fieldObject != NULL) {
    priority =
        ObjectEvent_GetById(LOCALID_PARTY_LEADER)->sprite->image.priority;
    fieldObject->objectEvent.priorityFlags = 0;
    fieldObject->objectEvent.sprite->image.priority = priority;
    fieldObject->objectEvent.sprite->shadow.priority = priority;
  }
}

/// Sets up the map on entry: the running children, LOCALID_TOLBI_OLD_MAN's
/// update, and everything the flags have already changed.
s32 Tolbi_ApplyEntryState(s32 unused) {
  struct ObjectEvent *objectEvent;
  void (*kidUpdate)(union FieldObject *);
  s32 clearFlags;

  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  // The two children run their loop south-east of the fountain.
  ObjectEvent_SetPosition(LOCALID_TOLBI_OLDER_BROTHER, PIXELS(352),
                          PIXELS(352));
  ObjectEvent_SetActionScript(LOCALID_TOLBI_OLDER_BROTHER,
                              gTolbiOlderBrotherRun);
  objectEvent = ObjectEvent_GetById(LOCALID_TOLBI_OLDER_BROTHER);
#ifdef UBFIX
  kidUpdate = Tolbi_UpdateKid;
#else
  kidUpdate = (void (*)(union FieldObject *))Tolbi_UpdateKid;
#endif
  objectEvent->timer = KID_CHASES_YOUNGER;
  objectEvent->update = kidUpdate;
  clearFlags = 0;
  Engine_ObjectEventSetPosition(LOCALID_TOLBI_YOUNGER_SISTER, PIXELS(368),
                                PIXELS(320));
  ObjectEvent_SetActionScript(LOCALID_TOLBI_YOUNGER_SISTER,
                              gTolbiYoungerSisterRun);
  objectEvent = ObjectEvent_GetById(LOCALID_TOLBI_YOUNGER_SISTER);
  objectEvent->timer = clearFlags;
  objectEvent->update = kidUpdate;
  objectEvent = ObjectEvent_GetById(LOCALID_TOLBI_OLD_MAN);
  objectEvent->update = Tolbi_CopyLeaderPriority;

  // What the party has already done here.
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SPRING_OPENED) != 0) {
    ObjectEvent_SetPosition(LOCALID_TOLBI_SPRING_VISITOR, PIXELS(316),
                            PIXELS(328));
  }
  // The puddle is frozen.
  if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_01) != 0) {
    Tolbi_FreezePuddle();
  }
  // The sprout has grown.
  if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_00) != 0) {
    Tolbi_GrowSprout();
    Engine_ObjectEventSetAnimation(LOCALID_TOLBI_SPROUT, 4);
  }

  // After Colosso the crowd has gone home and is moved off the map. On finals
  // day the scalper waits by the gate.
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    ObjectEvent_SetPosition(LOCALID_TOLBI_WOULD_BE_FIGHTER, PIXELS(520),
                            PIXELS(560));
    ObjectEvent_SetPosition(LOCALID_TOLBI_XIAN_VISITOR, PIXELS(520),
                            PIXELS(560));
    ObjectEvent_SetPosition(LOCALID_TOLBI_OVERSLEEPER, PIXELS(520),
                            PIXELS(560));
    ObjectEvent_SetPosition(LOCALID_TOLBI_FATHER, PIXELS(520), PIXELS(560));
    ObjectEvent_SetPosition(LOCALID_TOLBI_MOTHER, PIXELS(520), PIXELS(560));
    ObjectEvent_SetPosition(LOCALID_TOLBI_CRYING_BOY, PIXELS(520), PIXELS(560));
    ObjectEvent_SetPosition(LOCALID_TOLBI_SCALPER, PIXELS(520), PIXELS(560));
  } else {
    if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      ObjectEvent_SetPosition(LOCALID_TOLBI_SCALPER, PIXELS(280), PIXELS(80));
      ObjectEvent_FaceDirection(LOCALID_TOLBI_SCALPER, FACING_SOUTHEAST, 0);
      Engine_ObjectEventSetAnimation(LOCALID_TOLBI_SCALPER, ANIMATION_STAND);
    }
  }
  return 0;
}

/// The visitor at the spring's gate asks whether the party wants to test its
/// luck. On yes he steps aside for good and the way to Tolbi Spring opens.
static void Tolbi_TalkToSpringVisitor(void) {
  Engine_ScriptStart();
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 304, 312);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  Engine_ObjectEventFaceDirection(LOCALID_TOLBI_SPRING_VISITOR, FACING_SOUTH,
                                  0);
  Engine_ScriptWait(20);
  Engine_ScriptSetMessage(MsgTolbiWantTestLuck);
  Script_OpenMessage(LOCALID_TOLBI_SPRING_VISITOR, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    // Yes: "I'll give you the chance to go first."
    gFieldWork->message++;
    Engine_ScriptShowMessage(LOCALID_TOLBI_SPRING_VISITOR, 0);
    ObjectEvent_SetSpeed(LOCALID_TOLBI_SPRING_VISITOR, PIXELS(1),
                         PIXELS(1) / 2);
    ObjectEvent_WalkToAndWait(LOCALID_TOLBI_SPRING_VISITOR, 320, 304);
    ObjectEvent_WalkToAndWait(LOCALID_TOLBI_SPRING_VISITOR, 316, 328);
    Engine_ObjectEventFaceDirection(LOCALID_TOLBI_SPRING_VISITOR,
                                    FACING_NORTHWEST, 0);
    Engine_GameFlagSet(FLAG_TOLBI_SPRING_OPENED);
  } else {
    Engine_ScriptShowMessage(LOCALID_TOLBI_SPRING_VISITOR, 0);
  }
  Engine_ScriptEnd();
}

/// The gate to Tolbi Spring (TOLBI_CELL_SPRING_GATE), once the visitor at the
/// gate has stepped aside.
static void Tolbi_EnterSpring(void) {
  Engine_ScriptRequestWarp(TOLBI_TO_TOLBI_SPRING);
  Engine_ScriptEnd();
}

/// Leaves by the warp numbered as the metatile behavior the party walked into.
static void Tolbi_RequestTouchedWarp(void) {
  s16 *trigger = &gFieldWork->coordEvent;

  Engine_ScriptRequestWarp(*trigger);
}

/// Two soldiers searching for the missing Babi stop the party and show his
/// picture. They do so during the festival, the first time the party steps on
/// TOLBI_CELL_SOLDIERS. "Have you seen anyone who looks like this?" They leave
/// either way the party answers. Sets FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI.
static void Tolbi_MeetSearchingSoldiers(void) {
  s32 firstMsg;

  Engine_ScriptStart();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  ObjectEvent_SetSpeed(LOCALID_TOLBI_SOLDIER_A, PIXELS(1), PIXELS(1) / 2);
  Engine_ObjectEventSetSpeed(LOCALID_TOLBI_SOLDIER_B, PIXELS(1), PIXELS(1) / 2);
  firstMsg = MsgTolbiSeenAnyoneWho;
  Engine_ScriptSetMessage(firstMsg);

  // The soldiers come up to the party; the poster waits off to the side.
  ObjectEvent_SetPosition(LOCALID_TOLBI_SOLDIER_A, PIXELS(72), PIXELS(208));
  ObjectEvent_SetPosition(LOCALID_TOLBI_SOLDIER_B, PIXELS(56), PIXELS(208));
  Engine_ObjectEventSetChildValue(LOCALID_TOLBI_MISSING_PERSON_POSTER, 15);
  Engine_ObjectEventSetSpriteFlags(
      ObjectEvent_GetById(LOCALID_TOLBI_MISSING_PERSON_POSTER), 0);
  ObjectEvent_SetPosition(LOCALID_TOLBI_MISSING_PERSON_POSTER, PIXELS(95),
                          PIXELS(40));
  Engine_ObjectEventWalkTo(LOCALID_TOLBI_SOLDIER_A, 72, 248);
  Engine_ObjectEventWalkTo(LOCALID_TOLBI_SOLDIER_B, 56, 248);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 64, 264);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  Engine_ObjectEventWaitForMove(LOCALID_TOLBI_SOLDIER_A);
  Engine_ObjectEventSetAnimation(LOCALID_TOLBI_SOLDIER_A, ANIMATION_STAND);
  Engine_ObjectEventSetAnimation(LOCALID_TOLBI_SOLDIER_B, ANIMATION_STAND);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_ObjectEventFaceObjectEvent(LOCALID_TOLBI_SOLDIER_A,
                                    LOCALID_PARTY_LEADER, 0);
  Engine_ObjectEventFaceObjectEvent(LOCALID_TOLBI_SOLDIER_B,
                                    LOCALID_PARTY_LEADER, 0);
  Engine_ScriptWait(20);
  ObjectEvent_SetAttachedEffect(
      LOCALID_TOLBI_SOLDIER_A, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(
      LOCALID_TOLBI_SOLDIER_B, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_TOLBI_SOLDIER_A, 2);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_TOLBI_SOLDIER_B, 2);
  Engine_ScriptWait(20);

  // "Have you seen anyone who looks like this?", under a "Missing Person"
  // window showing Babi's portrait. The poster, placed with Babi's sprite,
  // becomes the speaker so that answering closes that portrait window.
  Script_OpenMessage(LOCALID_TOLBI_SOLDIER_A, 0);
  Engine_ScriptWait(25);
  Portrait_OpenWindow(SPRITE_BABI, 0, 12, 7);
  Message_OpenWindow((firstMsg + 3), 11, 12, 2);
  gFieldWork->speaker = LOCALID_TOLBI_MISSING_PERSON_POSTER;
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    // Yes: "Babi must be nearby!!!" They nod and run east.
    Engine_ScriptWait(20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_TOLBI_SOLDIER_B, 2);
    Engine_ScriptWait(30);
    Engine_ObjectEventFaceDirection(LOCALID_TOLBI_SOLDIER_B, FACING_EAST, 0);
    Engine_ScriptWait(30);
    Engine_ScriptWait(10);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_TOLBI_SOLDIER_A,
                                          ANIMATION_NOD);
    Engine_ScriptWait(20);
    Engine_ObjectEventFaceDirection(LOCALID_TOLBI_SOLDIER_A, FACING_EAST, 0);
    Engine_ScriptWait(30);
    Engine_ScriptShowMessage(LOCALID_TOLBI_SOLDIER_A, 0);
    Engine_ScriptWait(20);
    ObjectEvent_FaceDirection(LOCALID_TOLBI_SOLDIER_A, FACING_SOUTH, 0);
    Engine_ObjectEventFaceDirection(LOCALID_TOLBI_SOLDIER_B, FACING_SOUTH, 0);
    Engine_ScriptWait(30);
    Engine_ObjectEventSetAnimation(LOCALID_TOLBI_SOLDIER_A, ANIMATION_NOD);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_TOLBI_SOLDIER_B,
                                          ANIMATION_NOD);
    Engine_ScriptWait(20);
    ObjectEvent_SetSpeed(LOCALID_TOLBI_SOLDIER_A, PIXELS(9) / 5,
                         PIXELS(9) / 10);
    ObjectEvent_SetSpeed(LOCALID_TOLBI_SOLDIER_B, PIXELS(9) / 5,
                         PIXELS(9) / 10);
    ObjectEvent_WalkTo(LOCALID_TOLBI_SOLDIER_A, 232, 248);
    Engine_ScriptWait(2);
    ObjectEvent_WalkTo(LOCALID_TOLBI_SOLDIER_B, 232, 248);
    Engine_ObjectEventWaitForMove(LOCALID_TOLBI_SOLDIER_A);
    Engine_ObjectEventWalkTo(LOCALID_TOLBI_SOLDIER_A, 248, 248);
    Engine_ObjectEventWalkToAndWait(LOCALID_TOLBI_SOLDIER_B, 248, 248);
  } else {
    // No: "He doesn't seem to be here. Let's look over there." They shake
    // their heads and go back north.
    Engine_ScriptWait(20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_TOLBI_SOLDIER_B, 2);
    Engine_ScriptWait(30);
    Engine_ObjectEventFaceDirection(LOCALID_TOLBI_SOLDIER_B, FACING_EAST, 0);
    Engine_ScriptWait(30);
    Engine_ScriptWait(10);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_TOLBI_SOLDIER_A,
                                          ANIMATION_SHAKE_HEAD);
    Engine_ScriptWait(20);
    Engine_ObjectEventFaceDirection(LOCALID_TOLBI_SOLDIER_A, FACING_EAST, 0);
    Engine_ScriptWait(30);
    gFieldWork->message += 1;
    Engine_ScriptShowMessage(LOCALID_TOLBI_SOLDIER_A, 0);
    Engine_ScriptWait(20);
    ObjectEvent_FaceDirection(LOCALID_TOLBI_SOLDIER_A, FACING_SOUTH, 0);
    ObjectEvent_FaceDirection(LOCALID_TOLBI_SOLDIER_B, FACING_SOUTH, 0);
    Engine_ScriptWait(30);
    Engine_ObjectEventSetAnimation(LOCALID_TOLBI_SOLDIER_A, ANIMATION_NOD);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_TOLBI_SOLDIER_B,
                                          ANIMATION_NOD);
    Engine_ScriptWait(20);
    ObjectEvent_SetSpeed(LOCALID_TOLBI_SOLDIER_A, PIXELS(8) / 5, PIXELS(4) / 5);
    ObjectEvent_SetSpeed(LOCALID_TOLBI_SOLDIER_B, PIXELS(8) / 5, PIXELS(4) / 5);
    Engine_ObjectEventWalkTo(LOCALID_TOLBI_SOLDIER_A, 72, 184);
    Engine_ObjectEventWalkToAndWait(LOCALID_TOLBI_SOLDIER_B, 56, 184);
  }
  Engine_ObjectEventSetPosition(LOCALID_TOLBI_SOLDIER_A, 0, 0);
  Engine_ObjectEventSetPosition(LOCALID_TOLBI_SOLDIER_B, 0, 0);
  Engine_ObjectEventSetPosition(LOCALID_TOLBI_MISSING_PERSON_POSTER, 0, 0);
  Engine_GameFlagSet(FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI);
  Engine_ScriptEnd();
}

/// Frost on the puddle: the frozen pillar is drawn underfoot and stops
/// following the terrain, and its map cell becomes solid.
static void Tolbi_FreezePuddle(void) {
  struct ObjectEvent *puddle = ObjectEvent_GetById(LOCALID_TOLBI_PUDDLE);
  if (puddle != 0) {
    Engine_ObjectEventSetSpriteFlags(puddle, 0);
    puddle->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
    puddle->motionFlags = 0;
  }
  Engine_ObjectEventSetAnimation(LOCALID_TOLBI_PUDDLE, 5);
  {
    MapGrid_CopyCellAttributes(36, 16, 1, 1, 34, 16);
  }
  Engine_GameFlagSet(FLAG_MAP_TEMP_01);
}

/// The action script of a mirrored object event: flips its sprite and passes 7
/// on.
s32 gTolbiMirroredObjectEvent[] = {
    SC_SET(SW_SCALE_X, -Q16_ONE),
    SC_ANIMATION(7),
    SC_END,
};

/// The older brother runs a loop of eight points, 32 pixels across, south-east
/// of the fountain.
s32 gTolbiOlderBrotherRun[] = {
    SC_ANIMATION(2),
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 5),
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
    SC_ANIMATION(2),
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 5),
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

struct MapWarpDest gTolbiWarpDests[] = {
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
    {MAP_TABLE_END},
};

u32 gTolbiWarps[] = {
    MAP_WARPS(Tolbi),
    MAP_WARP(TOLBI_TO_HOUSE_SOUTH, TolbiInteriors, TOLBI_INTERIORS_HOUSE_SOUTH),
    MAP_WARP(TOLBI_TO_HOUSE_NORTHWEST, TolbiInteriors,
             TOLBI_INTERIORS_HOUSE_NORTHWEST),
    MAP_WARP(TOLBI_TO_INN, TolbiInteriors, TOLBI_INTERIORS_INN),
    MAP_WARP(TOLBI_TO_LUCKY_WHEELS, TolbiGameRooms,
             TOLBI_GAME_ROOMS_LUCKY_WHEELS_FROM_TOLBI),
    MAP_WARP(TOLBI_TO_HOUSE_NORTHEAST, TolbiInteriors,
             TOLBI_INTERIORS_HOUSE_NORTHEAST),
    MAP_WARP(TOLBI_TO_HOUSE_EAST, TolbiInteriors, TOLBI_INTERIORS_HOUSE_EAST),
    MAP_WARP(TOLBI_TO_WEAPON_AND_ARMOR_SHOP, TolbiInteriors,
             TOLBI_INTERIORS_WEAPON_AND_ARMOR_SHOP),
    MAP_WARP(TOLBI_TO_HOUSE_SOUTHWEST, TolbiInteriors,
             TOLBI_INTERIORS_HOUSE_SOUTHWEST),
    MAP_WARP(TOLBI_TO_SANCTUM, TolbiInteriors, TOLBI_INTERIORS_SANCTUM),
    MAP_WARP(TOLBI_TO_BABI_PALACE_ENTRANCE, BabiPalaceEntrance,
             BABI_PALACE_ENTRANCE_FROM_TOLBI),
    MAP_WARP(TOLBI_TO_COLOSSEUM_ENTRANCE, ColosseumEntrance,
             COLOSSEUM_ENTRANCE_FROM_TOLBI),
    MAP_WARP(TOLBI_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_TOLBI),
    MAP_WARP(TOLBI_TO_LUCKY_DICE, TolbiGameRooms,
             TOLBI_GAME_ROOMS_LUCKY_DICE_FROM_TOLBI),
    MAP_WARP(TOLBI_TO_TOLBI_SPRING, TolbiSpring, TOLBI_SPRING_FROM_TOLBI),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gTolbiObjectEventTemplates[] = {
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(552), 0, PIXELS(288), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(552), 0, PIXELS(264), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(264), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(208), 0, PIXELS(312), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(224), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(216), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(152), 0, PIXELS(304), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(24), 0, PIXELS(456), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (s32)gTolbiOlderBrotherRun,
        PIXELS(344), 0, PIXELS(328), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (s32)gTolbiYoungerSisterRun,
        PIXELS(376), 0, PIXELS(328), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(200), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUTH_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(400), 0, PIXELS(432), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUTH_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(184), 0, PIXELS(472), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(256), 0, PIXELS(168), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(184), FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(272), 0, PIXELS(168), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, (s32)gTolbiMirroredObjectEvent,
        PIXELS(312), 0, PIXELS(120), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(302), 0, PIXELS(296), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-9), 0, PIXELS(488), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-9), 0, PIXELS(488), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    // Ember, until its Djinni flag is set: twenty flags to an element.
    { MARS_DJINNI, FLAG_DJINNI(DJINNI_EMBER), MOVEMENT_TYPE_STAND,
        PIXELS(584), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BABI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-9), 0, PIXELS(488), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

/// During the festival, before the finals.
struct MapEvent gTolbiEventsFestival[] = {
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_GROWTH), LOCALID_TOLBI_SPROUT, FLAG_MAP_TEMP_00, EVENT_SCRIPT(Tolbi_GrowSprout) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTH, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTHWEST, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_INN, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_LUCKY_WHEELS, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTHEAST, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTHWEST, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_SANCTUM, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { MAP_EVENT_COORD | MAP_EVENT_FLAG_FACING | FACING_NORTH, TOLBI_TO_BABI_PALACE_ENTRANCE, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedWarp) },
    { MAP_EVENT_COORD | MAP_EVENT_FLAG_FACING | FACING_NORTH, TOLBI_TO_COLOSSEUM_ENTRANCE, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedWarp) },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, TOLBI_TO_WORLD_MAP },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), TOLBI_TO_WORLD_MAP, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedWarp) },
    { MAP_EVENT_COORD | MAP_EVENT_FLAG_FACING | FACING_NORTH, TOLBI_TO_LUCKY_DICE, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedWarp) },
    { MAP_EVENT_COORD, TOLBI_CELL_SOLDIERS, FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI, EVENT_SCRIPT(Tolbi_MeetSearchingSoldiers) },
    { MAP_EVENT_COORD | MAP_EVENT_FLAG_FACING | FACING_NORTH, TOLBI_CELL_SPRING_GATE, IF_FLAG_SET(FLAG_TOLBI_SPRING_OPENED), EVENT_SCRIPT(Tolbi_EnterSpring) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTolbisAlwaysReallyBustlingDuringColosso) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiColossoIsHeldInTheColosseum) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTryoutsStartedAtTheColosseumToday) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiColossoShouldHaveStartedTodayWhat) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiBabiWasAlreadyPrettyOldBack) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_AskAboutLivingPastHundred) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSister) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiGetYourColossoSouvenirsTheyMake) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiGetYourSnacksAndDrinksBefore) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiIveDecidedNotToFightAt) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiWowTolbisFestivalIsHumongousJust) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiRatsWeOversleptAndNowThe) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_FATHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToFather) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_MOTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToMother) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_CRYING_BOY, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToCryingBoy) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SCALPER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToSleepingScalper) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SPRING_VISITOR, FLAG_TOLBI_SPRING_OPENED, EVENT_SCRIPT(Tolbi_TalkToSpringVisitor) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiAhIGuessIJustDont) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTouristsArentTheOnlyOnesWho) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheTrialsAreStartingAtThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheTrialsAreOkayButThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiItsNotLikeThereHaveBeen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiEveryYearIGetOlderAnd) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiWhatIfBabiIsActuallyA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSister) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiWeHaveToSellAllOf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTheyreSellingTheseAtTheColosseum) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiISawThePeopleWhoAre) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTolbiIsNotLikeXianIt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiICantSayAnythingCauseIt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_FATHER, CONDITION_ALWAYS, MSGNO(MsgTolbiTheStuffTheyreSellingAtThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_MOTHER, CONDITION_ALWAYS, MSGNO(MsgTolbiWhyDoKidsWantSuchAwful) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiThoseSweetsSmellGreatImNot) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_SCALPER, CONDITION_ALWAYS, MSGNO(MsgTolbiIveBeenGuardingOurSpotBut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiIKeepLosingCoinsButI) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_EMBER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_JoinEmber) },
    { BG_EVENT(BG_EVENT_SILENT), TOLBI_SPRING_SIGN, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadSpringSign) },
    { BG_EVENT(BG_EVENT_SILENT), TOLBI_LUCKY_MEDAL_SIGN, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadLuckyMedalSign) },
    { BG_EVENT(BG_EVENT_BARREL), TOLBI_BARREL, FLAG_TOLBI_BARREL_COINS, FIND_COINS(9) },
// The international editions find the jar only when the party faces north.
    { TOLBI_JAR_SEARCH_KIND, TOLBI_JAR, FLAG_TOLBI_JAR_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_CHEST), TOLBI_HIDDEN_CHEST, FLAG_TOLBI_CHEST_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_TOLBI_PUDDLE, FLAG_MAP_TEMP_01, EVENT_SCRIPT(Tolbi_FreezePuddle) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RevealHiddenChest) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_HideRevealedChest) },
    { MAP_EVENTS_END },
};

/// On finals day.
struct MapEvent gTolbiEventsFinalsDay[] = {
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_GROWTH), LOCALID_TOLBI_SPROUT, FLAG_MAP_TEMP_00, EVENT_SCRIPT(Tolbi_GrowSprout) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTH, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTHWEST, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_INN, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_LUCKY_WHEELS, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTHEAST, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTHWEST, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_SANCTUM, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { MAP_EVENT_COORD | MAP_EVENT_FLAG_FACING | FACING_NORTH, TOLBI_TO_BABI_PALACE_ENTRANCE, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedWarp) },
    { MAP_EVENT_COORD | MAP_EVENT_FLAG_FACING | FACING_NORTH, TOLBI_TO_COLOSSEUM_ENTRANCE, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedWarp) },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, TOLBI_TO_WORLD_MAP },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), TOLBI_TO_WORLD_MAP, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedWarp) },
    { MAP_EVENT_COORD | MAP_EVENT_FLAG_FACING | FACING_NORTH, TOLBI_TO_LUCKY_DICE, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedWarp) },
    { MAP_EVENT_COORD | MAP_EVENT_FLAG_FACING | FACING_NORTH, TOLBI_CELL_SPRING_GATE, IF_FLAG_SET(FLAG_TOLBI_SPRING_OPENED), EVENT_SCRIPT(Tolbi_EnterSpring) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiColossoWillReachItsApexAny) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiAreYouPlanningToWatchThe) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiMyFamilySavedMeASeat) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiIThoughtSomethingHappenedToBabi) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheColosseumWasSoFullThat) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiNobodyElseHasNoticedButThis) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSister) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiHowAboutSomeColossoSouvenirsThis) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTheFinalsAreSoLongIts) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiTheTrialsWereSoRivetingThe) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiIHaveSeatsToTheFinals) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiIOversleptAgainSoICant) },
    // The family repeats its festival lines on finals day; the file's open
    // questions ask whether four unused messages were meant for it.
    { MAP_EVENT_TALK, LOCALID_TOLBI_FATHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToFather) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_MOTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToMother) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_CRYING_BOY, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToCryingBoy) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SCALPER, CONDITION_ALWAYS, MSGNO(MsgTolbiTheFinalsStartTodayIWonder) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SPRING_VISITOR, FLAG_TOLBI_SPRING_OPENED, EVENT_SCRIPT(Tolbi_TalkToSpringVisitor) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiAhIGuessIJustDont) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiIveGotToGetToThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiABattleToTheDeathWith) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheFinalsStartOnBabisSignal) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiSoWhyWereTheSoldiersAll) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiICouldStillBuySeatsFrom) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiNoMatterHowIAddIt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSister) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiWeCantSellAnythingOnceThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiThisIsTheFinalPushAnything) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiFightingInColossoRequiresALot) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTheyreSellingFinalsSeatsRightAt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiItsSupposedToBeFreeHaving) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_FATHER, CONDITION_ALWAYS, MSGNO(MsgTolbiOnceColossoEndsOurLivesCan) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_MOTHER, CONDITION_ALWAYS, MSGNO(MsgTolbiFestivalsAreSoExpensiveMyWallets) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiMomYouSaidYouWereAll) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_SCALPER, CONDITION_ALWAYS, MSGNO(MsgTolbiTodayIsABigDayFor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiIKeepLosingCoinsButI) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_EMBER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_JoinEmber) },
    { BG_EVENT(BG_EVENT_SILENT), TOLBI_SPRING_SIGN, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadSpringSign) },
    { BG_EVENT(BG_EVENT_SILENT), TOLBI_LUCKY_MEDAL_SIGN, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadLuckyMedalSign) },
    { BG_EVENT(BG_EVENT_BARREL), TOLBI_BARREL, FLAG_TOLBI_BARREL_COINS, FIND_COINS(9) },
    { BG_EVENT(BG_EVENT_JAR), TOLBI_JAR, FLAG_TOLBI_JAR_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_CHEST), TOLBI_HIDDEN_CHEST, FLAG_TOLBI_CHEST_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_TOLBI_PUDDLE, FLAG_MAP_TEMP_01, EVENT_SCRIPT(Tolbi_FreezePuddle) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RevealHiddenChest) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_HideRevealedChest) },
    { MAP_EVENTS_END },
};

/// After Colosso has ended.
struct MapEvent gTolbiEventsAfterColosso[] = {
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_GROWTH), LOCALID_TOLBI_SPROUT, FLAG_MAP_TEMP_00, EVENT_SCRIPT(Tolbi_GrowSprout) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTH, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTHWEST, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_INN, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_LUCKY_WHEELS, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTHEAST, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTHWEST, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TOLBI_TO_SANCTUM, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_EnterDoor) },
    { MAP_EVENT_COORD | MAP_EVENT_FLAG_FACING | FACING_NORTH, TOLBI_TO_BABI_PALACE_ENTRANCE, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedWarp) },
    { MAP_EVENT_COORD | MAP_EVENT_FLAG_FACING | FACING_NORTH, TOLBI_TO_COLOSSEUM_ENTRANCE, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedWarp) },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, TOLBI_TO_WORLD_MAP },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), TOLBI_TO_WORLD_MAP, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedWarp) },
    { MAP_EVENT_COORD | MAP_EVENT_FLAG_FACING | FACING_NORTH, TOLBI_TO_LUCKY_DICE, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RequestTouchedWarp) },
    { MAP_EVENT_COORD | MAP_EVENT_FLAG_FACING | FACING_NORTH, TOLBI_CELL_SPRING_GATE, IF_FLAG_SET(FLAG_TOLBI_SPRING_OPENED), EVENT_SCRIPT(Tolbi_EnterSpring) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiWellThisYearsColossoHasEnded) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiItsBackToOurNormalQuiet) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_FINALS_FAN, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_AskWhetherPartyWasInFinals) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiWithAllThoseRevelersGoneThe) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiIHadNoLuckInThe) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiItsSoQuietTheHustleAnd) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSister) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiIllBeBackToDoBusiness) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToSnackVendor) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SPRING_VISITOR, FLAG_TOLBI_SPRING_OPENED, EVENT_SCRIPT(Tolbi_TalkToSpringVisitor) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiAhIGuessIJustDont) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheSpectatorsHaveAllLeftSo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiThinkHowFunItWouldBe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiAllTheWarriorsWhoGainFame) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiComeToThinkOfItNo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiHmmSinceColossoEndedSoDramatically) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheCalmAfterTheStormIs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToOlderBrother) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_TalkToYoungerSister) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiEveryYearITryToSell) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiHowCanWeGetHomeIf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiIKeepLosingCoinsButI) },
    { MAP_EVENT_TALK, LOCALID_TOLBI_EMBER, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_JoinEmber) },
    { BG_EVENT(BG_EVENT_SILENT), TOLBI_SPRING_SIGN, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadSpringSign) },
    { BG_EVENT(BG_EVENT_SILENT), TOLBI_LUCKY_MEDAL_SIGN, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_ReadLuckyMedalSign) },
    { BG_EVENT(BG_EVENT_BARREL), TOLBI_BARREL, FLAG_TOLBI_BARREL_COINS, FIND_COINS(9) },
    { BG_EVENT(BG_EVENT_JAR), TOLBI_JAR, FLAG_TOLBI_JAR_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_CHEST), TOLBI_HIDDEN_CHEST, FLAG_TOLBI_CHEST_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_TOLBI_PUDDLE, FLAG_MAP_TEMP_01, EVENT_SCRIPT(Tolbi_FreezePuddle) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_RevealHiddenChest) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Tolbi_HideRevealedChest) },
    { MAP_EVENTS_END },
};
// clang-format on

/// How a door opens: two frames of cells copied over the closed door.
u16 gTolbiDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(39, 32, 1, 2, 6),
    MAPGRID_ANIMATION_STEP(40, 32, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The doors, in the order of their metatile behaviors 1 to 9, by map cell.
struct MapGridCellAnimation gTolbiDoors[] = {
    {gTolbiDoorOpening, 48, 6},  {gTolbiDoorOpening, 63, 4},
    {gTolbiDoorOpening, 71, 8},  {gTolbiDoorOpening, 71, 4},
    {gTolbiDoorOpening, 66, 21}, {gTolbiDoorOpening, 61, 24},
    {gTolbiDoorOpening, 50, 26}, {gTolbiDoorOpening, 50, 22},
    {gTolbiDoorOpening, 43, 20},
};
