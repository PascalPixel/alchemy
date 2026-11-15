/// @file
/// Naribwe, the village south of Kibombo where a fortune teller reads the
/// party's items, Whirlwind clears the ivy over the backyard ladder and Reveal
/// shows the stone that hides the Unicorn Ring.

#include "types.h"
#include "sprite_ids.h"
#include "field/field_work.h"
#include "flags.h"

/// The villager has asked where the party comes from.
#define FLAG_ASKED_WHERE_FROM 0x85e

/// The villager has asked about the Kibombo expedition.
#define FLAG_ASKED_ABOUT_EXPEDITION 0x85d
#include "text/msg_ids.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/scene_tables.h"

/// Naribwe, by the rows of the village's placements (the same before and
/// after the return from Kibombo); the last row goes to Isaac's party slot.
enum NaribweActorId {
  ACTOR_BONFIRE = ACTOR_FIRST_PLACED,
  ACTOR_WEST_BRAZIER,
  ACTOR_EAST_BRAZIER,
  ACTOR_WEST_BRAZIER_FLAME,
  ACTOR_EAST_BRAZIER_FLAME,
  /// "Don't tell me you just came back from Kibombo!"
  ACTOR_MAN_AMAZED_BY_THE_RETURN,
  /// "It's always hot here in Naribwe."
  ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION,
  /// "I would not venture far north of here if I were you..."
  ACTOR_MAN_WARNING_OF_THE_NORTH,
  ACTOR_OLD_WOMAN,
  ACTOR_OLD_MAN,
  /// "Can you tell me where you have come from?"
  ACTOR_WOMAN_ASKING_WHERE_FROM,
  ACTOR_BOY_ASKING_OF_KIBOMBO, ///< Naribwe_AskGoingToKibombo
  /// "On the far side of the river lies a huge mountain we call Magma Rock."
  ACTOR_VILLAGER_ON_MAGMA_ROCK,
  /// "Ouch... Some big brute came through here looking for a woman named
  /// Menardi."
  ACTOR_OLD_MAN_HURT_BY_A_BRUTE,
  /// "No one can see the future. Absolutely no one. Except for our..."
  ACTOR_GIRL_PRAISING_THE_FORTUNE_TELLER,
  ACTOR_IVY,
  /// The two posts of the rope Lash ties (Rope_Place).
  ACTOR_ROPE_POST_A,
  ACTOR_ROPE_POST_B,
  /// "I am but a humble pilgrim..."
  ACTOR_PILGRIM,
  ACTOR_STONE ///< Naribwe_HideStone, Naribwe_RevealStone
};
#include "field/event_inline.h"
#include "field/field.h"
#include "shared/tables.h"
#include "field/sprite.h"
#include "field/actor_script.h"
#include "fixed_math.h"

extern u32 gSceneExits[];

struct WarpEntry {
  const void *script;
  u16 x;
  u16 z;
};

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_End(void);
void Actor_DisableById(s32 id);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void Shop_Run(s32 a, s32 b);
void Sanctum_Run(s32 a);
void Naribwe_HideStone(void);

extern struct ActorPlacement gNaribwe_Placements[];
extern struct ActorPlacement gNaribwe_Entrance10Placements[];
extern struct ActorPlacement gNaribwe_PlacementsOnReturn[];
extern struct Trigger gNaribwe_Triggers[];
extern struct Trigger gNaribwe_TriggersOnReturn[];
extern struct WarpEntry gNaribwe_Doors[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const u8 *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.entrance == 10)
    return gNaribwe_Entrance10Placements;
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    return gNaribwe_PlacementsOnReturn;
  return gNaribwe_Placements;
}

/// Before Blaze is cast on the central fire, the event work notes whether the
/// party came in by entrance 10.
void Naribwe_PrepareBlaze(void) {
  u8 *work = Ram_HeapSlots->fieldWork;
  u8 *flag;

  flag = work + 0xd38;
  *flag = 1;
  if (gMainRoutineWork.entrance != 10)
    *flag = 0;
}

const struct Trigger *Scene_GetTriggers(void) {
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    return gNaribwe_TriggersOnReturn;
  return gNaribwe_Triggers;
}

/// "The Kibombo went on an expedition to the east, but they've already
/// returned." Asked once (FLAG_ASKED_ABOUT_EXPEDITION); afterwards the villager
/// talks of Akafubu.
void Naribwe_AskAboutKibomboExpedition(void) {
  s32 msg;

  if (Event_GetFlag(FLAG_ASKED_ABOUT_EXPEDITION) == 0) {
    msg = (s32)MsgNaribweKibomboWentOnExpedition;
    Event_SetMessageId(msg);
    // "The Kibombo went on an expedition to the east, but they've already..."
    Event_OpenMessage(ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(msg + 1);
      Event_SetFlag(FLAG_ASKED_ABOUT_EXPEDITION);
    } else {
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION, 0);
  } else {
    Event_SetMessageId((s32)MsgNaribweKibomboNeverMoveInSuch);
    // "The Kibombo never move in such large numbers without orders from..."
    Event_ShowMessage(ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION, 0);
  }
}

/// "Can you tell me where you have come from?" Asked once
/// (FLAG_ASKED_WHERE_FROM).
void Naribwe_AskWhereFrom(void) {
  s32 msg;

  if (Event_GetFlag(FLAG_ASKED_WHERE_FROM) == 0) {
    msg = (s32)MsgNaribweCanYouTellMeWhere;
    Event_SetMessageId(msg);
    // "Can you tell me where you have come from?"
    Event_OpenMessage(ACTOR_WOMAN_ASKING_WHERE_FROM, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(msg + 1);
      Event_SetFlag(FLAG_ASKED_ABOUT_EXPEDITION);
    } else {
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(ACTOR_WOMAN_ASKING_WHERE_FROM, 0);
  } else {
    Event_SetMessageId((s32)MsgNaribweItsAlwaysHotHere);
    // "It's always hot here in Naribwe. It doesn't matter if it's summer..."
    Event_ShowMessage(ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION, 0);
  }
}

/// "You don't plan on going to Kibombo, do you, mister?"
void Naribwe_AskGoingToKibombo(void) {
  s32 msg = (s32)MsgNaribweYouDontPlanOnGoing;

  Event_SetMessageId(msg);
  Event_OpenMessage(ACTOR_BOY_ASKING_OF_KIBOMBO, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(ACTOR_BOY_ASKING_OF_KIBOMBO, 0);
}

/// Naribwe's shops, as Shop_Run numbers them.
enum { SHOP_NARIBWE_ITEMS = 16 };

/// "No one can see the future. Absolutely no one. Except for our
/// fortune-teller."
void Naribwe_PraiseFortuneTeller(s32 actor) {
  if (FACING_NEAREST_CARDINAL(
          Actor_GetById(gMainRoutineWork.leaderActorId)->facing) ==
      FACING_WEST) {
    Shop_Run(SHOP_NARIBWE_ITEMS, actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNaribweNoOneCanSeeFuture);
  // "No one can see the future. Absolutely no one. Except for our..."
  Event_ShowMessage(actor, 0);
  Event_End();
}

void Naribwe_PraiseFortuneTellerOnReturn(s32 actor) {
  if (FACING_NEAREST_CARDINAL(
          Actor_GetById(gMainRoutineWork.leaderActorId)->facing) ==
      FACING_WEST) {
    Shop_Run(SHOP_NARIBWE_ITEMS, actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNaribweStrangersToOurTownWould);
  // "Strangers to our town would do well to visit the fortune-teller's hut."
  Event_ShowMessage(actor, 0);
  Event_End();
}

/// "I am but a humble pilgrim, serving the greater good in the heart of
/// Gondowan."
void Naribwe_TalkToPilgrim(s32 actor) {
  if (FACING_NEAREST_CARDINAL(
          Actor_GetById(gMainRoutineWork.leaderActorId)->facing) ==
      FACING_NORTH) {
    Sanctum_Run(actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNaribweIAmButHumblePilgrim);
  // "I am but a humble pilgrim, serving the greater good in the heart of..."
  Event_ShowMessage(ACTOR_PILGRIM, 0);
  Event_End();
}

void Naribwe_TalkToPilgrimOnReturn(s32 actor) {
  if (FACING_NEAREST_CARDINAL(
          Actor_GetById(gMainRoutineWork.leaderActorId)->facing) ==
      FACING_NORTH) {
    Sanctum_Run(actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNaribweItsStrangeAsHot);
  // "It's strange. As hot as it is here in Gondowan, if you head south..."
  Event_ShowMessage(ACTOR_PILGRIM, 0);
  Event_End();
}

/// The party leader walks into one of Naribwe's doors, which opens with its
/// cell animation, and leaves by the exit of the door's number.
void Naribwe_EnterDoor(void) {
  u8 *work = Ram_HeapSlots->fieldWork;
  s32 index;
  u32 id;

  Event_Start();
  Event_ResolvePendingActions(0);
  for (id = 8; id <= 63; id++) {
    u8 *actor = (u8 *)Actor_GetById(id);

    if (actor != NULL) {
      u8 *flags = &actor[85];
      s32 zero = 0;

      *flags = zero;
    }
  }
  work += 0x154;
  index = *(s16 *)work;
  Event_PlaySound(SE_DOOR_OPEN);
  index -= 4;
  MapGrid_AnimateCells(gNaribwe_Doors[index].script, gNaribwe_Doors[index].x,
                       gNaribwe_Doors[index].z);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Import_EventSetExitRequest(*(s16 *)work);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Whirlwind blows away the ivy in the back yard, uncovering the ladder (flag
/// 0x201).
void Naribwe_BlowAwayIvy(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetFlag(FLAG_SCENE_TEMP_01);
  Event_End();
}

/// Unused: writes the rope's posts (24 and 25) into the event work; nothing
/// calls it.
UNUSED void Naribwe_SetRopeWork(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->targetX = 0;
  work->targetY = 24;
  work->targetZ = 25;
}

/// Reveal shows the stone that hides the Unicorn Ring.
void Naribwe_RevealStone(void) {
  Actor_SetPosition(ACTOR_STONE, 0, 0);
  TreasureSpot_SetPosition(65, px(104), px(104));
}

void Naribwe_HideStone(void) {
  TreasureSpot_SetActorPosition(65, 0, 0);
  Actor_SetPosition(ACTOR_STONE, 104, 104);
}

s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_WEST_BRAZIER), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_EAST_BRAZIER), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_IVY), 0);
  Actor_GetById(ACTOR_IVY)->scaleY = 0x19999;
  Actor_GetById(ACTOR_STONE)->scaleX = Q16_ONE / 2;
  Actor_GetById(ACTOR_STONE)->scaleY = Q16_ONE / 2;
  Naribwe_HideStone();
  if (Event_GetFlag(FLAG_SCENE_TEMP_01))
    Actor_SetPosition(ACTOR_IVY, 0, 0);
  Rope_Initialize();
  Rope_Place(0, 0x202, ACTOR_ROPE_POST_A, ACTOR_ROPE_POST_B);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROPE_POST_A), 0);
  Actor_DisableById(ACTOR_ROPE_POST_B);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// Naribwe's data: its scripts, door, placements and triggers.

#include "text/msg_ids.h"

void Naribwe_PrepareBlaze(void);
void Naribwe_AskAboutKibomboExpedition(void);

void Naribwe_AskWhereFrom(void);

void Naribwe_AskGoingToKibombo(void);

void Naribwe_EnterDoor(void);

void Naribwe_BlowAwayIvy(void);

void Naribwe_RevealStone(void);

extern u32 gNaribwe_BrazierBurns[];
extern u32 gNaribwe_FlameRises[];

extern u32 gNaribwe_IvyAnimation[];
extern s16 gNaribwe_DoorOpening[];

void Naribwe_HideStone(void);

/// The village's placements, the same before and after the return with Piers.
#define DEFINE_NARIBWE_PLACEMENTS(name)                                        \
  struct ActorPlacement name[] = {                                             \
      ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS,                    \
                      MOVEMENT_TYPE_STAND, 232, 0, 246, FACING_SOUTH,          \
                      TALK_FACING_PARTY, 0),                                   \
      {304, CONDITION_ALWAYS, (s32)gNaribwe_BrazierBurns, px(316), 0, px(132), \
       FACING_SOUTH, 0, 0},                                                    \
      {304, CONDITION_ALWAYS, (s32)gNaribwe_BrazierBurns, px(372), 0, px(132), \
       FACING_SOUTH, 0, 0},                                                    \
      {410, CONDITION_ALWAYS, (s32)gNaribwe_FlameRises, px(316), 0, px(132),   \
       FACING_SOUTH, 0, 0},                                                    \
      {410, CONDITION_ALWAYS, (s32)gNaribwe_FlameRises, px(372), 0, px(132),   \
       FACING_SOUTH, 0, 0},                                                    \
      ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,                \
                      MOVEMENT_TYPE_STAND, 248, 0, 344, FACING_SOUTH,          \
                      TALK_FACING_PARTY_AND_BACK, 0),                          \
      ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 200,   \
                      0, 296, FACING_EAST, TALK_FACING_PARTY, 0),              \
      ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 152, \
                      FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),            \
      ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 88, 0, \
                      376, FACING_NORTHEAST, 0, 0),                            \
      ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 88,  \
                      FACING_SOUTHEAST, 1, 0),                                 \
      ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 224, \
                      FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),        \
      ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS,            \
                      MOVEMENT_TYPE_WANDER, 200, 0, 200, FACING_NORTHEAST,     \
                      TALK_FACING_PARTY, 0),                                   \
      ACTOR_PLACEMENT(147, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 232, \
                      FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),        \
      ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 56, 0, 232, \
                      FACING_EAST, TALK_FACING_PARTY, 0),                      \
      ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS,       \
                      MOVEMENT_TYPE_STAND, 168, 0, 360, FACING_EAST,           \
                      TALK_FACING_PARTY_AND_BACK, 0),                          \
      {307, CONDITION_ALWAYS, (s32)gNaribwe_IvyAnimation, px(40), 0, px(168),  \
       FACING_SOUTH, 2, 0},                                                    \
      ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS,                 \
                      MOVEMENT_TYPE_STAND, 360, 0, 376, FACING_SOUTH,          \
                      TALK_FACING_KEEP, 0),                                    \
      ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 376, \
                      FACING_SOUTH, TALK_FACING_KEEP, 0),                      \
      ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,    \
                      376, 0, 168, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK,   \
                      0),                                                      \
      ACTOR_PLACEMENT(SPRITE_ROCK_PILE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, \
                      104, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK,   \
                      0),                                                      \
      {SCENE_TABLE_END},                                                       \
  };

// clang-format off
// Unused: a list of actor 26 and flag 0x200; nothing reads it.
s16 gNaribwe_UnusedList[] = {
    26, 512, -1, 0,
};

/// The braziers burn, the fire's flames rising above them.
u32 gNaribwe_BrazierBurns[] = {
    SC_ANIMATION(2),
    SC_END,
};

u32 gNaribwe_FlameRises[] = {
    SC_ANIMATION(0),
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, px(24)),
    SC_END,
};

u32 gNaribwe_IvyAnimation[] = {
    SC_ANIMATION(3),
    SC_END,
};

/// The cell animation of the village's doors.
s16 gNaribwe_DoorOpening[] = {
    8, 42, 1, 2, 6, -1,
};

/// The village's doors that open with a cell animation as the party walks
/// through.
struct WarpEntry gNaribwe_Doors[] = {
    {gNaribwe_DoorOpening, 18, 51},
    {gNaribwe_DoorOpening, 21, 34},
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 224, 0, 190, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {                                                        
      SCENE_EXITS(Naribwe),                                                         
      SCENE_EXIT_UNLESS(1, NaribweInteriors, 1, EXIT_FLAG_NONE),                                 
      SCENE_EXIT_UNLESS(2, NaribweInteriors, 2, EXIT_FLAG_NONE),                                 
      SCENE_EXIT_UNLESS(3, NaribweInteriors, 3, EXIT_FLAG_NONE),                                 
      SCENE_EXIT_UNLESS(4, NaribweInteriors, 4, EXIT_FLAG_NONE),                                 
      SCENE_EXIT_UNLESS(5, NaribweInteriors, 5, EXIT_FLAG_NONE),                                 
      SCENE_EXIT_UNLESS(6, WorldMap, 23, EXIT_FLAG_NONE),                                
      SCENE_EXIT_UNLESS(7, WorldMap, 24, EXIT_FLAG_NONE),                                
      SCENE_EXITS_END,                                                         
  };

DEFINE_NARIBWE_PLACEMENTS(gNaribwe_Placements)

struct ActorPlacement gNaribwe_Entrance10Placements[] = {
    ACTOR_PLACEMENT(SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 246, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 316, 0, 132, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 372, 0, 132, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 316, 0, 132, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 372, 0, 132, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};
// clang-format on

DEFINE_NARIBWE_PLACEMENTS(gNaribwe_PlacementsOnReturn)

struct Trigger gNaribwe_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
     EVENT(Naribwe_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
     EVENT(Naribwe_EnterDoor)},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_MAN_AMAZED_BY_THE_RETURN, CONDITION_ALWAYS,
     MSGNO(MsgNaribweWelcomeToNaribweItsReally)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION, CONDITION_ALWAYS,
     EVENT(Naribwe_AskAboutKibomboExpedition)},
    {TRIGGER_TALK, ACTOR_MAN_WARNING_OF_THE_NORTH, CONDITION_ALWAYS,
     MSGNO(MsgNaribweIWouldNotVentureFar)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS,
     MSGNO(MsgNaribweItsBeenDangerousOutEver)},
    {TRIGGER_TALK, ACTOR_OLD_MAN, CONDITION_ALWAYS,
     MSGNO(MsgNaribweWhenOeiaWasTheirWitch)},
    {TRIGGER_TALK, ACTOR_WOMAN_ASKING_WHERE_FROM, CONDITION_ALWAYS,
     EVENT(Naribwe_AskWhereFrom)},
    {TRIGGER_TALK, ACTOR_BOY_ASKING_OF_KIBOMBO, CONDITION_ALWAYS,
     EVENT(Naribwe_AskGoingToKibombo)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_MAGMA_ROCK, CONDITION_ALWAYS,
     MSGNO(MsgNaribweOnFarSideOfRiver)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_HURT_BY_A_BRUTE, CONDITION_ALWAYS,
     MSGNO(MsgNaribweWhenIClimbedKibomboMountains)},
    {TRIGGER_TALK, ACTOR_GIRL_PRAISING_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
     EVENT(Naribwe_PraiseFortuneTeller)},
    {TRIGGER_TALK, ACTOR_PILGRIM, CONDITION_ALWAYS,
     EVENT(Naribwe_TalkToPilgrim)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AMAZED_BY_THE_RETURN,
     CONDITION_ALWAYS, MSGNO(MsgNaribweICanHardlyStandHeat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION, CONDITION_ALWAYS,
     MSGNO(MsgNaribweAkafubuIsAlmostCertainlyLooking)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WARNING_OF_THE_NORTH,
     CONDITION_ALWAYS, MSGNO(MsgNaribweItIsAwfulToNorth)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS,
     MSGNO(MsgNaribweThatWasDayWindBlew)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN, CONDITION_ALWAYS,
     MSGNO(MsgNaribweKibomboMagicWasOnceWidely)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_WHERE_FROM,
     CONDITION_ALWAYS, MSGNO(MsgNaribweWarriorWhoPassedThroughHere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ASKING_OF_KIBOMBO,
     CONDITION_ALWAYS, MSGNO(MsgNaribweToGetToKibombo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_MAGMA_ROCK,
     CONDITION_ALWAYS, MSGNO(MsgNaribweSometimesYouCanHearExplosions)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_HURT_BY_A_BRUTE,
     CONDITION_ALWAYS, MSGNO(MsgNaribweIRememberWhenIFound)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_GIRL_PRAISING_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweEvenIfIWanted)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PILGRIM, CONDITION_ALWAYS,
     MSGNO(MsgNaribweAnywhereICanDoSome)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgNaribweTheresOnlyLittleBit))},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(Naribwe_PrepareBlaze)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY, 0x201,
     EVENT(Naribwe_BlowAwayIvy)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST_A, 0x202, 0},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Naribwe_RevealStone)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Naribwe_HideStone)},
    {TRIGGER_TABLE_END},
};

struct Trigger gNaribwe_TriggersOnReturn[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
     EVENT(Naribwe_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
     EVENT(Naribwe_EnterDoor)},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_MAN_AMAZED_BY_THE_RETURN, CONDITION_ALWAYS,
     MSGNO(MsgNaribweDontTellMeYouJust)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION, CONDITION_ALWAYS,
     MSGNO(MsgNaribweIDidntSeeAnyKibombo)},
    {TRIGGER_TALK, ACTOR_MAN_WARNING_OF_THE_NORTH, CONDITION_ALWAYS,
     MSGNO(MsgNaribweISawSmokeRising)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS,
     MSGNO(MsgNaribweSomeSoldiersCameThroughHere)},
    {TRIGGER_TALK, ACTOR_OLD_MAN, CONDITION_ALWAYS,
     MSGNO(MsgNaribweSmokeComingFromKibomboMountains)},
    {TRIGGER_TALK, ACTOR_WOMAN_ASKING_WHERE_FROM, CONDITION_ALWAYS,
     MSGNO(MsgNaribweFortuneTellerPredictedThatKibombo)},
    {TRIGGER_TALK, ACTOR_BOY_ASKING_OF_KIBOMBO, CONDITION_ALWAYS,
     MSGNO(MsgNaribweManCameThroughWhileAgo)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_MAGMA_ROCK, CONDITION_ALWAYS,
     MSGNO(MsgNaribweIMentionedMagmaRock)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_HURT_BY_A_BRUTE, CONDITION_ALWAYS,
     MSGNO(MsgNaribweOuchSomeBigBruteCame)},
    {TRIGGER_TALK, ACTOR_GIRL_PRAISING_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
     EVENT(Naribwe_PraiseFortuneTellerOnReturn)},
    {TRIGGER_TALK, ACTOR_PILGRIM, CONDITION_ALWAYS,
     EVENT(Naribwe_TalkToPilgrimOnReturn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AMAZED_BY_THE_RETURN,
     CONDITION_ALWAYS, MSGNO(MsgNaribweWhoWouldTravelToKibombo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_WOMAN_ON_THE_KIBOMBO_EXPEDITION, CONDITION_ALWAYS,
     MSGNO(MsgNaribweImSoGladKibomboArent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WARNING_OF_THE_NORTH,
     CONDITION_ALWAYS, MSGNO(MsgNaribweThatSmokesBeenRising)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS,
     MSGNO(MsgNaribweWhatKindOfName)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN, CONDITION_ALWAYS,
     MSGNO(MsgNaribweSignalsSaySomethingHasEnded)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_WHERE_FROM,
     CONDITION_ALWAYS, MSGNO(MsgNaribweFortuneTellerHasNeverBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ASKING_OF_KIBOMBO,
     CONDITION_ALWAYS, MSGNO(MsgNaribweIfManTheyWereLooking)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_MAGMA_ROCK,
     CONDITION_ALWAYS, MSGNO(MsgNaribweIDontThinkAnyoneCan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_HURT_BY_A_BRUTE,
     CONDITION_ALWAYS, MSGNO(MsgNaribweWhatHasGoneWrong)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_GIRL_PRAISING_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweThoseWhoBecomeLostShould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PILGRIM, CONDITION_ALWAYS,
     MSGNO(MsgNaribweColdOfTundariaStiflesVirtually)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgNaribweTheresOnlyLittleBit))},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(Naribwe_PrepareBlaze)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY, 0x201,
     EVENT(Naribwe_BlowAwayIvy)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST_A, 0x202, 0},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Naribwe_RevealStone)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Naribwe_HideStone)},
    {TRIGGER_TABLE_END},
};
