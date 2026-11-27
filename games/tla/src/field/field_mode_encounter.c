/* Random encounters, and each scene's music, area name and battle background.
   Part of field_mode.c. */

#include "sounds.h"
#include "field/world_map_encounter_rules.h"
#include "game/stubs.h"

/* Marks the world map place the party has just entered as visited: sets
   FLAG_PLACE_VISITED of the first place whose visitScene is `id` and whose
   visitEntrance is `variant` (or any, for -1). The main routine calls it at
   every scene entry; fdMapScreenShow offers the marked places. */
void fdWorldMapMarkPlaceVisited(Sint32 id, Sint32 variant)
{
	const WORLD_MAP_PLACE *entry = fdWorldMapPlacesGetTable();

	for(; entry->flg != 0; entry++) {
		if(entry->visitScene != id)
			continue;
		if(entry->visitEntrance != -1 && variant != entry->visitEntrance)
			continue;
		syGameFlagSet(FLAG_PLACE_VISITED(entry->flg));
		break;
	}
}

/* Returns the value of the flag entry whose flag is flag, or 1 when there is
   none or its value is 0. */
Sint32 fdFlagEntriesGetValue(Sint32 flg)
{
	const WORLD_MAP_PLACE *entry = fdWorldMapPlacesGetTable();

	for(; entry->flg != 0; entry++) {
		if(entry->flg == flg && entry->value != 0)
			return entry->value;
	}
	return 1;
}

/* Chooses the current scene's encounter zones, one for each kind of ground the
   party can stand on (FieldStateWork->encounterZones, read by
   fdEncounterRollMapStep), from the first rule matching the scene and
   entrance. */
void fdEncounterSelectMapZones(Sint32 useRules)
{
	FIELD_STATE				   *state = RamHeapSlots->fieldState;
	Sint32						sceneId = MainRoutineStateWork.next;
	Sint32						entrance = MainRoutineStateWork.entrance;
	Uint8					   *groundZones = state->encounterZones;
	Uint32						first = 0;
	Uint32						second = 0;
	const SCENE_ENCOUNTER_RULE *rule = SceneEncounterRules;
	Uint32						i;

	if(useRules != 0) {
		for(; rule->scene != -1; rule++) {
			if(rule->scene != sceneId)
				continue;
			if(rule->entrance != -1 && rule->entrance != entrance)
				continue;
			if(rule->flg != -1 && syGameFlagGet(rule->flg))
				continue;
			first = rule->groundZone;
			second = rule->secondZone;
			break;
		}
	}
	*groundZones++ = first;
	*groundZones++ = second;
	for(i = 0; i < ARRAY_COUNT(state->encounterZones) - 2; i++)
		*groundZones++ = 0;
	state->encounterBias = 0;
	/* fdEncounterCountStepsAndSelectFormation starts a battle once
	   MainRoutineStateWork.encounterSteps reaches it. */
	state->encounterThreshold = toFIXED(16);
	fdBattleBackgroundSelectForMap();
}

/* The World Map's encounter regions: a block's region number past them takes
   the encounters of its number modulo WORLD_MAP_ENCOUNTER_REGIONS (Prox's
   blizzard, 36, those of 16), the higher numbers choosing only the ground's
   battle environment (fdWorldMapGetBattleEnvironment). */
#define WORLD_MAP_ENCOUNTER_REGIONS 20

/* Returns the World Map encounter zone for the ground at position (0 for none),
   and chooses the battle background for its battle environment
   (MainRoutineStateWork.battleBg). Flying, there are none; aboard the ship, the
   zones are WorldMapShipEncounterRules'; a rule's flag must be set for it to
   apply. */
Sint32 fdEncounterResolveWorldMapZoneAndBackdrop(void *pos)
{
	const WORLD_MAP_ENCOUNTER_RULE *rule = WorldMapEncounterRules;
	Sint32							environment;
	Uint32							region;
	Sint32							zone;
	MAIN_ROUTINE_STATE			   *party = &MainRoutineStateWork;

	if(party->movementMode == LEADER_MOVEMENT_FLYING_SHIP)
		return 0;
	if(party->movementMode == LEADER_MOVEMENT_SHIP)
		rule = WorldMapShipEncounterRules;
	environment = fdWorldMapGetBattleEnvironmentStub(pos, (Sint32 *)&region);
	region %= WORLD_MAP_ENCOUNTER_REGIONS;
	zone = 0;
	for(; rule->region != -1; rule++) {
		if(rule->region != region)
			continue;
		if(rule->environment != -1 && rule->environment != environment)
			continue;
		if(rule->flg != -1 && syGameFlagGet(rule->flg) == 0)
			continue;
		zone = rule->zone;
		break;
	}
	fdBattleBackgroundSelectForEnvironment(environment);
	return zone;
}

/* Advances the encounter counter for zone and returns the formation to fight,
   or 0 for no battle. steps (16.16) scales how far the counter moves; a battle
   starts once it reaches encounterThreshold. */
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
	Sint32			formation;
	Sint32			i;

	state = RamHeapSlots->fieldState;
	/* UB: FLAG_IGNORE_ENCOUNTER_CHECKS reaches the draw without setting
	   entry. */
#if FIX_UNINITIALIZED_READ
	entry = EncounterZones + zone;
#endif
	if(syGameFlagGet(FLAG_IGNORE_ENCOUNTER_CHECKS) == 0) {
		if(syGameFlagGet(FLAG_NO_ENCOUNTERS) != 0 ||
			syGameFlagGet(FLAG_NO_ENCOUNTERS_IN_AREA) != 0)
			return 0;
		if(zone == 0)
			return 0;
		if(MainRoutineStateWork.cloaked != 0)
			return 0;
		entry = EncounterZones + zone;
		rate = entry->rate;
		if(rate == 0)
			return 0;

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
	for(i = 0; i < (Sint32)ARRAY_COUNT(entry->weights); i++)
		sum += entry->weights[i];
	if(sum == 0)
		return 0;
	pick = syRandomBelow(sum);
	for(i = 0; i < (Sint32)ARRAY_COUNT(entry->weights); i++) {
		pick -= entry->weights[i];
		if(pick < 0)
			break;
	}
	formation = (Uint16)entry->formations[i];
	fdEncounterSelectBattleMusic(zone, i);
	return formation;
}

/* Rolls a random battle on the encounter zone of the ground underfoot. terrain:
   the ground, an index into the field state's encounterZones: 0 for ordinary
   ground, 1 for a MAP_METATILE_SECOND_ZONE metatile. steps: the step weight,
   passed on to fdEncounterCountStepsAndSelectFormation. */
Sint32 fdEncounterRollMapStep(Uint32 terrain, FIXED steps)
{
	Uint8 *zone = FieldStateWork->encounterZones;

	return fdEncounterCountStepsAndSelectFormation(zone[terrain], steps);
}

/* Rolls a random battle on the World Map, taking the zone from the metatile at
   pos. Returns the formation to fight, or 0 for no battle. */
Sint32 fdEncounterRollWorldMap(Sint32 pos, FIXED steps)
{
	return fdEncounterCountStepsAndSelectFormation(
		fdEncounterResolveWorldMapZoneAndBackdrop((void *)pos), steps);
}

/* Returns the formation in slot (0 to 7) of zone. */
Uint16 fdEncounterGetFormationAtSlot(Sint32 zone, Sint32 slot)
{
	return EncounterZones[zone].formations[slot];
}

/* Returns the formation of Djinni battle djinni, which the table gives as a
   zone and slot. */
Uint16 fdEncounterGetDjinnFormation(Sint32 djinni)
{
	Uint16 *zoneAndSlot = (Uint16 *)(EncounterDjinnBattles + djinni * 4);

#if FIX_FUNCTION_SIGNATURES
	return fdEncounterGetFormationAtSlot(zoneAndSlot[0], zoneAndSlot[1]);
#else
	/* UB: no return statement; the original leaves the called getter's result
	   in r0. */
	fdEncounterGetFormationAtSlot(zoneAndSlot[0], zoneAndSlot[1]);
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
	Sint16				  group = MainRoutineStateWork.area;
	const MAP_MUSIC_RULE *rule = SceneMusicRules;

	for(; rule->id != -1; rule++) {
		if(rule->byScene) {
			if(rule->id != sceneId)
				continue;
			if(rule->entrance != -1 && rule->entrance != entrance)
				continue;
			if(rule->flg != -1 && syGameFlagGet(rule->flg) == 0)
				continue;
			music = rule->music;
			break;
		} else {
			if(rule->id != group)
				continue;
			if(rule->entrance != -1 && rule->entrance != entrance)
				continue;
			if(rule->flg != -1 && syGameFlagGet(rule->flg) == 0)
				continue;
			music = rule->music;
			break;
		}
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
		if(rule->useSceneId) {
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
   of SceneReturnRules that matches the current scene and entrance: rows of
   eight halfwords that open {scene, entrance (-1 for any), return scene, return
   entrance}, ended by scene -1. Without a match the restart point stays as it
   was. */
void fdSetRestartPoint(void)
{
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	Sint16				sceneId;
	Sint16				entrance;
	const Sint16	   *rule;

	sceneId = party->next;
	entrance = party->entrance;

	for(rule = SceneReturnRules; rule[0] != -1; rule += 8) {
		if(rule[0] == sceneId && (rule[1] == -1 || rule[1] == entrance)) {
			party->restartScene = rule[2];
			party->restartEntrance = rule[3];
			return;
		}
	}
}

/* Where the party comes out on the world map after reaching scene and entrance
   (Teleport's choice calls it). With FLAG_SHIP_CAN_SAIL set (the party has the
   ship, whose mooring the map screen marks), finds the first rule of
   SceneReturnRules for scene and entrance (-1 on either side matches any
   entrance) and copies its position, x and z in 16.16 and the third value as
   is, to MainRoutineStateWork.shipX, shipZ and shipFacing. */
void fdWorldMapSetReturnForScene(Sint32 scene, Sint32 entrance)
{
	const Sint16 *rule = SceneReturnRules;

	if(syGameFlagGet(FLAG_SHIP_CAN_SAIL) == 0)
		return;
	for(; rule[0] != -1; rule += 8) {
		if(rule[0] == scene &&
			(rule[1] == -1 || entrance == -1 || rule[1] == entrance)) {
			MainRoutineStateWork.shipX = toFIXED(rule[4]);
			MainRoutineStateWork.shipZ = toFIXED(rule[5]);
			MainRoutineStateWork.shipFacing = rule[6];
			return;
		}
	}
}

/* fdWorldMapSetReturnForScene's twin for the rule's return scene and entrance
   (its third and fourth values): with FLAG_SHIP_CAN_SAIL set, clears
   ownerOverride and copies the rule's position to the party's saved
   position. */
void fdWorldMapSetReturnLeavingScene(Sint32 scene, Sint32 entrance)
{
	const Sint16 *rule = SceneReturnRules;

	if(syGameFlagGet(FLAG_SHIP_CAN_SAIL) == 0)
		return;
	MainRoutineStateWork.ownerOverride = 0;
	for(; rule[0] != -1; rule += 8) {
		if(rule[2] == scene &&
			(rule[3] == -1 || entrance == -1 || rule[3] == entrance)) {
			MainRoutineStateWork.shipX = toFIXED(rule[4]);
			MainRoutineStateWork.shipZ = toFIXED(rule[5]);
			MainRoutineStateWork.shipFacing = rule[6];
			return;
		}
	}
}

/* Returns the battle background chosen for the current scene. */
Sint16 fdBattleBackgroundGetSelected(void)
{
	return MainRoutineStateWork.battleBg;
}

/* A battle background rule's word (scene_rules.c's BATTLE_BACKGROUND): bit 31
   marks a battle background, its resource id below; any other word is a
   scene. */
#define BATTLE_BACKGROUND_RULE			0x80000000
#define BATTLE_BACKGROUND_RULE_RESOURCE 0xffff

/* Chooses the battle background for the current scene.
   SceneBattleBackgroundRules lists each battle background (a word with bit 31
   set) followed by the scenes that use it, ended by 0; a scene takes the one
   listed last before it, and a scene not listed takes the last one. Once the
   Mars Lighthouse has thawed, the second dragon hall backdrop becomes the
   first. */
void fdBattleBackgroundSelectForMap(void)
{
	Sint32		  tgt = MainRoutineStateWork.next;
	const Sint32 *tbl = SceneBattleBackgroundRules;
	Sint32		  entry = *tbl++;
	/* The first scene_rules.c word names a battle background. */
	Sint32 backdrop;

	if(entry != 0 && entry != tgt) {
		do {
			if(entry & BATTLE_BACKGROUND_RULE) {
				backdrop = entry & BATTLE_BACKGROUND_RULE_RESOURCE;
			}
			entry = *tbl++;
		} while(entry != 0 && entry != tgt);
	}
	if(backdrop == RESOURCE_DRAGON_HALL_B_BACKDROP &&
		syGameFlagGet(FLAG_MARS_LIGHTHOUSE_THAWED))
		backdrop = RESOURCE_DRAGON_HALL_A_BACKDROP;
	MainRoutineStateWork.battleBg = backdrop;
}

/* Chooses the battle background for a World Map battle environment (enum
   BATTLE_ENVIRONMENT; any other gives the ship deck's) and sets
   MainRoutineStateWork.battleBg. */
void fdBattleBackgroundSelectForEnvironment(Sint32 environment)
{
	Uint16 backdrop;

	switch(environment - 1) {
	case BATTLE_ENVIRONMENT_GRASSLAND - 1:
		backdrop = RESOURCE_GRASSLAND_B_BACKDROP;
		break;
	case BATTLE_ENVIRONMENT_FOREST - 1:
		backdrop = RESOURCE_DENSE_FOREST_BACKDROP;
		break;
	case BATTLE_ENVIRONMENT_DESERT - 1:
		backdrop = RESOURCE_DESERT_BACKDROP;
		break;
	case BATTLE_ENVIRONMENT_BEACH - 1:
	case BATTLE_ENVIRONMENT_SHALLOWS - 1:
		backdrop = RESOURCE_ROCKY_BEACH_BACKDROP;
		break;
	case BATTLE_ENVIRONMENT_SNOWFIELD - 1:
	case BATTLE_ENVIRONMENT_SNOW_FOREST - 1:
		backdrop = RESOURCE_SNOWFIELD_B_BACKDROP;
		break;
	case BATTLE_ENVIRONMENT_BLIZZARD - 1:
	case BATTLE_ENVIRONMENT_BLIZZARD_FOREST - 1:
		backdrop = RESOURCE_BLIZZARD_BACKDROP;
		break;
	default:
		backdrop = RESOURCE_SHIP_DECK_BACKDROP;
		break;
	}
	MainRoutineStateWork.battleBg = backdrop;
}

/* Whether the character is one of the first PARTY_BATTLE_SIZE party members,
   the ones in battle. */
Bool fdPartyIsInFrontRow(Sint32 owner)
{
	Sint32 cnt = gmPartyCountMembersStub();
	Sint32 i;

	if(cnt > PARTY_BATTLE_SIZE)
		cnt = PARTY_BATTLE_SIZE;
	for(i = 0; i < cnt; i++) {
		if(MainRoutineStateWork.party[i] == owner)
			return TRUE;
	}
	return FALSE;
}

/* Chooses the battle music for slot of zone (MainRoutineStateWork.battleMusic):
   a key takes the song listed last before it in EncounterBattleMusic, and a
   link battle (FLAG_LINK_BATTLE) keeps the music playing. With no song found,
   it is MUS_BATTLE_ON_THE_SEA on the ship before it flies, else by the front
   row: MUS_BATTLE_THE_LOST_AGE with Felix, MUS_BATTLE_THE_BROKEN_SEAL with
   Isaac, MUS_BATTLE_JENNA with Jenna, or MUS_BATTLE_THE_LOST_AGE. */
void fdEncounterSelectBattleMusic(Sint32 zone, Sint32 slot)
{
	Sint16		  key = (zone << 4) + slot;
	const Sint16 *tbl = EncounterBattleMusic;
	Uint16		  music;
	Uint16		  value;

	if(syGameFlagGet(FLAG_LINK_BATTLE)) {
		music = SOUND_KEEP_BGM;
	} else {
		for(;;) {
			value = *tbl++;
			if(value == 0)
				break;
			if(value == (Uint16)key)
				break;
			if(value & ENCOUNTER_BATTLE_MUSIC_MARKER)
				music = value & 0xfff;
		}
	}
	if(music == 0) {
		if(MainRoutineStateWork.ownerOverride != 0 &&
			!syGameFlagGet(FLAG_SHIP_FLIES))
			music = MUS_BATTLE_ON_THE_SEA;
		else if(fdPartyIsInFrontRow(FELIX))
			music = MUS_BATTLE_THE_LOST_AGE;
		else if(fdPartyIsInFrontRow(ISAAC))
			music = MUS_BATTLE_THE_BROKEN_SEAL;
		else if(fdPartyIsInFrontRow(JENNA))
			music = MUS_BATTLE_JENNA;
		else
			music = MUS_BATTLE_THE_LOST_AGE;
	}
	MainRoutineStateWork.battleMusic = music;
}

#undef WORLD_MAP_ENCOUNTER_REGIONS
#undef BATTLE_BACKGROUND_RULE
#undef BATTLE_BACKGROUND_RULE_RESOURCE
