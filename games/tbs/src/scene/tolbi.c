/// @file
/// Tolbi (SceneId_Tolbi), the town Babi rules, where Colosso, a festival of
/// warriors, is held each year in the colosseum. The party comes to Tolbi
/// across the Karagol Sea from Kalay while the festival is on, and the town's
/// talk follows the Colosso story in three stages. Tolbi_SelectTriggers gives
/// each stage its own trigger table; one actor placement table serves all
/// three, and Tolbi_ApplyEntryState moves the actors whose places
/// change:
///
/// - The festival (neither story flag set). The trials have begun at the
///   colosseum, but Babi is not there, and his soldiers are searching the
///   town. The first time the party steps on CELL_BEHAVIOR_TOLBI_SOLDIERS, two
///   of them stop it to show his picture, `Have you seen anyone who looks like
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
/// FLAG_TOLBI_SPRING_OPENED), and the gate (CELL_BEHAVIOR_TOLBI_GATE_TO_SPRING)
/// leads into Tolbi Spring (TOLBI_TO_TOLBI_SPRING). Ember, a Mars Djinni, joins
/// the party when spoken to. Growth raises a sprout and Frost freezes a puddle,
/// each a scene flag kept only through a return that keeps the party's
/// position; Reveal uncovers a chest with Power Bread, and a barrel and a jar
/// hold 9 coins and a Lucky Medal (during the festival, the editions from
/// TBS_PATCH_1 on find the jar only when the party faces north).
///
/// The doors from TOLBI_TO_HOUSE_SOUTH to TOLBI_TO_SANCTUM, whose cells carry
/// their exits' numbers, lead into Tolbi's houses, inns and shops and into
/// Tolbi Sanctum (all SceneId_StuccoRoomsB), but for TOLBI_TO_LUCKY_WHEELS,
/// which with TOLBI_TO_LUCKY_DICE leads into the Lucky Dice and Lucky Wheels
/// rooms (SceneId_StuccoRoomsC, whose area name is also Tolbi).
/// TOLBI_TO_BABI_PALACE_ENTRANCE leads to Babi's Palace,
/// TOLBI_TO_COLOSSEUM_ENTRANCE to the Colosseum Entrance and TOLBI_TO_WORLD_MAP
/// out onto the World Map.

#include "unit/scene/tolbi.h"
#include "djinn_ids.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "unit/runtime/runtime.h"
#include "tolbi.h"
#include "iwram_call.h"
#include "call.h"
#include "event.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "edition.h"
#include "scene_tables.h"
#include "item_ids.h"
#include "actor_script.h"
#include "songs.h"
#include "fixed_math.h"
#include "map_grid.h"

#if TBS_PATCH_1
#define TOLBI_JAR_SEARCH_KIND                                                  \
  (EXAMINE_TRIGGER(EXAMINE_JAR) | TRIGGER_FLAG_FACING | FACING_NORTH)
#else
#define TOLBI_JAR_SEARCH_KIND EXAMINE_TRIGGER(EXAMINE_JAR)
#endif

static void Tolbi_FreezePuddle(void);

/// Talking to Ember, a Mars Djinni, makes it join the party. Its index among
/// the Mars Djinn is 4, the fifth Mars name in the game text.
static void Tolbi_JoinEmber(void) {
  Djinn_JoinParty(ACTOR_TOLBI_EMBER, ELEMENT_MARS, DJINNI_INDEX(DJINNI_EMBER));
}

#include "shared/actor_distance.h"

/// Stops a running child when the target is close by and ahead of it, or
/// always when force is set; otherwise starts the child running again.
/// The child stops when the target is within one facing step of where it
/// looks; for the party leader, within two steps either side.
/// Returns 1 when the child stopped.
static s32 Tolbi_KidStopForTarget(struct Actor *kid, struct Actor *target,
                                  s32 range, s32 force) {
  s32 stopped = 0;
  const u8 *targetPos = (const u8 *)target + offsetof(struct Actor, x);
  const u8 *kidPos = (const u8 *)kid + offsetof(struct Actor, x);

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
      Actor_SelectAnimation(kid, ANIM_STAND);
      stopped = 1;
    }
    if (target == Actor_GetById(ACTOR_PARTY_LEADER) &&
        (farRight == facing || farLeft == facing)) {
      kid->paused = 1;
      Actor_SelectAnimation(kid, ANIM_STAND);
      stopped = 1;
    }
  } else {
    kid->paused = 0;
    Actor_SelectAnimation(kid, ANIM_WALK);
  }
  return stopped;
}

// The update of the two children running their loop. Each stops when
// it runs up behind the other, and otherwise when the party leader is in its
// way. While the party talks to someone or Mind Read shows thoughts (the range
// widens to 26 pixels), the child being talked to stops whatever the
// direction.
// UB: calling this s32 function through a void update routine is incompatible.
// Plain void removes its zero result; its aligned extent stays 152 bytes.
#ifdef UBFIX
static void Tolbi_UpdateKid(struct Actor *updated) {
#else
static s32 Tolbi_UpdateKid(struct Actor *updated) {
#endif
  struct Actor *self = updated;
  u8 **workAreas = gWindowWork;
  struct WindowWork *ui = (struct WindowWork *)workAreas[0];
  struct FieldWork *work =
      (struct FieldWork *)workAreas[HEAP_SLOT_FIELD - HEAP_SLOT_WINDOW];
  u16 *kidFlags = (u16 *)&self->timer;
  s32 force = 0;
  s32 range = 18;
  struct Actor *other;
  struct Actor *leader;

  if ((*kidFlags & KID_CHASES_YOUNGER) != 0) {
    other = Actor_GetById(ACTOR_TOLBI_YOUNGER_SISTER);
  } else {
    other = Actor_GetById(ACTOR_TOLBI_OLDER_BROTHER);
  }
  if (Tolbi_KidStopForTarget(self, other, 32, 0) != 0) {
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  }

  leader = Actor_GetById(ACTOR_PARTY_LEADER);

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

const struct SceneEntrance *Tolbi_GetEntrances(void) { return gTolbiEntrances; }

const struct SceneExitRegion *Tolbi_GetRegions(void) { return 0; }

const u32 *Tolbi_GetExits(void) { return gTolbiExits; }

const struct ActorPlacement *Tolbi_GetActorPlacements(void) {
  return gTolbiActorPlacements;
}

/// Growth on the sprout: the grown plant no longer blocks the party, and its
/// map cell becomes climbable.
static void Tolbi_GrowSprout(void) {
  struct Actor *sprout;

  sprout = Actor_GetById(ACTOR_TOLBI_SPROUT);
  if ((s32)sprout != 0) {
    sprout->collisionFlags = 0;
  }
  sprout = Actor_GetById(ACTOR_TOLBI_SPROUT);
  Actor_SetSpriteFlags(sprout, 0);
  Field_SetCellBehaviorAt(0, PIXELS(544), PIXELS(288), CELL_BEHAVIOR_CLIMB);
  GameFlag_Set(FLAG_TOLBI_SPROUT_GROWN);
}

/// Reveal uncovers the hidden chest: its map cell is drawn in and the chest
/// moves into place.
static void Tolbi_RevealHiddenChest(void) {
  MapGrid_CopyCellAttributes(3, 32, 1, 1, 3, 26);
  TreasureSpot_SetPosition(CELL_BEHAVIOR_TOLBI_HIDDEN_CHEST, PIXELS(56),
                           PIXELS(424));
}

/// When Reveal wears off, the chest's cell is covered again and the chest
/// moves out of sight.
static void Tolbi_HideRevealedChest(void) {
  MapGrid_CopyCellAttributes(2, 25, 1, 1, 3, 26);
  TreasureSpot_SetPosition(CELL_BEHAVIOR_TOLBI_HIDDEN_CHEST, -1, -1);
}

/// The first sign at Tolbi Spring: face away and toss a coin in.
static void Tolbi_ReadSpringSign(void) {
  Event_SetMessageId(MsgTolbiFaceAwayTolbi);
  Event_ShowMessageAndWait(-1, 0);
}

/// The second sign at Tolbi Spring: a Lucky Medal wins better prizes.
static void Tolbi_ReadLuckyMedalSign(void) {
  Event_SetMessageId(MsgTolbiTossLuckyMedal);
  Event_ShowMessageAndWait(-1, 0);
}

/// Talking to the older brother, or reading his mind. He stops running while
/// he speaks, then turns back the way he was going.
static void Tolbi_TalkToOlderBrother(void) {
  struct FieldWork *work = gFieldWork;
  struct Actor *kid;
  s16 facing;
  s32 msg;

  kid = Actor_GetById(ACTOR_TOLBI_OLDER_BROTHER);
  facing = kid->facing;
  Event_Start();
  kid->timer |= KID_TALKING;
  if (work->psynergy == 0) {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      msg = MsgTolbiYayEasyRun;
    } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      msg = MsgTolbiHoorayFinalsSeven;
    } else {
      msg = MsgTolbiWheeFestivalColosso;
    }
  } else {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      msg = MsgTolbiMainStreetTolbi;
    } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      msg = MsgTolbiWinBigUsed;
    } else {
      msg = MsgTolbiOldestShouldntShare;
    }
  }
  Event_SetMessageId(msg);
  Import_ActorSetAnimation(ACTOR_TOLBI_OLDER_BROTHER, 0);
  Actor_TurnToFaceEachOther(ACTOR_TOLBI_OLDER_BROTHER, ACTOR_PARTY_LEADER, 2);
  Event_ShowMessageThenWait(ACTOR_TOLBI_OLDER_BROTHER, 0, 10);
  kid->facing = facing;
  WaitFrames(1);
  kid->timer &= KID_CHASES_YOUNGER;
  Event_End();
}

/// Talking to the younger sister, or reading her mind, as above.
static void Tolbi_TalkToYoungerSister(void) {
  struct FieldWork *work = gFieldWork;
  struct Actor *kid;
  s16 facing;
  s32 msg;

  kid = Actor_GetById(ACTOR_TOLBI_YOUNGER_SISTER);
  facing = kid->facing;
  Event_Start();
  kid->timer |= KID_TALKING;
  if (work->psynergy == 0) {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      msg = MsgTolbiHaHaHa;
    } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      msg = MsgTolbiWaahBigBrother;
    } else {
      msg = MsgTolbiWaahSaidMoney;
    }
  } else {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      msg = MsgTolbiBrotherMeanDuring;
    } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      msg = MsgTolbiMamaWhyListen;
    } else {
      msg = MsgTolbiMamaToldShare;
    }
  }
  Event_SetMessageId(msg);
  Import_ActorSetAnimation(ACTOR_TOLBI_YOUNGER_SISTER, 0);
  Actor_TurnToFaceEachOther(ACTOR_TOLBI_YOUNGER_SISTER, ACTOR_PARTY_LEADER, 2);
  Event_ShowMessageThenWait(ACTOR_TOLBI_YOUNGER_SISTER, 0, 10);
  kid->facing = facing;
  WaitFrames(1);
  kid->timer &= KID_CHASES_YOUNGER;
  Event_End();
}

/// Picks the trigger table for Tolbi's current mood.
const struct Trigger *Tolbi_SelectTriggers(void) {
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    return gTolbiTriggersAfterColosso;
  }
  if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    return gTolbiTriggersFinalsDay;
  }
  return gTolbiTriggersFestival;
}

/// A suspicious old woman asks whether anyone can live past 100 years old.
/// She's ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, in Tolbi during the festival, and
/// asks whether someone can live to be more than 100 years old; either way the
/// talk turns to Babi, who is older still.
static void Tolbi_AskAboutLivingPastHundred(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiSomeoneLiveMore);
  Event_AskYesNoAndShowResponse(ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, 0);
  Event_End();
}

/// The father asks whether children really do love souvenirs, and owns up to
/// loving them himself.
static void Tolbi_TalkToFather(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiChildrenLoveSouvenirs);
  Event_AskYesNoAndShowResponse(ACTOR_TOLBI_FATHER, 0);
  Event_End();
}

/// The mother turns to her crying boy, then back to promise better sweets
/// later on. "Be patient, little guy."
static void Tolbi_TalkToMother(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiPatientLittleGuy);
  Actor_FaceDirection(ACTOR_TOLBI_MOTHER, FACING_NORTH, 0);
  Event_ShowMessageAndWait(ACTOR_TOLBI_MOTHER, 0);
  Actor_FaceDirection(ACTOR_TOLBI_MOTHER, FACING_WEST, 0);
  Event_ShowMessageAndWait(ACTOR_TOLBI_MOTHER, 0);
  Event_End();
}

/// The crying boy turns, stamps twice and wails for sweets.
static void Tolbi_TalkToCryingBoy(void) {
  Event_Start();
  Actor_FaceDirection(ACTOR_TOLBI_CRYING_BOY, FACING_SOUTH, 0);
  Actor_StartRepeatedMotion(ACTOR_TOLBI_CRYING_BOY, 2);
  Event_SetMessageId(MsgTolbiWaahBuySweets);
  Event_ShowMessageAndWait(ACTOR_TOLBI_CRYING_BOY, 0);
  Event_End();
}

/// During the festival the scalper sleeps under the archway: the inns are
/// full.
static void Tolbi_TalkToSleepingScalper(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiInnsFullStaying);
  Event_ShowMessageAndWait(ACTOR_TOLBI_SCALPER, 0);
  Event_End();
}

/// After Colosso, ACTOR_TOLBI_FINALS_FAN asks whether the party was in the
/// finals. The question is anchored to the father, who has left town by then.
static void Tolbi_AskWhetherPartyWasInFinals(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiFinalsWerent);
  Event_AskYesNoAndShowResponse(ACTOR_TOLBI_FATHER, 0);
  Event_End();
}

/// After Colosso, the snack vendor gives the party his leftover corn, once.
static void Tolbi_TalkToSnackVendor(void) {
  Event_Start();
  if (Event_GetFlag(FLAG_TOLBI_CORN_GIVEN) == 0) {
    GameFlag_Set(FLAG_TOLBI_CORN_GIVEN);
    Event_SetMessageId(MsgTolbiLeftovers);
    Event_ShowMessageAndWait(ACTOR_TOLBI_SNACK_VENDOR, 0);
    ItemDisplay_ShowItem(ITEM_CORN, 3);
    Event_ShowMessageAndWait(ACTOR_TOLBI_SNACK_VENDOR, 0);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    PartyInventory_GiveItem(ITEM_CORN, 0);
  } else {
    Event_SetMessageId(MsgTolbiFestivalLongerUsual);
    Event_ShowMessageAndWait(ACTOR_TOLBI_SNACK_VENDOR, 0);
  }
  Event_End();
}

/// Walking into one of the town's doors: the door opens, the leader steps in
/// (except at TOLBI_TO_WEAPON_AND_ARMOR_SHOP) and the scene changes through the
/// door's exit, numbered as its cell behavior.
static void Tolbi_EnterDoor(void) {
  struct FieldWork *work = gFieldWork;
  u32 actorId;
  s32 door;

  Event_Start();
  for (actorId = ACTOR_FIRST_PLACED; actorId <= 65; actorId++) {
    struct Actor *actor = Actor_GetById(actorId);
    if (actor != NULL) {
      actor->motionFlags = 0;
    }
  }
  door = work->stepTrigger - 1;
  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gTolbiDoors[door].steps, gTolbiDoors[door].x,
                       gTolbiDoors[door].y);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (door != TOLBI_TO_WEAPON_AND_ARMOR_SHOP - 1) {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
    Event_WaitFrames(10);
  }
  Import_EventSetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// The update of ACTOR_TOLBI_OLD_MAN, who during the festival says he is 65
/// and that Babi was already old when he was born: gives him the leader's
/// sprite priority every frame, for both of his sprite parts.
static void Tolbi_CopyLeaderPriority(struct Actor *updated) {
  s32 priority;

  if (updated != NULL) {
    priority = Actor_GetById(ACTOR_PARTY_LEADER)->sprite->image.priority;
    updated->priorityFlags = 0;
    updated->sprite->image.priority = priority;
    updated->sprite->shadow.priority = priority;
  }
}

/// Sets up the scene on entry: the running children, ACTOR_TOLBI_OLD_MAN's
/// update, and everything the flags have already changed.
s32 Tolbi_ApplyEntryState(s32 unused) {
  struct Actor *actor;
  void (*kidUpdate)(struct Actor *);
  s32 clearFlags;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  // The two children run their loop south-east of the fountain.
  Actor_SetPosition(ACTOR_TOLBI_OLDER_BROTHER, PIXELS(352), PIXELS(352));
  Actor_SetScript(ACTOR_TOLBI_OLDER_BROTHER, gTolbiOlderBrotherRun);
  actor = Actor_GetById(ACTOR_TOLBI_OLDER_BROTHER);
#ifdef UBFIX
  kidUpdate = Tolbi_UpdateKid;
#else
  kidUpdate = (void (*)(struct Actor *))Tolbi_UpdateKid;
#endif
  actor->timer = KID_CHASES_YOUNGER;
  actor->update = kidUpdate;
  clearFlags = 0;
  Import_ActorSetPosition(ACTOR_TOLBI_YOUNGER_SISTER, PIXELS(368), PIXELS(320));
  Actor_SetScript(ACTOR_TOLBI_YOUNGER_SISTER, gTolbiYoungerSisterRun);
  actor = Actor_GetById(ACTOR_TOLBI_YOUNGER_SISTER);
  actor->timer = clearFlags;
  actor->update = kidUpdate;
  actor = Actor_GetById(ACTOR_TOLBI_OLD_MAN);
  actor->update = Tolbi_CopyLeaderPriority;

  // What the party has already done here.
  if (GameFlag_Get(FLAG_TOLBI_SPRING_OPENED) != 0) {
    Actor_SetPosition(ACTOR_TOLBI_SPRING_VISITOR, PIXELS(316), PIXELS(328));
  }
  // The puddle is frozen.
  if (GameFlag_Get(FLAG_TOLBI_PUDDLE_FROZEN) != 0) {
    Tolbi_FreezePuddle();
  }
  // The sprout has grown.
  if (GameFlag_Get(FLAG_TOLBI_SPROUT_GROWN) != 0) {
    Tolbi_GrowSprout();
    Import_ActorSetAnimation(ACTOR_TOLBI_SPROUT, 4);
  }

  // After Colosso the crowd has gone home and is moved off the map. On finals
  // day the scalper waits by the gate.
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    Actor_SetPosition(ACTOR_TOLBI_WOULD_BE_FIGHTER, PIXELS(520), PIXELS(560));
    Actor_SetPosition(ACTOR_TOLBI_XIAN_VISITOR, PIXELS(520), PIXELS(560));
    Actor_SetPosition(ACTOR_TOLBI_OVERSLEEPER, PIXELS(520), PIXELS(560));
    Actor_SetPosition(ACTOR_TOLBI_FATHER, PIXELS(520), PIXELS(560));
    Actor_SetPosition(ACTOR_TOLBI_MOTHER, PIXELS(520), PIXELS(560));
    Actor_SetPosition(ACTOR_TOLBI_CRYING_BOY, PIXELS(520), PIXELS(560));
    Actor_SetPosition(ACTOR_TOLBI_SCALPER, PIXELS(520), PIXELS(560));
  } else {
    if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      Actor_SetPosition(ACTOR_TOLBI_SCALPER, PIXELS(280), PIXELS(80));
      Actor_FaceDirection(ACTOR_TOLBI_SCALPER, FACING_SOUTHEAST, 0);
      Import_ActorSetAnimation(ACTOR_TOLBI_SCALPER, ANIM_STAND);
    }
  }
  return 0;
}

/// The visitor at the spring's gate asks whether the party wants to test its
/// luck. On yes he steps aside for good and the way to Tolbi Spring opens.
static void Tolbi_TalkToSpringVisitor(void) {
  Event_Start();
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 304, 312);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_ArmCallback(ACTOR_TOLBI_SPRING_VISITOR, FACING_SOUTH, 0);
  Event_WaitFrames(20);
  Event_SetMessageId(MsgTolbiWantTestLuck);
  Event_OpenMessage(ACTOR_TOLBI_SPRING_VISITOR, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    // Yes: "I'll give you the chance to go first."
    gFieldWork->message++;
    Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_VISITOR, 0);
    Actor_SetSpeed(ACTOR_TOLBI_SPRING_VISITOR, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_TOLBI_SPRING_VISITOR, 320, 304);
    Actor_WalkToAndWait(ACTOR_TOLBI_SPRING_VISITOR, 316, 328);
    Actor_ArmCallback(ACTOR_TOLBI_SPRING_VISITOR, FACING_NORTHWEST, 0);
    GameFlag_Set(FLAG_TOLBI_SPRING_OPENED);
  } else {
    Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_VISITOR, 0);
  }
  Event_End();
}

/// The gate to Tolbi Spring (CELL_BEHAVIOR_TOLBI_GATE_TO_SPRING), once the
/// visitor at the gate has stepped aside.
static void Tolbi_EnterSpring(void) {
  Import_EventSetExitRequest(TOLBI_TO_TOLBI_SPRING);
  Event_End();
}

/// Leaves by the exit numbered as the cell behavior the party walked into.
static void Tolbi_RequestTouchedExit(void) {
  s16 *trigger = &gFieldWork->stepTrigger;

  Import_EventSetExitRequest(*trigger);
}

/// Two soldiers searching for the missing Babi stop the party and show his
/// picture. They do so during the festival, the first time the party steps on
/// CELL_BEHAVIOR_TOLBI_SOLDIERS. "Have you seen anyone who looks like this?"
/// They leave either way the party answers. Sets
/// FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI.
static void Tolbi_MeetSearchingSoldiers(void) {
  s32 firstMsg;

  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetSpeed(ACTOR_TOLBI_SOLDIER_A, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeedParameters(ACTOR_TOLBI_SOLDIER_B, PIXELS(1), PIXELS(1) / 2);
  firstMsg = MsgTolbiSeenAnyoneWho;
  Event_SetMessageId(firstMsg);

  // The soldiers come up to the party; the poster waits off to the side.
  Actor_SetPosition(ACTOR_TOLBI_SOLDIER_A, PIXELS(72), PIXELS(208));
  Actor_SetPosition(ACTOR_TOLBI_SOLDIER_B, PIXELS(56), PIXELS(208));
  Actor_ConfigureSpriteColor(ACTOR_TOLBI_MISSING_PERSON_POSTER,
                             SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TOLBI_MISSING_PERSON_POSTER), 0);
  Actor_SetPosition(ACTOR_TOLBI_MISSING_PERSON_POSTER, PIXELS(95), PIXELS(40));
  Actor_ResetAndSetPositionInMode2(ACTOR_TOLBI_SOLDIER_A, 72, 248);
  Actor_ResetAndSetPositionInMode2(ACTOR_TOLBI_SOLDIER_B, 56, 248);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 64, 264);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_TOLBI_SOLDIER_A);
  Import_ActorSetAnimation(ACTOR_TOLBI_SOLDIER_A, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_TOLBI_SOLDIER_B, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_SetAngleToward(ACTOR_TOLBI_SOLDIER_A, ACTOR_PARTY_LEADER, 0);
  Actor_SetAngleToward(ACTOR_TOLBI_SOLDIER_B, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(20);
  Actor_SetAttachedEffect(ACTOR_TOLBI_SOLDIER_A,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_TOLBI_SOLDIER_B,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_TOLBI_SOLDIER_A, 2);
  Actor_RunRepeatedMotion(ACTOR_TOLBI_SOLDIER_B, 2);
  Event_WaitFrames(20);

  // "Have you seen anyone who looks like this?", under a "Missing Person"
  // window showing Babi's portrait. The poster, placed with Babi's sprite,
  // becomes the speaker so that answering closes that portrait window.
  Event_OpenMessage(ACTOR_TOLBI_SOLDIER_A, 0);
  Event_WaitFrames(25);
  Portrait_OpenWindow(SPRITE_BABI, 0, 12, 7);
  Message_OpenWindow((firstMsg + 3), 11, 12, 2);
  gFieldWork->speaker = ACTOR_TOLBI_MISSING_PERSON_POSTER;
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    // Yes: "Babi must be nearby!!!" They nod and run east.
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_TOLBI_SOLDIER_B, 2);
    Event_WaitFrames(30);
    Actor_ArmCallback(ACTOR_TOLBI_SOLDIER_B, FACING_EAST, 0);
    Event_WaitFrames(30);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_TOLBI_SOLDIER_A, ANIM_NOD);
    Event_WaitFrames(20);
    Actor_ArmCallback(ACTOR_TOLBI_SOLDIER_A, FACING_EAST, 0);
    Event_WaitFrames(30);
    Event_ShowMessageAndWait(ACTOR_TOLBI_SOLDIER_A, 0);
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_TOLBI_SOLDIER_A, FACING_SOUTH, 0);
    Actor_ArmCallback(ACTOR_TOLBI_SOLDIER_B, FACING_SOUTH, 0);
    Event_WaitFrames(30);
    Import_ActorSetAnimation(ACTOR_TOLBI_SOLDIER_A, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_TOLBI_SOLDIER_B, ANIM_NOD);
    Event_WaitFrames(20);
    Actor_SetSpeed(ACTOR_TOLBI_SOLDIER_A, PIXELS(9) / 5, PIXELS(9) / 10);
    Actor_SetSpeed(ACTOR_TOLBI_SOLDIER_B, PIXELS(9) / 5, PIXELS(9) / 10);
    Actor_WalkTo(ACTOR_TOLBI_SOLDIER_A, 232, 248);
    Event_WaitFrames(2);
    Actor_WalkTo(ACTOR_TOLBI_SOLDIER_B, 232, 248);
    Actor_WaitForMoveById(ACTOR_TOLBI_SOLDIER_A);
    Actor_ResetAndSetPositionInMode2(ACTOR_TOLBI_SOLDIER_A, 248, 248);
    Actor_SetPositionAndReset(ACTOR_TOLBI_SOLDIER_B, 248, 248);
  } else {
    // No: "He doesn't seem to be here. Let's look over there." They shake
    // their heads and go back north.
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_TOLBI_SOLDIER_B, 2);
    Event_WaitFrames(30);
    Actor_ArmCallback(ACTOR_TOLBI_SOLDIER_B, FACING_EAST, 0);
    Event_WaitFrames(30);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_TOLBI_SOLDIER_A, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    Actor_ArmCallback(ACTOR_TOLBI_SOLDIER_A, FACING_EAST, 0);
    Event_WaitFrames(30);
    gFieldWork->message += 1;
    Event_ShowMessageAndWait(ACTOR_TOLBI_SOLDIER_A, 0);
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_TOLBI_SOLDIER_A, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_TOLBI_SOLDIER_B, FACING_SOUTH, 0);
    Event_WaitFrames(30);
    Import_ActorSetAnimation(ACTOR_TOLBI_SOLDIER_A, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_TOLBI_SOLDIER_B, ANIM_NOD);
    Event_WaitFrames(20);
    Actor_SetSpeed(ACTOR_TOLBI_SOLDIER_A, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_SetSpeed(ACTOR_TOLBI_SOLDIER_B, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_ResetAndSetPositionInMode2(ACTOR_TOLBI_SOLDIER_A, 72, 184);
    Actor_SetPositionAndReset(ACTOR_TOLBI_SOLDIER_B, 56, 184);
  }
  Import_ActorSetPosition(ACTOR_TOLBI_SOLDIER_A, 0, 0);
  Import_ActorSetPosition(ACTOR_TOLBI_SOLDIER_B, 0, 0);
  Import_ActorSetPosition(ACTOR_TOLBI_MISSING_PERSON_POSTER, 0, 0);
  GameFlag_Set(FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI);
  Event_End();
}

/// Frost on the puddle: the frozen pillar is drawn underfoot and stops
/// following the terrain, and its map cell becomes solid.
static void Tolbi_FreezePuddle(void) {
  struct Actor *puddle = Actor_GetById(ACTOR_TOLBI_PUDDLE);
  if (puddle != 0) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    puddle->motionFlags = 0;
  }
  Import_ActorSetAnimation(ACTOR_TOLBI_PUDDLE, 5);
  {
    MapGrid_CopyCellAttributes(36, 16, 1, 1, 34, 16);
  }
  GameFlag_Set(FLAG_TOLBI_PUDDLE_FROZEN);
}

/// The actor script of a mirrored actor: flips its sprite and passes 7
/// on.
s32 gTolbiMirroredActor[] = {
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

struct SceneEntrance gTolbiEntrances[] = {
    {0, CONDITION_ALWAYS, 296, 0, 232, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_FROM_HOUSE_SOUTH, CONDITION_ALWAYS, 304, 0, 448, FACING_NORTH, 0, -1,
     -1, -1, -1, 0},
    {TOLBI_FROM_HOUSE_NORTHWEST, CONDITION_ALWAYS, 168, 0, 200, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {TOLBI_FROM_INN, CONDITION_ALWAYS, 408, 0, 160, FACING_SOUTH, 0, -1, -1, -1,
     -1, 0},
    {TOLBI_FROM_STUCCO_ROOMS_B_EAST, CONDITION_ALWAYS, 536, 0, 232,
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
    {TOLBI_FROM_STUCCO_ROOMS_B_WEST, CONDITION_ALWAYS, 88, 0, 392, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
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
    SCENE_EXIT(TOLBI_TO_HOUSE_SOUTH, StuccoRoomsB, STUCCO_ROOMS_B_HOUSE_SOUTH),
    SCENE_EXIT(TOLBI_TO_HOUSE_NORTHWEST, StuccoRoomsB,
               STUCCO_ROOMS_B_HOUSE_NORTHWEST),
    SCENE_EXIT(TOLBI_TO_INN, StuccoRoomsB, STUCCO_ROOMS_B_INN),
    SCENE_EXIT(TOLBI_TO_LUCKY_WHEELS, StuccoRoomsC,
               STUCCO_ROOMS_C_LUCKY_WHEELS_FROM_TOLBI),
    SCENE_EXIT(TOLBI_TO_HOUSE_NORTHEAST, StuccoRoomsB,
               STUCCO_ROOMS_B_HOUSE_NORTHEAST),
    SCENE_EXIT(TOLBI_TO_HOUSE_EAST, StuccoRoomsB, STUCCO_ROOMS_B_HOUSE_EAST),
    SCENE_EXIT(TOLBI_TO_WEAPON_AND_ARMOR_SHOP, StuccoRoomsB,
               STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(TOLBI_TO_HOUSE_SOUTHWEST, StuccoRoomsB,
               STUCCO_ROOMS_B_HOUSE_SOUTHWEST),
    SCENE_EXIT(TOLBI_TO_SANCTUM, StuccoRoomsB, STUCCO_ROOMS_B_SANCTUM),
    SCENE_EXIT(TOLBI_TO_BABI_PALACE_ENTRANCE, BabiPalaceEntrance,
               BABI_PALACE_ENTRANCE_FROM_TOLBI),
    SCENE_EXIT(TOLBI_TO_COLOSSEUM_ENTRANCE, ColosseumEntrance,
               COLOSSEUM_ENTRANCE_FROM_TOLBI),
    SCENE_EXIT(TOLBI_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_TOLBI),
    SCENE_EXIT(TOLBI_TO_LUCKY_DICE, StuccoRoomsC,
               STUCCO_ROOMS_C_LUCKY_DICE_FROM_TOLBI),
    SCENE_EXIT(TOLBI_TO_TOLBI_SPRING, TolbiSpring, TOLBI_SPRING_FROM_TOLBI),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gTolbiActorPlacements[] = {
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
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, (s32)gTolbiMirroredActor,
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
    { SCENE_TABLE_END },
};

/// During the festival, before the finals.
struct Trigger gTolbiTriggersFestival[] = {
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_TOLBI_SPROUT, FLAG_TOLBI_SPROUT_GROWN, EVENT(Tolbi_GrowSprout) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTH, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTHWEST, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_INN, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_LUCKY_WHEELS, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTHEAST, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTHWEST, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_SANCTUM, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_BABI_PALACE_ENTRANCE, CONDITION_ALWAYS, EVENT(Tolbi_RequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_COLOSSEUM_ENTRANCE, CONDITION_ALWAYS, EVENT(Tolbi_RequestTouchedExit) },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, TOLBI_TO_WORLD_MAP },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TOLBI_TO_WORLD_MAP, CONDITION_ALWAYS, EVENT(Tolbi_RequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_LUCKY_DICE, CONDITION_ALWAYS, EVENT(Tolbi_RequestTouchedExit) },
    { TRIGGER_STEP, CELL_BEHAVIOR_TOLBI_SOLDIERS, FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI, EVENT(Tolbi_MeetSearchingSoldiers) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_TOLBI_GATE_TO_SPRING, IF_FLAG_SET(FLAG_TOLBI_SPRING_OPENED), EVENT(Tolbi_EnterSpring) },
    { TRIGGER_TALK, ACTOR_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTolbisAlwaysReallyBustlingDuringColosso) },
    { TRIGGER_TALK, ACTOR_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiColossoIsHeldInTheColosseum) },
    { TRIGGER_TALK, ACTOR_TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTryoutsStartedAtTheColosseumToday) },
    { TRIGGER_TALK, ACTOR_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiColossoShouldHaveStartedTodayWhat) },
    { TRIGGER_TALK, ACTOR_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiBabiWasAlreadyPrettyOldBack) },
    { TRIGGER_TALK, ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, EVENT(Tolbi_AskAboutLivingPastHundred) },
    { TRIGGER_TALK, ACTOR_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToOlderBrother) },
    { TRIGGER_TALK, ACTOR_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToYoungerSister) },
    { TRIGGER_TALK, ACTOR_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiGetYourColossoSouvenirsTheyMake) },
    { TRIGGER_TALK, ACTOR_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiGetYourSnacksAndDrinksBefore) },
    { TRIGGER_TALK, ACTOR_TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiIveDecidedNotToFightAt) },
    { TRIGGER_TALK, ACTOR_TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiWowTolbisFestivalIsHumongousJust) },
    { TRIGGER_TALK, ACTOR_TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiRatsWeOversleptAndNowThe) },
    { TRIGGER_TALK, ACTOR_TOLBI_FATHER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToFather) },
    { TRIGGER_TALK, ACTOR_TOLBI_MOTHER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToMother) },
    { TRIGGER_TALK, ACTOR_TOLBI_CRYING_BOY, CONDITION_ALWAYS, EVENT(Tolbi_TalkToCryingBoy) },
    { TRIGGER_TALK, ACTOR_TOLBI_SCALPER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToSleepingScalper) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_VISITOR, FLAG_TOLBI_SPRING_OPENED, EVENT(Tolbi_TalkToSpringVisitor) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiAhIGuessIJustDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTouristsArentTheOnlyOnesWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheTrialsAreStartingAtThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheTrialsAreOkayButThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiItsNotLikeThereHaveBeen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiEveryYearIGetOlderAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiWhatIfBabiIsActuallyA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToOlderBrother) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToYoungerSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiWeHaveToSellAllOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTheyreSellingTheseAtTheColosseum) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiISawThePeopleWhoAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTolbiIsNotLikeXianIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiICantSayAnythingCauseIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_FATHER, CONDITION_ALWAYS, MSGNO(MsgTolbiTheStuffTheyreSellingAtThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_MOTHER, CONDITION_ALWAYS, MSGNO(MsgTolbiWhyDoKidsWantSuchAwful) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiThoseSweetsSmellGreatImNot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SCALPER, CONDITION_ALWAYS, MSGNO(MsgTolbiIveBeenGuardingOurSpotBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiIKeepLosingCoinsButI) },
    { TRIGGER_TALK, ACTOR_TOLBI_EMBER, CONDITION_ALWAYS, EVENT(Tolbi_JoinEmber) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TOLBI_SPRING_SIGN, CONDITION_ALWAYS, EVENT(Tolbi_ReadSpringSign) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TOLBI_LUCKY_MEDAL_SIGN, CONDITION_ALWAYS, EVENT(Tolbi_ReadLuckyMedalSign) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_TOLBI_BARREL, FLAG_TOLBI_BARREL_COINS, FIND_COINS(9) },
// The international editions find the jar only when the party faces north.
    { TOLBI_JAR_SEARCH_KIND, CELL_BEHAVIOR_TOLBI_JAR, FLAG_TOLBI_JAR_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_TOLBI_HIDDEN_CHEST, FLAG_TOLBI_CHEST_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_TOLBI_PUDDLE, FLAG_TOLBI_PUDDLE_FROZEN, EVENT(Tolbi_FreezePuddle) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Tolbi_RevealHiddenChest) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Tolbi_HideRevealedChest) },
    { TRIGGERS_END },
};

/// On finals day.
struct Trigger gTolbiTriggersFinalsDay[] = {
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_TOLBI_SPROUT, FLAG_TOLBI_SPROUT_GROWN, EVENT(Tolbi_GrowSprout) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTH, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTHWEST, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_INN, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_LUCKY_WHEELS, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTHEAST, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTHWEST, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_SANCTUM, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_BABI_PALACE_ENTRANCE, CONDITION_ALWAYS, EVENT(Tolbi_RequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_COLOSSEUM_ENTRANCE, CONDITION_ALWAYS, EVENT(Tolbi_RequestTouchedExit) },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, TOLBI_TO_WORLD_MAP },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TOLBI_TO_WORLD_MAP, CONDITION_ALWAYS, EVENT(Tolbi_RequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_LUCKY_DICE, CONDITION_ALWAYS, EVENT(Tolbi_RequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_TOLBI_GATE_TO_SPRING, IF_FLAG_SET(FLAG_TOLBI_SPRING_OPENED), EVENT(Tolbi_EnterSpring) },
    { TRIGGER_TALK, ACTOR_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiColossoWillReachItsApexAny) },
    { TRIGGER_TALK, ACTOR_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiAreYouPlanningToWatchThe) },
    { TRIGGER_TALK, ACTOR_TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiMyFamilySavedMeASeat) },
    { TRIGGER_TALK, ACTOR_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiIThoughtSomethingHappenedToBabi) },
    { TRIGGER_TALK, ACTOR_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheColosseumWasSoFullThat) },
    { TRIGGER_TALK, ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiNobodyElseHasNoticedButThis) },
    { TRIGGER_TALK, ACTOR_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToOlderBrother) },
    { TRIGGER_TALK, ACTOR_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToYoungerSister) },
    { TRIGGER_TALK, ACTOR_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiHowAboutSomeColossoSouvenirsThis) },
    { TRIGGER_TALK, ACTOR_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTheFinalsAreSoLongIts) },
    { TRIGGER_TALK, ACTOR_TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiTheTrialsWereSoRivetingThe) },
    { TRIGGER_TALK, ACTOR_TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiIHaveSeatsToTheFinals) },
    { TRIGGER_TALK, ACTOR_TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiIOversleptAgainSoICant) },
    // The family repeats its festival lines on finals day. XXX (no evidence):
    // whether the four messages after MsgTolbiSpringOhThereAreAlwaysScalpersAt
    // (`The finals are about to start.` and the rest) were meant for it; no row
    // or code shows them.
    { TRIGGER_TALK, ACTOR_TOLBI_FATHER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToFather) },
    { TRIGGER_TALK, ACTOR_TOLBI_MOTHER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToMother) },
    { TRIGGER_TALK, ACTOR_TOLBI_CRYING_BOY, CONDITION_ALWAYS, EVENT(Tolbi_TalkToCryingBoy) },
    { TRIGGER_TALK, ACTOR_TOLBI_SCALPER, CONDITION_ALWAYS, MSGNO(MsgTolbiTheFinalsStartTodayIWonder) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_VISITOR, FLAG_TOLBI_SPRING_OPENED, EVENT(Tolbi_TalkToSpringVisitor) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiAhIGuessIJustDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiIveGotToGetToThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiABattleToTheDeathWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheFinalsStartOnBabisSignal) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiSoWhyWereTheSoldiersAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiICouldStillBuySeatsFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiNoMatterHowIAddIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToOlderBrother) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToYoungerSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiWeCantSellAnythingOnceThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiThisIsTheFinalPushAnything) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiFightingInColossoRequiresALot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTheyreSellingFinalsSeatsRightAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiItsSupposedToBeFreeHaving) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_FATHER, CONDITION_ALWAYS, MSGNO(MsgTolbiOnceColossoEndsOurLivesCan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_MOTHER, CONDITION_ALWAYS, MSGNO(MsgTolbiFestivalsAreSoExpensiveMyWallets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiMomYouSaidYouWereAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SCALPER, CONDITION_ALWAYS, MSGNO(MsgTolbiTodayIsABigDayFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiIKeepLosingCoinsButI) },
    { TRIGGER_TALK, ACTOR_TOLBI_EMBER, CONDITION_ALWAYS, EVENT(Tolbi_JoinEmber) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TOLBI_SPRING_SIGN, CONDITION_ALWAYS, EVENT(Tolbi_ReadSpringSign) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TOLBI_LUCKY_MEDAL_SIGN, CONDITION_ALWAYS, EVENT(Tolbi_ReadLuckyMedalSign) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_TOLBI_BARREL, FLAG_TOLBI_BARREL_COINS, FIND_COINS(9) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_TOLBI_JAR, FLAG_TOLBI_JAR_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_TOLBI_HIDDEN_CHEST, FLAG_TOLBI_CHEST_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_TOLBI_PUDDLE, FLAG_TOLBI_PUDDLE_FROZEN, EVENT(Tolbi_FreezePuddle) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Tolbi_RevealHiddenChest) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Tolbi_HideRevealedChest) },
    { TRIGGERS_END },
};

/// After Colosso has ended.
struct Trigger gTolbiTriggersAfterColosso[] = {
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_TOLBI_SPROUT, FLAG_TOLBI_SPROUT_GROWN, EVENT(Tolbi_GrowSprout) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTH, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTHWEST, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_INN, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_LUCKY_WHEELS, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTHEAST, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTHWEST, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_SANCTUM, CONDITION_ALWAYS, EVENT(Tolbi_EnterDoor) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_BABI_PALACE_ENTRANCE, CONDITION_ALWAYS, EVENT(Tolbi_RequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_COLOSSEUM_ENTRANCE, CONDITION_ALWAYS, EVENT(Tolbi_RequestTouchedExit) },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, TOLBI_TO_WORLD_MAP },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TOLBI_TO_WORLD_MAP, CONDITION_ALWAYS, EVENT(Tolbi_RequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_LUCKY_DICE, CONDITION_ALWAYS, EVENT(Tolbi_RequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_TOLBI_GATE_TO_SPRING, IF_FLAG_SET(FLAG_TOLBI_SPRING_OPENED), EVENT(Tolbi_EnterSpring) },
    { TRIGGER_TALK, ACTOR_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiWellThisYearsColossoHasEnded) },
    { TRIGGER_TALK, ACTOR_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiItsBackToOurNormalQuiet) },
    { TRIGGER_TALK, ACTOR_TOLBI_FINALS_FAN, CONDITION_ALWAYS, EVENT(Tolbi_AskWhetherPartyWasInFinals) },
    { TRIGGER_TALK, ACTOR_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiWithAllThoseRevelersGoneThe) },
    { TRIGGER_TALK, ACTOR_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiIHadNoLuckInThe) },
    { TRIGGER_TALK, ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiItsSoQuietTheHustleAnd) },
    { TRIGGER_TALK, ACTOR_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToOlderBrother) },
    { TRIGGER_TALK, ACTOR_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToYoungerSister) },
    { TRIGGER_TALK, ACTOR_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiIllBeBackToDoBusiness) },
    { TRIGGER_TALK, ACTOR_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, EVENT(Tolbi_TalkToSnackVendor) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_VISITOR, FLAG_TOLBI_SPRING_OPENED, EVENT(Tolbi_TalkToSpringVisitor) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiAhIGuessIJustDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheSpectatorsHaveAllLeftSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiThinkHowFunItWouldBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiAllTheWarriorsWhoGainFame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiComeToThinkOfItNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiHmmSinceColossoEndedSoDramatically) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheCalmAfterTheStormIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToOlderBrother) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT(Tolbi_TalkToYoungerSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiEveryYearITryToSell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiHowCanWeGetHomeIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiIKeepLosingCoinsButI) },
    { TRIGGER_TALK, ACTOR_TOLBI_EMBER, CONDITION_ALWAYS, EVENT(Tolbi_JoinEmber) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TOLBI_SPRING_SIGN, CONDITION_ALWAYS, EVENT(Tolbi_ReadSpringSign) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TOLBI_LUCKY_MEDAL_SIGN, CONDITION_ALWAYS, EVENT(Tolbi_ReadLuckyMedalSign) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_TOLBI_BARREL, FLAG_TOLBI_BARREL_COINS, FIND_COINS(9) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_TOLBI_JAR, FLAG_TOLBI_JAR_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_TOLBI_HIDDEN_CHEST, FLAG_TOLBI_CHEST_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_TOLBI_PUDDLE, FLAG_TOLBI_PUDDLE_FROZEN, EVENT(Tolbi_FreezePuddle) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Tolbi_RevealHiddenChest) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Tolbi_HideRevealedChest) },
    { TRIGGERS_END },
};
// clang-format on

/// How a door opens: two frames of cells copied over the closed door.
u16 gTolbiDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(39, 32, 1, 2, 6),
    MAPGRID_ANIMATION_STEP(40, 32, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The doors, in the order of their cell behaviors 1 to 9, by map cell.
struct MapGridCellAnimation gTolbiDoors[] = {
    {gTolbiDoorOpening, 48, 6},  {gTolbiDoorOpening, 63, 4},
    {gTolbiDoorOpening, 71, 8},  {gTolbiDoorOpening, 71, 4},
    {gTolbiDoorOpening, 66, 21}, {gTolbiDoorOpening, 61, 24},
    {gTolbiDoorOpening, 50, 26}, {gTolbiDoorOpening, 50, 22},
    {gTolbiDoorOpening, 43, 20},
};
