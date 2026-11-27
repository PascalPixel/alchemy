/* The party's units and their stats, a new game, the prologue, HP and PP. Part
   of game.c. */

#include "window/message_speed.h"

/* EWRAM from SceneLocalState on, as a new game clears it: the scene's state,
   the equipment put aside for the Colosso and the actors kept across a
   battle. */
typedef struct SCENE_STATE_VIEW {
	Uint8		   sceneState[120];		  /* SceneLocalState */
	Uint16		   inventorySnapshot[70]; /* InventorySnapshotWork */
	ACTOR_SNAPSHOT actorSnapshot;		  /* ActorSnapshotWork */
} SCENE_STATE_VIEW;

/* The running totals gmUnitCalc builds a unit's stats in. */
typedef struct STAT_STATE {
	Sint32 hp;		  /* maximum HP */
	Sint32 pp;		  /* maximum PP */
	Sint32 attack;	  /* Attack, 0 to UNIT_STAT_MAX once clamped */
	Sint32 defense;	  /* Defense, 0 to UNIT_STAT_MAX once clamped */
	Sint32 agility;	  /* Agility, 0 to UNIT_STAT_MAX once clamped */
	Sint32 unused;	  /* never written; a gap between Agility and Luck */
	Sint32 luck;	  /* Luck, 0 to UNIT_LUCK_MAX once clamped */
	Sint32 actionCnt; /* actions a turn, 0 to 2 */
	Sint32 hpRegen;	  /* HP regained each turn */
	Sint32 ppRegen;	  /* PP regained each turn */

	struct {
		Sint32 power;
		Sint32 resistance;
	} element[ELEMENT_COUNT]; /* each element's power and resistance */

	Sint32 kind;			   /* the item effect being applied */
	Sint32 unusedAfterKind[2]; /* never written */
	Sint32 amount;			   /* the amount of the item effect being applied */
	const ITEM_DEFINITION *item; /* the equipped item being applied */
	Sint32				   unusedAfterItem; /* never written */
} STAT_STATE;

/* A new game's settings: voice sounds "On" (MsgSpeechSetting + 1) and "Window
   brightness" one step brighter than the hue's own colour. */
#define NEW_GAME_SPEECH			   1
#define NEW_GAME_WINDOW_BRIGHTNESS (WINDOW_BRIGHTNESS_NEUTRAL + 1)

/* The distance between `a` and `b`, |a - b|. */
#define UNIT_STAT_DIFF(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

#include "game/restart_quest.c"

/* Returns a side's Djinn recovery table, its standby Djinn and their turns to
   recover: the party's (DJINN_RECOVERY_PARTY), kept in MainRoutineStateWork, or
   the opponent's (DJINN_RECOVERY_OPPONENT), in OPPONENT_RECOVERY_UNIT's
   record. */
DJINN_RECOVERY_TABLE *gmDjinnGetRecoveryTable(Sint32 enemySide)
{
	if(enemySide != 0)
		return (DJINN_RECOVERY_TABLE *)gmUnitGet(OPPONENT_RECOVERY_UNIT);
	return (DJINN_RECOVERY_TABLE *)&MainRoutineStateWork.holdings;
}

/* Returns the average level of the party members, rounded down (0 with no party
   members). */
Uint32 gmPartyGetAverageLevel(void)
{
	Sint32 totalLevels = 0;
	Sint32 partySize = gmPartyCountMembers();
	Sint32 i;

	if(partySize == 0)
		return 0;
	for(i = 0; i < partySize; i++)
		totalLevels += gmUnitGet(MainRoutineStateWork.party[i])->level;
	totalLevels /= partySize;
	return totalLevels;
}

/* Returns the unit of unitId: ISAAC to PIERS the characters, the six from
   FIRST_ENEMY_UNIT the enemies of the current battle; NULL for anything
   else. */
UNIT *gmUnitGet(Uint32 unitId)
{
	UNIT *states;
	UNIT *unit;
	/* DEAD: the return address, read and never used; the ROM keeps its copy of
	   lr into r3. */
	UNUSED void *caller = __builtin_return_address(0);

	states = CharacterUnits;
	/* FAKEMATCH(dead): the party return repeated under a test of the return
	 * address. */
	if(caller == NULL) {
		if(unitId <= PIERS) {
			unit = &states[unitId];
			return unit;
		}
	}
	if(unitId <= PIERS) {
		unit = &states[unitId];
		return unit;
	}
	if(unitId - FIRST_ENEMY_UNIT < ENEMY_BATTLE_SIZE && EnemyUnits != NULL) {
		unit = &((UNIT *)EnemyUnits)[unitId - FIRST_ENEMY_UNIT];
		return unit;
	}
	return NULL;
}

/* Returns the definition of enemy `enemyId`; enemy ids start past the eight
   characters', so that one id names any unit. */
const ENEMY_DEFINITION *gmEnemyGetDefinition(Sint32 enemyId)
{
	Uint32 row = enemyId - ENEMY_ID_FIRST;

	/* BUG: only rows from ENEMY_ID_MAX are refused, so those from
	   ENEMY_DEFINITION_COUNT (165) to ENEMY_ID_MAX - 1 read the tables after
	   it. gmBattleUnitSet refuses them itself. */
#if FIX_ENEMY_ID_BOUND
	if(row >= ENEMY_DEFINITION_COUNT) {
#else
	if(row > ENEMY_ID_MAX - 1U) {
#endif
		row = 0;
	}
	return &EnemyDefinitions[row];
}

/* Unused: copies `cnt` bytes from `first` to `second` when `direction` is
   set, and from `second` to `first` otherwise. */
UNUSED void gmMemoryCopyBytesDirectional(
	Uint8 *first, Uint8 *second, Sint32 cnt, Sint32 direction)
{
	Sint32 i;

	if(direction != 0) {
		for(i = 0; i < cnt; i++)
			*second++ = *first++;
	} else {
		for(i = 0; i < cnt; i++)
			*first++ = *second++;
	}
}

/* A scaling effect's number among the eight, from EQUIP_EFFECT_SCALE_HP. */
#define SCALE_EFFECT(kind)	((kind) - EQUIP_EFFECT_SCALE_HP)
/* The PP an Adept regains at the end of each battle turn, on top of its own,
   inside the lighthouse of its element. */
#define LIGHTHOUSE_PP_REGEN 4

/* Works out a unit's stats from its base stats, equipment, set Djinn, class and
   battle stat levels; its HP and PP keep their share of a new maximum. */
void gmUnitCalc(Sint32 unitId)
{
	STAT_STATE *stats;
	UNIT	   *unit;
	Sint32		idx, effectIdx, elementIdx;
	Sint32		value;
	Sint32		inOwnLighthouse;
	Sint32		effectKind;
	Sint32		effectAmount;
	Sint32		statLimit;
	Sint16		prevMaximum;

	/* Start with the unit's base stats and elemental power/resistance. */
	stats = syHeapAllocEWRAM(sizeof(STAT_STATE));
	unit = gmUnitGet(unitId);
	stats->hp = unit->baseHp;
	stats->pp = unit->basePp;
	stats->attack = unit->baseAttack;
	stats->defense = unit->baseDefense;
	stats->agility = unit->baseAgility;
	stats->luck = unit->baseLuck;
	stats->actionCnt = unit->baseActionCnt & UNIT_ACTION_COUNT_MASK;
	stats->hpRegen = unit->baseHpRegen;
	stats->ppRegen = unit->basePpRegen;
	for(idx = 0; idx < ELEMENT_COUNT; idx++) {
		stats->element[idx].power = unit->baseElement[idx].power;
		stats->element[idx].resistance = unit->baseElement[idx].resistance;
	}

	/* HP and PP keep their share of the maximum; gauges that no longer match
	   are reset to full first. */
	if(UNIT_STAT_DIFF(unit->hpMax * unit->hpGauge / UNIT_GAUGE_FULL, unit->hp) >
			1 ||
		UNIT_STAT_DIFF(
			unit->ppMax * unit->ppGauge / UNIT_GAUGE_FULL, unit->pp) > 1) {
		unit->hpGauge = UNIT_GAUGE_FULL;
		unit->ppGauge = UNIT_GAUGE_FULL;
		unit->hp = unit->hpMax;
		unit->pp = unit->ppMax;
	}

	/* CURSE_KEEP_BOUND keeps CURSE_BOUND, and CURSE_REMOVABLE clears both again
	   below. */
	unit->curse &= ~(CURSE_BOUND | CURSE_ITEM);
	if(unit->curse & CURSE_KEEP_BOUND)
		unit->curse |= CURSE_BOUND;
	if(unit->ready)
		stats->actionCnt++;
	unit->equipBonus[0] = 0;
	unit->equipBonus[1] = 0;

	/* Apply additive equipment and set-Djinn bonuses before class scaling. An
	   enemy (CLASS_NONE) skips equipment, Djinn and class entirely. */
	if(unit->classId != CLASS_NONE) {
		for(idx = 0; idx < INVENTORY_SLOTS; idx++) {
			if(!(unit->inventory[idx] & INVENTORY_EQUIPPED))
				continue;
			stats->item = gmItemGetDirect(unit->inventory[idx]);
			if(stats->item->flg & ITEM_CURSED)
				unit->curse |= CURSE_BOUND | CURSE_ITEM;
			stats->attack += stats->item->attack;
			stats->defense += stats->item->defense;
			for(effectIdx = 0; effectIdx < EQUIP_EFFECT_SLOTS; effectIdx++) {
				effectKind = stats->item->effs[effectIdx].kind;
				effectAmount = stats->item->effs[effectIdx].amount;
				stats->kind = effectKind;
				stats->amount = effectAmount;
				switch(effectKind) {
				case EQUIP_EFFECT_NONE:
					break;
				case EQUIP_EFFECT_ADD_HP:
					stats->hp += stats->amount;
					break;
				case EQUIP_EFFECT_ADD_HP_REGEN:
					stats->hpRegen += stats->amount;
					break;
				case EQUIP_EFFECT_ADD_PP:
					stats->pp += stats->amount;
					break;
				case EQUIP_EFFECT_ADD_PP_REGEN:
					stats->ppRegen += stats->amount;
					break;
				case EQUIP_EFFECT_ADD_AGILITY:
					stats->agility += stats->amount;
					break;
				case EQUIP_EFFECT_ADD_LUCK:
					stats->luck += stats->amount;
					break;
				case EQUIP_EFFECT_ADD_VENUS_POWER:
					stats->element[ELEMENT_VENUS].power += stats->amount;
					break;
				case EQUIP_EFFECT_ADD_MERCURY_POWER:
					stats->element[ELEMENT_MERCURY].power += stats->amount;
					break;
				case EQUIP_EFFECT_ADD_MARS_POWER:
					stats->element[ELEMENT_MARS].power += stats->amount;
					break;
				case EQUIP_EFFECT_ADD_JUPITER_POWER:
					stats->element[ELEMENT_JUPITER].power += stats->amount;
					break;
				case EQUIP_EFFECT_ADD_VENUS_RESIST:
					stats->element[ELEMENT_VENUS].resistance += stats->amount;
					break;
				case EQUIP_EFFECT_ADD_MERCURY_RESIST:
					stats->element[ELEMENT_MERCURY].resistance += stats->amount;
					break;
				case EQUIP_EFFECT_ADD_MARS_RESIST:
					stats->element[ELEMENT_MARS].resistance += stats->amount;
					break;
				case EQUIP_EFFECT_ADD_JUPITER_RESIST:
					stats->element[ELEMENT_JUPITER].resistance += stats->amount;
					break;
				case EQUIP_EFFECT_ADD_UNLEASH_CHANCE:
					unit->equipBonus[0] += stats->amount;
					break;
				case EQUIP_EFFECT_ADD_COUNTERSTRIKE_RATE:
					unit->equipBonus[1] += stats->amount;
					break;
				case EQUIP_EFFECT_CURSE_REMOVABLE:
					unit->curse |= CURSE_REMOVABLE;
					break;
				case EQUIP_EFFECT_ADD_ACTIONS:
					stats->actionCnt += stats->amount;
					break;
				}
			}
		}
		if(unit->curse & CURSE_REMOVABLE)
			unit->curse &= ~(CURSE_REMOVABLE | CURSE_BOUND);

		/* Only set Djinn contribute their stat bonuses. */
		for(elementIdx = 0; elementIdx < ELEMENT_COUNT; elementIdx++) {
			Uint32 activeDjinn = unit->djinnSet[elementIdx];

			for(idx = 0; idx < DJINN_PER_ELEMENT; idx++) {
				if(activeDjinn & (1 << idx)) {
					const DJINN_DEFINITION *djinn =
						gmDjinnGetDefinition(elementIdx, idx);

					stats->hp += djinn->hp;
					stats->pp += djinn->pp;
					stats->attack += djinn->attack;
					stats->defense += djinn->defense;
					stats->agility += djinn->agility;
					stats->luck += djinn->luck;
				}
			}
		}

		/* The class's multipliers are in tenths (STAT_SCALE_ONE keeps a
		   stat). */
		{
			CLASS_DEFINITION *classDefinition =
				gmClassGetDefinition(unit->classId);

			stats->hp = stats->hp * classDefinition->hp / STAT_SCALE_ONE;
			stats->pp = stats->pp * classDefinition->pp / STAT_SCALE_ONE;
			stats->attack =
				stats->attack * classDefinition->attack / STAT_SCALE_ONE;
			stats->defense =
				stats->defense * classDefinition->defense / STAT_SCALE_ONE;
			stats->agility =
				stats->agility * classDefinition->agility / STAT_SCALE_ONE;
			stats->luck = stats->luck * classDefinition->luck / STAT_SCALE_ONE;
		}

		/* Then the equipment's multiplying effects, EQUIP_EFFECT_SCALE_HP to
		   EQUIP_EFFECT_SCALE_LUCK, also in tenths. */
		for(idx = 0; idx < INVENTORY_SLOTS; idx++) {
			if(!(unit->inventory[idx] & INVENTORY_EQUIPPED))
				continue;
			stats->item = gmItemGetDirect(unit->inventory[idx]);
			for(effectIdx = 0; effectIdx < EQUIP_EFFECT_SLOTS; effectIdx++) {
				effectKind = stats->item->effs[effectIdx].kind;
				effectAmount = stats->item->effs[effectIdx].amount;
				stats->kind = effectKind;
				effectKind = SCALE_EFFECT(effectKind);
				stats->amount = effectAmount;
				switch(effectKind) {
				case SCALE_EFFECT(EQUIP_EFFECT_SCALE_HP):
					stats->hp = stats->hp * stats->amount / STAT_SCALE_ONE;
					break;
				case SCALE_EFFECT(EQUIP_EFFECT_SCALE_HP_REGEN):
					stats->hpRegen =
						stats->hpRegen * stats->amount / STAT_SCALE_ONE;
					break;
				case SCALE_EFFECT(EQUIP_EFFECT_SCALE_PP):
					stats->pp = stats->pp * stats->amount / STAT_SCALE_ONE;
					break;
				case SCALE_EFFECT(EQUIP_EFFECT_SCALE_PP_REGEN):
					stats->ppRegen =
						stats->ppRegen * stats->amount / STAT_SCALE_ONE;
					break;
				case SCALE_EFFECT(EQUIP_EFFECT_SCALE_ATTACK):
					stats->attack =
						stats->attack * stats->amount / STAT_SCALE_ONE;
					break;
				case SCALE_EFFECT(EQUIP_EFFECT_SCALE_DEFENSE):
					stats->defense =
						stats->defense * stats->amount / STAT_SCALE_ONE;
					break;
				case SCALE_EFFECT(EQUIP_EFFECT_SCALE_AGILITY):
					stats->agility =
						stats->agility * stats->amount / STAT_SCALE_ONE;
					break;
				case SCALE_EFFECT(EQUIP_EFFECT_SCALE_LUCK):
					stats->luck = stats->luck * stats->amount / STAT_SCALE_ONE;
					break;
				}
			}
		}
	}

	/* Then the battle's changes apply: the Attack, Defense and Agility stages,
	   a summon's boost to each element's power, and the Resistance stage. */
	stats->attack = stats->attack * (unit->attackStage + 8) / 8;
	stats->defense = stats->defense * (unit->defenseStage + 8) / 8;
	stats->agility = stats->agility * (unit->agilityStage + 8) / 8;
	for(idx = 0; idx < ELEMENT_COUNT; idx++)
		stats->element[idx].power +=
			(unit->summonBoost[idx] * unit->summonBoost[idx] +
				unit->summonBoost[idx]) *
			5;
	for(idx = 0; idx < ELEMENT_COUNT; idx++)
		stats->element[idx].resistance +=
			unit->resistanceStage * RESISTANCE_PER_STAGE;

	if(unit->classId != CLASS_NONE) {
		inOwnLighthouse = 0;
		/* Faster PP recovery inside the lighthouse of the character's own
		   element. */
		switch(unit->id) {
		case ISAAC:
			inOwnLighthouse = gmFlagGet(FLAG_IN_VENUS_LIGHTHOUSE);
			break;
		case GARET:
			inOwnLighthouse = gmFlagGet(FLAG_IN_MARS_LIGHTHOUSE);
			break;
		case IVAN:
			inOwnLighthouse = gmFlagGet(FLAG_IN_JUPITER_LIGHTHOUSE);
			break;
		case MIA:
			inOwnLighthouse = gmFlagGet(FLAG_IN_MERCURY_LIGHTHOUSE);
			break;
		case JENNA:
			inOwnLighthouse = gmFlagGet(FLAG_IN_MARS_LIGHTHOUSE);
			break;
		}
		if(inOwnLighthouse)
			stats->ppRegen += LIGHTHOUSE_PP_REGEN;
	}

	CLAMP(stats->attack, 0, UNIT_STAT_MAX);
	CLAMP(stats->defense, 0, UNIT_STAT_MAX);
	CLAMP(stats->agility, 0, UNIT_STAT_MAX);
	CLAMP(stats->luck, 0, UNIT_LUCK_MAX);
	CLAMP(stats->actionCnt, 0, 2);
	CLAMP(stats->hpRegen, 0, UNIT_HP_REGEN_MAX);
	CLAMP(stats->ppRegen, 0, UNIT_PP_REGEN_MAX);
	for(idx = 0; idx < ELEMENT_COUNT; idx++) {
		CLAMP(stats->element[idx].power, 0, UNIT_ELEMENT_MAX);
		CLAMP(stats->element[idx].resistance, 0, UNIT_ELEMENT_MAX);
	}

	unit->attack = stats->attack;
	unit->defense = stats->defense;
	unit->agility = stats->agility;
	unit->luck = stats->luck;
	unit->actionsPerTurn = stats->actionCnt;
	unit->hpRegen = stats->hpRegen;
	unit->ppRegen = stats->ppRegen;
	for(idx = 0; idx < ELEMENT_COUNT; idx++) {
		unit->element[idx].power = stats->element[idx].power;
		unit->element[idx].resistance = stats->element[idx].resistance;
	}

	statLimit = UNIT_HP_MAX;
	if(unit->classId != CLASS_NONE)
		statLimit = UNIT_CHARACTER_HP_MAX;

	prevMaximum = unit->hpMax;
	CLAMP(stats->hp, 0, statLimit);
	unit->hpMax = stats->hp;
	if(prevMaximum != unit->hpMax) {
		value = stats->hp * unit->hpGauge / UNIT_GAUGE_FULL;
		CLAMP(value, 0, statLimit);
		if(unit->hp != 0 && value == 0)
			value = 1;
		unit->hp = value;
	}

	prevMaximum = unit->ppMax;
	CLAMP(stats->pp, 0, statLimit);
	unit->ppMax = stats->pp;
	if(prevMaximum != unit->ppMax) {
		value = stats->pp * unit->ppGauge / UNIT_GAUGE_FULL;
		CLAMP(value, 0, statLimit);
		if(unit->pp != 0 && value == 0)
			value = 1;
		unit->pp = value;
	}

	syHeapRelease(stats);
}

/* Sets FLAG_LURE_CAP when any party member has the Lure Cap (the one item with
   EQUIP_EFFECT_LURE) equipped, and clears it otherwise. */
void gmFlagRefreshLureCap(void)
{
	Sint32 partySize;
	Sint32 member;

	gmFlagClear(FLAG_LURE_CAP);
	partySize = gmPartyCountMembers();
	for(member = 0; member < partySize; member++) {
		UNIT  *unit = gmUnitGet(MainRoutineStateWork.party[member]);
		Sint32 slot;

		for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
			const ITEM_DEFINITION *item;
			Sint32				   effect;

			if(!(unit->inventory[slot] & INVENTORY_EQUIPPED))
				continue;
			item = gmItemGetDirect(unit->inventory[slot]);
			for(effect = 0; effect < EQUIP_EFFECT_SLOTS; effect++) {
				if(item->effs[effect].kind == EQUIP_EFFECT_LURE)
					gmFlagSet(FLAG_LURE_CAP);
			}
		}
	}
}

/* Set in every build stamp's time. */
#define BUILD_STAMP_MARK  0x1000
/* Set on top of the time in a debug build's stamp (DebugMode): bit 15,
   sign-extended, so that its stamp is above every release build's. A release's
   saves show there as from an older build, and its own never do in a
   release. */
#define BUILD_STAMP_DEBUG 0xffff8000

/* Encodes the build stamp's time, which a new game takes as its start stamp and
   the file list compares saves with. */
Uint32 gmBuildStampGetTime(void)
{
	/* The stamp is six ASCII digits, hhmmss. The hours count 1024, the minutes
	   64 and the seconds 1, so the minutes past 15 reach into the hours. */
	const Uint8 *digits = syResourceGetTableEntry(RESOURCE_BUILD_STAMP);
	Sint32		 hours;
	Sint32		 minutes;
	Sint32		 seconds;
	Sint16		 time;
	Sint32		 stamp;

	hours = (*digits++ - '0') * 10;
	hours += *digits++ - '0';
	minutes = (*digits++ - '0') * 10;
	minutes += *digits++ - '0';
	seconds = (digits[0] - '0') * 10;
	seconds += digits[1] - '0';
	time = (hours * 16 + minutes) * 64 + seconds;
	time |= BUILD_STAMP_MARK;
	stamp = time;

	if(DebugMode != 0)
		stamp |= BUILD_STAMP_DEBUG;
	return (Uint16)stamp;
}

/* Clears the game's state and sets a new game's defaults: Isaac alone in the
   party, the standard buttons, the message speed and the Lucky Wheels odds. */
void gmMainRoutineStateInitDefaults(void)
{
	volatile Sint32 zero;

	/* Clear the state up to the characters, the scene state with its snapshots,
	   and the flags. */
	DMA_FILL32_CELL(
		zero, 0, &MainRoutineStateWork, sizeof(MainRoutineStateWork));
	DMA_FILL32_CELL(zero, 0, SceneLocalState, sizeof(SCENE_STATE_VIEW));
	WAIT_DMA();
	DMA_FILL32_CELL(zero, 0, GameFlagBytes, GAME_FLAG_COUNT / 8);
	((SCENE_STATE_VIEW *)SceneLocalState)->actorSnapshot.slots[0] =
		ACTOR_SNAPSHOT_EMPTY;
	DMA_FILL32_CELL(zero, 0, CharacterUnits, sizeof(CharacterUnits));
	gmPartyInitCharacters();
	MainRoutineStateWork.aKeys = KEY_A;
	MainRoutineStateWork.bKeys = KEY_B;
	MainRoutineStateWork.selectKeys = KEY_SELECT;
	MainRoutineStateWork.startKeys = KEY_START;
	MainRoutineStateWork.firstShortcutKeys = KEY_L;
	MainRoutineStateWork.secondShortcutKeys = KEY_R;
	MainRoutineStateWork.dashKeys = KEY_B;
	MainRoutineStateWork.firstShortcut = 0;
	MainRoutineStateWork.secondShortcut = 0;
	MainRoutineStateWork.leaderActorId = ISAAC;
	gmPartyAddMember(ISAAC);
	MainRoutineStateWork.holdings.coins = 0;
	MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
	MainRoutineStateWork.speech = NEW_GAME_SPEECH;
	MainRoutineStateWork.unusedAfterSpeech = 1;
	MainRoutineStateWork.winHue = 0;
	MainRoutineStateWork.winBrightness = NEW_GAME_WINDOW_BRIGHTNESS;
	MainRoutineStateWork.buildStamp = 0;
	MainRoutineStateWork.startStamp = gmBuildStampGetTime();
	PlayTimeTicks = 0;
	MainRoutineStateWork.playTime = 0;
	IdleSleepEnabled = 0;
	MainRoutineStateWork.idleSleepEnabled = IdleSleepEnabled;
	IdleTicks = 0;
	BupSlot = -1;
	/* The spring game's medal prizes, three to a ring, each ring out from the
	   centre twice as likely as the one inside it. */
	MainRoutineStateWork.holdings.prizeWeights[0] = 4;
	MainRoutineStateWork.holdings.prizeWeights[1] = 4;
	MainRoutineStateWork.holdings.prizeWeights[2] = 4;
	MainRoutineStateWork.holdings.prizeWeights[3] = 8;
	MainRoutineStateWork.holdings.prizeWeights[4] = 8;
	MainRoutineStateWork.holdings.prizeWeights[5] = 8;
	MainRoutineStateWork.holdings.prizeWeights[6] = 16;
	MainRoutineStateWork.holdings.prizeWeights[7] = 16;
	MainRoutineStateWork.holdings.prizeWeights[8] = 16;
	MainRoutineStateWork.holdings.prizeWeights[9] = 32;
	MainRoutineStateWork.holdings.prizeWeights[10] = 32;
	MainRoutineStateWork.holdings.prizeWeights[11] = 32;
	MainRoutineStateWork.holdings.prizeWeights[12] = 64;
	MainRoutineStateWork.holdings.prizeWeights[13] = 64;
	MainRoutineStateWork.holdings.prizeWeights[14] = 64;
}

/* Sets FLAG_CHILDHOOD and recomputes Isaac, Garet and Jenna, whose classes then
   take their childhood forms. */
void gmPartyStartChildhood(void)
{
	gmFlagSet(FLAG_CHILDHOOD);
	gmUnitUpdateClass(ISAAC);
	gmUnitUpdateClass(GARET);
	gmUnitUpdateClass(JENNA);
	gmUnitCalc(ISAAC);
	gmUnitCalc(GARET);
	gmUnitCalc(JENNA);
}

/* The coins the party sets out with after the prologue. */
#define PROLOGUE_COINS 300

/* The party after the prologue: Isaac and Garet healed and given the Short
   Sword, Move and Retreat for Isaac, Move for Garet, Mind Read for Ivan, and
   PROLOGUE_COINS. */
void gmPartyEndChildhood(void)
{
	Sint32 unitId;

	gmFlagClear(FLAG_CHILDHOOD);
	gmFlagClear(FLAG_ISAAC_WEARS_TUNIC);
	gmFlagSet(FLAG_VALE_THREE_YEARS_LATER);
	gmUnitUpdateClass(JENNA);
	gmUnitCalc(JENNA);
	gmFlagClear(FLAG_KEEP_MUSIC_HELD);
	gmFlagSet(FLAG_KEEP_MUSIC_ONCE);

	for(unitId = ISAAC; unitId <= GARET; unitId++) {
		UNIT  *unit = gmUnitGet(unitId);
		Sint32 slot;

		unit->hp = unit->hpMax;
		unit->pp = unit->ppMax;
		UNIT_UPDATE_GAUGES(unit);

		for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
			if((unit->inventory[slot] & INVENTORY_ITEM_ID_MASK) ==
				ITEM_MACHETE) {
				unit->inventory[slot] = ITEM_SHORT_SWORD;
				gmInventoryEquip(unitId, slot);
				break;
			}
		}

		gmUnitUpdateClass(unitId);
		gmUnitCalc(unitId);
	}

	gmUnitLearnPsynergy(ISAAC, ABILITY_MOVE);
	gmUnitLearnPsynergy(ISAAC, ABILITY_RETREAT);
	gmUnitLearnPsynergy(GARET, ABILITY_MOVE);
	gmUnitLearnPsynergy(IVAN, ABILITY_MIND_READ);

	MainRoutineStateWork.holdings.coins += PROLOGUE_COINS;
}

/* Restores every party member's HP and PP; with `cureConditions` 1 it also
   cures poison and haunting. */
void gmPartyRestoreHpPp(Sint32 cureConditions)
{
	Sint32 partySize;
	Sint32 partyIdx;
	UNIT  *unit;

	partySize = gmPartyCountMembers();
	for(partyIdx = 0; partyIdx < partySize; partyIdx++) {
		unit = gmUnitGet(MainRoutineStateWork.party[partyIdx]);

		unit->hp = unit->hpMax;
		unit->pp = unit->ppMax;
		UNIT_UPDATE_GAUGES(unit);

		if(cureConditions == 1) {
			unit->poison = 0;
			unit->haunt = 0;
		}
	}
}

/* Refills the PP of each party member whose lighthouse group
   (CharacterLighthouseGroupTable) holds the lighthouse the party is inside.
   The aeries of Venus and Mercury Lighthouse call it. */
void gmPartyRestorePpInLighthouse(void)
{
	Sint32 partySize;
	Sint32 partyIdx;
	Uint8  unitId;
	Sint32 lighthouseGroup;
	Sint32 restorePp;
	UNIT  *unit;

	partySize = gmPartyCountMembers();
	for(partyIdx = 0; partyIdx < partySize; partyIdx++) {
		unitId = MainRoutineStateWork.party[partyIdx];
		lighthouseGroup = CharacterLighthouseGroupTable[unitId];
		restorePp = 0;
		if(lighthouseGroup == LIGHTHOUSE_GROUP_VENUS_MARS) {
			if(gmFlagGet(FLAG_IN_VENUS_LIGHTHOUSE) ||
				gmFlagGet(FLAG_IN_MARS_LIGHTHOUSE)) {
				restorePp = 1;
			}
		} else {
			if(gmFlagGet(FLAG_IN_MERCURY_LIGHTHOUSE) ||
				gmFlagGet(FLAG_IN_JUPITER_LIGHTHOUSE)) {
				restorePp = 1;
			}
		}

		if(restorePp == 0) {
			continue;
		}

		unit = gmUnitGet(unitId);
		unit->pp = unit->ppMax;
		UNIT_UPDATE_GAUGES(unit);
	}
}

/* Unused: reserved entry; keeps its place in the stub table. */
UNUSED void gmUnitGaugeNoOperation(void)
{
}

/* Recomputes the HP and PP gauges of `unitId` from its current HP and PP, so
   that gmUnitCalc keeps the same fractions. */
void gmUnitRecalculateGauges(Sint32 unitId)
{
	UNIT *unit;

	unit = gmUnitGet(unitId);
	UNIT_UPDATE_GAUGES(unit);
}

/* Clamps a new HP or PP value to 0 and the maximum. */
static inline Sint32 gmUnitClampGaugeValue(Sint32 value, Sint32 maximum)
{
	Sint32 result;

	if(value > maximum)
		result = maximum;
	else {
		result = 0;
		if(value >= 0)
			result = value;
	}
	return result;
}

/* Each sets the unit's HP or PP, clamped to 0 and its maximum, and recomputes
   its HP and PP gauges. */
#define UNIT_DEFINE_SET(name, field, maximum) \
	void name(UNIT *unit, Sint32 value) \
	{ \
		Sint32 clamped; \
\
		clamped = gmUnitClampGaugeValue(value, unit->maximum); \
		unit->field = clamped; \
		UNIT_UPDATE_GAUGES(unit); \
	}

/* Sets the unit's HP, clamped to 0 and its maximum, and recomputes its
   gauges. */
UNIT_DEFINE_SET(gmUnitSetHp, hp, hpMax)
/* Unused: only its unnamed entry in the game module's stub table refers to
   it. */
UNIT_DEFINE_SET(gmUnitSetPp, pp, ppMax)

/* Each adds delta to the HP or PP of unit unitId, clamped to 0 and its maximum,
   recomputes its gauges and returns the new value, as `type`. */
#define UNIT_DEFINE_ADD(type, name, field, maximum) \
	type name(Sint32 unitId, Sint32 delta) \
	{ \
		UNIT  *unit = gmUnitGet(unitId); \
		Sint32 cur = unit->field; \
		Sint32 limit = unit->maximum; \
		Sint32 value = cur + delta; \
		Sint32 result; \
\
		result = gmUnitClampGaugeValue(value, limit); \
		unit->field = result; \
		gmUnitRecalculateGauges(unitId); \
		return unit->field; \
	}

/* Adds `delta` to a unit's HP, for the field's healing and damage, battle and
   the debug menu. */
UNIT_DEFINE_ADD(Sint32, gmUnitAddHp, hp, hpMax)
/* Adds `delta` to a unit's PP, for Psynergy used in the field, in battle and
   from the menu, PP recovery at each battle turn's end and the debug menu. */
UNIT_DEFINE_ADD(Sint16, gmUnitAddPp, pp, ppMax)

#undef UNIT_DEFINE_SET
#undef UNIT_DEFINE_ADD
#undef UNIT_STAT_DIFF

#undef SCALE_EFFECT
#undef LIGHTHOUSE_PP_REGEN
#undef BUILD_STAMP_MARK
#undef BUILD_STAMP_DEBUG
#undef PROLOGUE_COINS
