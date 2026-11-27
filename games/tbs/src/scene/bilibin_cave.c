/* The Bilibin Cave Entrance and Bilibin Cave are two areas between Bilibin and
   Imil that share this overlay.

   The Bilibin Cave Entrance is SceneIdBilibinCaveEntrance. Bilibin Cave is
   SceneIdBilibinCaveRapier and, beyond it, SceneIdBilibinCaveIce, whose far
   end opens onto the World Map farther north.

   No one speaks here. Each scene is a run of Psynergy puzzles. The getters pick
   its tables by MainRoutineStateWork.next, and scInit restores what the party
   has already solved in the scene being entered.

   The Bilibin Cave Entrance is a clearing below a cliff, entered from the World
   Map, with a waterfall to the west. The cave has two mouths in the cliff: a
   low one at ground level (metatile (21, 14)) and a high one on a ledge to the
   north-east (metatile (27, 7)). A sprout grows at the foot of the cliff
   (ACTOR_SPROUT, metatile (8, 13)); Growth turns it into a vine Isaac can climb
   to the ledge, and in by the high mouth (scBilibinCaveGrowPlant).
   FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN keeps it grown until the party leaves
   the Bilibin Cave Entrance, into the cave or out to the World Map, since both
   are other areas.

   The cliff-top path passes just above the low mouth, and its metatiles there
   switch the mouth's way in off and on again (scBilibinCaveCloseLowerCaveMouth,
   scBilibinCaveOpenLowerCaveMouth).

   Inside, in SceneIdBilibinCaveRapier, the high mouth leads to the chest of
   the Elven Rapier. Coming back later with Reveal, the party finds a way
   through the wall beside that chest (scBilibinCaveRevealHiddenPath), and
   scBilibinCaveHideRevealedPath closes it again when Reveal wears off. Beyond
   is a small room of two torches, the west one cold and the east one lit, with
   a little fire burning in the doorway between them (ACTOR_RAPIER_ROOM_FIRE).
   Douse puts it out for good (FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED), and the
   doorway leads to the cave's two last chests, a Psy Crystal and Water of Life.

   SceneIdBilibinCaveIce is the cold part of the cave on the way to Imil, full
   of ice pillars, with puddles, a fire and burning torches. Frost freezes a
   puddle into an ice pillar, but next to a fire or a torch the ice melts
   straight back (scBilibinCaveMeltFrostBack).

   The first puddle lies beside a little fire (metatile (17, 14)). Douse on the
   fire puts it out (FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED), and the puddle
   freezes by itself into a pillar, whose top the party hops across.

   The second puddle lies beside a lit torch near the Vial's chest, which cannot
   be moved, and Frost on it always melts back.

   The third puddle starts as an ice pillar (metatile (36, 23)), east of a pair
   of torches that can be pushed or moved with Move (ACTOR_THIRD_TORCH and
   ACTOR_FOURTH_TORCH). Set beside it, on metatile (35, 23), either torch melts
   it into a puddle, and the metatiles north and south of it become hop squares,
   so the party can jump over the puddle and go on. Take the torch away and it
   freezes again (scBilibinCaveUpdateThirdPuddleFromTorches).

   FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR and
   FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR remember which torch stands there,
   and FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED that the pillar has melted. So
   coming back from SceneIdBilibinCaveRapier puts the torches and the pillar
   back. Leaving Bilibin Cave resets them.

   Nine more pieces of ice only stand there: five pillars from
   ACTOR_FIRST_ICE_PILLAR and four lower pieces from ACTOR_FIRST_LOW_ICE.

   Snow has blown in at the cave's mouth to the World Map, the way out to Imil,
   around BILIBIN_CAVE_ICE_FROM_WORLD_MAP. Walking onto it Isaac starts leaving
   footprints (scBilibinCaveStartFootprints), and walking off it stops them
   (scBilibinCaveStopFootprints).

   Its chests hold a Vial, a Psy Crystal and Water of Life. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script.h"
#include "field/map_grid_scroll.h"
#include "field/pushed_actor.h"
#include "graphics/graphics.h"
#include "game/system_call.h"

extern SCENE_ENTRANCE BilibinCaveEntrancesOther[];
extern SCENE_ENTRANCE BilibinCaveEntranceEntrances[];
extern SCENE_ENTRANCE BilibinCaveRapierEntrances[];
extern SCENE_ENTRANCE BilibinCaveIceEntrances[];
extern Uint32		  BilibinCaveExits[];
extern ACTOR_PLACE	  BilibinCaveActorPlacementsOther[];
extern ACTOR_PLACE	  BilibinCaveEntranceActorPlacements[];
extern ACTOR_PLACE	  BilibinCaveRapierActorPlacements[];
extern ACTOR_PLACE	  BilibinCaveIceActorPlacements[];
extern CUE			  BilibinCaveCuesOther[];
extern CUE			  BilibinCaveEntranceCues[];
extern CUE			  BilibinCaveRapierCues[];
extern CUE			  BilibinCaveIceCues[];

/* In Bilibin Cave's ice room, a movable torch has melted the third puddle's ice
   pillar. */
#define FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED	 FLAG_AREA_TEMP_02
/* In Bilibin Cave's ice room, the third torch stands beside the third puddle's
   ice pillar. */
#define FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR	 FLAG_AREA_TEMP_03
/* In Bilibin Cave's ice room, the fourth torch stands beside the third puddle's
   ice pillar. */
#define FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR FLAG_AREA_TEMP_04
/* At the Bilibin Cave Entrance, Growth has grown the sprout into a vine. */
#define FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN	 FLAG_AREA_TEMP_05

static void scBilibinCaveRestoreEntrance(void);
static void scBilibinCaveRestoreRapierRoom(void);
static void scBilibinCaveRestoreIceRoom(void);
static void scBilibinCaveEmptyActorSetupHook(Sint32 actorId);

/* Actors of the Bilibin Cave Entrance (SceneIdBilibinCaveEntrance). */
enum {
	/* The sprout Growth grows into a vine. */
	ACTOR_SPROUT = ACTOR_FIRST_PLACED
};

/* Actors of SceneIdBilibinCaveRapier, numbered by the rows of
   BilibinCaveRapierActorPlacements. */
enum {
	/* A torch west of the fire. It is cold (TORCH_ANIMATION_UNLIT) where its
	   twin burns. */
	ACTOR_RAPIER_ROOM_WEST_TORCH = ACTOR_FIRST_PLACED,
	/* A little fire in the doorway between the two torches, which Douse puts
	   out. */
	ACTOR_RAPIER_ROOM_FIRE,
	/* A torch east of the fire. */
	ACTOR_RAPIER_ROOM_EAST_TORCH
};

/* Actors of SceneIdBilibinCaveIce, numbered by the rows of
   BilibinCaveIceActorPlacements. */
enum {
	/* A little fire beside the first puddle, which Douse puts out. */
	ACTOR_ICE_ROOM_FIRE = ACTOR_FIRST_PLACED,
	/* Six torches, 9 to 14. The second stands beside ACTOR_SECOND_PUDDLE. */
	ACTOR_FIRST_TORCH = 9,
	/* The third torch, moved by Move or pushed. */
	ACTOR_THIRD_TORCH = 11,
	/* The fourth torch, moved by Move or pushed. */
	ACTOR_FOURTH_TORCH = 12,
	/* A puddle beside ACTOR_ICE_ROOM_FIRE. It freezes into a pillar the party
	   hops across once the fire is out. */
	ACTOR_FIRST_PUDDLE = 15,
	/* A puddle beside a torch that stays put, so it never stays frozen. */
	ACTOR_SECOND_PUDDLE = 16,
	/* A puddle frozen into an ice pillar, melted while a torch stands on
	   metatile (35, 23). */
	ACTOR_THIRD_PUDDLE = 17,
	/* Five ice pillars, 18 to 22 (PUDDLE_ANIMATION_PILLAR). Nothing acts on
	   them. */
	ACTOR_FIRST_ICE_PILLAR = 18,
	/* Four pieces of ice, 23 to 26, held at the second step of freezing
	   (PUDDLE_ANIMATION_LOW_ICE). Nothing acts on them. */
	ACTOR_FIRST_LOW_ICE = 23
};

/* SPRITE_FIRE's flickering flame. */
enum { FIRE_ANIMATION_BURNING = 2 };

/* Metatile behaviors the three maps' cues answer besides the exits. */
typedef enum BILIBIN_CAVE_METATILE_BEHAVIOR {
	/* At the entrance, the cliff-top path just above the low cave mouth,
	   metatiles (20, 13..14) and (22, 13..14). */
	METATILE_BEHAVIOR_BILIBIN_CAVE_ABOVE_LOW_MOUTH = 10,
	/* At the entrance, the path on either side of that stretch, metatiles (19,
	   12..13) and (23, 12..13). */
	METATILE_BEHAVIOR_BILIBIN_CAVE_BESIDE_LOW_MOUTH = 11,
	/* The Elven Rapier's chest in SceneIdBilibinCaveRapier, metatile (35,
	   28). */
	METATILE_BEHAVIOR_BILIBIN_CAVE_ELVEN_RAPIER_CHEST = 100,
	/* The chests of SceneIdBilibinCaveIce, at (32, 16), (53, 36) and (52,
	   37). */
	METATILE_BEHAVIOR_BILIBIN_CAVE_VIAL_CHEST = 100,
	METATILE_BEHAVIOR_BILIBIN_CAVE_PSY_CRYSTAL_CHEST = 101,
	METATILE_BEHAVIOR_BILIBIN_CAVE_WATER_OF_LIFE_CHEST = 102,
	/* The dotted track the two movable torches are pushed along, round (31..33,
	   19..23). */
	METATILE_BEHAVIOR_BILIBIN_CAVE_TORCH_TRACK = 10,
	/* The track's east end, metatiles (34..36, 23), from which walking west
	   pushes a torch. */
	METATILE_BEHAVIOR_BILIBIN_CAVE_TORCH_TRACK_END = 11,
	/* The snow blown in at the mouth to the World Map, round (32..34,
	   32..35). */
	METATILE_BEHAVIOR_BILIBIN_CAVE_SNOW = 20,
	/* The metatiles round the snow's edge. */
	METATILE_BEHAVIOR_BILIBIN_CAVE_SNOW_EDGE = 21
} BILIBIN_CAVE_METATILE_BEHAVIOR;

/* Spare metatiles near the top-left corner of the entrance's and the ice room's
   maps, whose behaviors are copied where the low mouth, the vine and the
   puddles change, as (column, row). */
#define LOW_MOUTH_OPEN_METATILE			   0, 0 /* the low mouth's way into the cave */
/* plain ground on the cliff-top path */
#define LOW_MOUTH_CLOSED_METATILE		   1, 0
#define VINE_METATILE					   31, 0 /* a vine to climb */
#define FIRST_PILLAR_METATILE			   0, 0 /* a hop square on the ice pillar's top */
/* hop squares beside the puddle */
#define THIRD_PUDDLE_NORTH_HOP_METATILE	   0, 1
#define THIRD_PUDDLE_SOUTH_HOP_METATILE	   0, 2
/* plain ground beside the pillar */
#define THIRD_PUDDLE_NORTH_GROUND_METATILE 1, 1
#define THIRD_PUDDLE_SOUTH_GROUND_METATILE 1, 2

/* A script command that draws the puddle it runs on without a shadow. */
static Sint32 scBilibinCaveHideScriptedShadow(ACTOR *actor)
{
	fdActorSetSprFlags(actor, 0);
	return 0;
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_BILIBIN_CAVE_ENTRANCE) {
		return BilibinCaveEntranceEntrances;
	}
	if(sceneId == SCENE_BILIBIN_CAVE_RAPIER) {
		return BilibinCaveRapierEntrances;
	}
	if(sceneId == SCENE_BILIBIN_CAVE_ICE) {
		return BilibinCaveIceEntrances;
	}
	return BilibinCaveEntrancesOther;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return BilibinCaveExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_BILIBIN_CAVE_ENTRANCE) {
		return BilibinCaveEntranceActorPlacements;
	}
	if(sceneId == SCENE_BILIBIN_CAVE_RAPIER) {
		return BilibinCaveRapierActorPlacements;
	}
	if(sceneId == SCENE_BILIBIN_CAVE_ICE) {
		return BilibinCaveIceActorPlacements;
	}
	return BilibinCaveActorPlacementsOther;
}

const CUE *scGetCues(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_BILIBIN_CAVE_ENTRANCE) {
		return BilibinCaveEntranceCues;
	}
	if(sceneId == SCENE_BILIBIN_CAVE_RAPIER) {
		return BilibinCaveRapierCues;
	}
	if(sceneId == SCENE_BILIBIN_CAVE_ICE) {
		return BilibinCaveIceCues;
	}
	return BilibinCaveCuesOther;
}

/* Nothing visible happens when Isaac, on the grassy path along the cliff top
   from the vine toward the high cave mouth at the Bilibin Cave Entrance, steps
   onto the stretch drawn just above the low mouth, metatiles (20, 13..14) and
   (22, 13..14). The game quietly turns off the low mouth's way into the cave,
   metatile (21, 14), which lies between them in the map, so that walking along
   the path over it does not take him in. */
static void scBilibinCaveCloseLowerCaveMouth(void)
{
	fdMapGridCopyMetatileAttributes(LOW_MOUTH_CLOSED_METATILE, 1, 1, 21, 14);
}

/* Nothing visible happens when Isaac walks on along the cliff-top path past
   that stretch at the Bilibin Cave Entrance, metatiles (19, 12..13) and (23,
   12..13). The game turns the low mouth's way into the cave, metatile (21, 14),
   back on. */
static void scBilibinCaveOpenLowerCaveMouth(void)
{
	fdMapGridCopyMetatileAttributes(LOW_MOUTH_OPEN_METATILE, 1, 1, 21, 14);
}

/* Reveal in Bilibin Cave, near the Elven Rapier's chest, shows a way through
   the wall beside it, metatiles (32, 24..26), to the room where a little fire
   burns between two torches. */
static void scBilibinCaveRevealHiddenPath(void)
{
	fdMapGridCopyMetatilesTo(111, 37, 97, 21, 1, 3);
	{
		Sint32 dstMetatileX = 32;
		Sint32 dstMetatileZ = 24;

		fdMapGridCopyMetatileAttributes(
			46, 38, 3, 2, dstMetatileX, dstMetatileZ);
	}
}

/* As Reveal wears off in Bilibin Cave, the way through the wall beside the
   Elven Rapier's chest is hidden again. */
static void scBilibinCaveHideRevealedPath(void)
{
	fdMapGridCopyMetatilesTo(95, 21, 97, 21, 1, 3);
	{
		Sint32 dstMetatileX = 32;
		Sint32 dstMetatileZ = 25;

		fdMapGridCopyMetatileAttributes(
			46, 38, 3, 1, dstMetatileX, dstMetatileZ);
	}
}

/* Douse on the little fire in the doorway of the room of two torches, past the
   way Reveal shows beside the Elven Rapier's chest, puts it out for good. A
   rain cloud gathers over the doorway, the flame goes out, and the doorway on
   to the cave's last two chests is open. */
static void scBilibinCaveDouseRapierRoomFire(void)
{
	fdScriptStart();
	fdActorSetPos(ACTOR_RAPIER_ROOM_FIRE, 0, 0);
	gmFlagSet(FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED);
	fdScriptEnd();
}

/* Douse on the little fire in the cold part of Bilibin Cave (metatile (17, 14))
   puts it out, and the puddle beside it freezes by itself into a tall ice
   pillar, whose top the party hops across. */
static void scBilibinCaveDouseIceRoomFire(void)
{
	fdScriptStart();
	fdActorSetPos(ACTOR_ICE_ROOM_FIRE, 0, 0);
	gmFlagSet(FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED);
	fdScriptWait(40);
	/* The pillar is drawn under the party, and its metatile becomes a hop
	   square. */
	fdActorSetAnimationAndWait(ACTOR_FIRST_PUDDLE, PUDDLE_ANIMATION_FREEZE);
	fdActorGet(ACTOR_FIRST_PUDDLE)->motionFlg = 0;
	fdActorGet(ACTOR_FIRST_PUDDLE)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetSprPriority(ACTOR_FIRST_PUDDLE, 2);
	fdMapGridCopyMetatileAttributes(FIRST_PILLAR_METATILE, 1, 1, 18, 14);
	fdScriptEnd();
}

/* Frost on a puddle in Bilibin Cave that a fire or torch beside it keeps warm
   forms ice on it for a moment, which melts straight back into water. */
static inline void scBilibinCaveMeltFrostBack(Sint32 actorId)
{
	fdScriptStart();
	fdActorSetSprColById(actorId, SPRITE_COLOR_NORMAL);
	fdScriptWait(40);
	sySndPlay(SE_HIDDEN_PASSAGE_OPEN);
	fdActorSetAnimationAndWait(actorId, PUDDLE_ANIMATION_MELT);
	fdScriptEnd();
}

/* Frost on the puddle beside the little fire in the cold part of Bilibin Cave
   (metatile (18, 14)), while the fire burns, makes ice that melts straight
   back. Douse on the fire is what freezes it. */
static void scBilibinCaveFreezeFirstPuddle(void)
{
	scBilibinCaveMeltFrostBack(ACTOR_FIRST_PUDDLE);
}

/* Frost on the puddle beside the lit torch near the Vial's chest in the cold
   part of Bilibin Cave makes ice that melts straight back. The torch cannot be
   moved, so this puddle never freezes. */
static void scBilibinCaveFreezeSecondPuddle(void)
{
	scBilibinCaveMeltFrostBack(ACTOR_SECOND_PUDDLE);
}

/* Frost on the puddle a pushed torch has melted, at metatile (36, 23) in the
   cold part of Bilibin Cave, makes ice that melts straight back while the torch
   stands beside it (FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED). */
static void scBilibinCaveFreezeThirdPuddle(void)
{
	scBilibinCaveMeltFrostBack(ACTOR_THIRD_PUDDLE);
}

/* The metatile beside the third puddle's ice pillar where a torch melts it. */
#define TORCH_BY_PILLAR_COLUMN 35
#define TORCH_BY_PILLAR_ROW	   23

/* Move on either of the pair of torches west of the ice pillar in the cold part
   of Bilibin Cave slides it a metatile. Set beside the pillar, on metatile (35,
   23), a torch melts the pillar into a puddle, and the metatiles north and
   south of it, (36, 22) and (36, 24), become hop squares to jump it by. Taken
   away again, the puddle freezes back into a pillar, and the hop squares are
   plain ground once more. The same work follows every push. */
static void scBilibinCaveUpdateThirdPuddleFromTorches(void)
{
	ACTOR *firstActor;
	ACTOR *secondActor;
	Sint32 dstMetatileX;

	firstActor = fdActorGet(ACTOR_THIRD_TORCH);
	secondActor = fdActorGet(ACTOR_FOURTH_TORCH);
	if(FIELD_METATILE(firstActor->x.fixed) == TORCH_BY_PILLAR_COLUMN &&
		(FIELD_METATILE(firstActor->z.fixed)) == TORCH_BY_PILLAR_ROW) {
		gmFlagSet(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR);
	} else {
		gmFlagClear(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR);
	}
	if(FIELD_METATILE(secondActor->x.fixed) == TORCH_BY_PILLAR_COLUMN &&
		(FIELD_METATILE(secondActor->z.fixed)) == TORCH_BY_PILLAR_ROW) {
		gmFlagSet(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR);
	} else {
		gmFlagClear(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR);
	}
	if(gmFlagGet(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR) != 0 ||
		gmFlagGet(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR) != 0) {
		if(fdScriptGetFlag(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED) == 0) {
			fdScriptStart();
			fdScriptWait(40);
			sySndPlay(SE_HIDDEN_PASSAGE_OPEN);
			dstMetatileX = 36;
			fdActorSetAnimationAndWait(
				ACTOR_THIRD_PUDDLE, PUDDLE_ANIMATION_MELT);
			fdMapGridCopyMetatileAttributes(
				THIRD_PUDDLE_NORTH_HOP_METATILE, 1, 1, dstMetatileX, 22);
			fdMapGridCopyMetatileAttributes(
				THIRD_PUDDLE_SOUTH_HOP_METATILE, 1, 1, dstMetatileX, 24);
			fdScriptEnd();
		}
		gmFlagSet(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED);
	} else {
		if(fdScriptGetFlag(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED) != 0) {
			fdScriptStart();
			fdScriptWait(40);
			sySndPlay(SE_SHIMMER);
			dstMetatileX = 36;
			fdActorSetAnimationAndWait(
				ACTOR_THIRD_PUDDLE, PUDDLE_ANIMATION_FREEZE);
			fdMapGridCopyMetatileAttributes(
				THIRD_PUDDLE_NORTH_GROUND_METATILE, 1, 1, dstMetatileX, 22);
			fdMapGridCopyMetatileAttributes(
				THIRD_PUDDLE_SOUTH_GROUND_METATILE, 1, 1, dstMetatileX, 24);
			fdScriptEnd();
		}
		gmFlagClear(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED);
	}
}

/* Growth on the sprout at the foot of the cliff at the Bilibin Cave Entrance
   (metatile (8, 13)) grows it into a vine up the cliff, which Isaac can climb
   to the ledge and the high cave mouth. */
static void scBilibinCaveGrowPlant(void)
{
	fdMapGridCopyMetatileAttributes(VINE_METATILE, 1, 1, 8, 13);
	gmFlagSet(FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN);
}

/* Walking onto the snow blown in at the cave mouth toward Imil, in the cold
   part of Bilibin Cave (metatiles (32..34, 32..35)), Isaac starts leaving
   footprints. */
static void scBilibinCaveStartFootprints(void)
{
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	grid->footprints = 1;
}

/* Walking off the snow at the cave mouth toward Imil, onto the metatiles round
   its edge, Isaac stops leaving footprints. */
static void scBilibinCaveStopFootprints(void)
{
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	grid->footprints = 0;
}

/* Sets up the scene being entered, with its puzzles already solved. */
Sint32 scInit(void)
{
	Sint16 variant = MainRoutineStateWork.next;

	if(variant == SCENE_BILIBIN_CAVE_ENTRANCE) {
		scBilibinCaveRestoreEntrance();
	} else if(variant == SCENE_BILIBIN_CAVE_RAPIER) {
		scBilibinCaveRestoreRapierRoom();
	} else if(variant == SCENE_BILIBIN_CAVE_ICE) {
		scBilibinCaveRestoreIceRoom();
	}
	return 0;
}

/* Sets up the Bilibin Cave Entrance, with the vine Growth has grown and its
   metatile. */
static void scBilibinCaveRestoreEntrance(void)
{
	if(gmFlagGet(FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN) != 0) {
		fdMapGridCopyMetatileAttributes(VINE_METATILE, 1, 1, 8, 13);
		fdActorSetAnimation(ACTOR_SPROUT, SPROUT_ANIMATION_VINE);
	}
}

/* Sets up Bilibin Cave's SceneIdBilibinCaveRapier. */
static void scBilibinCaveRestoreRapierRoom(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);

	fdActorSetAnimation(ACTOR_RAPIER_ROOM_WEST_TORCH, TORCH_ANIMATION_UNLIT);
	fdActorSetAnimation(ACTOR_RAPIER_ROOM_EAST_TORCH, TORCH_ANIMATION_LIT);

	if(gmFlagGet(FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED) != 0) {
		fdActorSetPos(ACTOR_RAPIER_ROOM_FIRE, 0, 0);
	} else {
		fdActorSetSprFlags(fdActorGet(ACTOR_RAPIER_ROOM_FIRE), 0);
	}
}

/* Draws one of the ice room's torches or pieces of ice without a shadow. */
static inline void scBilibinCaveHideShadow(Sint32 actorId)
{
	ACTOR *actor = fdActorGet(actorId);
	fdActorSetSprFlags(actor, 0);
}

/* Sets up Bilibin Cave's SceneIdBilibinCaveIce. */
static void scBilibinCaveRestoreIceRoom(void)
{
	Sint32 dstMetatileX;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	scBilibinCaveHideShadow(ACTOR_FIRST_ICE_PILLAR);
	scBilibinCaveHideShadow(ACTOR_FIRST_ICE_PILLAR + 1);
	scBilibinCaveHideShadow(ACTOR_FIRST_ICE_PILLAR + 2);
	scBilibinCaveHideShadow(ACTOR_FIRST_ICE_PILLAR + 3);
	scBilibinCaveHideShadow(ACTOR_FIRST_ICE_PILLAR + 4);
	scBilibinCaveHideShadow(ACTOR_FIRST_LOW_ICE);
	scBilibinCaveHideShadow(ACTOR_FIRST_LOW_ICE + 1);
	scBilibinCaveHideShadow(ACTOR_FIRST_LOW_ICE + 2);
	scBilibinCaveHideShadow(ACTOR_FIRST_LOW_ICE + 3);
	fdActorSetAnimation(ACTOR_FIRST_ICE_PILLAR, PUDDLE_ANIMATION_PILLAR);
	fdActorSetAnimation(ACTOR_FIRST_ICE_PILLAR + 1, PUDDLE_ANIMATION_PILLAR);
	fdActorSetAnimation(ACTOR_FIRST_ICE_PILLAR + 2, PUDDLE_ANIMATION_PILLAR);
	fdActorSetAnimation(ACTOR_FIRST_ICE_PILLAR + 3, PUDDLE_ANIMATION_PILLAR);
	fdActorSetAnimation(ACTOR_FIRST_ICE_PILLAR + 4, PUDDLE_ANIMATION_PILLAR);
	fdActorSetAnimation(ACTOR_FIRST_LOW_ICE, PUDDLE_ANIMATION_LOW_ICE);
	fdActorSetAnimation(ACTOR_FIRST_LOW_ICE + 1, PUDDLE_ANIMATION_LOW_ICE);
	fdActorSetAnimation(ACTOR_FIRST_LOW_ICE + 2, PUDDLE_ANIMATION_LOW_ICE);
	fdActorSetAnimation(ACTOR_FIRST_LOW_ICE + 3, PUDDLE_ANIMATION_LOW_ICE);
	fdActorSetAnimation(ACTOR_FIRST_TORCH, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_FIRST_TORCH + 1, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_FIRST_TORCH + 2, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_FIRST_TORCH + 3, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_FIRST_TORCH + 4, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_FIRST_TORCH + 5, TORCH_ANIMATION_LIT);
	scBilibinCaveEmptyActorSetupHook(ACTOR_FIRST_ICE_PILLAR);
	scBilibinCaveEmptyActorSetupHook(ACTOR_FIRST_ICE_PILLAR + 1);
	scBilibinCaveEmptyActorSetupHook(ACTOR_FIRST_ICE_PILLAR + 2);
	scBilibinCaveEmptyActorSetupHook(ACTOR_FIRST_ICE_PILLAR + 3);
	scBilibinCaveEmptyActorSetupHook(ACTOR_FIRST_ICE_PILLAR + 4);
	scBilibinCaveEmptyActorSetupHook(ACTOR_FIRST_LOW_ICE);
	scBilibinCaveEmptyActorSetupHook(ACTOR_FIRST_LOW_ICE + 1);
	scBilibinCaveEmptyActorSetupHook(ACTOR_FIRST_LOW_ICE + 2);
	scBilibinCaveEmptyActorSetupHook(ACTOR_FIRST_LOW_ICE + 3);
	scBilibinCaveEmptyActorSetupHook(ACTOR_FIRST_TORCH);
	scBilibinCaveEmptyActorSetupHook(ACTOR_FIRST_TORCH + 1);
	scBilibinCaveEmptyActorSetupHook(ACTOR_FIRST_TORCH + 2);
	scBilibinCaveEmptyActorSetupHook(ACTOR_FIRST_TORCH + 3);
	scBilibinCaveEmptyActorSetupHook(ACTOR_FIRST_TORCH + 4);
	scBilibinCaveEmptyActorSetupHook(ACTOR_FIRST_TORCH + 5);
	if(gmFlagGet(FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED) != 0) {
		fdActorSetPos(ACTOR_ICE_ROOM_FIRE, 0, 0);
		fdActorSetAnimation(ACTOR_FIRST_PUDDLE, PUDDLE_ANIMATION_PILLAR);
		fdActorGet(ACTOR_FIRST_PUDDLE)->motionFlg = 0;
		fdActorGet(ACTOR_FIRST_PUDDLE)->y.fixed = -toFIXED(4);
		fdActorGet(ACTOR_FIRST_PUDDLE)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdActorSetSprPriority(ACTOR_FIRST_PUDDLE, 2);
		fdMapGridCopyMetatileAttributes(FIRST_PILLAR_METATILE, 1, 1, 18, 14);
	} else {
		fdActorSetAnimation(ACTOR_ICE_ROOM_FIRE, FIRE_ANIMATION_BURNING);
		fdActorSetSprFlags(fdActorGet(ACTOR_ICE_ROOM_FIRE), 0);
		fdActorSetAnimation(ACTOR_FIRST_PUDDLE, PUDDLE_ANIMATION_PUDDLE);
	}
	fdActorSetAnimation(ACTOR_SECOND_PUDDLE, PUDDLE_ANIMATION_PUDDLE);
	if(gmFlagGet(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED) != 0) {
		dstMetatileX = 36;
		fdActorSetAnimation(ACTOR_THIRD_PUDDLE, PUDDLE_ANIMATION_PUDDLE);
		fdMapGridCopyMetatileAttributes(
			THIRD_PUDDLE_NORTH_HOP_METATILE, 1, 1, dstMetatileX, 22);
		fdMapGridCopyMetatileAttributes(
			THIRD_PUDDLE_SOUTH_HOP_METATILE, 1, 1, dstMetatileX, 24);
	} else {
		dstMetatileX = 36;
		fdActorSetAnimation(ACTOR_THIRD_PUDDLE, PUDDLE_ANIMATION_PILLAR);
		fdMapGridCopyMetatileAttributes(
			THIRD_PUDDLE_NORTH_GROUND_METATILE, 1, 1, dstMetatileX, 22);
		fdMapGridCopyMetatileAttributes(
			THIRD_PUDDLE_SOUTH_GROUND_METATILE, 1, 1, dstMetatileX, 24);
	}
	if(gmFlagGet(FLAG_BILIBIN_CAVE_THIRD_TORCH_BY_PILLAR) != 0) {
		fdActorSetPos(ACTOR_THIRD_TORCH, 570, 376);
	}
	if(gmFlagGet(FLAG_BILIBIN_CAVE_FOURTH_TORCH_BY_PILLAR) != 0) {
		fdActorSetPos(ACTOR_FOURTH_TORCH, 570, 376);
	}
}

#include "shared/actor_find_at_metatile.c"

/* Walking into one of the pair of torches west of the ice pillar in the cold
   part of Bilibin Cave pushes it a metatile along the dotted track, if nothing
   stands in the metatile beyond and the ground there lets it go. Then, as after
   Move, it may melt or refreeze the pillar. */
static void scBilibinCavePushTorch(void)
{
	ACTOR	 *leader;
	ACTOR	 *tgt;
	ACTOR	 *blocker;
	Sint32	  heading;
	Sint32	  nextMetatileX;
	Sint32	  nextMetatileZ;
	FIELD_POS dst;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	heading = FACING_INDEX(leader->facing);

	nextMetatileX = (leader->x.part[0].integer +
						(FIXEDtoINT(PushedActorDirectionSteps[heading]))) >>
		4;
	nextMetatileZ = (leader->z.part[0].integer +
						(Sint16)PushedActorDirectionSteps[heading]) >>
		4;
	tgt = scActorFindAtMetatile(nextMetatileX, nextMetatileZ);
	if(tgt == NULL)
		return;

	nextMetatileX = (tgt->x.part[0].integer +
						(FIXEDtoINT(PushedActorDirectionSteps[heading]))) >>
		4;
	nextMetatileZ =
		(tgt->z.part[0].integer + (Sint16)PushedActorDirectionSteps[heading]) >>
		4;
	blocker = scActorFindAtMetatile(nextMetatileX, nextMetatileZ);
	if(blocker != NULL)
		return;

	tgt->layer = 2;

	fdActorStepFrom(&dst, tgt, PushedActorDirectionSteps[heading]);

	if(syFieldActorCheckMovementCollision(tgt, &dst) > 0)
		return;

	fdActorSelectAnimation(leader, ANIM_PUSH);
	sySynch(15);
	sySndPlay(SE_PUSH);

	tgt->speedLimit = toFIXED(1) / 5;
	tgt->acceleration = toFIXED(1) / 5;
	syFieldActorSetMoveTargetFX(tgt, dst.x, dst.y, dst.z);

	leader->speedLimit = toFIXED(1) / 5;
	leader->acceleration = toFIXED(1) / 5;
	syFieldActorSetMoveTargetFX(leader, dst.x, dst.y, dst.z);

	fdActorWaitForMove(tgt);

	tgt->x.fixed = dst.x;
	tgt->z.fixed = dst.z;
	/* blocker is null here: the torch comes to rest. */
	tgt->velX = (Sint32)blocker;
	tgt->velZ = (Sint32)blocker;

	fdActorSelectAnimation(leader, ANIM_STAND);
	scBilibinCaveUpdateThirdPuddleFromTorches();
}

/* Does nothing: the ice room calls it for each of its torches and pieces of
   ice. */
static void scBilibinCaveEmptyActorSetupHook(Sint32 actorId)
{
}

Sint32 BilibinCaveNoShadowScript[] = {
	SC_CALL(scBilibinCaveHideScriptedShadow),
	SC_END,
};

SCENE_ENTRANCE BilibinCaveEntrancesOther[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 72, 0, 488, FACING_NORTH, 0, -1, -1,
		-1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE BilibinCaveEntranceEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 372, 0, 272, FACING_NORTH, 0, -1, -1,
		-1, -1, 0},
	{BILIBIN_CAVE_ENTRANCE_FROM_WORLD_MAP, CONDITION_ALWAYS, 99, 0, 447,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_SOUTH, CONDITION_ALWAYS,
		342, 0, 254, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_NORTH, CONDITION_ALWAYS,
		440, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE BilibinCaveRapierEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 864, 0, 168, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_WEST, CONDITION_ALWAYS, 312,
		0, 756, FACING_NORTH, 0, 40, 60, 630, 840, 0},
	{BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_EAST, CONDITION_ALWAYS, 472,
		0, 680, FACING_NORTH, 0, 40, 60, 630, 840, 0},
	{BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_WEST, CONDITION_ALWAYS, 392, 0,
		200, FACING_SOUTH, 0, 40, 60, 630, 840, 0},
	{BILIBIN_CAVE_RAPIER_FROM_TORCHES, CONDITION_ALWAYS, 536, 0, 426,
		FACING_SOUTH, 0, 40, 60, 630, 840, 0},
	{BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_EAST, CONDITION_ALWAYS, 856, 0,
		124, FACING_SOUTH, 0, 760, 65, 925, 330, 0},
	{BILIBIN_CAVE_RAPIER_TORCHES_FROM_BILIBIN_CAVE_RAPIER, CONDITION_ALWAYS,
		808, 0, 248, FACING_NORTH, 0, 760, 65, 925, 330, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE BilibinCaveIceEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 300, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_WEST, CONDITION_ALWAYS, 120, 0,
		472, FACING_NORTH, 0, 60, 60, 790, 600, 0},
	{BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_EAST, CONDITION_ALWAYS, 312, 0,
		524, FACING_NORTH, 0, 60, 60, 790, 600, 0},
	{BILIBIN_CAVE_ICE_FROM_WORLD_MAP, CONDITION_ALWAYS, 538, 0, 548,
		FACING_NORTH, 0, 60, 60, 790, 600, 0},
	{BILIBIN_CAVE_ICE_FROM_CHESTS, CONDITION_ALWAYS, 344, 0, 176, FACING_SOUTH,
		0, 60, 60, 790, 600, 0},
	{BILIBIN_CAVE_ICE_CHESTS_FROM_BILIBIN_CAVE_ICE, CONDITION_ALWAYS, 856, 0,
		632, FACING_NORTH, 0, 740, 510, 980, 690, 0},
	{SCENE_TABLE_END},
};

Uint32 BilibinCaveExits[] = {
	SCENE_EXITS(BilibinCaveEntrance),
	SCENE_EXIT(BILIBIN_CAVE_ENTRANCE_TO_WORLD_MAP, WorldMap,
		WORLD_MAP_FROM_BILIBIN_CAVE_ENTRANCE),
	SCENE_EXIT(BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_SOUTH,
		BilibinCaveRapier, BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_WEST),
	SCENE_EXIT(BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_NORTH,
		BilibinCaveRapier, BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_EAST),
	SCENE_EXITS(BilibinCaveRapier),
	SCENE_EXIT(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_WEST,
		BilibinCaveEntrance,
		BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_SOUTH),
	SCENE_EXIT(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_EAST,
		BilibinCaveEntrance,
		BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_NORTH),
	SCENE_EXIT(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_WEST, BilibinCaveIce,
		BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_WEST),
	SCENE_EXIT(BILIBIN_CAVE_RAPIER_TO_TORCHES, BilibinCaveRapier,
		BILIBIN_CAVE_RAPIER_TORCHES_FROM_BILIBIN_CAVE_RAPIER),
	SCENE_EXIT(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_EAST, BilibinCaveIce,
		BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_EAST),
	SCENE_EXIT(BILIBIN_CAVE_RAPIER_TORCHES_TO_BILIBIN_CAVE_RAPIER,
		BilibinCaveRapier, BILIBIN_CAVE_RAPIER_FROM_TORCHES),
	SCENE_EXITS(BilibinCaveIce),
	SCENE_EXIT(BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_WEST, BilibinCaveRapier,
		BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_WEST),
	SCENE_EXIT(BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_EAST, BilibinCaveRapier,
		BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_EAST),
	SCENE_EXIT(BILIBIN_CAVE_ICE_TO_WORLD_MAP, WorldMap,
		WORLD_MAP_FROM_BILIBIN_CAVE_ICE),
	SCENE_EXIT(BILIBIN_CAVE_ICE_TO_CHESTS, BilibinCaveIce,
		BILIBIN_CAVE_ICE_CHESTS_FROM_BILIBIN_CAVE_ICE),
	SCENE_EXIT(BILIBIN_CAVE_ICE_CHESTS_TO_BILIBIN_CAVE_ICE, BilibinCaveIce,
		BILIBIN_CAVE_ICE_FROM_CHESTS),
	SCENE_EXITS_END,
};

ACTOR_PLACE BilibinCaveActorPlacementsOther[] = {
	{SCENE_TABLE_END},
};

/* clang-format off */
ACTOR_PLACE BilibinCaveEntranceActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		136, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE BilibinCaveRapierActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		840, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		856, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		872, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE BilibinCaveIceActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		152, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		520, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		504, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		536, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		504, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		584, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, (Sint32)BilibinCaveNoShadowScript,
		296, 0, 230, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, (Sint32)BilibinCaveNoShadowScript,
		504, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, (Sint32)BilibinCaveNoShadowScript,
		584, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		344, 0, 224, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		392, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		424, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		456, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		328, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		200, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		248, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		136, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		184, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};

CUE BilibinCaveCuesOther[] = {
	{ CUE_TABLE_END },
};

CUE BilibinCaveEntranceCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, BILIBIN_CAVE_ENTRANCE_TO_WORLD_MAP },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_SOUTH },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_NORTH },
	{ CUE_STEP, METATILE_BEHAVIOR_BILIBIN_CAVE_ABOVE_LOW_MOUTH, CONDITION_ALWAYS, SCRIPT_REF(scBilibinCaveCloseLowerCaveMouth) },
	{ CUE_STEP, METATILE_BEHAVIOR_BILIBIN_CAVE_BESIDE_LOW_MOUTH, CONDITION_ALWAYS, SCRIPT_REF(scBilibinCaveOpenLowerCaveMouth) },
	{ PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_BILIBIN_CAVE_ENTRANCE_SPROUT_GROWN, SCRIPT_REF(scBilibinCaveGrowPlant) },
	{ CUE_TABLE_END },
};

CUE BilibinCaveRapierCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_WEST },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_EAST },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_WEST },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_TORCHES },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_EAST },
	{ CUE_EXIT, 6, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TORCHES_TO_BILIBIN_CAVE_RAPIER },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_BILIBIN_CAVE_ELVEN_RAPIER_CHEST, FLAG_BILIBIN_CAVE_ELVEN_RAPIER, FIND_ITEM(ITEM_ELVEN_RAPIER) },
	{ PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scBilibinCaveRevealHiddenPath) },
	{ PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scBilibinCaveHideRevealedPath) },
	{ PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_RAPIER_ROOM_FIRE, FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED, SCRIPT_REF(scBilibinCaveDouseRapierRoomFire) },
	{ CUE_TABLE_END },
};

CUE BilibinCaveIceCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_WEST },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_EAST },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_WORLD_MAP },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_CHESTS },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_CHESTS_TO_BILIBIN_CAVE_ICE },
	{ CUE_STEP, METATILE_BEHAVIOR_BILIBIN_CAVE_SNOW, CONDITION_ALWAYS, SCRIPT_REF(scBilibinCaveStartFootprints) },
	{ CUE_STEP, METATILE_BEHAVIOR_BILIBIN_CAVE_SNOW_EDGE, CONDITION_ALWAYS, SCRIPT_REF(scBilibinCaveStopFootprints) },
	{ PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_ICE_ROOM_FIRE, FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED, SCRIPT_REF(scBilibinCaveDouseIceRoomFire) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE, FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED, SCRIPT_REF(scBilibinCaveFreezeFirstPuddle) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE, CONDITION_ALWAYS, SCRIPT_REF(scBilibinCaveFreezeSecondPuddle) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PUDDLE, IF_FLAG_SET(FLAG_BILIBIN_CAVE_THIRD_PUDDLE_MELTED), SCRIPT_REF(scBilibinCaveFreezeThirdPuddle) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_TORCH, CONDITION_ALWAYS, SCRIPT_REF(scBilibinCaveUpdateThirdPuddleFromTorches) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FOURTH_TORCH, CONDITION_ALWAYS, SCRIPT_REF(scBilibinCaveUpdateThirdPuddleFromTorches) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_BILIBIN_CAVE_TORCH_TRACK, CONDITION_ALWAYS, SCRIPT_REF(scBilibinCavePushTorch) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_BILIBIN_CAVE_TORCH_TRACK_END, CONDITION_ALWAYS, SCRIPT_REF(scBilibinCavePushTorch) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_BILIBIN_CAVE_VIAL_CHEST, FLAG_BILIBIN_CAVE_VIAL, FIND_ITEM(ITEM_VIAL) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_BILIBIN_CAVE_PSY_CRYSTAL_CHEST, FLAG_BILIBIN_CAVE_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_BILIBIN_CAVE_WATER_OF_LIFE_CHEST, FLAG_BILIBIN_CAVE_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
	{ CUE_TABLE_END },
};
/* clang-format on */

#include "shared/pushed_actor_direction_steps.c"
