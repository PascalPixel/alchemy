/// @file
/// The channels among the cliffs where the party's ship comes into Lemuria. In
/// the north-east is the harbor of moored Lemurian ships, whose quay is the
/// way ashore to the city. In the north-west a brown rock blocks the channel
/// until Grind breaks it up; past it the channel runs south to a second quay,
/// by two moored boats, the way ashore to another part of the ruins. Sailing
/// out after the audience with King Hydros, Sheba asks Piers how old he is.

#include "flags.h"
#include "types.h"
#include "field/field_sequence.h"
#include "field/field_sprite.h"
#include "sprite_ids.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
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
#include "graphics/window_close.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"
#include "scene_exits.h"

extern s32 gLemuriaEntrance_ShipSailsOut[];
/// The way in's actors, by the rows of gActorPlacements.
enum {
  /// The party's ship, placed blank; it sails out of the harbor
  /// (gLemuriaEntrance_ShipSailsOut).
  ACTOR_SHIP = ACTOR_FIRST_PLACED,
  /// Boats moored in the harbor: four SPRITE_MOORED_BOAT and six on the
  /// winged ship's sprite sheet (SPRITE_WINGED_SHIP).
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
  ACTOR_ROCK ///< the brown rock in the north-west channel, which Grind breaks
             ///< up
};

/// Who speaks a line of LemuriaEntrance_ShebaAsksPiersAge: the party by
/// their numbers, and Kraden as 8, who shows his own portrait.
enum { SPEAKER_KRADEN = 8 };

/// Unused: no exit leads here; it hands the ship to the player and enters again
/// by entrance 1.
enum { ENTRANCE_ABOARD_SHIP = 90 };

void LemuriaEntrance_ShowLineWithPortrait(s32 speaker, s32 frames);

void LemuriaEntrance_ShebaAsksPiersAge(void);

extern s32 gLemuriaEntrance_ShipAtRest[];

/// Grind has broken up the brown rock in the north-west channel.
#define FLAG_ROCK_GROUND_AWAY 0x909

/// The channel past the rock runs on in the map's south half: its north end
/// (exit and entrance 2) leads to its south end (exit and entrance 4), and
/// back.
enum { CHANNEL_NORTH_END = 2, CHANNEL_SOUTH_END = 4 };

void Text_GetMessageSize(s32 message, s32 *x, s32 *y, s32 *width, s32 *height);
void Message_OpenWindow(s32 message, s32 x, s32 y, u32 options);
s32 Portrait_OpenWindow(s32 speaker, s32 portraitOption, s32 x, s32 y);

/// Starts an actor's animation from its first frame.
s32 LemuriaEntrance_ResetAnimationFrame(struct Actor *actor) {
  ((struct FieldSprite *)actor->sprite)->flags = 0;
  return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// The ship sails up to the quay in the harbor of moored Lemurian ships, in the
/// north-east, and the party goes ashore on Lemuria's dock, the way to the
/// city. Clearing FLAG_LEMURIA_OTHER_WAY_IN makes the dock's ways lead back
/// here and on towards the city.
void LemuriaEntrance_LandAtHarbor(s32 exit) {
  struct MainRoutineState *party = &gMainRoutineState;
  party->ownerOverride = 0;
  Event_ClearFlag(FLAG_LEMURIA_OTHER_WAY_IN);
  Event_PlaySound(SE_MAP_EXIT);
  party->leaderActorId = ACTOR_SHIP;
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  party->leaderActorId = ACTOR_FELIX;
  FieldEvent_SetExitRequest(exit);
}

/// The ship sails up to the quay by the two moored boats, at the south end of
/// the channel past the rock Grind breaks up, and the party goes ashore on
/// Lemuria's dock. Setting FLAG_LEMURIA_OTHER_WAY_IN makes the dock's ways
/// lead back to this quay and on to the other part of the ruins.
void LemuriaEntrance_LandAtSouthQuay(s32 exit) {
  struct MainRoutineState *party = &gMainRoutineState;
  party->ownerOverride = 0;
  Event_SetFlag(FLAG_LEMURIA_OTHER_WAY_IN);
  Event_PlaySound(SE_MAP_EXIT);
  party->leaderActorId = ACTOR_SHIP;
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  party->leaderActorId = ACTOR_FELIX;
  FieldEvent_SetExitRequest(exit);
}

/// Felix casts Grind on the brown rock in the north-west channel: it breaks up
/// and is gone for good, opening the way south to the second quay.
void LemuriaEntrance_GrindRock(void) {
  Event_SetFlag(FLAG_ROCK_GROUND_AWAY);
  Actor_SetPosition(ACTOR_ROCK, 0, 0);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Unused: once FLAG_TRANSFERRED_DJINN_FIRST is set, the party boards the ship
/// and takes the helm; otherwise Felix walks on as before. Nothing calls it.
UNUSED void LemuriaEntrance_BoardShip(void) {
  if (Event_GetFlag(FLAG_TRANSFERRED_DJINN_FIRST) != 0) {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    {
      struct MainRoutineState *state = &gMainRoutineState;

      state->movementMode = LEADER_MOVEMENT_SHIP_ON_MAP;
      state->ownerOverride = 1;
    }
    Actor_SetAnimation(ACTOR_SHIP, 7);
    Event_End();
  } else {
    struct MainRoutineState *party = &gMainRoutineState;

    party->leaderActorId = ACTOR_FELIX;
    party->movementMode = LEADER_MOVEMENT_WALK;
    party->ownerOverride = 0;
  }
}

/// The scene's setup: places its actors, then plays what the entrance it was
/// reached by calls for.
s32 Scene_Initialize(void) {
  s32 *screen = &gFieldState->transition;
  s32 effect = SCENE_TRANSITION(TRANSITION_WINDOW, 4);

  *screen = effect;
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
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
  if (gMainRoutineState.entrance == ENTRANCE_ABOARD_SHIP) {
    gMainRoutineState.ownerOverride = 1;
    Event_SetDestination(SCENE_LEMURIA_ENTRANCE, 1);
  }
  if (gMainRoutineState.entrance == LEMURIA_ENTRANCE_FROM_LEMURIA_DOCK_NORTH ||
      gMainRoutineState.entrance == LEMURIA_ENTRANCE_FROM_LEMURIA_DOCK_SOUTH) {
    gMainRoutineState.leaderActorId = ACTOR_SHIP;
    gFieldState->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    gMainRoutineState.leaderActorId = ACTOR_FELIX;
    gFieldState->transition = effect;
  }
  if (gMainRoutineState.entrance == LEMURIA_ENTRANCE_FROM_LEMURIA_DOCK_NORTH &&
      Event_GetFlag(FLAG_SHEBA_TO_ASK_PIERS_AGE)) {
    Event_ClearFlag(FLAG_SHEBA_TO_ASK_PIERS_AGE);
    LemuriaEntrance_ShebaAsksPiersAge();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Shows the next line of a conversation beside its speaker's portrait
/// (Kraden's for SPEAKER_KRADEN), holds it the given frames once typed, and
/// closes it. Piers's lines stand further right with his portrait at their
/// right-hand end; everyone else's portrait is at the left.
void LemuriaEntrance_ShowLineWithPortrait(s32 speaker, s32 frames) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 height;
  s32 width;
  s32 top;
  s32 left;
  s32 voice;
  s32 character;
  s32 portrait; // the portrait's left edge, then its window

  left = 0;
  top = 0;
  character = SPRITE_KRADEN;
  if (speaker != SPEAKER_KRADEN)
    character = speaker;
  voice = Speaker_GetVoice(character);
  if (speaker == ACTOR_PIERS) {
    left = 10;
    top = 10;
    Text_GetMessageSize(state->message, &left, &top, &width, &height);
    portrait = left + width - 5;
  } else {
    left = 5;
    top = 10;
    portrait = 5;
  }
  Call4((void (*)())Message_OpenWindow, state->message++, left, top,
        MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_UNREAD);
  portrait = Portrait_OpenWindow(character, 0, portrait, 5);
  while (Message_AllTyped() == 0)
    System_Wait(1);
  FieldEvent_Wait(frames);
  Window_Close(portrait, 2);
  Message_CloseFinishedWindows();
}

/// Coming back aboard after the audience with King Hydros, the ship sails out
/// of the harbor and through the channels while Sheba asks Piers how old he
/// really is, "Umm... Piers?", and Piers will not say. Then the ship is out on
/// the sea.
void LemuriaEntrance_ShebaAsksPiersAge(void) {
  struct FieldState *state;
  u8 saved;

  saved = gMainRoutineState.messageSpeed;
  gMainRoutineState.messageSpeed = 2;
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetSpeed(ACTOR_SHIP, 0.27);
  Actor_SetScript(ACTOR_SHIP, gLemuriaEntrance_ShipSailsOut);
  FieldEvent_Wait(80);
  FieldEvent_SetMessageId((s32)&MsgFieldAddressCompanion);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_SHEBA, 90);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_PIERS, 90);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_SHEBA, 180);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_PIERS, 90);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_SHEBA, 250);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_PIERS, 180);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_JENNA, 250);
  LemuriaEntrance_ShowLineWithPortrait(SPEAKER_KRADEN, 90);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_PIERS, 180);
  LemuriaEntrance_ShowLineWithPortrait(SPEAKER_KRADEN, 180);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_PIERS, 180);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_SHEBA, 90);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_PIERS, 250);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_JENNA, 250);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_PIERS, 250);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_SHEBA, 90);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_JENNA, 90);
  LemuriaEntrance_ShowLineWithPortrait(SPEAKER_KRADEN, 90);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_PIERS, 250);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_FELIX, 90);
  LemuriaEntrance_ShowLineWithPortrait(ACTOR_PIERS, 250);
  FieldActor_WaitForScriptEndById(ACTOR_SHIP);
  FieldEvent_Wait(10);
  state = Ram_HeapSlots->fieldState;
  state->transitionFrames = 30;
  state->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(LEMURIA_ENTRANCE_TO_WORLD_MAP);
  gMainRoutineState.messageSpeed = saved;
  Event_End();
}

// clang-format off
/// The ship's course out of the harbor.
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

/// The ship's animation as it rests in the harbor.
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
    SCENE_EXIT_UNLESS(LEMURIA_ENTRANCE_TO_WORLD_MAP, WorldMap,
                      WORLD_MAP_FROM_LEMURIA_ENTRANCE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHANNEL_NORTH_END, LemuriaEntrance, CHANNEL_SOUTH_END,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_ENTRANCE_TO_LEMURIA_DOCK_NORTH, LemuriaDock,
                      LEMURIA_DOCK_FROM_LEMURIA_ENTRANCE_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CHANNEL_SOUTH_END, LemuriaEntrance, CHANNEL_NORTH_END,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_ENTRANCE_TO_LEMURIA_DOCK_SOUTH, LemuriaDock,
                      LEMURIA_DOCK_FROM_LEMURIA_ENTRANCE_NORTH, EXIT_FLAG_NONE),
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
    {TRIGGER_EXIT, LEMURIA_ENTRANCE_TO_WORLD_MAP, CONDITION_ALWAYS,
     LEMURIA_ENTRANCE_TO_WORLD_MAP},
    {TRIGGER_EXIT, CHANNEL_NORTH_END, CONDITION_ALWAYS, CHANNEL_NORTH_END},
    {TRIGGER_EXIT, CHANNEL_SOUTH_END, CONDITION_ALWAYS, CHANNEL_SOUTH_END},
    {TRIGGER_STEP, LEMURIA_ENTRANCE_TO_LEMURIA_DOCK_NORTH, CONDITION_ALWAYS,
     EVENT(LemuriaEntrance_LandAtHarbor)},
    {TRIGGER_STEP, LEMURIA_ENTRANCE_TO_LEMURIA_DOCK_SOUTH, CONDITION_ALWAYS,
     EVENT(LemuriaEntrance_LandAtSouthQuay)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ROCK, CONDITION_ALWAYS,
     EVENT(LemuriaEntrance_GrindRock)},
    {TRIGGER_TABLE_END},
};
