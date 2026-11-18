/// @file
/// Daila, the northernmost town of Indra, where the tidal wave left salt
/// puddles all over the streets, in two scenes that share this overlay.
///
/// The village is two scenes: SCENE_DAILA_FLOODED, under the standing salt
/// water the tidal wave left, and SCENE_DAILA once Tavi is rescued and it has
/// drained to puddles; the getters pick each scene's tables by
/// gMainRoutineWork.next. Doors into the houses are step triggers 1 to 6
/// (Daila_EnterDoor).
///
/// Once it drains, Alex waits for the party by the inn and goes on alone to
/// Madra (Daila_MeetAlex). Six puddles lie in its streets: Parch dries one for
/// the visit (Daila_ParchFirstPuddle to Daila_ParchSixthPuddle) and Frost
/// freezes one into a pillar of ice to jump onto from the cells around it
/// (Scene_RaiseHeldActor, gDailaRooms_PillarFrozenFlags). Scoop on one spot
/// digs up a Psy Crystal (Daila_DigUpPsyCrystal).

#include "field/field_sequence_event.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/field_event.h"
#include "field/scene_tables.h"
#include "field/event_inline.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "scene_ids.h"
#include "field/actor.h"
#include "flags.h"
#include "scene/scene.h"
#include "text/msg_ids.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "shop/shop.h"

/// The drained village's puddles dried by Parch for this visit, one flag each,
/// in the order of the puddles' rows, and frozen into pillars by Frost.
#define FLAG_DAILA_FIRST_PUDDLE_PARCHED 0x210
#define FLAG_DAILA_SECOND_PUDDLE_PARCHED 0x211
#define FLAG_DAILA_THIRD_PUDDLE_PARCHED 0x212
#define FLAG_DAILA_FOURTH_PUDDLE_PARCHED 0x213
#define FLAG_DAILA_FIFTH_PUDDLE_PARCHED 0x214
#define FLAG_DAILA_SIXTH_PUDDLE_PARCHED 0x215
/// The first of six flags, one for each of the drained village's puddles that
/// Frost has frozen into a pillar for this visit, by
/// gDailaRooms_PillarFrozenFlags.
#define FLAG_DAILA_PUDDLE_FROZEN_FIRST 0x216

/// The people and puddles the events move, numbering the rows of
/// gDailaActorPlacements (the flooded village's rows to ACTOR_ITEM_SHOPKEEPER
/// are alike).
enum DailaActorId {
  /// "You don't look familiar. Have you been to Daila before?"
  ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE = ACTOR_FIRST_PLACED,
  /// "Didn't it feel like all of Indra moved when that wave hit?"
  ACTOR_WOMAN_WHO_FELT_INDRA_MOVE,
  /// "I saw a bright light in the northwestern sky earlier... Somewhere over
  /// Gondowan."
  ACTOR_MAN_WHO_SAW_A_LIGHT,
  /// "Thank goodness for that mountain range to the north..."
  ACTOR_VILLAGER_THANKFUL_FOR_THE_MOUNTAINS,
  ACTOR_FALLEN_OLD_MAN,
  /// "That ancient tower to the east? It's the Shrine of the Sea God..."
  ACTOR_VILLAGER_ON_THE_SHRINE,
  /// "Riki and Tavi sure are out late..."
  ACTOR_VILLAGER_WAITING_UP_FOR_RIKI_AND_TAVI,
  /// "I wonder if those two are still out there... They know it's
  /// off-limits."
  ACTOR_VILLAGER_ON_RIKI_AND_TAVI,
  /// "Our village survived the wave, but all our boats were destroyed..."
  ACTOR_SAILOR_WITHOUT_A_BOAT,
  /// "Hm? You want a boat?"
  ACTOR_SAILOR_ON_THE_BOATS,
  /// "That tidal wave left a lot of standing water in our village..."
  ACTOR_MAN_ON_THE_STANDING_WATER,
  /// "Yaaay! Puddles in the village! Yaaay!"
  ACTOR_CHILD_WHO_LOVES_PUDDLES,
  /// "Bleah! I fell in a puddle and got my clothes dirty! Mom's gonna have a
  /// fit!"
  ACTOR_CHILD_WHO_FELL_IN_A_PUDDLE,
  /// The keeper of the item stall across from the inn.
  ACTOR_ITEM_SHOPKEEPER,
  ACTOR_FIRST_PUDDLE,
  ACTOR_SECOND_PUDDLE,
  ACTOR_THIRD_PUDDLE,
  ACTOR_FOURTH_PUDDLE,
  ACTOR_FIFTH_PUDDLE,
  ACTOR_ALEX,
  ACTOR_KRADEN,
  /// After Jenna's and Sheba's rows, which place the party's own actors.
  ACTOR_SIXTH_PUDDLE
};

/// The buried Psy Crystal: the village's third scene item, after the jar and
/// the barrel.
#define ACTOR_PSY_CRYSTAL (FIELD_TREASURE_ACTOR + 2)

extern struct SceneEntrance gDailaEntrances[];
extern s32 gDailaFloodedExits[];
extern s32 gDailaExits[];
extern struct ActorPlacement gDailaFloodedActorPlacements[];
extern struct ActorPlacement gDailaActorPlacements[];
extern struct Trigger gDailaFloodedTriggers[];
extern struct Trigger gDailaTriggers[];
extern s16 gDailaRooms_PillarFrozenFlags[];
extern s32 gDaila_FallenOldManScript[];
extern u32 gDaila_PuddleChildScript[];
void Field_TryJumpForward(void);
void Actor_FallOverById(s32 a, s32 b);
void FlagBlock_ApplyFlags(s16 *data);

const struct SceneEntrance *Scene_GetEntrances(void) { return gDailaEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

s32 *Scene_GetExits(void) {
  if (gMainRoutineWork.next == SCENE_DAILA)
    return gDailaExits;
  else
    return gDailaFloodedExits;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_DAILA)
    return gDailaActorPlacements;
  else
    return gDailaFloodedActorPlacements;
}

/// Stepping onto one of the village's doors: the leader walks through it and
/// the scene leaves by the exit of its number.
void Daila_EnterDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  Sound_Play(SE_MAP_EXIT);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_CenterAndWalk(gMainRoutineWork.leaderActorId, 2, -6);
  Event_Wait(10);
  Import_EventSetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Scoop on the puddle's cell digs up the buried Psy Crystal.
void Daila_DigUpPsyCrystal(s32 mode) {
  Event_DigUpTreasure(mode, ACTOR_PSY_CRYSTAL, FLAG_DAILA_PSY_CRYSTAL_DUG);
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_DAILA)
    return gDailaTriggers;
  else
    return gDailaFloodedTriggers;
}

/// The greeter asks once whether the party has been to Daila before; after
/// that, he marvels that the wave hurt no one.
void Daila_TalkBeenBefore(void) {
  s32 msg;

  if (GameFlag_Get(FLAG_DAILA_GREETER_ASKED) == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    msg = (s32)MsgDailaBeenBefore;
    Event_SetMessageId(msg);
    // "You don't look familiar. Have you been to Daila before?"
    FieldMessage_ShowAtActor(ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE, 0);
    GameFlag_Set(FLAG_DAILA_GREETER_ASKED);
    Event_End();
  } else {
    Event_SetMessageId((s32)MsgDailaWaveNoInjuries);
    // "We were hit by a gigantic tidal wave, but there were no injuries......"
    Event_ShowMessageAndWait(ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE, 0);
  }
}

/// A villager asks whether all of Indra seemed to move when the tidal wave hit,
/// and answers the choice.
void Daila_TalkIndraMoved(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaIndraMoved;
  Event_SetMessageId(msg);
  // "Didn't it feel like all of Indra moved when that wave hit?"
  FieldMessage_ShowAtActor(ACTOR_WOMAN_WHO_FELT_INDRA_MOVE, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(ACTOR_WOMAN_WHO_FELT_INDRA_MOVE, 0);
  Event_End();
}

// BUG: the question and its answer open at the woman who felt Indra move, as
// in Daila_TalkIndraMoved, not at the sailor who asks.
#if BUGFIX
#define BOAT_ASKER ACTOR_SAILOR_ON_THE_BOATS
#else
#define BOAT_ASKER ACTOR_WOMAN_WHO_FELT_INDRA_MOVE
#endif

/// A sailor asks whether the party wants a boat, and answers the choice.
void Daila_TalkBoat(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaWantBoat;
  Event_SetMessageId(msg);
  // "Hm? You want a boat?"
  FieldMessage_ShowAtActor(BOAT_ASKER, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(BOAT_ASKER, 0);
  Event_End();
}

/// Daila's item shop: facing the counter opens it; otherwise the keeper worries
/// that Tavi and Riki have not been seen lately.
void Daila_AskAfterTaviAndRiki(s32 keeper) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_DAILA_ITEMS, keeper);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDailaTaviRikiWorry);
    // "I haven't seen Tavi and Riki around lately... I like those little..."
    Event_ShowMessageAndWait(keeper, 0);
    Event_End();
  }
}

/// The old man knocked over asks whether the party thought the wave hurt his
/// hip, and answers the choice.
void Daila_TalkHip(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaHurtHip;
  Event_SetMessageId(msg);
  // "I bet you thought it was the wave that knocked me over and hurt my..."
  FieldMessage_ShowAtActor(ACTOR_FALLEN_OLD_MAN, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(ACTOR_FALLEN_OLD_MAN, 0);
  Event_End();
}

/// Daila's item shop once Tavi and Riki are home: facing the counter opens it;
/// otherwise the keeper tells of their return.
void Daila_TaviAndRikiAreHome(s32 keeper) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_DAILA_ITEMS, keeper);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDailaTaviRikiHome);
    // "Tavi and Riki are back home, safe and sound... They stopped by to..."
    Event_ShowMessageAndWait(keeper, 0);
    Event_End();
  }
}

/// Brings the actor to the player character's position.
static inline void Actor_JoinLeader(s32 actor) {
  struct Actor *leader;

  Actor_SetAnimationById(actor, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(actor, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(actor);
  Actor_SetPosition(actor, 0, 0);
}

/// Where the meeting's lines, from MsgDailaFelixCall on, go their own way after
/// Jenna asks whether to go to Madra with Alex.
enum MeetAlexMessage {
  /// "Ah... You...want to travel with me?"
  MEET_ALEX_TRAVEL_WITH_ME = 11,
  /// "Ah. Felix doesn't think it's a good idea."
  MEET_ALEX_NOT_A_GOOD_IDEA = 13,
  /// "No, that's not it. I'm just...preoccupied."
  MEET_ALEX_PREOCCUPIED = 15
};

/// Alex waits for the party in the street, says the mayor sent him to look for
/// a boat in Madra, turns down their company and leaves.
void Daila_MeetAlex(void) {
  s32 msg;

  if (GameFlag_Get(FLAG_DAILA_MET_ALEX))
    return;
  if (GameFlag_Get(FLAG_MADRA_GATE_INSPECTED))
    return;
  if (GameFlag_Get(FLAG_IN_PARTY(ACTOR_PIERS)))
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaFelixCall;
  Event_SetMessageId(msg);
  Actor_SetPosition(ACTOR_ALEX, 472, 264);
  Event_Wait(30);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 504, 296);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 0);
  Actor_MoveToAndWait(ACTOR_ALEX, 472, 280);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(60);
  // "Felix..."
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, 8, 16, FACING_NORTHWEST);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 16, 0, FACING_NORTHWEST);
  Actor_LaunchFromLeader(ACTOR_KRADEN, 32, -8, FACING_WEST);
  Event_Wait(60);
  // "Well, well... Our happy little family is back together again."
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Event_Wait(60);
  // "Alex... Where did you go?"
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_ALEX, FACING_EAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_ALEX, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  // "To look for a ship, of course."
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Event_Wait(30);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_KRADEN, 0);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(30);
  // "That's what Kraden said..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Event_Wait(30);
  Actor_TurnToFaceEachOther(gMainRoutineWork.leaderActorId, ACTOR_KRADEN, 0);
  Event_Wait(30);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_ALEX, 0);
  Actor_FaceActorById(gMainRoutineWork.leaderActorId, ACTOR_ALEX, 0);
  Event_Wait(60);
  // "And did you find one?"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Event_Wait(30);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_ALEX, 0);
  Actor_SetAnimationById(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  // "Unfortunately, there are none to be had."
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_ALEX, FACING_NORTH, 0);
  Event_Wait(60);
  // "I spoke to the mayor, and he suggested that we try the large town..."
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_ALEX, gMainRoutineWork.leaderActorId, 0);
  Event_Wait(60);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 1);
  Event_Wait(60);
  // "Is that where you're going?"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_ALEX, ANIM_NOD);
  Event_Wait(30);
  // "Yes. Madra may have boats available. That would be the most logical..."
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Event_Wait(30);
  Actor_FaceActorById(ACTOR_JENNA, gMainRoutineWork.leaderActorId, 0);
  // "So what do yout think? Should we go to Madra with Alex?"
  FieldMessage_ShowAtActor(ACTOR_JENNA, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(30);
    Event_SetMessageId(msg + MEET_ALEX_TRAVEL_WITH_ME);
    // "Ah... You...want to travel with me?"
    Event_ShowMessageAndWait(ACTOR_ALEX, 0);
    Event_Wait(60);
    Actor_ShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Event_Wait(60);
    Actor_FaceActorById(ACTOR_JENNA, ACTOR_ALEX, 0);
    // "What, you don't like that idea?"
    Event_ShowMessageAndWait(ACTOR_JENNA, 0);
    Event_SetMessageId(msg + MEET_ALEX_PREOCCUPIED);
  } else {
    Event_Wait(30);
    Event_SetMessageId(msg + MEET_ALEX_NOT_A_GOOD_IDEA);
    // "Ah. Felix doesn't think it's a good idea."
    Event_ShowMessageAndWait(ACTOR_ALEX, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
    Actor_FaceActorById(ACTOR_JENNA, ACTOR_ALEX, 0);
    // "You seem awfully pleased about that..."
    Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  }
  Actor_SetAnimationById(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  // "No, that's not it. I'm just...preoccupied."
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 1);
  Event_Wait(60);
  // "Self-absorbed is more like it!"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Event_Wait(60);
  Actor_FaceActorById(ACTOR_ALEX, ACTOR_SHEBA, 0);
  Event_Wait(60);
  // "I simply prefer to work alone."
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Event_Wait(60);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  // "If that's the case, we'll let you be..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Event_Wait(60);
  Actor_SetAnimationById(ACTOR_ALEX, ANIM_NOD);
  Event_Wait(60);
  // "Perhaps you will see me in Madra..."
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Actor_WalkToAndWait(ACTOR_ALEX, 504, 328);
  Actor_FaceActorById(gMainRoutineWork.leaderActorId, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_ALEX, 0);
  Actor_WalkToAndWait(ACTOR_ALEX, 456, 328);
  Actor_FaceActorById(gMainRoutineWork.leaderActorId, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_ALEX, 0);
  Actor_WalkToAndWait(ACTOR_ALEX, 424, 472);
  Actor_FaceActorById(gMainRoutineWork.leaderActorId, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_ALEX, 0);
  Event_Wait(60);
  Actor_SetPosition(ACTOR_ALEX, 0, 0);
  Event_Wait(120);
  Actor_Jump(ACTOR_JENNA, 4, 20);
  Actor_Jump(ACTOR_JENNA, 4, 20);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Event_Wait(60);
  // "Gah! The nerve of that guy!"
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Event_Wait(60);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(60);
  // "Yeah, I know... Now, I just want to find a boat before he does!"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Event_Wait(60);
  Actor_FaceActorById(ACTOR_KRADEN, gMainRoutineWork.leaderActorId, 0);
  Event_Wait(60);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH, 0);
  Event_Wait(30);
  // "Let's be on our way."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Event_Wait(60);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(120);
  Actor_JoinLeader(ACTOR_JENNA);
  Actor_JoinLeader(ACTOR_SHEBA);
  Actor_JoinLeader(ACTOR_KRADEN);
  Event_End();
  GameFlag_Set(FLAG_DAILA_MET_ALEX);
}

/// Parch on a puddle's cell dries it for the visit: the dry cell at column
/// fromX of row y is copied over it at toX, the puddle is hidden and its flag
/// set.
#define DEFINE_DAILA_PARCH_PUDDLE(name, fromX, y, toX, actor, flag)            \
  void name(void) {                                                            \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    MapGrid_CopyCellAttributes(fromX, y, 1, 1, toX, y);                        \
    Actor_SetPosition(actor, 0, 0);                                            \
    GameFlag_Set(flag);                                                        \
    Event_End();                                                               \
  }

DEFINE_DAILA_PARCH_PUDDLE(Daila_ParchFirstPuddle, 10, 19, 11,
                          ACTOR_FIRST_PUDDLE, FLAG_DAILA_FIRST_PUDDLE_PARCHED)

DEFINE_DAILA_PARCH_PUDDLE(Daila_ParchSecondPuddle, 25, 20, 26,
                          ACTOR_SECOND_PUDDLE, FLAG_DAILA_SECOND_PUDDLE_PARCHED)

DEFINE_DAILA_PARCH_PUDDLE(Daila_ParchThirdPuddle, 28, 23, 29,
                          ACTOR_THIRD_PUDDLE, FLAG_DAILA_THIRD_PUDDLE_PARCHED)

DEFINE_DAILA_PARCH_PUDDLE(Daila_ParchFourthPuddle, 17, 11, 18,
                          ACTOR_FOURTH_PUDDLE, FLAG_DAILA_FOURTH_PUDDLE_PARCHED)

DEFINE_DAILA_PARCH_PUDDLE(Daila_ParchFifthPuddle, 30, 17, 31,
                          ACTOR_FIFTH_PUDDLE, FLAG_DAILA_FIFTH_PUDDLE_PARCHED)

// BUG: hides actor 31, which the sixth puddle would be if Jenna's and Sheba's
// rows took numbers of their own; they place the party's actors, so the puddle
// is actor 29, no actor 31 exists, and the puddle stays on its dried cell.
#if BUGFIX
#define SIXTH_PUDDLE_HIDDEN ACTOR_SIXTH_PUDDLE
#else
#define SIXTH_PUDDLE_HIDDEN (ACTOR_SIXTH_PUDDLE + 2)
#endif
DEFINE_DAILA_PARCH_PUDDLE(Daila_ParchSixthPuddle, 16, 23, 17,
                          SIXTH_PUDDLE_HIDDEN, FLAG_DAILA_SIXTH_PUDDLE_PARCHED)

void Actor_SetWideSprite(s32 actor, s32 flag);

#include "shared/scene_raise_held_actor.h"

s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (gMainRoutineWork.next == SCENE_DAILA_FLOODED)
    Actor_FallOverById(ACTOR_FALLEN_OLD_MAN, FACING_EAST);
  else if (gMainRoutineWork.next == SCENE_DAILA)
    FlagBlock_PlaceAll(gDailaRooms_PillarFrozenFlags);
  Actor_SetSpritePriority(ACTOR_ITEM_SHOPKEEPER, 1);
  if (gMainRoutineWork.next == SCENE_DAILA &&
      GameFlag_Get(FLAG_DAILA_PSY_CRYSTAL_DUG) == 0)
    Actor_SetWideSprite(ACTOR_PSY_CRYSTAL, 0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.next == SCENE_DAILA) {
    if (GameFlag_Get(FLAG_DAILA_FIRST_PUDDLE_PARCHED))
      MapGrid_CopyCellAttributes(10, 19, 1, 1, 11, 19);
    if (GameFlag_Get(FLAG_DAILA_SECOND_PUDDLE_PARCHED))
      MapGrid_CopyCellAttributes(25, 20, 1, 1, 26, 20);
    if (GameFlag_Get(FLAG_DAILA_THIRD_PUDDLE_PARCHED))
      MapGrid_CopyCellAttributes(28, 23, 1, 1, 29, 23);
    if (GameFlag_Get(FLAG_DAILA_FOURTH_PUDDLE_PARCHED))
      MapGrid_CopyCellAttributes(17, 11, 1, 1, 18, 11);
    if (GameFlag_Get(FLAG_DAILA_FIFTH_PUDDLE_PARCHED))
      MapGrid_CopyCellAttributes(30, 17, 1, 1, 31, 17);
    if (GameFlag_Get(FLAG_DAILA_SIXTH_PUDDLE_PARCHED))
      MapGrid_CopyCellAttributes(16, 23, 1, 1, 17, 23);
  }
  return 0;
}

/// The step triggers of the cells to jump from, one for each heading.
void Daila_JumpForward(void) { Field_TryJumpForward(); }

// clang-format off
s16 gDailaRooms_PillarFrozenFlags[] = {
    ACTOR_FIRST_PUDDLE,  FLAG_DAILA_PUDDLE_FROZEN_FIRST + 0,
    ACTOR_SECOND_PUDDLE, FLAG_DAILA_PUDDLE_FROZEN_FIRST + 1,
    ACTOR_THIRD_PUDDLE,  FLAG_DAILA_PUDDLE_FROZEN_FIRST + 2,
    ACTOR_FOURTH_PUDDLE, FLAG_DAILA_PUDDLE_FROZEN_FIRST + 3,
    ACTOR_FIFTH_PUDDLE,  FLAG_DAILA_PUDDLE_FROZEN_FIRST + 4,
    ACTOR_SIXTH_PUDDLE,  FLAG_DAILA_PUDDLE_FROZEN_FIRST + 5,
    -1, 0,
};

s32 gDaila_FallenOldManScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
    SC_ANIMATION(0),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(40),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(5),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

u32 gDaila_PuddleChildScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SET(SW_GRAVITY, toFIXED(0.1)),
    SC_SPEED(0.4),
    SC_ANIMATION(0),
    SC_LABEL(1),
    SC_SET(SW_Y, toFIXED(4)),
    SC_WAIT(30),
    SC_SET(SW_Y, toFIXED(4)),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

struct SceneEntrance gDailaEntrances[] = {
    {0, CONDITION_ALWAYS, 120, 0, 338, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 368, 0, 224, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {20, CONDITION_ALWAYS, 80, 0, 352, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

s32 gDailaFloodedExits[] = {
    SCENE_EXITS(DailaFlooded),
    SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_WEST, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_NORTHWEST, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_NORTHEAST, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_EAST, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_SOUTHWEST, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_DAILA_ROOMS_HOUSES_SOUTHEAST, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_DAILA_ROOMS_SANCTUM, DailaRoomsSanctum, DAILA_ROOMS_SANCTUM_FROM_DAILA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_DAILA_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(DAILA_FLOODED_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_DAILA_NORTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

s32 gDailaExits[] = {
    SCENE_EXITS(Daila),
    SCENE_EXIT_UNLESS(1, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_DAILA_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, DailaRoomsSanctum, DAILA_ROOMS_SANCTUM_FROM_DAILA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, WorldMap, WORLD_MAP_FROM_DAILA_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, WorldMap, WORLD_MAP_FROM_DAILA_NORTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// Unused: an empty placement table nothing reads.
struct ActorPlacement gDaila_UnusedPlacements[] = {
    {SCENE_TABLE_END},
};
// clang-format on

struct ActorPlacement gDailaFloodedActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER, 40, 0, 280, FACING_EAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER_NEAR, 264, 0, 232, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER_NEAR, 88, 0, 376, FACING_EAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT_SMALL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 264, 0, 312, FACING_WEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS,
                    (s32)gDaila_FallenOldManScript, 360, 0, 344, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND_SMALL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 568, 0, 424, FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR_SMALL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 248, 0, 456, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT_SMALL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 264, 0, 456, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER_NEAR, 376, 0, 472, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 392, 0, 312, FACING_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    136, 0, 184, FACING_SOUTH_SOUTHWEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR_SMALL, CONDITION_ALWAYS,
                    (s32)gDaila_PuddleChildScript, 584, 0, 384, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT_SMALL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 568, 0, 328, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 312, 0, 424, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDailaActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER, 40, 0, 280, FACING_EAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER_NEAR, 264, 0, 232, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER_NEAR, 88, 0, 376, FACING_EAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT_SMALL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 264, 0, 312, FACING_WEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE_SMALL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 360, 0, 344, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND_SMALL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 568, 0, 424, FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR_SMALL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 248, 0, 456, FACING_EAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT_SMALL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 264, 0, 456, FACING_WEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER_NEAR, 376, 0, 472, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 392, 0, 312, FACING_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER_NEAR, 120, 0, 216, FACING_WEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR_SMALL, CONDITION_ALWAYS,
                    (s32)gDaila_PuddleChildScript, 584, 0, 384, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT_SMALL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 584, 0, 344, FACING_WEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 312, 0, 424, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_DAILA_FIRST_PUDDLE_PARCHED,
                    MOVEMENT_TYPE_STAND, 184, 0, 312, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_DAILA_SECOND_PUDDLE_PARCHED,
                    MOVEMENT_TYPE_STAND, 424, 0, 328, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_DAILA_THIRD_PUDDLE_PARCHED,
                    MOVEMENT_TYPE_STAND, 472, 0, 376, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_DAILA_FOURTH_PUDDLE_PARCHED,
                    MOVEMENT_TYPE_STAND, 296, 0, 184, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_DAILA_FIFTH_PUDDLE_PARCHED,
                    MOVEMENT_TYPE_STAND, 504, 0, 280, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_DAILA_SIXTH_PUDDLE_PARCHED,
                    MOVEMENT_TYPE_STAND, 280, 0, 376, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gDailaFloodedTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 6, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, DAILA_FLOODED_TO_DAILA_ROOMS_SANCTUM},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, DAILA_FLOODED_TO_WORLD_MAP_SOUTH},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, DAILA_FLOODED_TO_WORLD_MAP_NORTH},
    {TRIGGER_TALK, ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE, CONDITION_ALWAYS,
     EVENT(Daila_TalkBeenBefore)},
    {TRIGGER_TALK, ACTOR_WOMAN_WHO_FELT_INDRA_MOVE, CONDITION_ALWAYS,
     EVENT(Daila_TalkIndraMoved)},
    {TRIGGER_TALK, ACTOR_MAN_WHO_SAW_A_LIGHT, CONDITION_ALWAYS,
     MSGNO(MsgFieldISawABrightLightIn)},
    {TRIGGER_TALK, ACTOR_VILLAGER_THANKFUL_FOR_THE_MOUNTAINS, CONDITION_ALWAYS,
     MSGNO(MsgFieldThankGoodnessForThatMountainRange)},
    {TRIGGER_TALK, ACTOR_FALLEN_OLD_MAN, CONDITION_ALWAYS,
     MSGNO(MsgFieldOhhhThatWaveKnockedMeOver)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_SHRINE, CONDITION_ALWAYS,
     MSGNO(MsgFieldThatAncientTowerToTheEast)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WAITING_UP_FOR_RIKI_AND_TAVI,
     CONDITION_ALWAYS, MSGNO(MsgFieldRikiAndTaviSureAreOut)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_RIKI_AND_TAVI, CONDITION_ALWAYS,
     MSGNO(MsgFieldIWonderIfThoseTwoAre)},
    {TRIGGER_TALK, ACTOR_SAILOR_WITHOUT_A_BOAT, CONDITION_ALWAYS,
     MSGNO(MsgFieldOurVillageSurvivedTheWaveBut)},
    {TRIGGER_TALK, ACTOR_SAILOR_ON_THE_BOATS, CONDITION_ALWAYS,
     EVENT(Daila_TalkBoat)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_STANDING_WATER, CONDITION_ALWAYS,
     MSGNO(MsgFieldThatTidalWaveLeftALot)},
    {TRIGGER_TALK, ACTOR_CHILD_WHO_LOVES_PUDDLES, CONDITION_ALWAYS,
     MSGNO(MsgFieldYaaayPuddlesInTheVillageYaaay)},
    {TRIGGER_TALK, ACTOR_CHILD_WHO_FELL_IN_A_PUDDLE, CONDITION_ALWAYS,
     MSGNO(MsgFieldBleahIFellInAPuddle)},
    {TRIGGER_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(Daila_AskAfterTaviAndRiki)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE,
     CONDITION_ALWAYS, MSGNO(MsgFieldThatWaveTurnedThisPlaceInto)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WHO_FELT_INDRA_MOVE,
     CONDITION_ALWAYS, MSGNO(MsgFieldThatMustHaveBeenOneSerious)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_SAW_A_LIGHT,
     CONDITION_ALWAYS, MSGNO(MsgFieldIThoughtISawSomeLights)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_THANKFUL_FOR_THE_MOUNTAINS, CONDITION_ALWAYS,
     MSGNO(MsgFieldTheSeaGodMustHaveSaved)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_OLD_MAN,
     CONDITION_ALWAYS, MSGNO(MsgFieldMyHipsKillingMeAndAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_SHRINE,
     CONDITION_ALWAYS, MSGNO(MsgFieldICantBelieveThoseChildrenPlay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_WAITING_UP_FOR_RIKI_AND_TAVI, CONDITION_ALWAYS,
     MSGNO(MsgFieldTheShrineOfTheSeaGod)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_RIKI_AND_TAVI,
     CONDITION_ALWAYS, MSGNO(MsgFieldICantBelieveThoseLosersWere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WITHOUT_A_BOAT,
     CONDITION_ALWAYS, MSGNO(MsgFieldThatWaveWasBrutalItSmashed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_BOATS,
     CONDITION_ALWAYS, MSGNO(MsgFieldWithAllTheBoatsGoneWe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STANDING_WATER,
     CONDITION_ALWAYS, MSGNO(MsgFieldWithAllThisSaltWaterOn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_WHO_LOVES_PUDDLES,
     CONDITION_ALWAYS, MSGNO(MsgFieldYayWeNeverGetPuddlesLike)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_WHO_FELL_IN_A_PUDDLE,
     CONDITION_ALWAYS, MSGNO(MsgFieldBleccchThisWatersSaaalty)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgFieldImSureThoseTwoWentTo)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgDailaWellSeawater))},
    {TRIGGER_TABLE_END},
};

struct Trigger gDailaTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 6, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(Daila_MeetAlex)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 30, CONDITION_ALWAYS,
     EVENT(Daila_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS,
     EVENT(Daila_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS,
     EVENT(Daila_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 33, CONDITION_ALWAYS,
     EVENT(Daila_JumpForward)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE, CONDITION_ALWAYS,
     MSGNO(MsgFieldAfterThatWaveHitIWas)},
    {TRIGGER_TALK, ACTOR_WOMAN_WHO_FELT_INDRA_MOVE, CONDITION_ALWAYS,
     MSGNO(MsgFieldSureAreALotOfTravelers)},
    {TRIGGER_TALK, ACTOR_MAN_WHO_SAW_A_LIGHT, CONDITION_ALWAYS,
     MSGNO(MsgFieldThosePuddlesOfSeaWaterThe)},
    {TRIGGER_TALK, ACTOR_VILLAGER_THANKFUL_FOR_THE_MOUNTAINS, CONDITION_ALWAYS,
     MSGNO(MsgFieldAhTheSeaIsCalmAgain)},
    {TRIGGER_TALK, ACTOR_FALLEN_OLD_MAN, CONDITION_ALWAYS,
     EVENT(Daila_TalkHip)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_SHRINE, CONDITION_ALWAYS,
     MSGNO(MsgFieldIveAlwaysHeardThatTheShrine)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WAITING_UP_FOR_RIKI_AND_TAVI,
     CONDITION_ALWAYS, MSGNO(MsgFieldTheDreadPirateBriggsWasCaptured)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_RIKI_AND_TAVI, CONDITION_ALWAYS,
     MSGNO(MsgFieldThoseTwoLosersAreBackHome)},
    {TRIGGER_TALK, ACTOR_SAILOR_WITHOUT_A_BOAT, CONDITION_ALWAYS,
     MSGNO(MsgFieldIThoughtISawSomethingWhen)},
    {TRIGGER_TALK, ACTOR_SAILOR_ON_THE_BOATS, CONDITION_ALWAYS,
     MSGNO(MsgFieldSoIHearTheyCaughtThe)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_STANDING_WATER, CONDITION_ALWAYS,
     MSGNO(MsgFieldItWasCreepyRightBeforeThe)},
    {TRIGGER_TALK, ACTOR_CHILD_WHO_LOVES_PUDDLES, CONDITION_ALWAYS,
     MSGNO(MsgFieldThePuddlesAreGoneAndNow)},
    {TRIGGER_TALK, ACTOR_CHILD_WHO_FELL_IN_A_PUDDLE, CONDITION_ALWAYS,
     MSGNO(MsgFieldIGotHurtWhenIWas)},
    {TRIGGER_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(Daila_TaviAndRikiAreHome)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKING_IF_BEEN_BEFORE,
     CONDITION_ALWAYS, MSGNO(MsgFieldThankOurLuckyStarsOurLittle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WHO_FELT_INDRA_MOVE,
     CONDITION_ALWAYS, MSGNO(MsgFieldIsThatGuyStillMeetingWith)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_SAW_A_LIGHT,
     CONDITION_ALWAYS, MSGNO(MsgFieldThosePuddlesStankIThoughtId)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_THANKFUL_FOR_THE_MOUNTAINS, CONDITION_ALWAYS,
     MSGNO(MsgFieldImGladEverythingIsBackTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_OLD_MAN,
     CONDITION_ALWAYS, MSGNO(MsgFieldNobodyBelievesAWaveCouldBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_SHRINE,
     CONDITION_ALWAYS, MSGNO(MsgFieldIdSureLikeToSeeWhat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_WAITING_UP_FOR_RIKI_AND_TAVI, CONDITION_ALWAYS,
     MSGNO(MsgFieldIWonderWhatHappenedToBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_RIKI_AND_TAVI,
     CONDITION_ALWAYS, MSGNO(MsgFieldTaviAndRikiMustHaveBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WITHOUT_A_BOAT,
     CONDITION_ALWAYS, MSGNO(MsgFieldItsProbablyNothingButJunkAnd)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_ON_THE_BOATS,
     CONDITION_ALWAYS, MSGNO(MsgFieldTheSeasArePeacefulAndFree)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STANDING_WATER,
     CONDITION_ALWAYS, MSGNO(MsgFieldBeforeTheWaveHitTheTide)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_WHO_LOVES_PUDDLES,
     CONDITION_ALWAYS, MSGNO(MsgFieldIHopeSomeOfTheLittle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_WHO_FELL_IN_A_PUDDLE,
     CONDITION_ALWAYS, MSGNO(MsgFieldICaughtMyFootOnSomething)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgDailaTaviRikiPunishment)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 60,
     FLAG_DAILA_FIRST_PUDDLE_PARCHED, EVENT(Daila_ParchFirstPuddle)},
// BUG: cells 61 to 63 lie under the fourth, second and third puddles but take
// the flags of the second, third and fourth, so drying the second puddle
// turns off Parch on the fourth, the third the second's, and the fourth the
// third's, for the rest of the visit.
#if BUGFIX
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 61,
     FLAG_DAILA_FOURTH_PUDDLE_PARCHED, EVENT(Daila_ParchFourthPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 62,
     FLAG_DAILA_SECOND_PUDDLE_PARCHED, EVENT(Daila_ParchSecondPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 63,
     FLAG_DAILA_THIRD_PUDDLE_PARCHED, EVENT(Daila_ParchThirdPuddle)},
#else
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 61,
     FLAG_DAILA_SECOND_PUDDLE_PARCHED, EVENT(Daila_ParchFourthPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 62,
     FLAG_DAILA_THIRD_PUDDLE_PARCHED, EVENT(Daila_ParchSecondPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 63,
     FLAG_DAILA_FOURTH_PUDDLE_PARCHED, EVENT(Daila_ParchThirdPuddle)},
#endif
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 64,
     FLAG_DAILA_FIFTH_PUDDLE_PARCHED, EVENT(Daila_ParchFifthPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 65,
     FLAG_DAILA_SIXTH_PUDDLE_PARCHED, EVENT(Daila_ParchSixthPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE,
     FLAG_DAILA_PUDDLE_FROZEN_FIRST + 0, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE,
     FLAG_DAILA_PUDDLE_FROZEN_FIRST + 1, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PUDDLE,
     FLAG_DAILA_PUDDLE_FROZEN_FIRST + 2, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FOURTH_PUDDLE,
     FLAG_DAILA_PUDDLE_FROZEN_FIRST + 3, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIFTH_PUDDLE,
     FLAG_DAILA_PUDDLE_FROZEN_FIRST + 4, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIXTH_PUDDLE,
     FLAG_DAILA_PUDDLE_FROZEN_FIRST + 5, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 100,
     FLAG_DAILA_PSY_CRYSTAL_DUG, EVENT(Daila_DigUpPsyCrystal)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgDailaWellSeawater))},
    {TRIGGER_TABLE_END},
};
