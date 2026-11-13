/// @file
/// Naribwe, the village south of Kibombo where a fortune teller reads the
/// party's items, Whirlwind clears the ivy over the backyard ladder and Reveal
/// shows the stone that hides the Unicorn Ring.

#include "types.h"
#include "flags.h"
#include "text/msg_ids.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_tables.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "shared/tables.h"

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
/* VIEW(first): routine gNaribwe_Doors as this part declared it; the joined file
 * has another; tried as the one declaration extern const struct WarpEntry
 * gNaribwe_Doors[], it does not compile in tla-ja:
 * games/tla/src/scene/naribwe.c:211: `gNaribwe_Doors' undeclared (first use in
 * this function). */
extern const struct WarpEntry gNaribwe_Doors__first[] __asm__("gNaribwe_Doors");
#define gNaribwe_Doors gNaribwe_Doors__first

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const u8 *Scene_GetPlacements(void) {
  if (gMainRoutineWork.entrance == 10)
    return gNaribwe_Entrance10Placements;
  if (Event_GetFlag(0x8ff))
    return gNaribwe_PlacementsOnReturn;
  return gNaribwe_Placements;
}

/// Before Blaze is cast on the central fire, the event work notes whether the
/// party came in by entrance 10.
void Naribwe_PrepareBlaze(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  u8 *flag;

  flag = work + 0xd38;
  *flag = 1;
  if (gMainRoutineWork.entrance != 10)
    *flag = 0;
}

const struct Trigger *Scene_GetEvents(void) {
  if (Event_GetFlag(0x8ff))
    return gNaribwe_TriggersOnReturn;
  return gNaribwe_Triggers;
}

/// "The Kibombo went on an expedition to the east, but they've already
/// returned." Asked once (flag 0x85d); afterwards the villager talks of
/// Akafubu.
void Naribwe_AskAboutKibomboExpedition(void) {
  s32 msg;

  if (Event_GetFlag(0x85d) == 0) {
    msg = (s32)MsgNaribwe02;
    Event_SetMessageId(msg);
    Event_OpenMessage(14, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(msg + 1);
      Event_SetFlag(0x85d);
    } else {
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(14, 0);
  } else {
    Event_SetMessageId((s32)MsgNaribwe03);
    Event_ShowMessage(14, 0);
  }
}

/// "Can you tell me where you have come from?" Asked once (flag 0x85e).
void Naribwe_AskWhereFrom(void) {
  s32 msg;

  if (Event_GetFlag(0x85e) == 0) {
    msg = (s32)MsgNaribwe07;
    Event_SetMessageId(msg);
    Event_OpenMessage(18, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(msg + 1);
      Event_SetFlag(0x85d);
    } else {
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessage(18, 0);
  } else {
    Event_SetMessageId((s32)MsgNaribwe08);
    Event_ShowMessage(14, 0);
  }
}

/// "You don't plan on going to Kibombo, do you, mister?"
void Naribwe_AskGoingToKibombo(void) {
  s32 msg = (s32)MsgNaribwe09;

  Event_SetMessageId(msg);
  Event_OpenMessage(19, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(19, 0);
}

/// "No one can see the future. Absolutely no one. Except for our
/// fortune-teller."
void Naribwe_PraiseFortuneTeller(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0x8000) {
    Shop_Run(16, actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNaribwe21);
  Event_ShowMessage(actor, 0);
  Event_End();
}

void Naribwe_PraiseFortuneTellerOnReturn(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0x8000) {
    Shop_Run(16, actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNaribwe43);
  Event_ShowMessage(actor, 0);
  Event_End();
}

/// "I am but a humble pilgrim, serving the greater good in the heart of
/// Gondowan."
void Naribwe_TalkToPilgrim(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Sanctum_Run(actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNaribwe23);
  Event_ShowMessage(26, 0);
  Event_End();
}

void Naribwe_TalkToPilgrimOnReturn(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Sanctum_Run(actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNaribwe45);
  Event_ShowMessage(26, 0);
  Event_End();
}

/// The party leader walks into one of Naribwe's doors, which opens with its
/// cell animation, and leaves by the exit of the door's number.
void Naribwe_EnterDoor(void) {
  u8 *work = Ram_HeapSlots->eventWork;
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
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
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
  u8 *work = Ram_HeapSlots->eventWork;

  work[0xd31] = 0;
  work[0xd32] = 24;
  work[0xd33] = 25;
}

/// Reveal shows the stone that hides the Unicorn Ring.
void Naribwe_RevealStone(void) {
  Actor_SetPosition(27, 0, 0);
  TreasureSpot_SetPosition(65, 0x680000, 0x680000);
}

void Naribwe_HideStone(void) {
  ObjectEvent_PlaceOnGround(65, 0, 0);
  Actor_SetPosition(27, 104, 104);
}

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Actor_SetSpriteFlags(Actor_GetById(9), 0);
  Actor_SetSpriteFlags(Actor_GetById(10), 0);
  Actor_SetSpriteFlags(Actor_GetById(23), 0);
  Actor_GetById(23)->scaleY = 0x19999;
  Actor_GetById(27)->scaleX = Q16_ONE / 2;
  Actor_GetById(27)->scaleY = Q16_ONE / 2;
  Naribwe_HideStone();
  if (Event_GetFlag(FLAG_SCENE_TEMP_01))
    Actor_SetPosition(23, 0, 0);
  Rope_Initialize();
  Rope_Place(0, 0x202, 24, 25);
  Actor_SetSpriteFlags(Actor_GetById(24), 0);
  Actor_DisableById(25);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#undef gNaribwe_Doors
// @file
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

// The village's placements, the same before and after the return with Piers.
#define DEFINE_NARIBWE_PLACEMENTS(name)                                        \
  struct ActorPlacement name[] = {                                             \
      ACTOR_PLACEMENT(365, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 246, \
                      FACING_SOUTH, TALK_FACING_PARTY, 0),                     \
      {304, CONDITION_ALWAYS, (s32)gNaribwe_BrazierBurns, px(316), 0, px(132), \
       FACING_SOUTH, 0, 0},                                                    \
      {304, CONDITION_ALWAYS, (s32)gNaribwe_BrazierBurns, px(372), 0, px(132), \
       FACING_SOUTH, 0, 0},                                                    \
      {410, CONDITION_ALWAYS, (s32)gNaribwe_FlameRises, px(316), 0, px(132),   \
       FACING_SOUTH, 0, 0},                                                    \
      {410, CONDITION_ALWAYS, (s32)gNaribwe_FlameRises, px(372), 0, px(132),   \
       FACING_SOUTH, 0, 0},                                                    \
      ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 344, \
                      FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),            \
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
      ACTOR_PLACEMENT(136, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 200, 0,     \
                      200, FACING_NORTHEAST, TALK_FACING_PARTY, 0),            \
      ACTOR_PLACEMENT(147, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 232, \
                      FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),        \
      ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 56, 0, 232, \
                      FACING_EAST, TALK_FACING_PARTY, 0),                      \
      ACTOR_PLACEMENT(110, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 360, \
                      FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),             \
      {307, CONDITION_ALWAYS, (s32)gNaribwe_IvyAnimation, px(40), 0, px(168),  \
       FACING_SOUTH, 2, 0},                                                    \
      ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 376, \
                      FACING_SOUTH, TALK_FACING_KEEP, 0),                      \
      ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 376, \
                      FACING_SOUTH, TALK_FACING_KEEP, 0),                      \
      ACTOR_PLACEMENT(201, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 168, \
                      FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),            \
      ACTOR_PLACEMENT(328, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 104, \
                      FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),            \
      {SCENE_TABLE_END},                                                       \
  };

// clang-format off
// Unused: a list of actor 26 and flag 0x200; nothing reads it.
s16 gNaribwe_UnusedList[] = {
    26, 512, -1, 0,
};

// The braziers burn, the fire's flames rising above them.
u32 gNaribwe_BrazierBurns[] = {
    39, 2, 17,
};

u32 gNaribwe_FlameRises[] = {
    39, 0, 22, 26, 0, 23,
    6, 0x180000, 17,
};

u32 gNaribwe_IvyAnimation[] = {
    39, 3, 17,
};

// The cell animation of the village's doors.
s16 gNaribwe_DoorOpening[] = {
    8, 42, 1, 2, 6, -1,
};

s32 gNaribwe_Doors[] = {                                                               
      (s32)gNaribwe_DoorOpening,                                                              
      0x330012,                                                                      
      (s32)gNaribwe_DoorOpening,                                                              
      0x220015,                                                                      
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
    ACTOR_PLACEMENT(364, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 246, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(305, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 316, 0, 132, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(305, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 372, 0, 132, FACING_SOUTH, TALK_FACING_PARTY, 0),
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
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgNaribwe01)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS,
     EVENT(Naribwe_AskAboutKibomboExpedition)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgNaribwe04)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgNaribwe05)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgNaribwe06)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Naribwe_AskWhereFrom)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(Naribwe_AskGoingToKibombo)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgNaribwe10)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgNaribwe19)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Naribwe_PraiseFortuneTeller)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, EVENT(Naribwe_TalkToPilgrim)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe11)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe12)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe13)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe14)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe16)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe17)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe18)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe22)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe24)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgNaribwe47))},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(Naribwe_PrepareBlaze)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 23, 0x201,
     EVENT(Naribwe_BlowAwayIvy)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 24, 0x202, 0},
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
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgNaribwe25)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgNaribwe26)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgNaribwe27)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgNaribwe28)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgNaribwe29)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgNaribwe30)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgNaribwe31)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgNaribwe32)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgNaribwe41)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS,
     EVENT(Naribwe_PraiseFortuneTellerOnReturn)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, EVENT(Naribwe_TalkToPilgrimOnReturn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe33)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe34)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe35)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe36)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe37)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe38)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe39)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe40)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe42)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe44)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS,
     MSGNO(MsgNaribwe46)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgNaribwe47))},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(Naribwe_PrepareBlaze)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 23, 0x201,
     EVENT(Naribwe_BlowAwayIvy)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 24, 0x202, 0},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Naribwe_RevealStone)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Naribwe_HideStone)},
    {TRIGGER_TABLE_END},
};
#undef Naribwe_PraiseFortuneTellerOnReturn
