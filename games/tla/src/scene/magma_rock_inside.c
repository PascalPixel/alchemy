/* @file */
/* Magma Rock's inside, the rooms in the volcano that the great blast on its */
/* top opened. Three halls are each watched by a great stone face: Burst on */
/* the small statue beside it makes the statue spit a fireball into the face, */
/* whose eyes light red as it pours lava over the hall's floor, and a floor */
/* switch beside a grated gate lets the lava drain away again. While the lava */
/* is up, platforms float round it and Felix leaps from its edge onto them, or
 */
/* onto the pillars he has pushed into it. A Mars Djinni left over from the */
/* first journey may wait in the entrance hall's south, Fury waits in its own */
/* room, and the Lucky Medal, Mist Potion, Salamander Tail and Golem Core lie */
/* in chests. Deep inside, stepping onto an islet in the lava sets off a rock */
/* that bursts out of it, throwing burning rocks and flames round the room; a */
/* crest on the stone wall there opens into a stairway to a sea of lava crossed
 */
/* on stepping stones, where Jenna learns Blaze at the tablet. Blaze then */
/* carries a flame to a torch to sink the pillar on the way out, and back in */
/* the ember stone's room carries the fallen flames to the two torches that let
 */
/* the party reach the stone and take the Magma Ball from it. */

#include "graphics/palette_buffers.h"
#include "scene/falling_rocks.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "item_ids.h"
#include "field/event_inline.h"
#include "field/field_sequence.h"
#include "scene/magma_rock.h"
#include "scene/scene.h"
#include "game/game.h"
#include "macros.h"
#include "field/call.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

extern MAP_CELL MapGridCellBuffer[];

void scSetActorSprPriority(ACTOR *actor, s32 priority);

extern s32 MagmaRockInside_FlameScript[];

extern s32 MagmaRockInside_LastStepCell;

void scMagmaRockInsideBurstCrackedPillar(s32 stage, s32 cue);
void scMagmaRockInsideTrackStepCell(void);
void fdActorSetCellShape(s32 heightOffset, s32 layer, s32 owner);

/* Creates an actor of the given sprite through the overlay's import. */
static __inline__ ACTOR *Create(s32 kind, s32 x, s32 y, s32 z) {
	return syImportActorCreate(kind, x, y, z);
}
#include "shared/trial_road.h"
#include "shared/tables.h"
#include "shared/tundaria_tower.h"

/* The lava rises in three parts of the volcano, each when a statue's fireball
 */
/* strikes the great stone face there, and drains when a floor switch by that */
/* part's gate is pressed; the flags hold the state until the party comes back
 */
/* in from outside. */
#define FLAG_ENTRANCE_FLOODED	 FLAG_AREA_TEMP_00
#define FLAG_FURY_FLOODED		 FLAG_AREA_TEMP_01
#define FLAG_LOWER_ROOMS_FLOODED FLAG_AREA_TEMP_02
#define FLAG_WEST_TORCH_LIT		 FLAG_AREA_TEMP_03
#define FLAG_EAST_TORCH_LIT		 FLAG_AREA_TEMP_04
#define FLAG_TABLET_TORCH_LIT	 FLAG_AREA_TEMP_05
/* Set as Burst breaks each of the cracked pillars' room's two pillars, until */
/* the party leaves Magma Rock. */
#define FLAG_INSIDE_CRACKED_PILLAR_A_BURST FLAG_AREA_TEMP_50
#define FLAG_INSIDE_CRACKED_PILLAR_B_BURST FLAG_AREA_TEMP_51
/* Each pushed pillar's flag keeps it where it was pushed. */
#define FLAG_ENTRANCE_PILLAR_PUSHED 0x9f0
#define FLAG_FURY_PILLAR_PUSHED		0x9f1
#define FLAG_PILLAR_A_PUSHED		0x9f2
#define FLAG_PILLAR_B_PUSHED		0x9f3
#define FLAG_PILLAR_C_PUSHED		0x9f4
#define FLAG_NORTH_PILLAR_PUSHED	0x9f5
#define FLAG_EAST_PILLAR_PUSHED		0x9f6
#define FLAG_LEDGE_PILLAR_PUSHED	0x9f7
#define FLAG_ROCKS_RELEASED			0x9f8
#define FLAG_MAGMA_BALL_TAKEN		0x9f9
#define FLAG_BLAZE_LEARNED			0x9fa
/* The Blaze tablet's room: the map's upper layer numbers some of the stones */
/* on the way to the tablet; stopping on one raises the trigger */
/* STEP_CELL_TRIGGER_BASE plus its number, and its flag then keeps the stone */
/* behind it gone. */
#define FLAG_ANY_MARKER_LIT		FLAG_SCENE_TEMP_00
#define FLAG_MARKER_LIT(marker) (FLAG_SCENE_TEMP_00 + (marker))
/* The trigger a numbered stone of the Blaze tablet's room raises, less its */
/* number. */
enum { STEP_CELL_TRIGGER_BASE = 200 };
/* The numbers the Blaze tablet room's upper layer gives the cells of column 16
 */
/* on the way to the tablet: the end of the entrance's walkway (row 26), then */
/* a stone every second row, up to the foot of the tablet's platform (row 14).
 */
/* The five stones between are the ones that fade away. */
enum { MARKER_WALKWAY_END = 1, MARKER_PLATFORM_FOOT = 7 };

/* The cell behaviours the rooms' maps mark their spots with, which the */
/* trigger tables answer. A room may use a number in its own sense. */
typedef enum MAGMA_ROCK_INSIDE_CELL_BEHAVIOR {
	/* A floor switch beside a lava gate; the Golem Core's room has a second. */
	CELL_FLOOR_SWITCH = 30,
	CELL_SECOND_FLOOR_SWITCH = 31,
	/* The ember stone's room: the islets in the lava that set the stone off, */
	/* and the stone where it lands. */
	CELL_ERUPTION_ISLET = 30,
	CELL_EMBER_STONE = 31,
	/* The Blaze tablet, read from any side. */
	CELL_BLAZE_TABLET = 30,
	/* The cell before a fire-spitting statue that Burst is cast at. */
	CELL_STATUE = 50,
	CELL_LOWER_WEST_STATUE = 51,
	CELL_LOWER_EAST_STATUE = 52,
	/* The lava's edge, where Felix leaps onto what stands in the lava; the */
	/* cracked pillars' room's two pillars stand on edge cells of their own. */
	CELL_LAVA_EDGE = 70,
	CELL_CRACKED_PILLAR_A = 71,
	CELL_CRACKED_PILLAR_B = 72,
	CELL_LAVA_EDGE_NORTH = 73, /* leapt from walking north */
	CELL_LAVA_EDGE_WEST = 75,  /* leapt from walking west */
	CELL_LAVA_EDGE_EAST = 78,  /* the far side of the Lucky Medal room's lava */
	/* The Blaze tablet room's stepping stones, two pictures of them in turn. */
	CELL_STEPPING_STONE_A = 80,
	CELL_STEPPING_STONE_B = 81
} MAGMA_ROCK_INSIDE_CELL_BEHAVIOR;

/* The cracked pillars' room's entrances up to this one open onto its upper */
/* hall, whose lava rises with Fury's room's; the rest onto its lower rooms. */
enum { CRACKED_PILLARS_UPPER_HALL_LAST_ENTRANCE = 7 };

/* The Golem Core room's entrances up to this one open onto its north part, */
/* by the north floor switch; the rest onto its south part. */
enum { GOLEM_CORE_NORTH_LAST_ENTRANCE = 8 };

/* The room past the Lucky Medal's chest: one lava platform. */
typedef enum MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_ACTOR_ID {
	ACTOR_LUCKY_MEDAL_PLATFORM = ACTOR_FIRST_PLACED
} MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_ACTOR_ID;

/* The entrance hall: the small statue whose fireball sets the great face */
/* flooding it, the row of columns standing across its lava, the pillar pushed
 */
/* into that row, an unseen marker on its floor switch and the Mars Djinni */
/* "Fire19". */
typedef enum MAGMA_ROCK_INSIDE_ENTRANCE_ACTOR_ID {
	ACTOR_STATUE = ACTOR_FIRST_PLACED,
	ACTOR_COLUMN_A,
	ACTOR_COLUMN_B,
	ACTOR_COLUMN_C,
	ACTOR_COLUMN_D,
	ACTOR_COLUMN_E,
	ACTOR_COLUMN_F,
	ACTOR_ENTRANCE_PILLAR,
	ACTOR_ENTRANCE_SWITCH_MARKER,
	ACTOR_FIRE19 /* the Mars Djinni left over from the first journey */
} MAGMA_ROCK_INSIDE_ENTRANCE_ACTOR_ID;

/* Fury's room: the statue, an unseen marker on its floor switch, two lava */
/* platforms, the pillar pushed into the lava, the tightrope's posts and Fury.
 */
typedef enum MAGMA_ROCK_INSIDE_FURY_ACTOR_ID {
	ACTOR_FURY_SWITCH_MARKER = ACTOR_FIRST_PLACED + 1,
	ACTOR_WEST_PLATFORM,
	ACTOR_FURY_PILLAR,
	ACTOR_EAST_PLATFORM,
	ACTOR_TIGHTROPE_WEST_POST,
	ACTOR_TIGHTROPE_EAST_POST,
	ACTOR_FURY
} MAGMA_ROCK_INSIDE_FURY_ACTOR_ID;

/* The room of the cracked pillars: three statues, the two cracked pillars */
/* Burst breaks, ten lava platforms, three pillars to push and a column. */
typedef enum MAGMA_ROCK_INSIDE_CRACKED_PILLARS_ACTOR_ID {
	ACTOR_CRACKED_PILLAR_A = ACTOR_FIRST_PLACED + 1,
	ACTOR_CRACKED_PILLAR_B = ACTOR_FIRST_PLACED + 3,
	ACTOR_LOWER_WEST_STATUE = ACTOR_FIRST_PLACED + 5,
	ACTOR_LOWER_EAST_STATUE,
	ACTOR_LAVA_PLATFORM_A,
	ACTOR_LAVA_PLATFORM_B,
	ACTOR_LAVA_PLATFORM_C,
	ACTOR_LAVA_PLATFORM_D,
	ACTOR_LAVA_PLATFORM_E,
	ACTOR_LAVA_PLATFORM_F,
	ACTOR_LAVA_PLATFORM_G,
	ACTOR_LAVA_PLATFORM_H,
	ACTOR_PILLAR_A,
	ACTOR_PILLAR_B,
	ACTOR_PILLAR_C,
	ACTOR_LOWER_COLUMN
} MAGMA_ROCK_INSIDE_CRACKED_PILLARS_ACTOR_ID;

/* The Golem Core's room: unseen markers on its two floor switches and the two
 */
/* chains hanging down its wall either side of its doorway, which Whirlwind */
/* swings. */
typedef enum MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_ACTOR_ID {
	ACTOR_NORTH_SWITCH_MARKER = ACTOR_FIRST_PLACED,
	ACTOR_SOUTH_SWITCH_MARKER,
	ACTOR_HANGING_CHAIN_A,
	ACTOR_HANGING_CHAIN_B
} MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_ACTOR_ID;

/* The Salamander Tail's room: three lava platforms and two pillars to push. */
typedef enum MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_ACTOR_ID {
	ACTOR_NORTH_PLATFORM = ACTOR_FIRST_PLACED,
	ACTOR_NORTH_PILLAR,
	ACTOR_SOUTHWEST_PLATFORM,
	ACTOR_SOUTHEAST_PLATFORM,
	ACTOR_EAST_PILLAR
} MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_ACTOR_ID;

/* The room of the ember stone: the sunken pillars, the pillar the two torches
 */
/* lower, the stone that holds the Magma Ball, the rocks and flames that fall */
/* with it, the torches' flames and a pillar to push. */
typedef enum MAGMA_ROCK_INSIDE_EMBER_STONE_ACTOR_ID {
	ACTOR_SUNKEN_PILLAR_A = ACTOR_FIRST_PLACED,
	ACTOR_SUNKEN_PILLAR_B,
	ACTOR_SUNKEN_PILLAR_C,
	ACTOR_BLOCKING_PILLAR,
	ACTOR_EMBER_STONE,
	ACTOR_FALLEN_ROCK_A,
	ACTOR_FALLEN_ROCK_B,
	ACTOR_FALLEN_ROCK_C,
	ACTOR_FALLEN_ROCK_D,
	ACTOR_FALLEN_FLAME_A,
	ACTOR_FALLEN_FLAME_B,
	ACTOR_FALLEN_FLAME_C,
	ACTOR_FALLEN_FLAME_D,
	ACTOR_WEST_TORCH_FLAME,
	ACTOR_EAST_TORCH_FLAME,
	ACTOR_LEDGE_PILLAR
} MAGMA_ROCK_INSIDE_EMBER_STONE_ACTOR_ID;

/* The flag that keeps a torch of the ember stone's room lit, by its flame's */
/* actor: FLAG_WEST_TORCH_LIT or FLAG_EAST_TORCH_LIT. */
#define FLAG_TORCH_LIT(flame)                                                  \
	((flame) - ACTOR_WEST_TORCH_FLAME + FLAG_WEST_TORCH_LIT)

/* A torch Blaze lights: its flame starts a sixteenth of its size and grows by
 */
/* a sixteenth FLAME_GROW_STEPS times, 5 frames apart; then the pillar the */
/* torches guard sinks PILLAR_SINK_DEPTH pixels into the floor, a pixel every */
/* 5 frames, as deep as the ember stone room's sunken pillars stand. */
enum { FLAME_GROW_STEPS = 16 };

/* The quarter turn a facing lies in, as Felix faces a switch or the lava: an */
/* eighth of a turn added, then this kept. */
#define FACING_QUARTER_MASK (TRIG_TURN - DEGtoANG(90))

/* What scMagmaRockInsideRaiseLava and scMagmaRockInsideDrainLava keep while */
/* the lava is already up; only its being nonzero is read. */
enum { LAVA_ALREADY_UP = 212 };

/* A lava platform's path script is four corners, each a move and a wait for */
/* arrival, five words: the second platform of each pair of the cracked */
/* pillars' room starts at this word, the third corner, across from the first.
 */
enum { PLATFORM_PATH_THIRD_CORNER = 10 };

/* The Blaze tablet's room: the tablet, and on the ledge in the lava's */
/* south-west the pillar in the way out, the flame burning in the west bowl */
/* beside it, which Blaze carries, and the flame of the east bowl it lights. */
typedef enum MAGMA_ROCK_INSIDE_BLAZE_TABLET_ACTOR_ID {
	ACTOR_BLAZE_TABLET = ACTOR_FIRST_PLACED,
	ACTOR_TORCH_PILLAR,
	ACTOR_SOURCE_FLAME,
	ACTOR_TORCH_FLAME
} MAGMA_ROCK_INSIDE_BLAZE_TABLET_ACTOR_ID;

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

#include "shared/position_distance.h"

#include "../field/actor_find_in_cell.c"

struct ACTOR;
struct FIELD_SPR;
struct PARTICLE;
struct PARTICLE_SPR;

void scMagmaRockInsidePausePlatform();

extern s16 MagmaRockInside_EntrancePillars[];

extern s16 MagmaRockInside_FuryPillars[];

extern s16 MagmaRockInside_CrackedPillarsPillars[];

extern s16 MagmaRockInside_EmberStonePillars[];

void scBlocksMarkCells(u16 *ids);

void scMagmaRockInsideWaitForLanding(ACTOR *actor);

s32 fdMapCellGetAttributes(u32 layer, s32 x, s32 y);

s32 syStatueFindUnderBurst(u16 *table);

s32 scPosFlatDistance(FIXED *from, FIXED *to);

void scMagmaRockInsideTrailEmbers(ACTOR *actor);
void scMagmaRockInsideRaiseLava(void);

void scMagmaRockInsideDropRocks(s32 x, s32 y, s32 z, s32 count);

void scMagmaRockInsideStartFallingRocks(void);

void scMagmaRockInsideSetEntranceFlooded(void);
void scMagmaRockInsideSetFuryFlooded(void);
void scMagmaRockInsideSetLowerRoomsFlooded(void);

extern s32 MagmaRockInside_CrackedPillarsFirstPairPath[];
extern s32 MagmaRockInside_CrackedPillarsSecondPairPath[];
extern s32 MagmaRockInside_CrackedPillarsThirdPairPath[];
extern s32 MagmaRockInside_CrackedPillarsFourthPairPath[];

void scMagmaRockInsideRaiseActorCells(u16 *list);

void scDustScatter(s32 spread);
void fdLeaderSetCellShapeAt(s32 heightOffset, s32 layer);
void scMagmaRockInsideSpawnDustRing(void);
extern u32 Dust_ScatterScript[];

extern u32 Dust_BurstScript[];

/* Decompresses a tagged LZ resource through the overlay's import. */
void scMagmaRockInsideDecompressTaggedLz(void *entry, void *destination);
/* Sets up an effect sprite of the given size, attributes and tile, through the
 */
/* overlay's import. */
void scMagmaRockInsideInitRockSpr(EFFECT_SPR *sprite, s32 width, s32 height,
								  u32 attributes, s32 tile);

void btParticleUpdateFalling(void);

extern u32 FieldEffect_ShortScript[];
extern u32 FieldEffect_MediumScript[];

void scMagmaRockInsideBurstCrackedPillar(s32 stage, s32 cue);

void syImportFieldPsynergySetupActorPair(s32 owner, u16 value);
void fdActorPlaySndInView(ACTOR *actor, s32 sound);
s32 MagmaRockInside_StepCellWidth BSS_DATA;
s32 MagmaRockInside_StepCellHeight BSS_DATA;
s32 MagmaRockInside_StepCellX BSS_DATA;
s32 MagmaRockInside_StepCellZ BSS_DATA;

extern s32 PushLeaderSteps[];

#include "shared/block_push_onto.h"

#include "../field/map_cell_fill_byte.c"

/* Talking to Fury, the Mars Djinni on the ledge just west of the north doorway
 */
/* into its room's east hall, starts a battle with it. */
void scMagmaRockInsideFightFury(void) {
	fdRequestDjinnBattle(ACTOR_FURY, DJINNI_FURY);
}

/* Talking to the Mars Djinni in the entrance hall's south (it stands at cell */
/* (18, 31)) makes it join the party. It is always the Djinni the text calls */
/* "Fire19" (DJINNI_FIRE19), wearing the Mars Djinni's sprite, and it stands */
/* there only while that Djinni has not joined (its FLAG_DJINNI); it is kept */
/* for one of The Broken Seal's Mars Djinn the party never found there, and */
/* the walkthrough says such a Djinni may wait here. */
void scMagmaRockInsideMeetDjinni(void) {
	fdDjinnJoinParty(ACTOR_FIRE19, DJINNI_FIRE19 / DJINN_PER_ELEMENT,
					 DJINNI_INDEX(DJINNI_FIRE19));
}

#include "shared/position_flat_distance.h"

#include "shared/map_cell_set_shape.h"

/* Waits up to 60 frames for an actor's vertical speed to come to zero. */
void scMagmaRockInsideWaitForLanding(ACTOR *actor) {
	s32 frames;
	s32 busy;

	frames = 60;
	while(frames != 0) {
		syWait(1);
		busy = actor->velY;
		frames--;
		if(busy == 0) {
			break;
		}
	}
}

#include "../field/map_cell_get.c"

#include "shared/map_cell_set.c"

#include "shared/blocks_mark_cells.c"

/* Clears the object's mark and the cells ahead of and behind it that the mark
 */
/* blocked. */
#include "shared/block_clear_cells.h"

/* A lava platform's update: holds it still while a menu, Psynergy, an item or
 */
/* a transition is in progress. */
void scMagmaRockInsidePausePlatform(ACTOR *actor) {
	FIELD_STATE *fieldState = Ram_HeapSlots->fieldState;
	u8 *state = &actor->paused;

	if(fieldState->psynergy != 0 || fieldState->fieldMenu != 0 ||
	   fieldState->pauseMenu != 0 || fieldState->item != 0 ||
	   fieldState->transitionActive == 0)
		*state = 1;
	else
		*state = 0;
}

/* Sets a white lava platform afloat: risen and running round its path while */
/* flag (its part's lava) is set, sunk under the floor otherwise; its update */
/* holds it still during menus. */
void scMagmaRockInsideStartLavaPlatform(s32 id, s32 *path, s16 flag) {
	ACTOR *platform;
	s32 risen;

	platform = fdActorGetById(id);
	fdActorSelectAnimation(platform, 4);
	platform->steeringFlg = 0;
	platform->speedLimit = toFIXED(1) / 2;
	platform->acceleration = toFIXED(1) / 2;
	platform->motionFlg = ACTOR_FALLS;
	platform->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorSetSprFlags(platform, 0);
	if(fdEventGetFlag((u16)flag)) {
		platform->groundHeight = toFIXED(16);
		platform->y.fixed = toFIXED(16);
		if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION))
			fdActorStartScript(platform, path);
	}
	else {
		platform->groundHeight = -toFIXED(14);
		platform->y.fixed = -toFIXED(14);
	}
	platform->update = (OBJECT_UPDATE)scMagmaRockInsidePausePlatform;
}

s32 scMagmaRockInsideSetCellShape(s32 terrain, s32 x, s32 z, s32 height);

#include "shared/actors_raise_cells.h"

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

extern ACTOR_PLACE MagmaRockInside_Placements[];
extern ACTOR_PLACE MagmaRockInside_EntrancePlacements[];
extern ACTOR_PLACE MagmaRockInside_FuryPlacements[];
extern ACTOR_PLACE MagmaRockInside_CrackedPillarsPlacements[];
extern ACTOR_PLACE MagmaRockInside_GolemCoreChestPlacements[];
extern ACTOR_PLACE MagmaRockInside_SalamanderTailChestPlacements[];
extern ACTOR_PLACE MagmaRockInside_EmberStonePlacements[];
extern ACTOR_PLACE MagmaRockInside_BlazeTabletPlacements[];

extern s16 MagmaRockInside_SalamanderTailChestPillars[];

/* The placed actors of the room the party is in. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene != SCENE_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST) {
		if(scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
			return MagmaRockInside_EntrancePlacements;
		if(scene == SCENE_MAGMA_ROCK_INSIDE_FURY)
			return MagmaRockInside_FuryPlacements;
		if(scene == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
			return MagmaRockInside_CrackedPillarsPlacements;
		if(scene == SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST)
			return MagmaRockInside_GolemCoreChestPlacements;
		if(scene == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST)
			return MagmaRockInside_SalamanderTailChestPlacements;
		if(scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE)
			return MagmaRockInside_EmberStonePlacements;
		if(scene == SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET)
			return MagmaRockInside_BlazeTabletPlacements;
	}
	return MagmaRockInside_Placements;
}

/* Nothing visible happens as Felix starts to push one of the yellow-topped */
/* stone pillars, whether by walking into it or with Move; the game frees the */
/* cells it walled off, so that it can slide. */
void scMagmaRockInsideStartPushingPillar(void) {
	FIELD_STATE *state;
	s16 scene;

	state = Ram_HeapSlots->fieldState;
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
		fdActorLiftTargetBlock(MagmaRockInside_EntrancePillars);
	else if(scene == SCENE_MAGMA_ROCK_INSIDE_FURY)
		fdActorLiftTargetBlock(MagmaRockInside_FuryPillars);
	else if(scene == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
		fdActorLiftTargetBlock(MagmaRockInside_CrackedPillarsPillars);
	else if(scene == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST)
		fdActorLiftTargetBlock(MagmaRockInside_SalamanderTailChestPillars);
	else if(scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE)
		fdActorLiftTargetBlock(MagmaRockInside_EmberStonePillars);
	scMagmaRockInsideClearBlockCells(state->movedBlock);
}

/* A yellow-topped stone pillar Felix has pushed comes to rest a cell further */
/* on. Pushed off the ledge in the Salamander Tail's room or the ember stone's,
 */
/* it drops with a thud, into the lava when it is up, sinking until only its */
/* top shows. Once a pillar reaches its place its flag keeps it there when the
 */
/* party comes back: the entrance hall's pushed north into the row of columns */
/* (row 16 of the map), Fury's room's at row 41, the cracked pillars' room's */
/* three at column 21, the Salamander Tail room's at columns 47 and 57 and the
 */
/* ember stone room's at column 43. */
void scMagmaRockInsideFinishPushingPillar(void) {
	ACTOR *actor = FieldStateWork->movedBlock;
	s32 scene;
	s32 x;
	s32 z;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorPlaceMovedBlock();
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST) {
		scBlocksMarkCells(MagmaRockInside_SalamanderTailChestPillars);
		if(fdMapGetTerrainHeight(2, actor->x.fixed, actor->z.fixed) !=
		   actor->y.fixed) {
			actor->layer = 2;
			actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			syWait(1);
			if(fdEventGetFlag(FLAG_LOWER_ROOMS_FLOODED)) {
				ACTOR *copy;

				actor->gravity = toFIXED(1) / 16;
				actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
				REG_BLDALPHA = BLEND_ALPHA(0, 16);
				REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
				copy = fdActorCreateSilhouette(
					actor->x.fixed, actor->groundHeight,
					actor->z.fixed + toFIXED(16), SPRITE_YELLOW_BLOCK);
				scMagmaRockInsideWaitForLanding(actor);
				fdEventPlaySnd(SE_HEAVY_STOP);
				fdActorRelease(copy);
				fdActorSelectAnimation(actor, 4);
				syWait(1);
				REG_BLDCNT = 0;
			}
			else {
				scMagmaRockInsideWaitForLanding(actor);
				fdEventPlaySnd(SE_THUMP);
			}
			actor->motionFlg = 0;
			actor->layer = 0;
		}
	}
	else if(scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE) {
		scBlocksMarkCells(MagmaRockInside_EmberStonePillars);
		if(fdMapGetTerrainHeight(2, actor->x.fixed, actor->z.fixed) !=
		   actor->y.fixed) {
			ACTOR *copy;

			actor->layer = 2;
			actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			syWait(1);
			actor->gravity = toFIXED(1) / 16;
			actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
			REG_BLDALPHA = BLEND_ALPHA(0, 16);
			REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
			copy = fdActorCreateSilhouette(actor->x.fixed, actor->groundHeight,
										   actor->z.fixed + toFIXED(16),
										   SPRITE_YELLOW_BLOCK);
			scMagmaRockInsideWaitForLanding(actor);
			fdEventPlaySnd(SE_HEAVY_STOP);
			fdActorRelease(copy);
			fdActorSelectAnimation(actor, 4);
			syWait(1);
			REG_BLDCNT = 0;
			scMagmaRockInsideWaitForLanding(actor);
			actor->motionFlg = 0;
			actor->layer = 0;
		}
	}
	x = FIELD_CELL(actor->x.fixed);
	z = FIELD_CELL(actor->z.fixed);
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE) {
		if(z == 16)
			fdEventSetFlag(FLAG_ENTRANCE_PILLAR_PUSHED);
	}
	else if(scene == SCENE_MAGMA_ROCK_INSIDE_FURY) {
		if(z == 41)
			fdEventSetFlag(FLAG_FURY_PILLAR_PUSHED);
	}
	else if(scene == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS) {
		if(x == 21) {
			if(z == 43)
				fdEventSetFlag(FLAG_PILLAR_A_PUSHED);
			if(z == 45)
				fdEventSetFlag(FLAG_PILLAR_B_PUSHED);
			if(z == 47)
				fdEventSetFlag(FLAG_PILLAR_C_PUSHED);
		}
	}
	else if(scene == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST) {
		if(x == 47) {
			fdEventSetFlag(FLAG_NORTH_PILLAR_PUSHED);
			fdMapGridCopyCellAttributes(46, 74, 1, 1, 46, 73);
		}
		if(x == 57)
			fdEventSetFlag(FLAG_EAST_PILLAR_PUSHED);
	}
	else if(scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE) {
		if(x == 43)
			fdEventSetFlag(FLAG_LEDGE_PILLAR_PUSHED);
	}
	fdEventEnd();
}

/* The overlay's import stub to fdBattleEffectStopMainActor. */
s32 syImportBattleEffectStopMainActor(void);

/* Plays Burst from Felix at an actor, as when he casts it, for a statue's */
/* fireball that strikes one: in the cracked pillars' room, the pillar it hits
 */
/* breaks as if Burst had been cast at it. */
void scMagmaRockInsidePlayBurst(u16 value) {
	MAIN_ROUTINE_STATE *party;

	fdPsynergyLoadResources(ABILITY_BURST, 1);
	party = &MainRoutineStateWork;
	syImportFieldPsynergySetupActorPair(party->leaderActorId, value);
	fdPsynergyRunHandler(1);
	syImportBattleEffectStopMainActor();
	fdPsynergyEnd();
}

extern u32 MagmaRock_EmberScript[];

void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options);

/* A statue's fireball's update as it flies: it trails an ember every fourth */
/* frame, a few pixels to either side, and whooshes every sixteenth. */
void scMagmaRockInsideTrailEmbers(ACTOR *actor) {
	EFFECT_OPTIONS options;
	u32 frame = FrameCount;
	s32 randomZ;
	s32 vz;

	if((frame & 3) != 0)
		return;
	options.type = SPRITE_FLAME_PUFF;
	options.pri = 2;
	options.script = MagmaRock_EmberScript;
	if((frame & 15) == 0)
		fdActorPlaySndInView(actor, SE_WHOOSH);
	randomZ = syRandom() & 15;
	vz = ((syRandom() & 31) - 16) << 12;
	scFieldEffectSpawn(
		actor->x.fixed, actor->y.fixed, actor->z.fixed + ((randomZ - 8) << 15),
		0, 0, vz, EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | 1,
		&options);
}

#include "shared/statue_flash_under_burst.h"

#include "shared/statue_find_under_burst.h"

/* Nothing visible happens as Felix starts casting Burst at a fire-spitting */
/* statue; the game finishes what the field engine still had pending, so that */
/* the cast and the statue's fire play undisturbed. */
void scMagmaRockInsideResolveBeforeBurst(void) { fdEventEndLastingPsynergy(0); }

extern s16 MagmaRockInside_Statues[];
extern s16 MagmaRockInside_CrackedPillarsStatues[];

void syStatueFlashUnderBurst(ACTOR *actor);

/* As Felix casts Burst at the small statue beside a great stone face, the */
/* statue blinks red, slowly through the small explosions and faster at the */
/* last blast. In the entrance hall the statue's cell is (26, 12); in Fury's */
/* room (20, 15); the cracked pillars' room has three, at (12, 12), (20, 33) */
/* and (30, 33), and its own table. */
void scMagmaRockInsideTargetStatue(s32 stage) {
	u16 *table;
	FIELD_EFFECT_STATE *watch = Ram_HeapSlots->fieldEffState;
	s32 scene;

	table = MagmaRockInside_Statues;
	syFieldEventEndLastingPsynergy(0);
	scene = MainRoutineStateWork.next;
	/* DEAD: the entrance hall's and Fury's room's arms are empty; the ROM keeps
	 */
	/* both tests. */
	if(scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE) {
	}
	else if(scene == SCENE_MAGMA_ROCK_INSIDE_FURY) {
	}
	else if(scene == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS) {
		table = MagmaRockInside_CrackedPillarsStatues;
	}
	if(stage == BURST_STAGE_SMALL_BLAST) {
		ACTOR *actor;

		syStatueFindUnderBurst(table);
		actor = watch->tgt;
		actor->flashMask = 4;
		actor->update = (OBJECT_UPDATE)syStatueFlashUnderBurst;
	}
	if(stage == BURST_STAGE_GREAT_BLAST) {
		ACTOR *actor;

		syStatueFindUnderBurst(table);
		actor = watch->tgt;
		actor->flashMask = 2;
		actor->update = (OBJECT_UPDATE)syStatueFlashUnderBurst;
	}
}

/* An ember's update as a statue's fireball bursts: moves it by its velocity, */
/* kept in bounce, gravity and sceneWord, and slows it by an eighth. */
void scMagmaRockInsideDriftAndSlow(ACTOR *actor) {
	s32 dx, dy, dz;

	actor->x.fixed += dx = actor->bounce;
	actor->y.fixed += dy = actor->gravity;
	actor->z.fixed += dz = actor->sceneWord;
	dx -= dx >> 3;
	dy -= dy >> 3;
	dz -= dz >> 3;
	actor->bounce = dx;
	actor->gravity = dy;
	actor->sceneWord = dz;
}

/* The frames a statue's fireball bursts where it lands, throwing out an ember
 */
/* every other frame. */
enum { FIRE_BURST_FRAMES = 16 };

/* A statue's fireball flies the way the statue faces, the camera following, */
/* until it meets the edge of the fire's reach, a great stone face or */
/* something standing in its path. It bursts against the edge or a stone */
/* pillar; against a face it sets the face pouring lava */
/* (scMagmaRockInsideRaiseLava); against anything else it plays Burst there. */
void scMagmaRockInsideFireFromStatue(ACTOR *source) {
	ACTOR *flame = Create(SPRITE_FLAME_PUFF, source->x.fixed, source->y.fixed,
						  source->z.fixed);
	ACTOR *other;
	FIXED position[3];
	EFFECT_OPTIONS options;
	s32 step = syCos(source->facing) << 1;
	s32 cell;
	s32 id;
	s32 bit;

	flame->motionFlg = 0;
	fdActorSelectAnimation(flame, 1);
	fdActorSetSprFlags(flame, 0);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	position[0] = flame->x.fixed;
	position[1] = flame->y.fixed;
	position[2] = flame->z.fixed;
	for(;;) {
		position[0] += step;
		cell = fdMapCellGetAttributes(MAPGRID_LAYER_OVERHEAD,
									  FIELD_CELL(position[0]),
									  FIELD_CELL(position[2] - position[1])) >>
			   8;
		if(cell == CELL_BEHAVIOR_MAGMA_ROCK_FIRE_EDGE ||
		   cell == CELL_BEHAVIOR_MAGMA_ROCK_LAVA_FALL)
			goto end;
		for(id = ACTOR_FIRST_PLACED; id != FIELD_TREASURE_ACTOR; id++) {
			other = FieldStateWork->actors[id];
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
				goto end;
			goto hit;
		}
	}
end:
	flame->acceleration = toFIXED(2);
	flame->speedLimit = toFIXED(2);
	flame->update = (OBJECT_UPDATE)scMagmaRockInsideTrailEmbers;
	scFieldActorSetMoveTargetFX(flame, position[0], position[1], position[2]);
	fdCameraSetSpeed(2);
	fdCameraMoveToFX(position[0], position[1], position[2], 1);
	scFieldActorWaitForMove(flame);
	flame->update = NULL;
	fdActorRelease(flame);
	for(id = 0; id != FIRE_BURST_FRAMES; id++) {
		bit = FrameCount;
		bit &= 1;
		if(bit == 0) {
			options.type = SPRITE_FLAME_PUFF;
			options.pri = 1;
			options.update =
				(void (*)(FIELD_OBJECT *))scMagmaRockInsideDriftAndSlow;
			options.script = MagmaRock_EmberScript;
			scFieldEffectSpawn(position[0], position[1], position[2],
							   ((syRandom() & 31) - 16) << 13,
							   ((syRandom() & 31) - 16) << 13, bit,
							   EFFECT_USE_UPDATE | EFFECT_USE_SCRIPT |
								   EFFECT_USE_TYPE | EFFECT_USE_PRIORITY,
							   &options);
		}
		syWait(1);
	}
	syWait(24);
	if(cell == CELL_BEHAVIOR_MAGMA_ROCK_LAVA_FALL) {
		scMagmaRockInsideRaiseLava();
		fdEventEnd();
		return;
	}
	syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	syWait(10);
	fdEventEnd();
	return;
hit:
	flame->acceleration = toFIXED(2);
	flame->speedLimit = toFIXED(2);
	flame->update = (OBJECT_UPDATE)scMagmaRockInsideTrailEmbers;
	scFieldActorSetMoveTargetFX(flame, position[0], position[1], position[2]);
	fdCameraSetSpeed(2);
	fdCameraMoveToFX(position[0], position[1], position[2], 1);
	scFieldActorWaitForMove(flame);
	flame->update = NULL;
	fdActorRelease(flame);
	fdEventSetFlag(FLAG_FIRE_BURST_PLAYING);
	scMagmaRockInsidePlayBurst(id);
	fdEventClearFlag(FLAG_FIRE_BURST_PLAYING);
	syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	syWait(10);
	fdEventEnd();
}

void syRockEndFalling(void);

/* Once Felix's Burst on the small statue beside a great stone face is over, */
/* the statue flashes white and spits a fireball the way it faces. In the */
/* three halls with a face the fireball strikes it, and the face's eyes light */
/* red as it pours lava over the floor (scMagmaRockInsideFireFromStatue). */
void scMagmaRockInsideFireStatue(void) {
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
	scMagmaRockInsideFireFromStatue(state->tgt);
	fdEventEnd();
}

/* A cracked pillar's animation as Burst breaks it: its two cracked pictures, */
/* then its stump, 41 frames each. It starts at frame */
/* CRACKED_PILLAR_CRUMBLE_FRAME of the cast, among the small explosions, and */
/* the pillar has crumbled to its stump by the last blast, which takes it away.
 */
enum {
	CRACKED_PILLAR_ANIMATION_CRUMBLE = 3,
	CRACKED_PILLAR_CRUMBLE_FRAME = 178
};

/* Burst on one of the tall cracked pillars: rocks fall round it while Burst */
/* lasts, one at each small explosion and eight at the last blast, which also */
/* takes the pillar away, leaving a cloud of dust; meanwhile the pillar */
/* crumbles. */
void scMagmaRockInsideBurstCrackedPillar(s32 stage, s32 cue) {
	FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;

	if(cue == 0)
		scMagmaRockInsideStartFallingRocks();
	if(stage == BURST_STAGE_SMALL_BLAST) {
		ACTOR *actor = state->tgt;

		scMagmaRockInsideDropRocks(actor->x.fixed, actor->y.fixed + toFIXED(15),
								   actor->z.fixed, 1);
	}
	if(stage == BURST_STAGE_GREAT_BLAST) {
		ACTOR *actor = state->tgt;
		ACTOR *target;

		scMagmaRockInsideDropRocks(actor->x.fixed, actor->y.fixed + toFIXED(15),
								   actor->z.fixed, 8);
		target = state->tgt;
		SET_ACTOR_POSITION(target, 0, 0, 0);
	}
	if(cue == PSYNERGY_EVENT_LAST_FRAME)
		syRockEndFalling();
	if(cue == CRACKED_PILLAR_CRUMBLE_FRAME)
		fdActorSelectAnimation(state->tgt, CRACKED_PILLAR_ANIMATION_CRUMBLE);
}

/* Felix walks to the lava's edge where something stands in the lava two cells
 */
/* ahead, a white platform, a column or a pillar pushed in, and springs across
 */
/* onto it, keeping pace with a platform as it moves. Such edges are on both */
/* sides of the Lucky Medal room's lava, at cells (44, 39) and (50, 39), and */
/* along the lava in Fury's room, the cracked pillars' room and, once its lava
 */
/* is up, the Salamander Tail's room. Where nothing stands there, nothing */
/* happens. */
void scMagmaRockInsideLeapOverLava(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR *target;
	FIXED position[3];
	FIXED stepX;
	FIXED stepZ;
	s32 i;
	s32 angle = (leader->facing + DEGtoANG(45)) & FACING_QUARTER_MASK;

	position[0] = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
	position[1] = leader->y.fixed - toFIXED(16);
	position[2] = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
	syFieldVectorAddPolarOffset(32, angle, position);
	target = fdActorFindInCell(position, leader);
	if(target == NULL)
		return;
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
	syWait(6);
	fdEventPlaySnd(SE_JUMP);
	fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
	leader->speedLimit = toFIXED(3);
	leader->acceleration = toFIXED(2);
	leader->velY = toFIXED(24) / 5;
	leader->motionFlg &= ~(ACTOR_FOLLOWS_TERRAIN | ACTOR_MOTION_SPARE);
	fdActorSetSprFlags(leader, 0);
	stepX = (position[0] - leader->x.fixed) / 12;
	stepZ = (position[2] - leader->z.fixed) / 12;
	for(i = 0; i != 12; i++) {
		leader->x.fixed += target->x.fixed - position[0] + stepX;
		leader->z.fixed += target->z.fixed - position[2] + stepZ;
		position[0] = target->x.fixed;
		position[2] = target->z.fixed;
		syWait(1);
	}
	leader->x.fixed = target->x.fixed;
	leader->y.fixed = leader->groundHeight = target->y.fixed + toFIXED(16);
	leader->z.fixed = target->z.fixed;
	fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
	fdActorSetSprFlags(leader, 1);
	leader->motionFlg |= ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdEventEnd();
}

extern s32 MagmaRock_FlameEmberScript[];

/* The bursts of flame, 8 frames apart, as a statue's fireball strikes a great
 */
/* stone face. */
enum { LAVA_FALL_FLAME_BURSTS = 16 };

/* A statue's fireball strikes the great stone face: the screen shakes as */
/* flames burst over the face, it roars and its eyes light red, lava pours */
/* from its mouth and spreads over the hall's floor until the hall is flooded.
 */
/* When the lava is already up, the face only flares and pours again. */
void scMagmaRockInsideRaiseLava(void) {
	s32 srcX = 0;
	s32 srcZ = 0;
	FIXED baseX = 0;
	MAP_LAYER *layer =
		&((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers[MAPGRID_LAYER_MIDDLE];
	EFFECT_OPTIONS options;
	s32 position[3];
	/* LAVA_ALREADY_UP when the room is already flooded; then the frames the */
	/* lava takes to rise. */
	u32 frame = 0;
	u32 i;
	u32 row;

	options.type = SPRITE_FLAME_PUFF;
	options.pri = 1;
	options.script = MagmaRock_FlameEmberScript;
	options.startScaleX = FIXED_ONE * 9 / 5;
	options.startScaleY = FIXED_ONE * 9 / 5;
	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE) {
		baseX = toFIXED(328);
		position[1] = toFIXED(32);
		position[2] = toFIXED(200);
		srcX = 20;
		srcZ = 69;
		if(fdEventGetFlag(FLAG_ENTRANCE_FLOODED))
			frame = LAVA_ALREADY_UP;
	}
	else if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY) {
		baseX = toFIXED(232);
		position[1] = toFIXED(32);
		position[2] = toFIXED(248);
		srcX = 14;
		srcZ = 72;
		if(fdEventGetFlag(FLAG_FURY_FLOODED))
			frame = LAVA_ALREADY_UP;
	}
	else if(MainRoutineStateWork.next ==
			SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS) {
		baseX = toFIXED(392);
		position[1] = toFIXED(32);
		position[2] = toFIXED(536);
		srcX = 24;
		srcZ = 90;
		if(fdEventGetFlag(FLAG_LOWER_ROOMS_FLOODED))
			frame = LAVA_ALREADY_UP;
	}
	for(i = 0; i != LAVA_FALL_FLAME_BURSTS; i++) {
		position[0] = baseX + ((syRandom() & 31) << 16);
		fdEventPlaySnd(SE_CRASH);
		scFieldEffectSpawn(position[0], position[1], position[2], 0,
						   (syRandom() & 31) << 12, 0,
						   EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
							   EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | 1,
						   &options);
		fdMapGridSetShake(5, 5, 1);
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		fdEventWait(8);
	}
	fdEventPlaySnd(SE_BOSS_ROAR);
	options.pri = 1;
	options.startScaleX = FIXED_ONE * 2;
	options.startScaleY = FIXED_ONE * 2;
	options.tgtScaleX = FIXED_ONE / 2;
	options.tgtScaleY = FIXED_ONE / 2;
	baseX -= toFIXED(8);
	for(i = 0; i != 8; i++) {
		position[0] = baseX + i * toFIXED(6);
		scFieldEffectSpawn(position[0], position[1], position[2], 0, 0,
						   (syRandom() & 31) << 10,
						   EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET |
							   EFFECT_USE_PRIORITY | 1,
						   &options);
	}
	fdMapGridCopyCellsTo(68, 70, srcX, srcZ, 3, 6);
	fdMapGridSetShake(5, 7, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdEventWait(30);
	fdMapGridCopyCellsTo(71, 70, srcX, srcZ, 3, 6);
	options.type = SPRITE_FLAME_PUFF;
	options.pri = 1;
	options.pal = SPRITE_COLOR_RED_TINT;
	options.script = MagmaRock_EmberScript;
	if(frame != 0) {
		fdMapGridCopyCellsTo(68, 70, srcX, srcZ, 3, 6);
		fdEventWait(30);
		position[2] -= toFIXED(40);
	}
	else {
		fdMapGridSetShake(2, 1, 1);
		fdEventPlaySnd(SE_TREMOR);
		for(row = 0; row != 12; row++) {
			for(i = 0; i != 6; i++) {
				position[0] = baseX + ((syRandom() % 48) << 16);
				scFieldEffectSpawn(position[0], position[1],
								   position[2] + row * toFIXED(4) + toFIXED(8),
								   0, toFIXED(1), 0,
								   EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
									   EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
								   &options);
			}
			sySynch(3);
			fdMapGridCopyCellsTo(74, 70, srcX, srcZ, 3, (row >> 2) + 6);
		}
		fdMapGridCopyCellsTo(74, 70, srcX, srcZ, 3, 8);
		frame = 0;
		do {
			if((frame & 3) == 0) {
				for(i = 0; i != 4; i++) {
					position[0] = baseX + ((syRandom() % 48) << 16);
					scFieldEffectSpawn(
						position[0], position[1], position[2] + toFIXED(48), 0,
						toFIXED(1), 0,
						EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
							EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
						&options);
				}
			}
			if(frame == 0) {
				layer->ofsY -= toFIXED(32);
				fdMapGridCenterOnFollowedPoint();
				fdEventWait(1);
			}
			else if(frame <= 63) {
				REG_BLDALPHA = ((16 - (frame >> 2)) << 8) | (frame >> 2);
				REG_BLDCNT =
					BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
			}
			else if(frame == 64) {
				REG_BLDCNT = 0;
			}
			else if(frame <= 127) {
				layer->ofsY += toFIXED(1) / 2;
				position[2] -= toFIXED(1) / 2;
				fdMapGridCopyCellsTo(71, 76, srcX,
									 srcZ - ((frame - 47) >> 5) + 8, 3, 1);
			}
			else if(frame == 128) {
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			}
			frame++;
			sySynch(1);
		} while(frame != 160);
		syFieldMapGridWaitForShakeToSettle();
	}
	fdEventPlaySnd(SE_STOP_ACTION_SE);
	fdEventPlaySnd(SE_WHOOSH);
	options.startScaleY = FIXED_ONE * 9 / 5;
	options.startScaleX = FIXED_ONE * 9 / 5;
	for(i = 0; i != 6; i++) {
		position[0] = baseX + ((syRandom() % 48) << 16);
		scFieldEffectSpawn(
			position[0], position[1], position[2] + toFIXED(48), 0, 0, 0,
			EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
				EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
			&options);
	}
	fdMapGridSetShake(5, 7, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridCopyCellsTo(68, 64, srcX, srcZ, 3, 6);
	syFieldMapGridWaitForShakeToSettle();
	fdEventWait(30);
	layer->ofsY = 0;
	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
		scMagmaRockInsideSetEntranceFlooded();
	else if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY)
		scMagmaRockInsideSetFuryFlooded();
	else if(MainRoutineStateWork.next ==
			SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
		scMagmaRockInsideSetLowerRoomsFlooded();
}

/* Felix steps onto the small floor switch beside a grated gate and presses it
 */
/* down: the ground shakes and the gate lifts. If the lava is up it pours out */
/* through the gate, embers rising as it fades from the floor; then the gate */
/* drops shut and he steps back off the switch. The switches are at cell */
/* (19, 27) in the entrance hall, (32, 21) in Fury's room, and (47, 13) and */
/* (49, 42) in the Golem Core's room, which drain the lower rooms. */
void scMagmaRockInsideDrainLava(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	u32 facing = (leader->facing + DEGtoANG(45)) & FACING_QUARTER_MASK;
	s32 position[3];
	EFFECT_OPTIONS options;
	FIXED baseX = 0;
	s32 dstX = 0;
	s32 dstZ = 0;
	s32 open = 0;
	s32 srcX = 0;
	u8 *flags;
	/* The gate's row on the map, then each row of embers over the drained fall.
	 */
	u32 row = 0;
	u32 j;
	s32 rowAbove;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	position[0] = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
	position[2] = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId,
						 FIXEDtoINT(position[0]), FIXEDtoINT(position[2]));
	syFieldEventWait(20);
	fdEventPlaySnd(SE_BUTTON_PRESS);
	flags = &leader->priFlg;
	*flags &= ~ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	syFieldEventWait(20);
	fdMapGridSetShake(1, 2, 1);
	fdEventPlaySnd(SE_TREMOR);
	syFieldEventWait(30);
	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE) {
		srcX = 20;
		row = 89;
		dstX = 84;
		dstZ = 26;
		baseX = toFIXED(320);
		position[1] = 0;
		position[2] = toFIXED(424);
		if(fdEventGetFlag(FLAG_ENTRANCE_FLOODED))
			open = LAVA_ALREADY_UP;
		fdEventClearFlag(FLAG_ENTRANCE_FLOODED);
		facing = FACING_EAST;
		scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 15, 13,
							   MAP_CELL_CLIMBABLE);
		scFieldActorSetSprPriority(ACTOR_ENTRANCE_PILLAR, 3);
		for(j = 0; j != ACTOR_ENTRANCE_PILLAR - ACTOR_COLUMN_A + 1; j++)
			fdActorSetAnimation(j + ACTOR_COLUMN_A, 1);
	}
	else if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY) {
		srcX = 28;
		row = 83;
		dstX = 92;
		dstZ = 21;
		baseX = toFIXED(448);
		position[1] = 0;
		position[2] = toFIXED(344);
		if(fdEventGetFlag(FLAG_FURY_FLOODED))
			open = LAVA_ALREADY_UP;
		fdEventClearFlag(FLAG_FURY_FLOODED);
		scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 24, 15,
							   MAP_CELL_CLIMBABLE);
		scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 9, 16, MAP_CELL_CLIMBABLE);
		fdActorSetAnimation(ACTOR_FURY_PILLAR, 4);
	}
	else if(MainRoutineStateWork.next ==
			SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST) {
		if(MainRoutineStateWork.entrance <= GOLEM_CORE_NORTH_LAST_ENTRANCE) {
			srcX = 49;
			row = 75;
			dstX = 113;
			dstZ = 13;
			baseX = toFIXED(792);
			position[1] = 0;
			position[2] = toFIXED(216);
			if(fdEventGetFlag(FLAG_LOWER_ROOMS_FLOODED))
				open = LAVA_ALREADY_UP;
			fdEventClearFlag(FLAG_LOWER_ROOMS_FLOODED);
		}
		else {
			srcX = 51;
			row = 104;
			dstX = 115;
			dstZ = 42;
			baseX = toFIXED(824);
			position[1] = 0;
			position[2] = toFIXED(680);
			if(fdEventGetFlag(FLAG_LOWER_ROOMS_FLOODED))
				open = LAVA_ALREADY_UP;
			fdEventClearFlag(FLAG_LOWER_ROOMS_FLOODED);
		}
	}
	fdEventPlaySnd(SE_BOOM);
	fdMapGridSetShake(5, 7, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	rowAbove = row - 1;
	fdMapGridCopyCellsTo(srcX, row, srcX, rowAbove, 3, 4);
	if(open == 0) {
		syFieldMapGridWaitForShakeToSettle();
		syFieldEventWait(20);
	}
	else {
		fdMapGridSetShake(2, 1, 1);
		options.type = SPRITE_FLAME_PUFF;
		options.pri = 2;
		options.pal = SPRITE_COLOR_RED_TINT;
		options.script = MagmaRock_EmberScript;
		for(row = 0; row != 10; row++) {
			for(j = 0; j != 6; j++) {
				u32 random = syRandom();

				position[0] = baseX + ((random % 48) << 16);
				scFieldEffectSpawn(position[0], position[1],
								   position[2] + row * toFIXED(4) + toFIXED(23),
								   0, toFIXED(1), 0,
								   EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
									   EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
								   &options);
			}
			syWait(3);
			fdMapGridCopyCellsTo(75, 64, dstX, dstZ, 3, (row >> 1) + 1);
		}
		for(j = 0; j != BLEND_WEIGHT_MAX * 4; j++) {
			REG_BLDALPHA = ((j >> 2) << 8) | (BLEND_WEIGHT_MAX - (j >> 2));
			REG_BLDCNT =
				BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
			syWait(3);
		}
		fdMapGridCopyCellsTo(78, 64, dstX, dstZ, 3, 5);
	}
	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
		fdActorGet(ACTOR_ENTRANCE_PILLAR)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdEventPlaySnd(SE_STOP_ACTION_SE);
	fdEventPlaySnd(SE_BOOM);
	fdMapGridSetShake(5, 7, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridCopyCellsTo(72, 63, srcX, rowAbove, 3, 4);
	syFieldMapGridWaitForShakeToSettle();
	fdEventPlaySnd(SE_BUTTON_PRESS);
	syActorFlagsSet(flags, ACTOR_PRIORITY_WALKS_ON_PLATFORMS);
	syFieldEventWait(20);
	fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	position[0] = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
	position[1] = leader->y.fixed;
	position[2] = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
	syFieldVectorAddPolarOffset(16, (s32)facing ^ DEGtoANG(180), position);
	leader->facing = facing;
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId,
						 FIXEDtoINT(position[0]), FIXEDtoINT(position[2]));
	fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg |=
		ACTOR_FACES_MOTION;
	fdEventEnd();
}

/* Defines what Burst does to a cracked pillar: it crumbles */
/* (scMagmaRockInsideBurstCrackedPillar) and its flag is set. */
#define DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR(name, pillar, flag)              \
	void name(s32 stage, s32 cue) {                                            \
		FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;              \
                                                                               \
		state->tgt = fdActorGet(pillar);                                       \
		scMagmaRockInsideBurstCrackedPillar(stage, cue);                       \
		fdEventSetFlag(flag);                                                  \
	}

/* Burst cast at the tall cracked pillar east of the cracked pillars' room's */
/* north statue (cell (17, 12)) breaks it: rocks fall round it as it crumbles */
/* and the last blast leaves only dust. The statue's own fireball breaks it */
/* the same way. `FLAG_INSIDE_CRACKED_PILLAR_A_BURST` keeps it gone. */
DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR(MagmaRockInside_BurstCrackedPillarA,
									  ACTOR_CRACKED_PILLAR_A,
									  FLAG_INSIDE_CRACKED_PILLAR_A_BURST)

/* Burst cast at the cracked pillars' room's other tall cracked pillar (cells */
/* (19, 14) and (24, 16)) breaks it: rocks fall round it as it crumbles and the
 */
/* last blast leaves only dust. `FLAG_INSIDE_CRACKED_PILLAR_B_BURST` keeps it */
/* gone. */
DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR(MagmaRockInside_BurstCrackedPillarB,
									  ACTOR_CRACKED_PILLAR_B,
									  FLAG_INSIDE_CRACKED_PILLAR_B_BURST)

/* The update of a rock or flame the ember stone throws: it trails smoking */
/* embers every other frame for 64 frames. */
void scMagmaRockInsideSmokeEmber(ACTOR *actor) {
	if(++actor->tmr > 64)
		actor->update = NULL;
	if((actor->tmr & 1) == 0) {
		EFFECT_OPTIONS config;

		config.type = SPRITE_FLAME_PUFF;
		config.pri = 2;
		config.pal = SPRITE_COLOR_RED_TINT;
		config.script = MagmaRock_EmberScript;
		scFieldEffectSpawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0,
						   toFIXED(1) / 16, 0,
						   EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
							   EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
						   &config);
	}
}

extern s32 MagmaRock_FlameEmberScript[];

/* Throws a rock or flame up out of the lava where the ember stone lies, to */
/* land at (x, z) in pixels, smoking as it flies (scMagmaRockInsideSmokeEmber).
 */
static __inline__ void Arrive(s32 id, s32 x, s32 z) {
	fdActorSetPos(id, 776, 248);
	fdActorSetDestination(id, x, z);
	fdActorGetById(id)->velY = toFIXED(12);
	fdActorGetById(id)->gravity = toFIXED(1) / 2;
	fdActorGetById(id)->tmr = 0;
	fdActorGetById(id)->update = scMagmaRockInsideSmokeEmber;
}

/* The first time Felix hops onto the islet in the lava at cell (45, 19), in */
/* the ember stone's room east of the torches' ledge, the room shakes and the */
/* camera turns to the lava: embers spout and pound, four rocks and four */
/* flames are thrown out to land on the islets round the spot, and last */
/* the ember stone itself, a glowing red rock, lands on an islet to the west. */
/* `FLAG_ROCKS_RELEASED` keeps them there. */
void scMagmaRockInsideReleaseRocks(void) {
	FIXED position[3];
	EFFECT_OPTIONS options;
	s32 beat;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventPlaySnd(SE_TREMOR);
	fdMapGridSetShake(3, 3, 1);
	fdEventWait(30);
	fdCameraMoveTo(776, CAMERA_KEEP, 272, 1);
	fdCameraWaitForArrival();
	options.type = SPRITE_FLAME_PUFF;
	options.pri = 2;
	options.pal = SPRITE_COLOR_RED_TINT;
	options.script = MagmaRock_FlameEmberScript;
	options.startScaleY = FIXED_ONE * 3 / 2;
	options.startScaleX = FIXED_ONE * 3 / 2;
	position[1] = 0;
	/* A beat every 3 frames: embers burst from the stone, a pound every eighth
	 */
	/* beat; the rocks and flames are thrown eight beats apart from beat 32, and
	 */
	/* the stone itself at beat 112. */
	for(beat = 0; beat != 128; beat++) {
		position[0] = toFIXED(syRandom() & 31) + (toFIXED(760));
		position[2] = toFIXED(syRandom() & 31) + (toFIXED(256));
		scFieldEffectSpawn(
			position[0], position[1], position[2], 0, toFIXED(1), 0,
			EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
				EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
			&options);
		if((beat & 7) == 0)
			fdEventPlaySnd(SE_POUND);
		if(beat == 32) {
			Arrive(ACTOR_FALLEN_ROCK_A, 744, 216);
		}
		else if(beat == 40) {
			Arrive(ACTOR_FALLEN_FLAME_D, 776, 328);
		}
		else if(beat == 48) {
			Arrive(ACTOR_FALLEN_ROCK_B, 856, 232);
		}
		else if(beat == 56) {
			Arrive(ACTOR_FALLEN_FLAME_C, 696, 312);
		}
		else if(beat == 64) {
			Arrive(ACTOR_FALLEN_ROCK_C, 728, 280);
		}
		else if(beat == 72) {
			Arrive(ACTOR_FALLEN_FLAME_B, 824, 280);
		}
		else if(beat == 80) {
			Arrive(ACTOR_FALLEN_ROCK_D, 824, 312);
		}
		else if(beat == 88) {
			Arrive(ACTOR_FALLEN_FLAME_A, 776, 232);
		}
		else if(beat == 112) {
			fdActorSetPos(ACTOR_EMBER_STONE, 776, 248);
			fdActorSetDestination(ACTOR_EMBER_STONE, 696, 232);
			fdCameraMoveTo(696, CAMERA_KEEP, 232, 1);
			fdActorGetById(ACTOR_EMBER_STONE)->velY = toFIXED(13);
			fdActorGetById(ACTOR_EMBER_STONE)->gravity = toFIXED(1) / 2;
			fdActorGetById(ACTOR_EMBER_STONE)->tmr = 0;
			fdActorGetById(ACTOR_EMBER_STONE)->update =
				scMagmaRockInsideSmokeEmber;
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		}
		sySynch(3);
	}
	scFieldActorWaitForMoveById(ACTOR_EMBER_STONE);
	syFieldMapGridWaitForShakeToSettle();
	fdEventPlaySnd(SE_STOP_ACTION_SE);
	fdEventWait(40);
	fdEventSetFlag(FLAG_ROCKS_RELEASED);
	syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	sySynch(10);
	fdEventEnd();
}

/* Felix casts Blaze at one of the flames the ember stone threw into the lava:
 */
/* Blaze sprays far, and unless the east torch on the ledge west of the lava is
 */
/* already lit, a small hidden flame waits on it, which grows if the spray */
/* reaches it (scMagmaRockInsideGrowFlame). */
void scMagmaRockInsideTakeFlame(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	state->blazeLongSpray = 1;
	if(!fdEventGetFlag(FLAG_EAST_TORCH_LIT)) {
		scFieldActorSetSprCol(fdActorGet(ACTOR_EAST_TORCH_FLAME),
							  SPRITE_COLOR_HIDDEN);
		fdActorSetPos(ACTOR_EAST_TORCH_FLAME, 632, 312);
		fdActorGet(ACTOR_EAST_TORCH_FLAME)->scaleX =
			fdActorGet(ACTOR_EAST_TORCH_FLAME)->scaleY = FIXED_ONE / 16;
	}
}

/* Defines what Blaze at a torch does: unless litFlag is set, flame waits */
/* hidden on it at (x, z), a sixteenth of its size. */
#define DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH(name, litFlag, flame, x, z)       \
	void name(void) {                                                          \
		if(!fdEventGetFlag(litFlag)) {                                         \
			scFieldActorSetSprCol(fdActorGet(flame), SPRITE_COLOR_HIDDEN);     \
			fdActorSetPos(flame, x, z);                                        \
			fdActorGet(flame)->scaleX = fdActorGet(flame)->scaleY =            \
				FIXED_ONE / 16;                                                \
		}                                                                      \
	}

/* Felix casts Blaze at the east torch's flame on the ledge west of the ember */
/* stone's lava: unless the west torch beside it is lit, a small hidden flame */
/* waits on it, which grows if the spray reaches it */
/* (scMagmaRockInsideGrowFlame). */
DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH(MagmaRockInside_LightTorch,
									 FLAG_WEST_TORCH_LIT,
									 ACTOR_WEST_TORCH_FLAME, 600, 312)

struct ACTOR;

/* Blaze's spray reaches a torch on the ledge west of the ember stone's lava, */
/* and its flame grows until it burns full. Once both torches burn, the ground
 */
/* rumbles, the camera turns to the stone pillar between them, and it sinks */
/* into the floor, opening the doorway behind it (cell (38, 18)). */
void scMagmaRockInsideGrowFlame(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *object;
	ACTOR *actor;
	ACTOR *other;
	s32 i;

	object = fdActorGetById(state->psynergyHitObj);
	scFieldActorSetSprCol(object, SPRITE_COLOR_NORMAL);
	fdEventPlaySnd(SE_WHOOSH);
	i = 0;
	do {
		object->scaleY = object->scaleX += FIXED_ONE / FLAME_GROW_STEPS;
		sySynch(5);
		i++;
	} while(i != FLAME_GROW_STEPS);
	fdEventSetFlag(FLAG_TORCH_LIT(state->psynergyHitObj));
	if(fdEventGetFlag(FLAG_WEST_TORCH_LIT) &&
	   fdEventGetFlag(FLAG_EAST_TORCH_LIT)) {
		other = fdActorGetById(ACTOR_BLOCKING_PILLAR);
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		fdMapGridSetShake(2, 1, 1);
		fdEventPlaySnd(SE_RUMBLE);
		syFieldEventWait(30);
		fdCameraMoveTo(616, CAMERA_KEEP, 296, 1);
		fdCameraWaitForArrival();
		syFieldEventWait(30);
		fdEventPlaySnd(SE_RISE_WITH_SPARKS_LOW);
		REG_BLDALPHA = BLEND_ALPHA(0, 16);
		REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
		{
			ACTOR *prepared = fdActorCreateSilhouette(
				other->x.fixed, other->groundHeight,
				other->z.fixed + toFIXED(48), SPRITE_YELLOW_BLOCK);

			other->motionFlg = 0;
			actor = prepared;
		}
		i = 0;
		do {
			other->y.fixed -= toFIXED(1);
			syFieldEventWait(5);
			i++;
		} while(i != PILLAR_SINK_DEPTH);
		fdEventPlaySnd(SE_FLAME_GROW);
		fdActorSelectAnimation(other, 4);
		fdActorRelease(actor);
		syFieldEventWait(1);
		REG_BLDCNT = 0;
		scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 38, 18, 0);
		fdEventEnd();
	}
}

/* Nothing visible happens when Blaze, cast at a flame in the ember stone's */
/* room, reaches no torch; the game takes away the hidden flames it had set */
/* waiting on the unlit torches. */
void scMagmaRockInsidePutOutFlames(void) {
	if(FieldStateWork->psynergyHitObj == -1) {
		if(!fdEventGetFlag(FLAG_EAST_TORCH_LIT))
			fdActorSetPos(ACTOR_EAST_TORCH_FLAME, 0, 0);
		if(!fdEventGetFlag(FLAG_WEST_TORCH_LIT))
			fdActorSetPos(ACTOR_WEST_TORCH_FLAME, 0, 0);
	}
}

/* Examining the ember stone, the red rock the eruption threw onto an islet in
 */
/* the lava (cell (43, 14)): "There's something burning like an ember within */
/* the stone." Answering yes, Felix takes the Magma Ball from it, if someone */
/* in the party can carry it. */
void scMagmaRockInsideExamineEmberStone(void) {
	const u8 *message;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	message = MsgBlazeStoneEmber;
	mnMessageShowCentered(message, 1);
	syFieldEventSetMessageId(message + 1);
	fdEventOpenMessage(ACTOR_EMBER_STONE, 0);
	if(syFieldEventAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		gmPartyGiveItem(ITEM_MAGMA_BALL, 0);
		if(syFieldPartyInventoryFindMember(ITEM_MAGMA_BALL) == -1) {
			fdEventEnd();
			return;
		}
		fdItemDispShowItem(ITEM_MAGMA_BALL,
						   ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
		fdActorSetAnimation(ACTOR_EMBER_STONE, 2);
		fdEventSetFlag(FLAG_MAGMA_BALL_TAKEN);
	}
	fdEventEnd();
}

/* Walking north into the stone wall under its golden crest, at the end of the
 */
/* sunken pillars' path in the ember stone room's west (cell (15, 21)), opens a
 */
/* stairway in it step by step, and Felix walks in and on to the Blaze */
/* tablet's room. */
void scMagmaRockInsideOpenStairs(void) {
	s32 width = 1;
	s32 height = 2;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdEventPlaySnd(SE_THUMP);
	fdMapGridCopyCellsTo(72, 67, 15, 19, width, height);
	syFieldEventWait(8);
	fdMapGridCopyCellsTo(73, 67, 15, 19, width, height);
	syFieldEventWait(8);
	fdMapGridCopyCellsTo(74, 67, 15, 19, width, height);
	syFieldEventWait(8);
	fdMapGridCopyCellsTo(75, 67, 15, 19, width, height);
	syFieldEventWait(15);
	fdActorGet(MainRoutineStateWork.leaderActorId)->motionFlg = 0;
	fdActorGet(MainRoutineStateWork.leaderActorId)->speedLimit =
		fdActorGet(MainRoutineStateWork.leaderActorId)->acceleration =
			toFIXED(1) / 2;
	fdEventPlaySnd(SE_MAP_EXIT);
	fdActorWalkTo(MainRoutineStateWork.leaderActorId, 248, 328);
	syFieldEventWait(5);
	syFieldEventSetExitRequest(
		MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_INSIDE_BLAZE_TABLET);
	fdEventEnd();
}

/* A frame task: once Felix stops on a new cell, puts back the picture of the */
/* stepping stone he left and, in the Blaze tablet's room, raises the trigger */
/* of that stone if the map's upper layer numbers it (STEP_CELL_TRIGGER_BASE */
/* plus its number, scMagmaRockInsideStepOnMarker). */
void scMagmaRockInsideTrackStepCell(void) {
	ACTOR *actor = fdActorGet(syFieldPartyGetLeaderActor());
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);
	s32 key = CELL_KEY(x, z);
	s32 last;
	s32 value;
	s32 trigger;
	FIELD_STATE *state;

	if(actor->velX != 0 || actor->velZ != 0)
		return;
	last = MagmaRockInside_LastStepCell;
	if(last == -1 || key == last)
		return;
	x = CELL_KEY_X(last);
	z = CELL_KEY_Z(last);
	MagmaRockInside_LastStepCell = -1;
	fdMapGridCopyCellsLayered(
		73, 72, MagmaRockInside_StepCellWidth, MagmaRockInside_StepCellHeight,
		MagmaRockInside_StepCellX, MagmaRockInside_StepCellZ);
	if(MainRoutineStateWork.next != SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET)
		return;
	state = Ram_HeapSlots->fieldState;
	value = fdMapCellGetAttributes(MAPGRID_LAYER_OVERHEAD, x, z) >> 8;
	if(value != 0) {
		trigger = value + STEP_CELL_TRIGGER_BASE;
		state->raisedTrigger = trigger;
	}
}

/* Starts watching the stepping stones Felix walks on in the Blaze tablet's */
/* room (scMagmaRockInsideTrackStepCell). */
void scMagmaRockInsideStartStepCells(void) {
	MagmaRockInside_LastStepCell = -1;
	syFieldTaskAddFrame(scMagmaRockInsideTrackStepCell, TASK_PASS_FRAME);
}

/* Nothing seems to happen as Felix steps onto one of the stepping stones that
 */
/* cross the Blaze tablet room's lava, but he is seen standing on top of it: */
/* the game saves the stone's picture under him (to cell (73, 72)) and draws */
/* the picture kept at (70, 72) in its place, until he leaves it */
/* (scMagmaRockInsideTrackStepCell). The stones of this picture alternate with
 */
/* the other kind along each row (scMagmaRockInsideStampOtherStepCell), as */
/* Air's Rock's do (scAirsRockInsideStampWideCellUnderLeader). */
void scMagmaRockInsideStampStepCell(void) {
	ACTOR *actor = fdActorGet(syFieldPartyGetLeaderActor());
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 y = FIELD_CELL(actor->y.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);
	s32 key = CELL_KEY(x, z);

	if(key != MagmaRockInside_LastStepCell) {
		MagmaRockInside_LastStepCell = key;
		MagmaRockInside_StepCellWidth = 1;
		MagmaRockInside_StepCellHeight = 2;
		MagmaRockInside_StepCellX = x + 64;
		MagmaRockInside_StepCellZ = z - y;
		fdMapGridCopyCellsLayered(MagmaRockInside_StepCellX,
								  MagmaRockInside_StepCellZ, 1, 2, 73, 72);
		fdMapGridCopyCellsLayered(70, 72, MagmaRockInside_StepCellWidth,
								  MagmaRockInside_StepCellHeight,
								  MagmaRockInside_StepCellX,
								  MagmaRockInside_StepCellZ);
	}
}

/* Nothing seems to happen as Felix steps onto a stepping stone of the other */
/* kind in the Blaze tablet room's lava, but he is seen standing on top of it:
 */
/* as scMagmaRockInsideStampStepCell, drawing the picture kept at (71, 72). */
void scMagmaRockInsideStampOtherStepCell(void) {
	ACTOR *actor = fdActorGet(syFieldPartyGetLeaderActor());
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 y = FIELD_CELL(actor->y.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);
	s32 key = CELL_KEY(x, z);

	if(key != MagmaRockInside_LastStepCell) {
		MagmaRockInside_LastStepCell = key;
		MagmaRockInside_StepCellWidth = 1;
		MagmaRockInside_StepCellHeight = 2;
		MagmaRockInside_StepCellX = x + 64;
		MagmaRockInside_StepCellZ = z - y;
		fdMapGridCopyCellsLayered(MagmaRockInside_StepCellX,
								  MagmaRockInside_StepCellZ, 1, 2, 73, 72);
		fdMapGridCopyCellsLayered(71, 72, MagmaRockInside_StepCellWidth,
								  MagmaRockInside_StepCellHeight,
								  MagmaRockInside_StepCellX,
								  MagmaRockInside_StepCellZ);
	}
}

/* The stepping stone at (x, z), in pixels, in the line of stepping stones that
 */
/* leads to the Blaze tablet, glows white with a drip and fades away into the */
/* lava (an object window over it, blended out), and its cell is walled off. */
void scMagmaRockInsideFadeStoneAway(s32 x, s32 z) {
	ACTOR *actor;
	s32 i;
	s32 cellZ;

	actor =
		scFieldActorCreate(SPRITE_STACKED_CRATES, toFIXED(x), 0, toFIXED(z));
	REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	scSetActorSprPriority(actor, 0);
	fdActorSetSprFlags(actor, 0);
	((FIELD_SPR *)actor->spr)->objMode = OBJ_MODE_WINDOW;
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |
				 WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_ALL;
	REG_DISPCNT |= DISPCNT_OBJWIN_ON;
	fdEventPlaySnd(SE_DRIP);
	for(i = 0; i != BLEND_WEIGHT_MAX / 2; i++) {
		REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, i * 2);
		syWait(1);
	}
	for(i = 0; i != BLEND_WEIGHT_MAX; i++) {
		REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - i, BLEND_WEIGHT_MAX);
		syWait(1);
	}
	x >>= 4;
	cellZ = (z >> 4) - 1;
	fdMapGridCopyCellsTo(65, 1, x + 64, cellZ, 1, 2);
	scMapCellSetAttributes(MAPGRID_LAYER_GROUND, x, cellZ,
						   CELL_BEHAVIOR_WALL << 8);
	fdActorRelease(actor);
	fdEventEnd();
	REG_BLDCNT = 0;
	REG_DISPCNT &= ~DISPCNT_OBJWIN_ON;
}

/* Felix walks north from the Blaze tablet room's entrance along the line of */
/* stepping stones to the tablet's platform (column 16 of the map): on every */
/* second stone, the stone he has just stepped off glows white with a drip and
 */
/* fades away into the lava, so there is no way back. Five go in all, the */
/* first the stone just north of the entrance's walkway, the last the stone */
/* just below the tablet's platform. Retreat is barred in this room too, so */
/* the party leaves by the way Blaze opens (scMagmaRockInsideGrowTabletFlame).
 */
/* The flags keep the stones gone. */
void scMagmaRockInsideStepOnMarker(void) {
	FIELD_STATE *state = FieldStateWork;
	ACTOR *leader;
	s16 *trigger;
	s32 cell, row = 54;

	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	cell = fdMapCellGetAttributes(MAPGRID_LAYER_OVERHEAD,
								  FIELD_CELL(leader->x.fixed),
								  FIELD_CELL(leader->z.fixed));
	trigger = &state->raisedTrigger;
	/* The marker's row: 54 for the first, four rows up for each after it. */
	row -= (*trigger - (STEP_CELL_TRIGGER_BASE + 1)) << 2;
	cell >>= 8;
	if(cell != MARKER_WALKWAY_END && cell != MARKER_PLATFORM_FOOT) {
		scMagmaRockInsideStampStepCell();
		scMagmaRockInsideFadeStoneAway(264, row * 8 + 10);
		if(!fdEventGetFlag(FLAG_ANY_MARKER_LIT))
			fdEventSetFlag(FLAG_ANY_MARKER_LIT);
		fdEventSetFlag(FLAG_MARKER_LIT(*trigger - STEP_CELL_TRIGGER_BASE));
	}
	else if(fdEventGetFlag(FLAG_ANY_MARKER_LIT)) {
		scMagmaRockInsideFadeStoneAway(264, row * 8 + 10);
	}
}

/* Felix casts Blaze at the flame in the west bowl on the ledge in the Blaze */
/* tablet room's south-west: unless the east bowl beside it is lit, a small */
/* hidden flame waits on it, which grows if the spray reaches it */
/* (scMagmaRockInsideGrowTabletFlame). */
DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH(MagmaRockInside_LightTabletTorch,
									 FLAG_TABLET_TORCH_LIT, ACTOR_TORCH_FLAME,
									 136, 360)

/* Blaze's spray reaches the east bowl on the Blaze tablet room's south-west */
/* ledge, and its flame grows until it burns full; the ground rumbles, the */
/* camera turns to the stone pillar between the bowls, and it sinks into the */
/* floor until its top is a stepping stone across the lava (cell (7, 21)), */
/* the way out of the room. */
void scMagmaRockInsideGrowTabletFlame(void) {
	ACTOR *object;
	ACTOR *actor;
	ACTOR *other;
	s32 i;

	object = fdActorGet(FieldStateWork->psynergyHitObj);
	scFieldActorSetSprCol(object, SPRITE_COLOR_NORMAL);
	fdEventPlaySnd(SE_WHOOSH);
	i = 0;
	do {
		object->scaleY = object->scaleX += FIXED_ONE / FLAME_GROW_STEPS;
		syWait(5);
		i++;
	} while(i != FLAME_GROW_STEPS);
	fdEventSetFlag(FLAG_TABLET_TORCH_LIT);
	other = fdActorGet(ACTOR_TORCH_PILLAR);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdMapGridSetShake(2, 1, 1);
	fdEventPlaySnd(SE_RUMBLE);
	syFieldEventWait(30);
	fdCameraMoveTo(120, CAMERA_KEEP, 344, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(30);
	fdEventPlaySnd(SE_RISE_WITH_SPARKS_LOW);
	REG_BLDALPHA = BLEND_ALPHA(0, 16);
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	{
		ACTOR *prepared = fdActorCreateSilhouette(
			other->x.fixed, other->groundHeight, other->z.fixed + toFIXED(48),
			SPRITE_YELLOW_BLOCK);

		other->motionFlg = 0;
		actor = prepared;
	}
	i = 0;
	do {
		other->y.fixed -= toFIXED(1);
		syFieldEventWait(5);
		i++;
	} while(i != PILLAR_SINK_DEPTH);
	fdEventPlaySnd(SE_FLAME_GROW);
	fdActorSelectAnimation(other, 4);
	fdActorRelease(actor);
	syFieldEventWait(1);
	REG_BLDCNT = 0;
	scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 7, 21, MAP_CELL_JUMP);
	fdEventEnd();
}

/* Nothing visible happens when Blaze, cast at the flame on the Blaze tablet */
/* room's south-west ledge, does not reach the east bowl; the game takes away */
/* the hidden flame it had set waiting there. */
void scMagmaRockInsidePutOutTabletFlame(void) {
	if(FieldStateWork->psynergyHitObj == -1) {
		if(!fdEventGetFlag(FLAG_TABLET_TORCH_LIT))
			fdActorSetPos(ACTOR_TORCH_FLAME, 0, 0);
	}
}

/* Leaves a small red spark behind a spark circling in to Jenna as she learns */
/* Blaze (scMagmaRockInsideOrbitParent). */
void scMagmaRockInsideSpawnSpark(ACTOR *source) {
	ACTOR *actor = Create(SPRITE_PARTICLE, source->x.fixed, source->y.fixed,
						  source->z.fixed);

	actor->scaleY = FIXED_ONE * 2 / 5;
	actor->scaleX = FIXED_ONE * 2 / 5;
	actor->motionFlg = 0;
	scFieldActorSetSprCol(actor, SPRITE_COLOR_RED_TINT);
	scSetActorSprPriority(actor, 1);
	fdActorSelectAnimation(actor, 1);
	fdActorSetSprFlags(actor, 0);
	fdActorSetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
	fdActorStartScript(actor, MagmaRockInside_FlameScript);
}

/* A spark circling Jenna as she learns Blaze, as its script reads the actor: */
/* its place, the radius it circles at, the steps it has left, its angle and */
/* speed, and the actor it circles. */
typedef struct SPARK_ORBITER {
	u8 beforeX[8];
	s32 x;
	s32 y;
	s32 z;
	u8 beforeRadius[56];
	s32 radius;
	u8 beforeTmr[18];
	u8 stepsLeft;
	u8 afterStepsLeft;
	u16 ang;
	u16 speed;
	struct SPARK_ORBITER *parent;
} SPARK_ORBITER;

/* A spark's script call: circles in to its parent, leaving sparks, until its */
/* steps run out; 0 ends its script. */
s32 scMagmaRockInsideOrbitParent(SPARK_ORBITER *object) {
	u16 *angle = &object->ang;
	u16 *speed = &object->speed;
	SPARK_ORBITER *parent = object->parent;
	s32 *position;
	s16 heading;

	object->ang += (s16)*speed >> 1;
	object->x = parent->x;
	object->y = parent->y + toFIXED(8);
	object->z = parent->z;
	heading = *angle;
	syFieldVectorAddPolarOffsetFX(object->radius, heading,
								  position = &object->x);
	if(--object->stepsLeft == 0)
		return 0;
	if(object->radius == toFIXED(2))
		return 1;
	scMagmaRockInsideSpawnSpark(object);
	object->radius -= toFIXED(4);
	object->ang += (s16)*speed >> 1;
	*position = parent->x;
	object->y = parent->y + toFIXED(8);
	object->z = parent->z;
	syFieldVectorAddPolarOffsetFX(object->radius, (s16)*angle, position);
	scMagmaRockInsideSpawnSpark(object);
	object->radius -= toFIXED(4);
	return 1;
}

/* Moves around in a circle. */
#include "shared/orbit_update.h"

#include "shared/actor_aim_at_watched.h"

/* A rising spark's script call: drifts by its centre and radius, kept as */
/* velocities, for its phase's count of steps; 0 ends its script. */
s32 scMagmaRockInsideDriftForSteps(ORBIT *object) {
	u8 *steps;
	s32 left;
	u8 stepsLeft;

	object->x += object->centreX;
	object->y += object->orbitRadius;
	object->z += object->centreZ;
	steps = &object->phase;
	left = *steps + (u8)-1;
	*steps = left;
	stepsLeft = left;
	return stepsLeft != 0;
}

/* An actor's update: drops a white sparkle near it every eighth frame. */
void scMagmaRockInsideTrailSparks(ACTOR *actor) {
	EFFECT_OPTIONS options;
	FIXED position[3];
	u32 frame = FrameCount & 7;

	if(frame != 0)
		return;
	options.pal = SPRITE_COLOR_WHITE;
	options.type = SPRITE_SPARKLE;
	options.pri = 1;
	position[0] = actor->x.fixed + (((syRandom() & 31) - 16) << 16);
	position[1] = actor->y.fixed + toFIXED(syRandom() & 31) + toFIXED(8);
	position[2] = actor->z.fixed;
	scFieldEffectSpawn(
		position[0], position[1], position[2] + toFIXED(4), 0, toFIXED(1) / 2,
		frame, EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
		&options);
}

extern u32 Lesson_TabletRiseScript[];
extern s32 MagmaRockInside_RisingSparkScript[];
extern s32 MagmaRockInside_CirclingSparkScript[];
extern u32 Lesson_AdeptRiseScript[];
extern s32 MagmaRockInside_OrbitScript[];
extern s32 Lesson_TabletSettleScript[];
extern s32 Lesson_AdeptSettleScript[];

/* The Blaze tablet's lesson: the music fades, the ground shakes and the */
/* screen takes on a warm tint as the tablet lifts, blinking and shooting */
/* sparks up; red sparks circle in to the one who learns and rise round her as
 */
/* she lifts too, the screen flashes white, and both settle back as the music */
/* returns. */
void scMagmaRockInsideLearnBlaze(s32 tabletId, s32 learnerId) {
	ACTOR *tablet = fdActorGetById(tabletId);
	ACTOR *learner = fdActorGetById(learnerId);
	ACTOR *actor;
	ACTOR *spark;
	ACTOR *riser;
	s32 i;
	u16 noMotion;
	s32 probe;
	const u8 *script;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	s32 unused[3];

	script = Lesson_AdeptRiseScript;
	fdEventPlaySnd(SOUND_FADE_OUT_BGM);
	fdEventWait(30);
	fdCameraSetSpeed(0.5);
	fdCameraMoveToFX(toFIXED(tablet->x.part[0].integer),
					 toFIXED(tablet->y.part[0].integer),
					 toFIXED(tablet->z.part[0].integer), 1);
	fdEventPlaySnd(SE_RUMBLE);
	fdMapGridSetShake(2, 2, 1);
	fdPaletteFadeSetSource(PALETTE_TINT_NONE, 0);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 1);
	fdPaletteFadeStartInterpolation(60);
	tablet->motionFlg = 0;
	tablet->acceleration = toFIXED(1) / 5;
	tablet->speedLimit = toFIXED(1) / 5;
	fdActorSetScript(tabletId, Lesson_TabletRiseScript);
	fdEventPlaySnd(SE_TABLET_RISE);
	fdEventWait(45);
	fdActorSetSprColById(tabletId, SPRITE_COLOR_BLINK);
	REG_BLDALPHA = BLEND_ALPHA(8, 16);
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	i = 0;
	do {
		actor = (ACTOR *)syImportActorCreate(SPRITE_PARTICLE, tablet->x.fixed,
											 tablet->y.fixed, tablet->z.fixed);
		fdEventPlaySnd(SE_GLITTER);
		actor->x.fixed += ((syRandom() & 31) - 16) << 16;
		actor->y.fixed += ((syRandom() & 15) - 8) << 16;
		actor->speedLimit = toFIXED(8);
		actor->acceleration = toFIXED(1) / 5;
		actor->motionFlg = 0;
		scFieldActorSetSprCol(actor, SPRITE_COLOR_RED_TINT);
		scSetActorSprPriority(actor, 1);
		fdActorSelectAnimation(actor, 7);
		fdActorSetSprFlags(actor, 0);
		fdActorSetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
		fdActorStartScript(actor, MagmaRockInside_RisingSparkScript);
		REG_BLDALPHA = BLEND_ALPHA(4, 16);
		sySynch(2);
		REG_BLDALPHA = BLEND_ALPHA(10, 16);
		sySynch(2);
		REG_BLDALPHA = BLEND_ALPHA(16, 16);
		sySynch(2);
		REG_BLDALPHA = BLEND_ALPHA(10, 16);
		sySynch(2);
		i++;
	} while(i != TABLET_RISING_SPARKS);
	REG_BLDALPHA = BLEND_ALPHA(8, 16);
	sySynch(30);
	for(i = 0; i != LEARN_SPARKS; i++) {
		spark = scFieldActorCreate(SPRITE_PARTICLE, learner->x.fixed,
								   learner->y.fixed, learner->z.fixed);
		fdEventPlaySnd(SE_SPARKS);
		spark->tmr = (syRandom() & 0x7fff) + DEGtoANG(180);
		spark->userValue = (((i & 1) << 1) - 1) << ((syRandom() & 3) + 9);
		noMotion = 0;
		spark->motionFlg = noMotion;
		spark->sceneWord = toFIXED(122);
		spark->watchedActor = (FIELD_OBJECT *)learner;
		spark->stepsLeft =
			(LEARN_SPARKS - i) * LEARN_SPARK_INTERVAL + LEARN_SPIRAL_FRAMES;
		scFieldActorSetSprCol(spark, SPRITE_COLOR_RED_TINT);
		scSetActorSprPriority(spark, 1);
		fdActorSelectAnimation(spark, 7);
		fdActorSetSprFlags(spark, 0);
		fdActorSetBlendMode(spark, OBJ_MODE_SEMI_TRANSPARENT);
		syFieldVectorAddPolarOffsetFX(spark->sceneWord, spark->tmr,
									  &spark->x.fixed);
		fdActorStartScript(spark, MagmaRockInside_CirclingSparkScript);
		if(i == 3) {
			fdActorSetSprColById(learnerId, SPRITE_COLOR_BLINK);
			learner->motionFlg = noMotion;
			learner->acceleration = toFIXED(1) / 5;
			learner->speedLimit = toFIXED(1) / 5;
			fdActorSetScript(learnerId, script);
		}
		sySynch(LEARN_SPARK_INTERVAL);
	}
	fdEventPlaySnd(SE_SHIMMER);
	sySynch(16);
	i = 2;
	do {
		riser = scFieldActorCreate(
			SPRITE_PARTICLE, learner->x.fixed,
			learner->y.fixed + i * toFIXED(1) - toFIXED(8), learner->z.fixed);
		riser->tmr = syRandom();
		riser->userValue = TRIG_TURN / 32;
		((ORBIT *)riser)->phase = i;
		((ORBIT *)riser)->growth = 1;
		riser->bounce = riser->x.fixed;
		riser->sceneWord = 0;
		riser->gravity = riser->z.fixed;
		riser->watchedActor = (FIELD_OBJECT *)learner;
		riser->motionFlg = (u16)0;
		riser->priFlg = ACTOR_PRIORITY_AUTOMATIC;
		scFieldActorSetSprCol(riser, SPRITE_COLOR_RED_TINT);
		fdActorSelectAnimation(riser, 7);
		fdActorSetSprFlags(riser, 0);
		fdActorSetBlendMode(riser, OBJ_MODE_SEMI_TRANSPARENT);
		fdActorStartScript(riser, MagmaRockInside_OrbitScript);
		i += 2;
	} while(i != ORBIT_HALF_TURN);
	fdEventPlaySnd(SE_SHIMMER);
	sySynch(50);
	fdPaletteFadeSetTarget(RGB_WHITE, 0);
	fdPaletteFadeStartInterpolation(8);
	sySynch(16);
	fdMapGridSetShake(0, 0, 0);
	fdActorSetSprColById(tabletId, SPRITE_COLOR_NORMAL);
	fdActorSetSprColById(learnerId, SPRITE_COLOR_NORMAL);
	fdEventPlaySnd(SE_FLASH);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeStartInterpolation(80);
	fdActorSetScript(tabletId, Lesson_TabletSettleScript);
	fdActorSetScript(learnerId, Lesson_AdeptSettleScript);
	learner->update = (OBJECT_UPDATE)scMagmaRockInsideTrailSparks;
	sySynch(120);
	learner->update = NULL;
	fdEventPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	scPlayMusic();
	sySynch(1);
}

/* Felix reads the Blaze tablet from the south: "Wielder of Flame's strength, */
/* Lay your hands upon this stone." The first time, Jenna steps out beside him
 */
/* ("Flame's strength? Hey, that must mean it's finally my turn!"), he nods and
 */
/* steps back, she lays her hands on the tablet and learns Blaze, and after */
/* they nod to each other she walks back into the party. Once Blaze is */
/* learned, only the inscription is shown. */
void scMagmaRockInsideReadBlazeTablet(void) {
	ACTOR *actor;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	mnMessageShowCentered(MsgBlazeTabletInscription, 1);
	if(fdEventGetFlag(FLAG_BLAZE_LEARNED) != 0) {
		fdEventEnd();
	}
	else {
		syFieldEventSetMessageId((s32)MsgBlazeTabletInscription);
		FieldStateWork->msg++;
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.7);
		fdActorSetSpeed(ACTOR_JENNA, 0.7);
		fdActorPlaceOnActor(ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
		syFieldEventWait(1);
		fdActorWalkTo(MainRoutineStateWork.leaderActorId, 256, 136);
		fdActorWalkTo(ACTOR_JENNA, 272, 136);
		scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		scFieldActorWaitForMoveById(ACTOR_JENNA);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST,
							 0);
		fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
		syFieldEventWait(10);
		/* "Flame's strength? Hey, that must mean it's finally my turn!" */
		fdEventShowMessage(ACTOR_JENNA, 0);
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
		syFieldEventWait(30);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 256, 152);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 0);
		syFieldEventWait(20);
		fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
		syFieldEventWait(20);
		fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
		syFieldEventWait(30);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
		syFieldEventWait(20);
		fdActorWalkToAndWait(ACTOR_JENNA, 264, 137);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
		syFieldEventWait(20);
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
		mnMessageShowCentered(MsgBlazeTabletInscription + 3, 1);
#else
		mnMessageShowCentered(MsgBlazeTabletInscription + 2, 1);
#endif
		fdMapGridSetShake(5, 7, 1);
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		syFieldMapGridWaitForShakeToSettle();
		scMagmaRockInsideLearnBlaze(ACTOR_BLAZE_TABLET, ACTOR_JENNA);
		fdPartyLearnAbility(JENNA, ABILITY_BLAZE);
		fdEventSetFlag(FLAG_BLAZE_LEARNED);
		fdActorFaceEachOther(MainRoutineStateWork.leaderActorId, ACTOR_JENNA,
							 0);
		syFieldEventWait(10);
		fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
		syFieldEventWait(20);
		fdActorSetSpeed(ACTOR_JENNA, 1.2);
		fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
		actor = fdActorGet(MainRoutineStateWork.leaderActorId);
		if(actor != NULL)
			fdActorSetDestination(ACTOR_JENNA, actor->x.part[0].integer,
								  actor->z.part[0].integer);
		scFieldActorWaitForMoveById(ACTOR_JENNA);
		fdActorSetPos(ACTOR_JENNA, 0, 0);
		fdActorSetMovementType(fdActorGet(ACTOR_BLAZE_TABLET),
							   MOVEMENT_TYPE_TAKE_LEADER_LAYER);
		fdEventEnd();
	}
}

/* Felix reads the Blaze tablet from behind, on its north side: the first time
 */
/* he walks round its west side to the front and it is read there */
/* (scMagmaRockInsideReadBlazeTablet); once Blaze is learned only its */
/* inscription is shown. */
void scMagmaRockInsideReadBlazeTabletFromNorth(void) {
	if(fdEventGetFlag(FLAG_BLAZE_LEARNED)) {
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		mnMessageShowCentered(MsgBlazeTabletInscription, 1);
		fdEventEnd();
	}
	else {
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 248, 107);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 248, 136);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 264, 136);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 0);
		syFieldEventWait(10);
		scMagmaRockInsideReadBlazeTablet();
	}
}

/* Defines a function for reading the Blaze tablet from one side: once Blaze */
/* is learned only its inscription shows; before, Felix walks from sideX round
 */
/* to its front, faces it, and reads it. */
#define DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM(name, sideX)                 \
	void name(void) {                                                          \
		if(fdEventGetFlag(FLAG_BLAZE_LEARNED)) {                               \
			fdEventStart();                                                    \
			syFieldEventEndLastingPsynergy(0);                                 \
			mnMessageShowCentered(MsgBlazeTabletInscription, 1);               \
			fdEventEnd();                                                      \
		}                                                                      \
		else {                                                                 \
			fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, sideX,    \
								 136);                                         \
			fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 264,      \
								 136);                                         \
			fdActorFaceDirection(MainRoutineStateWork.leaderActorId,           \
								 FACING_NORTH, 0);                             \
			syFieldEventWait(10);                                              \
			scMagmaRockInsideReadBlazeTablet();                                \
		}                                                                      \
	}

/* Felix reads the Blaze tablet from its east side: the first time he steps */
/* round to the front and it is read there (scMagmaRockInsideReadBlazeTablet);
 */
/* once Blaze is learned only its inscription is shown. */
DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM(
	MagmaRockInside_ReadBlazeTabletFromEast, 280)

/* Felix reads the Blaze tablet from its west side: the first time he steps */
/* round to the front and it is read there (scMagmaRockInsideReadBlazeTablet);
 */
/* once Blaze is learned only its inscription is shown. */
DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM(
	MagmaRockInside_ReadBlazeTabletFromWest, 248)

extern TRIGGER MagmaRockInside_Triggers[];
extern TRIGGER MagmaRockInside_EntranceTriggers[];
extern TRIGGER MagmaRockInside_FuryTriggers[];
extern TRIGGER MagmaRockInside_CrackedPillarsTriggers[];
extern TRIGGER MagmaRockInside_GolemCoreChestTriggers[];
extern TRIGGER MagmaRockInside_SalamanderTailChestTriggers[];
extern TRIGGER MagmaRockInside_EmberStoneTriggers[];
extern TRIGGER MagmaRockInside_BlazeTabletTriggers[];

const TRIGGER *scGetTriggers(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene != SCENE_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST) {
		if(scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE)
			return MagmaRockInside_EntranceTriggers;
		if(scene == SCENE_MAGMA_ROCK_INSIDE_FURY)
			return MagmaRockInside_FuryTriggers;
		if(scene == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS)
			return MagmaRockInside_CrackedPillarsTriggers;
		if(scene == SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST)
			return MagmaRockInside_GolemCoreChestTriggers;
		if(scene == SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST)
			return MagmaRockInside_SalamanderTailChestTriggers;
		if(scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE)
			return MagmaRockInside_EmberStoneTriggers;
		if(scene == SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET)
			return MagmaRockInside_BlazeTabletTriggers;
	}
	return MagmaRockInside_Triggers;
}

/* The entrance hall as flooded: its flag set, the cell at (15, 13) no longer */
/* climbable, and its columns and pillar shown standing in the lava. */
void scMagmaRockInsideSetEntranceFlooded(void) {
	s32 i;

	fdEventSetFlag(FLAG_ENTRANCE_FLOODED);
	scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 15, 13, 0);
	for(i = 0; i != ACTOR_COLUMN_F - ACTOR_COLUMN_A + 1; i++)
		fdActorSetAnimation(i + ACTOR_COLUMN_A, 2);
	fdActorSetAnimation(ACTOR_ENTRANCE_PILLAR, 4);
}

/* Fury's room as flooded: its flag set, the cells at (24, 15) and (9, 16) no */
/* longer climbable, and its pillar shown standing in the lava; from the */
/* cracked pillars' room, only the cell at (26, 14). */
void scMagmaRockInsideSetFuryFlooded(void) {
	fdEventSetFlag(FLAG_FURY_FLOODED);
	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY) {
		scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 24, 15, 0);
		scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 9, 16, 0);
		fdActorSetAnimation(ACTOR_FURY_PILLAR, 4);
	}
	else {
		scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 26, 14, 0);
	}
}

/* The lower rooms as flooded: their flag set and, in the cracked pillars' */
/* room, its eight white platforms afloat in pairs, each pair's second starting
 */
/* across the square from the first, and its three pillars and column shown */
/* standing in the lava. */
void scMagmaRockInsideSetLowerRoomsFlooded(void) {
	fdEventSetFlag(FLAG_LOWER_ROOMS_FLOODED);
	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS) {
		s32 *script = MagmaRockInside_CrackedPillarsFirstPairPath;

		scMagmaRockInsideStartLavaPlatform(ACTOR_LAVA_PLATFORM_A, script,
										   FLAG_LOWER_ROOMS_FLOODED);
		scMagmaRockInsideStartLavaPlatform(
			ACTOR_LAVA_PLATFORM_B, MagmaRockInside_CrackedPillarsFirstPairPath,
			FLAG_LOWER_ROOMS_FLOODED);
		scMagmaRockInsideStartLavaPlatform(
			ACTOR_LAVA_PLATFORM_C, MagmaRockInside_CrackedPillarsSecondPairPath,
			FLAG_LOWER_ROOMS_FLOODED);
		scMagmaRockInsideStartLavaPlatform(
			ACTOR_LAVA_PLATFORM_D, MagmaRockInside_CrackedPillarsSecondPairPath,
			FLAG_LOWER_ROOMS_FLOODED);
		scMagmaRockInsideStartLavaPlatform(
			ACTOR_LAVA_PLATFORM_E, MagmaRockInside_CrackedPillarsThirdPairPath,
			FLAG_LOWER_ROOMS_FLOODED);
		scMagmaRockInsideStartLavaPlatform(
			ACTOR_LAVA_PLATFORM_F, MagmaRockInside_CrackedPillarsThirdPairPath,
			FLAG_LOWER_ROOMS_FLOODED);
		scMagmaRockInsideStartLavaPlatform(
			ACTOR_LAVA_PLATFORM_G, MagmaRockInside_CrackedPillarsFourthPairPath,
			FLAG_LOWER_ROOMS_FLOODED);
		scMagmaRockInsideStartLavaPlatform(
			ACTOR_LAVA_PLATFORM_H, MagmaRockInside_CrackedPillarsFourthPairPath,
			FLAG_LOWER_ROOMS_FLOODED);
		if(fdActorGet(ACTOR_LAVA_PLATFORM_B)->scriptCursor == 0) {
			fdActorGet(ACTOR_LAVA_PLATFORM_B)->scriptCursor =
				PLATFORM_PATH_THIRD_CORNER;
			fdActorGet(ACTOR_LAVA_PLATFORM_D)->scriptCursor =
				PLATFORM_PATH_THIRD_CORNER;
			fdActorGet(ACTOR_LAVA_PLATFORM_F)->scriptCursor =
				PLATFORM_PATH_THIRD_CORNER;
			fdActorGet(ACTOR_LAVA_PLATFORM_H)->scriptCursor =
				PLATFORM_PATH_THIRD_CORNER;
		}
		fdActorSetAnimation(ACTOR_PILLAR_A, 4);
		fdActorSetAnimation(ACTOR_PILLAR_B, 4);
		fdActorSetAnimation(ACTOR_PILLAR_C, 4);
		fdActorSetAnimation(ACTOR_LOWER_COLUMN, 2);
	}
}

/* The entrance hall pillar's update: its drawing order comes from the cells */
/* under it, except while Felix stands at its height and not in front of it, */
/* when it is drawn at the lowest priority. */
void scMagmaRockInsideLayerWithLeader(ACTOR *arg) {
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	ACTOR *actor;
	ACTOR *owner;
	s16 ownerY;
	s16 y;
	u8 *flags;

	actor = arg;
	owner = fdActorGet(party->leaderActorId);
	ownerY = FIXEDtoINT(owner->y.fixed);
	y = FIXEDtoINT(actor->y.fixed);
	flags = &actor->priFlg;

	if(ownerY != y ||
	   (s16)(FIXEDtoINT(owner->z.fixed)) < (s16)(FIXEDtoINT(actor->z.fixed))) {
		*flags = 1;
	}
	else {
		*flags = 0;
		scSetActorSprPriority(actor, 3);
	}
}

#undef DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR

/* The crumbling pillars and floors of Magma Rock's outside follow, unused */
/* here. */

extern s32 MagmaRockInside_LuckyMedalPlatformPath[];
extern s32 MagmaRockInside_FuryWestPlatformPath[];
extern s32 MagmaRockInside_FuryEastPlatformPath[];
extern s32 MagmaRockInside_CrackedPillarsWestPlatformPath[];
extern s32 MagmaRockInside_CrackedPillarsEastPlatformPath[];
extern s32 MagmaRockInside_SalamanderTailNorthPlatformPath[];
extern s32 MagmaRockInside_SalamanderTailSouthwestPlatformPath[];
extern s32 MagmaRockInside_SalamanderTailSoutheastPlatformPath[];
extern s16 MagmaRockInside_Statues[];

s32 scMapCellSetAttributes(u32 layer, s32 x, s32 y, s32 value);

/* Blends BG2 out of sight: weight 0 over every layer beneath it. */
#define CLOSED_BLEND()                                                         \
	{                                                                          \
		REG_BLDALPHA = BLEND_ALPHA(0, 16);                                     \
		REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;  \
	}

/* Stands an unseen marker 20 pixels up on a floor switch, falling, as a */
/* platform Felix stands on. */
static __inline__ void Raise(s32 id) {
	ACTOR *actor = fdActorGetById(id);

	actor->motionFlg = ACTOR_FALLS;
	actor->y.fixed = actor->groundHeight = toFIXED(20);
	actor->priFlg |= ACTOR_PRIORITY_PLATFORM;
}

/* Settles an actor at the given height, its gravity off. */
static __inline__ void Lower(s32 id, s32 height) {
	fdActorGetById(id)->motionFlg = 0;
	fdActorGetById(id)->y.fixed = fdActorGetById(id)->groundHeight = height;
}

s32 scInit(void) {
	s32 i;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST) {
		fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
		scMagmaRockInsideStartLavaPlatform(
			ACTOR_LUCKY_MEDAL_PLATFORM, MagmaRockInside_LuckyMedalPlatformPath,
			FLAG_ENTRANCE_FLOODED);
		if(!fdEventGetFlag(FLAG_ENTRANCE_FLOODED)) {
			CLOSED_BLEND();
			fdMapGridCopyCellsTo(71, 64, 47, 98, 1, 1);
		}
	}
	else if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE) {
		s32 j;

		fdEffectStartBlankOverlay(0);
		fdActorGetById(ACTOR_ENTRANCE_PILLAR)->update =
			scMagmaRockInsideLayerWithLeader;
		for(j = 0; j != ACTOR_COLUMN_F - ACTOR_COLUMN_A + 1; j++) {
			{
				ACTOR *actor = fdActorGetById(j + ACTOR_COLUMN_A);

				actor->motionFlg = 0;
				actor->y.fixed = actor->groundHeight = 0;
			}
		}
		if(fdEventGetFlag(FLAG_ENTRANCE_PILLAR_PUSHED))
			fdActorGetById(ACTOR_ENTRANCE_PILLAR)->z.fixed = toFIXED(264);
		fdActorPlaceBlocks(MagmaRockInside_EntrancePillars);
		scMagmaRockInsideRaiseActorCells(MagmaRockInside_Statues);
		fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
		Raise(ACTOR_ENTRANCE_SWITCH_MARKER);
		if(fdEventGetFlag(FLAG_ENTRANCE_FLOODED))
			scMagmaRockInsideSetEntranceFlooded();
		else
			CLOSED_BLEND();
	}
	else if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY) {
		fdEffectStartBlankOverlay(0);
		if(fdEventGetFlag(FLAG_FURY_PILLAR_PUSHED))
			fdActorGetById(ACTOR_FURY_PILLAR)->z.fixed = toFIXED(664);
		fdActorPlaceBlocks(MagmaRockInside_FuryPillars);
		fdTightropeInit();
		fdTightropePlace(0, ACTOR_TIGHTROPE_WEST_POST,
						 ACTOR_TIGHTROPE_EAST_POST);
		scMagmaRockInsideRaiseActorCells(MagmaRockInside_Statues);
		fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
		Raise(ACTOR_FURY_SWITCH_MARKER);
		scMagmaRockInsideStartLavaPlatform(ACTOR_WEST_PLATFORM,
										   MagmaRockInside_FuryWestPlatformPath,
										   FLAG_FURY_FLOODED);
		scMagmaRockInsideStartLavaPlatform(ACTOR_EAST_PLATFORM,
										   MagmaRockInside_FuryEastPlatformPath,
										   FLAG_FURY_FLOODED);
		if(fdEventGetFlag(FLAG_FURY_FLOODED))
			scMagmaRockInsideSetFuryFlooded();
		else
			CLOSED_BLEND();
	}
	else if(MainRoutineStateWork.next ==
			SCENE_MAGMA_ROCK_INSIDE_CRACKED_PILLARS) {
		fdEffectStartBlankOverlay(0);
		fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
		scMagmaRockInsideRaiseActorCells(MagmaRockInside_CrackedPillarsStatues);
		if(MainRoutineStateWork.entrance <=
		   CRACKED_PILLARS_UPPER_HALL_LAST_ENTRANCE) {
			scMagmaRockInsideStartLavaPlatform(
				ACTOR_WEST_PLATFORM,
				MagmaRockInside_CrackedPillarsWestPlatformPath,
				FLAG_FURY_FLOODED);
			scMagmaRockInsideStartLavaPlatform(
				ACTOR_EAST_PLATFORM,
				MagmaRockInside_CrackedPillarsEastPlatformPath,
				FLAG_FURY_FLOODED);
			if(fdEventGetFlag(FLAG_FURY_FLOODED)) {
				scMagmaRockInsideSetFuryFlooded();
			}
			else {
				CLOSED_BLEND();
				fdMapGridCopyCellsTo(71, 64, 35, 82, 1, 1);
			}
		}
		else {
			fdActorGetById(ACTOR_PILLAR_A)->priFlg |=
				ACTOR_PRIORITY_IGNORES_RISES;
			fdActorGetById(ACTOR_PILLAR_B)->priFlg |=
				ACTOR_PRIORITY_IGNORES_RISES;
			fdActorGetById(ACTOR_PILLAR_C)->priFlg |=
				ACTOR_PRIORITY_IGNORES_RISES;
			fdActorSetSprPriority(ACTOR_PILLAR_A, 3);
			fdActorSetSprPriority(ACTOR_PILLAR_B, 3);
			fdActorSetSprPriority(ACTOR_PILLAR_C, 3);
			if(fdEventGetFlag(FLAG_PILLAR_A_PUSHED))
				fdActorGetById(ACTOR_PILLAR_A)->x.fixed = toFIXED(344);
			if(fdEventGetFlag(FLAG_PILLAR_B_PUSHED))
				fdActorGetById(ACTOR_PILLAR_B)->x.fixed = toFIXED(344);
			if(fdEventGetFlag(FLAG_PILLAR_C_PUSHED))
				fdActorGetById(ACTOR_PILLAR_C)->x.fixed = toFIXED(344);
			fdActorPlaceBlocks(MagmaRockInside_CrackedPillarsPillars);
			Lower(ACTOR_LOWER_COLUMN, 0);
			/* The platforms ride in and out of view: each keeps its sprite's
			 * tiles */
			/* in VRAM and its picture composed while off screen, so it shows at
			 */
			/* once as it comes back. */
			fdActorGetById(ACTOR_LAVA_PLATFORM_A)->retainSprCache =
				ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
			fdActorGetById(ACTOR_LAVA_PLATFORM_B)->retainSprCache =
				ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
			fdActorGetById(ACTOR_LAVA_PLATFORM_C)->retainSprCache =
				ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
			fdActorGetById(ACTOR_LAVA_PLATFORM_D)->retainSprCache =
				ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
			fdActorGetById(ACTOR_LAVA_PLATFORM_E)->retainSprCache =
				ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
			fdActorGetById(ACTOR_LAVA_PLATFORM_F)->retainSprCache =
				ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
			fdActorGetById(ACTOR_LAVA_PLATFORM_G)->retainSprCache =
				ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
			fdActorGetById(ACTOR_LAVA_PLATFORM_H)->retainSprCache =
				ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
			scMagmaRockInsideStartLavaPlatform(
				ACTOR_LAVA_PLATFORM_A,
				MagmaRockInside_CrackedPillarsFirstPairPath,
				FLAG_LOWER_ROOMS_FLOODED);
			scMagmaRockInsideStartLavaPlatform(
				ACTOR_LAVA_PLATFORM_B,
				MagmaRockInside_CrackedPillarsFirstPairPath,
				FLAG_LOWER_ROOMS_FLOODED);
			scMagmaRockInsideStartLavaPlatform(
				ACTOR_LAVA_PLATFORM_C,
				MagmaRockInside_CrackedPillarsSecondPairPath,
				FLAG_LOWER_ROOMS_FLOODED);
			scMagmaRockInsideStartLavaPlatform(
				ACTOR_LAVA_PLATFORM_D,
				MagmaRockInside_CrackedPillarsSecondPairPath,
				FLAG_LOWER_ROOMS_FLOODED);
			scMagmaRockInsideStartLavaPlatform(
				ACTOR_LAVA_PLATFORM_E,
				MagmaRockInside_CrackedPillarsThirdPairPath,
				FLAG_LOWER_ROOMS_FLOODED);
			scMagmaRockInsideStartLavaPlatform(
				ACTOR_LAVA_PLATFORM_F,
				MagmaRockInside_CrackedPillarsThirdPairPath,
				FLAG_LOWER_ROOMS_FLOODED);
			scMagmaRockInsideStartLavaPlatform(
				ACTOR_LAVA_PLATFORM_G,
				MagmaRockInside_CrackedPillarsFourthPairPath,
				FLAG_LOWER_ROOMS_FLOODED);
			scMagmaRockInsideStartLavaPlatform(
				ACTOR_LAVA_PLATFORM_H,
				MagmaRockInside_CrackedPillarsFourthPairPath,
				FLAG_LOWER_ROOMS_FLOODED);
			if(fdEventGetFlag(FLAG_LOWER_ROOMS_FLOODED))
				scMagmaRockInsideSetLowerRoomsFlooded();
			else
				CLOSED_BLEND();
		}
	}
	else if(MainRoutineStateWork.next ==
			SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST) {
		fdEffectStartBlankOverlay(0);
		fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
		fdHangingVinePlace(ACTOR_HANGING_CHAIN_A, 1);
		fdHangingVinePlace(ACTOR_HANGING_CHAIN_B, 1);
		Raise(ACTOR_NORTH_SWITCH_MARKER);
		Raise(ACTOR_SOUTH_SWITCH_MARKER);
		if(fdEventGetFlag(FLAG_LOWER_ROOMS_FLOODED)) {
			scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 22, 26, 0);
			scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 25, 26, 0);
			fdActorSetSprPriority(FIELD_TREASURE_ACTOR, 3);
		}
		else {
			CLOSED_BLEND();
		}
	}
	else if(MainRoutineStateWork.next ==
			SCENE_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST) {
		fdEffectStartBlankOverlay(0);
		fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
		if(fdEventGetFlag(FLAG_NORTH_PILLAR_PUSHED)) {
			fdMapGridCopyCellAttributes(46, 74, 1, 1, 46, 73);
			fdActorGetById(ACTOR_NORTH_PILLAR)->x.fixed = toFIXED(760);
		}
		if(fdEventGetFlag(FLAG_EAST_PILLAR_PUSHED))
			fdActorSetPos(ACTOR_EAST_PILLAR, 920, 616);
		fdActorPlaceBlocks(MagmaRockInside_SalamanderTailChestPillars);
		scMagmaRockInsideStartLavaPlatform(
			ACTOR_NORTH_PLATFORM,
			MagmaRockInside_SalamanderTailNorthPlatformPath,
			FLAG_LOWER_ROOMS_FLOODED);
		scMagmaRockInsideStartLavaPlatform(
			ACTOR_SOUTHWEST_PLATFORM,
			MagmaRockInside_SalamanderTailSouthwestPlatformPath,
			FLAG_LOWER_ROOMS_FLOODED);
		scMagmaRockInsideStartLavaPlatform(
			ACTOR_SOUTHEAST_PLATFORM,
			MagmaRockInside_SalamanderTailSoutheastPlatformPath,
			FLAG_LOWER_ROOMS_FLOODED);
		if(fdEventGetFlag(FLAG_LOWER_ROOMS_FLOODED)) {
			fdActorSetAnimation(ACTOR_NORTH_PILLAR, 4);
			if(fdEventGetFlag(FLAG_EAST_PILLAR_PUSHED))
				fdActorSetAnimation(ACTOR_EAST_PILLAR, 4);
			if(fdEventGetFlag(FLAG_NORTH_PILLAR_PUSHED))
				scMapCellSetAttributes(MAPGRID_LAYER_OVERHEAD, 46, 8,
									   CELL_BEHAVIOR_WALL << 8);
			scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 10, 45, 0);
			scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 53, 40, 0);
		}
		else {
			CLOSED_BLEND();
			fdMapGridCopyCellsTo(71, 64, 50, 102, 1, 1);
		}
	}
	else if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE) {
		for(i = 0; i != 3; i++) {
			ACTOR *actor = fdActorGetById(i + ACTOR_SUNKEN_PILLAR_A);

			fdActorSelectAnimation(actor, 4);
			actor->motionFlg = 0;
			actor->y.fixed = actor->groundHeight = -toFIXED(PILLAR_SINK_DEPTH);
		}
		if(fdEventGetFlag(FLAG_LEDGE_PILLAR_PUSHED)) {
			fdActorSetAnimation(ACTOR_LEDGE_PILLAR, 4);
			fdActorSetPos(ACTOR_LEDGE_PILLAR, 696, 648);
		}
		fdActorPlaceBlocks(MagmaRockInside_EmberStonePillars);
		fdEffectStartBlankOverlay(0);
		if(!fdEventGetFlag(FLAG_WEST_TORCH_LIT))
			fdActorSetPos(ACTOR_WEST_TORCH_FLAME, 0, 0);
		if(!fdEventGetFlag(FLAG_EAST_TORCH_LIT))
			fdActorSetPos(ACTOR_EAST_TORCH_FLAME, 0, 0);
		if(fdEventGetFlag(FLAG_WEST_TORCH_LIT) &&
		   fdEventGetFlag(FLAG_EAST_TORCH_LIT)) {
			Lower(ACTOR_BLOCKING_PILLAR, -toFIXED(PILLAR_SINK_DEPTH));
			fdActorSetAnimation(ACTOR_BLOCKING_PILLAR, 4);
		}
		else {
			scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 38, 18,
								   CELL_BEHAVIOR_WALL << 8);
		}
		if(fdEventGetFlag(FLAG_MAGMA_BALL_TAKEN))
			fdActorSetAnimation(ACTOR_EMBER_STONE, 0);
		for(i = 0; i != 4; i++)
			fdActorSetAnimation(i + ACTOR_FALLEN_ROCK_A, 2);
		for(i = 0; i != ACTOR_LEDGE_PILLAR - ACTOR_EMBER_STONE; i++)
			fdActorSetSprFlags(fdActorGetById(i + ACTOR_EMBER_STONE), 0);
		if(!fdEventGetFlag(FLAG_ROCKS_RELEASED)) {
			for(i = 0; i != 9; i++)
				fdActorSetPos(i + ACTOR_EMBER_STONE, 0, 0);
		}
	}
	else if(MainRoutineStateWork.next == SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET) {
		scMagmaRockInsideStartStepCells();
		fdEventSetFlag(FLAG_RETREAT_BLOCKED);
		for(i = 0; i != 6; i++) {
			if(fdEventGetFlag(FLAG_MARKER_LIT(i + 2))) {
				s32 y = 24 - i * 2;

				fdMapGridCopyCellsTo(65, 1, 80, y, 1, 2);
				scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 16, y,
									   CELL_BEHAVIOR_WALL << 8);
			}
		}
		fdActorSetSprFlags(fdActorGetById(ACTOR_SOURCE_FLAME), 0);
		fdActorSetSprFlags(fdActorGetById(ACTOR_TORCH_FLAME), 0);
		if(fdEventGetFlag(FLAG_TABLET_TORCH_LIT)) {
			Lower(ACTOR_TORCH_PILLAR, -toFIXED(PILLAR_SINK_DEPTH));
			fdActorSetAnimation(ACTOR_TORCH_PILLAR, 4);
		}
		else {
			scMapCellSetAttributes(MAPGRID_LAYER_GROUND, 7, 21,
								   CELL_BEHAVIOR_WALL << 8);
			fdActorSetPos(ACTOR_TORCH_FLAME, 0, 0);
		}
		Lower(ACTOR_BLAZE_TABLET, toFIXED(16));
		fdActorSetMovementType(fdActorGetById(ACTOR_BLAZE_TABLET),
							   MOVEMENT_TYPE_TAKE_LEADER_LAYER);
	}
	return 0;
}

s32 scPrepareMap(void) {
	s32 scene = MainRoutineStateWork.next;
	s32 entrance = MainRoutineStateWork.entrance;

	if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		if((scene == SCENE_MAGMA_ROCK_INSIDE_ENTRANCE &&
			entrance == MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_STATUE) ||
		   (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE &&
			entrance == MAGMA_ROCK_INSIDE_EMBER_STONE_FROM_MAGMA_ROCK_SLIDE)) {
			fdEventClearFlag(FLAG_ENTRANCE_FLOODED);
			fdEventClearFlag(FLAG_FURY_FLOODED);
			fdEventClearFlag(FLAG_LOWER_ROOMS_FLOODED);
		}
	}
	return 0;
}

/* On entering, sets each listed pillar from its two flags, then lifts the */
/* leader onto the ground under it. */
#include "shared/pillar_restore_raised.h"

#include "shared/leader_check_crumbling_step.h"

/* Unused: a crumbling pillar named in the (actor, flag) table gives way with a
 */
/* shake, rubble and a thump, as on Magma Rock's outside. */
UNUSED void scMagmaRockInsideCrumblePillar(const void *data, s32 id) {
	const TRAP_ENTRY *entry = data;
	ACTOR *actor = fdActorGet(id);
	s32 flag;

	while(entry->actor != ACTOR_LIST_END) {
		if(entry->actor != id) {
			entry++;
			continue;
		}
		flag = entry->flg;
		fdEventStart();
		if(!fdEventGetFlag(flag + 1)) {
			fdEventPlaySnd(SE_PUSH);
			fdMapGridSetShake(1, 4, 1);
			scDustScatter(0);
			syWait(8);
			fdActorSelectAnimation(actor, 2);
			fdActorSetCellShape(-4, actor->layer, id);
			scDustScatter(1);
			syWait(16);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			syWait(8);
			scFieldActorSetPosAndResetMotion(actor, 0, 0, 0);
			syWait(2);
			fdEventPlaySnd(SE_THUMP);
			scMagmaRockInsideSpawnDustRing();
			syWait(20);
			fdEventSetFlag(flag);
			fdEventSetFlag(flag + 1);
		}
		fdEventEnd();
		return;
	}
}

#include "shared/pillar_take_out_of_play.c"

#include "shared/pillar_restore.h"

#include "shared/actor_take_place_of.h"

/* Unused: stepping onto a crumbling floor, as on Magma Rock's outside: the */
/* first time it shakes and cracks, the second time it gives way with the */
/* leader. */
UNUSED void scMagmaRockInsideStepOnTrap(const u16 *table) {
	ACTOR *leader;
	ACTOR *partner;
	ACTOR *trap;
	FIXED position[3];
	s32 id;
	s32 flag;
	s32 second;

	if(fdTryJumpForward() != 0)
		return;
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	partner = fdActorGetById(ACTOR_FIRST_PLACED);
	position[0] = leader->x.fixed;
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed;
	trap = fdActorFindAtCell(position, 0);
	if(trap == NULL)
		return;
	while(*table != ACTOR_LIST_END) {
		id = table[0];
		if(fdActorGetById(id) != trap) {
			table += 2;
			continue;
		}
		{
			flag = table[1];
			fdEventStart();
			if(!fdEventGetFlag(flag)) {
				fdActorSelectAnimation(partner, 1);
				fdActorTakePlaceOf(partner, trap);
				sySynch(1);
				fdEventPlaySnd(SE_CHEST_CLOSE);
				scFieldActorSetSprCol(fdActorGetById(ACTOR_FIRST_PLACED),
									  SPRITE_COLOR_WHITE);
				sySynch(2);
				fdActorSelectAnimation(partner, 0);
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
				fdActorTakePlaceOf(trap, partner);
				sySynch(1);
				fdEventSetFlag(flag);
			}
			else {
				second = flag + 1;
				if(!fdEventGetFlag(second)) {
					fdActorSelectAnimation(partner, 0);
					fdActorTakePlaceOf(partner, trap);
					sySynch(1);
					leader->motionFlg = 0;
					fdEventPlaySnd(SE_PUSH);
					fdMapGridSetShake(1, 4, 1);
					scDustScatter(0);
					sySynch(8);
					fdActorSelectAnimation(partner, 2);
					fdLeaderSetCellShapeAt(-2, trap->layer);
					scDustScatter(1);
					sySynch(16);
					fdLeaderSetCellShapeAt(-2, trap->layer);
					sySynch(4);
					fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
					sySynch(8);
					leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
					sySynch(5);
					fdActorSetPosAndResetMotion(partner, 0, 0, 0);
					sySynch(2);
					fdEventPlaySnd(SE_THUMP);
					scMagmaRockInsideSpawnDustRing();
					sySynch(20);
					fdEventSetFlag(second);
				}
			}
			fdEventEnd();
			return;
		}
	}
}

#include "../field/field_effect_advance_motion_priority.c"

#undef CLOSED_BLEND
/* The falling rocks and rubble, then the rooms' tables. */

#include "shared/dust_scatter.c"

/* Throws a ring of 16 dust clouds out from the leader as a pillar lands. */
void scMagmaRockInsideSpawnDustRing(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT *effect;
	u32 frame;
	u32 i;

	if(leader == NULL)
		return;
	frame = 0;
	for(i = 0; i <= DUST_RING_PUFFS; i++) {
		effect = (FIELD_EFFECT *)scFieldActorCreate(
			SPRITE_DUST_CLOUD, leader->x.fixed, leader->y.fixed,
			leader->z.fixed);
		if(effect != NULL) {
			frame = fdSprShareVramCacheEntry(effect->spr, frame);
			effect->motionFlg = 0;
			effect->collisionFlg = 0;
			fdActorSetSprFlags(effect, 0);
			fdActorSelectAnimation(effect, 2);
			fdActorStartScript(effect, Dust_BurstScript);
			effect->vel.x = syCos(i * FACING_STEP);
			effect->vel.y = 0;
			effect->vel.z = syMathSin(i * FACING_STEP);
			effect->vel.x += effect->vel.x >> 1;
			effect->vel.x += syRandomBelow(toFIXED(3) / 4) - toFIXED(3) / 8;
			effect->vel.z += syRandomBelow(toFIXED(3) / 8) - toFIXED(3) / 16;
			effect->spin = syRandomBelow(4096) - TRIG_TURN / 32;
			effect->scaleRateX = 0;
			effect->scaleRateY = 0;
			effect->update = fdEffectAdvanceMotionAtLeaderPriority;
			effect->spr->pri = ((FIELD_SPR *)leader->spr)->pri;
		}
	}
}

#include "shared/actor_set_cell_shape.h"

#include "shared/leader_set_cell_shape.h"

#include "shared/particle_update_falling.h"

/* Spawns count rocks around (x, y, z), each pushed out at a random angle and */
/* thrown up and out along it. */
void scMagmaRockInsideDropRocks(s32 x, s32 y, s32 z, s32 count) {
	ROCK_STATE *state;
	s32 i;

	state = Ram_HeapSlots->menuState;
	for(i = 0; i < count; i++) {
		s32 angle = syRandom();
		ROCK *rock = &state->rocks[state->next];

		rock->tmr = 0;
		rock->pos[0] = x;
		rock->pos[1] = y;
		rock->pos[2] = z;
		syFieldVectorAddPolarOffsetFX((syRandom() << 3) + toFIXED(8), angle,
									  rock->pos);
		rock->vel[0] = 0;
		rock->vel[1] = toFIXED(5);
		rock->vel[2] = 0;
		syFieldVectorAddPolarOffsetFX((syRandom() << 1) + toFIXED(2), angle,
									  rock->vel);
		state->next++;
		state->next &= ROCK_COUNT - 1;
	}
}

/* Starts the falling rocks: loads their picture, prepares 64 small sprites and
 */
/* adds the task that draws them. */
void scMagmaRockInsideStartFallingRocks(void) {
	ROCK_STATE *state = syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(ROCK_STATE));
	s32 entry;
	s32 tile;
	s32 i;
	ROCK *rock;
	EFFECT_SPR *sprite;

	scMagmaRockInsideDecompressTaggedLz(
		syResourceGetTableEntry(RESOURCE_FALLING_ROCK), state);
	entry = syVramCacheFindFreeEntry();
	tile = mnVramLoad(entry, sizeof(state->tiles), state);
	state->tileBase = tile;
	state->resource = entry;
	rock = state->rocks;
	for(i = 0; i < ROCK_COUNT; i++) {
		sprite = &state->sprs[i];
		scMagmaRockInsideInitRockSpr(sprite, 8, 8, OBJ_SIZE_16X16, tile);
		sprite->oam.fullCol = 1;
		sprite->oam.pal = SPRITE_COLOR_NORMAL;
		sprite->oam.pri = 0;
		sprite->depth = OAM_DEPTH_WINDOW;
		rock->tmr = -1;
		rock++;
	}
	state->next = 0;
	syFieldTaskAddFrame(btParticleUpdateFalling, TASK_PASS_FRAME);
}

#include "shared/rock_end_falling.h"

extern u32 FieldEffect_LongScript[];

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

#include "shared/push_leader_steps.c"

#include "shared/magma_rock_ember_scripts.c"

/* The Lucky Medal chest room's lava platform: round a rectangle, for as long */
/* as the lava flows. */
s32 MagmaRockInside_LuckyMedalPlatformPath[] = {
    SC_MOVE(776, 16, 680), SC_SYNC,
    SC_MOVE(744, 16, 680), SC_SYNC,
    SC_MOVE(744, 16, 632), SC_SYNC,
    SC_MOVE(776, 16, 632), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/* Fury's room's west lava platform: round a rectangle. */
s32 MagmaRockInside_FuryWestPlatformPath[] = {
    SC_MOVE(216, 16, 568), SC_SYNC,
    SC_MOVE(216, 16, 584), SC_SYNC,
    SC_MOVE(136, 16, 584), SC_SYNC,
    SC_MOVE(136, 16, 568), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/* Fury's room's east lava platform: east to 760 once, and it stays there. */
s32 MagmaRockInside_FuryEastPlatformPath[] = {
    SC_MOVE(600, 16, 664), SC_SYNC,
    SC_MOVE(728, 16, 664), SC_SYNC,
    SC_MOVE(728, 16, 632), SC_SYNC,
    SC_MOVE(760, 16, 632), SC_SYNC,
    SC_MOVE(760, 16, 664), SC_SYNC,
    SC_END,
};

/* The west lava platform of the cracked pillars' room's upper hall, which */
/* floods with Fury's room: round a rectangle. */
s32 MagmaRockInside_CrackedPillarsWestPlatformPath[] = {
    SC_MOVE(184, 16, 232), SC_SYNC,
    SC_MOVE(280, 16, 232), SC_SYNC,
    SC_MOVE(280, 16, 264), SC_SYNC,
    SC_MOVE(184, 16, 264), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/* The east lava platform of the cracked pillars' room's upper hall, which */
/* floods with Fury's room: round a rectangle. */
s32 MagmaRockInside_CrackedPillarsEastPlatformPath[] = {
    SC_MOVE(472, 16, 296), SC_SYNC,
    SC_MOVE(424, 16, 296), SC_SYNC,
    SC_MOVE(424, 16, 264), SC_SYNC,
    SC_MOVE(472, 16, 264), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/* The cracked pillars' room's lava platforms A and B: one square, B starting */
/* at its third corner (scMagmaRockInsideSetLowerRoomsFlooded). */
s32 MagmaRockInside_CrackedPillarsFirstPairPath[] = {
    SC_MOVE(216, 16, 600), SC_SYNC,
    SC_MOVE(280, 16, 600), SC_SYNC,
    SC_MOVE(280, 16, 664), SC_SYNC,
    SC_MOVE(216, 16, 664), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/* The cracked pillars' room's lava platforms C and D, as A and B. */
s32 MagmaRockInside_CrackedPillarsSecondPairPath[] = {
    SC_MOVE(312, 16, 664), SC_SYNC,
    SC_MOVE(376, 16, 664), SC_SYNC,
    SC_MOVE(376, 16, 600), SC_SYNC,
    SC_MOVE(312, 16, 600), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/* The cracked pillars' room's lava platforms E and F, as A and B. */
s32 MagmaRockInside_CrackedPillarsThirdPairPath[] = {
    SC_MOVE(440, 16, 600), SC_SYNC,
    SC_MOVE(504, 16, 600), SC_SYNC,
    SC_MOVE(504, 16, 664), SC_SYNC,
    SC_MOVE(440, 16, 664), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/* The cracked pillars' room's lava platforms G and H, as A and B. */
s32 MagmaRockInside_CrackedPillarsFourthPairPath[] = {
    SC_MOVE(536, 16, 664), SC_SYNC,
    SC_MOVE(600, 16, 664), SC_SYNC,
    SC_MOVE(600, 16, 600), SC_SYNC,
    SC_MOVE(536, 16, 600), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/* The Salamander Tail's room's north lava platform: round a rectangle. */
s32 MagmaRockInside_SalamanderTailNorthPlatformPath[] = {
    SC_MOVE(376, 16, 264), SC_SYNC,
    SC_MOVE(376, 16, 232), SC_SYNC,
    SC_MOVE(408, 16, 232), SC_SYNC,
    SC_MOVE(408, 16, 264), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/* The Salamander Tail's room's southwest lava platform: round a rectangle. */
s32 MagmaRockInside_SalamanderTailSouthwestPlatformPath[] = {
    SC_MOVE(216, 16, 680), SC_SYNC,
    SC_MOVE(280, 16, 680), SC_SYNC,
    SC_MOVE(280, 16, 712), SC_SYNC,
    SC_MOVE(216, 16, 712), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/* The Salamander Tail's room's southeast lava platform: round a rectangle. */
s32 MagmaRockInside_SalamanderTailSoutheastPlatformPath[] = {
    SC_MOVE(376, 16, 680), SC_SYNC,
    SC_MOVE(312, 16, 680), SC_SYNC,
    SC_MOVE(312, 16, 712), SC_SYNC,
    SC_MOVE(376, 16, 712), SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/* The pillars Move pushes in the entrance, ended by -1. */
s16 MagmaRockInside_EntrancePillars[] = {
    ACTOR_ENTRANCE_PILLAR, -1,
};

/* The statue Burst wakes in the entrance hall and in Fury's room, as (actor, */
/* the cell Burst is cast at, unused) rows ended by -1. */
ALIGNED(4) s16 MagmaRockInside_Statues[] = {
    ACTOR_STATUE, CELL_STATUE, -1, -1,
};

/* The pillars Move pushes in Fury's room, ended by -1. */
s16 MagmaRockInside_FuryPillars[] = {
    ACTOR_FURY_PILLAR, -1,
};

/* The cracked pillars' room's three statues, as (actor, the cell Burst is cast */
/* at, unused) rows ended by -1 (syStatueFindUnderBurst). */
s16 MagmaRockInside_CrackedPillarsStatues[] = {
    ACTOR_STATUE, CELL_STATUE, -1,
    ACTOR_LOWER_WEST_STATUE, CELL_LOWER_WEST_STATUE, -1,
    ACTOR_LOWER_EAST_STATUE, CELL_LOWER_EAST_STATUE, -1,
    -1,
};

/* The pillars Move pushes in the cracked pillars' room, ended by -1. */
s16 MagmaRockInside_CrackedPillarsPillars[] = {
    ACTOR_PILLAR_A, ACTOR_PILLAR_B, ACTOR_PILLAR_C, -1,
};

/* The pillars Move pushes in the Salamander Tail's chest room, ended by -1. */
s16 MagmaRockInside_SalamanderTailChestPillars[] = {
    ACTOR_NORTH_PILLAR, ACTOR_EAST_PILLAR, -1,
};

/* The pillars Move pushes in the ember stone's room, ended by -1. */
s16 MagmaRockInside_EmberStonePillars[] = {
    ACTOR_LEDGE_PILLAR, -1, 0,
};

/* A small spark's actor script (scMagmaRockInsideSpawnSpark). */
s32 MagmaRockInside_FlameScript[] = {
    SC_WAIT(6),
    SC_RELEASE,
    SC_END,
};

/* A spark shooting up from the Blaze tablet as it lifts: its actor script. */
s32 MagmaRockInside_RisingSparkScript[] = {
    SC_MOVE_BY(0, 160, 0),
    SC_SYNC,
    SC_RELEASE,
    SC_END,
};

/* A circling spark's actor script (scMagmaRockInsideOrbitParent). */
s32 MagmaRockInside_CirclingSparkScript[] = {
    SC_CALL(scMagmaRockInsideOrbitParent),
    SC_RELEASE,
    SC_END,
};

s32 scMagmaRockInsideDriftForSteps(ORBIT *object);

/* A rising spark's actor script: it circles and drifts for its steps */
/* (scMagmaRockInsideDriftForSteps). */
s32 MagmaRockInside_OrbitScript[] = {
    SC_CALL(syOrbitUpdate),
    SC_CALL(fdActorAimAtWatched),
    SC_CALL(scMagmaRockInsideDriftForSteps),
    SC_RELEASE,
    SC_END,
};

#include "shared/lesson_scripts.c"

#include "shared/dust_scatter_script.c"

#include "shared/dust_burst_script.c"

const s32 *FieldEffect_DurationScripts[] = {
    FieldEffect_ShortScript,
    FieldEffect_MediumScript,
    FieldEffect_LongScript,
};

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
    SCENE_EXITS(MagmaRockInsideLuckyMedalChest),
    SCENE_EXIT(1, MagmaRockInsideLuckyMedalChest, 4),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_A, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_B, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_B),
    SCENE_EXIT(4, MagmaRockInsideLuckyMedalChest, 1),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_A, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_B, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_C, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_C),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_FURY, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST),
    SCENE_EXITS(MagmaRockInsideEntrance),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_A, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_A, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_B, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_C, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_C),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_B, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_B),
    SCENE_EXIT(6, MagmaRockInsideEntrance, 7),
    SCENE_EXIT(7, MagmaRockInsideEntrance, 6),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_STATUE, MagmaRockStatue, MAGMA_ROCK_STATUE_FROM_MAGMA_ROCK_INSIDE_ENTRANCE),
    SCENE_EXITS(MagmaRockInsideFury),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST, MagmaRockInsideLuckyMedalChest, MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_FROM_MAGMA_ROCK_INSIDE_FURY),
    SCENE_EXIT(2, MagmaRockInsideFury, 6),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHWEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_NORTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_A, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_B, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_B),
    SCENE_EXIT(6, MagmaRockInsideFury, 2),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_WEST),
    SCENE_EXIT(8, MagmaRockInsideFury, 9),
    SCENE_EXIT(9, MagmaRockInsideFury, 8),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_SOUTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTH, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_FURY_SOUTH),
    SCENE_EXITS(MagmaRockInsideCrackedPillars),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHWEST, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_A, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_B, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_B),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_WEST, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_SOUTHWEST, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_SOUTH, MagmaRockInsideFury, MAGMA_ROCK_INSIDE_FURY_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTH),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTH, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTH),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_A, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_B, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_WEST, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_WEST, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTHWEST, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_A, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_B, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B),
    SCENE_EXITS(MagmaRockInsideGolemCoreChest),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_WEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTHWEST, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_NORTHWEST),
    SCENE_EXIT(3, MagmaRockInsideGolemCoreChest, 9),
    SCENE_EXIT(4, MagmaRockInsideGolemCoreChest, 10),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTH, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_SOUTH),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_B),
    SCENE_EXIT(9, MagmaRockInsideGolemCoreChest, 3),
    SCENE_EXIT(10, MagmaRockInsideGolemCoreChest, 4),
    SCENE_EXITS(MagmaRockInsideSalamanderTailChest),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_WEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_NORTHWEST, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTHWEST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_A),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTH, MagmaRockInsideCrackedPillars, MAGMA_ROCK_INSIDE_CRACKED_PILLARS_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTH),
    SCENE_EXIT(8, MagmaRockInsideSalamanderTailChest, 14),
    SCENE_EXIT(9, MagmaRockInsideSalamanderTailChest, 16),
    SCENE_EXIT(10, MagmaRockInsideSalamanderTailChest, 17),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_SOUTH, MagmaRockInsideGolemCoreChest, MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTH),
    SCENE_EXIT(12, MagmaRockInsideSalamanderTailChest, 18),
    SCENE_EXIT(13, MagmaRockInsideSalamanderTailChest, 19),
    SCENE_EXIT(14, MagmaRockInsideSalamanderTailChest, 8),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_EMBER_STONE, MagmaRockInsideEmberStone, MAGMA_ROCK_INSIDE_EMBER_STONE_FROM_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST),
    SCENE_EXIT(16, MagmaRockInsideSalamanderTailChest, 9),
    SCENE_EXIT(17, MagmaRockInsideSalamanderTailChest, 10),
    SCENE_EXIT(18, MagmaRockInsideSalamanderTailChest, 12),
    SCENE_EXIT(19, MagmaRockInsideSalamanderTailChest, 13),
    SCENE_EXITS(MagmaRockInsideEmberStone),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_INSIDE_BLAZE_TABLET, MagmaRockInsideBlazeTablet, MAGMA_ROCK_INSIDE_BLAZE_TABLET_FROM_MAGMA_ROCK_INSIDE_EMBER_STONE),
    SCENE_EXIT(2, MagmaRockInsideEmberStone, 5),
    SCENE_EXIT(3, MagmaRockInsideEmberStone, 6),
    SCENE_EXIT(4, MagmaRockInsideEmberStone, 7),
    SCENE_EXIT(5, MagmaRockInsideEmberStone, 2),
    SCENE_EXIT(6, MagmaRockInsideEmberStone, 3),
    SCENE_EXIT(7, MagmaRockInsideEmberStone, 4),
    SCENE_EXIT(8, MagmaRockInsideEmberStone, 9),
    SCENE_EXIT(9, MagmaRockInsideEmberStone, 8),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST, MagmaRockInsideSalamanderTailChest, MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_INSIDE_EMBER_STONE),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_SLIDE, MagmaRockSlide, MAGMA_ROCK_SLIDE_FROM_MAGMA_ROCK_INSIDE_EMBER_STONE),
    SCENE_EXITS(MagmaRockInsideBlazeTablet),
    SCENE_EXIT(MAGMA_ROCK_INSIDE_BLAZE_TABLET_TO_MAGMA_ROCK_INSIDE_EMBER_STONE, MagmaRockInsideEmberStone, MAGMA_ROCK_INSIDE_EMBER_STONE_FROM_MAGMA_ROCK_INSIDE_BLAZE_TABLET),
    SCENE_EXITS_END,
};

/* The actors placed in the Lucky Medal's room: its one white platform. */
ACTOR_PLACE MagmaRockInside_Placements[] = {
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

/* The actors placed in the entrance. */
ACTOR_PLACE MagmaRockInside_EntrancePlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 6, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 7, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 8, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 9, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FIRE19), MOVEMENT_TYPE_STAND, 296, 0, 504, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/* The actors placed in Fury's room. */
ACTOR_PLACE MagmaRockInside_FuryPlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FURY), MOVEMENT_TYPE_STAND, 456, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/* The actors placed in the cracked pillars' room. */
ACTOR_PLACE MagmaRockInside_CrackedPillarsPlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, FLAG_INSIDE_CRACKED_PILLAR_A_BURST, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, FLAG_INSIDE_CRACKED_PILLAR_B_BURST, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 6, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 7, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 8, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 9, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 10, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 11, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 12, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 13, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 14, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 15, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 16, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 17, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 18, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ORANGE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 19, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

/* The actors placed in the Golem Core's chest room. */
ACTOR_PLACE MagmaRockInside_GolemCoreChestPlacements[] = {
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The actors placed in the Salamander Tail's chest room. */
ACTOR_PLACE MagmaRockInside_SalamanderTailChestPlacements[] = {
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

/* The actors placed in the ember stone's room. */
ACTOR_PLACE MagmaRockInside_EmberStonePlacements[] = {
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_EMBER_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 12, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 13, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 14, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 15, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 16, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

/* The actors placed in the Blaze tablet's room. */
ACTOR_PLACE MagmaRockInside_BlazeTabletPlacements[] = {
    {SPRITE_SLATE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
/* clang-format on */

/* The triggers of the Lucky Medal's room. */
TRIGGER MagmaRockInside_Triggers[] = {
	{TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_A,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_A},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_B,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_WEST_B},
	{TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_A,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_A},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_B,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_B},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_C,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_ENTRANCE_EAST_C},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
	 MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_FURY,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_TO_MAGMA_ROCK_INSIDE_FURY},
	{STEP_TRIGGER_WALKING, CELL_LAVA_EDGE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideLeapOverLava)},
	{STEP_TRIGGER_WALKING, CELL_LAVA_EDGE_EAST, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideLeapOverLava)},
	{TRIGGER_TABLE_END},
};

/* The triggers of the entrance. */
TRIGGER MagmaRockInside_EntranceTriggers[] = {
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_A,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_A},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_A,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_A},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_B,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_B},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_C,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_EAST_C},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_B,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST_WEST_B},
	{TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
	{TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
	{TRIGGER_EXIT, MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_STATUE,
	 CONDITION_ALWAYS, MAGMA_ROCK_INSIDE_ENTRANCE_TO_MAGMA_ROCK_STATUE},
	{TRIGGER_STEP, CELL_FLOOR_SWITCH, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideDrainLava)},
	{TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_FIRE19, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideMeetDjinni)},
	{TRIGGER_PUSH, 0, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideStartPushingPillar)},
	{TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_ENTRANCE_PILLAR, FLAG_ENTRANCE_PILLAR_PUSHED,
	 EVENT(scMagmaRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ENTRANCE_PILLAR,
	 FLAG_ENTRANCE_PILLAR_PUSHED, EVENT(scMagmaRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), CELL_STATUE,
	 CONDITION_ALWAYS, EVENT(scMagmaRockInsideResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), CELL_STATUE,
	 CONDITION_ALWAYS, EVENT(scMagmaRockInsideTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_STATUE, CONDITION_ALWAYS, EVENT(scMagmaRockInsideFireStatue)},
	{TRIGGER_TABLE_END},
};

/* The triggers of Fury's room. */
TRIGGER MagmaRockInside_FuryTriggers[] = {
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
	 MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST},
	{TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHWEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHWEST},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_A,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_A},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_B,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTHEAST_B},
	{TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST},
	{TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
	{TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTH,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_FURY_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTH},
	{TRIGGER_STEP, CELL_FLOOR_SWITCH, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideDrainLava)},
	{STEP_TRIGGER_WALKING, CELL_LAVA_EDGE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideLeapOverLava)},
	{TRIGGER_TALK, ACTOR_FURY, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideFightFury)},
	{TRIGGER_PUSH, 0, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideStartPushingPillar)},
	{TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_FURY_PILLAR, FLAG_FURY_PILLAR_PUSHED,
	 EVENT(scMagmaRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FURY_PILLAR,
	 FLAG_FURY_PILLAR_PUSHED, EVENT(scMagmaRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), CELL_STATUE,
	 CONDITION_ALWAYS, EVENT(scMagmaRockInsideResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), CELL_STATUE,
	 CONDITION_ALWAYS, EVENT(scMagmaRockInsideTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_STATUE, CONDITION_ALWAYS, EVENT(scMagmaRockInsideFireStatue)},
	{TRIGGER_TABLE_END},
};

/* The triggers of the cracked pillars' room. */
TRIGGER MagmaRockInside_CrackedPillarsTriggers[] = {
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHWEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHWEST},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_A,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_A},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_B,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_NORTHEAST_B},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_WEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_WEST},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_SOUTHWEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_SOUTHWEST},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_SOUTH,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_FURY_SOUTH},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTH,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTH},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_A,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_A},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_B,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST_B},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_WEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_WEST},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_WEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_WEST},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTHWEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTHWEST},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_A,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_A},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_B,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_CRACKED_PILLARS_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST_B},
	{STEP_TRIGGER_WALKING, CELL_LAVA_EDGE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideLeapOverLava)},
	{STEP_TRIGGER_WALKING, CELL_CRACKED_PILLAR_A,
	 IF_FLAG_SET(FLAG_FURY_FLOODED), EVENT(scMagmaRockInsideLeapOverLava)},
	{STEP_TRIGGER_WALKING, CELL_CRACKED_PILLAR_B, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideLeapOverLava)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_LAVA_EDGE_NORTH,
	 CONDITION_ALWAYS, EVENT(scMagmaRockInsideLeapOverLava)},
	{STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_LAVA_EDGE_WEST,
	 CONDITION_ALWAYS, EVENT(scMagmaRockInsideLeapOverLava)},
	{TRIGGER_PUSH, 0, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideStartPushingPillar)},
	{TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_PILLAR_A, FLAG_PILLAR_A_PUSHED,
	 EVENT(scMagmaRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_A, FLAG_PILLAR_A_PUSHED,
	 EVENT(scMagmaRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_PILLAR_B, FLAG_PILLAR_B_PUSHED,
	 EVENT(scMagmaRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_B, FLAG_PILLAR_B_PUSHED,
	 EVENT(scMagmaRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_PILLAR_C, FLAG_PILLAR_C_PUSHED,
	 EVENT(scMagmaRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_C, FLAG_PILLAR_C_PUSHED,
	 EVENT(scMagmaRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_CRACKED_PILLAR_A, FLAG_INSIDE_CRACKED_PILLAR_A_BURST,
	 EVENT(MagmaRockInside_BurstCrackedPillarA)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_CRACKED_PILLAR_B, FLAG_INSIDE_CRACKED_PILLAR_B_BURST,
	 EVENT(MagmaRockInside_BurstCrackedPillarB)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), CELL_STATUE,
	 CONDITION_ALWAYS, EVENT(scMagmaRockInsideResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), CELL_STATUE,
	 FLAG_FIRE_BURST_PLAYING, EVENT(scMagmaRockInsideTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), CELL_STATUE,
	 CONDITION_ALWAYS, EVENT(MagmaRockInside_BurstCrackedPillarA)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_STATUE, CONDITION_ALWAYS, EVENT(scMagmaRockInsideFireStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
	 CELL_LOWER_WEST_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_LOWER_WEST_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_LOWER_WEST_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideFireStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
	 CELL_LOWER_EAST_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideResolveBeforeBurst)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
	 CELL_LOWER_EAST_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideTargetStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
	 CELL_LOWER_EAST_STATUE, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideFireStatue)},
	{TRIGGER_TABLE_END},
};

/* The triggers of the Golem Core's chest room. */
TRIGGER MagmaRockInside_GolemCoreChestTriggers[] = {
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTHWEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_NORTHWEST},
	{TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
	{TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTH,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_SOUTH},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_EAST},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B},
	{TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
	{TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
	{TRIGGER_STEP, CELL_FLOOR_SWITCH, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideDrainLava)},
	{TRIGGER_STEP, CELL_SECOND_FLOOR_SWITCH, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideDrainLava)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_HANGING_CHAIN_A,
	 CONDITION_ALWAYS, 0},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_HANGING_CHAIN_B,
	 CONDITION_ALWAYS, 0},
	{TRIGGER_TABLE_END},
};

/* The triggers of the Salamander Tail's chest room. */
TRIGGER MagmaRockInside_SalamanderTailChestTriggers[] = {
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_WEST},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_SOUTHWEST},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_NORTHWEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_NORTHWEST},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_EAST},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_A},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_EAST_B},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTH,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_CRACKED_PILLARS_NORTH},
	{TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
	{TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
	{TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_SOUTH,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST_SOUTH},
	{TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
	{TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
	{TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_EMBER_STONE,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_INSIDE_EMBER_STONE},
	{TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
	{TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
	{TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
	{TRIGGER_EXIT, 19, CONDITION_ALWAYS, 19},
	{STEP_TRIGGER_WALKING, CELL_LAVA_EDGE,
	 IF_FLAG_SET(FLAG_LOWER_ROOMS_FLOODED),
	 EVENT(scMagmaRockInsideLeapOverLava)},
	{TRIGGER_PUSH, 0, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideStartPushingPillar)},
	{TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_NORTH_PILLAR, FLAG_NORTH_PILLAR_PUSHED,
	 EVENT(scMagmaRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_NORTH_PILLAR,
	 FLAG_NORTH_PILLAR_PUSHED, EVENT(scMagmaRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_EAST_PILLAR, FLAG_EAST_PILLAR_PUSHED,
	 EVENT(scMagmaRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR,
	 FLAG_EAST_PILLAR_PUSHED, EVENT(scMagmaRockInsideFinishPushingPillar)},
	{TRIGGER_TABLE_END},
};

/* The triggers of the ember stone's room. */
TRIGGER MagmaRockInside_EmberStoneTriggers[] = {
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_INSIDE_BLAZE_TABLET,
	 CONDITION_ALWAYS, EVENT(scMagmaRockInsideOpenStairs)},
	{TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
	{TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
	{TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
	{TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
	{TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
	{TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
	{TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
	{TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
	 MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_INSIDE_SALAMANDER_TAIL_CHEST},
	{TRIGGER_EXIT, MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_SLIDE,
	 CONDITION_ALWAYS, MAGMA_ROCK_INSIDE_EMBER_STONE_TO_MAGMA_ROCK_SLIDE},
	{TRIGGER_STEP, CELL_ERUPTION_ISLET, FLAG_ROCKS_RELEASED,
	 EVENT(scMagmaRockInsideReleaseRocks)},
	{TRIGGER_EXAMINE, CELL_EMBER_STONE, FLAG_MAGMA_BALL_TAKEN,
	 EVENT(scMagmaRockInsideExamineEmberStone)},
	{TRIGGER_PUSH, 0, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideStartPushingPillar)},
	{TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_LEDGE_PILLAR, FLAG_LEDGE_PILLAR_PUSHED,
	 EVENT(scMagmaRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEDGE_PILLAR,
	 FLAG_LEDGE_PILLAR_PUSHED, EVENT(scMagmaRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_FALLEN_FLAME_A, CONDITION_ALWAYS, EVENT(scMagmaRockInsideTakeFlame)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_FALLEN_FLAME_B, CONDITION_ALWAYS, EVENT(scMagmaRockInsideTakeFlame)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_FALLEN_FLAME_C, CONDITION_ALWAYS, EVENT(scMagmaRockInsideTakeFlame)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_FALLEN_FLAME_D, CONDITION_ALWAYS, EVENT(scMagmaRockInsideTakeFlame)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_WEST_TORCH_FLAME, CONDITION_ALWAYS, 0},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_EAST_TORCH_FLAME, CONDITION_ALWAYS,
	 EVENT(MagmaRockInside_LightTorch)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
	 ACTOR_EAST_TORCH_FLAME, FLAG_EAST_TORCH_LIT,
	 EVENT(scMagmaRockInsideGrowFlame)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
	 ACTOR_WEST_TORCH_FLAME, FLAG_WEST_TORCH_LIT,
	 EVENT(scMagmaRockInsideGrowFlame)},
	{PSYNERGY_ON_ACTOR(ABILITY_BLAZE), ACTOR_FALLEN_FLAME_A, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsidePutOutFlames)},
	{PSYNERGY_ON_ACTOR(ABILITY_BLAZE), ACTOR_FALLEN_FLAME_B, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsidePutOutFlames)},
	{PSYNERGY_ON_ACTOR(ABILITY_BLAZE), ACTOR_FALLEN_FLAME_C, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsidePutOutFlames)},
	{PSYNERGY_ON_ACTOR(ABILITY_BLAZE), ACTOR_FALLEN_FLAME_D, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsidePutOutFlames)},
	{PSYNERGY_ON_ACTOR(ABILITY_BLAZE), ACTOR_EAST_TORCH_FLAME, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsidePutOutFlames)},
	{TRIGGER_TABLE_END},
};

/* The cell the leader last stopped on, x in the high half and z in the low, or
 */
/* -1 for none (scMagmaRockInsideTrackStepCell). */
s32 MagmaRockInside_LastStepCell = -1;

/* The triggers of the Blaze tablet's room. */
TRIGGER MagmaRockInside_BlazeTabletTriggers[] = {
	{TRIGGER_EXIT,
	 MAGMA_ROCK_INSIDE_BLAZE_TABLET_TO_MAGMA_ROCK_INSIDE_EMBER_STONE,
	 CONDITION_ALWAYS,
	 MAGMA_ROCK_INSIDE_BLAZE_TABLET_TO_MAGMA_ROCK_INSIDE_EMBER_STONE},
	{TRIGGER_STEP, CELL_STEPPING_STONE_A, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideStampStepCell)},
	{TRIGGER_STEP, CELL_STEPPING_STONE_B, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideStampOtherStepCell)},
	{EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BLAZE_TABLET, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideReadBlazeTablet)},
	{EXAMINE_TRIGGER_FACING(FACING_SOUTH), CELL_BLAZE_TABLET, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideReadBlazeTabletFromNorth)},
	{EXAMINE_TRIGGER_FACING(FACING_WEST), CELL_BLAZE_TABLET, CONDITION_ALWAYS,
	 EVENT(MagmaRockInside_ReadBlazeTabletFromEast)},
	{EXAMINE_TRIGGER_FACING(FACING_EAST), CELL_BLAZE_TABLET, CONDITION_ALWAYS,
	 EVENT(MagmaRockInside_ReadBlazeTabletFromWest)},
	{TRIGGER_RAISED, STEP_CELL_TRIGGER_BASE + 2, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideStepOnMarker)},
	{TRIGGER_RAISED, STEP_CELL_TRIGGER_BASE + 3, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideStepOnMarker)},
	{TRIGGER_RAISED, STEP_CELL_TRIGGER_BASE + 4, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideStepOnMarker)},
	{TRIGGER_RAISED, STEP_CELL_TRIGGER_BASE + 5, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideStepOnMarker)},
	{TRIGGER_RAISED, STEP_CELL_TRIGGER_BASE + 6, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsideStepOnMarker)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_SOURCE_FLAME, CONDITION_ALWAYS,
	 EVENT(MagmaRockInside_LightTabletTorch)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_TORCH_FLAME, CONDITION_ALWAYS, 0},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
	 ACTOR_TORCH_FLAME, FLAG_TABLET_TORCH_LIT,
	 EVENT(scMagmaRockInsideGrowTabletFlame)},
	{PSYNERGY_ON_ACTOR(ABILITY_BLAZE), ACTOR_SOURCE_FLAME, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsidePutOutTabletFlame)},
	{PSYNERGY_ON_ACTOR(ABILITY_BLAZE), ACTOR_TORCH_FLAME, CONDITION_ALWAYS,
	 EVENT(scMagmaRockInsidePutOutTabletFlame)},
	{TRIGGER_TABLE_END},
};

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY
#undef DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR
#undef DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH
#undef DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM
