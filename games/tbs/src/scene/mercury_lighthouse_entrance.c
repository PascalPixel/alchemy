// Mercury Lighthouse (SceneIdMercuryLighthouseEntrance), the lighthouse's ground floor inside its
// door, which leads up to its floors (mercury_lighthouse.c).
//
// Mia is Imil's healer and the last of the Mercury Clan who guard the lighthouse. She has sensed
// someone inside, where none but she and Alex, her apprentice who disappeared, can enter. She has
// hurried in ahead of the party to find out who.
//
// The floor is several rooms, joined by doorways, waterfalls and the floors above, in the order the
// party meets them:
//
// - The hall of six statues inside the door. The first time in, the party
//       finds Mia trying again and again to get past a monster, the Lizard Man,
//       who steps across the doorway at the north end to block her
//       (scMercuryLighthouseEntranceLizardManBlocksMia). Spoken to, she says `A
//       monster inside Mercury Lighthouse? How could this be!?` He sidesteps in
//       front of Isaac too. Talking to him, Mind Read on him or walking north
// into him starts the battle. Back from it, Mia nods to Isaac and goes on through the doorway
// (scMercuryLighthouseEntranceMiaEntersLighthouse,
//       FLAG_MERCURY_LIGHTHOUSE_ENTERED_BY_MIA). Until then, coming in by the
//       door always plays the Lizard Man's scene.
// - The goddess's room, beyond the doorway: a great statue of a goddess above
//       a pool. A white stone post at the pool's south edge reads `He who
// honors the heart of the goddess shall stand at the center of all, like a swan in the center of a
// rippling pond.` Jumping onto the goddess's hands and
//       casting Ply on her wakes the pools of the whole floor
//       (FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED) and sets their colours
//       shimmering. From then on the tile in the water below her hands gives
//       Isaac up to three lights, each hop out onto open water spends one, and
// a light holds him up where he lands (the helpers of
//       shared/mercury_lighthouse_pillar.c). Dropping into this room through
// the hole upstairs, the first time, Isaac lands on that tile in a burst, and
//       until Ply has worked the post's words appear.
// - The twin fountains, north of the goddess's room, two pools under
//       dragon-head spouts, whose tiles answer only once Ply has worked.
// - Two rooms of waterfalls, west and east. Walking into some waterfalls
//       carries Isaac to another room: the west room's to the room of the dry
//       fountain, the east room's left one back to the twin fountains, and its
//       right one, beside the chest that is a Mimic, to the room whose chest
//       holds Psynergy Armor across its pool.
// - The dry fountain's room, which fills with water once the channels
//       upstairs run (FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN).
//
// Four doors lead up to SceneIdMercuryLighthouseLower
// (MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST,
// MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST,
// MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTHWEST and
// MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST). So does the rise from
// the channels (MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_PIPE, then
// MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_PIPE).
// MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_EXTERIOR leads back out through the door to the
// ground before it, Mercury Lighthouse's SceneIdMercuryLighthouseExterior (mercury_lighthouse.c).
//
// Every entry sets FLAG_IN_MERCURY_LIGHTHOUSE, under which Mercury Adepts regain PP faster. It also
// makes SceneIdMercuryLighthouseExterior's MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP the Retreat
// point.
//
// The shimmering colours (scMercuryLighthouseCyclePalette) run once Ply has worked. In the
// goddess's room they always run (MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_SIX_STATUES and the two
// from the twin fountains, and MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE).
// Elsewhere they are black until then.
//
// The lights Isaac carries come back from their heap slot (shared/mercury_lighthouse_lights.c) on a
// return after a battle.
//
// Three entrances each have two rows, chosen by whether the channels upstairs run
// (FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN). They are
// MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_FROM_WEST_WATERFALLS,
// MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTHEAST and
// MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/facing.h"
#include "field/field_effect.h"
#include "graphics/graphics.h"
#include "scene/mercury_lighthouse_lights.h"
#include "scene/mercury_lighthouse_pillar.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

// The metatile behaviors this floor's own cues answer. The pools' tiles take the shared ones of
// MERCURY_LIGHTHOUSE_POOL_BEHAVIOR, and each doorway's metatiles carry its exit's number past
// EXIT_METATILE_BEHAVIOR_OFFSET.
typedef enum MERCURY_LIGHTHOUSE_ENTRANCE_METATILE_BEHAVIOR
{
	// The white stone post at the south edge of the goddess's pool, metatile (12, 38).
	METATILE_BEHAVIOR_HEART_INSCRIPTION            = 1,
	// Answered by scMercuryLighthouseEntranceUnusedReturnToSixStatues, but no metatile of the floor
	// has it.
	METATILE_BEHAVIOR_UNUSED_RETURN_TO_SIX_STATUES = 10,
	// Straight in front of the Lizard Man's doorway, metatile (8, 10).
	METATILE_BEHAVIOR_BEFORE_LIZARD_MAN            = 40,
	// Beside him in front of the doorway, metatiles (7, 10) and (9, 10).
	METATILE_BEHAVIOR_BESIDE_LIZARD_MAN            = 41,
	// One metatile back from beside him, metatiles (7, 11) and (9, 11).
	METATILE_BEHAVIOR_BEHIND_BESIDE_LIZARD_MAN     = 42,
	// The waterfall at metatile (14, 54) in the west room of waterfalls.
	METATILE_BEHAVIOR_WATERFALL_TO_DRY_FOUNTAIN    = 67,
	// The left waterfall of the east room of waterfalls, metatile (35, 54).
	METATILE_BEHAVIOR_WATERFALL_TO_TWIN_FOUNTAINS  = 68,
	// The right waterfall of the east room of waterfalls, metatile (43, 54).
	METATILE_BEHAVIOR_WATERFALL_TO_CHEST_FOUNTAIN  = 69,
	// The chest across the pool in its room, metatile (57, 19).
	METATILE_BEHAVIOR_PSYNERGY_ARMOR_CHEST         = 100,
	// The chest in the east room of waterfalls that is a Mimic, metatile (47, 54).
	METATILE_BEHAVIOR_MIMIC_CHEST                  = 101
} MERCURY_LIGHTHOUSE_ENTRANCE_METATILE_BEHAVIOR;

extern Sint32         MercuryLighthouseEntranceRippleScript[];
extern Sint32         MercuryLighthouseEntranceLizardManStepCenter[];
extern Sint32         MercuryLighthouseEntranceLizardManStepWest[];
extern Sint32         MercuryLighthouseEntranceLizardManStepEast[];
extern SCENE_ENTRANCE MercuryLighthouseEntranceEntrances[];
extern Uint32         MercuryLighthouseEntranceExits[];
extern ACTOR_PLACE    MercuryLighthouseEntranceActorPlacements[];
extern CUE            MercuryLighthouseEntranceCues[];
extern Sint32         GlintTwinkleScript[];

// This floor's actors, numbering its actor placement rows from ACTOR_FIRST_PLACED.
//
// Mia stands in her own party slot. The Lizard Man is the monster she cannot get past (his scripted
// battle fights him as "Lizard Man"). The Ply target is the invisible actor on the goddess's hands
// that Ply is cast on.
typedef enum MERCURY_LIGHTHOUSE_ENTRANCE_ACTOR_ID
{
	ACTOR_LIZARD_MAN = ACTOR_FIRST_PLACED,
	// Six statues line the way to the doorway, three on its west side (x 88), then three on its
	// east (x 184).
	ACTOR_FIRST_STATUE,
	// The east side's three statues, which the doorway's scripts draw mirrored.
	ACTOR_FIRST_EAST_STATUE = ACTOR_FIRST_STATUE + 3,
	ACTOR_PLY_TARGET        = ACTOR_FIRST_PLACED + 8
} MERCURY_LIGHTHOUSE_ENTRANCE_ACTOR_ID;

// Scene flags, kept while the party stays here.
enum
{
	FLAG_MIA_GOES_IN_SEEN         = FLAG_SCENE_TEMP_50,
	FLAG_DROP_FROM_ABOVE_SEEN     = FLAG_SCENE_TEMP_51,
	// The Lizard Man has stepped into the leader's column.
	FLAG_LIZARD_MAN_STEPPED_ASIDE = FLAG_SCENE_TEMP_05
};

static void scMercuryLighthouseEntranceLizardManBlocksMia(void);
static void scMercuryLighthouseEntranceMiaEntersLighthouse(void);

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"
#include "shared/pushed_actor.c"
#include "shared/mercury_lighthouse_sparks.c"

// The sound of the Lizard Man's sidestep, played by all three step scripts as he lands.
static Sint32 scMercuryLighthouseEntrancePlayStepSound(ACTOR *actor)
{
	sySoundPlay(SE_FOOTSTEP);
	return 0;
}

// The last row of half metatiles, north of 184 pixels, in which Mia watches Isaac.
#define MIA_WATCH_LAST_HALF_METATILE 22

// Mia's update while she waits by the Lizard Man: she turns to watch Isaac while he is near her,
// north of 184 pixels, and looks back north at the doorway otherwise.
static UNREAD_RESULT scMercuryLighthouseEntranceMiaWatchesLeader(ACTOR *actor)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	if (FIELD_HALF_METATILE(leader->z.fixed) <= MIA_WATCH_LAST_HALF_METATILE)
	{
		actor->facing = syAtan(leader->z.fixed - actor->z.fixed, leader->x.fixed - actor->x.fixed);
	}
	else if (actor->facing != FACING_NORTH)
	{
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	}
#if !FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

#include "shared/mercury_lighthouse_palette_cycle.c"

// Standing out on the water of one of the pools, held up by a light, Isaac sends blue ripples
// spreading round him.
//
// It answers the open-water tiles of every pool on this floor
// (METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER), such as metatiles (8, 34) to (16, 36) in the
// goddess's pool, once Ply has worked.
static void scMercuryLighthouseEntranceRippleAroundLeader(void)
{
	scMercuryLighthouseRippleAroundLeader();
}

// Stepping back onto the edge of a pool takes away the lights Isaac carries: the column of light
// over him swells and fades.
//
// It answers the edge tiles of every pool on this floor
// (METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP), the goddess's hands at metatile (12, 32) among
// them, once Ply has worked.
static void scMercuryLighthouseEntranceLoseLights(void)
{
	scMercuryLighthouseLoseLights();
}

// Pushing towards the water from a tile of one of the pools hops Isaac two metatiles to the next
// tile, once Ply has worked; a hop out onto open water spends one of his lights, which holds him up
// where he lands.
//
// It answers the pools' heart, open-water and edge tiles alike
// (METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART, METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER
// and METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP), laid two metatiles apart with water that
// cannot be walked between them.
static void scMercuryLighthouseEntranceHopAcrossPool(void)
{
	scMercuryLighthouseHopAcrossPool();
}

// Standing on the heart tile of a pool, such as the one below the goddess's hands at metatile (12,
// 34), gives Isaac lights, up to three, shown as a column of light over him.
//
// It answers METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART once Ply has worked.
static void scMercuryLighthouseEntranceChargeLights(void)
{
	scMercuryLighthouseChargeLights();
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return MercuryLighthouseEntranceEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return MercuryLighthouseEntranceExits;
}

// Returns Mia, the Lizard Man, the six statues and the two invisible actors in the goddess's room.
const ACTOR_PLACE *scGetActorPlacements(void)
{
	return MercuryLighthouseEntranceActorPlacements;
}

// Searching the white stone post at the south edge of the goddess's pool, metatile (12, 38), reads
// `He who honors the heart of the goddess shall stand at the center of all, like a swan in the
// center of a rippling pond.`
static void scMercuryLighthouseEntranceReadHeartInscription(void)
{
	fdScriptStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	mnFieldMessageShow(MsgMercuryWhoHonorsHeart, 1);
	fdScriptEnd();
}

#include "shared/mercury_lighthouse_waterfall.c"

// Walking north into the waterfall at metatile (14, 54), in the west room of waterfalls, carries
// Isaac to the room of the dry fountain.
//
// He walks on into the falling water, vanishes in a trail of sparkles, and the screen fades
// (MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_DRY_FOUNTAIN).
static void scMercuryLighthouseEntranceWaterfallToDryFountain(void)
{
	scMercuryLighthouseWalkIntoWaterfall(
		MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_DRY_FOUNTAIN);
}

// Walking north into the waterfall at metatile (35, 54), the left one of the east room of
// waterfalls, carries Isaac back to the east end of the twin fountains.
//
// He walks on into the falling water, vanishes in a trail of sparkles, and the screen fades
// (MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_TWIN_FOUNTAINS).
static void scMercuryLighthouseEntranceWaterfallToTwinFountains(void)
{
	scMercuryLighthouseWalkIntoWaterfall(
		MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_TWIN_FOUNTAINS);
}

// Walking north into the waterfall at metatile (43, 54), the right one of the east room of
// waterfalls, beside the Mimic's chest, carries Isaac to the room whose chest holds Psynergy Armor
// across its pool.
//
// He walks on into the falling water, vanishes in a trail of sparkles, and the screen fades
// (MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_CHEST_FOUNTAIN).
static void scMercuryLighthouseEntranceWaterfallToChestFountain(void)
{
	scMercuryLighthouseWalkIntoWaterfall(
		MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_CHEST_FOUNTAIN);
}

// Returns the floor's step, search, talk and Psynergy cues.
const CUE *scGetCues(void)
{
	return MercuryLighthouseEntranceCues;
}

// The glints that rise from the heart tile below the goddess's hands once Ply has worked.
#define PLY_GLINTS    15
// The puffs that burst round Isaac landing from the hole above.
#define LANDING_PUFFS 17

// Casting Ply on the goddess while standing on her hands, metatile (12, 32), wakes the pools: the
// room glows pale, glints rise from the tile below her hands, and the colours start to shimmer.
//
// That is the center of all the post by the pool speaks of; cast from anywhere else, the script
// does nothing. The flag it sets (FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED) lets every pool's tiles
// answer, and the goddess's pool takes its tiles' behaviours from the copy kept beside the map.
static void scMercuryLighthouseEntranceActivateWithPly(void)
{
	Sint32         leaderColumn;
	Sint32         leaderRow;
	Uint32         sparkIndex;
	Sint32         sparkX;
	Sint32         sparkZ;
	Sint32         speedRoll;
	Sint32         velocityY;
	EFFECT_OPTIONS options;

	leaderColumn = fdActorGet(ACTOR_PARTY_LEADER)->x.fixed / toFIXED(16);
	leaderRow    = fdActorGet(ACTOR_PARTY_LEADER)->z.fixed / toFIXED(16);
	if (leaderColumn == 12 && leaderRow == 32)
	{
		fdScriptStart();
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeStartInterpolation(60);
		fdScriptWait(120);
		fdPaletteFadeApplyTarget(PALETTE_TINT_PALE, PALETTES_BG);
		fdPaletteFadeStartInterpolation(60);
		fdScriptWait(40);
		for (sparkIndex = 0; sparkIndex < PLY_GLINTS; sparkIndex++)
		{
			options.priority = 1;
			options.type     = SPRITE_GLINT;
			options.script   = GlintTwinkleScript;
			sySoundPlay(SE_GLITTER);
			sparkX    = 208 - (syRandomBelow(16));
			sparkZ    = 560 - (syRandomBelow(16));
			// Rising at 0.6 to 1.05 pixels a tick.
			speedRoll = (syRandomBelow(4));
			velocityY = syMathDivide(toFIXED(speedRoll * 15) + toFIXED(60), 100);
			fdEffectSpawn(toFIXED(sparkX),
			              0,
			              toFIXED(sparkZ),
			              0,
			              velocityY,
			              0,
			              EFFECT_USE_PRIORITY | EFFECT_USE_TYPE | EFFECT_USE_SCRIPT
			                  | EFFECT_SCRIPT_MEDIUM,
			              &options);
			fdScriptWait(4);
		}
		sySoundPlay(SE_SHIMMER);
		fdScriptWait(60);
		// The goddess's pool takes its awakened tiles.
		gmFlagSet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED);
		sySetTickEvent(scMercuryLighthouseCyclePalette, EVENT_PASS_TICK);
		fdMapGridCopyMetatilesTo(37, 98, 10, 97, 5, 3);
		fdMapGridCopyMetatileAttributes(70, 32, 13, 7, 6, 32);
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeStartInterpolation(60);
		fdScriptWait(120);
		fdScriptEnd();
	}
}

// Never seen: no metatile of this floor answers its cue row, so nothing sends Isaac back to the
// Lizard Man's doorway this way.
//
// It would start the hall of six statues again at
// MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN, where Mia goes in if she has not yet.
static void scMercuryLighthouseEntranceUnusedReturnToSixStatues(void)
{
	syImportScriptSetExitRequest(MERCURY_LIGHTHOUSE_ENTRANCE_TO_SIX_STATUES_AFTER_LIZARD_MAN);
}

// Stepping back to the middle in front of the Lizard Man, metatile (8, 10), or one metatile back
// from beside him, metatile (7, 11) or (9, 11), makes him step back into the middle of the doorway.
//
// It plays only while he stands aside (FLAG_LIZARD_MAN_STEPPED_ASIDE).
static void scMercuryLighthouseEntranceLizardManReturnsToCenter(void)
{
	gmFlagClear(FLAG_LIZARD_MAN_STEPPED_ASIDE);
	fdActorSetScript(ACTOR_LIZARD_MAN, MercuryLighthouseEntranceLizardManStepCenter);
}

// Walking up beside the Lizard Man, metatile (7, 10) or (9, 10), makes him sidestep in front of
// Isaac with a footstep.
//
// He steps west or east to match Isaac, as he did to Mia.
static void scMercuryLighthouseEntranceLizardManBlocksLeader(void)
{
	FIXED  leaderX;
	Sint32 leaderColumn;

	leaderX      = fdActorGet(ACTOR_PARTY_LEADER)->x.fixed;
	leaderColumn = leaderX / toFIXED(16);
	gmFlagSet(FLAG_LIZARD_MAN_STEPPED_ASIDE);
	if (leaderColumn == 7)
	{
		fdActorSetScript(ACTOR_LIZARD_MAN, MercuryLighthouseEntranceLizardManStepWest);
	}
	else
	{
		fdActorSetScript(ACTOR_LIZARD_MAN, MercuryLighthouseEntranceLizardManStepEast);
	}
}

// Talking to the Lizard Man, Mind Read on him, or walking north into him from the metatiles in
// front of the doorway, (7, 10) to (9, 10), starts the battle with him.
//
// Back from the battle the party stands in the hall without him
// (MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN).
static void scMercuryLighthouseEntranceFightLizardMan(void)
{
	fdScriptStart();
	fdActorRunRepeatedMotion(ACTOR_LIZARD_MAN, 2);
	fdScriptWait(20);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
	fdScriptSetAfterBattleDestination(SCENE_MERCURY_LIGHTHOUSE_ENTRANCE,
	                                  MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN);
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	// The second slot of the encounter zone of Tret Tree's and Mercury Lighthouse's scripted
	// battles: the Lizard Man.
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, 1);
	fdScriptEnd();
}

#include "shared/mercury_lighthouse_arrive.c"

// Sets up Mercury Lighthouse's ground floor, then plays the script of the entrance taken.
Sint32 scInit(void)
{
	EFFECT_OPTIONS options;
	Sint32         velocity[3];
	ACTOR         *actor;
	Uint32         sparkIndex;
	Sint32         angle;
	Sint32         arrivalSeen;
	Sint32         alpha;

	IO_WRITE16(BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND, &REG_BLDCNT);
	alpha        = BLEND_ALPHA(16, 16);
	REG_BLDALPHA = alpha;
	scMercuryLighthouseRestoreLights(METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER,
	                                 (MERCURY_LIGHTHOUSE_LIGHTS *)SceneLocalState);
	gmFlagSet(FLAG_IN_MERCURY_LIGHTHOUSE);
	MainRoutineStateWork.retreatEntrance = MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP;
	MainRoutineStateWork.retreatScene    = SCENE_MERCURY_LIGHTHOUSE_EXTERIOR;
	fdEffectStartGroundMask(0);
	if (gmFlagGet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED))
		sySetTickEvent(scMercuryLighthouseCyclePalette, EVENT_PASS_TICK);
	else
		scMercuryLighthouseClearPalette();
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	// Each entrance restores its route or plays its one-time arrival.
	switch (MainRoutineStateWork.entrance)
	{
		case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR:
			// Until Mia has gone in, arriving here plays the Lizard Man script.
			if (!gmFlagGet(FLAG_MERCURY_LIGHTHOUSE_ENTERED_BY_MIA))
				syImportScriptSetExitRequest(
					MERCURY_LIGHTHOUSE_ENTRANCE_TO_SIX_STATUES_BEFORE_LIZARD_MAN);
			// Both doorway entrances mirror the east side's statues. Fall through.
		case MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_FROM_GODDESS:
			fdActorGet(ACTOR_FIRST_EAST_STATUE)->scaleX     = -toFIXED(1);
			fdActorGet(ACTOR_FIRST_EAST_STATUE + 1)->scaleX = -toFIXED(1);
			fdActorGet(ACTOR_FIRST_EAST_STATUE + 2)->scaleX = -toFIXED(1);
			sySynch(1);
			break;
		case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_WEST_WATERFALLS:
		case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_EAST:
		case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_WEST:
		case MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_EAST_WATERFALLS:
		case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST:
		case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST:
			// Once Ply has honored the heart, the twin fountains' two pools take the ground the
			// goddess's pool takes: metatiles out in the water, hearts and plain squares, which the
			// hops and lights answer, with the water between them a wall.
			if (gmFlagGet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED))
			{
				fdMapGridCopyMetatileAttributes(84, 5, 10, 7, 20, 5);
				fdMapGridCopyMetatileAttributes(101, 5, 12, 7, 37, 5);
			}
			break;
		case MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_SIX_STATUES:
		case MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_WEST:
		case MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_EAST:
		case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE:
			sySetTickEvent(scMercuryLighthouseCyclePalette, EVENT_PASS_TICK);
			// The metatiles scMercuryLighthouseEntranceActivateWithPly copies as it sets the flag,
			// redrawn on arrival.
			if (gmFlagGet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED))
			{
				fdMapGridCopyMetatilesTo(37, 98, 10, 97, 5, 3);
				fdMapGridCenterOnFollowedPoint();
				sySynch(1);
				fdMapGridCopyMetatileAttributes(70, 32, 13, 7, 6, 32);
			}
			if (MainRoutineStateWork.entrance
			    != MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE)
				break;
			arrivalSeen = fdScriptGetFlag(FLAG_DROP_FROM_ABOVE_SEEN);
			if (arrivalSeen != 0)
				break;
			// The first time, the leader drops from 130 pixels up and lands in a burst; until Ply
			// has been used, the words by the pool appear.
			gmFlagSet(FLAG_DROP_FROM_ABOVE_SEEN);
			fdScriptStart();
			fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
			fdMapGridCenterOnFollowedPoint();
			sySynch(1);
			fdActorGet(ACTOR_PARTY_LEADER)->y.fixed     = toFIXED(130);
			fdActorGet(ACTOR_PARTY_LEADER)->gravity     = toFIXED(1) / 2;
			fdActorGet(ACTOR_PARTY_LEADER)->bounce      = arrivalSeen;
			fdActorGet(ACTOR_PARTY_LEADER)->motionFlags = arrivalSeen;
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionTicks();
			fdScriptWait(30);
			fdActorGet(ACTOR_PARTY_LEADER)->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			sySoundPlay(SE_DUST_PUFF);
			fdScriptWait(24);
			// A ring of white puffs bursts out round him.
			actor           = fdActorGet(ACTOR_PARTY_LEADER);
			options.palette = SPRITE_COLOR_WHITE;
			for (sparkIndex = 0; sparkIndex < LANDING_PUFFS; sparkIndex++)
			{
				angle = sparkIndex * FACING_STEP;
				SET_VECTOR(velocity, syCos(angle), 0, sySin(angle));
				velocity[0] -= velocity[0] / 4;
				velocity[2] -= velocity[2] / 2;
				fdEffectSpawn(actor->x.fixed,
				              actor->y.fixed,
				              actor->z.fixed,
				              velocity[0],
				              velocity[1],
				              velocity[2],
				              EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
				              &options);
			}
			sySoundPlay(SE_THUMP);
			fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
			                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
			fdMapGridSetShake(5, 5, 1);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			fdMapGridWaitForShakeToSettle();
			fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
			                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
			fdActorGet(ACTOR_PARTY_LEADER)->gravity = toFIXED(1);
			fdActorGet(ACTOR_PARTY_LEADER)->bounce  = toFIXED(1) / 4;
			if (!gmFlagGet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED))
			{
				fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
				fdPaletteFadeApplyTarget(PALETTE_TINT_DIM, PALETTES_BG);
				fdPaletteFadeStartInterpolation(30);
				fdScriptWaitTransitionTicks();
				fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
				fdScriptWait(30);
				mnFieldMessageShowCentered(MsgMercuryWhoHonorsHeart, 0, 0);
				fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
				fdPaletteFadeStartInterpolation(30);
			}
			fdScriptEnd();
			break;
		case MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_EAST_WATERFALLS:
		case MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_TWIN_FOUNTAINS:
		case MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_WEST_WATERFALLS:
			scMercuryLighthouseArriveWithSparks();
			// All four arrivals play the same sound. Fall through.
		case MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_TWIN_FOUNTAINS:
			fdQueueMusic(SE_AMBIENT_WATER);
			break;
		case MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_PIPE:
			fdActorSetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
			fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
			fdScriptStart();
			fdMapGridCenterOnFollowedPoint();
			sySynch(1);
			FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionTicks();
			fdScriptWait(120);
			syImportScriptSetExitRequest(
				MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_PIPE);
			fdScriptEnd();
			break;
		case MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_BEFORE_LIZARD_MAN:
			if (!gmFlagGet(FLAG_KEEP_PARTY_POSITION))
				scMercuryLighthouseEntranceLizardManBlocksMia();
			else
				fdMapGridCopyMetatileAttributes(0, 0, 3, 3, 7, 9);
			break;
		case MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN:
			if (!gmFlagGet(FLAG_KEEP_PARTY_POSITION))
				scMercuryLighthouseEntranceMiaEntersLighthouse();
			break;
	}
	return 0;
}

// Mia tries twice to dodge past the Lizard Man in the doorway at the north end of the hall of six
// statues, east and then west, and each time he sidesteps to block her. She winces, and Isaac walks
// up behind her.
//
// It plays at MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_BEFORE_LIZARD_MAN, unless the party returns
// there after a battle. Then the metatiles before the doorway take the behaviours that make the
// Lizard Man block Isaac and fight him.
static void scMercuryLighthouseEntranceLizardManBlocksMia(void)
{
	Sint32 paceCycle;
	ACTOR *actor;

	fdScriptStart();
	fdActorGet(ACTOR_FIRST_EAST_STATUE)->scaleX     = -toFIXED(1);
	fdActorGet(ACTOR_FIRST_EAST_STATUE + 1)->scaleX = -toFIXED(1);
	fdActorGet(ACTOR_FIRST_EAST_STATUE + 2)->scaleX = -toFIXED(1);
	fdActorSetPos(ACTOR_MIA, 136, 184);
	fdActorSetPos(ACTOR_PARTY_LEADER, 136, 296);
	fdActorSetPos(ACTOR_LIZARD_MAN, 136, 152);
	fdActorSetSpeed(ACTOR_MIA, 1.5);
	fdActorSetSpeed(ACTOR_LIZARD_MAN, 1.5);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(136, CAMERA_KEEP, 184, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	fdActorGet(ACTOR_LIZARD_MAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdScriptWait(20);
	// Twice she tries the east side and then the west.
	for (paceCycle = 0; paceCycle < 2; paceCycle++)
	{
		fdActorWalkTo(ACTOR_MIA, 152, 168);
		fdScriptWait(10);
		fdActorSetScript(ACTOR_LIZARD_MAN, MercuryLighthouseEntranceLizardManStepEast);
		fdActorWaitForMoveById(ACTOR_MIA);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 30);
		fdActorRunRepeatedMotion(ACTOR_MIA, 1);
		fdActorGet(ACTOR_MIA)->steeringFlags &= ~ACTOR_FACES_MOTION;
		fdActorWalkTo(ACTOR_MIA, 136, 184);
		fdScriptWait(10);
		fdActorSetScript(ACTOR_LIZARD_MAN, MercuryLighthouseEntranceLizardManStepCenter);
		fdActorWaitForMoveById(ACTOR_MIA);
		fdActorGet(ACTOR_MIA)->steeringFlags |= ACTOR_FACES_MOTION;
		fdScriptWait(30);
		fdActorWalkTo(ACTOR_MIA, 120, 168);
		fdScriptWait(5);
		fdActorSetScript(ACTOR_LIZARD_MAN, MercuryLighthouseEntranceLizardManStepWest);
		fdActorWaitForMoveById(ACTOR_MIA);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 30);
		fdActorRunRepeatedMotion(ACTOR_MIA, 1);
		fdScriptWait(15);
		fdActorGet(ACTOR_MIA)->steeringFlags &= ~ACTOR_FACES_MOTION;
		fdActorWalkTo(ACTOR_MIA, 136, 184);
		fdScriptWait(15);
		fdActorSetScript(ACTOR_LIZARD_MAN, MercuryLighthouseEntranceLizardManStepCenter);
		fdActorWaitForMoveById(ACTOR_MIA);
		fdActorRunRepeatedMotion(ACTOR_MIA, 1);
		fdActorGet(ACTOR_MIA)->steeringFlags |= ACTOR_FACES_MOTION;
	}
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	actor = fdActorGet(ACTOR_MIA);
#if FIX_FUNCTION_SIGNATURES
	actor->update = scMercuryLighthouseEntranceMiaWatchesLeader;
#else
	actor->update = (void (*)(ACTOR *))scMercuryLighthouseEntranceMiaWatchesLeader;
#endif
	// The leader walks up behind her; the metatiles before the doorway become walkable.
	fdCameraMoveToActorById(ACTOR_PARTY_LEADER, 1);
	fdScriptWait(30);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 136, 264);
	fdCameraWaitForArrival();
	fdMapGridCopyMetatileAttributes(0, 0, 3, 3, 7, 9);
	fdScriptEnd();
}

// Back from the battle with the Lizard Man, Mia stands in the doorway, nods to Isaac and goes on
// into the lighthouse.
//
// It plays once, at MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN.
static void scMercuryLighthouseEntranceMiaEntersLighthouse(void)
{
	if (fdScriptGetFlag(FLAG_MIA_GOES_IN_SEEN) == 0)
	{
		gmFlagSet(FLAG_MIA_GOES_IN_SEEN);
		fdScriptStart();
		fdActorGet(ACTOR_FIRST_EAST_STATUE)->scaleX     = -toFIXED(1);
		fdActorGet(ACTOR_FIRST_EAST_STATUE + 1)->scaleX = -toFIXED(1);
		fdActorGet(ACTOR_FIRST_EAST_STATUE + 2)->scaleX = -toFIXED(1);
		fdActorSetPos(ACTOR_MIA, 136, 144);
		fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 10);
		FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		fdScriptWait(60);
		fdActorFaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
		fdScriptWait(30);
		fdActorWalkTo(ACTOR_MIA, 136, 72);
		fdScriptWait(40);
		fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
		fdActorWaitForMoveById(ACTOR_MIA);
		fdActorSetPos(ACTOR_MIA, 0, 0);
		gmFlagSet(FLAG_MERCURY_LIGHTHOUSE_ENTERED_BY_MIA);
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
		fdScriptEnd();
	}
}

#include "shared/mercury_lighthouse_lights.c"

// Spreads one blue ripple (SPRITE_RIPPLE) from an actor, for 30 ticks.
static void scMercuryLighthouseSpawnRipple(ACTOR *src)
{
	ACTOR *effect = syFieldActorCreate(SPRITE_RIPPLE, src->x.fixed, src->y.fixed, src->z.fixed);

	if (effect != NULL)
	{
		SPR *sprite = (SPR *)effect->sprite;

		fdActorStartScript(effect, MercuryLighthouseEntranceRippleScript);
		effect->motionFlags   = 0;
		effect->layer         = 1;
		effect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		if (sprite != NULL)
		{
			fdSpriteStartAnimation(sprite, 2);
			sprite->flags          = 0;
			sprite->image.priority = 3;
		}
	}
}

#include "shared/mercury_lighthouse_pillar.c"

#include "shared/field_effect_duration_scripts.c"

#include "shared/pushed_actor_tables.c"
// clang-format off

Sint32 MercuryLighthouseEntranceRippleScript[] = {
	SC_WAIT(30),
	SC_RELEASE,
};

Sint32 MercuryLighthouseSparkScript[] = {
	SC_CALL(scMercuryLighthouseRiseSpark),
	SC_RELEASE,
};

Sint32 MercuryLighthouseGrowLightScript[] = {
	SC_CALL(scMercuryLighthouseGrowLight),
	SC_END,
};

Sint32 MercuryLighthouseLightOutScript[] = {
	SC_CALL(scMercuryLighthouseSwellLightOut),
	SC_RELEASE,
};

#include "shared/mercury_lighthouse_pillar_tables.c"

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

Sint32 MercuryLighthouseEntranceLizardManStepCenter[] = {
	SC_SPEED(2),
	SC_ACCELERATION(2),
	SC_SET(SW_STEERING, 0),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(136, 0, 152),
	SC_CALL(scMercuryLighthouseEntrancePlayStepSound),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 MercuryLighthouseEntranceLizardManStepWest[] = {
	SC_SPEED(2),
	SC_ACCELERATION(2),
	SC_SET(SW_STEERING, 0),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(120, 0, 152),
	SC_CALL(scMercuryLighthouseEntrancePlayStepSound),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 MercuryLighthouseEntranceLizardManStepEast[] = {
	SC_SPEED(2),
	SC_ACCELERATION(2),
	SC_SET(SW_STEERING, 0),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(152, 0, 152),
	SC_CALL(scMercuryLighthouseEntrancePlayStepSound),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 PaletteCycleRed = 0;

Sint32 PaletteCycleGreen = 0;

Sint32 PaletteCycleBlue = 0;

SCENE_ENTRANCE MercuryLighthouseEntranceEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR, CONDITION_ALWAYS, 136, 0, 280, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_FROM_GODDESS, CONDITION_ALWAYS, 136, 0, 120, FACING_SOUTH, 0, 16, 32, 256, 320, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_SIX_STATUES, CONDITION_ALWAYS, 200, 0, 648, FACING_NORTH, 0, 80, 432, 320, 688, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_WEST, CONDITION_ALWAYS, 120, 0, 520, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_EAST, CONDITION_ALWAYS, 280, 0, 520, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE, CONDITION_ALWAYS, 200, 0, 552, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_WEST_WATERFALLS, CONDITION_ALWAYS, 360, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_EAST, CONDITION_ALWAYS, 456, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_WEST, CONDITION_ALWAYS, 616, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_EAST_WATERFALLS, CONDITION_ALWAYS, 760, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST, CONDITION_ALWAYS, 344, 0, 88, FACING_SOUTH, 0, 304, 32, 800, 208, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST, CONDITION_ALWAYS, 536, 0, 88, FACING_SOUTH, 0, 304, 32, 800, 208, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_FROM_WEST_WATERFALLS, FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN, 744, 0, 680, FACING_NORTH, 0, 544, 512, 784, 720, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTHEAST, FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN, 584, 0, 568, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST, FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN, 744, 0, 568, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_FROM_WEST_WATERFALLS, CONDITION_ALWAYS, 584, 0, 424, FACING_NORTH, 0, 384, 256, 624, 448, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTHEAST, CONDITION_ALWAYS, 424, 0, 312, FACING_SOUTH, 0, 384, 256, 624, 448, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST, CONDITION_ALWAYS, 584, 0, 312, FACING_SOUTH, 0, 384, 256, 624, 448, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_FOUNTAIN_FROM_EAST_WATERFALLS, CONDITION_ALWAYS, 744, 0, 424, FACING_NORTH, 0, 704, 256, 944, 448, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_TWIN_FOUNTAINS, CONDITION_ALWAYS, 344, 0, 872, FACING_SOUTH, 0, 48, 768, 400, 960, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_EAST_WATERFALLS, CONDITION_ALWAYS, 232, 0, 872, FACING_SOUTH, 0, 48, 768, 400, 960, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_TWIN_FOUNTAINS, CONDITION_ALWAYS, 568, 0, 872, FACING_SOUTH, 0, 464, 768, 816, 960, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_WEST_WATERFALLS, CONDITION_ALWAYS, 696, 0, 872, FACING_SOUTH, 0, 464, 768, 816, 960, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_PIPE, CONDITION_ALWAYS, 744, 0, 600, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_BEFORE_LIZARD_MAN, CONDITION_ALWAYS, 136, 0, 296, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN, CONDITION_ALWAYS, 136, 0, 168, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
	// XXX (no evidence): what the next two entrances were for; no exit or code leads to them.
	{ MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_UNUSED_NORTH, CONDITION_ALWAYS, 136, 0, 152, FACING_SOUTH, 0, 16, 32, 256, 320, 0 },
	{ MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_UNUSED_MIDDLE, CONDITION_ALWAYS, 136, 0, 232, FACING_EAST, 0, 16, 32, 256, 320, 0 },
	{ SCENE_TABLE_END },
};

Uint32 MercuryLighthouseEntranceExits[] = {
	SCENE_EXITS(MercuryLighthouseEntrance),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_EXTERIOR, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_ENTRANCE),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_TO_GODDESS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_SIX_STATUES),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_SIX_STATUES, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_FROM_GODDESS),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_WEST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_WEST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_EAST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_GODDESS_EAST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_WEST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_TWIN_FOUNTAINS),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_WEST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_WEST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_EAST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_FROM_TWIN_FOUNTAINS_EAST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_EAST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_TWIN_FOUNTAINS),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTHWEST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_SOUTHWEST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_WEST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_FROM_EAST_WATERFALLS),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_EAST_WATERFALLS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_FROM_WEST_WATERFALLS),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_TWIN_FOUNTAINS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_WEST_WATERFALLS),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_DRY_FOUNTAIN, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_DRY_FOUNTAIN_FROM_WEST_WATERFALLS),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_TWIN_FOUNTAINS, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_FROM_EAST_WATERFALLS),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_CHEST_FOUNTAIN, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_CHEST_FOUNTAIN_FROM_EAST_WATERFALLS),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_SIX_STATUES_BEFORE_LIZARD_MAN, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_BEFORE_LIZARD_MAN),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_PIPE, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_ENTRANCE_TO_SIX_STATUES_AFTER_LIZARD_MAN, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_AFTER_LIZARD_MAN),
	SCENE_EXITS_END,
};

ACTOR_PLACE MercuryLighthouseEntranceActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LIZARD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		88, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		88, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		88, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		184, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		184, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		184, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		200, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		200, 0, 512, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

CUE MercuryLighthouseEntranceCues[] = {
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), SCRIPT_REF(scMercuryLighthouseEntranceHopAcrossPool) },
	{ CUE_STEP, METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), SCRIPT_REF(scMercuryLighthouseEntranceLoseLights) },
	{ STEP_CUE_WALKING | STEP_CUE_FLAG_QUICK, METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), SCRIPT_REF(scMercuryLighthouseEntranceHopAcrossPool) },
	{ CUE_STEP, METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), SCRIPT_REF(scMercuryLighthouseEntranceRippleAroundLeader) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), SCRIPT_REF(scMercuryLighthouseEntranceHopAcrossPool) },
	{ CUE_STEP, METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), SCRIPT_REF(scMercuryLighthouseEntranceChargeLights) },
	{ CUE_STEP, METATILE_BEHAVIOR_UNUSED_RETURN_TO_SIX_STATUES, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseEntranceUnusedReturnToSixStatues) },
	{ CUE_STEP, METATILE_BEHAVIOR_BEFORE_LIZARD_MAN, IF_FLAG_SET(FLAG_LIZARD_MAN_STEPPED_ASIDE), SCRIPT_REF(scMercuryLighthouseEntranceLizardManReturnsToCenter) },
	{ CUE_STEP, METATILE_BEHAVIOR_BEHIND_BESIDE_LIZARD_MAN, IF_FLAG_SET(FLAG_LIZARD_MAN_STEPPED_ASIDE), SCRIPT_REF(scMercuryLighthouseEntranceLizardManReturnsToCenter) },
	{ CUE_STEP, METATILE_BEHAVIOR_BESIDE_LIZARD_MAN, FLAG_LIZARD_MAN_STEPPED_ASIDE, SCRIPT_REF(scMercuryLighthouseEntranceLizardManBlocksLeader) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_BEFORE_LIZARD_MAN, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseEntranceFightLizardMan) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_BESIDE_LIZARD_MAN, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseEntranceFightLizardMan) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_WATERFALL_TO_DRY_FOUNTAIN, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseEntranceWaterfallToDryFountain) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_WATERFALL_TO_TWIN_FOUNTAINS, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseEntranceWaterfallToTwinFountains) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_WATERFALL_TO_CHEST_FOUNTAIN, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseEntranceWaterfallToChestFountain) },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_EXTERIOR, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_EXTERIOR },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_TO_GODDESS, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_SIX_STATUES_TO_GODDESS },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_SIX_STATUES, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_SIX_STATUES },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_WEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_WEST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_EAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_GODDESS_TO_TWIN_FOUNTAINS_EAST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_WEST_WATERFALLS, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_WEST_WATERFALLS },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_WEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_WEST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_EAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_GODDESS_EAST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_EAST_WATERFALLS, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TWIN_FOUNTAINS_TO_EAST_WATERFALLS },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTHWEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTHWEST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_TO_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_WEST_WATERFALLS, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_EAST_WATERFALLS_TO_WEST_WATERFALLS },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_EAST_WATERFALLS, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_EAST_WATERFALLS },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_TWIN_FOUNTAINS, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_ENTRANCE_WEST_WATERFALLS_TO_TWIN_FOUNTAINS },
	{ CUE_TALK, ACTOR_MIA, CONDITION_ALWAYS, MSGNO(MsgMercuryMonsterInsideTower) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIA, CONDITION_ALWAYS, MSGNO(MsgMercuryLetMePass) },
	{ CUE_TALK, ACTOR_LIZARD_MAN, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseEntranceFightLizardMan) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LIZARD_MAN, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseEntranceFightLizardMan) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_HEART_INSCRIPTION, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseEntranceReadHeartInscription) },
	{ PSYNERGY_ON_ACTOR(ABILITY_PLY), ACTOR_PLY_TARGET, FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED, SCRIPT_REF(scMercuryLighthouseEntranceActivateWithPly) },
	{ PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseRemoveColumnOnRetreat) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_PSYNERGY_ARMOR_CHEST, FLAG_MERCURY_LIGHTHOUSE_PSYNERGY_ARMOR, FIND_ITEM(ITEM_PSYNERGY_ARMOR) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_MIMIC_CHEST, FLAG_MERCURY_LIGHTHOUSE_MIMIC_BEATEN, FIND_MIMIC },
	{ CUE_TABLE_END },
};

#include "shared/glint_twinkle_script.c"
// clang-format on
