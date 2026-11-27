/* The cave of the Apojii Islands: four clumps of weeds on the walls of its
   south chamber, each scattered for good by Whirlwind. The third hides the
   passage north, which leads out to the top of the falls where the Djinni Haze
   waits. */

#include "field/field_mode_script.h"
#include "field/field_mode.h"
#include "game/game.h"
#include "field/script_inline.h"
#include "field/actor_script.h"

/* The ways between the cave's south chamber and the passage north, by the
   passage the third clump of weeds hides. */
#define APOJII_ISLANDS_CAVE_SOUTH_TO_NORTH	 1
#define APOJII_ISLANDS_CAVE_NORTH_TO_SOUTH	 2
#define APOJII_ISLANDS_CAVE_SOUTH_FROM_NORTH 1
#define APOJII_ISLANDS_CAVE_NORTH_FROM_SOUTH 2

extern Uint32 ApojiiIslandsCave_WeedsScript[];

/* The four clumps of weeds on the south chamber's walls, from west to east,
   numbering the rows of ActorPlacements. */
typedef enum APOJII_ISLANDS_CAVE_ACTOR_ID {
	ACTOR_FIRST_WEEDS = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_WEEDS,
	ACTOR_PASSAGE_WEEDS, /* over the passage north */
	ACTOR_FOURTH_WEEDS
} APOJII_ISLANDS_CAVE_ACTOR_ID;

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

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

/* Whirlwind on a clump of weeds scatters it for good. */
void scApojiiIslandsCaveScatterWeeds(Sint32 unused, Sint32 weeds)
{
	syGameFlagSet(FLAG_APOJII_CAVE_FOLIAGE_BLOWN(weeds));
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

Sint32 scInit(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* clang-format off */
/* The weeds' script: each clump stands still three pixels up, on ground four */
/* pixels up, drawn half as tall again. */
Uint32 ApojiiIslandsCave_WeedsScript[] = {
	SC_SET(SW_MOTION, 0),
	SC_SET(SW_GROUND, toFIXED(4)),
	SC_SET(SW_Y, toFIXED(3)),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 2),
	SC_END,
};
/* clang-format on */

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(ApojiiIslandsCave),
	SCENE_EXIT_UNLESS(APOJII_ISLANDS_CAVE_SOUTH_TO_NORTH, ApojiiIslandsCave,
		APOJII_ISLANDS_CAVE_NORTH_FROM_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(APOJII_ISLANDS_CAVE_NORTH_TO_SOUTH, ApojiiIslandsCave,
		APOJII_ISLANDS_CAVE_SOUTH_FROM_NORTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(APOJII_ISLANDS_CAVE_TO_APOJII_ISLANDS_HAZE_SOUTHWEST,
		ApojiiIslandsHaze,
		APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_CAVE_SOUTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(APOJII_ISLANDS_CAVE_TO_APOJII_ISLANDS_HAZE_SOUTHEAST,
		ApojiiIslandsHaze,
		APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_CAVE_SOUTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(APOJII_ISLANDS_CAVE_TO_APOJII_ISLANDS_HAZE_NORTH,
		ApojiiIslandsHaze, APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_CAVE_NORTH,
		EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WEEDS,
		FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_FIRST_WEEDS),
		(Sint32)ApojiiIslandsCave_WeedsScript, 120, 0, 608, FACING_NORTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WEEDS,
		FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_SECOND_WEEDS),
		(Sint32)ApojiiIslandsCave_WeedsScript, 184, 0, 592, FACING_NORTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WEEDS,
		FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_PASSAGE_WEEDS),
		(Sint32)ApojiiIslandsCave_WeedsScript, 216, 0, 592, FACING_NORTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WEEDS,
		FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_FOURTH_WEEDS),
		(Sint32)ApojiiIslandsCave_WeedsScript, 280, 0, 640, FACING_NORTH,
		TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

CUE SceneCues[] = {
	{CUE_EXIT, 1,
		IF_FLAG_SET(FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_PASSAGE_WEEDS)),
		APOJII_ISLANDS_CAVE_SOUTH_TO_NORTH},
	{CUE_EXIT, 2, CONDITION_ALWAYS, APOJII_ISLANDS_CAVE_NORTH_TO_SOUTH},
	{CUE_EXIT, 3, CONDITION_ALWAYS,
		APOJII_ISLANDS_CAVE_TO_APOJII_ISLANDS_HAZE_SOUTHWEST},
	{CUE_EXIT, 4, CONDITION_ALWAYS,
		APOJII_ISLANDS_CAVE_TO_APOJII_ISLANDS_HAZE_SOUTHEAST},
	{CUE_EXIT, 5, CONDITION_ALWAYS,
		APOJII_ISLANDS_CAVE_TO_APOJII_ISLANDS_HAZE_NORTH},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_WEEDS,
		FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_FIRST_WEEDS),
		SCRIPT_REF(scApojiiIslandsCaveScatterWeeds)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_SECOND_WEEDS,
		FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_SECOND_WEEDS),
		SCRIPT_REF(scApojiiIslandsCaveScatterWeeds)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_PASSAGE_WEEDS,
		FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_PASSAGE_WEEDS),
		SCRIPT_REF(scApojiiIslandsCaveScatterWeeds)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FOURTH_WEEDS,
		FLAG_APOJII_CAVE_FOLIAGE_BLOWN(ACTOR_FOURTH_WEEDS),
		SCRIPT_REF(scApojiiIslandsCaveScatterWeeds)},
	{CUE_TABLE_END},
};
