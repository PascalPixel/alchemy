/* The Dehkan Plateau, the cliffs and caves Felix crosses after Daila on his way
   south to Madra, in six scenes that share this overlay, each of several
   screens.

   Its ground is cracked: the first step on a crack cracks it open, and the next
   drops Felix through to the caves below (scDehkanPlateauStepOnCrack), from
   which stairs lead back up. Gray pillars stand in the gaps; each holds Felix
   once and crumbles at his next step (scCrumblingPillarStep). Stone pillars are
   Moved or pushed into the gaps to bridge them.

   In story order: the first stretch, where the cracks begin and a chest with
   the Full Metal Vest stands on a ledge reached from the caves; the stretch of
   gray pillars with the Themis Axe's chest, where the Mars Djinni Cannon is
   first seen and runs off over the cracks; the stretch of the rope bridge and
   tightropes, where he Pounds a stake out of his way and cracks two pillars as
   he leaps across them; the caves below the cracks, with the Mint's chest; the
   cave Felix falls into beside Cannon, who drops the Pound Cube in his fright
   (scDehkanPlateauCannonDropsPoundCube); and Cannon's own cave, where he is
   cornered once Felix Pounds the stake in his way into the ground, and fights
   (scDehkanPlateauFightCannon). */

#include "macros.h"
#include "item_ids.h"
#include "field/script_inline.h"
#include "field/field_mode.h"
#include "scene/dehkan_plateau.h"
#include "scene/scene.h"
#include "shared/tundaria_tower.h"
#include "game/game.h"
#include "graphics/glyph.h"
#include "field/actor_script.h"
#include "scene/gabomba_statue.h"

/* Cannon's flight across the plateau, a flag for each stage: over the cracks
   and out east in the Themis Axe's stretch, then along the rope bridge, past
   the stake he Pounds and over the two pillars he cracks. */
#define FLAG_CANNON_FLED_OVER_CRACKS		 0x86c
#define FLAG_CANNON_FLED_EAST				 0x86d
#define FLAG_CANNON_LEAPT_AWAY				 0x86e
#define FLAG_CANNON_POUNDED_STAKE			 0x86f
#define FLAG_CANNON_CRACKED_PILLARS			 0x870
/* The Pound Cube's cave: Cannon drops the Pound Cube, and the party takes
   it. */
#define FLAG_CANNON_DROPPED_POUND_CUBE		 0x871
#define FLAG_POUND_CUBE_TAKEN				 0x878
/* The pillars Moved into place in the Themis Axe's and rope bridge's
   stretches. */
#define FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED	 0x840
#define FLAG_TIGHTROPES_SECOND_PILLAR_PLACED 0x841

/* Each room's actors, numbering its placement table. A room's gray crumbling
   pillars crack under the leader and crumble at his next step
   (scCrumblingPillarStep), its stakes sink when Pounded (fdPoundSinkTarget),
   and its stone pillars are Moved. Its unseen blocks draw nothing, but while
   they are drawn the leader among them shows in front of the cliff there
   (scDehkanPlateauSetBlocksShown). Chests number from 64. */

/* The first stretch, where cracks drop the party to the caves below. */
typedef enum DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_ACTOR_ID {
	ACTOR_FULL_METAL_VEST_CHEST_FIRST_CRUMBLING_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_FULL_METAL_VEST_CHEST_STAKE = ACTOR_FIRST_PLACED + 3,
	/* The stone pillar Moved over the edge into the gap below. */
	ACTOR_FULL_METAL_VEST_CHEST_PILLAR,
	ACTOR_FULL_METAL_VEST_CHEST_FIRST_BLOCK,
	ACTOR_FULL_METAL_VEST_CHEST = 64
} DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_ACTOR_ID;

/* The Themis Axe's stretch, where Cannon is first seen and flees over the
   cracks. */
typedef enum DEHKAN_PLATEAU_THEMIS_AXE_CHEST_ACTOR_ID {
	ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR = ACTOR_FIRST_PLACED + 3,
	ACTOR_THEMIS_AXE_CHEST_STAKE = ACTOR_FIRST_PLACED + 14,
	/* The stone pillar Moved into the gap. */
	ACTOR_THEMIS_AXE_CHEST_PILLAR,
	ACTOR_THEMIS_AXE_CHEST_CANNON,
	ACTOR_THEMIS_AXE_CHEST_FIRST_BLOCK
} DEHKAN_PLATEAU_THEMIS_AXE_CHEST_ACTOR_ID;

/* The rope bridge's stretch, of tightropes, a rope to Lash and the crumbling
   pillars Cannon leaps across. */
typedef enum DEHKAN_PLATEAU_TIGHTROPES_ACTOR_ID {
	/* The posts at the ends of the two tightropes, and of the rope, whose low
	   post Lash ties it from. */
	ACTOR_TIGHTROPES_FIRST_TIGHTROPE_WEST_POST = ACTOR_FIRST_PLACED,
	ACTOR_TIGHTROPES_FIRST_TIGHTROPE_EAST_POST,
	ACTOR_TIGHTROPES_SECOND_TIGHTROPE_WEST_POST,
	ACTOR_TIGHTROPES_SECOND_TIGHTROPE_EAST_POST,
	ACTOR_TIGHTROPES_ROPE_LOW_POST,
	ACTOR_TIGHTROPES_ROPE_HIGH_POST,
	/* The four stone pillars wrapped in vines, which the leader climbs and
	   pushes or Moves into the gaps. */
	ACTOR_TIGHTROPES_FIRST_VINE_PILLAR,
	ACTOR_TIGHTROPES_SECOND_VINE_PILLAR,
	ACTOR_TIGHTROPES_THIRD_VINE_PILLAR,
	ACTOR_TIGHTROPES_FOURTH_VINE_PILLAR,
	/* The stake Cannon Pounds down as he flees. */
	ACTOR_TIGHTROPES_STAKE,
	/* The stone pillar Moved on the raised ground. */
	ACTOR_TIGHTROPES_PILLAR,
	/* The stone pillar at the foot of the cliff, pushed off the vine behind
	   it. */
	ACTOR_TIGHTROPES_SECOND_PILLAR,
	ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR,
	ACTOR_TIGHTROPES_CANNON = ACTOR_FIRST_PLACED + 17,
	ACTOR_TIGHTROPES_FIRST_BLOCK
} DEHKAN_PLATEAU_TIGHTROPES_ACTOR_ID;

/* The caves below the cracks, with the Mint's chest. */
typedef enum DEHKAN_PLATEAU_MINT_CHEST_ACTOR_ID {
	ACTOR_MINT_CHEST_FIRST_BLOCK = ACTOR_FIRST_PLACED
} DEHKAN_PLATEAU_MINT_CHEST_ACTOR_ID;

/* The Pound Cube's cave, where a fall lands the leader beside Cannon and he
   drops the Pound Cube. */
typedef enum DEHKAN_PLATEAU_POUND_CUBE_ACTOR_ID {
	ACTOR_POUND_CUBE_STAKE = ACTOR_FIRST_PLACED,
	ACTOR_POUND_CUBE_CANNON
} DEHKAN_PLATEAU_POUND_CUBE_ACTOR_ID;

/* Cannon's cave, where Cannon is cornered behind the stake and fights. */
typedef enum DEHKAN_PLATEAU_CANNON_ACTOR_ID {
	ACTOR_CANNON_ROOM_STAKE = ACTOR_FIRST_PLACED,
	ACTOR_CANNON_ROOM_CRUMBLING_PILLAR,
	ACTOR_CANNON_ROOM_FIRST_BLOCK,
	ACTOR_CANNON = ACTOR_FIRST_PLACED + 8
} DEHKAN_PLATEAU_CANNON_ACTOR_ID;

/* The unseen blocks of each room (scActorsSetDrawn, scActorHide). */
enum {
	FULL_METAL_VEST_CHEST_BLOCK_COUNT = 14,
	THEMIS_AXE_CHEST_BLOCK_COUNT = 13,
	TIGHTROPES_BLOCK_COUNT = 10,
	MINT_CHEST_BLOCK_COUNT = 3,
	CANNON_ROOM_BLOCK_COUNT = 6
};

/* For this visit: the room's unseen blocks drawn
   (scDehkanPlateauRestoreBlocks). */
#define FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN		 FLAG_SCENE_TEMP_00
/* The leader stands on the crack he has just opened; set until he steps off. */
#define FLAG_STANDING_ON_OPENED_CRACK			 FLAG_SCENE_TEMP_01
/* Cannon's cave's stake's metatile holds the attributes
   scDehkanPlateauEnterLowerPath gives it while the leader is on the lower
   path. */
#define FLAG_STAKE_METATILE_CHANGED				 FLAG_SCENE_TEMP_02
/* Each room's stake Pounded into the ground, by its list's flag. */
#define FLAG_FULL_METAL_VEST_CHEST_STAKE_POUNDED FLAG_SCENE_TEMP_03
#define FLAG_THEMIS_AXE_CHEST_STAKE_POUNDED		 FLAG_SCENE_TEMP_04
#define FLAG_TIGHTROPES_STAKE_POUNDED			 FLAG_SCENE_TEMP_05
#define FLAG_POUND_CUBE_STAKE_POUNDED			 FLAG_SCENE_TEMP_06
/* Cannon's cave's stake, Pounded into the ground so Cannon is cornered. */
#define FLAG_CANNON_ROOM_STAKE_POUNDED			 FLAG_SCENE_TEMP_07
/* The rope bridge's stretch's rope, tied by Lash from its low post to its high
   one. */
#define FLAG_TIGHTROPES_ROPE_LASHED				 FLAG_SCENE_TEMP_08
/* Cannon met in Cannon's cave on this visit, or already in the party. */
#define FLAG_CANNON_MET							 FLAG_SCENE_TEMP_09
/* The first stretch's stone pillar, Moved into the gap, where it stays. */
#define FLAG_FULL_METAL_VEST_CHEST_PILLAR_IN_GAP FLAG_AREA_TEMP_01
/* A room's crumbling pillars, two flags to a pillar in its list's order: the
   first set once it cracks, the next once it crumbles
   (scCrumblingPillarStep). */
#define FLAG_PILLAR_CRACKED(n)					 (FLAG_SCENE_TEMP_10 + 2 * (n))
#define FLAG_PILLAR_CRUMBLED(n)					 (FLAG_PILLAR_CRACKED(n) + 1)
/* The first of a room's crack flags, one a listed crack, set while the crack is
   open (DEFINE_DEHKAN_PLATEAU_RESTORE_CRACKS). */
#define FLAG_CRACK_OPENED_FIRST					 FLAG_SCENE_TEMP_40
#define FLAG_CRACK_OPENED(n)					 (FLAG_CRACK_OPENED_FIRST + (n))
/* The crack the party fell through, one flag for each landing entrance from
   FIRST_LANDING_ENTRANCE, for the landing drawn below it. */
#define FLAG_FELL_THROUGH_FIRST					 FLAG_AREA_TEMP_20
#define FLAG_FELL_THROUGH_LAST					 FLAG_AREA_TEMP_5F
#define FIRST_LANDING_ENTRANCE					 11
#define LAST_LANDING_ENTRANCE					 39
#define FLAG_FELL_THROUGH(entrance) \
	(FLAG_FELL_THROUGH_FIRST + (entrance) - FIRST_LANDING_ENTRANCE)
/* Entrances from these up land a fall: the Pound Cube room's 36 and 37, and
   Cannon's room's 8 to 23. */
#define POUND_CUBE_LANDINGS_FROM  17
#define CANNON_ROOM_LANDINGS_FROM 6

/* The step code of each room's first crack, its list's first row; the rest
   follow in order, and a fall leaves by the exit of its crack's code. */
#define CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK 11
#define CUE_THEMIS_AXE_CHEST_FIRST_CRACK	  14
#define CUE_TIGHTROPES_FIRST_CRACK			  12
#define CUE_POUND_CUBE_FIRST_CRACK			  18
/* The Pound Cube's cave's crack Cannon Pounds the leader through, its sixth. */
#define CUE_POUND_CUBE_CANNON_CRACK			  (CUE_POUND_CUBE_FIRST_CRACK + 5)
/* The rope bridge's stretch's step code that runs
   scDehkanPlateauStepOnTightropesPillars on walking west, which a vine pillar
   pushed beside the crumbling pillars takes. */
#define CUE_TIGHTROPES_PILLARS_WEST			  11

/* The metatiles that run the rooms' scripts. Each room numbers its own; a
   metatile that leads out of a room has its exit's number instead, and the
   cracks follow from the step codes above. */
typedef enum DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_METATILE_BEHAVIOR {
	/* The foot of the cliff where the unseen blocks start to be drawn,
	   metatiles (34, 45) to (34, 50), and the metatiles beside it where they
	   stop, (35, 45) to (35, 50). */
	METATILE_BEHAVIOR_FULL_METAL_VEST_CHEST_BLOCKS_ON = 6,
	METATILE_BEHAVIOR_FULL_METAL_VEST_CHEST_BLOCKS_OFF,
	/* The three gray pillars, which crack and crumble under Felix: metatiles
	   (37, 46), (41, 46), (36, 49), (40, 49), (41, 50) and (45, 50). */
	METATILE_BEHAVIOR_FULL_METAL_VEST_CHEST_PILLARS = 9,
	/* The ground round the cracks: stepping off one lets it be stepped on
	   again. */
	METATILE_BEHAVIOR_FULL_METAL_VEST_CHEST_OFF_CRACK
} DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_METATILE_BEHAVIOR;

/* The Themis' Axe chest's stretch. */
typedef enum DEHKAN_PLATEAU_THEMIS_AXE_CHEST_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_THEMIS_AXE_CHEST_BLOCKS_ON = 6,
	METATILE_BEHAVIOR_THEMIS_AXE_CHEST_BLOCKS_OFF,
	/* The gray pillars: metatile (15, 17), which runs only as Felix walks west,
	   then the rest. */
	METATILE_BEHAVIOR_THEMIS_AXE_CHEST_PILLAR_FROM_EAST,
	METATILE_BEHAVIOR_THEMIS_AXE_CHEST_PILLARS,
	METATILE_BEHAVIOR_THEMIS_AXE_CHEST_OFF_CRACK,
	/* Where Felix first sees Cannon, in the cracked field, metatiles (39, 13)
	   to (41, 14), and where Cannon runs off east, (56, 13) to (59, 13). */
	METATILE_BEHAVIOR_CANNON_FLEES_OVER_CRACKS = 30,
	METATILE_BEHAVIOR_CANNON_FLEES_EAST
} DEHKAN_PLATEAU_THEMIS_AXE_CHEST_METATILE_BEHAVIOR;

/* The rope bridge's stretch. */
typedef enum DEHKAN_PLATEAU_TIGHTROPES_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_TIGHTROPES_BLOCKS_ON = 7,
	METATILE_BEHAVIOR_TIGHTROPES_BLOCKS_OFF,
	/* East of the second stone pillar, metatile (39, 33), walking west into
	   it. */
	METATILE_BEHAVIOR_TIGHTROPES_SECOND_PILLAR,
	/* The gray pillars, metatiles (39, 17), (43, 17) and (48, 20), and the
	   metatiles that run the same step only as Felix walks east, (44, 10), or
	   south, (46, 8); CUE_TIGHTROPES_PILLARS_WEST is the one for walking
	   west. */
	METATILE_BEHAVIOR_TIGHTROPES_PILLARS,
	METATILE_BEHAVIOR_TIGHTROPES_PILLARS_FROM_WEST = 26,
	METATILE_BEHAVIOR_TIGHTROPES_PILLARS_FROM_NORTH,
	/* Beside a vine pillar, walking east or west into it: west of them,
	   metatiles (16..17, 16), (39..40, 20) and (53..54, 20); east of them,
	   (14..15, 11), (19..20, 16), (42..43, 20) and (56..57, 20). */
	METATILE_BEHAVIOR_VINE_PILLAR_EAST = 20,
	METATILE_BEHAVIOR_VINE_PILLAR_WEST,
	/* Where Cannon leaps away along the rope bridge, metatiles (6, 6) and (7,
	   6); where he Pounds the stake, (23, 6) or (25, 16) to (27, 17); and where
	   he leaps across the gray pillars, (36, 12) to (36, 16), (35, 16) and (35,
	   17). */
	METATILE_BEHAVIOR_CANNON_LEAPS_AWAY = 30,
	METATILE_BEHAVIOR_CANNON_POUNDS_STAKE,
	METATILE_BEHAVIOR_CANNON_CRACKS_PILLARS
} DEHKAN_PLATEAU_TIGHTROPES_METATILE_BEHAVIOR;

/* The Mint chest's stretch. */
typedef enum DEHKAN_PLATEAU_MINT_CHEST_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_MINT_CHEST_BLOCKS_ON = 8,
	METATILE_BEHAVIOR_MINT_CHEST_BLOCKS_OFF
} DEHKAN_PLATEAU_MINT_CHEST_METATILE_BEHAVIOR;

/* The Pound Cube's cave. */
typedef enum DEHKAN_PLATEAU_POUND_CUBE_METATILE_BEHAVIOR {
	/* Where the Pound Cube lies once Cannon has dropped it, metatile (15, 5),
	   copied there from the spare metatile (0, 0). */
	METATILE_BEHAVIOR_POUND_CUBE = 12
} DEHKAN_PLATEAU_POUND_CUBE_METATILE_BEHAVIOR;

/* Cannon's cave. */
typedef enum DEHKAN_PLATEAU_CANNON_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_CANNON_ROOM_BLOCKS_ON = 5,
	METATILE_BEHAVIOR_CANNON_ROOM_BLOCKS_OFF,
	/* The gray pillar. */
	METATILE_BEHAVIOR_CANNON_ROOM_PILLAR = 8,
	/* The lower floor below the ledge, metatiles (43, 16) to (43, 18), and the
	   metatiles off it, (44, 16) to (44, 18) and (52, 15) to (54, 15). */
	METATILE_BEHAVIOR_LOWER_FLOOR,
	METATILE_BEHAVIOR_OFF_LOWER_FLOOR,
	/* The ledge toward Cannon, metatiles (43, 12) and (43, 13). */
	METATILE_BEHAVIOR_LEDGE_TO_CANNON
} DEHKAN_PLATEAU_CANNON_METATILE_BEHAVIOR;

/* The stone stake's two looks in stake.png (fdActorSetMetatileOpen). */
enum { ANIM_STONE_STAKE_STANDING = 3, ANIM_STONE_STAKE_SUNK };

/* Coming in by this entrance, from the Themis Axe's stretch's east screen into
   its west one, the leader is drawn behind the ledge there. */
enum { ENTRANCE_THEMIS_AXE_CHEST_WEST_SCREEN = 5 };

/* The ticks the leader takes to sink through a crack. */
#define FALL_TICKS 40

/* The cracks of each room's list. */
enum {
	FULL_METAL_VEST_CHEST_CRACKS = 14,
	THEMIS_AXE_CHEST_CRACKS = 8,
	TIGHTROPES_CRACKS = 6,
	POUND_CUBE_CRACKS = 16
};

/* The Pound Cube's cave's two cracks that have another just south of them, the
   next in its list; once both are open, the hole drawn below is JOINED_HOLE of
   DehkanPlateauPoundCubeHolePictures in place of its own. */
enum {
	POUND_CUBE_WEST_UPPER_CRACK = 0,
	POUND_CUBE_EAST_UPPER_CRACK = 11,
	JOINED_HOLE = 4
};

extern Sint16 DehkanPlateauFullMetalVestChestCrumblingPillars[];
/* The Pound Cube Cannon drops, while it lies on the ground. */
extern ACTOR *DehkanPlateauPoundCubeWork;
void		  scDehkanPlateauLeaveLowerPath(void);
void		  scDehkanPlateauForgetFall(void);

#include "shared/crumbling_pillar.h"

#include "../field/field_effect_advance_motion_priority.c"

#include "shared/dust.h"

#include "shared/actor_set_metatile_shape.h"

#include "shared/leader_set_metatile_shape.h"

#include "shared/pillar_raise.h"

#include "shared/pillar_lower_held_metatiles.h"

#include "shared/held_actor_raise_metatile.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#undef EFFECT_SET_SCRIPT

extern Uint16 DehkanPlateauFullMetalVestChestCrackPictures[];
extern Uint16 DehkanPlateauFullMetalVestChestCracks[][CRACK_FIELDS];

extern Sint16 DehkanPlateauThemisAxeChestCrumblingPillars[];
extern Sint16 DehkanPlateauTightropesCrumblingPillars[];
extern Sint16 DehkanPlateauCannonCrumblingPillar[];
extern Sint16 DehkanPlateauFullMetalVestChestStakes[];
extern Sint16 DehkanPlateauThemisAxeChestStakes[];
extern Sint16 DehkanPlateauTightropesStakes[];
extern Sint16 DehkanPlateauPoundCubeStakes[];
extern Sint16 DehkanPlateauCannonStakes[];
extern Sint16 DehkanPlateauThemisAxeChestPillars[];
extern Sint16 DehkanPlateauTightropesPillars[];

/* Talking to Cannon in his cave starts the battle with him. */
void scDehkanPlateauFightCannon(void)
{
	syGameFlagSet(FLAG_CANNON_MET);
	fdRequestDjinnBattle(ACTOR_CANNON, DJINNI_CANNON);
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

extern ACTOR_PLACE DehkanPlateauPlacements[];
extern ACTOR_PLACE DehkanPlateauFullMetalVestChestPlacements[];
extern ACTOR_PLACE DehkanPlateauThemisAxeChestPlacements[];
extern ACTOR_PLACE DehkanPlateauTightropesPlacements[];
extern ACTOR_PLACE DehkanPlateauMintChestPlacements[];
extern ACTOR_PLACE DehkanPlateauPoundCubePlacements[];
extern ACTOR_PLACE DehkanPlateauCannonPlacements[];
extern CUE		   DehkanPlateauCues[];
extern CUE		   DehkanPlateauFullMetalVestChestCues[];
extern CUE		   DehkanPlateauThemisAxeChestCues[];
extern CUE		   DehkanPlateauTightropesCues[];
extern CUE		   DehkanPlateauMintChestCues[];
extern CUE		   DehkanPlateauPoundCubeCues[];
extern CUE		   DehkanPlateauCannonCues[];

const Uint8 *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST)
		return DehkanPlateauFullMetalVestChestPlacements;
	if(scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST)
		return DehkanPlateauThemisAxeChestPlacements;
	if(scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES)
		return DehkanPlateauTightropesPlacements;
	if(scene == SCENE_DEHKAN_PLATEAU_MINT_CHEST)
		return DehkanPlateauMintChestPlacements;
	if(scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE)
		return DehkanPlateauPoundCubePlacements;
	if(scene == SCENE_DEHKAN_PLATEAU_CANNON)
		return DehkanPlateauCannonPlacements;
	return DehkanPlateauPlacements;
}

const CUE *scGetCues(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST)
		return DehkanPlateauFullMetalVestChestCues;
	if(scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST)
		return DehkanPlateauThemisAxeChestCues;
	if(scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES)
		return DehkanPlateauTightropesCues;
	if(scene == SCENE_DEHKAN_PLATEAU_MINT_CHEST)
		return DehkanPlateauMintChestCues;
	if(scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE)
		return DehkanPlateauPoundCubeCues;
	if(scene == SCENE_DEHKAN_PLATEAU_CANNON)
		return DehkanPlateauCannonCues;
	return DehkanPlateauCues;
}

/* Felix steps onto one of the three gray pillars of the first stretch: the
   first step cracks it, and the next crumbles it under him. */
void scDehkanPlateauStepOnFullMetalVestChestPillars(void)
{
	scCrumblingPillarStep(DehkanPlateauFullMetalVestChestCrumblingPillars);
}

/* Gives the metatiles of four of the Themis Axe's stretch's crumbling pillars,
   while each stands, the attributes of metatile (0, 0). */
void scDehkanPlateauSetThemisAxeChestStandingPillarMetatiles(void)
{
	if(syGameFlagGet(FLAG_PILLAR_CRUMBLED(1)) == 0)
		fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 9, 18);
	if(syGameFlagGet(FLAG_PILLAR_CRUMBLED(2)) == 0)
		fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 11, 18);
	if(syGameFlagGet(FLAG_PILLAR_CRUMBLED(8)) == 0)
		fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 14, 28);
	if(syGameFlagGet(FLAG_PILLAR_CRUMBLED(9)) == 0)
		fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 16, 28);
}

/* Felix steps onto one of the gray pillars of the Themis Axe's stretch, which
   cracks and then crumbles as in the first; on the low ones he is drawn behind
   the ledge. */
void scDehkanPlateauStepOnThemisAxeChestPillars(void)
{
	FIXED y;

	scCrumblingPillarStepWithStandIn(
		DehkanPlateauThemisAxeChestCrumblingPillars);
	scDehkanPlateauSetThemisAxeChestStandingPillarMetatiles();
	y = fdActorGetById(MainRoutineStateWork.leaderActorId)->y.fixed;
	if(y < toFIXED(8)) {
		fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 2);
		fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
			ACTOR_PRIORITY_AUTOMATIC;
	}
}

/* The same for two of the rope bridge's stretch's crumbling pillars, from
   metatile (63, 0). */
void scDehkanPlateauSetTightropesStandingPillarMetatiles(void)
{
	if(syGameFlagGet(FLAG_PILLAR_CRUMBLED(1)) == 0)
		fdMapGridCopyMetatileAttributes(63, 0, 1, 1, 50, 20);
	if(syGameFlagGet(FLAG_PILLAR_CRUMBLED(2)) == 0)
		fdMapGridCopyMetatileAttributes(63, 0, 1, 1, 52, 20);
}

/* Felix steps onto one of the gray pillars of the rope bridge's stretch, which
   cracks and then crumbles; below the ledge he is drawn behind it. */
void scDehkanPlateauStepOnTightropesPillars(void)
{
	scCrumblingPillarStep(DehkanPlateauTightropesCrumblingPillars);
	scDehkanPlateauSetTightropesStandingPillarMetatiles();
	if(fdActorGetById(MainRoutineStateWork.leaderActorId)->y.fixed <
		-toFIXED(24)) {
		fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 2);
		fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
			ACTOR_PRIORITY_AUTOMATIC;
	}
}

/* Felix steps onto the gray pillar in Cannon's cave, which cracks and then
   crumbles; once he is up on it he no longer counts as on the cave's lower
   floor (scDehkanPlateauLeaveLowerPath). */
void scDehkanPlateauStepOnCannonPillar(void)
{
	FIXED y;

	scCrumblingPillarStep(DehkanPlateauCannonCrumblingPillar);
	y = fdActorGetById(MainRoutineStateWork.leaderActorId)->y.fixed;
	if(y < toFIXED(24))
		scDehkanPlateauLeaveLowerPath();
}

/* Draws (show 1) or stops drawing the unseen blocks of the room the party is
   in. */
void scDehkanPlateauSetBlocksShown(Sint32 show)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST)
		scActorsSetDrawn(show, ACTOR_FULL_METAL_VEST_CHEST_FIRST_BLOCK,
			FULL_METAL_VEST_CHEST_BLOCK_COUNT);
	else if(scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST)
		scActorsSetDrawn(show, ACTOR_THEMIS_AXE_CHEST_FIRST_BLOCK,
			THEMIS_AXE_CHEST_BLOCK_COUNT);
	else if(scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES)
		scActorsSetDrawn(
			show, ACTOR_TIGHTROPES_FIRST_BLOCK, TIGHTROPES_BLOCK_COUNT);
	else if(scene == SCENE_DEHKAN_PLATEAU_MINT_CHEST)
		scActorsSetDrawn(
			show, ACTOR_MINT_CHEST_FIRST_BLOCK, MINT_CHEST_BLOCK_COUNT);
	else if(scene == SCENE_DEHKAN_PLATEAU_CANNON)
		scActorsSetDrawn(
			show, ACTOR_CANNON_ROOM_FIRST_BLOCK, CANNON_ROOM_BLOCK_COUNT);
}

/* Nothing visible happens when Felix steps onto the metatiles that run this, at
   the foot of a cliff in five of the rooms: the game starts drawing the room's
   unseen blocks again, and notes them shown. In the first room these metatiles
   lie beside those that stop drawing them (scDehkanPlateauHideBlocks), on the
   ways onto the terrace south of the pillars: (34, 13) to (34, 18) beside (35,
   13) to (35, 18); (47, 17), (47, 18) to (49, 18), (46, 20) and (47, 20) beside
   (46, 17) to (46, 19), (47, 19) and (48, 19); and (38, 28) and (39, 28) beside
   (37, 29) to (39, 29). The blocks stand on the terrace, between metatiles (38,
   25) and (46, 21), see-through and without collision (scActorHide). Walking
   south from the terrace down the neck past (38, 28) and (38, 29), which hides
   them, and back up, which shows them, Felix is drawn whole both ways. Walking
   east across the terrace from metatile (41, 24) he steps down past the edge of
   the higher ground there and is hidden by it from the shoulders to the feet,
   the blocks drawn or not: the ground hides him, not the blocks. */
void scDehkanPlateauShowBlocks(void)
{
	scDehkanPlateauSetBlocksShown(1);
	syGameFlagSet(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN);
}

/* Nothing visible happens when Felix steps onto the metatiles beside those of
   scDehkanPlateauShowBlocks (in the first stretch, (35, 45) to (35, 50)) while
   the blocks are drawn: the game stops drawing them, so that Felix is drawn on
   the other side of the cliff there again. */
void scDehkanPlateauHideBlocks(void)
{
	scDehkanPlateauSetBlocksShown(0);
	syGameFlagClear(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN);
}

#include "shared/actors_set_hidden.c"

/* Pound on the stake in the first stretch sinks it into the ground. */
void scDehkanPlateauPoundFullMetalVestChestStake(Sint32 mode, Sint32 frame)
{
	fdPoundSinkTarget(DehkanPlateauFullMetalVestChestStakes, mode, frame);
}

/* Pound on the stake in the Themis Axe's stretch sinks it into the ground. */
void scDehkanPlateauPoundThemisAxeChestStake(Sint32 mode, Sint32 frame)
{
	fdPoundSinkTarget(DehkanPlateauThemisAxeChestStakes, mode, frame);
}

/* Pound on the stake by the rope bridge, if Cannon has not already sunk it,
   sinks it into the ground. */
void scDehkanPlateauPoundTightropesStake(Sint32 mode, Sint32 frame)
{
	fdPoundSinkTarget(DehkanPlateauTightropesStakes, mode, frame);
}

/* Pound on the stake in the cave where Cannon drops the Pound Cube sinks it
   into the ground. */
void scDehkanPlateauPoundPoundCubeStake(Sint32 mode, Sint32 frame)
{
	fdPoundSinkTarget(DehkanPlateauPoundCubeStakes, mode, frame);
}

/* Pound on the stake in Cannon's cave, from the lower floor below his ledge,
   sinks it into the ground: with it gone Cannon has nothing to leap onto and is
   cornered. */
void scDehkanPlateauPoundCannonStake(Sint32 mode, Sint32 frame)
{
	fdPoundSinkTarget(DehkanPlateauCannonStakes, mode, frame);
}

#include "shared/actor_drop_and_wait.h"

/* Moved or pushed to the gap's edge in the first stretch, the stone pillar
   drops into the gap and stays there, bridging it
   (FLAG_FULL_METAL_VEST_CHEST_PILLAR_IN_GAP). */
void scDehkanPlateauDropFullMetalVestChestPillarIntoGap(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_FULL_METAL_VEST_CHEST_PILLAR);
	Sint32 x;

	x = FIELD_METATILE(actor->x.fixed);
	if(x == 56 && FIELD_METATILE(actor->z.fixed) == 32) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdActorDropAndWait(actor);
		fdMapCopyMetatileAttributeRect(54, 32, 2, 1, x, 64);
		syGameFlagSet(FLAG_FULL_METAL_VEST_CHEST_PILLAR_IN_GAP);
		fdScriptEnd();
	}
}

/* Move takes hold of the Themis Axe's stretch's stone pillar. */
void scDehkanPlateauLiftThemisAxeChestPillar(void)
{
	fdActorLiftTargetBlock(DehkanPlateauThemisAxeChestPillars);
}

/* Moved or pushed to the gap's edge in the Themis Axe's stretch, the stone
   pillar drops into the gap for good, bridging it
   (FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED). */
void scDehkanPlateauPlaceThemisAxeChestPillar(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_THEMIS_AXE_CHEST_PILLAR);
	Sint32 x = FIELD_METATILE(actor->x.fixed);

	fdActorPlaceMovedBlock();
	if(x == 52) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdMapCopyMetatileAttributeRect(51, 7, 1, 1, x, 7);
		sySynch(1);
		fdActorDropAndWait(actor);
		fdMapCopyMetatileAttributeRect(50, 7, 1, 1, x, 7);
		syGameFlagSet(FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED);
		fdScriptEnd();
	}
}

/* Move takes hold of the rope bridge's stretch's stone pillar. */
void scDehkanPlateauLiftTightropesPillar(void)
{
	fdActorLiftTargetBlock(DehkanPlateauTightropesPillars);
}

/* Moved or pushed to the gap's edge on the raised ground of the rope bridge's
   stretch, the stone pillar drops into the gap below. */
void scDehkanPlateauPlaceTightropesPillar(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_TIGHTROPES_PILLAR);
	Sint32 x = FIELD_METATILE(actor->x.fixed);

	fdActorPlaceMovedBlock();
	if(x == 47) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdMapCopyMetatileAttributeRect(47, 12, 1, 1, x, 13);
		sySynch(1);
		fdActorDropAndWait(actor);
		fdMapCopyMetatileAttributeRect(50, 16, 1, 1, x, 13);
		fdActorSetSpritePriority(ACTOR_TIGHTROPES_PILLAR, 2);
		fdScriptEnd();
	} else if(x == 46) {
		fdMapGridCopyMetatileAttributes(44, 13, 1, 1, 48, 13);
	}
}

/* Puts the rope bridge's stretch's second stone pillar a column west, off the
   vine it stood before, with the metatiles that let it be walked over and the
   vine climbed. */
void scDehkanPlateauSetTightropesSecondPillarInPlace(void)
{
	fdActorSetPos(ACTOR_TIGHTROPES_SECOND_PILLAR, 600, 536);
	fdMapGridCopyMetatileAttributes(38, 32, 1, 1, 37, 33);
	fdMapGridCopyMetatileAttributes(30, 32, 1, 2, 38, 32);
	fdMapGridCopyMetatileAttributes(40, 64, 4, 3, 36, 64);
	syGameFlagSet(FLAG_TIGHTROPES_SECOND_PILLAR_PLACED);
}

/* Move on the rope bridge's stretch's second stone pillar: a column west, it
   settles there. */
void scDehkanPlateauMoveTightropesSecondPillar(void)
{
	if(FIELD_METATILE(
		   fdActorGetById(ACTOR_TIGHTROPES_SECOND_PILLAR)->x.fixed) == 37)
		scDehkanPlateauSetTightropesSecondPillarInPlace();
}

/* Walking west into the rope bridge's stretch's second stone pillar pushes it a
   column west. */
void scDehkanPlateauPushTightropesSecondPillar(void)
{
	Uint8 *flags;

	if(FIELD_METATILE(
		   fdActorGetById(ACTOR_TIGHTROPES_SECOND_PILLAR)->x.fixed) == 38) {
		flags =
			&fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlags;
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		*flags = 0;
		sySynch(1);
		fdLeaderPushObjectAhead();
		scDehkanPlateauSetTightropesSecondPillarInPlace();
		sySynch(1);
		*flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		fdScriptEnd();
	}
}

/* Stepping off a crack opened on this visit lets it be stepped on again. */
void scDehkanPlateauStepOffCrack(void)
{
	syGameFlagClear(FLAG_STANDING_ON_OPENED_CRACK);
}

/* Forgets which crack the party fell through. */
void scDehkanPlateauForgetFall(void)
{
	Sint32 flag;

	flag = FLAG_FELL_THROUGH_FIRST;
	while(1) {
		syGameFlagClear(flag);
		if(flag == FLAG_FELL_THROUGH_LAST)
			return;
		flag++;
	}
}

/* Defines a function that restores a stretch's count cracks, listed in cracks
   and drawn open with pictures: on a return the opened ones are drawn open;
   otherwise each crack listed as cracked sets its flag, and the fall is
   forgotten. */
#define DEFINE_DEHKAN_PLATEAU_RESTORE_CRACKS(name, cracks, count, pictures) \
	void name(void) \
	{ \
		const Uint16 *metatile = &cracks[0][CRACK_STARTS_CRACKED]; \
		Sint32		  flag = FLAG_CRACK_OPENED_FIRST; \
		Uint32		  i; \
\
		for(i = 0; i < count; i++) { \
			if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) { \
				if(syGameFlagGet(flag) != 0) { \
					Sint32 x; \
					Sint32 y; \
\
					metatile = &cracks[i][CRACK_X]; \
					x = *metatile++; \
					y = *metatile++; \
					fdMapGridCopyMetatileTilesIndicesRect( \
						pictures[0], pictures[1], x, y, 1, 1); \
				} \
			} else { \
				if(*metatile != 0) \
					syGameFlagSet(flag); \
				metatile += CRACK_FIELDS; \
			} \
			flag++; \
		} \
		if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) \
			scDehkanPlateauForgetFall(); \
	}

/* Restores the first stretch's fourteen cracks: on a return the opened ones are
   drawn open, otherwise each listed crack starts its flag. */
DEFINE_DEHKAN_PLATEAU_RESTORE_CRACKS(
	scDehkanPlateauRestoreFullMetalVestChestCracks,
	DehkanPlateauFullMetalVestChestCracks, FULL_METAL_VEST_CHEST_CRACKS,
	DehkanPlateauFullMetalVestChestCrackPictures)

extern Uint16 DehkanPlateauThemisAxeChestCrackPictures[];
extern Uint16 DehkanPlateauThemisAxeChestCracks[][CRACK_FIELDS];

/* Restores the Themis Axe's stretch's eight cracks, as the first stretch's. */
DEFINE_DEHKAN_PLATEAU_RESTORE_CRACKS(scDehkanPlateauRestoreThemisAxeChestCracks,
	DehkanPlateauThemisAxeChestCracks, THEMIS_AXE_CHEST_CRACKS,
	DehkanPlateauThemisAxeChestCrackPictures)

extern Uint16 DehkanPlateauTightropesCrackPictures[];
extern Uint16 DehkanPlateauTightropesCracks[][CRACK_FIELDS];

/* Restores the rope bridge's stretch's six cracks, as the first stretch's. */
DEFINE_DEHKAN_PLATEAU_RESTORE_CRACKS(scDehkanPlateauRestoreTightropesCracks,
	DehkanPlateauTightropesCracks, TIGHTROPES_CRACKS,
	DehkanPlateauTightropesCrackPictures)

/* As the other rooms' (DEFINE_DEHKAN_PLATEAU_RESTORE_CRACKS) for the Pound
   Cube's cave's sixteen cracks. */
void scDehkanPlateauRestorePoundCubeCracks(void)
{
	Uint16 *record = &DehkanPlateauPoundCubeCracks[0][CRACK_STARTS_CRACKED];
	Sint32	flag = FLAG_CRACK_OPENED_FIRST;
	Uint32	i;

	for(i = 0; i < POUND_CUBE_CRACKS; flag++, i++) {
		if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
			if(syGameFlagGet(flag)) {
				Sint32 x;
				Sint32 y;

				record = &DehkanPlateauPoundCubeCracks[i][CRACK_X];
				x = *record++;
				y = *record++;
				fdMapGridCopyMetatileTilesIndicesRect(
					DehkanPlateauPoundCubeCrackPictures[0],
					DehkanPlateauPoundCubeCrackPictures[1], x, y, 1, 1);
			}
		} else {
			if(*record != 0)
				syGameFlagSet(flag);
			record += CRACK_FIELDS;
		}
	}
}

/* The metatile of the crack the leader has just opened, in the rope bridge's
   stretch and the Pound Cube's cave (scDehkanPlateauForgetCrackOnLeaving). */
Sint32 DehkanPlateauCrackZ BSS_DATA;
Sint32 DehkanPlateauCrackX BSS_DATA;

/* Felix steps onto a crack in one of the three stretches outside or in the
   Pound Cube's cave, and is set in the middle of its metatile. A whole crack
   cracks open under him with a rattle. One already open gives way: he raises
   his arms and sinks through it, the screen closes in on him, and he drops into
   the cave below, by the exit of the crack's number. */
void scDehkanPlateauStepOnCrack(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR		*leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32		 metatileX = (FIELD_METATILE(leader->x.fixed) << 1) + 1;
	Sint32		 metatileZ = (FIELD_METATILE(leader->z.fixed) << 1) + 1;
	Sint32		 n, flag;
	Uint16		*record, *next;

	if(MainRoutineStateWork.next ==
		SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST) {
		n = state->stepCue - CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK;
		flag = FLAG_CRACK_OPENED_FIRST + n;
		record = &DehkanPlateauFullMetalVestChestCracks[n][CRACK_X];
	} else if(MainRoutineStateWork.next ==
		SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST) {
		n = state->stepCue - CUE_THEMIS_AXE_CHEST_FIRST_CRACK;
		flag = FLAG_CRACK_OPENED_FIRST + n;
		record = &DehkanPlateauThemisAxeChestCracks[n][CRACK_X];
	} else if(MainRoutineStateWork.next == SCENE_DEHKAN_PLATEAU_TIGHTROPES) {
		n = state->stepCue - CUE_TIGHTROPES_FIRST_CRACK;
		flag = FLAG_CRACK_OPENED_FIRST + n;
		record = &DehkanPlateauTightropesCracks[n][CRACK_X];
	} else if(MainRoutineStateWork.next == SCENE_DEHKAN_PLATEAU_POUND_CUBE) {
		n = state->stepCue - CUE_POUND_CUBE_FIRST_CRACK;
		flag = FLAG_CRACK_OPENED_FIRST + n;
		record = &DehkanPlateauPoundCubeCracks[n][CRACK_X];
		if(syGameFlagGet(FLAG_FELL_THROUGH(
			   DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_TIGHTROPES_NORTH)))
			/* Once the party has fallen in beside Cannon, the fifth crack's
			   hole is drawn with the fourth hole picture instead of the
			   first. */
			DehkanPlateauPoundCubeCracks[4][CRACK_HOLE] = 3;
	} else {
		return;
	}
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 4);
	fdActorMoveToAndWaitById(
		MainRoutineStateWork.leaderActorId, metatileX << 3, metatileZ << 3);
	fdActorSetPosAndResetMotion(leader, toFIXED(metatileX * 8), leader->y.fixed,
		toFIXED(metatileZ * 8));
	fdScriptWait(4);
	fdCameraGetActor()->y.fixed -= toFIXED(8);
	next = record + 1;
	if(syGameFlagGet(flag)) {
		Sint32 x, y, index, toX, toZ;

		sySoundPlay(SE_SPLASH);
		x = *record;
		record = next;
		{
			Sint16 scene = MainRoutineStateWork.next;
			y = *record++;
			if(scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST)
				fdMapGridCopyMetatileTilesIndicesRect(
					DehkanPlateauFullMetalVestChestCrackPictures[2],
					DehkanPlateauFullMetalVestChestCrackPictures[3], x, y, 1,
					1);
			else if(scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST)
				fdMapGridCopyMetatileTilesIndicesRect(
					DehkanPlateauThemisAxeChestCrackPictures[2],
					DehkanPlateauThemisAxeChestCrackPictures[3], x, y, 1, 1);
			else if(scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES)
				fdMapGridCopyMetatileTilesIndicesRect(
					DehkanPlateauTightropesCrackPictures[2],
					DehkanPlateauTightropesCrackPictures[3], x, y, 1, 1);
			else if(scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE)
				fdMapGridCopyMetatileTilesIndicesRect(
					DehkanPlateauPoundCubeCrackPictures[2],
					DehkanPlateauPoundCubeCrackPictures[3], x, y, 1, 1);
		}
		/* The rest of the crack's record: its hole picture and where it is
		   drawn. */
		index = *record++;
		metatileX = index * 2;
		toX = record[0];
		toZ = record[1];
		{
			Sint16 scene = MainRoutineStateWork.next;
			if(scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST) {
				{
					Sint32 srcX = DehkanPlateauFullMetalVestChestHolePictures
							   [metatileX],
						   srcY = DehkanPlateauFullMetalVestChestHolePictures
							   [metatileX + 1],
						   width = 1, height = 2;
					fdMapGridCopyMetatileTilesIndicesRect(
						srcX, srcY, toX, toZ, width, height);
				}
			} else if(scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST) {
				{
					Sint32
						srcX =
							DehkanPlateauThemisAxeChestHolePictures[metatileX],
						srcY =
							DehkanPlateauThemisAxeChestHolePictures[metatileX +
								1],
						width = 1, height = 2;
					fdMapGridCopyMetatileTilesIndicesRect(
						srcX, srcY, toX, toZ, width, height);
				}
			} else if(scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES) {
				{
					Sint32
						srcX = DehkanPlateauTightropesHolePictures[metatileX],
						srcY =
							DehkanPlateauTightropesHolePictures[metatileX + 1],
						width = 1, height = 2;
					fdMapGridCopyMetatileTilesIndicesRect(
						srcX, srcY, toX, toZ, width, height);
				}
			} else if(scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE) {
				if((n == POUND_CUBE_WEST_UPPER_CRACK &&
					   syGameFlagGet(FLAG_CRACK_OPENED(
						   POUND_CUBE_WEST_UPPER_CRACK + 1))) ||
					(n == POUND_CUBE_EAST_UPPER_CRACK &&
						syGameFlagGet(FLAG_CRACK_OPENED(
							POUND_CUBE_EAST_UPPER_CRACK + 1)))) {
					Sint32
						srcX =
							DehkanPlateauPoundCubeHolePictures[2 * JOINED_HOLE],
						srcY =
							DehkanPlateauPoundCubeHolePictures[2 * JOINED_HOLE +
								1],
						width = 1, height = 2;
					fdMapGridCopyMetatileTilesIndicesRect(
						srcX, srcY, toX, toZ, width, height);
				} else {
					Sint32 srcX = DehkanPlateauPoundCubeHolePictures[metatileX],
						   srcY = DehkanPlateauPoundCubeHolePictures[metatileX +
							   1],
						   width = 1, height = 2;
					fdMapGridCopyMetatileTilesIndicesRect(
						srcX, srcY, toX, toZ, width, height);
				}
			}
		}
		fdScriptWait(12);
		leader->facing = FACING_SOUTH;
		leader->motionFlags = 0;
		fdActorSetSpriteFlags(
			fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
		{
			Sint16 scene = MainRoutineStateWork.next;
			if(scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST) {
				fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 3);
			} else if(scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST ||
				scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES) {
				fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 2);
			} else if(scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE) {
				if(DehkanPlateauPoundCubeWork != NULL)
					((FIELD_SPR *)DehkanPlateauPoundCubeWork->sprite)
						->priority = 3;
				fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 3);
			}
		}
		fdActorSetAnimationById(
			MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
		fdScriptWait(16);
		/* metatileX, done with the leader's metatile, counts the fall's
		   ticks. */
		for(metatileX = 0; (Uint32)metatileX < FALL_TICKS; metatileX++) {
			if(metatileX == 5)
				sySoundPlay(SE_DUST_PUFF);
			leader->scaleX += -(toFIXED(1) / 64);
			leader->scaleY += -(toFIXED(3) / 256);
			leader->y.fixed += -(toFIXED(3) / 5);
			sySynch(1);
		}
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionTicks();
		syImportScriptSetExitRequest(state->stepCue);
	} else {
		Sint32 x, y;

		sySoundPlay(SE_RATTLE);
		y = *next;
		x = *record;
		{
			Sint16 scene = MainRoutineStateWork.next;
			if(scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST)
				fdMapGridCopyMetatileTilesIndicesRect(
					DehkanPlateauFullMetalVestChestCrackPictures[0],
					DehkanPlateauFullMetalVestChestCrackPictures[1], x, y, 1,
					1);
			else if(scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST)
				fdMapGridCopyMetatileTilesIndicesRect(
					DehkanPlateauThemisAxeChestCrackPictures[0],
					DehkanPlateauThemisAxeChestCrackPictures[1], x, y, 1, 1);
			else if(scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES)
				fdMapGridCopyMetatileTilesIndicesRect(
					DehkanPlateauTightropesCrackPictures[0],
					DehkanPlateauTightropesCrackPictures[1], x, y, 1, 1);
			else if(scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE)
				fdMapGridCopyMetatileTilesIndicesRect(
					DehkanPlateauPoundCubeCrackPictures[0],
					DehkanPlateauPoundCubeCrackPictures[1], x, y, 1, 1);
		}
		fdScriptWait(12);
		syGameFlagSet(flag);
		syGameFlagSet(FLAG_STANDING_ON_OPENED_CRACK);
		{
			Sint16 scene = MainRoutineStateWork.next;
			if(scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE ||
				scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES) {
				DehkanPlateauCrackX = FIELD_METATILE(leader->x.fixed);
				DehkanPlateauCrackZ = FIELD_METATILE(leader->z.fixed);
			}
		}
		fdScriptEnd();
	}
}

/* The rope bridge's stretch's and the Pound Cube's cave's event: once the
   leader steps off the crack he opened, it can be stepped on again. */
void scDehkanPlateauForgetCrackOnLeaving(void)
{
	ACTOR *actor;

	if(fdScriptGetFlag(FLAG_STANDING_ON_OPENED_CRACK) != 0) {
		actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
		if(DehkanPlateauCrackX != FIELD_METATILE(actor->x.fixed) ||
			DehkanPlateauCrackZ != FIELD_METATILE(actor->z.fixed))
			syGameFlagClear(FLAG_STANDING_ON_OPENED_CRACK);
	}
}

#include "shared/actor_hide.h"

/* Back from a battle or a menu, stops drawing the room's unseen blocks again
   unless they were shown; otherwise notes them shown. */
void scDehkanPlateauRestoreBlocks(void)
{
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
		if(syGameFlagGet(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN) == 0)
			scDehkanPlateauHideBlocks();
	} else {
		syGameFlagSet(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN);
	}
}

/* Hides the first stretch's unseen blocks as it opens. */
void scDehkanPlateauHideFullMetalVestChestBlocks(void)
{
	Uint32 i;

	for(i = 0; i < FULL_METAL_VEST_CHEST_BLOCK_COUNT; i++)
		scActorHide(i + ACTOR_FULL_METAL_VEST_CHEST_FIRST_BLOCK);
	scDehkanPlateauRestoreBlocks();
}

/* Hides the Themis Axe's stretch's unseen blocks as it opens. */
void scDehkanPlateauHideThemisAxeChestBlocks(void)
{
	Uint32 i;

	for(i = 0; i < THEMIS_AXE_CHEST_BLOCK_COUNT; i++)
		scActorHide(i + ACTOR_THEMIS_AXE_CHEST_FIRST_BLOCK);
	scDehkanPlateauRestoreBlocks();
}

/* Hides the rope bridge's stretch's unseen blocks as it opens. */
void scDehkanPlateauHideTightropesBlocks(void)
{
	Uint32 i;

	for(i = 0; i < TIGHTROPES_BLOCK_COUNT; i++)
		scActorHide(i + ACTOR_TIGHTROPES_FIRST_BLOCK);
	scDehkanPlateauRestoreBlocks();
}

/* Hides the unseen blocks of the caves below the cracks as they open. */
void scDehkanPlateauHideMintChestBlocks(void)
{
	Uint32 i;

	for(i = 0; i < MINT_CHEST_BLOCK_COUNT; i++)
		scActorHide(i + ACTOR_MINT_CHEST_FIRST_BLOCK);
	scDehkanPlateauRestoreBlocks();
}

/* Hides Cannon's cave's unseen blocks as it opens. */
void scDehkanPlateauHideCannonBlocks(void)
{
	Uint32 i;

	for(i = 0; i < CANNON_ROOM_BLOCK_COUNT; i++)
		scActorHide(i + ACTOR_CANNON_ROOM_FIRST_BLOCK);
	scDehkanPlateauRestoreBlocks();
}

/* Sets the metatiles under a vine pillar of the rope bridge's stretch once it
   is pushed or Moved: off the ledge at column 12 it drops to the ground below,
   and at columns 18, 54 and 55 it bridges the way. */
void scDehkanPlateauPlaceVinePillar(ACTOR *actor)
{
	Sint32		  x;
	MAP_METATILE *metatile;

	scHeldActorRaiseMetatile(actor);
	x = FIELD_METATILE(actor->x.fixed);
	if(x == 12) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdMapCopyMetatileAttributeRect(0, 32, 1, 1, x, 11);
		sySynch(1);
		fdActorDropAndWait(actor);
		fdMapCopyMetatileAttributeRect(1, 32, 1, 1, x, 11);
		fdScriptEnd();
	} else if(x == 18) {
		metatile = ((MAP_STATE *)RamHeapSlots->mapGrid)->layers[0].metatiles;
		metatile += fdActorGetMetatileIndex(actor);
		metatile->attributeB &= ~MAP_METATILE_JUMP;
	} else if(x == 54) {
		metatile = ((MAP_STATE *)RamHeapSlots->mapGrid)->layers[0].metatiles;
		metatile += fdActorGetMetatileIndex(actor);
		metatile->attributeB &= ~MAP_METATILE_JUMP;
		metatile->collisionCode = CUE_TIGHTROPES_PILLARS_WEST;
	} else if(x == 55) {
		fdMapGridCopyMetatileAttributes(53, 20, 1, 1, 54, 20);
		fdMapCopyMetatileAttributeRect(57, 20, 1, 1, 56, 20);
	}
}

extern Sint16 DehkanPlateauTightropesVinePillars[];

/* Walking into one of the rope bridge's stretch's vine pillars pushes it a
   metatile on. */
void scDehkanPlateauPushVinePillar(void)
{
	ACTOR *actor;

	if(fdLeaderCanPushObjectAhead() != 0) {
		actor = fdActorGetById(
			fdFindNearbyTalkTarget(MainRoutineStateWork.leaderActorId));
		scPillarLowerHeldMetatiles(DehkanPlateauTightropesVinePillars);
		fdLeaderPushObjectAhead();
		scDehkanPlateauPlaceVinePillar(actor);
	}
}

/* Move takes hold of one of the rope bridge's stretch's vine pillars. */
void scDehkanPlateauLiftTightropesVinePillar(void)
{
	scPillarLowerHeldMetatiles(DehkanPlateauTightropesVinePillars);
}

/* Move lets go of one of the rope bridge's stretch's vine pillars. */
void scDehkanPlateauFinishMovingVinePillar(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR		*active;

	active = state->movedBlock;
	if(active != 0)
		scDehkanPlateauPlaceVinePillar(active);
}

/* Nothing visible happens when Felix steps onto the lower floor of Cannon's
   cave, below the ledge (metatiles (43, 16) to (43, 18)), before the stake is
   Pounded. The game holds the stake still and gives the stake's metatile (47,
   12) the attributes of the gray pillar's (51, 12),
   METATILE_BEHAVIOR_CANNON_ROOM_PILLAR with its ground 32 pixels up, so that a
   step onto it is answered as one on the pillar
   (scDehkanPlateauStepOnCannonPillar). Stepping off the lower floor puts the
   metatile back (scDehkanPlateauLeaveLowerPath). */
void scDehkanPlateauEnterLowerPath(void)
{
	Sint32 set = syGameFlagGet(FLAG_CANNON_ROOM_STAKE_POUNDED);

	if(set == 0) {
		fdActorGetById(ACTOR_CANNON_ROOM_STAKE)->motionFlags = set;
		fdMapGridCopyMetatileAttributes(51, 12, 1, 1, 47, 12);
		syGameFlagSet(FLAG_STAKE_METATILE_CHANGED);
	}
}

/* Nothing visible happens when Felix steps off the lower floor of Cannon's cave
   (metatiles (44, 16) to (44, 18), or (52, 15) to (54, 15)) before the stake is
   Pounded: the game gives the stake's metatile back its own attributes. */
void scDehkanPlateauLeaveLowerPath(void)
{
	if(syGameFlagGet(FLAG_CANNON_ROOM_STAKE_POUNDED) == 0) {
		fdMapGridCopyMetatileAttributes(48, 11, 1, 1, 47, 12);
		syGameFlagClear(FLAG_STAKE_METATILE_CHANGED);
	}
}

/* Jolts the camera down eight pixels and back over eight ticks, as the ground
   gives way. */
void scDehkanPlateauJoltCamera(void)
{
	ACTOR *actor = fdCameraGetActor();
	Uint32 i;

	actor->y.fixed -= toFIXED(8);
	sySynch(1);
	for(i = 0; i <= 7; i++) {
		actor->y.fixed += toFIXED(1);
		sySynch(1);
	}
}

/* Places an actor on the ground at a 16.16 position, facing a heading. */
static __inline__ void scActorPlaceFacing(
	Sint32 id, FIXED x, FIXED z, Uint16 angle)
{
	fdActorSetHorizontalPosAndAngleWithTerrain(id, x, z, angle);
}

/* Felix comes up into the cracked field of the Themis Axe's stretch (metatiles
   (39, 13) to (41, 14)) and sees the Mars Djinni Cannon for the first time: the
   view pans north to him, he starts, and he runs north across the cracks,
   cracking one open and breaking through another into a hole, before he hops
   off to the east. */
void scDehkanPlateauCannonFleesOverCracks(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraGetActor()->motionFlags = 0;
	fdCameraMoveTo(658, CAMERA_KEEP, 164, 1);
	fdCameraWaitForArrival();
	fdActorFaceActorById(
		ACTOR_THEMIS_AXE_CHEST_CANNON, MainRoutineStateWork.leaderActorId, 0);
	fdActorShowEmoteById(
		ACTOR_THEMIS_AXE_CHEST_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorSetSpeed(ACTOR_THEMIS_AXE_CHEST_CANNON, 1.6);
	fdActorWalkToAndWait(ACTOR_THEMIS_AXE_CHEST_CANNON, 664, 136);
	fdMapGridCopyMetatileTilesIndicesRect(68, 33, 105, 6, 1, 1);
	sySoundPlay(SE_RATTLE);
	scDehkanPlateauJoltCamera();
	syGameFlagSet(FLAG_CRACK_OPENED(7));
	fdActorWalkToAndWait(ACTOR_THEMIS_AXE_CHEST_CANNON, 664, 104);
	fdMapGridCopyMetatileTilesIndicesRect(69, 33, 105, 4, 1, 1);
	sySoundPlay(SE_SPLASH);
	scDehkanPlateauJoltCamera();
	fdScriptWait(10);
	fdActorJump(ACTOR_THEMIS_AXE_CHEST_CANNON, 6, 16);
	scActorPlaceFacing(ACTOR_THEMIS_AXE_CHEST_CANNON, toFIXED(888),
		toFIXED(168), FACING_SOUTH);
	fdScriptWait(20);
	syGameFlagSet(FLAG_CANNON_FLED_OVER_CRACKS);
	fdScriptEnd();
}

/* Felix reaches the east end of the Themis Axe's stretch (metatiles (56, 13) to
   (59, 13)): the view pans to Cannon by the cliff's edge, who starts and runs
   off east out of sight. */
void scDehkanPlateauCannonFleesEast(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraMoveTo(980, CAMERA_KEEP, 168, 1);
	fdCameraWaitForArrival();
	fdActorFaceActorById(
		ACTOR_THEMIS_AXE_CHEST_CANNON, MainRoutineStateWork.leaderActorId, 0);
	fdActorShowEmoteById(
		ACTOR_THEMIS_AXE_CHEST_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorGetById(ACTOR_THEMIS_AXE_CHEST_CANNON)->motionFlags = 0;
	fdActorSetSpritePriority(ACTOR_THEMIS_AXE_CHEST_CANNON, 1);
	fdActorSetSpeed(ACTOR_THEMIS_AXE_CHEST_CANNON, 1.6);
	fdActorWalkToAndWait(ACTOR_THEMIS_AXE_CHEST_CANNON, 940, 168);
	fdActorWalkToAndWait(ACTOR_THEMIS_AXE_CHEST_CANNON, 984, 152);
	fdActorWalkToAndWait(ACTOR_THEMIS_AXE_CHEST_CANNON, 1040, 152);
	fdActorSetPos(ACTOR_THEMIS_AXE_CHEST_CANNON, 0, 0);
	fdScriptWait(20);
	syGameFlagSet(FLAG_CANNON_FLED_EAST);
	fdScriptEnd();
}

/* Felix comes to the west end of the rope bridge (metatiles (6, 6) and (7, 6)):
   Cannon, standing on the bridge, starts at the sight of him, leaps along it
   and runs off east across. */
void scDehkanPlateauCannonLeapsAway(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraMoveTo(144, CAMERA_KEEP, 107, 1);
	fdCameraWaitForArrival();
	fdActorFaceActorById(
		ACTOR_TIGHTROPES_CANNON, MainRoutineStateWork.leaderActorId, 0);
	fdActorShowEmoteById(
		ACTOR_TIGHTROPES_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorSetSpeed(ACTOR_TIGHTROPES_CANNON, 1.6);
	fdActorWalkToAndWaitById(ACTOR_TIGHTROPES_CANNON, 168, 104);
	fdScriptWait(10);
	sySoundPlay(SE_HIGH_JUMP);
	fdActorGetById(ACTOR_TIGHTROPES_CANNON)->velocityY = toFIXED(8);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_TIGHTROPES_CANNON), 0);
	fdActorWalkToAndWaitById(ACTOR_TIGHTROPES_CANNON, 200, 104);
	fdActorSetSpriteFlags(
		fdActorGetById(ACTOR_TIGHTROPES_CANNON), SPRITE_FLAG_SHADOW);
	fdActorWalkToAndWait(ACTOR_TIGHTROPES_CANNON, 294, 104);
	scActorPlaceFacing(
		ACTOR_TIGHTROPES_CANNON, toFIXED(424), toFIXED(104), FACING_SOUTH);
	syGameFlagSet(FLAG_CANNON_LEAPT_AWAY);
	fdScriptEnd();
}

/* Felix comes near Cannon east of the rope bridge (metatile (23, 6), or from
   the south at (25, 16) to (27, 17)): Cannon starts, casts Pound on the stake
   in his way so that a great fist drives it into the ground, and runs on south
   over it, cracking open a crack behind him. */
void scDehkanPlateauCannonPoundsStake(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorWatchAndSetScript(
		MainRoutineStateWork.leaderActorId, ACTOR_TIGHTROPES_CANNON);
	fdCameraGetActor()->motionFlags = 0;
	fdCameraMoveTo(424, CAMERA_KEEP, 104, 1);
	fdCameraWaitForArrival();
	fdActorFaceActorById(
		ACTOR_TIGHTROPES_CANNON, MainRoutineStateWork.leaderActorId, 0);
	fdActorShowEmoteById(
		ACTOR_TIGHTROPES_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdCameraMoveTo(424, CAMERA_KEEP, 152, 1);
	fdActorSetSpeed(ACTOR_TIGHTROPES_CANNON, 1.6);
	fdActorWalkToAndWait(ACTOR_TIGHTROPES_CANNON, 424, 152);
	fdActorFaceDirectionById(ACTOR_TIGHTROPES_CANNON, FACING_EAST, 20);
	scDehkanPlateauCannonCastsPound();
	fdActorWalkToAndWait(ACTOR_TIGHTROPES_CANNON, 440, 152);
	fdActorWalkToAndWait(ACTOR_TIGHTROPES_CANNON, 456, 168);
	fdActorWalkToAndWait(ACTOR_TIGHTROPES_CANNON, 456, 184);
	syGameFlagSet(FLAG_CRACK_OPENED(1));
	fdMapGridCopyMetatileTilesIndicesRect(69, 51, 92, 11, 1, 1);
	sySoundPlay(SE_RATTLE);
	scDehkanPlateauJoltCamera();
	fdActorWalkToAndWait(ACTOR_TIGHTROPES_CANNON, 456, 216);
	fdActorWalkToAndWait(ACTOR_TIGHTROPES_CANNON, 584, 216);
	scActorPlaceFacing(
		ACTOR_TIGHTROPES_CANNON, toFIXED(616), toFIXED(280), FACING_SOUTH);
	syGameFlagSet(FLAG_CANNON_POUNDED_STAKE);
	fdScriptEnd();
}

/* The event of Cannon's Pound in the rope bridge's stretch: the stake sinks,
   and once the Psynergy ends it is down for good. */
void scDehkanPlateauSinkPoundedStake(void)
{
	FIELD_EFFECT_STATE *state = RamHeapSlots->fieldEffectState;
	ACTOR			   *actor = fdActorGetById(ACTOR_TIGHTROPES_STAKE);

	if(state->frame != -1 && state->frame <= 7)
		actor->scaleY += -(toFIXED(31) / 256);
	if(state->stage != 0) {
		fdMapGridCopyMetatileAttributes(27, 8, 1, 1, 27, 9);
		fdActorSetAnimationById(ACTOR_TIGHTROPES_STAKE, ANIM_STONE_STAKE_SUNK);
		actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
		actor->collisionFlags &= ~ACTOR_SOLID;
		actor->scaleY = toFIXED(1);
		syGameFlagSet(FLAG_TIGHTROPES_STAKE_POUNDED);
	}
}

/* Cannon casts Pound on the rope bridge's stretch's stake. */
void scDehkanPlateauCannonCastsPound(void)
{
	fdPsynergyLoadResources(ABILITY_POUND, 1);
	syFieldSetTickEvent(scDehkanPlateauSinkPoundedStake, EVENT_PASS_TICK);
	fdPsynergySetupActorPair(ACTOR_TIGHTROPES_CANNON, ACTOR_TIGHTROPES_STAKE);
	fdPsynergyStartCast();
	fdPsynergyRunHandler(1);
	fdBattleEffectStopMainActor();
	syCloseEvent(scDehkanPlateauSinkPoundedStake);
	fdPsynergyEnd();
}

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_DEHKAN_PLATEAU_RESTORE_CRACKS
#undef EFFECT_SET_MODE

extern Uint32 DehkanPlateauDustScript[];

/* Cracks a crumbling pillar as Cannon lands on it: it flashes white and takes
   its cracked look, and eight puffs of dust rise about the one standing on
   it. */
void scDehkanPlateauCrackPillar(Sint32 id, Sint32 standingId, Sint32 flag)
{
	ACTOR *standing;
	Sint32 vramEntry;
	Uint32 i;
	FIXED  y;

	sySoundPlay(SE_CHEST_CLOSE);
	fdActorSetSpriteColor(fdActorGetById(id), SPRITE_COLOR_WHITE);
	sySynch(2);
	fdActorSetAnimationById(id, 0);
	fdActorSetSpriteColor(fdActorGetById(id), SPRITE_COLOR_WHITE);
	sySynch(4);
	fdActorSetSpriteColor(fdActorGetById(id), SPRITE_COLOR_NORMAL);
	standing = fdActorGetById(standingId);
	vramEntry = 0;
	for(i = 0; i <= 7; i++) {
		ACTOR		 *actor;
		FIELD_EFFECT *effect;

		y = standing->y.fixed - toFIXED(syRandomBelow(14));
		actor = ((ACTOR *)syImportActorCreate(SPRITE_DUST_CLOUD,
			toFIXED(i * 2) + standing->x.fixed - toFIXED(8), y - toFIXED(2),
			standing->z.fixed));
		if(actor != NULL) {
			effect = (FIELD_EFFECT *)actor;
			vramEntry = fdSpriteShareVRAMCacheEntry(effect->sprite, vramEntry);
			actor->motionFlags = 0;
			actor->collisionFlags = 0;
			fdActorSetSpriteFlags(actor, 0);
			fdActorSelectAnimation(actor, 2);
			fdActorStartScript(actor, DehkanPlateauDustScript);
			actor->scaleX = toFIXED(7) / 10;
			actor->scaleY = toFIXED(7) / 10;
			effect->sprite->priority = 1;
			effect->velocity.x = 0;
			effect->velocity.z = 0;
			effect->velocity.y = -(syRandomBelow(16384) + toFIXED(3) / 8);
			effect->spin = syRandomBelow(512) - 256;
			effect->scaleRateX = -(toFIXED(1) / 32);
			effect->scaleRateY = -(toFIXED(3) / 128);
			effect->update = fdEffectAdvanceMotion;
		}
	}
	syGameFlagSet(flag);
	fdScriptWait(20);
}

/* Felix follows Cannon to the gray pillars east of the rope bridge (metatiles
   (36, 12) to (36, 16), (35, 16) and (35, 17)): Cannon starts, leaps across the
   pillars, cracking two as he lands on them, and runs off north over a crack,
   which opens into a hole as he goes, and he is gone. */
void scDehkanPlateauCannonCracksPillars(void)
{
	ACTOR *actor;

	actor = fdActorGetById(ACTOR_TIGHTROPES_CANNON);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorWatchAndSetScript(
		MainRoutineStateWork.leaderActorId, ACTOR_TIGHTROPES_CANNON);
	fdCameraGetActor()->motionFlags = 0;
	fdCameraMoveTo(616, CAMERA_KEEP, 280, 1);
	fdCameraWaitForArrival();
	fdActorFaceActorById(
		ACTOR_TIGHTROPES_CANNON, MainRoutineStateWork.leaderActorId, 0);
	fdActorShowEmoteById(
		ACTOR_TIGHTROPES_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorSetSpritePriority(ACTOR_TIGHTROPES_CANNON, 1);
	fdActorSetSpeed(ACTOR_TIGHTROPES_CANNON, 1.6);
	fdActorWalkToAndWait(ACTOR_TIGHTROPES_CANNON, 632, 280);
	fdActorSetSpeed(ACTOR_TIGHTROPES_CANNON, 2);
	sySoundPlay(SE_HIGH_JUMP);
	actor->velocityY = toFIXED(8);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_TIGHTROPES_CANNON), 0);
	fdActorWalkToAndWait(ACTOR_TIGHTROPES_CANNON, 664, 280);
	fdActorSetSpriteFlags(
		fdActorGetById(ACTOR_TIGHTROPES_CANNON), SPRITE_FLAG_SHADOW);
	scDehkanPlateauCrackPillar(ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR,
		ACTOR_TIGHTROPES_CANNON, FLAG_PILLAR_CRACKED(0));
	sySoundPlay(SE_HIGH_JUMP);
	actor->velocityY = toFIXED(8);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_TIGHTROPES_CANNON), 0);
	fdActorWalkToAndWait(ACTOR_TIGHTROPES_CANNON, 696, 280);
	fdActorSetSpriteFlags(
		fdActorGetById(ACTOR_TIGHTROPES_CANNON), SPRITE_FLAG_SHADOW);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_TIGHTROPES_CANNON, 1.6);
	fdActorWalkToAndWait(ACTOR_TIGHTROPES_CANNON, 712, 280);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(712, CAMERA_KEEP, 136, 1);
	fdMapGridCopyMetatileTilesIndicesRect(112, 6, 110, 4, 1, 1);
	fdActorSetSpritePriority(ACTOR_TIGHTROPES_CANNON, 1);
	fdActorWalkToAndWait(ACTOR_TIGHTROPES_CANNON, 712, 200);
	fdActorWalkToAndWaitById(ACTOR_TIGHTROPES_CANNON, 712, 168);
	fdActorStop(MainRoutineStateWork.leaderActorId);
	fdActorFaceDirectionAndRefreshById(ACTOR_TIGHTROPES_CANNON, FACING_EAST);
	fdActorSetSpeed(ACTOR_TIGHTROPES_CANNON, 2);
	sySoundPlay(SE_HIGH_JUMP);
	actor->velocityY = toFIXED(8);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_TIGHTROPES_CANNON), 0);
	fdActorWalkToAndWait(ACTOR_TIGHTROPES_CANNON, 744, 168);
	fdActorSetSpriteFlags(
		fdActorGetById(ACTOR_TIGHTROPES_CANNON), SPRITE_FLAG_SHADOW);
	scDehkanPlateauCrackPillar(ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR + 3,
		ACTOR_TIGHTROPES_CANNON, FLAG_PILLAR_CRACKED(3));
	fdActorFaceDirectionAndRefreshById(ACTOR_TIGHTROPES_CANNON, FACING_NORTH);
	sySoundPlay(SE_HIGH_JUMP);
	actor->velocityY = toFIXED(8);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_TIGHTROPES_CANNON), 0);
	fdActorWalkToAndWait(ACTOR_TIGHTROPES_CANNON, 744, 136);
	fdActorSetSpriteFlags(
		fdActorGetById(ACTOR_TIGHTROPES_CANNON), SPRITE_FLAG_SHADOW);
	fdScriptWait(10);
	fdActorWalkToAndWait(ACTOR_TIGHTROPES_CANNON, 744, 72);
	fdMapGridCopyMetatileTilesIndicesRect(66, 51, 110, 4, 1, 1);
	sySoundPlay(SE_SPLASH);
	scDehkanPlateauJoltCamera();
	fdScriptWait(10);
	fdActorJump(ACTOR_TIGHTROPES_CANNON, 6, 16);
	fdActorSetPos(ACTOR_TIGHTROPES_CANNON, 0, 0);
	fdScriptWait(20);
	syGameFlagSet(FLAG_CANNON_CRACKED_PILLARS);
	fdScriptEnd();
}

/* Felix steps onto the crack south of where the Pound Cube lies, in the cave
   where Cannon drops it (metatile (15, 8)), before he has fallen in beside
   Cannon:
   Cannon starts and casts Pound on him, a great fist slams down, and Felix
   sinks through the floor and lands in Cannon's cave below. */
void scDehkanPlateauCannonPoundsLeaderThrough(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Uint32 i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	{
		Sint32 facing = FACING_NORTH;

		fdActorSetHorizontalPosAndAngleWithTerrain(
			MainRoutineStateWork.leaderActorId, toFIXED(248), toFIXED(136),
			facing);
	}
	fdMapGridCopyMetatileTilesIndicesRect(65, 64, 15, 8, 1, 1);
	syGameFlagSet(FLAG_CRACK_OPENED(5));
	sySoundPlay(SE_RATTLE);
	scDehkanPlateauJoltCamera();
	fdActorFaceActorById(
		ACTOR_POUND_CUBE_CANNON, MainRoutineStateWork.leaderActorId, 0);
	fdActorShowEmoteById(
		ACTOR_POUND_CUBE_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	scDehkanPlateauCannonCastsPoundOnLeader();
	fdScriptWait(20);
	leader->facing = FACING_SOUTH;
	leader->motionFlags = 0;
	fdActorSetSpritePriority(ACTOR_POUND_CUBE_CANNON, 3);
	fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 3);
	fdActorSetAnimationById(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
	fdScriptWait(16);
	for(i = 0; i < FALL_TICKS; i++) {
		if(i == 5)
			sySoundPlay(SE_DUST_PUFF);
		leader->scaleX -= toFIXED(1) / 64;
		leader->scaleY -= toFIXED(3) / 256;
		leader->y.fixed -= toFIXED(3) / 5;
		sySynch(1);
	}
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(CUE_POUND_CUBE_CANNON_CRACK);
}

/* The event of Cannon's Pound on the leader: the floor under him opens. */
void scDehkanPlateauPoundLeaderThrough(void)
{
	if(((FIELD_EFFECT_STATE *)RamHeapSlots->fieldEffectState)->stage != 0) {
		sySoundPlay(SE_SPLASH);
		fdMapGridCopyMetatileTilesIndicesRect(64, 64, 15, 8, 1, 1);
		fdMapGridCopyMetatileTilesIndicesRect(64, 66, 15, 72, 1, 2);
		fdActorSetSpriteFlags(
			fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	}
}

/* Cannon casts Pound on the leader in the Pound Cube's cave. */
void scDehkanPlateauCannonCastsPoundOnLeader(void)
{
	fdPsynergyLoadResources(ABILITY_POUND, 1);
	syFieldSetTickEvent(scDehkanPlateauPoundLeaderThrough, EVENT_PASS_TICK);
	fdPsynergySetupActorPair(
		ACTOR_POUND_CUBE_CANNON, MainRoutineStateWork.leaderActorId);
	fdPsynergyStartCast();
	fdPsynergyRunHandler(1);
	fdBattleEffectStopMainActor();
	syCloseEvent(scDehkanPlateauPoundLeaderThrough);
	fdPsynergyEnd();
}

/* Felix falls through a crack in the north of the rope bridge's stretch and
   lands beside Cannon in the cave below; both bounce apart, Cannon drops the
   Pound Cube in his fright, glares at Felix and runs off north. */
void scDehkanPlateauCannonDropsPoundCube(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *cannon = fdActorGetById(ACTOR_POUND_CUBE_CANNON);

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	leader->y.fixed = toFIXED(130);
	leader->gravity = toFIXED(1) / 2;
	leader->bounce = 0;
	leader->motionFlags = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	sySoundPlay(SE_DUST_PUFF);
	leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdScriptWait(24);
	sySoundPlay(SE_KOLIMA_BRIDGE_HOP);
	scDehkanPlateauJoltCamera();
	if(DehkanPlateauPoundCubeWork != NULL) {
		scFieldActorSetPosAndResetMotion(
			DehkanPlateauPoundCubeWork, toFIXED(248), 0, toFIXED(120));
		sySynch(1);
		DehkanPlateauPoundCubeWork->speedLimit = toFIXED(8) / 5;
		DehkanPlateauPoundCubeWork->acceleration = toFIXED(8) / 5;
		DehkanPlateauPoundCubeWork->velocityY = toFIXED(8);
	}
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
	fdActorSetSpeed(ACTOR_POUND_CUBE_CANNON, 1.6);
	leader->velocityY = toFIXED(6);
	cannon->velocityY = toFIXED(6);
	scFieldActorSetMoveTarget(leader, 216, 0, 120);
	scFieldActorSetMoveTarget(cannon, 280, 0, 120);
	if(DehkanPlateauPoundCubeWork != NULL)
		scFieldActorSetMoveTarget(DehkanPlateauPoundCubeWork, 248, 0, 88);
	fdActorWaitForMove(leader);
	fdScriptWait(20);
	fdActorShowEmoteById(
		ACTOR_POUND_CUBE_CANNON, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_POUND_CUBE_CANNON, FACING_WEST, 60);
	fdActorWalkToAndWait(ACTOR_POUND_CUBE_CANNON, 296, 104);
	fdActorWalkToAndWait(ACTOR_POUND_CUBE_CANNON, 296, 80);
	fdActorSetPos(ACTOR_POUND_CUBE_CANNON, 0, 0);
	fdScriptWait(20);
	if(DehkanPlateauPoundCubeWork != NULL) {
		DehkanPlateauPoundCubeWork->motionFlags = 0;
		fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 15, 5);
		sySynch(1);
	}
	syGameFlagSet(FLAG_CANNON_DROPPED_POUND_CUBE);
	fdScriptEnd();
}

/* While the stake stands, Cannon sees Felix coming along the ledge, leaps onto
   the stake, then onto the gray pillar beyond it, which flashes and cracks
   under him, and on to the far ledge, where he waits. */
void scDehkanPlateauCannonCracksLastPillar(void)
{
	ACTOR *actor;

	actor = fdActorGetById(ACTOR_CANNON);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId, ACTOR_CANNON);
	fdCameraGetActor()->motionFlags = 0;
	fdCameraMoveTo(760, 32, 204, 1);
	fdCameraWaitForArrival();
	fdActorFaceActorById(ACTOR_CANNON, MainRoutineStateWork.leaderActorId, 0);
	fdActorShowEmote(ACTOR_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorSetSpeed(ACTOR_CANNON, 2);
	sySoundPlay(SE_HIGH_JUMP);
	actor->velocityY = toFIXED(8);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_CANNON), 0);
	fdActorWalkToAndWaitById(ACTOR_CANNON, 760, 200);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_CANNON), SPRITE_FLAG_SHADOW);
	fdScriptWait(20);
	sySoundPlay(SE_HIGH_JUMP);
	actor->velocityY = toFIXED(8);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_CANNON), 0);
	fdActorWalkToAndWaitById(ACTOR_CANNON, 792, 200);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_CANNON), SPRITE_FLAG_SHADOW);
	scDehkanPlateauCrackPillar(ACTOR_CANNON_ROOM_CRUMBLING_PILLAR, ACTOR_CANNON,
		FLAG_PILLAR_CRACKED(0));
	sySoundPlay(SE_HIGH_JUMP);
	actor->velocityY = toFIXED(8);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_CANNON), 0);
	fdActorWalkToAndWaitById(ACTOR_CANNON, 824, 200);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_CANNON), SPRITE_FLAG_SHADOW);
	fdScriptWait(20);
	fdActorWatchAndSetScript(ACTOR_CANNON, MainRoutineStateWork.leaderActorId);
	fdActorStop(MainRoutineStateWork.leaderActorId);
	fdScriptWait(20);
	fdScriptEnd();
}

/* Once the stake is down, Cannon sees Felix coming and finds he has nowhere to
   leap: he looks about him and turns to face Felix. */
void scDehkanPlateauCannonCornered(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorFaceActorById(ACTOR_CANNON, MainRoutineStateWork.leaderActorId, 0);
	fdActorShowEmoteById(ACTOR_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirectionAndRefreshById(ACTOR_CANNON, FACING_EAST);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_CANNON, 3);
	fdActorFaceDirectionAndWait(ACTOR_CANNON, FACING_WEST);
	fdActorWatchAndSetScript(ACTOR_CANNON, MainRoutineStateWork.leaderActorId);
	fdEffectSetAttached(
		ACTOR_CANNON, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdScriptEnd();
}

/* Felix walks east along the ledge toward Cannon in his cave (metatiles (43,
   12) and (43, 13)), once a visit. While the stake stands Cannon leaps away
   over the stake and the gray pillar, cracking it; once Felix has Pounded the
   stake into the ground Cannon is cornered and turns to face him. */
void scDehkanPlateauMeetCannon(void)
{
	if(syGameFlagGet(FLAG_CANNON_ROOM_STAKE_POUNDED) != 0)
		scDehkanPlateauCannonCornered();
	else
		scDehkanPlateauCannonCracksLastPillar();
	syGameFlagSet(FLAG_CANNON_MET);
}

/* Felix examines the Pound Cube where Cannon dropped it, north of the crack
   Cannon Pounds him through (metatile (15, 5)), and takes it. */
void scDehkanPlateauTakePoundCube(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if(DehkanPlateauPoundCubeWork != 0)
		fdItemDisplayShowPicture(DehkanPlateauPoundCubeWork,
			ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
	fdPartyInventoryGiveItem(ITEM_POUND_CUBE, 0);
	if(DehkanPlateauPoundCubeWork != 0)
		fdActorRelease(DehkanPlateauPoundCubeWork);
	fdMapGridCopyMetatileAttributes(1, 0, 1, 1, 15, 5);
	syGameFlagSet(FLAG_POUND_CUBE_TAKEN);
	fdScriptEnd();
}

/* Felix walks into a cave mouth in the cliff of the first stretch (map metatile
   (84, 21)) or of the Themis Axe's stretch (metatile (113, 12)): the exit sound
   plays, the screen fades to black, and he comes out in the caves below the
   cracks. Both metatiles are on the upper level, which map.txt keeps from
   column 64. */
void scDehkanPlateauLeaveRoom(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if(MainRoutineStateWork.next == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST)
		fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 3);
	sySoundPlay(SE_MAP_EXIT);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(state->stepCue);
}

/* Sets up the room of the plateau the party comes into. */
Sint32 scInit(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST)
		scDehkanPlateauInitFullMetalVestChest();
	else if(scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST)
		scDehkanPlateauInitThemisAxeChest();
	else if(scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES)
		scDehkanPlateauInitTightropes();
	else if(scene == SCENE_DEHKAN_PLATEAU_MINT_CHEST)
		scDehkanPlateauInitMintChest();
	else if(scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE)
		scDehkanPlateauInitPoundCube();
	else if(scene == SCENE_DEHKAN_PLATEAU_CANNON)
		scDehkanPlateauInitCannon();
	return 0;
}

/* The first stretch's event: draws the chest on the ledge at the leader's
   sprite priority each tick, so it stays in front of or behind him with the
   ground. */
void scDehkanPlateauKeepPriorityWithLeader(void)
{
	fdActorSetSpritePriority(ACTOR_FULL_METAL_VEST_CHEST,
		((SPR_ATTR2_HIGH *)fdActorGetById(MainRoutineStateWork.leaderActorId)
				->sprite)
			->priority);
}

/* Sets up the first stretch: its pillars, the stone pillar already in the gap,
   its cracks and unseen blocks, and the chest's event. */
void scDehkanPlateauInitFullMetalVestChest(void)
{
	syCrumblingPillarPlace(DehkanPlateauFullMetalVestChestCrumblingPillars);
	fdActorSetSpritePriority(
		ACTOR_FULL_METAL_VEST_CHEST_FIRST_CRUMBLING_PILLAR, 3);
	scPillarPlace(DehkanPlateauFullMetalVestChestStakes);
	fdActorDisableById(ACTOR_FULL_METAL_VEST_CHEST_PILLAR);
	if(syGameFlagGet(FLAG_FULL_METAL_VEST_CHEST_PILLAR_IN_GAP) != 0) {
		fdMapGridCopyMetatileAttributes(54, 32, 2, 1, 56, 64);
		fdActorSetPosAndResetMotion(
			fdActorGetById(ACTOR_FULL_METAL_VEST_CHEST_PILLAR), toFIXED(904),
			toFIXED(64), toFIXED(520));
		sySynch(10);
	}
	scDehkanPlateauRestoreFullMetalVestChestCracks();
	scDehkanPlateauHideFullMetalVestChestBlocks();
	{
		Sint32 priority = EVENT_PASS_TICK;

		sySetEvent(scDehkanPlateauKeepPriorityWithLeader, priority);
	}
}

/* Sets up the Themis Axe's stretch: its crumbling pillars, stake and stone
   pillar, its cracks, and Cannon as far as he has fled. */
void scDehkanPlateauInitThemisAxeChest(void)
{
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
			ACTOR_PRIORITY_IGNORES_RISES;
		if(MainRoutineStateWork.entrance ==
			ENTRANCE_THEMIS_AXE_CHEST_WEST_SCREEN) {
			fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 2);
			fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
				ACTOR_PRIORITY_AUTOMATIC;
		}
	}
	scCrumblingPillarPlaceWithStandIn(
		DehkanPlateauThemisAxeChestCrumblingPillars);
	scDehkanPlateauSetThemisAxeChestStandingPillarMetatiles();
	scPillarPlace(DehkanPlateauThemisAxeChestStakes);
	if(syGameFlagGet(FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED) != 0) {
		fdActorDisableById(ACTOR_THEMIS_AXE_CHEST_PILLAR);
		fdMapGridCopyMetatileAttributes(50, 7, 1, 1, 52, 7);
		scFieldActorSetPosAndResetMotion(
			fdActorGetById(ACTOR_THEMIS_AXE_CHEST_PILLAR), toFIXED(840), 0,
			toFIXED(120));
	} else {
		fdActorPlaceBlocks(DehkanPlateauThemisAxeChestPillars);
	}
	scDehkanPlateauRestoreThemisAxeChestCracks();
	if(syGameFlagGet(FLAG_CANNON_FLED_OVER_CRACKS) != 0) {
		Sint32 angle = FACING_SOUTH;

		scActorPlaceFacing(
			ACTOR_THEMIS_AXE_CHEST_CANNON, toFIXED(888), toFIXED(168), angle);
		fdMapGridCopyMetatileTilesIndicesRect(68, 33, 105, 6, 1, 1);
		fdMapGridCopyMetatileTilesIndicesRect(69, 33, 105, 4, 1, 1);
		syGameFlagSet(FLAG_CRACK_OPENED(7));
	}
	if(syGameFlagGet(FLAG_CANNON_FLED_EAST) != 0)
		fdActorSetPos(ACTOR_THEMIS_AXE_CHEST_CANNON, 0, 0);
	scDehkanPlateauHideThemisAxeChestBlocks();
}

void scDehkanPlateauForgetCrackOnLeaving(void);

/* Sets up the rope bridge's stretch: its tightropes and the rope to Lash, its
   stone, vine and stakes and crumbling pillars, its cracks, and Cannon as far
   as he has gone. */
void scDehkanPlateauInitTightropes(void)
{
	ACTOR *actor;
	FIXED  pillarX;

	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
			ACTOR_PRIORITY_IGNORES_RISES;
		fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 2);
		fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
			ACTOR_PRIORITY_AUTOMATIC;
	}
	fdActorSetSpritePriority(ACTOR_TIGHTROPES_FIRST_TIGHTROPE_WEST_POST, 1);
	fdActorSetSpritePriority(ACTOR_TIGHTROPES_FIRST_TIGHTROPE_EAST_POST, 1);
	fdActorSetSpritePriority(ACTOR_TIGHTROPES_SECOND_TIGHTROPE_WEST_POST, 1);
	fdActorSetSpritePriority(ACTOR_TIGHTROPES_SECOND_TIGHTROPE_EAST_POST, 1);
	fdTightropeInit();
	fdTightropePlace(0, ACTOR_TIGHTROPES_FIRST_TIGHTROPE_WEST_POST,
		ACTOR_TIGHTROPES_FIRST_TIGHTROPE_EAST_POST);
	fdTightropePlace(1, ACTOR_TIGHTROPES_SECOND_TIGHTROPE_WEST_POST,
		ACTOR_TIGHTROPES_SECOND_TIGHTROPE_EAST_POST);
	fdActorSetSpritePriority(ACTOR_TIGHTROPES_ROPE_LOW_POST, 1);
	fdActorSetSpritePriority(ACTOR_TIGHTROPES_ROPE_HIGH_POST, 1);
	scDehkanPlateauInitRopes();
	fdRopePlace(0, FLAG_TIGHTROPES_ROPE_LASHED, ACTOR_TIGHTROPES_ROPE_LOW_POST,
		ACTOR_TIGHTROPES_ROPE_HIGH_POST);
	fdActorPlaceBlocks(DehkanPlateauTightropesPillars);
	if(fdActorGetById(ACTOR_TIGHTROPES_PILLAR)->y.fixed > (Sint32)-toFIXED(8))
		fdActorSetSpritePriority(ACTOR_TIGHTROPES_PILLAR, 1);
	else
		fdActorSetSpritePriority(ACTOR_TIGHTROPES_PILLAR, 2);
	fdActorDisableById(ACTOR_TIGHTROPES_SECOND_PILLAR);
	fdActorGetById(ACTOR_TIGHTROPES_SECOND_PILLAR)->priorityFlags |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdActorSetSpritePriority(ACTOR_TIGHTROPES_SECOND_PILLAR, 2);
	if(syGameFlagGet(FLAG_TIGHTROPES_SECOND_PILLAR_PLACED) != 0)
		scDehkanPlateauSetTightropesSecondPillarInPlace();
	else
		fdMapGridCopyMetatileAttributes(38, 32, 1, 1, 38, 33);
	fdActorGetById(ACTOR_TIGHTROPES_FIRST_VINE_PILLAR)->priorityFlags &=
		~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGetById(ACTOR_TIGHTROPES_SECOND_VINE_PILLAR)->priorityFlags &=
		~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGetById(ACTOR_TIGHTROPES_THIRD_VINE_PILLAR)->priorityFlags &=
		~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGetById(ACTOR_TIGHTROPES_FOURTH_VINE_PILLAR)->priorityFlags &=
		~ACTOR_PRIORITY_AUTOMATIC;
	scPillarRaise(DehkanPlateauTightropesVinePillars);
	{
		ACTOR *pillar = fdActorGetById(ACTOR_TIGHTROPES_FOURTH_VINE_PILLAR);

		pillarX = pillar->x.fixed;
		if(FIELD_METATILE(pillarX) == 54) {
			MAP_METATILE *metatile =
				((MAP_STATE *)RamHeapSlots->mapGrid)->layers[0].metatiles;

			metatile += fdActorGetMetatileIndex(pillar);
			metatile->attributeB &= ~MAP_METATILE_JUMP;
			metatile->collisionCode = CUE_TIGHTROPES_PILLARS_WEST;
		}
	}
	fdActorSetSpritePriority(ACTOR_TIGHTROPES_FIRST_VINE_PILLAR, 1);
	fdActorSetSpritePriority(ACTOR_TIGHTROPES_SECOND_VINE_PILLAR, 1);
	scDehkanPlateauRestoreTightropesCracks();
	if(syGameFlagGet(FLAG_CANNON_LEAPT_AWAY) != 0) {
		fdActorSetPosAndFacing(
			ACTOR_TIGHTROPES_CANNON, toFIXED(424), toFIXED(104), FACING_SOUTH);
	}
	fdActorSetAnimationById(ACTOR_TIGHTROPES_STAKE, ANIM_STONE_STAKE_STANDING);
	if(syGameFlagGet(FLAG_CANNON_POUNDED_STAKE) != 0) {
		actor = fdActorGetById(ACTOR_TIGHTROPES_STAKE);
		fdMapGridCopyMetatileAttributes(27, 8, 1, 1, 27, 9);
		fdActorSetAnimationById(ACTOR_TIGHTROPES_STAKE, ANIM_STONE_STAKE_SUNK);
		actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
		actor->collisionFlags &= ~ACTOR_SOLID;
		syGameFlagSet(FLAG_TIGHTROPES_STAKE_POUNDED);
		syGameFlagSet(FLAG_CRACK_OPENED(1));
		fdMapGridCopyMetatileTilesIndicesRect(69, 51, 92, 11, 1, 1);
		fdActorSetPosAndFacing(
			ACTOR_TIGHTROPES_CANNON, toFIXED(616), toFIXED(280), FACING_SOUTH);
	}
	if(syGameFlagGet(FLAG_CANNON_CRACKED_PILLARS) != 0) {
		syGameFlagSet(FLAG_PILLAR_CRACKED(0));
		syGameFlagSet(FLAG_PILLAR_CRACKED(3));
		fdActorSetPos(ACTOR_TIGHTROPES_CANNON, 0, 0);
	}
	syCrumblingPillarPlace(DehkanPlateauTightropesCrumblingPillars);
	scDehkanPlateauSetTightropesStandingPillarMetatiles();
	scDehkanPlateauHideTightropesBlocks();
	/* The second unseen block, at (408, 72), stands on a metatile 32 pixels
	   high beside the path; it is held at the path's height instead. */
	{
		ACTOR *block = fdActorGetById(ACTOR_TIGHTROPES_FIRST_BLOCK + 1);

		block->motionFlags = 0;
		block->y.fixed = 0;
	}
	DehkanPlateauCrackX = 0;
	DehkanPlateauCrackZ = 0;
	sySetEvent(scDehkanPlateauForgetCrackOnLeaving, EVENT_PASS_TICK);
}

extern Uint16 DehkanPlateauFallLandingMetatiles[];

/* Defines a function that draws, in a cave below the cracks, where the party
   landed under the crack it fell through (DehkanPlateauFallLandingMetatiles):
   the floor metatile from (sourceX, sourceZ), a column of five metatiles from
   (columnSourceX, columnSourceZ), and four metatiles from (sourceX,
   belowSourceZ), belowOffsetZ rows below that column on the lower layer. */
#define DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(name, sourceX, sourceZ, \
	columnSourceX, columnSourceZ, belowOffsetZ, belowSourceZ) \
	void name(void) \
	{ \
		Sint32 flag = FLAG_FELL_THROUGH_FIRST; \
\
		while(1) { \
			if(syGameFlagGet(flag)) { \
				Sint32	fall = flag - FLAG_FELL_THROUGH_FIRST; \
				Uint16 *entry = &DehkanPlateauFallLandingMetatiles[fall * 4]; \
				Sint32	y; \
				Sint32	x; \
\
				x = *entry++; \
				y = *entry++; \
				fdMapGridCopyMetatilesTo(sourceX, sourceZ, x, y, 1, 1); \
				x = entry[0]; \
				y = entry[1]; \
				fdMapGridCopyMetatilesTo( \
					columnSourceX, columnSourceZ, x, y, 1, 5); \
				/* The second crack's shaft of light starts a row lower: its \
				 * top metatile takes the shaft's second picture, the Mint \
				 * chest cave's (67, 4). */ \
				if(fall == 1) \
					fdMapGridCopyMetatilesTo(67, 4, 69, 3, 1, 1); \
				x -= 64; \
				y += belowOffsetZ; \
				fdMapGridCopyMetatilesTo(sourceX, belowSourceZ, x, y, 1, 4); \
				return; \
			} \
			if(flag == FLAG_FELL_THROUGH_LAST) \
				return; \
			flag++; \
		} \
	}

/* Draws, in the caves below the cracks, the metatiles where the party landed
   under the crack it fell through. */
DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(
	scDehkanPlateauDrawMintChestFallLanding, 3, 8, 67, 3, 66, 69)

/* Draws, in the Pound Cube's cave, the metatiles where the party landed under
   the crack it fell through. */
DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(
	scDehkanPlateauDrawPoundCubeFallLanding, 13, 5, 77, 0, 65, 65)

/* Sets up the caves below the cracks: the floor Cannon opened, and the landing
   of a fall. */
void scDehkanPlateauInitMintChest(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
		ACTOR_PRIORITY_IGNORES_RISES;
	scDehkanPlateauHideMintChestBlocks();
	if(syGameFlagGet(FLAG_CANNON_FLED_OVER_CRACKS) != 0) {
		fdMapGridCopyMetatileTilesIndicesRect(3, 8, 30, 33, 1, 1);
		fdMapGridCopyMetatileTilesIndicesRect(67, 3, 94, 28, 1, 5);
		fdMapGridCopyMetatileTilesIndicesRect(3, 69, 30, 93, 1, 4);
	}
	if(MainRoutineStateWork.entrance >= FIRST_LANDING_ENTRANCE &&
		MainRoutineStateWork.entrance <= LAST_LANDING_ENTRANCE) {
		Sint32 entrance = MainRoutineStateWork.entrance;

		scDehkanPlateauForgetFall();
		syGameFlagSet(FLAG_FELL_THROUGH(entrance));
	}
	scDehkanPlateauDrawMintChestFallLanding();
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		if(MainRoutineStateWork.entrance >= FIRST_LANDING_ENTRANCE) {
			if(MainRoutineStateWork.entrance <= LAST_LANDING_ENTRANCE)
				scDehkanPlateauLandFromFall();
		}
	}
}

/* Felix drops in from the crack above in a shaft of light, landing in a ring of
   dust. */
void scDehkanPlateauLandFromFall(void)
{
	ACTOR		  *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	EFFECT_OPTIONS options;
	Uint32		   i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	leader->y.fixed = toFIXED(130);
	leader->gravity = toFIXED(1) / 2;
	leader->bounce = 0;
	leader->motionFlags = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	sySoundPlay(SE_DUST_PUFF);
	leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdScriptWait(24);
	options.palette = SPRITE_COLOR_WHITE;
	options.update = fdEffectAdvanceMotion;
	options.startScaleX = toFIXED(16) / 25;
	options.startScaleY = toFIXED(16) / 25;
	for(i = 0; i <= TRIG_TURN / FACING_STEP; i++) {
		Sint32 angle;
		FIXED  velocity[3];

		angle = i * FACING_STEP;
		velocity[0] = syCos(angle);
		velocity[1] = 0;
		velocity[2] = sySin(angle);
		velocity[0] += velocity[0] >> 1;
		velocity[0] += syRandomBelow(toFIXED(3) / 4);
		velocity[0] += -(toFIXED(3) / 8);
		velocity[2] += syRandomBelow(toFIXED(3) / 8) + -(toFIXED(3) / 16);
		scFieldEffectSpawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
			velocity[0], velocity[1], velocity[2],
			EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1,
			&options);
	}
	sySoundPlay(SE_THUMP);
	fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetAnimationById(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_SIT);
	fdMapGridSetShake(5, 5, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	leader->gravity = toFIXED(1);
	leader->bounce = toFIXED(1) / 4;
	fdScriptWait(10);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdScriptEnd();
}

/* Sets up the Pound Cube's cave: the Pound Cube where Cannon dropped it, its
   cracks, and the landing of a fall, beside Cannon the first time. */
void scDehkanPlateauInitPoundCube(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32 entrance;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	leader->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
	if(syGameFlagGet(FLAG_POUND_CUBE_TAKEN) == 0) {
		scDehkanPlateauCreatePoundCube();
		if(syGameFlagGet(FLAG_CANNON_DROPPED_POUND_CUBE) != 0) {
			fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 15, 5);
			if(DehkanPlateauPoundCubeWork != NULL) {
				scFieldActorSetPosAndResetMotion(
					DehkanPlateauPoundCubeWork, toFIXED(248), 0, toFIXED(88));
				sySynch(1);
			}
		}
	}
	scPillarPlace(DehkanPlateauPoundCubeStakes);
	scDehkanPlateauRestorePoundCubeCracks();
	entrance = MainRoutineStateWork.entrance;
	if(entrance >= FIRST_LANDING_ENTRANCE) {
		scDehkanPlateauForgetFall();
		syGameFlagSet(FLAG_FELL_THROUGH(entrance));
	}
	if(fdScriptGetFlag(FLAG_CANNON_DROPPED_POUND_CUBE) != 0)
		fdActorSetPos(ACTOR_POUND_CUBE_CANNON, 0, 0);
	scDehkanPlateauDrawPoundCubeFallLanding();
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		if(MainRoutineStateWork.entrance >= POUND_CUBE_LANDINGS_FROM) {
			if(MainRoutineStateWork.entrance ==
					DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_TIGHTROPES_NORTH &&
				syGameFlagGet(FLAG_CANNON_DROPPED_POUND_CUBE) == 0)
				scDehkanPlateauCannonDropsPoundCube();
			else
				scDehkanPlateauLandFromFall();
		}
	}
	DehkanPlateauCrackX = 0;
	DehkanPlateauCrackZ = 0;
	sySetEvent(scDehkanPlateauForgetCrackOnLeaving, EVENT_PASS_TICK);
}

/* Creates the Pound Cube Cannon drops, drawn with the item's own icon. */
void scDehkanPlateauCreatePoundCube(void)
{
	ACTOR **slot = &DehkanPlateauPoundCubeWork;
	Uint8	clear = 0;
	ACTOR  *actor =
		((ACTOR *)syImportActorCreate(SPRITE_UNSEEN_MARKER, 0, 0, 0));

	*slot = actor;
	if(actor != 0) {
		FIELD_SPR	  *sprite = actor->sprite;
		ICON_TRANSFER *glyph;

		sprite->fullColor = 0;
		sprite->palette = 0;
		actor->motionFlags = clear;
		actor->retainSpriteCache = ACTOR_KEEP_SPRITE_VRAM;
		sprite->flags = clear;
		sprite->count = clear;
		glyph =
			syHeapSlotAllocate(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		mnIconDrawItem(ITEM_POUND_CUBE);
		syVRAMCacheLoad(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	}
}

/* Sets up Cannon's cave: its stake and crumbling pillar, Cannon met already if
   he is in the party, and the landing of a fall. */
void scDehkanPlateauInitCannon(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
		ACTOR_PRIORITY_IGNORES_RISES;
	if(syGameFlagGet(FLAG_DJINNI(DJINNI_CANNON)) != 0)
		syGameFlagSet(FLAG_CANNON_MET);
	scPillarPlace(DehkanPlateauCannonStakes);
	syCrumblingPillarPlace(DehkanPlateauCannonCrumblingPillar);
	if(syGameFlagGet(FLAG_CANNON_ROOM_STAKE_POUNDED) == 0) {
		scDehkanPlateauEnterLowerPath();
		sySynch(10);
	}
	scDehkanPlateauHideCannonBlocks();
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		if(MainRoutineStateWork.entrance >= CANNON_ROOM_LANDINGS_FROM) {
			if(MainRoutineStateWork.entrance <= LAST_LANDING_ENTRANCE)
				scDehkanPlateauLandFromFall();
		}
	}
}

Sint32 scPrepareMap(void)
{
	return 0;
}

#undef DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG

extern Uint32 FieldEffectLongScript[];

/* clang-format off */
#include "shared/dust_scatter_script.c"

#include "shared/dust_burst_script.c"

#include "shared/field_effect_duration_scripts.c"

/* The puffs of dust a crumbling pillar raises. */
Uint32 DehkanPlateauDustScript[] = {
	SC_WAIT(33),
	SC_SET(SW_HOOK, 0),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_RELEASE,
};

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

/* Each room's crumbling pillars, with the first of the two flags that */
/* remember each one cracked and crumbled. */
Sint16 DehkanPlateauFullMetalVestChestCrumblingPillars[] = {
	ACTOR_FULL_METAL_VEST_CHEST_FIRST_CRUMBLING_PILLAR, FLAG_PILLAR_CRACKED(0),
	ACTOR_FULL_METAL_VEST_CHEST_FIRST_CRUMBLING_PILLAR + 1, FLAG_PILLAR_CRACKED(1),
	ACTOR_FULL_METAL_VEST_CHEST_FIRST_CRUMBLING_PILLAR + 2, FLAG_PILLAR_CRACKED(2),
	ACTOR_LIST_END,
};
Sint16 DehkanPlateauThemisAxeChestCrumblingPillars[] = {
	ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR, FLAG_PILLAR_CRACKED(0),
	ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 1, FLAG_PILLAR_CRACKED(1),
	ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 2, FLAG_PILLAR_CRACKED(2),
	ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 3, FLAG_PILLAR_CRACKED(3),
	ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 4, FLAG_PILLAR_CRACKED(4),
	ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 5, FLAG_PILLAR_CRACKED(5),
	ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 6, FLAG_PILLAR_CRACKED(6),
	ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 7, FLAG_PILLAR_CRACKED(7),
	ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 8, FLAG_PILLAR_CRACKED(8),
	ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 9, FLAG_PILLAR_CRACKED(9),
	ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 10, FLAG_PILLAR_CRACKED(10),
	ACTOR_LIST_END,
};
Sint16 DehkanPlateauTightropesCrumblingPillars[] = {
	ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR, FLAG_PILLAR_CRACKED(0),
	ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR + 1, FLAG_PILLAR_CRACKED(1),
	ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR + 2, FLAG_PILLAR_CRACKED(2),
	ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR + 3, FLAG_PILLAR_CRACKED(3),
	ACTOR_LIST_END,
};
Sint16 DehkanPlateauCannonCrumblingPillar[] = {
	ACTOR_CANNON_ROOM_CRUMBLING_PILLAR, FLAG_PILLAR_CRACKED(0), ACTOR_LIST_END,
};

/* Each room's stakes, with the flag that remembers each one Pounded. */
Sint16 DehkanPlateauFullMetalVestChestStakes[] = {
	ACTOR_FULL_METAL_VEST_CHEST_STAKE, 1,
	FLAG_FULL_METAL_VEST_CHEST_STAKE_POUNDED, ACTOR_LIST_END,
};
Sint16 DehkanPlateauThemisAxeChestStakes[] = {
	ACTOR_THEMIS_AXE_CHEST_STAKE, 1, FLAG_THEMIS_AXE_CHEST_STAKE_POUNDED,
	ACTOR_LIST_END,
};
Sint16 DehkanPlateauTightropesStakes[] = {
	ACTOR_TIGHTROPES_STAKE, 1, FLAG_TIGHTROPES_STAKE_POUNDED,
	ACTOR_LIST_END,
};
Sint16 DehkanPlateauPoundCubeStakes[] = {
	ACTOR_POUND_CUBE_STAKE, 1, FLAG_POUND_CUBE_STAKE_POUNDED,
	ACTOR_LIST_END,
};
Sint16 DehkanPlateauCannonStakes[] = {
	ACTOR_CANNON_ROOM_STAKE, 1, FLAG_CANNON_ROOM_STAKE_POUNDED,
	ACTOR_LIST_END,
};

/* The stone pillars Move takes hold of. */
Sint16 DehkanPlateauThemisAxeChestPillars[] = {
	ACTOR_THEMIS_AXE_CHEST_PILLAR, ACTOR_LIST_END,
};
Sint16 DehkanPlateauTightropesPillars[] = {
	ACTOR_TIGHTROPES_PILLAR, ACTOR_LIST_END,
};

/* The rope bridge's stretch's vine pillars. */
Sint16 DehkanPlateauTightropesVinePillars[] = {
	ACTOR_TIGHTROPES_FIRST_VINE_PILLAR,  ACTOR_TIGHTROPES_SECOND_VINE_PILLAR,
	ACTOR_TIGHTROPES_THIRD_VINE_PILLAR,  ACTOR_TIGHTROPES_FOURTH_VINE_PILLAR,
	ACTOR_LIST_END,
};

/* The map pictures a room's crack is drawn with: cracked open, then as a */
/* hole. */
Uint16 DehkanPlateauFullMetalVestChestCrackPictures[] = {
	4, 33, 5, 33,
};
Uint16 DehkanPlateauThemisAxeChestCrackPictures[] = {
	68, 33, 69, 33,
};
Uint16 DehkanPlateauTightropesCrackPictures[] = {
	69, 51, 70, 51,
};
Uint16 DehkanPlateauPoundCubeCrackPictures[] = {
	65, 64, 64, 64,
};

/* The pictures of the hole under each crack, drawn as the leader falls. */
Uint16 DehkanPlateauFullMetalVestChestHolePictures[] = {
	1, 36, 2, 36, 3, 36, 4, 36,
	5, 36, 6, 36, 7, 36, 8, 36,
};
Uint16 DehkanPlateauThemisAxeChestHolePictures[] = {
	65, 36, 66, 36, 67, 36, 68, 36,
	69, 36, 70, 36, 71, 36, 72, 36,
};
Uint16 DehkanPlateauTightropesHolePictures[] = {
	66, 54, 67, 54, 68, 54, 69, 54,
	70, 54, 71, 54, 72, 54, 73, 54,
};
Uint16 DehkanPlateauPoundCubeHolePictures[] = {
	64, 66, 65, 66, 66, 66, 67, 66,
	68, 66,
};

/* Each room's cracks: whether it starts cracked, its metatile, its hole */
/* picture and where the hole is drawn below. */
Uint16 DehkanPlateauFullMetalVestChestCracks[][CRACK_FIELDS] = {
	{0, 76, 12, 0, 76, 48},
	{0, 77, 13, 0, 77, 49},
	{1, 79, 12, 0, 79, 48},
	{0, 79, 14, 1, 79, 50},
	{0, 81, 12, 0, 81, 48},
	{0, 118, 17, 4, 118, 53},
	{1, 119, 16, 0, 119, 52},
	{0, 119, 18, 7, 119, 54},
	{1, 120, 12, 7, 120, 48},
	{0, 120, 14, 7, 120, 50},
	{0, 120, 17, 2, 120, 53},
	{0, 122, 12, 7, 122, 48},
	{1, 122, 16, 0, 122, 52},
	{0, 122, 18, 5, 122, 54},
};
Uint16 DehkanPlateauThemisAxeChestCracks[][CRACK_FIELDS] = {
	{1, 105, 4, 0, 41, 36},
	{0, 109, 17, 0, 45, 49},
	{0, 110, 19, 3, 46, 51},
	{0, 111, 17, 3, 47, 49},
	{0, 112, 21, 0, 48, 53},
	{0, 113, 20, 3, 49, 52},
	{0, 114, 18, 0, 50, 50},
	{0, 105, 6, 0, 41, 38},
};
Uint16 DehkanPlateauTightropesCracks[][CRACK_FIELDS] = {
	{1, 91, 11, 0, 27, 43},
	{0, 92, 11, 0, 28, 43},
	{1, 93, 9, 0, 29, 41},
	{1, 93, 12, 0, 29, 44},
	{1, 109, 7, 0, 45, 39},
	{1, 112, 6, 0, 48, 38},
};
Uint16 DehkanPlateauPoundCubeCracks[][CRACK_FIELDS] = {
	{1, 13, 8, 2, 13, 72},
	{0, 13, 9, 0, 13, 73},
	{0, 13, 11, 1, 13, 75},
	{0, 13, 6, 0, 13, 70},
	{1, 15, 6, 0, 15, 70},
	{0, 15, 8, 0, 15, 72},
	{0, 15, 10, 1, 15, 74},
	{1, 14, 10, 0, 14, 74},
	{0, 14, 12, 0, 14, 76},
	{1, 15, 12, 0, 15, 76},
	{0, 16, 6, 0, 16, 70},
	{1, 16, 8, 2, 16, 72},
	{0, 16, 9, 0, 16, 73},
	{0, 16, 11, 0, 16, 75},
	{1, 18, 9, 0, 18, 73},
	{0, 18, 11, 0, 18, 75},
};

/* Where each fall lands in the rooms below. */
Uint16 DehkanPlateauFallLandingMetatiles[] = {
	4, 7, 68, 2, 5, 8, 69, 3,
	7, 7, 71, 2, 7, 9, 71, 4,
	9, 7, 73, 2, 40, 12, 104, 7,
	41, 11, 105, 6, 41, 13, 105, 8,
	42, 5, 106, 0, 42, 9, 106, 4,
	42, 12, 106, 7, 44, 7, 108, 2,
	44, 11, 108, 6, 44, 13, 108, 8,
	11, 23, 75, 18, 12, 25, 76, 20,
	13, 23, 77, 18, 14, 27, 78, 22,
	15, 26, 79, 21, 16, 24, 80, 19,
	30, 35, 94, 30, 26, 9, 90, 4,
	27, 9, 91, 4, 28, 7, 92, 2,
	28, 10, 92, 5, 12, 8, 76, 3,
	15, 7, 79, 2, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 30, 33, 94, 28,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
};

ACTOR *DehkanPlateauPoundCubeWork = NULL;

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 752, 0, 348, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(DehkanPlateauFullMetalVestChest),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_A, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_A),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_B, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_B),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_SOUTHEAST, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_SOUTHEAST),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_THEMIS_AXE_CHEST, DehkanPlateauThemisAxeChest, DEHKAN_PLATEAU_THEMIS_AXE_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST),
	SCENE_EXIT(6, DehkanPlateauFullMetalVestChest, 7),
	SCENE_EXIT(7, DehkanPlateauFullMetalVestChest, 6),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_C, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_C),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_D, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_D),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_E, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_E),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_F, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_F),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_G, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_G),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_A, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_A),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_B, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_B),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_C, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_C),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_D, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_D),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_E, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_E),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_F, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_F),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_G, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_G),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_H, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_H),
	SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_I, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_I),
	SCENE_EXITS(DehkanPlateauThemisAxeChest),
	SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST, DehkanPlateauFullMetalVestChest, DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST),
	SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_EAST),
	SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_SOUTHEAST_A, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_SOUTHEAST_A),
	SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_TIGHTROPES, DehkanPlateauTightropes, DEHKAN_PLATEAU_TIGHTROPES_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST),
	SCENE_EXIT(5, DehkanPlateauThemisAxeChest, 6),
	SCENE_EXIT(6, DehkanPlateauThemisAxeChest, 5),
	SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_NORTHWEST_A, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_NORTHWEST_A),
	SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_A, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_WEST_A),
	SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_NORTHWEST_B, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_NORTHWEST_B),
	SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_B, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_WEST_B),
	SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_C, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_WEST_C),
	SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_NORTHWEST_C, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_NORTHWEST_C),
	SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_SOUTHEAST_B, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_SOUTHEAST_B),
	SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_SOUTHEAST_C, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_SOUTHEAST_C),
	SCENE_EXITS(DehkanPlateauTightropes),
	SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_THEMIS_AXE_CHEST, DehkanPlateauThemisAxeChest, DEHKAN_PLATEAU_THEMIS_AXE_CHEST_FROM_DEHKAN_PLATEAU_TIGHTROPES),
	SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_MINT_CHEST_NORTH, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_TIGHTROPES_NORTH),
	SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_POUND_CUBE_NORTHEAST, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_TIGHTROPES_NORTHEAST),
	SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_POUND_CUBE_SOUTHWEST, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_TIGHTROPES_SOUTHWEST),
	SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_POUND_CUBE_SOUTH, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_TIGHTROPES_SOUTH),
	SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_DEHKAN_PLATEAU_TIGHTROPES),
	SCENE_EXIT(7, DehkanPlateauTightropes, 8),
	SCENE_EXIT(8, DehkanPlateauTightropes, 7),
	SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_MINT_CHEST_SOUTHWEST, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_TIGHTROPES_SOUTHWEST),
	SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_MINT_CHEST_SOUTH_A, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_TIGHTROPES_SOUTH_A),
	SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_MINT_CHEST_NORTHEAST, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_TIGHTROPES_NORTHEAST),
	SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_MINT_CHEST_SOUTH_B, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_TIGHTROPES_SOUTH_B),
	SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_POUND_CUBE_NORTHWEST, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_TIGHTROPES_NORTHWEST),
	SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_POUND_CUBE_NORTH, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_TIGHTROPES_NORTH),
	SCENE_EXITS(DehkanPlateauMintChest),
	SCENE_EXIT(DEHKAN_PLATEAU_MINT_CHEST_TO_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_A, DehkanPlateauFullMetalVestChest, DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_FROM_DEHKAN_PLATEAU_MINT_CHEST_WEST_A),
	SCENE_EXIT(DEHKAN_PLATEAU_MINT_CHEST_TO_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_B, DehkanPlateauFullMetalVestChest, DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_FROM_DEHKAN_PLATEAU_MINT_CHEST_WEST_B),
	SCENE_EXIT(DEHKAN_PLATEAU_MINT_CHEST_TO_DEHKAN_PLATEAU_TIGHTROPES_NORTH, DehkanPlateauTightropes, DEHKAN_PLATEAU_TIGHTROPES_FROM_DEHKAN_PLATEAU_MINT_CHEST_NORTH),
	SCENE_EXIT(DEHKAN_PLATEAU_MINT_CHEST_TO_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_SOUTHEAST, DehkanPlateauFullMetalVestChest, DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_FROM_DEHKAN_PLATEAU_MINT_CHEST_SOUTHEAST),
	SCENE_EXIT(DEHKAN_PLATEAU_MINT_CHEST_TO_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_SOUTHEAST_A, DehkanPlateauThemisAxeChest, DEHKAN_PLATEAU_THEMIS_AXE_CHEST_FROM_DEHKAN_PLATEAU_MINT_CHEST_SOUTHEAST_A),
	SCENE_EXIT(DEHKAN_PLATEAU_MINT_CHEST_TO_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_EAST, DehkanPlateauThemisAxeChest, DEHKAN_PLATEAU_THEMIS_AXE_CHEST_FROM_DEHKAN_PLATEAU_MINT_CHEST_EAST),
	SCENE_EXIT(40, DehkanPlateauMintChest, 42),
	SCENE_EXIT(41, DehkanPlateauMintChest, 43),
	SCENE_EXIT(42, DehkanPlateauMintChest, 40),
	SCENE_EXIT(43, DehkanPlateauMintChest, 41),
	SCENE_EXITS(DehkanPlateauPoundCube),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_TIGHTROPES_NORTHEAST, DehkanPlateauTightropes, DEHKAN_PLATEAU_TIGHTROPES_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTHEAST),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_WEST_A, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_WEST_A),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTHEAST_A, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTHEAST_A),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_TIGHTROPES_SOUTHWEST, DehkanPlateauTightropes, DEHKAN_PLATEAU_TIGHTROPES_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTHWEST),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_TIGHTROPES_SOUTH, DehkanPlateauTightropes, DEHKAN_PLATEAU_TIGHTROPES_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTH),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_SOUTHEAST_A, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTHEAST_A),
	SCENE_EXIT(7, DehkanPlateauPoundCube, 10),
	SCENE_EXIT(8, DehkanPlateauPoundCube, 9),
	SCENE_EXIT(9, DehkanPlateauPoundCube, 8),
	SCENE_EXIT(10, DehkanPlateauPoundCube, 7),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTHWEST_A, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTHWEST_A),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_WEST_B, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_WEST_B),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_SOUTHWEST_A, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTHWEST_A),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTHWEST_B, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTHWEST_B),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTH_A, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTH_A),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTH_B, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTH_B),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_SOUTHWEST_B, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTHWEST_B),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_WEST_C, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_WEST_C),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_SOUTHWEST_C, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTHWEST_C),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_SOUTH_A, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTH_A),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTH_C, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTH_C),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTH_D, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTH_D),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTHEAST_B, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTHEAST_B),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_SOUTH_B, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTH_B),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_EAST, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_EAST),
	SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_SOUTHEAST_B, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTHEAST_B),
	SCENE_EXITS(DehkanPlateauCannon),
	SCENE_EXIT(DEHKAN_PLATEAU_CANNON_TO_DEHKAN_PLATEAU_POUND_CUBE_WEST_A, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_CANNON_WEST_A),
	SCENE_EXIT(DEHKAN_PLATEAU_CANNON_TO_DEHKAN_PLATEAU_POUND_CUBE_NORTHEAST_A, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_CANNON_NORTHEAST_A),
	SCENE_EXIT(DEHKAN_PLATEAU_CANNON_TO_DEHKAN_PLATEAU_POUND_CUBE_SOUTHEAST_A, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_CANNON_SOUTHEAST_A),
	SCENE_EXIT(40, DehkanPlateauCannon, 41),
	SCENE_EXIT(41, DehkanPlateauCannon, 40),
	SCENE_EXIT(42, DehkanPlateauCannon, 43),
	SCENE_EXIT(43, DehkanPlateauCannon, 42),
	SCENE_EXITS_END,
};

/* The placements of a room with none of its own. */
ACTOR_PLACE DehkanPlateauPlacements[] = {
	{SCENE_TABLE_END},
};

/* Each room's actors (DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_ACTOR_ID and the */
/* rest); the rows at scene points stand where the map's points put them. */
ACTOR_PLACE DehkanPlateauFullMetalVestChestPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 920, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 656, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

ACTOR_PLACE DehkanPlateauThemisAxeChestPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 10, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 10, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 10, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 12, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 13, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

ACTOR_PLACE DehkanPlateauTightropesPlacements[] = {
	{SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_VINE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_VINE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_VINE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_VINE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 12, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 13, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 14, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 15, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 16, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 920, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

ACTOR_PLACE DehkanPlateauMintChestPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

ACTOR_PLACE DehkanPlateauPoundCubePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 536, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE DehkanPlateauCannonPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 872, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 872, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_CANNON), MOVEMENT_TYPE_STAND, 728, 0, 200, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* The cues of a room with none of its own. */
CUE DehkanPlateauCues[] = {
	{CUE_TABLE_END},
};

/* Each room's exits, cracks, pillars, unseen blocks and Cannon's flight. */
CUE DehkanPlateauFullMetalVestChestCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_STEP, DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_A, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauLeaveRoom)},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 30, CONDITION_ALWAYS, 6},
	{CUE_EXIT, 31, CONDITION_ALWAYS, 7},
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_FULL_METAL_VEST_CHEST_PILLARS, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauStepOnFullMetalVestChestPillars)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_FULL_METAL_VEST_CHEST_STAKE, FLAG_FULL_METAL_VEST_CHEST_STAKE_POUNDED, SCRIPT_REF(scDehkanPlateauPoundFullMetalVestChestStake)},
	{CUE_PUSHED, 0, FLAG_FULL_METAL_VEST_CHEST_PILLAR_IN_GAP, SCRIPT_REF(scDehkanPlateauDropFullMetalVestChestPillarIntoGap)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FULL_METAL_VEST_CHEST_PILLAR, FLAG_FULL_METAL_VEST_CHEST_PILLAR_IN_GAP, SCRIPT_REF(scDehkanPlateauDropFullMetalVestChestPillarIntoGap)},
	{CUE_STEP, METATILE_BEHAVIOR_FULL_METAL_VEST_CHEST_BLOCKS_ON, FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN, SCRIPT_REF(scDehkanPlateauShowBlocks)},
	{CUE_STEP, METATILE_BEHAVIOR_FULL_METAL_VEST_CHEST_BLOCKS_OFF, IF_FLAG_SET(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN), SCRIPT_REF(scDehkanPlateauHideBlocks)},
	{CUE_STEP, METATILE_BEHAVIOR_FULL_METAL_VEST_CHEST_OFF_CRACK, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauStepOffCrack)},
	{CUE_STEP, CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK + 1, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK + 2, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK + 3, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK + 4, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK + 5, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK + 6, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK + 7, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK + 8, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK + 9, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK + 10, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK + 11, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK + 12, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_FULL_METAL_VEST_CHEST_FIRST_CRACK + 13, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_TABLE_END},
};

CUE DehkanPlateauThemisAxeChestCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
	{CUE_STEP, DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_SOUTHEAST_A, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauLeaveRoom)},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 12, CONDITION_ALWAYS, 6},
	{CUE_STEP, METATILE_BEHAVIOR_CANNON_FLEES_OVER_CRACKS, FLAG_CANNON_FLED_OVER_CRACKS, SCRIPT_REF(scDehkanPlateauCannonFleesOverCracks)},
	{CUE_STEP, METATILE_BEHAVIOR_CANNON_FLEES_EAST, FLAG_CANNON_FLED_EAST, SCRIPT_REF(scDehkanPlateauCannonFleesEast)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_THEMIS_AXE_CHEST_PILLAR_FROM_EAST, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauStepOnThemisAxeChestPillars)},
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_THEMIS_AXE_CHEST_PILLARS, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauStepOnThemisAxeChestPillars)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_THEMIS_AXE_CHEST_STAKE, FLAG_THEMIS_AXE_CHEST_STAKE_POUNDED, SCRIPT_REF(scDehkanPlateauPoundThemisAxeChestStake)},
	{CUE_PUSH, 0, FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED, SCRIPT_REF(scDehkanPlateauLiftThemisAxeChestPillar)},
	{CUE_PUSHED, 0, FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED, SCRIPT_REF(scDehkanPlateauPlaceThemisAxeChestPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_THEMIS_AXE_CHEST_PILLAR, FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED, SCRIPT_REF(scDehkanPlateauLiftThemisAxeChestPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THEMIS_AXE_CHEST_PILLAR, FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED, SCRIPT_REF(scDehkanPlateauPlaceThemisAxeChestPillar)},
	{CUE_STEP, METATILE_BEHAVIOR_THEMIS_AXE_CHEST_BLOCKS_ON, FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN, SCRIPT_REF(scDehkanPlateauShowBlocks)},
	{CUE_STEP, METATILE_BEHAVIOR_THEMIS_AXE_CHEST_BLOCKS_OFF, IF_FLAG_SET(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN), SCRIPT_REF(scDehkanPlateauHideBlocks)},
	{CUE_STEP, METATILE_BEHAVIOR_THEMIS_AXE_CHEST_OFF_CRACK, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauStepOffCrack)},
	{CUE_STEP, CUE_THEMIS_AXE_CHEST_FIRST_CRACK, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_THEMIS_AXE_CHEST_FIRST_CRACK + 1, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_THEMIS_AXE_CHEST_FIRST_CRACK + 2, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_THEMIS_AXE_CHEST_FIRST_CRACK + 3, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_THEMIS_AXE_CHEST_FIRST_CRACK + 4, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_THEMIS_AXE_CHEST_FIRST_CRACK + 5, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_THEMIS_AXE_CHEST_FIRST_CRACK + 6, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_THEMIS_AXE_CHEST_FIRST_CRACK + 7, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_TABLE_END},
};

CUE DehkanPlateauTightropesCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT, 24, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 25, CONDITION_ALWAYS, 8},
	{CUE_STEP, METATILE_BEHAVIOR_CANNON_LEAPS_AWAY, FLAG_CANNON_LEAPT_AWAY, SCRIPT_REF(scDehkanPlateauCannonLeapsAway)},
	{CUE_STEP, METATILE_BEHAVIOR_CANNON_POUNDS_STAKE, FLAG_CANNON_POUNDED_STAKE, SCRIPT_REF(scDehkanPlateauCannonPoundsStake)},
	{CUE_STEP, METATILE_BEHAVIOR_CANNON_CRACKS_PILLARS, FLAG_CANNON_CRACKED_PILLARS, SCRIPT_REF(scDehkanPlateauCannonCracksPillars)},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_TIGHTROPES_ROPE_LOW_POST, FLAG_TIGHTROPES_ROPE_LASHED, 0},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_TIGHTROPES_STAKE, FLAG_TIGHTROPES_STAKE_POUNDED, SCRIPT_REF(scDehkanPlateauPoundTightropesStake)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauLiftTightropesPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauPlaceTightropesPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauLiftTightropesPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauPlaceTightropesPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_SECOND_PILLAR, FLAG_TIGHTROPES_SECOND_PILLAR_PLACED, SCRIPT_REF(scDehkanPlateauMoveTightropesSecondPillar)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_TIGHTROPES_SECOND_PILLAR, FLAG_TIGHTROPES_SECOND_PILLAR_PLACED, SCRIPT_REF(scDehkanPlateauPushTightropesSecondPillar)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_VINE_PILLAR_EAST, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauPushVinePillar)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_VINE_PILLAR_WEST, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauPushVinePillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_FIRST_VINE_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauLiftTightropesVinePillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_SECOND_VINE_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauLiftTightropesVinePillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_THIRD_VINE_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauLiftTightropesVinePillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_FOURTH_VINE_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauLiftTightropesVinePillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_FIRST_VINE_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauFinishMovingVinePillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_SECOND_VINE_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauFinishMovingVinePillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_THIRD_VINE_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauFinishMovingVinePillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_FOURTH_VINE_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauFinishMovingVinePillar)},
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_TIGHTROPES_PILLARS, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauStepOnTightropesPillars)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), CUE_TIGHTROPES_PILLARS_WEST, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauStepOnTightropesPillars)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_TIGHTROPES_PILLARS_FROM_WEST, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauStepOnTightropesPillars)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_TIGHTROPES_PILLARS_FROM_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauStepOnTightropesPillars)},
	{CUE_STEP, METATILE_BEHAVIOR_TIGHTROPES_BLOCKS_ON, FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN, SCRIPT_REF(scDehkanPlateauShowBlocks)},
	{CUE_STEP, METATILE_BEHAVIOR_TIGHTROPES_BLOCKS_OFF, IF_FLAG_SET(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN), SCRIPT_REF(scDehkanPlateauHideBlocks)},
	{CUE_STEP, CUE_TIGHTROPES_FIRST_CRACK, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_TIGHTROPES_FIRST_CRACK + 1, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_TIGHTROPES_FIRST_CRACK + 2, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_TIGHTROPES_FIRST_CRACK + 3, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_TIGHTROPES_FIRST_CRACK + 4, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_TIGHTROPES_FIRST_CRACK + 5, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_TABLE_END},
};

CUE DehkanPlateauMintChestCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT, 40, CONDITION_ALWAYS, 40},
	{CUE_EXIT, 41, CONDITION_ALWAYS, 41},
	{CUE_EXIT, 42, CONDITION_ALWAYS, 42},
	{CUE_EXIT, 43, CONDITION_ALWAYS, 43},
	{CUE_STEP, METATILE_BEHAVIOR_MINT_CHEST_BLOCKS_ON, FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN, SCRIPT_REF(scDehkanPlateauShowBlocks)},
	{CUE_STEP, METATILE_BEHAVIOR_MINT_CHEST_BLOCKS_OFF, IF_FLAG_SET(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN), SCRIPT_REF(scDehkanPlateauHideBlocks)},
	{CUE_TABLE_END},
};

CUE DehkanPlateauPoundCubeCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_STEP, CUE_POUND_CUBE_CANNON_CRACK, FLAG_CANNON_DROPPED_POUND_CUBE, SCRIPT_REF(scDehkanPlateauCannonPoundsLeaderThrough)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_POUND_CUBE_STAKE, FLAG_POUND_CUBE_STAKE_POUNDED, SCRIPT_REF(scDehkanPlateauPoundPoundCubeStake)},
	{CUE_EXAMINE, METATILE_BEHAVIOR_POUND_CUBE, FLAG_POUND_CUBE_TAKEN, SCRIPT_REF(scDehkanPlateauTakePoundCube)},
	{CUE_STEP, CUE_POUND_CUBE_FIRST_CRACK, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_POUND_CUBE_FIRST_CRACK + 1, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_POUND_CUBE_FIRST_CRACK + 2, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_POUND_CUBE_FIRST_CRACK + 3, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_POUND_CUBE_FIRST_CRACK + 4, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_POUND_CUBE_CANNON_CRACK, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_POUND_CUBE_FIRST_CRACK + 6, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_POUND_CUBE_FIRST_CRACK + 7, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_POUND_CUBE_FIRST_CRACK + 8, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_POUND_CUBE_FIRST_CRACK + 9, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_POUND_CUBE_FIRST_CRACK + 10, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_POUND_CUBE_FIRST_CRACK + 11, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_POUND_CUBE_FIRST_CRACK + 12, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_POUND_CUBE_FIRST_CRACK + 13, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_POUND_CUBE_FIRST_CRACK + 14, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_STEP, CUE_POUND_CUBE_FIRST_CRACK + 15, FLAG_STANDING_ON_OPENED_CRACK, SCRIPT_REF(scDehkanPlateauStepOnCrack)},
	{CUE_TABLE_END},
};

CUE DehkanPlateauCannonCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 40, CONDITION_ALWAYS, 40},
	{CUE_EXIT, 41, CONDITION_ALWAYS, 41},
	{CUE_EXIT, 42, CONDITION_ALWAYS, 42},
	{CUE_EXIT, 43, CONDITION_ALWAYS, 43},
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_CANNON_ROOM_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauStepOnCannonPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_CANNON_ROOM_STAKE, FLAG_CANNON_ROOM_STAKE_POUNDED, SCRIPT_REF(scDehkanPlateauPoundCannonStake)},
	{CUE_STEP, METATILE_BEHAVIOR_LOWER_FLOOR, FLAG_STAKE_METATILE_CHANGED, SCRIPT_REF(scDehkanPlateauEnterLowerPath)},
	{CUE_STEP, METATILE_BEHAVIOR_OFF_LOWER_FLOOR, IF_FLAG_SET(FLAG_STAKE_METATILE_CHANGED), SCRIPT_REF(scDehkanPlateauLeaveLowerPath)},
	{CUE_STEP, METATILE_BEHAVIOR_CANNON_ROOM_BLOCKS_ON, FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN, SCRIPT_REF(scDehkanPlateauShowBlocks)},
	{CUE_STEP, METATILE_BEHAVIOR_CANNON_ROOM_BLOCKS_OFF, IF_FLAG_SET(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN), SCRIPT_REF(scDehkanPlateauHideBlocks)},
	{CUE_STEP, METATILE_BEHAVIOR_LEDGE_TO_CANNON, FLAG_CANNON_MET, SCRIPT_REF(scDehkanPlateauMeetCannon)},
	{CUE_TALK, ACTOR_CANNON, CONDITION_ALWAYS, SCRIPT_REF(scDehkanPlateauFightCannon)},
	{CUE_TABLE_END},
};
