/// @file
/// Tret Tree's rooms (SceneId_TreeRooms): the hollow inside of Tret, the holy
/// tree of Kolima Forest, where the party come to face his wrathful side. Six
/// rooms are stacked inside the trunk. Doors in the four upper floors lead out
/// onto his branches (SceneId_TretTreeBranch, tret_tree_branch.c), where vines
/// climb from one floor's door to the next.
///
/// The party come in from Kolima Forest (kolima_forest_tree.c) by the door at
/// the south end of the entrance room (cell (30, 16)), a maze of root walls
/// with a round hole in the middle of its floor (cell (30, 9)). A vine near its
/// north wall (cell (26, 4)) climbs to the second floor.
///
/// The floors above are spider webs stretched over holes, with yellow leaves
/// lying on them. Isaac crosses a web by jumping two cells at a time from leaf
/// to leaf (TreeRooms_JumpOffLeaf). The first time he lands on a leaf it tears.
/// The next time it breaks, and he falls through into the room below, straight
/// under the leaf (TreeRooms_StepOnLeaf). Four leaves are torn from the start.
///
/// - The second floor's web holds nine leaves, and its west and east doors
///   lead out onto the branches. Its leaves drop Isaac into the entrance room.
/// - The third floor, entered from the branches by its south or east door, has
///   a leaf on a web either side of the Healing Ring's chest, a Psynergy Stone
///   by its north wall and a vine up to the fourth floor. Its leaves drop him
///   to the second floor. A third web, in the south-west, has only a small
///   spider on it.
/// - The fourth floor is bare, with a hole in the middle of its floor, the top
///   of the vine from the third floor, a west door and a south door.
/// - The fifth floor, the highest, is one great web of ten leaves beside the
///   Nut's chest, entered by its west or east door. Its leaves drop him to the
///   fourth floor.
///
/// The leaf in the middle of the fifth floor's web is the way to Tret. It drops
/// Isaac through the holes in the fourth, third and second floors and the
/// entrance room, in turn, into Tret's room under the entrance room, where his
/// face looks out of the north wall (TreeRooms_LandFromFall).
///
/// Walking up to the face starts the battle with Tret (TreeRooms_FightTret).
/// Afterwards he comes to himself, finds he turned the people of Kolima into
/// trees, fails to free them and sinks away, and a vine grows in the middle of
/// his room (TreeRooms_PlayPostBattleConversation). From then on that vine
/// joins his room and the hole in the entrance room's floor. FLAG_TRET_DEFEATED
/// records the battle won and the conversation over. In Kolima Forest it lets
/// Hermes' Water heal Tret and lift the curse.
///
/// The light inside the tree slowly swells and fades
/// (TretPalette_UpdateMapTint), and casting Reveal stops it.
///
/// Each leaf has a temporary flag of its own, set when it tears
/// (FLAG_TRET_TREE_SECOND_FLOOR_LEAF_A_TORN and the others). The cells of a
/// leaf carry the number of the entrance its fall lands at, so the step
/// triggers name them by those entrances. A torn leaf is redrawn after a
/// battle (FLAG_KEEP_PARTY_POSITION) from gTreeRooms_MapPatches.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/create_configured_actor.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/map_grid_scroll.h"
#include "field/scene.h"
#include "field/scene_tables.h"
#include "field/speaker.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/field_effect_spawn.h"
#include "scene/walk_into_leader.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/dma.h"
#include "system/heap_slots.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern s16 gTreeRooms_MapPatches[];
extern s32 gTreeRooms_LeaderGathersActions[];
extern s32 gTreeRooms_GaretGathersActions[];
extern s32 gTreeRooms_IvanGathersActions[];
extern s32 gTreeRooms_MiaGathersActions[];
extern s32 gTreeRooms_FaceSinksActions[];
extern s32 gTreeRooms_WalkBackToLeaderScript[];
extern u32 gTreeRooms_TintSteps[];
extern struct SceneEntrance gTreeRoomsEntrances[];
extern struct SceneExitRegion gTreeRoomsRegions[];
extern u32 gTreeRoomsExits[];
extern struct ActorPlacement gTreeRoomsActorPlacements[];
extern struct Trigger gTreeRoomsTriggers[];
extern s32 gTreeRooms_TintIndex;

/// Tret's face in the north wall of his room (SPRITE_TRET), the first actor
/// placed after the party, and a second Tret actor (SPRITE_TRET_RAISED) placed
/// at the same spot.
///
/// Both speak Tret's lines. The face scorns the party before the battle; the
/// second roars before it and speaks every line after it.
enum {
  ACTOR_TRET = ACTOR_FIRST_PLACED,
  /// Draws nothing the render shows beyond Tret's face.
  ACTOR_SECOND_TRET
};

/// The speaker of a message. Every speaker in this scene has
/// SPEAKER_WINDOW_BELOW.
#define TREE_ROOMS_SPEAKER(actorId) (SPEAKER_WINDOW_BELOW | (actorId))

/// The cells Tret Tree's examine and step triggers answer besides the doors,
/// the vines and the leaves. The doors' cells and the tops of the vines that
/// lead down carry the numbers of their exits, and each leaf's cell the number
/// of the entrance its fall lands at.
enum TreeRoomsCellBehavior {
  /// The row in front of Tret's face in the lowest room, (9, 5) to (11, 5).
  CELL_BEHAVIOR_TREE_ROOMS_BEFORE_TRET = 40,
  /// The Healing Ring's chest on the third floor, between its webs (10, 24).
  CELL_BEHAVIOR_TREE_ROOMS_HEALING_RING_CHEST = 100,
  /// The Nut's chest on the fifth floor, east of its web (54, 25).
  CELL_BEHAVIOR_TREE_ROOMS_NUT_CHEST = 101,
  /// The Psynergy Stone by the third floor's north wall (9, 21).
  CELL_BEHAVIOR_TREE_ROOMS_PSYNERGY_STONE = 102
};

/// How long Isaac spins as he drops through a broken leaf.
#define LEAF_FALL_SPIN_FRAMES 30
#define SWIRL_SPARK_LAST_FRAME 79
#define SWIRL_SPARK_SHRINK_LAST_FRAME 39
/// The light's slow swell inside Tret (TretPalette_UpdateMapTint) covers
/// background palettes 1 to 4 but palette 4's last colour.
#define TRET_TINT_COLOR_COUNT (4 * PALETTE_COLORS - 1)
#define TRET_TINT_CHANNEL_MAX 31
#define TRET_TINT_END 99

/// The fall from the fifth floor's centre leaf goes down a floor at a time
/// through entrances ten apart: TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_LEAF on
/// the fourth floor, then TREE_ROOMS_FALL_THROUGH_A to _C, then
/// TREE_ROOMS_FALL_LANDING in Tret's room.
#define FALL_ENTRANCE_STEP                                                     \
  (TREE_ROOMS_FALL_THROUGH_B - TREE_ROOMS_FALL_THROUGH_C)

static void TretPalette_SaveMapColors(void);
static void TretPalette_UpdateMapTint(void);
/// The face changes TreeRooms_ChangeFace plays on Tret. Each starts from the
/// face with its eyes open, and the wrathful ones are on his wrathful face.
enum TreeRoomsFaceChange {
  TRET_FACE_CHANGE_CLOSE_EYES = 0,
  TRET_FACE_CHANGE_OPEN_EYES = 1,
  TRET_FACE_CHANGE_LOOK_LEFT = 2,
  TRET_FACE_CHANGE_LOOK_RIGHT = 3,
  TRET_FACE_CHANGE_BLINK = 4, ///< twice
  TRET_FACE_CHANGE_HALF_CLOSE_EYES = 5,
  TRET_FACE_CHANGE_WRATHFUL_CLOSE_EYES = 7,
  TRET_FACE_CHANGE_WRATHFUL_OPEN_EYES = 8,
  TRET_FACE_CHANGE_WRATHFUL_LOOK_RIGHT = 9,
  TRET_FACE_CHANGE_WRATHFUL_LOOK_LEFT = 10,
  TRET_FACE_CHANGE_WRATHFUL_BLINK = 11, ///< twice
  TRET_FACE_CHANGE_WRATHFUL_STARE =
      12 ///< as TRET_FACE_CHANGE_WRATHFUL_OPEN_EYES
};

static void TreeRooms_ChangeFace(s32 change);
static void TreeRooms_SpawnSwirlSparks(void);
static void TreeRooms_ApplyFlaggedMapPatches(void);
static void TreeRooms_LandFromFall(void);

static struct FieldPosition sSparkOrigin ALIGNED(8);
static s32 sSparkFrame;
static s16 *sLastLeaf;

#include "shared/field_effect_create.c"

#include "shared/field_effect_spawn.c"

/// Set once the west leaf of the pair in the north of the second floor's web,
/// cell (49, 6), is torn, so that landing on it again breaks it.
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_A_TORN FLAG_SCENE_TEMP_00
/// Set once the east leaf of the pair in the north of the second floor's web,
/// cell (51, 6), is torn, so that landing on it again breaks it. It is torn
/// from the start: the setup sets this flag.
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_B_TORN FLAG_SCENE_TEMP_01
/// Set once the upper of the two leaves down the west side of the second
/// floor's web, cell (46, 8), is torn, so that landing on it again breaks it.
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_C_TORN FLAG_SCENE_TEMP_02
/// Set once the lower of the two leaves down the west side of the second
/// floor's web, cell (46, 10), is torn, so that landing on it again breaks it.
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_D_TORN FLAG_SCENE_TEMP_03
/// Set once the west leaf of the pair in the middle of the second floor's web,
/// cell (47, 12), is torn, so that landing on it again breaks it.
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_E_TORN FLAG_SCENE_TEMP_04
/// Set once the east leaf of the pair in the middle of the second floor's web,
/// cell (49, 12), is torn, so that landing on it again breaks it.
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_F_TORN FLAG_SCENE_TEMP_05
/// Set once the west leaf of the row along the south of the second floor's web,
/// cell (48, 14), is torn, so that landing on it again breaks it.
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_G_TORN FLAG_SCENE_TEMP_06
/// Set once the middle leaf of the row along the south of the second floor's
/// web, cell (50, 14), is torn, so that landing on it again breaks it.
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_H_TORN FLAG_SCENE_TEMP_07
/// Set once the east leaf of the row along the south of the second floor's web,
/// cell (52, 14), is torn, so that landing on it again breaks it.
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_I_TORN FLAG_SCENE_TEMP_08
/// Set once the leaf on the third floor's west web, cell (7, 23), is torn, so
/// that landing on it again breaks it.
#define FLAG_TRET_TREE_THIRD_FLOOR_WEST_LEAF_TORN FLAG_SCENE_TEMP_09
/// Set once the leaf on the third floor's east web, cell (13, 24), is torn, so
/// that landing on it again breaks it.
#define FLAG_TRET_TREE_THIRD_FLOOR_EAST_LEAF_TORN FLAG_SCENE_TEMP_0A
/// Set once the leaf in the middle of the fifth floor's web, the way down to
/// Tret, cell (50, 26), is torn, so that landing on it again breaks it.
#define FLAG_TRET_TREE_FIFTH_FLOOR_CENTER_LEAF_TORN FLAG_SCENE_TEMP_0B
/// Set once the north-west leaf of the fifth floor's web, cell (48, 24), is
/// torn, so that landing on it again breaks it.
#define FLAG_TRET_TREE_FIFTH_FLOOR_NORTHWEST_LEAF_TORN FLAG_SCENE_TEMP_0C
/// Set once the north leaf of the fifth floor's web, cell (50, 24), is torn, so
/// that landing on it again breaks it. It is torn from the start: the setup
/// sets this flag.
#define FLAG_TRET_TREE_FIFTH_FLOOR_NORTH_LEAF_TORN FLAG_SCENE_TEMP_0D
/// Set once the north-east leaf of the fifth floor's web, cell (52, 24), is
/// torn, so that landing on it again breaks it.
#define FLAG_TRET_TREE_FIFTH_FLOOR_NORTHEAST_LEAF_TORN FLAG_SCENE_TEMP_0E
/// Set once the west leaf of the fifth floor's web, cell (48, 26), is torn, so
/// that landing on it again breaks it. It is torn from the start: the setup
/// sets this flag.
#define FLAG_TRET_TREE_FIFTH_FLOOR_WEST_LEAF_TORN FLAG_SCENE_TEMP_0F
/// Set once the east leaf of the fifth floor's web, cell (52, 26), is torn, so
/// that landing on it again breaks it.
#define FLAG_TRET_TREE_FIFTH_FLOOR_EAST_LEAF_TORN FLAG_SCENE_TEMP_10
/// Set once the south-west leaf of the fifth floor's web, cell (48, 28), is
/// torn, so that landing on it again breaks it.
#define FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHWEST_LEAF_TORN FLAG_SCENE_TEMP_11
/// Set once the south leaf of the fifth floor's web, cell (50, 28), is torn, so
/// that landing on it again breaks it.
#define FLAG_TRET_TREE_FIFTH_FLOOR_SOUTH_LEAF_TORN FLAG_SCENE_TEMP_12
/// Set once the south-east leaf of the fifth floor's web, cell (52, 28), is
/// torn, so that landing on it again breaks it. It is torn from the start: the
/// setup sets this flag.
#define FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHEAST_LEAF_TORN FLAG_SCENE_TEMP_13
/// Set once the leaf at the east end of the fifth floor's south row, cell (54,
/// 28), is torn, so that landing on it again breaks it.
#define FLAG_TRET_TREE_FIFTH_FLOOR_FAR_SOUTHEAST_LEAF_TORN FLAG_SCENE_TEMP_14

/// Returns the scene's entrances, where the leaves' falls land among them.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gTreeRoomsEntrances;
}

/// Returns the tops of the three vines that climb to the room above: from the
/// entrance room to the second floor, from the third floor to the fourth, and,
/// once Tret is beaten, from Tret's room to the entrance room. Climbing to
/// between 28 and 36 pixels up one of them takes Isaac through.

const struct SceneExitRegion *Scene_GetExitRegions(void) {
  return gTreeRoomsRegions;
}

const u32 *Scene_GetExits(void) { return gTreeRoomsExits; }

/// Returns the party's other slots (Garet, Ivan and, once she has joined,
/// Mia), placed for the events, and the two Tret actors.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gTreeRoomsActorPlacements;
}

/// Isaac landing on a leaf on one of Tret Tree's webs tears it the first time.
/// The next time the leaf breaks: he starts in surprise, spins as he drops
/// through the web, leaving a hole where the leaf lay, and falls into the room
/// below, landing straight under the leaf.
/// The torn leaf and the hole are pictures kept on a spare row of the map
/// (`sourceColumn` on `sourceRow`, and the row below it), copied over the
/// leaf's cell in the map's upper layer. `entrance` is where the fall lands.
/// `sLastLeaf` keeps a landing from counting again while he stands on the leaf,
/// until he jumps off it (TreeRooms_JumpOffLeaf). FLAG_ARRIVE_FALLING has him
/// drop into the room below, except from the fifth floor's centre leaf, whose
/// fall goes on down through the rooms (Scene_Initialize).
static void TreeRooms_StepOnLeaf(s32 leafFlag, s32 sourceColumn, s32 sourceRow,
                                 s32 entrance) {
  s32 destinationColumn;
  s32 destinationRow;
  s32 leaderActorId;
  struct Actor *actor;
  s32 spinFrame;

  destinationColumn = FIELD_CELL(gMainRoutineWork.x) + 64;
  destinationRow = FIELD_CELL(gMainRoutineWork.z);
  leaderActorId = gMainRoutineWork.leaderActorId;
  actor = Actor_Get(leaderActorId);
  if (entrance == *sLastLeaf)
    return;
  *sLastLeaf = entrance;
  if (GameFlag_Get(leafFlag) == 0) {
    MapGrid_CopyCells(sourceColumn, sourceRow, 1, 1, destinationColumn,
                      destinationRow);
    GameFlag_Set(leafFlag);
    return;
  }
  // Landing on a torn leaf: draw the hole and spin Isaac down through the
  // web.
  *sLastLeaf = -1;
  MapGrid_CopyCells(sourceColumn, sourceRow + 1, 1, 1, destinationColumn,
                    destinationRow);
  Sound_Play(SE_CURSE_FALL);
  Event_Start();
  Event_SetDestination(SCENE_TREE_ROOMS, entrance);
  Actor_SetAnimation(leaderActorId, ANIM_LEADER_WALK_BACKWARDS);
  Actor_SetSpriteFlags(Actor_Get(leaderActorId), 0);
  Actor_SetAttachedEffect(leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Event_Wait(30);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  actor->motionFlags = ACTOR_FALLS;
  actor->groundHeight = -toFIXED(160);
  actor->gravity = toFIXED(1) / 2;
  Sound_Play(SE_DUST_PUFF);
  Event_Wait(3);
  actor->layer = 2;
  Actor_SetSpritePriority(leaderActorId, 3);
  for (spinFrame = 0; spinFrame < LEAF_FALL_SPIN_FRAMES; spinFrame++) {
    actor->facing += 2 * FACING_STEP;
    wait(1);
  }
  if (entrance != TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_LEAF)
    GameFlag_Set(FLAG_ARRIVE_FALLING);
}

// The leaves' step triggers, one for each leaf: each passes its flag, the
// column of its torn picture on the spare row and its landing entrance to
// TreeRooms_StepOnLeaf. The second floor's nine are lettered A to I in the
// order of their landing entrances, 21 to 29.

/// Isaac landing on the west leaf of the pair in the north of the second
/// floor's web (cell (49, 6)) tears it. Landing on it again breaks it, and he
/// falls into the entrance room below.
static void TreeRooms_StepOnSecondFloorLeafA(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_A_TORN, 64, 35,
                       TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_A);
}

/// The east leaf of the pair in the north of the second floor's web (cell (51,
/// 6)) is torn from the start, so the first time Isaac lands on it, it breaks
/// and he falls into the entrance room below.
static void TreeRooms_StepOnSecondFloorLeafB(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_B_TORN, 65, 35,
                       TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_B);
}

/// Isaac landing on the upper of the two leaves down the west side of the
/// second floor's web (cell (46, 8)), the one he can jump to from the floor by
/// the west door, tears it. Landing on it again breaks it, and he falls into
/// the entrance room below.
static void TreeRooms_StepOnSecondFloorLeafC(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_C_TORN, 66, 35,
                       TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_C);
}

/// Isaac landing on the lower of the two leaves down the west side of the
/// second floor's web (cell (46, 10)) tears it. Landing on it again breaks it,
/// and he falls into the entrance room below.
static void TreeRooms_StepOnSecondFloorLeafD(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_D_TORN, 67, 35,
                       TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_D);
}

/// Isaac landing on the west leaf of the pair in the middle of the second
/// floor's web (cell (47, 12)) tears it. Landing on it again breaks it, and he
/// falls into the entrance room below.
static void TreeRooms_StepOnSecondFloorLeafE(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_E_TORN, 68, 35,
                       TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_E);
}

/// Isaac landing on the east leaf of the pair in the middle of the second
/// floor's web (cell (49, 12)) tears it. Landing on it again breaks it, and he
/// falls into the entrance room below.
static void TreeRooms_StepOnSecondFloorLeafF(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_F_TORN, 69, 35,
                       TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_F);
}

/// Isaac landing on the west leaf of the row along the south of the second
/// floor's web (cell (48, 14)) tears it. Landing on it again breaks it, and he
/// falls into the entrance room below.
static void TreeRooms_StepOnSecondFloorLeafG(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_G_TORN, 70, 35,
                       TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_G);
}

/// Isaac landing on the middle leaf of the row along the south of the second
/// floor's web (cell (50, 14)) tears it. Landing on it again breaks it, and he
/// falls into the entrance room below.
static void TreeRooms_StepOnSecondFloorLeafH(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_H_TORN, 71, 35,
                       TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_H);
}

/// Isaac landing on the east leaf of the row along the south of the second
/// floor's web (cell (52, 14)) tears it. Landing on it again breaks it, and he
/// falls into the entrance room below.
static void TreeRooms_StepOnSecondFloorLeafI(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_I_TORN, 72, 35,
                       TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_I);
}

/// Isaac landing on the leaf on the third floor's west web (cell (7, 23)), west
/// of the Healing Ring's chest, tears it. Landing on it again breaks it, and he
/// falls to the second floor.
static void TreeRooms_StepOnThirdFloorWestLeaf(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_THIRD_FLOOR_WEST_LEAF_TORN, 73, 35,
                       TREE_ROOMS_FROM_THIRD_FLOOR_WEST_LEAF);
}

/// Isaac landing on the leaf on the third floor's east web (cell (13, 24)),
/// east of the Healing Ring's chest and below the vine to the fourth floor,
/// tears it. Landing on it again breaks it, and he falls to the second floor.
static void TreeRooms_StepOnThirdFloorEastLeaf(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_THIRD_FLOOR_EAST_LEAF_TORN, 74, 35,
                       TREE_ROOMS_FROM_THIRD_FLOOR_EAST_LEAF);
}

/// Isaac landing on the leaf in the middle of the fifth floor's web (cell (50,
/// 26)), just south of the torn north leaf, tears it. Landing on it again
/// breaks it, and he falls all the way down to Tret: through the fourth floor,
/// the third, the second and the entrance room, into Tret's room at the bottom
/// of the tree.
static void TreeRooms_StepOnFifthFloorCenterLeaf(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_CENTER_LEAF_TORN, 79, 35,
                       TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_LEAF);
}

/// Isaac landing on the north-west leaf of the fifth floor's web (cell (48,
/// 24)) tears it. Landing on it again breaks it, and he falls to the fourth
/// floor.
static void TreeRooms_StepOnFifthFloorNorthwestLeaf(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_NORTHWEST_LEAF_TORN, 75, 35,
                       TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHWEST_LEAF);
}

/// The north leaf of the fifth floor's web (cell (50, 24)) is torn from the
/// start, so the first time Isaac lands on it, it breaks and he falls to the
/// fourth floor.
static void TreeRooms_StepOnFifthFloorNorthLeaf(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_NORTH_LEAF_TORN, 76, 35,
                       TREE_ROOMS_FROM_FIFTH_FLOOR_NORTH_LEAF);
}

/// Isaac landing on the north-east leaf of the fifth floor's web (cell (52,
/// 24)), west of the Nut's chest, tears it. Landing on it again breaks it, and
/// he falls to the fourth floor.
static void TreeRooms_StepOnFifthFloorNortheastLeaf(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_NORTHEAST_LEAF_TORN, 77, 35,
                       TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHEAST_LEAF);
}

/// The west leaf of the fifth floor's web (cell (48, 26)) is torn from the
/// start, so the first time Isaac lands on it, it breaks and he falls to the
/// fourth floor.
static void TreeRooms_StepOnFifthFloorWestLeaf(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_WEST_LEAF_TORN, 78, 35,
                       TREE_ROOMS_FROM_FIFTH_FLOOR_WEST_LEAF);
}

/// Isaac landing on the east leaf of the fifth floor's web (cell (52, 26))
/// tears it. Landing on it again breaks it, and he falls to the fourth floor.
static void TreeRooms_StepOnFifthFloorEastLeaf(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_EAST_LEAF_TORN, 80, 35,
                       TREE_ROOMS_FROM_FIFTH_FLOOR_EAST_LEAF);
}

/// Isaac landing on the south-west leaf of the fifth floor's web (cell (48,
/// 28)) tears it. Landing on it again breaks it, and he falls to the fourth
/// floor, not straight under it but two cells further north, where the west
/// leaf's fall lands (cell (28, 26)).
static void TreeRooms_StepOnFifthFloorSouthwestLeaf(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHWEST_LEAF_TORN, 81, 35,
                       TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHWEST_LEAF);
}

/// Isaac landing on the south leaf of the fifth floor's web (cell (50, 28))
/// tears it. Landing on it again breaks it, and he falls to the fourth floor.
static void TreeRooms_StepOnFifthFloorSouthLeaf(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_SOUTH_LEAF_TORN, 82, 35,
                       TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTH_LEAF);
}

/// The south-east leaf of the fifth floor's web (cell (52, 28)) is torn from
/// the start, so the first time Isaac lands on it, it breaks and he falls to
/// the fourth floor.
static void TreeRooms_StepOnFifthFloorSoutheastLeaf(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHEAST_LEAF_TORN, 83, 35,
                       TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHEAST_LEAF);
}

/// Isaac landing on the leaf at the east end of the fifth floor's south row
/// (cell (54, 28)), below the Nut's chest, tears it. Landing on it again breaks
/// it, and he falls to the fourth floor.
static void TreeRooms_StepOnFifthFloorFarSoutheastLeaf(void) {
  TreeRooms_StepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_FAR_SOUTHEAST_LEAF_TORN, 84,
                       35, TREE_ROOMS_FROM_FIFTH_FLOOR_FAR_SOUTHEAST_LEAF);
}

/// Walking while Isaac stands on a leaf in Tret Tree makes him jump two cells
/// that way, over the web to the next leaf, wherever there is room to land
/// (Field_TryJumpForward).
/// Once he has jumped, `sLastLeaf` is cleared, so that the leaf he lands on
/// counts as a fresh landing.
static void TreeRooms_JumpOffLeaf(void) {
  if (Field_TryJumpForward() == 0)
    *sLastLeaf = -1;
}

/// Returns the doors, the vines, the leaves, Tret, Reveal, the chests and the
/// Psynergy Stone.
const struct Trigger *Scene_GetTriggers(void) { return gTreeRoomsTriggers; }

/// Walking up to Tret's face in the north wall of the lowest room in Tret Tree
/// (cells (9, 5) to (11, 5)) starts the battle with him. He roars "Hm
/// HROOOM!!!", Garet, Ivan and Mia, once she has joined, gather beside Isaac
/// before him, and his face turns wrathful: "You hoped to find my kindly self?"
/// The party start back, and he swears they will all become trees before the
/// screen breaks into bars for the battle.
/// It runs only while FLAG_TRET_DEFEATED is clear. The battle returns to
/// TREE_ROOMS_AFTER_FIGHT_TRET, where the setup plays
/// TreeRooms_PlayPostBattleConversation.
static void TreeRooms_FightTret(void) {
  s32 hasMia;

  hasMia = GameFlag_Get(FLAG_IN_PARTY(MIA));
  Event_Start();
  Sound_Play(SOUND_FADE_OUT_BGM);
  Event_SetMessageId(MsgTretHmHrooom);
  // "Hm HROOOM!!!"
  Event_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 20);
  Sound_Play(MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_Get(ACTOR_MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_MIA, 2);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Scene_PlaceAtLeader(ACTOR_GARET);
  Scene_PlaceAtLeader(ACTOR_IVAN);
  if (hasMia != 0) {
    Scene_PlaceAtLeader(ACTOR_MIA);
    Actor_SetScript(ACTOR_MIA, gTreeRooms_MiaGathersActions);
  }
  Actor_SetScript(ACTOR_PARTY_LEADER, gTreeRooms_LeaderGathersActions);
  Actor_SetScript(ACTOR_GARET, gTreeRooms_GaretGathersActions);
  Actor_RunScriptAndWait(ACTOR_IVAN, gTreeRooms_IvanGathersActions);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 40);
  Actor_SetAnimation(ACTOR_TRET, TRET_FACE_TURNING_WRATHFUL);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_CLOSED);
  Event_Wait(20);
  TreeRooms_ChangeFace(TRET_FACE_CHANGE_WRATHFUL_OPEN_EYES);
  Event_ShowMessage(TREE_ROOMS_SPEAKER(ACTOR_TRET), 0);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  TreeRooms_ChangeFace(TRET_FACE_CHANGE_WRATHFUL_BLINK);
  FieldEvent_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_TRET), 0, 10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_StartRepeatedMotion(ACTOR_MIA, 1);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_ShowMessage(TREE_ROOMS_SPEAKER(ACTOR_TRET), 0);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  TreeRooms_ChangeFace(TRET_FACE_CHANGE_WRATHFUL_BLINK);
  Event_ShowMessage(TREE_ROOMS_SPEAKER(ACTOR_TRET), 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  gFieldWork->transitionFrames = 64;
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(SCENE_TREE_ROOMS,
                                  TREE_ROOMS_AFTER_FIGHT_TRET);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY,
                                        0);
  Event_End();
}

/// After the battle with Tret, the view slides down the middle of his room as a
/// vine grows up out of the floor, and Tret comes to himself ("Oh...
/// Mmmmm..."). He thanks the party and says good-bye, but Garet and Ivan will
/// not let him die while they still need his help. Learning that he turned the
/// people of Kolima into trees, he tries to free them: the room turns purple
/// and sparks swirl out from his face, but his power no longer reaches Kolima.
/// His face sinks into the wall, and the party resolve to heal him.
/// Only then is FLAG_TRET_DEFEATED set and the light's slow swell started again
/// (TretPalette_UpdateMapTint). Answering "Do you need something?" with no
/// makes Garet and Ivan scold Isaac.
static void TreeRooms_PlayPostBattleConversation(void) {
  s32 hasMia;
  TaskFunc sparkTask;
  const s32 *actions;

  hasMia = GameFlag_Get(FLAG_IN_PARTY(MIA));
  Actor_Get(ACTOR_MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_MIA, 2);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  PartyInventory_FindMember(ITEM_HERMES_WATER);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  Actor_SetSpeed(ACTOR_MIA, 0.8);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 166, 80);
  Actor_Get(ACTOR_PARTY_LEADER)->facing = FACING_NORTH;
  Actor_SetPosition(ACTOR_GARET, 148, 90);
  Actor_Get(ACTOR_GARET)->facing = FACING_NORTH;
  Actor_SetPosition(ACTOR_IVAN, 182, 90);
  Actor_Get(ACTOR_IVAN)->facing = FACING_NORTH;
  if (hasMia != 0) {
    Actor_SetPosition(ACTOR_MIA, 166, 104);
    Actor_Get(ACTOR_MIA)->facing = FACING_NORTH;
  }
  TreeRooms_ChangeFace(TRET_FACE_CHANGE_CLOSE_EYES);
  wait(10);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 48;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 152, 1);
  FieldCamera_WaitForMove();
  Event_Wait(10);
  Sound_Play(SE_MAP_EXIT);
  MapGrid_CopyCellAttributes(26, 3, 1, 2, 10, 8);
  MapGrid_CopyCells(26, 38, 1, 1, 10, 43);
  wait(4);
  MapGrid_CopyCells(26, 37, 1, 2, 10, 42);
  wait(4);
  MapGrid_CopyCells(26, 36, 1, 3, 10, 41);
  wait(4);
  MapGrid_CopyCells(26, 35, 1, 4, 10, 40);
  wait(80);
  // Tret comes to.
  Event_SetMessageId(MsgTretMmmmm);
  // "Oh... Mmmmm..."
  FieldEvent_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 90, 1);
  FieldCamera_WaitForMove();
  Event_Wait(40);
  TreeRooms_ChangeFace(TRET_FACE_CHANGE_OPEN_EYES);
  Event_Wait(60);
  Sound_Play(MUS_FOREST_THEME);
  TreeRooms_ChangeFace(TRET_FACE_CHANGE_BLINK);
  FieldEvent_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Event_ShowMessage(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0);
  Event_Wait(40);
  FieldEvent_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 20);
  TreeRooms_ChangeFace(TRET_FACE_CHANGE_CLOSE_EYES);
  Event_Wait(40);
  FieldEvent_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  // Garet and Ivan will not let Tret die while they still need his help.
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  FieldEvent_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_GARET), 0, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  FieldEvent_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_IVAN), 0, 20);
  TreeRooms_ChangeFace(TRET_FACE_CHANGE_CLOSE_EYES);
  Event_Wait(40);
  FieldEvent_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 40);
  TreeRooms_ChangeFace(TRET_FACE_CHANGE_BLINK);
  // "Do you need something?" Answering no: "What are you doing, Isaac! Now's
  // not the time to fool around!"
  Event_OpenMessage(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
    Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Event_SetMessageId(MsgTretDoingNowsNot);
    // "What are you doing, Isaac! Now's not the time to fool around!"
    Event_ShowMessage(TREE_ROOMS_SPEAKER(ACTOR_GARET), 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Event_ShowMessage(TREE_ROOMS_SPEAKER(ACTOR_IVAN), 0);
  }
  Event_Wait(20);
  TreeRooms_ChangeFace(TRET_FACE_CHANGE_BLINK);
  Event_SetMessageId(MsgTretTurnedPeopleKolima);
  // "I turned the people of Kolima into trees?"
  FieldEvent_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 20);
  FieldEvent_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 10);
  TreeRooms_ChangeFace(TRET_FACE_CHANGE_CLOSE_EYES);
  Event_Wait(20);
  // Tret tries to release the people of Kolima: the room turns purple and
  // sparks swirl out from his face, then fade.
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  wait(40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
  Event_Wait(20);
  sSparkFrame = 0;
  {
    struct FieldPosition *origin = &sSparkOrigin;
    origin->x = toFIXED(168);
    origin->y = toFIXED(32);
    sparkTask = TreeRooms_SpawnSwirlSparks;
    origin->z = toFIXED(52);
  }
  Task_AddFrame(sparkTask, TASK_PASS_FRAME);
  Event_Wait(220);
  Task_Remove(sparkTask);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  wait(40);
  TreeRooms_ChangeFace(TRET_FACE_CHANGE_BLINK);
  Event_Wait(20);
  // "I can't do it... My power no longer reaches Kolima..." His face sinks
  // away on "People of Kolima... Forgive me...", and the party resolve to
  // heal him.
  FieldEvent_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 10);
  TreeRooms_ChangeFace(TRET_FACE_CHANGE_CLOSE_EYES);
  Event_ShowMessage(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0);
  Actor_RunScriptAndWait(ACTOR_TRET, gTreeRooms_FaceSinksActions);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_ShowMessage(TREE_ROOMS_SPEAKER(ACTOR_GARET), 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 10);
  Event_ShowMessage(TREE_ROOMS_SPEAKER(ACTOR_IVAN), 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_GARET), 0, 10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_IVAN), 0, 10);
  if (hasMia != 0) {
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_ShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_MIA), 0, 10);
  }
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  actions = gTreeRooms_WalkBackToLeaderScript;
  Actor_SetScript(ACTOR_GARET, actions);
  if (hasMia != 0) {
    Actor_SetScript(ACTOR_MIA, actions);
  }
  Actor_RunScriptAndWait(ACTOR_IVAN, actions);
  Event_Wait(20);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  GameFlag_Set(FLAG_TRET_DEFEATED);
  Task_AddFrame(TretPalette_UpdateMapTint, TASK_PASS_FRAME);
  Event_End();
}

/// Sets Tret's room and the entrance room for whether Tret is beaten, after
/// playing the conversation that follows the battle when `playRecoveryEvent`
/// asks for it. Before, the middle of Tret's room (cell (10, 8)) is bare floor
/// and the hole in the entrance room's floor is a bare hole. After, the vine
/// stands in Tret's room and can be climbed, and its top shows in the hole in
/// the entrance room (cell (30, 9)), where stepping on it takes Isaac down to
/// Tret's room.
/// The vine's cells take their climbing from the vine near the entrance room's
/// north wall (cell (26, 3)), and the hole's cells take the exit down to Tret
/// from the spare cell at (0, 0).
static void TreeRooms_ApplyPostBattleState(s32 playRecoveryEvent) {
  if (playRecoveryEvent != 0 && GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0)
    TreeRooms_PlayPostBattleConversation();

  wait(1);
  if (GameFlag_Get(FLAG_TRET_DEFEATED) != 0) {
    MapGrid_CopyCells(121, 34, 3, 1, 93, 10);
    {
      MapGrid_CopyCells(46, 38, 1, 1, 30, 43);
      MapGrid_CopyCellAttributes(0, 0, 1, 2, 30, 9);
    }
    MapGrid_CopyCellAttributes(26, 3, 1, 2, 10, 8);
    MapGrid_CopyCells(26, 35, 1, 4, 10, 40);
  } else {
    MapGrid_CopyCellAttributes(11, 8, 1, 2, 10, 8);
  }
}

/// Entering any of Tret Tree's rooms: once Tret is beaten his face is gone from
/// the lowest room, the light's slow swell starts, leaves torn before a battle
/// are torn again, and four leaves start torn. Arriving by a fall from the
/// fifth floor's centre leaf, Isaac drops in from above and on through the hole
/// in the floor, a room at a time, until he lands before Tret
/// (TreeRooms_LandFromFall). Back from the battle with Tret, the conversation
/// after it plays.
/// The setup also saves the rooms' colours for the swell
/// (TretPalette_SaveMapColors) and sets the map's first layer to scroll at nine
/// tenths of the pace of the view, offset to the room.
s32 Scene_Initialize(void) {
  struct Actor *tret;
  struct Actor *actor;
  struct MapGridScrollWork *grid;
  struct MapGridLayerScroll *layer;
  struct FieldWork *work;
  s32 entrance;
  s32 leaderActorId;
  struct Actor *secondTret;

  tret = Actor_Get(ACTOR_TRET);
  sLastLeaf = (s16 *)gSceneLocalState;
  TretPalette_SaveMapColors();
  tret->motionFlags = 0;
  tret->y.fixed = -toFIXED(10);
  secondTret = Actor_Get(ACTOR_SECOND_TRET);

  secondTret->motionFlags = 0;
  secondTret->y.fixed = -toFIXED(10);
  Actor_SetSpriteColorById(ACTOR_SECOND_TRET, SPRITE_COLOR_HIDDEN);
  TreeRooms_ChangeFace(TRET_FACE_CHANGE_CLOSE_EYES);
  if (gMainRoutineWork.entrance != TREE_ROOMS_AFTER_FIGHT_TRET)
    Task_AddFrame(TretPalette_UpdateMapTint, TASK_PASS_FRAME);
  if (GameFlag_Get(FLAG_TRET_DEFEATED)) {
    Actor_SetPosition(ACTOR_SECOND_TRET, 0, 0);
    Actor_SetPosition(ACTOR_TRET, 0, 0);
  }
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    TreeRooms_ApplyFlaggedMapPatches();
  grid = gMapGridWork[0];
  // Match the first layer's parallax offset and scale to the room bounds.
  layer = &grid->layers[0];
  layer->offsetX += Iwram_MulFX(grid->minX + toFIXED(160), FIXED_ONE / 10);
  layer->offsetY += Iwram_MulFX(grid->minY + toFIXED(136), FIXED_ONE / 10);
  layer->scaleX = 9 * FIXED_ONE / 10;
  layer->scaleY = 9 * FIXED_ONE / 10;
  // The layout draws four leaves already torn, so the first landing on them
  // breaks them.
  GameFlag_Set(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_B_TORN);
  GameFlag_Set(FLAG_TRET_TREE_FIFTH_FLOOR_NORTH_LEAF_TORN);
  GameFlag_Set(FLAG_TRET_TREE_FIFTH_FLOOR_WEST_LEAF_TORN);
  GameFlag_Set(FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHEAST_LEAF_TORN);
  wait(1);
  TreeRooms_ApplyPostBattleState(0);
  work = gFieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  entrance = gMainRoutineWork.entrance;
  leaderActorId = gMainRoutineWork.leaderActorId;
  actor = Actor_Get(leaderActorId);
  // Falling through a room, Isaac falls on to the entrance numbered ten lower;
  // at the fall's end he lands in a ring of dust in Tret's room.
  // TREE_ROOMS_AFTER_FIGHT_TRET: back from the battle with Tret.
  if (entrance == TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_LEAF ||
      entrance == TREE_ROOMS_FALL_THROUGH_A ||
      entrance == TREE_ROOMS_FALL_THROUGH_B ||
      entrance == TREE_ROOMS_FALL_THROUGH_C) {
    Event_StartScreenTransition();
    Actor_SetAnimation(leaderActorId, ANIM_LEADER_WALK_BACKWARDS);
    Actor_SetSpriteFlags(Actor_Get(leaderActorId), 0);
    Actor_SetAttachedEffect(leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    actor->motionFlags = ACTOR_FALLS;
    actor->y.fixed = toFIXED(100);
    actor->groundHeight = -toFIXED(160);
    actor->gravity = toFIXED(1) / 2;
    Sound_Play(SE_DUST_PUFF);
    Event_SetDestination(SCENE_TREE_ROOMS, entrance - FALL_ENTRANCE_STEP);
    Event_Wait(20);
    actor->layer = 2;
    Actor_SetSpritePriority(leaderActorId, 3);
    Event_Wait(2);
    Actor_SetAttachedEffect(leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
    Event_Wait(8);
  } else if (entrance == TREE_ROOMS_FALL_LANDING) {
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
      TreeRooms_LandFromFall();
  } else if (entrance == TREE_ROOMS_AFTER_FIGHT_TRET) {
    TreeRooms_ApplyPostBattleState(1);
  }
  return 0;
}

/// Saves background palettes 0 to 13, the rooms' own colours, which the
/// light's slow swell inside Tret brightens (TretPalette_UpdateMapTint).
static void TretPalette_SaveMapColors(void) {
  Dma_Set((const void *)BG_PLTT, gPaletteFadeWork->palette, DMA_COPY_WORDS(112),
          REG_DMA3);
}

/// The light inside Tret Tree slowly swells and fades: over a few seconds the
/// walls and floors grow a little brighter and warmer, then sink back, again
/// and again. It pauses while Psynergy is being cast, and stops for good once
/// Reveal is cast (TreeRooms_CastReveal).
/// Every 32 frames it adds the next red, green and blue step of
/// gTreeRooms_TintSteps, a ramp from 0 up to 4 and back, to the saved colours
/// of background palettes 1 to 4, skipping a random few steps each time; each
/// later palette takes a weaker step.
static void TretPalette_UpdateMapTint(void) {
  u16 *sourceColors;
  u16 *destinationColors;
  u32 colorIndex;
  s32 redStep;
  s32 greenStep;
  s32 blueStep;
  s32 red;
  s32 green;
  s32 blue;
  u32 color;
  struct FieldWork *work;
  struct PaletteFade *paletteWork;

  work = gFieldWork;
  paletteWork = gPaletteFadeWork;
  sourceColors = paletteWork->palette;
  if (work->psynergy != 0)
    return;
  if ((gFrameCount & 31) != 0)
    return;
  sourceColors += PALETTE_COLORS;
  destinationColors = (u16 *)&BG_PLTT_COLOR(1, 0);
  for (colorIndex = 0; colorIndex < TRET_TINT_COLOR_COUNT;
       colorIndex++, sourceColors++) {
    redStep = gTreeRooms_TintSteps[gTreeRooms_TintIndex];
    greenStep = gTreeRooms_TintSteps[gTreeRooms_TintIndex + 1];
    blueStep = gTreeRooms_TintSteps[gTreeRooms_TintIndex + 2];
    if (colorIndex >= 3 * PALETTE_COLORS) {
      redStep -= redStep / 2 + redStep / 3;
      greenStep -= greenStep / 2 + greenStep / 3;
      blueStep -= blueStep / 2 + blueStep / 3;
    } else if (colorIndex >= 2 * PALETTE_COLORS) {
      redStep -= redStep / 3 + redStep / 4;
      greenStep -= greenStep / 3 + greenStep / 4;
      blueStep -= blueStep / 3 + blueStep / 4;
    } else if (colorIndex >= PALETTE_COLORS) {
      redStep -= redStep / 4 + redStep / 5;
      greenStep -= greenStep / 4 + greenStep / 5;
      blueStep -= blueStep / 4 + blueStep / 5;
    }
    color = *sourceColors;
    red = RGB_RED(color);
    green = RGB_GREEN(color);
    blue = RGB_BLUE(color);
    red += redStep;
    green += greenStep;
    blue += blueStep;
    if (red > TRET_TINT_CHANNEL_MAX)
      red = TRET_TINT_CHANNEL_MAX;
    if (green > TRET_TINT_CHANNEL_MAX)
      green = TRET_TINT_CHANNEL_MAX;
    if (blue > TRET_TINT_CHANNEL_MAX)
      blue = TRET_TINT_CHANNEL_MAX;
    if (red < 0)
      red = 0;
    if (green < 0)
      green = 0;
    if (blue < 0)
      blue = 0;
    *destinationColors++ = RGB(red, green, blue);
  }
  gTreeRooms_TintIndex += (Random_Next() & 7) * 3;
  if (gTreeRooms_TintSteps[gTreeRooms_TintIndex] == TRET_TINT_END)
    gTreeRooms_TintIndex = 0;
}

/// Changes Tret's face in the north wall of his room (opening or closing his
/// eyes, blinking, looking aside), six frames to a picture, and waits twelve
/// frames after it.
static void TreeRooms_ChangeFace(s32 change) {
  switch (change) {
  case TRET_FACE_CHANGE_CLOSE_EYES:
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    wait(6);
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_CLOSED);
    break;
  case TRET_FACE_CHANGE_OPEN_EYES:
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    break;
  case TRET_FACE_CHANGE_LOOK_LEFT:
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    wait(6);
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_LOOK_LEFT);
    break;
  case TRET_FACE_CHANGE_LOOK_RIGHT:
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    wait(6);
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_LOOK_RIGHT);
    break;
  case TRET_FACE_CHANGE_BLINK:
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    wait(6);
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_CLOSED);
    wait(6);
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    wait(6);
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_CLOSED);
    wait(6);
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    break;
  case TRET_FACE_CHANGE_HALF_CLOSE_EYES:
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    wait(6);
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_HALF_CLOSED);
    break;
  case TRET_FACE_CHANGE_WRATHFUL_CLOSE_EYES:
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    wait(6);
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_CLOSED);
    break;
  case TRET_FACE_CHANGE_WRATHFUL_OPEN_EYES:
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    break;
  case TRET_FACE_CHANGE_WRATHFUL_LOOK_RIGHT:
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    wait(6);
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_LOOK_RIGHT);
    break;
  case TRET_FACE_CHANGE_WRATHFUL_LOOK_LEFT:
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    wait(6);
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_LOOK_LEFT);
    break;
  case TRET_FACE_CHANGE_WRATHFUL_BLINK:
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    wait(6);
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_CLOSED);
    wait(6);
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    wait(6);
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_CLOSED);
    wait(6);
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    break;
  case TRET_FACE_CHANGE_WRATHFUL_STARE:
    Actor_SetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    break;
  }
  wait(12);
}

/// One of the sparks that swirl out from Tret's face as he tries to free the
/// people of Kolima: it spirals outward from his face, shrinking as it goes,
/// and is put away after 80 frames.
static void TreeRooms_UpdateSwirlSpark(struct Actor *spark) {
  struct FieldPosition position;
  s32 frame;

  frame = spark->timer;
  if (frame <= SWIRL_SPARK_LAST_FRAME) {
    position.x = spark->targetX;
    position.y = spark->targetY;
    position.z = spark->targetZ;
    {
      s32 initialAngle = (s16)spark->userValue;

      Vector_AddPolarOffset(toFIXED(frame), ((frame * 3) << 8) + initialAngle,
                            &position);
    }
    spark->x.fixed = position.x;
    spark->y.fixed = position.y;
    spark->z.fixed = position.z;
    if (spark->timer <= SWIRL_SPARK_SHRINK_LAST_FRAME) {
      spark->scaleX -= FIXED_ONE / 50;
      spark->scaleY -= FIXED_ONE / 50;
    }
    spark->timer++;
  } else {
    VramCache_FreeEntry(spark->sprite->slot);
    Actor_Release(spark);
  }
}

/// Sends out the sparks from Tret's face as he tries to free the people of
/// Kolima: five waves, ten frames apart, of six, five, four, three and two
/// sparks, each wave with a shimmering sound.
static void TreeRooms_SpawnSwirlSparks(void) {
  struct Actor *spark;
  u32 frame;
  void *previousResourceEntry;
  s32 wave;
  u32 sparkIndex;

  frame = sSparkFrame;
  previousResourceEntry = 0;
  wave = (s32)frame / 10;
  switch (frame) {
  case 0:
  case 10:
  case 20:
  case 30:
  case 40:
    Sound_Play(SE_SHIMMER);
    // Waves of 6, 5, 4, 3 and 2 sparks, spread evenly round a turn.
    for (sparkIndex = 0; sparkIndex < 6 - wave; sparkIndex++) {
      spark = FieldActor_Create(SPRITE_SPARKLE, sSparkOrigin.x, sSparkOrigin.y,
                                sSparkOrigin.z);
      if (spark != NULL) {
        previousResourceEntry =
            Sprite_ShareVramCacheEntry(spark->sprite, previousResourceEntry);
        spark->motionFlags = 0;
        spark->sprite->image.priority = 0;
        Actor_SetSpriteFlags(spark, 0);
        Actor_SelectAnimation(spark, 1);
        spark->timer = 0;
        spark->userValue = DEGtoANG(360 / (u32)(6 - wave) * sparkIndex);
        spark->targetX = sSparkOrigin.x;
        spark->targetY = sSparkOrigin.y;
        spark->targetZ = sSparkOrigin.z;
        spark->speedLimit = toFIXED(8) / 5;
        spark->update = TreeRooms_UpdateSwirlSpark;
      }
    }
    // Each wave also plays the closing sound.
    // fall through
  case 44:
    Sound_Play(SOUND_FADE_OUT_ACTION_SE);
    break;
  }
  if (++sSparkFrame > 120) {
    sSparkFrame = 0;
  }
}

/// Tears again each leaf that was torn before a battle, so that the webs look
/// as the party left them.
static void TreeRooms_ApplyFlaggedMapPatches(void) {
  s32 patchOffset;

  for (patchOffset = 0; gTreeRooms_MapPatches[patchOffset] != -1;
       patchOffset += 6) {
    if (GameFlag_Get(gTreeRooms_MapPatches[patchOffset]) &&
        gTreeRooms_MapPatches[patchOffset + 1] != 0)
      MapGrid_CopyCellsTo(gTreeRooms_MapPatches[patchOffset + 2],
                          gTreeRooms_MapPatches[patchOffset + 3],
                          gTreeRooms_MapPatches[patchOffset + 4],
                          gTreeRooms_MapPatches[patchOffset + 5], 1, 1);
  }
}

/// Casting Reveal anywhere inside Tret Tree stops the light's slow swell and
/// fade, leaving the rooms' colours as they are, until Isaac next goes through
/// a door, up or down a vine or through a leaf into another room.
static void TreeRooms_CastReveal(void) {
  Task_Remove(TretPalette_UpdateMapTint);
}

#include "shared/field_effect_advance.c"

/// Isaac lands in Tret's room at the end of the fall from the fifth floor's
/// centre leaf: he drops into the middle of the room, south of Tret's face, and
/// hits the floor with a thump in a ring of white dust as the screen shakes,
/// staggering back in surprise.
static void TreeRooms_LandFromFall(void) {
  struct EffectOptions options;
  struct FieldPosition velocity;
  struct Actor *leader;
  u32 particleIndex;
  s32 velocityX;
  FIXED velocityZ;

  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Actor_Get(ACTOR_PARTY_LEADER)->y.fixed = toFIXED(130);
  Actor_Get(ACTOR_PARTY_LEADER)->gravity = toFIXED(1) / 2;
  Actor_Get(ACTOR_PARTY_LEADER)->bounce = 0;
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_DUST_PUFF);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_Wait(24);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  options.palette = SPRITE_COLOR_WHITE;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 4 * FIXED_ONE / 5;
  options.startScaleY = 4 * FIXED_ONE / 5;
  for (particleIndex = 0; particleIndex <= TRIG_TURN / FACING_STEP;
       particleIndex++) {
    velocity.x = Trig_Cos(particleIndex * FACING_STEP);
    velocity.y = 0;
    velocityZ = Trig_Sin(particleIndex * FACING_STEP);
    velocityX = velocity.x;
    velocity.z = velocityZ;
    velocityX += velocityX / 2;
    velocity.x = velocityX;
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      velocityX, velocity.y, velocityZ,
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
                      &options);
  }
  Sound_Play(SE_THUMP);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  Actor_Get(ACTOR_PARTY_LEADER)->gravity = toFIXED(1);
  Actor_Get(ACTOR_PARTY_LEADER)->bounce = FIXED_ONE / 4;
  Event_End();
}

#include "shared/field_effect_duration_scripts.c"

/// The leaves torn again after a battle. Each row is the leaf's flag, whether
/// it is redrawn (0 for the four the map draws torn), the torn picture's cell
/// x/y on the spare row and the leaf's cell x/y in the map's upper layer.
// clang-format off
s16 gTreeRooms_MapPatches[] = {
    FLAG_TRET_TREE_SECOND_FLOOR_LEAF_A_TORN, 1, 64, 35, 113, 6,
    FLAG_TRET_TREE_SECOND_FLOOR_LEAF_B_TORN, 0, 65, 35, 115, 6,
    FLAG_TRET_TREE_SECOND_FLOOR_LEAF_C_TORN, 1, 66, 35, 110, 8,
    FLAG_TRET_TREE_SECOND_FLOOR_LEAF_D_TORN, 1, 67, 35, 110, 10,
    FLAG_TRET_TREE_SECOND_FLOOR_LEAF_E_TORN, 1, 68, 35, 111, 12,
    FLAG_TRET_TREE_SECOND_FLOOR_LEAF_F_TORN, 1, 69, 35, 113, 12,
    FLAG_TRET_TREE_SECOND_FLOOR_LEAF_G_TORN, 1, 70, 35, 112, 14,
    FLAG_TRET_TREE_SECOND_FLOOR_LEAF_H_TORN, 1, 71, 35, 114, 14,
    FLAG_TRET_TREE_SECOND_FLOOR_LEAF_I_TORN, 1, 72, 35, 116, 14,
    FLAG_TRET_TREE_THIRD_FLOOR_WEST_LEAF_TORN, 1, 73, 35, 71, 23,
    FLAG_TRET_TREE_THIRD_FLOOR_EAST_LEAF_TORN, 1, 74, 35, 77, 24,
    FLAG_TRET_TREE_FIFTH_FLOOR_NORTHWEST_LEAF_TORN, 1, 75, 35, 112, 24,
    FLAG_TRET_TREE_FIFTH_FLOOR_NORTH_LEAF_TORN, 0, 76, 35, 114, 24,
    FLAG_TRET_TREE_FIFTH_FLOOR_NORTHEAST_LEAF_TORN, 1, 77, 35, 116, 24,
    FLAG_TRET_TREE_FIFTH_FLOOR_WEST_LEAF_TORN, 0, 78, 35, 112, 26,
    FLAG_TRET_TREE_FIFTH_FLOOR_CENTER_LEAF_TORN, 1, 79, 35, 114, 26,
    FLAG_TRET_TREE_FIFTH_FLOOR_EAST_LEAF_TORN, 1, 80, 35, 116, 26,
    FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHWEST_LEAF_TORN, 1, 81, 35, 112, 28,
    FLAG_TRET_TREE_FIFTH_FLOOR_SOUTH_LEAF_TORN, 1, 82, 35, 114, 28,
    FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHEAST_LEAF_TORN, 0, 83, 35, 116, 28,
    FLAG_TRET_TREE_FIFTH_FLOOR_FAR_SOUTHEAST_LEAF_TORN, 1, 84, 35, 118, 28,
    -1, 0,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The party gathers in front of Tret before the battle (TreeRooms_FightTret),
/// each walking to its own place.
s32 gTreeRooms_LeaderGathersActions[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(166, 0, 80),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gTreeRooms_GaretGathersActions[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(148, 0, 90),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gTreeRooms_IvanGathersActions[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(182, 0, 90),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gTreeRooms_MiaGathersActions[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(166, 0, 104),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/// Tret's face sinking into the wall at the end of the conversation after the
/// battle. Eight times it is raised 80 pixels for 4 frames and lowered 78, a
/// sixteenth smaller each time, and then it is put away.
s32 gTreeRooms_FaceSinksActions[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, toFIXED(80)),
    SC_WAIT(4),
    SC_ADD(SW_Y, -toFIXED(78)),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(4),
    SC_LOOP(8, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// Garet, Ivan and Mia walk back into Isaac at the end of the conversation
/// after the battle, and are put away.
s32 gTreeRooms_WalkBackToLeaderScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(166, 0, 80),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

u32 gTreeRooms_TintSteps[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 1, 0, 0, 1, 0,
    0, 1, 0, 0, 1, 0, 0, 1,
    0, 0, 1, 0, 0, 1, 1, 0,
    1, 1, 0, 1, 1, 0, 1, 1,
    0, 1, 1, 0, 1, 1, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 1,
    1, 2, 1, 1, 2, 1, 1, 2,
    1, 1, 2, 2, 1, 2, 2, 1,
    2, 2, 2, 3, 2, 2, 3, 2,
    2, 3, 3, 2, 3, 3, 2, 3,
    3, 2, 3, 3, 2, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 4, 3, 3, 4,
    3, 3, 4, 3, 3, 4, 3, 3,
    4, 3, 3, 4, 3, 3, 4, 4,
    3, 4, 4, 3, 4, 4, 3, 4,
    4, 3, 4, 4, 3, 4, 4, 3,
    4, 4, 3, 4, 4, 3, 4, 4,
    3, 4, 4, 3, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 3, 4, 4, 3, 4, 4,
    3, 4, 4, 3, 4, 4, 3, 4,
    4, 3, 4, 4, 3, 4, 4, 3,
    4, 4, 3, 4, 4, 3, 4, 3,
    3, 4, 3, 3, 4, 3, 3, 4,
    3, 3, 4, 3, 3, 4, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 2, 3, 3, 2, 3, 3, 2,
    3, 3, 2, 3, 2, 2, 3, 2,
    2, 2, 2, 2, 2, 2, 1, 2,
    2, 1, 2, 1, 1, 2, 1, 1,
    2, 1, 1, 2, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0,
    1, 1, 0, 1, 1, 0, 1, 1,
    0, 1, 1, 0, 1, 1, 0, 1,
    0, 0, 1, 0, 0, 1, 0, 0,
    1, 0, 0, 1, 0, 0, 1, 0,
    0, 1, 0, 0, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99,
};

struct SceneEntrance gTreeRoomsEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 136, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { TREE_ROOMS_FROM_KOLIMA_FOREST_TREE, CONDITION_ALWAYS, 488, 0, 248, FACING_NORTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTH_SOUTHWEST, CONDITION_ALWAYS, 712, 0, 152, FACING_EAST, 0, 672, 0, 944, 272, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTH_SOUTHEAST, CONDITION_ALWAYS, 920, 0, 152, FACING_WEST, 0, 672, 0, 944, 272, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTH, CONDITION_ALWAYS, 168, 0, 520, FACING_NORTH, 0, 32, 272, 304, 544, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTHEAST, CONDITION_ALWAYS, 296, 0, 424, FACING_WEST, 0, 32, 272, 304, 544, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTHWEST, CONDITION_ALWAYS, 376, 0, 424, FACING_EAST, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTH, CONDITION_ALWAYS, 488, 0, 520, FACING_NORTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTH_NORTHWEST, CONDITION_ALWAYS, 712, 0, 424, FACING_EAST, 0, 672, 272, 960, 544, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTH_NORTHEAST, CONDITION_ALWAYS, 920, 0, 424, FACING_WEST, 0, 672, 272, 960, 544, 0 },
    { TREE_ROOMS_FALL_LANDING, CONDITION_ALWAYS, 168, 0, 152, FACING_SOUTH, 0, 32, 0, 304, 272, 0 },
    { TREE_ROOMS_ENTRANCE_FROM_SECOND_FLOOR, CONDITION_ALWAYS, 424, 24, 72, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_SECOND_FLOOR_FROM_ENTRANCE, CONDITION_ALWAYS, 744, 0, 88, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { TREE_ROOMS_THIRD_FLOOR_FROM_FOURTH_FLOOR, CONDITION_ALWAYS, 232, 24, 344, FACING_SOUTH, 0, 32, 272, 304, 544, 0 },
    { TREE_ROOMS_FOURTH_FLOOR_FROM_THIRD_FLOOR, CONDITION_ALWAYS, 552, 0, 360, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_TRET_FROM_ENTRANCE, CONDITION_ALWAYS, 168, 24, 152, FACING_SOUTH, 0, 32, 0, 304, 272, 0 },
    { TREE_ROOMS_ENTRANCE_FROM_TRET, CONDITION_ALWAYS, 488, 0, 168, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_A, CONDITION_ALWAYS, 472, 0, 104, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_B, CONDITION_ALWAYS, 504, 0, 104, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_C, CONDITION_ALWAYS, 424, 0, 136, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_D, CONDITION_ALWAYS, 424, 0, 168, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_E, CONDITION_ALWAYS, 440, 0, 200, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_F, CONDITION_ALWAYS, 488, 0, 200, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_G, CONDITION_ALWAYS, 456, 0, 232, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_H, CONDITION_ALWAYS, 488, 0, 232, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_I, CONDITION_ALWAYS, 520, 0, 232, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_THIRD_FLOOR_WEST_LEAF, CONDITION_ALWAYS, 760, 0, 104, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { TREE_ROOMS_FROM_THIRD_FLOOR_EAST_LEAF, CONDITION_ALWAYS, 856, 0, 120, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHWEST_LEAF, CONDITION_ALWAYS, 456, 0, 392, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_NORTH_LEAF, CONDITION_ALWAYS, 488, 0, 392, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHEAST_LEAF, CONDITION_ALWAYS, 520, 0, 392, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_WEST_LEAF, CONDITION_ALWAYS, 456, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_EAST_LEAF, CONDITION_ALWAYS, 520, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHWEST_LEAF, CONDITION_ALWAYS, 456, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTH_LEAF, CONDITION_ALWAYS, 488, 0, 456, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHEAST_LEAF, CONDITION_ALWAYS, 520, 0, 456, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_FAR_SOUTHEAST_LEAF, CONDITION_ALWAYS, 552, 0, 456, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_LEAF, CONDITION_ALWAYS, 488, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FALL_THROUGH_A, CONDITION_ALWAYS, 168, 0, 424, FACING_SOUTH, 0, 32, 272, 304, 544, 0 },
    { TREE_ROOMS_FALL_THROUGH_B, CONDITION_ALWAYS, 808, 0, 152, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { TREE_ROOMS_FALL_THROUGH_C, CONDITION_ALWAYS, 488, 0, 152, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_AFTER_FIGHT_TRET, CONDITION_ALWAYS, 168, 0, 72, FACING_SOUTH, 0, 32, 0, 304, 272, 0 },
    { SCENE_TABLE_END },
};

struct SceneExitRegion gTreeRoomsRegions[] = {
    { 420, 28, 68, 428, 36, 76, -1, TREE_ROOMS_ENTRANCE_TO_SECOND_FLOOR },
    { 228, 28, 338, 236, 36, 346, -1, TREE_ROOMS_THIRD_FLOOR_TO_FOURTH_FLOOR },
#if !TBS_PATCH_3
    { 163, 28, 148, 171, 36, 156, -1, TREE_ROOMS_TRET_TO_ENTRANCE },
#else
    { 159, 24, 144, 175, 40, 160, -1, TREE_ROOMS_TRET_TO_ENTRANCE },
#endif
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};

u32 gTreeRoomsExits[] = {
    SCENE_EXITS(TreeRooms),
    SCENE_EXIT(TREE_ROOMS_TO_KOLIMA_FOREST_TREE, KolimaForestTree, KOLIMA_FOREST_TREE_FROM_TREE_ROOMS),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHWEST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTH_SOUTHWEST),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHEAST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTH_SOUTHEAST),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTH),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTHEAST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTHEAST),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTHWEST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTHWEST),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTH),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH_NORTHWEST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTH_NORTHWEST),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH_NORTHEAST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTH_NORTHEAST),
    SCENE_EXIT(TREE_ROOMS_ENTRANCE_TO_SECOND_FLOOR, TreeRooms, TREE_ROOMS_SECOND_FLOOR_FROM_ENTRANCE),
    SCENE_EXIT(TREE_ROOMS_SECOND_FLOOR_TO_ENTRANCE, TreeRooms, TREE_ROOMS_ENTRANCE_FROM_SECOND_FLOOR),
    SCENE_EXIT(TREE_ROOMS_THIRD_FLOOR_TO_FOURTH_FLOOR, TreeRooms, TREE_ROOMS_FOURTH_FLOOR_FROM_THIRD_FLOOR),
    SCENE_EXIT(TREE_ROOMS_FOURTH_FLOOR_TO_THIRD_FLOOR, TreeRooms, TREE_ROOMS_THIRD_FLOOR_FROM_FOURTH_FLOOR),
    SCENE_EXIT(TREE_ROOMS_TRET_TO_ENTRANCE, TreeRooms, TREE_ROOMS_ENTRANCE_FROM_TRET),
    SCENE_EXIT(TREE_ROOMS_ENTRANCE_TO_TRET, TreeRooms, TREE_ROOMS_TRET_FROM_ENTRANCE),
    SCENE_EXITS_END,
};

struct ActorPlacement gTreeRoomsActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TRET, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        168, 0, 48, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TRET_RAISED, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        168, 0, 48, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};
// clang-format on

struct Trigger gTreeRoomsTriggers[] = {
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH,
     TREE_ROOMS_TO_KOLIMA_FOREST_TREE, CONDITION_ALWAYS,
     TREE_ROOMS_TO_KOLIMA_FOREST_TREE},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_WEST,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHWEST, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHWEST},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_EAST,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHEAST, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHEAST},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_EAST,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTHEAST, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTHEAST},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_WEST,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTHWEST, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTHWEST},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_WEST,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH_NORTHWEST, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH_NORTHWEST},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_EAST,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH_NORTHEAST, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH_NORTHEAST},
    {TRIGGER_EXIT, TREE_ROOMS_SECOND_FLOOR_TO_ENTRANCE, CONDITION_ALWAYS,
     TREE_ROOMS_SECOND_FLOOR_TO_ENTRANCE},
    {TRIGGER_EXIT, TREE_ROOMS_FOURTH_FLOOR_TO_THIRD_FLOOR, CONDITION_ALWAYS,
     TREE_ROOMS_FOURTH_FLOOR_TO_THIRD_FLOOR},
    {TRIGGER_EXIT, TREE_ROOMS_ENTRANCE_TO_TRET, CONDITION_ALWAYS,
     TREE_ROOMS_ENTRANCE_TO_TRET},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_A, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_B, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_C, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_D, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_E, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_F, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_G, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_H, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_I, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_THIRD_FLOOR_WEST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_THIRD_FLOOR_EAST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHWEST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_FIFTH_FLOOR_NORTH_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHEAST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_FIFTH_FLOOR_WEST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_FIFTH_FLOOR_EAST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHWEST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTH_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHEAST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     TREE_ROOMS_FROM_FIFTH_FLOOR_FAR_SOUTHEAST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpOffLeaf)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_A, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorLeafA)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_B, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorLeafB)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_C, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorLeafC)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_D, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorLeafD)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_E, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorLeafE)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_F, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorLeafF)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_G, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorLeafG)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_H, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorLeafH)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_I, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorLeafI)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_THIRD_FLOOR_WEST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnThirdFloorWestLeaf)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_THIRD_FLOOR_EAST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnThirdFloorEastLeaf)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorCenterLeaf)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHWEST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorNorthwestLeaf)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_NORTH_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorNorthLeaf)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHEAST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorNortheastLeaf)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_WEST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorWestLeaf)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_EAST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorEastLeaf)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHWEST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorSouthwestLeaf)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTH_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorSouthLeaf)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHEAST_LEAF, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorSoutheastLeaf)},
    {TRIGGER_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_FAR_SOUTHEAST_LEAF,
     CONDITION_ALWAYS, EVENT(TreeRooms_StepOnFifthFloorFarSoutheastLeaf)},
    {TRIGGER_STEP, CELL_BEHAVIOR_TREE_ROOMS_BEFORE_TRET, FLAG_TRET_DEFEATED,
     EVENT(TreeRooms_FightTret)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(TreeRooms_CastReveal)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST),
     CELL_BEHAVIOR_TREE_ROOMS_HEALING_RING_CHEST, FLAG_TRET_TREE_HEALING_RING,
     FIND_ITEM(ITEM_HEALING_RING)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_TREE_ROOMS_NUT_CHEST,
     FLAG_TRET_TREE_NUT, FIND_ITEM(ITEM_NUT)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TREE_ROOMS_PSYNERGY_STONE,
     FLAG_AREA_TEMP_50, FIND_PSYNERGY_STONE},
    {TRIGGER_TABLE_END},
};

s32 gTreeRooms_TintIndex = 0;
