/* @file */
/* The four small caves that each keep a Summon's stone tablet, in four scenes
 */
/* that share this overlay. In Indra Cavern, west of where the Lemurian ship */
/* lands on Indra (SceneId_CavernsIndra), Lash ties a rope up to a ledge and */
/* Move pushes a stone pillar across a gap, so that Felix can leap to Zagan's */
/* tablet (scCavernsJumpEastAcrossGap). In Osenia Cavern, far north of */
/* Mikasalla (CavernsOsenia), a maze of rocks walls Megaera's tablet in: Felix
 */
/* climbs down a hole into a tunnel, and Scoop digs the hole at its other end */
/* (scCavernsScoopOseniaPassage). In Atteka Cavern, which only the flying ship
 */
/* reaches (CavernsAtteka), Parch dries the pool before Coatlicue's tablet */
/* (scCavernsParchAttekaWater). In Angara Cavern, north of Loho */
/* (CavernsAngara), a stone door leads down to Haurus's chamber */
/* (scCavernsOpenAngaraDoor), where two grey spiral blocks are pushed, Moved */
/* and Carried until one stands on the other as a step up to the tablet */
/* (scCavernsFinishMovingAngaraBlock). */

#include "scene/event_layer.h"
#include "field/event_inline.h"
#include "field/actor_script.h"
#include "game/game.h"
#include "macros.h"
#include "field/field_sequence.h"
#include "field/map_grid.h"
#include "field/field_psynergy_state.h"

/* Story flags: Osenia Cavern's passage dug open by Scoop, Atteka Cavern's */
/* water dried by Parch. */
#define FLAG_OSENIA_CAVERN_SCOOPED 0x9af
#define FLAG_ATTEKA_CAVERN_PARCHED 0x917
/* Angara Cavern: one block carried on top of the other, and the cells about */
/* the block's path raised while it moves. */
#define FLAG_ANGARA_BLOCK_ON_TOP FLAG_SCENE_TEMP_12
#define FLAG_ANGARA_CELLS_RAISED FLAG_SCENE_TEMP_13
/* Indra Cavern: set once Lash has tied the rope between its posts. */
#define FLAG_INDRA_ROPE_TIED FLAG_SCENE_TEMP_00

/* The map cells the caves' step, examine and Psynergy triggers answer. */
typedef enum CAVERNS_CELL_BEHAVIOR {
	/* Osenia Cavern's two holes down to the tunnel: the west one, and the one
	 */
	/* Scoop digs. Each takes the number of the exit it leads down by. */
	CELL_BEHAVIOR_WEST_HOLE = 2,
	CELL_BEHAVIOR_SCOOPED_HOLE = 3,
	/* The sand in the pocket of rocks where Scoop digs the second hole. */
	CELL_BEHAVIOR_SCOOP_SAND = 10,
	/* Indra Cavern's ledge edge by Zagan's tablet, over the gap. */
	CELL_BEHAVIOR_GAP_EDGE = 10,
	/* Atteka Cavern's green pool before Coatlicue's tablet. */
	CELL_BEHAVIOR_GREEN_POOL = 60,
	/* Angara Cavern's stone door to Haurus's chamber, which takes the number of
	 */
	/* the exit it leads through. */
	CELL_BEHAVIOR_HAURUS_DOOR = 8
} CAVERNS_CELL_BEHAVIOR;

extern s16 Caverns_AngaraBlocks[];
extern s16 Caverns_OseniaRegions[];
extern ACTOR_PLACE Caverns_Placements[];
extern ACTOR_PLACE Caverns_IndraPlacements[];
extern ACTOR_PLACE Caverns_AngaraPlacements[];
extern TRIGGER Caverns_OseniaTriggers[];
extern TRIGGER Caverns_IndraTriggers[];
extern TRIGGER Caverns_AttekaTriggers[];
extern TRIGGER Caverns_AngaraTriggers[];

void scCavernsPlaceAngaraBlocks(const s16 *data);
void scCavernsPrepareOsenia(void);
void scCavernsPrepareAtteka(void);
void scCavernsInitIndra(void);
void scCavernsPrepareAngara(void);
void scCavernsInitAngara(void);
/* Jumps the leader across to the position given when nothing is in the way. */
s32 scCavernsJumpAcross(ACTOR *actor, FIELD_COORDINATE *position);

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

struct ACTOR;

extern u32 FieldEffect_ShortScript[];

extern u32 FieldEffect_MediumScript[];
extern u32 FieldEffect_LongScript[];

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const s16 *scGetExitRegions(void) {
	const s16 *result = NULL;

	if(MainRoutineStateWork.next == SCENE_CAVERNS_OSENIA)
		result = Caverns_OseniaRegions;
	return result;
}

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	s32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_CAVERNS_INDRA)
		return Caverns_IndraPlacements;
	if(scene == SCENE_CAVERNS_ANGARA)
		return Caverns_AngaraPlacements;
	return Caverns_Placements;
}

#include "../field/map_cell_get.c"

#include "shared/map_cell_set.c"

#include "../field/actor_wait_until_landed.c"

/* Nothing visible happens: Angara Cavern's two spiral blocks are made solid */
/* where they stand. The ground under each is raised by a block's height, two */
/* steps, and its cell marked as one the leader jumps onto, so that Felix can */
/* climb onto a block and stand on it. */
void scCavernsPlaceAngaraBlocks(const s16 *data) {
	const u16 *ids = (const u16 *)data;
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *actor;
	s32 height;
	s32 cell;

	for(; *ids != ACTOR_LIST_END; ids++) {
		actor = fdActorGetById(*ids);
		actor->motionFlg = 0;
		actor->collisionFlg = 0;
		actor->bounce = 0;
		fdActorSetSprFlags((ACTOR *)actor, 0);
		height = CELL_HEIGHT_STEPS(
					 fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed)) +
				 2;
		fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 0, height);
		fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 2, height);
		cell = fdMapCellGetAttributes(0, FIELD_CELL(actor->x.fixed),
									  FIELD_CELL(actor->z.fixed));
		actor->sceneWord = cell;
		scMapCellSetAttributes(0, FIELD_CELL(actor->x.fixed),
							   FIELD_CELL(actor->z.fixed),
							   cell | MAP_CELL_JUMP);
	}
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		sySynch(1);
		leader->y.fixed = leader->groundHeight = fdMapGetTerrainHeight(
			leader->layer, leader->x.fixed, leader->z.fixed);
		leader->velY = 0;
		leader->tgtY = ACTOR_NO_TARGET;
		fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 0);
	}
}

/* The terrain height of layer 2 at a point. */
static __inline__ s32 scCavernsGetLayer2Height(s32 x, s32 z) {
	return fdMapGetTerrainHeight(2, x, z);
}

/* The whole pixels of an actor's z coordinate. */
static __inline__ s32 scActorGetPixelZ(ACTOR *actor) {
	return actor->z.part[0].integer;
}

/* Sets the layer 2 cell at (x, z), 16.16 pixels, to the ground height there */
/* plus offset, in height steps, with `height` keeping it. */
#define SET_CELL_HEIGHT(x, z)                                                  \
	height = CELL_HEIGHT_STEPS(scCavernsGetLayer2Height(x, z)) + offset;       \
	syFieldMapGridSetCellHeight(x, z, 2, height)

/* The z of Angara Cavern's two rows of cells the blocks start in: the second */
/* block's, and the first's south of it. */
#define ANGARA_NORTH_ROW_Z 536
#define ANGARA_SOUTH_ROW_Z 552

/* Nothing visible happens: while a spiral block moves, the floor around its */
/* path in Angara's chamber is raised out of its way (`offset` 1) and lowered */
/* back afterwards (-1), by eight steps: the two cells by the ledge while the */
/* block is up on it, else the three cells of whichever of the two rows the */
/* block is not in. */
void scCavernsShiftAngaraCellHeights(s32 offset) {
	ACTOR *block = FieldStateWork->movedBlock;
	s32 height;

	offset <<= 3;
	if(block->y.part[0].integer > 31 ||
	   fdEventGetFlag(FLAG_ANGARA_CELLS_RAISED)) {
		SET_CELL_HEIGHT(toFIXED(568), toFIXED(ANGARA_NORTH_ROW_Z));
		SET_CELL_HEIGHT(toFIXED(584), toFIXED(ANGARA_SOUTH_ROW_Z));
		if(offset == 8)
			fdEventSetFlag(FLAG_ANGARA_CELLS_RAISED);
		else
			fdEventClearFlag(FLAG_ANGARA_CELLS_RAISED);
	}
	else if(scActorGetPixelZ(block) == ANGARA_NORTH_ROW_Z ||
			fdEventGetFlag(FLAG_ANGARA_CELLS_RAISED)) {
		SET_CELL_HEIGHT(toFIXED(536), toFIXED(ANGARA_SOUTH_ROW_Z));
		SET_CELL_HEIGHT(toFIXED(552), toFIXED(ANGARA_SOUTH_ROW_Z));
		SET_CELL_HEIGHT(toFIXED(568), toFIXED(ANGARA_SOUTH_ROW_Z));
		/* DEAD: offset is 8 or -8, never 6, so this branch only clears the
		 * flag; */
		/* the ROM keeps the compare and the set. */
		if(offset == 6)
			fdEventSetFlag(FLAG_ANGARA_CELLS_RAISED);
		else
			fdEventClearFlag(FLAG_ANGARA_CELLS_RAISED);
	}
	else {
		SET_CELL_HEIGHT(toFIXED(536), toFIXED(ANGARA_NORTH_ROW_Z));
		SET_CELL_HEIGHT(toFIXED(552), toFIXED(ANGARA_NORTH_ROW_Z));
		SET_CELL_HEIGHT(toFIXED(568), toFIXED(ANGARA_NORTH_ROW_Z));
	}
}

/* Nothing visible happens as Felix starts to push, Move or Carry one of the */
/* two spiral blocks in Angara Cavern's chamber: the game finds the block (the
 */
/* one ahead of him, or the one the Psynergy's hand holds) and lowers the */
/* ground under it back to the floor, so that the block can leave its cell. */
void scCavernsStartMovingAngaraBlock(void) {
	FIXED position[3];
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	FIELD_EFFECT_STATE *current = Ram_HeapSlots->fieldEffState;
	const u16 *ids = (const u16 *)Caverns_AngaraBlocks;
	ACTOR *actor = NULL;
	ACTOR *leader;
	s32 height;
	s32 angle;

	state->movedBlock = NULL;
	if(current != NULL && state->psynergy != 0) {
		actor = state->movedBlock = current->tgt;
		if(current->ability != ABILITY_CARRY)
			scCavernsShiftAngaraCellHeights(1);
	}
	else {
		leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
		angle = FACING_QUARTER(leader->facing + DEGtoANG(45));
		position[0] = leader->x.fixed;
		position[1] = leader->y.fixed;
		position[2] = leader->z.fixed;
		syVectorAddPolarOffset(toFIXED(16), angle, position);
		while(*ids != ACTOR_LIST_END) {
			actor = fdActorGetById(*ids);
			if(FIELD_CELL(position[0]) == FIELD_CELL(actor->x.fixed) &&
			   position[1] / toFIXED(1) == actor->y.fixed / toFIXED(1) &&
			   FIELD_CELL(position[2]) == FIELD_CELL(actor->z.fixed))
				break;
			ids++;
			actor = NULL;
		}
		if(actor == NULL)
			return;
		state->movedBlock = actor;
		scCavernsShiftAngaraCellHeights(1);
	}
	height = CELL_HEIGHT_STEPS(
				 fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed)) -
			 2;
	fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 0, height);
	fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 2, height);
	scMapCellSetAttributes(0, FIELD_CELL(actor->x.fixed),
						   FIELD_CELL(actor->z.fixed), actor->sceneWord);
}

/* A dust puff loses a DUST_DRAG_X-th of its speed east or west each frame, and
 */
/* a DUST_DRAG_Z-th of its speed north or south. */
enum { DUST_DRAG_X = 12, DUST_DRAG_Z = 10 };

/* One puff of the ring of dust a fallen block throws up: each frame it drifts
 */
/* outwards, slows, grows and spins. */
void scCavernsDriftDust(FIELD_OBJECT *object) {
	FIELD_EFFECT *effect = &object->eff;

	effect->x += effect->vel.x;
	effect->y += effect->vel.y;
	effect->z += effect->vel.z;
	effect->vel.x -= effect->vel.x / DUST_DRAG_X;
	effect->vel.z -= effect->vel.z / DUST_DRAG_Z;
	effect->scaleX += effect->scaleRateX;
	effect->scaleY += effect->scaleRateY;
	effect->spr->rotation += effect->spin;
}

/* The block stacked on top of the other, each frame: Felix is drawn in front */
/* of it while he stands south of it, and behind it otherwise. */
void scCavernsLayerLeaderByTopBlock(ACTOR *actor) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	if(leader->z.fixed > actor->z.fixed) {
		leader->priFlg |= ACTOR_PRIORITY_OVERHEAD;
	}
	else {
		leader->priFlg &= ~(ACTOR_PRIORITY_UNDERFOOT | ACTOR_PRIORITY_OVERHEAD);
	}
}

/* When a push, Move or Carry has finished moving one of the spiral blocks in */
/* Angara Cavern's chamber, a block left with no ground under it drops: onto */
/* the floor below with a thump and a ring of dust, or onto the other block, */
/* where it stays, the two stacked into a step up to Haurus's tablet. Then the
 */
/* ground under the block rises again so that it stands solid where it is. */
/* A block dropping onto a cell that has a behaviour sinks out of sight */
/* instead, as a yellow block, and is gone. */
void scCavernsFinishMovingAngaraBlock(void) {
	FIXED velocity[3];
	EFFECT_OPTIONS options;
	FIELD_STATE *state;
	FIELD_EFFECT_STATE *current;
	ACTOR *actor;
	ACTOR *other;
	s32 kind;
	s32 height;
	s32 value;
	s32 angle;
	u32 i;
	ACTOR *spawned;
	s32 sum;

	state = Ram_HeapSlots->fieldState;
	current = Ram_HeapSlots->fieldEffState;
	actor = state->movedBlock;
	if(current != NULL && state->psynergy != 0) {
		if(current->ability != ABILITY_CARRY)
			scCavernsShiftAngaraCellHeights(-1);
	}
	else {
		scCavernsShiftAngaraCellHeights(-1);
	}
	if(fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed) !=
	   actor->y.fixed) {
		actor->layer = 2;
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		actor->bounce = 0;
		sySynch(1);
		kind = fdMapGridGetCellBehaviorAt(2, actor->x.fixed, actor->z.fixed);
		if(kind != 0) {
			actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
			REG_BLDALPHA = BLEND_ALPHA(0, 16);
			REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
			/* FAKEMATCH(temporary): sum holds kind + 2 for the one call; as
			 * kind + 2 at its use. */
			sum = kind + 2;
			spawned = fdActorCreateSilhouette(
				actor->x.fixed, 0, actor->z.fixed + FIELD_CELL_ORIGIN(sum),
				SPRITE_YELLOW_BLOCK);
			sySndPlay(SE_DUST_PUFF);
			fdActorWaitUntilLanded(actor);
			fdActorWaitUntilLanded(actor);
			actor->x.fixed = actor->y.fixed = actor->z.fixed = 0;
			fdActorRelease(spawned);
			sySynch(1);
			REG_BLDCNT = 0;
		}
		else {
			fdActorWaitUntilLanded(actor);
			fdActorWaitUntilLanded(actor);
			sySndPlay(SE_THUMP);
			options.pal = SPRITE_COLOR_WHITE;
			options.pri = 2;
			options.update = scCavernsDriftDust;
			options.startScaleX = FIXED_ONE * 4 / 5;
			options.startScaleY = FIXED_ONE * 4 / 5;
			i = 0;
			do {
				angle = i * (TRIG_TURN / DUST_RING_PUFFS);
				velocity[0] = syCos(angle) * 2;
				velocity[1] = 0;
				velocity[2] = sySin(angle) * 2;
				velocity[0] += velocity[0] >> 1;
				velocity[0] += (u16)(syRandom() * 3 >> 2) - (toFIXED(3) / 8);
				velocity[2] += (u16)(syRandom() * 3 >> 3) - (toFIXED(3) / 16);
				/* FAKEMATCH(dead): sum, never read again, assigned the flags
				 * inside the call's arguments. */
				scFieldEffectSpawn(
					actor->x.fixed, actor->y.fixed, actor->z.fixed, velocity[0],
					velocity[1], velocity[2],
					sum = EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
						  EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
					&options);
				i++;
			} while(i <= DUST_RING_PUFFS);
		}
		actor->motionFlg = 0;
		actor->layer = 0;
		/* The velocity array is reused for the position under the actor. */
		velocity[0] = actor->x.fixed;
		velocity[1] = actor->y.fixed - toFIXED(16);
		velocity[2] = actor->z.fixed;
		other = fdActorFindAtCell(velocity, 1);
		if(other != NULL) {
			actor->update = (OBJECT_UPDATE)scCavernsLayerLeaderByTopBlock;
			actor->sceneCnt = 1;
			other->sceneCnt = 1;
			syGameFlagSet(FLAG_ANGARA_BLOCK_ON_TOP);
		}
	}
	height = CELL_HEIGHT_STEPS(
				 fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed)) +
			 2;
	fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 0, height);
	fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 2, height);
	value = fdMapCellGetAttributes(0, FIELD_CELL(actor->x.fixed),
								   FIELD_CELL(actor->z.fixed));
	actor->sceneWord = value;
	scMapCellSetAttributes(0, FIELD_CELL(actor->x.fixed),
						   FIELD_CELL(actor->z.fixed), value | MAP_CELL_JUMP);
}

const TRIGGER *scGetTriggers(void) {
	s32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_CAVERNS_INDRA)
		return Caverns_IndraTriggers;
	if(scene == SCENE_CAVERNS_ATTEKA)
		return Caverns_AttekaTriggers;
	if(scene == SCENE_CAVERNS_ANGARA)
		return Caverns_AngaraTriggers;
	return Caverns_OseniaTriggers;
}

s32 scInit(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(MainRoutineStateWork.next == SCENE_CAVERNS_INDRA)
		scCavernsInitIndra();
	if(MainRoutineStateWork.next == SCENE_CAVERNS_ANGARA)
		scCavernsInitAngara();
	return 0;
}

s32 scPrepareMap(void) {
	s32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_CAVERNS_OSENIA)
		scCavernsPrepareOsenia();
	else if(scene == SCENE_CAVERNS_ATTEKA)
		scCavernsPrepareAtteka();
	else if(scene == SCENE_CAVERNS_ANGARA)
		scCavernsPrepareAngara();
	return 0;
}

/* Indra Cavern's row of cells the pillar stands and moves in (z 168), and the
 */
/* column of its cell marked as a gap to jump onto, which is copied under the */
/* pillar. */
#define INDRA_PILLAR_ROW 10
#define INDRA_GAP_COLUMN 16

/* Indra Cavern's things: the post on the floor with the rope Lash throws, the
 */
/* post up on the east ledge it is tied to, and the stone pillar Move pushes */
/* across the gap between the ledges. */
typedef enum CAVERNS_INDRA_ACTOR_ID {
	ACTOR_LOWER_ROPE_POST = ACTOR_FIRST_PLACED,
	ACTOR_UPPER_ROPE_POST,
	ACTOR_PILLAR
} CAVERNS_INDRA_ACTOR_ID;

/* Indra Cavern on entry: the rope lies by its post, or runs up to the ledge */
/* once Lash has tied it (FLAG_INDRA_ROPE_TIED); the stone pillar stands still,
 */
/* and its cell is one the leader jumps onto. */
void scCavernsInitIndra(void) {
	ACTOR *actor;

	fdActorGetById(ACTOR_FELIX)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdRopeInit();
	fdRopePlace(0, FLAG_INDRA_ROPE_TIED, ACTOR_LOWER_ROPE_POST,
				ACTOR_UPPER_ROPE_POST);
	actor = fdActorGetById(ACTOR_PILLAR);
	actor->motionFlg = 0;
	{
		s32 row = INDRA_PILLAR_ROW;

		fdMapCopyCellAttributeRect(INDRA_GAP_COLUMN, row, 1, 1,
								   FIELD_CELL(actor->x.fixed), row);
	}
}

/* Move on the stone pillar at the edge of the ledge by Zagan's tablet pushes */
/* it one cell east into the gap; afterwards its new cell is the one the */
/* leader jumps onto, and the cells it left are plain floor again. */
void scCavernsMoveIndraPillar(s32 unused, s32 id) {
	ACTOR *actor = fdActorGetById(id);

	actor->motionFlg = 0;
	fdMapCopyCellAttributeRect(13, 12, 2, 1, 13, INDRA_PILLAR_ROW);
	fdMapCopyCellAttributeRect(INDRA_GAP_COLUMN, INDRA_PILLAR_ROW, 1, 1,
							   FIELD_CELL(actor->x.fixed), INDRA_PILLAR_ROW);
}

s32 scCavernsJumpAcross(ACTOR *actor, FIELD_COORDINATE *position) {
	s32 saved = actor->motionFlg;
	s32 blocked = fdMapGridCheckCollision(actor, position);

	if(blocked == 0) {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
		sySynch(6);
		sySndPlay(SE_JUMP);
		fdActorSelectAnimation(actor, ANIM_LEADER_LEAP);
		actor->speedLimit = toFIXED(3);
		actor->acceleration = toFIXED(2);
		actor->velY = toFIXED(4);
		actor->motionFlg &= ACTOR_JUMP_KEEPS_MOTION;
		fdActorSetSprFlags(actor, 0);
		{
			MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
			s32 x = position[0].part[0].integer;
			s32 z = position[2].part[0].integer;
			s32 owner = party->leaderActorId;

			fdActorMoveToAndWaitById(owner, x, z);
		}
		fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
		fdActorSetSprFlags(actor, 1);
		actor->motionFlg = saved;
		fdEventEnd();
		return 1;
	}
	return 0;
}

/* Walking east off the ledge by Zagan's tablet in Indra Cavern (cell (12, 10))
 */
/* makes Felix spring and leap two cells east over the gap, when nothing blocks
 */
/* where he would land. */
void scCavernsJumpEastAcrossGap(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	s32 position[3];
	FIXED x;

	/* Once Move has pushed the stone pillar east into the gap, Felix does not
	 */
	/* leap: walking east from the ledge he steps onto the pillar's top, level
	 */
	/* with the ledges, and on onto the far ledge by the rope's post. */
	x = leader->x.fixed;
	x += toFIXED(32);
	SET_VECTOR(position, x, leader->y.fixed, leader->z.fixed);
	scCavernsJumpAcross(leader, (FIELD_COORDINATE *)position);
}

/* Osenia Cavern's one actor, the row of Caverns_Placements: an unseen marker */
/* scCavernsWalkIntoPassage sets down by the hole as Felix climbs into it. */
typedef enum CAVERNS_OSENIA_ACTOR_ID {
	ACTOR_PASSAGE_MARKER = ACTOR_FIRST_PLACED
} CAVERNS_OSENIA_ACTOR_ID;

void scCavernsOpenOseniaPassage(void);

/* Osenia Cavern's map: once Scoop has dug it, the second hole stays open. */
void scCavernsPrepareOsenia(void) {
	s32 scooped;

	scooped = fdEventGetFlag(FLAG_OSENIA_CAVERN_SCOOPED);
	if(scooped != 0) {
		scCavernsOpenOseniaPassage();
	}
	return;
}

/* Digs Osenia Cavern's second hole into the map: a hole and the cells around */
/* it, copied with their behaviours from a spare one beside the cave, over the
 */
/* sand in the pocket of rocks (cells (18, 12) to (20, 14)). */
void scCavernsOpenOseniaPassage(void) {
	fdMapGridCopyCellTilesIndicesRect(35, 12, 18, 12, 3, 3);
	fdMapGridCopyCellAttributes(35, 12, 3, 3, 18, 12);
	fdMapGridCopyCellTilesIndicesRect(84, 13, 83, 13, 1, 1);
	return;
}

/* Felix, over one of Osenia Cavern's two holes, turns his back and climbs down
 */
/* into it, and the screen leaves for the tunnel below: from the west hole to */
/* its west end, from the hole Scoop dug to its east end. */
void scCavernsWalkIntoPassage(void) {
	s32 value = ((FIELD_STATE *)Ram_HeapSlots->fieldState)->stepTrigger;

	if(value == CELL_BEHAVIOR_WEST_HOLE)
		fdActorSetPos(ACTOR_PASSAGE_MARKER, 144, 224);
	else
		fdActorSetPos(ACTOR_PASSAGE_MARKER, 224, 224);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
	fdEventWait(10);
	fdActorGetById(ACTOR_FELIX)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdMapGridCopyCellsTo(96, 14, 72, 14, 2, 1);
	fdActorSetDestinationOffsetById(ACTOR_FELIX, 0, 4);
	fdActorWaitForMoveById(ACTOR_FELIX);
	fdActorSetSprFlags(fdActorGetById(ACTOR_FELIX), 0);
	fdActorSetDestinationOffsetById(ACTOR_FELIX, 0, 16);
	fdActorWaitForMoveById(ACTOR_FELIX);
	fdActorSetAnimationById(ACTOR_FELIX, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
	sySndPlay(SE_MAP_EXIT);
	fdEventWait(14);
	syImportEventSetExitRequest(value);
}

/* Walking east into one of Osenia Cavern's holes in the floor (cell (8, 13), */
/* or (18, 13) once Scoop has dug the second) brings Felix round over the hole,
 */
/* and he climbs down into it to the tunnel below. */
void scCavernsWalkEastIntoPassage(void) {
	fdActorWalkByAndWait(ACTOR_FELIX, 0, -14);
	fdActorWalkByAndWaitById(ACTOR_FELIX, 16, 0);
	fdEventWait(10);
	scCavernsWalkIntoPassage();
	return;
}

/* Walking west into one of Osenia Cavern's holes in the floor (cell (10, 13),
 */
/* or (20, 13) once Scoop has dug the second) brings Felix round over the hole,
 */
/* and he climbs down into it to the tunnel below. */
void scCavernsWalkWestIntoPassage(void) {
	fdActorWalkByAndWait(ACTOR_FELIX, 0, -14);
	fdActorWalkByAndWaitById(ACTOR_FELIX, -16, 0);
	fdEventWait(10);
	scCavernsWalkIntoPassage();
	return;
}

/* Walking south into one of Osenia Cavern's holes in the floor (cell (9, 12),
 */
/* or (19, 12) once Scoop has dug the second) has Felix turn his back, as on a
 */
/* ladder, and climb down into it to the tunnel below. */
void scCavernsWalkSouthIntoPassage(void) {
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
	fdEventWait(10);
	scCavernsWalkIntoPassage();
	return;
}

/* Scoop on the sand in the pocket of rocks east of Osenia Cavern's middle */
/* (cell (19, 13)) digs a second hole there for good, the tunnel's way up to */
/* Megaera's side of the rocks. */
void scCavernsScoopOseniaPassage(s32 mode) {
	if(mode == SCOOP_STAGE_DUG) {
		scCavernsOpenOseniaPassage();
		syGameFlagSet(FLAG_OSENIA_CAVERN_SCOOPED);
	}
	return;
}

/* Atteka Cavern's map: once Parch has dried the pool, it stays dry. */
void scCavernsPrepareAtteka(void) {
	s32 parched;

	parched = fdEventGetFlag(FLAG_ATTEKA_CAVERN_PARCHED);
	if(parched != 0) {
		fdMapGridCopyCellAttributes(31, 43, 9, 12, 48, 47);
		fdMapGridCopyCellsTo(30, 110, 48, 110, 9, 13);
		fdMapGridCopyCellAttributes(52, 110, 1, 1, 52, 111);
	}
	return;
}

/* Parch on the green pool before Coatlicue's tablet in Atteka Cavern dries it
 */
/* up for good in a burst of heat and a white flash, opening the way to the */
/* tablet. */
void scCavernsParchAttekaWater(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdMapGridCopyCellAttributes(31, 43, 9, 11, 48, 47);
	fdMapGridCopyCellsTo(30, 110, 48, 110, 9, 13);
	fdMapGridCopyCellAttributes(52, 110, 1, 1, 52, 111);
	syGameFlagSet(FLAG_ATTEKA_CAVERN_PARCHED);
	fdEventEnd();
	return;
}

/* Angara Cavern's map: when the party arrives by entrance 5 or 6, the two */
/* staircases at cells (28, 5) and (36, 5) lead on by the exits of cells */
/* (28, 0) and (36, 0) instead of their own. */
void scCavernsPrepareAngara(void) {
	if(MainRoutineStateWork.entrance >= 5 &&
	   MainRoutineStateWork.entrance <= 6) {
		fdMapGridCopyCellAttributes(28, 0, 1, 1, 28, 5);
		fdMapCopyCellAttributeRect(36, 0, 1, 1, 36, 5);
	}
}

/* Angara Cavern's things in Haurus's chamber, numbering */
/* Caverns_AngaraPlacements. */
typedef enum CAVERNS_ANGARA_ACTOR_ID {
	/* The two grey blocks carved with a spiral, one on the floor and one up on
	 */
	/* the ledge, carried until one stands on top of the other. */
	ACTOR_FIRST_BLOCK = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_BLOCK,
	/* The lit candlesticks on either side of Haurus's tablet, which the
	   searches */
	/* for a block skip. */
	ACTOR_WEST_CANDLESTICK,
	ACTOR_EAST_CANDLESTICK
} CAVERNS_ANGARA_ACTOR_ID;

/* Angara Cavern on entry: the two spiral blocks stand solid where they were */
/* left, are drawn over the ground's rises, and the tablet's candlesticks are */
/* left out of the search for a block to push. */
void scCavernsInitAngara(void) {
	scCavernsPlaceAngaraBlocks(Caverns_AngaraBlocks);
	fdActorGetById(ACTOR_FIRST_BLOCK)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGetById(ACTOR_SECOND_BLOCK)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGetById(ACTOR_WEST_CANDLESTICK)->collisionFlg |= ACTOR_NOT_TARGETED;
	fdActorGetById(ACTOR_EAST_CANDLESTICK)->collisionFlg |= ACTOR_NOT_TARGETED;
}

/* Walking north into the grey stone door at the west end of the small cave */
/* room in Angara Cavern (cell (6, 32)) opens it, and the screen wipes to */
/* Haurus's chamber beyond. */
void scCavernsOpenAngaraDoor(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	sySndPlay(SE_DOOR_OPEN);
	fdEventWait(10);
	fdMapGridCopyCellsTo(7, 94, 6, 94, 1, 2);
	fdEventWait(16);
	sySndPlay(SE_MAP_EXIT);
	syImportEventSetExitRequest(CELL_BEHAVIOR_HAURUS_DOOR);
	fdEventEnd();
}

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* The spiral blocks of Angara Cavern's chamber, by actor id, ended by -1. */
s16 Caverns_AngaraBlocks[] = {
    ACTOR_FIRST_BLOCK, ACTOR_SECOND_BLOCK, -1, 0,
};

const s32 *FieldEffect_DurationScripts[] = {
    FieldEffect_ShortScript,
    FieldEffect_MediumScript,
    FieldEffect_LongScript,
};

SCENE_ENTRANCE SceneEntrances[] = {
    {0, CONDITION_ALWAYS, 467, 0, 153, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

/* Osenia Cavern's exit regions: the tunnel's two ends, climbed up to the west */
/* hole (exit 4) and to the one Scoop digs (exit 5). */
s16 Caverns_OseniaRegions[] = {
    132, 28, 692, 140, 36, 700, -1, 4,
    292, 28, 692, 300, 36, 700, -1, 5,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 SceneExits[] = {
    SCENE_EXITS(CavernsOsenia),
    SCENE_EXIT_UNLESS(CAVERNS_OSENIA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CAVERNS_OSENIA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, CavernsOsenia, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, CavernsOsenia, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, CavernsOsenia, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, CavernsOsenia, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(CavernsIndra),
    SCENE_EXIT_UNLESS(CAVERNS_INDRA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CAVERNS_INDRA, EXIT_FLAG_NONE),
    SCENE_EXITS(CavernsAtteka),
    SCENE_EXIT_UNLESS(CAVERNS_ATTEKA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CAVERNS_ATTEKA, EXIT_FLAG_NONE),
    SCENE_EXITS(CavernsAngara),
    SCENE_EXIT_UNLESS(CAVERNS_ANGARA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CAVERNS_ANGARA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, CavernsAngara, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, CavernsAngara, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, CavernsAngara, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, CavernsAngara, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, CavernsAngara, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, CavernsAngara, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, CavernsAngara, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, CavernsAngara, 8, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/* Osenia and Atteka Caverns' actors: the passage marker. */
ACTOR_PLACE Caverns_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* Indra Cavern's actors, in CAVERNS_INDRA_ACTOR_ID's order. */
ACTOR_PLACE Caverns_IndraPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 168, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 168, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 168, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* Angara Cavern's actors, in CAVERNS_ANGARA_ACTOR_ID's order. */
ACTOR_PLACE Caverns_AngaraPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 552, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 536, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 512, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 512, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* Osenia Cavern's exit, its two holes walked into from three sides, and Scoop */
/* on the sand where the second is dug. */
TRIGGER Caverns_OseniaTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, CAVERNS_OSENIA_TO_WORLD_MAP},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_WEST_HOLE, CONDITION_ALWAYS, EVENT(scCavernsWalkSouthIntoPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_WEST_HOLE, CONDITION_ALWAYS, EVENT(scCavernsWalkWestIntoPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_WEST_HOLE, CONDITION_ALWAYS, EVENT(scCavernsWalkEastIntoPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_SCOOPED_HOLE, CONDITION_ALWAYS, EVENT(scCavernsWalkSouthIntoPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_SCOOPED_HOLE, CONDITION_ALWAYS, EVENT(scCavernsWalkWestIntoPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_SCOOPED_HOLE, CONDITION_ALWAYS, EVENT(scCavernsWalkEastIntoPassage)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_SCOOP_SAND, FLAG_OSENIA_CAVERN_SCOOPED, EVENT(scCavernsScoopOseniaPassage)},
    {TRIGGER_TABLE_END},
};

/* Indra Cavern's exit, the ledge's edge by the gap, Lash on the rope post and */
/* Move on the pillar. */
TRIGGER Caverns_IndraTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, CAVERNS_INDRA_TO_WORLD_MAP},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_GAP_EDGE, CONDITION_ALWAYS, EVENT(scCavernsJumpEastAcrossGap)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_LOWER_ROPE_POST, FLAG_INDRA_ROPE_TIED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(scCavernsMoveIndraPillar)},
    {TRIGGER_TABLE_END},
};

/* Atteka Cavern's exit and Parch on its pool. */
TRIGGER Caverns_AttekaTriggers[] = {
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, CAVERNS_ATTEKA_TO_WORLD_MAP},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_GREEN_POOL, CONDITION_ALWAYS, EVENT(scCavernsParchAttekaWater)},
    {TRIGGER_TABLE_END},
};

/* Angara Cavern's exits and stairs, its stone door, and pushing, Move and Carry */
/* on either spiral block until they are stacked. */
TRIGGER Caverns_AngaraTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, CAVERNS_ANGARA_TO_WORLD_MAP},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_HAURUS_DOOR, CONDITION_ALWAYS, EVENT(scCavernsOpenAngaraDoor)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_PUSH, 0, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(scCavernsStartMovingAngaraBlock)},
    {TRIGGER_PUSHED, 0, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(scCavernsFinishMovingAngaraBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(scCavernsStartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(scCavernsFinishMovingAngaraBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SECOND_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(scCavernsStartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(scCavernsFinishMovingAngaraBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(scCavernsStartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(scCavernsFinishMovingAngaraBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SECOND_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(scCavernsStartMovingAngaraBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_SECOND_BLOCK, FLAG_ANGARA_BLOCK_ON_TOP, EVENT(scCavernsFinishMovingAngaraBlock)},
    {TRIGGER_TABLE_END},
};
