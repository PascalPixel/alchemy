/* @file */
/* Magma Rock's outside: the climb up the mountain across the river from */
/* Naribwe, past yellow statues that answer Burst, to the great fire on its */
/* top that hides the way down into it. Felix comes here for a Magma Ball to */
/* load Loho's cannon. */
/* */
/* At the foot (the slide, `SceneId_MagmaRockSlide`) Lift moves a boulder off */
/* the path, and the cave mouth above it, between two statues throwing a */
/* fireball back and forth, leads a little way inside. Felix then climbs the */
/* wall by the cave mouth. Burst on the statue on the ledge above makes it */
/* spit a fireball that breaks a cracked pillar; pushing two stone pillars */
/* opens the climb, and Burst on the statue further west makes its cap rise */
/* on a column of flame, carrying Felix up to the next ledge. */
/* */
/* The cliff above (`SceneId_MagmaRockSalamanderTailChest`, with the Salamander
 */
/* Tail's chest) is four screens on one map, which the code calls levels and */
/* tells apart by the entrance Felix came in by: the low east and low west */
/* halves of the cliff, then the high west and high east halves above them. */
/* Growth grows vines up its walls, Lash ties a rope, two cracked pillars */
/* crumble under Felix's feet, more statues spit fire or lift him, and on the */
/* high east wall three pairs of statues throw fireballs that knock him off it.
 */
/* At the top (`SceneId_MagmaRockStatue`) Burst on the statue east of the great
 */
/* fire makes it spit a fireball into the fire, and the blast opens the way */
/* into Magma Rock. */
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "field/field_sequence.h"
#include "field/event_inline.h"
#include "field/map_grid.h"
#include "scene/magma_rock.h"
#include "game/game.h"
#include "macros.h"
#include "field/call.h"
#include "scene/falling_rocks.h"
#include "scene/mars_lighthouse_star_room.h"
#include "field/field_psynergy_state.h"
#include "field/actor_script.h"

/* Each pushed pillar's flag keeps it where it was pushed: two on the slide, */
/* one on each of the cliff's high west, high east and low west levels and two
 */
/* on its low east level. */
#define FLAG_SLIDE_PILLAR_A_PUSHED	   0x8d5
#define FLAG_SLIDE_PILLAR_B_PUSHED	   0x8d6
#define FLAG_HIGH_WEST_PILLAR_B_PUSHED 0x8d7
#define FLAG_HIGH_EAST_PILLAR_PUSHED   0x8d8
#define FLAG_LOW_WEST_PILLAR_PUSHED	   0x8d9
#define FLAG_LOW_EAST_PILLAR_A_PUSHED  0x8da
#define FLAG_LOW_EAST_PILLAR_B_PUSHED  0x8db
/* The top's great fire is put out and the rocks round it scattered, */
/* which opens the way into Magma Rock (scMagmaRockScatterEmbers). */
#define FLAG_WAY_IN_BLASTED 0x8dc
/* Set as Burst breaks each cracked pillar, until the scene is left. */
#define FLAG_CRACKED_PILLAR_A_BURST FLAG_SCENE_TEMP_00
#define FLAG_CRACKED_PILLAR_B_BURST FLAG_SCENE_TEMP_01
#define FLAG_CRACKED_PILLAR_C_BURST FLAG_SCENE_TEMP_02
#define FLAG_CRACKED_PILLAR_D_BURST FLAG_SCENE_TEMP_03
#define FLAG_CRACKED_PILLAR_E_BURST FLAG_SCENE_TEMP_04
/* The high west level's crumbling pillars: each one's flag is set once it has
 */
/* cracked under the leader, and the flag after it once it has fallen. The */
/* first's is FLAG_CRACKED_PILLAR_A_BURST's too. */
#define FLAG_CRUMBLING_PILLAR_A_CRACKED FLAG_SCENE_TEMP_00
#define FLAG_CRUMBLING_PILLAR_B_CRACKED FLAG_SCENE_TEMP_02
/* Set once Felix has been on the cave mouth's cell (scMagmaRockEnterCave), so
 */
/* that when the game moves him onto it he is not sent straight in; his next */
/* step there by himself clears it (scMagmaRockResetCaveDoor). */
#define FLAG_CAVE_DOOR_USED FLAG_SCENE_TEMP_F8
/* Set while Felix stands just behind the high west level's statue, the one */
/* place Burst can wake it from (scMagmaRockReadyHighWestStatue). */
#define FLAG_HIGH_WEST_STATUE_READY FLAG_SCENE_TEMP_12
/* Set while a fireball knocks Felix off the high east wall */
/* (scMagmaRockKnockLeaderOffWall); the fireballs stop whooshing meanwhile. */
#define FLAG_LEADER_KNOCKED_DOWN FLAG_SCENE_TEMP_60
/* The cliff's low east level: each vine Growth has grown there. */
#define FLAG_VINE_A_GROWN FLAG_SCENE_TEMP_50
#define FLAG_VINE_B_GROWN FLAG_SCENE_TEMP_51
#define FLAG_VINE_C_GROWN FLAG_SCENE_TEMP_52
/* Its low west level: the rope Lash ties between the two posts. */
#define FLAG_LOW_WEST_ROPE_LASHED FLAG_SCENE_TEMP_50

/* The cliff's four levels, by the last entrance of each: its high west and */
/* high east halves, then the low west and low east halves below them. They */
/* share one map, but to the player each is a screen of its own, with its own */
/* placed actors and triggers, left by climbing a wall off its top or foot. */
/* From the slide the party climbs onto the low east level, and the high east */
/* level's wall leads up to the top. */
typedef enum MAGMA_ROCK_SALAMANDER_TAIL_LEVEL {
	SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE = 3,
	SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE = 8,
	SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE = 12
} MAGMA_ROCK_SALAMANDER_TAIL_LEVEL;

/* The cliff's entrances from within the scene, by the level each opens onto */
/* and the level it leads from; scene_exits.h names those from the slide and */
/* the top. Two walls lead up from the low west level to the high west level, A
 */
/* the western, and one from the low east level to the high east level; a path
 */
/* leads from the high west level to the high east, and two from the low west */
/* level to the low east. */
typedef enum MAGMA_ROCK_SALAMANDER_TAIL_ENTRANCE {
	ENTRANCE_HIGH_WEST_FROM_BELOW_A = 1,
	ENTRANCE_HIGH_WEST_FROM_BELOW_B,
	ENTRANCE_HIGH_WEST_FROM_HIGH_EAST,
	ENTRANCE_HIGH_EAST_FROM_HIGH_WEST,
	ENTRANCE_HIGH_EAST_FROM_BELOW = 7,
	ENTRANCE_LOW_WEST_FROM_ABOVE_A = 9,
	ENTRANCE_LOW_WEST_FROM_ABOVE_B,
	ENTRANCE_LOW_WEST_FROM_LOW_EAST_A,
	ENTRANCE_LOW_WEST_FROM_LOW_EAST_B,
	ENTRANCE_LOW_EAST_FROM_LOW_WEST_A,
	ENTRANCE_LOW_EAST_FROM_ABOVE,
	ENTRANCE_LOW_EAST_FROM_LOW_WEST_B
} MAGMA_ROCK_SALAMANDER_TAIL_ENTRANCE;

/* The cliff's exits from one of its levels to another, each leading to the */
/* entrance of the same letter: the high west level's two walls down to the */
/* low west level and the high east level's down to the low east level, the */
/* walls back up, and the paths between the levels side by side. */
typedef enum MAGMA_ROCK_SALAMANDER_TAIL_EXIT {
	EXIT_HIGH_WEST_DOWN_A = 1,
	EXIT_HIGH_WEST_DOWN_B,
	EXIT_HIGH_WEST_TO_HIGH_EAST,
	EXIT_HIGH_EAST_TO_HIGH_WEST,
	EXIT_HIGH_EAST_DOWN = 7,
	EXIT_LOW_WEST_UP_A = 9,
	EXIT_LOW_WEST_UP_B,
	EXIT_LOW_WEST_TO_LOW_EAST_A,
	EXIT_LOW_WEST_TO_LOW_EAST_B,
	EXIT_LOW_EAST_TO_LOW_WEST_A,
	EXIT_LOW_EAST_UP,
	EXIT_LOW_EAST_TO_LOW_WEST_B
} MAGMA_ROCK_SALAMANDER_TAIL_EXIT;

/* The cell behaviours the scene's triggers answer. The three maps and the */
/* cliff's four levels each read them their own way, so one number can stand */
/* for two places. Cells on raised ground hold their behaviour some rows south
 */
/* of where they show on screen. */
typedef enum MAGMA_ROCK_CELL_BEHAVIOR {
	/* The cave mouth above the slide's four cracked pillars, cell (16, 28). */
	CELL_BEHAVIOR_MAGMA_ROCK_CAVE_DOOR = 2,
	/* The tops of the high west level's two crumbling pillars, cells (15, 29)
	 */
	/* and (17, 29). */
	CELL_BEHAVIOR_MAGMA_ROCK_CRUMBLING_PILLAR = 20,
	/* A statue that spits a fireball once Burst wakes it */
	/* (scMagmaRockFireStatue): the slide's, at cell (25, 25), the high west */
	/* level's while Felix stands behind it, and the low east level's upper one,
	 */
	/* at cell (48, 39). */
	CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE = 30,
	/* The low east level's lower statue that spits fire, cell (52, 40). */
	CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE_B = 31,
	/* The cracked pillars Burst breaks, lettered as the events that break */
	/* them: on the slide A to D in front of the cave mouth, cells (15, 31), */
	/* (17, 31), (14, 34) and (18, 34), and E on the ledge above, cell (20, 25);
	 */
	/* on the cliff A on the high west level, cell (22, 34), and B on the low */
	/* east level, cell (42, 39). */
	CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_A = 31,
	CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_B,
	CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_C,
	CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_D,
	CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_E,
	/* Where walking into a stone pillar pushes it: west of the low west level's
	 */
	/* pillar, cell (14, 36), and east of the low east level's, cells (37, 38)
	 */
	/* to (40, 38). */
	CELL_BEHAVIOR_MAGMA_ROCK_PILLAR_PUSH = 40,
	/* A statue whose grey cap rides up on a column of flame a while after */
	/* Burst wakes it (scMagmaRockIgniteStatue), a level's first and second: the
	 */
	/* slide's at cell (9, 25), the high east level's at cells (54, 33) and */
	/* (58, 34), the low west level's at (6, 38) and (26, 41) and the low east
	 */
	/* level's at (44, 39). */
	CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A = 50,
	CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B = 51,
	/* On the high west level, the cell just behind its statue, cell (15, 34),
	 */
	/* and the one behind that, cell (14, 34). */
	CELL_BEHAVIOR_MAGMA_ROCK_BESIDE_HIGH_WEST_STATUE = 50,
	CELL_BEHAVIOR_MAGMA_ROCK_PAST_HIGH_WEST_STATUE = 51,
	/* The statue east of the great fire on the top, cell (50, 22). */
	CELL_BEHAVIOR_MAGMA_ROCK_STATUE_ROOM_STATUE = 50
} MAGMA_ROCK_CELL_BEHAVIOR;

/* The trigger a fireball raises as it runs into the leader */
/* (FIELD_STATE.raisedTrigger), which the high east level answers by knocking */
/* him off the wall. */
enum { TRIGGER_FIREBALL_HITS_LEADER = 200 };

/* A burning statue's flame column (scMagmaRockBurnStatue): from frame 35 of */
/* its burning each part rises 8 pixels a frame to its level, 16 pixels above */
/* the one below, and from frame 332 sinks as fast; the cap on top rises a */
/* frame ahead of the column. */
#define FLAME_RISE_FRAME   35
#define FLAME_SINK_FRAME   332
#define FLAME_SPEED		   toFIXED(8)
#define FLAME_LEVEL_HEIGHT toFIXED(16)

/* The frame of Burst's explosions at which a cracked pillar they shake starts
 */
/* to crumble to a stump (its animation 3), before the last blast takes it */
/* away. */
#define PILLAR_CRUMBLE_FRAME 178

/* The slide: its two stone pillars, the statue on the west of the upper */
/* ledge whose cap rides up on a column of flame, the statue on the east of it
 */
/* that spits fire, the pair of statues on either side of the cave mouth with */
/* a fireball flying between them, and its cracked pillars, lettered as the */
/* events that break them: E on the upper ledge, A to D in front of the cave */
/* mouth. */
typedef enum MAGMA_ROCK_SLIDE_ACTOR_ID {
	ACTOR_SLIDE_PILLAR_A = ACTOR_FIRST_PLACED,
	ACTOR_SLIDE_COLUMN_STATUE,
	ACTOR_SLIDE_PILLAR_B,
	ACTOR_CRACKED_PILLAR_E,
	ACTOR_SLIDE_FIRING_STATUE,
	ACTOR_SLIDE_PAIR_WEST_STATUE,
	ACTOR_SLIDE_PAIR_EAST_STATUE,
	ACTOR_CRACKED_PILLAR_A,
	ACTOR_CRACKED_PILLAR_B,
	ACTOR_CRACKED_PILLAR_C,
	ACTOR_CRACKED_PILLAR_D,
	ACTOR_SLIDE_BOULDER, /* Lift raises it */
	ACTOR_SLIDE_STATUE_CAP,
	ACTOR_SLIDE_FIREBALL /* drawn as SPRITE_FLAME_PUFF, flying between the pair
						  */
} MAGMA_ROCK_SLIDE_ACTOR_ID;

/* The cliff's high west level: two stone pillars to push, the two crumbling */
/* pillars, the tightrope's posts, and the statue that spits fire at the */
/* cracked pillar east of it. */
typedef enum MAGMA_ROCK_SALAMANDER_TAIL_HIGH_WEST_ACTOR_ID {
	ACTOR_HIGH_WEST_PILLAR_A = ACTOR_FIRST_PLACED,
	ACTOR_CRUMBLING_PILLAR_A,
	ACTOR_CRUMBLING_PILLAR_B,
	ACTOR_HIGH_WEST_TIGHTROPE_WEST_POST,
	ACTOR_HIGH_WEST_TIGHTROPE_EAST_POST,
	ACTOR_HIGH_WEST_FIRING_STATUE,
	ACTOR_HIGH_WEST_CRACKED_PILLAR_A,
	ACTOR_HIGH_WEST_PILLAR_B
} MAGMA_ROCK_SALAMANDER_TAIL_HIGH_WEST_ACTOR_ID;

/* Its high east level: three pairs of statues facing each other across the */
/* wall Felix climbs, one above another, with a fireball flying between each */
/* pair, two statues whose caps ride up on columns of flame, and a stone */
/* pillar to push. */
typedef enum MAGMA_ROCK_SALAMANDER_TAIL_HIGH_EAST_ACTOR_ID {
	ACTOR_STATUE_PAIR_A_WEST = ACTOR_FIRST_PLACED,
	ACTOR_STATUE_PAIR_A_EAST,
	ACTOR_STATUE_PAIR_B_WEST,
	ACTOR_STATUE_PAIR_B_EAST,
	ACTOR_STATUE_PAIR_C_WEST,
	ACTOR_STATUE_PAIR_C_EAST,
	ACTOR_HIGH_EAST_COLUMN_STATUE_A,
	ACTOR_HIGH_EAST_STATUE_CAP_A,
	ACTOR_HIGH_EAST_PILLAR,
	ACTOR_HIGH_EAST_COLUMN_STATUE_B,
	ACTOR_HIGH_EAST_STATUE_CAP_B,
	ACTOR_FIREBALL_A,
	ACTOR_FIREBALL_B,
	ACTOR_FIREBALL_C
} MAGMA_ROCK_SALAMANDER_TAIL_HIGH_EAST_ACTOR_ID;

/* Its low west level: a stone pillar to push, two statues whose caps ride up */
/* on columns of flame, and the posts Lash ties the rope between. */
typedef enum MAGMA_ROCK_SALAMANDER_TAIL_LOW_WEST_ACTOR_ID {
	ACTOR_LOW_WEST_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_LOW_WEST_COLUMN_STATUE_A,
	ACTOR_LOW_WEST_STATUE_CAP_A,
	ACTOR_LOW_WEST_COLUMN_STATUE_B,
	ACTOR_LOW_WEST_STATUE_CAP_B,
	ACTOR_LOW_WEST_ROPE_WEST_POST,
	ACTOR_LOW_WEST_ROPE_EAST_POST
} MAGMA_ROCK_SALAMANDER_TAIL_LOW_WEST_ACTOR_ID;

/* Its low east level: two stone pillars to push, the cracked pillar, two */
/* statues that spit fire, one whose cap rides up on a column of flame, and */
/* three sprouts. */
typedef enum MAGMA_ROCK_SALAMANDER_TAIL_LOW_EAST_ACTOR_ID {
	ACTOR_LOW_EAST_PILLAR_A = ACTOR_FIRST_PLACED,
	ACTOR_LOW_EAST_PILLAR_B,
	ACTOR_LOW_EAST_CRACKED_PILLAR,
	ACTOR_LOW_EAST_FIRING_STATUE_A,
	ACTOR_LOW_EAST_COLUMN_STATUE,
	ACTOR_LOW_EAST_STATUE_CAP,
	ACTOR_LOW_EAST_FIRING_STATUE_B = ACTOR_FIRST_PLACED + 7,
	ACTOR_LOW_EAST_SPROUT_A, /* Growth grows a vine from each */
	ACTOR_LOW_EAST_SPROUT_B,
	ACTOR_LOW_EAST_SPROUT_C
} MAGMA_ROCK_SALAMANDER_TAIL_LOW_EAST_ACTOR_ID;

/* The top: the great fire before the way in, and the statue east of it that */
/* spits fire at it. */
typedef enum MAGMA_ROCK_STATUE_ACTOR_ID {
	ACTOR_GREAT_FIRE = ACTOR_FIRST_PLACED,
	ACTOR_STATUE_ROOM_STATUE
} MAGMA_ROCK_STATUE_ACTOR_ID;
#include "shared/tables.h"

/* Unused: creates a hidden effect actor at a place: no motion, never */
/* targeted, drawn under the actors. */
UNUSED ACTOR *scMagmaRockCreateEffectActor(s32 x, s32 y, s32 z, s32 kind) {
	ACTOR *actor = (scFieldActorCreate(kind, x, y, z));

	if(actor != NULL) {
		scSetActorSprPriority(actor, 1);
		actor->motionFlg = 0;
		actor->collisionFlg = ACTOR_NOT_TARGETED;
		fdActorSetSprFlags(actor, 0);
		scFieldActorSetSprCol(actor, SPRITE_COLOR_HIDDEN);
		actor->priFlg |=
			ACTOR_PRIORITY_IGNORES_RISES | ACTOR_PRIORITY_UNDERFOOT;
		return actor;
	}
	return NULL;
}

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

#include "shared/position_distance.h"

#include "../field/actor_find_in_cell.c"

extern s32 PushLeaderSteps[];

#include "shared/block_push_onto.h"

#include "../field/map_cell_fill_byte.c"

/* Unused: talking to Fury starts its battle, as in Magma Rock's inside; no */
/* Fury stands outside. */
UNUSED void scMagmaRockFightFury(void) {
	fdRequestDjinnBattle(ACTOR_FIRST_PLACED, DJINNI_FURY);
}

#include "shared/position_flat_distance.h"

s32 fdMapGetTerrainHeight(s32 layer, s32 x, s32 z);

#include "shared/map_cell_set_shape.h"

#include "../field/map_cell_get.c"

#include "shared/actors_raise_cells.h"

#include "shared/blocks_mark_cells.c"

#include "shared/block_clear_cells.h"

/* For each pillar of a list ended by ACTOR_LIST_END: raises its cell to its */
/* top, keeps the cell's attributes, and where it stands on a climbable cell */
/* (MAP_CELL_CLIMBABLE) blocks the cells ahead of and behind it. */
void scMagmaRockSettleBlocks(u16 *ids) {
	for(; *ids != ACTOR_LIST_END; ids++) {
		ACTOR *actor = fdActorGet(*ids);
		u8 *terrainId = &actor->layer;
		s32 height =
			fdMapGetTerrainHeight(*terrainId, actor->x.fixed, actor->z.fixed);
		u8 *blocked;
		MAP_STATE *map;
		MAP_CELL **cells;
		MAP_CELL *cell;
		s32 x, z;

		actor->sceneWord = CELL_ATTRIBUTE_BITS &
						   fdMapCellGetAttributes(MAPGRID_LAYER_GROUND,
												  FIELD_CELL(actor->x.fixed),
												  FIELD_CELL(actor->z.fixed));
		actor->motionFlg = 0;
		actor->collisionFlg = 0;
		fdActorSetSprFlags(actor, 0);
		scMagmaRockInsideSetCellShape(*terrainId, FIELD_CELL(actor->x.fixed),
									  FIELD_CELL(actor->z.fixed),
									  CELL_HEIGHT_STEPS(height) + 4);
		blocked = &actor->sceneByte;
		*blocked = 0;
		if(fdMapCellGetAttributes(MAPGRID_LAYER_GROUND,
								  FIELD_CELL(actor->x.fixed),
								  FIELD_CELL(actor->z.fixed)) &
		   MAP_CELL_CLIMBABLE) {
			map = Ram_HeapSlots->mapGrid;
			if(map == NULL)
				return;
			cells = &map->layers[MAPGRID_LAYER_OVERHEAD].cells;
			z = FIELD_CELL(actor->z.fixed - actor->y.fixed);
			x = FIELD_CELL(actor->x.fixed);
			cell = *cells;
			cell += x + ((z + 1) << 7);
			cell->collisionCode = CELL_BEHAVIOR_WALL;
			cell = *cells;
			cell += x + ((z - 1) << 7);
			cell->collisionCode = CELL_BEHAVIOR_WALL;
			*blocked = 1;
		}
	}
}

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

extern SCENE_REGION MagmaRock_Regions[];
extern SCENE_REGION MagmaRock_SalamanderTailRegions[];
extern SCENE_REGION MagmaRock_StatueRegions[];

const SCENE_REGION *scGetExitRegions(void) {
	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST)
		return MagmaRock_SalamanderTailRegions;
	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_STATUE)
		return MagmaRock_StatueRegions;
	return MagmaRock_Regions;
}

const u32 *scGetExits(void) { return SceneExits; }

extern ACTOR_PLACE MagmaRock_Placements[];
extern ACTOR_PLACE MagmaRock_SalamanderTailHighWestPlacements[];
extern ACTOR_PLACE MagmaRock_SalamanderTailHighEastPlacements[];
extern ACTOR_PLACE MagmaRock_SalamanderTailLowWestPlacements[];
extern ACTOR_PLACE MagmaRock_SalamanderTailLowEastPlacements[];
extern ACTOR_PLACE MagmaRock_StatuePlacements[];

const ACTOR_PLACE *scGetActorPlacements(void) {
	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
		if(MainRoutineStateWork.entrance <=
		   SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE)
			return MagmaRock_SalamanderTailHighWestPlacements;
		if(MainRoutineStateWork.entrance <=
		   SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE)
			return MagmaRock_SalamanderTailHighEastPlacements;
		if(MainRoutineStateWork.entrance <=
		   SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE)
			return MagmaRock_SalamanderTailLowWestPlacements;
		return MagmaRock_SalamanderTailLowEastPlacements;
	}
	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_STATUE)
		return MagmaRock_StatuePlacements;
	return MagmaRock_Placements;
}

/* Nothing visible happens as Felix starts to push one of the striped stone */
/* pillars, with Move or by walking into it; the push itself is the field */
/* engine's. The game first takes the pillar out of the map, so that its cell */
/* is plain ground under it as it slides. On the slide and on the low west and
 */
/* low east levels it also clears the walls the map's upper layer keeps along */
/* the pillars' paths (copied from spare cells off the screen), so that the */
/* pillar can slide along them; scMagmaRockFinishPushingPillar puts them back.
 */
void scMagmaRockStartPushingPillar(void) {
	/* On the slide the overhead layer's closed cells there, as probed, are */
	/* columns 15 and 17 of rows 20 to 23, the upper pillar's cell (15, 22) */
	/* among them, and columns 14, 16 and 18 of rows 23 to 26, the lower */
	/* pillar's (16, 25) among them; the copies rewrite map.txt's (78, 21) to */
	/* (81, 26). While no pillar moves, those of columns 15 and 17 stop a climb
	 */
	/* down those columns from the ledge on row 27, as fdClimbDown tests the */
	/* wall's face below him: standing on cell (15, 27) Felix holds Down and */
	/* stays at the ledge's edge, while from cell (16, 27) he climbs down the */
	/* pegs to the cave mouth above the four cracked pillars. */
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;

	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_SLIDE) {
		fdMapGridCopyCellAttributes(98, 18, 4, 3, 78, 21);
		fdMapGridCopyCellAttributes(98, 24, 4, 3, 78, 24);
		fdActorLiftTargetBlock(MagmaRock_SlidePillars);
	}
	else if(MainRoutineStateWork.next ==
			SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
		if(MainRoutineStateWork.entrance <=
		   SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE) {
			fdActorLiftTargetBlock(MagmaRock_SalamanderTailHighWestPillars);
			/* Unlike the rest of the scene, these tests give the high east
			 * level's */
			/* last entrance to the low west level's pillars, and the low west
			 */
			/* level's last to the low east level's. */
		}
		else if(MainRoutineStateWork.entrance <
				SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE) {
			fdActorLiftTargetBlock(MagmaRock_SalamanderTailHighEastPillars);
		}
		else if(MainRoutineStateWork.entrance <
				SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE) {
			fdMapGridCopyCellAttributes(79, 46, 2, 1, 79, 36);
			fdActorLiftTargetBlock(MagmaRock_SalamanderTailLowWestPillars);
		}
		else {
			if(state->tgt->z.part[0].integer == 600)
				fdMapGridCopyCellAttributes(104, 38, 1, 1, 103, 38);
			else
				fdMapGridCopyCellAttributes(94, 44, 5, 1, 99, 38);
			fdActorLiftTargetBlock(MagmaRock_SalamanderTailLowEastPillars);
		}
	}
	scMagmaRockInsideClearBlockCells(event->movedBlock);
}

/* The overlay's stub to fdEventEnd. */
s32 scMagmaRockEndEvent();

/* A striped stone pillar comes to rest after a push, and stands in the way */
/* again where it stopped. One that has reached the place the climb needs */
/* stays there when the party comes back: on the slide the pillar on the */
/* statues' ledge pushed west out of the way, to cell (15, 25), and the one on
 */
/* the ledge above pushed east, to cell (16, 22), before the climb to the */
/* cliff; on the cliff the high west level's second pillar at cell (27, 33), */
/* the high east level's at (55, 35), the low west level's at (16, 36), and */
/* the low east level's two at (40, 37) and (35, 38). The game stands the */
/* pillar in the map again, puts back the upper layer's walls */
/* scMagmaRockStartPushingPillar cleared, and sets the pillar's flag. */
s32 scMagmaRockFinishPushingPillar(void) {
	ACTOR *pillar = FieldStateWork->movedBlock;
	s32 x, z;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorPlaceMovedBlock();
	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_SLIDE) {
		fdMapGridCopyCellAttributes(98, 21, 4, 3, 78, 21);
		fdMapGridCopyCellAttributes(98, 27, 4, 3, 78, 24);
		scBlocksMarkCells(MagmaRock_SlidePillars);
	}
	else if(MainRoutineStateWork.next ==
			SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
		if(MainRoutineStateWork.entrance <=
		   SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE) {
			scBlocksMarkCells(MagmaRock_SalamanderTailHighWestPillars);
		}
		else if(MainRoutineStateWork.entrance <=
				SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE) {
			scBlocksMarkCells(MagmaRock_SalamanderTailHighEastPillars);
		}
		else if(MainRoutineStateWork.entrance <=
				SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE) {
			fdMapGridCopyCellAttributes(79, 37, 2, 1, 79, 36);
			scBlocksMarkCells(MagmaRock_SalamanderTailLowWestPillars);
		}
		else {
			fdMapGridCopyCellAttributes(103, 39, 1, 1, 103, 38);
			fdMapGridCopyCellAttributes(99, 42, 5, 1, 99, 38);
			scBlocksMarkCells(MagmaRock_SalamanderTailLowEastPillars);
		}
	}
	x = FIELD_CELL(pillar->x.fixed);
	z = FIELD_CELL(pillar->z.fixed);
	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_SLIDE) {
		if(x == 15 && z == 25)
			fdEventSetFlag(FLAG_SLIDE_PILLAR_B_PUSHED);
		else if(x == 16 && z == 22)
			fdEventSetFlag(FLAG_SLIDE_PILLAR_A_PUSHED);
	}
	else if(MainRoutineStateWork.next ==
			SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
		if(MainRoutineStateWork.entrance <=
		   SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE) {
			if(x == 27 && z == 33)
				fdEventSetFlag(FLAG_HIGH_WEST_PILLAR_B_PUSHED);
		}
		else if(MainRoutineStateWork.entrance <=
				SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE) {
			if(x == 55 && z == 35)
				fdEventSetFlag(FLAG_HIGH_EAST_PILLAR_PUSHED);
		}
		else if(MainRoutineStateWork.entrance <=
				SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE) {
			if(x == 16 && z == 36)
				fdEventSetFlag(FLAG_LOW_WEST_PILLAR_PUSHED);
		}
		else if(x == 40 && z == 37) {
			fdEventSetFlag(FLAG_LOW_EAST_PILLAR_A_PUSHED);
		}
		else if(x == 35 && z == 38) {
			fdEventSetFlag(FLAG_LOW_EAST_PILLAR_B_PUSHED);
		}
	}
	return scMagmaRockEndEvent();
}

/* The overlay's import stub that sets up a field Psynergy's caster and */
/* target. */
void syImportFieldPsynergySetupActorPair(s32 owner, u16 value);

/* Plays Burst from the leader at an actor, outside the trigger table: the */
/* explosion a statue's fireball makes where it lands. */
void scMagmaRockPlayBurst(u16 value) {
	MAIN_ROUTINE_STATE *party;

	fdPsynergyLoadResources(ABILITY_BURST, 1);
	party = &MainRoutineStateWork;
	syImportFieldPsynergySetupActorPair(party->leaderActorId, value);
	fdPsynergyRunHandler(1);
	fdBattleEffectStopMainActor();
	fdPsynergyEnd();
}

extern s32 MagmaRock_FlameEmberScript[];

/* A statue's update when the fireball of its pair reaches it: it throws */
/* embers and blinks red, faster and faster, flashes white, and after 144 */
/* frames spits the fireball back. */
void scMagmaRockThrowEmbers(ACTOR *actor) {
	ACTOR *linked = &actor->watchedActor->actor;
	s16 *timer = &actor->tmr;

	if(*timer < 50) {
		u32 phase = FrameCount & 3;

		if(phase == 0) {
			EFFECT_OPTIONS params;
			s32 dx;
			s32 dy;

			params.type = SPRITE_FLAME_PUFF;
			params.pri = 1;
			params.script = MagmaRock_FlameEmberScript;
			dx = (syRandom() & 15) - 8;
			dy = syRandom() & 31;
			scFieldEffectSpawn(actor->x.fixed + INTtoFIXED(dx),
							   actor->y.fixed + INTtoFIXED(dy) - toFIXED(8),
							   actor->z.fixed, 0, phase, phase,
							   EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
								   EFFECT_USE_PRIORITY | 1,
							   &params);
		}
		if(FrameCount & 4)
			scFieldActorSetSprCol(actor, SPRITE_COLOR_RED_TINT);
		else
			scFieldActorSetSprCol(actor, SPRITE_COLOR_NORMAL);
	}
	else if(*timer < 100) {
		if(FrameCount & 4)
			scFieldActorSetSprCol(actor, SPRITE_COLOR_RED_TINT);
		else
			scFieldActorSetSprCol(actor, SPRITE_COLOR_NORMAL);
	}
	else if(*timer < 140) {
		if(FrameCount & 2)
			scFieldActorSetSprCol(actor, SPRITE_COLOR_RED_TINT);
		else
			scFieldActorSetSprCol(actor, SPRITE_COLOR_NORMAL);
	}
	else if(*timer < 144) {
		scFieldActorSetSprCol(actor, SPRITE_COLOR_WHITE);
	}
	else {
		scFieldActorSetSprCol(actor, SPRITE_COLOR_NORMAL);
		actor->update = NULL;
		linked->x.fixed = actor->x.fixed;
		linked->y.fixed = actor->y.fixed;
		linked->z.fixed = actor->z.fixed;
		linked->update = scMagmaRockMoveFireball;
	}
	(*timer)++;
}

void fdActorPlaySndInView(ACTOR *actor, s32 value);

/* The update of a fireball or a statue's fire: every fourth frame it leaves */
/* an ember near it, drifting a little north or south, and every sixteenth it */
/* whooshes, unless one has just knocked Felix down. */
void scMagmaRockLeaveEmbers(ACTOR *actor) {
	u32 frame = FrameCount;

	if((frame & 3) == 0) {
		EFFECT_OPTIONS config;
		s32 dz;
		s32 velocityZ;
		FIXED z;

		config.type = SPRITE_FLAME_PUFF;
		config.pri = 2;
		config.script = MagmaRock_EmberScript;
		if((frame & 15) == 0 && !fdEventGetFlag(FLAG_LEADER_KNOCKED_DOWN))
			fdActorPlaySndInView(actor, SE_WHOOSH);
		dz = ((syRandom() & 15) - 8) * (toFIXED(1) / 2);
		velocityZ = ((syRandom() & 31) - 16) * (toFIXED(1) / 16);
		z = actor->z.fixed;
		scFieldEffectSpawn(
			actor->x.fixed, actor->y.fixed, z + dz, 0, 0, velocityZ,
			EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | 1,
			&config);
	}
}

/* A fireball's update: it flies toward the other statue of its pair leaving */
/* embers; running into Felix raises TRIGGER_FIREBALL_HITS_LEADER and takes it
 */
/* away, and reaching the statue hands it over (scMagmaRockThrowEmbers). */
void scMagmaRockMoveFireball(ACTOR *actor) {
	FIXED step;
	ACTOR *partner;
	ACTOR *leader;

	step = syCos(actor->facing);
	partner = fdActorGet(actor->userValue + actor->rollSide);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	actor->x.fixed += step * 2;
	scMagmaRockLeaveEmbers(actor);
	if(leader->x.fixed - toFIXED(8) < actor->x.fixed + toFIXED(6) &&
	   actor->x.fixed - toFIXED(6) < leader->x.fixed + toFIXED(8) &&
	   leader->z.fixed - toFIXED(8) < actor->z.fixed + toFIXED(6) &&
	   actor->z.fixed - toFIXED(6) < leader->z.fixed + toFIXED(8) &&
	   leader->y.fixed < actor->y.fixed + toFIXED(8) &&
	   actor->y.fixed - toFIXED(8) < leader->y.fixed + toFIXED(15)) {
		FieldStateWork->raisedTrigger = TRIGGER_FIREBALL_HITS_LEADER;
		actor->update = NULL;
		actor->x.fixed = 0;
		actor->y.fixed = 0;
		actor->z.fixed = 0;
	}
	if(scPosFlatDistance(&actor->x.fixed, &partner->x.fixed) <= 8 &&
	   FIELD_CELL(actor->y.fixed) == FIELD_CELL(partner->y.fixed)) {
		actor->x.fixed = 0;
		actor->update = NULL;
		/* It turns back, every other time a sixteenth of a turn off the line,
		 * so */
		/* it rolls a little slower one way than the other. */
		actor->facing ^= FACING_WEST | TRIG_TURN / 16;
		actor->y.fixed = 0;
		actor->z.fixed = 0;
		actor->rollSide ^= 1;
		partner->tmr = 0;
		partner->update = (void *)scMagmaRockThrowEmbers;
		partner->watchedActor = (FIELD_OBJECT *)actor;
	}
}

/* Unless the party keeps its place: sets a fireball on one statue of the pair
 */
/* whose first statue is `first` (`side` 0 that one, 1 the other), facing the */
/* other, and starts it flying. */
void scMagmaRockPlaceFireball(u16 id, s16 first, s16 side) {
	ACTOR *actor = fdActorGet(id);
	ACTOR *firstStatue;
	ACTOR *secondStatue;
	u16 firstId;
	u16 startSide;
	s32 startId;
	s32 secondId;
	u8 *flags;
	u8 rollSide;
	s16 *pairFirst;

	if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION))
		return;
	firstId = first;
	startSide = side;
	startId = firstId + startSide;
	actor->x.fixed = fdActorGet(startId)->x.fixed;
	actor->y.fixed = fdActorGet(startId)->y.fixed;
	actor->z.fixed = fdActorGet(startId)->z.fixed;
	scSetActorSprPriority(actor, 2);
	pairFirst = &actor->userValue;
	*pairFirst = first;
	rollSide = side ^ 1;
	actor->rollSide = rollSide;
	actor->update = (void *)scMagmaRockMoveFireball;
	secondId = firstId + 1;
	flags = &actor->motionFlg;
	if(startSide != 0) {
		firstStatue = fdActorGet(firstId);
		secondStatue = fdActorGet(secondId);
		if(firstStatue->x.fixed < secondStatue->x.fixed)
			actor->facing = FACING_WEST;
		else
			actor->facing = FACING_EAST;
	}
	else {
		firstStatue = fdActorGet(firstId);
		secondStatue = fdActorGet(secondId);
		if(firstStatue->x.fixed > secondStatue->x.fixed)
			actor->facing = FACING_WEST;
		else
			actor->facing = FACING_EAST;
	}
	*flags = 0;
}

#include "shared/statue_flash_under_burst.h"

/* The update of a statue Burst has woken, counting down from 1,024 while no */
/* menu, Psynergy or item is in use: it blinks red quickly, then slowly from */
/* 528, and from 400 it burns (scMagmaRockBurnStatue). */
void scMagmaRockWarnStatue(ACTOR *actor) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	if(actor->tmr > 528) {
		if(actor->tmr & 2)
			scFieldActorSetSprCol(actor, SPRITE_COLOR_RED_TINT);
		else
			scFieldActorSetSprCol(actor, SPRITE_COLOR_NORMAL);
	}
	else if(actor->tmr > 400) {
		if(actor->tmr & 16)
			scFieldActorSetSprCol(actor, SPRITE_COLOR_RED_TINT);
		else
			scFieldActorSetSprCol(actor, SPRITE_COLOR_NORMAL);
	}
	else {
		actor->update = scMagmaRockBurnStatue;
		actor->tmr = 1;
	}
	if(state->psynergy == 0 && state->fieldMenu == 0 && state->pauseMenu == 0 &&
	   state->item == 0 && state->transitionActive != 0)
		actor->tmr--;
}

#include "shared/statue_find_under_burst.h"

/* Looks up the cell at (x, z) in an (actor, cell, value) table ended by */
/* ACTOR_LIST_END and returns the value of its row. */
s32 scMagmaRockLookUpCellStatue(s32 x, s32 z, u16 *table) {
	s32 result = 0;
	s32 code = fdMapCellGetAttributes(MAPGRID_LAYER_GROUND, FIELD_CELL(x),
									  FIELD_CELL(z)) >>
			   8;

	while(*table != ACTOR_LIST_END) {
		table++;
		if(code == *table) {
			result = table[1];
			break;
		}
		table++;
		result = *table++;
	}
	return result;
}

/* Felix casts Burst at one of the yellow statues, and it glows red while the */
/* explosions go off round it, blinking faster at the last blast. The game */
/* finds the statue by its cell in the room's table (syStatueFindUnderBurst) */
/* and blinks it (syStatueFlashUnderBurst), every fourth frame while the */
/* explosions shake the ground (BURST_STAGE_SMALL_BLAST) and every other one at
 */
/* the last blast (BURST_STAGE_GREAT_BLAST). The statue then fires */
/* (scMagmaRockFireStatue) or lights (scMagmaRockIgniteStatue). */
void scMagmaRockTargetStatue(s32 stage) {
	FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;
	s16 *table = MagmaRock_SlideStatues;
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

	if(party->next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
		if(party->entrance <= SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE)
			table = MagmaRock_SalamanderTailHighWestStatues;
		else if(party->entrance <= SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE)
			table = MagmaRock_SalamanderTailHighEastStatues;
		else if(party->entrance <= SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE)
			table = MagmaRock_SalamanderTailLowWestStatues;
		else
			table = MagmaRock_SalamanderTailLowEastStatues;
	}
	else if(party->next == SCENE_MAGMA_ROCK_STATUE) {
		table = MagmaRock_StatueStatues;
	}
	if(stage == BURST_STAGE_SMALL_BLAST) {
		ACTOR *target;

		syStatueFindUnderBurst(table);
		target = state->tgt;
		target->flashMask = 4;
		target->update = (OBJECT_UPDATE)syStatueFlashUnderBurst;
	}
	if(stage == BURST_STAGE_GREAT_BLAST) {
		ACTOR *target;

		syStatueFindUnderBurst(table);
		target = state->tgt;
		target->flashMask = 2;
		target->update = (OBJECT_UPDATE)syStatueFlashUnderBurst;
	}
}

/* An ember's update: it moves by its velocity, kept in bounce, gravity and */
/* sceneWord, and slows by an eighth. */
void scMagmaRockDriftAndSlow(ACTOR *actor) {
	actor->x.fixed += actor->bounce;
	actor->y.fixed += actor->gravity;
	actor->z.fixed += actor->sceneWord;
	actor->bounce -= actor->bounce >> 3;
	actor->gravity -= actor->gravity >> 3;
	actor->sceneWord -= actor->sceneWord >> 3;
}

s32 scPosFlatDistance(FIXED *a, FIXED *b);
s32 scMagmaRockEndEvent(void);

/* A statue's fireball flies straight ahead, the camera following it, until it
 */
/* meets the edge of its ledge, a striped stone pillar or anything else */
/* standing in its way: at the edge or a stone pillar it bursts into embers, */
/* and on anything else it explodes as Burst does (scMagmaRockPlayBurst). Then
 */
/* the camera goes back to Felix. */
s32 scMagmaRockFireFromStatue(ACTOR *source) {
	FIXED position[3];
	EFFECT_OPTIONS options;
	ACTOR *actor;
	ACTOR *other;
	s32 step;
	s32 parity;
	s32 i;
	s32 attributes;

	{
		FIXED x = source->x.fixed;
		FIXED y = source->y.fixed;
		FIXED z = source->z.fixed;

		actor = scFieldActorCreate(SPRITE_FLAME_PUFF, x, y, z);
	}
	step = syCos(source->facing) << 1;
	actor->motionFlg = 0;
	fdActorSelectAnimation(actor, 1);
	fdActorSetSprFlags(actor, 0);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	position[0] = actor->x.fixed;
	position[1] = actor->y.fixed;
	position[2] = actor->z.fixed;
	for(;;) {
		position[0] += step;
		attributes = fdMapCellGetAttributes(
						 MAPGRID_LAYER_OVERHEAD, FIELD_CELL(position[0]),
						 FIELD_CELL(position[2] - position[1])) >>
					 8;
		if(attributes == CELL_BEHAVIOR_MAGMA_ROCK_FIRE_EDGE ||
		   attributes == CELL_BEHAVIOR_MAGMA_ROCK_LAVA_FALL)
			goto stop;
		for(i = ACTOR_FIRST_PLACED; i != FIELD_TREASURE_ACTOR; i++) {
			other = FieldStateWork->actors[i];
			if(other->script == NULL)
				continue;
			if(FIELD_CELL(source->x.fixed) == FIELD_CELL(other->x.fixed) &&
			   FIELD_CELL(source->y.fixed) == FIELD_CELL(other->y.fixed) &&
			   FIELD_CELL(source->z.fixed) == FIELD_CELL(other->z.fixed))
				continue;
			if(scPosFlatDistance(&other->x.fixed, position) > 8)
				continue;
			if(FIELD_CELL(other->y.fixed) != FIELD_CELL(position[1]))
				continue;
			if(((SPR *)other->spr)->entries[0]->anmId == SPRITE_STONE_PILLAR)
				goto stop;
			goto hit;
		}
	}

stop:
	actor->acceleration = toFIXED(2);
	actor->speedLimit = toFIXED(2);
	actor->update = scMagmaRockLeaveEmbers;
	scFieldActorSetMoveTargetFX(actor, position[0], position[1], position[2]);
	fdCameraSetSpeed(2);
	fdCameraMoveToFX(position[0], position[1], position[2], 1);
	scFieldActorWaitForMove(actor);
	actor->update = NULL;
	fdActorRelease(actor);
	for(i = 0; i != 16; i++) {
		parity = FrameCount & 1;

		if(parity == 0) {
			s32 randomX;
			s32 randomY;

			/* Each ember flies off at up to 2 pixels a frame across and up or
			 * down. */
			options.type = SPRITE_FLAME_PUFF;
			options.pri = 1;
			options.update = (void (*)(FIELD_OBJECT *))scMagmaRockDriftAndSlow;
			options.script = MagmaRock_EmberScript;
			randomX = syRandom() & 31;
			randomY = syRandom() & 31;
			scFieldEffectSpawn(position[0], position[1], position[2],
							   (randomX - 16) << 13, (randomY - 16) << 13,
							   parity,
							   EFFECT_USE_UPDATE | EFFECT_USE_SCRIPT |
								   EFFECT_USE_TYPE | EFFECT_USE_PRIORITY,
							   &options);
		}
		syWait(1);
	}
	syWait(24);
	syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	syWait(10);
	return scMagmaRockEndEvent();

hit:
	actor->acceleration = toFIXED(2);
	actor->speedLimit = toFIXED(2);
	actor->update = scMagmaRockLeaveEmbers;
	scFieldActorSetMoveTargetFX(actor, position[0], position[1], position[2]);
	fdCameraSetSpeed(2);
	fdCameraMoveToFX(position[0], position[1], position[2], 1);
	scFieldActorWaitForMove(actor);
	actor->update = NULL;
	fdActorRelease(actor);
	fdEventSetFlag(FLAG_FIRE_BURST_PLAYING);
	scMagmaRockPlayBurst(i);
	fdEventClearFlag(FLAG_FIRE_BURST_PLAYING);
	syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	syWait(10);
	return scMagmaRockEndEvent();
}

/* After Felix's Burst, the yellow statue he cast it at flashes white and spits
 */
/* a fireball straight ahead: on the slide's statues' ledge and the high west */
/* level at the cracked pillar beyond it, which the fireball's blast breaks, */
/* and on the top at the great fire. The fireball's flight is */
/* scMagmaRockFireFromStatue's. On the low east level both statues face west: */
/* the upper one's fireball meets the cracked pillar at cell (42, 39), and the
 */
/* lower one's, from cell (52, 40), the level's second cracked pillar, at */
/* (760, 648) on the same level (the placement table's seventh actor), and */
/* explodes on it as Burst does. */
void scMagmaRockFireStatue(void) {
	FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;
	ACTOR *actor;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventWait(10);
	actor = state->tgt;
	actor->update = NULL;
	scFieldActorSetSprCol(actor, SPRITE_COLOR_WHITE);
	syFieldEventWait(3);
	scFieldActorSetSprCol(state->tgt, SPRITE_COLOR_NORMAL);
	syFieldEventWait(1);
	scMagmaRockFireFromStatue(state->tgt);
	scMagmaRockEndEvent();
}

/* After Felix's Burst, a yellow statue with a grey cap blinks red for about */
/* ten seconds, faster at first; then fire bursts from it and its cap rides up
 */
/* on a column of flame for about five seconds, carrying Felix up to the ledge
 */
/* above if he is standing on it, before it sinks again. The game only starts */
/* the statue's countdown (scMagmaRockWarnStatue, then scMagmaRockBurnStatue).
 */
void scMagmaRockIgniteStatue(void) {
	FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;
	ACTOR *actor;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	actor = state->tgt;
	actor->tmr = 1024;
	actor->update = scMagmaRockWarnStatue;
	scMagmaRockEndEvent();
}

/* Burst goes off at a cracked pillar: rocks fly up round it while the */
/* explosions shake the ground, the pillar crumbles to a stump, and the last */
/* blast takes it away. Felix's own Burst at a cracked pillar runs it through */
/* the pillar's rows; a statue that spits fire runs it as its fireball */
/* explodes on the cracked pillar in its way. On the cliff's high west level, */
/* Felix's Burst at the statue on cell (16, 34) has it flash white and spit a */
/* fireball east, and the fireball's blast breaks the cracked pillar on cell */
/* (22, 34) this way. The statue's rows that run it come after the */
/* scMagmaRockTargetStatue rows, which take Felix's own Burst and are passed */
/* over while the fireball's blast plays (FLAG_FIRE_BURST_PLAYING); they set */
/* no flag for the pillar. Rocks are thrown one a frame during the shaking */
/* (BURST_STAGE_SMALL_BLAST) and eight at the last blast */
/* (BURST_STAGE_GREAT_BLAST), which moves the target out of the scene; the */
/* stump is the pillar's animation from PILLAR_CRUMBLE_FRAME. */
void scMagmaRockBurstCrackedPillar(s32 stage, s32 frame) {
	FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;

	if(frame == 0)
		scMagmaRockStartFallingRocks();
	if(stage == BURST_STAGE_SMALL_BLAST) {
		ACTOR *actor = state->tgt;

		scMagmaRockDropRocks(actor->x.fixed, actor->y.fixed + toFIXED(15),
							 actor->z.fixed, 1);
	}
	if(stage == BURST_STAGE_GREAT_BLAST) {
		ACTOR *actor = state->tgt;
		ACTOR *target;

		scMagmaRockDropRocks(actor->x.fixed, actor->y.fixed + toFIXED(15),
							 actor->z.fixed, 8);
		target = state->tgt;
		SET_ACTOR_POSITION(target, 0, 0, 0);
	}
	if(frame == PSYNERGY_EVENT_LAST_FRAME)
		syRockEndFalling();
	if(frame == PILLAR_CRUMBLE_FRAME)
		fdActorSelectAnimation(state->tgt, 3);
}

/* The update of one of the four parts of a statue's flame column */
/* (scMagmaRockBurnStatue): it rises and sinks with the statue's burning, and */
/* is released once sunk. */
void scMagmaRockRaiseFlamePart(ACTOR *part) {
	ACTOR *owner = &part->watchedActor->actor;

	if(owner->tmr > FLAME_RISE_FRAME &&
	   owner->tmr < part->flameLevel * 2 + FLAME_RISE_FRAME + 1) {
		part->y.fixed = owner->y.fixed + toFIXED(32);
		part->y.fixed +=
			owner->tmr * FLAME_SPEED - FLAME_RISE_FRAME * FLAME_SPEED;
	}
	else if(part->flameLevel * 2 + FLAME_RISE_FRAME + 1 < owner->tmr &&
			owner->tmr < FLAME_SINK_FRAME) {
		part->y.fixed = owner->y.fixed + part->flameLevel * FLAME_LEVEL_HEIGHT +
						toFIXED(32);
	}
	else if(owner->tmr > FLAME_SINK_FRAME &&
			owner->tmr < part->flameLevel * 2 + FLAME_SINK_FRAME + 1) {
		part->y.fixed = owner->y.fixed + part->flameLevel * FLAME_LEVEL_HEIGHT +
						toFIXED(32);
		part->y.fixed -=
			owner->tmr * FLAME_SPEED - FLAME_SINK_FRAME * FLAME_SPEED;
	}
	else if(part->flameLevel * 2 + FLAME_SINK_FRAME + 1 < owner->tmr) {
		fdActorRelease(part);
	}
}

/* The update of a statue's cap on top of its flame column: it rises and sinks
 */
/* with the column, a frame ahead, throws a flame puff every eighth frame, and
 */
/* raises the statue's cell with it, so that the leader on the cap rides up. */
void scMagmaRockRaiseFlameTop(FIELD_OBJECT *object) {
	ACTOR *watched = (ACTOR *)object->eff.countdown;
	EFFECT_OPTIONS config;
	s32 step;
	s32 mask;

	if(watched->tmr >= FLAME_RISE_FRAME &&
	   watched->tmr <= FLAME_RISE_FRAME + 7) {
		object->eff.y = watched->y.fixed + toFIXED(24);
		object->eff.y = object->eff.y + watched->tmr * FLAME_SPEED -
						(FLAME_RISE_FRAME - 1) * FLAME_SPEED;
	}
	else {
		if(watched->tmr >= 9 && watched->tmr < FLAME_SINK_FRAME) {
			object->eff.y = watched->y.fixed +
							object->eff.sceneCnt * FLAME_LEVEL_HEIGHT +
							toFIXED(24);
		}
		else if(watched->tmr > FLAME_SINK_FRAME &&
				watched->tmr <
					object->eff.sceneCnt * 2 + FLAME_SINK_FRAME + 1) {
			object->eff.y = watched->y.fixed +
							object->eff.sceneCnt * FLAME_LEVEL_HEIGHT +
							toFIXED(24);
			object->eff.y -=
				watched->tmr * FLAME_SPEED - FLAME_SINK_FRAME * FLAME_SPEED;
		}
		else if(object->eff.sceneCnt * 2 + FLAME_SINK_FRAME + 1 <
				watched->tmr) {
			object->eff.y = object->actor.groundHeight;
			object->eff.update = 0;
		}
	}

	mask = 7;
	step = FrameCount & mask;
	if(step == 0) {
		config.type = SPRITE_FLAME_PUFF;
		config.pal = SPRITE_COLOR_RED_TINT;
		config.pri = 2;
		config.update = (void (*)(FIELD_OBJECT *))scMagmaRockDriftAndSlow;
		config.script = MagmaRock_EmberScript;
		scFieldEffectSpawn(
			object->eff.x + toFIXED(1), object->eff.y - toFIXED(4),
			object->eff.z, ((syRandom() & mask) - 3) << 14, -toFIXED(2), step,
			EFFECT_USE_UPDATE | EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
				EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
			&config);
	}
	scMagmaRockInsideSetCellShape(watched->layer, FIELD_CELL(watched->x.fixed),
								  FIELD_CELL(watched->z.fixed),
								  CELL_HEIGHT_STEPS(object->eff.y) + 1);
}

void fdActorPlaySndInView(ACTOR *actor, s32 a);

/* A statue's update once it burns: embers first, then at frame 33 its flame */
/* column of four parts rises, and the cap on its top with it */
/* (scMagmaRockRaiseFlameTop). */
void scMagmaRockBurnStatue(ACTOR *actor) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 type = SPRITE_STEAM_VENT;

	if((FrameCount & 15) == 0)
		fdActorPlaySndInView(actor, SE_WHOOSH);
	scFieldActorSetSprCol(actor, SPRITE_COLOR_RED_TINT);
	if(actor->tmr <= 31) {
		u32 phase = FrameCount & 3;

		if(phase == 0) {
			EFFECT_OPTIONS config;
			s32 randomX;
			s32 randomZ;

			config.type = SPRITE_FLAME_PUFF;
			config.pal = SPRITE_COLOR_RED_TINT;
			config.pri = 2;
			config.update = (void (*)(FIELD_OBJECT *))scMagmaRockDriftAndSlow;
			config.script = MagmaRock_EmberScript;
			/* Each ember flies off at up to 2 pixels a frame across the ground.
			 */
			randomX = syRandom() & 15;
			randomZ = syRandom() & 15;
			scFieldEffectSpawn(
				actor->x.fixed + toFIXED(1), actor->y.fixed + toFIXED(24),
				actor->z.fixed, (randomX - 8) << 14, 0, (randomZ - 8) << 14,
				EFFECT_USE_UPDATE | EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
					EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
				&config);
		}
	}
	else if(actor->tmr == 33) {
		ACTOR *parts[4];
		ACTOR *fifthPart;
		s32 id;
		s32 i;

		fdActorPlaySndInView(actor, SE_CLATTER);
		parts[0] =
			(scFieldActorCreate(type, actor->x.fixed + toFIXED(1),
								actor->y.fixed + toFIXED(32), actor->z.fixed));
		for(i = 1; i != 4; i++) {
			SPR *first = parts[0]->spr;
			ACTOR *part = (scFieldActorCreate(type, 0, 0, 0));
			SPR *sprite;

			parts[i] = part;
			sprite = part->spr;
			sprite->slot = first->slot;
			sprite->retainCacheEntry = 1;
			part->x.fixed = parts[0]->x.fixed;
			part->y.fixed = parts[0]->y.fixed;
			part->z.fixed = parts[0]->z.fixed;
			part->motionFlg = 0;
			fdActorSetSprFlags(part, 0);
			fdActorSelectAnimation(parts[i], 3);
			{
				ACTOR *flame = parts[i];

				flame->flameLevel = 3 - i;
				flame->watchedActor = (FIELD_OBJECT *)actor;
				flame->update = scMagmaRockRaiseFlamePart;
				scFieldActorSetSprCol(flame, SPRITE_COLOR_BGR);
			}
		}
		{
			ACTOR *part = parts[0];

			part->motionFlg = 0;
			fdActorSetSprFlags(part, 0);
			fdActorSelectAnimation(parts[0], 3);
			scSetActorSprPriority(parts[0], 3);
			{
				ACTOR *flame = parts[0];

				flame->flameLevel = 3;
				flame->watchedActor = (FIELD_OBJECT *)actor;
				flame->update = scMagmaRockRaiseFlamePart;
				scFieldActorSetSprCol(flame, SPRITE_COLOR_BGR);
			}
		}
		id = 0;
		if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_SLIDE) {
			id = scMagmaRockLookUpCellStatue(actor->x.fixed, actor->z.fixed,
											 MagmaRock_SlideStatues);
		}
		else if(MainRoutineStateWork.next ==
				SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
			s16 entrance = MainRoutineStateWork.entrance;
			FIXED x = actor->x.fixed;
			FIXED z = actor->z.fixed;

			if(entrance <= SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE)
				id = scMagmaRockLookUpCellStatue(
					x, z, MagmaRock_SalamanderTailHighWestStatues);
			else if(entrance <= SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE)
				id = scMagmaRockLookUpCellStatue(
					x, z, MagmaRock_SalamanderTailHighEastStatues);
			else if(entrance <= SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE)
				id = scMagmaRockLookUpCellStatue(
					x, z, MagmaRock_SalamanderTailLowWestStatues);
			else
				id = scMagmaRockLookUpCellStatue(
					x, z, MagmaRock_SalamanderTailLowEastStatues);
		}
		fifthPart = fdActorGet(id);
		fifthPart->flameLevel = 4;
		fifthPart->watchedActor = (FIELD_OBJECT *)actor;
		fifthPart->update = scMagmaRockRaiseFlameTop;
		actor->tmr++;
	}
	else if(actor->tmr > 364) {
		scFieldActorSetSprCol(actor, SPRITE_COLOR_NORMAL);
		actor->update = NULL;
	}
	if(state->psynergy == 0 && state->fieldMenu == 0 && state->pauseMenu == 0 &&
	   state->item == 0 && (u16)state->transitionActive != 0)
		actor->tmr++;
}

/* Burst at a cracked pillar on the slide or on the cliff: the pillar breaks */
/* (scMagmaRockBurstCrackedPillar) and its flag keeps it gone until the party */
/* leaves. */
#define DEFINE_BURST_SLIDE_OR_CLIFF_PILLAR(name, slidePillar, cliffPillar,     \
										   flag)                               \
	void name(s32 stage, s32 frame) {                                          \
		FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;              \
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;                     \
                                                                               \
		{                                                                      \
			s32 scene = party->next;                                           \
                                                                               \
			if(scene == SCENE_MAGMA_ROCK_SLIDE)                                \
				state->tgt = fdActorGet(slidePillar);                          \
			else if(scene == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST)           \
				state->tgt = fdActorGet(cliffPillar);                          \
			scMagmaRockBurstCrackedPillar(stage, frame);                       \
		}                                                                      \
		fdEventSetFlag(flag);                                                  \
	}

/* Burst breaks the north-west one of the four cracked pillars in front of the
 */
/* slide's cave mouth (cell (15, 31)), or the cracked pillar on the cliff's */
/* high west level (cell (22, 34)). Either sets `FLAG_CRACKED_PILLAR_A_BURST`.
 */
/* The high west statue's fireball breaks that pillar through the statue's own
 */
/* rows instead (scMagmaRockBurstCrackedPillar), not this. */
DEFINE_BURST_SLIDE_OR_CLIFF_PILLAR(MagmaRock_BurstCrackedPillarA,
								   ACTOR_CRACKED_PILLAR_A,
								   ACTOR_HIGH_WEST_CRACKED_PILLAR_A,
								   FLAG_CRACKED_PILLAR_A_BURST)

/* Burst breaks the north-east one of the four cracked pillars in front of the
 */
/* slide's cave mouth (cell (17, 31)), or the cracked pillar on the cliff's low
 */
/* east level (cell (42, 39)) as its statue's fireball lands on it. Either sets
 */
/* `FLAG_CRACKED_PILLAR_B_BURST`. */
DEFINE_BURST_SLIDE_OR_CLIFF_PILLAR(MagmaRock_BurstCrackedPillarB,
								   ACTOR_CRACKED_PILLAR_B,
								   ACTOR_LOW_EAST_CRACKED_PILLAR,
								   FLAG_CRACKED_PILLAR_B_BURST)

/* Defines what Burst does to a cracked pillar: it breaks the pillar actor */
/* (scMagmaRockBurstCrackedPillar) and sets its flag. */
#define DEFINE_BURST_CRACKED_PILLAR(name, pillar, flag)                        \
	void name(s32 stage, s32 frame) {                                          \
		FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;              \
                                                                               \
		state->tgt = fdActorGet(pillar);                                       \
		scMagmaRockBurstCrackedPillar(stage, frame);                           \
		fdEventSetFlag(flag);                                                  \
	}

/* Burst breaks the south-west one of the four cracked pillars in front of the
 */
/* slide's cave mouth (cell (14, 34)). It sets `FLAG_CRACKED_PILLAR_C_BURST`. */
DEFINE_BURST_CRACKED_PILLAR(MagmaRock_BurstCrackedPillarC,
							ACTOR_CRACKED_PILLAR_C, FLAG_CRACKED_PILLAR_C_BURST)

/* Burst breaks the south-east one of the four cracked pillars in front of the
 */
/* slide's cave mouth (cell (18, 34)). It sets `FLAG_CRACKED_PILLAR_D_BURST`. */
DEFINE_BURST_CRACKED_PILLAR(MagmaRock_BurstCrackedPillarD,
							ACTOR_CRACKED_PILLAR_D, FLAG_CRACKED_PILLAR_D_BURST)

/* The fireball of the statue at the east end of the slide's upper ledge */
/* lands on the cracked pillar west of it (cell (20, 25)), and the blast breaks
 */
/* the pillar, opening the climb up to the ledge where it stood. It sets */
/* `FLAG_CRACKED_PILLAR_E_BURST`. */
DEFINE_BURST_CRACKED_PILLAR(MagmaRock_BurstCrackedPillarE,
							ACTOR_CRACKED_PILLAR_E, FLAG_CRACKED_PILLAR_E_BURST)

/* Nothing visible happens when Felix, come out of the Ember Stone's room */
/* (MAGMA_ROCK_SLIDE_FROM_MAGMA_ROCK_INSIDE_EMBER_STONE), takes his first step
 */
/* of his own in the cave mouth above the slide's cracked pillars (cell */
/* (16, 28)). He arrives standing in the mouth facing out, and the game's */
/* placing him there only sets `FLAG_CAVE_DOOR_USED` (scMagmaRockEnterCave), so
 */
/* he is not sent straight back in; this step clears it, so that walking north
 */
/* into the cave takes him back inside from then on. */
void scMagmaRockResetCaveDoor(void) {
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

	if(party->movementMode != LEADER_MOVEMENT_KEYS)
		fdEventClearFlag(FLAG_CAVE_DOOR_USED);
}

/* Felix walks north into the dark cave mouth above the slide's cracked */
/* pillars (cell (16, 28)) and goes inside Magma Rock, to the room of the Ember
 */
/* Stone. The game walks him a step further in, plays the exit sound and */
/* leaves; when the game rather than the player moved him onto the cell, it */
/* only sets `FLAG_CAVE_DOOR_USED`. */
void scMagmaRockEnterCave(void) {
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

	if(party->movementMode != LEADER_MOVEMENT_KEYS) {
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		fdActorGet(party->leaderActorId)->motionFlg = 0;
		fdActorWalkTo(party->leaderActorId, 264, 440);
		syFieldEventWait(5);
		fdEventPlaySnd(SE_MAP_EXIT);
		syFieldEventSetExitRequest(
			MAGMA_ROCK_SLIDE_TO_MAGMA_ROCK_INSIDE_EMBER_STONE);
		scMagmaRockEndEvent();
	}
	fdEventSetFlag(FLAG_CAVE_DOOR_USED);
}

/* Nothing visible happens as Felix begins to cast Burst at one of the yellow */
/* statues; the game settles whatever the field engine has pending before the */
/* statue's own events run (scMagmaRockTargetStatue). */
void scMagmaRockResolveBeforeBurst(void) { fdEventEndLastingPsynergy(0); }

/* Felix comes down on one of the two dark pillars on the cliff's high west */
/* level (cells (15, 29) and (17, 29)) without hopping straight on: the first */
/* time it cracks with a flash and a puff of dust, the second time it crumbles
 */
/* under him and he drops with it to the ground below */
/* (scMagmaRockStepOnCrumblingPillar). */
void scMagmaRockCheckCrumblingPillars(void) {
	scMagmaRockStepOnCrumblingPillar(MagmaRock_CrumblingPillars);
}

/* Nothing visible happens when Felix steps onto the cell just behind the */
/* yellow statue on the cliff's high west level (cell (15, 34)); from then on */
/* Burst cast at the statue wakes it, so that it spits a fireball east at the */
/* cracked pillar. The game makes the statue's cell answer Burst as a statue */
/* that spits fire, and sets `FLAG_HIGH_WEST_STATUE_READY`. */
void scMagmaRockReadyHighWestStatue(void) {
	fdSetCellBehaviorAt(MAPGRID_LAYER_GROUND, toFIXED(264), toFIXED(552),
						CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE);
	fdEventSetFlag(FLAG_HIGH_WEST_STATUE_READY);
}

/* Nothing visible happens when Felix steps a cell further west from behind */
/* the high west level's yellow statue (cell (14, 34)); Burst no longer wakes */
/* the statue until he steps back behind it. The game makes the statue's cell */
/* plain ground again and clears `FLAG_HIGH_WEST_STATUE_READY`. */
void scMagmaRockResetHighWestStatue(void) {
	fdSetCellBehaviorAt(MAPGRID_LAYER_GROUND, toFIXED(264), toFIXED(552),
						CELL_BEHAVIOR_NORMAL);
	fdEventClearFlag(FLAG_HIGH_WEST_STATUE_READY);
}

/* A fireball flying between a pair of yellow statues on the cliff's high east
 */
/* wall hits Felix as he climbs: he turns grey among dark puffs, falls to the */
/* foot of the wall with a thump that shakes the ground, and shakes his head. */
void scMagmaRockKnockLeaderOffWall(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	EFFECT_OPTIONS options;
	s32 position[3];
	s32 i;
	u32 phase;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetFlag(FLAG_LEADER_KNOCKED_DOWN);
	fdEventPlaySnd(SE_IMPACT);
	scFieldActorSetSprCol(fdActorGetById(MainRoutineStateWork.leaderActorId),
						  SPRITE_COLOR_GREY);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_LEADER_CLIMB);
	fdActorSetAnimationSpeedById(MainRoutineStateWork.leaderActorId,
								 ANIM_STEP_NORMAL * 2);
	options.pri = 1;
	options.pal = SPRITE_COLOR_BLACK;
	options.startScaleX = FIXED_ONE * 4 / 5;
	options.startScaleY = FIXED_ONE * 4 / 5;
	for(i = 0; i != 8; i++) {
		position[0] = toFIXED(syRandom() & 15) - toFIXED(8);
		position[1] = (syRandom() & 15) << 16;
		scFieldEffectSpawn(
			leader->x.fixed + position[0], leader->y.fixed + position[1],
			leader->z.fixed, 0, toFIXED(6) / 5, 0,
			EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
			&options);
		sySynch(5);
	}
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	leader->gravity = toFIXED(4) / 5;
	leader->bounce = 0;
	fdEventWait(1);
	leader->facing = FACING_NORTH;
	/* He falls for up to 60 frames, until he lands, a dark puff rising round
	 * him */
	/* every other frame. */
	i = 60;
	do {
		phase = FrameCount & 1;
		if(phase == 0) {
			position[0] = toFIXED(syRandom() & 15) - toFIXED(8);
			position[1] = (syRandom() & 15) << 16;
			scFieldEffectSpawn(leader->x.fixed + position[0],
							   leader->y.fixed + position[1], leader->z.fixed,
							   0, toFIXED(6) / 5, phase,
							   EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
								   EFFECT_USE_PALETTE,
							   &options);
		}
		sySynch(1);
		if(leader->velY == 0)
			break;
		i--;
	} while(i != 0);
	fdEventPlaySnd(SE_THUMP);
	fdActorSetSprFlags(leader, 1);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId,
						ANIM_LEADER_LEAN_BACK);
	fdMapGridSetShake(5, 8, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdCameraMoveToActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_SHAKE_HEAD);
	scFieldActorSetSprCol(fdActorGetById(MainRoutineStateWork.leaderActorId),
						  SPRITE_COLOR_NORMAL);
	fdActorWaitForAnimationChange(MainRoutineStateWork.leaderActorId);
	fdEventClearFlag(FLAG_LEADER_KNOCKED_DOWN);
	leader->gravity = toFIXED(1);
	leader->bounce = FIXED_ONE / 4;
	scMagmaRockEndEvent();
}

void scMagmaRockInsideClearBlockCells(ACTOR *actor);
void scMagmaRockInsidePushBlockOnto(void);

/* Felix walks east into the striped stone pillar on the cliff's low west level
 */
/* from cell (14, 36) and pushes it a cell east, to cell (16, 36), where it */
/* stays when the party comes back. The game takes the pillar out of the map, */
/* clears the upper layer's walls along its path for the push */
/* (scMagmaRockInsidePushBlockOnto), puts them back and stands it in the map */
/* again. */
void scMagmaRockPushPillarEast(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	s32 position[3];
	ACTOR *actor;
	u8 *terrainId;
	s32 x;
	s32 z;
	s32 height;

	position[0] = leader->x.fixed + toFIXED(16);
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed;
	actor = fdActorFindInCell(position, leader);
	scMagmaRockInsideClearBlockCells(actor);
	terrainId = &actor->layer;
	x = FIELD_CELL(actor->x.fixed);
	z = FIELD_CELL(actor->z.fixed);
	height = CELL_HEIGHT_STEPS(actor->groundHeight);
	scMagmaRockInsideSetCellShape(*terrainId, x, z, height);
	fdMapGridCopyCellAttributes(79, 46, 2, 1, 79, 36);
	scMagmaRockInsidePushBlockOnto();
	*terrainId = 0;
	fdMapGridCopyCellAttributes(79, 37, 2, 1, 79, 36);
	scMagmaRockInsideSetCellShape(*terrainId, FIELD_CELL(actor->x.fixed),
								  FIELD_CELL(actor->z.fixed),
								  CELL_HEIGHT_STEPS(actor->groundHeight) + 4);
	scBlocksMarkCells(MagmaRock_SalamanderTailLowWestPillars);
	position[0] = FIELD_CELL(actor->x.fixed);
	position[2] = FIELD_CELL(actor->z.fixed);
	if(position[0] == 16 && position[2] == 36)
		fdEventSetFlag(FLAG_LOW_WEST_PILLAR_PUSHED);
}

/* Felix walks west into the striped stone pillar beside the cracked pillar on
 */
/* the cliff's low east level, from any of cells (37, 38) to (40, 38), and */
/* pushes it a cell west along a grey track; after four pushes it stands on the
 */
/* grey square at the track's end, cell (35, 38), and stays there when the */
/* party comes back. The game moves the pillar as scMagmaRockPushPillarEast */
/* does. */
void scMagmaRockPushPillarWest(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	s32 position[3];
	ACTOR *actor;
	u8 *terrainId;
	s32 x;
	s32 z;
	s32 height;

	position[0] = leader->x.fixed - toFIXED(16);
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed;
	actor = fdActorFindInCell(position, leader);
	scMagmaRockInsideClearBlockCells(actor);
	terrainId = &actor->layer;
	x = FIELD_CELL(actor->x.fixed);
	z = FIELD_CELL(actor->z.fixed);
	height = CELL_HEIGHT_STEPS(actor->groundHeight);
	scMagmaRockInsideSetCellShape(*terrainId, x, z, height);
	fdMapGridCopyCellAttributes(94, 44, 5, 1, 99, 38);
	scMagmaRockInsidePushBlockOnto();
	*terrainId = 0;
	fdMapGridCopyCellAttributes(99, 42, 5, 1, 99, 38);
	scMagmaRockInsideSetCellShape(*terrainId, FIELD_CELL(actor->x.fixed),
								  FIELD_CELL(actor->z.fixed),
								  CELL_HEIGHT_STEPS(actor->groundHeight) + 4);
	scBlocksMarkCells(MagmaRock_SalamanderTailLowEastPillars);
	position[0] = FIELD_CELL(actor->x.fixed);
	position[2] = FIELD_CELL(actor->z.fixed);
	if(position[0] == 35 && position[2] == 38)
		fdEventSetFlag(FLAG_LOW_EAST_PILLAR_B_PUSHED);
}

/* Growth on the west one of the two sprouts on the north-east ledge of the */
/* cliff's low east level (cell (59, 39)) grows a vine up the wall above it. */
/* The game opens the three cells of wall above the sprout for climbing and */
/* sets `FLAG_VINE_A_GROWN`. */
void scMagmaRockGrowVineA(void) {
	fdMapGridCopyCellAttributes(121, 48, 1, 3, 123, 48);
	fdEventSetFlag(FLAG_VINE_A_GROWN);
}

/* Growth on the east one of the two sprouts on the north-east ledge of the */
/* cliff's low east level (cell (61, 39)) grows a vine up the wall above it. */
/* The game opens the three cells of wall above the sprout for climbing and */
/* sets `FLAG_VINE_B_GROWN`. */
void scMagmaRockGrowVineB(void) {
	fdMapGridCopyCellAttributes(121, 48, 1, 3, 125, 48);
	fdEventSetFlag(FLAG_VINE_B_GROWN);
}

/* Growth on the sprout on the path where Felix comes up from the slide, on */
/* the cliff's low east level (cell (45, 41)), grows a vine up the wall above */
/* it. The game opens the three cells of wall above the sprout for climbing and
 */
/* sets `FLAG_VINE_C_GROWN`. */
void scMagmaRockGrowVineC(void) {
	fdMapGridCopyCellAttributes(108, 56, 1, 3, 109, 56);
	fdEventSetFlag(FLAG_VINE_C_GROWN);
}

/* Four large flame puffs scatter round (x, z) in the blast at the great fire,
 */
/* and the way in stays open for good. */
void scMagmaRockScatterEmbers(s32 x, s32 z) {
	EFFECT_OPTIONS config;
	s32 position[3];
	s32 i;
	s32 kind;

	config.startScaleX = FIXED_ONE * 2;
	config.startScaleY = FIXED_ONE * 2;
	config.type = SPRITE_FLAME_PUFF;
	kind = 1;
	config.script = MagmaRock_EmberScript;
	config.pri = kind;
	for(i = 0; i != 4; i++) {
		position[0] = x + toFIXED(syRandom() & 63) - FIXED_ONE * 32;
		position[1] = (syRandom() & 15) << 12;
		position[2] = z + toFIXED(syRandom() & 63) - FIXED_ONE * 32;
		scFieldEffectSpawn(position[0], 0, position[2], 0, position[1], 0,
						   EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
							   EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | 1,
						   &config);
	}
	fdEventSetFlag(FLAG_WAY_IN_BLASTED);
}

/* The great fire's update as Burst plays on it: it blinks white on the frames
 */
/* its mask picks. */
void scMagmaRockFlashTarget(ACTOR *actor) {
	if(FrameCount & actor->flashMask)
		scFieldActorSetSprCol(actor, SPRITE_COLOR_WHITE);
	else
		scFieldActorSetSprCol(actor, SPRITE_COLOR_NORMAL);
}

/* The statue's fireball explodes on the great fire on the top of Magma Rock: */
/* the fire blinks white as flame puffs scatter and rocks fly while the */
/* explosions shake the ground, and at the last blast the fire is gone and the
 */
/* rocks round it lie scattered, opening the way down into Magma Rock (cells */
/* (42, 17) to (46, 24)). The game copies the open ground and its pictures over
 */
/* those cells from spare ones below the room. */
void scMagmaRockBlastWayIn(s32 stage, s32 frame) {
	FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;

	if(frame == 0) {
		ACTOR *actor;

		scMagmaRockStartFallingRocks();
		actor = state->tgt;
		actor->flashMask = 4;
		actor->update = (void *)scMagmaRockFlashTarget;
	}
	if(stage == BURST_STAGE_SMALL_BLAST) {
		scMagmaRockScatterEmbers(state->tgt->x.fixed, state->tgt->z.fixed);
		scMagmaRockDropRocks(state->tgt->x.fixed,
							 state->tgt->y.fixed + toFIXED(15),
							 state->tgt->z.fixed, 1);
	}
	if(stage == BURST_STAGE_GREAT_BLAST) {
		ACTOR *target;
		s32 last = 8;

		scMagmaRockScatterEmbers(state->tgt->x.fixed, state->tgt->z.fixed);
		scMagmaRockScatterEmbers(state->tgt->x.fixed, state->tgt->z.fixed);
		scMagmaRockScatterEmbers(state->tgt->x.fixed,
								 state->tgt->z.fixed - toFIXED(32));
		scMagmaRockDropRocks(state->tgt->x.fixed,
							 state->tgt->y.fixed + toFIXED(15),
							 state->tgt->z.fixed, 16);
		target = state->tgt;
		SET_ACTOR_POSITION(target, 0, 0, 0);
		fdMapGridCopyCellAttributes(0, 32, 5, 8, 42, 17);
		fdMapGridCopyCellAttributes(0, 96, 5, 8, 42, 81);
		fdMapGridCopyCellsTo(0, 32, 42, 17, 5, last);
		fdMapGridCopyCellsTo(0, 96, 42, 81, 5, last);
	}
	if(frame == PSYNERGY_EVENT_LAST_FRAME)
		syRockEndFalling();
}

extern TRIGGER MagmaRock_Triggers[];
extern TRIGGER MagmaRock_SalamanderTailHighWestTriggers[];
extern TRIGGER MagmaRock_SalamanderTailHighEastTriggers[];
extern TRIGGER MagmaRock_SalamanderTailLowWestTriggers[];
extern TRIGGER MagmaRock_SalamanderTailLowEastTriggers[];
extern TRIGGER MagmaRock_StatueTriggers[];

const TRIGGER *scGetTriggers(void) {
	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
		if(MainRoutineStateWork.entrance <=
		   SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE)
			return MagmaRock_SalamanderTailHighWestTriggers;
		if(MainRoutineStateWork.entrance <=
		   SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE)
			return MagmaRock_SalamanderTailHighEastTriggers;
		if(MainRoutineStateWork.entrance <=
		   SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE)
			return MagmaRock_SalamanderTailLowWestTriggers;
		return MagmaRock_SalamanderTailLowEastTriggers;
	}
	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_STATUE)
		return MagmaRock_StatueTriggers;
	return MagmaRock_Triggers;
}

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_BURST_SLIDE_OR_CLIFF_PILLAR
#undef DEFINE_BURST_CRACKED_PILLAR
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY

void fdEffectAdvanceMotion();

/* Felix arrives on the cliff's high east level by dropping from the top */
/* (MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_EAST_A): he falls */
/* in from 130 pixels up, lands with a thump in a ring of dust and leans back.
 */
void scMagmaRockLaunchLeader(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	EFFECT_OPTIONS options;
	u32 i;
	s32 angle;
	FIXED velocity[3];

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
	leader->y.fixed += toFIXED(130);
	leader->gravity = toFIXED(1) / 2;
	leader->bounce = 0;
	leader->motionFlg = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventPlaySnd(SE_DUST_PUFF);
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syFieldEventWait(24);
	options.pal = SPRITE_COLOR_WHITE;
	options.update = fdEffectAdvanceMotion;
	options.startScaleX = FIXED_ONE * 4 / 5;
	options.startScaleY = FIXED_ONE * 4 / 5;
	for(i = 0; i <= DUST_RING_PUFFS; i++) {
		angle = i * (TRIG_TURN / DUST_RING_PUFFS);
		velocity[0] = syCos(angle);
		velocity[1] = 0;
		velocity[2] = syMathSin(angle);
		velocity[0] += velocity[0] >> 1;
		velocity[0] += syRandomBelow(toFIXED(3) / 4) - toFIXED(3) / 8;
		velocity[2] += syRandomBelow(toFIXED(3) / 8) - toFIXED(3) / 16;
		scFieldEffectSpawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
						   velocity[0], velocity[1], velocity[2],
						   EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
							   EFFECT_USE_PALETTE | 1,
						   &options);
	}
	fdEventPlaySnd(SE_THUMP);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId,
						ANIM_LEADER_LEAN_BACK);
	fdMapGridSetShake(5, 5, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	syFieldMapGridWaitForShakeToSettle();
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	leader->gravity = toFIXED(1);
	leader->bounce = FIXED_ONE / 4;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	scMagmaRockEndEvent();
}

#include "shared/leader_lift.h"

s32 fdMapGetTerrainHeight(s32 terrain, s32 x, s32 z);

/* Arriving with the party's place kept: sets the leader on the ground under */
/* it. */
void scMagmaRockLandLeader(void) {
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);

	if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		s32 height;

		syWait(1);
		height =
			fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
		actor->velY = 0;
		actor->groundHeight = height;
		actor->y.fixed = height;
		actor->tgtY = ACTOR_NO_TARGET;
		syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 0);
	}
}

/* Hangs a part of a flame column on its statue at the given level */
/* (scMagmaRockRaiseFlamePart). */
static inline void AttachPart(ACTOR *part, ACTOR *anchor, u8 order) {
	part->flameLevel = order;
	part->watchedActor = (FIELD_OBJECT *)anchor;
	part->update = scMagmaRockRaiseFlamePart;
	scFieldActorSetSprCol(part, SPRITE_COLOR_BGR);
}

/* Builds the four parts of a statue's flame column on re-entry, as */
/* scMagmaRockBurnStatue does. */
static inline void BuildParts(ACTOR *anchor) {
	ACTOR *parts[4];
	ACTOR *part;
	SPR *first;
	SPR *sprite;
	u32 i;

	parts[0] = ((ACTOR *)syImportActorCreate(
		SPRITE_STEAM_VENT, anchor->x.fixed + toFIXED(1),
		anchor->y.fixed + toFIXED(32), anchor->z.fixed));
	for(i = 1; i != 4; i++) {
		first = parts[0]->spr;
		part = ((ACTOR *)syImportActorCreate(SPRITE_STEAM_VENT, 0, 0, 0));
		parts[i] = part;
		sprite = part->spr;
		sprite->slot = first->slot;
		sprite->retainCacheEntry = 1;
		part->x.fixed = parts[0]->x.fixed;
		part->y.fixed = parts[0]->y.fixed;
		part->z.fixed = parts[0]->z.fixed;
		part->motionFlg = 0;
		fdActorSetSprFlags(part, 0);
		fdActorSelectAnimation(parts[i], 3);
		AttachPart(parts[i], anchor, i - 1);
	}
	part = parts[0];
	part->motionFlg = 0;
	fdActorSetSprFlags(part, 0);
	fdActorSelectAnimation(parts[0], 3);
	scSetActorSprPriority(parts[0], 3);
	AttachPart(parts[0], anchor, 3);
}

/* On entry: rebuilds the flame column under a statue's cap that was riding */
/* one when Felix left. */
void scMagmaRockRebuildFlameParts(u16 id) {
	if(fdActorGet(id)->update != scMagmaRockRaiseFlameTop)
		return;
	BuildParts(&fdActorGet(id)->watchedActor->actor);
}

void scMagmaRockSettleBlocks();
/* The overlay's import stub to fdRopePlaceStub. */
void scMagmaRockPlaceRope();

s32 scInit(void) {
	MAIN_ROUTINE_STATE *state;
	s32 scene;
	s32 *owner, *leader;
	s16 *entrance;
	ACTOR *actor;
	FIXED nudge, rise;
	FIXED offset;
	s32 priorityBit, destinationX, destinationColumn, width;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	state = &MainRoutineStateWork;
	state->retreatScene = SCENE_MAGMA_ROCK_SLIDE;
	state->retreatEntrance = MAGMA_ROCK_SLIDE_FROM_WORLD_MAP;
	scene = state->next;
	if(scene == SCENE_MAGMA_ROCK_SLIDE) {
		priorityBit = ACTOR_PRIORITY_IGNORES_RISES;
		fdActorGetById(state->leaderActorId)->priFlg |= priorityBit;
		fdActorGetById(ACTOR_SLIDE_PILLAR_A)->priFlg |= priorityBit;
		fdActorGetById(ACTOR_SLIDE_PILLAR_B)->priFlg |= priorityBit;
		scMagmaRockInsideRaiseActorCells(MagmaRock_SlideStatues);
		if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
			scMagmaRockPlaceFireball(ACTOR_SLIDE_FIREBALL,
									 ACTOR_SLIDE_PAIR_WEST_STATUE, 0);
		if(fdEventGetFlag(FLAG_SLIDE_PILLAR_A_PUSHED))
			fdActorSetPos(ACTOR_SLIDE_PILLAR_A, 264, 360);
		if(fdEventGetFlag(FLAG_SLIDE_PILLAR_B_PUSHED))
			fdActorSetPos(ACTOR_SLIDE_PILLAR_B, 248, 408);
		destinationX = 78;
		fdActorPlaceBlocks(MagmaRock_SlidePillars);
		fdMapGridCopyCellAttributes(98, 21, 4, 3, destinationX, 21);
		fdMapGridCopyCellAttributes(98, 27, 4, 3, destinationX, 24);
		scBlocksMarkCells(MagmaRock_SlidePillars);
		if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
			actor = fdActorGetById(ACTOR_SLIDE_STATUE_CAP);
			actor->x.fixed += toFIXED(1);
			actor->motionFlg = 0;
			actor->collisionFlg = 0;
			actor->groundHeight = actor->y.fixed += toFIXED(24);
			fdActorSetSprFlags(actor, 0);
		}
		scMagmaRockRebuildFlameParts(ACTOR_SLIDE_STATUE_CAP);
		switch(state->entrance) {
		case MAGMA_ROCK_SLIDE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST:
		case MAGMA_ROCK_SLIDE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST:
			offset = toFIXED(16);
			goto move;
		}
		goto settle;
	}
	else if(scene == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
		owner = &state->leaderActorId;
		fdActorGetById(*owner)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
		entrance = &state->entrance;
		if(state->entrance <= SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE) {
			s32 id;

			scMagmaRockRestoreCrumblingPillars(MagmaRock_CrumblingPillars);
			scMagmaRockInsideRaiseActorCells(
				MagmaRock_SalamanderTailHighWestStatues);
			fdTightropeInit();
			scMagmaRockPlaceTightrope(0, ACTOR_HIGH_WEST_TIGHTROPE_WEST_POST,
									  ACTOR_HIGH_WEST_TIGHTROPE_EAST_POST);
			if(fdEventGetFlag(FLAG_HIGH_WEST_PILLAR_B_PUSHED))
				fdActorSetPos(ACTOR_HIGH_WEST_PILLAR_B, 440, 536);
			fdActorPlaceBlocks(MagmaRock_SalamanderTailHighWestPillars);
			id = *entrance;
			if(id == ENTRANCE_HIGH_WEST_FROM_BELOW_A) {
				if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
					actor = fdActorGetById(*owner);
					actor->z.fixed += toFIXED(16);
				}
				fdLeaderLift(toFIXED(-16));
			}
			else if(id == ENTRANCE_HIGH_WEST_FROM_BELOW_B) {
				if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
					actor = fdActorGetById(*owner);
					actor->z.fixed += toFIXED(16);
				}
				fdLeaderLift(toFIXED(-96));
			}
		}
		else {
			if(*entrance <= SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE) {
				s32 id;

				if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
					fdActorGetById(ACTOR_STATUE_PAIR_A_WEST)->motionFlg = 0;
					rise = toFIXED(80);
					fdActorGetById(ACTOR_STATUE_PAIR_A_WEST)->y.fixed += rise;
					fdActorGetById(ACTOR_STATUE_PAIR_A_EAST)->motionFlg = 0;
					fdActorGetById(ACTOR_STATUE_PAIR_A_EAST)->y.fixed += rise;
					scMagmaRockPlaceFireball(ACTOR_FIREBALL_A,
											 ACTOR_STATUE_PAIR_A_WEST, 0);
					fdActorGetById(ACTOR_STATUE_PAIR_B_WEST)->motionFlg = 0;
					rise = toFIXED(160);
					fdActorGetById(ACTOR_STATUE_PAIR_B_WEST)->y.fixed += rise;
					fdActorGetById(ACTOR_STATUE_PAIR_B_EAST)->motionFlg = 0;
					fdActorGetById(ACTOR_STATUE_PAIR_B_EAST)->y.fixed += rise;
					scMagmaRockPlaceFireball(ACTOR_FIREBALL_B,
											 ACTOR_STATUE_PAIR_B_WEST, 1);
					fdActorGetById(ACTOR_STATUE_PAIR_C_WEST)->motionFlg = 0;
					rise = toFIXED(240);
					fdActorGetById(ACTOR_STATUE_PAIR_C_WEST)->y.fixed += rise;
					fdActorGetById(ACTOR_STATUE_PAIR_C_EAST)->motionFlg = 0;
					fdActorGetById(ACTOR_STATUE_PAIR_C_EAST)->y.fixed += rise;
					scMagmaRockPlaceFireball(ACTOR_FIREBALL_C,
											 ACTOR_STATUE_PAIR_C_WEST, 0);
				}
				if(fdEventGetFlag(FLAG_HIGH_EAST_PILLAR_PUSHED))
					fdActorSetPos(ACTOR_HIGH_EAST_PILLAR, 888, 568);
				fdActorPlaceBlocks(MagmaRock_SalamanderTailHighEastPillars);
				scMagmaRockInsideRaiseActorCells(
					MagmaRock_SalamanderTailHighEastStatues);
				if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
					actor = fdActorGetById(ACTOR_HIGH_EAST_STATUE_CAP_A);
					nudge = toFIXED(1);
					actor->x.fixed += nudge;
					actor->motionFlg = 0;
					actor->collisionFlg = 0;
					actor->groundHeight = actor->y.fixed += toFIXED(24);
					fdActorSetSprFlags(actor, 0);
					actor = fdActorGetById(ACTOR_HIGH_EAST_STATUE_CAP_B);
					actor->x.fixed += nudge;
					actor->motionFlg = 0;
					actor->collisionFlg = 0;
					actor->groundHeight = actor->y.fixed += toFIXED(24);
					fdActorSetSprFlags(actor, 0);
				}
				scMagmaRockRebuildFlameParts(ACTOR_HIGH_EAST_STATUE_CAP_A);
				scMagmaRockRebuildFlameParts(ACTOR_HIGH_EAST_STATUE_CAP_B);
				id = *entrance;
				if(id ==
				   MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_WEST) {
					fdLeaderLift(toFIXED(288));
				}
				else if(
					id ==
					MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_EAST_A) {
					if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
						scMagmaRockLaunchLeader();
				}
				else if(id == ENTRANCE_HIGH_EAST_FROM_BELOW) {
					if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
						actor = fdActorGetById(*owner);
						actor->z.fixed += toFIXED(16);
					}
					fdLeaderLift(toFIXED(-48));
				}
				else if(
					id ==
					MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_EAST_B) {
					offset = toFIXED(104);
					goto move;
				}
			}
			else if(*entrance <= SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE) {
				if(fdEventGetFlag(FLAG_LOW_WEST_PILLAR_PUSHED))
					fdActorSetPos(ACTOR_LOW_WEST_PILLAR, 264, 584);
				fdActorPlaceBlocks(MagmaRock_SalamanderTailLowWestPillars);
				scBlocksMarkCells(MagmaRock_SalamanderTailLowWestPillars);
				fdRopeInit();
				Call4((void (*)())scMagmaRockPlaceRope, 0,
					  FLAG_LOW_WEST_ROPE_LASHED, ACTOR_LOW_WEST_ROPE_EAST_POST,
					  ACTOR_LOW_WEST_ROPE_WEST_POST);
				scMagmaRockInsideRaiseActorCells(
					MagmaRock_SalamanderTailLowWestStatues);
				if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
					actor = fdActorGetById(ACTOR_LOW_WEST_STATUE_CAP_A);
					nudge = toFIXED(1);
					actor->x.fixed += nudge;
					actor->motionFlg = 0;
					actor->collisionFlg = 0;
					actor->groundHeight = actor->y.fixed += toFIXED(24);
					fdActorSetSprFlags(actor, 0);
					actor = fdActorGetById(ACTOR_LOW_WEST_STATUE_CAP_B);
					actor->x.fixed += nudge;
					actor->motionFlg = 0;
					actor->collisionFlg = 0;
					actor->groundHeight = actor->y.fixed += toFIXED(24);
					fdActorSetSprFlags(actor, 0);
				}
				scMagmaRockRebuildFlameParts(ACTOR_LOW_WEST_STATUE_CAP_A);
				scMagmaRockRebuildFlameParts(ACTOR_LOW_WEST_STATUE_CAP_B);
				if(*entrance == ENTRANCE_LOW_WEST_FROM_ABOVE_A ||
				   *entrance == ENTRANCE_LOW_WEST_FROM_ABOVE_B) {
					offset = toFIXED(48);
					goto move;
				}
			}
			else {
				scMagmaRockInsideRaiseActorCells(
					MagmaRock_SalamanderTailLowEastStatues);
				if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
					actor = fdActorGetById(ACTOR_LOW_EAST_STATUE_CAP);
					actor->x.fixed += toFIXED(1);
					actor->motionFlg = 0;
					actor->collisionFlg = 0;
					actor->groundHeight = actor->y.fixed += toFIXED(24);
					fdActorSetSprFlags(actor, 0);
				}
				scMagmaRockRebuildFlameParts(ACTOR_LOW_EAST_STATUE_CAP);
				if(fdEventGetFlag(FLAG_LOW_EAST_PILLAR_A_PUSHED))
					fdActorSetPos(ACTOR_LOW_EAST_PILLAR_A, 648, 600);
				if(fdEventGetFlag(FLAG_LOW_EAST_PILLAR_B_PUSHED))
					fdActorSetPos(ACTOR_LOW_EAST_PILLAR_B, 568, 616);
				scMagmaRockSettleBlocks(MagmaRock_SalamanderTailLowEastPillars);
				if(fdEventGetFlag(FLAG_VINE_A_GROWN))
					fdMapGridCopyCellAttributes(121, 48, 1, 3, 123, 48);
				if(fdEventGetFlag(FLAG_VINE_B_GROWN))
					fdMapGridCopyCellAttributes(121, 48, 1, 3, 125, 48);
				if(fdEventGetFlag(FLAG_VINE_C_GROWN))
					fdMapGridCopyCellAttributes(108, 56, 1, 3, 109, 56);
				if(*entrance == ENTRANCE_LOW_EAST_FROM_ABOVE) {
					fdLeaderLift(toFIXED(48));
				}
				else if(
					*entrance ==
						MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_SLIDE_WEST ||
					*entrance ==
						MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_SLIDE_EAST) {
					if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
						actor = fdActorGetById(state->leaderActorId);
						actor->z.fixed += toFIXED(16);
					}
					offset = -toFIXED(32);
				move:
					fdLeaderLift(offset);
				}
			}
		settle:
			scMagmaRockLandLeader();
		}
	}
	else if(scene == SCENE_MAGMA_ROCK_STATUE) {
		if(fdEventGetFlag(FLAG_WAY_IN_BLASTED)) {
			fdActorSetPos(ACTOR_GREAT_FIRE, 0, 0);
			destinationColumn = 42;
			fdMapGridCopyCellAttributes(0, 32, 5, 8, destinationColumn, 17);
			fdMapGridCopyCellAttributes(0, 96, 5, 8, destinationColumn, 81);
			fdMapGridCopyCellsTo(0, 32, 42, 17, 5, 8);
			fdMapGridCopyCellsTo(0, 96, 42, 81, 5, 8);
		}
		leader = &state->leaderActorId;
		fdActorGetById(*leader)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
		scMagmaRockInsideRaiseActorCells(MagmaRock_StatueStatues);
		if(state->entrance <=
		   MAGMA_ROCK_STATUE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_B) {
			if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
				actor = fdActorGetById(*leader);
				actor->z.fixed += toFIXED(16);
			}
			fdLeaderLift(toFIXED(-32));
		}
	}
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* On entry: sets each listed pillar from its two flags (cracked, then fallen),
 */
/* then lifts the leader onto the ground under it. */
void scMagmaRockRestoreCrumblingPillars(const u16 *table) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	ACTOR *actor;
	s32 id;
	s32 flag;
	s32 nextFlag;
	MAP_CELL *cells;
	u8 *shapes;
	MAP_CELL *cell;
	s32 index;
	ACTOR *leader;
	s32 ground;

	for(; *table != ACTOR_LIST_END; table += 2) {
		id = table[0];
		actor = fdActorGet(id);
		flag = table[1];
		nextFlag = flag + 1;
		cells = map->layers[actor->layer].cells;
		shapes = MapShapeGrid + (cells - (MAP_CELL *)Ram_MapCellBuffer);
		if(fdEventGetFlag(nextFlag)) {
			fdActorSetPos(id, 0, 0);
		}
		else {
			s32 shape;
			u8 *slot;
			s32 height;

			index = fdActorGetCellIndex(actor);
			cell = &cells[index];
			slot = shapes + index;
			shape = fdMapGridGetTerrainKind(actor->layer, actor->x.fixed,
											actor->z.fixed);
			height = fdMapGetTerrainHeight(actor->layer, actor->x.fixed,
										   actor->z.fixed);
			fdActorDisableById(id);
			cell->attributeB = MAP_CELL_JUMP;
			*slot = fdMapShapeFindOrAdd(actor->layer, shape,
										CELL_HEIGHT_STEPS(height) + 4);
			if(fdEventGetFlag(flag))
				fdActorSelectAnimation(actor, 0);
		}
	}
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ground =
		fdMapGetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
	if(leader->y.fixed < ground) {
		leader->groundHeight = ground;
		leader->y.fixed = ground;
	}
}

void scMagmaRockSpawnDustRing(void);

/* Felix on a crumbling pillar: the first time it cracks with a flash, the */
/* second time it crumbles and he drops with it, landing with a thump in a ring
 */
/* of dust. */
void scMagmaRockStepOnCrumblingPillar(u16 *list) {
	ACTOR *leader;
	ACTOR *target;
	FIXED position[3];

	if(fdTryJumpForward() != 0)
		return;
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	position[0] = leader->x.fixed;
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed;
	target = fdActorFindAtCell(position, 0);
	if(target == NULL)
		return;
	while(list[0] != ACTOR_LIST_END) {
		s32 id = list[0];

		if(fdActorGetById(id) != target) {
			list += 2;
			continue;
		}
		{
			s32 flag = list[1];

			fdEventStart();
			if(!fdEventGetFlag(flag)) {
				fdEventPlaySnd(SE_CHEST_CLOSE);
				scFieldActorSetSprCol(fdActorGetById(id), SPRITE_COLOR_WHITE);
				sySynch(2);
				fdActorSelectAnimation(target, 0);
				scFieldActorSetSprCol(fdActorGetById(id), SPRITE_COLOR_NORMAL);
				sySynch(2);
				scFieldActorSetSprCol(fdActorGetById(id), SPRITE_COLOR_WHITE);
				sySynch(4);
				scFieldActorSetSprCol(fdActorGetById(id), SPRITE_COLOR_NORMAL);
				scDustScatter(0);
				fdEventSetFlag(flag);
			}
			else if(!fdEventGetFlag(flag + 1)) {
				leader->motionFlg = 0;
				fdEventPlaySnd(SE_PUSH);
				fdMapGridSetShake(1, 4, 1);
				scDustScatter(0);
				sySynch(8);
				fdActorSelectAnimation(target, 2);
				fdLeaderSetCellShapeAt(-2, target->layer);
				scDustScatter(1);
				sySynch(16);
				fdLeaderSetCellShapeAt(-2, target->layer);
				sySynch(4);
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				sySynch(8);
				leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
				sySynch(5);
				fdActorSetPosAndResetMotion(target, 0, 0, 0);
				sySynch(2);
				fdEventPlaySnd(SE_THUMP);
				scMagmaRockSpawnDustRing();
				sySynch(20);
				fdEventSetFlag(flag + 1);
			}
		}
		scMagmaRockEndEvent();
		return;
	}
}

/* Unused: a crumbling pillar named in the (actor, flag) table gives way with a
 */
/* shake, dust and a thump. */
UNUSED void scMagmaRockCrumblePillar(MAGMA_ROCK_CRUMBLING_PILLAR *entry,
									 s32 id) {
	ACTOR *target;
	s32 flag;
	s32 nextFlag;

	target = fdActorGet(id);
	while(entry->id != ACTOR_LIST_END) {
		if(entry->id != id) {
			entry++;
			continue;
		}
		flag = entry->flg;
		fdEventStart();
		nextFlag = flag + 1;
		if(fdEventGetFlag(nextFlag) == 0) {
			fdEventPlaySnd(SE_PUSH);
			fdMapGridSetShake(1, 4, 1);
			scDustScatter(0);
			syWait(8);
			fdActorSelectAnimation(target, 2);
			fdActorSetCellShape(-4, target->layer, id);
			scDustScatter(1);
			syWait(16);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			syWait(8);
			scFieldActorSetPosAndResetMotion(target, 0, 0, 0);
			syWait(2);
			fdEventPlaySnd(SE_THUMP);
			scMagmaRockSpawnDustRing();
			syWait(20);
			fdEventSetFlag(flag);
			fdEventSetFlag(nextFlag);
		}
		scMagmaRockEndEvent();
		return;
	}
}

#include "shared/pillar_take_out_of_play.c"

#include "shared/pillar_restore.h"

#include "shared/actor_take_place_of.h"

/* Unused: stepping onto a crumbling floor: the first time it shakes and */
/* cracks, the second time it gives way with the leader. */
UNUSED void scMagmaRockStepOnTrap(u16 *list) {
	ACTOR *leader;
	ACTOR *firstPlaced;
	ACTOR *target;
	FIXED position[3];

	if(fdTryJumpForward() != 0)
		return;
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	firstPlaced = fdActorGetById(ACTOR_FIRST_PLACED);
	position[0] = leader->x.fixed;
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed;
	target = fdActorFindAtCell(position, 0);
	if(target == NULL)
		return;
	while(*list != ACTOR_LIST_END) {
		u32 id = list[0];

		if(fdActorGetById(id) != target) {
			list += 2;
			continue;
		}
		{
			s32 flag = list[1];

			fdEventStart();
			if(!fdEventGetFlag(flag)) {
				fdActorSelectAnimation(firstPlaced, 1);
				fdActorTakePlaceOf(firstPlaced, target);
				sySynch(1);
				fdEventPlaySnd(SE_CHEST_CLOSE);
				scFieldActorSetSprCol(fdActorGetById(ACTOR_FIRST_PLACED),
									  SPRITE_COLOR_WHITE);
				sySynch(2);
				fdActorSelectAnimation(firstPlaced, 0);
				fdActorCopyGraphics(id, ACTOR_FIRST_PLACED + 1);
				scFieldActorSetSprCol(fdActorGetById(ACTOR_FIRST_PLACED),
									  SPRITE_COLOR_NORMAL);
				sySynch(2);
				scFieldActorSetSprCol(fdActorGetById(ACTOR_FIRST_PLACED),
									  SPRITE_COLOR_WHITE);
				sySynch(4);
				scFieldActorSetSprCol(fdActorGetById(ACTOR_FIRST_PLACED),
									  SPRITE_COLOR_NORMAL);
				scDustScatter(0);
				fdActorTakePlaceOf(target, firstPlaced);
				sySynch(1);
				fdEventSetFlag(flag);
			}
			else if(!fdEventGetFlag(flag + 1)) {
				u8 *flags;

				fdActorSelectAnimation(firstPlaced, 0);
				fdActorTakePlaceOf(firstPlaced, target);
				sySynch(1);
				flags = &leader->motionFlg;
				*flags = 0;
				fdEventPlaySnd(SE_PUSH);
				fdMapGridSetShake(1, 4, 1);
				scDustScatter(0);
				sySynch(8);
				fdActorSelectAnimation(firstPlaced, 2);
				fdLeaderSetCellShapeAt(-2, target->layer);
				scDustScatter(1);
				sySynch(16);
				fdLeaderSetCellShapeAt(-2, target->layer);
				sySynch(4);
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				sySynch(8);
				*flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
				sySynch(5);
				fdActorSetPosAndResetMotion(firstPlaced, 0, 0, 0);
				sySynch(2);
				fdEventPlaySnd(SE_THUMP);
				scMagmaRockSpawnDustRing();
				sySynch(20);
				fdEventSetFlag(flag + 1);
			}
			scMagmaRockEndEvent();
			return;
		}
	}
}

#include "../field/field_effect_advance_motion_priority.c"

#include "shared/dust_scatter.c"

/* Throws a ring of dust clouds out from Felix as he lands with a crumbled */
/* pillar. */
void scMagmaRockSpawnDustRing(void) {
	ACTOR *leader;
	FIELD_EFFECT *effect;
	s32 state;
	u32 i;
	s32 angle;

	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(leader == NULL)
		return;
	state = 0;
	for(i = 0; i <= DUST_RING_PUFFS; i++) {
		effect = (FIELD_EFFECT *)scFieldActorCreate(
			SPRITE_DUST_CLOUD, leader->x.fixed, leader->y.fixed,
			leader->z.fixed);
		if(effect != NULL) {
			state = fdSprShareVramCacheEntry(effect->spr, state);
			effect->motionFlg = 0;
			effect->collisionFlg = 0;
			fdActorSetSprFlags((ACTOR *)effect, 0);
			fdActorSelectAnimation(effect, 2);
			fdActorStartScript(effect, Dust_BurstScript);
			angle = i * (TRIG_TURN / DUST_RING_PUFFS);
			effect->vel.x = syCos(angle);
			effect->vel.y = 0;
			effect->vel.z = syMathSin(angle);
			effect->vel.x += effect->vel.x >> 1;
			effect->vel.x += (syRandomBelow(toFIXED(3) / 4)) - toFIXED(3) / 8;
			effect->vel.z += (syRandomBelow(toFIXED(3) / 8)) - toFIXED(3) / 16;
			effect->spin = (syRandomBelow(TRIG_TURN / 16)) - TRIG_TURN / 32;
			effect->scaleRateX = 0;
			effect->scaleRateY = 0;
			effect->update = fdEffectAdvanceMotionAtLeaderPriority;
			effect->spr->pri = ((FIELD_SPR *)leader->spr)->pri;
		}
	}
}

s32 fdMapGetTerrainHeight(s32 layer, s32 x, s32 z);

#include "shared/actor_set_cell_shape.h"

#include "shared/leader_set_cell_shape.h"

#include "shared/particle_update_falling.h"

/* Drops count rocks from (x, y, z), each flung out at a random angle. */
void scMagmaRockDropRocks(s32 x, s32 y, s32 z, s32 count) {
	ROCK_STATE *state;
	s32 i;

	state = Ram_HeapSlots->menuState;
	if(count > 0) {
		i = count;
		do {
			ROCK *rock;
			s32 angle = syRandom();

			rock = &state->rocks[state->next];
			rock->tmr = 0;
			rock->pos[0] = x;
			rock->pos[1] = y;
			rock->pos[2] = z;
			syFieldVectorAddPolarOffsetFX(syRandom() * 8 + toFIXED(8), angle,
										  rock->pos);
			rock->vel[0] = 0;
			rock->vel[1] = toFIXED(5);
			rock->vel[2] = 0;
			syFieldVectorAddPolarOffsetFX(syRandom() * 2 + toFIXED(2), angle,
										  rock->vel);
			{
				s32 mask = ROCK_COUNT - 1;

				state->next = (state->next + 1) & mask;
			}
			i--;
		} while(i != 0);
	}
}

/* The overlay's import stub to the tagged LZ decompressor. */
void syImportDecompressTaggedLz(void *entry, void *destination);

/* Starts the falling rocks: loads their picture, prepares a small sprite for */
/* each rock and adds the task that draws them. */
void scMagmaRockStartFallingRocks(void) {
	ROCK_STATE *state = syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(ROCK_STATE));
	s32 entry;
	s32 tile;
	s32 i;
	ROCK *rock;
	EFFECT_SPR *sprite;

	syImportDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_FALLING_ROCK),
							   state);
	entry = syVramCacheFindFreeEntry();
	tile = mnVramLoad(entry, sizeof(state->tiles), state);
	state->tileBase = tile;
	state->resource = entry;
	rock = state->rocks;
	for(i = 0; i < ROCK_COUNT; i++) {
		sprite = &state->sprs[i];
		fdEffectSprInit(sprite, 8, 8, OBJ_SIZE_16X16, tile);
		sprite->oam.fullCol = 1;
		sprite->oam.pal = SPRITE_COLOR_NORMAL;
		sprite->oam.pri = 0;
		sprite->depth = OAM_DEPTH_WINDOW;
		rock->tmr = -1;
		rock++;
	}
	state->next = 0;
	syTaskAddFrame(btParticleUpdateFalling, TASK_PASS_FRAME);
}

#include "shared/rock_end_falling.h"

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

#include "shared/push_leader_steps.c"

#include "shared/magma_rock_ember_scripts.c"

/* The pillars Move pushes on the slide, ended by -1. */
s16 MagmaRock_SlidePillars[] = {
    ACTOR_SLIDE_PILLAR_A, ACTOR_SLIDE_PILLAR_B, -1,
};

/* The slide's statues (syStatueFindUnderBurst): each row a statue, the cell */
/* Burst finds it by and the cap its flame column raises (-1 for none), ended */
/* by -1. */
s16 MagmaRock_SlideStatues[] = {
    ACTOR_SLIDE_COLUMN_STATUE, CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, ACTOR_SLIDE_STATUE_CAP,
    ACTOR_SLIDE_FIRING_STATUE, CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, -1,
    -1,
};

/* The pillars Move pushes on the cliff's high west level, ended by -1. */
s16 MagmaRock_SalamanderTailHighWestPillars[] = {
    ACTOR_HIGH_WEST_PILLAR_A, ACTOR_HIGH_WEST_PILLAR_B, -1,
};

/* The high west level's statues, as MagmaRock_SlideStatues. */
s16 MagmaRock_SalamanderTailHighWestStatues[] = {
    ACTOR_HIGH_WEST_FIRING_STATUE, CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, -1,
    -1,
};

/* The high west level's crumbling pillars: (actor, flag) rows ended by -1, the */
/* flag set once the pillar has cracked and the next once it has fallen. */
s16 MagmaRock_CrumblingPillars[] = {
    ACTOR_CRUMBLING_PILLAR_A, FLAG_CRUMBLING_PILLAR_A_CRACKED,
    ACTOR_CRUMBLING_PILLAR_B, FLAG_CRUMBLING_PILLAR_B_CRACKED,
    -1,
};

/* The high east level's statues, as MagmaRock_SlideStatues. */
s16 MagmaRock_SalamanderTailHighEastStatues[] = {
    ACTOR_HIGH_EAST_COLUMN_STATUE_A, CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, ACTOR_HIGH_EAST_STATUE_CAP_A,
    ACTOR_HIGH_EAST_COLUMN_STATUE_B, CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, ACTOR_HIGH_EAST_STATUE_CAP_B,
    -1,
};

/* The pillars Move pushes on the high east level, ended by -1. */
s16 MagmaRock_SalamanderTailHighEastPillars[] = {
    ACTOR_HIGH_EAST_PILLAR, -1,
};

/* The low west level's statues, as MagmaRock_SlideStatues. */
s16 MagmaRock_SalamanderTailLowWestStatues[] = {
    ACTOR_LOW_WEST_COLUMN_STATUE_A, CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, ACTOR_LOW_WEST_STATUE_CAP_A,
    ACTOR_LOW_WEST_COLUMN_STATUE_B, CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, ACTOR_LOW_WEST_STATUE_CAP_B,
    -1,
};

/* The pillars Move pushes on the low west level, ended by -1. */
s16 MagmaRock_SalamanderTailLowWestPillars[] = {
    ACTOR_LOW_WEST_PILLAR, -1,
};

/* The pillars Move pushes on the low east level, ended by -1. */
s16 MagmaRock_SalamanderTailLowEastPillars[] = {
    ACTOR_LOW_EAST_PILLAR_A, ACTOR_LOW_EAST_PILLAR_B, -1,
};

/* The low east level's statues, as MagmaRock_SlideStatues. */
s16 MagmaRock_SalamanderTailLowEastStatues[] = {
    ACTOR_LOW_EAST_FIRING_STATUE_A, CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, -1,
    ACTOR_LOW_EAST_COLUMN_STATUE, CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, ACTOR_LOW_EAST_STATUE_CAP,
    ACTOR_LOW_EAST_FIRING_STATUE_B, CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE_B, -1,
    -1,
};

/* The statue on the top, as MagmaRock_SlideStatues. */
s16 MagmaRock_StatueStatues[] = {
    ACTOR_STATUE_ROOM_STATUE, CELL_BEHAVIOR_MAGMA_ROCK_STATUE_ROOM_STATUE, -1,
    -1, 0,
};

#include "shared/dust_scatter_script.c"

#include "shared/dust_burst_script.c"

const s32 *FieldEffect_DurationScripts[] = {
    FieldEffect_ShortScript,
    FieldEffect_MediumScript,
    FieldEffect_LongScript,
};

#include "shared/scene_entrances_default.c"

/* The exit regions of the slide. */
SCENE_REGION MagmaRock_Regions[] = {
    {240, 328, 352, 256, 344, 368, CONDITION_ALWAYS, MAGMA_ROCK_SLIDE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST},
    {448, 328, 352, 464, 344, 368, CONDITION_ALWAYS, MAGMA_ROCK_SLIDE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST},
    {SCENE_TABLE_END},
};

/* The exit regions of the cliff. */
SCENE_REGION MagmaRock_SalamanderTailRegions[] = {
    {272, 56, 576, 288, 72, 592, CONDITION_ALWAYS, EXIT_HIGH_WEST_DOWN_A},
    {352, 40, 560, 368, 56, 576, CONDITION_ALWAYS, EXIT_HIGH_WEST_DOWN_B},
    {752, 512, 536, 768, 528, 552, CONDITION_ALWAYS, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_STATUE_WEST},
    {784, 80, 592, 800, 96, 608, CONDITION_ALWAYS, EXIT_HIGH_EAST_DOWN},
    {896, 512, 528, 912, 528, 544, CONDITION_ALWAYS, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_STATUE_EAST_B},
    {272, 48, 576, 288, 64, 592, CONDITION_ALWAYS, EXIT_LOW_WEST_UP_A},
    {352, 32, 560, 368, 48, 576, CONDITION_ALWAYS, EXIT_LOW_WEST_UP_B},
    {784, 56, 592, 800, 72, 608, CONDITION_ALWAYS, EXIT_LOW_EAST_UP},
    {752, -344, 672, 768, -328, 688, CONDITION_ALWAYS, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_SLIDE_WEST},
    {976, -344, 672, 992, -328, 688, CONDITION_ALWAYS, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_SLIDE_EAST},
    {SCENE_TABLE_END},
};

/* The exit regions of the top. */
SCENE_REGION MagmaRock_StatueRegions[] = {
    {240, -56, 448, 256, -40, 464, CONDITION_ALWAYS, MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST},
    {384, -56, 448, 400, -40, 464, CONDITION_ALWAYS, MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_B},
    {416, -128, 448, 432, -112, 464, CONDITION_ALWAYS, MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_A},
    {SCENE_TABLE_END},
};

u32 SceneExits[] = {
    SCENE_EXITS(MagmaRockSlide),
    SCENE_EXIT(MAGMA_ROCK_SLIDE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_MAGMA_ROCK_SLIDE),
    SCENE_EXIT(MAGMA_ROCK_SLIDE_TO_MAGMA_ROCK_INSIDE_EMBER_STONE, MagmaRockInsideEmberStone, MAGMA_ROCK_INSIDE_EMBER_STONE_FROM_MAGMA_ROCK_SLIDE),
    SCENE_EXIT(MAGMA_ROCK_SLIDE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST, MagmaRockSalamanderTailChest, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_SLIDE_WEST),
    SCENE_EXIT(MAGMA_ROCK_SLIDE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST, MagmaRockSalamanderTailChest, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_SLIDE_EAST),
    SCENE_EXITS(MagmaRockSalamanderTailChest),
    SCENE_EXIT(EXIT_HIGH_WEST_DOWN_A, MagmaRockSalamanderTailChest, ENTRANCE_LOW_WEST_FROM_ABOVE_A),
    SCENE_EXIT(EXIT_HIGH_WEST_DOWN_B, MagmaRockSalamanderTailChest, ENTRANCE_LOW_WEST_FROM_ABOVE_B),
    SCENE_EXIT(EXIT_HIGH_WEST_TO_HIGH_EAST, MagmaRockSalamanderTailChest, ENTRANCE_HIGH_EAST_FROM_HIGH_WEST),
    SCENE_EXIT(EXIT_HIGH_EAST_TO_HIGH_WEST, MagmaRockSalamanderTailChest, ENTRANCE_HIGH_WEST_FROM_HIGH_EAST),
    SCENE_EXIT(MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_STATUE_WEST, MagmaRockStatue, MAGMA_ROCK_STATUE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST),
    SCENE_EXIT(MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_STATUE_EAST_A, MagmaRockStatue, MAGMA_ROCK_STATUE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_B),
    SCENE_EXIT(EXIT_HIGH_EAST_DOWN, MagmaRockSalamanderTailChest, ENTRANCE_LOW_EAST_FROM_ABOVE),
    SCENE_EXIT(MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_STATUE_EAST_B, MagmaRockStatue, MAGMA_ROCK_STATUE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_B),
    SCENE_EXIT(EXIT_LOW_WEST_UP_A, MagmaRockSalamanderTailChest, ENTRANCE_HIGH_WEST_FROM_BELOW_A),
    SCENE_EXIT(EXIT_LOW_WEST_UP_B, MagmaRockSalamanderTailChest, ENTRANCE_HIGH_WEST_FROM_BELOW_B),
    SCENE_EXIT(EXIT_LOW_WEST_TO_LOW_EAST_A, MagmaRockSalamanderTailChest, ENTRANCE_LOW_EAST_FROM_LOW_WEST_A),
    SCENE_EXIT(EXIT_LOW_WEST_TO_LOW_EAST_B, MagmaRockSalamanderTailChest, ENTRANCE_LOW_EAST_FROM_LOW_WEST_B),
    SCENE_EXIT(EXIT_LOW_EAST_TO_LOW_WEST_A, MagmaRockSalamanderTailChest, ENTRANCE_LOW_WEST_FROM_LOW_EAST_A),
    SCENE_EXIT(EXIT_LOW_EAST_UP, MagmaRockSalamanderTailChest, ENTRANCE_HIGH_EAST_FROM_BELOW),
    SCENE_EXIT(EXIT_LOW_EAST_TO_LOW_WEST_B, MagmaRockSalamanderTailChest, ENTRANCE_LOW_WEST_FROM_LOW_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_SLIDE_WEST, MagmaRockSlide, MAGMA_ROCK_SLIDE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST),
    SCENE_EXIT(MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_SLIDE_EAST, MagmaRockSlide, MAGMA_ROCK_SLIDE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST),
    SCENE_EXITS(MagmaRockStatue),
    SCENE_EXIT(MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST, MagmaRockSalamanderTailChest, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_WEST),
    SCENE_EXIT(MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_B, MagmaRockSalamanderTailChest, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_INSIDE_ENTRANCE, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_STATUE),
    SCENE_EXIT(MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_A, MagmaRockSalamanderTailChest, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_EAST_A),
    SCENE_EXITS_END,
};

/* The actors placed in the slide. */
ACTOR_PLACE MagmaRock_Placements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 6, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 7, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 12, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STATUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_FLAME_PUFF, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The actors placed in the cliff's high west level. */
ACTOR_PLACE MagmaRock_SalamanderTailHighWestPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

/* The actors placed in the cliff's high east level. */
ACTOR_PLACE MagmaRock_SalamanderTailHighEastPlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 9, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 10, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 9, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 10, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 9, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 10, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STATUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 12, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 13, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STATUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 13, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_FLAME_PUFF, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_PUFF, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_PUFF, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The actors placed in the cliff's low west level. */
ACTOR_PLACE MagmaRock_SalamanderTailLowWestPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 14, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 15, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STATUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 15, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 16, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STATUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 16, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 17, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 18, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SCENE_TABLE_END},
};

/* The actors placed in the cliff's low east level. */
ACTOR_PLACE MagmaRock_SalamanderTailLowEastPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 19, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 20, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 21, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 22, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 23, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STATUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 23, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 25, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 26, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 27, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 28, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

/* The actors placed on the top. */
ACTOR_PLACE MagmaRock_StatuePlacements[] = {
    {SPRITE_GREAT_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};
/* clang-format on */

/* The triggers of the slide. */
TRIGGER MagmaRock_Triggers[] = {
	{TRIGGER_EXIT, MAGMA_ROCK_SLIDE_TO_WORLD_MAP, CONDITION_ALWAYS,
	 MAGMA_ROCK_SLIDE_TO_WORLD_MAP},
	{STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK | FACING_NORTH,
	 CELL_BEHAVIOR_MAGMA_ROCK_CAVE_DOOR, FLAG_CAVE_DOOR_USED,
	 EVENT(scMagmaRockEnterCave)},
	{TRIGGER_STEP, CELL_BEHAVIOR_MAGMA_ROCK_CAVE_DOOR, CONDITION_ALWAYS,
	 EVENT(scMagmaRockResetCaveDoor)},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_SLIDE_BOULDER, CONDITION_ALWAYS, 0},
	{TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scMagmaRockStartPushingPillar)},
	{TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
	 EVENT(scMagmaRockFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_SLIDE_PILLAR_A, FLAG_SLIDE_PILLAR_A_PUSHED,
	 EVENT(scMagmaRockStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SLIDE_PILLAR_A,
	 FLAG_SLIDE_PILLAR_A_PUSHED, EVENT(scMagmaRockFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_SLIDE_PILLAR_B, FLAG_SLIDE_PILLAR_B_PUSHED,
	 EVENT(scMagmaRockStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SLIDE_PILLAR_B,
	 FLAG_SLIDE_PILLAR_B_PUSHED, EVENT(scMagmaRockFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_A, FLAG_CRACKED_PILLAR_A_BURST,
	 EVENT(MagmaRock_BurstCrackedPillarA)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_B, FLAG_CRACKED_PILLAR_B_BURST,
	 EVENT(MagmaRock_BurstCrackedPillarB)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_C, FLAG_CRACKED_PILLAR_C_BURST,
	 EVENT(MagmaRock_BurstCrackedPillarC)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_D, FLAG_CRACKED_PILLAR_D_BURST,
	 EVENT(MagmaRock_BurstCrackedPillarD)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_E, FLAG_CRACKED_PILLAR_E_BURST,
	 EVENT(MagmaRock_BurstCrackedPillarE)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, FLAG_FIRE_BURST_PLAYING,
	 EVENT(scMagmaRockTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockBurstCrackedPillar)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockFireStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
	 EVENT(scMagmaRockResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
	 EVENT(scMagmaRockTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
	 EVENT(scMagmaRockIgniteStatue)},
	{TRIGGER_TABLE_END},
};

/* The triggers of the cliff's high west level. */
TRIGGER MagmaRock_SalamanderTailHighWestTriggers[] = {
	{TRIGGER_EXIT, EXIT_HIGH_WEST_TO_HIGH_EAST, CONDITION_ALWAYS,
	 EXIT_HIGH_WEST_TO_HIGH_EAST},
	{STEP_TRIGGER_WALKING, CELL_BEHAVIOR_MAGMA_ROCK_CRUMBLING_PILLAR,
	 CONDITION_ALWAYS, EVENT(scMagmaRockCheckCrumblingPillars)},
	{TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scMagmaRockStartPushingPillar)},
	{TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
	 EVENT(scMagmaRockFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_HIGH_WEST_PILLAR_A, CONDITION_ALWAYS,
	 EVENT(scMagmaRockStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HIGH_WEST_PILLAR_A,
	 CONDITION_ALWAYS, EVENT(scMagmaRockFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_HIGH_WEST_PILLAR_B, FLAG_HIGH_WEST_PILLAR_B_PUSHED,
	 EVENT(scMagmaRockStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HIGH_WEST_PILLAR_B,
	 FLAG_HIGH_WEST_PILLAR_B_PUSHED, EVENT(scMagmaRockFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_A, FLAG_CRACKED_PILLAR_A_BURST,
	 EVENT(MagmaRock_BurstCrackedPillarA)},
	{TRIGGER_STEP, CELL_BEHAVIOR_MAGMA_ROCK_BESIDE_HIGH_WEST_STATUE,
	 FLAG_HIGH_WEST_STATUE_READY, EVENT(scMagmaRockReadyHighWestStatue)},
	{TRIGGER_STEP, CELL_BEHAVIOR_MAGMA_ROCK_PAST_HIGH_WEST_STATUE,
	 CONDITION_ALWAYS, EVENT(scMagmaRockResetHighWestStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, FLAG_FIRE_BURST_PLAYING,
	 EVENT(scMagmaRockTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockBurstCrackedPillar)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockFireStatue)},
	{TRIGGER_TABLE_END},
};

/* The triggers of the cliff's high east level. */
TRIGGER MagmaRock_SalamanderTailHighEastTriggers[] = {
	{TRIGGER_EXIT, EXIT_HIGH_EAST_TO_HIGH_WEST, CONDITION_ALWAYS,
	 EXIT_HIGH_EAST_TO_HIGH_WEST},
	{TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scMagmaRockStartPushingPillar)},
	{TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
	 EVENT(scMagmaRockFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_HIGH_EAST_PILLAR, FLAG_HIGH_EAST_PILLAR_PUSHED,
	 EVENT(scMagmaRockStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HIGH_EAST_PILLAR,
	 FLAG_HIGH_EAST_PILLAR_PUSHED, EVENT(scMagmaRockFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
	 EVENT(scMagmaRockResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
	 EVENT(scMagmaRockTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
	 EVENT(scMagmaRockIgniteStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, CONDITION_ALWAYS,
	 EVENT(scMagmaRockResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, CONDITION_ALWAYS,
	 EVENT(scMagmaRockTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, CONDITION_ALWAYS,
	 EVENT(scMagmaRockIgniteStatue)},
	{TRIGGER_RAISED, TRIGGER_FIREBALL_HITS_LEADER, CONDITION_ALWAYS,
	 EVENT(scMagmaRockKnockLeaderOffWall)},
	{TRIGGER_TABLE_END},
};

/* The triggers of the cliff's low west level. */
TRIGGER MagmaRock_SalamanderTailLowWestTriggers[] = {
	{TRIGGER_EXIT, EXIT_LOW_WEST_TO_LOW_EAST_A, CONDITION_ALWAYS,
	 EXIT_LOW_WEST_TO_LOW_EAST_A},
	{TRIGGER_EXIT, EXIT_LOW_WEST_TO_LOW_EAST_B, CONDITION_ALWAYS,
	 EXIT_LOW_WEST_TO_LOW_EAST_B},
	/* Flag 8 comes after the party members' flags and no code sets it, so the
	 */
	/* push always answers, as the low east level's does. */
	{STEP_TRIGGER_WALKING_INTO(FACING_EAST),
	 CELL_BEHAVIOR_MAGMA_ROCK_PILLAR_PUSH, 0x8,
	 EVENT(scMagmaRockPushPillarEast)},
	{TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scMagmaRockStartPushingPillar)},
	{TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
	 EVENT(scMagmaRockFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_LOW_WEST_PILLAR, FLAG_LOW_WEST_PILLAR_PUSHED,
	 EVENT(scMagmaRockStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOW_WEST_PILLAR,
	 FLAG_LOW_WEST_PILLAR_PUSHED, EVENT(scMagmaRockFinishPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_LOW_WEST_ROPE_EAST_POST,
	 FLAG_LOW_WEST_ROPE_LASHED, 0},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
	 EVENT(scMagmaRockResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
	 EVENT(scMagmaRockTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
	 EVENT(scMagmaRockIgniteStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, CONDITION_ALWAYS,
	 EVENT(scMagmaRockResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, CONDITION_ALWAYS,
	 EVENT(scMagmaRockTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, CONDITION_ALWAYS,
	 EVENT(scMagmaRockIgniteStatue)},
	{TRIGGER_TABLE_END},
};

/* The triggers of the cliff's low east level. */
TRIGGER MagmaRock_SalamanderTailLowEastTriggers[] = {
	{TRIGGER_EXIT, EXIT_LOW_EAST_TO_LOW_WEST_A, CONDITION_ALWAYS,
	 EXIT_LOW_EAST_TO_LOW_WEST_A},
	{TRIGGER_EXIT, EXIT_LOW_EAST_TO_LOW_WEST_B, CONDITION_ALWAYS,
	 EXIT_LOW_EAST_TO_LOW_WEST_B},
	{STEP_TRIGGER_WALKING_INTO(FACING_WEST),
	 CELL_BEHAVIOR_MAGMA_ROCK_PILLAR_PUSH, CONDITION_ALWAYS,
	 EVENT(scMagmaRockPushPillarWest)},
	{TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scMagmaRockStartPushingPillar)},
	{TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
	 EVENT(scMagmaRockFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_LOW_EAST_PILLAR_A, FLAG_LOW_EAST_PILLAR_A_PUSHED,
	 EVENT(scMagmaRockStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOW_EAST_PILLAR_A,
	 FLAG_LOW_EAST_PILLAR_A_PUSHED, EVENT(scMagmaRockFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_LOW_EAST_PILLAR_B, FLAG_LOW_EAST_PILLAR_B_PUSHED,
	 EVENT(scMagmaRockStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOW_EAST_PILLAR_B,
	 FLAG_LOW_EAST_PILLAR_B_PUSHED, EVENT(scMagmaRockFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, FLAG_FIRE_BURST_PLAYING,
	 EVENT(scMagmaRockTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockBurstCrackedPillar)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockFireStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE_B, CONDITION_ALWAYS,
	 EVENT(scMagmaRockResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE_B, FLAG_FIRE_BURST_PLAYING,
	 EVENT(scMagmaRockTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE_B, CONDITION_ALWAYS,
	 EVENT(scMagmaRockBurstCrackedPillar)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE_B, CONDITION_ALWAYS,
	 EVENT(scMagmaRockFireStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_B, FLAG_CRACKED_PILLAR_B_BURST,
	 EVENT(MagmaRock_BurstCrackedPillarB)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
	 EVENT(scMagmaRockResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
	 EVENT(scMagmaRockTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
	 EVENT(scMagmaRockIgniteStatue)},
	{PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_LOW_EAST_SPROUT_A,
	 FLAG_VINE_A_GROWN, EVENT(scMagmaRockGrowVineA)},
	{PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_LOW_EAST_SPROUT_B,
	 FLAG_VINE_B_GROWN, EVENT(scMagmaRockGrowVineB)},
	{PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_LOW_EAST_SPROUT_C,
	 FLAG_VINE_C_GROWN, EVENT(scMagmaRockGrowVineC)},
	{TRIGGER_TABLE_END},
};

/* The triggers of the top. */
TRIGGER MagmaRock_StatueTriggers[] = {
	{TRIGGER_EXIT, MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_INSIDE_ENTRANCE,
	 CONDITION_ALWAYS, MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_INSIDE_ENTRANCE},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
	 CELL_BEHAVIOR_MAGMA_ROCK_STATUE_ROOM_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_STATUE_ROOM_STATUE, FLAG_FIRE_BURST_PLAYING,
	 EVENT(scMagmaRockTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_MAGMA_ROCK_STATUE_ROOM_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockBlastWayIn)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_BEHAVIOR_MAGMA_ROCK_STATUE_ROOM_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockFireStatue)},
	{TRIGGER_TABLE_END},
};
