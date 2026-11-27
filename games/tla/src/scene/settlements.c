// Four small places the ship can put in at, sharing one overlay, each with its houses. In Gondowan
// Settlement, below Magma Rock, Cyclone blows away the leftmost bush to uncover a ladder down to a
// cave with Star Dust. Hesperia Settlement is built into a cliff: Growth turns its sprout into a
// vine, the crate on top of the wall is pushed down to make a step up to Tinder, and a hole beside
// a palm on the wall top leads down into a room below. On Kalt Island Catch takes the apple out of
// the snowy trees, the frozen river slides Felix along, Lash ties a rope across it, and Gel waits
// by the house. On SW Atteka Islet, by the falls at the edge of the world, the pillar pushed into
// the river stops the crate sent down the falls, and once the boulder is lifted a Jupiter Djinni
// the party lacks from The Broken Seal waits among the posts in the river.

// This unit declares its own form of fdRopeLash, below.
#define OWN_fdRopeLash

#include "scene/script_layer.h"
#include "field/actor_script.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"
#include "item_ids.h"
#include "macros.h"
#include "field/script_inline.h"
#include "game/game.h"
#include "edition.h"
#include "scene/scene.h"
#include "field/field_mode.h"
#include "field/call.h"
#include "graphics/glyph.h"
#include "text/msg_ids.h"

// The ROM's callers here are compiled with no declaration in scope, as against this form: no
// parameter list and an Sint32 result, where the definition takes the rope and two posts and
// returns nothing.
Sint32 fdRopeLash();

#define EFFECT_SET_MODE   fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

// SW Atteka Islet's background palette whose water colours run down the falls.
#define FALLS_PALETTE             3

// Kalt Island's rope across the frozen river, tied by Lash, for this visit.
#define FLAG_KALT_ROPE_TIED       FLAG_SCENE_TEMP_03
// Kalt Island: set by a step onto metatile (5, 12), the top of the climb on the frozen river's west
// bank, and cleared by one onto metatile (5, 13) below it; while it is set the river's ice slides
// no one (fdLeaderSlideOnIce).
#define FLAG_ICE_SLIDES_STOPPED   FLAG_SCENE_TEMP_01
// SW Atteka Islet: the pillar has been pushed into the river at metatile (11, 12), and its top
// stands in the water like the river's other posts.
#define FLAG_PILLAR_SUNK          FLAG_SCENE_TEMP_04
// SW Atteka Islet: the crate has gone down the falls and stuck against the sunk pillar, making the
// path to the Dragon Skin's chest.
#define FLAG_FALLS_CRATE_AT_REST  FLAG_SCENE_TEMP_05
// SW Atteka Islet: the crate has gone down the falls with no pillar to stop it, and is lost.
#define FLAG_FALLS_CRATE_LOST     FLAG_SCENE_TEMP_06
// SW Atteka Islet: the crate, moved onto metatile (16, 12) at the grass's south edge, has dropped
// down the cliff there.
#define FLAG_FALLS_CRATE_DROPPED  FLAG_SCENE_TEMP_07
// SW Atteka Islet: Lift has raised the boulder.
#define FLAG_BOULDER_LIFTED       FLAG_SCENE_TEMP_08
// Hesperia Settlement: Felix has stepped on metatile (13, 11) and walks by the wall top's metatiles
// (scSettlementsStepOntoWall).
#define FLAG_ON_WALL              FLAG_SCENE_TEMP_20
// Hesperia Settlement: the crate has been pushed down off the wall.
#define FLAG_WALL_CRATE_DROPPED   FLAG_SCENE_TEMP_21
// Hesperia Settlement: the crate has been pushed off the wall at metatile (16, 15), the wall's
// whole height, to the ground below.
#define FLAG_WALL_CRATE_ON_GROUND FLAG_SCENE_TEMP_05
// Hesperia Settlement: the crate, pushed down onto the step at metatile (12, 15), has been pushed
// east off it to the ground at metatile (13, 15).
#define FLAG_WALL_CRATE_OFF_STEP  FLAG_SCENE_TEMP_04
// Hesperia Settlement: Growth has made the sprout below the west cliff a vine.
#define FLAG_VINE_GROWN           FLAG_SCENE_TEMP_30

// The bush over the ladder hole in the west of Gondowan Settlement is blown away.
#define FLAG_LADDER_BUSH_CUT      0xa60

// Gondowan Settlement's metatiles that run a script. The two beside the ladder hole are the numbers
// of the exits they lead down by.
typedef enum GONDOWAN_SETTLEMENT_METATILE
{
	// Metatile (5, 16), north of the hole.
	GONDOWAN_LADDER_HOLE_NORTH = 3,
	// Metatile (4, 17), west of the hole.
	GONDOWAN_LADDER_HOLE_WEST  = 4,
	// The leafy bushes west of the village, such as metatile (7, 15).
	GONDOWAN_BUSH              = 10,
	// The leftmost bush, at metatile (5, 17), over the ladder hole.
	GONDOWAN_LADDER_BUSH       = 11
} GONDOWAN_SETTLEMENT_METATILE;

// Hesperia Settlement's metatiles that run a script. The door and the three sides of the ladder
// hole are the numbers of the exits they lead by.
typedef enum HESPERIA_SETTLEMENT_METATILE
{
	// Metatile (23, 16), before the door in the cliff east of Tinder's ledge.
	HESPERIA_HOUSE_DOOR        = 2,
	// Metatile (23, 12), north of the hole beside the palm on the wall top.
	HESPERIA_LADDER_HOLE_NORTH = 3,
	// Metatile (22, 13), west of the hole.
	HESPERIA_LADDER_HOLE_WEST  = 4,
	// Metatile (24, 13), east of the hole.
	HESPERIA_LADDER_HOLE_EAST  = 5,
	// Metatile (13, 11), on the stairs in the cliff west of the wall, where Felix comes onto the
	// wall top's metatiles.
	HESPERIA_ONTO_WALL         = 34,
	// Metatile (10, 9), further along the stairs, where he leaves them.
	HESPERIA_OFF_WALL          = 35
} HESPERIA_SETTLEMENT_METATILE;

// Kalt Island's metatiles that run a script.
typedef enum KALT_ISLAND_METATILE
{
	// Metatile (14, 18), before the house's door (its exit's number).
	KALT_HOUSE_DOOR     = 2,
	// Metatile (5, 12), the top of the climb on the frozen river's west bank.
	KALT_ICE_SLIDES_OFF = 10,
	// Metatile (5, 13), below it.
	KALT_ICE_SLIDES_ON  = 11,
	// The frozen river's ice.
	KALT_ICE            = 20
} KALT_ISLAND_METATILE;

// The people of Gondowan Settlement, by the rows of SettlementsGondowanSettlementPlacements.
typedef enum GONDOWAN_SETTLEMENT_ACTOR_ID
{
	ACTOR_MAN_ON_THE_RIDDLE_OF_THE_ROCK = ACTOR_FIRST_PLACED,
	ACTOR_GONDOWAN_BOY
} GONDOWAN_SETTLEMENT_ACTOR_ID;

// The Gondowan Settlement's houses.
typedef enum GONDOWAN_SETTLEMENT_HOUSES_ACTOR_ID
{
	// "I hear the Kibombo has finally..."
	ACTOR_WOMAN_ON_THE_KIBOMBO = ACTOR_FIRST_PLACED
} GONDOWAN_SETTLEMENT_HOUSES_ACTOR_ID;

// The Hesperia Settlement, by the rows of SettlementsHesperiaSettlementPlacements.
typedef enum HESPERIA_SETTLEMENT_ACTOR_ID
{
	ACTOR_HESPERIA_MAN = ACTOR_FIRST_PLACED,
	ACTOR_SPROUT, // below the west cliff, grown into a vine by Growth
	ACTOR_WALL_CRATE,
	ACTOR_TINDER
} HESPERIA_SETTLEMENT_ACTOR_ID;

// The Hesperia Settlement's houses.
typedef enum HESPERIA_SETTLEMENT_HOUSES_ACTOR_ID
{
	// "There's a place called Shaman Village..."
	ACTOR_WOMAN_ON_SHAMAN_VILLAGE = ACTOR_FIRST_PLACED,
	// "The people of Shaman Village..."
	ACTOR_BOY_ON_THE_SHAMANS
} HESPERIA_SETTLEMENT_HOUSES_ACTOR_ID;

// Kalt Island, by the rows of SettlementsKaltIslandPlacements.
typedef enum KALT_ISLAND_ACTOR_ID
{
	// The two posts on either bank of the frozen river, tied by Lash.
	ACTOR_ROPE_POST_A = ACTOR_FIRST_PLACED,
	ACTOR_ROPE_POST_B,
	ACTOR_GEL,
	ACTOR_APPLE // in the snowy trees west of the river, taken by Catch
} KALT_ISLAND_ACTOR_ID;

// Kalt Island's houses.
typedef enum KALT_ISLAND_HOUSES_ACTOR_ID
{
	// "If you want to cross the glaciers of the Northern Sea..."
	ACTOR_OLD_MAN_ON_THE_GLACIERS = ACTOR_FIRST_PLACED,
	// "A long time ago, we used to trade with a village named Prox..."
	ACTOR_OLD_WOMAN_ON_PROX_TRADE
} KALT_ISLAND_HOUSES_ACTOR_ID;

// SW Atteka Islet, by the rows of SettlementsAttekaIsletPlacements.
typedef enum ATTEKA_ISLET_ACTOR_ID
{
	ACTOR_ISLANDER = ACTOR_FIRST_PLACED,
	ACTOR_PILLAR,      // on the grass by the river, pushed into it
	ACTOR_FALLS_CRATE, // by the tent, pushed down the falls
	// The stone posts standing in the river, alike, bobbing in the current; the last is kept out of
	// sight until the pillar sinks in its place.
	ACTOR_RIVER_STONE_A,
	ACTOR_RIVER_STONE_B,
	ACTOR_RIVER_STONE_C,
	ACTOR_RIVER_STONE_D,
	ACTOR_RIVER_STONE_E,
	ACTOR_RIVER_STONE_F,
	ACTOR_RIVER_STONE_G,
	ACTOR_RIVER_STONE_H,
	ACTOR_RIVER_STONE_I,
	ACTOR_RIVER_STONE_J,
	ACTOR_RIVER_STONE_K,
	ACTOR_BOULDER, // on the grass above the river's posts, raised by Lift
	// The Jupiter Djinni among the posts in the river, standing for whichever of The Broken Seal's
	// Jupiter Djinn the party lacks.
	ACTOR_JUPITER_DJINNI
} ATTEKA_ISLET_ACTOR_ID;

// SW Atteka Islet's houses.
typedef enum ATTEKA_ISLET_HOUSES_ACTOR_ID
{
	// "There's nothing to the west..."
	ACTOR_ISLANDER_ON_THE_WEST = ACTOR_FIRST_PLACED
} ATTEKA_ISLET_HOUSES_ACTOR_ID;

extern Uint8        MsgSettlementsDontTellMeYoureHere[];

extern Sint16       SettlementsFallsObjects[];

void                scSmokeSpawn(Sint32 x, Sint32 y, Sint32 z);
void                scSettlementsCycleWaterColors(void);
void                scScrollStartHBlank(void);
void                scSettlementsSinkPillar(void);
void                scSettlementsSlideCrateDownFalls(void);
void                scSettlementsRestoreWallMetatiles(void);
void                scSettlementsDropCrate(void);
void                fdLeaderSlideOnIce(void);
void                scSettlementsClimbDownLadder(void);
void                scSmokeDrift(ACTOR *actor);
void                scScrollHBlank(void);
void                scLayerTrackScroll(void);

extern SCENE_REGION SettlementsHesperiaSettlementHousesRegions[];
extern SCENE_REGION SettlementsGondowanSettlementCaveRegions[];

#include "shared/script_check_spheres_overlap.h"

extern Sint16 SlideOnIceAnglesByKey[];

void          scSettlementsStepOntoWall(void);

void          fdLeaderStopIfPending(void);

void          scSettlementsMeetTinder(void);

void          scSettlementsCatchApple(void);
void          scSettlementsCutBushes(void);

void          scSettlementsCutLadderBush(void);

void          scSettlementsStepOntoLadderFromEast(void);

void          scSettlementsTurnAndClimbDownLadder(void);

void          scSettlementsStopIceSlides(void);

void          scSettlementsResumeIceSlides(void);
void          scSettlementsStepOnIceSlide(void);
void          scSettlementsLashRope(void);

void          scSettlementsUnblockFallsObjects(void);

void          scSettlementsStartPushingFallsObject(void);

void          scSettlementsMoveCrate(void);

void          scSettlementsStepOffWall(void);

void          scSettlementsUnblockCrate(void);

void          scSettlementsBlockCrate(void);

void          scSettlementsLiftBoulder(void);

extern Uint32 FieldEffectShortScript[];
extern Uint32 FieldEffectMediumScript[];
extern Sint32 SettlementsBobScript[];
extern Sint32 SettlementsCrateBobScript[];

// Three 16.16 fixed-point components.
typedef struct SETTLEMENTS_VECTOR
{
	Sint32 x;
	Sint32 y;
	Sint32 z;
} SETTLEMENTS_VECTOR;

void                                   scSettlementsStepOntoWall(void);

// The falls' palette as the map loads it, and as it is shown with its water colours turned.
Uint16                                 SettlementsLoadedWaterColors[16] BSS_DATA;
Uint16                                 SettlementsShownWaterColors[16] BSS_DATA;
Uint8                                  SettlementsUnusedAfterShownWaterColors[64] BSS_DATA;
Sint32 SettlementsWaterfallLine        BSS_DATA;
Uint16 SettlementsWaterfallScrollAbove BSS_DATA;
Uint16 SettlementsWaterfallScrollBelow BSS_DATA;

void                                   scSettlementsSpawnDustRing(Sint32 actorId);

void                                   scSettlementsMovePillar(void);

// Felix talks to Gel, the Mercury Djinni in the snow east of Kalt Island's house, and Gel joins the
// party: "Felix found the Mercury Djinni Gel!"
void                                   scSettlementsMeetGel(void)
{
	fdDjinnJoinParty(ACTOR_GEL, DJINNI_GEL / DJINN_PER_ELEMENT, DJINNI_INDEX(DJINNI_GEL));
}

// Felix talks to Tinder, the Mars Djinni on the ledge below Hesperia Settlement's wall, and Tinder
// joins the party: "Felix found the Mars Djinni Tinder!"
void scSettlementsMeetTinder(void)
{
	fdDjinnJoinParty(ACTOR_TINDER, DJINNI_TINDER / DJINN_PER_ELEMENT, DJINNI_INDEX(DJINNI_TINDER));
}

// Felix talks to the Jupiter Djinni among the posts in SW Atteka Islet's river, west of the boulder
// Lift moves, and it joins the party. It is whichever of The Broken Seal's seven Jupiter Djinn the
// party did not bring; with none missing, it is not there at all.
void scSettlementsMeetLostJupiterDjinni(void)
{
	fdDjinnJoinParty(
		ACTOR_JUPITER_DJINNI, DJINNI_WIND19 / DJINN_PER_ELEMENT, DJINNI_INDEX(DJINNI_WIND19));
}

// A script call: clears the actor's sprite flags.
Sint32 scSettlementsClearSpriteFlags(ACTOR *actor)
{
	fdActorSetSpriteFlags(actor, 0);
	return 0;
}

#include "shared/actor_bob.h"

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	Sint32        scene = MainRoutineStateWork.next;
	SCENE_REGION *placements;

	if (scene == SCENE_HESPERIA_SETTLEMENT_HOUSES)
		return SettlementsHesperiaSettlementHousesRegions;
	placements = 0;
	if (scene == SCENE_GONDOWAN_SETTLEMENT_CAVE)
		placements = SettlementsGondowanSettlementCaveRegions;
	return placements;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

extern ACTOR_PLACE SettlementsPlacements[];
extern ACTOR_PLACE SettlementsGondowanSettlementPlacements[];
extern ACTOR_PLACE SettlementsGondowanSettlementHousesPlacements[];
extern ACTOR_PLACE SettlementsHesperiaSettlementPlacements[];
extern ACTOR_PLACE SettlementsHesperiaSettlementHousesPlacements[];
extern ACTOR_PLACE SettlementsAttekaIsletPlacements[];
extern ACTOR_PLACE SettlementsAttekaIsletHousesPlacements[];
extern ACTOR_PLACE SettlementsKaltIslandPlacements[];
extern ACTOR_PLACE SettlementsKaltIslandHousesPlacements[];

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if (scene == SCENE_GONDOWAN_SETTLEMENT)
		return SettlementsGondowanSettlementPlacements;
	if (scene == SCENE_GONDOWAN_SETTLEMENT_HOUSES)
		return SettlementsGondowanSettlementHousesPlacements;
	if (scene == SCENE_HESPERIA_SETTLEMENT)
		return SettlementsHesperiaSettlementPlacements;
	if (scene == SCENE_HESPERIA_SETTLEMENT_HOUSES)
		return SettlementsHesperiaSettlementHousesPlacements;
	if (scene == SCENE_KALT_ISLAND)
		return SettlementsKaltIslandPlacements;
	if (scene == SCENE_KALT_ISLAND_HOUSES)
		return SettlementsKaltIslandHousesPlacements;
	if (scene == SCENE_SOUTHWEST_ATTEKA_ISLET)
		return SettlementsAttekaIsletPlacements;
	if (scene == SCENE_SOUTHWEST_ATTEKA_ISLET_HOUSES)
		return SettlementsAttekaIsletHousesPlacements;
	return SettlementsPlacements;
}

// Felix casts Catch on the apple floating in the snowy trees west of Kalt Island's frozen river;
// the hand brings it to him, he holds it up, and "Felix got an Apple."
void scSettlementsCatchApple(void)
{
	Sint32 mode;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetPos(ACTOR_APPLE, 0, 0);
	fdScriptSetFlag(FLAG_KALT_ISLAND_APPLE_TAKEN);
	fdItemDisplayShowItem(ITEM_APPLE, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
	mode = 0;
	gmPartyGiveItem(ITEM_APPLE, mode);
	fdScriptEnd();
}

// Talking to the man wandering in front of the houses of Gondowan Settlement: "Don't tell me,
// you're here to solve the riddle of the rock!" Yes, and he warns that even the warriors who make
// it back barely survive; no, and he says to steer clear of the mountain.
void scSettlementsWarnOfRiddleOfRock(Sint32 actor)
{
	Sint32 msg = (Sint32)MsgSettlementsDontTellMeYoureHere;

	syFieldScriptSetMessageId(msg);
	fdScriptOpenMessage(actor, 0);
	if (syFieldScriptAskYesNoWithActorAnimation(syFieldPartyGetLeaderActor(), 0) == 0)
	{
		syFieldScriptWait(10);
		syFieldScriptSetMessageId(msg + 1);
	}
	else
	{
		syFieldScriptWait(20);
		syFieldScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessage(actor, 0);
}

// Felix casts Cyclone on the leafy bushes west of Gondowan Settlement's houses (metatile (7, 15)
// and the rest), and the ones the whirlwind reaches are blown away and stay gone.
void scSettlementsCutBushes(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(5, 70, 9, 7, 5, 43, 1);
}

// Felix casts Cyclone on the leftmost bush west of Gondowan Settlement (metatile (5, 17)), and
// blowing it away uncovers a hole with a ladder down to a cave. Had he been standing on the bush,
// he hops off it to the south first.
void scSettlementsCutLadderBush(void)
{
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 x     = FIELD_METATILE(actor->x.fixed);
	Sint32 z     = FIELD_METATILE(actor->z.fixed);

	if (x == 5 && z == 17)
	{
		scFieldActorJump(MainRoutineStateWork.leaderActorId, 2, 0);
		fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 88, 296);
	}
	fdTileCopiesCopyTouchedMetatiles(5, 70, 9, 7, 5, 43, 1);
	fdMapGridCopyMetatilesTo(4, 25, 5, 17, 1, 1);
	fdMapGridCopyMetatileAttributes(4, 25, 1, 1, 5, 17);
	fdScriptSetFlag(FLAG_LADDER_BUSH_CUT);
}

// Felix steps onto the ladder's top, faces north and climbs down it, and the scene changes to the
// place below.
void scSettlementsClimbDownLadder(void)
{
	Sint32 next = FieldStateWork->stepCue;

	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	syFieldScriptWait(10);
	fdActorGet(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdMapGridCopyMetatilesTo(96, 14, 72, 14, 2, 1);
	fdActorSetDestinationOffset(ACTOR_FELIX, 0, 4);
	scFieldActorWaitForMoveById(ACTOR_FELIX);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_FELIX), 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
	fdActorSetDestinationOffset(ACTOR_FELIX, 0, 16);
	scFieldActorWaitForMoveById(ACTOR_FELIX);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB);
	syFieldScriptWait(10);
	fdScriptPlaySound(SE_MAP_EXIT);
	syFieldScriptSetExitRequest(next);
	fdScriptEnd();
}

// Felix walks east onto the ladder hole on Hesperia Settlement's wall top, beside the palm
// (metatile (22, 13)), or onto the hole Cyclone uncovered in Gondowan Settlement (metatile (4,
// 17)), and climbs down: in Hesperia into a room with beds below, in Gondowan into the cave with
// the Star Dust.
void scSettlementsStepOntoLadderFromWest(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorWalkByAndWait(ACTOR_FELIX, 0, -14);
	fdActorWalkByAndWait(ACTOR_FELIX, 16, 0);
	scSettlementsClimbDownLadder();
}

// Felix walks west onto the ladder hole beside the palm on Hesperia Settlement's wall top (metatile
// (24, 13)) and climbs down into the room with beds below.
void scSettlementsStepOntoLadderFromEast(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorWalkByAndWait(ACTOR_FELIX, -2, -6);
	fdActorWalkByAndWait(ACTOR_FELIX, -14, -8);
	scSettlementsClimbDownLadder();
}

// Felix walks south onto the ladder hole beside the palm on Hesperia Settlement's wall top
// (metatile (23, 12)), or onto the hole Cyclone uncovered in Gondowan Settlement (metatile (5,
// 16)), turns to face north and climbs down.
void scSettlementsTurnAndClimbDownLadder(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	scSettlementsClimbDownLadder();
}

// Felix casts Growth on the sprout below Hesperia Settlement's west cliff, and it grows into a vine
// he can climb (its metatile, (3, 15), becomes climbable).
void scSettlementsGrowVine(void)
{
	fdMapGridCopyMetatileAttributes(0, 3, 1, 1, 3, 15);
	fdScriptSetFlag(FLAG_VINE_GROWN);
}

extern CUE SettlementsCues[];
extern CUE SettlementsGondowanSettlementCues[];
extern CUE SettlementsGondowanSettlementHousesCues[];
extern CUE SettlementsHesperiaSettlementCues[];
extern CUE SettlementsHesperiaSettlementHousesCues[];
extern CUE SettlementsKaltIslandCues[];
extern CUE SettlementsKaltIslandHousesCues[];
extern CUE SettlementsAttekaIsletCues[];
extern CUE SettlementsAttekaIsletHousesCues[];

const CUE *scGetCues(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if (scene == SCENE_GONDOWAN_SETTLEMENT)
		return SettlementsGondowanSettlementCues;
	if (scene == SCENE_GONDOWAN_SETTLEMENT_HOUSES)
		return SettlementsGondowanSettlementHousesCues;
	if (scene == SCENE_HESPERIA_SETTLEMENT)
		return SettlementsHesperiaSettlementCues;
	if (scene == SCENE_HESPERIA_SETTLEMENT_HOUSES)
		return SettlementsHesperiaSettlementHousesCues;
	if (scene == SCENE_KALT_ISLAND)
		return SettlementsKaltIslandCues;
	if (scene == SCENE_KALT_ISLAND_HOUSES)
		return SettlementsKaltIslandHousesCues;
	if (scene == SCENE_SOUTHWEST_ATTEKA_ISLET)
		return SettlementsAttekaIsletCues;
	if (scene == SCENE_SOUTHWEST_ATTEKA_ISLET_HOUSES)
		return SettlementsAttekaIsletHousesCues;
	return SettlementsCues;
}

#include "shared/smoke_drift.h"

#include "shared/smoke_spawn.h"

// An event on Kalt Island: a puff from the house's chimney every 64 ticks.
void scSettlementsPuffChimneySmoke(void)
{
	if ((TickCount & 63) == 0)
		scSmokeSpawn(toFIXED(248), toFIXED(48), toFIXED(256));
}

// The apple each tick: it sways up to two pixels either side of its spot and dips up to two pixels
// below it, at a random pace, as The Broken Seal's Nut floats. A frozen actor is never steered, so
// the apple keeps the sway's angle in its speedLimit and its spot in targetX and targetY.
Sint32 scSettlementsFloatApple(ACTOR *apple)
{
	FIELD_SPR *sprite = (FIELD_SPR *)apple->sprite;
	FIXED      dip    = syMathSin(apple->speedLimit) * 2;

	if (dip > 0)
		dip = -dip;
	apple->x.fixed     = apple->targetX + syCos(apple->speedLimit) * 2;
	apple->y.fixed     = apple->targetY + dip;
	sprite->rotation   = syCos(apple->speedLimit + DEGtoANG(180)) / 8;
	// The pace: a sixty-fourth of a turn and two random steps under 1/128 each.
	apple->speedLimit += syRandomBelow(512) + syRandomBelow(512) + TRIG_TURN / 64;
	return 0;
}

#include "shared/field_rope_place.c"

// Sets up Kalt Island: the see-through sprites, the chimney smoke, the rope if it is tied, and,
// until it is taken, the apple floating in the trees.
void scSettlementsSetUpKaltIsland(void)
{
	ACTOR         *actor;
	FIELD_SPR     *sprite;
	ICON_TRANSFER *glyph;

	REG_BLDALPHA = BLEND_ALPHA(8, 12);
	REG_BLDCNT   = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	{
		Sint32 priority = EVENT_PASS_TICK;

		syFieldSetTickEvent(scSettlementsPuffChimneySmoke, priority);
	}
	fdRopeInit();
	fdRopeInit();
	scFieldRopePlace(1, FLAG_KALT_ROPE_TIED, ACTOR_ROPE_POST_A, ACTOR_ROPE_POST_B);
	if (fdScriptGetFlag(FLAG_KALT_ISLAND_APPLE_TAKEN))
	{
		fdActorSetPos(ACTOR_APPLE, 0, 0);
		return;
	}
	actor             = fdActorGet(ACTOR_APPLE);
	sprite            = (FIELD_SPR *)actor->sprite;
	sprite->priority  = 1;
	sprite->fullColor = 0;
	sprite->palette   = 0;
	sprite->count     = 0;
	fdActorSetSpriteFlags(actor, 0);
	actor->retainSpriteCache = 0;
	actor->motionFlags       = 0;
	if (fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
		actor->y.fixed += toFIXED(16);
	actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	actor->frozen         = 1;
	glyph                 = syHeapSlotAllocate(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	gmItemLoadIcon(ITEM_APPLE);
	mnVRAMLoad(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	actor->targetX           = actor->x.fixed;
	actor->speedLimit        = 0;
	actor->targetY           = actor->y.fixed;
	actor->retainSpriteCache = ACTOR_KEEP_SPRITE_VRAM;
	actor->update            = (OBJECT_UPDATE)scSettlementsFloatApple;
	actor->arrivalAxis       = 0;
}

extern Sint16 SettlementsWallCrate[];

void          scSettlementsStartWaterfall(void);

Sint32        scInit(void)
{
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if (MainRoutineStateWork.next == SCENE_KALT_ISLAND)
		scSettlementsSetUpKaltIsland();
	if (MainRoutineStateWork.next == SCENE_SOUTHWEST_ATTEKA_ISLET)
	{
		fdEffectStartBlankOverlay(0);
		fdActorPlaceBlocks(SettlementsFallsObjects);
		if (fdScriptGetFlag(FLAG_FALLS_CRATE_LOST))
			fdActorSetPos(ACTOR_FALLS_CRATE, 0, 0);
		fdActorGetById(ACTOR_FALLS_CRATE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
		fdActorSetAnimation(ACTOR_RIVER_STONE_H, 2);
		fdActorSetAnimation(ACTOR_RIVER_STONE_I, 2);
		fdActorSetAnimation(ACTOR_RIVER_STONE_K, 2);
		if (fdScriptGetFlag(FLAG_FALLS_CRATE_AT_REST))
			fdActorSetAnimation(ACTOR_FALLS_CRATE, 2);
		scSettlementsStartWaterfall();
#if ATTEKA_ISLET_REBUILD_KEEPS_SUNK_PILLAR
		if (fdScriptGetFlag(FLAG_PILLAR_SUNK))
		{
			fdActorSetPos(ACTOR_PILLAR, 0, 0);
			fdActorSetPos(ACTOR_RIVER_STONE_K, 184, 184);
			fdActorGetById(ACTOR_RIVER_STONE_K)->motionFlags = 0;
			fdMapGridCopyMetatileAttributes(9, 11, 1, 1, 11, 12);
		}
#endif
	}
	if (MainRoutineStateWork.next == SCENE_HESPERIA_SETTLEMENT)
	{
		if (fdScriptGetFlag(FLAG_VINE_GROWN))
		{
			fdMapGridCopyMetatileAttributes(0, 3, 1, 1, 3, 15);
			fdActorSetAnimation(ACTOR_SPROUT, 4);
		}
		if (fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION) && fdScriptGetFlag(FLAG_ON_WALL))
		{
			fdScriptClearFlag(FLAG_ON_WALL);
			scSettlementsStepOntoWall();
		}
		fdActorPlaceBlocks(SettlementsWallCrate);
	}
	if (MainRoutineStateWork.next == SCENE_GONDOWAN_SETTLEMENT_CAVE)
		fdEffectStartBlankOverlay(0);
	if (MainRoutineStateWork.next == SCENE_HESPERIA_SETTLEMENT_CAVE)
		fdEffectStartBlankOverlay(0);
	return 0;
}

Sint32 scPrepareMap(void)
{
	if (MainRoutineStateWork.next == SCENE_SOUTHWEST_ATTEKA_ISLET)
	{
		if (fdScriptGetFlag(FLAG_FALLS_CRATE_AT_REST))
		{
			fdMapGridCopyMetatileAttributes(9, 11, 1, 1, 11, 12);
			fdMapGridCopyMetatilesTo(12, 32, 11, 15, 1, 1);
			fdMapGridCopyMetatilesTo(77, 15, 75, 15, 1, 1);
		}
#if ATTEKA_ISLET_REBUILD_KEEPS_CRATE_CELL
		if (fdScriptGetFlag(FLAG_FALLS_CRATE_AT_REST))
			fdMapGridCopyMetatileAttributes(9, 11, 1, 1, 11, 11);
#else
		if (fdScriptGetFlag(FLAG_PILLAR_SUNK))
		{
			fdActorSetPos(ACTOR_PILLAR, 0, 0);
			fdActorSetPos(ACTOR_RIVER_STONE_K, 184, 184);
			fdActorGetById(ACTOR_RIVER_STONE_K)->motionFlags = 0;
			fdMapGridCopyMetatileAttributes(9, 11, 1, 1, 11, 12);
		}
#endif
	}
	if (MainRoutineStateWork.next == SCENE_GONDOWAN_SETTLEMENT)
	{
		if (fdScriptGetFlag(FLAG_LADDER_BUSH_CUT))
		{
			fdMapGridCopyMetatilesTo(5, 46, 5, 47, 1, 1);
			fdMapGridCopyMetatilesTo(4, 25, 5, 17, 1, 1);
			fdMapGridCopyMetatileAttributes(4, 25, 1, 1, 5, 17);
		}
	}
	return 0;
}

// Nothing visible happens when Felix climbs the ladder up the low cliff on Kalt Island's frozen
// river's west bank onto metatile (5, 12), the ledge at its top; from then on the ice no longer
// slides him, until he comes back down past metatile (5, 13). The ledge is the one the rope's west
// post stands on, at the river's edge: walking east from the ladder's top Felix stops beside the
// rope lying coiled on its post.
void scSettlementsStopIceSlides(void)
{
	syGameFlagSet(FLAG_ICE_SLIDES_STOPPED);
}

// Nothing visible happens when Felix climbs down onto metatile (5, 13) on Kalt Island's frozen
// river's west bank; the ice slides him again.
void scSettlementsResumeIceSlides(void)
{
	syGameFlagClear(FLAG_ICE_SLIDES_STOPPED);
}

// Felix steps onto Kalt Island's frozen river and slides along the ice in the way he was walking
// until he stops against a bank.
void scSettlementsStepOnIceSlide(void)
{
	fdLeaderSlideOnIce();
}

// Felix walks north into a house's door: the door in the cliff east of Tinder's ledge in Hesperia
// Settlement (metatile (23, 16)), or the door of Kalt Island's house (metatile (14, 18)). The
// people outside stop, the door opens, and he walks in as the screen fades.
void scSettlementsEnterHouse(void)
{
	Uint32 i;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	for (i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++)
	{
		ACTOR *actor = fdActorGet(i);

		if (actor != 0)
			actor->motionFlags = 0;
	}
	if (MainRoutineStateWork.next == SCENE_HESPERIA_SETTLEMENT)
		fdMapGridCopyMetatilesTo(33, 30, 23, 14, 1, 2);
	else
		fdMapGridCopyMetatilesTo(27, 29, 40, 16, 1, 2);
	fdScriptPlaySound(SE_DOOR_OPEN);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorCenterAndWalk(MainRoutineStateWork.leaderActorId, 2, -4);
	syFieldScriptWait(6);
	// Both houses' doors are their scene's exit 2.
	syFieldScriptSetExitRequest(HESPERIA_HOUSE_DOOR);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
}

// Felix casts Lash on the post at the west end of Kalt Island's rope, and the rope is tied across
// the frozen river to the post on its east bank.
void scSettlementsLashRope(void)
{
	fdRopeLash(1, ACTOR_ROPE_POST_A, ACTOR_ROPE_POST_B);
	fdScriptSetFlag(FLAG_KALT_ROPE_TIED);
}

extern Sint32 SettlementsWaterColorTick;

// An event on SW Atteka Islet: the falls' water runs, its four colours moving along by one every 8
// ticks.
void          scSettlementsCycleWaterColors(void)
{
	FIELD_STATE *state = (FIELD_STATE *)RamHeapSlots->fieldState;
	Sint32      *frame;
	Sint32       phase, i;

	if (state->psynergy != 0 || state->revealActive != 0)
		return;
	frame = &SettlementsWaterColorTick;
	if (*frame == -1)
	{
		Uint16 *colors = SettlementsLoadedWaterColors;

		syWordsCopy(
			colors, (void *)&BG_PLTT_COLOR(FALLS_PALETTE, 0), sizeof(SettlementsLoadedWaterColors));
		syWordsCopy(SettlementsShownWaterColors, colors, sizeof(SettlementsLoadedWaterColors));
		*frame = 0;
	}
	phase = (*frame / 8) % 4;
	for (i = 0; i < WATER_COLORS; i++)
		SettlementsShownWaterColors[FIRST_WATER_COLOR + (phase + i) % WATER_COLORS] =
			SettlementsLoadedWaterColors[FIRST_WATER_COLOR + i];
	{
		IO_WRITE_QUEUE *q = &IoWriteQueueWork;
		Uint32          saved;
		Sint32          count;

		IRQ_LOCK(saved);
		count = q->count;
		if (count < IO_WRITE_QUEUE_SIZE)
		{
			Uint32 *entry = q->entries[count];

			*entry++      = (Uint32)SettlementsShownWaterColors;
			q->count      = count + 1;
			*entry++      = (Uint32)&BG_PLTT_COLOR(FALLS_PALETTE, 0);
			*entry        = DMA_COPY_WORDS(PALETTE_COLORS / 2);
		}
		IRQ_UNLOCK(saved);
	}
	SettlementsWaterColorTick++;
}

// Starts SW Atteka Islet's falls: the running colours of the water, the sound of water, and the
// split of the screen that scScrollHBlank draws.
void scSettlementsStartWaterfall(void)
{
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	syFieldSetTickEvent(scSettlementsCycleWaterColors, EVENT_PASS_TICK);
	fdQueueMusic(SE_AMBIENT_WATER);
	scScrollStartHBlank();
}

// Each line of the screen as it is drawn: above the line scLayerTrackScroll sets, background layer
// BG1 keeps to the map and is drawn solid; below it, the layer drifts sideways with time and is
// drawn see-through.
void scScrollHBlank(void)
{
	if (REG_VCOUNT < SettlementsWaterfallLine)
	{
		syIOWrite16(SettlementsWaterfallScrollAbove, &REG_BG1HOFS);
		REG_BLDCNT = 0;
	}
	else
	{
		syIOWrite16(SettlementsWaterfallScrollBelow, &REG_BG1HOFS);
		REG_BLDALPHA = BLEND_ALPHA(12, 16);
		REG_BLDCNT   = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	}
}

// Each tick: where scScrollHBlank's line falls on screen, from the third map layer's scroll, and
// how far BG1 is scrolled above and below it.
void scLayerTrackScroll(void)
{
	MAP_LAYER *layer                = fdMapGridGetLayer(2);
	Sint16     y                    = FIXEDtoINT(layer->y);

	// The third layer's picture starts 1024 pixels down the map; the line is the map's row 424,
	// line 72 of the screen with the view at the foot of its box (render probe SouthwestAttekaIslet
	// --entrance 99).
	SettlementsWaterfallLine        = 1024 + 424 - y;
	SettlementsWaterfallScrollAbove = FIXEDtoINT(layer->x);
	SettlementsWaterfallScrollBelow = (Sint16)FIXEDtoINT(layer->x) - (TickCount >> 2);
}

#include "shared/scroll_start_hblank.h"

// Nothing visible happens as Felix casts Move on SW Atteka Islet's pillar or crate; the game frees
// the metatile it stands on so it can move.
void scSettlementsUnblockFallsObjects(void)
{
	fdActorLiftTargetBlock(SettlementsFallsObjects);
}

// Nothing visible happens as Felix starts to push SW Atteka Islet's pillar or crate; the game frees
// the metatile it stands on and keeps Felix drawn in front of it.
void scSettlementsStartPushingFallsObject(void)
{
	fdActorGet(MainRoutineStateWork.leaderActorId)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorLiftTargetBlock(SettlementsFallsObjects);
}

void scSettlementsSplashIntoRiver(Sint32 id);

// The pillar pushed off the grass into SW Atteka Islet's river (metatile (11, 12)) sinks with a
// splash until only its top shows, a post like the others in the water, which the crate can later
// stop against.
void scSettlementsSinkPillar(void)
{
	ACTOR *actor  = fdActorGetById(ACTOR_PILLAR);
	Sint32 column = FIELD_METATILE(actor->x.fixed);
	Sint32 row    = FIELD_METATILE(actor->z.fixed);
	Sint32 i;

	if (!fdScriptGetFlag(FLAG_PILLAR_SUNK) && column == 11 && row == 12)
	{
		actor->motionFlags = 0;
		for (i = 10; i > 0; i--)
		{
			actor->y.fixed -= toFIXED(2);
			fdScriptWait(1);
		}
		scSettlementsSplashIntoRiver(ACTOR_PILLAR);
		fdActorSetPos(ACTOR_PILLAR, 0, 0);
		fdActorSetPos(ACTOR_RIVER_STONE_K, 184, 184);
		fdActorGetById(ACTOR_RIVER_STONE_K)->motionFlags  = 0;
		fdActorGetById(ACTOR_RIVER_STONE_K)->y.fixed     -= toFIXED(64);
		fdMapGridCopyMetatileAttributes(9, 11, 1, 1, 11, 12);
		fdScriptSetFlag(FLAG_PILLAR_SUNK);
	}
}

// Sinks actor 2 pixels a tick for the given ticks, counting in i.
#define SINK(ticks)                                                                                \
	i = (ticks);                                                                                   \
	do                                                                                             \
	{                                                                                              \
		syFieldScriptWait(1);                                                                      \
		actor->y.fixed -= toFIXED(2);                                                              \
		i--;                                                                                       \
	} while (i > 0)

// The crate pushed into the river above SW Atteka Islet's falls (metatile (11, 9)) sinks in with a
// splash, drifts to the edge and goes down the falls with the camera watching. With the pillar sunk
// below, it sticks against it and makes the path to the Dragon Skin's chest; without, it is swept
// away and lost for this visit. Pushed off the grass's south edge at metatile (16, 12) instead, it
// drops down the cliff.
void scSettlementsSlideCrateDownFalls(void)
{
	ACTOR *actor  = fdActorGetById(ACTOR_FALLS_CRATE);
	Sint32 column = FIELD_METATILE(actor->x.fixed);
	Sint32 row    = FIELD_METATILE(actor->z.fixed);
	Sint32 i;
	Uint8 *flags;

	if (column == 11 && row == 9)
	{
		actor->motionFlags = 0;
		SINK(8);
		scSettlementsSplashIntoRiver(ACTOR_FALLS_CRATE);
		syFieldScriptWait(20);
		fdCameraMoveTo(192, CAMERA_KEEP, 224, 1);
		fdActorSetSpeed(ACTOR_FALLS_CRATE, 0.8);
		fdActorSetDestinationOffset(ACTOR_FALLS_CRATE, 0, 8);
		scFieldActorWaitForMoveById(ACTOR_FALLS_CRATE);
		syFieldScriptWait(10);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
		fdActorSetSpeed(ACTOR_FALLS_CRATE, 2.4);
		fdActorSetDestinationOffset(ACTOR_FALLS_CRATE, 0, 80);
		scFieldActorWaitForMoveById(ACTOR_FALLS_CRATE);
		scSettlementsSplashIntoRiver(ACTOR_FALLS_CRATE);
		if (!fdScriptGetFlag(FLAG_PILLAR_SUNK))
		{
			syFieldScriptWait(10);
			fdActorSetSpeed(ACTOR_FALLS_CRATE, 0.8);
			fdActorSetDestinationOffset(ACTOR_FALLS_CRATE, 0, 80);
			scFieldActorWaitForMoveById(ACTOR_FALLS_CRATE);
			fdActorSetSpeed(ACTOR_FALLS_CRATE, 2.5);
			fdActorSetDestinationOffset(ACTOR_FALLS_CRATE, 0, 80);
			syFieldScriptWait(5);
			fdActorSetPos(ACTOR_FALLS_CRATE, 0, 0);
			fdScriptSetFlag(FLAG_FALLS_CRATE_LOST);
		}
		else
		{
			fdMapGridCopyMetatilesTo(12, 32, 11, 15, 1, 1);
			fdMapGridCopyMetatilesTo(77, 15, 75, 15, 1, 1);
			fdActorSetDestinationOffset(ACTOR_FALLS_CRATE, 0, 12);
			scFieldActorWaitForMoveById(ACTOR_FALLS_CRATE);
			syFieldScriptWait(20);
			fdMapGridCopyMetatileAttributes(9, 11, 1, 1, 11, 11);
			fdActorSetAnimation(ACTOR_FALLS_CRATE, 2);
			syFieldScriptWait(30);
			fdScriptSetFlag(FLAG_FALLS_CRATE_AT_REST);
		}
		fdScriptEnd();
	}
	else if (!fdScriptGetFlag(FLAG_FALLS_CRATE_DROPPED) && column == 16 && row == 12)
	{
		fdMapGridCopyMetatileAttributes(17, 12, 1, 1, column, row);
		flags  = &actor->motionFlags;
		*flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		syFieldScriptWait(30);
		*flags = 0;
		fdScriptSetFlag(FLAG_FALLS_CRATE_DROPPED);
		fdScriptEnd();
	}
}

// Felix moves SW Atteka Islet's pillar with Move; moved into the river, it sinks.
void scSettlementsMovePillar(void)
{
	fdActorPlaceMovedBlock();
	scSettlementsSinkPillar();
}

// Felix moves SW Atteka Islet's crate with Move; moved into the river above the falls, it goes down
// them.
void scSettlementsMoveCrate(void)
{
	fdActorPlaceMovedBlock();
	scSettlementsSlideCrateDownFalls();
}

// Felix pushes SW Atteka Islet's pillar or crate one metatile: the pillar pushed into the river
// sinks, and the crate pushed into the river above the falls goes down them.
void scSettlementsPushIntoFalls(void)
{
	fdActorPlaceMovedBlock();
	fdActorGet(MainRoutineStateWork.leaderActorId)->priorityFlags -= ACTOR_PRIORITY_UNDERFOOT;
	scSettlementsSinkPillar();
	scSettlementsSlideCrateDownFalls();
}

// Keeps the metatiles of Hesperia Settlement's wall before its crate moves.
void scSettlementsSaveWallMetatiles(void)
{
	fdMapGridCopyMetatileAttributes(11, 73, 10, 10, 64, 0);
	fdMapGridCopyMetatileAttributes(42, 73, 10, 10, 64, 10);
}

// Gives Hesperia Settlement's wall its kept metatiles back.
void scSettlementsRestoreWallMetatiles(void)
{
	fdMapGridCopyMetatileAttributes(64, 0, 10, 10, 11, 73);
	fdMapGridCopyMetatileAttributes(64, 10, 10, 10, 42, 73);
}

// Nothing visible happens when Felix steps on metatile (13, 11), on the stairs in the cliff west of
// Hesperia Settlement's wall; the game swaps in the wall top's metatiles until he steps on metatile
// (10, 9). Until the crate is pushed off the wall it is done again when he comes back after a
// battle there. The metatiles swapped in are the overhead layer's (map.txt's rows 73 on are its
// rows 9 on): its closed block over the wall, metatiles (11, 11) to (20, 14), becomes open ground
// 96 pixels up at (12, 12) to (19, 14), ringed by closed metatiles, as the probe of the room with
// FLAG_ON_WALL set shows. What they let him do shows nowhere on screen: with the swap made (the
// room entered after a battle with FLAG_ON_WALL set, which makes it at entry) and without it, Move
// cast from metatile (17, 13) at the crate two metatiles north of him, with South held, pulls it
// one metatile south, and Felix then walks west and north past its old place along its west side,
// alike. A walk from metatile (12, 9) down the stairs onto (13, 11) and on into the fenced square
// did not run this.
void scSettlementsStepOntoWall(void)
{
	if (!fdScriptGetFlag(FLAG_WALL_CRATE_DROPPED) && !fdScriptGetFlag(FLAG_ON_WALL))
	{
		fdMapGridCopyMetatileAttributes(11, 73, 10, 10, 64, 50);
		fdMapGridCopyMetatileAttributes(42, 73, 10, 10, 64, 60);
		fdMapGridCopyMetatileAttributes(42, 11, 10, 4, 11, 75);
		fdScriptSetFlag(FLAG_ON_WALL);
	}
}

// Nothing visible happens when Felix steps on metatile (10, 9), at the other end of the stairs west
// of Hesperia Settlement's wall; the wall's own metatiles come back.
void scSettlementsStepOffWall(void)
{
	if (fdScriptGetFlag(FLAG_ON_WALL))
	{
		fdMapGridCopyMetatileAttributes(64, 50, 10, 10, 11, 73);
		fdMapGridCopyMetatileAttributes(64, 60, 10, 10, 42, 73);
		fdScriptClearFlag(FLAG_ON_WALL);
	}
}

// Nothing visible happens as Felix starts to move or push the crate on Hesperia Settlement's wall;
// the game frees the metatiles where it may go, by where it stands.
void scSettlementsUnblockCrate(void)
{
	ACTOR *actor = fdActorGet(ACTOR_WALL_CRATE);
	Sint32 x     = FIELD_METATILE(actor->x.fixed);
	Sint32 z     = FIELD_METATILE(actor->z.fixed);

	scSettlementsSaveWallMetatiles();
	fdActorLiftTargetBlock(SettlementsWallCrate);
	if (x == 12 && z == 15)
	{
		fdMapGridCopyMetatileAttributes(42, 3, 7, 3, 42, 14);
		fdMapGridCopyMetatileAttributes(42, 9, 10, 10, 11, 73);
	}
	else if (x == 13 && z == 15)
	{
		fdMapGridCopyMetatileAttributes(42, 0, 7, 3, 42, 14);
		fdMapGridCopyMetatileAttributes(42, 9, 10, 10, 11, 73);
	}
	else if (x == 16 && z == 15)
	{
		fdMapGridCopyMetatileAttributes(42, 0, 7, 3, 42, 14);
		fdMapGridCopyMetatileAttributes(42, 9, 10, 10, 11, 73);
	}
	else
	{
		fdMapGridCopyMetatileAttributes(42, 9, 10, 10, 11, 73);
	}
}

extern Sint32 SettlementsCrateDropScript[];

// Hesperia Settlement's crate pushed off the south edge of the wall drops with the camera following
// it and lands in a ring of dust: through the leftmost gap onto the step at metatile (12, 15), from
// there east to the ground at metatile (13, 15) (the way to Tinder, leaving a step up the wall), at
// metatile (16, 15) the wall's whole height to the ground, or onto Tinder's ledge at metatile (19,
// 15), where Tinder, not yet met, is gone for this visit.
void          scSettlementsDropCrate(void)
{
	ACTOR *actor;
	Sint32 column;
	Sint32 row;
	Sint32 i;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syScriptCameraFollowActor(ACTOR_WALL_CRATE, 1);
	actor  = fdActorGet(ACTOR_WALL_CRATE);
	column = FIELD_METATILE(actor->x.fixed);
	row    = FIELD_METATILE(actor->z.fixed);
	if (column == 12 && row == 15)
	{
		fdScriptSetFlag(FLAG_WALL_CRATE_DROPPED);
		actor->motionFlags = 0;
		SINK(32);
		scSettlementsSpawnDustRing(ACTOR_WALL_CRATE);
		syFieldScriptWait(20);
	}
	else if (!fdScriptGetFlag(FLAG_WALL_CRATE_ON_GROUND) && column == 16 && row == 15)
	{
		actor->motionFlags = 0;
		fdScriptSetFlag(FLAG_WALL_CRATE_ON_GROUND);
		fdScriptSetFlag(FLAG_WALL_CRATE_DROPPED);
		SINK(48);
		scSettlementsSpawnDustRing(ACTOR_WALL_CRATE);
		syFieldScriptWait(30);
	}
	else if (column == 19 && row == 15)
	{
		fdMapGridCopyMetatileAttributes(18, 16, 1, 1, column, row);
		actor->motionFlags = 0;
		fdScriptSetFlag(FLAG_WALL_CRATE_DROPPED);
		SINK(32);
		scSettlementsSpawnDustRing(ACTOR_WALL_CRATE);
		if (!fdScriptGetFlag(FLAG_DJINNI(DJINNI_TINDER)))
		{
			fdActorSetPos(ACTOR_TINDER, 0, 0);
			fdActorSetScript(ACTOR_WALL_CRATE, SettlementsCrateDropScript);
			syFieldScriptWait(20);
		}
		syFieldScriptWait(20);
	}
	else if (!fdScriptGetFlag(FLAG_WALL_CRATE_OFF_STEP) && column == 13 && row == 15)
	{
		fdMapGridCopyMetatileAttributes(1, 3, 1, 1, 12, row);
		fdMapGridCopyMetatileAttributes(1, 3, 1, 1, column, row);
		actor->motionFlags = 0;
		SINK(16);
		scSettlementsSpawnDustRing(ACTOR_WALL_CRATE);
		syFieldScriptWait(20);
		fdScriptSetFlag(FLAG_WALL_CRATE_OFF_STEP);
	}
	else if (column == 14 && row == 15)
	{
		fdMapGridCopyMetatileAttributes(1, 3, 1, 1, 12, row);
		fdMapGridCopyMetatileAttributes(1, 3, 1, 1, column, row);
	}
	fdScriptEnd();
}

// Felix has moved or pushed the crate on Hesperia Settlement's wall; pushed over the edge, it
// drops.
void scSettlementsBlockCrate(void)
{
	fdActorPlaceMovedBlock();
	scSettlementsRestoreWallMetatiles();
	scSettlementsDropCrate();
}

extern Sint32 SettlementsSmokeScript[];

// Water splashes up where the pillar or crate goes into the river.
void          scSettlementsSplashIntoRiver(Sint32 id)
{
	// LEFTOVER(frame): an unused local the ROM's frame reserves and never
	// * touches.
	UNUSED SETTLEMENTS_VECTOR velocity;
	EFFECT_OPTIONS            spawn;
	ACTOR                    *actor = fdActorGet(id);

	spawn.type                      = SPRITE_WATER_SPRAY;
	spawn.startScaleX               = toFIXED(4) / 5;
	spawn.startScaleY               = toFIXED(4) / 5;
	spawn.targetScaleX              = toFIXED(1);
	spawn.targetScaleY              = toFIXED(1);
	spawn.script                    = SettlementsSmokeScript;
	fdScriptPlaySound(SE_CAST);
	scFieldEffectSpawn(actor->x.fixed,
	                   actor->y.fixed,
	                   actor->z.fixed,
	                   0,
	                   0,
	                   0,
	                   EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET,
	                   &spawn);
}

#include "../field/field_effect_advance_motion.c"

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef SINK
void scFieldEffectSpawn(Sint32                x,
                        Sint32                y,
                        Sint32                z,
                        Sint32                velocityX,
                        Sint32                velocityY,
                        Sint32                velocityZ,
                        Uint32                flags,
                        const EFFECT_OPTIONS *options);

// Puffs a ring of dust out from the crate as it lands.
void scSettlementsSpawnDustRing(Sint32 actorId)
{
	ACTOR         *actor;
	VECTOR3        velocity;
	EFFECT_OPTIONS options;
	Uint32         angleStep;
	Sint32         angle;
	Sint32         velocityX;
	FIXED          velocityZ;

	actor          = fdActorGet(actorId);
	options.update = fdEffectAdvanceMotion;
	for (angleStep = 0; angleStep <= TRIG_TURN / FACING_STEP; angleStep += 2)
	{
		angle       = angleStep * FACING_STEP;
		velocity.x  = syCos(angle);
		velocity.y  = 0;
		velocityZ   = syMathSin(angle);
		velocityX   = velocity.x;
		velocity.z  = velocityZ;
		velocityX  += velocityX / 3;
		velocity.x  = velocityX;
		scFieldEffectSpawn(actor->x.fixed,
		                   actor->y.fixed,
		                   actor->z.fixed,
		                   velocityX,
		                   velocity.y,
		                   velocityZ,
		                   EFFECT_USE_UPDATE | EFFECT_SCRIPT_MEDIUM,
		                   &options);
	}
}

// Felix casts Lift on the round boulder on SW Atteka Islet's grass above the river, and the hands
// raise it up out of the way, opening the way to the posts in the river and the Jupiter Djinni.
void scSettlementsLiftBoulder(void)
{
	syGameFlagSet(FLAG_BOULDER_LIFTED);
}

extern Sint16 SlideOnIceSteerAngles[];

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

extern Sint16 SlideOnIceAnglesByKey[];

#include "shared/leader_slide_on_ice.h"

volatile Sint16 SlideOnIceStopped BSS_DATA;

extern Sint16                     SlideOnIceAnglesByKey[];
void                              fdActorSteerByPad(ACTOR *actor);
void                              fdLeaderStopIfPending(void);
Sint32                            fdMapGetTerrainHeight(Sint32 layer, Sint32 x, Sint32 z);

#include "shared/slide_on_ice_from_edge.c"

// Stops or restarts Felix's slide on the ice (fdLeaderSlideOnIce).
void scLeaderSetSlideStopped(Sint32 value)
{
	SlideOnIceStopped = value;
}

#include "shared/leader_slide_on_ice_ahead.h"

void          scSettlementsMeetGel(void);
void          scSettlementsMeetLostJupiterDjinni(void);
void          scSettlementsStepOntoLadderFromWest(void);
void          scSettlementsGrowVine(void);
void          scSettlementsEnterHouse(void);
void          scSettlementsPushIntoFalls(void);

extern Uint32 FieldEffectLongScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

// The blocks Move pushes, ended by ACTOR_LIST_END: by the falls, a pillar and a crate; by the wall, a crate.
Sint16 SettlementsFallsObjects[] = {
	ACTOR_PILLAR, ACTOR_FALLS_CRATE, ACTOR_LIST_END,
};
Sint16 SettlementsWallCrate[] = {
	ACTOR_WALL_CRATE, ACTOR_LIST_END,
};

#include "shared/slide_on_ice_angles.c"

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

Sint32 SettlementsBobScript[] = {
	SC_ANIMATION(3),
	SC_SET(SW_MOTION, 0),
	SC_ADD(SW_Y, -toFIXED(16)),
	SC_CALL((Sint32)scSettlementsClearSpriteFlags),
	SC_END,
};

Sint32 SettlementsSmokeScript[] = {
	SC_ANIMATION(2),
	SC_END,
};

Sint32 SettlementsCrateBobScript[] = {
	SC_SET(SW_MOTION, 0),
	SC_END,
};

Sint32 SettlementsCrateDropScript[] = {
	SC_CALL((Sint32)scActorBob),
	SC_END,
};

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 216, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{99, CONDITION_ALWAYS, 320, 0, 830, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_REGION SettlementsHesperiaSettlementHousesRegions[] = {
	{672, 46, 400, 688, 62, 416, CONDITION_ALWAYS, 14},
	{SCENE_TABLE_END},
};

SCENE_REGION SettlementsGondowanSettlementCaveRegions[] = {
	{212, 12, 132, 220, 20, 140, CONDITION_ALWAYS, 1},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(GondowanSettlement),
	SCENE_EXIT_UNLESS(GONDOWAN_SETTLEMENT_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_GONDOWAN_SETTLEMENT, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(GONDOWAN_SETTLEMENT_TO_GONDOWAN_SETTLEMENT_HOUSES_NORTH, GondowanSettlementHouses, GONDOWAN_SETTLEMENT_HOUSES_FROM_GONDOWAN_SETTLEMENT_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(GONDOWAN_SETTLEMENT_TO_GONDOWAN_SETTLEMENT_CAVE_A, GondowanSettlementCave, GONDOWAN_SETTLEMENT_CAVE_FROM_GONDOWAN_SETTLEMENT, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(GONDOWAN_SETTLEMENT_TO_GONDOWAN_SETTLEMENT_CAVE_B, GondowanSettlementCave, GONDOWAN_SETTLEMENT_CAVE_FROM_GONDOWAN_SETTLEMENT, EXIT_FLAG_NONE),
	SCENE_EXITS(GondowanSettlementHouses),
	SCENE_EXIT_UNLESS(GONDOWAN_SETTLEMENT_HOUSES_TO_GONDOWAN_SETTLEMENT_NORTH, GondowanSettlement, GONDOWAN_SETTLEMENT_FROM_GONDOWAN_SETTLEMENT_HOUSES_NORTH, EXIT_FLAG_NONE),
	SCENE_EXITS(GondowanSettlementCave),
	SCENE_EXIT_UNLESS(GONDOWAN_SETTLEMENT_CAVE_TO_GONDOWAN_SETTLEMENT, GondowanSettlement, GONDOWAN_SETTLEMENT_FROM_GONDOWAN_SETTLEMENT_CAVE_A, EXIT_FLAG_NONE),
	// Hesperia Settlement's entrance 4 is where the caves below come up.
	SCENE_EXIT_UNLESS(GONDOWAN_SETTLEMENT_CAVE_TO_HESPERIA_SETTLEMENT, HesperiaSettlement, HESPERIA_SETTLEMENT_FROM_CAVES, EXIT_FLAG_NONE),
	SCENE_EXITS(HesperiaSettlement),
	SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_HESPERIA_SETTLEMENT, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_TO_HESPERIA_SETTLEMENT_HOUSES_NORTH, HesperiaSettlementHouses, HESPERIA_SETTLEMENT_HOUSES_FROM_HESPERIA_SETTLEMENT_NORTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_TO_HESPERIA_SETTLEMENT_HOUSES_SOUTH_A, HesperiaSettlementHouses, HESPERIA_SETTLEMENT_HOUSES_FROM_HESPERIA_SETTLEMENT_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_TO_HESPERIA_SETTLEMENT_HOUSES_SOUTH_B, HesperiaSettlementHouses, HESPERIA_SETTLEMENT_HOUSES_FROM_HESPERIA_SETTLEMENT_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_TO_HESPERIA_SETTLEMENT_HOUSES_SOUTH_C, HesperiaSettlementHouses, HESPERIA_SETTLEMENT_HOUSES_FROM_HESPERIA_SETTLEMENT_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_TO_HESPERIA_SETTLEMENT_CAVE, HesperiaSettlementCave, HESPERIA_SETTLEMENT_CAVE_FROM_HESPERIA_SETTLEMENT, EXIT_FLAG_NONE),
	SCENE_EXITS(HesperiaSettlementCave),
	SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_CAVE_TO_HESPERIA_SETTLEMENT, HesperiaSettlement, HESPERIA_SETTLEMENT_FROM_CAVES, EXIT_FLAG_NONE),
	SCENE_EXITS(HesperiaSettlementHouses),
	SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_HOUSES_TO_HESPERIA_SETTLEMENT_NORTH, HesperiaSettlement, HESPERIA_SETTLEMENT_FROM_HESPERIA_SETTLEMENT_HOUSES_NORTH, EXIT_FLAG_NONE),
	// The stairs between the room with beds and the floor above it.
	SCENE_EXIT_UNLESS(12, HesperiaSettlementHouses, 13, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(13, HesperiaSettlementHouses, 12, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_HOUSES_TO_HESPERIA_SETTLEMENT_SOUTH, HesperiaSettlement, HESPERIA_SETTLEMENT_FROM_HESPERIA_SETTLEMENT_HOUSES_SOUTH_A, EXIT_FLAG_NONE),
	SCENE_EXITS(KaltIsland),
	SCENE_EXIT_UNLESS(KALT_ISLAND_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KALT_ISLAND, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(KALT_ISLAND_TO_KALT_ISLAND_HOUSES, KaltIslandHouses, KALT_ISLAND_HOUSES_FROM_KALT_ISLAND, EXIT_FLAG_NONE),
	SCENE_EXITS(KaltIslandHouses),
	SCENE_EXIT_UNLESS(KALT_ISLAND_HOUSES_TO_KALT_ISLAND, KaltIsland, KALT_ISLAND_FROM_KALT_ISLAND_HOUSES, EXIT_FLAG_NONE),
	SCENE_EXITS(SouthwestAttekaIslet),
	SCENE_EXIT_UNLESS(SOUTHWEST_ATTEKA_ISLET_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SOUTHWEST_ATTEKA_ISLET, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(SOUTHWEST_ATTEKA_ISLET_TO_SOUTHWEST_ATTEKA_ISLET_HOUSES_NORTH, SouthwestAttekaIsletHouses, SOUTHWEST_ATTEKA_ISLET_HOUSES_FROM_SOUTHWEST_ATTEKA_ISLET_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXITS(SouthwestAttekaIsletHouses),
	SCENE_EXIT_UNLESS(SOUTHWEST_ATTEKA_ISLET_HOUSES_TO_SOUTHWEST_ATTEKA_ISLET_NORTH, SouthwestAttekaIslet, SOUTHWEST_ATTEKA_ISLET_FROM_SOUTHWEST_ATTEKA_ISLET_HOUSES_NORTH, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE SettlementsPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 808, FACING_NORTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE SettlementsGondowanSettlementPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 192, 0, 240, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_SPIKY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 336, 0, 240, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE SettlementsGondowanSettlementHousesPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 120, 0, 632, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE SettlementsHesperiaSettlementPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 392, 0, 280, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 56, 0, 240, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, SettlementsCrateBobScript, 280, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_TINDER), MOVEMENT_TYPE_STAND, 312, 0, 248, FACING_WEST, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE SettlementsHesperiaSettlementHousesPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 168, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_SPIKY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 696, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE SettlementsAttekaIsletPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 176, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 200, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STACKED_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 168, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (Sint32)SettlementsBobScript, 120, 0, 72, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (Sint32)SettlementsBobScript, 72, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (Sint32)SettlementsBobScript, 88, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (Sint32)SettlementsBobScript, 120, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (Sint32)SettlementsBobScript, 72, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (Sint32)SettlementsBobScript, 104, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (Sint32)SettlementsBobScript, 104, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (Sint32)SettlementsBobScript, 120, 0, 184, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (Sint32)SettlementsBobScript, 152, 0, 184, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (Sint32)SettlementsBobScript, 152, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (Sint32)SettlementsBobScript, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 92, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_WIND19), MOVEMENT_TYPE_STAND, 72, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE SettlementsAttekaIsletHousesPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 96, 0, 816, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE SettlementsKaltIslandPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_GEL), MOVEMENT_TYPE_STAND, 312, 0, 296, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 88, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE SettlementsKaltIslandHousesPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 216, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 136, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

CUE SettlementsCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 12, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 13, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 14, CONDITION_ALWAYS, 9},
	{CUE_TABLE_END},
};
// clang-format on

CUE           SettlementsGondowanSettlementCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, 1},
    {CUE_EXIT, 2, CONDITION_ALWAYS, 2},
    {STEP_CUE_WALKING_INTO(FACING_SOUTH),
     GONDOWAN_LADDER_HOLE_NORTH, CONDITION_ALWAYS,
     SCRIPT_REF(scSettlementsTurnAndClimbDownLadder)},
    {STEP_CUE_WALKING_INTO(FACING_EAST),
     GONDOWAN_LADDER_HOLE_WEST, CONDITION_ALWAYS,
     SCRIPT_REF(scSettlementsStepOntoLadderFromWest)},
    {CUE_TALK,
     ACTOR_MAN_ON_THE_RIDDLE_OF_THE_ROCK, CONDITION_ALWAYS,
     SCRIPT_REF(scSettlementsWarnOfRiddleOfRock)},
    {CUE_TALK, ACTOR_GONDOWAN_BOY, CONDITION_ALWAYS, MSGNO(MsgSettlementsThereWasGiantExplosion)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_MAN_ON_THE_RIDDLE_OF_THE_ROCK, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsMountainIsCalledMagmaRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_GONDOWAN_BOY, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsExplosionsWithinMagmaRock)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     GONDOWAN_BUSH, CONDITION_ALWAYS,
     SCRIPT_REF(scSettlementsCutBushes)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     GONDOWAN_LADDER_BUSH, CONDITION_ALWAYS,
     SCRIPT_REF(scSettlementsCutLadderBush)},
    {CUE_TABLE_END},
};

CUE SettlementsGondowanSettlementHousesCues[] = {
	{CUE_EXIT, 11, CONDITION_ALWAYS, 1},
	{CUE_TALK,
     ACTOR_WOMAN_ON_THE_KIBOMBO, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsIHearKibomboHasFinally)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_WOMAN_ON_THE_KIBOMBO, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsWereLuckyRiverFloodsOver)},
	{CUE_TABLE_END},
};

CUE SettlementsHesperiaSettlementCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     HESPERIA_HOUSE_DOOR, CONDITION_ALWAYS,
     SCRIPT_REF(scSettlementsEnterHouse)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH),
     HESPERIA_LADDER_HOLE_NORTH, CONDITION_ALWAYS,
     SCRIPT_REF(scSettlementsTurnAndClimbDownLadder)},
	{STEP_CUE_WALKING_INTO(FACING_EAST),
     HESPERIA_LADDER_HOLE_WEST, CONDITION_ALWAYS,
     SCRIPT_REF(scSettlementsStepOntoLadderFromWest)},
	{STEP_CUE_WALKING_INTO(FACING_WEST),
     HESPERIA_LADDER_HOLE_EAST, CONDITION_ALWAYS,
     SCRIPT_REF(scSettlementsStepOntoLadderFromEast)},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_NORTH, 6, CONDITION_ALWAYS, 6},
	{CUE_STEP, HESPERIA_ONTO_WALL, FLAG_ON_WALL, SCRIPT_REF(scSettlementsStepOntoWall)},
	{CUE_STEP, HESPERIA_OFF_WALL, IF_FLAG_SET(FLAG_ON_WALL), SCRIPT_REF(scSettlementsStepOffWall)},
	{CUE_TALK, ACTOR_HESPERIA_MAN, CONDITION_ALWAYS, MSGNO(MsgSettlementsYouMustBeVeryBrave)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_HESPERIA_MAN, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsForContinentAsDesolate)},
	{CUE_TALK, ACTOR_TINDER, CONDITION_ALWAYS, SCRIPT_REF(scSettlementsMeetTinder)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_WALL_CRATE, CONDITION_ALWAYS,
     SCRIPT_REF(scSettlementsUnblockCrate)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_WALL_CRATE, CONDITION_ALWAYS,
     SCRIPT_REF(scSettlementsBlockCrate)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scSettlementsUnblockCrate)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scSettlementsBlockCrate)},
	{PSYNERGY_ON_ACTOR(ABILITY_GROWTH),
     ACTOR_SPROUT, FLAG_VINE_GROWN,
     SCRIPT_REF(scSettlementsGrowVine)},
	{CUE_TABLE_END},
};

CUE SettlementsHesperiaSettlementHousesCues[] = {
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 12, CONDITION_ALWAYS, 12},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, 13},
	{CUE_TALK,
     ACTOR_WOMAN_ON_SHAMAN_VILLAGE, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsTheresPlaceCalledShamanVillage)},
	{CUE_TALK,
     ACTOR_BOY_ON_THE_SHAMANS, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsPeopleOfShamanVillage)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_WOMAN_ON_SHAMAN_VILLAGE, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsIThinkTheresCaveDown)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_BOY_ON_THE_SHAMANS, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsLongAgoShamanWasRavaged)},
	{CUE_TABLE_END},
};

CUE SettlementsKaltIslandCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     KALT_HOUSE_DOOR, CONDITION_ALWAYS,
     SCRIPT_REF(scSettlementsEnterHouse)},
	{CUE_STEP, KALT_ICE_SLIDES_OFF, CONDITION_ALWAYS, SCRIPT_REF(scSettlementsStopIceSlides)},
	{CUE_STEP, KALT_ICE_SLIDES_ON, CONDITION_ALWAYS, SCRIPT_REF(scSettlementsResumeIceSlides)},
	{CUE_STEP, KALT_ICE, FLAG_ICE_SLIDES_STOPPED, SCRIPT_REF(scSettlementsStepOnIceSlide)},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH),
     ACTOR_ROPE_POST_A, FLAG_KALT_ROPE_TIED,
     SCRIPT_REF(scSettlementsLashRope)},
	{CUE_TALK, ACTOR_GEL, CONDITION_ALWAYS, SCRIPT_REF(scSettlementsMeetGel)},
	{PSYNERGY_ON_ACTOR(ABILITY_CATCH),
     ACTOR_APPLE, FLAG_KALT_ISLAND_APPLE_TAKEN,
     SCRIPT_REF(scSettlementsCatchApple)},
	{CUE_TABLE_END},
};

CUE SettlementsKaltIslandHousesCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_TALK,
     ACTOR_OLD_MAN_ON_THE_GLACIERS, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsIfYouWantToCross)},
	{CUE_TALK,
     ACTOR_OLD_WOMAN_ON_PROX_TRADE, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsLongTimeAgoWeUsed)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_OLD_MAN_ON_THE_GLACIERS, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsPeopleOfLohoHaveLegendary)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_OLD_WOMAN_ON_PROX_TRADE, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsNorthernSeaIsThick)},
	{EXAMINE_CUE(EXAMINE_OVEN),
     200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgSettlementsTheresKalticStewMade))},
	{CUE_TABLE_END},
};

CUE SettlementsAttekaIsletCues[] = {
	{CUE_EXIT | CUE_FLAG_FACING | FACING_NORTH, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_NORTH, 2, CONDITION_ALWAYS, 2},
	{CUE_TALK, ACTOR_ISLANDER, CONDITION_ALWAYS, MSGNO(MsgSettlementsSeaWestOfHereSpills)},
	{CUE_TALK,
     ACTOR_JUPITER_DJINNI, CONDITION_ALWAYS,
     SCRIPT_REF(scSettlementsMeetLostJupiterDjinni)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_ISLANDER, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsMyBuddyThinksImNuts)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PILLAR, FLAG_PILLAR_SUNK,
     SCRIPT_REF(scSettlementsUnblockFallsObjects)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_PILLAR, FLAG_PILLAR_SUNK,
     SCRIPT_REF(scSettlementsMovePillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FALLS_CRATE, FLAG_FALLS_CRATE_AT_REST,
     SCRIPT_REF(scSettlementsUnblockFallsObjects)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_FALLS_CRATE, FLAG_FALLS_CRATE_AT_REST,
     SCRIPT_REF(scSettlementsMoveCrate)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scSettlementsStartPushingFallsObject)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scSettlementsPushIntoFalls)},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT),
     ACTOR_BOULDER, FLAG_BOULDER_LIFTED,
     SCRIPT_REF(scSettlementsLiftBoulder)},
	{CUE_TABLE_END},
};

CUE SettlementsAttekaIsletHousesCues[] = {
	{CUE_EXIT, 11, CONDITION_ALWAYS, 1},
	{CUE_TALK,
     ACTOR_ISLANDER_ON_THE_WEST, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsTheresNothingToWest)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_ISLANDER_ON_THE_WEST, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsCurrentsAreSoStrongNear)},
	{CUE_TABLE_END},
};

// The ticks the falls' water colours have run; -1 before they start.
Sint32 SettlementsWaterColorTick = -1;
