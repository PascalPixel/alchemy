// Tret Tree's branches (SceneIdTretTreeBranch): the outside of Tret, the holy tree of Kolima
// Forest, high up under open sky, with clouds drifting past above and the green of the forest far
// below (places/tret-tree-branch/map-0.png). Isaac climbs Tret from the inside (SceneIdTreeRooms,
// tree_rooms.c), where the story is told; out here there is no story script, and nothing tests a
// story flag.
//
// Arched doors in Tret's trunk lead out onto the branches, two from each of the second to fifth
// floors. Walking into one goes back inside:
// - the second floor's west door (metatile (20, 26)) opens onto a branch
// that  leads nowhere, and its east door (metatile (26, 27)) onto the branch
// below  the southeast vine; - the third floor's south door (metatile (23,
// 22)) opens onto a branch that  runs west, and its east door (metatile (26,
// 20)) onto the branch above the  southeast vine; - the fourth floor's south
// door (metatile (23, 16)) opens onto the branch below  the northeast vine,
// and its west door (metatile (20, 15)) onto the branch  below the west
// vine; - the fifth floor's west door (metatile (20, 9)) opens onto the
// branch above the  west vine, and its east door (metatile (26, 8)) onto the
// branch above the  northeast vine.
//
// So the way up Tret goes out of the second floor's east door, up the southeast vine and in at the
// third floor's east door; then out of the fourth floor's south door, east and up the northeast
// vine to the fifth floor's east door, where the Nut's chest is; and out of the fourth floor's west
// door, along the branch and up the west vine to the fifth floor's west door, which leads to the
// leaf that drops Isaac to Tret.
//
// Breeze, a Jupiter Djinni, waits at the east end of the highest branch, above the fifth floor's
// east door, until it has joined the party.
//
// The three vines climb like any vine: Isaac stands on the vine and holds Up or Down. Nothing
// visible happens at the vines' tops, but the game has work to do there. The stretch of upper
// branch at each vine's top is stored twice in the map: once as the branch Isaac walks on, and once
// as a hidden copy, 5 or 6 metatiles further south and as much higher, so that both are drawn on
// the same pixels. Only the hidden copy joins the vine. One metatile beside each vine moves Isaac
// onto the copy, and one on the copy moves him back (scTretTreeBranchJoinWestVine and the others).
//
// Leaves hang in front of the branches, so the scene keeps an oval round Isaac in which they are
// see-through, from when he steps out until he goes back in. Reveal's own ripple takes its place
// while Reveal lasts, and the scene puts it back afterwards.
//
// The clouds above the forest drift slowly right while the forest below moves only with the map
// (scTretTreeBranchUpdateBg3ScrollSplit).
//
// The overlay also keeps three unused hooks and a link-cable input exchange no table reaches.

#include "ability_ids.h"
#include "flags.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script.h"
#include "field/map_grid_scroll.h"
#include "graphics/display_transition.h"
#include "graphics/graphics.h"
#include "game/system_call.h"

extern SCENE_ENTRANCE TretTreeBranchEntrances[];
extern Uint32         TretTreeBranchExits[];
extern ACTOR_PLACE    TretTreeBranchActorPlacements[];
extern CUE            TretTreeBranchCues[];

// Breeze, a Jupiter Djinni (DJINNI_BREEZE), is the only placed actor. Its actor placement is gone
// once its flag (FLAG_DJINNI(DJINNI_BREEZE)) is set.
enum
{
	ACTOR_BREEZE = ACTOR_FIRST_PLACED
};

// The metatiles at the vines' tops that move Isaac between the branch he walks on and the hidden
// copy of it that joins the vine. A door's metatiles carry its exit number
// (TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTH_SOUTHWEST and the others).
typedef enum TRET_TREE_BRANCH_METATILE_BEHAVIOR
{
	// Beside the west vine's top, on the branch Isaac walks on: metatile (7, 12).
	METATILE_BEHAVIOR_TRET_JOIN_WEST_VINE       = 10,
	// On the hidden copy, east of the west vine's top: metatile (8, 17).
	METATILE_BEHAVIOR_TRET_LEAVE_WEST_VINE      = 11,
	// Above the northeast vine's top, on the branch: metatile (37, 11).
	METATILE_BEHAVIOR_TRET_JOIN_NORTHEAST_VINE  = 12,
	// On the hidden copy, west of the northeast vine's top: metatile (36, 16).
	METATILE_BEHAVIOR_TRET_LEAVE_NORTHEAST_VINE = 13,
	// West of the southeast vine's top, on the branch: metatile (41, 20).
	METATILE_BEHAVIOR_TRET_JOIN_SOUTHEAST_VINE  = 14,
	// On the hidden copy, west of the southeast vine's top: metatile (40, 26).
	METATILE_BEHAVIOR_TRET_LEAVE_SOUTHEAST_VINE = 15,
} TRET_TREE_BRANCH_METATILE_BEHAVIOR;

// Starts the clouds drifting above the forest.
static void   scTretTreeBranchStartBg3Split(void);

// The header and data of the unused input exchange (scTretTreeBranchExchangeInputPacket).
static Uint16 TransferHeader[16] ALIGNED(16);
static Uint8  TransferData[96];
// The map height at which the background splits: the clouds drift above it, and the forest far
// below lies under it.
#define BG3_SPLIT_Y 192

// The screen line where BG3 splits, and its horizontal scroll below the split (the forest) and
// above it (the drifting clouds).
static Sint32 SplitLine;
static Uint16 HofsAtOrAfterSplit;
static Uint16 HofsBeforeSplit;

// Talking to Breeze, the Jupiter Djinni at the east end of Tret's highest branch, makes it hop and
// starts the battle with it.
static void   scTretTreeBranchTalkToBreeze(void)
{
	fdRequestDjinnBattle(ACTOR_BREEZE, DJINNI_BREEZE);
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return TretTreeBranchEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return TretTreeBranchExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return TretTreeBranchActorPlacements;
}

// Moves the leader and the camera the same number of metatiles, each set on the ground of the
// metatile it lands on. Between a branch and its hidden copy the metatiles south are as much
// higher, so the picture does not change.
static void scTretTreeBranchShiftLeaderAndCameraByMetatiles(Sint32 metatileOffsetX,
                                                            Sint32 metatileOffsetZ)
{
	FIELD_STATE *state = FieldStateWork;
	ACTOR       *actor;
	Sint32       offsetX = metatileOffsetX;
	Sint32       offsetZ = metatileOffsetZ;
	FIXED        groundHeight;

	actor    = fdActorGet(MainRoutineStateWork.leaderActorId);
	offsetX *= toFIXED(16);
	offsetZ *= toFIXED(16);

	if (actor != NULL)
	{
		actor->x.fixed += offsetX;
		actor->z.fixed += offsetZ;
		groundHeight    = fdMapGridGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
		actor->y.fixed  = groundHeight;
		actor->groundHeight = groundHeight;
	}

	actor = state->camera;
	if (actor != NULL)
	{
		actor->x.fixed += offsetX;
		actor->z.fixed += offsetZ;
		groundHeight    = fdMapGridGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
		actor->y.fixed  = groundHeight;
		actor->groundHeight = groundHeight;
	}
}

// On the upper branch beside the west vine's top (metatile (7, 12)), moves Isaac onto the hidden
// copy that joins the vine, so that Down climbs him down it.
static void scTretTreeBranchJoinWestVine(void)
{
	scTretTreeBranchShiftLeaderAndCameraByMetatiles(0, 5);
}

// Stepping east off the west vine's top (metatile (8, 17)) moves Isaac back onto the drawn branch,
// the one to the fifth floor's west door.
static void scTretTreeBranchLeaveWestVine(void)
{
	scTretTreeBranchShiftLeaderAndCameraByMetatiles(0, -5);
}

// On the northeast vine's top (metatile (37, 11)), on the branch to the fifth floor's east door,
// moves Isaac onto the hidden copy that joins the vine.
static void scTretTreeBranchJoinNortheastVine(void)
{
	scTretTreeBranchShiftLeaderAndCameraByMetatiles(0, 5);
}

// Stepping west off the northeast vine's top (metatile (36, 16)) moves Isaac back onto the drawn
// branch, the one to the fifth floor's east door and Breeze.
static void scTretTreeBranchLeaveNortheastVine(void)
{
	scTretTreeBranchShiftLeaderAndCameraByMetatiles(0, -5);
}

// At the southeast vine's top (metatile (41, 20)), on the branch from the third floor's east door,
// moves Isaac onto the hidden copy that joins the vine, down which he climbs to the second floor's
// east door.
static void scTretTreeBranchJoinSoutheastVine(void)
{
	scTretTreeBranchShiftLeaderAndCameraByMetatiles(0, 6);
}

// Stepping west off the southeast vine's top (metatile (40, 26)) moves Isaac back onto the drawn
// branch to the third floor's east door.
static void scTretTreeBranchLeaveSoutheastVine(void)
{
	scTretTreeBranchShiftLeaderAndCameraByMetatiles(0, -6);
}

// Isaac walks into one of the arched doors in Tret's trunk, such as the second floor's west door at
// the end of the lowest branch (metatile (20, 26)), and goes back inside Tret Tree to the floor
// that door belongs to.
//
// A moment's push against the doorway is enough (STEP_CUE_FLAG_QUICK). The exit sound plays, and
// the door's metatile carries its own exit number, which the step cue hands on as the exit to take.
static void scTretTreeBranchEnterTretTreeRooms(void)
{
	FIELD_STATE *state = FieldStateWork;

	sySoundPlay(SE_MAP_EXIT);
	syImportScriptSetExitRequest(state->stepCue);
}

// The oval round Isaac (WINDOW_TRANSITION_LEADER) in which the leaves in front of the branches are
// see-through: inside it BG1, the leaves, is blended over the rest, and outside it every layer
// shows without the colour effect. REVEAL_WINDOW_STEPS is how far it is open.
#define REVEAL_WININ        (WININ_WIN0_ALL | WININ_WIN1_ALL)
#define REVEAL_WINOUT       (WINOUT_WIN01_ALL & ~WINOUT_WIN01_CLR)
#define REVEAL_WINDOW_STEPS 10

// Opens the see-through oval round Isaac, as the scene starts and again after Reveal.
static inline void scTretTreeBranchSetRevealWindow(SCREEN_TRANSITION_STATE *state)
{
	state->insideLayers  = REVEAL_WININ;
	state->outsideLayers = REVEAL_WINOUT;
	state->value         = REVEAL_WINDOW_STEPS;
}

// When Reveal wears off, the leaves round Isaac turn see-through again: Reveal drew its own
// rippling oval in place of the scene's, so the scene's oval and the leaves' blend are set back as
// scInit set them.
static void scTretTreeBranchRevealFades(void)
{
	fdScreenTransitionStartHeldWindow(WINDOW_TRANSITION_LEADER);

	REG_BLDCNT   = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BLDALPHA = BLEND_ALPHA(4, 12);

	scTretTreeBranchSetRevealWindow(ScreenTransitionStateWork);
}

// Returns the scene's scripts: its eight doors, the six metatiles at the vines' tops, Breeze and
// Reveal.
const CUE *scGetCues(void)
{
	return TretTreeBranchCues;
}

// Unused script hook: begins and immediately ends a script.
static void UNUSED scTretTreeBranchRunEmptyScript(void)
{
	fdScriptStart();
	fdScriptEnd();
}

// Unused: an empty function nothing refers to, the first of two here.
static void UNUSED scTretTreeBranchUnusedHookFirst(void)
{
}

// Unused: an empty function nothing refers to, the second of two here.
static void UNUSED scTretTreeBranchUnusedHookSecond(void)
{
}

// Unused exchange callback: puts held/repeated keys in the transfer header.
static void UNUSED scTretTreeBranchExchangeInputPacket(void)
{
	Uint16 *header = TransferHeader;

	header[0]      = PerData;
	header[1]      = PerRepeat;
	sySerialPollAndTransfer(header, TransferData);
}

// Opens Tret's branches with a backdrop fade, opens the see-through oval in the leaves round Isaac
// and starts the clouds drifting.
Sint32 scInit(void)
{
	SCREEN_TRANSITION_STATE *state;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

	fdScreenTransitionStartHeldWindow(WINDOW_TRANSITION_LEADER);

	REG_BLDCNT   = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BLDALPHA = BLEND_ALPHA(4, 12);

	state        = ScreenTransitionStateWork;
	scTretTreeBranchSetRevealWindow(state);

	scTretTreeBranchStartBg3Split();
	return 0;
}

// At each scanline, picks the background's horizontal scroll: the drifting clouds' above the split,
// the map's below it.
static void scTretTreeBranchApplyBg3ScrollAtScanline(void)
{
	Uint16 *scrollOffset;
	Uint32  horizontalScroll;

	if (REG_VCOUNT >= SplitLine)
	{
		scrollOffset = &HofsAtOrAfterSplit;
	}
	else
	{
		scrollOffset = &HofsBeforeSplit;
	}
	horizontalScroll = *scrollOffset;
	REG_BG3HOFS      = horizontalScroll;
}

// Each tick, moves the split with the map's vertical scroll, so that it stays at BG3_SPLIT_Y on the
// map. The clouds above it lag the map by one more pixel every four ticks, so they drift slowly
// right.
static void scTretTreeBranchUpdateBg3ScrollSplit(void)
{
	MAP_GRID_LAYER_SCROLL *layer =
		&((MAP_GRID_SCROLL_STATE *)MapGridState[0])->layers[MAPGRID_LAYER_GROUND];
	Sint32 horizontalOffset;

	SplitLine          = BG3_SPLIT_Y - (Sint16)(FIXEDtoINT(layer->y));
	HofsAtOrAfterSplit = horizontalOffset = (Sint16)(FIXEDtoINT(layer->x));
	HofsBeforeSplit                       = horizontalOffset - (TickCount >> 2);
}

// Starts the clouds drifting: the split is chosen at every scanline and moved every tick.
static void scTretTreeBranchStartBg3Split(void)
{
	syIntFunction(IRQ_HBLANK, 0, scTretTreeBranchApplyBg3ScrollAtScanline);
	sySetTickEvent(scTretTreeBranchUpdateBg3ScrollSplit, EVENT_PASS_TICK);
}

SCENE_ENTRANCE TretTreeBranchEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 100, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTH_SOUTHWEST,
     CONDITION_ALWAYS, 328,
     0, 424,
     FACING_WEST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTH_SOUTHEAST,
     CONDITION_ALWAYS, 424,
     0, 440,
     FACING_EAST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTH,
     CONDITION_ALWAYS, 376,
     0, 360,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTHEAST,
     CONDITION_ALWAYS, 424,
     0, 328,
     FACING_EAST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTH,
     CONDITION_ALWAYS, 376,
     0, 264,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTHWEST,
     CONDITION_ALWAYS, 328,
     0, 248,
     FACING_WEST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTH_NORTHWEST,
     CONDITION_ALWAYS, 328,
     0, 152,
     FACING_WEST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTH_NORTHEAST,
     CONDITION_ALWAYS, 424,
     0, 136,
     FACING_EAST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SCENE_TABLE_END},
};

Uint32 TretTreeBranchExits[] = {
	SCENE_EXITS(TretTreeBranch),
	SCENE_EXIT(TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTH_SOUTHWEST,
               TreeRooms,
               TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTH_SOUTHWEST),
	SCENE_EXIT(TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTH_SOUTHEAST,
               TreeRooms,
               TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTH_SOUTHEAST),
	SCENE_EXIT(
		TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTH, TreeRooms, TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTH),
	SCENE_EXIT(TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTHEAST,
               TreeRooms,
               TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTHEAST),
	SCENE_EXIT(
		TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTH, TreeRooms, TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTH),
	SCENE_EXIT(TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTHWEST,
               TreeRooms,
               TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTHWEST),
	SCENE_EXIT(TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTH_NORTHWEST,
               TreeRooms,
               TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTH_NORTHWEST),
	SCENE_EXIT(TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTH_NORTHEAST,
               TreeRooms,
               TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTH_NORTHEAST),
	SCENE_EXITS_END,
};

// clang-format off
ACTOR_PLACE TretTreeBranchActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_BREEZE), MOVEMENT_TYPE_STAND,
		648, 0, 88, FACING_WEST, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};
// clang-format on

CUE TretTreeBranchCues[] = {
	{STEP_CUE_WALKING | STEP_CUE_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTH_SOUTHWEST, CONDITION_ALWAYS,
     SCRIPT_REF(scTretTreeBranchEnterTretTreeRooms)},
	{STEP_CUE_WALKING | STEP_CUE_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTH_SOUTHEAST, CONDITION_ALWAYS,
     SCRIPT_REF(scTretTreeBranchEnterTretTreeRooms)},
	{STEP_CUE_WALKING | STEP_CUE_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTH, CONDITION_ALWAYS,
     SCRIPT_REF(scTretTreeBranchEnterTretTreeRooms)},
	{STEP_CUE_WALKING | STEP_CUE_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTHEAST, CONDITION_ALWAYS,
     SCRIPT_REF(scTretTreeBranchEnterTretTreeRooms)},
	{STEP_CUE_WALKING | STEP_CUE_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTH, CONDITION_ALWAYS,
     SCRIPT_REF(scTretTreeBranchEnterTretTreeRooms)},
	{STEP_CUE_WALKING | STEP_CUE_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTHWEST, CONDITION_ALWAYS,
     SCRIPT_REF(scTretTreeBranchEnterTretTreeRooms)},
	{STEP_CUE_WALKING | STEP_CUE_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTH_NORTHWEST, CONDITION_ALWAYS,
     SCRIPT_REF(scTretTreeBranchEnterTretTreeRooms)},
	{STEP_CUE_WALKING | STEP_CUE_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTH_NORTHEAST, CONDITION_ALWAYS,
     SCRIPT_REF(scTretTreeBranchEnterTretTreeRooms)},
	{CUE_STEP,
     METATILE_BEHAVIOR_TRET_JOIN_WEST_VINE, CONDITION_ALWAYS,
     SCRIPT_REF(scTretTreeBranchJoinWestVine)},
	{CUE_STEP,
     METATILE_BEHAVIOR_TRET_LEAVE_WEST_VINE, CONDITION_ALWAYS,
     SCRIPT_REF(scTretTreeBranchLeaveWestVine)},
	{CUE_STEP,
     METATILE_BEHAVIOR_TRET_JOIN_NORTHEAST_VINE, CONDITION_ALWAYS,
     SCRIPT_REF(scTretTreeBranchJoinNortheastVine)},
	{CUE_STEP,
     METATILE_BEHAVIOR_TRET_LEAVE_NORTHEAST_VINE, CONDITION_ALWAYS,
     SCRIPT_REF(scTretTreeBranchLeaveNortheastVine)},
	{CUE_STEP,
     METATILE_BEHAVIOR_TRET_JOIN_SOUTHEAST_VINE, CONDITION_ALWAYS,
     SCRIPT_REF(scTretTreeBranchJoinSoutheastVine)},
	{CUE_STEP,
     METATILE_BEHAVIOR_TRET_LEAVE_SOUTHEAST_VINE, CONDITION_ALWAYS,
     SCRIPT_REF(scTretTreeBranchLeaveSoutheastVine)},
	{CUE_TALK, ACTOR_BREEZE, CONDITION_ALWAYS, SCRIPT_REF(scTretTreeBranchTalkToBreeze)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_AFTER_END),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scTretTreeBranchRevealFades)},
	{CUE_TABLE_END},
};

// Unused: the two steps of a door opening, as Vale's doors have them
// (ValePrologueSanctumDoorOpens). Nothing on the branch plays them.
Uint16 TretTreeBranchUnusedMetatileSteps[] = {
	MAPGRID_ANIMATION_STEP(0, 96, 2, 2, 2),
	MAPGRID_ANIMATION_STEP(2, 96, 2, 2, 2),
	MAPGRID_ANIMATION_STEPS_END,
};
