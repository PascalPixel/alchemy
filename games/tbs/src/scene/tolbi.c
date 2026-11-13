/// @file
/// Tolbi (SceneId_Tolbi), the town Babi rules, where Colosso, a festival of
/// warriors, is held each year in the colosseum.
///
/// The party comes to Tolbi across the Karagol Sea from Kalay while the
/// festival is on, and the town's talk follows the Colosso story in three
/// stages.
///
/// Tolbi_SelectTriggers gives each stage its own trigger table. One actor
/// placement table serves all three, and Tolbi_ApplyEntryState moves the
/// actors whose places change.
///
/// - The festival (neither story flag set). The trials have begun at the
///   colosseum, but Babi is not there, and his soldiers are searching the
///   town. The first time the party steps on CELL_BEHAVIOR_TOLBI_SOLDIERS, two
///   of them stop it to show his picture, `Have you seen anyone who looks like
///   this?`, and go their way whatever it answers
///   (Tolbi_MeetSearchingSoldiers, FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI).
/// - Finals day (FLAG_BABI_SAVED), once the party has saved Babi in
///   Altmiller Cave. He has shown himself at the finals, and the town hurries
///   to the colosseum. The scalper who slept under the archway has moved,
///   sending people to his buddy for seats.
/// - After Colosso (FLAG_COLOSSO_OVER). The visitors have left, so the family
///   seen at the festival and four others are moved off the map. Those who
///   stay talk of the quiet and of the ships that no longer sail the
///   Karagol. The snack vendor gives away his leftover corn, once
///   (FLAG_TOLBI_CORN_GIVEN).
///
/// In every stage two children run a small loop south-east of the fountain
/// (gTolbiOlderBrotherRun, gTolbiYoungerSisterRun) and quarrel over their
/// allowance. Each stops when it runs up behind the other or finds the leader
/// in its way (Tolbi_UpdateKid). The recording of talking to the brother
/// shows them running it
/// (https://alchemy.pascalpixel.com/places/tolbi/events/Tolbi_TalkToOlderBrother.gif).
///
/// A visitor testing his luck stands at the gate of Tolbi Spring. Once the
/// party agrees to go first he steps aside for good (Tolbi_TalkToSpringVisitor,
/// FLAG_TOLBI_SPRING_OPENED), and the gate (CELL_BEHAVIOR_TOLBI_GATE_TO_SPRING)
/// leads into Tolbi Spring (TOLBI_TO_TOLBI_SPRING).
///
/// Ember, a Mars Djinni, joins the party when spoken to.
///
/// Growth raises a sprout and Frost freezes a puddle, each a scene flag kept
/// only through a return that keeps the party's position. Reveal uncovers a
/// chest with Power Bread.
///
/// A barrel and a jar hold 9 coins and a Lucky Medal. During the festival,
/// the editions from TBS_PATCH_1 on find the jar only when the party faces
/// north.
///
/// The doors from TOLBI_TO_HOUSE_SOUTH to TOLBI_TO_SANCTUM, whose cells carry
/// their exits' numbers, lead into Tolbi's houses, inns and shops and into
/// Tolbi Sanctum (all SceneId_StuccoRoomsB). The exception is
/// TOLBI_TO_LUCKY_WHEELS, which with TOLBI_TO_LUCKY_DICE leads into the Lucky
/// Dice and Lucky Wheels rooms (SceneId_StuccoRoomsC, whose area name is also
/// Tolbi).
///
/// TOLBI_TO_BABI_PALACE_ENTRANCE leads to Babi's Palace,
/// TOLBI_TO_COLOSSEUM_ENTRANCE to the Colosseum Entrance and TOLBI_TO_WORLD_MAP
/// out onto the World Map.

#include "module/scene/tolbi.h"
#include "djinn_ids.h"
#include "module/window/screens.h"
#include "tolbi.h"
#include "iwram_call.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "sounds.h"
#include "map_grid.h"

#if TBS_PATCH_1
#define TOLBI_JAR_SEARCH_KIND                                                  \
  (EXAMINE_TRIGGER(EXAMINE_JAR) | TRIGGER_FLAG_FACING | FACING_NORTH)
#else
#define TOLBI_JAR_SEARCH_KIND EXAMINE_TRIGGER(EXAMINE_JAR)
#endif

static void Tolbi_FreezePuddle(void);

/// Talking to Ember, a Mars Djinni, makes it join the party.
static void Tolbi_JoinEmber(void) {
  Djinn_JoinParty(ACTOR_TOLBI_EMBER, ELEMENT_MARS, DJINNI_INDEX(DJINNI_EMBER));
}

#include "shared/actor_distance.h"

/// Stops a running child when the target is close by and ahead of it, or
/// always when force is set.
///
/// The child stops when the target is within one facing step of where it
/// looks. For the party leader, that is within two steps either side.
///
/// Returns 1 when the child stopped.
static s32 Tolbi_KidStopForTarget(struct Actor *kid, struct Actor *target,
                                  s32 range, s32 force) {
  s32 stopped = 0;

  if (Actor_Distance(target, kid) < range || force != 0) {
    // The heading to the target, and the eight-way directions around it.
    u32 heading = (u16)ArcTan2(target->z.fixed - kid->z.fixed,
                               target->x.fixed - kid->x.fixed);
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
    if (target == Actor_Get(ACTOR_PARTY_LEADER) &&
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

// The update of the two children running their loop. Each stops when it runs up
// behind the other, and otherwise when the party leader is in its way. While
// the party talks to someone or Mind Read shows thoughts (the range widens to
// 26 pixels), the child being talked to stops whatever the direction.
static UNREAD_RESULT Tolbi_UpdateKid(struct Actor *updated) {
  struct WindowWork *window = (struct WindowWork *)gWindowWork[0];
  struct FieldWork *field = FIELD_WORK_FROM(gWindowWork, HEAP_SLOT_WINDOW);
  s32 force = 0;
  s32 range = 18;
  struct Actor *other;
  struct Actor *leader;

  if ((updated->timer & KID_CHASES_YOUNGER) != 0) {
    other = Actor_Get(ACTOR_TOLBI_YOUNGER_SISTER);
  } else {
    other = Actor_Get(ACTOR_TOLBI_OLDER_BROTHER);
  }
  if (Tolbi_KidStopForTarget(updated, other, 32, 0) != 0) {
    RETURN_UNREAD(0);
  }

  leader = Actor_Get(ACTOR_PARTY_LEADER);

  if ((s16)field->talk != 0 || window->mindRead != 0) {
    range = 26;
    if ((updated->timer & KID_TALKING) != 0) {
      force = 1;
    }
  }

  Tolbi_KidStopForTarget(updated, leader, range, force);
  RETURN_UNREAD(0);
}

const struct SceneEntrance *Tolbi_GetEntrances(void) { return gTolbiEntrances; }

const struct SceneExitRegion *Tolbi_GetRegions(void) { return 0; }

const u32 *Tolbi_GetExits(void) { return gTolbiExits; }

const struct ActorPlacement *Tolbi_GetActorPlacements(void) {
  return gTolbiActorPlacements;
}

/// Growth on the sprout makes a plant the party can climb.
static void Tolbi_GrowSprout(void) {
  struct Actor *sprout;

  sprout = Actor_Get(ACTOR_TOLBI_SPROUT);
  if (sprout != 0) {
    sprout->collisionFlags = 0;
  }
  sprout = Actor_Get(ACTOR_TOLBI_SPROUT);
  Actor_SetSpriteFlags(sprout, 0);
  Field_SetCellBehaviorAt(0, 544, 288, CELL_BEHAVIOR_CLIMB);
  GameFlag_Set(FLAG_TOLBI_SPROUT_GROWN);
}

/// Reveal uncovers the hidden chest.
static void Tolbi_RevealHiddenChest(void) {
  MapGrid_CopyCellAttributes(3, 32, 1, 1, 3, 26);
  TreasureSpot_SetPosition(CELL_BEHAVIOR_TOLBI_HIDDEN_CHEST, px(56), px(424));
}

/// When Reveal wears off, the chest's cell is covered again and the chest
/// moves out of sight.
static void Tolbi_HideRevealedChest(void) {
  MapGrid_CopyCellAttributes(2, 25, 1, 1, 3, 26);
  TreasureSpot_SetPosition(CELL_BEHAVIOR_TOLBI_HIDDEN_CHEST, -1, -1);
}

/// The first sign at Tolbi Spring says to face away and toss a coin in.
static void Tolbi_ReadSpringSign(void) {
  Event_SetMessageId(MsgTolbiFaceAwayTolbi);
  // "Face away from Tolbi Spring and toss a coin into it. If you..."
  Event_ShowMessage(-1, 0);
}

/// The second sign at Tolbi Spring says a Lucky Medal wins better prizes.
static void Tolbi_ReadLuckyMedalSign(void) {
  Event_SetMessageId(MsgTolbiTossLuckyMedal);
  // "If you toss a Lucky Medal and you get really lucky, you'll..."
  Event_ShowMessage(-1, 0);
}

/// Talking to the older brother, or reading his mind.
static void Tolbi_TalkToOlderBrother(void) {
  struct FieldWork *work = gFieldWork;
  struct Actor *kid;
  s16 facing;
  s32 msg;

  kid = Actor_Get(ACTOR_TOLBI_OLDER_BROTHER);
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
  Actor_SetAnimation(ACTOR_TOLBI_OLDER_BROTHER, ANIM_STILL);
  Actor_FaceEachOther(ACTOR_TOLBI_OLDER_BROTHER, ACTOR_PARTY_LEADER, 2);
  // "I'm the oldest, so I shouldn't have to share my allowance."
  Event_ShowMessageThenWait(ACTOR_TOLBI_OLDER_BROTHER, 0, 10);
  kid->facing = facing;
  wait(1);
  kid->timer &= KID_CHASES_YOUNGER;
  Event_End();
}

/// Talking to the younger sister, or reading her mind, as above.
static void Tolbi_TalkToYoungerSister(void) {
  struct FieldWork *work = gFieldWork;
  struct Actor *kid;
  s16 facing;
  s32 msg;

  kid = Actor_Get(ACTOR_TOLBI_YOUNGER_SISTER);
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
  Actor_SetAnimation(ACTOR_TOLBI_YOUNGER_SISTER, ANIM_STILL);
  Actor_FaceEachOther(ACTOR_TOLBI_YOUNGER_SISTER, ACTOR_PARTY_LEADER, 2);
  // "Mama told us to share the money. Doesn't my brother remember?"
  Event_ShowMessageThenWait(ACTOR_TOLBI_YOUNGER_SISTER, 0, 10);
  kid->facing = facing;
  wait(1);
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
static void Tolbi_AskAboutLivingPastHundred(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiSomeoneLiveMore);
  // "Can someone live to be more than 100 years old?"
  // Yes: "Hmmm. Still, I'm pretty sure Babi is a lot older than 100..."
  // No: "You could be right. But then, what about Tolbi's ruler,..."
  Event_AskYesNo(ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, 0);
  Event_End();
}

/// The father asks whether children really do love souvenirs, and owns up to
/// loving them himself.
static void Tolbi_TalkToFather(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiChildrenLoveSouvenirs);
  // "Children really do love souvenirs, don't they?"
  // Yes: "I'll bet you like them, too. Don't try to hide it! I used to..."
  // No: "Hmph! You're only saying that because your parents never..."
  Event_AskYesNo(ACTOR_TOLBI_FATHER, 0);
  Event_End();
}

/// The mother turns to her crying boy, then back to promise better sweets
/// later on.
static void Tolbi_TalkToMother(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiPatientLittleGuy);
  Actor_FaceDirection(ACTOR_TOLBI_MOTHER, FACING_NORTH, 0);
  // "Be patient, little guy."
  Event_ShowMessage(ACTOR_TOLBI_MOTHER, 0);
  Actor_FaceDirection(ACTOR_TOLBI_MOTHER, FACING_WEST, 0);
  // "I'll buy you better sweets than those later on."
  Event_ShowMessage(ACTOR_TOLBI_MOTHER, 0);
  Event_End();
}

/// The crying boy turns, stamps twice and wails for sweets.
static void Tolbi_TalkToCryingBoy(void) {
  Event_Start();
  Actor_FaceDirection(ACTOR_TOLBI_CRYING_BOY, FACING_SOUTH, 0);
  Actor_StartRepeatedMotion(ACTOR_TOLBI_CRYING_BOY, 2);
  Event_SetMessageId(MsgTolbiWaahBuySweets);
  // "Waah! Buy me some sweets! I said buy me some sweets!"
  Event_ShowMessage(ACTOR_TOLBI_CRYING_BOY, 0);
  Event_End();
}

/// During the festival the scalper sleeps under the archway, because the inns
/// are full.
static void Tolbi_TalkToSleepingScalper(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiInnsFullStaying);
  // "The inns are all full, so I'm staying here. This archway..."
  Event_ShowMessage(ACTOR_TOLBI_SCALPER, 0);
  Event_End();
}

/// After Colosso, ACTOR_TOLBI_FINALS_FAN asks whether the party was in the
/// finals.
static void Tolbi_AskWhetherPartyWasInFinals(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiFinalsWerent);
  // "You were in the finals, weren't you?"
  // Yes: "I was moved by how you were so determined, despite your size."
  // No: "What? You weren't? You know, you look just like that warrior."
  Event_AskYesNo(ACTOR_TOLBI_FATHER, 0);
  Event_End();
}

/// After Colosso, the snack vendor gives the party his leftover corn, once.
static void Tolbi_TalkToSnackVendor(void) {
  Event_Start();
  if (Event_GetFlag(FLAG_TOLBI_CORN_GIVEN) == 0) {
    GameFlag_Set(FLAG_TOLBI_CORN_GIVEN);
    Event_SetMessageId(MsgTolbiLeftovers);
    // "Hey there, these are leftovers, but have some!"
    Event_ShowMessage(ACTOR_TOLBI_SNACK_VENDOR, 0);
    Item_ShowFound(ITEM_CORN, 3);
    // "You got some corn."
    Event_ShowMessage(ACTOR_TOLBI_SNACK_VENDOR, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Party_GiveItem(ITEM_CORN, 0);
  } else {
    Event_SetMessageId(MsgTolbiFestivalLongerUsual);
    // "The festival was longer than usual this year, so I made more..."
    Event_ShowMessage(ACTOR_TOLBI_SNACK_VENDOR, 0);
  }
  Event_End();
}

/// Walking into one of the town's doors opens it and takes the party inside.
static void Tolbi_EnterDoor(void) {
  struct FieldWork *work = gFieldWork;
  u32 actorId;
  s32 door;

  Event_Start();
  for (actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_ID; actorId++) {
    struct Actor *actor = Actor_Get(actorId);
    if (actor != NULL) {
      actor->motionFlags = 0;
    }
  }
  door = work->stepTrigger - 1;
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gTolbiDoors[door].steps, gTolbiDoors[door].x,
                       gTolbiDoors[door].y);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (door != TOLBI_TO_WEAPON_AND_ARMOR_SHOP - 1) {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
    Event_Wait(10);
  }
  Import_EventSetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// The update of ACTOR_TOLBI_OLD_MAN, who during the festival says he is 65
/// and that Babi was already old when he was born.
static void Tolbi_CopyLeaderPriority(struct Actor *updated) {
  s32 priority;

  if (updated != NULL) {
    priority = Actor_Get(ACTOR_PARTY_LEADER)->sprite->image.priority;
    updated->priorityFlags = 0;
    updated->sprite->image.priority = priority;
    updated->sprite->shadow.priority = priority;
  }
}

/// Sets up the scene on entry.
s32 Tolbi_ApplyEntryState(s32 unused) {
  struct Actor *actor;
  void (*kidUpdate)(struct Actor *);

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  // The two children run their loop south-east of the fountain.
  Actor_SetPosition(ACTOR_TOLBI_OLDER_BROTHER, 352, 352);
  Actor_SetScript(ACTOR_TOLBI_OLDER_BROTHER, gTolbiOlderBrotherRun);
  actor = Actor_Get(ACTOR_TOLBI_OLDER_BROTHER);
  kidUpdate = (void (*)(struct Actor *))Tolbi_UpdateKid;
  actor->timer = KID_CHASES_YOUNGER;
  actor->update = kidUpdate;
  Actor_SetPosition(ACTOR_TOLBI_YOUNGER_SISTER, 368, 320);
  Actor_SetScript(ACTOR_TOLBI_YOUNGER_SISTER, gTolbiYoungerSisterRun);
  actor = Actor_Get(ACTOR_TOLBI_YOUNGER_SISTER);
  actor->timer = 0;
  actor->update = kidUpdate;
  actor = Actor_Get(ACTOR_TOLBI_OLD_MAN);
  actor->update = Tolbi_CopyLeaderPriority;

  // What the party has already done here.
  if (GameFlag_Get(FLAG_TOLBI_SPRING_OPENED) != 0) {
    Actor_SetPosition(ACTOR_TOLBI_SPRING_VISITOR, 316, 328);
  }
  if (GameFlag_Get(FLAG_TOLBI_PUDDLE_FROZEN) != 0) {
    Tolbi_FreezePuddle();
  }
  if (GameFlag_Get(FLAG_TOLBI_SPROUT_GROWN) != 0) {
    Tolbi_GrowSprout();
    Actor_SetAnimation(ACTOR_TOLBI_SPROUT, 4);
  }

  // After Colosso the crowd has gone home and is moved off the map. On finals
  // day the scalper waits by the gate.
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    Actor_SetPosition(ACTOR_TOLBI_WOULD_BE_FIGHTER, 520, 560);
    Actor_SetPosition(ACTOR_TOLBI_XIAN_VISITOR, 520, 560);
    Actor_SetPosition(ACTOR_TOLBI_OVERSLEEPER, 520, 560);
    Actor_SetPosition(ACTOR_TOLBI_FATHER, 520, 560);
    Actor_SetPosition(ACTOR_TOLBI_MOTHER, 520, 560);
    Actor_SetPosition(ACTOR_TOLBI_CRYING_BOY, 520, 560);
    Actor_SetPosition(ACTOR_TOLBI_SCALPER, 520, 560);
  } else {
    if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      Actor_SetPosition(ACTOR_TOLBI_SCALPER, 280, 80);
      Actor_FaceDirection(ACTOR_TOLBI_SCALPER, FACING_SOUTHEAST, 0);
      Actor_SetAnimation(ACTOR_TOLBI_SCALPER, ANIM_STAND);
    }
  }
  return 0;
}

/// The visitor at the spring's gate asks whether the party wants to test its
/// luck.
static void Tolbi_TalkToSpringVisitor(void) {
  Event_Start();
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 304, 312);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_TOLBI_SPRING_VISITOR, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_SetMessageId(MsgTolbiWantTestLuck);
  // "Hmm? Do you want to test your luck at Tolbi Spring?"
  Event_OpenMessage(ACTOR_TOLBI_SPRING_VISITOR, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    // Yes: "I'll give you the chance to go first."
    gFieldWork->message++;
    // "Really? Well then, I'll give you the chance to go first."
    Event_ShowMessage(ACTOR_TOLBI_SPRING_VISITOR, 0);
    Actor_SetSpeed(ACTOR_TOLBI_SPRING_VISITOR, 1);
    Actor_WalkToAndWait(ACTOR_TOLBI_SPRING_VISITOR, 320, 304);
    Actor_WalkToAndWait(ACTOR_TOLBI_SPRING_VISITOR, 316, 328);
    Actor_FaceDirection(ACTOR_TOLBI_SPRING_VISITOR, FACING_NORTHWEST, 0);
    GameFlag_Set(FLAG_TOLBI_SPRING_OPENED);
  } else {
    // "Oh, you don't? Well, I'm just going to keep trying for a while."
    Event_ShowMessage(ACTOR_TOLBI_SPRING_VISITOR, 0);
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
  Import_EventSetExitRequest(gFieldWork->stepTrigger);
}

/// Two soldiers searching for the missing Babi stop the party and show his
/// picture.
static void Tolbi_MeetSearchingSoldiers(void) {
  s32 firstMsg;

  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetSpeed(ACTOR_TOLBI_SOLDIER_A, 1);
  Actor_SetSpeed(ACTOR_TOLBI_SOLDIER_B, 1);
  firstMsg = MsgTolbiSeenAnyoneWho;
  Event_SetMessageId(firstMsg);

  // The soldiers come up to the party; the poster waits off to the side.
  Actor_SetPosition(ACTOR_TOLBI_SOLDIER_A, 72, 208);
  Actor_SetPosition(ACTOR_TOLBI_SOLDIER_B, 56, 208);
  Actor_SetSpriteColorById(ACTOR_TOLBI_MISSING_PERSON_POSTER,
                           SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_TOLBI_MISSING_PERSON_POSTER), 0);
  Actor_SetPosition(ACTOR_TOLBI_MISSING_PERSON_POSTER, 95, 40);
  Actor_WalkTo(ACTOR_TOLBI_SOLDIER_A, 72, 248);
  Actor_WalkTo(ACTOR_TOLBI_SOLDIER_B, 56, 248);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 64, 264);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_TOLBI_SOLDIER_A);
  Actor_SetAnimation(ACTOR_TOLBI_SOLDIER_A, ANIM_STAND);
  Actor_SetAnimation(ACTOR_TOLBI_SOLDIER_B, ANIM_STAND);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceActor(ACTOR_TOLBI_SOLDIER_A, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_TOLBI_SOLDIER_B, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_TOLBI_SOLDIER_A,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_TOLBI_SOLDIER_B,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_TOLBI_SOLDIER_A, 2);
  Actor_RunRepeatedMotion(ACTOR_TOLBI_SOLDIER_B, 2);
  Event_Wait(20);

  // "Have you seen anyone who looks like this?", under a "Missing Person"
  // window showing Babi's portrait. The poster, placed with Babi's sprite,
  // becomes the speaker so that answering closes that portrait window.
  // "Hey, have you seen anyone who looks like this?"
  Event_OpenMessage(ACTOR_TOLBI_SOLDIER_A, 0);
  Event_Wait(25);
  Portrait_OpenWindow(SPRITE_BABI, 0, 12, 7);
  Message_OpenWindow((firstMsg + 3), 11, 12, 2);
  gFieldWork->speaker = ACTOR_TOLBI_MISSING_PERSON_POSTER;
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    // Yes: "Babi must be nearby!!!" They nod and run east.
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_TOLBI_SOLDIER_B, 2);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_TOLBI_SOLDIER_B, FACING_EAST, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_TOLBI_SOLDIER_A, ANIM_NOD);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_TOLBI_SOLDIER_A, FACING_EAST, 0);
    Event_Wait(30);
    // "Babi must be nearby!!!"
    Event_ShowMessage(ACTOR_TOLBI_SOLDIER_A, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_TOLBI_SOLDIER_A, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_TOLBI_SOLDIER_B, FACING_SOUTH, 0);
    Event_Wait(30);
    Actor_SetAnimation(ACTOR_TOLBI_SOLDIER_A, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_TOLBI_SOLDIER_B, ANIM_NOD);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_TOLBI_SOLDIER_A, 1.8);
    Actor_SetSpeed(ACTOR_TOLBI_SOLDIER_B, 1.8);
    Actor_WalkTo(ACTOR_TOLBI_SOLDIER_A, 232, 248);
    Event_Wait(2);
    Actor_WalkTo(ACTOR_TOLBI_SOLDIER_B, 232, 248);
    Actor_WaitForMoveById(ACTOR_TOLBI_SOLDIER_A);
    Actor_WalkTo(ACTOR_TOLBI_SOLDIER_A, 248, 248);
    Actor_WalkToAndWait(ACTOR_TOLBI_SOLDIER_B, 248, 248);
  } else {
    // No: "He doesn't seem to be here. Let's look over there." They shake
    // their heads and go back north.
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_TOLBI_SOLDIER_B, 2);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_TOLBI_SOLDIER_B, FACING_EAST, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_TOLBI_SOLDIER_A, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_TOLBI_SOLDIER_A, FACING_EAST, 0);
    Event_Wait(30);
    gFieldWork->message += 1;
    // "Where's Babi!? He doesn't seem to be here. Let's look over..."
    Event_ShowMessage(ACTOR_TOLBI_SOLDIER_A, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_TOLBI_SOLDIER_A, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_TOLBI_SOLDIER_B, FACING_SOUTH, 0);
    Event_Wait(30);
    Actor_SetAnimation(ACTOR_TOLBI_SOLDIER_A, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_TOLBI_SOLDIER_B, ANIM_NOD);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_TOLBI_SOLDIER_A, 1.6);
    Actor_SetSpeed(ACTOR_TOLBI_SOLDIER_B, 1.6);
    Actor_WalkTo(ACTOR_TOLBI_SOLDIER_A, 72, 184);
    Actor_WalkToAndWait(ACTOR_TOLBI_SOLDIER_B, 56, 184);
  }
  Actor_SetPosition(ACTOR_TOLBI_SOLDIER_A, 0, 0);
  Actor_SetPosition(ACTOR_TOLBI_SOLDIER_B, 0, 0);
  Actor_SetPosition(ACTOR_TOLBI_MISSING_PERSON_POSTER, 0, 0);
  GameFlag_Set(FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI);
  Event_End();
}

/// Frost on the puddle freezes it into a pillar.
static void Tolbi_FreezePuddle(void) {
  struct Actor *puddle = Actor_Get(ACTOR_TOLBI_PUDDLE);
  if (puddle != 0) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    puddle->motionFlags = 0;
  }
  Actor_SetAnimation(ACTOR_TOLBI_PUDDLE, PUDDLE_ANIMATION_PILLAR);
  MapGrid_CopyCellAttributes(36, 16, 1, 1, 34, 16);
  GameFlag_Set(FLAG_TOLBI_PUDDLE_FROZEN);
}

/// The actor script of a mirrored actor, which flips its sprite and passes 7
/// on.
s32 gTolbiMirroredActor[] = {
    SC_SET(SW_SCALE_X, -Q16_ONE),
    SC_ANIMATION(7),
    SC_END,
};

/// The older brother runs a loop of eight points, 32 pixels across, south-east
/// of the fountain.
s32 gTolbiOlderBrotherRun[] = {
    SC_ANIMATION(ANIM_WALK),       SC_SPEED(0.8),
    SC_ACCELERATION(0.2),          SC_LABEL(1),
    SC_MOVE(352, 0, 352),          SC_SYNC,
    SC_MOVE(368, 0, 352),          SC_SYNC,
    SC_MOVE(376, 0, 344),          SC_SYNC,
    SC_MOVE(376, 0, 328),          SC_SYNC,
    SC_MOVE(368, 0, 320),          SC_SYNC,
    SC_MOVE(352, 0, 320),          SC_SYNC,
    SC_MOVE(344, 0, 328),          SC_SYNC,
    SC_MOVE(344, 0, 344),          SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

/// The younger sister runs the same loop half a lap ahead.
s32 gTolbiYoungerSisterRun[] = {
    SC_ANIMATION(ANIM_WALK),       SC_SPEED(0.8),
    SC_ACCELERATION(0.2),          SC_LABEL(1),
    SC_MOVE(368, 0, 320),          SC_SYNC,
    SC_MOVE(352, 0, 320),          SC_SYNC,
    SC_MOVE(344, 0, 328),          SC_SYNC,
    SC_MOVE(344, 0, 344),          SC_SYNC,
    SC_MOVE(352, 0, 352),          SC_SYNC,
    SC_MOVE(368, 0, 352),          SC_SYNC,
    SC_MOVE(376, 0, 344),          SC_SYNC,
    SC_MOVE(376, 0, 328),          SC_SYNC,
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
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        552, 0, 288, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        552, 0, 264, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 264, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        208, 0, 312, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 224, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 216, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        152, 0, 304, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        24, 0, 456, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (s32)gTolbiOlderBrotherRun,
        344, 0, 328, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (s32)gTolbiYoungerSisterRun,
        376, 0, 328, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 200, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        400, 0, 432, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 120, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        184, 0, 472, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        -64, 0, -64, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        256, 0, 168, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 184, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        272, 0, 168, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, (s32)gTolbiMirroredActor,
        312, 0, 120, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        302, 0, 296, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        -9, 0, 488, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        -9, 0, 488, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    // Ember, until its Djinni flag is set: twenty flags to an element.
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_EMBER), MOVEMENT_TYPE_STAND,
        584, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BABI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        -9, 0, 488, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
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
    { TRIGGER_TABLE_END },
};

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
    { TRIGGER_TABLE_END },
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
    { TRIGGER_TABLE_END },
};
// clang-format on

/// How a door opens, as two frames of cells copied over the closed door.
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
