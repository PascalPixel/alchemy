/// @file
/// Mikasalla, the village in the far south of Osenia, and the tunnels under
/// it, in two scenes that share this overlay (SceneId_Mikasalla, the village,
/// and SceneId_MikasallaNorth, the tunnels).
///
/// Scoop digs up a ladder by the sheep (Mikasalla_DigUpLadder); the tunnels
/// lead to the inn's back yard, where Spark the Mars Djinni joins the party
/// (Mikasalla_MeetSpark).

#include "flags.h"
#include "sprite_ids.h"
#include "shared/tables.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "field/field_sequence_event.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "field/actor_script.h"
#include "field/sprite.h"
#include "djinn_ids.h"

/// Set once Scoop has dug up the ladder down to the tunnels by the sheep; the
/// village redraws it on every visit.
#define FLAG_MIKASALLA_LADDER_DUG_UP 0x85b
/// Set once Parch has dried the tunnels for this visit.
#define FLAG_MIKASALLA_TUNNELS_PARCHED FLAG_SCENE_TEMP_00

extern s16 gMikasalla_TunnelRegions[];
extern struct ActorPlacement gMikasalla_VillagePlacements[];
extern struct ActorPlacement gMikasalla_TunnelPlacements[];

extern struct Trigger gMikasalla_TunnelTriggers[];

extern struct Trigger gMikasalla_VillageTriggers[];
extern s16 gMikasalla_DoorOpening[];
extern s16 gMikasalla_ThirdDoorOpening[];
extern s16 gMikasalla_WideDoorOpening[];
extern s32 gMikasalla_ChickenPaces[];
extern s32 gMikasalla_SheepPaces[];

/// The village's actors, numbering the rows of gMikasalla_VillagePlacements.
enum MikasallaActorId {
  ACTOR_VILLAGER_NO_VISITORS = ACTOR_FIRST_PLACED,
  /// Mikasalla_AskLookingForSomeone's villager.
  ACTOR_VILLAGER_LOOKING_FOR_SOMEONE,
  ACTOR_VILLAGER_AFTER_EARTHQUAKE,
  ACTOR_VILLAGER_DEEPER_INTO_OSENIA,
  ACTOR_VILLAGER_AIRS_ROCK,
  ACTOR_VILLAGER_TWO_WAYS,
  ACTOR_VILLAGER_TUNDARIA,
  ACTOR_VILLAGER_GAROH,
  ACTOR_CHICKEN,
  ACTOR_SHEEP,
  ACTOR_SPARK
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

s16 *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_MIKASALLA_NORTH)
    return gMikasalla_TunnelRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_MIKASALLA)
    return gMikasalla_VillagePlacements;
  return gMikasalla_TunnelPlacements;
}

void TilesetAnim_Pause(s32 a);

void Mikasalla_ClimbDownLadder(void);

extern u8 MsgFieldAreYouLookingForSomeone[];

/// A door that opens with a cell animation: the animation and the cell it
/// plays at.
struct WarpTarget {
  const void *script;
  u16 x;
  u16 z;
};

extern struct WarpTarget gMikasalla_Doors[];

/// The party leader walks into one of the village's doors. Everyone stops, the
/// door opens with its cell animation (gMikasalla_Doors), and the leader walks
/// in and leaves by the exit of the door's number. The fifth door is entered
/// without the walk.
void Mikasalla_EnterDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u32 id;
  s16 *choice;
  s32 index;

  Event_Start();
  Event_ResolvePendingActions(0);
  for (id = 8; id <= 63; id++) {
    struct Actor *actor = Actor_GetById(id);

    if (actor != NULL)
      actor->motionFlags = 0;
  }
  choice = &work->stepTrigger;
  index = *choice;
  Event_PlaySound(SE_DOOR_OPEN);
  index--;
  MapGrid_AnimateCells(gMikasalla_Doors[index].script,
                       gMikasalla_Doors[index].x, gMikasalla_Doors[index].z);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = 0;

  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  if (index != 4) {
    Actor_CenterAndWalk(gMainRoutineWork.leaderActorId, 2, -8);
    Event_Wait(10);
  }
  Import_EventSetExitRequest(*choice);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Spark, the Mars Djinni behind the inn, joins the party when spoken to,
/// without a fight.
void Mikasalla_MeetSpark(void) {
  Djinn_JoinParty(ACTOR_SPARK, DJINNI_SPARK / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_SPARK));
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_MIKASALLA_NORTH)
    return gMikasalla_TunnelTriggers;
  return gMikasalla_VillageTriggers;
}

/// A villager asks whether the party is looking for someone, and answers either
/// way that strangers are rare here.
/// "Are you looking for someone?"
void Mikasalla_AskLookingForSomeone(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)&MsgFieldAreYouLookingForSomeone;
  Event_SetMessageId(msg);
  // "Are you looking for someone?"
  Event_OpenMessage(ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, 0);
  Event_End();
}

/// Scoop on the patch of ground by the sheep digs up the ladder down to the
/// tunnels, and it stays dug, Scene_PrepareMap redrawing it on
/// every visit.
void Mikasalla_DigUpLadder(void) {
  MapGrid_CopyCellsTo(5, 49, 5, 41, 1, 1);
  MapGrid_CopyCellAttributes(4, 48, 3, 2, 4, 40);
  Event_SetFlag(FLAG_MIKASALLA_LADDER_DUG_UP);
}

/// Parch on cell 60 in the tunnels redraws three stretches of the cave and its
/// ground, once a visit, as Scene_PrepareMap redraws them on a
/// return.
void Mikasalla_CastParch(void) {
  if (Event_GetFlag(FLAG_MIKASALLA_TUNNELS_PARCHED) == 0) {
    TilesetAnim_Pause(0);
    TilesetAnim_Pause(1);
    TilesetAnim_Pause(2);
    MapGrid_CopyCellsTo(56, 46, 30, 46, 16, 16);
    MapGrid_CopyCellsTo(56, 174, 30, 174, 16, 16);
    MapGrid_CopyCells(28, 23, 16, 16, 15, 23);
    MapGrid_CopyCells(28, 90, 2, 2, 21, 88);
    MapGrid_CopyCells(28, 90, 4, 4, 17, 89);
    MapGrid_CopyCells(28, 90, 2, 1, 17, 91);
    MapGrid_CopyCells(28, 90, 2, 2, 19, 91);
    MapGrid_CopyCells(28, 92, 2, 2, 15, 92);
    MapGrid_CopyCells(30, 92, 2, 2, 17, 92);
    Event_SetFlag(FLAG_MIKASALLA_TUNNELS_PARCHED);
  }
}

s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  struct MainRoutineWork *party;
  struct Actor *object;
  u8 flags;

  object = Actor_GetById(ACTOR_CHICKEN);
  flags = object->priorityFlags & ~1;
  do {
    party = &gMainRoutineWork;
  } while (0);
  object->priorityFlags = flags;
  Actor_GetById(party->leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (party->next == SCENE_MIKASALLA_NORTH) {
    if (Event_GetFlag(FLAG_MIKASALLA_TUNNELS_PARCHED)) {
      TilesetAnim_Pause(0);
      TilesetAnim_Pause(1);
      TilesetAnim_Pause(2);
      MapGrid_CopyCellsTo(56, 46, 30, 46, 16, 16);
      MapGrid_CopyCellsTo(56, 174, 30, 174, 16, 16);
      MapGrid_CopyCells(28, 23, 16, 16, 15, 23);
      MapGrid_CopyCells(28, 90, 2, 2, 21, 88);
      MapGrid_CopyCells(28, 90, 4, 4, 17, 89);
      MapGrid_CopyCells(28, 90, 2, 1, 17, 91);
      MapGrid_CopyCells(28, 90, 2, 2, 19, 91);
      MapGrid_CopyCells(28, 92, 2, 2, 15, 92);
      MapGrid_CopyCells(30, 92, 2, 2, 17, 92);
    }
  } else if (Event_GetFlag(FLAG_MIKASALLA_LADDER_DUG_UP)) {
    Mikasalla_DigUpLadder();
  }
  return 0;
}

/// The leader, already beside a ladder top, turns north, steps onto the rungs
/// and climbs down out of sight, then leaves by exit 12 from the village or
/// exit 3 from the tunnels.
void Mikasalla_ClimbDownLadder(void) {
  s32 exit;
  s32 drop;

  if (gMainRoutineWork.next == SCENE_MIKASALLA) {
    exit = 12;
    drop = 17;
  } else {
    exit = 3;
    drop = 9;
  }
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_SetDestinationOffset(ACTOR_FELIX, 0, 4);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FELIX), 0);
  Actor_SetAnimation(ACTOR_FELIX, 13);
  Actor_SetDestinationOffset(ACTOR_FELIX, 0, drop);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetAnimation(ACTOR_FELIX, 10);
  Event_Wait(10);
  Event_PlaySound(SE_MAP_EXIT);
  Import_EventSetExitRequest(exit);
  Event_End();
}

/// Stepping onto a ladder top facing east: the leader steps up and across onto
/// the rungs and climbs down.
void Mikasalla_StepOntoLadderFacingEast(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWait(ACTOR_FELIX, 16, 0);
  Mikasalla_ClimbDownLadder();
}

/// Stepping onto a ladder top facing west: the leader steps up and across onto
/// the rungs and climbs down.
void Mikasalla_StepOntoLadderFacingWest(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(ACTOR_FELIX, -2, -6);
  Actor_WalkByAndWait(ACTOR_FELIX, -14, -8);
  Mikasalla_ClimbDownLadder();
}

/// Stepping onto a ladder top facing south: Felix turns north and climbs down.
void Mikasalla_TurnAndClimbDownLadder(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Mikasalla_ClimbDownLadder();
}

/// Stepping onto a ladder top facing north: the leader steps onto the rungs
/// from whichever side of the cell he stands and climbs down.
void Mikasalla_StepOntoLadderFacingNorth(void) {
  s32 x = Actor_GetById(ACTOR_FELIX)->x.part[0].pixel;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (x % 16 <= 7) {
    Actor_WalkByAndWait(ACTOR_FELIX, -4, -4);
    Actor_WalkByAndWait(ACTOR_FELIX, -4, -8);
  } else {
    Actor_WalkByAndWait(ACTOR_FELIX, 4, -4);
    Actor_WalkByAndWait(ACTOR_FELIX, 4, -8);
  }
  Actor_WalkToAndWait(ACTOR_FELIX, 424, 99);
  Mikasalla_ClimbDownLadder();
}

/// Stepping onto a ladder top straight ahead: the leader steps onto the rungs
/// and climbs down.
void Mikasalla_StepOntoLadderAhead(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -4);
  Actor_WalkToAndWait(ACTOR_FELIX, 424, 99);
  Mikasalla_ClimbDownLadder();
}

#include "text/msg_ids.h"

// clang-format off
/// The doors' cell animations: most doors, the third door's own cells, and
/// the fifth door, two cells wide.
s16 gMikasalla_DoorOpening[] = {
    81, 50, 1, 2, 6, 80, 50, 1,
    2, 6, -1,
};

s16 gMikasalla_ThirdDoorOpening[] = {
    79, 50, 1, 2, 6, 78, 50, 1,
    2, 6, -1,
};

s16 gMikasalla_WideDoorOpening[] = {
    78, 52, 2, 2, 6, 78, 53, 2,
    2, 6, -1, 0,
};

/// The village's doors that open with a cell animation as the party walks
/// through.
struct WarpTarget gMikasalla_Doors[] = {
    {gMikasalla_DoorOpening, 84, 29},
    {gMikasalla_DoorOpening, 77, 34},
    {gMikasalla_ThirdDoorOpening, 86, 38},
    {gMikasalla_DoorOpening, 75, 41},
    {gMikasalla_WideDoorOpening, 69, 27},
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 456, 0, 584, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

/// The tunnels' exit regions (Scene_GetExitRegions): two boxes, each with the
/// exit it leads to.
s16 gMikasalla_TunnelRegions[] = {
    68, 28, 404, 76, 36, 412, -1, 1,
    244, 28, 100, 252, 36, 108, -1, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(Mikasalla),
    SCENE_EXIT_UNLESS(1, MikasallaInteriorsHouses, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, MikasallaInteriorsHouses, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, MikasallaInteriorsHouses, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, MikasallaInteriorsHouses, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, MikasallaInteriorsSanctum, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, WorldMap, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, Mikasalla, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, Mikasalla, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, Mikasalla, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, Mikasalla, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, MikasallaNorth, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, MikasallaNorth, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(MikasallaNorth),
    SCENE_EXIT_UNLESS(1, Mikasalla, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, MikasallaNorth, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, MikasallaNorth, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Mikasalla, 11, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// The chicken and the sheep pace back and forth, squashing as they turn.
s32 gMikasalla_ChickenPaces[] = {                                                               
      SC_SET(SW_COLLISION, 129),                                               
      SC_ACCELERATION(0.2),                                                
      SC_SPEED(0.6),                                                    
      SC_LABEL(1),                                                             
      SC_ANIMATION(2),                                                               
      SC_MOVE(120, 0, 648),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_WAIT(20), /* A half turn; FACING_WEST names a heading, not a turn. */ 
      SC_ADD(SW_FACING, -0x8000),                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, 0x2000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x2000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(4),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, 0x2000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x2000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(4),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, 0x2000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x2000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(4),                                                              
      SC_ANIMATION(2),                                                               
      SC_MOVE(72, 0, 648),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_WAIT(20), /* A half turn; FACING_WEST names a heading, not a turn. */ 
      SC_ADD(SW_FACING, -0x8000),                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, 0x2000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x2000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(4),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, 0x2000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x2000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(4),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, 0x2000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x2000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(4),                                                              
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
      SC_END,                                                                  
  };

/// The sheep's script: it wanders its pen.
s32 gMikasalla_SheepPaces[] = {                                                               
      SC_SET(SW_COLLISION, 129),                                               
      SC_ACCELERATION(0.2),                                                
      SC_SPEED(0.3),                                                    
      SC_LABEL(1),                                                             
      SC_ANIMATION(2),                                                               
      SC_MOVE(88, 0, 680),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_WAIT(20), /* A half turn; FACING_WEST names a heading, not a turn. */ 
      SC_ADD(SW_FACING, -0x8000),                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, 0x2000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x2000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(4),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, 0x2000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x2000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(4),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, 0x2000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x2000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(4),                                                              
      SC_ANIMATION(2),                                                               
      SC_MOVE(120, 0, 680),                                      
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_WAIT(20), /* A half turn; FACING_WEST names a heading, not a turn. */ 
      SC_ADD(SW_FACING, -0x8000),                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, 0x2000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x2000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(4),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, 0x2000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x2000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(4),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, 0x2000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x2000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, 0x1000),                                              
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),                                               
      SC_WAIT(4),                                                              
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
      SC_END,                                                                  
  };

/// The village (enum MikasallaActorId).
struct ActorPlacement gMikasalla_VillagePlacements[] = {
    ACTOR_PLACEMENT(71, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 552, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(72, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 600, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(73, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 504, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(74, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 264, 0, 680, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(75, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 664, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(76, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 680, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(77, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 632, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(78, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 616, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(203, CONDITION_ALWAYS, (s32)gMikasalla_ChickenPaces, 72, 0, 648, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(81, CONDITION_ALWAYS, (s32)gMikasalla_SheepPaces, 120, 0, 680, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, 0x60, MOVEMENT_TYPE_STAND, 312, 0, 168, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The tunnels under the village (MikasallaNorth).
struct ActorPlacement gMikasalla_TunnelPlacements[] = {
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
/// The village's doors, talk, Mind Read, the ladders, and Spark.
struct Trigger gMikasalla_VillageTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Mikasalla_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Mikasalla_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(Mikasalla_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(Mikasalla_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Mikasalla_EnterDoor)},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 30, CONDITION_ALWAYS, EVENT(Mikasalla_TurnAndClimbDownLadder)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS, EVENT(Mikasalla_StepOntoLadderFacingEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS, EVENT(Mikasalla_StepOntoLadderFacingWest)},
    {TRIGGER_TALK, ACTOR_VILLAGER_NO_VISITORS, CONDITION_ALWAYS, MSGNO(MsgFieldItSBeenALong)},
    {TRIGGER_TALK, ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, CONDITION_ALWAYS, EVENT(Mikasalla_AskLookingForSomeone)},
    {TRIGGER_TALK, ACTOR_VILLAGER_AFTER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgFieldIWentOutToInvestigate)},
    {TRIGGER_TALK, ACTOR_VILLAGER_DEEPER_INTO_OSENIA, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouReGoingDeeper)},
    {TRIGGER_TALK, ACTOR_VILLAGER_AIRS_ROCK, CONDITION_ALWAYS, MSGNO(MsgFieldInTheHeartOfOsenia)},
    {TRIGGER_TALK, ACTOR_VILLAGER_TWO_WAYS, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouWantToGo)},
    {TRIGGER_TALK, ACTOR_VILLAGER_TUNDARIA, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouLookAcrossThe)},
    {TRIGGER_TALK, ACTOR_VILLAGER_GAROH, CONDITION_ALWAYS, MSGNO(MsgFieldATownCalledGarohLies)},
    {TRIGGER_TALK, ACTOR_CHICKEN, CONDITION_ALWAYS, MSGNO(MsgFieldBukBukBuk)},
    {TRIGGER_TALK, ACTOR_SHEEP, CONDITION_ALWAYS, MSGNO(MsgFieldBahBaaaaaaa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_NO_VISITORS, CONDITION_ALWAYS, MSGNO(MsgFieldWhyWouldAnyoneTravelAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, CONDITION_ALWAYS, MSGNO(MsgFieldILikeThisTimeOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_AFTER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgFieldOseniaWasBigAlreadyBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DEEPER_INTO_OSENIA, CONDITION_ALWAYS, MSGNO(MsgFieldNoWayIDLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_AIRS_ROCK, CONDITION_ALWAYS, MSGNO(MsgFieldAirSRockHasStood)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TWO_WAYS, CONDITION_ALWAYS, MSGNO(MsgFieldItSWeirdThatYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TUNDARIA, CONDITION_ALWAYS, MSGNO(MsgFieldItSBitterColdIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_GAROH, CONDITION_ALWAYS, MSGNO(MsgFieldThePeopleInGarohAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHICKEN, CONDITION_ALWAYS, MSGNO(MsgFieldTheGroundHereIsSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHEEP, CONDITION_ALWAYS, MSGNO(MsgFieldOhYeahMmmIJust)},
    {TRIGGER_TALK, ACTOR_SPARK, CONDITION_ALWAYS, EVENT(Mikasalla_MeetSpark)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgFieldItSCompletelyFullOf))},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 50, CONDITION_ALWAYS, EVENT(Mikasalla_DigUpLadder)},
    {TRIGGER_TABLE_END},
};
// clang-format on

/// The tunnels' exits and the ladders.
struct Trigger gMikasalla_TunnelTriggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 30, CONDITION_ALWAYS,
     EVENT(Mikasalla_TurnAndClimbDownLadder)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS,
     EVENT(Mikasalla_StepOntoLadderFacingEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS,
     EVENT(Mikasalla_StepOntoLadderFacingWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 33, CONDITION_ALWAYS,
     EVENT(Mikasalla_StepOntoLadderFacingNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 34, CONDITION_ALWAYS,
     EVENT(Mikasalla_StepOntoLadderAhead)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 60,
     FLAG_MIKASALLA_TUNNELS_PARCHED, EVENT(Mikasalla_CastParch)},
    {TRIGGER_TABLE_END},
};
