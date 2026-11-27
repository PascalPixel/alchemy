// The Izumo Ruins, the grey caves under the ring of giant stones in Izumo, reached by the ladder
// Reveal shows in the ring's centre. They are two scenes that share this overlay
// (SceneIdIzumoRuinsFirstRoom, SceneIdIzumoRuinsSecondRoom), each of several rooms.
//
// In the first room's south part, where the ladder comes down, two wooden stakes stand between two
// puddles across the floor, with patches of sand beside them: Sand takes Felix through the sand
// (scIzumoRuinsSinkIntoSand), Frost freezes the puddles into pillars of ice
// (scIzumoRuinsFreezePuddle) and Pound drives the stakes down (scIzumoRuinsPoundStake). Its north
// part has a third stake by a strip of sand and a stone pillar Move pulls along
// (scIzumoRuinsFinishMovingPillar), and grey pillars crumble under a second step
// (shared/crumbling_pillar.h). The second scene, a floor down, holds the Phantasmal Mail, two more
// stakes and a green pool: Parch dries the pool (scIzumoRuinsParchThePool) and opens the way to the
// room of the stone tablet that teaches Ulysses, which stands in a ring of six stone pillars.

#include "field/field_mode.h"
#include "field/script_inline.h"
#include "scene/scene.h"
#include "shared/trial_road.h"
#include "game/game.h"
#include "field/call.h"
#include "field/actor_script.h"

void                fdPoundSinkTarget(Uint16 *list, Sint32 mode, Sint32 count);
void                scIzumoRuinsShowStakeState(ACTOR *actor);

extern Sint16       IzumoRuinsFirstRoomPillars[];
extern Sint16       IzumoRuinsSecondRoomPillars[];
extern Sint16       IzumoRuinsFirstRoomStakes[];
extern Sint16       IzumoRuinsSecondRoomStakes[];
extern Sint16       IzumoRuinsPuddles[];
extern SCENE_REGION IzumoRuinsFirstRoomRegions[];
extern SCENE_REGION IzumoRuinsSecondRoomRegions[];
extern ACTOR_PLACE  IzumoRuinsActorPlacements[];
extern ACTOR_PLACE  IzumoRuinsFirstRoomActorPlacements[];
extern ACTOR_PLACE  IzumoRuinsSecondRoomActorPlacements[];
extern CUE          IzumoRuinsFirstRoomCues[];
extern CUE          IzumoRuinsSecondRoomCues[];

void                scIzumoRuinsPlaceStakes(Uint16 *list);
void                scDustScatter(Sint32 mode);
void                scDustBurst(void);
Sint32              fdMapGetTerrainHeight(Sint32 layer, Sint32 x, Sint32 z);
void                fdLeaderSetMetatileShapeAt(Sint32 level, Sint32 layer);

void                fdActorSetMetatileOpen(Sint32 id, Uint32 kind, Sint32 flag, Sint32 layer);

#include "shared/crumbling_pillar.h"

#include "../field/field_effect_advance_motion_priority.c"

#include "shared/dust.h"

#include "shared/actor_set_metatile_shape.h"

#include "shared/leader_set_metatile_shape.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE   fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

extern Uint32 FieldEffectShortScript[];
extern Uint32 FieldEffectMediumScript[];
extern Uint32 FieldEffectLongScript[];

// Set as Parch dries the green pool on the way to the room of the stone tablet.
#define FLAG_IZUMO_RUINS_POOL_PARCHED 0x941

// The green pool's metatiles, which answer Parch.
enum
{
	METATILE_POOL = 50
};

// Both scenes' first two actors: wooden stakes Pound drives down (IzumoRuinsFirstRoomStakes,
// IzumoRuinsSecondRoomStakes), at metatiles (10, 51) and (12, 51) in the first and (27, 34) and
// (25, 34) in the second.
enum
{
	ACTOR_FIRST_STAKE = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_STAKE
};

// The first scene's other actors: the puddles Frost freezes, west and east of its first two stakes
// (metatiles (8, 51) and (14, 51)), a third stake in the north part by the strip of sand (metatile
// (43, 15)), and the stone pillar Move pulls, at the north end (metatile (32, 5)).
enum
{
	ACTOR_WEST_PUDDLE = ACTOR_FIRST_PLACED + 2,
	ACTOR_EAST_PUDDLE,
	ACTOR_THIRD_STAKE,
	ACTOR_FIRST_ROOM_PILLAR
};

// The second scene's six stone pillars Move pushes, in a ring round the stone tablet in its room in
// the east.
enum
{
	ACTOR_RING_NORTHWEST_PILLAR = ACTOR_FIRST_PLACED + 2,
	ACTOR_RING_NORTHEAST_PILLAR,
	ACTOR_RING_WEST_PILLAR,
	ACTOR_RING_SOUTHWEST_PILLAR,
	ACTOR_RING_SOUTHEAST_PILLAR,
	ACTOR_RING_EAST_PILLAR
};

// Set while Pound has driven a stake into the ground: each scene's first and second, and the first
// scene's third.
#define FLAG_FIRST_STAKE_POUNDED  FLAG_SCENE_TEMP_00
#define FLAG_SECOND_STAKE_POUNDED FLAG_SCENE_TEMP_01
#define FLAG_THIRD_STAKE_POUNDED  FLAG_SCENE_TEMP_02
// Set once Frost has frozen a puddle of the first scene.
#define FLAG_WEST_PUDDLE_FROZEN   FLAG_SCENE_TEMP_03
#define FLAG_EAST_PUDDLE_FROZEN   FLAG_SCENE_TEMP_04

// The metatile between the ring's two south pillars, south of the stone tablet: kept a wall while
// Move carries a pillar, so that none is set down there.
#define RING_SOUTH_GAP_COLUMN     50
#define RING_SOUTH_GAP_ROW        13

// How far a standing stake's top rises over the ground under it, in height steps of 8 pixels.
#define STAKE_HEIGHT_STEPS        4

#include "../field/map_metatile_get.c"

#include "shared/map_metatile_set.c"

// The dried bed of the green pool, copied over it from elsewhere on the map: first where Felix can
// walk, then its pictures.
#define scIzumoRuinsShowThePoolDried()                                                             \
	(fdMapGridCopyMetatileAttributes(7, 2, 8, 7, 7, 12),                                           \
	 fdMapGridCopyMetatilesTo(7, 66, 7, 76, 8, 9))

// Fits the metatile under a stake to whether it stands or is driven down (its flag, kept in its
// timer). Standing, its top is a step STAKE_HEIGHT_STEPS above the floor that Felix jumps onto and
// across, and under Sand it blocks the way unless the floor round it is sand. Driven down, the
// metatile is floor again, closed to Felix under the sand. A driven stake is drawn behind whoever
// walks over it.
void scIzumoRuinsShowStakeState(ACTOR *actor)
{
	Sint32 column = FIELD_METATILE(actor->x.fixed);
	Sint32 row    = FIELD_METATILE(actor->z.fixed);
	Sint32 lower  = fdMapMetatileGetAttributes(MAPGRID_LAYER_GROUND, column, row);
	Sint32 upper  = fdMapMetatileGetAttributes(MAPGRID_LAYER_OVERHEAD, column, row);
	Sint32 height =
		METATILE_HEIGHT_STEPS(fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed));

	if (fdScriptGetFlag(actor->timer))
	{
		upper |= METATILE_ATTRIBUTES(METATILE_BEHAVIOR_WALL, 0);
		lower &= ~MAP_METATILE_JUMP;
		if (METATILE_HEIGHT_STEPS(actor->groundHeight) != height)
			height -= STAKE_HEIGHT_STEPS;
		fdSetActorSpritePriority(actor, 3);
	}
	else
	{
		if (METATILE_CODE(lower) != METATILE_BEHAVIOR_SAND)
			upper = METATILE_ATTRIBUTES(METATILE_BEHAVIOR_WALL, upper & METATILE_ATTRIBUTE_BITS);
		else
			upper = METATILE_ATTRIBUTES(METATILE_BEHAVIOR_SAND, upper & METATILE_ATTRIBUTE_BITS);
		lower  |= MAP_METATILE_JUMP;
		height += STAKE_HEIGHT_STEPS;
		fdSetActorSpritePriority(actor, 2);
	}
	scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, column, row, lower);
	scMapMetatileSetAttributes(MAPGRID_LAYER_OVERHEAD, column, row, upper);
	fdMapGridSetMetatileHeight(
		FIELD_METATILE_ORIGIN(column), FIELD_METATILE_ORIGIN(row), actor->layer, height);
}

// Places Pound's stakes from a list of rows (actor, metatile kind, flag), ended by -1: each
// standing, or driven down if its flag is set.
void scIzumoRuinsPlaceStakes(Uint16 *list)
{
	ACTOR *actor;
	Sint32 flag;

	for (; list[0] != ACTOR_LIST_END; list += 3)
	{
		actor = fdActorGet(list[0]);
		flag  = (Sint16)list[2];
		fdActorSetSpriteFlags(actor, 0);
		fdActorSelectAnimation(actor, fdScriptGetFlag((Uint16)flag) + 1);
		actor->motionFlags     = 0;
		actor->collisionFlags &= ~ACTOR_SOLID;
		actor->timer           = flag;
		scIzumoRuinsShowStakeState(actor);
	}
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if (scene == SCENE_IZUMO_RUINS_FIRST_ROOM)
		return IzumoRuinsFirstRoomRegions;
	if (scene == SCENE_IZUMO_RUINS_SECOND_ROOM)
		return IzumoRuinsSecondRoomRegions;
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if (scene == SCENE_IZUMO_RUINS_FIRST_ROOM)
		return IzumoRuinsFirstRoomActorPlacements;
	if (scene == SCENE_IZUMO_RUINS_SECOND_ROOM)
		return IzumoRuinsSecondRoomActorPlacements;
	return IzumoRuinsActorPlacements;
}

// Pound on one of the ruins' wooden stakes: the giant fist hammers it down flush with the floor, so
// that it is no longer a step to jump on. The stakes are the pair between the puddles where the
// ladder comes down, the one by the strip of sand in the first scene's north part, and the pair in
// the second scene.
//
// fdPoundSinkTarget animates the blow from the scene's list of stakes; as the stake lands (mode 1)
// scIzumoRuinsShowStakeState makes its metatile floor.
void scIzumoRuinsPoundStake(Sint32 mode, Sint32 frame)
{
	FIELD_EFFECT_STATE *state = RamHeapSlots->fieldEffectState;
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	Sint16              scene;

	scene = party->next;
	if (scene == SCENE_IZUMO_RUINS_FIRST_ROOM)
		fdPoundSinkTarget(IzumoRuinsFirstRoomStakes, mode, frame);
	else
		fdPoundSinkTarget(IzumoRuinsSecondRoomStakes, mode, frame);
	if (mode == POUND_STAGE_LANDED)
		scIzumoRuinsShowStakeState(fdActorGet(state->targetId));
}

// Frost on one of the two puddles either side of the stakes where the ladder comes down (metatiles
// (8, 51) and (14, 51)) freezes it into a pillar of ice that Felix can jump onto.
void scIzumoRuinsFreezePuddle(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR       *actor;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdFlagBlockApplyFlags(IzumoRuinsPuddles);
	actor                 = state->movedBlock;
	actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
	actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
	fdScriptEnd();
}

// Casting Sand while standing on one of the ruins' sand patches takes Felix into the sand: he moves
// onto the metatiles of the map's overhead layer, which mark the sand, and can travel only through
// sand, under the stakes and past what blocks the floor above. Cast off the sand, nothing changes.
void scIzumoRuinsSinkIntoSand(void)
{
	ACTOR *leader   = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 metatile = fdMapMetatileGetAttributes(
		MAPGRID_LAYER_OVERHEAD, FIELD_METATILE(leader->x.fixed), FIELD_METATILE(leader->z.fixed));

	if (METATILE_CODE(metatile) == METATILE_BEHAVIOR_SAND)
		leader->layer = MAPGRID_LAYER_OVERHEAD;
}

// As Sand wears off, Felix comes up out of the sand onto the ruins' floor, the metatiles of the
// map's ground layer.
void scIzumoRuinsRiseOutOfSand(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	leader->layer = MAPGRID_LAYER_GROUND;
}

// Move (or a push) takes hold of a stone pillar: the one at the north end of the first scene, or
// one of the six round the stone tablet. Round the tablet, the metatile south of it between the
// ring's two south pillars is first closed, so that no pillar can be set down in the way to it.
void scIzumoRuinsStartMovingPillar(void)
{
	if (MainRoutineStateWork.next == SCENE_IZUMO_RUINS_FIRST_ROOM)
	{
		fdActorLiftTargetBlock(IzumoRuinsFirstRoomPillars);
	}
	else
	{
		Sint32 metatile = fdMapMetatileGetAttributes(
			MAPGRID_LAYER_OVERHEAD, RING_SOUTH_GAP_COLUMN, RING_SOUTH_GAP_ROW);

		metatile = METATILE_ATTRIBUTES(METATILE_BEHAVIOR_WALL, metatile & METATILE_ATTRIBUTE_BITS);
		scMapMetatileSetAttributes(
			MAPGRID_LAYER_OVERHEAD, RING_SOUTH_GAP_COLUMN, RING_SOUTH_GAP_ROW, metatile);
		fdActorLiftTargetBlock(IzumoRuinsSecondRoomPillars);
	}
}

// Move sets a stone pillar down; round the stone tablet the way in from the south is open floor
// again.
void scIzumoRuinsFinishMovingPillar(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorPlaceMovedBlock();
	if (MainRoutineStateWork.next == SCENE_IZUMO_RUINS_SECOND_ROOM)
		scMapMetatileSetAttributes(MAPGRID_LAYER_OVERHEAD,
		                           RING_SOUTH_GAP_COLUMN,
		                           RING_SOUTH_GAP_ROW,
		                           fdMapMetatileGetAttributes(MAPGRID_LAYER_OVERHEAD,
		                                                      RING_SOUTH_GAP_COLUMN,
		                                                      RING_SOUTH_GAP_ROW)
		                               & METATILE_ATTRIBUTE_BITS);
	fdScriptEnd();
}

// Parch cast at the green pool in the west of the second scene (metatiles (8,
// 14) to (14, 21)) dries it for good: the water is gone, and its bed is floor
// that leads on towards the room of the stone tablet.
void scIzumoRuinsParchThePool(void)
{
	fdScriptStart();
	scIzumoRuinsShowThePoolDried();
	fdScriptSetFlag(FLAG_IZUMO_RUINS_POOL_PARCHED);
	fdScriptEnd();
}

const CUE *scGetCues(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if (scene == SCENE_IZUMO_RUINS_FIRST_ROOM)
		return IzumoRuinsFirstRoomCues;
	return IzumoRuinsSecondRoomCues;
}

// Sets up the scene being entered: the window in, its stone pillars and stakes, the first scene's
// puddles, and the second scene's pool dried if Parch has dried it. Returns 0.
Sint32 scInit(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR       *actor;
	Sint32       height;

	state->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdActorGet(MainRoutineStateWork.leaderActorId)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
	fdEffectStartBlankOverlay(0);
	if (MainRoutineStateWork.next == SCENE_IZUMO_RUINS_FIRST_ROOM)
	{
		fdActorPlaceBlocks(IzumoRuinsFirstRoomPillars);
		scIzumoRuinsPlaceStakes((Uint16 *)IzumoRuinsFirstRoomStakes);
		fdFlagBlockPlaceAll(IzumoRuinsPuddles);
	}
	else
	{
		fdActorGet(ACTOR_RING_NORTHWEST_PILLAR)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
		fdActorGet(ACTOR_RING_NORTHEAST_PILLAR)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
		fdActorPlaceBlocks(IzumoRuinsSecondRoomPillars);
		scIzumoRuinsPlaceStakes((Uint16 *)IzumoRuinsSecondRoomStakes);
		if (fdScriptGetFlag(FLAG_IZUMO_RUINS_POOL_PARCHED))
		{
			scIzumoRuinsShowThePoolDried();
			fdMapGridCenterOnFollowedPoint();
			syWait(1);
			if (fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
			{
				actor  = fdActorGet(MainRoutineStateWork.leaderActorId);
				height = fdMapGetTerrainHeight(
					actor->layer, actor->x.fixed, actor->z.fixed - actor->y.fixed);
				actor->groundHeight = height;
				actor->y.fixed      = height;
				syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 0);
			}
		}
	}
	return 0;
}

// Nothing to prepare before the map is built. Returns 0.
Sint32 scPrepareMap(void)
{
	return 0;
}

// clang-format off
#include "shared/dust_scatter_script.c"

#include "shared/dust_burst_script.c"

#include "shared/field_effect_duration_scripts.c"

// The stone pillars Move can push in each scene, ended by -1.
Sint16 IzumoRuinsFirstRoomPillars[] = {
	ACTOR_FIRST_ROOM_PILLAR, -1,
};
Sint16 IzumoRuinsSecondRoomPillars[] = {
	ACTOR_RING_NORTHWEST_PILLAR, ACTOR_RING_NORTHEAST_PILLAR,
	ACTOR_RING_WEST_PILLAR, ACTOR_RING_SOUTHWEST_PILLAR,
	ACTOR_RING_SOUTHEAST_PILLAR, ACTOR_RING_EAST_PILLAR,
	-1, 0,
};

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

// Pound's stakes in each scene (scIzumoRuinsPlaceStakes, fdPoundSinkTarget): actor, the metatile kind Pound opens under it, and the flag set while it is driven down; ended by -1.
Sint16 IzumoRuinsFirstRoomStakes[] = {
	ACTOR_FIRST_STAKE, 0, FLAG_FIRST_STAKE_POUNDED,
	ACTOR_SECOND_STAKE, 0, FLAG_SECOND_STAKE_POUNDED,
	ACTOR_THIRD_STAKE, 0, FLAG_THIRD_STAKE_POUNDED,
	-1,
};
Sint16 IzumoRuinsSecondRoomStakes[] = {
	ACTOR_FIRST_STAKE, 0, FLAG_FIRST_STAKE_POUNDED,
	ACTOR_SECOND_STAKE, 0, FLAG_SECOND_STAKE_POUNDED,
	-1,
};

// The first scene's puddles Frost freezes: actor and the flag set once frozen, ended by -1.
Sint16 IzumoRuinsPuddles[] = {
	ACTOR_WEST_PUDDLE, FLAG_WEST_PUDDLE_FROZEN,
	ACTOR_EAST_PUDDLE, FLAG_EAST_PUDDLE_FROZEN,
	-1,
};

#include "shared/scene_entrances_default.c"

// The top of the ladder up out of each scene, which takes Felix out by its exit: up to Izumo from the first, up to the first from the second.
SCENE_REGION IzumoRuinsFirstRoomRegions[] = {
	{208, 44, 752, 224, 60, 768, CONDITION_ALWAYS,
	 IZUMO_RUINS_FIRST_ROOM_TO_IZUMO},
	{SCENE_TABLE_END},
};

SCENE_REGION IzumoRuinsSecondRoomRegions[] = {
	{400, 16, 352, 416, 32, 368, CONDITION_ALWAYS,
	 IZUMO_RUINS_SECOND_ROOM_TO_IZUMO_RUINS_FIRST_ROOM},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(IzumoRuinsFirstRoom),
	SCENE_EXIT_UNLESS(IZUMO_RUINS_FIRST_ROOM_TO_IZUMO, Izumo,
					  IZUMO_FROM_IZUMO_RUINS_FIRST_ROOM, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(2, IzumoRuinsFirstRoom, 3, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(3, IzumoRuinsFirstRoom, 2, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(IZUMO_RUINS_FIRST_ROOM_TO_IZUMO_RUINS_SECOND_ROOM,
					  IzumoRuinsSecondRoom,
					  IZUMO_RUINS_SECOND_ROOM_FROM_IZUMO_RUINS_FIRST_ROOM,
					  EXIT_FLAG_NONE),
	SCENE_EXITS(IzumoRuinsSecondRoom),
	SCENE_EXIT_UNLESS(IZUMO_RUINS_SECOND_ROOM_TO_IZUMO_RUINS_FIRST_ROOM,
					  IzumoRuinsFirstRoom,
					  IZUMO_RUINS_FIRST_ROOM_FROM_IZUMO_RUINS_SECOND_ROOM,
					  EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(2, IzumoRuinsSecondRoom, 3, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(3, IzumoRuinsSecondRoom, 2, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE IzumoRuinsActorPlacements[] = {
	{SCENE_TABLE_END},
};

ACTOR_PLACE IzumoRuinsFirstRoomActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 824, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 824, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 824, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 824, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 88, FACING_NORTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE IzumoRuinsSecondRoomActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 552, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 552, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 216, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 216, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 856, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};
// clang-format on

CUE IzumoRuinsFirstRoomCues[] = {
	{CUE_EXIT, IZUMO_RUINS_FIRST_ROOM_TO_IZUMO, CONDITION_ALWAYS, IZUMO_RUINS_FIRST_ROOM_TO_IZUMO},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT,
     IZUMO_RUINS_FIRST_ROOM_TO_IZUMO_RUINS_SECOND_ROOM, CONDITION_ALWAYS,
     IZUMO_RUINS_FIRST_ROOM_TO_IZUMO_RUINS_SECOND_ROOM},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_FIRST_STAKE, FLAG_FIRST_STAKE_POUNDED,
     SCRIPT_REF(scIzumoRuinsPoundStake)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_SECOND_STAKE, FLAG_SECOND_STAKE_POUNDED,
     SCRIPT_REF(scIzumoRuinsPoundStake)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_THIRD_STAKE, FLAG_THIRD_STAKE_POUNDED,
     SCRIPT_REF(scIzumoRuinsPoundStake)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST),
     ACTOR_WEST_PUDDLE, FLAG_WEST_PUDDLE_FROZEN,
     SCRIPT_REF(scIzumoRuinsFreezePuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST),
     ACTOR_EAST_PUDDLE, FLAG_EAST_PUDDLE_FROZEN,
     SCRIPT_REF(scIzumoRuinsFreezePuddle)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsSinkIntoSand)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsRiseOutOfSand)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FIRST_ROOM_PILLAR, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsStartMovingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_FIRST_ROOM_PILLAR, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsFinishMovingPillar)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scIzumoRuinsStartMovingPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scIzumoRuinsFinishMovingPillar)},
	{CUE_TABLE_END},
};

CUE IzumoRuinsSecondRoomCues[] = {
	{CUE_EXIT,
     IZUMO_RUINS_SECOND_ROOM_TO_IZUMO_RUINS_FIRST_ROOM, CONDITION_ALWAYS,
     IZUMO_RUINS_SECOND_ROOM_TO_IZUMO_RUINS_FIRST_ROOM},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_FIRST_STAKE, FLAG_FIRST_STAKE_POUNDED,
     SCRIPT_REF(scIzumoRuinsPoundStake)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_SECOND_STAKE, FLAG_SECOND_STAKE_POUNDED,
     SCRIPT_REF(scIzumoRuinsPoundStake)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
     METATILE_POOL, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsParchThePool)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_RING_NORTHWEST_PILLAR, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsStartMovingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_RING_NORTHWEST_PILLAR, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsFinishMovingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_RING_NORTHEAST_PILLAR, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsStartMovingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_RING_NORTHEAST_PILLAR, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsFinishMovingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_RING_WEST_PILLAR, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsStartMovingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_RING_WEST_PILLAR, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsFinishMovingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_RING_SOUTHWEST_PILLAR, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsStartMovingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_RING_SOUTHWEST_PILLAR, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsFinishMovingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_RING_SOUTHEAST_PILLAR, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsStartMovingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_RING_SOUTHEAST_PILLAR, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsFinishMovingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_RING_EAST_PILLAR, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsStartMovingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_RING_EAST_PILLAR, CONDITION_ALWAYS,
     SCRIPT_REF(scIzumoRuinsFinishMovingPillar)},
	{CUE_TABLE_END},
};
