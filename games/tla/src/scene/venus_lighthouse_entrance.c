/// @file
/// Outside the Venus Lighthouse, where the prologue hands the story to Jenna.
/// Felix has left the group, and Jenna and Kraden come out of the lighthouse
/// alone:
///
/// - Two birds land below the door (VenusLighthouseEntrance_BirdsLand) and fly
///   off as the party walks past (VenusLighthouseEntrance_BirdsFlyOff).
/// - Tolbi's soldiers and the workmen of Lalivero close in. Alex arrives, sends
///   Jenna and Kraden west to Idejima, throws two soldiers skyward on columns
///   of water and drives the rest off
///   (VenusLighthouseEntrance_SoldiersAmbushParty).
/// - On the road west a Ruffian left to wait for backup fights the two
///   (VenusLighthouseEntrance_RuffianWaitsForBackup). A lost battle in the
///   prologue wakes them by the door, with Alex come back for them
///   (VenusLighthouseEntrance_WakeUp).
#include "fixed_math.h"
#include "scene_ids.h"
#include "sprite_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/effect_sprite.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "gamepak/far_call/system.h"
#include "graphics/resource.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "sound/sound.h"
#include "system/heap_release.h"
#include "system/iwram_call.h"
#include "system/main_routine_state.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/task.h"
#include "window/item_icon.h"
#include "text/msg_ids.h"

void Field_StartColorTransition(s32 frames, s32 color);
void VenusLighthouseEntrance_BlastActorSkyward(s32 id);
void Party_RestoreHpPp(s32 cureConditions);
s32 Actor_GetSpritePriority(struct Actor *actor);
void Actor_Release(struct Actor *actor);

/// Set as Tolbi's soldiers ambush the party; the soldiers and workmen are gone
/// from then on.
#define FLAG_VENUS_LIGHTHOUSE_AMBUSH_SEEN 0x890
/// Set as the birds land the first time the party comes out of the lighthouse.
#define FLAG_VENUS_LIGHTHOUSE_BIRDS_LANDED 0x891
/// Set as the birds fly off; they are gone from then on.
#define FLAG_VENUS_LIGHTHOUSE_BIRDS_FLEW_OFF 0x892
/// Set as the party comes back from beating the Ruffian who waited for backup;
/// he then lies beaten by the road.
#define FLAG_VENUS_LIGHTHOUSE_RUFFIAN_BEATEN 0x893

/// The ways into the scene that its set-up answers, besides scene_exits.h's.
enum VenusLighthouseEntranceEntrance {
  /// The map's own way in, in the lighthouse's doorway at the top of its
  /// stairs: the party goes on to it once Jenna takes over, and wakes there
  /// after a defeat.
  ENTRANCE_LIGHTHOUSE_DOOR = 1,
  /// On the path south of the plaza, between the bushes: where the debug
  /// menu's Venus Lighthouse start puts the party (MainRoutine_Run).
  ENTRANCE_PLAZA_PATH = 10,
  /// Further south, between the workmen and the soldiers.
  ENTRANCE_BY_WORKMEN = 11,
  /// On the west road, beside the Ruffian who waits for backup.
  ENTRANCE_WEST_ROAD = 12,
  /// Where the prologue hands the party to Jenna: Felix, Sheba and Piers leave
  /// it.
  ENTRANCE_JENNA_TAKES_OVER = 90,
  /// Back from the battle with the Ruffian who waited for backup.
  ENTRANCE_AFTER_RUFFIAN_BATTLE = 99
};

/// The map cells the step triggers answer, by the behaviour the map gives
/// them: the row before the lighthouse door, either side of it (cells (17, 7)
/// to (29, 7)), where the birds take off; the path below the door, cells
/// (21, 15) to (25, 15), where the soldiers close in; the road west, cells
/// (5, 14) to (5, 18), where the Ruffian waits; and the way east, cells
/// (28, 24) to (28, 29), where Kraden turns Jenna back.
enum VenusLighthouseEntranceCell {
  CELL_BESIDE_DOOR = 9,
  CELL_BELOW_DOOR = 10,
  CELL_ROAD_WEST = 11,
  CELL_WAY_EAST = 12
};

/// The scene's placed actors, in the rows of gActorPlacements.
enum VenusLighthouseEntranceActorId {
  ACTOR_KRADEN = ACTOR_FIRST_PLACED,
  ACTOR_ALEX,
  /// The two birds that land by the door and fly off.
  ACTOR_WEST_BIRD,
  ACTOR_EAST_BIRD,
  /// Tolbi's soldiers: the two Alex hurls skyward, the two who flee before
  /// him and the one sent to Lalivero for backup.
  ACTOR_SHOUTING_SOLDIER,
  ACTOR_SURPRISED_SOLDIER,
  ACTOR_DOUBTING_SOLDIER,
  ACTOR_SOLDIER_CAPTAIN,
  ACTOR_SOLDIER_SENT_FOR_BACKUP,
  /// The workmen of Lalivero who come looking for Sheba: four who run from
  /// Alex, and the one their boss sends to the caves.
  ACTOR_SCOLDING_WORKMAN,
  ACTOR_INDIGNANT_WORKMAN,
  ACTOR_EAGER_WORKMAN,
  ACTOR_WORKMAN_BOSS,
  ACTOR_WORKMAN_SENT_TO_CAVES,
  /// The Ruffian left on the road west to wait for backup.
  ACTOR_WAITING_RUFFIAN
};

struct Actor;

/// Starts the next dust puff at a random place near the given point, rising
/// and drifting outwards.
void VenusLighthouseEntrance_EmitDust(struct Actor *actor, s32 x, s32 y, s32 z);
/// Loads the dust puffs' pictures and starts drawing them each frame.
void VenusLighthouseEntrance_StartDust(void);
/// Stops the dust puffs and frees their pictures and state.
void VenusLighthouseEntrance_StopDust(void);
extern u32 gVenusLighthouseEntrance_BirdLands[];

void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      s32 *position);

/// Each frame of the column of water: it rises, holds, hurls the soldier off
/// the top of the screen, sinks, and its puddle dries away as the ground
/// shakes.
void VenusLighthouseEntrance_UpdateBlastColumn(void);
/// Each frame, draws and moves the dust puffs still playing.
void VenusLighthouseEntrance_UpdateDust(void);

void VenusLighthouseEntrance_BirdsFlyOff(void);

void VenusLighthouseEntrance_SoldiersAmbushParty(void);

void VenusLighthouseEntrance_RuffianWaitsForBackup(void);

void VenusLighthouseEntrance_PointToMeetingSpot(void);

/// The dust puffs kept at once, each emitted in turn over the oldest, and the
/// frames a puff lasts.
#define DUST_PUFFS 96
#define DUST_PUFF_FRAMES 16

/// The dust puffs' state, kept in Ram_HeapSlots' menu runtime while they play:
/// their pictures, the puffs each with its place and sprite, and the next to
/// emit.
struct DustPuffState {
  u8 pictures[2048]; ///< decompressed, then loaded into the VRAM cache
  struct SparkState states[DUST_PUFFS];
  struct EffectSprite sprites[DUST_PUFFS];
  s16 next;     ///< the next puff to emit, taken in turn
  u16 tileBase; ///< the pictures' first tile
  s16 resource; ///< their VRAM cache entry
};

s32 Actor_FallOverWithAnimationById();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Talking to the soldier in front of the three in a row, before the ambush:
/// the screen turns blue, a column of water rises under him and throws him
/// up off the top of the screen, as Alex's Psynergy does in the ambush, and
/// the colours come back. The player never gets to talk to him: the soldiers
/// stand south of the plaza, and the only way down to them is the path
/// between the bushes, which the cells below the door, (21, 15) to (25, 15),
/// span from side to side; stepping there starts the ambush, after which they
/// are gone. Walking Jenna down either side stair from the lighthouse door
/// reaches the plaza without starting it, and the bushes stop her there.
void VenusLighthouseEntrance_BlastActorWithPsynergy(void *soldierId) {
  Field_StartColorTransition(8, RGB(4, 4, 16));
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_JENNA),
                                   CASTER_LOOK_CYCLING_COLORS);
  FieldEvent_Wait(20);
  VenusLighthouseEntrance_BlastActorSkyward(soldierId);
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_JENNA), CASTER_LOOK_NORMAL);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Field_StartPaletteTransition(8);
}

/// A running soldier's update: it kicks up dust where it is, with a scuff
/// every eighth frame.
void VenusLighthouseEntrance_KickUpDust(struct Actor *actor) {
  VenusLighthouseEntrance_EmitDust(actor, actor->x.fixed, actor->y.fixed,
                                   actor->z.fixed);
  if (++actor->timer % 8 == 0)
    Event_PlaySound(SE_CHEST_CLOSE);
}

/// Talking to the soldier captain, before the ambush: he dashes off
/// north-east a few steps and back again, kicking up a trail of dust with a
/// scuffing sound. The player never gets to talk to him, for the reason given
/// at VenusLighthouseEntrance_BlastActorWithPsynergy.
void VenusLighthouseEntrance_RunAndKickUpDust(s32 actorId) {
  struct Actor *actor = Actor_Get(actorId);

  VenusLighthouseEntrance_StartDust();
  actor->update = VenusLighthouseEntrance_KickUpDust;
  actor->priorityFlags = 0;
  Actor_WalkByAndWait(actorId, 96, -96);
  Actor_WalkByAndWait(actorId, -96, 96);
  actor->update = 0;
  actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  FieldEvent_Wait(30);
  VenusLighthouseEntrance_StopDust();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

void VenusLighthouseEntrance_BirdsLand(void);
void VenusLighthouseEntrance_WakeUp(void);

s32 Scene_Initialize(void) {
  struct Actor *ruffian;
  struct Actor *leader;

  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  leader->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_SetAnimation(ACTOR_WEST_BIRD, 5);
  FieldActor_SetSpritePriority(ACTOR_WEST_BIRD, 1);
  Actor_SetAnimation(ACTOR_EAST_BIRD, 5);
  FieldActor_SetSpritePriority(ACTOR_EAST_BIRD, 1);
  FieldActor_SetSpritePriority(ACTOR_DOUBTING_SOLDIER, 2);
  FieldActor_SetSpritePriority(ACTOR_EAGER_WORKMAN, 2);
  if (Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_BIRDS_FLEW_OFF)) {
    Actor_SetPosition(ACTOR_WEST_BIRD, 0, 0);
    Actor_SetPosition(ACTOR_EAST_BIRD, 0, 0);
  }
  if (Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_AMBUSH_SEEN)) {
    Actor_SetPosition(ACTOR_SHOUTING_SOLDIER, 0, 0);
    Actor_SetPosition(ACTOR_SURPRISED_SOLDIER, 0, 0);
    Actor_SetPosition(ACTOR_DOUBTING_SOLDIER, 0, 0);
    Actor_SetPosition(ACTOR_SOLDIER_CAPTAIN, 0, 0);
    Actor_SetPosition(ACTOR_SOLDIER_SENT_FOR_BACKUP, 0, 0);
    Actor_SetPosition(ACTOR_SCOLDING_WORKMAN, 0, 0);
    Actor_SetPosition(ACTOR_INDIGNANT_WORKMAN, 0, 0);
    Actor_SetPosition(ACTOR_EAGER_WORKMAN, 0, 0);
    Actor_SetPosition(ACTOR_WORKMAN_BOSS, 0, 0);
    Actor_SetPosition(ACTOR_WORKMAN_SENT_TO_CAVES, 0, 0);
  }
  if (gMainRoutineState.entrance ==
      VENUS_LIGHTHOUSE_ENTRANCE_FROM_VENUS_LIGHTHOUSE) {
    if (!Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_BIRDS_LANDED)) {
      Event_Start();
      FieldEvent_EndLastingPsynergy(0);
      VenusLighthouseEntrance_BirdsLand();
      Event_End();
    }
  }
  if (gMainRoutineState.entrance == VENUS_LIGHTHOUSE_ENTRANCE_AFTER_DEFEAT) {
    gMainRoutineState.returnEntrance = ENTRANCE_LIGHTHOUSE_DOOR;
    gMainRoutineState.entrance = ENTRANCE_LIGHTHOUSE_DOOR;
    Party_RestoreHpPp(1);
    VenusLighthouseEntrance_WakeUp();
  }
  if (gMainRoutineState.entrance == ENTRANCE_AFTER_RUFFIAN_BATTLE) {
    Event_SetFlag(FLAG_VENUS_LIGHTHOUSE_RUFFIAN_BEATEN);
  }
  if (gMainRoutineState.entrance == ENTRANCE_JENNA_TAKES_OVER) {
    gMainRoutineState.leaderActorId = ACTOR_JENNA;
    Party_RemoveMember(ACTOR_FELIX);
    Party_RemoveMember(ACTOR_SHEBA);
    Party_RemoveMember(ACTOR_PIERS);
    Event_SetDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE,
                         ENTRANCE_LIGHTHOUSE_DOOR);
  }
  if (!Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_RUFFIAN_BEATEN)) {
    Actor_SetAnimation(ACTOR_WAITING_RUFFIAN, 5);
    FieldActor_SetSpritePriority(ACTOR_WAITING_RUFFIAN, 1);
  } else {
    Actor_SetPosition(ACTOR_WAITING_RUFFIAN, 40, 248);
    ruffian = Actor_Get(ACTOR_WAITING_RUFFIAN);
    ruffian->facing = FACING_SOUTH;
    Actor_FallOverWithAnimationById(ACTOR_WAITING_RUFFIAN, FACING_SOUTH, 0);
    FieldActor_SetSpritePriority(ACTOR_WAITING_RUFFIAN, 3);
    Actor_SetMovementType(Actor_Get(ACTOR_WAITING_RUFFIAN),
                          MOVEMENT_TYPE_SQUASH);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

s32 Actor_StartScript();

/// As Jenna and Kraden first come out of the lighthouse, two birds fly down
/// and land below the door.
void VenusLighthouseEntrance_BirdsLand(void) {
  struct Actor *westBird;
  struct Actor *eastBird;

  Event_SetFlag(FLAG_VENUS_LIGHTHOUSE_BIRDS_LANDED);
  westBird = Actor_Get(ACTOR_WEST_BIRD);
  westBird->motionFlags = 0;
  westBird->gravity = toFIXED(1) / 100;
  westBird->speedLimit = toFIXED(3) / 2;
  westBird->acceleration = toFIXED(1) / 2;
  FieldActor_SetPositionAndResetMotion(westBird, toFIXED(216), toFIXED(96),
                                       toFIXED(176));
  eastBird = Actor_Get(ACTOR_EAST_BIRD);
  eastBird->motionFlags = 0;
  eastBird->gravity = toFIXED(3) / 100;
  eastBird->speedLimit = toFIXED(3) / 2;
  eastBird->acceleration = toFIXED(1) / 2;
  FieldActor_SetPositionAndResetMotion(eastBird, toFIXED(616), toFIXED(96),
                                       toFIXED(152));
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 248, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  westBird = Actor_Get(ACTOR_WEST_BIRD);
  Actor_SetAnimation(ACTOR_WEST_BIRD, 4);
  Event_PlaySound(SE_BIRD_WINGS);
  FieldActor_SetMoveTarget(westBird, 352, 8, 176);
  Actor_StartScript(westBird, (s32)gVenusLighthouseEntrance_BirdLands);
  FieldEvent_Wait(25);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 104, 1);
  eastBird = Actor_Get(ACTOR_EAST_BIRD);
  Actor_SetAnimation(ACTOR_EAST_BIRD, 4);
  FieldActor_SetMoveTarget(eastBird, 416, 8, 152);
  Actor_StartScript(eastBird, (s32)gVenusLighthouseEntrance_BirdLands);
  FieldEvent_Wait(60);
  Event_PlaySound(SE_BIRD_WINGS);
  FieldActor_WaitForMoveById(ACTOR_WEST_BIRD);
  FieldActor_WaitForMoveById(ACTOR_EAST_BIRD);
  FieldCamera_WaitForArrival();
}

extern u32 gVenusLighthouseEntrance_BirdFlies[];

/// Walking past the lighthouse door, cells (17, 7) to (29, 7), the birds
/// take off and fly away.
void VenusLighthouseEntrance_BirdsFlyOff(void) {
  struct Actor *westBird;
  struct Actor *eastBird;

  Event_SetFlag(FLAG_VENUS_LIGHTHOUSE_BIRDS_FLEW_OFF);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Event_PlaySound(SE_BIRD_WINGS);
  Actor_SetAnimation(ACTOR_WEST_BIRD, 3);
  FieldActor_SetAnimation(ACTOR_EAST_BIRD, 3);
  FieldEvent_Wait(21);
  westBird = Actor_Get(ACTOR_WEST_BIRD);
  westBird->motionFlags = 0;
  westBird->speedLimit = toFIXED(2);
  westBird->acceleration = toFIXED(1) / 2;
  Actor_SetAnimation(ACTOR_WEST_BIRD, 4);
  FieldActor_SetMoveTarget(westBird, 616, 96, 176);
  Actor_StartScript(westBird, (s32)gVenusLighthouseEntrance_BirdFlies);
  eastBird = Actor_Get(ACTOR_EAST_BIRD);
  eastBird->motionFlags = 0;
  eastBird->speedLimit = toFIXED(2);
  eastBird->acceleration = toFIXED(1) / 2;
  Actor_SetAnimation(ACTOR_EAST_BIRD, 4);
  FieldActor_SetMoveTarget(eastBird, 120, 96, 176);
  Call2(Actor_StartScript, (s32)eastBird,
        (s32)gVenusLighthouseEntrance_BirdFlies);
  Event_End();
}

/// Walking down the path below the lighthouse door, cells (21, 15) to
/// (25, 15): Tolbi's soldiers and the workmen of Lalivero close in on Jenna
/// and Kraden. Alex arrives, tells them to meet him at Idejima, throws two
/// soldiers skyward on columns of water and sends the workmen running, then
/// follows the last soldiers to Lalivero.
void VenusLighthouseEntrance_SoldiersAmbushParty(void) {
  struct Actor *workman;
  struct Actor *actor;

  Event_SetFlag(FLAG_VENUS_LIGHTHOUSE_AMBUSH_SEEN);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgThereTheyAre);
  Actor_SetSpeed(ACTOR_SHOUTING_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_SURPRISED_SOLDIER, 1);
  // "There they are! Get 'em!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHOUTING_SOLDIER, 0, 0);
  Actor_WalkToAndWait(ACTOR_JENNA, 376, 280);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(20);
  Actor_WalkTo(ACTOR_JENNA, 376, 360);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 424, 1);
  FieldCamera_WaitForArrival();
  FieldActor_WaitForMoveById(ACTOR_JENNA);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_SHOUTING_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Iodem sent us to stand guard here, but..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHOUTING_SOLDIER, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SURPRISED_SOLDIER, 2);
  FieldEvent_Wait(10);
  // "I never thought we'd actually run into them!"
  FieldEvent_ShowMessageThenWait(ACTOR_SURPRISED_SOLDIER, 0, 10);
  Actor_ShowEmote(ACTOR_SCOLDING_WORKMAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Scoundrels! What have you done with Sheba?"
  FieldEvent_ShowMessageThenWait(ACTOR_SCOLDING_WORKMAN, 0, 10);
  FieldActor_Jump(ACTOR_INDIGNANT_WORKMAN, 6, 15);
  FieldActor_Jump(ACTOR_INDIGNANT_WORKMAN, 6, 25);
  // "Return her now, or you're really going to regret it!"
  FieldEvent_ShowMessageThenWait(ACTOR_INDIGNANT_WORKMAN, 0, 10);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_NORTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "Um... Aren't there supposed to be more of them?"
  FieldEvent_ShowMessageThenWait(ACTOR_DOUBTING_SOLDIER, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SOLDIER_CAPTAIN, 2);
  FieldEvent_Wait(10);
  // "Huh... Maybe they're just the first ones to come out..."
  FieldEvent_ShowMessageThenWait(ACTOR_SOLDIER_CAPTAIN, 0, 10);
  Actor_FaceDirection(ACTOR_EAGER_WORKMAN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "Great! That means we've got 'em outnumbered!"
  FieldEvent_ShowMessageThenWait(ACTOR_EAGER_WORKMAN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_WORKMAN_BOSS, ANIM_NOD);
  FieldEvent_Wait(10);
  // "We can probably take 'em, you think?"
  FieldEvent_ShowMessageThenWait(ACTOR_WORKMAN_BOSS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_EAGER_WORKMAN, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_SOLDIER_CAPTAIN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_SOLDIER_SENT_FOR_BACKUP, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "Go fetch the other Tolbi soldiers still stationed in Lalivero..."
  FieldEvent_ShowMessageThenWait(ACTOR_SOLDIER_CAPTAIN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_SOLDIER_SENT_FOR_BACKUP, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Yes, sir!"
  FieldEvent_ShowMessageThenWait(ACTOR_SOLDIER_SENT_FOR_BACKUP, 0, 10);
  Actor_WalkByAndWait(ACTOR_SOLDIER_SENT_FOR_BACKUP, 64, 0);
  Actor_SetPosition(ACTOR_SOLDIER_SENT_FOR_BACKUP, 0, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_WORKMAN_BOSS, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_WORKMAN_SENT_TO_CAVES, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "And we'll go tell the guys in the caves!"
  FieldEvent_ShowMessageThenWait(ACTOR_WORKMAN_BOSS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_WORKMAN_SENT_TO_CAVES, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Got it, Boss!"
  FieldEvent_ShowMessageThenWait(ACTOR_WORKMAN_SENT_TO_CAVES, 0, 10);
  Actor_WalkByAndWait(ACTOR_WORKMAN_SENT_TO_CAVES, -64, 64);
  Actor_SetPosition(ACTOR_WORKMAN_SENT_TO_CAVES, 0, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SOLDIER_CAPTAIN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_EAGER_WORKMAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_WORKMAN_BOSS, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 360, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, -16, -16, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_ALEX, 16, -16, FACING_SOUTH);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_ALEX, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, 16);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  // "How unfortunate... It looks like an ambush. What should we do, Jenna?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  // "What do you mean? Look around... We'll have to fight them!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 10);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Are you serious?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_WalkByAndWait(ACTOR_ALEX, 0, 16);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "Jenna, are you really prepared to fight these men?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "We don't have any other choice. We have to..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "Well, then. Allow me to shoulder some of your burden."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_SetSpeed(ACTOR_ALEX, 0.5);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_ALEX, 0, 16);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "...Burden?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  // "We regroup along the road leading away from Lalivero. Do you
  // understand?" Yes: "You don't need to remind us, Alex." No: "It's quite
  // simple, Jenna."
  ((void (*)())FieldMessage_ShowAtActor)(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_JENNA, 0) == 0) {
    FieldEvent_Wait(20);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    FieldEvent_Wait(30);
    Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
    FieldEvent_Wait(20);
    // "You don't need to remind us, Alex. We know the place."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    gFieldState->message += 1;
  } else {
    FieldEvent_Wait(30);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    gFieldState->message += 1;
    // "It's quite simple, Jenna. The meeting place is in the direction..."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  }
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(20);
  // "Follow this road west, and take the path down through the canyon..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "On the other side of that cave, you will find a small peninsula..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(312, CAMERA_KEEP, 424, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  // "Thankfully, only those workmen block your route."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  // "And fortunately..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  // "They don't look too tough..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_SCOLDING_WORKMAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_INDIGNANT_WORKMAN, FACING_EAST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_SCOLDING_WORKMAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_INDIGNANT_WORKMAN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  // "Hello! We can hear you!!!"
  FieldEvent_ShowMessageThenWait(ACTOR_SCOLDING_WORKMAN, 0, 10);
  Actor_ShowEmote(ACTOR_INDIGNANT_WORKMAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "That's just plain rude!"
  FieldEvent_ShowMessageThenWait(ACTOR_INDIGNANT_WORKMAN, 0, 10);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 424, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(25);
  // "Why don't the two of you escape and leave the Tolbi soldiers to me?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "The two of us? Alone?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 360, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "You can use Psynergy, can't you, Jenna?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Um... Yes."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "Then you'll be fine."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "We shall reconvene at the peninsula."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 408, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_ALEX, 0.5);
  Actor_WalkByAndWait(ACTOR_ALEX, 0, 16);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SCOLDING_WORKMAN, FACING_EAST_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_INDIGNANT_WORKMAN, FACING_EAST_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_EAGER_WORKMAN, FACING_EAST_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_WORKMAN_BOSS, FACING_EAST_NORTHEAST, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_SHOUTING_SOLDIER, 2);
  FieldEvent_Wait(10);
  // "You think you can take on Tolbi's finest on your own, little man?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHOUTING_SOLDIER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "I do not wish to inflict unnecessary bloodshed."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SURPRISED_SOLDIER, 2);
  FieldEvent_Wait(10);
  // "Don't wish to... What's that supposed to mean?"
  FieldEvent_ShowMessageThenWait(ACTOR_SURPRISED_SOLDIER, 0, 10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_WEST_NORTHWEST, 0);
  FieldEvent_Wait(25);
  // "If you lay a hand on these two, I assure you that you will be made..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_SHOUTING_SOLDIER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SURPRISED_SOLDIER, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_SHOUTING_SOLDIER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SURPRISED_SOLDIER, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_SHOUTING_SOLDIER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SURPRISED_SOLDIER, FACING_NORTHWEST, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_SHOUTING_SOLDIER, 1.2);
  Actor_SetSpeed(ACTOR_SURPRISED_SOLDIER, 1.2);
  Actor_WalkByAndWait(ACTOR_SHOUTING_SOLDIER, -26, 8);
  Actor_FaceDirection(ACTOR_SHOUTING_SOLDIER, FACING_NORTH_NORTHWEST, 0);
  Actor_WalkByAndWait(ACTOR_SURPRISED_SOLDIER, -16, -12);
  Actor_FaceDirection(ACTOR_SURPRISED_SOLDIER, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "By advancing, I assume you mean to fight. Permit me to strike the..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_ALEX, 8);
  FieldEvent_Wait(50);
  BattleEffect_StartArcEffect();
  actor = Actor_GetById(ACTOR_ALEX);
  BattleEffect_SetObjectUpdateMode(actor, CASTER_LOOK_CYCLING_COLORS);
  Event_PlaySound(SE_GATHERING_POWER);
  FieldEvent_Wait(20);
  Field_StartColorTransition(8, RGB(4, 4, 16));
  FieldEvent_Wait(60);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  FieldActor_SetSpritePriority(ACTOR_SHOUTING_SOLDIER, 0);
  VenusLighthouseEntrance_BlastActorSkyward(ACTOR_SHOUTING_SOLDIER);
  Actor_ShowEmote(ACTOR_SCOLDING_WORKMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_INDIGNANT_WORKMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_EAGER_WORKMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_WORKMAN_BOSS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SURPRISED_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_DOUBTING_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_SOLDIER_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  50);
  FieldActor_SetSpritePriority(ACTOR_SURPRISED_SOLDIER, 1);
  VenusLighthouseEntrance_BlastActorSkyward(ACTOR_SURPRISED_SOLDIER);
  FieldEvent_Wait(30);
  actor = Actor_GetById(ACTOR_ALEX);
  BattleEffect_SetObjectUpdateMode(actor, CASTER_LOOK_NORMAL);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Field_StartPaletteTransition(16);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_ALEX, 9);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_DOUBTING_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "What was that!?"
  FieldEvent_ShowMessageThenWait(ACTOR_DOUBTING_SOLDIER, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SOLDIER_CAPTAIN, 2);
  FieldEvent_Wait(10);
  // "I don't know, but whatever he did, it's out of our league!"
  FieldEvent_ShowMessageThenWait(ACTOR_SOLDIER_CAPTAIN, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_DOUBTING_SOLDIER, 3);
  Actor_SetAttachedEffect(ACTOR_DOUBTING_SOLDIER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_SOLDIER_CAPTAIN, 3);
  Actor_SetAttachedEffect(ACTOR_SOLDIER_CAPTAIN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SOLDIER_CAPTAIN, FACING_WEST, 0);
  FieldActor_Jump(ACTOR_DOUBTING_SOLDIER, 6, 15);
  FieldActor_Jump(ACTOR_DOUBTING_SOLDIER, 6, 25);
  // "You guys deal with him until backup arrives!"
  FieldEvent_ShowMessageThenWait(ACTOR_DOUBTING_SOLDIER, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Oh, so you're next?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_SetSpeed(ACTOR_ALEX, 0.8);
  Actor_WalkByAndWait(ACTOR_ALEX, -16, 16);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHWEST, 0);
  FieldCamera_MoveTo(360, CAMERA_KEEP, 408, 1);
  FieldCamera_WaitForArrival();
  FieldActor_StartRepeatedMotion(ACTOR_SCOLDING_WORKMAN, 3);
  Actor_SetAttachedEffect(ACTOR_SCOLDING_WORKMAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_INDIGNANT_WORKMAN, 3);
  Actor_SetAttachedEffect(ACTOR_INDIGNANT_WORKMAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_EAGER_WORKMAN, 3);
  Actor_SetAttachedEffect(ACTOR_EAGER_WORKMAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_WORKMAN_BOSS, 3);
  Actor_SetAttachedEffect(ACTOR_WORKMAN_BOSS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_WORKMAN_SENT_TO_CAVES, 3);
  Actor_SetAttachedEffect(ACTOR_WORKMAN_SENT_TO_CAVES,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_SetAnimationAndWait(ACTOR_SCOLDING_WORKMAN, ANIM_SHAKE_HEAD);
  // "Hey, no need to worry about us!"
  FieldEvent_ShowMessageThenWait(ACTOR_SCOLDING_WORKMAN, 0, 10);
  Actor_ShowEmote(ACTOR_INDIGNANT_WORKMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "We're not soldiers... We're not anybody!"
  FieldEvent_ShowMessageThenWait(ACTOR_INDIGNANT_WORKMAN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_EAGER_WORKMAN, ANIM_SHAKE_HEAD);
  // "Seriously, there's no way we're gonna face off against you."
  FieldEvent_ShowMessageThenWait(ACTOR_EAGER_WORKMAN, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_WORKMAN_BOSS, 3);
  Actor_SetAttachedEffect(ACTOR_WORKMAN_BOSS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(55);
  Actor_FaceDirection(ACTOR_SCOLDING_WORKMAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_INDIGNANT_WORKMAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_EAGER_WORKMAN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_WORKMAN_BOSS, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_WORKMAN_BOSS, 2);
  FieldEvent_Wait(10);
  // "Let's get outta here!"
  FieldEvent_ShowMessageThenWait(ACTOR_WORKMAN_BOSS, 0, 10);
  FieldActor_Jump(ACTOR_INDIGNANT_WORKMAN, 6, 0);
  FieldActor_Jump(ACTOR_EAGER_WORKMAN, 6, 0);
  FieldActor_Jump(ACTOR_WORKMAN_BOSS, 6, 25);
  VenusLighthouseEntrance_StartDust();
  Actor_SetSpeed(ACTOR_WORKMAN_BOSS, 1.8);
  Actor_SetSpeed(ACTOR_EAGER_WORKMAN, 1.8);
  Actor_SetSpeed(ACTOR_INDIGNANT_WORKMAN, 1.8);
  Actor_SetSpeed(ACTOR_SCOLDING_WORKMAN, 1.8);
  workman = Actor_GetById(ACTOR_WORKMAN_BOSS);
  workman->timer = 0;
  workman->update = VenusLighthouseEntrance_KickUpDust;
  workman = Actor_GetById(ACTOR_EAGER_WORKMAN);
  workman->timer = 0;
  workman->update = VenusLighthouseEntrance_KickUpDust;
  workman = Actor_GetById(ACTOR_INDIGNANT_WORKMAN);
  workman->timer = 0;
  workman->update = VenusLighthouseEntrance_KickUpDust;
  Actor_SetAnimation(ACTOR_WORKMAN_BOSS, 10);
  Actor_SetAnimation(ACTOR_EAGER_WORKMAN, 10);
  Actor_SetAnimation(ACTOR_INDIGNANT_WORKMAN, 10);
  Actor_SetDestinationOffset(ACTOR_WORKMAN_BOSS, -24, 24);
  Actor_SetDestinationOffset(ACTOR_EAGER_WORKMAN, -24, 24);
  Actor_SetDestinationOffset(ACTOR_INDIGNANT_WORKMAN, -24, 24);
  FieldActor_WaitForMoveById(ACTOR_INDIGNANT_WORKMAN);
  Actor_SetDestination(ACTOR_WORKMAN_BOSS, 224, 480);
  Actor_SetDestination(ACTOR_EAGER_WORKMAN, 224, 480);
  Actor_MoveToAndWait(ACTOR_INDIGNANT_WORKMAN, 224, 480);
  workman = Actor_GetById(ACTOR_WORKMAN_BOSS);
  workman->update = NULL;
  workman = Actor_GetById(ACTOR_EAGER_WORKMAN);
  workman->update = NULL;
  workman = Actor_GetById(ACTOR_INDIGNANT_WORKMAN);
  workman->update = NULL;
  FieldEvent_Wait(1);
  FieldActor_Jump(ACTOR_SCOLDING_WORKMAN, 6, 15);
  FieldActor_Jump(ACTOR_SCOLDING_WORKMAN, 6, 25);
  workman = Actor_GetById(ACTOR_SCOLDING_WORKMAN);
  workman->timer = 0;
  workman->update = VenusLighthouseEntrance_KickUpDust;
  Actor_SetAnimation(ACTOR_SCOLDING_WORKMAN, 10);
  Actor_SetDestinationOffset(ACTOR_SCOLDING_WORKMAN, -20, 20);
  FieldActor_WaitForMoveById(ACTOR_SCOLDING_WORKMAN);
  FieldActor_Jump(ACTOR_SCOLDING_WORKMAN, 4, 0);
  Actor_SetDestinationOffset(ACTOR_SCOLDING_WORKMAN, -20, 20);
  FieldActor_WaitForMoveById(ACTOR_SCOLDING_WORKMAN);
  Actor_SetAnimation(ACTOR_SCOLDING_WORKMAN, ANIM_STAND);
  Event_PlaySound(SE_IMPACT);
  workman->x.fixed += toFIXED(8);
  workman->update = NULL;
  workman->z.fixed += -toFIXED(8);
  workman->facing = FACING_NORTH;
  ((struct FieldSprite *)workman->sprite)->rotation = TRIG_TURN * 5 / 8;
  FieldEvent_Wait(55);
  workman->x.fixed += -toFIXED(4);
  workman->z.fixed += toFIXED(4);
  workman->facing = FACING_SOUTH;
  ((struct FieldSprite *)workman->sprite)->rotation = 0;
  FieldActor_Jump(ACTOR_SCOLDING_WORKMAN, 6, 40);
  Actor_SetAnimation(ACTOR_SCOLDING_WORKMAN, 11);
  Event_PlaySound(SE_CLICK);
  FieldEvent_Wait(14);
  Event_PlaySound(SE_CLICK);
  FieldEvent_Wait(14);
  Event_PlaySound(SE_CLICK);
  FieldEvent_Wait(14);
  Event_PlaySound(SE_CLICK);
  FieldEvent_Wait(14);
  FieldEvent_Wait(20);
  workman->timer = 0;
  workman->update = VenusLighthouseEntrance_KickUpDust;
  Actor_SetAnimation(ACTOR_SCOLDING_WORKMAN, 10);
  Actor_MoveToAndWait(ACTOR_SCOLDING_WORKMAN, 224, 472);
  FieldEvent_Wait(30);
  workman->update = NULL;
  Actor_SetPosition(ACTOR_WORKMAN_BOSS, 0, 0);
  Actor_SetPosition(ACTOR_EAGER_WORKMAN, 0, 0);
  Actor_SetPosition(ACTOR_INDIGNANT_WORKMAN, 0, 0);
  Actor_SetPosition(ACTOR_SCOLDING_WORKMAN, 0, 0);
  VenusLighthouseEntrance_StopDust();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(10);
  // "How undignified... and how shameful."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 408, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(15);
  // "Jenna, now is your chance. Go to Idejima and wait for me."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SOLDIER_CAPTAIN, FACING_NORTHWEST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SOLDIER_CAPTAIN, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_DOUBTING_SOLDIER, 1.5);
  Actor_WalkByAndWait(ACTOR_DOUBTING_SOLDIER, -32, 0);
  actor = Actor_GetById(ACTOR_ALEX);
  actor->facing = FACING_SOUTHEAST;
  // "I'm not finished with you yet."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  Actor_SetSpeed(ACTOR_ALEX, 1.5);
  Actor_WalkByAndWait(ACTOR_ALEX, 0, 16);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_DOUBTING_SOLDIER, 2);
  FieldEvent_Wait(10);
  FieldActor_SetSpritePriority(ACTOR_DOUBTING_SOLDIER, 3);
  Actor_SetSpeed(ACTOR_DOUBTING_SOLDIER, 2);
  Actor_GetById(ACTOR_DOUBTING_SOLDIER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_DOUBTING_SOLDIER, 456, 424);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_DOUBTING_SOLDIER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_SOUTHWEST, 0);
  Actor_SetSpeed(ACTOR_DOUBTING_SOLDIER, 1);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "You mentioned backup from Lalivero?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  Actor_SetSpeed(ACTOR_ALEX, 0.7);
  Actor_WalkByAndWait(ACTOR_ALEX, 48, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Actor_SetSpeed(ACTOR_SOLDIER_CAPTAIN, 1);
  Actor_GetById(ACTOR_SOLDIER_CAPTAIN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SOLDIER_CAPTAIN, 456, 440);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_SOLDIER_CAPTAIN)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Let's go meet them together, shall we?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_DOUBTING_SOLDIER, 2);
  FieldEvent_Wait(10);
  // "Hey, pal! When they get here, you'll really be in for it!"
  FieldEvent_ShowMessageThenWait(ACTOR_DOUBTING_SOLDIER, 0, 10);
  Actor_WalkByAndWait(ACTOR_ALEX, 16, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Do you honestly believe that even a hundred of you could stop me?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_DOUBTING_SOLDIER, 2);
  Actor_RunRepeatedMotion(ACTOR_SOLDIER_CAPTAIN, 2);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_DOUBTING_SOLDIER, 0.5);
  Actor_SetSpeed(ACTOR_SOLDIER_CAPTAIN, 0.5);
  Actor_GetById(ACTOR_DOUBTING_SOLDIER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_DOUBTING_SOLDIER, 488, 424);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_DOUBTING_SOLDIER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_SOLDIER_CAPTAIN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SOLDIER_CAPTAIN, 488, 440);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_SOLDIER_CAPTAIN)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_ALEX, 456, 432);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "How amusing... This I must see."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_DOUBTING_SOLDIER, 3);
  Actor_SetAttachedEffect(ACTOR_DOUBTING_SOLDIER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_SOLDIER_CAPTAIN, 3);
  Actor_SetAttachedEffect(ACTOR_SOLDIER_CAPTAIN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_GetById(ACTOR_DOUBTING_SOLDIER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_DOUBTING_SOLDIER, 504, 424);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_DOUBTING_SOLDIER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetPosition(ACTOR_DOUBTING_SOLDIER, 0, 0);
  Actor_GetById(ACTOR_SOLDIER_CAPTAIN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SOLDIER_CAPTAIN, 504, 440);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_SOLDIER_CAPTAIN)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetPosition(ACTOR_SOLDIER_CAPTAIN, 0, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_ALEX, 0.8);
  Actor_WalkByAndWait(ACTOR_ALEX, 64, 0);
  Actor_SetPosition(ACTOR_ALEX, 0, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveToActorById(ACTOR_JENNA, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "They're gone..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  // "Come on, Jenna... Let's be going."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  FieldEvent_Wait(10);
  FieldActor_Jump(ACTOR_KRADEN, 4, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  // "There's nothing standing in our way now!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "But..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What is it now?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 80);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  // "Ah... You're worried about hurting any innocents?" Yes: "Well, just try
  // to hold back when you're fighting." No: "Oh. You must be concerned
  // about Felix."
  Event_OpenMessage(ACTOR_KRADEN, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_JENNA, 0) == 0) {
    FieldEvent_Wait(20);
    // "Well, just try to hold back when you're fighting."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    gFieldState->message += 1;
  } else {
    FieldEvent_Wait(40);
    gFieldState->message += 1;
    // "Oh. You must be concerned about Felix. We can't do anything to..."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  }
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Let's get going! To the peninsula!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  actor = Actor_Get(ACTOR_JENNA);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_KRADEN, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  EventCamera_FollowActor(ACTOR_JENNA, 1);
  Event_End();
}

/// On the road west, cells (5, 14) to (5, 18), the Ruffian left to wait for
/// backup sees that Alex is not with Jenna and Kraden, and fights them.
void VenusLighthouseEntrance_RuffianWaitsForBackup(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgWhyDoI);
  Actor_WalkToAndWait(ACTOR_JENNA, 72, 266);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  FieldEvent_Wait(10);
  // "Why do I gotta be the one to wait for backup!?"
  FieldEvent_ShowMessageThenWait(ACTOR_WAITING_RUFFIAN, 0, 10);
  FieldActor_SetAnimation(ACTOR_WAITING_RUFFIAN, ANIM_STAND);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_WAITING_RUFFIAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_WAITING_RUFFIAN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldActor_Jump(ACTOR_WAITING_RUFFIAN, 6, 15);
  FieldActor_Jump(ACTOR_WAITING_RUFFIAN, 6, 35);
  Actor_FaceDirection(ACTOR_WAITING_RUFFIAN, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_WAITING_RUFFIAN, 12);
  // "Yaaah! P-P-Please! Just let me go, and... Hey!"
  FieldEvent_ShowMessageThenWait(ACTOR_WAITING_RUFFIAN, 0, 0);
  Actor_SetAnimation(ACTOR_WAITING_RUFFIAN, ANIM_STAND);
  Actor_ShowEmote(ACTOR_WAITING_RUFFIAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  50);
  Actor_FaceDirection(ACTOR_WAITING_RUFFIAN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  // "That scary guy with the blue hair's not with you..."
  FieldEvent_ShowMessageThenWait(ACTOR_WAITING_RUFFIAN, 0, 10);
  FieldActor_SetAnimationAndWait(ACTOR_WAITING_RUFFIAN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_WAITING_RUFFIAN, 1);
  Actor_WalkByAndWait(ACTOR_WAITING_RUFFIAN, 16, 0);
  FieldEvent_Wait(10);
  // "Great! Now, you'll see what I'm really made of!"
  FieldEvent_ShowMessageThenWait(ACTOR_WAITING_RUFFIAN, 0, 10);
  Actor_SetSpeed(ACTOR_WAITING_RUFFIAN, 2);
  FieldActor_Jump(ACTOR_WAITING_RUFFIAN, 6, 15);
  FieldActor_Jump(ACTOR_WAITING_RUFFIAN, 6, 25);
  FieldActor_Jump(ACTOR_WAITING_RUFFIAN, 6, 0);
  Actor_WalkBy(ACTOR_WAITING_RUFFIAN, 26, 0);
  FieldEvent_Wait(8);
  Actor_SetAnimation(ACTOR_WAITING_RUFFIAN, ANIM_STAND);
  FieldEvent_SetAfterBattleDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE,
                                       ENTRANCE_AFTER_RUFFIAN_BATTLE);
  Event_SetDefeatDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE,
                             VENUS_LIGHTHOUSE_ENTRANCE_AFTER_DEFEAT);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_EARLY_STORY, 0);
  Event_End();
}

/// Heading east, cells (28, 24) to (28, 29), Kraden jumps out from behind
/// Jenna: "Not that way, Jenna! The meeting spot is west of here." She steps
/// back.
void VenusLighthouseEntrance_PointToMeetingSpot(void) {
  struct Actor *actor;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgNotThatWayThe);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 16, 0, FACING_WEST);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  // "Not that way, Jenna! The meeting spot is west of here. We must go..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  actor = Actor_Get(ACTOR_JENNA);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_KRADEN, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_JENNA, -16, 0);
  Event_End();
}

/// After a lost battle in the prologue, Jenna wakes by the door with Kraden and
/// Alex beside her; Alex leaves again for Lalivero.
void VenusLighthouseEntrance_WakeUp(void) {
  struct Actor *actor;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetPosition(ACTOR_JENNA, 364, 156);
  Actor_SetPosition(ACTOR_KRADEN, 362, 142);
  Actor_SetPosition(ACTOR_ALEX, 390, 144);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH, 0);
  FieldActor_SetSpritePriority(ACTOR_KRADEN, 2);
  Actor_SetSpriteRotationById(ACTOR_JENNA, DEGtoANG(90));
  Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNA), 0);
  FieldCamera_MoveToActorById(ACTOR_JENNA, 0);
  gFieldState->transitionFrames = 60;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId((s32)MsgAreAreYou);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Jenna... Are... Are you all right?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Ah, Jenna... You're awake."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  // "Can you stand?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(30);
  Actor_SetPosition(ACTOR_JENNA, 376, 164);
  Actor_SetSpriteRotationById(ACTOR_JENNA, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNA), 1);
  FieldActor_Jump(ACTOR_JENNA, 6, 50);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_JENNA, -14, -2);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "It's a good thing I came back for you two..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Perhaps you ought to come with us this time..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "No. Someone has to keep Tolbi's soldiers at bay."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_SetSpeed(ACTOR_ALEX, 0.8);
  Actor_WalkByAndWait(ACTOR_ALEX, 0, 16);
  Actor_FaceDirection(ACTOR_ALEX, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "I'll have to go to Lalivero and confront them there."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "We'll meet at Idejima."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, ACTOR_ALEX);
  Actor_SetSpeed(ACTOR_ALEX, 0.8);
  Actor_WalkBy(ACTOR_ALEX, 0, 120);
  FieldEvent_Wait(40);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkByAndWait(ACTOR_KRADEN, 24, 0);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_WaitForMoveById(ACTOR_ALEX);
  Actor_SetPosition(ACTOR_ALEX, 0, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "He's gone again."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(20);
  FieldActor_Jump(ACTOR_KRADEN, 4, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "And we'd best be going as well..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  actor = Actor_Get(ACTOR_JENNA);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_KRADEN, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  gFieldState->transitionFrames = 8;
  Event_End();
}

/// The blast column's stage once it has burnt out.
#define BLAST_FINISHED 999

/// The blast column: its pieces, 16 pixels each, and its 16x16 pictures in 256
/// colours, four frames for its top and then four for the column below.
#define BLAST_COLUMN_PIECES 16
#define BLAST_COLUMN_FRAMES 4
#define BLAST_PICTURE_TILES SPRITE_TILES_8BPP(16, 16)

/// The rising column's state, kept in Ram_HeapSlots' menu runtime while it
/// plays: its pictures, its pieces of water, the soldier it lifts and the
/// puddle left under him.
struct BlastColumnState {
  u8 pictures[1024]; ///< decompressed, then loaded into the VRAM cache
  struct EffectSprite sprites[BLAST_COLUMN_PIECES];
  struct SparkState states[BLAST_COLUMN_PIECES];
  struct Actor *soldier;
  struct Actor *puddle;
  FIXED x; ///< where the column stands: the soldier's feet
  FIXED y;
  FIXED z;
  s16 stage; ///< BLAST_FINISHED once the column has burnt out
  s16 timer; ///< frames into the stage
  u16 columnTile;
  u16 topTile;
  s16 height;  ///< the column's height in pixels
  s8 carrying; ///< nonzero while the soldier rides the column's top
};

void VenusLighthouseEntrance_UpdateBlastColumn(void) {
  struct BlastColumnState *blastColumnState = Ram_HeapSlots->menuState;
  struct Actor *actor = blastColumnState->soldier;
  struct Actor *puddle = blastColumnState->puddle;
  s32 i, y;
  FIXED scale;
  s32 frameStep;
  s32 mask;
  struct SparkState *state;
  struct EffectSprite *sprite;

  switch (blastColumnState->stage) {
  case 0: // the first frame
    if (blastColumnState->timer == 0) {
      blastColumnState->stage++;
      blastColumnState->timer = -1;
    }
    break;
  case 1: // the column rises under the soldier, 4 pixels a frame
    blastColumnState->height += 4;
    if (blastColumnState->height > 31) {
      blastColumnState->stage++;
      blastColumnState->timer = -1;
    }
    break;
  case 2: // it holds him up for a second and a half
    if (blastColumnState->timer == 90) {
      blastColumnState->stage++;
      blastColumnState->timer = -1;
    }
    break;
  case 3: // it shoots up and hurls him off the top of the screen
    if (blastColumnState->timer == 0) {
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      actor->velocityY = toFIXED(24);
      blastColumnState->carrying = 0;
      Sound_Play(SE_HEAVY_IMPACT);
    }
    blastColumnState->height += 8;
    if (blastColumnState->height > 79) {
      blastColumnState->stage++;
      blastColumnState->timer = -1;
    }
    break;
  case 4: { // out of sight, he is taken off the stage
    s16 *timer = &blastColumnState->timer;

    if (blastColumnState->timer == 10) {
      actor->z.fixed = 0;
      actor->x.fixed = 0;
    }
    if (*timer == 30) {
      blastColumnState->stage++;
      blastColumnState->timer = -1;
    }
    break;
  }
  case 5: // the column sinks
    blastColumnState->height -= 8;
    if (blastColumnState->height <= 0) {
      blastColumnState->stage++;
      blastColumnState->timer = -1;
    }
    break;
  case 6: { // the puddle dries away, with the sound of a fall
    s16 *timer;

    scale = puddle->scaleX;
    if (scale > 0) {
      scale -= FIXED_ONE * 2 / 100;
      puddle->scaleX = scale;
      puddle->scaleY = scale;
    } else {
      puddle->scaleX = 0;
      puddle->scaleY = 0;
    }
    timer = &blastColumnState->timer;
    if (blastColumnState->timer == 10)
      Sound_Play(SE_FALL);
    if (*timer == 90) {
      blastColumnState->stage++;
      blastColumnState->timer = -1;
    }
    break;
  }
  case 7: { // a clatter, and the ground shakes for half a second
    s16 *timer = &blastColumnState->timer;

    if (blastColumnState->timer == 0) {
      Sound_Play(SE_CLATTER);
      MapGrid_SetShake(3, 3, 1);
    }
    if (*timer == 30) {
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      if (blastColumnState->timer == 30) {
        blastColumnState->stage = BLAST_FINISHED;
        blastColumnState->timer = -1;
      }
    }
    break;
  }
  }
  actor->facing += TRIG_TURN / 16;
  if (blastColumnState->height > 0) {
    for (i = 0, y = 0;
         i < BLAST_COLUMN_PIECES && y < blastColumnState->height - 8;
         i++, y += 16) {
      state = &blastColumnState->states[i];
      state->x = blastColumnState->x;
      state->y = blastColumnState->y + toFIXED(y) + toFIXED(8);
      state->z = blastColumnState->z;
      sprite = &blastColumnState->sprites[i];
      state->frame++;
      frameStep = state->frame >> 2;
      mask = 3;
      sprite->oam.index = blastColumnState->columnTile +
                          (frameStep & mask) * BLAST_PICTURE_TILES;
      EffectSprite_DrawAtWorldPosition(sprite, &state->x);
    }
    if (i < BLAST_COLUMN_PIECES) {
      state = &blastColumnState->states[i];
      state->x = blastColumnState->x;
      state->y = blastColumnState->y + toFIXED(blastColumnState->height);
      state->z = blastColumnState->z;
      state->frame++;
      state->y +=
          Math_MulFX(-toFIXED(2), Math_Sin(state->frame * (TRIG_TURN / 16))) -
          toFIXED(2);
      sprite = &blastColumnState->sprites[i];
      sprite->oam.index = blastColumnState->topTile +
                          ((state->frame >> 2) & 3) * BLAST_PICTURE_TILES;
      EffectSprite_DrawAtWorldPosition(sprite, &state->x);
      if (blastColumnState->carrying != 0)
        actor->y.fixed = state->y;
    }
  }
  blastColumnState->timer++;
}

s32 Actor_GetSpriteDepth(struct Actor *actor);

/// A column of water rises under a soldier, holds him up, then hurls him off
/// the top of the screen, and the ground shakes as the column sinks.
void VenusLighthouseEntrance_BlastActorSkyward(s32 id) {
  struct BlastColumnState *blastColumnState =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct BlastColumnState));
  struct Actor *actor = Actor_Get(id);
  struct Actor *puddle;
  struct EffectSprite *sprite;
  struct SparkState *state;
  s32 resource, tile, i;
  u8 unpaused = 0;

  actor->paused = unpaused;
  blastColumnState->soldier = actor;
  blastColumnState->x = actor->x.fixed;
  blastColumnState->y = actor->y.fixed;
  blastColumnState->z = actor->z.fixed + toFIXED(2);
  Decompress_TaggedLz(gField_GeyserTiles, blastColumnState);
  resource = VramCache_FindFreeEntry();
  tile = Vram_Load(resource, 2048, blastColumnState);
  blastColumnState->topTile = tile;
  blastColumnState->columnTile =
      tile + BLAST_COLUMN_FRAMES * BLAST_PICTURE_TILES;
  state = blastColumnState->states;
  sprite = blastColumnState->sprites;
  for (i = BLAST_COLUMN_PIECES - 1; i >= 0;) {
    EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, tile);
    sprite->depth = Actor_GetSpriteDepth(actor) - 2;
    sprite->oam.priority = Actor_GetSpritePriority(actor);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    state->frame = -1;
    i--;
    sprite++;
    state++;
  }
  puddle = (FieldActor_Create(SPRITE_PUDDLE, actor->x.fixed, actor->y.fixed,
                              actor->z.fixed));
  blastColumnState->puddle = puddle;
  Actor_SelectAnimation(puddle, 1);
  Actor_SetSpriteFlags(puddle, 0);
  puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpriteFlags(actor, 0);
  ((struct Sprite *)actor->sprite)->rotation = DEGtoANG(90);
  actor->x.fixed += -toFIXED(12);
  actor->motionFlags = 0;
  blastColumnState->stage = 0;
  blastColumnState->timer = 0;
  blastColumnState->height = 0;
  blastColumnState->carrying = 1;
  Event_PlaySound(SE_RISE_WITH_SPARKS);
  Task_AddFrame(VenusLighthouseEntrance_UpdateBlastColumn, TASK_PASS_FRAME);
  while (blastColumnState->stage != BLAST_FINISHED)
    System_Wait(1);
  FieldTask_Remove(VenusLighthouseEntrance_UpdateBlastColumn);
  Actor_Release(puddle);
  VramCache_FreeEntry(resource);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

void BattleEffect_StepVector3(struct SparkState *state, s32 damping,
                              s32 gravity);

void VenusLighthouseEntrance_UpdateDust(void) {
  struct DustPuffState *dustPuffState = Ram_HeapSlots->menuState;
  struct SparkState *state = dustPuffState->states;
  struct EffectSprite *sprite = dustPuffState->sprites;
  s32 frame;
  s32 i;

  for (i = 0; i < DUST_PUFFS; i++, sprite++, state++) {
    frame = state->frame;
    if ((u32)frame < DUST_PUFF_FRAMES) {
      sprite->oam.index = dustPuffState->tileBase +
                          ((frame >> 1) & 7) * SPRITE_TILES_8BPP(16, 16);
      EffectSprite_DrawAtWorldPosition(sprite, &state->x);
      BattleEffect_StepVector3(state, 64, 0);
      state->frame++;
    }
  }
}

void VenusLighthouseEntrance_EmitDust(struct Actor *source, s32 x, s32 y,
                                      s32 z) {
  struct DustPuffState *dustPuffState = Ram_HeapSlots->menuState;
  s32 angle = Random16();
  s16 *next = &dustPuffState->next;
  struct EffectSprite *sprite = &dustPuffState->sprites[*next];
  struct SparkState *state = &dustPuffState->states[*next];

  sprite->oam.priority = Actor_GetSpritePriority(source);
  sprite->depth = Actor_GetSpriteDepth(source);
  state->frame = 0;
  state->x = x;
  state->y = y;
  state->z = z;
  FieldVector_AddPolarOffsetFX(Random16() * 2 + toFIXED(2), angle, &state->x);
  state->velocityX = 0;
  state->velocityY = (Random16() >> 1) + toFIXED(3) / 4;
  state->velocityZ = 0;
  FieldVector_AddPolarOffsetFX(Random16() >> 2, angle, &state->velocityX);
  *next = *next + 1;
  *next = *next % DUST_PUFFS;
}

void VenusLighthouseEntrance_StartDust(void) {
  struct DustPuffState *dustPuffState =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct DustPuffState));
  s32 resource;
  s32 tileBase;
  s32 i;
  struct SparkState *state;
  struct EffectSprite *sprite;

  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_DUST_PUFF_A),
                      dustPuffState);
  resource = VramCache_FindFreeEntry();
  tileBase = Vram_Load(resource, 2048, dustPuffState);
  dustPuffState->tileBase = tileBase;
  dustPuffState->resource = resource;
  state = dustPuffState->states;
  for (i = 0; i < DUST_PUFFS; i++) {
    sprite = &dustPuffState->sprites[i];
    EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, tileBase);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = OAM_DEPTH_WINDOW;
    state->frame = -1;
    state++;
  }
  dustPuffState->next = 0;
  Task_AddFrame(VenusLighthouseEntrance_UpdateDust, TASK_PASS_FRAME);
}

void VenusLighthouseEntrance_StopDust(void) {
  struct DustPuffState *state = Ram_HeapSlots->menuState;

  FieldTask_Remove(VenusLighthouseEntrance_UpdateDust);
  VramCache_FreeEntry(state->resource);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VENUS_LIGHTHOUSE_ENTRANCE_FROM_VENUS_LIGHTHOUSE, CONDITION_ALWAYS, 376, 0,
     104, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_PLAZA_PATH, CONDITION_ALWAYS, 376, 0, 232, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {ENTRANCE_BY_WORKMEN, CONDITION_ALWAYS, 384, 0, 392, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {ENTRANCE_WEST_ROAD, CONDITION_ALWAYS, 112, 0, 272, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {ENTRANCE_JENNA_TAKES_OVER, CONDITION_ALWAYS, 376, 0, 104, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {VENUS_LIGHTHOUSE_ENTRANCE_AFTER_DEFEAT, CONDITION_ALWAYS, 384, 0, 168,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_AFTER_RUFFIAN_BATTLE, CONDITION_ALWAYS, 72, 0, 264, FACING_WEST,
     0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(VenusLighthouseEntrance),
    SCENE_EXIT(VENUS_LIGHTHOUSE_ENTRANCE_TO_SUHALLA_GATE_ROAD_EAST,
               SuhallaGateRoad,
               SUHALLA_GATE_ROAD_FROM_VENUS_LIGHTHOUSE_ENTRANCE_EAST),
    SCENE_EXIT(VENUS_LIGHTHOUSE_ENTRANCE_TO_VENUS_LIGHTHOUSE_A, VenusLighthouse,
               VENUS_LIGHTHOUSE_FROM_VENUS_LIGHTHOUSE_ENTRANCE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0,
                    8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEAGULL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352,
                    0, 176, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEAGULL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416,
                    0, 152, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424,
                    0, 408, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440,
                    0, 408, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424,
                    0, 424, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440,
                    0, 440, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456,
                    0, 408, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    328, 0, 408, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 312, 0, 408, FACING_NORTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    328, 0, 424, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 312, 0, 440, FACING_NORTHEAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    296, 0, 408, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24,
                    0, 264, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, CELL_BESIDE_DOOR, FLAG_VENUS_LIGHTHOUSE_BIRDS_FLEW_OFF,
     EVENT(VenusLighthouseEntrance_BirdsFlyOff)},
    {TRIGGER_STEP, CELL_BELOW_DOOR, FLAG_VENUS_LIGHTHOUSE_AMBUSH_SEEN,
     EVENT(VenusLighthouseEntrance_SoldiersAmbushParty)},
    {TRIGGER_STEP, CELL_ROAD_WEST, FLAG_VENUS_LIGHTHOUSE_RUFFIAN_BEATEN,
     EVENT(VenusLighthouseEntrance_RuffianWaitsForBackup)},
    {TRIGGER_STEP, CELL_WAY_EAST, CONDITION_ALWAYS,
     EVENT(VenusLighthouseEntrance_PointToMeetingSpot)},
    {TRIGGER_TALK, ACTOR_WAITING_RUFFIAN, CONDITION_ALWAYS,
     MSGNO(MsgUggghICan)},
    {TRIGGER_TALK, ACTOR_DOUBTING_SOLDIER, CONDITION_ALWAYS,
     EVENT(VenusLighthouseEntrance_BlastActorWithPsynergy)},
    {TRIGGER_TALK, ACTOR_SOLDIER_CAPTAIN, CONDITION_ALWAYS,
     EVENT(VenusLighthouseEntrance_RunAndKickUpDust)},
    {TRIGGER_TABLE_END},
};

// clang-format off
u32 gVenusLighthouseEntrance_BirdLands[] = {
    SC_SYNC,
    SC_WAIT(1),
    SC_ANIMATION(2),
    SC_WAIT(4),
    SC_SET(SW_MOTION, ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS),
    SC_SET(SW_VELOCITY_Y, 0),
    SC_WAIT(1),
    SC_END,
};

u32 gVenusLighthouseEntrance_BirdFlies[] = {
    SC_SYNC,
    SC_WAIT(1),
    SC_RELEASE,
};
// clang-format on
