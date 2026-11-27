/* @file */
/* Kolima Forest (SceneId_KolimaForestGlade), the second of the forest's five */
/* scenes, on the party's way to Tret between SceneId_KolimaForestEntrance and
 */
/* SceneId_KolimaForestCliff. */
/* */
/* Three logs lie in the grass to be rolled by walking into them. A Nut floats
 */
/* out of reach until Catch pulls it in (FLAG_KOLIMA_FOREST_NUT_CAUGHT). */
/* */
/* Until Tret lifts the curse (FLAG_KOLIMA_CURSE_LIFTED), scKolimaPaletteCurse
 */
/* gives the forest redder, darker colours. */
/* */
/* Until the third patch the Nut's actor placement row tested Vale's */
/* FLAG_VALE_NUT_CAUGHT rather than FLAG_KOLIMA_FOREST_NUT_CAUGHT, so once */
/* Vale's Nut had been caught this one was never placed. */
/* */
/* The scene's name, Glade, is ours, for what sets the scene apart. It is open
 */
/* grass and stumps ringed by trees, with no cliff or river */
/* (places/kolima-forest-glade_cursed/map-0.png). */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "debug/debug.h"
#include "field/event.h"
#include "field/speaker.h"
#include "graphics/graphics.h"
#include "scene/floating_nut.h"
#include "scene/catch_nut.h"
#include "scene/kolima_palette.h"

extern SCENE_ENTRANCE KolimaForestGlade_Entrances[];
extern u32 KolimaForestGlade_Exits[];
extern ACTOR_PLACE KolimaForestGlade_ActorPlacements[];
extern TRIGGER KolimaForestGlade_Triggers[];

/* Kolima Forest's actors, numbering the rows of */
/* KolimaForestGlade_ActorPlacements. */
typedef enum KOLIMA_FOREST_GLADE_ACTOR_ID {
	/* The three logs. */
	ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
	/* The Nut that floats until Catch pulls it in. */
	ACTOR_NUT = ACTOR_FIRST_LOG + 3
} KOLIMA_FOREST_GLADE_ACTOR_ID;

/* How far the curse tints the glade until it is lifted: green and blue lose */
/* 1 / GLADE_CURSE_DIVISOR of themselves (scKolimaPaletteCurse). */
#define GLADE_CURSE_DIVISOR 11

#include "shared/pushed_actor.c"

/* Where the party enters, from SceneId_KolimaForestEntrance and */
/* SceneId_KolimaForestCliff. */
const SCENE_ENTRANCE *scGetEntrances(void) {
	return KolimaForestGlade_Entrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void) { return 0; }

/* The ways on into the forest (SceneId_KolimaForestCliff) and back */
/* (SceneId_KolimaForestEntrance). */
const u32 *scGetExits(void) { return KolimaForestGlade_Exits; }

/* The three logs and the Nut. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	return KolimaForestGlade_ActorPlacements;
}

/* In Kolima Forest (SceneId_KolimaForestGlade), walking into a log rolls it */
/* ahead until something stops it. */
static void scKolimaForestGladePushLog(void) {
	PUSHED_ACTOR_PROBE result;
	fdEventStart();
	if(scPushedActorFindClearPos(&result))
		scPushedActorRoll(result);
	fdEventEnd();
}

/* In Kolima Forest (SceneId_KolimaForestGlade), Catch on the floating Nut */
/* makes it vanish, and the party finds a Nut. */
static void scKolimaForestGladeCatchNut(void) {
	scCatchNut(ACTOR_NUT, FLAG_KOLIMA_FOREST_NUT_CAUGHT);
}

/* The logs, the Nut and the two ways out. */
const TRIGGER *scGetTriggers(void) { return KolimaForestGlade_Triggers; }

/* Sets up the floating Nut and the logs on entry. */
s32 scInit(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(gmFlagGet(FLAG_KOLIMA_FOREST_NUT_CAUGHT) == 0) {
		scFloatNut(ACTOR_NUT);
	}
	scPushedActorBlockFootprint(ACTOR_FIRST_LOG);
	scPushedActorBlockFootprint(ACTOR_FIRST_LOG + 1);
	scPushedActorBlockFootprint(ACTOR_FIRST_LOG + 2);
	if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
		scKolimaPaletteCurse(GLADE_CURSE_DIVISOR);
	}
	return 0;
}

#include "shared/kolima_palette.c"
#include "shared/floating_nut.c"

#include "shared/pushed_actor_tables.c"
/* clang-format off */

SCENE_ENTRANCE KolimaForestGlade_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_GLADE_FROM_KOLIMA_FOREST_ENTRANCE, CONDITION_ALWAYS, 480, 0, 376, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_GLADE_FROM_KOLIMA_FOREST_CLIFF, CONDITION_ALWAYS, 312, 0, 40, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

/* KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_CLIFF on into the forest, and */
/* KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_ENTRANCE back. */
u32 KolimaForestGlade_Exits[] = {
    SCENE_EXITS(KolimaForestGlade),
    SCENE_EXIT(KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_CLIFF, KolimaForestCliff, KOLIMA_FOREST_CLIFF_FROM_KOLIMA_FOREST_GLADE),
    SCENE_EXIT(KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_ENTRANCE, KolimaForestEntrance, KOLIMA_FOREST_ENTRANCE_FROM_KOLIMA_FOREST_GLADE),
    SCENE_EXITS_END,
};

/* Until the third patch the Nut's actor placement tested Vale's */
/* FLAG_VALE_NUT_CAUGHT, not the flag catching it sets. */
ACTOR_PLACE KolimaForestGlade_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        360, 0, 368, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        160, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        136, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
#if !TBS_PATCH_3
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
        280, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
#else
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_KOLIMA_FOREST_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
        280, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
#endif
    { SCENE_TABLE_END },
};
/* clang-format on */

TRIGGER KolimaForestGlade_Triggers[] = {
	{TRIGGER_EXIT, 1, CONDITION_ALWAYS,
	 KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_CLIFF},
	{TRIGGER_EXIT, 2, CONDITION_ALWAYS,
	 KOLIMA_FOREST_GLADE_TO_KOLIMA_FOREST_ENTRANCE},
	{TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 10, CONDITION_ALWAYS,
	 EVENT(scKolimaForestGladePushLog)},
	{PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_KOLIMA_FOREST_NUT_CAUGHT,
	 EVENT(scKolimaForestGladeCatchNut)},
	{TRIGGER_TABLE_END},
};

/* The healthy colours, kept by scKolimaPaletteSaveHealthy: the palette fade's
 */
/* 448 background and sprite colours, in the array's first half. */
u8 KolimaPalette_Healthy[1792] BSS_DATA ALIGNED(16);
/* The cursed colours, kept by scKolimaPaletteSaveCursed. */
u8 KolimaPalette_Cursed[896] BSS_DATA;
