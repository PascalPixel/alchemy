// Lighthouse Aerie (SceneIdVenusLighthouseAerie), the top of Venus Lighthouse and the only scene of
// this overlay.
//
// What happens here is one chain of scripts cut by two battles and two trips to the World Map. Each
// part starts in scInit from the entrance the part before sent the party to. Every arrival blocks
// Retreat and sets FLAG_IN_VENUS_LIGHTHOUSE.
//
// The parts, in the order the entrances chain them:
// - The arrival (VENUS_LIGHTHOUSE_AERIE_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR,
//       scVenusLighthouseAerieOverhearSaturosAndMenardi). A floating platform
//       carries Isaac up beside the tower in the north-east onto its top, and
//       the party overhears Saturos, Menardi, Felix and Sheba arguing at the
//       beacon, with one yes-or-no question that changes only the reply.
// - Two rows of three floating platforms bob in the air, one between the
//       tower and the middle of the aerie and one west of the middle
//       (METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_PLATFORM,
// METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_FLOOR_BY_PLATFORMS). Isaac jumps from one to the next,
// and each stops bobbing and dips under him as he lands. FLAG_VENUS_AERIE_ON_PLATFORM is set while
// he stands on one.
// - The confrontation, when Isaac walks up to the beacon
//       (METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_BEFORE_BEACON,
//       scVenusLighthouseAerieConfrontSaturosAndMenardi). The Shaman's Rod is
//       handed over for Sheba's sake, remembered in FLAG_SHAMANS_ROD_CARRIED_BY
//       plus the party member who carried it. The first battle follows
//       (ENCOUNTER_ZONE_SCRIPTED, slot 1, FLAG_AERIE_FIRST_BATTLE_BEGUN).
// - After it (VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE,
//       scVenusLighthouseAerieCombineSaturosAndMenardi). Saturos throws the
// Venus Star into the beacon (ACTOR_VENUS_STAR_SHADOW), Felix and Sheba leave,
//       and Saturos and Menardi fuse. gmPartyRestorePpInLighthouse refills the
// PP of Isaac and Garet, inside the lighthouse of their element group, and the
//       second battle follows (slot 0, FLAG_AERIE_SECOND_BATTLE_BEGUN).
// - After that (VENUS_LIGHTHOUSE_AERIE_AFTER_SECOND_BATTLE,
//       scVenusLighthouseAerieFinishSaturosAndMenardiBattle). Saturos and
// Menardi fall into the lighthouse, and the beacon erupts. Its beam
//       (ACTOR_BEAM_BOTTOM to ACTOR_BEAM_TOP) rises, the aerie splits and its
//       statues move apart. The light lifts the party away to the World Map's
//       lighthouse transition (WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT). The
// beam's five segments are sprites stacked over the beacon, with particles rising round them. The
// split is drawn by copying the map's metatiles
//       (fdMapGridCopyMetatilesTo in scVenusLighthouseAerieEruptBeacon).
// - A look back (VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT,
//       scVenusLighthouseAerieShowShebaInDanger). Sheba hangs from the broken
//       aerie, and VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP returns to the World Map
//       (WORLD_MAP_BEFORE_SHEBAS_FALL) with its music kept.
// - Sheba's fall (VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP,
//       scVenusLighthouseAerieStageShebasFall). Sheba and Felix fall, and the
//       party is taken to Lalivero (VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_C)
//       with FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED set.
//
// A lost battle returns to VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT, which gives the Shaman's Rod back
// to whoever carried it and leaves for Lalivero Sanctum (VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_B).

#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/field_psynergy_state.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "scene/script_psynergy.h"
#include "text/msg_ids.h"

extern Sint32         VenusLighthouseAerieJoinLeaderScript[];
extern Sint32         VenusLighthouseAerieStepBackWestScript[];
extern Sint32         VenusLighthouseAerieStepBackEastScript[];
extern Sint32         VenusLighthouseAerieBreatheHeavilyScript[];
extern Sint32         VenusLighthouseAerieBeamOpenScript[];
extern Sint32         VenusLighthouseAerieBeamNarrowScript[];
extern Sint32         VenusLighthouseAerieBeamRiseScript[];
extern Sint32         VenusLighthouseAerieBeamCloseScript[];
extern Sint32         VenusLighthouseAerieBeamEndScript[];
extern Sint32         VenusLighthouseAerieOrbitParticleScript[];
extern Sint32         VenusLighthouseAerieRiseParticleScript[];
extern Sint32         VenusLighthouseAerieWalkOutScript[];
extern Sint32         VenusLighthouseAerieOrbPulseScript[];
extern Sint32         VenusLighthouseAerieShebaFallScript[];
extern Sint32         VenusLighthouseAerieFelixDiveScript[];
extern Sint32         VenusLighthouseAeriePartyExitScript[];
extern Sint32         VenusLighthouseAerieLookAboutScript[];
extern SCENE_ENTRANCE VenusLighthouseAerieEntrances[];
extern Uint32         VenusLighthouseAerieExits[];
extern ACTOR_PLACE    VenusLighthouseAerieActorPlacements[];
extern Sint32         VenusLighthouseAerieLandedPlatform;
extern CUE            VenusLighthouseAerieCues[];
extern Sint32         BeaconSparkScript[];

// The aerie's actors, in the order of its actor placements (its entrances and exits are in
// scene_exits.h); Garet, Ivan, Mia and Sheba keep their character numbers.
enum
{
	// The four statues around the beacon (SPRITE_STATUE_WITH_WREATH), the east two drawn mirrored.
	// When the beacon erupts and splits the aerie, the west two move west with their half and the
	// east two east.
	ACTOR_BEACON_STATUE_SOUTHWEST = 8,
	ACTOR_BEACON_STATUE_NORTHWEST = 9,
	ACTOR_BEACON_STATUE_NORTHEAST = 10,
	ACTOR_BEACON_STATUE_SOUTHEAST = 11,
	// Six round platforms (SPRITE_FLOATING_PLATFORM) the leader jumps across, each dipping under
	// him as he lands, and the one that carries him up to the aerie.
	ACTOR_FIRST_PLATFORM          = 12,
	ACTOR_LAST_PLATFORM           = 17,
	ACTOR_RISING_PLATFORM         = 18,
	ACTOR_MENARDI                 = 19,
	ACTOR_SATUROS                 = 20,
	ACTOR_FELIX                   = 21,
	// A blank actor (SPRITE_BLANK) that runs across the floor under the Venus Star as Saturos
	// throws it into the beacon, so that only its shadow shows, the star's shadow.
	ACTOR_VENUS_STAR_SHADOW       = 22,
	// The orb the rising particles circle as the party is lifted away: SPRITE_BLUE_ORB drawn in
	// SPRITE_COLOR_GBR, which turns its blue to green, and later solid white (SPRITE_COLOR_WHITE).
	ACTOR_BEACON_ORB              = 23,
	// The beacon's beam, in five segments (SPRITE_PIPE_NORTH_SOUTH) from the bottom up; particles
	// circle the lowest.
	ACTOR_BEAM_BOTTOM             = 24,
	ACTOR_BEAM_LOWER              = 25,
	ACTOR_BEAM_MIDDLE             = 26,
	ACTOR_BEAM_UPPER              = 27,
	ACTOR_BEAM_TOP                = 28
};

// An area flag, plus the party member (0 to 3) who carried the Shaman's Rod when it was handed over
// before the aerie's first battle. A defeat gives it back.
#define FLAG_SHAMANS_ROD_CARRIED_BY FLAG_AREA_TEMP_45

// Area flags that choose the aerie's field music after each battle.
//
// They work by the scene's field music rules (SceneMusicRules, field/music_rules.s). The music is
// MUS_SILENCE before the first battle, MUS_BABIS_THEME once the first has begun and
// MUS_ELEMENTAL_STARS once the second has.
//
// VenusLighthouseFirstFloor and VenusLighthouseFourthFloor, in the same area, keep their Psynergy
// Stones in the same two flags, which no one goes back down to see.
enum
{
	FLAG_AERIE_FIRST_BATTLE_BEGUN  = FLAG_AREA_TEMP_50,
	FLAG_AERIE_SECOND_BATTLE_BEGUN = FLAG_AREA_TEMP_51
};

static void scVenusLighthouseAerieFloatPlatforms(void);
static void scVenusLighthouseAerieOverhearSaturosAndMenardi(void);
static void scVenusLighthouseAerieShowPartyMember(Sint32 actorId, Sint32 shown);
static void scVenusLighthouseAerieFlashActorPalette(ACTOR *actor);
static void scSpawnPsynergyRings(ACTOR *parent);
static void scVenusLighthouseAerieUpdateShebaPalette(void);
static void scVenusLighthouseAerieCombineSaturosAndMenardi(void);
static void scVenusLighthouseAerieFlashScreen(void);
static void scVenusLighthouseAerieSpawnUpperOrbitParticles(void);
static void scVenusLighthouseAerieUpdateFusion(void);
static void scVenusLighthouseAerieFinishSaturosAndMenardiBattle(void);
static void scVenusLighthouseAerieShowShebaInDanger(void);
static void scVenusLighthouseAerieStageShebasFall(void);
static void scVenusLighthouseAerieShowSaturosAndMenardiFall(void);
static void scVenusLighthouseAerieDiscussSaturosAndMenardiDefeat(void);
static void scVenusLighthouseAerieEruptBeacon(void);
static void scVenusLighthouseAerieRestorePartyOnBrokenAerie(void);
static void scVenusLighthouseAerieTryToPreventFall(void);
static void scVenusLighthouseAeriePlayShebasFall(void);
static void scVenusLighthouseAerieStopActorMotion(ACTOR *actor);
static void scVenusLighthouseAerieUpdateBeaconRise(void);
static void scVenusLighthouseAerieAdvanceActorRise(ACTOR *actor);
static void scVenusLighthouseAerieSpawnSideOrbitParticles(void);
static void scVenusLighthouseAerieUpdateOrbitingEffect(ACTOR *actor);

// The metatile behaviors the aerie's step cues answer.
typedef enum VENUS_LIGHTHOUSE_AERIE_METATILE_BEHAVIOR
{
	// before the beacon, where Isaac confronts Saturos and Menardi
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_BEFORE_BEACON      = 5,
	// a floating platform's metatiles
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_PLATFORM           = 10,
	// The floor beside the platforms, from which the leader jumps onto them.
	METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_FLOOR_BY_PLATFORMS = 11
} VENUS_LIGHTHOUSE_AERIE_METATILE_BEHAVIOR;

// Animations the aerie plays by number beyond ACTOR_ANIMATION.
//
// They come from each sprite's animation table (graphics/sprite/<name>.tsv). The party's are
// reached through their animation lists.
enum
{
	// Menardi standing, her hair blown back (menardi.tsv).
	MENARDI_ANIMATION_STAND_WINDSWEPT = 5,
	MENARDI_ANIMATION_LIE             = 7, // lying on her side
	                           // Menardi falling into the lighthouse, as SATUROS_ANIMATION_TIP to
	                           // _SINK.
	MENARDI_ANIMATION_TIP             = 8,
	MENARDI_ANIMATION_SLUMP           = 9,
	MENARDI_ANIMATION_SINK            = 10,
	// Garet on his knees, Ivan and Mia lying down, as the party lies on the broken aerie (garet.tsv
	// to mia.tsv).
	ANIM_PARTY_KNOCKED_DOWN           = 18,
	ANIM_LEADER_KNOCKED_DOWN          = 19 // Isaac lying on his back (isaac.tsv)
};

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

// The heavy breathing of someone worn out: Saturos and Menardi lying hurt after the first battle,
// and Isaac as he gets up after Sheba's fall. The sprite stretches taller and narrower and settles
// back over six ticks, then keeps still for 80 to 159 ticks before the next breath.
static Sint32 scVenusLighthouseAeriePulseActorScale(ACTOR *actor)
{
	switch (actor->timer)
	{
		case 6:
			actor->scaleX -= toFIXED(1) / 8;
			actor->scaleY += toFIXED(1) / 16;
			break;
		case 4:
			actor->scaleX += toFIXED(1) / 16;
			actor->scaleY -= toFIXED(1) / 32;
			break;
		case 2:
			actor->scaleX += toFIXED(1) / 32;
			actor->scaleY -= toFIXED(1) / 64;
			break;
		case 0:
			actor->scaleX += toFIXED(1) / 32;
			actor->scaleY -= toFIXED(1) / 64;
			actor->timer   = syMathRemainderUnsigned(syRandomNext(), 80) + 80;
			break;
	}
	{
		Sint32 ticks = (Uint16)actor->timer;
		ticks--;
		actor->timer = ticks;
	}
	return 1;
}

// A drifting particle loses a PARTICLE_DRAG_X-th of its x speed and a PARTICLE_DRAG_Z-th of its z
// speed each tick.
enum
{
	PARTICLE_DRAG_X = 22,
	PARTICLE_DRAG_Z = 20
};

// A particle of the bursts that fly out of Saturos and Menardi as they draw on the beacon: it slows
// as it drifts, turning and shrinking.
static void scVenusLighthouseAerieUpdateDriftingParticle(ACTOR *actor)
{
	FIELD_EFFECT *effect    = (FIELD_EFFECT *)actor;
	Sint32        velocityX = effect->velocity.x;
	Sint32        velocityZ;

	effect->x                += velocityX;
	effect->y                += effect->velocity.y;
	velocityZ                 = effect->velocity.z;
	effect->z                += velocityZ;
	effect->velocity.x        = velocityX - velocityX / PARTICLE_DRAG_X;
	effect->velocity.z        = velocityZ - velocityZ / PARTICLE_DRAG_Z;
	effect->scaleX           += effect->scaleRateX;
	effect->scaleY           += effect->scaleRateY;
	effect->sprite->rotation += effect->spin;
}

// Felix's sprite turns as he dives off the broken aerie after Sheba, 5/128 of a turn a tick, until
// he has turned over: from upright, 13 ticks and about half a turn.
static void scVenusLighthouseAerieStepSpriteRotation(ACTOR *actor)
{
	SPR   *sprite   = actor->sprite;
	Uint16 rotation = sprite->rotation;

	if ((Sint16)(rotation - 1) < 0)
	{
		{
			Sint32 nextRotation = rotation - TRIG_TURN * 5 / 128;
			sprite->rotation    = nextRotation;
		}
	}
}

#include "shared/actor_heading.c"

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return VenusLighthouseAerieEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return VenusLighthouseAerieExits;
}

// Returns the beacon's statues, the platforms, Saturos and Menardi, Felix and Sheba, the party and
// the beacon effects.
const ACTOR_PLACE *scGetActorPlacements(void)
{
	return VenusLighthouseAerieActorPlacements;
}

#include "shared/actor_distance.c"
#include "shared/floating_platform.c"

// Saturos and Menardi slide together as they fuse.
#define FLAG_VENUS_AERIE_FUSION_SLIDING FLAG_SCENE_TEMP_34
// Saturos and Menardi grow as they fuse.
#define FLAG_VENUS_AERIE_FUSION_GROWING FLAG_SCENE_TEMP_35
// The beacon has erupted, and its particles rise higher.
#define FLAG_VENUS_AERIE_BEACON_ERUPTED FLAG_SCENE_TEMP_36
// The beacon's light has lifted the party and flashed white.
#define FLAG_VENUS_AERIE_PARTY_LIFTED   FLAG_SCENE_TEMP_37
// The leader stands on one of the floating platforms.
#define FLAG_VENUS_AERIE_ON_PLATFORM    FLAG_SCENE_TEMP_50

// Walking off the edge of the floor or of a platform toward the next one, Isaac springs and leaps
// two metatiles across onto it, if there is somewhere to land.
static void scVenusLighthouseAerieJumpToNextPlatform(void)
{
	ACTOR *actor;

	union
	{
		FIELD_POS pos;
		Sint32    coordinates[3];
	} dest;

	Sint32 angle;
	Uint8  flags;

	actor = fdActorGet(ACTOR_PARTY_LEADER);
	angle = FACING_NEAREST_EIGHTH(actor->facing);
	flags = actor->motionFlags;
	SET_VECTOR(dest.coordinates,
	           (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8),
	           actor->y.fixed,
	           (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(8));
	syVectorAddPolarOffset(toFIXED(32), angle, dest.coordinates);
	if (syFieldActorCheckMovementCollision(actor, &dest.pos) == 0)
	{
		gmFlagClear(FLAG_VENUS_AERIE_ON_PLATFORM);
		scVenusLighthouseAerieFloatPlatforms();
		fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
		sySynch(6);
		fdActorSelectAnimation(actor, ANIM_LEADER_LEAP);
		actor->speedLimit   = toFIXED(3);
		actor->acceleration = toFIXED(2);
		sySoundPlay(SE_JUMP);
		actor->velocityY    = toFIXED(4);
		actor->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
		fdActorSetSpriteFlags(actor, 0);
		fdActorMoveToAndWait(
			ACTOR_PARTY_LEADER, FIXEDtoINT(dest.coordinates[0]), FIXEDtoINT(dest.coordinates[2]));
		fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
		fdActorSetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
		actor->motionFlags = flags;
	}
}

// As Isaac lands on a platform, it stops bobbing, dips under him and springs back up.
static void scVenusLighthouseAerieLandOnPlatform(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	ACTOR *platform;
	Sint32 platformId;
	FIXED  reboundStep;

	fdScriptStart();
	platformId                         = scActorFindNearestFloatingPlatform();
	VenusLighthouseAerieLandedPlatform = platformId;
	if (platformId != 0)
	{
		gmFlagSet(FLAG_VENUS_AERIE_ON_PLATFORM);
		platform               = fdActorGet(VenusLighthouseAerieLandedPlatform);
		platform->motionFlags  = 0;
		leader->motionFlags   &= ~ACTOR_FOLLOWS_TERRAIN;
		platform->y.fixed     -= toFIXED(3);
		leader->y.fixed       -= toFIXED(3);
		leader->groundHeight  -= toFIXED(3);
		sySynch(2);
		platform->y.fixed    -= toFIXED(2);
		leader->y.fixed      -= toFIXED(2);
		leader->groundHeight -= toFIXED(2);
		sySynch(10);
		reboundStep           = toFIXED(2);
		platform->y.fixed    += reboundStep;
		leader->y.fixed      += reboundStep;
		leader->groundHeight += reboundStep;
		sySynch(4);
		platform->y.fixed    += reboundStep;
		leader->y.fixed      += reboundStep;
		leader->groundHeight += reboundStep;
		sySynch(4);
		platform->y.fixed    += toFIXED(1);
		leader->y.fixed      += toFIXED(1);
		leader->groundHeight += toFIXED(1);
	}
	fdScriptEnd();
}

// Stepping onto the floor at either end of a row of platforms (metatiles (12,
// 12) and (25, 12) beside the middle of the aerie, and (31, 10) on the tower in
// the north-east), and just before each jump, sets all six platforms bobbing in the air again, so
// the one Isaac stood on no longer holds still.
static void scVenusLighthouseAerieFloatPlatforms(void)
{
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlags       = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorGet(ACTOR_FIRST_PLATFORM)->motionFlags     = ACTOR_FLOATS;
	fdActorGet(ACTOR_FIRST_PLATFORM + 1)->motionFlags = ACTOR_FLOATS;
	fdActorGet(ACTOR_FIRST_PLATFORM + 2)->motionFlags = ACTOR_FLOATS;
	fdActorGet(ACTOR_FIRST_PLATFORM + 3)->motionFlags = ACTOR_FLOATS;
	fdActorGet(ACTOR_FIRST_PLATFORM + 4)->motionFlags = ACTOR_FLOATS;
	fdActorGet(ACTOR_LAST_PLATFORM)->motionFlags      = ACTOR_FLOATS;
}

// The aerie's scripts: the platforms, and the beacon where Saturos and Menardi wait.
const CUE *scGetCues(void)
{
	return VenusLighthouseAerieCues;
}

// Sets up the beacon's statues and the six platforms.
Sint32 scInit(void)
{
	FIXED  mirroredScale;
	Uint32 platformId;
	Sint16 entrance;
	ACTOR *platform;

	gmFlagSet(FLAG_RETREAT_BLOCKED);
	sySynch(1);
	gmFlagSet(FLAG_IN_VENUS_LIGHTHOUSE);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BEACON_STATUE_SOUTHWEST), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BEACON_STATUE_NORTHWEST), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BEACON_STATUE_NORTHEAST), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BEACON_STATUE_SOUTHEAST), 0);
	mirroredScale                                     = -toFIXED(1);
	fdActorGet(ACTOR_BEACON_STATUE_NORTHEAST)->scaleX = mirroredScale;
	fdActorGet(ACTOR_BEACON_STATUE_SOUTHEAST)->scaleX = mirroredScale;
	for (platformId = ACTOR_FIRST_PLATFORM; platformId <= ACTOR_LAST_PLATFORM; platformId++)
	{
		platform = fdActorGet(platformId);
		fdActorSetSpriteFlags(fdActorGet(platformId), 0);
		fdActorSetSpritePriority(platformId, 1);
		platform->motionFlags    = ACTOR_FLOATS;
		platform->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
		platform->y.fixed        = toFIXED(1) / 2;
	}

	entrance = MainRoutineStateWork.entrance;

	switch (entrance)
	{
		case VENUS_LIGHTHOUSE_AERIE_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR:
			if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
			{
				scVenusLighthouseAerieOverhearSaturosAndMenardi();
			}
			break;
		case VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE:
			scVenusLighthouseAerieCombineSaturosAndMenardi();
			break;
		case VENUS_LIGHTHOUSE_AERIE_AFTER_SECOND_BATTLE:
			scVenusLighthouseAerieFinishSaturosAndMenardiBattle();
			break;
		case VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT:
			scVenusLighthouseAerieShowShebaInDanger();
			break;
		case VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP: scVenusLighthouseAerieStageShebasFall(); break;
		case VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT:
			fdScriptStart();
			if (gmFlagGet(FLAG_SHAMANS_ROD_CARRIED_BY) != 0)
			{
				gmInventoryAddItem(ISAAC, ITEM_SHAMANS_ROD);
			}
			else if (gmFlagGet(FLAG_SHAMANS_ROD_CARRIED_BY + 1) != 0)
			{
				gmInventoryAddItem(GARET, ITEM_SHAMANS_ROD);
			}
			else if (gmFlagGet(FLAG_SHAMANS_ROD_CARRIED_BY + 2) != 0)
			{
				gmInventoryAddItem(IVAN, ITEM_SHAMANS_ROD);
			}
			else
			{
				gmInventoryAddItem(MIA, ITEM_SHAMANS_ROD);
			}
			syImportScriptSetExitRequest(VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_B);
			break;
	}

	if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0)
	{
		Sint32 slot = scActorFindNearestFloatingPlatform();

		if (slot != 0)
		{
			ACTOR *nearestPlatform = fdActorGet(slot);
			if (nearestPlatform != NULL)
			{
				nearestPlatform->motionFlags = 0;
			}
		}
	}
	return 0;
}

// Shows the script's next line, then waits ten ticks.
static void scVenusLighthouseAerieShowMessage(Sint32 speaker)
{
	fdScriptShowMessage(speaker, 0);
	fdScriptWait(10);
}

// Turns one actor and waits ten ticks.
static void scVenusLighthouseAerieFaceActor(Sint32 actorId, Sint32 facing)
{
	fdActorFaceDirection(actorId, facing, 10);
}

// A floating platform carries Isaac up beside the tower in the north-east of the aerie onto its
// top, and the party looks across to the beacon, where Saturos and Menardi argue with Felix over
// Sheba and the lighthouses still to light.
static void scVenusLighthouseAerieOverhearSaturosAndMenardi(void)
{
	ACTOR *arrivalPlatform;
	ACTOR *leader;

	arrivalPlatform = fdActorGet(ACTOR_RISING_PLATFORM);
	fdScriptStart();
	scVenusLighthouseAerieShowPartyMember(ACTOR_GARET, 0);
	scVenusLighthouseAerieShowPartyMember(ACTOR_IVAN, 0);
	scVenusLighthouseAerieShowPartyMember(ACTOR_MIA, 0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	arrivalPlatform->motionFlags    = 0;
	arrivalPlatform->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetSpriteFlags(fdActorGet(ACTOR_RISING_PLATFORM), 0);
	fdActorSetSpritePriority(ACTOR_RISING_PLATFORM, 1);
	fdActorSetPos(ACTOR_RISING_PLATFORM, 580, 338);
	leader              = fdActorGet(ACTOR_PARTY_LEADER);
	leader->motionFlags = 0;
	fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 581, 288);
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_RISING_PLATFORM, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorSetDestination(ACTOR_RISING_PLATFORM, 580, 221);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 581, 171);
	fdActorSetDestination(ACTOR_RISING_PLATFORM, 530, 211);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 531, 161);
	fdActorSetDestination(ACTOR_RISING_PLATFORM, 520, 191);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 521, 141);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_RISING_PLATFORM), SPRITE_FLAG_SHADOW);
	fdActorSetDestination(ACTOR_RISING_PLATFORM, 515, 171);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 516, 121);
	sySoundPlay(SE_STOP_ACTION_SE);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_SPRING);
	fdActorWaitForAnimationChange(ACTOR_PARTY_LEADER);
	leader->x.fixed     = toFIXED(516);
	leader->y.fixed     = toFIXED(8);
	leader->z.fixed     = toFIXED(148);
	leader->facing      = FACING_WEST;
	leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	sySoundPlay(SE_JUMP);
	leader->velocityY = toFIXED(4);
	sySoundPlay(SE_JUMP);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 504, 148);
	fdScriptWait(10);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
	leader->facing                                 = FACING_SOUTH;
	leader->y.fixed                                = -toFIXED(32);
	fdScriptWait(20);
	sySoundPlay(SE_MATERIALIZE);
	fdActorMoveToAndWait(ACTOR_RISING_PLATFORM, 524, 191);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_RISING_PLATFORM), 0);
	fdActorMoveToAndWait(ACTOR_RISING_PLATFORM, 530, 211);
	fdActorMoveToAndWait(ACTOR_RISING_PLATFORM, 580, 221);
	fdActorSetDestination(ACTOR_RISING_PLATFORM, 580, 338);
	fdActorGet(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	scVenusLighthouseAerieShowPartyMember(ACTOR_GARET, 1);
	scVenusLighthouseAerieShowPartyMember(ACTOR_IVAN, 1);
	scVenusLighthouseAerieShowPartyMember(ACTOR_MIA, 1);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 492, 164);
	fdActorWalkTo(ACTOR_GARET, 514, 164);
	fdActorWalkTo(ACTOR_IVAN, 492, 140);
	fdActorWalkToAndWait(ACTOR_MIA, 514, 140);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdActorWaitForMoveById(ACTOR_RISING_PLATFORM);
	fdActorSetPos(ACTOR_RISING_PLATFORM, 0, 0);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	fdScriptSetMessageId(MsgVenusLongLastTime);
	// "At long last, the time has come to light Venus Lighthouse's beacon."
	scVenusLighthouseAerieShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MENARDI);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	scVenusLighthouseAerieFaceActor(ACTOR_MIA, FACING_WEST);
	fdCameraGetActor()->motionFlags = 0;
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(304, 32, 158, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	scVenusLighthouseAerieFaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHEAST);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	sySoundPlay(MUS_CHANCE_MEETING_WITH_SWORN_ENEMIES);
	// "With the second beacon lit, only two will remain."
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorSetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
	// "No. Lighting Jupiter is virtually the same as lighting Mars."
	scVenusLighthouseAerieShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MENARDI);
	scVenusLighthouseAerieFaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHWEST);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	// "That is true. Jupiter is really all that remains."
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	// "That's right. Your dreams are within your grasp now."
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 20);
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(294, CAMERA_KEEP, 194, 1);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_FELIX, 0.8);
	fdActorWalkToAndWait(ACTOR_FELIX, 272, 200);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	fdScriptWait(20);
	// "Why are you still here, Felix?"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	// "I thought I ordered you on ahead to the ship."
	scVenusLighthouseAerieShowMessage(ACTOR_MENARDI);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	// "I've already sent Jenna and the others to the dock at Idejima."
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	// "Then why did you come back?"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
	// "Because I have a different promise to keep."
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	// "A promise, you say?"
	fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 40);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	scVenusLighthouseAerieFaceActor(ACTOR_MENARDI, FACING_WEST);
	// "You must mean Sheba..."
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	// "Yes, Sheba. Why haven't you released her yet?"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 40);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
	// "You promised to free her once you got into Venus Lighthouse."
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
	scVenusLighthouseAerieFaceActor(ACTOR_MENARDI, FACING_SOUTH_SOUTHWEST);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	// "And that's what brought you back here?"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	// "You promised not to involve anyone who wasn't related to this matter."
	fdScriptShowMessageThenWait(ACTOR_FELIX, 0, 20);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 40);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
	// "Sheba is special."
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 40);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	// "Of course..."
	fdScriptShowMessageThenWait(ACTOR_FELIX, 0, 20);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 20);
	fdActorStartRepeatedMotion(ACTOR_FELIX, 2);
	// "Because Sheba is a special Adept?"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	fdActorStartRepeatedMotion(ACTOR_FELIX, 2);
	// "You guys learned something from the first two lighthouses."
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	// "And what would that be?"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	// "In order to enter each lighthouse, you needed an Adept of that clan."
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorStartRepeatedMotion(ACTOR_MENARDI, 1);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
	// "Right now, we have Fire, Earth, and Water Adepts, but no Wind Adept."
	fdScriptShowMessageThenWait(ACTOR_FELIX, 0, 20);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	// "That does seem to be the case."
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 20);
	scVenusLighthouseAerieFaceActor(ACTOR_FELIX, FACING_NORTH_NORTHWEST);
	// "Do we need Sheba to get into the Jupiter Lighthouse?"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	scVenusLighthouseAerieFaceActor(ACTOR_SHEBA, FACING_SOUTH_SOUTHEAST);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdScriptWait(20);
	scVenusLighthouseAerieFaceActor(ACTOR_FELIX, FACING_NORTH_NORTHEAST);
	fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
	// "I'm sorry, Felix, but we cannot even enter without a Wind Adept."
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
	// "That being the case, I guess I understand why you can't set Sheba free."
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
	// "It seems that you, too, have learned something. If that is all, then..."
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
	// "No. I cannot leave Sheba behind!"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(294, CAMERA_KEEP, 180, 1);
	fdActorWalkToAndWait(ACTOR_FELIX, 262, 176);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 40);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 20);
	fdActorStartRepeatedMotion(ACTOR_FELIX, 2);
	// "I'm taking Sheba to the ship!"
	scVenusLighthouseAerieShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	// "What are you talking about, Felix?"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorStartRepeatedMotion(ACTOR_FELIX, 2);
	// "We don't know what will happen when you light this beacon. I won't..."
	scVenusLighthouseAerieShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	// "We told you that we'd protect Sheba. Don't you trust us?"
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 20);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	// "That's not what I'm saying... It's just best to be safe."
	scVenusLighthouseAerieShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	// "Don't tell us what's best, Felix. You're not taking Sheba."
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	// "Why not? What? You don't trust me?"
	scVenusLighthouseAerieShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FELIX);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
	// "How can we, after you just demanded we free Sheba?"
	scVenusLighthouseAerieShowMessage(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT
	                                  | ACTOR_SATUROS);
	fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
	// "If we had trusted you a moment ago, Sheba would be free now."
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 20);
	// "Come with me, Sheba! I shall protect you!"
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 40);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	// "Are you ready to do this, even though it means fighting us?"
	scVenusLighthouseAerieShowMessage(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT
	                                  | ACTOR_SATUROS);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 40);
	fdActorSetAnimation(ACTOR_MENARDI, ANIM_NOD);
	// "...I believe he is! He would not have defied us if he lacked the will..."
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
	// "If it means protecting Sheba from harm, then I must."
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_FELIX, 0, 20);
	// "You have seen what we can do. You know you can't stop us."
	scVenusLighthouseAerieShowMessage(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT
	                                  | ACTOR_SATUROS);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorSetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
	// "How can you protect Sheba, when you can't even protect yourself?"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	// "This rebellious attitude is exactly why we've never let you fight."
	scVenusLighthouseAerieShowMessage(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT
	                                  | ACTOR_SATUROS);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	// "Haven't you been given enough reason to fear us in these travels?"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_SATUROS, 2);
	// "You're no fighter, Felix. How can you hope to stand against us?"
	scVenusLighthouseAerieShowMessage(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT
	                                  | ACTOR_SATUROS);
	fdActorSetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
	// "You've questioned us for the last time! Let's see what you can do!"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraMoveTo(504, -24, 168, 0);
	sySynch(1);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	// "It looks like they're having a falling out over Sheba."
	scVenusLighthouseAerieShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GARET);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	// "There's something more to it, though..."
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	// "It seems Saturos and Menardi no longer trust Felix."
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0, 40);
	fdActorJump(ACTOR_GARET, 2, 20);
	// "Looks like the perfect opportunity has just fallen into our laps!"
	scVenusLighthouseAerieShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GARET);
	// "Now's our only chance to prevent them from lighting the beacon!"
	fdScriptOpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1)
	{
		FieldStateWork->msg += 1;
	}
	fdScriptWait(20);
	// Yes: "There's not a moment to spare! Let's move!" No: "Don't chicken out now! If we hesitate,
	// they'll light the beacon!"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdActorSetScript(ACTOR_GARET, VenusLighthouseAerieJoinLeaderScript);
	fdActorSetScript(ACTOR_IVAN, VenusLighthouseAerieJoinLeaderScript);
	fdActorRunScriptAndWait(ACTOR_MIA, VenusLighthouseAerieJoinLeaderScript);
	fdScriptWait(20);
	fdScriptEnd();
}

// Shows or hides one companion, setting its movement speed when shown.
static void scVenusLighthouseAerieShowPartyMember(Sint32 actorId, Sint32 shown)
{
	if (shown != 0)
	{
		fdActorSetSpriteColorById(actorId, SPRITE_COLOR_NORMAL);
		fdActorSetSpriteFlags(fdActorGet(actorId), SPRITE_FLAG_SHADOW);
		fdActorSetSpeed(actorId, 0.8);
	}
	else
	{
		fdActorSetSpriteColorById(actorId, SPRITE_COLOR_HIDDEN);
		fdActorSetSpriteFlags(fdActorGet(actorId), 0);
	}
}

// Isaac walks up to the beacon in the middle of the aerie (metatiles (20, 14) and (21, 14)) and the
// party confronts Saturos and Menardi. Saturos offers a deal for the Shaman's Rod, the party hands
// it over for Sheba's sake, Saturos keeps her all the same, and the battle begins.
static void scVenusLighthouseAerieConfrontSaturosAndMenardi(void)
{
	Sint32 rodHolder;
	ACTOR *actor;
	Sint32 answeredYes;
	Uint32 frame;
	Sint32 receiptMsg;

	fdScriptStart();
	fdCameraGetActor()->motionFlags = 0;
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(332, 32, 180, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 340, 184);
	scVenusLighthouseAerieFaceActor(ACTOR_PARTY_LEADER, FACING_WEST);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
	fdScriptSetMessageId(MsgVenusIsaac);
	// "Isaac..."
	scVenusLighthouseAerieShowMessage(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_FELIX);
	fdCameraGetActor()->motionFlags = 0;
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(304, 32, 180, 1);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorSetSpeed(ACTOR_MIA, 1.4);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetPosFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
	}
	fdActorWalkTo(ACTOR_GARET, 328, 168);
	fdActorWalkTo(ACTOR_IVAN, 340, 196);
	fdActorWalkToAndWait(ACTOR_MIA, 326, 204);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	scVenusLighthouseAerieFaceActor(ACTOR_MIA, FACING_WEST);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 40);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
	// "So! You've come to stop us?"
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	scVenusLighthouseAerieFaceActor(ACTOR_MENARDI, FACING_WEST);
	// "Look what your defiance has brought us, Felix!"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	// "I don't need you or anyone to rescue me, Isaac!"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorStartRepeatedMotion(ACTOR_FELIX, 2);
	// "I was just about to settle things with Saturos and Menardi... You can..."
	fdScriptShowMessageThenWait(ACTOR_FELIX, 0, 20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 40);
	// "And then we'll show you the true power of this beacon's light!"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
	scVenusLighthouseAerieFaceActor(ACTOR_FELIX, FACING_EAST);
	// "But...Sheba has nothing to do with this! So set Sheba free!"
	fdScriptShowMessageThenWait(ACTOR_FELIX, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	scVenusLighthouseAerieFaceActor(ACTOR_SATUROS, FACING_WEST);
	// "Silence! You'd best tread lightly, Felix, or both you and Sheba will..."
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorSetAttachedEffect(ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	// "If anything happens to Sheba, we will have failed Faran..."
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	// "We won't permit you to lay a hand on Sheba or Felix!"
	scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
	scVenusLighthouseAerieFaceActor(ACTOR_MENARDI, FACING_EAST);
	// "Oh ho! So you're worried about Sheba, too?"
	scVenusLighthouseAerieShowMessage(ACTOR_MENARDI);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_EAST, 20);
	// "Don't worry. We won't hurt Sheba...if you meet our conditions."
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	scVenusLighthouseAerieFaceActor(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	// "What are you doing, Saturos?"
	scVenusLighthouseAerieShowMessage(ACTOR_MENARDI);
	scVenusLighthouseAerieFaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHWEST);
	// "Leave this to me!"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 20);
	// "Sheba! Do you remember the rod connected to Hesperia?"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 20);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	// "The Rod of Hesperia?"
	scVenusLighthouseAerieShowMessage(ACTOR_SHEBA);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	// "The rod required to reach the Jupiter Lighthouse!"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	// "Ye-Yes..."
	scVenusLighthouseAerieShowMessage(ACTOR_SHEBA);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
	// "Those who pursue us bear a rod... Is it the one?"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorSetSpeed(ACTOR_SHEBA, 0.8);
	fdActorWalkToAndWait(ACTOR_SHEBA, 260, 186);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 0);
	fdActorWalkToAndWait(ACTOR_SHEBA, 276, 192);
	fdActorGet(ACTOR_MENARDI)->facing = FACING_SOUTH_SOUTHWEST;
	sySynch(1);
	fdActorStartRepeatedMotion(ACTOR_MENARDI, 2);
	// "Sheba! Stay where you are! You can do it from there!"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorJump(ACTOR_SHEBA, 2, 20);
	fdActorSetSpeed(ACTOR_SHEBA, 2.4);
	fdActorWalkToAndWait(ACTOR_SHEBA, 260, 186);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorWalkToAndWait(ACTOR_SHEBA, 248, 172);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 20);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdScriptWait(40);
	scStartScriptPsynergy();
	sySynch(1);
	for (frame = 0; frame <= 39; frame++)
	{
		scVenusLighthouseAerieFlashActorPalette(fdActorGet(ACTOR_SHEBA));
		sySynch(1);
	}
	sySetTickEvent(scVenusLighthouseAerieUpdateShebaPalette, EVENT_PASS_TICK);
	fdScriptWait(80);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	scVenusLighthouseAerieFaceActor(ACTOR_FELIX, FACING_WEST);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	// "How about it, Sheba?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 20);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	fdActorStartRepeatedMotion(ACTOR_MENARDI, 2);
	// "Well, Sheba?"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	syCloseEvent(scVenusLighthouseAerieUpdateShebaPalette);
	sySynch(1);
	fdActorSetSpriteColorById(ACTOR_SHEBA, SPRITE_COLOR_NORMAL);
	sySynch(10);
	scEndScriptPsynergy();
	fdActorJump(ACTOR_SHEBA, 2, 40);
	// "...Perhaps..."
	scVenusLighthouseAerieShowMessage(ACTOR_SHEBA);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	// ""Perhaps..." Perhaps what!?"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdActorGet(ACTOR_SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_SHEBA, 250, 176);
	fdScriptWait(1);
	fdActorGet(ACTOR_SHEBA)->steeringFlags |= ACTOR_FACES_MOTION;
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 20);
	// "Don't yell at her!"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
	// "We're not yelling! We just want a straight answer."
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdScriptWait(40);
	fdActorSetSpeed(ACTOR_SHEBA, 0.6);
	fdActorGet(ACTOR_SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_SHEBA, 248, 172);
	fdScriptWait(1);
	fdActorGet(ACTOR_SHEBA)->steeringFlags |= ACTOR_FACES_MOTION;
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
	// "So, Sheba?"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	// "Their minds are veiled. I cannot see clearly...but I think...it's the..."
	scVenusLighthouseAerieShowMessage(ACTOR_SHEBA);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 20);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorSetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
	// "It's a waste to make her try again."
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	// "So, what do we do?"
	scVenusLighthouseAerieShowMessage(ACTOR_MENARDI);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 100);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	fdScriptWait(20);
	scVenusLighthouseAerieFaceActor(ACTOR_SATUROS, FACING_EAST);
	scVenusLighthouseAerieFaceActor(ACTOR_MENARDI, FACING_EAST);
	// "Do you know the terms we will require before we agree not to harm Sheba?"
	fdScriptOpenMessage(ACTOR_SATUROS, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
	{
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
		answeredYes = 1;
	}
	else
	{
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
		FieldStateWork->msg += 1;
		answeredYes          = 0;
	}
	// Yes: "You're smarter than I thought if you've already figured it out." No: "I want the rod
	// that will get us into Jupiter Lighthouse."
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	if (answeredYes != 0)
	{
		FieldStateWork->msg += 1;
	}
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 20);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	// "If you give us the Shaman's Rod, Sheba will not be harmed. Agreed?"
	fdScriptOpenMessage(ACTOR_SATUROS, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
	{
		fdScriptWait(20);
		fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
		answeredYes = 1;
	}
	else
	{
		fdScriptWait(20);
		fdActorSetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
		FieldStateWork->msg += 1;
		answeredYes          = 0;
	}
	// Yes: "The Shaman's Rod? If it means saving Sheba, I will give it to you." No: "He wants the
	// Shaman's Rod. If it will save Sheba, I'll give it up."
	scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
	if (answeredYes != 0)
	{
		FieldStateWork->msg += 1;
	}
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	scVenusLighthouseAerieFaceActor(ACTOR_GARET, FACING_SOUTH);
	// "What? But Hammet gave that to you, Ivan!"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	// "Don't you care what happens to Sheba?"
	scVenusLighthouseAerieShowMessage(ACTOR_MENARDI);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	scVenusLighthouseAerieFaceActor(ACTOR_MIA, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
	// "Then you will give us the rod!"
	fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	scVenusLighthouseAerieFaceActor(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimation(ACTOR_MENARDI, ANIM_NOD);
	// "Well done, Saturos! We finally have the Shaman's Rod!"
	scVenusLighthouseAerieShowMessage(ACTOR_MENARDI);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
	fdScriptWait(40);
	scVenusLighthouseAerieFaceActor(ACTOR_SATUROS, FACING_WEST);
	// "Go get it, Felix!"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
	fdScriptWait(20);
	// "Why me?"
	fdScriptShowMessageThenWait(ACTOR_FELIX, 0, 40);
	// "To ensure your loyalty to us!"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
	scVenusLighthouseAerieFaceActor(ACTOR_MENARDI, FACING_WEST);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	// "Go on, Felix!"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_FELIX, 0.8);
	fdActorWalkToAndWait(ACTOR_FELIX, 288, 192);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_EAST, 0);
	fdActorWalkToAndWait(ACTOR_FELIX, 310, 192);
	fdActorWalkToAndWait(ACTOR_FELIX, 328, 186);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);

	// Felix takes the Shaman's Rod from Isaac and carries it back to Saturos.
	receiptMsg = MsgVenusIsaacHandedOverShamansRod;
	// "Isaac handed over the Shaman's Rod."
	mnFieldMessageShow(receiptMsg, 1);
	fdActorWalkToAndWait(ACTOR_FELIX, 310, 192);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
	fdActorWalkToAndWait(ACTOR_FELIX, 288, 192);
	fdActorWalkToAndWait(ACTOR_FELIX, 262, 176);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 40);
	fdScriptSetMessageId(receiptMsg + 1);
	// "What should I do with the rod?"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
	// "Keep it safe!"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	scVenusLighthouseAerieFaceActor(ACTOR_SATUROS, FACING_EAST);
	scVenusLighthouseAerieFaceActor(ACTOR_FELIX, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetSpeed(ACTOR_SHEBA, 0.8);
	fdActorWalkToAndWait(ACTOR_SHEBA, 260, 186);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 0);
	fdActorWalkToAndWait(ACTOR_SHEBA, 276, 192);
	sySoundPlay(MUS_SILENCE);
	fdActorGet(ACTOR_MENARDI)->facing = FACING_SOUTH_SOUTHWEST;
	sySynch(1);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	// "Where do you think you're going, Sheba?"
	scVenusLighthouseAerieShowMessage(ACTOR_MENARDI);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH_NORTHEAST, 20);
	fdActorJump(ACTOR_MIA, 2, 20);
	scVenusLighthouseAerieFaceActor(ACTOR_MIA, FACING_NORTHWEST);
	// "We gave you the rod. Now, release Sheba!"
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 80);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_EAST, 40);
	sySoundPlay(MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE);
	// "Whatever do you mean?"
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH_NORTHWEST, 20);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	// "You said you'd free Sheba if we gave you the rod."
	scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
	scVenusLighthouseAerieFaceActor(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
	// "Those were your terms."
	scVenusLighthouseAerieShowMessage(ACTOR_MENARDI);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	scVenusLighthouseAerieFaceActor(ACTOR_SHEBA, FACING_NORTH_NORTHEAST);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	// "We gave you the rod, so release Sheba!"
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHEAST, 20);
	// "You seem to have misunderstood. I said we wouldn't harm Sheba... but..."
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	scVenusLighthouseAerieFaceActor(ACTOR_FELIX, FACING_EAST);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	// "You're altering the bargain?"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	scVenusLighthouseAerieFaceActor(ACTOR_MENARDI, FACING_EAST);
	fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
	// "Ha ha! We can't help it if you misinterpreted Saturos's terms!"
	scVenusLighthouseAerieShowMessage(ACTOR_MENARDI);
	scVenusLighthouseAerieFaceActor(ACTOR_SATUROS, FACING_EAST);
	fdActorSetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
	// "Besides, if we were to release Sheba, she'd be in quite a fix."
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH_NORTHEAST, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	// "What do you mean?"
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	scVenusLighthouseAerieFaceActor(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
	// "Sheba can't get back to Lalivero alone."
	scVenusLighthouseAerieShowMessage(ACTOR_MENARDI);
	fdActorSetAttachedEffect(ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	// "What do you mean? We're going to take Sheba... Oh no..."
	fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
	scVenusLighthouseAerieFaceActor(ACTOR_MENARDI, FACING_EAST);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
	// "That's right. You won't be going back. This is it for you!"
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	scVenusLighthouseAerieFaceActor(ACTOR_MIA, FACING_NORTHWEST);
	// "Excuse me?"
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	// "Enough! You're the ones who aren't going anywhere!"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 40);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	fdActorStartRepeatedMotion(ACTOR_SATUROS, 2);
	// "Bring it on, whelp! You won't live long enough to regret your stupidity!"
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	// "Prepare to experience first-hand the horror of a master Fire Adept!"
	scVenusLighthouseAerieShowMessage(ACTOR_MENARDI);
	rodHolder = gmPartyInventoryFindMember(ITEM_SHAMANS_ROD);
	gmFlagSet(rodHolder + FLAG_SHAMANS_ROD_CARRIED_BY);
	gmPartyInventoryRemove(ITEM_SHAMANS_ROD);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_GARET)->steeringFlags        &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_IVAN)->steeringFlags         &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_MIA)->steeringFlags          &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_MENARDI)->steeringFlags      &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_SATUROS)->steeringFlags      &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_FELIX)->steeringFlags        &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_SHEBA)->steeringFlags        &= ~ACTOR_FACES_MOTION;
	fdActorSetScript(ACTOR_PARTY_LEADER, VenusLighthouseAerieStepBackWestScript);
	fdActorSetScript(ACTOR_GARET, VenusLighthouseAerieStepBackWestScript);
	fdActorSetScript(ACTOR_IVAN, VenusLighthouseAerieStepBackWestScript);
	fdActorSetScript(ACTOR_MIA, VenusLighthouseAerieStepBackWestScript);
	fdActorSetScript(ACTOR_MENARDI, VenusLighthouseAerieStepBackEastScript);
	fdActorSetScript(ACTOR_SATUROS, VenusLighthouseAerieStepBackEastScript);
	fdActorSetScript(ACTOR_FELIX, VenusLighthouseAerieStepBackWestScript);
	fdActorRunScriptAndWait(ACTOR_SHEBA, VenusLighthouseAerieStepBackWestScript);
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	fdScriptSetAfterBattleDestination(SCENE_VENUS_LIGHTHOUSE_AERIE,
	                                  VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE);
	fdScriptSetDefeatDestination(SCENE_VENUS_LIGHTHOUSE_AERIE, VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 1);
	gmFlagSet(FLAG_AERIE_FIRST_BATTLE_BEGUN);
}

// The ticks between the Psynergy rings round a flashing actor.
enum
{
	PSYNERGY_RINGS_INTERVAL = 15
};

// Flashes the actor white two ticks in four, with Psynergy rings every PSYNERGY_RINGS_INTERVAL
// ticks.
static void scVenusLighthouseAerieFlashActorPalette(ACTOR *actor)
{
	if (TickCount & 2)
	{
		fdActorSetColorMode(actor, SPRITE_COLOR_WHITE);
	}
	else
	{
		fdActorSetColorMode(actor, SPRITE_COLOR_NORMAL);
	}
	if (syMathRemainderUnsigned(TickCount, PSYNERGY_RINGS_INTERVAL) == 0)
	{
		scSpawnPsynergyRings(actor);
	}
}

// Cycles the actor through palettes 0 to 5, with Psynergy rings every PSYNERGY_RINGS_INTERVAL
// ticks.
static void scVenusLighthouseAerieCycleActorPalette(ACTOR *actor)
{
	if (TickCount & 1)
	{
		fdActorSetColorMode(actor, syMathRemainderUnsigned(TickCount >> 1, 6));
	}
	if (syMathRemainderUnsigned(TickCount, PSYNERGY_RINGS_INTERVAL) == 0)
	{
		scSpawnPsynergyRings(actor);
	}
}

// The sound the rings of light play here (psynergy_rings.c).
#define PSYNERGY_RINGS_SOUND SE_PSYNERGY_SPARKLE
#include "shared/psynergy_rings.c"

#include "shared/script_psynergy.c"

// Sheba glows through changing colours, with rings of Psynergy, each tick while she reads the
// party's minds.
static void scVenusLighthouseAerieUpdateShebaPalette(void)
{
	scVenusLighthouseAerieCycleActorPalette(fdActorGet(ACTOR_SHEBA));
}

// A burst of particles flies out in BURST_DIRECTIONS directions round a turn, the first one twice.
enum
{
	BURST_DIRECTIONS = 16
};

// Builds the horizontal velocity for an elliptical radial particle burst.
static inline void scVenusLighthouseAerieAimBurst(Sint32 velocity[3], Sint32 angle)
{
	Sint32 sine;
	Sint32 cosine;

	velocity[0]  = syCos(angle);
	velocity[1]  = 0;
	sine         = sySin(angle);
	cosine       = velocity[0];
	cosine      += cosine * 2;
	sine        *= 2;
	velocity[2]  = sine;
	velocity[0]  = cosine;
}

// Gives Felix, Sheba and the party the same reaction, then waits after Mia.
static inline void scVenusLighthouseAerieShowGroupEmote(Sint32 emote, Sint32 waitTicks)
{
	fdActorShowEmote(ACTOR_FELIX, emote, 0);
	fdActorShowEmote(ACTOR_SHEBA, emote, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, emote, 0);
	fdActorShowEmote(ACTOR_GARET, emote, 0);
	fdActorShowEmote(ACTOR_IVAN, emote, 0);
	fdActorShowEmote(ACTOR_MIA, emote, waitTicks);
}

// After the first battle, Saturos and Menardi rise again. Saturos throws the Venus Star into the
// beacon, Felix and Sheba leave, and the two fuse into the Fusion Dragon for the second battle
// (VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE).
static void scVenusLighthouseAerieCombineSaturosAndMenardi(void)
{
	EFFECT_OPTIONS menardiBurst;
	Sint32         velocity[3];
	EFFECT_OPTIONS saturosBurst;
	ACTOR         *actor;
	ACTOR         *menardi;
	ACTOR         *saturos;
	ACTOR         *star = NULL;
	ACTOR         *starShadow;
	SPR           *sprite;
	ICON_TRANSFER *transfer;
	Uint32         answeredYes;

	fdScriptStart();
	fdMapGridCopyMetatileAttributes(17, 10, 4, 2, 17, 8);
	fdActorGet(ACTOR_GARET)->facing = FACING_WEST;
	fdActorSetPos(ACTOR_GARET, 328, 168);
	fdActorGet(ACTOR_IVAN)->facing = FACING_WEST;
	fdActorSetPos(ACTOR_IVAN, 340, 196);
	fdActorGet(ACTOR_MIA)->facing = FACING_WEST;
	fdActorSetPos(ACTOR_MIA, 326, 204);
	fdActorGet(ACTOR_SHEBA)->facing = FACING_SOUTH_SOUTHEAST;
	fdActorSetPos(ACTOR_SHEBA, 268, 154);
	fdActorGet(ACTOR_FELIX)->facing = FACING_SOUTH_SOUTHEAST;
	fdActorSetPos(ACTOR_FELIX, 268, 164);
	actor         = fdActorGet(ACTOR_SATUROS);
	actor->timer  = 10;
	actor->facing = FACING_NORTH_NORTHEAST;
	fdActorSetPos(ACTOR_SATUROS, 294, 212);
	fdActorSetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_LIE);
	fdActorSetScript(ACTOR_SATUROS, VenusLighthouseAerieBreatheHeavilyScript);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_SATUROS), 0);
	actor         = fdActorGet(ACTOR_MENARDI);
	actor->timer  = 10;
	actor->facing = FACING_EAST;
	fdActorSetPos(ACTOR_MENARDI, 286, 192);
	fdActorSetAnimation(ACTOR_MENARDI, MENARDI_ANIMATION_LIE);
	fdActorSetScript(ACTOR_MENARDI, VenusLighthouseAerieBreatheHeavilyScript);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_MENARDI), 0);
	fdCameraGetActor()->motionFlags = 0;
	fdCameraMoveTo(304, 32, 180, 0);
	sySynch(1);
	fdMapGridRedraw();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(80);

	fdActorJump(ACTOR_GARET, 2, 20);
	scVenusLighthouseAerieFaceActor(ACTOR_GARET, FACING_SOUTHEAST);
	fdScriptSetMessageId(MsgVenusBeatEm);
	// "We did it! We beat 'em!"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET);
	scVenusLighthouseAerieFaceActor(ACTOR_MIA, FACING_NORTHWEST);
	// "All right, Sheba... Let's go!"
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	scVenusLighthouseAerieFaceActor(ACTOR_IVAN, FACING_NORTHWEST);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
	fdActorSetSpeed(ACTOR_FELIX, 0.8);
	fdActorGet(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_FELIX, 280, 164);
	fdScriptWait(1);
	fdActorGet(ACTOR_FELIX)->steeringFlags |= ACTOR_FACES_MOTION;
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	// "Felix! What are you doing?"
	scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
	// "I cannot give up Sheba..."
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	// "What!? But we just saved you!"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
	scVenusLighthouseAerieFaceActor(ACTOR_FELIX, FACING_EAST);
	// "I never asked you to help us..."
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	// "Felix, you're not one of them. You're free to go now..."
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
	// "So why are you doing this?"
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	scVenusLighthouseAerieFaceActor(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST);
	// "The four beacons must still be lit..."
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorJump(ACTOR_IVAN, 2, 20);
	// "You're going to light the beacons!? That's ridiculous!"
	scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	// "I felt you'd say that. But I'm going to do it anyway!"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	scVenusLighthouseAerieFaceActor(ACTOR_GARET, FACING_SOUTHEAST);
	// "So, wait... If he means to light the beacons, then he's our enemy, right?"
	fdScriptOpenMessage(ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
	{
		answeredYes = TRUE;
	}
	else
	{
		FieldStateWork->msg++;
		answeredYes = FALSE;
	}
	sySynch(20);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	scVenusLighthouseAerieFaceActor(ACTOR_MIA, FACING_NORTHWEST);
	scVenusLighthouseAerieFaceActor(ACTOR_FELIX, FACING_EAST);
	// Yes: "Do what you will, Isaac, but if you try to stop me, I'll fight you." No: "Friend or
	// enemy, you won't stop me from taking Sheba with me!"
	fdScriptShowMessage(ACTOR_FELIX, 0);
	sySoundPlay(SOUND_FADE_OUT_BGM);
	fdScriptWait(40);
	if (answeredYes)
	{
		FieldStateWork->msg++;
	}

	// Saturos and Menardi recover from their defeat before using the Venus Star.
	fdActorStop(ACTOR_SATUROS);
	fdActorStop(ACTOR_MENARDI);
	sySynch(1);
	actor         = fdActorGet(ACTOR_SATUROS);
	actor->scaleX = toFIXED(1);
	actor->scaleY = toFIXED(1);
	actor         = fdActorGet(ACTOR_MENARDI);
	actor->scaleX = toFIXED(1);
	actor->scaleY = toFIXED(1);
	sySynch(1);
	// "I hope you don't think you've finished us off."
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 40);
	fdActorSetAnimation(ACTOR_SATUROS, ANIM_STAND);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_SATUROS), SPRITE_FLAG_SHADOW);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_SATUROS, 0.2);
	fdActorWalkToAndWait(ACTOR_SATUROS, 300, 206);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
	fdScriptWait(20);
	fdActorGet(ACTOR_SATUROS)->scaleX = -toFIXED(1);
	sySoundPlay(SE_RATTLE);
	fdActorSetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_BREATHE);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_MENARDI, ANIM_STAND);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_MENARDI), SPRITE_FLAG_SHADOW);
	fdActorJump(ACTOR_MENARDI, 4, 40);
	fdActorSetSpeed(ACTOR_MENARDI, 0.2);
	fdActorGet(ACTOR_MENARDI)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_MENARDI, 296, 186);
	fdActorSetSpeed(ACTOR_MENARDI, 0.1);
	fdActorWalkToAndWait(ACTOR_MENARDI, 292, 186);
	fdActorGet(ACTOR_MENARDI)->steeringFlags |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_MENARDI)->scaleX         = -toFIXED(1);
	sySoundPlay(SE_RATTLE);
	fdActorSetAnimation(ACTOR_MENARDI, MENARDI_ANIMATION_STAND_WINDSWEPT);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 2);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	fdScriptWait(80);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
	// "You may not be finished, but you can barely stand."
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 2);
	fdScriptWait(40);
	// "Right now, yes... But we'll be back on our feet... as soon as we do THIS!"
	scVenusLighthouseAerieShowMessage(ACTOR_MENARDI);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 20);

	actor         = fdActorGet(ACTOR_SATUROS);
	actor->facing = FACING_SOUTH_SOUTHEAST;
	actor->scaleX = toFIXED(1);
	fdActorSetAnimation(ACTOR_SATUROS, ANIM_STAND);
	fdScriptWait(10);
	scVenusLighthouseAerieFaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHEAST);
	fdActorJump(ACTOR_SATUROS, 6, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	star = (ACTOR *)syFieldActorCreate(
		SPRITE_BLANK, actor->x.fixed, actor->y.fixed + toFIXED(8), actor->z.fixed);
	if (star != NULL)
	{
		sprite                   = star->sprite;
		sprite->count            = 0;
		sprite->image.fullColor  = 0;
		sprite->image.palette    = 0;
		sprite->image.priority   = 0;
		star->priorityFlags     &= ~ACTOR_PRIORITY_AUTOMATIC;
		star->motionFlags        = 0;
		star->retainSpriteCache  = 1;
		star->speedLimit         = toFIXED(8) / 5;
		star->acceleration       = toFIXED(4) / 5;
		transfer = syHeapSlotAllocate(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		// Display the Venus Star icon before sending it toward the beacon.
		gmItemLoadIcon(ITEM_VENUS_STAR);
		mnVRAMLoad(sprite->slot, ICON_TILE_BYTES, transfer->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	}
	fdActorSetSpritePriority(ACTOR_VENUS_STAR_SHADOW, 1);
	starShadow               = fdActorGet(ACTOR_VENUS_STAR_SHADOW);
	starShadow->x.fixed      = actor->x.fixed;
	starShadow->y.fixed      = toFIXED(32);
	starShadow->z.fixed      = actor->z.fixed;
	starShadow->motionFlags  = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	starShadow->speedLimit   = toFIXED(8) / 5;
	starShadow->acceleration = toFIXED(4) / 5;
	starShadow->scaleX       = toFIXED(3) / 4;
	starShadow->scaleY       = toFIXED(3) / 4;
	if (star != NULL)
	{
		star->motionFlags                  = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		((FIELD_EFFECT *)star)->velocity.y = toFIXED(3) / 5;
		((FIELD_EFFECT *)star)->velocity.x = toFIXED(4) / 5;
		star->velocityY                    = toFIXED(8);
		syFieldActorSetMoveTarget(star, 308, 32, 164);
	}
	fdActorMoveToAndWait(ACTOR_VENUS_STAR_SHADOW, 308, 164);
	fdActorSetPos(ACTOR_VENUS_STAR_SHADOW, 0, 0);
	fdActorSetSpritePriority(ACTOR_FELIX, 0);
	if (star != NULL)
	{
		fdScriptPlaySound(SE_STAR_BOUNCE);
		fdActorSetSpriteFlags(star, 0);
		star->velocityY = toFIXED(4);
		syFieldActorSetMoveTarget(star, 314, 32, 137);
		fdActorWaitForMove(star);
		fdScriptPlaySound(SE_STAR_BOUNCE);
		star->sprite->image.priority = 1;
		star->velocityY              = toFIXED(6);
		syFieldActorSetMoveTarget(star, 285, 32, 146);
		fdActorWaitForMove(star);
		sySoundPlay(SE_STAR_BOUNCE);
		star->velocityY = toFIXED(5);
		syFieldActorSetMoveTarget(star, 300, 32, 154);
		fdActorWaitForMove(star);
		sySynch(6);
		star->x.fixed = 0;
		star->y.fixed = 0;
		star->z.fixed = 0;
		scVenusLighthouseAerieStopActorMotion(star);
	}
	scVenusLighthouseAerieShowGroupEmote(EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorSetSpritePriority(ACTOR_FELIX, 1);
	fdActorGet(ACTOR_FELIX)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	// "Oh, no! He threw the Elemental Star into the lighthouse!"
	scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	// "How could this happen... We couldn't keep them from lighting the beacon!"
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 40);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	fdScriptWait(20);
	// "That's not all..."
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH_SOUTHEAST, 0);
	scVenusLighthouseAerieFaceActor(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST);
	scVenusLighthouseAerieShowGroupEmote(EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	// "The energy of the beacon will restore our power."
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 40);
	// "How can it restore their power? Venus is the earth elemental, right?"
	fdScriptOpenMessage(ACTOR_GARET, 0);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
	{
		fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
		answeredYes = TRUE;
	}
	else
	{
		FieldStateWork->msg++;
		fdActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
		answeredYes = FALSE;
	}
	// Yes: "Then why would Venus give power to Fire Adepts?" No: "Have you forgotten everything
	// we've learned? Venus is earth. It..."
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	if (answeredYes)
	{
		FieldStateWork->msg++;
	}
	fdScriptWait(20);

	fdActorSetSpriteFlags(fdActorGet(ACTOR_BEAM_BOTTOM), 0);
	fdActorSetSpriteColorById(ACTOR_BEAM_BOTTOM, SPRITE_COLOR_WHITE);
	actor              = fdActorGet(ACTOR_BEAM_BOTTOM);
	actor->scaleY      = -toFIXED(1);
	actor->scaleX      = toFIXED(1) / 10;
	actor->motionFlags = 0;
	actor->y.fixed     = toFIXED(64);
	actor->x.fixed     = toFIXED(304);
	actor->z.fixed     = toFIXED(158);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BEAM_LOWER), 0);
	fdActorSetSpriteColorById(ACTOR_BEAM_LOWER, SPRITE_COLOR_WHITE);
	actor              = fdActorGet(ACTOR_BEAM_LOWER);
	actor->scaleY      = -toFIXED(1);
	actor->scaleX      = toFIXED(1) / 10;
	actor->motionFlags = 0;
	actor->y.fixed     = toFIXED(96);
	actor->x.fixed     = toFIXED(304);
	actor->z.fixed     = toFIXED(158);
	scVenusLighthouseAerieShowGroupEmote(EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorSetScript(ACTOR_BEAM_BOTTOM, VenusLighthouseAerieBeamOpenScript);
	fdActorSetScript(ACTOR_BEAM_LOWER, VenusLighthouseAerieBeamOpenScript);
	sySoundPlay(SE_CRASH);
	fdMapGridSetShake(6, 6, 1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
	fdPaletteFadeInterpolate(16);
	sySynch(20);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeInterpolate(24);
	sySynch(60);
	sySetTickEvent(scVenusLighthouseAerieSpawnUpperOrbitParticles, EVENT_PASS_TICK);
	sySoundPlay(SE_RUMBLE);
	fdMapGridSetShake(4, 4, 1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
	fdPaletteFadeInterpolate(120);
	fdActorSetScript(ACTOR_BEAM_BOTTOM, VenusLighthouseAerieBeamNarrowScript);
	fdActorSetScript(ACTOR_BEAM_LOWER, VenusLighthouseAerieBeamNarrowScript);
	sySynch(120);
	fdMapGridSetShake(3, 3, 1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12), PALETTES_ALL);
	fdPaletteFadeInterpolate(120);
	sySynch(120);
	sySoundPlay(MUS_MOUNT_ALEPH_ERUPTS);
	fdMapGridSetShake(2, 2, 1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeInterpolate(120);
	sySynch(120);
	fdMapGridSetShake(1, 1, 1);

	fdActorSetAnimation(ACTOR_MENARDI, ANIM_STAND);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 0);
	fdActorGet(ACTOR_MENARDI)->scaleX = toFIXED(1);
	fdActorJump(ACTOR_MENARDI, 4, 40);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	// "Fire and earth share a symbiotic relationship."
	fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH_SOUTHEAST, 20);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	// "This light will restore our Psynergy!"
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	scVenusLighthouseAerieFaceActor(ACTOR_MIA, FACING_WEST);
	// "We'll fight you as many times as it takes. We won't lose."
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	scVenusLighthouseAerieFaceActor(ACTOR_SATUROS, FACING_EAST);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
	// "You may have beaten the two of us..."
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	fdActorStartRepeatedMotion(ACTOR_MENARDI, 2);
	// "But now, you face the true might of Fire Clan of the north!"
	scVenusLighthouseAerieShowMessage(ACTOR_MENARDI);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	scVenusLighthouseAerieFaceActor(ACTOR_GARET, FACING_SOUTHWEST);
	// "Fire Clan of the north?"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	scVenusLighthouseAerieFaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHEAST);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
	// "Bear witness to the true strength of the northern wilds!"
	fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	// "Oh, give it up! We beat you once, we'll do it again!"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHWEST, 20);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	// "Go, Felix! Take Sheba and go!"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 1);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	scVenusLighthouseAerieFaceActor(ACTOR_MIA, FACING_NORTHWEST);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	// "I can...go?"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	// "We may not have the strength to protect Sheba after this."
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 20);
	scVenusLighthouseAerieFaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHWEST);
	// "You must take Sheba to the ship without fail!"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHWEST, 60);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 40);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	// "What are you waiting for? Go! Now!"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 40);
	// "No. I cannot leave you to fight them alone."
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHEAST, 40);
	scVenusLighthouseAerieFaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHWEST);
	// "Are you saying you don't want to lose to Isaac?"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 40);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 40);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHWEST, 20);
	fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	// "I wonder if that's true..."
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	scVenusLighthouseAerieFaceActor(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	// "What was that?"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 20);
	fdActorSetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
	// "Wouldn't you be just a little sad to see your childhood friend perish?"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 80);
	// "What are you saying?"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
	// "I'm saying you're here to protect this brat from your past."
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	fdActorStartRepeatedMotion(ACTOR_FELIX, 2);
	// "Do you really think that I would help them?"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorSetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
	// "Let's just say that I wouldn't put it past you."
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
	// "And you, Saturos? Do you agree?"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	fdActorSetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
	// "I wouldn't say that... But it would be best not to raise our suspicion."
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 0, 40);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
	fdScriptWait(20);
	// "Now, who's telling whom what's best? Fine! I'll take Sheba for you!"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST, 20);
	fdActorSetAnimation(ACTOR_MENARDI, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHWEST, 20);
	// "Come on, Sheba!"
	scVenusLighthouseAerieShowMessage(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX);
	fdActorSetSpeed(ACTOR_SHEBA, 1);
	fdActorSetSpeed(ACTOR_FELIX, 1);
	fdActorSetScript(ACTOR_FELIX, VenusLighthouseAerieWalkOutScript);
	fdScriptWait(20);
	fdActorSetScript(ACTOR_SHEBA, VenusLighthouseAerieWalkOutScript);
	scVenusLighthouseAerieFaceActor(ACTOR_GARET, FACING_SOUTHWEST);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	// "Wait, Felix! I won't let you go!"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	sySoundPlay(SOUND_FADE_OUT_BGM);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(13, 8, 9), PALETTES_BG);
	fdPaletteFadeInterpolate(40);
	scVenusLighthouseAerieFaceActor(ACTOR_SATUROS, FACING_NORTH_NORTHEAST);
	// "Heh heh... Felix is the least of your worries right now."
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	scVenusLighthouseAerieFlashScreen();
	fdActorSetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_WHITE);
	fdActorSetSpriteColorById(ACTOR_MENARDI, SPRITE_COLOR_WHITE);
	fdScriptWait(20);
	fdActorSetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_BLINK);
	fdActorSetSpriteColorById(ACTOR_MENARDI, SPRITE_COLOR_BLINK);
	fdScriptWait(20);
	scVenusLighthouseAerieFlashScreen();
	fdActorJump(ACTOR_MIA, 2, 20);
	// "Oh my-they're glowing!"
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	scVenusLighthouseAerieFaceActor(ACTOR_MENARDI, FACING_EAST);
	// "It's time you learned what true power is!"
	scVenusLighthouseAerieShowMessage(ACTOR_MENARDI);
	scVenusLighthouseAerieFlashScreen();

	menardi                  = fdActorGet(ACTOR_MENARDI);
	menardiBurst.palette     = 7;
	menardiBurst.update      = scVenusLighthouseAerieUpdateDriftingParticle;
	menardiBurst.startScaleX = toFIXED(1);
	menardiBurst.startScaleY = toFIXED(1);
	for (answeredYes = 0; answeredYes <= BURST_DIRECTIONS; answeredYes++)
	{
		scVenusLighthouseAerieAimBurst(velocity, answeredYes * (TRIG_TURN / BURST_DIRECTIONS));
		fdEffectSpawn(menardi->x.fixed,
		              menardi->y.fixed,
		              menardi->z.fixed,
		              velocity[0],
		              velocity[1],
		              velocity[2],
		              EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
		              &menardiBurst);
	}
	sySoundPlay(SE_POWER_UP);
	sySynch(6);
	scVenusLighthouseAerieFlashScreen();
	saturos                  = fdActorGet(ACTOR_SATUROS);
	saturosBurst.palette     = 7;
	saturosBurst.update      = scVenusLighthouseAerieUpdateDriftingParticle;
	saturosBurst.startScaleX = toFIXED(1);
	saturosBurst.startScaleY = toFIXED(1);
	for (answeredYes = 0; answeredYes <= BURST_DIRECTIONS; answeredYes++)
	{
		scVenusLighthouseAerieAimBurst(velocity, answeredYes * (TRIG_TURN / BURST_DIRECTIONS));
		fdEffectSpawn(saturos->x.fixed,
		              saturos->y.fixed,
		              saturos->z.fixed,
		              velocity[0],
		              velocity[1],
		              velocity[2],
		              EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
		              &saturosBurst);
	}
	sySoundPlay(SE_POWER_UP);
	fdActorJump(ACTOR_IVAN, 6, 20);
	sySoundPlay(MUS_BATTLE_WITH_THE_FUSION_DRAGON);
	// "Their Psynergy is overflowing!"
	scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
	fdActorSetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
	// "Hya ha ha! It's too late to run!"
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	scVenusLighthouseAerieFlashScreen();
	fdActorSetSpeed(ACTOR_SATUROS, 0.2);
	fdActorSetSpeed(ACTOR_MENARDI, 0.2);
	fdActorGet(ACTOR_SATUROS)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_MENARDI)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorSetDestination(ACTOR_SATUROS, 294, 196);
	fdActorSetDestination(ACTOR_MENARDI, 294, 196);
	sySetTickEvent(scVenusLighthouseAerieUpdateFusion, EVENT_PASS_TICK);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	// "Uh-oh... They're fusing!"
	fdScriptShowMessage(ACTOR_GARET, 0);
	gmFlagSet(FLAG_VENUS_AERIE_FUSION_SLIDING);
	// "They've merged into one another! Everyone get back!"
	fdScriptShowMessage(ACTOR_IVAN, 0);
	gmFlagSet(FLAG_VENUS_AERIE_FUSION_GROWING);
	scVenusLighthouseAerieFlashScreen();
	fdScriptWait(20);
	scVenusLighthouseAerieFlashScreen();
	fdScriptWait(20);
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	fdScriptSetAfterBattleDestination(SCENE_VENUS_LIGHTHOUSE_AERIE,
	                                  VENUS_LIGHTHOUSE_AERIE_AFTER_SECOND_BATTLE);
	fdScriptSetDefeatDestination(SCENE_VENUS_LIGHTHOUSE_AERIE, VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 0);
	gmPartyRestorePpInLighthouse();
	gmFlagSet(FLAG_AERIE_SECOND_BATTLE_BEGUN);
}

// The aerie flashes white and goes back to the dark red it turns while a thin beam of light rises
// from the beacon and Saturos and Menardi glow with its power.
static void scVenusLighthouseAerieFlashScreen(void)
{
	sySoundPlay(SE_FALLING_LIGHT);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_BG);
	fdPaletteFadeStartInterpolation(1);
	sySynch(4);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(13, 8, 9), PALETTES_BG);
	fdPaletteFadeStartInterpolation(1);
	sySynch(1);
}

// A sparkle circling the beam of light over the beacon, a thirty-second of a turn a tick, in a flat
// ellipse.
static void scVenusLighthouseAerieUpdateUpperOrbit(ACTOR *effect)
{
	ACTOR *center    = fdActorGet(ACTOR_BEAM_BOTTOM);
	Uint16 angle     = effect->timer;

	effect->x.fixed  = center->x.fixed + syCos(angle) * (effect->speedLimit + 3);
	effect->z.fixed  = center->z.fixed + sySin(angle) * 2;
	effect->targetX  = effect->x.fixed;
	effect->targetZ  = effect->z.fixed;
	effect->timer   -= TRIG_TURN / 32;
}

// A sparkle circling the orb of light as it rises out of the beacon, in an ellipse that widens as
// the orb climbs.
static void scVenusLighthouseAerieUpdateRisingOrbit(ACTOR *actor)
{
	ACTOR *anchor = fdActorGet(ACTOR_BEACON_ORB);
	Sint32 angle  = (Uint16)actor->timer;
	FIXED  cosine;
	FIXED  sine;
	FIXED  along;
	FIXED  across;

	cosine         = syCos(angle);
	along          = anchor->x.fixed + cosine * (actor->speedLimit + actor->sceneCount + 6);
	actor->x.fixed = along;

	sine           = sySin(angle);
	across         = anchor->z.fixed + sine * (actor->sceneCount + 4);
	actor->z.fixed = across;

	actor->targetX = actor->x.fixed;
	actor->targetZ = across;

	{
		Sint32 next   = (Uint16)actor->timer;
		next         -= TRIG_TURN / 32;
		actor->timer  = next;
	}
}

// Sparkles rise around the beam of light over the beacon, more of them and higher once it has
// erupted.
static void scVenusLighthouseAerieSpawnUpperOrbitParticles(void)
{
	ACTOR *center;
	ACTOR *effect;
	SPR   *sprite;
	Sint32 angle;

	if (fdScriptGetFlag(FLAG_VENUS_AERIE_BEACON_ERUPTED) == 0
	    && syMathRemainderUnsigned(TickCount, 3) != 0)
	{
		return;
	}
	center = fdActorGet(ACTOR_BEAM_BOTTOM);
	if (gmFlagGet(FLAG_VENUS_AERIE_BEACON_ERUPTED) != 0)
	{
		effect =
			syFieldActorCreate(SPRITE_PARTICLE,
		                       center->x.fixed,
		                       (Sint32)toFIXED(syRandomBelow(256)) + center->y.fixed - toFIXED(28),
		                       center->z.fixed);
	}
	else
	{
		effect =
			syFieldActorCreate(SPRITE_PARTICLE,
		                       center->x.fixed,
		                       (Sint32)toFIXED(syRandomBelow(64)) + center->y.fixed - toFIXED(28),
		                       center->z.fixed);
	}
	if (effect != NULL)
	{
		sprite = effect->sprite;
		fdActorStartScript(effect, VenusLighthouseAerieOrbitParticleScript);
		fdActorSetSpriteColor(effect, SPRITE_COLOR_GRB);
		effect->motionFlags    = 0;
		angle                  = syRandomNext() & 0xffff000;
		effect->timer          = angle;
		effect->userValue      = 0;
		effect->update         = scVenusLighthouseAerieUpdateUpperOrbit;
		effect->speedLimit     = sySin(syRandomBelow(TRIG_TURN - 1) >> 4) * 24 >> 16;
		sprite->flags          = 0;
		sprite->image.priority = 1;
	}
}

// Whether an actor has no move target left: it has reached where it was sent, or was never sent
// anywhere.
static inline Sint32 scVenusLighthouseAerieHasArrived(ACTOR *actor)
{
	if (actor->targetX == ACTOR_NO_TARGET && actor->targetY == actor->targetX
	    && actor->targetZ == actor->targetY)
	{
		return 1;
	}
	return 0;
}

// Each tick of the fusion: Saturos and Menardi, facing east, flicker in turn, one shown while the
// other is hidden, so that they seem to merge. While FLAG_VENUS_AERIE_FUSION_GROWING is set both
// glow white and grow to a quarter larger, and while FLAG_VENUS_AERIE_FUSION_SLIDING is set they
// slide together toward the beacon.
static void scVenusLighthouseAerieUpdateFusion(void)
{
	ACTOR *saturos;
	ACTOR *menardi;
	Sint32 pairArrived;

	saturos = fdActorGet(ACTOR_SATUROS);
	menardi = fdActorGet(ACTOR_MENARDI);
	// BUG: Saturos's test is overwritten by Menardi's, so only Menardi needs to have arrived.
#if FIX_FUSION_WAITS_FOR_BOTH
	pairArrived =
		scVenusLighthouseAerieHasArrived(saturos) && scVenusLighthouseAerieHasArrived(menardi);
#else
	pairArrived = scVenusLighthouseAerieHasArrived(saturos);
	pairArrived = scVenusLighthouseAerieHasArrived(menardi);
#endif
	if (pairArrived != 0)
	{
		saturos->facing = FACING_EAST;
		menardi->facing = FACING_EAST;
		if (gmFlagGet(FLAG_VENUS_AERIE_FUSION_GROWING) != 0)
		{
			fdActorSetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_WHITE);
			fdActorSetSpriteColorById(ACTOR_MENARDI, SPRITE_COLOR_WHITE);
			if (saturos->scaleX < toFIXED(5) / 4)
			{
				saturos->scaleX += toFIXED(1) / 128;
				saturos->scaleY += toFIXED(1) / 128;
				menardi->scaleX += toFIXED(1) / 128;
				menardi->scaleY += toFIXED(1) / 128;
			}
		}
		else
		{
			if ((TickCount & 2) != 0)
			{
				fdActorSetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_HIDDEN);
				fdActorSetSpriteColorById(ACTOR_MENARDI, SPRITE_COLOR_NORMAL);
			}
			else
			{
				fdActorSetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_NORMAL);
				fdActorSetSpriteColorById(ACTOR_MENARDI, SPRITE_COLOR_HIDDEN);
			}
		}
		if (gmFlagGet(FLAG_VENUS_AERIE_FUSION_SLIDING) != 0)
		{
			if (saturos->x.fixed < toFIXED(312))
			{
				saturos->x.fixed += toFIXED(1) / 16;
				menardi->x.fixed += toFIXED(1) / 16;
			}
			if (saturos->z.fixed > toFIXED(182))
			{
				saturos->z.fixed -= toFIXED(1) / 16;
				menardi->z.fixed -= toFIXED(1) / 16;
			}
		}
	}
}

// Plays Saturos and Menardi's defeat, the party's discussion and the beacon's eruption.
static void scVenusLighthouseAerieFinishSaturosAndMenardiBattle(void)
{
	fdScriptStart();
	scVenusLighthouseAerieShowSaturosAndMenardiFall();
	scVenusLighthouseAerieDiscussSaturosAndMenardiDefeat();
	scVenusLighthouseAerieEruptBeacon();
	fdScriptEnd();
}

// A look back from the World Map shows Sheba in danger (VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT,
// after the World Map's lighthouse transition).
static void scVenusLighthouseAerieShowShebaInDanger(void)
{
	fdScriptStart();
	scVenusLighthouseAerieRestorePartyOnBrokenAerie();
	scVenusLighthouseAerieTryToPreventFall();
	gmFlagSet(FLAG_KEEP_MUSIC_ONCE);

	FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
	FieldStateWork->transitionFrame = 24;

	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP);
	fdScriptEnd();
}

// Back from the World Map, stages the broken aerie for Sheba's fall and the party's escape
// (VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP).
static void scVenusLighthouseAerieStageShebasFall(void)
{
	ACTOR *leader;
	ACTOR *actor;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	fdScriptStart();
	fdCameraGetActor()->motionFlags = 0;
	fdMapGridCopyMetatilesTo(102, 4, 74, 4, 18, 23);
	fdMapGridCopyMetatilesTo(39, 72, 11, 72, 16, 20);
	fdMapGridCopyMetatileAttributes(19, 6, 3, 7, 22, 6);
	fdMapGridCopyMetatileAttributes(19, 6, 3, 7, 13, 6);
	fdMapGridCopyMetatileAttributes(19, 6, 3, 7, 22, 13);
	fdMapGridCopyMetatileAttributes(19, 6, 3, 7, 13, 13);
	sySynch(1);
	fdCameraMoveTo(192, -64, 238, 0);
	sySynch(1);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdActorReleaseById(ACTOR_SATUROS);
	fdActorReleaseById(ACTOR_MENARDI);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_KNOCKED_DOWN);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	leader->x.fixed = toFIXED(346);
	leader->z.fixed = toFIXED(205);
	leader->y.fixed = toFIXED(32);
	leader->facing  = FACING_SOUTHWEST;
	scVenusLighthouseAerieStopActorMotion(leader);
	fdActorSetAnimation(ACTOR_GARET, ANIM_PARTY_KNOCKED_DOWN);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_GARET), 0);
	actor          = fdActorGet(ACTOR_GARET);
	actor->x.fixed = toFIXED(356);
	actor->z.fixed = toFIXED(192);
	actor->facing  = FACING_NORTHWEST;
	actor->y.fixed = toFIXED(32);
	scVenusLighthouseAerieStopActorMotion(actor);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_PARTY_KNOCKED_DOWN);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_IVAN), 0);
	actor          = fdActorGet(ACTOR_IVAN);
	actor->x.fixed = toFIXED(360);
	actor->facing  = FACING_SOUTHEAST;
	actor->y.fixed = toFIXED(32);
	actor->z.fixed = toFIXED(222);
	scVenusLighthouseAerieStopActorMotion(actor);
	fdActorSetAnimation(ACTOR_MIA, ANIM_PARTY_KNOCKED_DOWN);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_MIA), 0);
	actor          = fdActorGet(ACTOR_MIA);
	actor->x.fixed = toFIXED(334);
	actor->facing  = FACING_WEST;
	actor->y.fixed = toFIXED(32);
	actor->z.fixed = toFIXED(222);
	scVenusLighthouseAerieStopActorMotion(actor);
	fdActorSetPos(ACTOR_FELIX, 196, 220);
	fdActorSetAnimation(ACTOR_FELIX, FELIX_ANIMATION_KNEEL);
	fdActorSetPos(ACTOR_SHEBA, 188, 316);
	fdActorSetAnimation(ACTOR_SHEBA, SHEBA_ANIMATION_HANG_ONE_HAND);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_SHEBA), 0);
	actor           = fdActorGet(ACTOR_BEACON_STATUE_SOUTHWEST);
	actor->x.fixed -= toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_BEACON_STATUE_NORTHWEST);
	actor->x.fixed -= toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_BEACON_STATUE_NORTHEAST);
	actor->x.fixed += toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_BEACON_STATUE_SOUTHEAST);
	actor->x.fixed += toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	fdMapGridSetShake(1, 1, 1);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BEACON_ORB), 0);
	fdActorGet(ACTOR_BEACON_ORB)->motionFlags = ACTOR_FLOATS;
	fdActorSetSpriteColorById(ACTOR_BEACON_ORB, SPRITE_COLOR_GBR);
	fdActorGet(ACTOR_BEACON_ORB)->y.fixed = toFIXED(40);
	sySetTickEvent(scVenusLighthouseAerieSpawnSideOrbitParticles, EVENT_PASS_TICK);
	FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
	FieldStateWork->transitionFrame = 24;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	scVenusLighthouseAeriePlayShebasFall();
	gmFlagSet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED);
	syImportScriptSetExitRequest(VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_C);
}

// Shows Saturos and Menardi falling into the beacon after their final defeat.
static void scVenusLighthouseAerieShowSaturosAndMenardiFall(void)
{
	ACTOR *actor;

	sySoundPlay(SE_RUMBLE);
	fdMapGridCopyMetatileAttributes(17, 10, 4, 2, 17, 8);
	fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_NORTHWEST;
	fdActorGet(ACTOR_GARET)->facing        = FACING_NORTHWEST;
	fdActorSetPos(ACTOR_GARET, 328, 168);
	fdActorGet(ACTOR_IVAN)->facing = FACING_NORTHWEST;
	fdActorSetPos(ACTOR_IVAN, 340, 196);
	fdActorGet(ACTOR_MIA)->facing = FACING_NORTHWEST;
	fdActorSetPos(ACTOR_MIA, 326, 204);
	fdActorGet(ACTOR_FELIX)->facing = FACING_NORTH_NORTHEAST;
	fdActorSetPos(ACTOR_FELIX, 200, 216);
	fdActorGet(ACTOR_SHEBA)->facing = FACING_NORTH_NORTHEAST;
	fdActorSetPos(ACTOR_SHEBA, 200, 216);
	fdActorGet(ACTOR_SATUROS)->facing = FACING_SOUTH_SOUTHEAST;
	fdActorSetPos(ACTOR_SATUROS, 310, 158);
	fdActorGet(ACTOR_MENARDI)->facing = FACING_SOUTH_SOUTHEAST;
	fdActorSetPos(ACTOR_MENARDI, 292, 158);

	fdActorSetSpriteFlags(fdActorGet(ACTOR_BEAM_BOTTOM), 0);
	fdActorSetSpriteColorById(ACTOR_BEAM_BOTTOM, SPRITE_COLOR_WHITE);
	fdActorSetSpritePriority(ACTOR_BEAM_BOTTOM, 1);
	actor              = fdActorGet(ACTOR_BEAM_BOTTOM);
	actor->scaleY      = -toFIXED(1);
	actor->scaleX      = toFIXED(1) / 5;
	actor->motionFlags = 0;
	actor->x.fixed     = toFIXED(304);
	actor->y.fixed     = toFIXED(2);
	actor->z.fixed     = toFIXED(96);

	fdActorSetSpriteFlags(fdActorGet(ACTOR_BEAM_LOWER), 0);
	fdActorSetSpriteColorById(ACTOR_BEAM_LOWER, SPRITE_COLOR_WHITE);
	fdActorSetSpritePriority(ACTOR_BEAM_LOWER, 1);
	actor              = fdActorGet(ACTOR_BEAM_LOWER);
	actor->scaleY      = -toFIXED(1);
	actor->scaleX      = toFIXED(1) / 5;
	actor->motionFlags = 0;
	actor->x.fixed     = toFIXED(304);
	actor->y.fixed     = toFIXED(34);
	actor->z.fixed     = toFIXED(96);

	sySetTickEvent(scVenusLighthouseAerieSpawnUpperOrbitParticles, EVENT_PASS_TICK);
	fdCameraGetActor()->motionFlags = 0;
	fdCameraMoveTo(304, 32, 180, 0);
	sySynch(1);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdMapGridSetShake(1, 1, 1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
	fdScriptWait(10);
	fdScriptSetMessageId(MsgVenusPairDefeated);
	// "How... How... did we lose?"
	scVenusLighthouseAerieShowMessage(ACTOR_SATUROS);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 3);
	fdScriptWait(20);
	// "We are superior in every way, but still we were defeated..."
	fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 40);
	sySoundPlay(SOUND_FADE_OUT_BGM);

	actor          = fdActorGet(ACTOR_SATUROS);
	actor->x.fixed = toFIXED(308);
	actor->y.fixed = toFIXED(28);
	actor->z.fixed = toFIXED(152);
	fdActorSetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_TIP);
	fdScriptWait(20);
	actor->x.fixed = toFIXED(306);
	actor->y.fixed = toFIXED(28);
	actor->z.fixed = toFIXED(152);
	fdActorSetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_SLUMP);
	fdScriptWait(12);
	actor->x.fixed = toFIXED(304);
	actor->y.fixed = toFIXED(21);
	actor->z.fixed = toFIXED(152);
	fdActorSetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_SINK);
	fdScriptWait(8);
	fdActorReleaseById(ACTOR_SATUROS);

	actor          = fdActorGet(ACTOR_MENARDI);
	actor->x.fixed = toFIXED(294);
	actor->y.fixed = toFIXED(28);
	actor->z.fixed = toFIXED(152);
	fdActorSetAnimation(ACTOR_MENARDI, MENARDI_ANIMATION_TIP);
	fdScriptWait(20);
	actor->x.fixed = toFIXED(300);
	actor->y.fixed = toFIXED(27);
	actor->z.fixed = toFIXED(152);
	fdActorSetAnimation(ACTOR_MENARDI, MENARDI_ANIMATION_SLUMP);
	fdScriptWait(12);
	actor->x.fixed = toFIXED(304);
	actor->y.fixed = toFIXED(17);
	actor->z.fixed = toFIXED(152);
	fdActorSetAnimation(ACTOR_MENARDI, MENARDI_ANIMATION_SINK);
	fdScriptWait(8);
	fdActorReleaseById(ACTOR_MENARDI);
	fdScriptWait(160);
}

// Lets the party discuss the battle while Felix and Sheba remain on the aerie.
static void scVenusLighthouseAerieDiscussSaturosAndMenardiDefeat(void)
{
	Sint32 answeredYes;
	Sint32 garetSpeaker;
	Sint32 miaSpeaker;

	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(20);
	// "We did it! We won!"
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdScriptWait(20);
	// "We beat them...but have we really won?"
	fdScriptOpenMessage(ACTOR_IVAN, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	scVenusLighthouseAerieFaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH);
	answeredYes = 0;
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
	{
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
		answeredYes = 1;
	}
	else
	{
		fdScriptWait(20);
		fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
		FieldStateWork->msg += 1;
	}
	// Yes: "Even though we couldn't stop the beacon from being lit?" No: "All our efforts were for
	// naught."
	scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
	if (answeredYes != 0)
	{
		FieldStateWork->msg += 1;
	}
	fdActorSetSpeed(ACTOR_GARET, 1.6);
	garetSpeaker = SPEAKER_PORTRAIT_LEFT | ACTOR_GARET;
	fdActorWalkToAndWait(ACTOR_GARET, 321, 174);
	scVenusLighthouseAerieFaceActor(ACTOR_GARET, FACING_SOUTHEAST);
	// "What are you talking about?"
	scVenusLighthouseAerieShowMessage(garetSpeaker);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	scVenusLighthouseAerieFaceActor(ACTOR_MIA, FACING_NORTH);
	scVenusLighthouseAerieFaceActor(ACTOR_GARET, FACING_SOUTH);
	// "We did everything we could!"
	scVenusLighthouseAerieShowMessage(garetSpeaker);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
	// "Sure we couldn't save the Venus Lighthouse, but..."
	scVenusLighthouseAerieShowMessage(garetSpeaker);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 40);
	// "We beat them! They're gone! You saw them fall down into the pit..."
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_GARET, 0, 40);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
	// "So we don't have to worry about any more beacons being lit!"
	scVenusLighthouseAerieShowMessage(garetSpeaker);
	sySoundPlay(SOUND_FADE_OUT_BGM);
	fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
	// "But Felix is gone, and he's taken Sheba..."
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 80);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 80);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 60);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	scVenusLighthouseAerieFaceActor(ACTOR_IVAN, FACING_NORTH);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 40);
	fdActorJump(ACTOR_IVAN, 4, 60);
	scVenusLighthouseAerieFaceActor(ACTOR_GARET, FACING_SOUTHEAST);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	// "What's with you guys? Isaac... What are you lookin' at?!?"
	scVenusLighthouseAerieShowMessage(garetSpeaker);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 40);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorJump(ACTOR_GARET, 4, 40);
	// "Felix!"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	fdActorSetSpeed(ACTOR_FELIX, 0.8);
	fdActorSetDestination(ACTOR_FELIX, 200, 188);
	fdActorSetDestination(ACTOR_SHEBA, 200, 204);
	fdCameraSetSpeed(3.2);
	fdCameraMoveTo(252, 0, 190, 1);
	fdCameraWaitForMove();
	fdScriptWait(40);
	sySoundPlay(MUS_DUNGEON_THEME);
	fdActorSetAttachedEffect(ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	// "Did you finish them?"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
	fdScriptWait(20);
	// "No one knew their power better than I did. If you destroyed them..."
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
	// "Then I am no match for you now, Isaac... I must go..."
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 20);
	// "Come, Sheba!"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorSetAttachedEffect(ACTOR_SHEBA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	miaSpeaker = SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA;
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	// "Wait, Felix!"
	scVenusLighthouseAerieShowMessage(miaSpeaker);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
	fdScriptWait(20);
	// "Why do you run from us?"
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN);
	scVenusLighthouseAerieFaceActor(ACTOR_FELIX, FACING_NORTHEAST);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	// "They fell into the lighthouse. It's all over!"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
	// "Poor fools... If you think it's over, you are sadly mistaken!"
	fdScriptShowMessageThenWait(ACTOR_FELIX, 0, 20);
	// "If you plan to light the other beacons, you will still need..."
	scVenusLighthouseAerieShowMessage(miaSpeaker);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
	// "The Elemental Stars? We have them!"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorSetAttachedEffect(ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	// "They're gone, Felix! You don't have to light the beacons anymore..."
	scVenusLighthouseAerieShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	// "Yes, I do. If I don't light the beacons..."
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	// "What will happen?"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_SHAKE_HEAD);
	// "It's no use talking about it... Just wait and see!"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHWEST, 20);
	fdActorJump(ACTOR_MIA, 4, 20);
	// "If you won't release Sheba, we'll have to take her back!"
	fdScriptShowMessageThenWait(miaSpeaker, 0, 20);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
}

static Sint32 BeaconRiseTimer; // set as the light starts lifting the party
// The ticks scVenusLighthouseAerieUpdateBeaconRise has run, held while the orb rises.
static Sint32 BeaconRiseStep;

// Raises the beacon effects and splits the aerie beneath the group.
static void   scVenusLighthouseAerieEruptBeacon(void)
{
	ACTOR *middle;
	ACTOR *upper;
	ACTOR *top;
	ACTOR *actor;
	Uint16 white;

	sySoundPlay(MUS_SILENCE);
	sySoundPlay(SE_STOP_ACTION_SE);
	fdMapGridSetShake(3, 3, 1);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH_NORTHEAST, 0);
	fdMapGridSetShake(2, 2, 1);
	fdScriptWait(10);
	fdMapGridSetShake(1, 1, 1);
	fdCameraMoveTo(304, 32, 180, 1);
	fdCameraWaitForMove();
	fdActorGet(ACTOR_BEAM_BOTTOM)->scaleX = toFIXED(1) / 10;
	fdActorGet(ACTOR_BEAM_LOWER)->scaleX  = toFIXED(1) / 10;
	fdActorSetScript(ACTOR_BEAM_BOTTOM, VenusLighthouseAerieBeamOpenScript);
	fdActorSetScript(ACTOR_BEAM_LOWER, VenusLighthouseAerieBeamOpenScript);
	sySoundPlay(SE_CRASH);
	fdMapGridSetShake(6, 6, 1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
	fdPaletteFadeStartInterpolation(16);
	sySynch(20);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(24);
	sySynch(60);
	sySoundPlay(SE_RUMBLE);
	gmFlagSet(FLAG_VENUS_AERIE_BEACON_ERUPTED);
	fdActorGet(ACTOR_BEAM_BOTTOM)->y.fixed = -toFIXED(96);
	fdActorGet(ACTOR_BEAM_LOWER)->y.fixed  = -toFIXED(64);
	fdActorSetSpriteColorById(ACTOR_BEAM_MIDDLE, SPRITE_COLOR_WHITE);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BEAM_MIDDLE), 0);
	// Stack three more beam segments above the lower two.
	middle              = fdActorGet(ACTOR_BEAM_MIDDLE);
	middle->scaleY      = -toFIXED(1);
	middle->scaleX      = fdActorGet(ACTOR_BEAM_BOTTOM)->scaleX;
	middle->motionFlags = 0;
	middle->x.fixed     = toFIXED(304);
	middle->y.fixed     = -toFIXED(32);
	middle->z.fixed     = toFIXED(96);
	fdActorSetSpriteColorById(ACTOR_BEAM_UPPER, SPRITE_COLOR_WHITE);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BEAM_UPPER), 0);
	upper              = fdActorGet(ACTOR_BEAM_UPPER);
	upper->scaleY      = -toFIXED(1);
	upper->scaleX      = fdActorGet(ACTOR_BEAM_BOTTOM)->scaleX;
	upper->motionFlags = 0;
	upper->x.fixed     = toFIXED(304);
	upper->y.fixed     = 0;
	upper->z.fixed     = toFIXED(96);
	fdActorSetSpriteColorById(ACTOR_BEAM_TOP, SPRITE_COLOR_WHITE);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BEAM_TOP), 0);
	top              = fdActorGet(ACTOR_BEAM_TOP);
	top->scaleY      = -toFIXED(1);
	top->scaleX      = fdActorGet(ACTOR_BEAM_BOTTOM)->scaleX;
	top->motionFlags = 0;
	top->x.fixed     = toFIXED(304);
	top->y.fixed     = toFIXED(32);
	top->z.fixed     = toFIXED(96);
	fdMapGridCopyMetatilesTo(102, 4, 74, 4, 18, 23);
	fdMapGridCopyMetatilesTo(39, 72, 11, 72, 16, 21);
	fdMapGridCopyMetatileAttributes(19, 6, 3, 7, 22, 6);
	fdMapGridCopyMetatileAttributes(19, 6, 3, 7, 13, 6);
	fdMapGridCopyMetatileAttributes(19, 6, 3, 7, 22, 13);
	fdMapGridCopyMetatileAttributes(19, 6, 3, 7, 13, 13);
	sySynch(1);
	actor           = fdActorGet(ACTOR_BEACON_STATUE_SOUTHWEST);
	actor->x.fixed -= toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_BEACON_STATUE_NORTHWEST);
	actor->x.fixed -= toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_BEACON_STATUE_NORTHEAST);
	actor->x.fixed += toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_BEACON_STATUE_SOUTHEAST);
	actor->x.fixed += toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_PARTY_LEADER);
	actor->x.fixed += toFIXED(16);
	actor->z.fixed += toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_GARET);
	actor->x.fixed += toFIXED(16);
	actor->z.fixed += toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_IVAN);
	actor->x.fixed += toFIXED(16);
	actor->z.fixed += toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_MIA);
	actor->x.fixed += toFIXED(16);
	actor->z.fixed += toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	fdActorSetPos(ACTOR_FELIX, 196, 220);
	fdActorSetAnimation(ACTOR_FELIX, FELIX_ANIMATION_KNEEL);
	fdActorSetPos(ACTOR_SHEBA, 188, 316);
	fdActorSetAnimation(ACTOR_SHEBA, SHEBA_ANIMATION_HANG_ONE_HAND);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_SHEBA), 0);
	fdMapGridSetShake(4, 4, 1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
	fdPaletteFadeStartInterpolation(120);
	fdActorSetScript(ACTOR_BEAM_BOTTOM, VenusLighthouseAerieBeamRiseScript);
	fdActorSetScript(ACTOR_BEAM_LOWER, VenusLighthouseAerieBeamRiseScript);
	fdActorSetScript(ACTOR_BEAM_MIDDLE, VenusLighthouseAerieBeamRiseScript);
	fdActorSetScript(ACTOR_BEAM_UPPER, VenusLighthouseAerieBeamRiseScript);
	fdActorSetScript(ACTOR_BEAM_TOP, VenusLighthouseAerieBeamRiseScript);
	sySynch(120);
	fdMapGridSetShake(3, 3, 1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12), PALETTES_ALL);
	fdPaletteFadeStartInterpolation(120);
	sySynch(120);
	fdMapGridSetShake(2, 2, 1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(120);
	sySynch(120);
	fdMapGridSetShake(1, 1, 1);
	fdActorGet(ACTOR_BEAM_BOTTOM)->scaleY = toFIXED(1) / 50;
	fdActorSetScript(ACTOR_BEAM_LOWER, VenusLighthouseAerieBeamCloseScript);
	fdActorSetScript(ACTOR_BEAM_MIDDLE, VenusLighthouseAerieBeamCloseScript);
	fdActorSetScript(ACTOR_BEAM_UPPER, VenusLighthouseAerieBeamCloseScript);
	fdActorRunScriptAndWait(ACTOR_BEAM_TOP, VenusLighthouseAerieBeamCloseScript);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	fdActorSetSpriteColorById(ACTOR_BEAM_BOTTOM, SPRITE_COLOR_HIDDEN);
	fdScriptWait(20);
	fdActorRunScriptAndWait(ACTOR_BEAM_BOTTOM, VenusLighthouseAerieBeamEndScript);
	syCloseEvent(scVenusLighthouseAerieSpawnUpperOrbitParticles);
	fdActorJump(ACTOR_IVAN, 2, 20);
	// "The lighthouse...!"
	scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
	fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	// "Isaac! They're too far away! We can't reach Felix!"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	fdActorSetSpeed(ACTOR_MIA, 0.8);
	fdActorWalkToAndWait(ACTOR_MIA, 326, 220);
	fdScriptWait(40);
	fdActorSetAttachedEffect(ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	// "That's not all! There's light coming from down below!"
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	actor                                     = fdActorGet(ACTOR_PARTY_LEADER);
	actor->sceneCount                         = 0;
	actor->sceneByte                          = 1;
	actor->sceneWord                          = actor->y.fixed;
	actor->facing                             = FACING_NORTHWEST;
	actor                                     = fdActorGet(ACTOR_GARET);
	actor->sceneCount                         = 0;
	actor->sceneByte                          = 1;
	actor->facing                             = FACING_NORTHWEST;
	actor->sceneWord                          = actor->y.fixed;
	actor                                     = fdActorGet(ACTOR_IVAN);
	actor->sceneCount                         = 0;
	actor->sceneByte                          = 1;
	actor->facing                             = FACING_NORTHWEST;
	actor->sceneWord                          = actor->y.fixed;
	actor                                     = fdActorGet(ACTOR_MIA);
	actor->sceneCount                         = 0;
	actor->sceneByte                          = 1;
	actor->facing                             = FACING_NORTHWEST;
	actor->sceneWord                          = actor->y.fixed;
	actor                                     = fdActorGet(ACTOR_FELIX);
	actor->sceneCount                         = 0;
	actor->sceneByte                          = 1;
	actor->sceneWord                          = actor->y.fixed;
	actor                                     = fdActorGet(ACTOR_SHEBA);
	actor->sceneCount                         = 0;
	actor->sceneByte                          = 1;
	actor->sceneWord                          = actor->y.fixed;
	fdActorGet(ACTOR_BEACON_ORB)->motionFlags = 0;
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BEACON_ORB), 0);
	fdActorSetSpriteColorById(ACTOR_BEACON_ORB, SPRITE_COLOR_WHITE);
	fdActorSetSpritePriority(ACTOR_BEACON_ORB, 2);
	BeaconRiseStep  = 0;
	// DEAD: nothing reads BeaconRiseTimer; the ROM keeps its store.
	BeaconRiseTimer = 240;
	sySetTickEvent(scVenusLighthouseAerieUpdateBeaconRise, EVENT_PASS_TICK);
	do
	{
		sySynch(1);
	} while (gmFlagGet(FLAG_VENUS_AERIE_PARTY_LIFTED) == 0);
	gmFlagSet(FLAG_SKIP_SCENE_CHANGE_SOUND);
	fdScriptWait(30);
	gmFlagSet(FLAG_KEEP_MUSIC_ONCE);
	fdActorSnapshot();
	fdScriptSetDestination(SCENE_WORLD_MAP, WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT);
	white                           = RGB_WHITE;

	PLTT_BACKDROP                   = white;
	FieldStateWork->transitionFrame = 1;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
}

// Places the party on the broken platform and resumes its falling effects.
static void scVenusLighthouseAerieRestorePartyOnBrokenAerie(void)
{
	ACTOR *actor;

	fdActorReleaseById(ACTOR_SATUROS);
	fdActorReleaseById(ACTOR_MENARDI);
	sySoundPlay(SE_RUMBLE);
	fdMapGridCopyMetatileAttributes(17, 10, 4, 2, 17, 8);
	fdMapGridCopyMetatilesTo(102, 4, 74, 4, 18, 23);
	fdMapGridCopyMetatilesTo(39, 72, 11, 72, 16, 21);
	fdMapGridCopyMetatileAttributes(19, 6, 3, 7, 22, 6);
	fdMapGridCopyMetatileAttributes(19, 6, 3, 7, 13, 6);
	fdMapGridCopyMetatileAttributes(19, 6, 3, 7, 22, 13);
	fdMapGridCopyMetatileAttributes(19, 6, 3, 7, 13, 13);
	fdMapGridSetShake(2, 2, 1);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorRestore();
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	sySoundPlay(SE_ITEM_BREAK);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_KNOCKED_DOWN);
	fdActorSetAnimation(ACTOR_GARET, ANIM_PARTY_KNOCKED_DOWN);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_PARTY_KNOCKED_DOWN);
	fdActorSetAnimation(ACTOR_MIA, ANIM_PARTY_KNOCKED_DOWN);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_GARET), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_IVAN), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_MIA), 0);

	actor          = fdActorGet(ACTOR_PARTY_LEADER);
	actor->x.fixed = toFIXED(346);
	actor->y.fixed = toFIXED(32);
	actor->z.fixed = toFIXED(205);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor->sceneByte = 0;
	actor->velocityY = toFIXED(2);

	actor            = fdActorGet(ACTOR_GARET);
	actor->x.fixed   = toFIXED(356);
	actor->y.fixed   = toFIXED(32);
	actor->z.fixed   = toFIXED(192);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor->sceneByte = 0;
	actor->velocityY = toFIXED(2);

	actor            = fdActorGet(ACTOR_IVAN);
	actor->x.fixed   = toFIXED(360);
	actor->y.fixed   = toFIXED(32);
	actor->z.fixed   = toFIXED(222);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor->sceneByte = 0;
	actor->velocityY = toFIXED(2);

	actor            = fdActorGet(ACTOR_MIA);
	actor->x.fixed   = toFIXED(334);
	actor->y.fixed   = toFIXED(32);
	actor->z.fixed   = toFIXED(222);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor->sceneByte                           = 0;
	actor->velocityY                           = toFIXED(2);

	fdActorGet(ACTOR_FELIX)->sceneCount        = 0;
	fdActorGet(ACTOR_SHEBA)->sceneCount        = 0;
	fdActorGet(ACTOR_BEACON_ORB)->motionFlags |= ACTOR_FLOATS;
	fdActorSetSpriteColorById(ACTOR_BEACON_ORB, SPRITE_COLOR_GBR);
	sySetTickEvent(scVenusLighthouseAerieSpawnSideOrbitParticles, EVENT_PASS_TICK);
	FieldStateWork->transitionFrame = 1;
	fdScriptStartScreenTransition();
	fdPaletteFadeApplySource(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(40);
	sySynch(60);
}

// Shows Felix urging Sheba to hold on at the edge of the platform.
static void scVenusLighthouseAerieTryToPreventFall(void)
{
	fdScriptSetMessageId(MsgVenusNoooo);
	// "Noooo! Sheba!"
	scVenusLighthouseAerieShowMessage(ACTOR_FELIX);
	sySoundPlay(MUS_SHEBA_HANGS_ON);
	fdMapGridSetShake(1, 1, 1);
	fdCameraSetSpeed(4.8);
	fdCameraSetSpeed(4);
	fdCameraMoveTo(192, -64, 238, 1);
	fdCameraWaitForMove();
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
	// "Hold on, Sheba! Take my hand!"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 40);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 3);
	// "I can't..."
	scVenusLighthouseAerieShowMessage(ACTOR_SHEBA);
	fdActorSetAttachedEffect(ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	// "You must try! If you don't, you'll fall!"
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 80);
	fdActorSetAttachedEffect(ACTOR_SHEBA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorStartRepeatedMotion(ACTOR_SHEBA, 2);
	// "Look down... The foundations of the lighthouse are crumbling!"
	scVenusLighthouseAerieShowMessage(ACTOR_SHEBA);
}

// Shows Sheba falling, Felix jumping after her and the party escaping.
static void scVenusLighthouseAeriePlayShebasFall(void)
{
	ACTOR *actor;
	Sint32 answeredYes;

	answeredYes = 0;
	// Sheba loses her grip while Felix tries to keep her on the platform.
	fdScriptSetMessageId(MsgVenusWhyHappeningProtectVenusLighthouse);
	fdActorSetAttachedEffect(ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(20);
	// "Why is this happening? Is it to protect Venus Lighthouse?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	// "I'm slipping... I can't hold on any longer."
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 20);
	fdActorSetAnimation(ACTOR_SHEBA, SHEBA_ANIMATION_HANG);
	fdScriptWait(10);
	fdActorStartRepeatedMotion(ACTOR_FELIX, 2);
	// "Nooo! Don't let go! You can't! You'll..."
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_FELIX, 0, 40);
	fdActorSetAnimation(ACTOR_SHEBA, SHEBA_ANIMATION_SLIP);
	// "Good-bye, Felix, and thank you."
	fdScriptShowMessageThenWait(ACTOR_SHEBA, 0, 20);
	sySoundPlay(SOUND_FADE_OUT_BGM);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdActorSetScript(ACTOR_SHEBA, VenusLighthouseAerieShebaFallScript);
	fdScriptWait(20);
	fdActorSetAttachedEffect(ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorWaitForScriptEndById(ACTOR_SHEBA);
	fdScriptWait(160);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
	fdScriptWait(20);
	fdActorGet(ACTOR_FELIX)->facing = FACING_SOUTH_SOUTHWEST;
	fdActorSetAnimation(ACTOR_FELIX, ANIM_STILL);
	fdScriptWait(80);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
	fdScriptWait(40);
	// "Sheba!!! No! I won't let you die!"
	fdScriptShowMessageThenWait(ACTOR_FELIX, 0, 40);
	fdActorSetAnimation(ACTOR_FELIX, FELIX_ANIMATION_KNEEL);
	fdScriptWait(10);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_FELIX), 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_STILL);
	fdActorJump(ACTOR_FELIX, 6, 0);
	fdActorSetSpeed(ACTOR_FELIX, 3);
	fdActorGet(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
	actor                                   = fdActorGet(ACTOR_FELIX);
	// Felix jumps after Sheba; the remaining party watches from the far side.
	actor->update                           = scVenusLighthouseAerieStepSpriteRotation;
	actor->facing                           = FACING_WEST;
	fdActorMoveToAndWait(ACTOR_FELIX, 184, 237);
	fdActorRunScriptAndWait(ACTOR_FELIX, VenusLighthouseAerieFelixDiveScript);
	fdScriptWait(120);
	// "!!!"
	scVenusLighthouseAerieShowMessage(ACTOR_PARTY_LEADER);
	sySoundPlay(MUS_FELIXS_RESOLVE);
	fdCameraSetSpeed(4);
	fdCameraMoveTo(342, 32, 212, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(40);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorJump(ACTOR_PARTY_LEADER, 6, 60);
	fdActorGet(ACTOR_PARTY_LEADER)->timer = 10;
	fdActorSetScript(ACTOR_PARTY_LEADER, VenusLighthouseAerieBreatheHeavilyScript);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
	// "Sheba fell after that last tremor..."
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 40);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdScriptWait(20);
	// "Felix didn't dive into the sea after her, did he?"
	scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	// "I can't believe he did that from so high up!"
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_GARET), SPRITE_FLAG_SHADOW);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorJump(ACTOR_GARET, 6, 60);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdScriptWait(20);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_IVAN), SPRITE_FLAG_SHADOW);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorJump(ACTOR_IVAN, 6, 40);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_MIA), SPRITE_FLAG_SHADOW);
	fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
	fdActorJump(ACTOR_MIA, 6, 60);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 60);
	fdActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
	// "You idiot, Felix! I can't believe he said he'd light the other beacons."
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	// "We would have had to fight him if this hadn't happened..."
	scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
	scVenusLighthouseAerieFaceActor(ACTOR_MIA, FACING_SOUTHWEST);
	fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
	// "But he sacrificed himself to save Sheba. He's not like Saturos."
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 80);
	scVenusLighthouseAerieFaceActor(ACTOR_GARET, FACING_SOUTH);
	// "I'm at a loss, Isaac. We have to decide what to do next."
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 40);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
	// "What to do? What can we do?"
	fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 40);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
	// "What about Jenna and the others at Idejima?"
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	scVenusLighthouseAerieFaceActor(ACTOR_GARET, FACING_SOUTH);
	// "Oh yeah! I completely forgot about Jenna!"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorSetSpeed(ACTOR_GARET, 1.6);
	fdActorWalkToAndWait(ACTOR_GARET, 333, 194);
	fdActorWalkToAndWait(ACTOR_GARET, 333, 206);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	// "Isaac! Listen up! We've got to find Jenna!"
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 80);
	fdActorStop(ACTOR_PARTY_LEADER);
	sySynch(1);
	actor->scaleX = toFIXED(1);
	actor->scaleY = toFIXED(1);
	fdScriptWait(40);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
	scVenusLighthouseAerieFaceActor(ACTOR_GARET, FACING_SOUTHEAST);
	fdActorJump(ACTOR_GARET, 4, 0);
	// "Look at that! Isaac has finally come back to his senses!"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	scVenusLighthouseAerieFaceActor(ACTOR_GARET, FACING_NORTHEAST);
	// "Shouldn't we go find Jenna and the others right away?"
	fdScriptOpenMessage(ACTOR_GARET, 0);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
	{
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		// "It's about time you started agreeing with me!"
		scVenusLighthouseAerieShowMessage(ACTOR_GARET);
		fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
		// "Isaac, you'll have to tell Jenna about Felix."
		scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
		fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
		// "I know it will be hard, but you'll be there to comfort her."
		scVenusLighthouseAerieShowMessage(ACTOR_MIA);
		answeredYes = 1;
	}
	else
	{
		FieldStateWork->msg += 3;
		fdScriptWait(60);
		// "After all this, you don't have the guts to tell Jenna about Felix?"
		scVenusLighthouseAerieShowMessage(ACTOR_GARET);
		fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
		// "If you don't tell her what happened to Felix, then who will?"
		scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
		fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
		// "You're going to have to find a way to console Jenna..."
		scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	}
	if (answeredYes != 0)
	{
		FieldStateWork->msg += 3;
	}
	scVenusLighthouseAerieFaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH);
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(80);
	sySoundPlay(SOUND_FADE_OUT_BGM);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	// "What's the matter, Garet?"
	scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
	fdActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
	// "I was just thinkin'... How the heck are we supposed to get down?"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 40);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 40);
	fdActorSetAttachedEffect(ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	// "Good point. This is going to make leaving very difficult..."
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	sySoundPlay(SE_RUMBLE);
	fdMapGridSetShake(6, 6, 1);
	fdScriptWait(40);
	sySoundPlay(SE_CRASH);
	fdMapGridCopyMetatilesTo(110, 105, 74, 4, 18, 23);
	fdMapGridCopyMetatilesTo(92, 86, 83, 4, 8, 23);
	fdMapGridCopyMetatilesTo(75, 28, 75, 4, 8, 23);
	fdMapGridCopyMetatilesTo(92, 86, 11, 72, 16, 20);
	fdMapGridCopyMetatilesTo(19, 92, 19, 68, 8, 21);
	actor             = fdActorGet(ACTOR_PARTY_LEADER);
	actor->z.fixed   -= toFIXED(32);
	actor->userValue  = 0;
	scVenusLighthouseAerieStopActorMotion(actor);
	actor             = fdActorGet(ACTOR_GARET);
	actor->x.fixed   -= toFIXED(4);
	actor->z.fixed   -= toFIXED(32);
	actor->userValue  = 0;
	scVenusLighthouseAerieStopActorMotion(actor);
	actor             = fdActorGet(ACTOR_IVAN);
	actor->x.fixed   -= toFIXED(4);
	actor->z.fixed   -= toFIXED(32);
	actor->userValue  = 0;
	scVenusLighthouseAerieStopActorMotion(actor);
	actor             = fdActorGet(ACTOR_MIA);
	actor->x.fixed   -= toFIXED(4);
	actor->z.fixed   -= toFIXED(18);
	actor->userValue  = 0;
	scVenusLighthouseAerieStopActorMotion(actor);
	fdActorGet(ACTOR_BEACON_ORB)->y.fixed = toFIXED(56);
	fdCameraMoveTo(338, 32, 180, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
	fdActorJump(ACTOR_GARET, 6, 0);
	fdActorJump(ACTOR_IVAN, 6, 0);
	fdActorJump(ACTOR_MIA, 6, 0);
	fdActorSetScript(ACTOR_PARTY_LEADER, VenusLighthouseAerieLookAboutScript);
	fdActorSetScript(ACTOR_GARET, VenusLighthouseAerieLookAboutScript);
	fdActorSetScript(ACTOR_IVAN, VenusLighthouseAerieLookAboutScript);
	fdActorSetScript(ACTOR_MIA, VenusLighthouseAerieLookAboutScript);
	fdMapGridSetShake(3, 3, 1);
	fdScriptWait(80);
	fdMapGridSetShake(6, 6, 1);
	fdScriptWait(40);
	sySoundPlay(SE_CRASH);
	fdMapGridCopyMetatileAttributes(110, 106, 18, 14, 10, 5);
	fdMapGridCopyMetatilesTo(110, 105, 74, 4, 18, 23);
	fdMapGridCopyMetatilesTo(92, 86, 11, 68, 16, 20);
	actor           = fdActorGet(ACTOR_PARTY_LEADER);
	actor->x.fixed -= toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_GARET);
	actor->x.fixed -= toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_IVAN);
	actor->x.fixed -= toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_MIA);
	actor->x.fixed -= toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_BEACON_STATUE_SOUTHWEST);
	actor->x.fixed += toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_BEACON_STATUE_NORTHWEST);
	actor->x.fixed += toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_BEACON_STATUE_NORTHEAST);
	actor->x.fixed -= toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	actor           = fdActorGet(ACTOR_BEACON_STATUE_SOUTHEAST);
	actor->x.fixed -= toFIXED(16);
	scVenusLighthouseAerieStopActorMotion(actor);
	fdCameraMoveTo(322, 32, 180, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
	fdActorJump(ACTOR_GARET, 6, 0);
	fdActorJump(ACTOR_IVAN, 6, 0);
	fdActorJump(ACTOR_MIA, 6, 0);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdScriptWait(80);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	fdScriptWait(40);
	fdActorStop(ACTOR_PARTY_LEADER);
	fdActorStop(ACTOR_GARET);
	fdActorStop(ACTOR_IVAN);
	fdActorStop(ACTOR_MIA);
	fdScriptWait(120);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
	// "It's back to normal!"
	scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	// "That was odd... What's going on around here?"
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
	fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
	// "I don't think we'll ever know."
	scVenusLighthouseAerieShowMessage(ACTOR_MIA);
	fdActorJump(ACTOR_IVAN, 2, 20);
	// "At least now, we can go search for Jenna and the others."
	scVenusLighthouseAerieShowMessage(ACTOR_IVAN);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(20);
	scVenusLighthouseAerieFaceActor(ACTOR_GARET, FACING_EAST);
	// "All right, let's go, Isaac! It's time to find Jenna."
	scVenusLighthouseAerieShowMessage(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorSetScript(ACTOR_GARET, VenusLighthouseAeriePartyExitScript);
	fdActorSetScript(ACTOR_IVAN, VenusLighthouseAeriePartyExitScript);
	fdActorSetScript(ACTOR_MIA, VenusLighthouseAeriePartyExitScript);
	fdScriptWait(60);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 272, 216);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 272, 254);
	fdScriptWait(80);
	FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	FieldStateWork->transitionFrame = MAP_TRANSITION_TICKS;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(80);
	fdActorSystemSuspendProjectionAndShowSprites();
	((WIN_STATE *)WindowStateWork[0])->voiceSound     = 0;
	((WIN_STATE *)WindowStateWork[0])->voiceCountdown = 0;
	// "But despite a long and tiring search, Jenna was nowhere to be found."
	mnFieldMessageShowCentered(MsgVenusDespiteLongTiring, 0, 0);
	fdActorSystemResumeProjectionAndHideLayers();
	fdScriptWait(80);
}

// Stops an actor where it stands, with no move under way.
static void scVenusLighthouseAerieStopActorMotion(ACTOR *actor)
{
	actor->targetX = ACTOR_NO_TARGET;
	actor->targetY = ACTOR_NO_TARGET;
	actor->targetZ = ACTOR_NO_TARGET;

	SET_VELOCITY(actor, 0, 0, 0);

	actor->timer = 0;
}

// Each tick as the beacon's light lifts the party away: a flash and a shake, then from tick 24 the
// orb rises from below the aerie, held at tick 25 until it is above the floor and at 26 until it is
// 40 pixels up, with particles circling it to tick 34; the screen fades to white at 36, and at 48
// the party is lifted (FLAG_VENUS_AERIE_PARTY_LIFTED).
static void scVenusLighthouseAerieUpdateBeaconRise(void)
{
	ACTOR *center;
	ACTOR *effect;
	SPR   *sprite;
	Sint32 spawnParticles;
	Uint32 randomHeight;
	Sint32 angle;

	center         = fdActorGet(ACTOR_BEACON_ORB);
	spawnParticles = 0;
	switch (BeaconRiseStep)
	{
		case 0:
			sySoundPlay(SE_SHIMMER);
			fdMapGridSetShake(6, 6, 1);
			fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(31, 31, 24), PALETTES_BG);
			fdPaletteFadeStartInterpolation(8);
			break;
		case 8:
			fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
			fdPaletteFadeStartInterpolation(8);
			break;
		case 16: fdMapGridSetShake(1, 1, 1); break;

		case 24:
			center->x.fixed = toFIXED(304);
			center->y.fixed = -toFIXED(360);
			center->z.fixed = toFIXED(164);
			center->scaleX  = toFIXED(1);
			center->scaleY  = toFIXED(1);
			scVenusLighthouseAerieStopActorMotion(center);
			fdActorSetScript(ACTOR_BEACON_ORB, VenusLighthouseAerieOrbPulseScript);
			break;
		case 25:
			BeaconRiseStep--;
			if (center->y.fixed > 0)
			{
				fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 16, 12), PALETTES_ALL);
				fdPaletteFadeStartInterpolation(16);
				BeaconRiseStep++;
				fdActorGet(ACTOR_PARTY_LEADER)->sceneCount = 1;
				fdActorGet(ACTOR_GARET)->sceneCount        = 1;
				fdActorGet(ACTOR_IVAN)->sceneCount         = 1;
				fdActorGet(ACTOR_MIA)->sceneCount          = 1;
				fdActorGet(ACTOR_FELIX)->sceneCount        = 1;
				fdActorGet(ACTOR_SHEBA)->sceneCount        = 1;
			}
			else
			{
				if ((TickCount & 7) == 0)
					sySoundPlay(SE_GLITTER);
				spawnParticles   = 1;
				center->y.fixed += toFIXED(9) / 4;
			}
			break;
		case 26:
			BeaconRiseStep--;
			if (center->y.fixed > (toFIXED(40)))
			{
				fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
				fdPaletteFadeStartInterpolation(40);
				BeaconRiseStep++;
			}
			else
			{
				if ((TickCount & 7) == 0)
					sySoundPlay(SE_GLITTER);
				spawnParticles   = 1;
				center->y.fixed += toFIXED(9) / 4;
			}
			break;
		case 27:
		case 28:
		case 29:
		case 30:
		case 31:
		case 32:
		case 33:
		case 34: spawnParticles = 1; break;
		case 36:
			sySoundPlay(SE_FALLING_LIGHT);
			fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
			fdPaletteFadeStartInterpolation(12);
			break;
		case 48:
			fdActorStop(ACTOR_BEACON_ORB);
			fdScriptSetFlag(FLAG_VENUS_AERIE_PARTY_LIFTED);
			break;
	}
	if (spawnParticles)
	{
		randomHeight = toFIXED(syRandomBelow(80));
		effect       = syFieldActorCreate(SPRITE_PARTICLE,
                                    center->x.fixed,
                                    center->y.fixed - randomHeight - toFIXED(8),
                                    center->z.fixed);
		if (effect != NULL)
		{
			sprite = effect->sprite;
			fdActorStartScript(effect, VenusLighthouseAerieRiseParticleScript);
			fdActorSetSpriteColor(effect, SPRITE_COLOR_GRB);
			effect->motionFlags    = 0;
			angle                  = syRandomNext() & 0xffff000;
			effect->timer          = angle;
			effect->userValue      = 0;
			effect->sceneCount     = syRandomBelow(8);
			effect->update         = scVenusLighthouseAerieUpdateRisingOrbit;
			effect->speedLimit     = sySin(syRandomBelow(TRIG_TURN - 1) >> 4) * 24;
			effect->speedLimit     = FIXEDtoINT(center->speedLimit);
			sprite->flags          = 0;
			sprite->image.priority = 1;
		}
	}
	BeaconRiseStep++;
	scVenusLighthouseAerieAdvanceActorRise(fdActorGet(ACTOR_PARTY_LEADER));
	scVenusLighthouseAerieAdvanceActorRise(fdActorGet(ACTOR_GARET));
	scVenusLighthouseAerieAdvanceActorRise(fdActorGet(ACTOR_IVAN));
	scVenusLighthouseAerieAdvanceActorRise(fdActorGet(ACTOR_MIA));
	scVenusLighthouseAerieAdvanceActorRise(fdActorGet(ACTOR_FELIX));
	scVenusLighthouseAerieAdvanceActorRise(fdActorGet(ACTOR_SHEBA));
}

// An actor the light lifts rises this many quarter pixels, one a tick.
enum
{
	RISE_QUARTER_PIXELS = 32
};

// Lifts one of the party, Felix or Sheba off the aerie as the beacon's light takes them, a quarter
// pixel a tick up to 8 pixels.
static void scVenusLighthouseAerieAdvanceActorRise(ACTOR *actor)
{
	if (actor->sceneByte != 0)
	{
		Uint8 counter  = actor->sceneCount;

		actor->y.fixed = actor->sceneWord + toFIXED(counter >> 2);

		scVenusLighthouseAerieStopActorMotion(actor);

		if (actor->sceneCount != 0)
		{
			if (actor->sceneCount < RISE_QUARTER_PIXELS)
			{
				++actor->sceneCount;
			}
		}
	}
}

// Sparkles circle the foot of the beam of light, each on its own flat ellipse.
static void scVenusLighthouseAerieSpawnSideOrbitParticles(void)
{
	ACTOR                 *center;
	ACTOR                 *effect;
	SPR                   *sprite;
	MAP_GRID_SCROLL_STATE *grid;
	Sint32                 drift;
	Sint32                 phase;
	Sint32                 angle;

	center = fdActorGet(ACTOR_BEACON_ORB);
	grid   = MapGridState[0];
	drift  = toFIXED(syRandomBelow(48));
	if ((Sint16)(FIXEDtoINT(grid->view[1])) <= 129)
	{
		if (TickCount & 1)
		{
			fdActorSetPos(ACTOR_BEACON_ORB, 304, 164);
			fdActorGet(ACTOR_BEACON_ORB)->scaleX = toFIXED(1);
			fdActorGet(ACTOR_BEACON_ORB)->scaleY = toFIXED(1);
		}
		else
		{
			fdActorSetPos(ACTOR_BEACON_ORB, 304, 171);
			fdActorGet(ACTOR_BEACON_ORB)->scaleX = toFIXED(13) / 10;
			fdActorGet(ACTOR_BEACON_ORB)->scaleY = toFIXED(13) / 10;
		}
	}
	else
	{
		fdActorSetPos(ACTOR_BEACON_ORB, 0, 0);
	}
	if (center == NULL)
		return;
	phase = TickCount & 15;
	if (phase != 0)
		return;
	effect  = syFieldActorCreate(SPRITE_PARTICLE,
                                center->x.fixed + toFIXED(8),
                                center->y.fixed + drift + toFIXED(8),
                                center->z.fixed);
	drift  /= toFIXED(6);
	drift   = toFIXED(drift);
	if (effect == NULL)
		return;
	sprite = effect->sprite;
	fdActorStartScript(effect, BeaconSparkScript);
	fdActorSetSpriteColor(effect, SPRITE_COLOR_BRG);
	effect->motionFlags    = phase;
	angle                  = syRandomNext() & 0xffff000;
	effect->timer          = angle;
	effect->userValue      = phase;
	effect->watchedActor   = (ACTOR_OR_EFFECT *)center;
	effect->update         = scVenusLighthouseAerieUpdateOrbitingEffect;
	effect->speedLimit     = FIXEDtoINT(sySin((drift & 0xfffff) >> 4) * 24);
	sprite->flags          = 0;
	sprite->image.priority = center->sprite->image.priority;
}

// Moves a sparkle round the foot of the beam on its ellipse.
static void scVenusLighthouseAerieUpdateOrbitingEffect(ACTOR *actor)
{
	FIELD_EFFECT *effect = (FIELD_EFFECT *)actor;
	ACTOR        *anchor;
	Sint32        spin;

	anchor          = (ACTOR *)actor->watchedActor;
	spin            = effect->spin;
	effect->x       = anchor->x.fixed + syCos(spin) * (actor->speedLimit + 28);
	effect->z       = sySin(spin) * 16 + toFIXED(164);
	actor->targetX  = effect->x;
	actor->targetZ  = effect->z;
	effect->spin   -= TRIG_TURN / 128;
}

#include "shared/field_effect_duration_scripts.c"

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

// Garet, Ivan and Mia walk up to Isaac on the tower once the party has agreed to stop Saturos and
// Menardi, and step out of sight into him.
Sint32 VenusLighthouseAerieJoinLeaderScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(492, 0, 164),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

// Everyone steps half a metatile apart as the first battle begins: the party, Felix and Sheba west,
// Saturos and Menardi east.
Sint32 VenusLighthouseAerieStepBackWestScript[] = {
	SC_SPEED(0.2),
	SC_ACCELERATION(0.1),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_BY(-8, 0, 0),
	SC_SYNC,
	SC_END,
};

Sint32 VenusLighthouseAerieStepBackEastScript[] = {
	SC_SPEED(0.2),
	SC_ACCELERATION(0.1),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_BY(8, 0, 0),
	SC_SYNC,
	SC_END,
};

// Breathes heavily for as long as it runs (scVenusLighthouseAeriePulseActorScale).
Sint32 VenusLighthouseAerieBreatheHeavilyScript[] = {
	SC_CALL(scVenusLighthouseAeriePulseActorScale),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

// Widens a beam segment from a sliver.
Sint32 VenusLighthouseAerieBeamOpenScript[] = {
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_WAIT(1),
	SC_LOOP(32, SC_TOP),
	SC_END,
};

// Narrows a beam segment again as Saturos and Menardi take its power.
Sint32 VenusLighthouseAerieBeamNarrowScript[] = {
	SC_ADD(SW_SCALE_X, -toFIXED(7) / 512),
	SC_WAIT(1),
	SC_LOOP(140, SC_TOP),
	SC_END,
};

// Narrows each segment of the erupting beam, for longer than VenusLighthouseAerieBeamNarrowScript.
Sint32 VenusLighthouseAerieBeamRiseScript[] = {
	SC_ADD(SW_SCALE_X, -toFIXED(7) / 512),
	SC_WAIT(1),
	SC_LOOP(160, SC_TOP),
	SC_END,
};

// Lifts a beam segment four pixels a tick, then takes it off the stage.
Sint32 VenusLighthouseAerieBeamCloseScript[] = {
	SC_ADD(SW_Y, toFIXED(4)),
	SC_WAIT(1),
	SC_LOOP(30, SC_TOP),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

// Lifts the beam's flattened bottom two pixels a tick, then takes it off the stage.
Sint32 VenusLighthouseAerieBeamEndScript[] = {
	SC_ADD(SW_Y, toFIXED(2)),
	SC_WAIT(1),
	SC_LOOP(160, SC_TOP),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

// A sparkle round the beam over the beacon rises 16 pixels and is gone.
Sint32 VenusLighthouseAerieOrbitParticleScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(4) / 5),
	SC_SET(SW_SCALE_Y, toFIXED(4) / 5),
	SC_ANIMATION(ANIM_STAND),
	SC_LABEL(1),
	SC_ADD(SW_Y, toFIXED(1)),
	SC_WAIT(1),
	SC_LOOP(16, SC_TO(1)),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
};

// A sparkle round the rising orb sinks 16 pixels and is gone.
Sint32 VenusLighthouseAerieRiseParticleScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(4) / 5),
	SC_SET(SW_SCALE_Y, toFIXED(4) / 5),
	SC_ANIMATION(ANIM_STAND),
	SC_LABEL(1),
	SC_ADD(SW_Y, -toFIXED(1)),
	SC_WAIT(1),
	SC_LOOP(16, SC_TO(1)),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
};

// Felix, then Sheba, walk off down the aerie's stairs after the first battle.
Sint32 VenusLighthouseAerieWalkOutScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(268, 0, 186),
	SC_SYNC,
	SC_MOVE(268, 0, 252),
	SC_SYNC,
	SC_MOVE(232, 0, 222),
	SC_SYNC,
	SC_MOVE(232, 0, 136),
	SC_SYNC,
	SC_MOVE(200, 0, 136),
	SC_SYNC,
	SC_MOVE(200, 0, 200),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

// The orb rising out of the beacon narrows and widens again, four ticks each way, as long as it
// shows.
Sint32 VenusLighthouseAerieOrbPulseScript[] = {
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, -toFIXED(3) / 128),
	SC_WAIT(1),
	SC_LOOP(4, SC_TO(1)),
	SC_LABEL(2),
	SC_ADD(SW_SCALE_X, toFIXED(3) / 128),
	SC_WAIT(1),
	SC_LOOP(4, SC_TO(2)),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

// Sheba drops from the broken aerie, 90 pixels over two seconds, and is gone.
Sint32 VenusLighthouseAerieShebaFallScript[] = {
	SC_ADD(SW_Y, -(toFIXED(3) / 4)),
	SC_WAIT(1),
	SC_LOOP(120, SC_TOP),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

// Felix dives after her, much faster, and is gone.
Sint32 VenusLighthouseAerieFelixDiveScript[] = {
	SC_ADD(SW_Y, -(toFIXED(9) / 2)),
	SC_WAIT(1),
	SC_LOOP(80, SC_TOP),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

// Garet, Ivan and Mia walk off ahead of Isaac as the party leaves to find Jenna.
Sint32 VenusLighthouseAeriePartyExitScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(330, 0, 173),
	SC_SYNC,
	SC_RELEASE,
	SC_END,
};

// The party looks about them at random.
Sint32 VenusLighthouseAerieLookAboutScript[] = {
	SC_CALL(scLookAboutRandomly),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

SCENE_ENTRANCE VenusLighthouseAerieEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 304, 0, 216, FACING_SOUTH_SOUTHEAST, 0, 0, 0, 600, 420, 0},
	{VENUS_LIGHTHOUSE_AERIE_FROM_VENUS_LIGHTHOUSE_FOURTH_FLOOR,
     CONDITION_ALWAYS, 504,
     0, 168,
     FACING_SOUTH_SOUTHEAST, 0,
     0, 0,
     600, 420,
     0},
	{VENUS_LIGHTHOUSE_AERIE_AFTER_FIRST_BATTLE,
     CONDITION_ALWAYS, 340,
     0, 184,
     FACING_WEST, 0,
     0, 0,
     600, 420,
     0},
	{VENUS_LIGHTHOUSE_AERIE_AFTER_SECOND_BATTLE,
     CONDITION_ALWAYS, 340,
     0, 184,
     FACING_WEST, 0,
     0, 0,
     600, 420,
     0},
	{VENUS_LIGHTHOUSE_AERIE_AFTER_DEFEAT,
     CONDITION_ALWAYS, 0,
     0, 0,
     FACING_WEST, 0,
     0, 0,
     600, 420,
     0},
	{SCENE_TABLE_END},
};

Uint32 VenusLighthouseAerieExits[] = {
	SCENE_EXITS(VenusLighthouseAerie),
	SCENE_EXIT(VENUS_LIGHTHOUSE_AERIE_TO_WORLD_MAP, WorldMap, WORLD_MAP_BEFORE_SHEBAS_FALL),
	SCENE_EXIT(VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_C,
               TimberRoomsC,
               TIMBER_ROOMS_C_FROM_VENUS_LIGHTHOUSE_AERIE),
	SCENE_EXIT(VENUS_LIGHTHOUSE_AERIE_TO_TIMBER_ROOMS_B, TimberRoomsB, TIMBER_ROOMS_B_SANCTUM),
	SCENE_EXITS_END,
};

// clang-format off
ACTOR_PLACE VenusLighthouseAerieActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		248, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		328, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STATUE_WITH_WREATH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		360, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		440, 0, 252, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		472, 0, 252, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		504, 0, 252, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		104, 0, 252, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		136, 0, 252, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		168, 0, 252, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		504, 0, 148, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		504, 0, 148, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		504, 0, 148, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 164, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		296, 0, 180, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		248, 0, 172, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 252, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};
// clang-format on

// The floating platform the leader has landed on, 0 when none.
Sint32 VenusLighthouseAerieLandedPlatform = 0;

CUE    VenusLighthouseAerieCues[]         = {
    {CUE_STEP | STEP_CUE_FLAG_WALKING,
     METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_FLOOR_BY_PLATFORMS, CONDITION_ALWAYS,
     SCRIPT_REF(scVenusLighthouseAerieJumpToNextPlatform)},
    {CUE_STEP,
     METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_FLOOR_BY_PLATFORMS, CONDITION_ALWAYS,
     SCRIPT_REF(scVenusLighthouseAerieFloatPlatforms)},
    {CUE_STEP | STEP_CUE_FLAG_WALKING,
     METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_PLATFORM, CONDITION_ALWAYS,
     SCRIPT_REF(scVenusLighthouseAerieJumpToNextPlatform)},
    {CUE_STEP,
     METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_PLATFORM, FLAG_VENUS_AERIE_ON_PLATFORM,
     SCRIPT_REF(scVenusLighthouseAerieLandOnPlatform)},
    {CUE_STEP,
     METATILE_BEHAVIOR_VENUS_LIGHTHOUSE_AERIE_BEFORE_BEACON, CONDITION_ALWAYS,
     SCRIPT_REF(scVenusLighthouseAerieConfrontSaturosAndMenardi)},
    {CUE_TABLE_END},
};

#include "shared/beacon_spark_script.c"
