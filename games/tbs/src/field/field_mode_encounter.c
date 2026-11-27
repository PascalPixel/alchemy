/* Random encounters, and each scene's music, area name and battle background.
   Part of field_mode.c. */

#include "field_mode_internal.h"

/* Chooses the current scene's encounter zones, one for each kind of ground the
   party can stand on (FieldStateWork->encounterZones, read by
   fdEncounterRollMapStep), from the first rule matching the scene and
   entrance. */
void fdEncounterSelectMapZones(Sint32 useRules)
{
	FIELD_STATE				   *state;
	Sint32						sceneId;
	Sint32						entrance;
	Uint8					   *groundZones;
	const SCENE_ENCOUNTER_RULE *rule;
	Sint32						zone;
	Sint16						firstZoneOnGround;
	Uint32						i;

	state = FieldStateWork;
	sceneId = MainRoutineStateWork.next;
	entrance = MainRoutineStateWork.entrance;
	groundZones = state->encounterZones;
	rule = SceneEncounterRules;
	zone = 0;
	firstZoneOnGround = 0;
	if(useRules != 0) {
		for(; rule->sceneId != -1; rule++) {
			if(rule->sceneId == sceneId &&
				(rule->entrance == -1 || rule->entrance == entrance)) {
				if(rule->flg == -1 || gmFlagGetStub(rule->flg) == 0) {
					zone = rule->firstZone;
					firstZoneOnGround = rule->firstZoneOnGround;
					break;
				}
			}
		}
	}

	/* Ground zero is normally safe; the rule can move the first zone onto
	   it. */
	*groundZones++ = 0;
	for(i = 0; i < ARRAY_COUNT(state->encounterZones) - 1; i++) {
		*groundZones++ = zone;
		if(zone != 0)
			zone++;
	}
	if(firstZoneOnGround != 0) {
		state->encounterZones[0] = state->encounterZones[1];
		state->encounterZones[1] = 0;
	}
	state->encounterBias = 0;
	/* fdEncounterCountStepsAndSelectFormation starts a battle once
	   MainRoutineStateWork.encounterSteps reaches it. */
	state->encounterThreshold = toFIXED(16);
	fdBattleBackgroundSelectForMap();
}

/* Returns the World Map encounter zone for the ground at pos (0 for none), and
   chooses the battle background for its battle environment
   (MainRoutineStateWork.battleBg). */
Sint32 fdEncounterResolveWorldMapZoneAndBackdrop(void *pos)
{
	Sint32							region;
	const WORLD_MAP_ENCOUNTER_RULE *rule = WorldMapEncounterRules;
	Sint32 environment = fdWorldMapGetBattleEnvironmentStub(pos, &region);
	Sint32 zone = 0;

	for(; rule->region != -1; rule++) {
		if(rule->region == region &&
			(rule->environment == -1 || rule->environment == environment) &&
			(rule->flg == -1 || gmFlagGetStub(rule->flg) == 0)) {
			zone = rule->zone;
			break;
		}
	}
	fdBattleBackgroundSelectForEnvironment(environment);
	return zone;
}

/* With Jenna in the party, random battles stop once her experience is past
   this. */
#define JENNA_ENCOUNTERS_EXPERIENCE_MAX 130
/* A battle background rule's word (battle_background_rules.c's
   BATTLE_BACKGROUND): bit 31 marks a battle background, its resource id below;
   any other word is a scene. */
#define BATTLE_BACKGROUND_RULE			0x80000000
#define BATTLE_BACKGROUND_RULE_RESOURCE 0xffff

/* Advances the encounter counter for zone and returns the formation to fight (a
   row of formations.c), or 0 for no battle. steps (16.16) scales how far the
   counter moves; a battle starts once it reaches encounterThreshold. */
Sint32 fdEncounterCountStepsAndSelectFormation(Sint32 zone, FIXED steps)
{
	FIELD_STATE	   *state;
	ENCOUNTER_ZONE *entry;
	Sint32			rate;
	Sint32			level;
	Sint32			bias;
	FIXED			step;
	Sint32			total;
	Sint32			sum;
	Sint32			pick;
	Sint32			i;

	state = FieldStateWork;
	/* UB: FLAG_IGNORE_ENCOUNTER_CHECKS reaches the draw without setting
	   entry. */
#if FIX_UNINITIALIZED_READ
	entry = (ENCOUNTER_ZONE *)EncounterZones + zone;
#endif
	if(gmFlagGetStub(FLAG_IGNORE_ENCOUNTER_CHECKS) == 0) {
		if(gmFlagGetStub(FLAG_NO_ENCOUNTERS) != 0 ||
			gmFlagGetStub(FLAG_NO_ENCOUNTERS_IN_AREA) != 0)
			return 0;
		if(zone == 0)
			return 0;
		if(MainRoutineStateWork.cloaked != 0)
			return 0;
		entry = (ENCOUNTER_ZONE *)EncounterZones + zone;
		rate = entry->rate;
		if(rate == 0)
			return 0;
		if(gmFlagGetStub(FLAG_IN_PARTY(JENNA)) != 0) {
			level = gmUnitGetStub(JENNA)->experience;
			if(level > JENNA_ENCOUNTERS_EXPERIENCE_MAX)
				return 0;
		}

		/* Each level (up to five) the party's average is above the zone's
		   raises the rate by 5; with Avoid active such a zone has no encounters
		   at all. */
		level = gmPartyGetAverageLevelStub() - entry->level;
		CLAMP(level, 0, 5);
		if(level > 0 && MainRoutineStateWork.avoidDistance != 0)
			return 0;
		rate += level * 5;

		/* A random bias varies the distance between battles: two differences of
		   random numbers, halved, so from -65535 to 65535 and most often near
		   0. */
		bias = state->encounterBias;
		if(bias == 0) {
			Sint32 a = syRandom();
			Sint32 b = syRandom();
			Sint32 c = syRandom();

			bias = (a - b + c - (Sint32)syRandom()) / 2;
			state->encounterBias = bias;
		}
		/* The rate and the bias divide the step, so a higher rate (a party
		   above the zone's level) means smaller steps and fewer battles: a step
		   is 16 / (16 * rate + 16 * (rate - 1) * bias), that is 1 / (rate +
		   (rate - 1) * bias) with bias a 16.16 fraction from -1 to 1. */
		step =
			syDivFX(rate * toFIXED(16) + (rate - 1) * 16 * bias, toFIXED(16));
		total = MainRoutineStateWork.encounterSteps += syMulFX(step, steps);
		if(total < state->encounterThreshold)
			return 0;
	}

	/* Draw one of the zone's formations by weight. */
	state->encounterBias = 0;
	sum = 0;
	for(i = 0; i < ENCOUNTER_GROUP_COUNT; i++)
		sum += entry->weights[i];
	/* Zone zero stays encounter-free even when FLAG_IGNORE_ENCOUNTER_CHECKS
	   bypasses the rate checks. */
	if(sum == 0)
		return 0;
	/* syRandom is below 65536, so this draw is below sum and must stop at one
	   of the ENCOUNTER_GROUP_COUNT formations. */
	pick = (syRandom() * sum) >> 16;
	pick -= entry->weights[0];
	i = 0;
	if(pick >= 0) {
		do {
			i++;
			if(i >= ENCOUNTER_GROUP_COUNT)
				break;
			pick -= entry->weights[i];
		} while(pick >= 0);
	}
	sum = entry->formations[i];
	fdEncounterSelectBattleMusic(zone, i);
	return sum;
}

/* Rolls a random battle on the encounter zone of the ground underfoot. terrain:
   the ground, an index into state->encounterZones: 0 for ordinary ground, 1 and
   2 for METATILE_BEHAVIOR_ENCOUNTER_GROUND_A and _B metatiles. steps:
   the step weight, passed on to fdEncounterCountStepsAndSelectFormation. */
Sint32 fdEncounterRollMapStep(Uint32 terrain, FIXED steps)
{
	Uint8 *zone;

	zone = FieldStateWork->encounterZones;
	return fdEncounterCountStepsAndSelectFormation(zone[terrain], steps);
}

#include "field/encounter_roll_world_map.c"

/* A ENCOUNTER_ZONE in halfwords, and where its formations start. */
#define ENCOUNTER_ZONE_HALFWORDS \
	((Sint32)(sizeof(ENCOUNTER_ZONE) / sizeof(Uint16)))
#define ENCOUNTER_ZONE_FORMATIONS \
	((Sint32)(offsetof(ENCOUNTER_ZONE, formations) / sizeof(Uint16)))

/* Returns the formation in slot (0 to 7) of zone, read from EncounterZones as
   halfwords. */
Uint16 fdEncounterGetFormationAtSlot(Sint32 zone, Sint32 slot)
{
	return EncounterZones[zone * ENCOUNTER_ZONE_HALFWORDS + slot +
		ENCOUNTER_ZONE_FORMATIONS];
}

/* Returns the formation of Djinni battle djinni, which the table gives as a
   zone and slot. */
Sint16 fdEncounterGetDjinnFormation(Sint32 djinni)
{
	const DJINNI_BATTLE *battle = &EncounterDjinnBattles[djinni];

#if FIX_FUNCTION_SIGNATURES
	return fdEncounterGetFormationAtSlot(battle->zone, battle->slot);
#else
	/* UB: no return statement; callers get the formation only because it is the
	   last call's result. */
	fdEncounterGetFormationAtSlot(battle->zone, battle->slot);
#endif
}

/* Chooses the current scene's field music (MainRoutineStateWork.snd) from the
   first rule matching the scene or its area and the entrance; SOUND_KEEP_BGM
   when none does. */
void fdChooseMusic(void)
{
	Sint16				  music = SOUND_KEEP_BGM;
	Sint16				  sceneId = MainRoutineStateWork.next;
	Sint16				  entrance = MainRoutineStateWork.entrance;
	Sint16				  area = MainRoutineStateWork.area;
	const MAP_MUSIC_RULE *rule = SceneMusicRules;

	for(; rule->id != -1; rule++) {
		if(rule->idIsMap) {
			if(rule->id != sceneId)
				continue;
			if(rule->entrance != -1 && rule->entrance != entrance)
				continue;
			if(rule->flg != -1 && gmFlagGetStub(rule->flg) == 0)
				continue;
			music = rule->music;
			break;
		}
		if(rule->id != area)
			continue;
		if(rule->entrance != -1 && rule->entrance != entrance)
			continue;
		if(rule->flg != -1 && gmFlagGetStub(rule->flg) == 0)
			continue;
		music = rule->music;
		break;
	}

	MainRoutineStateWork.sceneMusic = music;
}

/* Returns the area name of sceneId and entrance, from the first rule matching
   the scene or its area; 0, the Title's name, when none does. */
Sint32 fdAreaGetNameIndex(Sint32 sceneId, Sint32 entrance)
{
	const AREA_NAME_RULE *rule = AreaNameRules;
	Sint32				  name = 0;
	Sint32				  area = gmGetArea(sceneId);

	for(; rule->id != -1; rule++) {
		if(rule->useMapId) {
			if(rule->id == sceneId) {
				if(rule->entrance == -1 || rule->entrance == entrance) {
					name = rule->areaNameIdx;
					break;
				}
			}
		} else {
			if(rule->id == area) {
				if(rule->entrance == -1 || rule->entrance == entrance) {
					name = rule->areaNameIdx;
					break;
				}
			}
		}
	}
	return name;
}

/* Sets the scene the party returns to after a defeat
   (MainRoutineStateWork.restartScene and restartEntrance), from the first rule
   of SceneRestartRules that matches the current scene and entrance: {scene,
   entrance (-1 for any), return scene, return entrance}, ended by scene -1. */
void fdSetRestartPoint(void)
{
	Sint16		  sceneId = MainRoutineStateWork.next;
	Sint16		  entrance = MainRoutineStateWork.entrance;
	const Sint16 *rule;

	for(rule = SceneRestartRules; rule[0] != -1; rule += 4) {
		if(rule[0] == sceneId && (rule[1] == -1 || rule[1] == entrance)) {
			MainRoutineStateWork.restartScene = rule[2];
			MainRoutineStateWork.restartEntrance = rule[3];
			return;
		}
	}
}

#include "field/battle_background_get_selected.c"

/* Chooses the battle background for the current scene.
   SceneBattleBackgroundRules lists each battle background (a word with bit 31
   set) followed by the scenes that use it, ended by 0; a scene takes the one
   listed last before it, and a scene not listed takes the last one. */
void fdBattleBackgroundSelectForMap(void)
{
	Sint32	tgt = MainRoutineStateWork.next;
	Sint32 *tbl = SceneBattleBackgroundRules;
	Sint32	entry = *tbl++;
	/* The first battle_background_rules.c word names a battle background. */
	Sint32 backdrop;

	while(entry != 0 && entry != tgt) {
		if(entry & BATTLE_BACKGROUND_RULE) {
			backdrop = entry & BATTLE_BACKGROUND_RULE_RESOURCE;
		}
		entry = *tbl++;
	}
	MainRoutineStateWork.battleBg = backdrop;
}

/* Chooses the battle background for a World Map battle environment (enum
   BATTLE_ENVIRONMENT; any other gives Sol Sanctum's) and sets
   MainRoutineStateWork.battleBg. */
void fdBattleBackgroundSelectForEnvironment(Sint32 environment)
{
	Uint16 backdrop;

	switch(environment) {
	case BATTLE_ENVIRONMENT_GRASSLAND:
		backdrop = RESOURCE_GRASSLAND_BACKDROP;
		break;
	case BATTLE_ENVIRONMENT_FOREST:
		backdrop = RESOURCE_FOREST_BACKDROP;
		break;
	case BATTLE_ENVIRONMENT_DESERT:
		backdrop = RESOURCE_DESERT_BACKDROP;
		break;
	case BATTLE_ENVIRONMENT_BEACH:
	case BATTLE_ENVIRONMENT_SHALLOWS:
		backdrop = RESOURCE_BEACH_BACKDROP;
		break;
	case BATTLE_ENVIRONMENT_SNOWFIELD:
	case BATTLE_ENVIRONMENT_SNOW_FOREST:
		backdrop = RESOURCE_SNOWFIELD_BACKDROP;
		break;
	default:
		backdrop = RESOURCE_SOL_SANCTUM_BACKDROP;
		break;
	}
	MainRoutineStateWork.battleBg = backdrop;
}

/* Chooses the battle music for slot of zone
   (MainRoutineStateWork.battleMusic). */
void fdEncounterSelectBattleMusic(Sint32 zone, Sint32 slot)
{
	Uint16		  key = ENCOUNTER_KEY(zone, slot);
	const Uint16 *tbl = EncounterBattleMusic;
	/* EncounterBattleMusic opens with a music marker, so the loop assigns
	   music before either stopping test holds. */
	Uint16 music;
	Uint16 value;

	if(gmFlagGetStub(FLAG_LINK_BATTLE)) {
		music = SOUND_KEEP_BGM;
	} else {
		for(;;) {
			value = *tbl++;
			if(value == 0)
				break;
			if(value == key)
				break;
			if(value & ENCOUNTER_BATTLE_MUSIC_MARKER)
				music = value & ENCOUNTER_BATTLE_MUSIC_MASK;
		}
	}
	MainRoutineStateWork.battleMusic = music;
}

#undef JENNA_ENCOUNTERS_EXPERIENCE_MAX
#undef BATTLE_BACKGROUND_RULE
#undef BATTLE_BACKGROUND_RULE_RESOURCE
#undef ENCOUNTER_ZONE_HALFWORDS
#undef ENCOUNTER_ZONE_FORMATIONS
