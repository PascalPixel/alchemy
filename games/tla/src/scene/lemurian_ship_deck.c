/// @file
/// The Lemurian ship, in two scenes: the East Indra Shore, where the ship lies
/// moored below the cliffs (SceneId_LemurianShipMainDeck), and its deck at the
/// tiller once it sails (SceneId_LemurianShipTiller).
///
/// The party first comes down the sandy path to the shore before the ceremony
/// at the Gabomba Statue, and Kraden takes the ship for Menardi's, drifted
/// there with no orb to sail it. A puddle on the path west of the ship freezes
/// with Frost into a pillar of ice to the wooden box on the ledge. Back with
/// Piers after the ceremony, he agrees on the shore to take the party to
/// Lemuria once he has seen the mayor of Madra; boarding by the rope ladder on
/// the ship's side, he holds the Black Crystal up to the cabin door, which
/// opens, and leads the way down to the engine (lemurian_ship.c). At sea the
/// deck by the tiller plays Piers teaching Felix to sail, and later the party's
/// talk after a whirlpool of the Sea of Time throws the ship back.

#include "battle/battle.h"
#include "graphics/palette_buffers.h"
#include "sprite_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "field/field_sequence.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "graphics/glyph.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "system/main_routine_state.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "system/io_reg.h"
#include "field/map_grid.h"
#include "system/frame.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "shared/tables.h"
#include "version.h"
#include "macros.h"
#include "field/field_sprite.h"
#include "system/task.h"
#include "field/call.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "text/msg_ids.h"

s32 gLemurianShipDeck_BobPhase BSS_DATA;   ///< angle, TRIG_TURN / 128 a frame
s32 gLemurianShipDeck_WaveScroll BSS_DATA; ///< 16.16 pixels, to 32

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// The deck's actors, by the rows of both of its actor placement tables;
/// Jenna's, Sheba's and Piers' rows go to their party slots.
enum {
  ACTOR_PUDDLE =
      ACTOR_FIRST_PLACED, ///< SPRITE_PUDDLE, on the path west of the ship
  ACTOR_KRADEN
};

/// The shore's cells, as map.txt has them; the cabin door's step cells are the
/// exit to the cabins and leave by it.
enum LemurianShipDeckCellBehavior {
  /// The head of the sandy path down to the shore, cells (14, 4) to (19, 4).
  CELL_BEHAVIOR_HEAD_OF_PATH = 10,
  /// The row below it, cells (12, 5) to (21, 5).
  CELL_BEHAVIOR_TOP_OF_SHORE,
  /// The shallows at the foot of the rope ladder on the ship's side, cell
  /// (27, 27).
  CELL_BEHAVIOR_FOOT_OF_LADDER,
  /// The cabin door's face, cell (22, 24), which Felix checks.
  CELL_BEHAVIOR_CABIN_DOOR = 14,
  /// The deck east of the cabin door, cells (72, 25) and (72, 26).
  CELL_BEHAVIOR_EAST_OF_DOOR,
  /// The cells either side of those, (71, 25), (73, 25), (71, 26) and
  /// (73, 26).
  CELL_BEHAVIOR_BESIDE_EAST_OF_DOOR
};

#define FLAG_PUDDLE_FROZEN FLAG_SCENE_TEMP_00 ///< Frost froze the path's puddle
#define FLAG_CABIN_DOOR_OPEN FLAG_SCENE_TEMP_01 ///< Piers opened it this visit
/// Felix's first and second steps east of the cabin door, while Kraden waits.
#define FLAG_FIRST_STEP_EAST_OF_DOOR FLAG_SCENE_TEMP_10
#define FLAG_SECOND_STEP_EAST_OF_DOOR FLAG_SCENE_TEMP_11
/// Felix stands east of the cabin door; stepping beside it clears this.
#define FLAG_STANDING_EAST_OF_DOOR FLAG_SCENE_TEMP_12

/// Draws the cabin door open, from spare cells of the map at (51, 38).
#define LemurianShipDeck_DrawCabinDoorOpen()                                   \
  MapGrid_CopyCellsTo(51, 38, 70, 22, 1, 2)

/// Draws plain hull over the cells at (67, 16) of the ship's middle layer: the
/// copy that Lemuria's dock makes there among those that hide the ship's wings
/// until it can fly (lemuria_dock.c), one cell wider.
#define LemurianShipDeck_HideWing() MapGrid_CopyCellsTo(60, 53, 67, 16, 7, 3)

extern struct ActorPlacement gLemurianShipDeck_Placements[];
extern struct ActorPlacement gLemurianShipDeck_TillerPlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_LEMURIAN_SHIP_TILLER)
    return gLemurianShipDeck_TillerPlacements;
  return gLemurianShipDeck_Placements;
}

/// Felix walks north into the cabin door on the ship's deck, cell (22, 25),
/// once Piers has opened it: he steps into the doorway and the screen closes on
/// the cabins below. Before then nothing happens.
///
/// If the door is not yet open on this visit (`FLAG_CABIN_DOOR_OPEN`), it
/// swings open first. Every placed actor stops where it is.
void LemurianShipDeck_EnterCabin(void) {
  if (Event_GetFlag(FLAG_PIERS_BACK_ON_HIS_SHIP)) {
    struct FieldState *state = Ram_HeapSlots->fieldState;
    u32 i;

    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    for (i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
      struct Actor *actor = Actor_Get(i);

      if (actor != 0) {
        actor->motionFlags = 0;
        actor->y.fixed = toFIXED(32);
      }
    }
    if (!Event_GetFlag(FLAG_CABIN_DOOR_OPEN)) {
      Event_PlaySound(SE_DOOR_OPEN);
      LemurianShipDeck_DrawCabinDoorOpen();
    }
    Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_WALK);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
    Actor_CenterAndWalk(gMainRoutineState.leaderActorId, 2, -4);
    FieldEvent_Wait(8);
    Event_PlaySound(SE_MAP_EXIT);
    FieldEvent_SetExitRequest(state->stepTrigger);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Event_End();
  }
}

extern struct Trigger gLemurianShipDeck_Triggers[];
extern struct Trigger gLemurianShipDeck_MainDeckTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_LEMURIAN_SHIP_MAIN_DECK)
    return gLemurianShipDeck_MainDeckTriggers;
  return gLemurianShipDeck_Triggers;
}

/// Felix checks the cabin door on the ship's deck, cell (22, 24), before Piers
/// has come aboard: "The door is held closed by a powerful force."
void LemurianShipDeck_TryHeldDoor(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Message_ShowCentered(MsgLemurianShipDeckDoorIsHeldClosed, 1);
  Event_End();
}

/// Defines a task that moves colours last-1 .. first of LEMURIAN_SHIP_PALETTE
/// one place up and the last round to first, every eighth frame.
#define DEFINE_ROTATE_PALETTE_COLORS(name, last, first)                        \
  void name(void) {                                                            \
    if ((gFrameCount & 7) == 0) {                                              \
      s16 saved = BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, last);                  \
      s32 i;                                                                   \
                                                                               \
      for (i = last; i != first; i--)                                          \
        BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, i) =                              \
            BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, i - 1);                       \
      BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, first) = saved;                     \
    }                                                                          \
  }

/// A task: turns the surf's colours round by one.
DEFINE_ROTATE_PALETTE_COLORS(LemurianShipDeck_RotateSurfColors, SURF_LAST_COLOR,
                             SURF_FIRST_COLOR)

/// A task: turns the blues of the band along the hull round by one.
DEFINE_ROTATE_PALETTE_COLORS(LemurianShipDeck_RotateHullBandColors,
                             HULL_BAND_LAST_COLOR, HULL_BAND_FIRST_COLOR)

void TilesetAnim_Resume(s32 channelIndex);

/// The entrance whose arrival lights the band along the hull.
#define MAIN_DECK_ARRIVAL_ENTRANCE 99
enum {
  /// The map's two tile animations, held until that arrival, that light the
  /// band one after the other.
  HULL_BAND_FIRST_ANIMATION = 13,
  HULL_BAND_SECOND_ANIMATION
};

/// The party arrives on the shore by MAIN_DECK_ARRIVAL_ENTRANCE: the screen
/// opens and the blue band along the ship's hull lights up in two steps.
///
/// No exit table or event in the game names entrance 99 of the main deck: the
/// cabins' and the world map's exits lead in by entrances 1 and 2, so this
/// arrival is reached by none of them.
void LemurianShipDeck_ShowArrival(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Event_StartScreenTransition();
  FieldTask_AddFrame(LemurianShipDeck_RotateHullBandColors, TASK_PASS_FRAME);
  Event_WaitTransitionFrames();
  FieldEvent_Wait(60);
  TilesetAnim_Resume(HULL_BAND_FIRST_ANIMATION);
  FieldEvent_Wait(60);
  FieldEvent_Wait(60);
  TilesetAnim_Resume(HULL_BAND_SECOND_ANIMATION);
  Event_End();
}

/// A task: scrolls the waves' layer half a pixel a frame.
void LemurianShipDeck_ScrollWaves(void) {
  struct MapLayer *layer =
      &((struct MapState *)Ram_HeapSlots->mapGrid)->layers[0];
  FIXED phase = gLemurianShipDeck_WaveScroll;

  layer->offsetX = phase;
  phase += toFIXED(1) / 2;
  gLemurianShipDeck_WaveScroll = phase;
  if (phase == toFIXED(32))
    gLemurianShipDeck_WaveScroll = 0;
}

/// A task: bobs the camera's actor and the first map layer together on a sine
/// wave, a full wave every 128 frames.
void LemurianShipDeck_BobDeck(void) {
  struct MapLayer *layer =
      &((struct MapState *)Ram_HeapSlots->mapGrid)->layers[0];
  struct Actor *actor = EventCamera_GetActor();
  s32 offset = Math_Sin(gLemurianShipDeck_BobPhase) >> 2;

  actor->y.fixed += offset;
  layer->offsetY += offset;
  gLemurianShipDeck_BobPhase += TRIG_TURN / 128;
}

extern s32 gLemurianShipDeck_JennaLooksAround[];

extern s32 gLemurianShipDeck_ShebaLooksAround[];

extern s32 gLemurianShipDeck_KradenLooksAround[];

void FieldCamera_WaitForArrival(void);

extern s16 gLemurianShipDeck_Puddles[];

void FlagBlock_PlaceAll(const void *data);

void FlagBlock_ApplyFlags(const void *data);

extern u32 gLemurianShipDeck_SparkleFades[];
extern s32 gLemurianShipDeck_ReturnToShipScript[];

void Actor_Release(struct Actor *actor);

extern s32 gLemurianShipDeck_ReachPiersShipScript[];
extern u32 gLemurianShipDeck_SplashFades[];

/// Places an actor facing an angle, the angle taken as the u16 an actor's
/// facing is, as Madra's and Loho's scenes also place them.
static inline void LemurianShipDeck_PlaceFacing(s32 id, s32 x, s32 z,
                                                u16 facing) {
  Actor_SetPositionAndFacing(id, x, z, facing);
}

/// The first time the ship sails, on the deck by the tiller: Piers tells Felix,
/// "Take the tiller, Felix!", the party talks over where to go (Felix's
/// answer to Piers's wish to see Vale changes Kraden's reply), and Kraden ends,
/// "To the open sea, Felix!" The player then steers the ship on the world map.
///
/// It runs from entrance 10 and sets FLAG_SHIP_CAN_SAIL.
void LemurianShipDeck_TakeTheTiller(void) {
  s32 turn;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(342, 48, 408, 0);
  System_Wait(1);
  LemurianShipDeck_HideWing();
  LemurianShipDeck_PlaceFacing(gMainRoutineState.leaderActorId, toFIXED(552),
                               toFIXED(376), FACING_EAST);
  LemurianShipDeck_PlaceFacing(ACTOR_KRADEN, toFIXED(558), toFIXED(345),
                               FACING_NORTH_NORTHEAST);
  LemurianShipDeck_PlaceFacing(ACTOR_PIERS, toFIXED(534), toFIXED(404),
                               FACING_EAST);
  turn = FACING_SOUTHEAST;
  LemurianShipDeck_PlaceFacing(ACTOR_SHEBA, toFIXED(568), toFIXED(408), turn);
  LemurianShipDeck_PlaceFacing(ACTOR_JENNA, toFIXED(546), toFIXED(440),
                               FACING_SOUTH);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  Actor_SetScript(ACTOR_JENNA, gLemurianShipDeck_JennaLooksAround);
  Actor_SetScript(ACTOR_SHEBA, gLemurianShipDeck_ShebaLooksAround);
  Actor_SetScript(ACTOR_KRADEN, gLemurianShipDeck_KradenLooksAround);
  Event_StartScreenTransition();
  gLemurianShipDeck_WaveScroll = 0;
  Task_AddFrame(LemurianShipDeck_ScrollWaves, TASK_PASS_FRAME);
  gLemurianShipDeck_BobPhase = 0;
  Task_AddFrame(LemurianShipDeck_BobDeck, TASK_PASS_FRAME);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(542, 44, 396, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(80);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
  FieldEvent_SetMessageId((s32)MsgTakeTheTiller);
  // "Take the tiller, Felix!"
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_ShowEmote(gMainRoutineState.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  gFieldState->message++;
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHWEST, 20);
  // "If you hope to reach Lemuria, you'll have to learn a few things..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  FieldActor_Stop(ACTOR_JENNA);
  FieldActor_Stop(ACTOR_KRADEN);
  FieldActor_Stop(ACTOR_SHEBA);
  System_Wait(1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "Kraden, shouldn't we be heading straight for Lemuria?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, turn);
  // "Well, I suppose if we have to learn to sail, it's better to do it..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "Then I want to go to Lalivero! And Tolbi! Oh! And Vale! Can we go to..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  // "I'd love to see your hometown, Jenna! Shall we? Let's do it! To Vale..."
  Event_OpenMessage(ACTOR_PIERS, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                            0) == 0) {
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    // Yes: "I don't think that's a good idea."
    Event_ShowMessage(ACTOR_KRADEN, 0);
    gFieldState->message++;
  } else {
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    gFieldState->message++;
    // No: "I'm with Felix. I don't think that would be a wise course of..."
    Event_ShowMessage(ACTOR_KRADEN, 0);
  }
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  // "Come, now! Why not?"
  Event_ShowMessage(ACTOR_PIERS, 0);
  // "Think about the roads we have traveled to get where we are..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "I can't... I was never any good at geography."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
  // "Then allow me to explain... The coastline of Angara, the continent..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Oh... You don't think Piers's boat could land there?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  // "And as for Gondowan, I would guess the people from Lalivero..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  // "He's right. Saturos and Menardi went a little wild in Lalivero, and..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHEAST);
  // "Oh... But Sheba can explain the situation for us. Faran and the..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
  // "And then they'd make me stay in Lalivero! Would you just leave me..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  // "Oh yeah... Maybe that wouldn't be such a good idea after all..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  // "Then it's decided!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "What's decided?"
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "The sea is vast... We can go wherever we please! So, let's explore a..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_Jump(ACTOR_SHEBA, 4, 10);
  // "That's a great idea! We've already met werewolves and discovered new..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  FieldActor_Jump(ACTOR_JENNA, 2, 10);
  FieldActor_Jump(ACTOR_JENNA, 4, 10);
  // "Wow! This is going to be great!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  // "Hmm... The seas can be quite dangerous. It won't be my fault if..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  // "To the open sea, Felix!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  gMainRoutineState.movementMode = LEADER_MOVEMENT_SHIP;
  gMainRoutineState.ownerOverride = 1;
  Event_SetFlag(FLAG_SHIP_CAN_SAIL);
  FieldEvent_SetExitRequest(LEMURIAN_SHIP_TILLER_TO_WORLD_MAP_C);
}

/// Sends actor id at a run to where Felix stands, and off the deck.
static inline void LemurianShipDeck_RunToFelix(s32 id) {
  struct Actor *target;

  Actor_SetSpeed(id, 1.2);
  Actor_SetAnimation(id, ANIM_WALK);
  target = Actor_GetById(ACTOR_FELIX);
  if (target != NULL)
    Actor_SetDestination(id, target->x.part[0].integer,
                         target->z.part[0].integer);
  Actor_WaitForMoveById(id);
  Actor_SetPosition(id, 0, 0);
}

/// The first time a whirlpool of the Sea of Time throws the ship back, the
/// party stands round Felix at the tiller as the deck rocks: Sheba cries "Hold
/// on to that tiller!", Piers admits he has never had to find his way back
/// into Lemuria, and Kraden says they will need a way past the currents. The
/// others then run to Felix and the ship is back on the world map.
///
/// It runs from entrance 20; the waves scroll and the deck bobs throughout.
void LemurianShipDeck_HoldOnToTiller(void) {
#if !TLA_PATCH_2
  /* FAKEMATCH(dead): a call that never runs. */
  if (0)
    Task_AddFrame(LemurianShipDeck_BobDeck, TASK_PASS_FRAME);
#endif
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(458, 48, 408, 0);
  FieldEvent_SetMessageId(MsgLemurianShipDeckHoldOnToThatTiller);
  Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(470), toFIXED(392),
                             FACING_EAST);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 0, -24, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, -16, -16, FACING_SOUTHEAST);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, -24, 8, FACING_EAST_NORTHEAST);
  FieldActor_LaunchFromLeader(ACTOR_PIERS, -4, 24, FACING_NORTHEAST);
  LemurianShipDeck_HideWing();
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  gLemurianShipDeck_WaveScroll = 0;
  Task_AddFrame(LemurianShipDeck_ScrollWaves, TASK_PASS_FRAME);
  gLemurianShipDeck_BobPhase = 0;
  Task_AddFrame(LemurianShipDeck_BobDeck, TASK_PASS_FRAME);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTH);
  FieldEvent_Wait(10);
  // "Hold on to that tiller! Do you want us to get swallowed by the waves!?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "I know you're worried, Sheba, but he's doing the best he can..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 5);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
  FieldEvent_Wait(10);
  // "That was an incredibly strong whirlpool, but I doubt it could sink..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
  FieldEvent_Wait(30);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  // "See, Sheba? There's nothing to worry about. Even if we hit a..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTH);
  // "But still... Piers, can't you show Felix how to get past the..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 5);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
  FieldEvent_Wait(10);
  // "Er, yes, well..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTHEAST);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What's wrong, Piers? Lemuria's just beyond these whirlpools, isn't it?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Yeah, I...think so..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_FaceEachOther(ACTOR_SHEBA, ACTOR_JENNA, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "You..."think so"!?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(5);
  // "Aren't you...from Lemuria? Shouldn't you know exactly where it is?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHWEST);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Yes, I'm from Lemuria... I've never had to get back into it before!..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  // "Oh, I see. So that must mean... You don't actually know which..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Don't be silly! I mean... I just need to get my bearings."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Hm... That doesn't sound good."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH);
  FieldEvent_Wait(10);
  // "If Lemuria really is beyond those currents, then we'll need to find..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  // "And until we do..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "We won't be getting into Lemuria."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_FaceEachOther(ACTOR_KRADEN, ACTOR_PIERS, 0);
  Actor_FaceEachOther(ACTOR_SHEBA, ACTOR_JENNA, 0);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(40);
  LemurianShipDeck_RunToFelix(ACTOR_PIERS);
  LemurianShipDeck_RunToFelix(ACTOR_JENNA);
  LemurianShipDeck_RunToFelix(ACTOR_SHEBA);
  LemurianShipDeck_RunToFelix(ACTOR_KRADEN);
  FieldEvent_SetExitRequest(LEMURIAN_SHIP_TILLER_TO_WORLD_MAP_A);
}

/// The deck by the tiller (SCENE_LEMURIAN_SHIP_TILLER): the surf and the band
/// along the hull shimmer, and entrances 10 and 20 play their scenes.
void LemurianShipDeck_SetUpTillerDeck(void) {
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  FieldTask_AddFrame(LemurianShipDeck_RotateSurfColors, TASK_PASS_FRAME);
  FieldTask_AddFrame(LemurianShipDeck_RotateHullBandColors, TASK_PASS_FRAME);
  if (gMainRoutineState.entrance == LEMURIAN_SHIP_TILLER_FROM_WORLD_MAP_C)
    LemurianShipDeck_TakeTheTiller();
  if (gMainRoutineState.entrance ==
      LEMURIAN_SHIP_TILLER_FROM_SEA_OF_TIME_SWIRLS)
    LemurianShipDeck_HoldOnToTiller();
}

s32 Scene_Initialize(void) {
  struct MapBits *map = (struct MapBits *)Ram_HeapSlots->mapGrid;

  map->wading = 1;
  if (gMainRoutineState.next == SCENE_LEMURIAN_SHIP_TILLER) {
    LemurianShipDeck_SetUpTillerDeck();
  } else {
    gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
    Actor_SetAnimation(ACTOR_PUDDLE, PUDDLE_ANIMATION_PUDDLE);
    FlagBlock_PlaceAll(gLemurianShipDeck_Puddles);
    FieldTask_AddFrame(LemurianShipDeck_RotateSurfColors, TASK_PASS_FRAME);
    if (gMainRoutineState.entrance == MAIN_DECK_ARRIVAL_ENTRANCE)
      LemurianShipDeck_ShowArrival();
    else
      FieldTask_AddFrame(LemurianShipDeck_RotateHullBandColors,
                         TASK_PASS_FRAME);
  }
  return 0;
}

/// At sea, by the tiller, the ship lies at no pier: open water is drawn over
/// the pier and the gangplank the map shares with Lemuria's dock, on its ground
/// and overhead layers. Returns 0.
s32 Scene_PrepareMap(void) {
  if (gMainRoutineState.next == SCENE_LEMURIAN_SHIP_TILLER) {
    MapGrid_CopyCellsTo(2, 2, 10, 24, 18, 10);
    MapGrid_CopyCellsTo(0, 62, 26, 62, 3, 8);
    MapGrid_CopyCellsTo(50, 65, 25, 61, 6, 1);
  }
  return 0;
}

/// The first time Felix comes down the sandy path to the East Indra Shore,
/// cells (14, 4) to (19, 4), before the ceremony at the Gabomba Statue: the
/// party gathers, Kraden cries "Look... It's Menardi's ship!", and the view
/// pans over the moored ship to its dragon prow and back. Felix answers whether
/// it could have drifted here; either way they agree they cannot sail it
/// without the orb Saturos had.
///
/// Coming later, after the ceremony, only marks the ship seen
/// (`FLAG_MENARDIS_SHIP_SEEN`).
void LemurianShipDeck_SightMenardisShip(void) {
  s32 answer;
  s32 leader;

  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) != 0) {
    Event_SetFlag(FLAG_MENARDIS_SHIP_SEEN);
    return;
  }
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetSpeed(ACTOR_FELIX, 0.8);
  Actor_WalkToAndWait(ACTOR_FELIX, 264, 136);
  FieldEvent_SetMessageId(MsgLemurianShipDeckLookItsMenardisShip);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, 16, -16, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 32, -16, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, -16, -16, FACING_SOUTH);
  FieldEvent_Wait(80);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  Actor_SetSpeed(ACTOR_KRADEN, 1.6);
  Actor_WalkToAndWait(ACTOR_KRADEN, 300, 152);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  // "Look... It's Menardi's ship!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(200, CAMERA_KEEP, 344, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(600, CAMERA_KEEP, 344, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  FieldCamera_SetSpeed(6.4);
  FieldCamera_MoveTo(264, CAMERA_KEEP, 128, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "It doesn't look like they sailed here, though..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  // "Of course not... They were killed at the lighthouse, weren't they?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  // "Do you think it could have drifted here?"
  Event_OpenMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHEAST, 0);
  answer =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0);
  leader = gMainRoutineState.leaderActorId;
  if (answer == 0) {
    Actor_WaitForAnimationChange(leader);
    Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
    // Yes: "Yes, Felix, That would be logical..."
    Event_ShowMessage(ACTOR_KRADEN, 0);
    gFieldState->message++;
  } else {
    Actor_WaitForAnimationChange(leader);
    FieldEvent_SetMessageId(MsgLemurianShipDeckLookItsMenardisShip + 5);
    // No: "How else could it have come to be here, Felix?"
    Event_ShowMessage(ACTOR_KRADEN, 0);
  }
  Actor_SetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "If we had that thingie Saturos had, we could just take their boat."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  // "For all we know, the orb is at the bottom of the sea...with them..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "Well, there's no point in hanging around here all day."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_Get(ACTOR_KRADEN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_KRADEN, 288, 144);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_KRADEN)->steeringFlags |= ACTOR_FACES_MOTION;
  // "Let's go see about finding ourselves a ship we can use."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  {
    struct Actor *target;

    Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
    target = Actor_Get(ACTOR_FELIX);
    if (target != NULL)
      Actor_SetDestination(ACTOR_KRADEN, target->x.part[0].integer,
                           target->z.part[0].integer);
    Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
    target = Actor_Get(ACTOR_FELIX);
    if (target != NULL)
      Actor_SetDestination(ACTOR_JENNA, target->x.part[0].integer,
                           target->z.part[0].integer);
    Actor_SetAnimation(ACTOR_SHEBA, ANIM_WALK);
    target = Actor_Get(ACTOR_FELIX);
    if (target != NULL)
      Actor_SetDestination(ACTOR_SHEBA, target->x.part[0].integer,
                           target->z.part[0].integer);
  }
  FieldActor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Event_End();
  Event_SetFlag(FLAG_MENARDIS_SHIP_SEEN);
}

/// Felix casts Frost on the puddle on the path west of the ship: it freezes
/// into a pillar of ice to jump across to the wooden box on the ledge, with
/// its Cookie.
void LemurianShipDeck_FreezePuddle(void) {
  FlagBlock_ApplyFlags(gLemurianShipDeck_Puddles);
}

void Actor_PlaceOnActor(s32 id, s32 target);

/// Felix steps onto the deck just east of the cabin door, cells (72, 25) and
/// (72, 26), before Piers has come aboard. The first two times nothing visible
/// happens; the third time Kraden comes out, walks east and says, "Well,
/// there's no point in hanging around here all day. Let's go see about finding
/// ourselves a ship we can use.", and goes back into the party.
///
/// A step counts only after Felix has stepped beside those cells
/// (LemurianShipDeck_StepBesideEastOfDoor). The scene's temporary flags count
/// the steps, and FLAG_KRADEN_WENT_TO_FIND_A_SHIP ends it for good.
void LemurianShipDeck_KradenGoesToFindShip(void) {
  struct Actor *actor;

  if (Event_GetFlag(FLAG_STANDING_EAST_OF_DOOR))
    return;
  if (Event_GetFlag(FLAG_KRADEN_WENT_TO_FIND_A_SHIP))
    return;
  Event_SetFlag(FLAG_STANDING_EAST_OF_DOOR);
  if (Event_GetFlag(FLAG_SECOND_STEP_EAST_OF_DOOR)) {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineState.leaderActorId);
    System_Wait(1);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
    Actor_SetSpeed(ACTOR_KRADEN, 0.8);
    Actor_WalkToAndWait(ACTOR_KRADEN, 416, 428);
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
    FieldEvent_SetMessageId(MsgLemurianShipDeckWellTheresNoPoint);
    // "Well, there's no point in hanging around here all day. Let's go see..."
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
    actor = Actor_Get(gMainRoutineState.leaderActorId);
    if (actor != 0)
      Actor_SetDestination(ACTOR_KRADEN, actor->x.part[0].integer,
                           actor->z.part[0].integer);
    FieldActor_WaitForMoveById(ACTOR_KRADEN);
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    Event_SetFlag(FLAG_KRADEN_WENT_TO_FIND_A_SHIP);
    Event_End();
  } else if (Event_GetFlag(FLAG_FIRST_STEP_EAST_OF_DOOR)) {
    Event_SetFlag(FLAG_SECOND_STEP_EAST_OF_DOOR);
  } else {
    Event_SetFlag(FLAG_FIRST_STEP_EAST_OF_DOOR);
  }
}

/// Nothing visible happens as Felix steps beside the cells east of the cabin
/// door, before Piers has come aboard; his next step back onto them counts
/// toward Kraden's going to find a ship.
void LemurianShipDeck_StepBesideEastOfDoor(void) {
  GameFlag_Clear(FLAG_STANDING_EAST_OF_DOOR);
}

void Actor_StartScript(struct Actor *actor, const void *data);

/// A task: as Piers opens the cabin door, sparkles drift in from around the
/// Black Crystal toward it.
void LemurianShipDeck_EmitSparkle(void) {
  s32 position[3];
  struct Actor *actor;

  SET_VECTOR(position, toFIXED(360), toFIXED(64), toFIXED(424));
  FieldVector_AddPolarOffset(32, Random16(), position);
  actor =
      FieldActor_Create(SPRITE_GLINT, position[0], position[1], position[2]);
  if (actor != NULL) {
    actor->motionFlags = 0;
    actor->speedLimit = toFIXED(2);
    actor->acceleration = toFIXED(1) / 2;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SelectAnimation(actor, 0);
    FieldActor_SetMoveTarget(actor, 360, 64, 424);
    Actor_StartScript(actor, gLemurianShipDeck_SparkleFades);
  }
}

/// The Black Crystal, drawn from its item icon, as an actor on the deck.
struct Actor *LemurianShipDeck_CreateBlackCrystal(void) {
  struct Actor *actor = (struct Actor *)Import_ActorCreate(
      SPRITE_UNSEEN_MARKER, toFIXED(360), toFIXED(64), toFIXED(424));
  u8 *buffer = 0;
  struct FieldSprite *sprite;

  if (actor == 0)
    return 0;
  sprite = actor->sprite;
  sprite->fullColor = 0;
  sprite->palette = 0;
  sprite->priority = 1;
  actor->motionFlags = 0;
  actor->retainSpriteCache = ACTOR_KEEP_SPRITE_VRAM;
  sprite->flags = 0;
  sprite->count = 0;
  buffer = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
  Icon_DrawItem(ITEM_BLACK_CRYSTAL);
  VramCache_Load(sprite->slot, 128, ((IconTransfer *)buffer)->tiles);
  HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
  return actor;
}

/// The first time Felix comes to the cabin door on the ship's deck, cell
/// (22, 25), with Piers in the party: Piers jumps for joy, "At last, I'm back
/// on my ship.", holds up the Black Crystal before the door, and as light
/// gathers the screen glows red and flashes white and the door opens. "To set
/// sail, we must go down to the power chamber belowdecks."
///
/// It sets FLAG_PIERS_BACK_ON_HIS_SHIP.
void LemurianShipDeck_ReturnToShip(void) {
  struct Actor *crystal;
  void (*sparkles)(void);
  struct Actor *actor;

  if (Event_GetFlag(FLAG_PIERS_BACK_ON_HIS_SHIP) != 0)
    return;
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)) == 0)
    return;
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_PlaceOnActor(ACTOR_PIERS, gMainRoutineState.leaderActorId);
  System_Wait(1);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_SetSpeed(ACTOR_PIERS, 1.6);
  Actor_WalkToAndWait(ACTOR_PIERS, 412, 424);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHWEST);
  FieldEvent_SetMessageId((s32)MsgLemurianShipDeckAtLastImBack);
  FieldActor_Jump(ACTOR_PIERS, 4, 10);
  // "At last, I'm back on my ship."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
  // "Finally, I can return to my beloved Lemuria."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_WalkToAndWait(ACTOR_PIERS, 386, 424);
  Actor_FaceActor(ACTOR_PIERS, gMainRoutineState.leaderActorId, 0);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_NOD);
  // "Wait there a moment."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
  Actor_SetScript(gMainRoutineState.leaderActorId,
                  gLemurianShipDeck_ReturnToShipScript);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_WalkToAndWait(ACTOR_PIERS, 360, 424);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
  Event_PlaySound(SE_GEM_SET);
  crystal = LemurianShipDeck_CreateBlackCrystal();
  FieldEvent_Wait(20);
  Event_PlaySound(SE_SHIMMER);
  sparkles = LemurianShipDeck_EmitSparkle;
  Task_Add(sparkles, TASK_PASS_FRAME);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_Interpolate(20);
  FieldEvent_Wait(20);
  PaletteFade_ApplyTarget(PALETTE_TINT_RED, PALETTES_BG);
  PaletteFade_Interpolate(20);
  FieldEvent_Wait(20);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(20);
  FieldEvent_Wait(20);
  LemurianShipDeck_DrawCabinDoorOpen();
  Event_SetFlag(FLAG_CABIN_DOOR_OPEN);
  FieldTask_Remove(sparkles);
  if (crystal != 0)
    Actor_Release(crystal);
  FieldEvent_Wait(20);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_Interpolate(20);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
  // "To set sail, we must go down to the power chamber belowdecks."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  // "Let's head down."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_WALK);
  actor = Actor_GetById(gMainRoutineState.leaderActorId);
  if (actor != 0)
    Actor_SetDestination(ACTOR_PIERS, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  FieldActor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Event_SetFlag(FLAG_PIERS_BACK_ON_HIS_SHIP);
  Event_End();
}

/// Felix comes down the sandy path to the shore, cells (12, 5) to (21, 5),
/// with Piers after the ceremony at the Gabomba Statue: "My ship! At long
/// last... I have returned to my ship." Kraden asks him to take the party to
/// Lemuria, and after Felix's two answers Piers agrees: "I will take you to
/// Lemuria." He then asks to stop in Madra first to tell the mayor he has his
/// orb back.
///
/// It runs until the farewells in Madra and sets FLAG_PIERS_SETTING_SAIL.
void LemurianShipDeck_ReachPiersShip(void) {
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) == 0)
    return;
  if (Event_GetFlag(FLAG_MADRA_FAREWELL_SAID) != 0)
    return;
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId(MsgLemurianShipDeckMyShipAtLongLast);
  Actor_WalkTo(ACTOR_FELIX, 264, 176);
  FieldActor_WaitForMoveById(ACTOR_FELIX);
  FieldActor_LaunchFromLeader(ACTOR_PIERS, 16, 8, FACING_SOUTH);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(288, CAMERA_KEEP, 208, 1);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 32, 0, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 48, -8, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, -16, -8, FACING_SOUTH);
  FieldEvent_Wait(40);
  Actor_SetSpeed(ACTOR_PIERS, 2.4);
  Actor_WalkToAndWait(ACTOR_PIERS, 280, 224);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  FieldActor_Jump(ACTOR_PIERS, 4, 10);
  FieldActor_Jump(ACTOR_PIERS, 4, 20);
  // "My ship! At long last... I have returned to my ship."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "Well, Piers?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(288, CAMERA_KEEP, 184, 1);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_WalkByAndWait(ACTOR_PIERS, 0, -16);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  // "I'm thrilled... I'm just happy to be back with my ship."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  // "Uh, that's not what Kraden's asking..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  // "You forgot, didn't you, Piers? When we talked in Kibombo? Remember..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  // "He wanted you to take us to your home... To Lemuria."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  // "Remember how we said we'd talk about it later? Well, it's later now..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHWEST);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "Oh... Is it? I suppose we have to talk about it sooner or later. You..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "Piers...if you do not wish to take us, you do not have to. Just tell..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  // "And if not, well, we'll just have to get ourselves a boat and find..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  // "Right, Felix?" Yes: "What's with the guilt trip? Piers hasn't..." No:
  // "Sheba is right! You guys just let Piers give..."
  Event_OpenMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    // Yes: "What's with the guilt trip? Piers hasn't even given us his..."
    Event_ShowMessage(ACTOR_JENNA, 0);
    // "Kraden! Felix! Stop trying to pressure Piers!"
    Event_ShowMessage(ACTOR_SHEBA, 0);
    gFieldState->message += 2;
  } else {
    gFieldState->message += 2;
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    // No: "Sheba is right! You guys just let Piers give us his answer!"
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
    Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
    // "I agree with Felix. We shouldn't try to pressure Piers, Kraden."
    Event_ShowMessage(ACTOR_SHEBA, 0);
  }
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTHEAST);
  // "But that's not what I was°"
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                    0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "Don't worry, Sheba... I've actually been thinking about this quite a..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
  // "I just never got the opportunity to mention it before."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "Well? Well!?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "It's not nice to test the patience of an old man. Will you take us..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "We will go to Lemuria, Kraden."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  // "I figured it was too much to..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 20);
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_EAST);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH);
  // "What did you just say!?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  // "I will take you to Lemuria."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "Kraden? Are you OK?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  FieldActor_StartRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  // "Is he OK!?"
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  FieldActor_StartRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "Piers! Are you... Are you serious?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  // "After what we've been through, it would be wrong to deny you. That's..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "What's what made up your mind?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "The fact that you are truly good people."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHWEST);
  // "You have gone to great trouble to help one whose problems needn't..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  // "Witnessing that kindness... It made me want to help, too."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  // "Piers, you..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHWEST);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "No, Jenna. Please, don't say a word. I've made up my mind."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
  // "I shall be leaving immediately, but I want you to come, Kraden."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHWEST);
  // "Let us set sail, Felix! To my ship! And the Eastern Sea!"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "So at long last, I will finally reach Lemuria..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  // "I wonder what kind of advanced civilization we'll find in Lemuria..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  // "I'm actually excited to find out!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_WalkByAndWait(ACTOR_PIERS, -16, 0);
  FieldEvent_Wait(40);
  Actor_WalkByAndWait(ACTOR_PIERS, -16, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_WalkByAndWait(ACTOR_JENNA, 0, 16);
  // "What's wrong, Piers?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  // "It just occurred to me... The mayor of Madra and his wife must be..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 20);
  // "We should go tell them that I've retrieved my orb. Then they..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
  // "Is that acceptable, Felix? Can we stop by Madra briefly before we..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  // "What do you say, Felix? Shall we go see the mayor?"
  Event_OpenMessage(ACTOR_KRADEN, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 1) {
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    // No: "Don't worry about me. I've waited this long. I can wait a..."
    Event_ShowMessage(ACTOR_KRADEN, 0);
    gFieldState->message += 1;
  } else {
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    gFieldState->message += 1;
    // Yes: "I have no objection, so why not? Let's go."
    Event_ShowMessage(ACTOR_KRADEN, 0);
  }
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "But we must depart as soon as we're done at the mayor's house, Piers."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  // "Oh, thank you!"
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetSpeed(ACTOR_PIERS, 1.2);
  Actor_SetScript(ACTOR_JENNA, gLemurianShipDeck_ReachPiersShipScript);
  Actor_SetScript(ACTOR_KRADEN, gLemurianShipDeck_ReachPiersShipScript);
  Actor_SetScript(ACTOR_SHEBA, gLemurianShipDeck_ReachPiersShipScript);
  Actor_RunScriptAndWait(ACTOR_PIERS, gLemurianShipDeck_ReachPiersShipScript);
  FieldEvent_Wait(20);
  Event_SetFlag(FLAG_PIERS_SETTING_SAIL);
  Event_End();
}

/// A wading splash rises at an actor's feet every eighth frame while it walks.
void LemurianShipDeck_EmitSplash(struct Actor *actor) {
  struct Actor *spawned;

  if ((gFrameCount & 7) == 0) {
    spawned = FieldActor_Create(SPRITE_WADING_SPLASH, actor->x.fixed,
                                actor->y.fixed, actor->z.fixed);
    if (spawned != 0) {
      Actor_SetSpriteFlags(spawned, 0);
      Actor_SelectAnimation(spawned, 1);
      Actor_StartScript(spawned, gLemurianShipDeck_SplashFades);
    }
  }
}

/// Felix wades to the foot of the rope ladder on the ship's side, cell
/// (27, 27), with Piers in the party before the farewells in Madra: Piers
/// splashes out beside him, "Then let's waste no time! We should set out for
/// Madra at once!", and rejoins the party.
void LemurianShipDeck_SetOutForMadra(void) {
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    struct Actor *actor;

    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    Actor_SetSpeed(ACTOR_FELIX, 0.8);
    Actor_Get(ACTOR_PIERS)->update = (ObjectUpdate)LemurianShipDeck_EmitSplash;
    FieldActor_LaunchFromLeader(ACTOR_PIERS, 16, 0, FACING_WEST);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PIERS), 0);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
    FieldEvent_SetMessageId(MsgLemurianShipDeckThenLetsWasteNoTime);
    FieldActor_WaitForMoveById(ACTOR_PIERS);
    Actor_Get(ACTOR_PIERS)->update = NULL;
    // "Then let's waste no time! We should set out for Madra at once!"
    Event_ShowMessage(ACTOR_PIERS, 0);
    Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
    Actor_Get(gMainRoutineState.leaderActorId)->update =
        (ObjectUpdate)LemurianShipDeck_EmitSplash;
    Actor_WalkByAndWait(ACTOR_FELIX, 0, 16);
    Actor_Get(gMainRoutineState.leaderActorId)->update = NULL;
    Actor_Get(ACTOR_PIERS)->update = (ObjectUpdate)LemurianShipDeck_EmitSplash;
    Actor_SetAnimation(ACTOR_PIERS, ANIM_WALK);
    actor = Actor_Get(ACTOR_FELIX);
    if (actor != 0)
      Actor_SetDestination(ACTOR_PIERS, actor->x.part[0].integer,
                           actor->z.part[0].integer);
    FieldActor_WaitForMoveById(ACTOR_PIERS);
    Actor_SetPosition(ACTOR_PIERS, 0, 0);
    Actor_Get(ACTOR_PIERS)->update = NULL;
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PIERS), 1);
    Event_End();
  }
}

// clang-format off
/// The puddle on the path west of the ship, by actor id and flag.
s16 gLemurianShipDeck_Puddles[] = {
    ACTOR_PUDDLE, FLAG_PUDDLE_FROZEN, -1, 0,
};

/// The party walking off across the shore, into Felix's party, once Piers has
/// agreed to sail for Lemuria.
s32 gLemurianShipDeck_ReachPiersShipScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(263, 0, 176),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

/// Felix stepping back from the cabin door and turning to watch Piers open
/// it.
s32 gLemurianShipDeck_ReturnToShipScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(368, 0, 448),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_END,
};

/// Jenna looking about the deck as the ship first sails.
s32 gLemurianShipDeck_JennaLooksAround[] = {
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(70),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Sheba looking about the deck as the ship first sails.
s32 gLemurianShipDeck_ShebaLooksAround[] = {
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(70),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Kraden looking about the deck as the ship first sails.
s32 gLemurianShipDeck_KradenLooksAround[] = {
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_WAIT(70),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(LemurianShipMainDeck),
    SCENE_EXIT(LEMURIAN_SHIP_MAIN_DECK_TO_LEMURIAN_SHIP_CABINS, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_MAIN_DECK),
    SCENE_EXIT(LEMURIAN_SHIP_MAIN_DECK_TO_LEMURIAN_SHIP_CABINS, WorldMap, WORLD_MAP_FIRST_VOYAGE),
    SCENE_EXIT(LEMURIAN_SHIP_MAIN_DECK_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_LEMURIAN_SHIP_MAIN_DECK_NORTH),
    SCENE_EXITS(LemurianShipTiller),
    SCENE_EXIT(LEMURIAN_SHIP_TILLER_TO_WORLD_MAP_A, WorldMap, WORLD_MAP_FROM_SEA_OF_TIME_SWIRLS),
    SCENE_EXIT(LEMURIAN_SHIP_TILLER_TO_WORLD_MAP_B, WorldMap, WORLD_MAP_FROM_LEMURIAN_SHIP_MAIN_DECK_NORTH),
    SCENE_EXIT(LEMURIAN_SHIP_TILLER_TO_WORLD_MAP_C, WorldMap, WORLD_MAP_FROM_LEMURIAN_SHIP_TILLER_B),
    SCENE_EXITS_END,
};

/// The East Indra Shore's actors.
struct ActorPlacement gLemurianShipDeck_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The tiller deck's actors.
struct ActorPlacement gLemurianShipDeck_TillerPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The tiller deck's triggers: none.
struct Trigger gLemurianShipDeck_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// The East Indra Shore's triggers.
struct Trigger gLemurianShipDeck_MainDeckTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), LEMURIAN_SHIP_MAIN_DECK_TO_LEMURIAN_SHIP_CABINS, CONDITION_ALWAYS, EVENT(LemurianShipDeck_EnterCabin)},
    {TRIGGER_EXIT, LEMURIAN_SHIP_MAIN_DECK_TO_WORLD_MAP_NORTH, CONDITION_ALWAYS, LEMURIAN_SHIP_MAIN_DECK_TO_WORLD_MAP_NORTH},
    {TRIGGER_STEP, CELL_BEHAVIOR_HEAD_OF_PATH, FLAG_MENARDIS_SHIP_SEEN, EVENT(LemurianShipDeck_SightMenardisShip)},
    {TRIGGER_STEP, CELL_BEHAVIOR_TOP_OF_SHORE, FLAG_PIERS_SETTING_SAIL, EVENT(LemurianShipDeck_ReachPiersShip)},
    {TRIGGER_STEP, CELL_BEHAVIOR_FOOT_OF_LADDER, FLAG_MADRA_FAREWELL_SAID, EVENT(LemurianShipDeck_SetOutForMadra)},
    {TRIGGER_STEP, LEMURIAN_SHIP_MAIN_DECK_TO_LEMURIAN_SHIP_CABINS, FLAG_PIERS_BACK_ON_HIS_SHIP, EVENT(LemurianShipDeck_ReturnToShip)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_CABIN_DOOR, FLAG_PIERS_BACK_ON_HIS_SHIP, EVENT(LemurianShipDeck_TryHeldDoor)},
    {TRIGGER_STEP, CELL_BEHAVIOR_EAST_OF_DOOR, FLAG_PIERS_BACK_ON_HIS_SHIP, EVENT(LemurianShipDeck_KradenGoesToFindShip)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BESIDE_EAST_OF_DOOR, FLAG_PIERS_BACK_ON_HIS_SHIP, EVENT(LemurianShipDeck_StepBesideEastOfDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT(LemurianShipDeck_FreezePuddle)},
    {TRIGGER_TABLE_END},
};

/// A sparkle drifting in to the Black Crystal, gone as it arrives.
u32 gLemurianShipDeck_SparkleFades[] = {
    SC_WAIT(10),
    SC_SYNC,
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

/// A wading splash, gone after a quarter of a second.
u32 gLemurianShipDeck_SplashFades[] = {
    SC_WAIT(15),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};
// clang-format on
