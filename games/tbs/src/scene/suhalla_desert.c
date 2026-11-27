// Suhalla Desert, the desert of sandstorms between Suhalla and Suhalla Gate, which the party
// crosses with Iodem after Babi's audience while Sheba is missing in it.
//
// Three scenes share this overlay and the area name Suhalla Desert:
//
// - The north half (SceneIdSuhallaDesertNorth), entered from the World Map
//       just south of Suhalla at the top of the map. Five sandstorms lie in
// wait: on the west path and on the east path toward the Virtuous Armlet's chest near the top, and
// three in the south, the middle one beside the chest
//       the Mimic hides in. The way on leaves by the map's south-west edge.
// - The south half (SceneIdSuhallaDesertSouth), entered at its east side.
//       A long valley ends in three ways north, each with a storm: the west way
//       to a chest with a Cookie, the east way to a chest with a Lucky Medal.
//       Below lie two cave mouths: by the west one Flash, the Mars Djinni, is
//       reached over a pit with a pillar Reveal shows, and the east one opens
//       into a hollow where a great red storm turns. Across the wide plain
//       south, past a half-buried stone head, a storm gives chase, and near the
//       way out in the south-east Iodem asks whether to search on for Sheba
//       (scSuhallaDesertIodemAsksWhetherToPressOn,
//       FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA).
// - The caves under the south half (SceneIdSuhallaDesertCaves), which join
//       its west cave mouth to the hollow of the red storm. They place nothing.
//
// One exit table and, for the two halves, one cue table serve the overlay. The exits in that cue
// table keep their numbers, which each half's exit rows lead its own way.
//
// The storms, ACTOR_FIRST_SANDSTORM to ACTOR_LAST_SANDSTORM in both halves, start shrunk to nothing
// in the sand. Stepping next to one (the metatiles north and south of it) makes a white whirlwind
// rise there with a roar (scSuhallaDesertRaiseStorm); the first time, blowing sand fills the air.
// That haze is BG1 blended over the rest (SUHALLA_DESERT_HAZE_BLEND). The first storm to rise while
// Iodem travels with the party brings him out to warn of them.
//
// Walking into a storm draws the leader to its centre and lifts the party, spinning, for
// SANDSTORM_CAPTURE_TICKS (scSuhallaDesertCatchInStorm).
//
// Douse in that time stops the storm, and the monster in it attacks: a Tornado Lizard, or the
// Tempest Lizard in the red storm and the Storm Lizard in the storm that gives chase
// (scSuhallaDesertDouseStorm). Otherwise the storm carries the party up out of sight and drops it
// on the World Map outside Suhalla (scSuhallaDesertStormCarriesPartyAway), which is why everyone
// the storms catch winds up there; the red storm drops it far off, near Crossbone Isle.
//
// A doused storm's flag is an area flag, five from FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED
// and five from FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED, so the storm stays gone until the
// party leaves the desert's area. The chasing storm, once doused, is gone for good
// (FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_DOUSED).
//
// Its scene flags:
//
// - FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN to
//       FLAG_SUHALLA_DESERT_FIFTH_SANDSTORM_RISEN: storms ACTOR_FIRST_SANDSTORM
//       to ACTOR_LAST_SANDSTORM have risen on this visit.
// - FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_SET_OFF: the chasing storm has set
//       off.
// - FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM: a storm has caught the leader.
// - FLAG_SUHALLA_DESERT_CAPTURE_TICKS_LEFT: a byte, the ticks left before
//       the storm carries the party away.
// - FLAG_SUHALLA_DESERT_CATCHING_SANDSTORM: a byte, the storm that caught the
//       leader.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field_psynergy_state.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "scene/field_effect_spawn.h"
#include "scene/walk_into_leader.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE SuhallaDesertEntrancesOther[];
extern SCENE_ENTRANCE SuhallaDesertNorthEntrances[];
extern SCENE_ENTRANCE SuhallaDesertSouthEntrances[];
extern SCENE_ENTRANCE SuhallaDesertCavesEntrances[];
extern Uint32         SuhallaDesertExits[];
extern ACTOR_PLACE    SuhallaDesertActorPlacementsOther[];
extern ACTOR_PLACE    SuhallaDesertNorthActorPlacements[];
extern ACTOR_PLACE    SuhallaDesertSouthActorPlacements[];
extern ACTOR_PLACE    SuhallaDesertCavesActorPlacements[];
extern Sint32         SuhallaDesertChaseScript[];
extern CUE            SuhallaDesertCuesOther[];
extern CUE            SuhallaDesertCavesCues[];

// Storms doused in the north and south halves, one flag per storm from the first.
//
// Each storm stays away once doused.
enum
{
	// The north half's storms, five from this one.
	FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED  = FLAG_AREA_TEMP_01,
	// The south half's storms, five from this one.
	FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED = FLAG_AREA_TEMP_11,
	// A storm has risen, so the sand haze stays faded in.
	FLAG_SUHALLA_DESERT_SANDSTORMS_RISEN            = FLAG_AREA_TEMP_40,
	// The Psynergy Stone has been found, an area flag.
	FLAG_SUHALLA_DESERT_PSYNERGY_STONE              = FLAG_AREA_TEMP_51
};

// The actors, numbering the actor placement rows of both halves of the desert from
// ACTOR_FIRST_PLACED.
//
// The caves place none.
enum
{
	// The storms, five in each half.
	ACTOR_FIRST_SANDSTORM   = ACTOR_FIRST_PLACED,
	// In the south half, the great red storm in the hollow by the east cave mouth, which holds the
	// Tempest Lizard.
	ACTOR_FOURTH_SANDSTORM  = ACTOR_FIRST_SANDSTORM + 3,
	ACTOR_LAST_SANDSTORM    = ACTOR_FIRST_SANDSTORM + 4,
	// In the south half the last storm waits off the map until it gives chase across the plain. It
	// holds the Storm Lizard.
	ACTOR_CHASING_SANDSTORM = ACTOR_LAST_SANDSTORM,
	// Iodem, who steps out of the party to talk and then rejoins it.
	ACTOR_IODEM,
	// In the south half, the stone pillar Reveal shows in the pit east of Flash, drawn as Goma
	// Cave's stone pillars.
	ACTOR_REVEALED_PILLAR,
	// In the south half, Flash, the Mars Djinni, by the west cave mouth.
	ACTOR_FLASH
};

// The metatile behaviors of the desert's step cues.
typedef enum SUHALLA_DESERT_METATILE_BEHAVIOR
{
	// Near the south half's way out, where Iodem asks whether to press on.
	METATILE_BEHAVIOR_SUHALLA_DESERT_IODEM_ASKS       = 9,
	// A storm's own metatile in the north half, one behavior per storm from the first.
	METATILE_BEHAVIOR_SUHALLA_DESERT_NORTH_STORM      = 41,
	// A storm's own metatiles in the south half, one behavior per storm from the first; the fourth
	// is the red storm's.
	METATILE_BEHAVIOR_SUHALLA_DESERT_SOUTH_STORM      = 51,
	// The metatiles north and south of a storm in the north half, which raise it.
	METATILE_BEHAVIOR_SUHALLA_DESERT_NEAR_NORTH_STORM = 61,
	// The metatiles north and south of a storm in the south half, which raise it.
	METATILE_BEHAVIOR_SUHALLA_DESERT_NEAR_SOUTH_STORM = 71,
	// The metatiles across the south half's plain that set a storm chasing.
	METATILE_BEHAVIOR_SUHALLA_DESERT_STORM_CHASES     = 81
} SUHALLA_DESERT_METATILE_BEHAVIOR;

// The monster a doused storm lets loose, by its slot of ENCOUNTER_ZONE_SCRIPTED.
enum
{
	ENCOUNTER_SLOT_TORNADO_LIZARD = 5,
	ENCOUNTER_SLOT_STORM_LIZARD   = 6, // in the storm that gives chase
	ENCOUNTER_SLOT_TEMPEST_LIZARD = 7  // in the great red storm
};

// Ticks a caught party spins before the storm carries it away.
enum
{
	SANDSTORM_CAPTURE_TICKS = 180
};

// The treasure spot of the Psynergy Stone hidden in the south half, at metatile (12, 71) on the
// plain, whose search row finds the stone (FIND_PSYNERGY_STONE).
//
// Its actor stands in its metatile only while Reveal lasts (scSuhallaDesertRevealStarts moves it to
// the metatile's centre). It is moved off the map as Reveal ends.
enum
{
	TREASURE_SPOT_PSYNERGY_STONE = 100
};

// The scripts the storms raise.
//
// Either the spinning has run out, or the chasing storm has reached the moving leader.
enum
{
	RAISED_CHASING_SANDSTORM_CATCHES    = 55,
	RAISED_SANDSTORM_CARRIES_PARTY_AWAY = 99
};

// The sand haze, which is BG1 blended over every layer.
//
// Its weight rises from 0 to BLEND_WEIGHT_MAX as the first storm rises.
#define SUHALLA_DESERT_HAZE_BLEND (BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

static void   scSuhallaDesertSetUpFirstMapStorms(void);
static void   scSuhallaDesertSetUpSecondMapStorms(void);

// The colour Reveal pulses, as it was before Reveal, put back as it ends.
static Uint16 SuhallaDesertSavedColor;

// The colour Reveal pulses, the last of BG palette 12.
#define SUHALLA_REVEAL_PALETTE_COLOR BG_PLTT_COLOR(12, 15)

// Lalivero's area, whose FLAG_AREA_VISITED says the party has been there (MapScreenAreaMarkers
// marks it at SCENE_LALIVERO).
#define AREA_LALIVERO                55

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

// The first sandstorm (ACTOR_FIRST_SANDSTORM) has risen on this visit.
//
// The four flags after it are the next four sandstorms'.
#define FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN     FLAG_SCENE_TEMP_06
// The second sandstorm has risen on this visit.
#define FLAG_SUHALLA_DESERT_SECOND_SANDSTORM_RISEN    FLAG_SCENE_TEMP_07
// The third sandstorm has risen on this visit.
#define FLAG_SUHALLA_DESERT_THIRD_SANDSTORM_RISEN     FLAG_SCENE_TEMP_08
// The fourth sandstorm (ACTOR_FOURTH_SANDSTORM) has risen on this visit.
#define FLAG_SUHALLA_DESERT_FOURTH_SANDSTORM_RISEN    FLAG_SCENE_TEMP_09
// The fifth sandstorm (ACTOR_LAST_SANDSTORM) has risen on this visit.
#define FLAG_SUHALLA_DESERT_FIFTH_SANDSTORM_RISEN     FLAG_SCENE_TEMP_0A
// In the south half, the sandstorm that gives chase has set off after the leader.
#define FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_SET_OFF FLAG_SCENE_TEMP_0E
// A sandstorm has caught the leader and lifts the party, and Douse can put it out.
#define FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM       FLAG_SCENE_TEMP_0F
// A byte flag that holds the ticks left before the sandstorm that caught the leader carries the
// party away.
#define FLAG_SUHALLA_DESERT_CAPTURE_TICKS_LEFT        FLAG_SCENE_TEMP_10
// A byte flag that holds the actor id of the sandstorm that caught the leader.
#define FLAG_SUHALLA_DESERT_CATCHING_SANDSTORM        FLAG_SCENE_TEMP_18

// Talking to Flash, the Mars Djinni by the west cave mouth in the south half of Suhalla Desert
// (metatile (4, 43)), starts its battle.
//
// The party reaches it by hopping across the pit on the pillar Reveal shows.
static void scSuhallaDesertFightFlash(void)
{
	fdRequestDjinnBattle(ACTOR_FLASH, DJINNI_FLASH);
}

// The first step of a storm's actor script: the storm casts no shadow and nothing bumps into it.
static Sint32 scSuhallaDesertClearActorSpriteAndCollisionFlags(ACTOR *actor)
{
	fdActorSetSpriteFlags(actor, 0);
	actor->collisionFlags = 0;
	return 0;
}

// The rest of a storm's actor script, run every tick: the storm's sprite is drawn one tick and not
// the next (ACTOR_SPRITE_ONE and ACTOR_SPRITE_NONE), so the whirlwind flickers, half there.
static Sint32 scSuhallaDesertFlickerStorm(ACTOR *actor)
{
	actor->spriteStorageKind ^= 1;
	return 1;
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if (scene == SCENE_SUHALLA_DESERT_NORTH)
	{
		return SuhallaDesertNorthEntrances;
	}
	if (scene == SCENE_SUHALLA_DESERT_SOUTH)
	{
		return SuhallaDesertSouthEntrances;
	}
	if (scene == SCENE_SUHALLA_DESERT_CAVES)
	{
		return SuhallaDesertCavesEntrances;
	}
	return SuhallaDesertEntrancesOther;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

// Where each exit of the three scenes leads.
const Uint32 *scGetExits(void)
{
	return SuhallaDesertExits;
}

// The storms, Iodem, and in the south half Flash and the pillar Reveal shows.
const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if (scene == SCENE_SUHALLA_DESERT_NORTH)
	{
		return SuhallaDesertNorthActorPlacements;
	}
	if (scene == SCENE_SUHALLA_DESERT_SOUTH)
	{
		return SuhallaDesertSouthActorPlacements;
	}
	if (scene == SCENE_SUHALLA_DESERT_CAVES)
	{
		return SuhallaDesertCavesActorPlacements;
	}
	return SuhallaDesertActorPlacementsOther;
}

// Each tick while caught, spins the leader round and counts down to the storm carrying the party
// away (RAISED_SANDSTORM_CARRIES_PARTY_AWAY). The count stands still while the menu is open.
static void scSuhallaDesertUpdateCaptureTimer(void)
{
	ACTOR       *leader;
	FIELD_STATE *state;
	Sint32       remainingTicks;

	leader         = fdActorGet(MainRoutineStateWork.leaderActorId);
	state          = FieldStateWork;
	leader->facing = TickCount * FACING_STEP;

	remainingTicks = syFieldGameFlagGetByte(FLAG_SUHALLA_DESERT_CAPTURE_TICKS_LEFT);
	if (remainingTicks != 0)
	{
		if (remainingTicks == 1)
		{
			state->raisedCue = RAISED_SANDSTORM_CARRIES_PARTY_AWAY;
		}
		else if (gmFlagGet(FLAG_MENU_OPEN) == 0)
		{
			remainingTicks -= 1;
		}
	}
	syFieldGameFlagSetByte(FLAG_SUHALLA_DESERT_CAPTURE_TICKS_LEFT, remainingTicks);
}

// The leader walks into a storm: he is drawn to its centre, looking about, and lifted spinning into
// the air with the roar of the wind.
//
// Until the count runs out, Douse can still stop it.
static void scSuhallaDesertCatchInStorm(Sint32 stormId)
{
	Sint32 leaderId;
	ACTOR *leader;
	ACTOR *storm;

	leaderId = MainRoutineStateWork.leaderActorId;
	leader   = fdActorGet(leaderId);
	// DEAD: the storm looked up and dropped; the ROM keeps its call.
	fdActorGet(stormId);
	if (fdScriptGetFlag(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM) == 0)
	{
		fdScriptStart();
		// The leader is drawn to the storm's centre, then lifted.
		fdActorSetAttachedEffect(leaderId, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
		fdActorSetAnimation(leaderId, ANIM_LEADER_LOOK_AROUND);
		storm = fdActorGet(stormId);
		if (storm != NULL)
		{
			fdActorSetDestination(leaderId, storm->x.part[0].integer, storm->z.part[0].integer);
		}
		fdActorWaitForMoveById(leaderId);
		sySoundPlay(SE_SANDSTORM);
		sySetTickEvent(scSuhallaDesertUpdateCaptureTimer, EVENT_PASS_TICK);
		leader->motionFlags = 0;
		syFieldActorSetMoveTargetFX(
			leader, leader->x.fixed, leader->y.fixed + toFIXED(32), leader->z.fixed);
		fdActorWaitForMoveById(leaderId);
		leader->velocityY                 = 0;
		leader->motionFlags               = ACTOR_FLOATS;
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_AIRBORNE;
		gmFlagSet(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM);
		syFieldGameFlagSetByte(FLAG_SUHALLA_DESERT_CATCHING_SANDSTORM, stormId);
		syFieldGameFlagSetByte(FLAG_SUHALLA_DESERT_CAPTURE_TICKS_LEFT, SANDSTORM_CAPTURE_TICKS);
		fdScriptEnd();
		FieldStateWork->encounter = 0; // drop any battle this step rolled
	}
}

// Walking into the sandstorm on the west path near the top of Suhalla Desert's north half (metatile
// (3, 16)), or the one in the west way north out of the south half's long valley toward the
// Cookie's chest (metatile (13, 20)), gets the party caught up in it.
static void scSuhallaDesertCaughtByFirstStorm(void)
{
	scSuhallaDesertCatchInStorm(ACTOR_FIRST_SANDSTORM);
}

// Walking into the sandstorm on the east path near the top of Suhalla Desert's north half, toward
// the Virtuous Armlet's chest (metatile (28, 16)), or the one in the middle way north out of the
// south half's long valley (metatile (18, 20)), gets the party caught up in it.
static void scSuhallaDesertCaughtBySecondStorm(void)
{
	scSuhallaDesertCatchInStorm(ACTOR_FIRST_SANDSTORM + 1);
}

// Walking into the sandstorm in the south-west of Suhalla Desert's north half (metatile (7, 43)),
// or the one in the east way north out of the south half's long valley toward the Lucky Medal's
// chest (metatile (25, 20)), gets the party caught up in it.
static void scSuhallaDesertCaughtByThirdStorm(void)
{
	scSuhallaDesertCatchInStorm(ACTOR_FIRST_SANDSTORM + 2);
}

// Walking into the sandstorm beside the Mimic's chest in the south of Suhalla Desert's north half
// (metatile (16, 43)), or into the great red storm in the hollow by the south half's east cave
// mouth (metatiles (24, 37) to (24, 40)), gets the party caught up in it.
static void scSuhallaDesertCaughtByFourthStorm(void)
{
	scSuhallaDesertCatchInStorm(ACTOR_FIRST_SANDSTORM + 3);
}

// Walking into the sandstorm in the south-east of Suhalla Desert's north half (metatile (27, 45))
// gets the party caught up in it, and so does the storm chasing the party across the south half's
// plain when it reaches a leader on the move (RAISED_CHASING_SANDSTORM_CATCHES).
static void scSuhallaDesertCaughtByFifthStorm(void)
{
	scSuhallaDesertCatchInStorm(ACTOR_FIRST_SANDSTORM + 4);
}

// Stepping next to a storm makes a white whirlwind rise out of the sand with a roar, growing to
// full size in about a quarter of a second, and the first time the air fills with blowing sand.
//
// The first time a storm rises while Iodem travels with the party, he steps out, cries "A
// sandstorm!", and warns that they must step carefully.
static void scSuhallaDesertRaiseStorm(Sint32 stormId)
{
	Sint32 frame;
	Sint32 weight;
	Sint32 leaderX;
	Sint32 leaderZ;
	ACTOR *storm;
	Sint32 hazeShown;
	ACTOR *leader;
	ACTOR *partyLeader;

	leader    = fdActorGet(MainRoutineStateWork.leaderActorId);
	storm     = fdActorGet(stormId);
	hazeShown = gmFlagGet(FLAG_SUHALLA_DESERT_SANDSTORMS_RISEN);
	leaderX   = leader->x.part[0].integer;
	leaderZ   = leader->z.part[0].integer;
	fdScriptStart();
	sySoundPlay(SE_SANDSTORM);
	for (frame = 0; frame < 16; frame++)
	{
		storm->scaleX = frame * (toFIXED(1) / 32) + toFIXED(1) / 32;
		storm->scaleY = frame * (toFIXED(1) / 16) + toFIXED(1) / 16;
		if (hazeShown == 0)
		{
			weight = frame + 1;
			IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA,
			                    BLEND_ALPHA(weight, BLEND_WEIGHT_MAX - weight),
			                    IO_WRITE_HALFWORD);
		}
		sySynch(1);
	}
	gmFlagSet(FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN - ACTOR_FIRST_SANDSTORM + stormId);
	gmFlagSet(FLAG_SUHALLA_DESERT_SANDSTORMS_RISEN);
	if (gmFlagGet(FLAG_IODEM_FOLLOWING_PARTY) != 0
	    && fdScriptGetFlag(FLAG_SUHALLA_DESERT_IODEM_WARNED) == 0)
	{
		gmFlagSet(FLAG_SUHALLA_DESERT_IODEM_WARNED);
		fdActorSetSpeed(ACTOR_IODEM, 1);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
		partyLeader = fdActorGet(ACTOR_PARTY_LEADER);
		if (partyLeader != NULL)
		{
			fdActorSetPosFX(ACTOR_IODEM, partyLeader->x.fixed, partyLeader->z.fixed);
		}
#if FIX_FACING_PASSED_AS_ACTOR_ID
		fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
#else
		// BUG: a heading where fdActorFaceActor takes an actor id.
		fdActorFaceActor(ACTOR_IODEM, FACING_SOUTH, 0);
#endif
		fdActorGet(ACTOR_IODEM)->steeringFlags &= ~ACTOR_FACES_MOTION;
		fdActorWalkTo(ACTOR_IODEM, leaderX, leaderZ - 16);
		fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, leaderX + 8, leaderZ - 40);
		fdScriptWait(1);
		fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
		fdActorSetAnimation(ACTOR_IODEM, ANIM_STAND);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
		fdScriptSetMessageId(MsgSuhallaSandstorm);
		fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
		// "A sandstorm!"
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
		// "There'll be trouble if we get caught up in one of those..."
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
		fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
		// "But something is wrong... Why are there so many?"
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH, 30);
		fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
		// "If we don't get past here, we'll never make it to Babi..."
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		// "We must step carefully if we hope to escape the desert."
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdActorSetSpeed(ACTOR_IODEM, 1);
		scWalkIntoLeader(ACTOR_IODEM);
	}
	fdScriptEnd();
}

// Stepping next to the hidden storm on the west path near the top of Suhalla Desert's north half
// (metatiles (3, 15) and (3, 17)), or the one in the west way out of the south half's long valley
// (metatiles (13, 19) and (13, 21)), makes it rise.
static void scSuhallaDesertFirstStormRises(void)
{
	scSuhallaDesertRaiseStorm(ACTOR_FIRST_SANDSTORM);
}

// Stepping next to the hidden storm on the east path near the top of Suhalla Desert's north half
// (metatiles (28, 15) and (28, 17)), or the one in the middle way out of the south half's long
// valley (metatiles (18, 19) and (18, 21)), makes it rise.
static void scSuhallaDesertSecondStormRises(void)
{
	scSuhallaDesertRaiseStorm(ACTOR_FIRST_SANDSTORM + 1);
}

// Stepping next to the hidden storm in the south-west of Suhalla Desert's north half (metatiles (7,
// 42) and (7, 44)), or the one in the east way out of the south half's long valley (metatiles (25,
// 19) and (25, 21)), makes it rise.
static void scSuhallaDesertThirdStormRises(void)
{
	scSuhallaDesertRaiseStorm(ACTOR_FIRST_SANDSTORM + 2);
}

// Stepping next to the hidden storm beside the Mimic's chest in the south of Suhalla Desert's north
// half (metatiles (16, 42) and (16, 44)) makes it rise.
static void scSuhallaDesertFourthStormRises(void)
{
	scSuhallaDesertRaiseStorm(ACTOR_FIRST_SANDSTORM + 3);
}

// Stepping next to the hidden storm in the south-east of Suhalla Desert's north half (metatiles
// (27, 44) and (27, 46)) makes it rise.
static void scSuhallaDesertFifthStormRises(void)
{
	scSuhallaDesertRaiseStorm(ACTOR_FIRST_SANDSTORM + 4);
}

// Near the way out of Suhalla Desert's south half, in its south-east corner (metatile (27, 79)),
// Iodem steps out of the party: Sheba is nowhere to be found, should they search the desert once
// more or press on to Lalivero? Either way he decides to head for the guard gate ahead first.
//
// It plays once, while Iodem travels with the party, after he has heard the wounded soldiers in
// Suhalla and before the party reaches Lalivero.
static void scSuhallaDesertIodemAsksWhetherToPressOn(void)
{
	ACTOR *leader;

	if (gmFlagGet(FLAG_IODEM_FOLLOWING_PARTY) != 0
	    && gmFlagGet(FLAG_AREA_VISITED(AREA_LALIVERO)) == 0
	    && gmFlagGet(FLAG_IODEM_MET_WOUNDED_SOLDIERS) != 0)
	{
		gmFlagSet(FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA);
		fdScriptStart();
		fdScriptSetMessageId(MsgSuhallaNowhereFound);
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		if (leader != NULL)
		{
			fdActorSetPosFX(ACTOR_IODEM, leader->x.fixed, leader->z.fixed);
		}
#if FIX_FACING_PASSED_AS_ACTOR_ID
		fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
#else
		// BUG: a heading where fdActorFaceActor takes an actor id.
		fdActorFaceActor(ACTOR_IODEM, FACING_NORTH, 0);
#endif
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 440, 1256);
		fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 444, 1240);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 30);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
		// "Sheba is nowhere to be found."
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
		fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
		// "I wonder what happened..."
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(30);
		fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
		// "Should we search the desert once more or press on to Lalivero?"
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH, 30);
		// "What do you say, Isaac? Should we keep going?" Yes: "Ah, yes. I had forgotten about your
		// foes..." No: "But I must carry out Babi's orders..."
		fdScriptAskYesNo(ACTOR_IODEM, 0);
		fdScriptWait(30);
		fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
		// "Our guard gate lies ahead."
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		// "We can leave Sheba to the soldiers at the gate."
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdActorSetSpeed(ACTOR_IODEM, 0.7);
		fdActorWalkToAndWait(ACTOR_IODEM, 440, 1256);
		// "Well, perhaps we should head there first, then."
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		scWalkIntoLeader(ACTOR_IODEM);
		fdScriptEnd();
	}
}

// How far apart two coordinates are.
#define GAP(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

// The chasing storm's script: it speeds after the leader, throwing up sand, and once it reaches a
// leader on the move it catches him.
//
// It holds still while the field handles another script.
static Sint32 scSuhallaDesertChaseLeader(ACTOR *actor)
{
	EFFECT_OPTIONS options;
	ACTOR         *leader;
	Sint32         sparkFrame;

	leader              = fdActorGet(MainRoutineStateWork.leaderActorId);
	actor->acceleration = toFIXED(1) / 4;
	actor->speedLimit   = toFIXED(3) / 2;
	actor->motionFlags  = 0;
	fdActorSetSpriteFlags(actor, 0);
	// Drawn every other tick, as scSuhallaDesertFlickerStorm draws the others.
	actor->spriteStorageKind ^= 1;
	if (gmFlagGet(FLAG_FIELD_HANDLING_REQUESTS))
	{
		actor->targetX = ACTOR_NO_TARGET;
		actor->targetY = ACTOR_NO_TARGET;
		actor->targetZ = ACTOR_NO_TARGET;
	}
	else
	{
		actor->targetX = leader->x.fixed;
		actor->targetY = leader->groundHeight;
		actor->targetZ = leader->z.fixed;
		if (GAP(actor->x.fixed, leader->x.fixed) + GAP(actor->z.fixed, leader->z.fixed)
		    < toFIXED(8))
		{
			FIELD_STATE *state = FieldStateWork;

			if (leader->motionFlags != 0)
			{
				state->raisedCue = RAISED_CHASING_SANDSTORM_CATCHES;
			}
			actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			actor->targetX     = leader->x.fixed;
			actor->targetY     = leader->y.fixed;
			actor->targetZ     = leader->z.fixed;
		}
	}
	// Sand every eight ticks, with a random spin.
	sparkFrame = TickCount & 7;
	if (sparkFrame == 0)
	{
		options.startScaleX = toFIXED(4) / 5;
		options.startScaleY = toFIXED(4) / 5;
		options.spin        = FIELD_EFFECT_RANDOM_SPIN();
		fdEffectSpawn(actor->x.fixed,
		              actor->y.fixed,
		              actor->z.fixed,
		              0,
		              sparkFrame,
		              sparkFrame,
		              EFFECT_USE_SPIN | EFFECT_USE_START_SCALE | EFFECT_SCRIPT_MEDIUM,
		              &options);
	}
	return 1;
}

// Crossing the wide plain in the south of Suhalla Desert's south half (metatiles (18, 48) to (18,
// 52), (22, 51) to (22, 52), or (14, 62) to (21, 62)) sets a storm chasing the party from beside
// the half-buried stone head (metatile (7, 51)), until it has been doused.
static void scSuhallaDesertStormGivesChase(void)
{
	if (gmFlagGet(FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_DOUSED) == 0)
	{
		gmFlagSet(FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_SET_OFF);
		fdActorSetPos(ACTOR_CHASING_SANDSTORM, 120, 824);
		fdActorSetScript(ACTOR_CHASING_SANDSTORM, SuhallaDesertChaseScript);
	}
}

// Casting Douse while a sandstorm in Suhalla Desert holds the party makes the rain fall on the
// storm from above.
static void scSuhallaDesertDouseCastAtStorm(void)
{
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;

	effectState->sceneVariant       = 1;
}

// Douse on a sandstorm in Suhalla Desert that holds the party stops it, and the monster inside
// attacks: a Tornado Lizard, or the Tempest Lizard in the red storm by the south half's east cave
// mouth, or the Storm Lizard in the storm that chased the party across the plain.
//
// The storm stays gone until the party leaves the desert; the chasing storm is gone for good.
static void scSuhallaDesertDouseStorm(Sint32 unused, Sint32 stormId)
{
	// The storm no longer carries the party away.
	if (FieldStateWork->raisedCue == RAISED_SANDSTORM_CARRIES_PARTY_AWAY)
	{
		FieldStateWork->raisedCue = 0;
	}
	gmFlagClear(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM);
	if (MainRoutineStateWork.next == SCENE_SUHALLA_DESERT_NORTH)
	{
		gmFlagSet(
			(stormId + FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED - ACTOR_FIRST_SANDSTORM));
	}
	else
	{
		if (MainRoutineStateWork.next == SCENE_SUHALLA_DESERT_SOUTH)
		{
			gmFlagSet((stormId + FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED
			           - ACTOR_FIRST_SANDSTORM));
		}
	}
	syFieldGameFlagSetByte(FLAG_SUHALLA_DESERT_CAPTURE_TICKS_LEFT, 0);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, ENCOUNTER_SLOT_TORNADO_LIZARD);
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	if (MainRoutineStateWork.next == SCENE_SUHALLA_DESERT_SOUTH)
	{
		if (stormId == ACTOR_FOURTH_SANDSTORM)
		{
			fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED,
			                                  ENCOUNTER_SLOT_TEMPEST_LIZARD);
		}
		else
		{
			if (stormId == ACTOR_CHASING_SANDSTORM)
			{
				fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED,
				                                  ENCOUNTER_SLOT_STORM_LIZARD);
				fdActorStop(ACTOR_CHASING_SANDSTORM);
				fdActorSetPos(ACTOR_CHASING_SANDSTORM, 0, 0);
			}
		}
	}
	fdActorGet(MainRoutineStateWork.leaderActorId)->motionFlags =
		ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
}

// When a sandstorm in Suhalla Desert has held the party for three seconds without being doused, it
// rises with them faster and faster out of sight, the screen closes, and the party drops onto the
// World Map outside Suhalla.
//
// The red storm by the south half's east cave mouth drops them far off instead, near Crossbone Isle
// beside the World Map's own storm (WORLD_MAP_FROM_SUHALLA_DESERT_SANDSTORM).
static void scSuhallaDesertStormCarriesPartyAway(void)
{
	Sint32 stormId;
	ACTOR *leader;
	ACTOR *storm;
	Sint32 frame;

	stormId = syFieldGameFlagGetByte(FLAG_SUHALLA_DESERT_CATCHING_SANDSTORM);
	leader  = fdActorGet(MainRoutineStateWork.leaderActorId);
	storm   = fdActorGet(stormId);
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySoundPlay(SE_GUSH);
	// BUG: this passes an actor id where the sprite helper needs a pointer.
#if FIX_SANDSTORM_LEADER_SHADOW
	fdActorSetSpriteFlags(leader, 0);
#else
	fdActorSetSpriteFlags((ACTOR *)MainRoutineStateWork.leaderActorId, 0);
#endif
	storm->motionFlags  = 0;
	leader->motionFlags = 0;
	leader->velocityY   = 0;
	leader->frozen      = 1;
	storm->frozen       = 1;
	// Both rise faster and faster for a second.
	for (frame = 0; frame < 60; frame++)
	{
		leader->velocityY += toFIXED(1) / 5;
		storm->velocityY  += toFIXED(1) / 5;
		sySynch(1);
	}
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
	gmFlagSet(FLAG_ARRIVE_FALLING);
	if (MainRoutineStateWork.next == SCENE_SUHALLA_DESERT_SOUTH
	    && syFieldGameFlagGetByte(FLAG_SUHALLA_DESERT_CATCHING_SANDSTORM) == ACTOR_FOURTH_SANDSTORM)
	{
		fdScriptSetDestination(SCENE_WORLD_MAP, WORLD_MAP_FROM_SUHALLA_DESERT_SANDSTORM);
	}
	else
	{
		fdScriptSetDestination(SCENE_WORLD_MAP, WORLD_MAP_FROM_SUHALLA);
	}
}

// While Reveal lasts, one colour of the desert's picture pulses grey, brightening and dimming about
// once a second.
static void scSuhallaDesertPulseRevealPalette(void)
{
	Uint16 brightnessFrame = TickCount & 63;
	Uint16 grey;

	if (brightnessFrame > 31)
		brightnessFrame = 64 - brightnessFrame;
	grey = (brightnessFrame >> 1) + 7;
	grey = RGB(grey, grey, grey);
	IO_WRITE16(grey, &SUHALLA_REVEAL_PALETTE_COLOR);
}

#if FIX_FUNCTION_SIGNATURES
// When Reveal wears off in Suhalla Desert's south half, the stone pillar in the pit east of Flash
// (metatile (15, 44)) disappears again, and the Psynergy Stone on the plain (metatile (12, 71)) can
// no longer be found.
//
// The pillar's metatile takes back the attributes of the metatile east of it, and the stone's
// metatile is stashed away at (127, 127). The scene runs it on entry too, so both start hidden.
static void scSuhallaDesertRevealEnds(void)
{
#else
// When Reveal wears off in Suhalla Desert's south half, the stone pillar in the pit east of Flash
// (metatile (15, 44)) disappears again, and the Psynergy Stone on the plain (metatile (12, 71)) can
// no longer be found.
//
// The pillar's metatile takes back the attributes of the metatile east of it, and the stone's
// metatile is stashed away at (127, 127). The scene runs it on entry too, so both start hidden.
static Sint32 scSuhallaDesertRevealEnds(void)
{
	Sint32 removalResult;
#endif
	if (MainRoutineStateWork.next == SCENE_SUHALLA_DESERT_SOUTH)
	{
		fdActorGet(ACTOR_REVEALED_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		fdActorGet(ACTOR_REVEALED_PILLAR)->motionFlags   = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		fdActorSetPos(ACTOR_REVEALED_PILLAR, 0, 0);
		fdMapGridCopyMetatileAttributes(16, 44, 1, 1, 15, 44);
		fdTreasureSpotSetPos(TREASURE_SPOT_PSYNERGY_STONE, 0, 0);
		fdMapGridCopyMetatileAttributes(12, 71, 1, 1, 127, 127);
		fdMapGridCopyMetatileAttributes(11, 71, 1, 1, 12, 71);
#if FIX_FUNCTION_SIGNATURES
		syCloseEvent(scSuhallaDesertPulseRevealPalette);
#else
		removalResult = syCloseEvent(scSuhallaDesertPulseRevealPalette);
#endif
		IO_WRITE16(SuhallaDesertSavedColor, &SUHALLA_REVEAL_PALETTE_COLOR);
#if !FIX_FUNCTION_SIGNATURES
		return removalResult;
#endif
	}
}

// Reveal in Suhalla Desert's south half shows a stone pillar standing in the pit east of Flash
// (metatile (15, 44)), to hop across while Reveal lasts, and lets the party find a Psynergy Stone
// hidden on the plain (metatile (12, 71)).
//
// The pillar's metatile takes the attributes of a spare metatile off the drawn map (31, 95), and
// the stone's metatile comes back from its stash.
static UNREAD_RESULT scSuhallaDesertRevealStarts(void)
{
	if (MainRoutineStateWork.next == SCENE_SUHALLA_DESERT_SOUTH)
	{
		fdActorGet(ACTOR_REVEALED_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		fdActorGet(ACTOR_REVEALED_PILLAR)->motionFlags   = 0;
		fdActorSetPos(ACTOR_REVEALED_PILLAR, 248, 712);
		fdMapGridCopyMetatileAttributes(31, 95, 1, 1, 15, 44);
		fdTreasureSpotSetPos(TREASURE_SPOT_PSYNERGY_STONE, -1, -1);
		fdTreasureSpotMoveLeaderAway();
		fdMapGridCopyMetatileAttributes(127, 127, 1, 1, 12, 71);
#if FIX_FUNCTION_SIGNATURES
		sySetTickEvent(scSuhallaDesertPulseRevealPalette, EVENT_PASS_TICK);
#else
		return sySetTickEvent(scSuhallaDesertPulseRevealPalette, EVENT_PASS_TICK);
#endif
	}
}

// The caves' exits, or the storms, Iodem, treasure and Reveal of the two halves.
const CUE *scGetCues(void)
{
	if (MainRoutineStateWork.next == SCENE_SUHALLA_DESERT_CAVES)
	{
		return SuhallaDesertCavesCues;
	}
	return SuhallaDesertCuesOther;
}

// Sets a part of the desert up as the party enters: a party still held by a storm (after the menu)
// goes on spinning, what Reveal shows is hidden, the storms that are left start shrunk into the
// sand, and the blowing sand is shown only if a storm has already risen.
Sint32 scInit(void)
{
	MAP_GRID_SCROLL_STATE *grid;

	grid                       = MapGridState[0];
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	if (syFieldGameFlagGetByte(FLAG_SUHALLA_DESERT_CAPTURE_TICKS_LEFT) != 0)
	{
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_AIRBORNE;
		sySetTickEvent(scSuhallaDesertUpdateCaptureTimer, EVENT_PASS_TICK);
	}
	if (MainRoutineStateWork.next == SCENE_SUHALLA_DESERT_NORTH
	    || MainRoutineStateWork.next == SCENE_SUHALLA_DESERT_SOUTH)
	{
		SuhallaDesertSavedColor = SUHALLA_REVEAL_PALETTE_COLOR;
		scSuhallaDesertRevealEnds();
	}
	if (MainRoutineStateWork.next == SCENE_SUHALLA_DESERT_NORTH)
	{
		scSuhallaDesertSetUpFirstMapStorms();
	}
	else if (MainRoutineStateWork.next == SCENE_SUHALLA_DESERT_SOUTH)
	{
		scSuhallaDesertSetUpSecondMapStorms();
	}
	else
	{
		sySoundPlay(SE_STOP_ACTION_SE);
	}
	if (MainRoutineStateWork.entrance == ENTRANCE_DEFAULT)
	{
		grid->flags &= ~DISPCNT_BG1_ON; // no haze on a fresh arrival
	}
	return 0;
}

// The north half's storms on entry: doused ones count as risen, and the rest start shrunk into the
// sand unless the party is coming back to where it was.
static void scSuhallaDesertSetUpFirstMapStorms(void)
{
	ACTOR *actor;
	Sint32 stormId;
	Sint32 hazeWeight = 0;

	if (gmFlagGet(FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED))
	{
		gmFlagSet(FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN);
	}
	if (gmFlagGet(FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 1))
	{
		gmFlagSet(FLAG_SUHALLA_DESERT_SECOND_SANDSTORM_RISEN);
	}
	if (gmFlagGet(FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 2))
	{
		gmFlagSet(FLAG_SUHALLA_DESERT_THIRD_SANDSTORM_RISEN);
	}
	if (gmFlagGet(FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 3))
	{
		gmFlagSet(FLAG_SUHALLA_DESERT_FOURTH_SANDSTORM_RISEN);
	}
	if (gmFlagGet(FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 4))
	{
		gmFlagSet(FLAG_SUHALLA_DESERT_FIFTH_SANDSTORM_RISEN);
	}
	for (stormId = ACTOR_FIRST_SANDSTORM; stormId <= ACTOR_LAST_SANDSTORM; stormId++)
	{
		actor = fdActorGet(stormId);
		if (actor != NULL)
		{
			if (!gmFlagGet(FLAG_KEEP_PARTY_POSITION))
			{
				actor->scaleX = toFIXED(1) / 32;
				actor->scaleY = toFIXED(1) / 32;
			}
			actor->sprite->flags = 0;
		}
	}
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDCNT, SUHALLA_DESERT_HAZE_BLEND, IO_WRITE_HALFWORD);
	if (gmFlagGet(FLAG_SUHALLA_DESERT_SANDSTORMS_RISEN))
	{
		hazeWeight = BLEND_WEIGHT_MAX;
		fdQueueMusic(SE_SANDSTORM);
	}
	// The haze at full weight once a storm has risen, else not at all: BLEND_ALPHA(hazeWeight,
	// BLEND_WEIGHT_MAX - hazeWeight), its bottom weight written first.
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA,
	                    ((BLEND_WEIGHT_MAX - hazeWeight) << 8) | hazeWeight,
	                    IO_WRITE_HALFWORD);
}

// The south half's storms on entry, as the north half's; the fourth is tinted red, and is already
// full size.
static void scSuhallaDesertSetUpSecondMapStorms(void)
{
	ACTOR *actor;
	SPR   *sprite;
	Sint32 stormId;
	Sint32 hazeWeight = 0;

	if (gmFlagGet(FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED))
	{
		gmFlagSet(FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN);
	}
	if (gmFlagGet(FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 1))
	{
		gmFlagSet(FLAG_SUHALLA_DESERT_SECOND_SANDSTORM_RISEN);
	}
	if (gmFlagGet(FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 2))
	{
		gmFlagSet(FLAG_SUHALLA_DESERT_THIRD_SANDSTORM_RISEN);
	}
	for (stormId = ACTOR_FIRST_SANDSTORM; stormId < ACTOR_FOURTH_SANDSTORM; stormId++)
	{
		actor = fdActorGet(stormId);
		if (actor != NULL)
		{
			if (!gmFlagGet(FLAG_KEEP_PARTY_POSITION))
			{
				actor->scaleX = toFIXED(1) / 32;
				actor->scaleY = toFIXED(1) / 32;
			}
			sprite        = actor->sprite;
			sprite->flags = 0;
		}
	}
	actor = fdActorGet(ACTOR_FOURTH_SANDSTORM);
	if (actor != NULL)
	{
		SPR_ANIMATION *part;

		sprite = actor->sprite;
		part   = sprite->entries[0];
		if (part != NULL)
		{
			part->colorMode = SPRITE_COLOR_RED_TINT;
		}
		sprite->dirty = 1;
		sprite->flags = 0;
	}
	if (gmFlagGet(FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 4))
	{
		gmFlagSet(FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_DOUSED);
	}
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDCNT, SUHALLA_DESERT_HAZE_BLEND, IO_WRITE_HALFWORD);
	if (gmFlagGet(FLAG_SUHALLA_DESERT_SANDSTORMS_RISEN))
	{
		hazeWeight = BLEND_WEIGHT_MAX;
		fdQueueMusic(SE_SANDSTORM);
	}
	// The haze at full weight once a storm has risen, else not at all: BLEND_ALPHA(hazeWeight,
	// BLEND_WEIGHT_MAX - hazeWeight), its bottom weight written first.
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA,
	                    ((BLEND_WEIGHT_MAX - hazeWeight) << 8) | hazeWeight,
	                    IO_WRITE_HALFWORD);
}

#include "shared/field_effect_duration_scripts.c"

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

Sint32 SuhallaDesertStormScript[] = {
	SC_CALL(scSuhallaDesertClearActorSpriteAndCollisionFlags),
	SC_CALL(scSuhallaDesertFlickerStorm),
	SC_END,
};

Sint32 SuhallaDesertFourthStormScript[] = {
	SC_CALL(scSuhallaDesertClearActorSpriteAndCollisionFlags),
	SC_CALL(scSuhallaDesertFlickerStorm),
	SC_END,
};

SCENE_ENTRANCE SuhallaDesertEntrancesOther[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE SuhallaDesertNorthEntrances[] = {
	{SUHALLA_DESERT_NORTH_FROM_WORLD_MAP,
     CONDITION_ALWAYS, 264,
     0, 56,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SUHALLA_DESERT_NORTH_FROM_SUHALLA_DESERT_SOUTH,
     CONDITION_ALWAYS, 24,
     0, 936,
     FACING_EAST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE SuhallaDesertSouthEntrances[] = {
	{SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_NORTH,
     CONDITION_ALWAYS, 488,
     0, 456,
     FACING_WEST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SUHALLA_DESERT_SOUTH_FROM_WORLD_MAP,
     CONDITION_ALWAYS, 488,
     0, 1208,
     FACING_WEST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_EAST,
     CONDITION_ALWAYS, 440,
     0, 624,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_WEST,
     CONDITION_ALWAYS, 56,
     0, 624,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE SuhallaDesertCavesEntrances[] = {
	{SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_WEST,
     CONDITION_ALWAYS, 72,
     0, 248,
     FACING_NORTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SUHALLA_DESERT_CAVES_WEST_FROM_EAST,
     CONDITION_ALWAYS, 440,
     0, 152,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SUHALLA_DESERT_CAVES_EAST_FROM_WEST,
     CONDITION_ALWAYS, 632,
     0, 152,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_EAST,
     CONDITION_ALWAYS, 648,
     0, 280,
     FACING_NORTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SCENE_TABLE_END},
};

Uint32 SuhallaDesertExits[] = {
	SCENE_EXITS(SuhallaDesertNorth),
	SCENE_EXIT(SUHALLA_DESERT_NORTH_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SUHALLA_DESERT_NORTH),
	SCENE_EXIT(SUHALLA_DESERT_NORTH_TO_SUHALLA_DESERT_SOUTH,
               SuhallaDesertSouth,
               SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_NORTH),
	SCENE_EXITS(SuhallaDesertSouth),
	SCENE_EXIT(SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_NORTH,
               SuhallaDesertNorth,
               SUHALLA_DESERT_NORTH_FROM_SUHALLA_DESERT_SOUTH),
	SCENE_EXIT(SUHALLA_DESERT_SOUTH_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SUHALLA_DESERT_SOUTH),
	SCENE_EXIT(SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_CAVES_EAST,
               SuhallaDesertCaves,
               SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_EAST),
	SCENE_EXIT(SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_CAVES_WEST,
               SuhallaDesertCaves,
               SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_WEST),
	SCENE_EXITS(SuhallaDesertCaves),
	SCENE_EXIT(SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_WEST,
               SuhallaDesertSouth,
               SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_WEST),
	SCENE_EXIT(
		SUHALLA_DESERT_CAVES_WEST_TO_EAST, SuhallaDesertCaves, SUHALLA_DESERT_CAVES_EAST_FROM_WEST),
	SCENE_EXIT(
		SUHALLA_DESERT_CAVES_EAST_TO_WEST, SuhallaDesertCaves, SUHALLA_DESERT_CAVES_WEST_FROM_EAST),
	SCENE_EXIT(SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_EAST,
               SuhallaDesertSouth,
               SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_EAST),
	SCENE_EXITS_END,
};

ACTOR_PLACE SuhallaDesertActorPlacementsOther[] = {
	{SCENE_TABLE_END},
};

// clang-format off
ACTOR_PLACE SuhallaDesertNorthActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED, (Sint32)SuhallaDesertStormScript,
		56, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 1, (Sint32)SuhallaDesertStormScript,
		456, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 2, (Sint32)SuhallaDesertStormScript,
		120, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 3, (Sint32)SuhallaDesertStormScript,
		264, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 4, (Sint32)SuhallaDesertStormScript,
		440, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE SuhallaDesertSouthActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED, (Sint32)SuhallaDesertStormScript,
		216, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 1, (Sint32)SuhallaDesertStormScript,
		296, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 2, (Sint32)SuhallaDesertStormScript,
		408, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 3, (Sint32)SuhallaDesertFourthStormScript,
		376, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 4, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FLASH), MOVEMENT_TYPE_STAND,
		72, 0, 696, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE SuhallaDesertCavesActorPlacements[] = {
	{ SCENE_TABLE_END },
};

Sint32 SuhallaDesertChaseScript[] = {
	SC_CALL(scSuhallaDesertChaseLeader),
	SC_END,
};

// The scripts of the desert's two halves, in one table.
//
// Its exits keep their numbers, since each half's exit table gives them its own ways out. Exit 1 is SUHALLA_DESERT_NORTH_TO_WORLD_MAP in the north half and SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_NORTH in the south half.
CUE SuhallaDesertCuesOther[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, 3 },
#if !SUHALLA_DESERT_WEST_CAVE_EXIT_NEEDS_FACING_NORTH
	{ CUE_EXIT, 4, CONDITION_ALWAYS, 4 },
#else
	{ CUE_EXIT | CUE_FLAG_FACING | FACING_NORTH, 4, CONDITION_ALWAYS, 4 },
#endif
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_IODEM_ASKS, FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA, SCRIPT_REF(scSuhallaDesertIodemAsksWhetherToPressOn) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_NORTH_STORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED, SCRIPT_REF(scSuhallaDesertCaughtByFirstStorm) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_NORTH_STORM + 1, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 1, SCRIPT_REF(scSuhallaDesertCaughtBySecondStorm) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_NORTH_STORM + 2, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 2, SCRIPT_REF(scSuhallaDesertCaughtByThirdStorm) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_NORTH_STORM + 3, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 3, SCRIPT_REF(scSuhallaDesertCaughtByFourthStorm) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_NORTH_STORM + 4, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 4, SCRIPT_REF(scSuhallaDesertCaughtByFifthStorm) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_SOUTH_STORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED, SCRIPT_REF(scSuhallaDesertCaughtByFirstStorm) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_SOUTH_STORM + 1, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 1, SCRIPT_REF(scSuhallaDesertCaughtBySecondStorm) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_SOUTH_STORM + 2, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 2, SCRIPT_REF(scSuhallaDesertCaughtByThirdStorm) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_NEAR_NORTH_STORM, FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN, SCRIPT_REF(scSuhallaDesertFirstStormRises) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_NEAR_NORTH_STORM + 1, FLAG_SUHALLA_DESERT_SECOND_SANDSTORM_RISEN, SCRIPT_REF(scSuhallaDesertSecondStormRises) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_NEAR_NORTH_STORM + 2, FLAG_SUHALLA_DESERT_THIRD_SANDSTORM_RISEN, SCRIPT_REF(scSuhallaDesertThirdStormRises) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_NEAR_NORTH_STORM + 3, FLAG_SUHALLA_DESERT_FOURTH_SANDSTORM_RISEN, SCRIPT_REF(scSuhallaDesertFourthStormRises) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_NEAR_NORTH_STORM + 4, FLAG_SUHALLA_DESERT_FIFTH_SANDSTORM_RISEN, SCRIPT_REF(scSuhallaDesertFifthStormRises) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_NEAR_SOUTH_STORM, FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN, SCRIPT_REF(scSuhallaDesertFirstStormRises) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_NEAR_SOUTH_STORM + 1, FLAG_SUHALLA_DESERT_SECOND_SANDSTORM_RISEN, SCRIPT_REF(scSuhallaDesertSecondStormRises) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_NEAR_SOUTH_STORM + 2, FLAG_SUHALLA_DESERT_THIRD_SANDSTORM_RISEN, SCRIPT_REF(scSuhallaDesertThirdStormRises) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_SOUTH_STORM + 3, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 3, SCRIPT_REF(scSuhallaDesertCaughtByFourthStorm) },
	{ CUE_STEP, METATILE_BEHAVIOR_SUHALLA_DESERT_STORM_CHASES, FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_SET_OFF, SCRIPT_REF(scSuhallaDesertStormGivesChase) },
	{ CUE_TALK, ACTOR_FLASH, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaDesertFightFlash) },
	{ EXAMINE_CUE(EXAMINE_SILENT), TREASURE_SPOT_PSYNERGY_STONE, FLAG_SUHALLA_DESERT_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
	{ EXAMINE_CUE(EXAMINE_CHEST), 103, FLAG_SUHALLA_DESERT_VIRTUOUS_ARMLET, FIND_ITEM(ITEM_VIRTUOUS_ARMLET) },
	{ EXAMINE_CUE(EXAMINE_CHEST), 104, FLAG_SUHALLA_DESERT_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ EXAMINE_CUE(EXAMINE_CHEST), 101, FLAG_SUHALLA_DESERT_COOKIE, FIND_ITEM(ITEM_COOKIE) },
	{ EXAMINE_CUE(EXAMINE_CHEST), 102, FLAG_SUHALLA_DESERT_MIMIC_BEATEN, FIND_MIMIC | 6 },
	{ PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_SANDSTORM, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), SCRIPT_REF(scSuhallaDesertDouseCastAtStorm) },
	{ PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_SANDSTORM + 1, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), SCRIPT_REF(scSuhallaDesertDouseCastAtStorm) },
	{ PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_SANDSTORM + 2, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), SCRIPT_REF(scSuhallaDesertDouseCastAtStorm) },
	{ PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_SANDSTORM + 3, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), SCRIPT_REF(scSuhallaDesertDouseCastAtStorm) },
	{ PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_SANDSTORM + 4, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), SCRIPT_REF(scSuhallaDesertDouseCastAtStorm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_SANDSTORM, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), SCRIPT_REF(scSuhallaDesertDouseStorm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_SANDSTORM + 1, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), SCRIPT_REF(scSuhallaDesertDouseStorm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_SANDSTORM + 2, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), SCRIPT_REF(scSuhallaDesertDouseStorm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_SANDSTORM + 3, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), SCRIPT_REF(scSuhallaDesertDouseStorm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_SANDSTORM + 4, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), SCRIPT_REF(scSuhallaDesertDouseStorm) },
	{ PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaDesertRevealStarts) },
	{ PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaDesertRevealEnds) },
	{ CUE_RAISED, RAISED_SANDSTORM_CARRIES_PARTY_AWAY, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaDesertStormCarriesPartyAway) },
	{ CUE_RAISED, RAISED_CHASING_SANDSTORM_CATCHES, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaDesertCaughtByFifthStorm) },
	{ CUE_TABLE_END },
};
// clang-format on

CUE SuhallaDesertCavesCues[] = {
	{CUE_EXIT,
     SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_WEST, CONDITION_ALWAYS,
     SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_WEST},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT,
     SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_EAST, CONDITION_ALWAYS,
     SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_EAST},
	{CUE_TABLE_END},
};
