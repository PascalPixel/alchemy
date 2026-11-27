/* Sol Sanctum (SceneId_SolSanctumPsynergyStone), a room off the statue room
   (SceneId_SolSanctumStatue) that holds a Psynergy Stone.

   Its one exit, SOL_SANCTUM_PSYNERGY_STONE_TO_SOL_SANCTUM_STATUE, leads back to
   the statue room, and it places nobody.

   Searching in it finds the Psynergy Stone, which restores all PP. The stone is
   kept by an area flag (FLAG_SOL_SANCTUM_PSYNERGY_STONE_TAKEN) rather than a
   story flag, so gmArrive puts it back whenever the party enters another area.

   Retreat does not work here. Once Mt. Aleph has erupted, the room rumbles and
   shakes, as the rest of the sanctum does. */

#include "flags.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script.h"
#include "graphics/graphics.h"

extern SCENE_ENTRANCE SolSanctumPsynergyStoneEntrances[];
extern Uint32		  SolSanctumPsynergyStoneExits[];
extern ACTOR_PLACE	  SolSanctumPsynergyStoneActorPlacements[];
extern CUE			  SolSanctumPsynergyStoneCues[];

/* Area flags: gmArrive clears 0x300 to 0x3ff when the party enters another
   area, and other areas use the numbers for their own. */
#define FLAG_SOL_SANCTUM_PSYNERGY_STONE_TAKEN FLAG_AREA_TEMP_50

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SolSanctumPsynergyStoneEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return SolSanctumPsynergyStoneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return SolSanctumPsynergyStoneActorPlacements;
}

/* The exit and the Psynergy Stone. */
const CUE *scGetCues(void)
{
	return SolSanctumPsynergyStoneCues;
}

/* Opens the map with the window transition and blocks Retreat. */
Sint32 scInit(void)
{
	FIXED shakeX = toFIXED(1);
	FIXED shakeY = toFIXED(1);

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);

	syImportGameFlagSet(FLAG_RETREAT_BLOCKED);
	if(gmFlagGet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
		fdQueueMusic(SE_RUMBLE);
		fdMapGridSetScreenShake(shakeX, shakeY, toFIXED(1));
		fdEffectInitSparkles();
	}
	return 0;
}

SCENE_ENTRANCE SolSanctumPsynergyStoneEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 87, 0, 142, FACING_NORTH, 0, -1, -1,
		-1, -1, 0},
	{SOL_SANCTUM_PSYNERGY_STONE_FROM_SOL_SANCTUM_STATUE, CONDITION_ALWAYS, 87,
		0, 142, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 SolSanctumPsynergyStoneExits[] = {
	SCENE_EXITS(SolSanctumPsynergyStone),
	SCENE_EXIT(SOL_SANCTUM_PSYNERGY_STONE_TO_SOL_SANCTUM_STATUE,
		SolSanctumStatue, SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_PSYNERGY_STONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE SolSanctumPsynergyStoneActorPlacements[] = {
	{SCENE_TABLE_END},
};

CUE SolSanctumPsynergyStoneCues[] = {
	{CUE_EXIT, SOL_SANCTUM_PSYNERGY_STONE_TO_SOL_SANCTUM_STATUE,
		CONDITION_ALWAYS, SOL_SANCTUM_PSYNERGY_STONE_TO_SOL_SANCTUM_STATUE},
	{EXAMINE_CUE(EXAMINE_SILENT), 100, FLAG_SOL_SANCTUM_PSYNERGY_STONE_TAKEN,
		FIND_PSYNERGY_STONE},
	{CUE_TABLE_END},
};
