/// @file
/// The way into Lemuria from its dock: a rock Grind breaks up, two ways on to
/// the city, and the ship's harbour, where Sheba asks Piers his age as they
/// sail out.

#include "flags.h"
#include "types.h"
#include "field/field_sequence.h"
#include "field/field_sprite.h"
#include "sprite_ids.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "field/field_sequence_event.h"
#include "field/call.h"
#include "scene/scene.h"
#include "window/window.h"
#include "graphics/uiflow.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

extern s32 gLemuriaEntrance_ShipSailsOut[];
/// The way in's actors, by the rows of gActorPlacements.
enum {
  /// The party's ship, placed blank; it sails out of the harbour
  /// (gLemuriaEntrance_ShipSailsOut).
  ACTOR_SHIP = ACTOR_FIRST_PLACED,
  /// Boats moored in the harbour: four on sprite 342, six on the ship's own
  /// boat sheet (sprite 11).
  ACTOR_MOORED_BOAT_A,
  ACTOR_MOORED_BOAT_B,
  ACTOR_MOORED_BOAT_C,
  ACTOR_MOORED_BOAT_D,
  ACTOR_MOORED_SHIP_A,
  ACTOR_MOORED_SHIP_B,
  ACTOR_MOORED_SHIP_C,
  ACTOR_MOORED_SHIP_D,
  ACTOR_MOORED_SHIP_E,
  ACTOR_MOORED_SHIP_F,
  ACTOR_ROCK ///< sprite 424, which Grind breaks up
};

/// Who speaks a line of LemuriaEntrance_ShebaAsksPiersAge: the party by
/// their numbers, and Kraden as 8, who shows his own portrait.
enum { SPEAKER_KRADEN = 8 };

/// Unused: no exit leads here; it hands the ship to the player and enters again
/// by entrance 1.
enum { ENTRANCE_ABOARD_SHIP = 90 };

void LemuriaEntrance_ShowPortraitLine(s32 speaker, s32 frames);

void LemuriaEntrance_ShebaAsksPiersAge(void);

extern s32 gLemuriaEntrance_ShipAtRest[];

/// Grind has broken up the rock in the way.
#define FLAG_ROCK_GROUND_AWAY 0x909

void Text_GetMessageSize(s32 id, s32 *a, s32 *b, s32 *c, s32 *d);
void Message_OpenWindow(s32 id, s32 x, s32 y, u32 flags);
s32 Portrait_OpenWindow(s32 a, s32 b, s32 c, s32 d);

/// Starts an actor's animation from its first frame.
s32 LemuriaEntrance_ResetAnimationFrame(struct Actor *object) {
  ((struct FieldSprite *)object->sprite)->flags = 0;
  return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// The party leaves the harbour for Lemuria by its first way in, clearing
/// FLAG_LEMURIA_OTHER_WAY_IN.
void LemuriaEntrance_LeaveByExit3(s32 exit) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  party->ownerOverride = 0;
  Event_ClearFlag(FLAG_LEMURIA_OTHER_WAY_IN);
  Event_PlaySound(SE_MAP_EXIT);
  party->leaderActorId = ACTOR_SHIP;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  party->leaderActorId = ACTOR_FELIX;
  FieldEvent_SetExitRequest(exit);
}

/// The party leaves for Lemuria by its other way in
/// (FLAG_LEMURIA_OTHER_WAY_IN).
void LemuriaEntrance_LeaveByExit5(s32 exit) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  party->ownerOverride = 0;
  Event_SetFlag(FLAG_LEMURIA_OTHER_WAY_IN);
  Event_PlaySound(SE_MAP_EXIT);
  party->leaderActorId = ACTOR_SHIP;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  party->leaderActorId = ACTOR_FELIX;
  FieldEvent_SetExitRequest(exit);
}

/// Grind breaks up the rock in the way, for good (FLAG_ROCK_GROUND_AWAY).
void LemuriaEntrance_GrindRock(void) {
  Event_SetFlag(FLAG_ROCK_GROUND_AWAY);
  Actor_SetPosition(ACTOR_ROCK, 0, 0);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Unused: once flag 0x800 is set, the party boards the ship and
/// sails it (render mode 6); otherwise the leader takes control again. Nothing
/// calls it.
UNUSED void LemuriaEntrance_BoardShip(void) {
  if (Event_GetFlag(0x800) != 0) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    {
      struct MainRoutineWork *state = &gMainRoutineWork;

      state->movementMode = LEADER_MOVEMENT_SHIP_ON_MAP;
      state->ownerOverride = 1;
    }
    Actor_SetAnimation(ACTOR_SHIP, 7);
    Event_End();
  } else {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->leaderActorId = ACTOR_FELIX;
    party->movementMode = LEADER_MOVEMENT_WALK;
    party->ownerOverride = 0;
  }
}

/// The scene's setup: places its actors, then plays what the entrance it was
/// reached by calls for.
s32 Scene_Initialize(void) {
  s32 *screen = &gFieldWork->transition;
  s32 effect = SCENE_TRANSITION(TRANSITION_WINDOW, 4);

  *screen = effect;
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ROCK), 0);
  Event_SetFlag(FLAG_LEMURIA_REACHED);
  Event_ClearFlag(FLAG_SHIP_AT_ATTEKA_INLET);
  if (Event_GetFlag(FLAG_ROCK_GROUND_AWAY))
    Actor_SetPosition(ACTOR_ROCK, 0, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MOORED_BOAT_A), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MOORED_BOAT_B), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MOORED_BOAT_C), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MOORED_BOAT_D), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MOORED_SHIP_A), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MOORED_SHIP_B), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MOORED_SHIP_C), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MOORED_SHIP_D), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MOORED_SHIP_E), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MOORED_SHIP_F), 0);
  if (gMainRoutineWork.entrance == ENTRANCE_ABOARD_SHIP) {
    gMainRoutineWork.ownerOverride = 1;
    Event_SetDestination(SCENE_LEMURIA_ENTRANCE, 1);
  }
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5) {
    gMainRoutineWork.leaderActorId = ACTOR_SHIP;
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    gMainRoutineWork.leaderActorId = ACTOR_FELIX;
    gFieldWork->transition = effect;
  }
  if (gMainRoutineWork.entrance == 3 &&
      Event_GetFlag(FLAG_SHEBA_TO_ASK_PIERS_AGE)) {
    Event_ClearFlag(FLAG_SHEBA_TO_ASK_PIERS_AGE);
    LemuriaEntrance_ShebaAsksPiersAge();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Shows the next line of a conversation in a window beside its speaker's
/// portrait (Kraden's for SPEAKER_KRADEN) and closes it after the frames.
void LemuriaEntrance_ShowPortraitLine(s32 speaker, s32 frames) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 b;
  s32 a;
  s32 height;
  s32 width;
  s32 resource;
  s32 id;
  s32 x;

  width = 0;
  height = 0;
  id = SPRITE_KRADEN;
  if (speaker != SPEAKER_KRADEN)
    id = speaker;
  resource = Speaker_GetVoice(id);
  if (speaker == ACTOR_PIERS) {
    width = 10;
    height = 10;
    Text_GetMessageSize(work->message, &width, &height, &a, &b);
    x = width + a - 5;
  } else {
    width = 5;
    height = 10;
    x = 5;
  }
  Call4((void (*)())Message_OpenWindow, work->message++, width, height,
        (resource << 16) | 2);
  x = Portrait_OpenWindow(id, 0, x, 5);
  while (Message_AllTyped() == 0)
    System_Wait(1);
  FieldEvent_Wait(frames);
  Window_Close(x, 2);
  Message_CloseFinishedWindows();
}

/// As the ship sails out of the harbour, Sheba asks Piers how old he really is.
/// "Umm... Piers?"
void LemuriaEntrance_ShebaAsksPiersAge(void) {
  struct FieldWork *work;
  u8 saved;

  saved = gMainRoutineWork.messageSpeed;
  gMainRoutineWork.messageSpeed = 2;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(ACTOR_SHIP, 0.27);
  Actor_SetScript(ACTOR_SHIP, gLemuriaEntrance_ShipSailsOut);
  FieldEvent_Wait(80);
  FieldEvent_SetMessageId((s32)&MsgFieldAddressCompanion);
  LemuriaEntrance_ShowPortraitLine(ACTOR_SHEBA, 90);
  LemuriaEntrance_ShowPortraitLine(ACTOR_PIERS, 90);
  LemuriaEntrance_ShowPortraitLine(ACTOR_SHEBA, 180);
  LemuriaEntrance_ShowPortraitLine(ACTOR_PIERS, 90);
  LemuriaEntrance_ShowPortraitLine(ACTOR_SHEBA, 250);
  LemuriaEntrance_ShowPortraitLine(ACTOR_PIERS, 180);
  LemuriaEntrance_ShowPortraitLine(ACTOR_JENNA, 250);
  LemuriaEntrance_ShowPortraitLine(SPEAKER_KRADEN, 90);
  LemuriaEntrance_ShowPortraitLine(ACTOR_PIERS, 180);
  LemuriaEntrance_ShowPortraitLine(SPEAKER_KRADEN, 180);
  LemuriaEntrance_ShowPortraitLine(ACTOR_PIERS, 180);
  LemuriaEntrance_ShowPortraitLine(ACTOR_SHEBA, 90);
  LemuriaEntrance_ShowPortraitLine(ACTOR_PIERS, 250);
  LemuriaEntrance_ShowPortraitLine(ACTOR_JENNA, 250);
  LemuriaEntrance_ShowPortraitLine(ACTOR_PIERS, 250);
  LemuriaEntrance_ShowPortraitLine(ACTOR_SHEBA, 90);
  LemuriaEntrance_ShowPortraitLine(ACTOR_JENNA, 90);
  LemuriaEntrance_ShowPortraitLine(SPEAKER_KRADEN, 90);
  LemuriaEntrance_ShowPortraitLine(ACTOR_PIERS, 250);
  LemuriaEntrance_ShowPortraitLine(ACTOR_FELIX, 90);
  LemuriaEntrance_ShowPortraitLine(ACTOR_PIERS, 250);
  FieldActor_WaitForScriptEndById(ACTOR_SHIP);
  FieldEvent_Wait(10);
  work = Ram_HeapSlots->fieldWork;
  work->transitionFrames = 30;
  work->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(1);
  gMainRoutineWork.messageSpeed = saved;
  Event_End();
}

// clang-format off
/// The ship's course out of the harbour.
s32 gLemuriaEntrance_ShipSailsOut[] = {
    SC_MOVE(728, 0, 160),
    SC_SYNC,
    SC_MOVE(608, 0, 152),
    SC_SYNC,
    SC_MOVE(488, 0, 152),
    SC_SYNC,
    SC_MOVE(368, 0, 152),
    SC_SYNC,
    SC_MOVE(368, 0, 112),
    SC_SYNC,
    SC_MOVE(336, 0, 88),
    SC_SYNC,
    SC_MOVE(320, 0, 88),
    SC_SYNC,
    SC_MOVE(288, 0, 120),
    SC_SYNC,
    SC_MOVE(288, 0, 176),
    SC_SYNC,
    SC_MOVE(272, 0, 192),
    SC_SYNC,
    SC_MOVE(272, 0, 280),
    SC_SYNC,
    SC_MOVE(344, 0, 280),
    SC_SYNC,
    SC_MOVE(384, 0, 320),
    SC_SYNC,
    SC_MOVE(384, 0, 408),
    SC_SYNC,
    SC_MOVE(416, 0, 440),
    SC_SYNC,
    SC_MOVE(416, 0, 552),
    SC_SYNC,
    SC_END,
};
// clang-format on

s32 gLemuriaEntrance_ShipAtRest[] = {
    SC_CALL(LemuriaEntrance_ResetAnimationFrame),
    SC_ANIMATION(7),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(LemuriaEntrance),
    SCENE_EXIT_UNLESS(1, WorldMap, 45, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, LemuriaEntrance, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, LemuriaDock, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, LemuriaEntrance, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, LemuriaDock, 1, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS,
                    (s32)gLemuriaEntrance_ShipAtRest, 14, 0, 4, FACING_NORTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOORED_BOAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    636, 0, 112, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOORED_BOAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    668, 0, 112, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_MOORED_BOAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    668, 0, 216, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_MOORED_BOAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    732, 0, 216, FACING_WEST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_WINGED_SHIP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    736, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_SHIP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    640, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_WINGED_SHIP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    704, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_WINGED_SHIP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    52, 0, 796, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_SHIP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    52, 0, 828, FACING_WEST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_WINGED_SHIP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    52, 0, 860, FACING_WEST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144,
                    0, 160, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(LemuriaEntrance_LeaveByExit3)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(LemuriaEntrance_LeaveByExit5)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ROCK, CONDITION_ALWAYS,
     EVENT(LemuriaEntrance_GrindRock)},
    {TRIGGER_TABLE_END},
};
