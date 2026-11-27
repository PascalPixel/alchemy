/* @file */
/* The lower rooms of Jupiter Lighthouse's right tower, past its stone archer */
/* statue. In the big east hall the Jupiter Djinni Whorl waits on a ledge */
/* (scJupiterLighthouseFightWhorl). Two energy path blocks, one carrying a */
/* straight piece of the path and one a corner, pushed or Moved into two of the
 */
/* three gaps in its floor (scJupiterLighthousePlacePathBlock), light the path
 */
/* to new Hover pads and raise a cone-shaped stake; Pound on the stake throws */
/* the blocks back out of their gaps to try another layout */
/* (scJupiterLighthouseRaiseBlocksFromGaps), and one layout reaches Whorl. In */
/* the west hall a third block, pushed into the gap at cell (8, 12), lights the
 */
/* path under six grey blocks, which rise so that Felix can jump across from */
/* block to block, and the west stake Pounded throws it out again. Down in the
 */
/* room of the pit, the stone pillar pushed or Moved one cell west onto the */
/* purple button lowers the bridge across the pit for good, a shortcut. */

/* This unit declares its own form of fdMapGridGetCellBehavior, below. */
#define OWN_MapGrid_GetCellBehavior

#include "system/field_effect.h"
#include "field/field_sequence.h"
#include "field/event_inline.h"
#include "game/game.h"

extern s16 JupiterLighthouse_WestPathCells[][2];

void scJupiterLighthouseLightWestPath(s32 mode);
void scJupiterLighthouseThrowOutBlock(s32 id, s32 x, s32 z, s32 dx, s32 dz);
void fdLeaderCenterInCell(void);
#include "field/call.h"
#include "scene/scene.h"
#include "field/cell_target.h"
#include "macros.h"
#include "field/actor_script.h"
#include "scene/lighthouse_palettes.h"

/* Where the first two path blocks lie, each block in each of the east hall's */
/* three gaps: the block's flag after FLAG_PATH_BLOCK_PLACED(0), and what */
/* scJupiterLighthouseFindBlockGap returns. */
typedef enum PATH_BLOCK_PLACE {
	STRAIGHT_BLOCK_IN_EAST_GAP,
	CORNER_BLOCK_IN_EAST_GAP,
	STRAIGHT_BLOCK_IN_WEST_GAP,
	CORNER_BLOCK_IN_WEST_GAP,
	STRAIGHT_BLOCK_IN_NORTH_GAP,
	CORNER_BLOCK_IN_NORTH_GAP,
} PATH_BLOCK_PLACE;

/* A path block lies in a gap of the east hall, a flag for each PATH_BLOCK_PLACE
 */
/* from the first. */
#define FLAG_PATH_BLOCK_PLACED_FIRST  0xa10
#define FLAG_PATH_BLOCK_PLACED(place) (FLAG_PATH_BLOCK_PLACED_FIRST + (place))
/* The east stake is down in the floor: Pounded, or not yet raised by the two */
/* blocks both lying in gaps. */
#define FLAG_EAST_STAKE_SUNK 0xa16
/* The west stake is down in the floor, Pounded; the third block is then out */
/* of its gap. */
#define FLAG_WEST_STAKE_SUNK 0xa17
/* The third path block lies in its gap below the west hall's north door. */
#define FLAG_THIRD_BLOCK_IN_GAP 0xa1a
/* The pillar stands on the button by the pit: the bridge stays across it. */
#define FLAG_BUTTON_PILLAR_PLACED 0xa1b
/* Set while Felix stands on the button, which holds the bridge across. */
#define FLAG_BRIDGE_BUTTON_HELD FLAG_SCENE_TEMP_02

/* The east hall's three gaps, by cell. */
#define EAST_GAP_CELL_X	 35
#define EAST_GAP_CELL_Z	 15
#define WEST_GAP_CELL_X	 31
#define WEST_GAP_CELL_Z	 16
#define NORTH_GAP_CELL_X 33
#define NORTH_GAP_CELL_Z 10
/* The third block's gap, by cell. */
#define THIRD_GAP_CELL_X 8
#define THIRD_GAP_CELL_Z 12
/* The button by the pit, by cell. */
#define BUTTON_CELL_X 8
#define BUTTON_CELL_Z 37
/* The rooms' own cell behaviors. */
typedef enum JUPITER_LIGHTHOUSE_WHORL_CELL_BEHAVIOR {
	/* The purple button by the pit, cell (8, 37), and the eight cells round */
	/* it. */
	CELL_BEHAVIOR_BRIDGE_BUTTON = 20,
	CELL_BEHAVIOR_OFF_BRIDGE_BUTTON,
	/* The west hall's floor among the grey blocks, (8, 16) to (15, 20), where
	 */
	/* Felix jumps from block to block while they stand raised. */
	CELL_BEHAVIOR_GREY_BLOCK_JUMP = 30,
	/* The cells beside the gaps, each named for the side of the gap it is on:
	 */
	/* walking into the gap from it pushes a block or jumps across. */
	CELL_BEHAVIOR_EAST_OF_GAP = 40,
	CELL_BEHAVIOR_SOUTH_OF_GAP,
	CELL_BEHAVIOR_WEST_OF_GAP,
	CELL_BEHAVIOR_NORTH_OF_GAP
} JUPITER_LIGHTHOUSE_WHORL_CELL_BEHAVIOR;
/* A gap's floor, 16 pixels below the hall's, where a thrown block starts. */
#define GAP_FLOOR_HEIGHT toFIXED(-16)
/* The most frames the room waits for a block to come down. */
#define BLOCK_LANDING_FRAMES 30
/* The way in from the bridge's floor, through its west arch. */
#define ENTRANCE_FROM_BRIDGE_FLOOR 12
/* The first of the two ways into the east hall; the other is the next. */
#define ENTRANCE_EAST_HALL_FIRST 3

/* The whorl's actors, numbering its placement table. */
typedef enum JUPITER_LIGHTHOUSE_WHORL_ACTOR_ID {
	ACTOR_ARCHER_STATUE = ACTOR_FIRST_PLACED,
	ACTOR_WEST_STAKE,
	/* The six grey blocks; those on the west hall's path rise while it is lit.
	 */
	ACTOR_FIRST_GREY_BLOCK,
	ACTOR_LAST_GREY_BLOCK = ACTOR_FIRST_GREY_BLOCK + 5,
	ACTOR_BUTTON_PILLAR,
	ACTOR_EAST_STAKE,
	ACTOR_STRAIGHT_PATH_BLOCK,
	ACTOR_CORNER_PATH_BLOCK,
	ACTOR_THIRD_PATH_BLOCK,
	ACTOR_WHORL
} JUPITER_LIGHTHOUSE_WHORL_ACTOR_ID;

extern s16 JupiterLighthouse_WhorlPoundTargets[];
extern s32 PaletteCyclePausesInEvents;
/* The state of the code every Jupiter Lighthouse room's overlay keeps its own
 */
/* copy of: a whirlwind's ride (shared/whirlwind_*.h), the palette cycle */
/* (shared/palette_cycle_step.h), the lit actors' sprites */
/* (shared/light_sprites_*.h) and the cracked tiles (shared/cracked_tile_*.h).
 */
/* These rooms have neither whirlwind nor cracked tile, but their copies of */
/* that code keep their state here all the same. */
s32 WhirlwindRiding BSS_DATA;
u16 PaletteCycleColors[16] BSS_DATA;
/* Palette 13's first two colours; it is copied whole, with the unlit colours */
/* after them. */
u16 PaletteCycleUnlitPalette[2] BSS_DATA;
u16 PaletteCycleUnlitColors[14] BSS_DATA;
s16 LightSpriteTile BSS_DATA;
s16 LightSpriteCount BSS_DATA;
u32 LightSpriteActors[16] BSS_DATA;
LIGHT_OAM LightSpriteOams[16] BSS_DATA;
s16 LightSpritesLarge BSS_DATA;
s16 LightSpritePriority BSS_DATA;
CELL_TARGET CrackedTiles BSS_DATA;
void scJupiterLighthouseRaiseBridge(s32 mode);
void scJupiterLighthouseLowerBridge(void);
void scJupiterLighthouseCheckPillarOnButton(void);

/* Felix talks to Whorl on its ledge in the east hall, facing north: the */
/* Jupiter Djinni fights the party. */
void scJupiterLighthouseFightWhorl(void) {
	fdRequestDjinnBattle(ACTOR_WHORL, DJINNI_WHORL);
}

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* The bridge's two panels, standing on end at the pit's sides, swing down to */
/* lie across it; with `mode` 1 they do so with a thud and a picture halfway */
/* down, otherwise they simply lie there. */
void scJupiterLighthouseRaiseBridge(s32 mode) {
	if(mode == 1) {
		fdEventPlaySnd(SE_IMPACT);
		fdMapGridCopyCellsLayered(76, 42, 4, 3, 76, 35);
		syWait(6);
	}
	fdMapGridCopyCellsLayered(81, 42, 4, 3, 76, 35);
	fdMapGridCopyCellAttributes(12, 41, 4, 1, 12, 35);
}

/* The bridge's two panels swing back up on end with a heavy thud. */
void scJupiterLighthouseLowerBridge(void) {
	fdEventPlaySnd(SE_HEAVY_IMPACT);
	fdMapGridCopyCellsLayered(76, 42, 4, 3, 76, 35);
	syWait(6);
	fdMapGridCopyCellsLayered(71, 42, 4, 3, 76, 35);
	fdMapGridCopyCellAttributes(12, 36, 4, 1, 12, 35);
}

/* Lays the bridge across the pit for good once the pillar stands on the */
/* button. */
void scJupiterLighthouseCheckPillarOnButton(void) {
	ACTOR *actor = fdActorGet(ACTOR_BUTTON_PILLAR);

	if(!fdEventGetFlag(FLAG_BUTTON_PILLAR_PLACED) &&
	   FIELD_CELL(actor->x.fixed) == BUTTON_CELL_X &&
	   FIELD_CELL(actor->z.fixed) == BUTTON_CELL_Z) {
		fdEventSetFlag(FLAG_BUTTON_PILLAR_PLACED);
		scJupiterLighthouseRaiseBridge(1);
	}
}

/* Felix steps onto the purple button by the pit, cell (8, 37): the bridge's */
/* panels swing down across the pit with a thud, and stay while he stands */
/* there. */
void scJupiterLighthouseStepOnButton(void) {
	if(!fdEventGetFlag(FLAG_BRIDGE_BUTTON_HELD)) {
		fdEventSetFlag(FLAG_BRIDGE_BUTTON_HELD);
		scJupiterLighthouseRaiseBridge(1);
	}
}

/* Felix steps off the purple button by the pit: the bridge's panels swing back
 */
/* up on end with a heavy thud. If the stone pillar has just been pushed onto */
/* the button, the bridge stays across the pit for good instead. */
void scJupiterLighthouseStepOffButton(void) {
	if(fdEventGetFlag(FLAG_BRIDGE_BUTTON_HELD) == 1) {
		fdEventClearFlag(FLAG_BRIDGE_BUTTON_HELD);
		scJupiterLighthouseLowerBridge();
	}
	scJupiterLighthouseCheckPillarOnButton();
}

/* The stone pillar beside the purple button by the pit, Moved one cell west */
/* onto the button at cell (8, 37), holds the bridge across the pit for good, */
/* a shortcut back through the tower. Moved anywhere else, it does nothing. */
void scJupiterLighthouseMoveButtonPillar(void) {
	syWait(10);
	scJupiterLighthouseCheckPillarOnButton();
}

/* Marks the gap the first two blocks have just come to rest in; returns */
/* where the block lies, or -1 for no gap. */
s32 scJupiterLighthouseFindBlockGap(void) {
	s32 x, z, place;
	ACTOR *corner;
	ACTOR *straight;

	straight = fdActorGet(ACTOR_STRAIGHT_PATH_BLOCK);
	corner = fdActorGet(ACTOR_CORNER_PATH_BLOCK);
	place = -1;
	x = FIELD_CELL(straight->x.fixed);
	z = FIELD_CELL(straight->z.fixed);
	if(x == EAST_GAP_CELL_X && z == EAST_GAP_CELL_Z &&
	   !fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP))) {
		fdEventSetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP));
		place = STRAIGHT_BLOCK_IN_EAST_GAP;
	}
	if(x == WEST_GAP_CELL_X && z == WEST_GAP_CELL_Z &&
	   !fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP))) {
		fdEventSetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP));
		place = STRAIGHT_BLOCK_IN_WEST_GAP;
	}
	if(x == NORTH_GAP_CELL_X && z == NORTH_GAP_CELL_Z &&
	   !fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP))) {
		fdEventSetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP));
		place = STRAIGHT_BLOCK_IN_NORTH_GAP;
	}
	x = FIELD_CELL(corner->x.fixed);
	z = FIELD_CELL(corner->z.fixed);
	if(x == EAST_GAP_CELL_X && z == EAST_GAP_CELL_Z &&
	   !fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP))) {
		fdEventSetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP));
		place = CORNER_BLOCK_IN_EAST_GAP;
	}
	if(x == WEST_GAP_CELL_X && z == WEST_GAP_CELL_Z &&
	   !fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP))) {
		fdEventSetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP));
		place = CORNER_BLOCK_IN_WEST_GAP;
	}
	if(x == NORTH_GAP_CELL_X && z == NORTH_GAP_CELL_Z &&
	   !fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP))) {
		fdEventSetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP));
		place = CORNER_BLOCK_IN_NORTH_GAP;
	}
	return place;
}

/* How many of the first two blocks lie in gaps. */
s32 scJupiterLighthouseCountBlocksInGaps(void) {
	s32 count = 0;

	if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP)) ||
	   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)) ||
	   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP)))
		count = 1;
	if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP)) ||
	   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP)) ||
	   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)))
		count++;
	return count;
}

/* Draws the east hall's floor for the blocks that lie in gaps: each filled */
/* gap, the path lit between them, and the Hover tiles that path reaches. */
void scJupiterLighthouseDrawBlocksInGaps(void) {
	/* Every gap empty, two height steps deep, and no Hover tile. */
	fdMapGridCopyCellsLayered(94, 60, 9, 8, 94, 10);
	Call4((void (*)())fdMapGridSetCellHeight, toFIXED(560), toFIXED(240),
		  MAPGRID_LAYER_GROUND, -2);
	Call4((void (*)())fdMapGridSetCellHeight, toFIXED(496), toFIXED(256),
		  MAPGRID_LAYER_GROUND, -2);
	Call4((void (*)())fdMapGridSetCellHeight, toFIXED(528), toFIXED(160),
		  MAPGRID_LAYER_GROUND, -2);
	syFieldMapGridSetCellBehavior(toFIXED(608), toFIXED(160),
								  MAPGRID_LAYER_OVERHEAD, CELL_BEHAVIOR_NORMAL);
	syFieldMapGridSetCellBehavior(toFIXED(496), toFIXED(160),
								  MAPGRID_LAYER_OVERHEAD, CELL_BEHAVIOR_NORMAL);
	syFieldMapGridSetCellBehavior(toFIXED(496), toFIXED(224),
								  MAPGRID_LAYER_OVERHEAD, CELL_BEHAVIOR_NORMAL);
	if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP)) ||
	   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP)))
		fdMapGridSetCellHeight(toFIXED(560), toFIXED(240), MAPGRID_LAYER_GROUND,
							   0);
	if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)) ||
	   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP)))
		fdMapGridSetCellHeight(toFIXED(496), toFIXED(256), MAPGRID_LAYER_GROUND,
							   0);
	if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP)) ||
	   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)))
		fdMapGridSetCellHeight(toFIXED(528), toFIXED(160), MAPGRID_LAYER_GROUND,
							   0);
	if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP))) {
		fdMapGridCopyCellsLayered(96, 56, 7, 2, 96, 15);
		fdMapGridCopyCellsLayered(111, 51, 1, 1, 99, 15);
		if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP))) {
			fdMapGridCopyCellsLayered(105, 51, 1, 3, 95, 14);
			syFieldMapGridSetCellBehavior(toFIXED(496), toFIXED(224),
										  MAPGRID_LAYER_OVERHEAD,
										  CELL_BEHAVIOR_HOVER);
		}
		if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)))
			fdMapGridCopyCellsLayered(111, 53, 1, 1, 97, 10);
	}
	else if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP))) {
		fdMapGridCopyCellsLayered(98, 51, 5, 1, 98, 10);
		fdMapGridCopyCellsLayered(99, 52, 4, 5, 99, 11);
		syFieldMapGridSetCellBehavior(toFIXED(608), toFIXED(160),
									  MAPGRID_LAYER_OVERHEAD,
									  CELL_BEHAVIOR_HOVER);
		if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)))
			fdMapGridCopyCellsLayered(106, 53, 1, 1, 95, 16);
		if(fdEventGetFlag(
			   FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP))) {
			fdMapGridCopyCellsLayered(109, 51, 3, 1, 95, 10);
			syFieldMapGridSetCellBehavior(toFIXED(496), toFIXED(160),
										  MAPGRID_LAYER_OVERHEAD,
										  CELL_BEHAVIOR_HOVER);
		}
	}
	else {
		if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)))
			fdMapGridCopyCellsLayered(106, 53, 1, 1, 95, 16);
		if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP)))
			fdMapGridCopyCellsLayered(107, 53, 1, 1, 95, 16);
		if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP)))
			fdMapGridCopyCellsLayered(111, 52, 1, 1, 97, 10);
		if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)))
			fdMapGridCopyCellsLayered(111, 53, 1, 1, 97, 10);
	}
}

/* Whether the actor stands on one of the cells of the west hall's path. */
s32 scJupiterLighthouseIsOnWestPath(ACTOR *actor) {
	s32 i;

	for(i = 0; JupiterLighthouse_WestPathCells[i][0] != -1; i++) {
		if(JupiterLighthouse_WestPathCells[i][0] ==
			   FIELD_CELL(actor->x.fixed) &&
		   JupiterLighthouse_WestPathCells[i][1] == FIELD_CELL(actor->z.fixed))
			return 1;
	}
	return 0;
}

/* Lights or darkens the west hall's path as the third block lies in its gap */
/* or not, raising the grey blocks that stand on it or letting them down */
/* (gradually in mode 1). */
void scJupiterLighthouseLightWestPath(s32 mode) {
	ACTOR *actor;
	s32 i, frame;

	if(!fdEventGetFlag(FLAG_THIRD_BLOCK_IN_GAP)) {
		fdMapGridCopyCellsLayered(80, 46, 1, 7, 72, 9);
		fdMapGridCopyCellsLayered(80, 53, 8, 5, 72, 16);
		fdMapGridCopyCellsLayered(78, 47, 1, 1, 8, 74);
		fdMapGridSetCellHeight(toFIXED(128), toFIXED(192), MAPGRID_LAYER_GROUND,
							   4);
		if(mode == 1) {
			for(frame = 0; frame <= 15; frame++) {
				for(i = 0; i <= 5; i++) {
					actor = fdActorGet(ACTOR_FIRST_GREY_BLOCK + i);
					if(scJupiterLighthouseIsOnWestPath(actor)) {
						actor->motionFlg = 0;
						actor->y.fixed -= toFIXED(2);
					}
				}
				syWait(1);
			}
		}
		i = 0;
		do {
			actor = fdActorGet(ACTOR_FIRST_GREY_BLOCK + i);
			actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			i++;
			actor->y.fixed = actor->groundHeight;
		} while(i <= 5);
	}
	else {
		fdMapGridCopyCellsLayered(72, 46, 1, 7, 72, 9);
		fdMapGridCopyCellsLayered(72, 53, 8, 5, 72, 16);
		fdMapGridCopyCellsLayered(70, 47, 1, 1, 8, 74);
		fdMapGridSetCellHeight(toFIXED(128), toFIXED(192), MAPGRID_LAYER_GROUND,
							   6);
		scFieldActorStop(MainRoutineStateWork.leaderActorId);
		fdActorSetPos(ACTOR_THIRD_PATH_BLOCK, 0, 0);
		if(mode == 1) {
			for(frame = 0; frame <= 15; frame++) {
				for(i = 0; i <= 5; i++) {
					actor = fdActorGet(ACTOR_FIRST_GREY_BLOCK + i);
					if(scJupiterLighthouseIsOnWestPath(actor)) {
						actor->motionFlg = 0;
						actor->y.fixed += toFIXED(2);
					}
				}
				syWait(1);
			}
		}
		for(i = 0; i <= 5; i++) {
			actor = fdActorGet(ACTOR_FIRST_GREY_BLOCK + i);
			if(scJupiterLighthouseIsOnWestPath(actor)) {
				actor->motionFlg = ACTOR_FLOATS;
				actor->y.fixed = toFIXED(65) / 2;
			}
		}
	}
}

/* Nothing visible happens as Felix starts to push or Move an energy path */
/* block. When it is the west hall's block, the cell it leaves becomes plain */
/* floor again, no longer a step to jump onto. */
void scJupiterLighthouseLiftPathBlock(s32 unused, s32 id) {
	ACTOR *actor = fdActorGet(id);

	if(id == ACTOR_THIRD_PATH_BLOCK) {
		fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed,
							   MAPGRID_LAYER_GROUND, 6);
		fdMapGridClearAttributeBBitsAt(actor->x.fixed, actor->z.fixed,
									   MAPGRID_LAYER_GROUND, MAP_CELL_JUMP);
	}
}

s32 fdMapGetTerrainHeight(s32 layer, s32 x, s32 z);

/* An energy path block, pushed or Moved, comes to rest, landing with a thump */
/* if it dropped off a ledge. In one of the east hall's three gaps, (35, 15), */
/* (31, 16) or (33, 10), it sinks in and lights the path through it, with a */
/* sound where the lit path reaches it; once both blocks lie in gaps, the */
/* camera turns to the cone-shaped stake as it rises. The west hall's block, in
 */
/* its gap at cell (8, 12), lights the path under the grey blocks and raises */
/* the west stake. Set down anywhere else, the west hall's block stands as a */
/* step Felix can jump onto. */
void scJupiterLighthousePlacePathBlock(s32 unused, s32 id) {
	ACTOR *actor = fdActorGetById(id);
	s32 place;
	s32 i;

	if(actor->groundHeight !=
	   fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed)) {
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		sySynch(2);
		i = 0;
		if(actor->y.fixed > actor->groundHeight || actor->velY != 0) {
			do {
				sySynch(1);
				i++;
				if(i >= BLOCK_LANDING_FRAMES)
					break;
			} while(actor->y.fixed > actor->groundHeight || actor->velY != 0);
		}
		fdEventPlaySnd(SE_THUMP);
		actor->motionFlg = 0;
	}
	if((u32)(id - ACTOR_STRAIGHT_PATH_BLOCK) <= 1) {
		place = scJupiterLighthouseFindBlockGap();
		if(place == -1)
			return;
		fdActorSetSprFlags(fdActorGetById(id), 0);
		fdEventWait(30);
		scJupiterLighthouseDrawBlocksInGaps();
		/* A sound as the lit path reaches the block. */
		if(place == STRAIGHT_BLOCK_IN_EAST_GAP)
			fdEventPlaySnd(SE_MAP_TRANSITION);
		if(place == CORNER_BLOCK_IN_EAST_GAP)
			fdEventPlaySnd(SE_MAP_TRANSITION);
		if(place == CORNER_BLOCK_IN_WEST_GAP &&
		   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP)))
			fdEventPlaySnd(SE_MAP_TRANSITION);
		if(place == STRAIGHT_BLOCK_IN_NORTH_GAP &&
		   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP)))
			fdEventPlaySnd(SE_MAP_TRANSITION);
		/* The block is drawn into the floor now, its actor put away. */
		fdActorStop(MainRoutineStateWork.leaderActorId);
		fdActorSetPos(id, 0, 0);
		if(scJupiterLighthouseCountBlocksInGaps() != 2 ||
		   !fdEventGetFlag(FLAG_EAST_STAKE_SUNK))
			return;
		actor = fdActorGetById(ACTOR_EAST_STAKE);
		fdEventClearFlag(FLAG_EAST_STAKE_SUNK);
		fdEventWait(10);
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		fdCameraMoveTo(664, CAMERA_KEEP, 136, 1);
		fdCameraWaitForArrival();
		fdEventWait(20);
		fdEventPlaySnd(SE_LIGHT_APPEAR);
		fdActorSelectAnimation(actor, 3);
		actor->priFlg &= ~ACTOR_PRIORITY_UNDERFOOT;
		syFieldMapGridSetCellBehavior(actor->x.fixed, actor->z.fixed,
									  actor->layer, CELL_BEHAVIOR_WALL);
		fdEventWait(30);
		syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
		fdCameraWaitForArrival();
		fdEventEnd();
		return;
	}
	if(id != ACTOR_THIRD_PATH_BLOCK)
		return;
	if(FIELD_CELL(actor->x.fixed) == THIRD_GAP_CELL_X &&
	   FIELD_CELL(actor->z.fixed) == THIRD_GAP_CELL_Z) {
		fdEventSetFlag(FLAG_THIRD_BLOCK_IN_GAP);
		fdActorSetSprFlags(fdActorGetById(ACTOR_THIRD_PATH_BLOCK), 0);
		fdEventWait(30);
		fdEventPlaySnd(SE_MAP_TRANSITION);
		scJupiterLighthouseLightWestPath(1);
		if(!fdEventGetFlag(FLAG_WEST_STAKE_SUNK))
			return;
		actor = fdActorGetById(ACTOR_WEST_STAKE);
		fdEventClearFlag(FLAG_WEST_STAKE_SUNK);
		fdEventWait(20);
		fdEventPlaySnd(SE_LIGHT_APPEAR);
		fdActorSelectAnimation(actor, 3);
		actor->priFlg &= ~ACTOR_PRIORITY_UNDERFOOT;
		syFieldMapGridSetCellBehavior(actor->x.fixed, actor->z.fixed,
									  actor->layer, CELL_BEHAVIOR_WALL);
	}
	else {
		fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed,
							   MAPGRID_LAYER_GROUND, 8);
		fdMapGridSetCellAttributeBitsAt(actor->x.fixed, actor->z.fixed,
										MAPGRID_LAYER_GROUND, MAP_CELL_JUMP);
	}
}

/* Nothing visible happens as Felix starts to cast Pound on the east hall's */
/* stake; whatever the hall was still doing is settled first */
/* (`fdEventEndLastingPsynergy`). */
void scJupiterLighthouseStartPound(void) { fdEventEndLastingPsynergy(0); }

/* Pound hammers the cone-shaped stake in the east or the west hall down into */
/* the floor. */
void scJupiterLighthouseSinkPoundTarget(s32 mode, s32 frame) {
	fdPoundSinkTarget(JupiterLighthouse_WhorlPoundTargets, mode, frame);
}

/* Throws a block up out of its gap at cell (x, z) to land beside it. */
void scJupiterLighthouseThrowOutBlock(s32 id, s32 x, s32 z, s32 dx, s32 dz) {
	ACTOR *actor = fdActorGet(id);

	fdActorSetPosFX(id, FIELD_CELL_CENTER(x), FIELD_CELL_CENTER(z));
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	actor->groundHeight = GAP_FLOOR_HEIGHT;
	actor->y.fixed = GAP_FLOOR_HEIGHT;
	actor->velY = toFIXED(8);
	fdActorSetSpeed(id, 1.6);
	fdActorSetDestinationOffset(id, dx, dz);
	fdEventPlaySnd(SE_WHIRLWIND);
}

/* Once Pound has hammered the east hall's stake down, the camera turns to the
 */
/* gaps and the two energy path blocks fly up out of them with a rush of wind,
 */
/* each landing on the floor beside its gap, and the path goes dark. */
void scJupiterLighthouseRaiseBlocksFromGaps(void) {
	ACTOR *actor;
	s32 i;

	if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP)) +
		   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)) +
		   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP)) +
		   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP)) +
		   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP)) +
		   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)) ==
	   0)
		return;
	fdCameraMoveTo(592, CAMERA_KEEP, 248, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(10);
	if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP)))
		scJupiterLighthouseThrowOutBlock(ACTOR_STRAIGHT_PATH_BLOCK,
										 EAST_GAP_CELL_X, EAST_GAP_CELL_Z, -16,
										 0);
	if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP)))
		scJupiterLighthouseThrowOutBlock(
			ACTOR_CORNER_PATH_BLOCK, EAST_GAP_CELL_X, EAST_GAP_CELL_Z, -16, 0);
	if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)))
		scJupiterLighthouseThrowOutBlock(
			ACTOR_STRAIGHT_PATH_BLOCK, WEST_GAP_CELL_X, WEST_GAP_CELL_Z, 16, 0);
	if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP)))
		scJupiterLighthouseThrowOutBlock(
			ACTOR_CORNER_PATH_BLOCK, WEST_GAP_CELL_X, WEST_GAP_CELL_Z, 16, 0);
	if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP)))
		scJupiterLighthouseThrowOutBlock(ACTOR_STRAIGHT_PATH_BLOCK,
										 NORTH_GAP_CELL_X, NORTH_GAP_CELL_Z,
										 -16, 0);
	if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)))
		scJupiterLighthouseThrowOutBlock(ACTOR_CORNER_PATH_BLOCK,
										 NORTH_GAP_CELL_X, NORTH_GAP_CELL_Z,
										 -16, 0);
	fdEventClearFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP));
	fdEventClearFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP));
	fdEventClearFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP));
	fdEventClearFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP));
	fdEventClearFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP));
	fdEventClearFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP));
	scJupiterLighthouseDrawBlocksInGaps();
	syWait(2);
	actor = fdActorGet(ACTOR_STRAIGHT_PATH_BLOCK);
	i = 0;
	if(actor->y.fixed > actor->groundHeight || actor->velY != 0) {
		do {
			syWait(1);
			i++;
			if(i >= BLOCK_LANDING_FRAMES)
				break;
		} while(actor->y.fixed > actor->groundHeight || actor->velY != 0);
	}
	actor->motionFlg = 0;
	actor = fdActorGet(ACTOR_CORNER_PATH_BLOCK);
	i = 0;
	if(actor->y.fixed > actor->groundHeight || actor->velY != 0) {
		do {
			syWait(1);
			i++;
			if(i >= BLOCK_LANDING_FRAMES)
				break;
		} while(actor->y.fixed > actor->groundHeight || actor->velY != 0);
	}
	actor->motionFlg = 0;
	syFieldEventWait(30);
	syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
}

/* Once Pound has hammered the west hall's stake down, its energy path block */
/* flies up out of its gap at cell (8, 12) and lands beside it, and the path */
/* under the grey blocks goes dark, letting them down. */
void scJupiterLighthouseRaiseThirdBlockFromGap(void) {
	ACTOR *actor;
	s32 i;

	if(fdEventGetFlag(FLAG_THIRD_BLOCK_IN_GAP))
		scJupiterLighthouseThrowOutBlock(
			ACTOR_THIRD_PATH_BLOCK, THIRD_GAP_CELL_X, THIRD_GAP_CELL_Z, 16, 0);
	fdEventClearFlag(FLAG_THIRD_BLOCK_IN_GAP);
	scJupiterLighthouseLightWestPath(1);
	syWait(2);
	actor = fdActorGet(ACTOR_THIRD_PATH_BLOCK);
	for(i = 0; i < BLOCK_LANDING_FRAMES; i++) {
		if(actor->y.fixed <= actor->groundHeight && actor->velY == 0)
			break;
		syWait(1);
	}
	actor->motionFlg = 0;
}

/* Felix walks towards a gap from a cell beside it, in the east hall or round */
/* the west hall's gap: a block in front of him he pushes; an empty gap he */
/* jumps across, where there is floor to land on; over a gap a block fills, */
/* he walks on. */
void scJupiterLighthouseJumpOrPushBlock(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR *straight = fdActorGet(ACTOR_STRAIGHT_PATH_BLOCK);
	ACTOR *corner = fdActorGet(ACTOR_CORNER_PATH_BLOCK);
	FIXED position[3];
	s32 found = 0;
	s32 x;
	s32 z;
	s32 facing;

	facing = (leader->facing + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
	position[0] = leader->x.fixed;
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed;
	syFieldVectorAddPolarOffset(16, facing, position);
	x = FIELD_CELL(position[0]);
	z = FIELD_CELL(position[2]);
	if(straight->y.fixed == 0 && x == FIELD_CELL(straight->x.fixed) &&
	   z == FIELD_CELL(straight->z.fixed))
		found = 1;
	if(corner->y.fixed == 0 && x == FIELD_CELL(corner->x.fixed) &&
	   z == FIELD_CELL(corner->z.fixed))
		found = 1;
	if(found == 0) {
		if(x == EAST_GAP_CELL_X && z == EAST_GAP_CELL_Z &&
		   (fdEventGetFlag(
				FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP)) ||
			fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP))))
			found = 2;
		if(x == WEST_GAP_CELL_X && z == WEST_GAP_CELL_Z &&
		   (fdEventGetFlag(
				FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)) ||
			fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP))))
			found = 2;
		if(x == NORTH_GAP_CELL_X && z == NORTH_GAP_CELL_Z &&
		   (fdEventGetFlag(
				FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP)) ||
			fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP))))
			found = 2;
		if(found == 0) {
			fdTryJumpForward();
			return;
		}
	}
	if(found == 1)
		fdLeaderPushObjectAhead();
}

/* Felix walks among the raised grey blocks in the west hall, between cells */
/* (8, 16) and (15, 20), while its path is lit: he jumps forward onto the next
 */
/* block, where there is one to land on. */
void scJupiterLighthouseJumpForward(void) { fdTryJumpForward(); }

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

void scActorsSetCellsOpen(const u16 *list);
void scLightSprsInitB(void);
void scLightSprsAddCounted(s32 id, s32 frame);

/* Sets up the rooms as the party enters: the archer statue at three quarters */
/* of its size, the stakes and the bridge as their flags leave them, the */
/* blocks' glow, and the grey blocks as platforms. Returns 0. */
s32 scInit(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	/* In the east hall the leader is drawn by the cells under him, never hidden
	 */
	/* behind the rises in front. */
	if(MainRoutineStateWork.entrance >= ENTRANCE_EAST_HALL_FIRST &&
	   MainRoutineStateWork.entrance <= ENTRANCE_EAST_HALL_FIRST + 1)
		fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGet(ACTOR_ARCHER_STATUE)->scaleX = FIXED_ONE * 3 / 4;
	fdActorGet(ACTOR_ARCHER_STATUE)->scaleY = FIXED_ONE * 3 / 4;
	scActorsSetCellsOpen((const u16 *)JupiterLighthouse_WhorlPoundTargets);
	if(fdEventGetFlag(FLAG_BUTTON_PILLAR_PLACED)) {
		fdActorSetPos(ACTOR_BUTTON_PILLAR, 136, 600);
		scJupiterLighthouseRaiseBridge(0);
	}
	if(fdEventGetFlag(FLAG_BRIDGE_BUTTON_HELD)) {
		scJupiterLighthouseRaiseBridge(0);
	}
	scLightSprsInitB();
	scLightSprsSetPriority(2);
	scLightSprsAddCounted(ACTOR_STRAIGHT_PATH_BLOCK, 2);
	scLightSprsAddCounted(ACTOR_CORNER_PATH_BLOCK, 1);
	scLightSprsAddCounted(ACTOR_THIRD_PATH_BLOCK, 0);
	if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP)) ||
	   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)) ||
	   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP)))
		fdActorSetPos(ACTOR_STRAIGHT_PATH_BLOCK, 0, 0);
	if(fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP)) ||
	   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP)) ||
	   fdEventGetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)))
		fdActorSetPos(ACTOR_CORNER_PATH_BLOCK, 0, 0);
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	fdActorGet(ACTOR_FIRST_GREY_BLOCK)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorGet(ACTOR_FIRST_GREY_BLOCK + 1)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorGet(ACTOR_FIRST_GREY_BLOCK + 2)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorGet(ACTOR_FIRST_GREY_BLOCK + 3)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorGet(ACTOR_FIRST_GREY_BLOCK + 4)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorGet(ACTOR_LAST_GREY_BLOCK)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	return 0;
}

/* Lays out the blocks as the map is drawn: the first two stay in their gaps */
/* only when both lie in them or the party comes back to where it left, the */
/* third lies in its gap while the west stake stands, and the floor and the */
/* west hall's path follow. Returns 0. */
s32 scPrepareMap(void) {
	ACTOR *actor;
	s32 height;

	scSetArrivalFlags(0);
	scJupiterLighthouseLoadPalettes(LIGHTHOUSE_PALETTES_LIT);
	/* Arriving from the bridge's floor, the grey blocks stand afresh on six of
	 */
	/* the west hall's path cells. */
	if(MainRoutineStateWork.entrance == ENTRANCE_FROM_BRIDGE_FLOOR) {
		fdActorSetPos(ACTOR_FIRST_GREY_BLOCK, 168, 264);
		fdActorSetPos(ACTOR_FIRST_GREY_BLOCK + 1, 200, 264);
		fdActorSetPos(ACTOR_FIRST_GREY_BLOCK + 2, 232, 264);
		fdActorSetPos(ACTOR_FIRST_GREY_BLOCK + 3, 168, 296);
		fdActorSetPos(ACTOR_FIRST_GREY_BLOCK + 4, 136, 328);
		fdActorSetPos(ACTOR_LAST_GREY_BLOCK, 168, 328);
	}
	if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) &&
	   scJupiterLighthouseCountBlocksInGaps() != 2) {
		fdEventClearFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP));
		fdEventClearFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP));
		fdEventClearFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP));
		fdEventClearFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP));
		fdEventClearFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP));
		fdEventClearFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP));
	}
	if(scJupiterLighthouseCountBlocksInGaps() == 2)
		fdEventClearFlag(FLAG_EAST_STAKE_SUNK);
	else
		fdEventSetFlag(FLAG_EAST_STAKE_SUNK);
	if(fdEventGetFlag(FLAG_WEST_STAKE_SUNK))
		fdEventClearFlag(FLAG_THIRD_BLOCK_IN_GAP);
	else
		fdEventSetFlag(FLAG_THIRD_BLOCK_IN_GAP);
	fdActorGet(ACTOR_STRAIGHT_PATH_BLOCK)->motionFlg = 0;
	fdActorGet(ACTOR_CORNER_PATH_BLOCK)->motionFlg = 0;
	fdActorGet(ACTOR_THIRD_PATH_BLOCK)->motionFlg = 0;
	scJupiterLighthouseDrawBlocksInGaps();
	if(!fdEventGetFlag(FLAG_THIRD_BLOCK_IN_GAP)) {
		actor = fdActorGet(ACTOR_THIRD_PATH_BLOCK);
		height = fdMapGridGetCellHeight(actor->x.fixed, actor->z.fixed,
										MAPGRID_LAYER_GROUND);
		fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed,
							   MAPGRID_LAYER_GROUND, height + 2);
		fdMapGridSetCellAttributeBitsAt(actor->x.fixed, actor->z.fixed,
										MAPGRID_LAYER_GROUND, MAP_CELL_JUMP);
	}
	scJupiterLighthouseLightWestPath(0);
	return 0;
}

#include "shared/scene_set_arrival_flags.c"

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 PaletteCycleFrame;

/* Restarts the palette cycle at the given frame. */
static inline void scPaletteCycleSetFrame(s32 value) {
	PaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 WhirlwindEndScript[];
/* The ROM's callers here are compiled against this form, which returns an s32
 */
/* where the definition returns a u8. */
s32 fdMapGridGetCellBehavior(s32 x, s32 z, s32 layer);

#include "shared/whirlwind_update.h"

struct CELL_TARGET;
struct CELL_TARGET_ENTRY;
struct LIGHT_OAM;

extern u32 FieldEffect_ShortScript[];
extern u32 FieldEffect_MediumScript[];
extern u32 FieldEffect_LongScript[];

void fdActorSetCellOpen(s32 id, u32 mode, s32 flag, s32 layer);

void scCrackedTileCrack(CELL_TARGET *table, CELL_TARGET_ENTRY *entry);

void scCrackedTileFall(CELL_TARGET *table, CELL_TARGET_ENTRY *entry);

void fdWhirlwindUpdate(ACTOR *self);
void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/actors_set_cells_open.h"

#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add_counted.h"

/* Sets the priority the lit actors' sprites are drawn at. */
void scLightSprsSetPriority(u16 value) { LightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

extern const s32 *FieldEffect_DurationScripts[];

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY syOamAddSprAtDepthClamped
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

struct CELL_TARGET_ENTRY;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options);

#include "shared/cracked_tile_drop_in.h"

void syGameFlagSetByte(s32 flag, s32 value);

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

/* The cells of the west hall's path, ending at an x of -1. */
/* clang-format off */
s16 JupiterLighthouse_WestPathCells[][2] = {
    {8, 16}, {10, 16}, {11, 16}, {12, 16}, {14, 16}, {15, 16}, {10, 18},
    {12, 18}, {13, 18}, {8, 20},  {10, 20}, {13, 20}, {15, 20}, {-1, 0},
};
/* clang-format on */

/* clang-format off */
#define FIELD_EFFECT_DURATION_SCRIPT_SECTION LATE_DATA
#include "shared/field_effect_duration_scripts.c"

/* The two stakes Pound sinks: each stake, its kind and the flag set once it is */
/* sunk, ended by -1. */
LATE_DATA s16 JupiterLighthouse_WhorlPoundTargets[] = {
    ACTOR_EAST_STAKE, STAKE_STONE, FLAG_EAST_STAKE_SUNK, ACTOR_WEST_STAKE, STAKE_STONE, FLAG_WEST_STAKE_SUNK, -1, 0,
};

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

LATE_DATA u32 SceneExits[] = {                                                        
      SCENE_EXITS(JupiterLighthouseWhorl),         SCENE_EXIT(JUPITER_LIGHTHOUSE_WHORL_TO_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST, JupiterLighthouseRoomPhaetonsBladeChest, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_FROM_JUPITER_LIGHTHOUSE_WHORL),   SCENE_EXIT(2, JupiterLighthouseWhorl, 3),   
      SCENE_EXIT(3, JupiterLighthouseWhorl, 2),    SCENE_EXIT(4, JupiterLighthouseWhorl, 5),   SCENE_EXIT(5, JupiterLighthouseWhorl, 4),   
      SCENE_EXIT(6, JupiterLighthouseWhorl, 7),   SCENE_EXIT(7, JupiterLighthouseWhorl, 6),  SCENE_EXIT(JUPITER_LIGHTHOUSE_WHORL_TO_JUPITER_LIGHTHOUSE_TERRACES, JupiterLighthouseTerraces, JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_WHORL),   
      SCENE_EXIT(9, JupiterLighthouseWhorl, 10),   SCENE_EXIT(10, JupiterLighthouseWhorl, 9), SCENE_EXIT(11, JupiterLighthouseWhorl, 12), 
      SCENE_EXIT(12, JupiterLighthouseWhorl, 11), SCENE_EXITS_END,                                
  };

LATE_DATA ACTOR_PLACE ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ARCHER_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 564, 0, 712, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 216, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 264, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 264, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 600, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 136, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_WHORL), MOVEMENT_TYPE_STAND, 440, 0, 216, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA TRIGGER SceneEvents[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_GREY_BLOCK_JUMP, IF_FLAG_SET(FLAG_THIRD_BLOCK_IN_GAP), EVENT(scJupiterLighthouseJumpForward)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BRIDGE_BUTTON, FLAG_BUTTON_PILLAR_PLACED, EVENT(scJupiterLighthouseStepOnButton)},
    {TRIGGER_STEP, CELL_BEHAVIOR_OFF_BRIDGE_BUTTON, FLAG_BUTTON_PILLAR_PLACED, EVENT(scJupiterLighthouseStepOffButton)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_EAST_OF_GAP, CONDITION_ALWAYS, EVENT(scJupiterLighthouseJumpOrPushBlock)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_SOUTH_OF_GAP, CONDITION_ALWAYS, EVENT(scJupiterLighthouseJumpOrPushBlock)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_WEST_OF_GAP, CONDITION_ALWAYS, EVENT(scJupiterLighthouseJumpOrPushBlock)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_NORTH_OF_GAP, CONDITION_ALWAYS, EVENT(scJupiterLighthouseJumpOrPushBlock)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_WHORL, CONDITION_ALWAYS, EVENT(scJupiterLighthouseFightWhorl)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_STRAIGHT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(scJupiterLighthouseLiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STRAIGHT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(scJupiterLighthousePlacePathBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CORNER_PATH_BLOCK, CONDITION_ALWAYS, EVENT(scJupiterLighthouseLiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CORNER_PATH_BLOCK, CONDITION_ALWAYS, EVENT(scJupiterLighthousePlacePathBlock)},
    /* A push trigger answers every pushed actor; its 35 is never compared. */
    {TRIGGER_PUSH, 35, CONDITION_ALWAYS, EVENT(scJupiterLighthouseLiftPathBlock)},
    {TRIGGER_PUSHED, 35, CONDITION_ALWAYS, EVENT(scJupiterLighthousePlacePathBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_THIRD_PATH_BLOCK, CONDITION_ALWAYS, EVENT(scJupiterLighthouseLiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_PATH_BLOCK, CONDITION_ALWAYS, EVENT(scJupiterLighthousePlacePathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_GREY_BLOCK, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_GREY_BLOCK + 1, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_GREY_BLOCK + 2, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_GREY_BLOCK + 3, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_GREY_BLOCK + 4, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LAST_GREY_BLOCK, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BUTTON_PILLAR, FLAG_BUTTON_PILLAR_PLACED, EVENT(scJupiterLighthouseMoveButtonPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_FIRST), ACTOR_EAST_STAKE, FLAG_EAST_STAKE_SUNK, EVENT(scJupiterLighthouseStartPound)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_EAST_STAKE, FLAG_EAST_STAKE_SUNK, EVENT(scJupiterLighthouseSinkPoundTarget)},
    {PSYNERGY_ON_ACTOR(ABILITY_POUND), ACTOR_EAST_STAKE, FLAG_EAST_STAKE_SUNK, EVENT(scJupiterLighthouseRaiseBlocksFromGaps)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_WEST_STAKE, FLAG_WEST_STAKE_SUNK, EVENT(scJupiterLighthouseSinkPoundTarget)},
    {PSYNERGY_ON_ACTOR(ABILITY_POUND), ACTOR_WEST_STAKE, FLAG_WEST_STAKE_SUNK, EVENT(scJupiterLighthouseRaiseThirdBlockFromGap)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s32 PaletteCycleFrame = -1;

LATE_DATA s32 PaletteCyclePausesInEvents = 1;

LATE_DATA s32 WhirlwindEndScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};
/* clang-format on */

LATE_DATA const s32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};
