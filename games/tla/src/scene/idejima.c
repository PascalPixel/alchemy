/// @file
/// Idejima, the peninsula at the end of the road west of Venus Lighthouse,
/// where the story of The Lost Age begins.
///
/// The scene is entered by its story's three cutscenes, one entrance each, and
/// then by the party washed ashore:
///
/// - Jenna and Kraden reach Idejima and find Menardi's ship without the orb
///   that moves it. Alex joins them, the beacon is lit, and the tremor tears
///   Idejima away from Gondowan (Idejima_BreakAwayFromGondowan, entrance 11).
/// - Adrift, Alex shows Jenna and Kraden that Felix and Sheba have washed up on
///   the island (Idejima_FindFelixAndSheba, entrance 12).
/// - Sheba wakes, land comes in sight, and a tidal wave carries the island
///   onto it (Idejima_RideTheTidalWave, entrance 13).
/// - Felix wakes on the grass of the island, now run aground on the coast of
///   Indra near Daila (Idejima_FelixWakes), with Jenna, Sheba and Kraden still
///   lying where the wave left them (Idejima_LayDownTheOthers). Each wakes when
///   he talks to them (Idejima_WakeKraden, Idejima_WakeJenna,
///   Idejima_WakeSheba), or runs after him if he heads west without them
///   (Idejima_CatchUpWithFelix). Once all three have joined, they wonder where
///   Alex has gone (Idejima_WonderWhereAlexHasGone), and the island's west edge
///   leads out onto the world map.
///
/// While the island drifts, the sea around it ripples
/// (Idejima_UpdateWaveLines, Idejima_ApplyWaveLines).

#include "battle/battle.h"
#include "field/field_sequence_event.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "macros.h"
#include "field/field_event.h"
#include "field/scene_tables.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
#include "system/ram_buffer.h"
#include "field/event_inline.h"
#include "field/call.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "system/dma.h"
#include "field/map_grid.h"
#include "field/sprite.h"
#include "shared/tables.h"
#include "system/input.h"
#include "system/task.h"
#include "scene_ids.h"
#include "flags.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "text/msg_ids.h"
#include "sprite_ids.h"

/// Set once the leader has been walked back from the edge of the map by
/// Idejima_CatchUpWithFelix, so the next one to catch up finds him there.
#define FLAG_IDEJIMA_LEADER_WALKED_BACK FLAG_SCENE_TEMP_00

/// The entrances the triggers are chosen by: two at the foot of the mountain,
/// where only the unseen door answers, and the two of the island run aground.
enum IdejimaEntrance {
  /// At the foot of the mountain, cell (11, 10), with the unseen door north.
  /// No exit table or event in the game leads here by it; the unseen door
  /// leads back by ENTRANCE_FROM_DOOR_AGAIN.
  ENTRANCE_FROM_DOOR = 1,
  /// The island run aground, where Felix wakes after the tidal wave.
  ENTRANCE_WAKE = 2,
  ENTRANCE_FROM_DOOR_AGAIN = 3,
  /// Back onto the island from the world map, at its west edge.
  ENTRANCE_BACK_FROM_WORLD_MAP = 4,
  /// Two of the story's three scenes before the shore, each played on arriving
  /// by its own entrance (Scene_Initialize); the third is the title's
  /// IDEJIMA_RIDE_THE_TIDAL_WAVE.
  ENTRANCE_BREAK_AWAY = 11,
  ENTRANCE_FIND_FELIX_AND_SHEBA = 12
};

/// The unseen door, which leads back out at ENTRANCE_FROM_DOOR_AGAIN.
enum { EXIT_DOOR = 3 };

/// The cells the triggers answer, by their behaviour in the map.
enum IdejimaCellBehavior {
  /// The unseen door at the foot of the mountain, cell (11, 10).
  CELL_BEHAVIOR_DOOR = 3,
  /// The line across the grass at (5..6, 44..47), where the party talks about
  /// Alex.
  CELL_BEHAVIOR_WHERE_ALEX_WENT = 8,
  /// The line across the grass at (8..10, 44..47), east of it, where the
  /// sleepers catch up with Felix.
  CELL_BEHAVIOR_CATCH_UP = 9,
  /// Stamped under Kraden, Jenna and Sheba where each lies asleep, from the
  /// spare cells at (38..39, 32..34).
  CELL_BEHAVIOR_BY_KRADEN = 10,
  CELL_BEHAVIOR_BY_JENNA,
  CELL_BEHAVIOR_BY_SHEBA
};

/// The scene's placed actors, numbering the rows of gIdejimaPlacements; its
/// rows of Jenna's and Sheba's sprites place ACTOR_JENNA and ACTOR_SHEBA.
enum IdejimaActorId {
  ACTOR_KRADEN = ACTOR_FIRST_PLACED,
  /// Menardi's ship, moored in a cove of the peninsula.
  ACTOR_SHIP,
  ACTOR_ALEX,
  /// An actor with no picture that speaks the lines no one says, above Felix
  /// as he wakes.
  ACTOR_NARRATOR
};

/// Kraden's sprite lying asleep on his side.
enum { KRADEN_ANIMATION_ASLEEP = 5 };

/// Animation 18 of Sheba's sheet sits her up on the ground; of Jenna's, it
/// lays her on her side.
enum { ANIM_SHEBA_SIT_UP = 18, ANIM_JENNA_LIE_ON_SIDE = 18 };

/// Menardi's ship rocking every 10 frames, quicker than at rest, as the island
/// drifts.
enum { SHIP_ANIMATION_ROCK_FAST = 2 };

extern struct SceneEntrance gSceneEntrances[];
extern u32 gIdejimaExits[];
extern struct Trigger gIdejimaEvents[];
extern struct Trigger gIdejimaEventsFromDoor[];
extern struct Trigger gIdejimaEventsWake[];
extern struct ActorPlacement gIdejimaPlacements[];
extern s32 gIdejima_StretchScript[];
extern u32 gIdejima_KradenStepsAsideScript[];
extern u32 gIdejima_JoinTheLeaderScript[];
extern u32 gIdejima_GoAfterAlexScript[];
extern u32 gIdejima_KradenToTheShipScript[];
extern u32 gIdejima_JennaToTheShipScript[];
extern u32 gIdejima_AlexLeadsTheWayScript[];
extern u32 gIdejima_KradenFollowsAlexScript[];
extern u32 gIdejima_JennaFollowsAlexScript[];
extern u32 gIdejima_KradenRunsToFelixScript[];
extern u32 gIdejima_JennaRunsToFelixScript[];
extern u32 gIdejima_JennaToTheShoreScript[];
extern u32 gIdejima_ShebaToTheShoreScript[];
extern u32 gIdejima_KradenToTheShoreScript[];
extern u32 gIdejima_FelixToTheShoreScript[];

/// The wave scroll's two tables of BG3 row offsets, one for each frame in turn.
s16 gIdejima_WaveLines[2][160] BSS_DATA ALIGNED(16);
/// The frame count the wave scroll's phase follows.
s32 gIdejima_WavePhase BSS_DATA;
/// Which of gIdejima_WaveLines the next frame computes.
s16 gIdejima_WaveBuffer BSS_DATA;

void Idejima_ResetWaveLines(void);
void Idejima_ApplyWaveLines(void);
void Idejima_UpdateWaveLines(void);
void Idejima_UseDriftPalette(void);
void DriftScene_Prepare(void);
void Idejima_LayDownTheOthers(void);
void Idejima_WakeKraden(void);
void Idejima_WakeJenna(void);
void Idejima_WakeSheba(void);
void Idejima_CatchUpWithFelix(void);
void Idejima_WonderWhereAlexHasGone(void);
void Party_RestoreHpPp(s32 a);
void Actor_PlaceOnActor();
/// Counts the answer just given (Event_CountAnswerFar).
void Idejima_CountAnswer();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gIdejimaExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gIdejimaPlacements;
}

const struct Trigger *Scene_GetTriggers(void) {
  switch (gMainRoutineState.entrance) {
  case ENTRANCE_FROM_DOOR:
  case ENTRANCE_FROM_DOOR_AGAIN:
    return gIdejimaEventsFromDoor;
  case ENTRANCE_WAKE:
  case ENTRANCE_BACK_FROM_WORLD_MAP:
    return gIdejimaEventsWake;
  default:
    return gIdejimaEvents;
  }
}

/// Stands a sleeper up where it lay, with a little hop.
void Idejima_GetUp(s32 id) {
  struct Actor *actor = Actor_Get(id);

  Actor_SetAnimation(id, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_Get(id), 1);
  actor->x.fixed -= toFIXED(12);
  ((struct FieldSprite *)actor->sprite)->rotation = 0;
  System_Wait(1);
  Actor_SetSpriteFlags(Actor_Get(id), 1);
  actor->velocityY = toFIXED(4);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  System_Wait(40);
}

/// Talking to Kraden, asleep on the grass north-east of where Felix woke, or
/// reading his mind: he groans awake, marvels that the wave carried them to
/// solid land and joins the party.
void Idejima_WakeKraden(void) {
  s32 message;
  struct Actor *actor;
  struct Actor *leader;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  message = (s32)MsgIdejimaGroan;
  FieldEvent_SetMessageId(message);
  // "Oooog..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  // "Oh... Felix..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  MapGrid_CopyCellAttributes(38, 31, 2, 1, 25, 42);
  actor = Actor_Get(ACTOR_KRADEN);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_KRADEN), 1);
  System_Wait(1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_KRADEN), 1);
  actor->velocityY = toFIXED(4);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  System_Wait(40);
  // "Ouch..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceActor(ACTOR_KRADEN, gMainRoutineState.leaderActorId, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "That was quite a blow..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "What happened when that wave hit?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_KRADEN, 416, 680);
  // "Felix, look!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 20);
  Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "That wave carried us to the continent!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceActor(ACTOR_KRADEN, gMainRoutineState.leaderActorId, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(gMainRoutineState.leaderActorId, ACTOR_KRADEN, 0);
  // "We've hit solid land! Let's go, Felix!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_KRADEN, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Message_ShowCentered(message + 8, 1);
  Event_SetFlag(FLAG_IDEJIMA_KRADEN_JOINED);
  Event_End();
}

/// Talking to Jenna, asleep on the grass west of where Felix woke, or reading
/// her mind: she wakes, asks whether he has checked on the others and joins
/// the party.
void Idejima_WakeJenna(void) {
  struct Actor *actor;
  struct Actor *leader;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId(MsgIdejimaAwakeningExclamation);
  // "What..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "What am I doing down here?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  MapGrid_CopyCellAttributes(38, 31, 2, 1, 17, 46);
  actor = Actor_Get(ACTOR_JENNA);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNA), 1);
  System_Wait(1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNA), 1);
  actor->velocityY = toFIXED(4);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  System_Wait(40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Oh... I must have blacked out when the wave hit..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 40);
  FieldActor_Jump(ACTOR_JENNA, 4, 20);
  // "Hey! We've hit land!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceActor(ACTOR_JENNA, gMainRoutineState.leaderActorId, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(gMainRoutineState.leaderActorId, ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  // "That wave must have carried us here! What luck!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
  FieldEvent_Wait(40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  // "Have you checked everyone else? Are they all right?"
  Event_OpenMessage(ACTOR_JENNA, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                            0) == 0) {
    Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
    // "Great! Then let's go!"
    Event_ShowMessage(ACTOR_JENNA, 0);
  } else {
    gFieldState->message += 2;
    // "Then let's go find them and get off this island!"
    Event_ShowMessage(ACTOR_JENNA, 0);
  }
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_JENNA, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  FieldActor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  FieldEvent_AddPartyMember(ACTOR_JENNA, 1);
  Event_SetFlag(FLAG_IDEJIMA_JENNA_JOINED);
  Event_End();
}

/// Talking to Sheba, asleep on the grass north-west of where Felix woke: she
/// wakes, wonders where they are and joins the party.
void Idejima_WakeSheba(void) {
  struct Actor *leader;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_SetMessageId(MsgIdejimaSecondGroan);
  // "Ohhh..."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 40);
  // "Nnng."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_SHEBA_SIT_UP);
  // "The wave!"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  MapGrid_CopyCellAttributes(38, 31, 2, 1, 14, 40);
  Actor_GetById(ACTOR_SHEBA)->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_Jump(ACTOR_SHEBA, 4, 0);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SHEBA), 1);
  Event_Wait(20);
  Actor_TurnToFaceEachOther(ACTOR_SHEBA, gMainRoutineState.leaderActorId, 0);
  // "What happened to the tidal wave?"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  // "I thought we were done for, but we're all fine..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "And we've run aground!"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceActorById(ACTOR_SHEBA, gMainRoutineState.leaderActorId, 0);
  Event_Wait(20);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  // "That tidal wave turned out to be a stroke of luck!"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
  // "I wonder where we are..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.6);
  Actor_WalkToAndWait(ACTOR_SHEBA, 212, 680);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 20);
  // "We should take a look around!"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_SHEBA, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Event_AddPartyMember(ACTOR_SHEBA, 1);
  GameFlag_Set(FLAG_IDEJIMA_SHEBA_JOINED);
  Event_End();
}

/// Walking north onto the bare grass at the foot of the mountain, cell
/// (11, 10), plays a door's sound and leaves through a door that is not drawn,
/// to come straight back out on the same spot.
///
/// Only entrances 1 and 3 answer this cell, and the door itself is the only
/// way the game enters by 3, so a player who arrives with the story never
/// meets it.
void Idejima_EnterDoor(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(EXIT_DOOR);
}

/// Walks Felix back from the edge of the shore, once a visit.
void Idejima_WalkLeaderBack(void) {
  if (!Event_GetFlag(FLAG_IDEJIMA_LEADER_WALKED_BACK)) {
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 152, 756);
    Event_SetFlag(FLAG_IDEJIMA_LEADER_WALKED_BACK);
  }
}

/// Walking west across the grass towards the island's edge, cells
/// (8..10, 44..47), while any of the others is still asleep: Felix is walked
/// back a little, and each sleeper in turn wakes, runs after him, scolds him
/// for leaving them behind and joins the party.
void Idejima_CatchUpWithFelix(void) {
  struct Actor *leader;

  if (Event_GetFlag(FLAG_IDEJIMA_KRADEN_JOINED) == 0) {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    Idejima_WalkLeaderBack();
    FieldEvent_SetMessageId((s32)MsgIdejimaHeyFelix);
    // "Hey, Felix!"
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Actor_SetPosition(ACTOR_JENNA, 0, 0);
    Actor_SetPosition(ACTOR_SHEBA, 0, 0);
    FieldCamera_SetSpeed(0.6);
    FieldCamera_MoveTo(168, CAMERA_KEEP, 721, 1);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHEAST, 0);
    MapGrid_CopyCellAttributes(38, 31, 2, 1, 25, 42);
    Idejima_GetUp(ACTOR_KRADEN);
    Actor_SetPosition(ACTOR_KRADEN, 312, 666);
    System_Wait(1);
    Actor_SetSpeed(ACTOR_KRADEN, 1.6);
    Actor_WalkToAndWait(ACTOR_KRADEN, 212, 680);
    Actor_WalkToAndWait(ACTOR_KRADEN, 192, 712);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
    FieldActor_WaitForScriptEndById(ACTOR_KRADEN);
    Actor_RunScriptAndWait(ACTOR_KRADEN, gIdejima_StretchScript);
    FieldActor_Jump(ACTOR_KRADEN, 4, 20);
    // "You weren't planning on leaving me, were you?"
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    // "Hm?"
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    // "Well! It's land!"
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
    // "The wave carried us back ashore!"
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
    // "I assume you merely...forgot about me in your excitement?"
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    // "Understandable, but you should have waited for me, Felix."
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Actor_WalkToAndWait(ACTOR_KRADEN, 172, 756);
    Actor_FaceEachOther(ACTOR_KRADEN, gMainRoutineState.leaderActorId, 0);
    Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
    // "Nevertheless, let us be on our way!"
    Event_ShowMessage(
        SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
    Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
    leader = Actor_GetById(gMainRoutineState.leaderActorId);
    if (leader != 0)
      Actor_SetDestinationById(ACTOR_KRADEN, leader->x.part[0].integer,
                               leader->z.part[0].integer);
    FieldActor_WaitForMoveById(ACTOR_KRADEN);
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    // "Kraden has joined your party."
    Message_ShowCentered((s32)MsgIdejimaHeyFelix + 8, 1);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 20);
    Event_SetFlag(FLAG_IDEJIMA_KRADEN_JOINED);
    Event_End();
  }
  if (Event_GetFlag(FLAG_IDEJIMA_JENNA_JOINED) == 0) {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    Idejima_WalkLeaderBack();
    Actor_SetPosition(ACTOR_JENNA, 312, 666);
    System_Wait(1);
    FieldEvent_SetMessageId((s32)MsgIdejimaCantBelieveYouTwo);
    // "I can't believe you two!"
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_SetPosition(ACTOR_SHEBA, 0, 0);
    FieldCamera_SetSpeed(0.6);
    FieldCamera_MoveTo(168, CAMERA_KEEP, 721, 1);
    Actor_SetSpeed(ACTOR_KRADEN, 1.6);
    Actor_SetScript(ACTOR_KRADEN, gIdejima_KradenStepsAsideScript);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHEAST, 0);
    MapGrid_CopyCellAttributes(38, 31, 2, 1, 17, 46);
    Idejima_GetUp(ACTOR_JENNA);
    Actor_SetSpeed(ACTOR_JENNA, 1.6);
    Actor_WalkToAndWait(ACTOR_JENNA, 212, 680);
    Actor_WalkToAndWait(ACTOR_JENNA, 192, 712);
    Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
    FieldActor_WaitForScriptEndById(ACTOR_JENNA);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    FieldActor_Jump(ACTOR_JENNA, 4, 20);
    // "How could you!?"
    Event_ShowMessage(ACTOR_JENNA, 0);
    // "Leaving me on that island while you go off exploring on your own!"
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_KRADEN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    // "And here I was, looking everywhere for you!"
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
    // "I'm sorry, my dear... Can you ever forgive us?"
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    // "Oh... All right. Just don't leave me alone like that again..."
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 40);
    // "Anyway, look! That wave must have carried us to a new continent!"
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHWEST, 20);
    Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
    Actor_WalkToAndWait(ACTOR_JENNA, 178, 728);
    Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
    // "Let's go exploring!"
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHEAST, 20);
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
    Actor_SetScript(ACTOR_KRADEN, gIdejima_JoinTheLeaderScript);
    Actor_RunScriptAndWait(ACTOR_JENNA, gIdejima_JoinTheLeaderScript);
    FieldEvent_AddPartyMember(ACTOR_JENNA, 1);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 20);
    Event_SetFlag(FLAG_IDEJIMA_JENNA_JOINED);
    Event_End();
  }
  if (Event_GetFlag(FLAG_IDEJIMA_SHEBA_JOINED) == 0) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    Idejima_WalkLeaderBack();
    Actor_SetPosition(ACTOR_SHEBA, 312, 666);
    System_Wait(1);
    Event_SetMessageId((s32)MsgIdejimaWaitForMe);
    // "Wa-Wait!"
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    FieldCamera_SetSpeed(0.6);
    FieldCamera_MoveTo(168, CAMERA_KEEP, 721, 1);
    Actor_SetSpeed(ACTOR_KRADEN, 1.6);
    Actor_SetScript(ACTOR_KRADEN, (s32)gIdejima_KradenStepsAsideScript);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHEAST, 0);
    MapGrid_CopyCellAttributes(38, 31, 2, 1, 14, 40);
    Actor_GetById(ACTOR_SHEBA)->motionFlags =
        ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    Actor_Jump(ACTOR_SHEBA, 4, 0);
    Actor_SetAnimationById(ACTOR_SHEBA, ANIM_STAND);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_SHEBA), 1);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_SHEBA, 1.6);
    Actor_WalkToAndWait(ACTOR_SHEBA, 212, 680);
    Actor_WalkToAndWait(ACTOR_SHEBA, 192, 712);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
    FieldActor_WaitForScriptEndById(ACTOR_SHEBA);
    // "Aren't you worried about me in the slightest?"
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    // "Did you rescue me from the lighthouse just to leave me for dead on..."
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    // "Now, I'm sure he meant to do no such thing..."
    Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
    // "I wonder..."
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_ShowEmoteById(gMainRoutineState.leaderActorId,
                        EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
    // "Oh, don't give me that sad face... You know I can't stay mad at you..."
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_SetAnimationById(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
    // "After all, you did save me, Felix. I won't soon forget that..."
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    // "But don't leave me behind like that again, OK?"
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 40);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 40);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
    // "So that big wave pushed us aground? Way to go, Nature!"
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    // "What's out here, anyway?"
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_SHAKE_HEAD);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    // "That's what we were about to find out!"
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    // "Let's get started, then!"
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Actor_SetScript(ACTOR_KRADEN, (s32)gIdejima_JoinTheLeaderScript);
    Actor_RunScriptAndWait(ACTOR_SHEBA, (s32)gIdejima_JoinTheLeaderScript);
    Event_AddPartyMember(ACTOR_SHEBA, 1);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 20);
    GameFlag_Set(FLAG_IDEJIMA_SHEBA_JOINED);
    Event_End();
  }
}

/// Walking on west, cells (5..6, 44..47), with all four together: Kraden
/// wonders where Alex has gone, the party decides to follow him to the Great
/// Western Sea, and they ask Sheba why she travels with them.
void Idejima_WonderWhereAlexHasGone(void) {
  const u8 *script;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(108, CAMERA_KEEP, 720, 1);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 86, 712);
  Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineState.leaderActorId);
  System_Wait(1);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_KRADEN, 104, 712);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  FieldEvent_SetMessageId((s32)MsgIdejimaWhereHasAlexGone);
  // "By the way... Where has Alex gone?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 104, 728);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 20);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  // "He doesn't seem to be anywhere on the island."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 20);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  // "You don't think he set out on his own, do you?" Yes: "Then I guess there's
  // no reason to look for him here." No: "Well, he's no longer on Idejima, at
  // any rate."
  Event_AskYesNo(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  // "He might have gone to look for a ship."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_PlaceOnActor(ACTOR_JENNA, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_SHEBA, gMainRoutineState.leaderActorId);
  System_Wait(1);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_WalkTo(ACTOR_JENNA, 86, 712);
  Actor_WalkToAndWait(ACTOR_SHEBA, 86, 728);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  FieldActor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_STAND);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  // "Alex wants a ship?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "Did you forget what he was saying?"
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                            0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    // "He wants to return Alchemy to its former place in the world."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
    gFieldState->message += 1;
  } else {
    gFieldState->message += 1;
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    // "Then you know he wants to restore Alchemy to the world."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  }
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  // "By lighting the four elemental lighthouses, he might just succeed."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  // "But why the ship?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  // "The remaining two lighthouses are unreachable by land."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(gMainRoutineState.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "None remain to be lit across the Eastern Sea."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "So what should we°"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "We must go to the Great Western Sea..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 20);
  // "The Western Sea..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  // "Is that where we're going, Felix?"
  Event_OpenMessage(ACTOR_JENNA, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_KRADEN, 0) == 0) {
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    // "Well said, Felix! And go there, we shall!"
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
    gFieldState->message += 1;
  } else {
    gFieldState->message += 1;
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
    // "Don't be foolish, Felix. We must pursue Alex. We have to go there..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
    Idejima_CountAnswer();
  }
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  // "That's right! Our parents' lives depend on it!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  // "What about you, Sheba?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 40);
  // "What do you mean?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 20);
  // "It's...not going to be an easy trip."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 20);
  // "And there is no reason you should have to face that danger... Right..."
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                            0) == 0) {
    Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 20);
    // "Then you should know my reason for traveling with all of you, Felix..."
    Event_ShowMessage(ACTOR_SHEBA, 0);
    gFieldState->message += 1;
  } else {
    gFieldState->message += 1;
    Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 20);
    // "Felix remembers... I have a reason to be traveling with you..."
    Event_ShowMessage(ACTOR_SHEBA, 0);
  }
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  // "What do you mean, Sheba? What reason?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  // "It's my destiny..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "This quest is your... destiny?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldActor_WatchAndSetScript(gMainRoutineState.leaderActorId, ACTOR_SHEBA);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_SHEBA);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, ACTOR_SHEBA);
  Actor_SetSpeed(ACTOR_SHEBA, 0.4);
  Actor_WalkToAndWait(ACTOR_SHEBA, 86, 748);
  FieldEvent_Wait(40);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 80);
  // "And you couldn't tell us about this earlier?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldActor_Stop(gMainRoutineState.leaderActorId);
  FieldActor_Stop(ACTOR_KRADEN);
  FieldActor_Stop(ACTOR_JENNA);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  FieldEvent_Wait(40);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "But how can we trust you when we don't know why you're with us?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  FieldActor_WaitForScriptEndById(ACTOR_SHEBA);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  // "I'm sorry... But please, you have to take me! You must! You need me!"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHWEST, 20);
  // "I don't understand. Why exactly do we need you?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  // "You know that I am an Adept, don't you?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  // "I can control the wind, as Felix already knows..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  // "You knew that she was a Wind Adept?" Yes: "Saturos must have mentioned it
  // atop the lighthouse... I see..." No: "Saturos must have known it, even
  // though he never told you..."
  Event_AskYesNo(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 40);
  // "They saw that I was a Wind Adept right away..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  // "They kidnapped me for my powers... They needed them on their journey."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "Oh, yeah... They would have needed an alignment that complemented..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  // "They said they would need a Wind Adept in order to light Jupiter's..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Ah, of course... And I suspect we'll need your power there as well."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 20);
  // "So you see, you do need me."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "All right... I guess I understand now."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  // "But you want to know what she meant by destiny, too, don't you?"
  Event_OpenMessage(ACTOR_JENNA, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                            0) == 0) {
    Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
    // "I wish we could read minds like Sheba. Then we'd know..."
    Event_ShowMessage(ACTOR_JENNA, 0);
    gFieldState->message += 1;
  } else {
    gFieldState->message += 1;
    Actor_SetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    // "Oh, so it's just me? Too bad I can't read minds like Sheba, huh?"
    Event_ShowMessage(ACTOR_JENNA, 0);
  }
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(20);
  // "I'm sure she will tell us in her own time. Won't you, Sheba?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  // "I think, for now, that we should simply trust Sheba."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldActor_Jump(ACTOR_KRADEN, 4, 20);
  // "Now, I am quite famished. Shall we get moving?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  script = gIdejima_GoAfterAlexScript;
  Actor_SetScript(ACTOR_KRADEN, script);
  Actor_SetScript(ACTOR_JENNA, script);
  Actor_RunScriptAndWait(ACTOR_SHEBA, script);
  Event_SetFlag(FLAG_IDEJIMA_ASKED_WHERE_ALEX_WENT);
  Event_End();
}

/// Starts the wave scroll from its first phase and table.
void Idejima_ResetWaveLines(void) {
  gIdejima_WaveBuffer = 0;
  gIdejima_WavePhase = 0;
}

/// The sea's wave scroll: each scanline's vertical offset for BG3 from the
/// table finished last frame, streamed by DMA 0 at every HBlank.
void Idejima_ApplyWaveLines(void) {
  s16 *line = gIdejima_WaveLines[(s16)(gIdejima_WaveBuffer ^ 1)];

  Io_Write16(line[0], &REG_BG3VOFS);
  Dma_Set(line + 1, (void *)&REG_BG3VOFS, DMA_HBLANK_HALFWORD_PER_LINE,
          REG_DMA0);
}

/// Fills the other table of gIdejima_WaveLines with BG3's scroll plus twice
/// the sine plus one, 0 to 4 pixels: a wave that moves down the screen a row
/// each frame.
void Idejima_UpdateWaveLines(void) {
  struct MapLayer *layer = MapGrid_GetLayer(0);
  s16 *line = gIdejima_WaveLines[gIdejima_WaveBuffer];
  s32 i;

  for (i = 0; i != DISPLAY_HEIGHT; i++) {
    s32 wave =
        Math_Sin(
            ((u8)gIdejima_WavePhase - i - FIXEDtoINT(layer->y) + DISPLAY_HEIGHT)
            << 9) +
        FIXED_ONE;

    line[i] = FIXEDtoINT(layer->y) + FIXEDtoINT_TWICE(wave);
  }
  gIdejima_WaveBuffer ^= 1;
  gIdejima_WavePhase++;
}

/// Copies BG palettes 8 to 11 of the palette being faded to over palettes 0 to
/// 3 and clears colour 0, for the island adrift.
void Idejima_UseDriftPalette(void) {
  struct FadePalette *palette = Ram_HeapSlots->paletteFade;
  u16 *to = palette->colors;
  u16 *from = palette->colors + 8 * PALETTE_COLORS;
  u32 i;

  for (i = 0; i < 4 * PALETTE_COLORS; i++)
    *to++ = *from++;
  palette->colors[0] = 0;
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 0);
}

/// Readies the island adrift: the sea's layer drifts a quarter of a pixel east
/// and an eighth south each frame, the beacon's cells are copied in and the
/// ship rocks quickly.
void DriftScene_Prepare(void) {
  struct MapLayer *layer = MapGrid_GetLayer(0);

  layer->speedX = toFIXED(1) / 4;
  layer->speedY = toFIXED(1) / 8;
  MapGrid_CopyCellsTo(72, 10, 75, 8, 1, 1);
  Actor_SetAnimation(ACTOR_SHIP, SHIP_ANIMATION_ROCK_FAST);
}

#include "shared/emote_pop_tilted.c"

/// Jenna and Kraden reach Idejima and find Menardi's ship, which cannot sail
/// without the orb that moved it. Alex joins them, the beacon is lit and the
/// tremor tears Idejima away from Gondowan.
void Idejima_BreakAwayFromGondowan(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  System_Wait(1);
  FieldCamera_MoveTo(252, CAMERA_KEEP, 216, 0);
  System_Wait(1);
  MapGrid_CenterOnFollowedPoint();
  Actor_SetPosition(gMainRoutineState.leaderActorId, 0, 0);
  Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(184), toFIXED(168),
                             FACING_SOUTH);
  {
    struct Actor *kraden = Actor_Get(ACTOR_KRADEN);

    kraden->facing = FACING_WEST;
  }
  Actor_SetPosition(ACTOR_ALEX, 186, 164);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_ALEX), SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 0);
  System_Wait(1);
  Event_StartScreenTransition();
  FieldCamera_SetSpeed(0.6);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_WalkToAndWait(ACTOR_JENNA, 204, 196);
  Actor_WalkToAndWait(ACTOR_JENNA, 208, 218);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_PlaceOnActor(ACTOR_KRADEN, ACTOR_JENNA);
  System_Wait(1);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_KRADEN, 186, 216);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_SetMessageId(MsgIdejimaIsThisThePlace);
  // "This is the place, isn't it?"
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  FieldEvent_AskYesNoWithActorAnimation(ACTOR_JENNA, 0);
  Actor_WaitForAnimationChange(ACTOR_JENNA);
  // "The peninsula at the end of the road to the west... That's what he..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 80);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  // "This must be Idejima!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  // "Hm... Menardi said a ship would be here..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldActor_Jump(ACTOR_KRADEN, 4, 0);
  // "We ought to see if we can find it."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetScript(ACTOR_KRADEN, gIdejima_KradenToTheShipScript);
  FieldEvent_Wait(40);
  Actor_RunScriptAndWait(ACTOR_JENNA, gIdejima_JennaToTheShipScript);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.6);
  Actor_WalkToAndWait(ACTOR_KRADEN, 258, 172);
  // "Jenna, it's over here!"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Actor_SetSpeed(ACTOR_JENNA, 0.4);
  Actor_WalkToAndWait(ACTOR_JENNA, 242, 172);
  Actor_WalkToAndWait(ACTOR_KRADEN, 248, 156);
  Actor_WalkToAndWait(ACTOR_KRADEN, 226, 164);
  Actor_WalkToAndWait(ACTOR_KRADEN, 228, 180);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 40);
  // "At first glance, it seems like a normal ship, but..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 226, 164);
  Actor_WalkToAndWait(ACTOR_KRADEN, 248, 156);
  Actor_WalkToAndWait(ACTOR_KRADEN, 258, 172);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  // "There's something odd about it..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  FieldActor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 256, 198);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 20);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "Kraden, wait! Oh, I swear! You're like a kid with a new toy!"
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0, 20);
  Actor_WalkToAndWait(ACTOR_JENNA, 224, 182);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Actor_WalkToAndWait(ACTOR_JENNA, 242, 172);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  FieldActor_Jump(ACTOR_JENNA, 2, 10);
  FieldActor_Jump(ACTOR_JENNA, 4, 20);
  // "Hurry up in there!"
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0, 40);
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(262, CAMERA_KEEP, 180, 1);
  Actor_SetPosition(ACTOR_KRADEN, 256, 198);
  System_Wait(1);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  FieldActor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 258, 172);
  FieldEvent_Wait(40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  // "What's wrong?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "It's missing..."
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  // "Hm? What are you talking about?"
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0, 20);
  // "The thingie... that makes it go."
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Emote_PopTilted(toFIXED(238), toFIXED(48), toFIXED(172), DEGtoANG(315));
  FieldEvent_Wait(10);
  Emote_PopTilted(toFIXED(246), toFIXED(48), toFIXED(172), DEGtoANG(45));
  FieldEvent_Wait(30);
  // "What do you mean? What's so important about this...thingie?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 40);
  // "The boat can't move without it..."
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 20);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_WalkToAndWait(ACTOR_JENNA, 248, 148);
  // "That Menardi! She tricked us!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "Tricked us?"
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  // "Yeah, by passing this dud boat off on us!"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "There's no trick. If this boat is a dud, how did it get here in the..."
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Oh, right."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_KRADEN, 244, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 258, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 274, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 258, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 244, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 258, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 274, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 258, 170);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  // "It must have been the orb..."
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  // "What orb?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "Don't you remember that crystal Saturos was carrying?"
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  // "That big, black pearl thing?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  // "I'm certain he said he could move the ship with it."
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  // "Jenna, Kraden!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX,
                    0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 1);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_ALEX), 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 40);
  Actor_SetAnimation(ACTOR_ALEX, ANIM_NOD);
  // "There you are."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX,
                    0);
  Actor_SetSpeed(ACTOR_ALEX, 0.8);
  Actor_WalkToAndWait(ACTOR_ALEX, 216, 164);
  Actor_WalkToAndWait(ACTOR_ALEX, 234, 160);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 20);
  // "I see you've found the boat."
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHEAST, 0);
  // "We set sail as soon as the beacon has been fired, correct?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "Strange..."
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 0);
  // "What is it?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0);
  // "The beacon... It hasn't been lit yet."
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHEAST, 40);
  // "You're right... Why is it taking so long?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  // "And why hasn't my brother returned?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 80);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(IDEJIMA_TO_WORLD_MAP_NORTH_A);
}

/// Adrift, Alex leads Jenna and Kraden to Felix and Sheba, washed up on the
/// island.
void Idejima_FindFelixAndSheba(void) {
  struct Actor *actor = Actor_Get(ACTOR_JENNA);

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Idejima_UseDriftPalette();
  DriftScene_Prepare();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetPosition(ACTOR_SHEBA, 444, 212);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_COLLAPSE);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SHEBA), 0);
  Actor_SetPosition(gMainRoutineState.leaderActorId, 432, 234);
  Actor_Get(gMainRoutineState.leaderActorId)->scaleX = -FIXED_ONE;
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_LEADER_LIE_ON_BACK);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineState.leaderActorId), 0);
  Actor_SetPosition(ACTOR_ALEX, 392, 172);
  System_Wait(1);
  FieldCamera_MoveTo(192, CAMERA_KEEP, 214, 0);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  Actor_SetPosition(ACTOR_JENNA, 208, 218);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_COLLAPSE);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNA), 0);
  Actor_SetPosition(ACTOR_KRADEN, 176, 204);
  Actor_SetAnimation(ACTOR_KRADEN, KRADEN_ANIMATION_ASLEEP);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_KRADEN), 0);
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(120);
  FieldEvent_SetMessageId((s32)MsgIdejimaHowLongWillWeDrift);
  // "Oh, Kraden... How long will this island continue to drift?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 40);
  // "I don't know... I wish you could tell me."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  // "Sheesh... You're no help in times like this."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  // "Hey, I'm hungry..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  // "How can you even think about food?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 80);
  // "I wonder what happened to my brother..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  // "I can't be certain, but it looks like they lit the Venus beacon."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  // "Yeah, I know. You don't have to tell me that. That's how we wound up..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  // "My, aren't you cranky today, Jenna!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "Well, I suppose you're just worried about Felix."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "Isaac and the others must have been at the lighthouse, too."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  // "I suspect you're right..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  // "I miss them... Isaac... Felix..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetSpeed(ACTOR_ALEX, 0.8);
  Actor_WalkToAndWait(ACTOR_ALEX, 304, 172);
  Actor_WalkToAndWait(ACTOR_ALEX, 224, 172);
  Actor_WalkToAndWait(ACTOR_ALEX, 208, 192);
  // "Come with me."
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  // "There is something I wish to show you."
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_SetScript(ACTOR_ALEX, gIdejima_AlexLeadsTheWayScript);
  FieldEvent_Wait(40);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_KRADEN), 1);
  FieldActor_Jump(ACTOR_KRADEN, 4, 20);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNA), 1);
  FieldActor_Jump(ACTOR_JENNA, 4, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 40);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetScript(ACTOR_KRADEN, gIdejima_KradenFollowsAlexScript);
  FieldEvent_Wait(40);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(368, CAMERA_KEEP, 210, 1);
  actor->timer = 0;
  Actor_SetScript(ACTOR_JENNA, gIdejima_JennaFollowsAlexScript);
  do {
    System_Wait(1);
  } while (actor->timer == 0);
  FieldEvent_Wait(10);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Felix... Sheba..."
  // "Sheba! Felix!"
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_JENNA, 7, 13, 2, 12, ACTOR_KRADEN, 9,
                                    2, 4, 1, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.6);
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_SetScript(ACTOR_KRADEN, gIdejima_KradenRunsToFelixScript);
  Actor_RunScriptAndWait(ACTOR_JENNA, gIdejima_JennaRunsToFelixScript);
  FieldEvent_Wait(40);
  // "They... They're alive..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN,
                    0);
  FieldActor_Jump(ACTOR_JENNA, 4, 10);
  // "They're really alive!"
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  // "What is this? Alex, what happened?"
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0, 40);
  gFieldState->transitionFrames = 120;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(IDEJIMA_TO_TITLE);
}

/// Sheba wakes and tells what happened at the lighthouse, a new continent comes
/// in sight, and a tidal wave sweeps the island towards it.
void Idejima_RideTheTidalWave(void) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  DriftScene_Prepare();
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(314), toFIXED(200),
                             FACING_SOUTH_SOUTHEAST);
  Actor_SetPositionAndFacing(ACTOR_ALEX, toFIXED(328), toFIXED(240),
                             FACING_NORTH_NORTHEAST);
  Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(340), toFIXED(200),
                             FACING_SOUTH);
  Actor_SetPositionAndFacing(ACTOR_SHEBA, toFIXED(342), toFIXED(230),
                             FACING_EAST);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_COLLAPSE);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SHEBA), 0);
  Actor_SetPositionAndFacing(gMainRoutineState.leaderActorId, toFIXED(312),
                             toFIXED(230), FACING_WEST);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_LEADER_LIE_ON_BACK);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineState.leaderActorId), 0);
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId(MsgIdejimaThoughtTheyWereInsideLighthouse);
  // "I thought Felix and Sheba were still inside the lighthouse."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
  // "It's miraculous..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "Miracle or no, I'm just happy my brother's alive."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHEAST, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
  // "What happened back there?"
  // "..."
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_KRADEN, 9, 1, 7, 6, ACTOR_ALEX, 12,
                                    16, 18, 11, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Sheba..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 1);
  FieldEvent_Wait(40);
  // "Oh..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "Thank the elements, she's awake!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  // "Sheba, are you all right? It's me, Jenna."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "Jenna?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_SHEBA_SIT_UP);
  FieldEvent_Wait(40);
  // "What...happened?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  // "It seems you drifted here with Felix."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
  // "With Felix...?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 40);
  Actor_Get(ACTOR_SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(ACTOR_SHEBA, 340, 226);
  FieldActor_Jump(ACTOR_SHEBA, 4, 0);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SHEBA), 1);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "Take your time... Do you feel all right, Sheba?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_Get(ACTOR_SHEBA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  FieldActor_WaitForScriptEndById(ACTOR_SHEBA);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  FieldEvent_Wait(40);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_SHEBA);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, ACTOR_SHEBA);
  FieldActor_WatchAndSetScript(ACTOR_ALEX, ACTOR_SHEBA);
  Actor_SetSpeed(ACTOR_SHEBA, 0.4);
  Actor_WalkToAndWait(ACTOR_SHEBA, 358, 222);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 80);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  // "Where are we?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 20);
  // "Idejima. We were all to meet here... But now, the island is floating..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  // "This island is moving?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "I know it must be very hard to believe, Sheba."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_Jump(ACTOR_JENNA, 4, 0);
  // "When the Venus Lighthouse was lit, a massive tremor tore us off the..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  // "The ground beneath the lighthouse roiled, as though it might crumble..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.6);
  Actor_WalkToAndWait(ACTOR_SHEBA, 348, 222);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  // "I remember now... And that's what carried us away from Gondowan."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  FieldActor_Stop(ACTOR_KRADEN);
  FieldActor_Stop(ACTOR_JENNA);
  FieldActor_Stop(ACTOR_ALEX);
  Actor_SetSpeed(ACTOR_KRADEN, 0.4);
  Actor_WalkToAndWait(ACTOR_KRADEN, 320, 200);
  // "But what happened to you, Sheba? You were in the sea, and Saturos°"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 40);
  // "Saturos and Menardi are gone..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  // "What do you mean?"
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  // "Another group came... They fought Saturos and Menardi and won."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  // "Was it Isaac?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  // "Isaac... Yes, I think that's what they called him..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "You expect me to believe Isaac and his companions defeated Saturos..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  FieldActor_WaitForScriptEndById(ACTOR_SHEBA);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "Have they really grown so powerful in so short a time?"
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  // "But how did you wind up in the sea?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  FieldActor_WaitForScriptEndById(ACTOR_SHEBA);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  // "When the beacon was lit, the earthquake knocked me off the lighthouse..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 20);
  Actor_RunRepeatedMotion(gMainRoutineState.leaderActorId, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  // "Then what's my brother doing here?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
  Actor_ShowEmote(gMainRoutineState.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  FieldActor_WaitForScriptEndById(ACTOR_SHEBA);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  // "He tried to save me from drowning..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(gMainRoutineState.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_RunRepeatedMotion(gMainRoutineState.leaderActorId, 2);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Felix jumped from the top of the lighthouse!? Dear me!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  FieldActor_StartRepeatedMotion(gMainRoutineState.leaderActorId, 3);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 20);
  // "Felix... are you awake?"
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_LEADER_SIT_UP);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 20);
  // "Ah! Felix, you're awake!"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  // "Brother..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_Get(gMainRoutineState.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_WalkTo(gMainRoutineState.leaderActorId, 314, 226);
  FieldActor_Jump(gMainRoutineState.leaderActorId, 4, 0);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineState.leaderActorId), 1);
  leader->scaleX = FIXED_ONE;
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Are you sure you should be standing?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_Get(gMainRoutineState.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHWEST, 40);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHEAST, 40);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHWEST, 80);
  // "You don't seem very surprised to find yourself on a floating island."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(gMainRoutineState.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
  // "Felix, once you'd saved Sheba, you must have swum out here, correct?"
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  // "You must have seen that this island was floating when you were swimming."
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "But what do we do now?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 20);
  // "I have no idea. Nobody knows what lies beyond the Eastern Sea."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  // "Unfortunately, I am a student of Alchemy, not geography..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetSpeed(ACTOR_ALEX, 1.6);
  Actor_WalkToAndWait(ACTOR_ALEX, 368, 256);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  FieldActor_WaitForScriptEndById(ACTOR_SHEBA);
  // "What is it...Alex?"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA,
                    0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_WalkToAndWait(ACTOR_ALEX, 400, 264);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 0);
  // "What's the matter?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  // "Can't you see it?"
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(396, CAMERA_KEEP, 234, 1);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
  leader->timer = 0;
  Actor_SetScript(ACTOR_KRADEN, gIdejima_KradenToTheShoreScript);
  Actor_SetScript(ACTOR_JENNA, gIdejima_JennaToTheShoreScript);
  Actor_SetScript(gMainRoutineState.leaderActorId,
                  gIdejima_FelixToTheShoreScript);
  Actor_SetScript(ACTOR_SHEBA, gIdejima_ShebaToTheShoreScript);
  do
    System_Wait(1);
  while (leader->timer == 0);
  FieldCamera_WaitForArrival();
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "It's land! An island!"
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  // "It's a little... big for an island."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  // "That's no island... It's a new continent!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldEvent_SetExitRequest(IDEJIMA_TO_WORLD_MAP_NORTHEAST);
}

/// Begins a step of Felix's waking: empty.
void FelixWake_StartStep(void) {}

/// Ends a step of Felix's waking: empty.
void FelixWake_EndStep(void) {}

/// Waits for a button, or 3 seconds.
void FelixWake_WaitForButton(void) {
  s32 frames = 180;

  FelixWake_StartStep();
  while (--frames != -1) {
    System_Wait(1);
    if (gInput.pressed != 0)
      break;
  }
  FelixWake_EndStep();
}

/// Felix wakes on the grass, stirring a little more at each button, gets up
/// and is asked "Do you want to check yourself for injuries?"; Yes has him
/// check his arms and then his legs, No has him walk about a bit.
void Idejima_FelixWakes(void) {
  s32 answer;
  s32 leader;

  FelixWake_StartStep();
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  System_Wait(1);
  FieldCamera_MoveTo(384, CAMERA_KEEP, 728, 0);
  System_Wait(1);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_LEADER_LIE_ON_BACK);
  System_Wait(1);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineState.leaderActorId), 0);
  System_Wait(1);
  Actor_SetPosition(gMainRoutineState.leaderActorId, 384, 728);
  Actor_SetPosition(ACTOR_NARRATOR, 384, 702);
  System_Wait(1);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  FelixWake_EndStep();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  System_Wait(10);
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Actor_RunRepeatedMotion(gMainRoutineState.leaderActorId, 2);
  System_Wait(1);
  FelixWake_EndStep();
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_LEADER_SIT_UP);
  System_Wait(1);
  FelixWake_EndStep();
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineState.leaderActorId), 1);
  System_Wait(1);
  FieldActor_Jump(gMainRoutineState.leaderActorId, 4, 20);
  FieldEvent_SetMessageId((s32)MsgIdejimaCheckForInjuries);
  // "Do you want to check yourself for injuries?"
  Event_OpenMessage(ACTOR_NARRATOR, 0);
  System_Wait(1);
  answer =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0);
  leader = gMainRoutineState.leaderActorId;
  if (answer == 0) {
    Actor_WaitForAnimationChange(leader);
    Actor_SetAnimation(gMainRoutineState.leaderActorId,
                       ANIM_LEADER_SWING_RIGHT_ARM);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Actor_SetAnimation(gMainRoutineState.leaderActorId,
                       ANIM_LEADER_SWING_LEFT_ARM);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    // "Looks like the arms are still attached. That's a good start."
    Event_ShowMessage(ACTOR_NARRATOR, 0);
    System_Wait(1);
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
    FelixWake_EndStep();
    FieldEvent_Wait(20);
    FelixWake_StartStep();
    Actor_SetAnimation(gMainRoutineState.leaderActorId,
                       ANIM_LEADER_KICK_RIGHT_LEG);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Actor_SetAnimation(gMainRoutineState.leaderActorId,
                       ANIM_LEADER_KICK_LEFT_LEG);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    // "The legs are working fine, too. Yup. You're fine!"
    Event_ShowMessage(ACTOR_NARRATOR, 0);
    System_Wait(1);
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
  } else {
    Actor_WaitForAnimationChange(leader);
    gFieldState->message += 2;
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.4);
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 384, 746);
    FelixWake_EndStep();
    FieldEvent_Wait(20);
    FelixWake_StartStep();
    // "You walk around a bit, and everything seems fine."
    Event_ShowMessage(ACTOR_NARRATOR, 0);
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
  }
  Actor_SetPosition(ACTOR_NARRATOR, 0, 0);
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  Event_SetFlag(FLAG_IDEJIMA_FELIX_WOKE);
  Event_End();
  FelixWake_EndStep();
}

/// Starts the sea's ripple, then runs what the entrance taken calls for: the
/// island run aground (after the tidal wave, or back from the world map, both
/// making Daila's sanctum the place a defeat returns the party to), or one of
/// the story's three cutscenes before it.
s32 Scene_Initialize(void) {
  s32 entrance;

  Idejima_ResetWaveLines();
  Task_AddFrame(Idejima_ApplyWaveLines, TASK_PASS_VBLANK);
  Task_AddFrame(Idejima_UpdateWaveLines, TASK_PASS_FRAME);
  {
    struct MainRoutineState *party = &gMainRoutineState;

    entrance = party->entrance;
  }
  switch (entrance) {
  case ENTRANCE_WAKE:
    if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
      Party_RestoreHpPp(1);
  case ENTRANCE_BACK_FROM_WORLD_MAP: {
    struct MainRoutineState *party = &gMainRoutineState;

    party->restartScene = SCENE_DAILA_ROOMS_SANCTUM;
    party->restartEntrance = 1;
  }
    Idejima_LayDownTheOthers();
    break;
  case ENTRANCE_BREAK_AWAY:
    Idejima_BreakAwayFromGondowan();
    break;
  case ENTRANCE_FIND_FELIX_AND_SHEBA:
    Idejima_FindFelixAndSheba();
    break;
  case IDEJIMA_RIDE_THE_TIDAL_WAVE:
    Idejima_RideTheTidalWave();
    break;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Unused: lays an actor down, the reverse of Idejima_GetUp.
UNUSED void Idejima_LieDown(s32 id) {
  struct Actor *actor = Actor_Get(id);

  Actor_SetSpriteFlags(Actor_Get(id), 0);
  actor->facing = FACING_SOUTH;
  actor->motionFlags = 0;
  actor->x.fixed += toFIXED(12);
  actor->y.fixed = toFIXED(16);
  actor->collisionFlags = 0;
  ((struct FieldSprite *)actor->sprite)->rotation = DEGtoANG(270);
}

/// Lays each of Kraden, Jenna and Sheba who has not yet joined asleep on the
/// grass where the wave left them, and wakes Felix the first time.
void Idejima_LayDownTheOthers(void) {
  s32 flag;

  if (!Event_GetFlag(FLAG_IDEJIMA_KRADEN_JOINED)) {
    struct Actor *actor;

    Actor_SetPosition(ACTOR_KRADEN, 416, 680);
    MapGrid_CopyCellAttributes(38, 32, 2, 1, 25, 42);
    actor = Actor_Get(ACTOR_KRADEN);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_KRADEN), 0);
    actor->facing = FACING_SOUTH;
    actor->motionFlags = 0;
    actor->y.fixed = toFIXED(12);
    actor->collisionFlags = 0;
    Actor_SetAnimation(ACTOR_KRADEN, KRADEN_ANIMATION_ASLEEP);
  }
  if (!Event_GetFlag(FLAG_IDEJIMA_JENNA_JOINED)) {
    struct Actor *actor;

    Actor_SetPosition(ACTOR_JENNA, 288, 744);
    MapGrid_CopyCellAttributes(38, 33, 2, 1, 17, 46);
    actor = Actor_Get(ACTOR_JENNA);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNA), 0);
    actor->facing = FACING_SOUTH;
    actor->motionFlags = 0;
    actor->y.fixed = toFIXED(12);
    actor->collisionFlags = 0;
    Actor_SetAnimation(ACTOR_JENNA, ANIM_JENNA_LIE_ON_SIDE);
  }
  flag = Event_GetFlag(FLAG_IDEJIMA_SHEBA_JOINED);
  if (flag == 0) {
    struct Actor *actor;

    actor = Actor_Get(ACTOR_SHEBA);
    Actor_SetPosition(ACTOR_SHEBA, 240, 648);
    MapGrid_CopyCellAttributes(38, 34, 2, 1, 14, 40);
    Actor_SetAnimation(ACTOR_SHEBA, ANIM_COLLAPSE);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_SHEBA), 0);
    actor->motionFlags = 0;
    actor->y.fixed = toFIXED(12);
  }
  MapGrid_CopyCellsTo(72, 42, 75, 40, 1, 1);
  System_Wait(1);
  if (!Event_GetFlag(FLAG_IDEJIMA_FELIX_WOKE))
    Idejima_FelixWakes();
}

u32 gIdejima_KradenToTheShipScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(216, 0, 172),
    SC_SYNC,
    SC_MOVE(264, 0, 164),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

u32 gIdejima_JennaToTheShipScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(216, 0, 182),
    SC_SYNC,
    SC_MOVE(242, 0, 164),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

u32 gIdejima_AlexLeadsTheWayScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(224, 0, 172),
    SC_SYNC,
    SC_MOVE(392, 0, 172),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

u32 gIdejima_KradenFollowsAlexScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(224, 0, 172),
    SC_SYNC,
    SC_MOVE(304, 0, 172),
    SC_SYNC,
    SC_MOVE(336, 0, 192),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gIdejima_JennaFollowsAlexScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(216, 0, 172),
    SC_SYNC,
    SC_MOVE(296, 0, 172),
    SC_SYNC,
    SC_MOVE(316, 0, 208),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_SET(SW_TIMER, 1),
    SC_END,
};

u32 gIdejima_KradenRunsToFelixScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(420, 0, 200),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

u32 gIdejima_JennaRunsToFelixScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(408, 0, 232),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

u32 gIdejima_JennaToTheShoreScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(424, 0, 200),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gIdejima_ShebaToTheShoreScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(408, 0, 228),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gIdejima_KradenToTheShoreScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(400, 0, 214),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gIdejima_FelixToTheShoreScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(412, 0, 248),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_SET(SW_TIMER, 1),
    SC_END,
};

s32 gIdejima_StretchScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 192),
    SC_ADD(SW_SCALE_Y, 384),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -384),
    SC_ADD(SW_SCALE_Y, -768),
    SC_WAIT(1),
    SC_LOOP(12, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_X, 192),
    SC_ADD(SW_SCALE_Y, 384),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(3)),
    SC_LABEL(4),
    SC_ADD(SW_SCALE_X, -384),
    SC_ADD(SW_SCALE_Y, -768),
    SC_WAIT(1),
    SC_LOOP(12, SC_TO(4)),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_WAIT(1),
    SC_END,
};

u32 gIdejima_KradenStepsAsideScript[] = {
    SC_POSITION(152, 0, 756),
    SC_WAIT(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(146, 0, 736),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(1),
    SC_END,
};

u32 gIdejima_JoinTheLeaderScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(152, 0, 756),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

u32 gIdejima_GoAfterAlexScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(104, 0, 728),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

#include "shared/scene_entrances_default.c"

// clang-format off
u32 gIdejimaExits[] = {
    SCENE_EXITS(Idejima),
    SCENE_EXIT(EXIT_DOOR, Idejima, ENTRANCE_FROM_DOOR_AGAIN),
    SCENE_EXIT(IDEJIMA_TO_WORLD_MAP_SOUTHWEST, WorldMap, WORLD_MAP_FROM_IDEJIMA_SOUTHWEST),
    SCENE_EXIT(IDEJIMA_TO_WORLD_MAP_NORTH_A, WorldMap, WORLD_MAP_VENUS_COLLAPSE),
    SCENE_EXIT(IDEJIMA_TO_TITLE, Title, TITLE_FROM_IDEJIMA),
    SCENE_EXIT(IDEJIMA_TO_WORLD_MAP_NORTHEAST, WorldMap, WORLD_MAP_DRIFT_NORTH),
    SCENE_EXITS_END,
};

struct ActorPlacement gIdejimaPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MENARDIS_SHIP, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 256, 0, 200, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gIdejimaEvents[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gIdejimaEventsFromDoor[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_DOOR,
     CONDITION_ALWAYS, EVENT(Idejima_EnterDoor)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIdejimaEventsWake[] = {
    {TRIGGER_EXIT, IDEJIMA_TO_WORLD_MAP_SOUTHWEST, CONDITION_ALWAYS,
     IDEJIMA_TO_WORLD_MAP_SOUTHWEST},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_BY_KRADEN, FLAG_IDEJIMA_KRADEN_JOINED,
     EVENT(Idejima_WakeKraden)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_BY_JENNA, FLAG_IDEJIMA_JENNA_JOINED,
     EVENT(Idejima_WakeJenna)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_BY_SHEBA, FLAG_IDEJIMA_SHEBA_JOINED,
     EVENT(Idejima_WakeSheba)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_KRADEN | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
     EVENT(Idejima_WakeKraden)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_JENNA | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
     EVENT(Idejima_WakeJenna)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CATCH_UP, CONDITION_ALWAYS,
     EVENT(Idejima_CatchUpWithFelix)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WHERE_ALEX_WENT,
     FLAG_IDEJIMA_ASKED_WHERE_ALEX_WENT, EVENT(Idejima_WonderWhereAlexHasGone)},
    {TRIGGER_TABLE_END},
};

u32 gEmote_PopTiltedScript[] = {
    SC_WAIT(46),
    SC_RELEASE,
};
