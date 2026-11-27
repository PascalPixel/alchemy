/* Tret Tree's rooms (SceneId_TreeRooms): the hollow inside of Tret, the holy
   tree of Kolima Forest, where the party come to face his wrathful side. Six
   rooms are stacked inside the trunk. Doors in the four upper floors lead out
   onto his branches (SceneId_TretTreeBranch, tret_tree_branch.c), where vines
   climb from one floor's door to the next.

   The party come in from Kolima Forest (kolima_forest_tree.c) by the door at
   the south end of the entrance room (metatile (30, 16)), a maze of root walls
   with a round hole in the middle of its floor (metatile (30, 9)). A vine near
   its north wall (metatile (26, 4)) climbs to the second floor.

   The floors above are spider webs stretched over holes, with yellow leaves
   lying on them. Isaac crosses a web by jumping two metatiles at a time from
   leaf to leaf (scTreeRoomsJumpOffLeaf). The first time he lands on a leaf it
   tears. The next time it breaks, and he falls through into the room below,
   straight under the leaf (scTreeRoomsStepOnLeaf). Four leaves are torn from
   the start.

   - The second floor's web holds nine leaves, and its west and east doors
	 lead out onto the branches. Its leaves drop Isaac into the entrance room.
   - The third floor, entered from the branches by its south or east door, has
	 a leaf on a web either side of the Healing Ring's chest, a Psynergy Stone
	 by its north wall and a vine up to the fourth floor. Its leaves drop him
	 to the second floor. A third web, in the south-west, has only a small
	 spider on it.
   - The fourth floor is bare, with a hole in the middle of its floor, the top
	 of the vine from the third floor, a west door and a south door.
   - The fifth floor, the highest, is one great web of ten leaves beside the
	 Nut's chest, entered by its west or east door. Its leaves drop him to the
	 fourth floor.

   The leaf in the middle of the fifth floor's web is the way to Tret. It drops
   Isaac through the holes in the fourth, third and second floors and the
   entrance room, in turn, into Tret's room under the entrance room, where his
   face looks out of the north wall (scTreeRoomsLandFromFall).

   Walking up to the face starts the battle with Tret (scTreeRoomsFightTret).
   Afterwards he comes to himself, finds he turned the people of Kolima into
   trees, fails to free them and sinks away, and a vine grows in the middle of
   his room (scTreeRoomsPlayPostBattleConversation). From then on that vine
   joins his room and the hole in the entrance room's floor. FLAG_TRET_DEFEATED
   records the battle won and the conversation over. In Kolima Forest it lets
   Hermes' Water heal Tret and lift the curse.

   The light inside the tree slowly swells and fades
   (scTretPaletteUpdateMapTint), and casting Reveal stops it.

   Each leaf has a temporary flag of its own, set when it tears
   (FLAG_TRET_TREE_SECOND_FLOOR_LEAF_A_TORN and the others). The metatiles of a
   leaf carry the number of the entrance its fall lands at, so the step cues
   name them by those entrances. A torn leaf is redrawn after a battle
   (FLAG_KEEP_PARTY_POSITION) from TreeRooms_MapPatches. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/map_grid_scroll.h"
#include "field/speaker.h"
#include "graphics/graphics.h"
#include "scene/walk_into_leader.h"
#include "text/msg_ids.h"

extern Sint16 TreeRooms_MapPatches[];

/* The columns of a row of TreeRooms_MapPatches, six halfwords, whose rows run
   on until a flag of -1. */
enum {
	MAP_PATCH_FLAG,
	MAP_PATCH_REDRAWN,
	MAP_PATCH_SOURCE_X,
	MAP_PATCH_SOURCE_Y,
	MAP_PATCH_DESTINATION_X,
	MAP_PATCH_DESTINATION_Y,
	MAP_PATCH_ROW_LENGTH
};

extern Sint32			 TreeRooms_LeaderGathersActions[];
extern Sint32			 TreeRooms_GaretGathersActions[];
extern Sint32			 TreeRooms_IvanGathersActions[];
extern Sint32			 TreeRooms_MiaGathersActions[];
extern Sint32			 TreeRooms_FaceSinksActions[];
extern Sint32			 TreeRooms_WalkBackToLeaderScript[];
extern Uint32			 TreeRooms_TintSteps[];
extern SCENE_ENTRANCE	 TreeRoomsEntrances[];
extern SCENE_EXIT_REGION TreeRoomsRegions[];
extern Uint32			 TreeRoomsExits[];
extern ACTOR_PLACE		 TreeRoomsActorPlacements[];
extern CUE				 TreeRoomsCues[];
extern Sint32			 TreeRooms_TintIndex;

/* Tret's face in the north wall of his room (SPRITE_TRET), the first actor
   placed after the party, and a second Tret actor (SPRITE_TRET_RAISED) placed
   at the same spot.

   Both speak Tret's lines. The face scorns the party before the battle; the
   second roars before it and speaks every line after it. */
enum {
	ACTOR_TRET = ACTOR_FIRST_PLACED,
	/* Kept hidden (SPRITE_COLOR_HIDDEN): only a speaker. */
	ACTOR_SECOND_TRET
};

/* The speaker of a message. Every speaker in this scene has
   SPEAKER_WINDOW_BELOW. */
#define TREE_ROOMS_SPEAKER(actorId) (SPEAKER_WINDOW_BELOW | (actorId))

/* The metatiles Tret Tree's examine and step cues answer besides the doors, the
   vines and the leaves. The doors' metatiles and the tops of the vines that
   lead down carry the numbers of their exits, and each leaf's metatile the
   number of the entrance its fall lands at. */
typedef enum TREE_ROOMS_METATILE_BEHAVIOR {
	/* The row in front of Tret's face in the lowest room, (9, 5) to (11, 5). */
	METATILE_BEHAVIOR_TREE_ROOMS_BEFORE_TRET = 40,
	/* The Healing Ring's chest on the third floor, between its webs (10,
	   24). */
	METATILE_BEHAVIOR_TREE_ROOMS_HEALING_RING_CHEST = 100,
	/* The Nut's chest on the fifth floor, east of its web (54, 25). */
	METATILE_BEHAVIOR_TREE_ROOMS_NUT_CHEST = 101,
	/* The Psynergy Stone by the third floor's north wall (9, 21). */
	METATILE_BEHAVIOR_TREE_ROOMS_PSYNERGY_STONE = 102
} TREE_ROOMS_METATILE_BEHAVIOR;

/* How long Isaac spins as he drops through a broken leaf. */
#define LEAF_FALL_SPIN_FRAMES		  30
#define SWIRL_SPARK_LAST_FRAME		  79
#define SWIRL_SPARK_SHRINK_LAST_FRAME 39
/* The light's slow swell inside Tret (scTretPaletteUpdateMapTint) covers
   background palettes 1 to 4 but palette 4's last colour. */
#define TRET_TINT_COLOR_COUNT		  (4 * PALETTE_COLORS - 1)
#define TRET_TINT_CHANNEL_MAX		  31
#define TRET_TINT_END				  99

/* The fall from the fifth floor's centre leaf goes down a floor at a time
   through entrances ten apart: TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_LEAF on the
   fourth floor, then TREE_ROOMS_FALL_THROUGH_A to _C, then
   TREE_ROOMS_FALL_LANDING in Tret's room. */
#define FALL_ENTRANCE_STEP \
	(TREE_ROOMS_FALL_THROUGH_B - TREE_ROOMS_FALL_THROUGH_C)

static void scTretPaletteSaveMapCols(void);
static void scTretPaletteUpdateMapTint(void);

/* The face changes scTreeRoomsChangeFace plays on Tret. Each starts from the
   face with its eyes open, and the wrathful ones are on his wrathful face. */
typedef enum TREE_ROOMS_FACE_CHANGE {
	TRET_FACE_CHANGE_CLOSE_EYES = 0,
	TRET_FACE_CHANGE_OPEN_EYES = 1,
	TRET_FACE_CHANGE_LOOK_LEFT = 2,
	TRET_FACE_CHANGE_LOOK_RIGHT = 3,
	TRET_FACE_CHANGE_BLINK = 4, /* twice */
	TRET_FACE_CHANGE_HALF_CLOSE_EYES = 5,
	TRET_FACE_CHANGE_WRATHFUL_CLOSE_EYES = 7,
	TRET_FACE_CHANGE_WRATHFUL_OPEN_EYES = 8,
	TRET_FACE_CHANGE_WRATHFUL_LOOK_RIGHT = 9,
	TRET_FACE_CHANGE_WRATHFUL_LOOK_LEFT = 10,
	TRET_FACE_CHANGE_WRATHFUL_BLINK = 11, /* twice */
	/* as TRET_FACE_CHANGE_WRATHFUL_OPEN_EYES */
	TRET_FACE_CHANGE_WRATHFUL_STARE = 12
} TREE_ROOMS_FACE_CHANGE;

static void scTreeRoomsChangeFace(Sint32 change);
static void scTreeRoomsSpawnSwirlSparks(void);
static void scTreeRoomsApplyFlaggedMapPatches(void);
static void scTreeRoomsLandFromFall(void);

/* Where the sparks spring from, the frame of their 121-frame cycle, and the
   metatile keeping the entrance of the leaf last landed on, -1 once he jumps
   off. */
static FIELD_POS sSparkOrigin ALIGNED(8);
static Sint32				  sSparkFrame;
static Sint16				 *sLastLeaf;

#include "shared/field_effect_create.c"

#include "shared/field_effect_spawn.c"

/* Each leaf's flag is set once the leaf is torn, so that landing on it again
   breaks it. Four start torn: scInit sets their flags. */

/* Second floor, the west of the pair in the north of the web, metatile (49,
   6). */
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_A_TORN			   FLAG_SCENE_TEMP_00
/* Second floor, the east of the pair in the north, metatile (51, 6); starts
   torn. */
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_B_TORN			   FLAG_SCENE_TEMP_01
/* Second floor, the upper of the two down the west side, metatile (46, 8). */
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_C_TORN			   FLAG_SCENE_TEMP_02
/* Second floor, the lower of the two down the west side, metatile (46, 10). */
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_D_TORN			   FLAG_SCENE_TEMP_03
/* Second floor, the west of the pair in the middle, metatile (47, 12). */
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_E_TORN			   FLAG_SCENE_TEMP_04
/* Second floor, the east of the pair in the middle, metatile (49, 12). */
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_F_TORN			   FLAG_SCENE_TEMP_05
/* Second floor, the west of the row along the south, metatile (48, 14). */
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_G_TORN			   FLAG_SCENE_TEMP_06
/* Second floor, the middle of the row along the south, metatile (50, 14). */
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_H_TORN			   FLAG_SCENE_TEMP_07
/* Second floor, the east of the row along the south, metatile (52, 14). */
#define FLAG_TRET_TREE_SECOND_FLOOR_LEAF_I_TORN			   FLAG_SCENE_TEMP_08
/* Third floor, the leaf on the west web, metatile (7, 23). */
#define FLAG_TRET_TREE_THIRD_FLOOR_WEST_LEAF_TORN		   FLAG_SCENE_TEMP_09
/* Third floor, the leaf on the east web, metatile (13, 24). */
#define FLAG_TRET_TREE_THIRD_FLOOR_EAST_LEAF_TORN		   FLAG_SCENE_TEMP_0A
/* Fifth floor, the centre leaf, the way down to Tret, metatile (50, 26). */
#define FLAG_TRET_TREE_FIFTH_FLOOR_CENTER_LEAF_TORN		   FLAG_SCENE_TEMP_0B
/* Fifth floor, metatile (48, 24). */
#define FLAG_TRET_TREE_FIFTH_FLOOR_NORTHWEST_LEAF_TORN	   FLAG_SCENE_TEMP_0C
/* Fifth floor, metatile (50, 24); starts torn. */
#define FLAG_TRET_TREE_FIFTH_FLOOR_NORTH_LEAF_TORN		   FLAG_SCENE_TEMP_0D
/* Fifth floor, metatile (52, 24). */
#define FLAG_TRET_TREE_FIFTH_FLOOR_NORTHEAST_LEAF_TORN	   FLAG_SCENE_TEMP_0E
/* Fifth floor, metatile (48, 26); starts torn. */
#define FLAG_TRET_TREE_FIFTH_FLOOR_WEST_LEAF_TORN		   FLAG_SCENE_TEMP_0F
/* Fifth floor, metatile (52, 26). */
#define FLAG_TRET_TREE_FIFTH_FLOOR_EAST_LEAF_TORN		   FLAG_SCENE_TEMP_10
/* Fifth floor, metatile (48, 28). */
#define FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHWEST_LEAF_TORN	   FLAG_SCENE_TEMP_11
/* Fifth floor, metatile (50, 28). */
#define FLAG_TRET_TREE_FIFTH_FLOOR_SOUTH_LEAF_TORN		   FLAG_SCENE_TEMP_12
/* Fifth floor, metatile (52, 28); starts torn. */
#define FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHEAST_LEAF_TORN	   FLAG_SCENE_TEMP_13
/* Fifth floor, the east end of the south row, metatile (54, 28). */
#define FLAG_TRET_TREE_FIFTH_FLOOR_FAR_SOUTHEAST_LEAF_TORN FLAG_SCENE_TEMP_14

/* Returns the scene's entrances, where the leaves' falls land among them. */
const SCENE_ENTRANCE *scGetEntrances(void)
{
	return TreeRoomsEntrances;
}

/* Returns the tops of the three vines that climb to the room above: from the
   entrance room to the second floor, from the third floor to the fourth, and,
   once Tret is beaten, from Tret's room to the entrance room. Climbing to
   between 28 and 36 pixels up one of them takes Isaac through. */
const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return TreeRoomsRegions;
}

const Uint32 *scGetExits(void)
{
	return TreeRoomsExits;
}

/* Returns the party's other slots (Garet, Ivan and, once she has joined, Mia),
   placed for the scripts, and the two Tret actors. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	return TreeRoomsActorPlacements;
}

/* Isaac landing on a leaf on one of Tret Tree's webs tears it the first time.
   The next time the leaf breaks: he starts in surprise, spins as he drops
   through the web, leaving a hole where the leaf lay, and falls into the room
   below, landing straight under the leaf. The torn leaf and the hole are
   pictures kept on a spare row of the map (`sourceColumn` on `sourceRow`, and
   the row below it), copied over the leaf's metatile in the map's upper layer.
   `entrance` is where the fall lands. `sLastLeaf` keeps a landing from counting
   again while he stands on the leaf, until he jumps off it
   (scTreeRoomsJumpOffLeaf). FLAG_ARRIVE_FALLING has him drop into the room
   below, except from the fifth floor's centre leaf, whose fall goes on down
   through the rooms (scInit). */
static void scTreeRoomsStepOnLeaf(
	Sint32 leafFlag, Sint32 sourceColumn, Sint32 sourceRow, Sint32 entrance)
{
	Sint32 destinationColumn;
	Sint32 destinationRow;
	Sint32 leaderActorId;
	ACTOR *actor;
	Sint32 spinFrame;

	destinationColumn = FIELD_METATILE(MainRoutineStateWork.x) + 64;
	destinationRow = FIELD_METATILE(MainRoutineStateWork.z);
	leaderActorId = MainRoutineStateWork.leaderActorId;
	actor = fdActorGet(leaderActorId);
	if(entrance == *sLastLeaf)
		return;
	*sLastLeaf = entrance;
	if(gmFlagGet(leafFlag) == 0) {
		fdMapGridCopyMetatiles(
			sourceColumn, sourceRow, 1, 1, destinationColumn, destinationRow);
		gmFlagSet(leafFlag);
		return;
	}
	/* Landing on a torn leaf: draw the hole and spin Isaac down through the
	   web. */
	*sLastLeaf = -1;
	fdMapGridCopyMetatiles(
		sourceColumn, sourceRow + 1, 1, 1, destinationColumn, destinationRow);
	sySndPlay(SE_CURSE_FALL);
	fdScriptStart();
	fdScriptSetDestination(SCENE_TREE_ROOMS, entrance);
	fdActorSetAnimation(leaderActorId, ANIM_LEADER_WALK_BACKWARDS);
	fdActorSetSprFlags(fdActorGet(leaderActorId), 0);
	fdActorSetAttachedEffect(
		leaderActorId, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdScriptWait(30);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	actor->motionFlg = ACTOR_FALLS;
	actor->groundHeight = -toFIXED(160);
	actor->gravity = toFIXED(1) / 2;
	sySndPlay(SE_DUST_PUFF);
	fdScriptWait(3);
	actor->layer = 2;
	fdActorSetSprPriority(leaderActorId, 3);
	for(spinFrame = 0; spinFrame < LEAF_FALL_SPIN_FRAMES; spinFrame++) {
		actor->facing += 2 * FACING_STEP;
		sySynch(1);
	}
	if(entrance != TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_LEAF)
		gmFlagSet(FLAG_ARRIVE_FALLING);
}

/* The leaves' step cues, one for each leaf: each passes its flag, the column of
   its torn picture on the spare row and its landing entrance to
   scTreeRoomsStepOnLeaf. The second floor's nine are lettered A to I in the
   order of their landing entrances. A leaf on the second floor drops Isaac into
   the entrance room, on the third to the second floor, and on the fifth to the
   fourth. */

static void scTreeRoomsStepOnSecondFloorLeafA(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_A_TORN, 64, 35,
		TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_A);
}

static void scTreeRoomsStepOnSecondFloorLeafB(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_B_TORN, 65, 35,
		TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_B);
}

/* The leaf Isaac can jump to from the floor by the west door. */
static void scTreeRoomsStepOnSecondFloorLeafC(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_C_TORN, 66, 35,
		TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_C);
}

static void scTreeRoomsStepOnSecondFloorLeafD(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_D_TORN, 67, 35,
		TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_D);
}

static void scTreeRoomsStepOnSecondFloorLeafE(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_E_TORN, 68, 35,
		TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_E);
}

static void scTreeRoomsStepOnSecondFloorLeafF(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_F_TORN, 69, 35,
		TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_F);
}

static void scTreeRoomsStepOnSecondFloorLeafG(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_G_TORN, 70, 35,
		TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_G);
}

static void scTreeRoomsStepOnSecondFloorLeafH(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_H_TORN, 71, 35,
		TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_H);
}

static void scTreeRoomsStepOnSecondFloorLeafI(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_I_TORN, 72, 35,
		TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_I);
}

/* West of the Healing Ring's chest. */
static void scTreeRoomsStepOnThirdFloorWestLeaf(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_THIRD_FLOOR_WEST_LEAF_TORN, 73, 35,
		TREE_ROOMS_FROM_THIRD_FLOOR_WEST_LEAF);
}

/* East of the Healing Ring's chest, below the vine to the fourth floor. */
static void scTreeRoomsStepOnThirdFloorEastLeaf(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_THIRD_FLOOR_EAST_LEAF_TORN, 74, 35,
		TREE_ROOMS_FROM_THIRD_FLOOR_EAST_LEAF);
}

/* The way to Tret: the fall goes on through every room below it (scInit) into
   Tret's room. */
static void scTreeRoomsStepOnFifthFloorCenterLeaf(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_CENTER_LEAF_TORN, 79, 35,
		TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_LEAF);
}

static void scTreeRoomsStepOnFifthFloorNorthwestLeaf(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_NORTHWEST_LEAF_TORN, 75,
		35, TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHWEST_LEAF);
}

static void scTreeRoomsStepOnFifthFloorNorthLeaf(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_NORTH_LEAF_TORN, 76, 35,
		TREE_ROOMS_FROM_FIFTH_FLOOR_NORTH_LEAF);
}

/* West of the Nut's chest. */
static void scTreeRoomsStepOnFifthFloorNortheastLeaf(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_NORTHEAST_LEAF_TORN, 77,
		35, TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHEAST_LEAF);
}

static void scTreeRoomsStepOnFifthFloorWestLeaf(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_WEST_LEAF_TORN, 78, 35,
		TREE_ROOMS_FROM_FIFTH_FLOOR_WEST_LEAF);
}

static void scTreeRoomsStepOnFifthFloorEastLeaf(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_EAST_LEAF_TORN, 80, 35,
		TREE_ROOMS_FROM_FIFTH_FLOOR_EAST_LEAF);
}

/* Its fall lands not straight below but where the west leaf's does, two
   metatiles further north (metatile (28, 26)). */
static void scTreeRoomsStepOnFifthFloorSouthwestLeaf(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHWEST_LEAF_TORN, 81,
		35, TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHWEST_LEAF);
}

static void scTreeRoomsStepOnFifthFloorSouthLeaf(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_SOUTH_LEAF_TORN, 82, 35,
		TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTH_LEAF);
}

static void scTreeRoomsStepOnFifthFloorSoutheastLeaf(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHEAST_LEAF_TORN, 83,
		35, TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHEAST_LEAF);
}

/* Below the Nut's chest. */
static void scTreeRoomsStepOnFifthFloorFarSoutheastLeaf(void)
{
	scTreeRoomsStepOnLeaf(FLAG_TRET_TREE_FIFTH_FLOOR_FAR_SOUTHEAST_LEAF_TORN,
		84, 35, TREE_ROOMS_FROM_FIFTH_FLOOR_FAR_SOUTHEAST_LEAF);
}

/* Walking while Isaac stands on a leaf in Tret Tree makes him jump two
   metatiles that way, over the web to the next leaf, wherever there is room to
   land (fdTryJumpForward). Once he has jumped, `sLastLeaf` is cleared, so that
   the leaf he lands on counts as a fresh landing. */
static void scTreeRoomsJumpOffLeaf(void)
{
	if(fdTryJumpForward() == 0)
		*sLastLeaf = -1;
}

/* Returns the doors, the vines, the leaves, Tret, Reveal, the chests and the
   Psynergy Stone. */
const CUE *scGetCues(void)
{
	return TreeRoomsCues;
}

/* Walking up to Tret's face in the north wall of the lowest room in Tret Tree
   (metatiles (9, 5) to (11, 5)) starts the battle with him. He roars "Hm
   HROOOM!!!", Garet, Ivan and Mia, once she has joined, gather beside Isaac
   before him, and his face turns wrathful: "You hoped to find my kindly self?"
   The party start back, and he swears they will all become trees before the
   screen breaks into bars for the battle. It runs only while FLAG_TRET_DEFEATED
   is clear. The battle returns to TREE_ROOMS_AFTER_FIGHT_TRET, where the setup
   plays scTreeRoomsPlayPostBattleConversation. */
static void scTreeRoomsFightTret(void)
{
	Sint32 hasMia;

	hasMia = gmFlagGet(FLAG_IN_PARTY(MIA));
	fdScriptStart();
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdScriptSetMessageId(MsgTretHmHrooom);
	/* "Hm HROOOM!!!" */
	fdScriptShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 20);
	sySndPlay(MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorGet(ACTOR_MIA)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprPriority(ACTOR_MIA, 2);
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 2);
	scPlaceAtLeader(ACTOR_GARET);
	scPlaceAtLeader(ACTOR_IVAN);
	if(hasMia != 0) {
		scPlaceAtLeader(ACTOR_MIA);
		fdActorSetScript(ACTOR_MIA, TreeRooms_MiaGathersActions);
	}
	fdActorSetScript(ACTOR_PARTY_LEADER, TreeRooms_LeaderGathersActions);
	fdActorSetScript(ACTOR_GARET, TreeRooms_GaretGathersActions);
	fdActorRunScriptAndWait(ACTOR_IVAN, TreeRooms_IvanGathersActions);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 40);
	fdActorSetAnimation(ACTOR_TRET, TRET_FACE_TURNING_WRATHFUL);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_CLOSED);
	fdScriptWait(20);
	scTreeRoomsChangeFace(TRET_FACE_CHANGE_WRATHFUL_OPEN_EYES);
	/* "You! You hoped to find my kindly self?" */
	fdScriptShowMessage(TREE_ROOMS_SPEAKER(ACTOR_TRET), 0);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	scTreeRoomsChangeFace(TRET_FACE_CHANGE_WRATHFUL_BLINK);
	/* "Fools! You should never have come here, and now, you shall never
	   leave!" */
	syFieldScriptShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_TRET), 0, 10);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdActorStartRepeatedMotion(ACTOR_MIA, 1);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	/* "Your powers protected you in the forest, but now you are in the heart of
	   my power!" */
	fdScriptShowMessage(TREE_ROOMS_SPEAKER(ACTOR_TRET), 0);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	scTreeRoomsChangeFace(TRET_FACE_CHANGE_WRATHFUL_BLINK);
	/* "Here, you will all become trees, and you'll wither with the rest of
	   us!" */
	fdScriptShowMessage(TREE_ROOMS_SPEAKER(ACTOR_TRET), 0);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
	FieldStateWork->transitionFrm = 64;
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	fdScriptSetAfterBattleDestination(
		SCENE_TREE_ROOMS, TREE_ROOMS_AFTER_FIGHT_TRET);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, 0);
	fdScriptEnd();
}

/* After the battle with Tret, the view slides down the middle of his room as a
   vine grows up out of the floor, and Tret comes to himself ("Oh... Mmmmm...").
   He thanks the party and says good-bye, but Garet and Ivan will not let him
   die while they still need his help. Learning that he turned the people of
   Kolima into trees, he tries to free them: the room turns purple and sparks
   swirl out from his face, but his power no longer reaches Kolima. His face
   sinks into the wall, and the party resolve to heal him. Only then is
   FLAG_TRET_DEFEATED set and the light's slow swell started again
   (scTretPaletteUpdateMapTint). Answering "Do you need something?" with no
   makes Garet and Ivan scold Isaac. */
static void scTreeRoomsPlayPostBattleConversation(void)
{
	Sint32		  hasMia;
	EVENT_FUNC	  sparkTask;
	const Sint32 *actions;

	hasMia = gmFlagGet(FLAG_IN_PARTY(MIA));
	fdActorGet(ACTOR_MIA)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprPriority(ACTOR_MIA, 2);
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 2);
	gmPartyInventoryFindMember(ITEM_HERMES_WATER);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdScriptStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	fdActorSetSpeed(ACTOR_MIA, 0.8);
	fdActorSetPos(ACTOR_PARTY_LEADER, 166, 80);
	fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_NORTH;
	fdActorSetPos(ACTOR_GARET, 148, 90);
	fdActorGet(ACTOR_GARET)->facing = FACING_NORTH;
	fdActorSetPos(ACTOR_IVAN, 182, 90);
	fdActorGet(ACTOR_IVAN)->facing = FACING_NORTH;
	if(hasMia != 0) {
		fdActorSetPos(ACTOR_MIA, 166, 104);
		fdActorGet(ACTOR_MIA)->facing = FACING_NORTH;
	}
	scTreeRoomsChangeFace(TRET_FACE_CHANGE_CLOSE_EYES);
	sySynch(10);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	FieldStateWork->transitionFrm = 48;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdScriptWait(20);
	fdCameraSetSpeed(1.2);
	fdCameraMoveTo(168, CAMERA_KEEP, 152, 1);
	fdCameraWaitForMove();
	fdScriptWait(10);
	sySndPlay(SE_MAP_EXIT);
	fdMapGridCopyMetatileAttributes(26, 3, 1, 2, 10, 8);
	fdMapGridCopyMetatiles(26, 38, 1, 1, 10, 43);
	sySynch(4);
	fdMapGridCopyMetatiles(26, 37, 1, 2, 10, 42);
	sySynch(4);
	fdMapGridCopyMetatiles(26, 36, 1, 3, 10, 41);
	sySynch(4);
	fdMapGridCopyMetatiles(26, 35, 1, 4, 10, 40);
	sySynch(80);
	fdScriptSetMessageId(MsgTretMmmmm);
	/* "Oh... Mmmmm..." */
	syFieldScriptShowMessageThenWait(
		TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	fdCameraMoveTo(168, CAMERA_KEEP, 90, 1);
	fdCameraWaitForMove();
	fdScriptWait(40);
	scTreeRoomsChangeFace(TRET_FACE_CHANGE_OPEN_EYES);
	fdScriptWait(60);
	sySndPlay(MUS_FOREST_THEME);
	scTreeRoomsChangeFace(TRET_FACE_CHANGE_BLINK);
	/* "What has happened to me?" */
	syFieldScriptShowMessageThenWait(
		TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	/* "Was I... I see... My heart was all but lost in my rage." */
	fdScriptShowMessage(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0);
	fdScriptWait(40);
	/* "And you defeated the evil within me?" */
	syFieldScriptShowMessageThenWait(
		TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(20);
	/* "I thank you. I could not leave this world with so much evil in my
	   heart." */
	syFieldScriptShowMessageThenWait(
		TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 20);
	scTreeRoomsChangeFace(TRET_FACE_CHANGE_CLOSE_EYES);
	fdScriptWait(40);
	/* "I am free now to leave the world without regret. Good-bye, great
	   warriors." */
	syFieldScriptShowMessageThenWait(
		TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 20);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
	/* "Hey! Wait a minute! We can't let Tret die like this!" */
	syFieldScriptShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_GARET), 0, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	/* "That's right, Isaac. We still need his help!!" */
	syFieldScriptShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_IVAN), 0, 20);
	scTreeRoomsChangeFace(TRET_FACE_CHANGE_CLOSE_EYES);
	fdScriptWait(40);
	/* "...What?" */
	syFieldScriptShowMessageThenWait(
		TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 10);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 40);
	scTreeRoomsChangeFace(TRET_FACE_CHANGE_BLINK);
	/* "Why have you not left me to my dying? Do you need something?" */
	fdScriptOpenMessage(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) != 0) {
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
		fdActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
		fdScriptSetMessageId(MsgTretDoingNowsNot);
		/* "What are you doing, Isaac! Now's not the time to fool around!" */
		fdScriptShowMessage(TREE_ROOMS_SPEAKER(ACTOR_GARET), 0);
		fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
		fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
		/* "That's right, Isaac... We have to save everyone in Kolima!" */
		fdScriptShowMessage(TREE_ROOMS_SPEAKER(ACTOR_IVAN), 0);
	}
	fdScriptWait(20);
	scTreeRoomsChangeFace(TRET_FACE_CHANGE_BLINK);
	fdScriptSetMessageId(MsgTretTurnedPeopleKolima);
	/* "I turned the people of Kolima into trees?" */
	syFieldScriptShowMessageThenWait(
		TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 20);
	/* "Hroom! I must release them before they die with the forest!" */
	syFieldScriptShowMessageThenWait(
		TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 10);
	scTreeRoomsChangeFace(TRET_FACE_CHANGE_CLOSE_EYES);
	fdScriptWait(20);
	/* Tret tries to release the people of Kolima: the room turns purple and
	   sparks swirl out from his face, then fade. */
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
	fdPaletteFadeStartInterpolation(20);
	sySynch(40);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
	fdScriptWait(20);
	sSparkFrame = 0;
	{
		FIELD_POS *origin = &sSparkOrigin;
		origin->x = toFIXED(168);
		origin->y = toFIXED(32);
		sparkTask = scTreeRoomsSpawnSwirlSparks;
		origin->z = toFIXED(52);
	}
	sySetFrameEvent(sparkTask, EVENT_PASS_FRAME);
	fdScriptWait(220);
	syCloseEvent(sparkTask);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
	fdPaletteFadeStartInterpolation(20);
	sySynch(40);
	scTreeRoomsChangeFace(TRET_FACE_CHANGE_BLINK);
	fdScriptWait(20);
	/* "I can't do it... My power no longer reaches Kolima..." */
	syFieldScriptShowMessageThenWait(
		TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0, 10);
	scTreeRoomsChangeFace(TRET_FACE_CHANGE_CLOSE_EYES);
	/* "People of Kolima... Forgive me..." */
	fdScriptShowMessage(TREE_ROOMS_SPEAKER(ACTOR_SECOND_TRET), 0);
	fdActorRunScriptAndWait(ACTOR_TRET, TreeRooms_FaceSinksActions);
	fdScriptWait(40);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	/* "Tret's too weak! He doesn't have the power to save Kolima." */
	fdScriptShowMessage(TREE_ROOMS_SPEAKER(ACTOR_GARET), 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 10);
	/* "No! We have to heal him! We can't let the people of Kolima die with the
	   forest!" */
	fdScriptShowMessage(TREE_ROOMS_SPEAKER(ACTOR_IVAN), 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	/* "Every life in Kolima depends on us!" */
	fdScriptShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_GARET), 0, 10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	/* "We have to restore Tret and save the people of Kolima!" */
	fdScriptShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_IVAN), 0, 10);
	if(hasMia != 0) {
		fdActorRunRepeatedMotion(ACTOR_MIA, 1);
		/* "Let's go, Isaac." */
		fdScriptShowMessageThenWait(TREE_ROOMS_SPEAKER(ACTOR_MIA), 0, 10);
	}
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	actions = TreeRooms_WalkBackToLeaderScript;
	fdActorSetScript(ACTOR_GARET, actions);
	if(hasMia != 0) {
		fdActorSetScript(ACTOR_MIA, actions);
	}
	fdActorRunScriptAndWait(ACTOR_IVAN, actions);
	fdScriptWait(20);
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	gmFlagSet(FLAG_TRET_DEFEATED);
	sySetFrameEvent(scTretPaletteUpdateMapTint, EVENT_PASS_FRAME);
	fdScriptEnd();
}

/* Sets Tret's room and the entrance room for whether Tret is beaten, after
   playing the conversation that follows the battle when `playRecoveryEvent`
   asks for it. Before, the middle of Tret's room (metatile (10, 8)) is bare
   floor and the hole in the entrance room's floor is a bare hole. After, the
   vine stands in Tret's room and can be climbed, and its top shows in the hole
   in the entrance room (metatile (30, 9)), where stepping on it takes Isaac
   down to Tret's room. The vine's metatiles take their climbing from the vine
   near the entrance room's north wall (metatile (26, 3)), and the hole's
   metatiles take the exit down to Tret from the spare metatile at (0, 0). */
static void scTreeRoomsApplyPostBattleState(Sint32 playRecoveryEvent)
{
	if(playRecoveryEvent != 0 && gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
		scTreeRoomsPlayPostBattleConversation();

	sySynch(1);
	if(gmFlagGet(FLAG_TRET_DEFEATED) != 0) {
		fdMapGridCopyMetatiles(121, 34, 3, 1, 93, 10);
		fdMapGridCopyMetatiles(46, 38, 1, 1, 30, 43);
		fdMapGridCopyMetatileAttributes(0, 0, 1, 2, 30, 9);
		fdMapGridCopyMetatileAttributes(26, 3, 1, 2, 10, 8);
		fdMapGridCopyMetatiles(26, 35, 1, 4, 10, 40);
	} else {
		fdMapGridCopyMetatileAttributes(11, 8, 1, 2, 10, 8);
	}
}

/* Entering any of Tret Tree's rooms: once Tret is beaten his face is gone from
   the lowest room, the light's slow swell starts, leaves torn before a battle
   are torn again, and four leaves start torn. Arriving by a fall from the fifth
   floor's centre leaf, Isaac drops in from above and on through the hole in the
   floor, a room at a time, until he lands before Tret
   (scTreeRoomsLandFromFall). Back from the battle with Tret, the conversation
   after it plays. The setup also saves the rooms' colours for the swell
   (scTretPaletteSaveMapCols) and sets the map's first layer to scroll at nine
   tenths of the pace of the view, offset to the room. */
Sint32 scInit(void)
{
	ACTOR				  *tret;
	ACTOR				  *actor;
	MAP_GRID_SCROLL_STATE *grid;
	MAP_GRID_LAYER_SCROLL *layer;
	FIELD_STATE			  *state;
	Sint32				   entrance;
	Sint32				   leaderActorId;
	ACTOR				  *secondTret;

	tret = fdActorGet(ACTOR_TRET);
	sLastLeaf = (Sint16 *)SceneLocalState;
	scTretPaletteSaveMapCols();
	tret->motionFlg = 0;
	tret->y.fixed = -toFIXED(10);
	secondTret = fdActorGet(ACTOR_SECOND_TRET);

	secondTret->motionFlg = 0;
	secondTret->y.fixed = -toFIXED(10);
	fdActorSetSprColById(ACTOR_SECOND_TRET, SPRITE_COLOR_HIDDEN);
	scTreeRoomsChangeFace(TRET_FACE_CHANGE_CLOSE_EYES);
	if(MainRoutineStateWork.entrance != TREE_ROOMS_AFTER_FIGHT_TRET)
		sySetFrameEvent(scTretPaletteUpdateMapTint, EVENT_PASS_FRAME);
	if(gmFlagGet(FLAG_TRET_DEFEATED)) {
		fdActorSetPos(ACTOR_SECOND_TRET, 0, 0);
		fdActorSetPos(ACTOR_TRET, 0, 0);
	}
	if(fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
		scTreeRoomsApplyFlaggedMapPatches();
	grid = MapGridState[0];
	/* Match the first layer's parallax offset and scale to the room bounds. */
	layer = &grid->layers[0];
	layer->ofsX += syMulFX(grid->minX + toFIXED(160), toFIXED(1) / 10);
	layer->ofsY += syMulFX(grid->minY + toFIXED(136), toFIXED(1) / 10);
	layer->scaleX = toFIXED(9) / 10;
	layer->scaleY = toFIXED(9) / 10;
	/* The layout draws four leaves already torn, so the first landing on them
	   breaks them. */
	gmFlagSet(FLAG_TRET_TREE_SECOND_FLOOR_LEAF_B_TORN);
	gmFlagSet(FLAG_TRET_TREE_FIFTH_FLOOR_NORTH_LEAF_TORN);
	gmFlagSet(FLAG_TRET_TREE_FIFTH_FLOOR_WEST_LEAF_TORN);
	gmFlagSet(FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHEAST_LEAF_TORN);
	sySynch(1);
	scTreeRoomsApplyPostBattleState(0);
	state = FieldStateWork;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	entrance = MainRoutineStateWork.entrance;
	leaderActorId = MainRoutineStateWork.leaderActorId;
	actor = fdActorGet(leaderActorId);
	/* Falling through a room, Isaac falls on to the entrance numbered ten
	   lower; at the fall's end he lands in a ring of dust in Tret's room.
	   TREE_ROOMS_AFTER_FIGHT_TRET: back from the battle with Tret. */
	if(entrance == TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_LEAF ||
		entrance == TREE_ROOMS_FALL_THROUGH_A ||
		entrance == TREE_ROOMS_FALL_THROUGH_B ||
		entrance == TREE_ROOMS_FALL_THROUGH_C) {
		fdScriptStartScreenTransition();
		fdActorSetAnimation(leaderActorId, ANIM_LEADER_WALK_BACKWARDS);
		fdActorSetSprFlags(fdActorGet(leaderActorId), 0);
		fdActorSetAttachedEffect(
			leaderActorId, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		actor->motionFlg = ACTOR_FALLS;
		actor->y.fixed = toFIXED(100);
		actor->groundHeight = -toFIXED(160);
		actor->gravity = toFIXED(1) / 2;
		sySndPlay(SE_DUST_PUFF);
		fdScriptSetDestination(SCENE_TREE_ROOMS, entrance - FALL_ENTRANCE_STEP);
		fdScriptWait(20);
		actor->layer = 2;
		fdActorSetSprPriority(leaderActorId, 3);
		fdScriptWait(2);
		fdActorSetAttachedEffect(
			leaderActorId, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
		fdScriptWait(8);
	} else if(entrance == TREE_ROOMS_FALL_LANDING) {
		if(!gmFlagGet(FLAG_KEEP_PARTY_POSITION))
			scTreeRoomsLandFromFall();
	} else if(entrance == TREE_ROOMS_AFTER_FIGHT_TRET) {
		scTreeRoomsApplyPostBattleState(1);
	}
	return 0;
}

/* Saves background palettes 0 to 13, the rooms' own colours, which the light's
   slow swell inside Tret brightens (scTretPaletteUpdateMapTint). */
static void scTretPaletteSaveMapCols(void)
{
	syDMAXCopy((const void *)BG_PLTT, PaletteFadeState->pal,
		DMA_COPY_WORDS(112), REG_DMA3);
}

/* The light inside Tret Tree slowly swells and fades: over a few seconds the
   walls and floors grow a little brighter and warmer, then sink back, again and
   again. It pauses while Psynergy is being cast, and stops for good once Reveal
   is cast (scTreeRoomsCastReveal). Every 32 frames it adds the next red, green
   and blue step of TreeRooms_TintSteps, a ramp from 0 up to 4 and back, to the
   saved colours of background palettes 1 to 4, skipping a random few steps each
   time; each later palette takes a weaker step. */
static void scTretPaletteUpdateMapTint(void)
{
	Uint16		 *sourceColors;
	Uint16		 *destinationColors;
	Uint32		  colorIndex;
	Sint32		  redStep;
	Sint32		  greenStep;
	Sint32		  blueStep;
	Sint32		  red;
	Sint32		  green;
	Sint32		  blue;
	Uint32		  color;
	FIELD_STATE	 *state;
	PALETTE_FADE *paletteState;

	state = FieldStateWork;
	paletteState = PaletteFadeState;
	sourceColors = paletteState->pal;
	if(state->psynergy != 0)
		return;
	if((FrameCount & 31) != 0)
		return;
	sourceColors += PALETTE_COLORS;
	destinationColors = (Uint16 *)&BG_PLTT_COLOR(1, 0);
	for(colorIndex = 0; colorIndex < TRET_TINT_COLOR_COUNT;
		colorIndex++, sourceColors++) {
		redStep = TreeRooms_TintSteps[TreeRooms_TintIndex];
		greenStep = TreeRooms_TintSteps[TreeRooms_TintIndex + 1];
		blueStep = TreeRooms_TintSteps[TreeRooms_TintIndex + 2];
		if(colorIndex >= 3 * PALETTE_COLORS) {
			redStep -= redStep / 2 + redStep / 3;
			greenStep -= greenStep / 2 + greenStep / 3;
			blueStep -= blueStep / 2 + blueStep / 3;
		} else if(colorIndex >= 2 * PALETTE_COLORS) {
			redStep -= redStep / 3 + redStep / 4;
			greenStep -= greenStep / 3 + greenStep / 4;
			blueStep -= blueStep / 3 + blueStep / 4;
		} else if(colorIndex >= PALETTE_COLORS) {
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
		if(red > TRET_TINT_CHANNEL_MAX)
			red = TRET_TINT_CHANNEL_MAX;
		if(green > TRET_TINT_CHANNEL_MAX)
			green = TRET_TINT_CHANNEL_MAX;
		if(blue > TRET_TINT_CHANNEL_MAX)
			blue = TRET_TINT_CHANNEL_MAX;
		if(red < 0)
			red = 0;
		if(green < 0)
			green = 0;
		if(blue < 0)
			blue = 0;
		*destinationColors++ = RGB(red, green, blue);
	}
	TreeRooms_TintIndex += (syRandomNext() & 7) * 3;
	if(TreeRooms_TintSteps[TreeRooms_TintIndex] == TRET_TINT_END)
		TreeRooms_TintIndex = 0;
}

/* Changes Tret's face in the north wall of his room (opening or closing his
   eyes, blinking, looking aside), six frames to a picture, and waits twelve
   frames after it. */
static void scTreeRoomsChangeFace(Sint32 change)
{
	switch(change) {
	case TRET_FACE_CHANGE_CLOSE_EYES:
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
		sySynch(6);
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_CLOSED);
		break;
	case TRET_FACE_CHANGE_OPEN_EYES:
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
		break;
	case TRET_FACE_CHANGE_LOOK_LEFT:
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
		sySynch(6);
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_LOOK_LEFT);
		break;
	case TRET_FACE_CHANGE_LOOK_RIGHT:
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
		sySynch(6);
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_LOOK_RIGHT);
		break;
	case TRET_FACE_CHANGE_BLINK:
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
		sySynch(6);
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_CLOSED);
		sySynch(6);
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
		sySynch(6);
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_CLOSED);
		sySynch(6);
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
		break;
	case TRET_FACE_CHANGE_HALF_CLOSE_EYES:
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
		sySynch(6);
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_HALF_CLOSED);
		break;
	case TRET_FACE_CHANGE_WRATHFUL_CLOSE_EYES:
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
		sySynch(6);
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_CLOSED);
		break;
	case TRET_FACE_CHANGE_WRATHFUL_OPEN_EYES:
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
		break;
	case TRET_FACE_CHANGE_WRATHFUL_LOOK_RIGHT:
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
		sySynch(6);
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_LOOK_RIGHT);
		break;
	case TRET_FACE_CHANGE_WRATHFUL_LOOK_LEFT:
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
		sySynch(6);
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_LOOK_LEFT);
		break;
	case TRET_FACE_CHANGE_WRATHFUL_BLINK:
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
		sySynch(6);
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_CLOSED);
		sySynch(6);
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
		sySynch(6);
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_CLOSED);
		sySynch(6);
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
		break;
	case TRET_FACE_CHANGE_WRATHFUL_STARE:
		fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
		break;
	}
	sySynch(12);
}

/* One of the sparks that swirl out from Tret's face as he tries to free the
   people of Kolima: it spirals outward from his face, shrinking as it goes, and
   is put away after 80 frames. */
static void scTreeRoomsUpdateSwirlSpark(ACTOR *spark)
{
	FIELD_POS position;
	Sint32	  frame;

	frame = spark->tmr;
	if(frame <= SWIRL_SPARK_LAST_FRAME) {
		position.x = spark->tgtX;
		position.y = spark->tgtY;
		position.z = spark->tgtZ;
		{
			Sint32 initialAngle = spark->userValue;

			syVectorAddPolarOffset(toFIXED(frame),
				((frame * 3) << 8) + initialAngle, (FIXED *)&position);
		}
		spark->x.fixed = position.x;
		spark->y.fixed = position.y;
		spark->z.fixed = position.z;
		if(spark->tmr <= SWIRL_SPARK_SHRINK_LAST_FRAME) {
			spark->scaleX -= toFIXED(1) / 50;
			spark->scaleY -= toFIXED(1) / 50;
		}
		spark->tmr++;
	} else {
		syVRAMCacheFreeEntry(spark->spr->slot);
		fdActorRelease(spark);
	}
}

/* Sends out the sparks from Tret's face as he tries to free the people of
   Kolima: five waves, ten frames apart, of six, five, four, three and two
   sparks, each wave with a shimmering sound. */
static void scTreeRoomsSpawnSwirlSparks(void)
{
	ACTOR *spark;
	Uint32 frame;
	void  *previousResourceEntry;
	Sint32 wave;
	Uint32 sparkIndex;

	frame = sSparkFrame;
	previousResourceEntry = 0;
	wave = (Sint32)frame / 10;
	switch(frame) {
	case 0:
	case 10:
	case 20:
	case 30:
	case 40:
		sySndPlay(SE_SHIMMER);
		/* Waves of 6, 5, 4, 3 and 2 sparks, spread evenly round a turn. */
		for(sparkIndex = 0; sparkIndex < 6 - wave; sparkIndex++) {
			spark = syFieldActorCreate(
				SPRITE_SPARKLE, sSparkOrigin.x, sSparkOrigin.y, sSparkOrigin.z);
			if(spark != NULL) {
				previousResourceEntry =
					fdSprShareVRAMCacheEntry(spark->spr, previousResourceEntry);
				spark->motionFlg = 0;
				spark->spr->image.pri = 0;
				fdActorSetSprFlags(spark, 0);
				fdActorSelectAnimation(spark, 1);
				spark->tmr = 0;
				spark->userValue =
					DEGtoANG(360 / (Uint32)(6 - wave) * sparkIndex);
				spark->tgtX = sSparkOrigin.x;
				spark->tgtY = sSparkOrigin.y;
				spark->tgtZ = sSparkOrigin.z;
				spark->speedLimit = toFIXED(8) / 5;
				spark->update = scTreeRoomsUpdateSwirlSpark;
			}
		}
		/* fall through */
	case 44:
		sySndPlay(SOUND_FADE_OUT_ACTION_SE);
		break;
	}
	if(++sSparkFrame > 120) {
		sSparkFrame = 0;
	}
}

/* Tears again each leaf that was torn before a battle, so that the webs look as
   the party left them. */
static void scTreeRoomsApplyFlaggedMapPatches(void)
{
	Sint32 patchOffset;

	for(patchOffset = 0; TreeRooms_MapPatches[patchOffset] != -1;
		patchOffset += MAP_PATCH_ROW_LENGTH) {
		if(gmFlagGet(TreeRooms_MapPatches[patchOffset + MAP_PATCH_FLAG]) &&
			TreeRooms_MapPatches[patchOffset + MAP_PATCH_REDRAWN] != 0)
			fdMapGridCopyMetatilesTo(
				TreeRooms_MapPatches[patchOffset + MAP_PATCH_SOURCE_X],
				TreeRooms_MapPatches[patchOffset + MAP_PATCH_SOURCE_Y],
				TreeRooms_MapPatches[patchOffset + MAP_PATCH_DESTINATION_X],
				TreeRooms_MapPatches[patchOffset + MAP_PATCH_DESTINATION_Y], 1,
				1);
	}
}

/* Casting Reveal anywhere inside Tret Tree stops the light's slow swell and
   fade, leaving the rooms' colours as they are, until Isaac next goes through a
   door, up or down a vine or through a leaf into another room. */
static void scTreeRoomsCastReveal(void)
{
	syCloseEvent(scTretPaletteUpdateMapTint);
}

#include "shared/field_effect_advance.c"

/* Isaac lands in Tret's room at the end of the fall from the fifth floor's
   centre leaf: he drops into the middle of the room, south of Tret's face, and
   hits the floor with a thump in a ring of white dust as the screen shakes,
   staggering back in surprise. */
static void scTreeRoomsLandFromFall(void)
{
	EFFECT_OPTIONS options;
	FIELD_POS	   velocity;
	ACTOR		  *leader;
	Uint32		   particleIndex;
	Sint32		   velocityX;
	FIXED		   velocityZ;

	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdActorGet(ACTOR_PARTY_LEADER)->y.fixed = toFIXED(130);
	fdActorGet(ACTOR_PARTY_LEADER)->gravity = toFIXED(1) / 2;
	fdActorGet(ACTOR_PARTY_LEADER)->bounce = 0;
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	sySndPlay(SE_DUST_PUFF);
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg =
		ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdScriptWait(24);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	options.pal = SPRITE_COLOR_WHITE;
	options.update = scFieldEffectAdvanceMotion;
	options.startScaleX = toFIXED(4) / 5;
	options.startScaleY = toFIXED(4) / 5;
	for(particleIndex = 0; particleIndex <= TRIG_TURN / FACING_STEP;
		particleIndex++) {
		velocity.x = syCos(particleIndex * FACING_STEP);
		velocity.y = 0;
		velocityZ = sySin(particleIndex * FACING_STEP);
		velocityX = velocity.x;
		velocity.z = velocityZ;
		velocityX += velocityX / 2;
		velocity.x = velocityX;
		fdEffectSpawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
			velocityX, velocity.y, velocityZ,
			EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE |
				EFFECT_SCRIPT_MEDIUM,
			&options);
	}
	sySndPlay(SE_THUMP);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
	fdMapGridSetShake(5, 5, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	fdActorGet(ACTOR_PARTY_LEADER)->gravity = toFIXED(1);
	fdActorGet(ACTOR_PARTY_LEADER)->bounce = toFIXED(1) / 4;
	fdScriptEnd();
}

#include "shared/field_effect_duration_scripts.c"

/* The leaves torn again after a battle. Each row is the leaf's flag, whether it
   is redrawn (0 for the four the map draws torn), the torn picture's metatile
   x/y on the spare row and the leaf's metatile x/y in the map's upper layer. */
/* clang-format off */
Sint16 TreeRooms_MapPatches[] = {
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

const Sint32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

/* The party gathers in front of Tret before the battle */
/* (scTreeRoomsFightTret), each walking to its own place. */
Sint32 TreeRooms_LeaderGathersActions[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(166, 0, 80),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 TreeRooms_GaretGathersActions[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(148, 0, 90),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 TreeRooms_IvanGathersActions[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(182, 0, 90),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 TreeRooms_MiaGathersActions[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(166, 0, 104),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* Tret's face sinking into the wall at the end of the conversation after the */
/* battle. Eight times it is raised 80 pixels for 4 frames and lowered 78, a */
/* sixteenth smaller each time, and then it is put away. */
Sint32 TreeRooms_FaceSinksActions[] = {
	SC_SET(SW_MOTION, 0),
	SC_ADD(SW_Y, toFIXED(80)),
	SC_WAIT(4),
	SC_ADD(SW_Y, -toFIXED(78)),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
	SC_WAIT(4),
	SC_LOOP(8, SC_TOP),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

/* Garet, Ivan and Mia walk back into Isaac at the end of the conversation */
/* after the battle, and are put away. */
Sint32 TreeRooms_WalkBackToLeaderScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(166, 0, 80),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

/* The tint's steps of red, green and blue, three to an entry: a ramp from 0 */
/* up to 4 and back. */
Uint32 TreeRooms_TintSteps[] = {
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

SCENE_ENTRANCE TreeRoomsEntrances[] = {
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

SCENE_EXIT_REGION TreeRoomsRegions[] = {
	{ 420, 28, 68, 428, 36, 76, -1, TREE_ROOMS_ENTRANCE_TO_SECOND_FLOOR },
	{ 228, 28, 338, 236, 36, 346, -1, TREE_ROOMS_THIRD_FLOOR_TO_FOURTH_FLOOR },
#if !ALTIN_LADDER_AND_TRET_VINE_EXITS_ENLARGED
	{ 163, 28, 148, 171, 36, 156, -1, TREE_ROOMS_TRET_TO_ENTRANCE },
#else
	{ 159, 24, 144, 175, 40, 160, -1, TREE_ROOMS_TRET_TO_ENTRANCE },
#endif
	{ -1, 0, 0, 0, 0, 0, 0, 0 },
};

Uint32 TreeRoomsExits[] = {
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

ACTOR_PLACE TreeRoomsActorPlacements[] = {
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
/* clang-format on */

CUE TreeRoomsCues[] = {
	{CUE_EXIT | CUE_FLAG_FACING | FACING_SOUTH,
		TREE_ROOMS_TO_KOLIMA_FOREST_TREE, CONDITION_ALWAYS,
		TREE_ROOMS_TO_KOLIMA_FOREST_TREE},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_WEST,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHWEST, CONDITION_ALWAYS,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHWEST},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_EAST,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHEAST, CONDITION_ALWAYS,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHEAST},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_SOUTH,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH, CONDITION_ALWAYS,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_EAST,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTHEAST, CONDITION_ALWAYS,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTHEAST},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_WEST,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTHWEST, CONDITION_ALWAYS,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTHWEST},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_SOUTH,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH, CONDITION_ALWAYS,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_WEST,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH_NORTHWEST, CONDITION_ALWAYS,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH_NORTHWEST},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_EAST,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH_NORTHEAST, CONDITION_ALWAYS,
		TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH_NORTHEAST},
	{CUE_EXIT, TREE_ROOMS_SECOND_FLOOR_TO_ENTRANCE, CONDITION_ALWAYS,
		TREE_ROOMS_SECOND_FLOOR_TO_ENTRANCE},
	{CUE_EXIT, TREE_ROOMS_FOURTH_FLOOR_TO_THIRD_FLOOR, CONDITION_ALWAYS,
		TREE_ROOMS_FOURTH_FLOOR_TO_THIRD_FLOOR},
	{CUE_EXIT, TREE_ROOMS_ENTRANCE_TO_TRET, CONDITION_ALWAYS,
		TREE_ROOMS_ENTRANCE_TO_TRET},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_A,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_B,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_C,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_D,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_E,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_F,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_G,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_H,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_I,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_THIRD_FLOOR_WEST_LEAF,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_THIRD_FLOOR_EAST_LEAF,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_LEAF,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING,
		TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHWEST_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_FIFTH_FLOOR_NORTH_LEAF,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING,
		TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHEAST_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_FIFTH_FLOOR_WEST_LEAF,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_FIFTH_FLOOR_EAST_LEAF,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING,
		TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHWEST_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTH_LEAF,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING,
		TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHEAST_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING,
		TREE_ROOMS_FROM_FIFTH_FLOOR_FAR_SOUTHEAST_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsJumpOffLeaf)},
	{CUE_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_A, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnSecondFloorLeafA)},
	{CUE_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_B, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnSecondFloorLeafB)},
	{CUE_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_C, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnSecondFloorLeafC)},
	{CUE_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_D, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnSecondFloorLeafD)},
	{CUE_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_E, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnSecondFloorLeafE)},
	{CUE_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_F, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnSecondFloorLeafF)},
	{CUE_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_G, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnSecondFloorLeafG)},
	{CUE_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_H, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnSecondFloorLeafH)},
	{CUE_STEP, TREE_ROOMS_FROM_SECOND_FLOOR_LEAF_I, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnSecondFloorLeafI)},
	{CUE_STEP, TREE_ROOMS_FROM_THIRD_FLOOR_WEST_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnThirdFloorWestLeaf)},
	{CUE_STEP, TREE_ROOMS_FROM_THIRD_FLOOR_EAST_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnThirdFloorEastLeaf)},
	{CUE_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnFifthFloorCenterLeaf)},
	{CUE_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHWEST_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnFifthFloorNorthwestLeaf)},
	{CUE_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_NORTH_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnFifthFloorNorthLeaf)},
	{CUE_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHEAST_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnFifthFloorNortheastLeaf)},
	{CUE_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_WEST_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnFifthFloorWestLeaf)},
	{CUE_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_EAST_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnFifthFloorEastLeaf)},
	{CUE_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHWEST_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnFifthFloorSouthwestLeaf)},
	{CUE_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTH_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnFifthFloorSouthLeaf)},
	{CUE_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHEAST_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnFifthFloorSoutheastLeaf)},
	{CUE_STEP, TREE_ROOMS_FROM_FIFTH_FLOOR_FAR_SOUTHEAST_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(scTreeRoomsStepOnFifthFloorFarSoutheastLeaf)},
	{CUE_STEP, METATILE_BEHAVIOR_TREE_ROOMS_BEFORE_TRET, FLAG_TRET_DEFEATED,
		SCRIPT_REF(scTreeRoomsFightTret)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scTreeRoomsCastReveal)},
	{EXAMINE_CUE(EXAMINE_CHEST),
		METATILE_BEHAVIOR_TREE_ROOMS_HEALING_RING_CHEST,
		FLAG_TRET_TREE_HEALING_RING, FIND_ITEM(ITEM_HEALING_RING)},
	{EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_TREE_ROOMS_NUT_CHEST,
		FLAG_TRET_TREE_NUT, FIND_ITEM(ITEM_NUT)},
	{EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_TREE_ROOMS_PSYNERGY_STONE,
		FLAG_AREA_TEMP_50, FIND_PSYNERGY_STONE},
	{CUE_TABLE_END},
};

/* Where in TreeRooms_TintSteps the tint is, a random number of entries on each
   time. */
Sint32 TreeRooms_TintIndex = 0;
