// The party's units and their stats, a new game, HP and PP, and the data transfer from The Broken
// Seal up to Isaac's party joining at Contigo. Part of game.c.

#include "macros.h"
#include "field/mode_stubs.h"
#include "window/stubs.h"
#include "window/window.h"

// Starts a new game: clears the main routine's state and restarts at the opening.
void gmRestartQuest(void)
{
	gmMainRoutineStateInitDefaults();
	gmMainRoutineRunStub(0);
}

// Returns the party's Djinn recovery bank, the standby Djinn and their turns to recover, or for the
// enemy side a battle unit read as one.
DJINN_RECOVERY_TABLE *gmDjinnGetRecoveryTable(Sint32 enemySide)
{
	if (enemySide != 0)
		return (DJINN_RECOVERY_TABLE *)syUnitGet(FIRST_ENEMY_UNIT + 3);
	return (DJINN_RECOVERY_TABLE *)&MainRoutineStateWork.summonMask;
}

// Returns the average level of the party members, rounded down (0 with no party members).
Uint32 gmPartyGetAverageLevel(void)
{
	Sint32 totalLevels = 0;
	Sint32 partySize   = gmPartyCountMembers();
	Sint32 i;

	if (partySize == 0)
		return 0;
	for (i = 0; i < partySize; i++)
		totalLevels += syUnitGet(MainRoutineStateWork.party[i])->level;
	totalLevels /= partySize;
	return totalLevels;
}

// Returns the definition of enemy `enemyId`; enemy ids start past the eight characters', so that
// one id names any unit.
const ENEMY_DEFINITION *gmEnemyGetDefinition(Sint32 enemyId)
{
	Uint32 recordIndex;

	recordIndex = enemyId - ENEMY_ID_FIRST;
	// BUG: the table has ENEMY_COUNT rows, but only rows from ENEMY_ID_MAX on are refused, so those
	// from ENEMY_COUNT read the tables after it. gmBattleUnitSet takes the first row for them
	// itself.
#if FIX_ENEMY_ID_BOUND
	if (recordIndex > ENEMY_COUNT - 1U)
#else
	if (recordIndex > ENEMY_ID_MAX - 1U)
#endif
		recordIndex = 0;
	return &EnemyDefinitions[recordIndex];
}

// Copies `count` bytes from `first` to `second` when `direction` is set, and from `second` to
// `first` otherwise.
void gmMemoryCopyBytesDirectional(Uint8 *first, Uint8 *second, Sint32 count, Sint32 direction)
{
	Sint32 i;

	if (direction != 0)
	{
		for (i = 0; i < count; i++)
			*second++ = *first++;
	}
	else
	{
		for (i = 0; i < count; i++)
			*first++ = *second++;
	}
}

// The distance between `a` and `b`, |a - b|.
#define UNIT_STAT_DIFF(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

// A scaling effect's number among the eight, from EQUIP_EFFECT_SCALE_HP.
#define SCALE_EFFECT(kind)   ((kind) - EQUIP_EFFECT_SCALE_HP)

// Works out a unit's stats from its base stats, equipment, set Djinn, class and battle stat levels;
// its HP and PP keep their share of a new maximum.
void gmUnitCalculate(Sint32 unitId)
{
	STAT_STATE *stats;
	UNIT       *unit;
	Sint32      index, effectIndex, elementIndex;
	Sint32      value;
	Sint32      lighthouse;
	Sint32      effectKind;
	Sint32      effectAmount;
	Sint32      hpLimit;
	Sint32      ppLimit;
	Sint16      prevMaximum;

	// Start with the unit's base stats and elemental power/resistance.
	stats              = syHeapAllocateEWRAM(sizeof(STAT_STATE));
	unit               = syUnitGet(unitId);
	stats->hp          = unit->baseHp;
	stats->pp          = unit->basePp;
	stats->attack      = unit->baseAttack;
	stats->defense     = unit->baseDefense;
	stats->agility     = unit->baseAgility;
	stats->luck        = unit->baseLuck;
	stats->actionCount = unit->baseActionCount & UNIT_ACTION_COUNT_MASK;
	stats->hpRegen     = unit->baseHpRegen;
	stats->ppRegen     = unit->basePpRegen;
	for (index = 0; index < ELEMENT_COUNT; index++)
	{
		stats->element[index].power      = unit->baseElement[index].power;
		stats->element[index].resistance = unit->baseElement[index].resistance;
	}

	// HP and PP keep their share of the maximum; gauges that no longer match are reset to full
	// first.
	if (UNIT_STAT_DIFF(unit->hpMax * unit->hpGauge / UNIT_GAUGE_FULL, unit->hp) > 1
	    || UNIT_STAT_DIFF(unit->ppMax * unit->ppGauge / UNIT_GAUGE_FULL, unit->pp) > 1)
	{
		unit->hpGauge = UNIT_GAUGE_FULL;
		unit->ppGauge = UNIT_GAUGE_FULL;
		unit->hp      = unit->hpMax;
		unit->pp      = unit->ppMax;
	}

	// CURSE_KEEP_BOUND keeps CURSE_BOUND, and CURSE_REMOVABLE clears both again below.
	unit->curse &= ~(CURSE_BOUND | CURSE_ITEM);
	if (unit->curse & CURSE_KEEP_BOUND)
		unit->curse |= CURSE_BOUND;
	if (unit->ready)
		stats->actionCount++;
	unit->equipBonus[0] = 0;

	// Apply additive equipment and set-Djinn bonuses before class scaling. An enemy (CLASS_NONE)
	// skips equipment, Djinn and class entirely.
	if (unit->classId != CLASS_NONE)
	{
		for (index = 0; index < INVENTORY_SLOTS; index++)
		{
			if (!(unit->inventory[index] & INVENTORY_EQUIPPED))
				continue;
			stats->item = gmItemGetDirect(unit->inventory[index]);
			if (stats->item->flags & ITEM_CURSED)
				unit->curse |= CURSE_BOUND | CURSE_ITEM;
			stats->attack  += stats->item->attack;
			stats->defense += stats->item->defense;
			for (effectIndex = 0; effectIndex < EQUIP_EFFECT_SLOTS; effectIndex++)
			{
				effectKind    = stats->item->effects[effectIndex].kind;
				effectAmount  = stats->item->effects[effectIndex].amount;
				stats->kind   = effectKind;
				stats->amount = effectAmount;
				switch (effectKind)
				{
					case EQUIP_EFFECT_NONE        : break;
					case EQUIP_EFFECT_ADD_HP      : stats->hp += stats->amount; break;
					case EQUIP_EFFECT_ADD_HP_REGEN: stats->hpRegen += stats->amount; break;
					case EQUIP_EFFECT_ADD_PP      : stats->pp += stats->amount; break;
					case EQUIP_EFFECT_ADD_PP_REGEN: stats->ppRegen += stats->amount; break;
					case EQUIP_EFFECT_ADD_AGILITY : stats->agility += stats->amount; break;
					case EQUIP_EFFECT_ADD_LUCK    : stats->luck += stats->amount; break;
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
					case EQUIP_EFFECT_CURSE_REMOVABLE: unit->curse |= CURSE_REMOVABLE; break;
					case EQUIP_EFFECT_ADD_ACTIONS    : stats->actionCount += stats->amount; break;
				}
			}
		}
		if (unit->curse & CURSE_REMOVABLE)
			unit->curse &= ~(CURSE_REMOVABLE | CURSE_BOUND);

		// Only set Djinn contribute their stat bonuses.
		for (elementIndex = 0; elementIndex < ELEMENT_COUNT; elementIndex++)
		{
			Uint32 activeDjinn = unit->djinnSet[elementIndex];

			for (index = 0; index < DJINN_PER_ELEMENT; index++)
			{
				if (activeDjinn & (1 << index))
				{
					const DJINN_DEFINITION *djinn  = gmDjinnGetDefinition(elementIndex, index);

					stats->hp                     += djinn->hp;
					stats->pp                     += djinn->pp;
					stats->attack                 += djinn->attack;
					stats->defense                += djinn->defense;
					stats->agility                += djinn->agility;
					stats->luck                   += djinn->luck;
				}
			}
		}

		// The class's multipliers are in tenths (STAT_SCALE_ONE keeps a stat)
		{
			const CLASS_DEFINITION *classDefinition = gmClassGetDefinition(unit->classId);

			stats->hp      = stats->hp * classDefinition->hp / STAT_SCALE_ONE;
			stats->pp      = stats->pp * classDefinition->pp / STAT_SCALE_ONE;
			stats->attack  = stats->attack * classDefinition->attack / STAT_SCALE_ONE;
			stats->defense = stats->defense * classDefinition->defense / STAT_SCALE_ONE;
			stats->agility = stats->agility * classDefinition->agility / STAT_SCALE_ONE;
			stats->luck    = stats->luck * classDefinition->luck / STAT_SCALE_ONE;
		}

		// Then the equipment's multiplying effects, EQUIP_EFFECT_SCALE_HP to
		// EQUIP_EFFECT_SCALE_LUCK, also in tenths, applied after the class.
		for (index = 0; index < INVENTORY_SLOTS; index++)
		{
			if (!(unit->inventory[index] & INVENTORY_EQUIPPED))
				continue;
			stats->item = gmItemGetDirect(unit->inventory[index]);
			for (effectIndex = 0; effectIndex < EQUIP_EFFECT_SLOTS; effectIndex++)
			{
				effectKind    = stats->item->effects[effectIndex].kind;
				effectAmount  = stats->item->effects[effectIndex].amount;
				stats->kind   = effectKind;
				effectKind    = SCALE_EFFECT(effectKind);
				stats->amount = effectAmount;
				switch (effectKind)
				{
					case SCALE_EFFECT(EQUIP_EFFECT_SCALE_HP):
						stats->hp = stats->hp * stats->amount / STAT_SCALE_ONE;
						break;
					case SCALE_EFFECT(EQUIP_EFFECT_SCALE_HP_REGEN):
						stats->hpRegen = stats->hpRegen * stats->amount / STAT_SCALE_ONE;
						break;
					case SCALE_EFFECT(EQUIP_EFFECT_SCALE_PP):
						stats->pp = stats->pp * stats->amount / STAT_SCALE_ONE;
						break;
					case SCALE_EFFECT(EQUIP_EFFECT_SCALE_PP_REGEN):
						stats->ppRegen = stats->ppRegen * stats->amount / STAT_SCALE_ONE;
						break;
					case SCALE_EFFECT(EQUIP_EFFECT_SCALE_ATTACK):
						stats->attack = stats->attack * stats->amount / STAT_SCALE_ONE;
						break;
					case SCALE_EFFECT(EQUIP_EFFECT_SCALE_DEFENSE):
						stats->defense = stats->defense * stats->amount / STAT_SCALE_ONE;
						break;
					case SCALE_EFFECT(EQUIP_EFFECT_SCALE_AGILITY):
						stats->agility = stats->agility * stats->amount / STAT_SCALE_ONE;
						break;
					case SCALE_EFFECT(EQUIP_EFFECT_SCALE_LUCK):
						stats->luck = stats->luck * stats->amount / STAT_SCALE_ONE;
						break;
				}
			}
		}
	}

	// Then the battle's changes apply: the Attack, Defense and Agility stages, a summon's boost to
	// each element's power, and the Resistance stage.
	stats->attack  = stats->attack * (unit->attackStage + 8) / 8;
	stats->defense = stats->defense * (unit->defenseStage + 8) / 8;
	stats->agility = stats->agility * (unit->agilityStage + 8) / 8;
	for (index = 0; index < ELEMENT_COUNT; index++)
		stats->element[index].power +=
			(unit->summonBoost[index] * unit->summonBoost[index] + unit->summonBoost[index]) * 5;
	for (index = 0; index < ELEMENT_COUNT; index++)
		stats->element[index].resistance += unit->resistanceStage * RESISTANCE_PER_STAGE;

	if (unit->classId != CLASS_NONE)
	{
		lighthouse = 0;
		// Faster PP recovery inside the lighthouse of the character's own element.
		switch (unit->enemyId)
		{
			case GARET: lighthouse = syGameFlagGet(FLAG_IN_MARS_LIGHTHOUSE); break;
			case MIA  : lighthouse = syGameFlagGet(FLAG_IN_MERCURY_LIGHTHOUSE); break;
			case JENNA: lighthouse = syGameFlagGet(FLAG_IN_MARS_LIGHTHOUSE); break;
			case ISAAC:
			case FELIX: lighthouse = syGameFlagGet(FLAG_IN_VENUS_LIGHTHOUSE); break;
			case IVAN :
			case SHEBA: lighthouse = syGameFlagGet(FLAG_IN_JUPITER_LIGHTHOUSE); break;
			case PIERS: lighthouse = syGameFlagGet(FLAG_IN_MERCURY_LIGHTHOUSE); break;
		}
		if (lighthouse)
			stats->ppRegen += 4;
	}

	CLAMP(stats->attack, 0, UNIT_STAT_MAX);
	CLAMP(stats->defense, 0, UNIT_STAT_MAX);
	CLAMP(stats->agility, 0, UNIT_STAT_MAX);
	CLAMP(stats->luck, 0, UNIT_LUCK_MAX);
	// An enemy may act up to four times a turn, a character twice.
	if (unit->classId == CLASS_NONE)
	{
		CLAMP(stats->actionCount, 0, 4);
	}
	else
	{
		CLAMP(stats->actionCount, 0, 2);
	}
	CLAMP(stats->hpRegen, 0, UNIT_HP_REGEN_MAX);
	CLAMP(stats->ppRegen, 0, UNIT_PP_REGEN_MAX);
	for (index = 0; index < ELEMENT_COUNT; index++)
	{
		CLAMP(stats->element[index].power, 0, UNIT_ELEMENT_MAX);
		CLAMP(stats->element[index].resistance, 0, UNIT_ELEMENT_MAX);
	}

	unit->attack         = stats->attack;
	unit->defense        = stats->defense;
	unit->agility        = stats->agility;
	unit->luck           = stats->luck;
	unit->actionsPerTurn = stats->actionCount;
	unit->hpRegen        = stats->hpRegen;
	unit->ppRegen        = stats->ppRegen;
	for (index = 0; index < ELEMENT_COUNT; index++)
	{
		unit->element[index].power      = stats->element[index].power;
		unit->element[index].resistance = stats->element[index].resistance;
	}

	if (unit->classId == CLASS_NONE)
	{
		hpLimit = UNIT_ENEMY_HP_MAX;
		ppLimit = UNIT_ENEMY_PP_MAX;
	}
	else
	{
		hpLimit = ppLimit = UNIT_CHARACTER_HP_MAX;
	}

	prevMaximum = unit->hpMax;
	CLAMP(stats->hp, 0, hpLimit);
	unit->hpMax = stats->hp;
	if (prevMaximum != unit->hpMax)
	{
		value = stats->hp * unit->hpGauge / UNIT_GAUGE_FULL;
		CLAMP(value, 0, hpLimit);
		if (unit->hp != 0 && value == 0)
			value = 1;
		unit->hp = value;
	}

	prevMaximum = unit->ppMax;
	CLAMP(stats->pp, 0, ppLimit);
	unit->ppMax = stats->pp;
	if (prevMaximum != unit->ppMax)
	{
		value = stats->pp * unit->ppGauge / UNIT_GAUGE_FULL;
		CLAMP(value, 0, ppLimit);
		if (unit->pp != 0 && value == 0)
			value = 1;
		unit->pp = value;
	}

	syHeapRelease(stats);
}

// Sets FLAG_LURE_CAP when any party member has the Lure Cap (the one item with EQUIP_EFFECT_LURE)
// equipped, and clears it otherwise.
void gmFlagRefreshLureCap(void)
{
	Sint32 partySize;
	Sint32 member;

	syGameFlagClear(FLAG_LURE_CAP);
	partySize = gmPartyCountMembers();
	for (member = 0; member < partySize; member++)
	{
		UNIT  *unit = syUnitGet(MainRoutineStateWork.party[member]);
		Sint32 slot;

		for (slot = 0; slot < INVENTORY_SLOTS; slot++)
		{
			const ITEM_DEFINITION *item;
			Sint32                 effect;

			if (!(unit->inventory[slot] & INVENTORY_EQUIPPED))
				continue;
			item = gmItemGetDirect(unit->inventory[slot]);
			for (effect = 0; effect < EQUIP_EFFECT_SLOTS; effect++)
			{
				if (item->effects[effect].kind == EQUIP_EFFECT_LURE)
					syGameFlagSet(FLAG_LURE_CAP);
			}
		}
	}
}

// Returns the unit's PP over its maximum PP, in 16.16; 0 with no maximum.
static __inline__ FIXED gmOwnerGetPpRatio(Sint32 owner)
{
	UNIT *unit = syUnitGet(owner);

	if (unit->ppMax <= 0)
		return 0;
	return toFIXED(unit->pp) / unit->ppMax;
}

// Returns the party's PP over its maximum PP, in 16.16; 0 with no members.
FIXED gmPartyGetPpRatio(void)
{
	Sint32 pp    = 0;
	Sint32 max   = 0;
	Sint32 count = gmPartyCountMembers();
	Sint32 i;

	if (count == 0)
		return 0;
	for (i = 0; i < count; i++)
	{
		UNIT *unit  = syUnitGet(MainRoutineStateWork.party[i]);

		pp         += unit->pp;
		max        += unit->ppMax;
	}
	if (max == 0)
		return 0;
	return toFIXED(pp) / max;
}

// Takes 1 PP from the member whose PP is fullest for its maximum.
Sint32 gmPartyDrainPpFromFullest(void)
{
	FIXED  best      = 0;
	Sint32 bestOwner = -1;
	Sint32 count     = gmPartyCountMembers();
	Sint32 i;

	if (count == 0)
		return 0;
	for (i = 0; i < count; i++)
	{
		FIXED ratio = gmOwnerGetPpRatio(MainRoutineStateWork.party[i]);

		if (ratio > best)
		{
			best      = ratio;
			bestOwner = MainRoutineStateWork.party[i];
		}
	}
	if (bestOwner == -1)
		return 0;
	// Its result taken as a whole word, where the definition returns Sint16.
	return ((Sint32 (*)(Sint32, Sint32))gmUnitAddPp)(bestOwner, -1);
}

// Takes `count` PP from the party, one at a time from the fullest member.
void gmPartyDrainPp(Sint32 count)
{
	Sint32 i;

	for (i = 0; i < count; i++)
		gmPartyDrainPpFromFullest();
	// The ratio is worked out again and not used.
	gmPartyGetPpRatio();
}

// Set in every build stamp's time.
#define BUILD_STAMP_MARK               0x1000
// Set on top of the time in a debug build's stamp (DebugMode): bit 15, sign-extended, so that its
// stamp is above every release build's.
#define BUILD_STAMP_DEBUG              0xffff8000
// A build stamp's time counts 64 seconds to a minute and 16 minutes to an hour, so the minutes past
// 15 reach into the hours.
#define BUILD_STAMP_SECONDS_PER_MINUTE 64
#define BUILD_STAMP_MINUTES_PER_HOUR   16

// Encodes the build stamp's time, which a new game takes as its start stamp and the file list
// compares saves with.
Uint32 gmBuildStampGetTime(void)
{
	// The stamp is six ASCII digits, hhmmss.
	const Uint8 *digits = syResourceGetTableEntry(RESOURCE_BUILD_STAMP);
	Sint32       hours;
	Sint32       minutes;
	Sint32       seconds;
	Sint16       time;
	Sint32       stamp;

	hours    = (*digits++ - '0') * 10;
	hours   += *digits++ - '0';
	minutes  = (*digits++ - '0') * 10;
	minutes += *digits++ - '0';
	seconds  = (digits[0] - '0') * 10;
	seconds += digits[1] - '0';
	time =
		(hours * BUILD_STAMP_MINUTES_PER_HOUR + minutes) * BUILD_STAMP_SECONDS_PER_MINUTE + seconds;
	time  |= BUILD_STAMP_MARK;
	stamp  = time;

	if (DebugMode != 0)
		stamp |= BUILD_STAMP_DEBUG;
	return (Uint16)stamp;
}

// Waits until the DMA channel's transfer is done.
static __inline__ void gmWaitDMA(DMA_REGISTERS volatile *channel)
{
	while ((channel->control & DMA_BUSY) != 0)
	{
	}
}

// The words a new game clears from SceneLocalState up to SoundFlags: the scene's state, the actor
// snapshot, the rare item stock, the scene state and the inventory snapshot.
#define SCENE_STATE_WORDS            1992
// The coins a new game starts with.
#define NEW_GAME_COINS               200
// Where SceneLocalState holds the actor snapshot's slot ids, ACTOR_SNAPSHOT_CAPACITY bytes before
// SnapshotObjects.
#define SCENE_STATE_SNAPSHOT_SLOTS   4
// The two tables a new game fills just after RareItemStock's ITEM_COUNTER_COUNT counters, as places
// in SceneLocalState: 32 rows of eight random numbers 0 to 4, SCENE_STATE_RANDOM_ROW_BYTES apart,
// then a copy of NewGameSceneStateTable.
#define SCENE_STATE_RANDOM_ROWS      4536
#define SCENE_STATE_RANDOM_ROW_COUNT 32
#define SCENE_STATE_RANDOM_ROW_BYTES 12
#define SCENE_STATE_TABLE_COPY                                                                     \
	(SCENE_STATE_RANDOM_ROWS + SCENE_STATE_RANDOM_ROW_COUNT * SCENE_STATE_RANDOM_ROW_BYTES)

#if !NEW_GAME_DMA_WAIT_REBUILDS_ADDRESS
// The Japanese release's first two fills of a new game, through `zero`: MainRoutineStateWork and
// the scene state. FAKEMATCH(wrapper): a Japanese-only inline layer round the first two fills.
// * Its definition takes two constant-pool labels (MainRoutineStateWork and
// * SceneLocalState) before the deferred functions are compiled, so the pool
// * symbols of gmMainRoutineStateInitDefaults are numbered two higher, and
// * reload_cse's lookup of 0x040000d4 meets the one syDMAXCopy left in r3: the
// * wait's pointer becomes the ROM's copy of r3. The plain fills with
// * gmWaitDMA(DMA3) rebuild 0x040000d4 (4 lines in tla-ja).
static __inline__ void gmMainRoutineStateClearStates(volatile Sint32 *zero)
{
	DMA_FILL32_CELL(*zero, 0, &MainRoutineStateWork, sizeof(MainRoutineStateWork));
	DMA_FILL32_CELL(*zero, 0, SceneLocalState, SCENE_STATE_WORDS * 4);
}
#endif

// Starts a new game's state: clears MainRoutineStateWork, the scene state and the snapshots, the
// flags and the characters by DMA, builds the characters, sets the defaults (the button layout,
// Felix and Jenna in the party, their first Psynergy, NEW_GAME_COINS, the message settings and the
// play time clock) and fills the two tables after the rare item stock.
void gmMainRoutineStateInitDefaults(void)
{
	volatile Sint32 zero;
	Sint32          i;
	Sint32          j;

	// Clear the state up to the characters, the scene state with its snapshots, and the flags.
#if NEW_GAME_DMA_WAIT_REBUILDS_ADDRESS
	DMA_FILL32_CELL(zero, 0, &MainRoutineStateWork, sizeof(MainRoutineStateWork));
	DMA_FILL32_CELL(zero, 0, SceneLocalState, SCENE_STATE_WORDS * 4);
#else
	gmMainRoutineStateClearStates(&zero);
#endif
	gmWaitDMA(DMA3);
	DMA_FILL32_CELL(zero, 0, GameFlagBytes, 128 * 4);
	// A new game holds no saved actors.
	SceneLocalState[SCENE_STATE_SNAPSHOT_SLOTS] = ACTOR_SNAPSHOT_EMPTY;
	DMA_FILL32_CELL(zero, 0, CharacterUnits, (PIERS + 1) * sizeof(UNIT));
	gmPartyInitCharacters();
	MainRoutineStateWork.aKeys              = KEY_A;
	MainRoutineStateWork.bKeys              = KEY_B;
	MainRoutineStateWork.selectKeys         = KEY_SELECT;
	MainRoutineStateWork.startKeys          = KEY_START;
	MainRoutineStateWork.firstShortcutKeys  = KEY_L;
	MainRoutineStateWork.secondShortcutKeys = KEY_R;
	MainRoutineStateWork.dashKeys           = KEY_B;
	MainRoutineStateWork.firstShortcut      = 0;
	MainRoutineStateWork.secondShortcut     = 0;
	MainRoutineStateWork.leaderActorId      = FELIX;
	gmPartyAddMember(FELIX);
	gmPartyAddMember(JENNA);
	gmUnitLearnPsynergy(FELIX, ABILITY_RETREAT);
	gmUnitLearnPsynergy(FELIX, ABILITY_MOVE);
	gmUnitLearnPsynergy(SHEBA, ABILITY_MIND_READ);
	gmUnitLearnPsynergy(PIERS, ABILITY_DOUSE);
	gmUnitLearnPsynergy(ISAAC, ABILITY_RETREAT);
	gmUnitLearnPsynergy(ISAAC, ABILITY_MOVE);
	gmUnitLearnPsynergy(GARET, ABILITY_MOVE);
	gmUnitLearnPsynergy(IVAN, ABILITY_MIND_READ);
	gmUnitLearnPsynergy(IVAN, ABILITY_REVEAL);
	MainRoutineStateWork.coins              = NEW_GAME_COINS;
	MainRoutineStateWork.transferredCoins   = 0;
	MainRoutineStateWork.forgeItem          = 0;
	MainRoutineStateWork.weirdAnswers       = 0;
	// The settings a new game starts with: Normal messages with speech on, and the first window
	// colour one step brighter than neutral.
	MainRoutineStateWork.msgSpeed           = MESSAGE_SPEED_NORMAL;
	MainRoutineStateWork.speech             = 1;
	MainRoutineStateWork.unusedAfterSpeech  = 1;
	MainRoutineStateWork.windowHue          = 0;
	MainRoutineStateWork.windowBrightness   = WINDOW_BRIGHTNESS_NEUTRAL + 1;
	MainRoutineStateWork.buildStamp         = 0;
	MainRoutineStateWork.startStamp         = gmBuildStampGetTime();
	PlayTimeTicks                           = 0;
	IdleSleepEnabled                        = 0;
	MainRoutineStateWork.playTime           = 0;
	MainRoutineStateWork.idleSleepEnabled   = IdleSleepEnabled;
	IdleTicks                               = 0;
	BupSlot                                 = -1;
	MainRoutineStateWork.clearedFileAtStart = AnyFileCleared;
	// Lemuria's spring's medal prizes (LemuriaSpringMedalPrizes): three share each weight, and each
	// three weigh twice the last.
	MainRoutineStateWork.prizeWeights[0]    = 4;
	MainRoutineStateWork.prizeWeights[1]    = 4;
	MainRoutineStateWork.prizeWeights[2]    = 4;
	MainRoutineStateWork.prizeWeights[3]    = 8;
	MainRoutineStateWork.prizeWeights[4]    = 8;
	MainRoutineStateWork.prizeWeights[5]    = 8;
	MainRoutineStateWork.prizeWeights[6]    = 16;
	MainRoutineStateWork.prizeWeights[7]    = 16;
	MainRoutineStateWork.prizeWeights[8]    = 16;
	MainRoutineStateWork.prizeWeights[9]    = 32;
	MainRoutineStateWork.prizeWeights[10]   = 32;
	MainRoutineStateWork.prizeWeights[11]   = 32;
	MainRoutineStateWork.prizeWeights[12]   = 64;
	MainRoutineStateWork.prizeWeights[13]   = 64;
	MainRoutineStateWork.prizeWeights[14]   = 64;

	// XXX (no evidence): what reads the two tables; no object of the main image or of any overlay
	// holds their addresses, reads SceneLocalState or RareItemStock that far in, or has their
	// places in its literal pools, and nothing in reach watches the memory of a played game.
	for (i = 0; i < SCENE_STATE_RANDOM_ROW_COUNT; i++)
	{
		for (j = 0; j < 8; j++)
			SceneLocalState[SCENE_STATE_RANDOM_ROWS + i * SCENE_STATE_RANDOM_ROW_BYTES + j] =
				syRandomBelow(5);
	}
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 5; j++)
			SceneLocalState[SCENE_STATE_TABLE_COPY + i * 5 + j] = NewGameSceneStateTable[i][j];
	}
}

// Makes Jenna the leader.
void gmPartySetLeaderJenna(void)
{
	MainRoutineStateWork.leaderActorId = JENNA;
}

// Makes Felix the leader, with Jenna and Sheba out of the party.
void gmPartySetLeaderFelix(void)
{
	MainRoutineStateWork.leaderActorId = FELIX;
	gmPartyAddMember(FELIX);
	gmPartyRemoveMember(JENNA);
	gmPartyRemoveMember(SHEBA);
}

// Gives Piers his two Mercury Djinn, Spring and Shade (Mercury 9 and 10), set, then evens out the
// party's Djinn and recalculates its members. Kibombo calls it when Piers joins, and the world map
// (WORLD_MAP_PIERS_DJINN).
void gmPartyGivePiersDjinn(void)
{
	Sint32 i;
	Sint32 count;

	// i counts Spring and Shade from 1.
	for (i = DJINNI_INDEX(DJINNI_SPRING) + 1; i <= DJINNI_INDEX(DJINNI_SHADE) + 1; i++)
	{
		syGameFlagSet(FLAG_DJINNI(ELEMENT_MERCURY * DJINN_PER_ELEMENT + i - 1));
		gmDjinnGive(PIERS, ELEMENT_MERCURY, i - 1);
		gmDjinnSet(PIERS, ELEMENT_MERCURY, i - 1);
	}
	// Called with an argument it does not take.
	((void (*)(Sint32))gmPartyBalanceDjinn)(0);
	count = gmPartyCountMembers();
	for (i = 0; i < count; i++)
		gmUnitCalculate(MainRoutineStateWork.party[i]);
}

// Restores every party member's HP and PP; with `cureConditions` 1 it also cures poison and
// haunting.
void gmPartyRestoreHpPp(Sint32 cureConditions)
{
	Sint32 partySize;
	Sint32 partyIndex;
	UNIT  *unit;

	partySize = gmPartyCountMembers();
	for (partyIndex = 0; partyIndex < partySize; partyIndex++)
	{
		unit     = syUnitGet(MainRoutineStateWork.party[partyIndex]);

		unit->hp = unit->hpMax;
		unit->pp = unit->ppMax;
		UNIT_UPDATE_GAUGES(unit);

		if (cureConditions == 1)
		{
			unit->poison = 0;
			unit->haunt  = 0;
		}
	}
}

// Restores the PP of each party member whose element group's lighthouse the party is inside: Venus
// or Mars for Isaac, Garet, Felix and Jenna, Mercury or Jupiter for the others. Nothing calls it or
// its stub.
void gmPartyRestorePpInLighthouse(void)
{
	Sint32 partySize;
	Sint32 partyIndex;
	Uint8  unitId;
	Sint32 elementGroup;
	Sint32 restorePp;
	UNIT  *unit;

	partySize = gmPartyCountMembers();
	for (partyIndex = 0; partyIndex < partySize; partyIndex++)
	{
		unitId       = MainRoutineStateWork.party[partyIndex];
		elementGroup = CharacterElementGroupTable[unitId];
		restorePp    = 0;
		if (elementGroup == ELEMENT_GROUP_VENUS_MARS)
		{
			if (syGameFlagGet(FLAG_IN_VENUS_LIGHTHOUSE) || syGameFlagGet(FLAG_IN_MARS_LIGHTHOUSE))
			{
				restorePp = 1;
			}
		}
		else
		{
			if (syGameFlagGet(FLAG_IN_MERCURY_LIGHTHOUSE)
			    || syGameFlagGet(FLAG_IN_JUPITER_LIGHTHOUSE))
			{
				restorePp = 1;
			}
		}

		if (restorePp == 0)
		{
			continue;
		}

		unit     = syUnitGet(unitId);
		unit->pp = unit->ppMax;
		UNIT_UPDATE_GAUGES(unit);
	}
}

// Writes the element and index of unit's last Djinni and returns 0, or returns -1 when it has none.
Sint32 gmUnitFindLastDjinn(Sint32 unit, Sint32 *elementOut, Sint32 *indexOut)
{
	Uint32 *sets    = syUnitGet(unit)->djinnHeld;
	Sint32  element = -1;
	Sint32  index   = -1;
	Sint32  i;
	Sint32  j;

	for (i = 0; i < DJINN_PER_ELEMENT; i++)
	{
		for (j = 0; j < ELEMENT_COUNT; j++)
		{
			if (sets[j] & (1 << i))
			{
				element = j;
				index   = i;
			}
		}
	}
	if (element == -1)
		return element;
	*elementOut = element;
	*indexOut   = index;
	return 0;
}

// More Djinn than a party member can hold: where the search for the fewest starts.
#define DJINN_COUNT_ABOVE_ANY 999

// Until the Djinn counts of the active owners are within one, moves the last Djinni of the owner
// holding most to the one holding least.
void gmPartyBalanceDjinn(void)
{
	Sint32 count = gmPartyCountMembers();

	for (;;)
	{
		Sint32 lowOwner  = 0;
		Sint32 high      = 0;
		Sint32 low       = DJINN_COUNT_ABOVE_ANY;
		Sint32 highOwner = 0;
		Sint32 i;
		Sint32 sum;
		Sint32 element;
		Sint32 index;

		for (i = 0; i < count; i++)
		{
			Sint32 owner = MainRoutineStateWork.party[i];
			UNIT  *unit  = syUnitGet(owner);
			Uint8 *counts;
			Sint32 j;

			sum    = 0;
			counts = unit->djinnHeldCounts;
			for (j = 0; j < ELEMENT_COUNT; j++)
			{
				Uint8 value = *counts;
				counts++;
				sum += value;
			}
			if (sum > high)
			{
				high      = sum;
				highOwner = owner;
			}
			if (sum < low)
			{
				low      = sum;
				lowOwner = owner;
			}
		}
		if (high - low <= 1)
			break;
		sum = gmUnitFindLastDjinn(highOwner, &element, &index);
		if (sum != 0)
			break;
		gmDjinnTransfer(highOwner, element, index, lowOwner);
	}
}

// Returns flag `flags` of flag bytes laid out as GameFlagBytes: another save's flags, as
// gmDataTransferApply reads them.
Sint32 gmFlagBitsTest(Uint8 *bytes, Sint32 flag)
{
	Sint32 bit;

	bit  = flag & 7;
	flag = GAME_FLAG_BYTE(flag);
	return (bytes[flag] >> bit) & 1;
}

// The data transfer from Golden Sun ("Transfer data from Golden Sun",
// MsgTransferDataFromGoldenSun): copies a The Broken Seal save's Djinn, six of its flags, its
// coins, its options (when it came by link rather than by password) and its four units into this
// game's state.
void gmDataTransferApply(Uint8 *save, Sint32 byPassword)
{
	Uint8              *flags = save + BUP_IMAGE_GAME_FLAGS;
	MAIN_ROUTINE_STATE *party = (MAIN_ROUTINE_STATE *)(save + BUP_IMAGE_MAIN_ROUTINE_STATE);
	UNIT               *units = (UNIT *)(save + BUP_IMAGE_UNITS);
	MAIN_ROUTINE_STATE *state;
	Uint8               name[16];
	Sint32              i;
	Sint32              k;
	Sint32              index;
	Sint32              count;
	Sint32              j;

	// Each element's first seven Djinn, The Broken Seal's, become the transferred Djinn flags.
	for (i = 0, k = 0; i < ELEMENT_COUNT; i++, k += TRANSFERRED_DJINN_PER_ELEMENT)
	{

		for (j = 0; j < TRANSFERRED_DJINN_PER_ELEMENT; j++)
		{
			Sint32 flag = FLAG_TRANSFERRED_DJINN_FIRST + k + j;

			index       = i * DJINN_PER_ELEMENT + j;
			if (gmFlagBitsTest(flags, FLAG_DJINNI(index)))
				syGameFlagSet(flag);
			else
				syGameFlagClear(flag);
		}
	}
	for (i = 0; i < TRANSFERRED_FLAG_COUNT; i++)
	{
		if (gmFlagBitsTest(flags, DataTransferFlags[i]))
			syGameFlagSet(FLAG_TRANSFERRED_DODONPA_LOCKED_UP + i);
		else
			syGameFlagClear(FLAG_TRANSFERRED_DODONPA_LOCKED_UP + i);
	}
	if (syGameFlagGet(FLAG_TRANSFERRED_ISAAC_SAVED_ULMUCH))
		syGameFlagClear(FLAG_TRANSFERRED_ISAAC_SAVED_ULMUCH);
	else
		syGameFlagSet(FLAG_TRANSFERRED_ISAAC_SAVED_ULMUCH);

	state                   = &MainRoutineStateWork;
	state->transferredCoins = party->coins;
	if (byPassword == 0)
	{
		state->windowHue        = party->windowHue;
		state->windowBrightness = party->windowBrightness;
		state->msgSpeed         = party->msgSpeed;
		state->speech           = party->speech;
		state->idleSleepEnabled = party->idleSleepEnabled;
		mnWindowColorApplyStub(state->windowHue, state->windowBrightness);
	}

	// Isaac's party, the units moved (j reused).
	j = MIA + 1;
	for (i = 0; i < (Sint32)j; i++)
	{
		UNIT *unit = syUnitGet(i);

		// The saved name is put aside and written back over the copy, so the unit takes The Broken
		// Seal's name either way.
		memcpy(name, &units[i], sizeof(units->name));
		syWordsCopyEntry(unit, &units[i], sizeof(UNIT));
		memcpy(unit, name, sizeof(units->name));
		if (byPassword == 0)
			unit->enemyId = unit->id;
		for (index = INVENTORY_SLOTS - 1; index >= 0; index--)
		{
			Sint32 item = unit->inventory[index];

			if (item != 0 && (gmItemGetDirect(item)->flags & ITEM_NOT_TRANSFERRED))
			{
				while (gmInventoryRemove(i, index) == 1)
					;
			}
		}
		gmUnitCalculate(i);
	}
	gmDjinnSetAllForParty();
	gmPartyRestoreHpPp(1);
	syGameFlagSet((byPassword & 1) + FLAG_TRANSFERRED_BY_LINK);
	syGameFlagSet(FLAG_DATA_TRANSFERRED);
	PlayTimeTicks = MainRoutineStateWork.playTime;
}

// Carries Clear Data over to a new quest: the options, coins and each unit's base stats, level and
// experience.
void gmClearDataApply(Uint8 *save)
{
	MAIN_ROUTINE_STATE *party = (MAIN_ROUTINE_STATE *)(save + BUP_IMAGE_MAIN_ROUTINE_STATE);
	UNIT               *units = (UNIT *)(save + BUP_IMAGE_UNITS);
	Uint8               unused[16]; // Unused: gives the stack frame its 16 bytes.
	Sint32              i;
	Sint32              count;

	MainRoutineStateWork.windowHue        = party->windowHue;
	MainRoutineStateWork.windowBrightness = party->windowBrightness;
	MainRoutineStateWork.msgSpeed         = party->msgSpeed;
	MainRoutineStateWork.speech           = party->speech;
	MainRoutineStateWork.idleSleepEnabled = party->idleSleepEnabled;
	mnWindowColorApplyStub(MainRoutineStateWork.windowHue, MainRoutineStateWork.windowBrightness);
	count = PIERS + 1;
	for (i = 0; i < count; i++)
	{
		UNIT *unit        = syUnitGet(i);
		UNIT *saved       = &units[i];

		unit->baseHp      = saved->baseHp;
		unit->basePp      = saved->basePp;
		// The gauges take the base HP and PP, which are no share of HP or PP; gmUnitCalculate,
		// finding a gauge out of step with its HP or PP, fills both gauges and sets HP and PP to
		// their maximums.
		unit->hpGauge     = unit->baseHp;
		unit->ppGauge     = unit->basePp;
		unit->baseAttack  = saved->baseAttack;
		unit->baseDefense = saved->baseDefense;
		unit->baseAgility = saved->baseAgility;
		unit->baseLuck    = saved->baseLuck;
		unit->level       = saved->level;
		unit->experience  = saved->experience;
		gmUnitCalculate(i);
	}
	MainRoutineStateWork.coins = party->coins;
}

// Gives owner item unless a party member already has it.
void gmPartyGiveItemIfMissing(Sint32 item, Sint32 owner)
{
	if (gmPartyInventoryFindMember(item) == -1)
		gmInventoryAddItem(owner, item);
}

// Isaac's party joins: the transferred Djinn flags move to their places, Isaac, Garet, Ivan and Mia
// join with their Djinn evened out and restored, a missing Djinni of each element is picked, and
// the transferred items and coins are given. Contigo calls it.
void gmPartyAddIsaacsParty(void)
{
	Sint32 i;
	Sint32 djinni;
	Sint8  missing[8];
	Sint32 count;
	Sint32 k;

	for (i = 0, k = 0; i < ELEMENT_COUNT; i++, k += TRANSFERRED_DJINN_PER_ELEMENT)
	{
		Sint32 j;

		for (j = 0; j < TRANSFERRED_DJINN_PER_ELEMENT; j++)
		{
			djinni = i * DJINN_PER_ELEMENT + j;
			if (syGameFlagGet(FLAG_TRANSFERRED_DJINN_FIRST + k + j))
				syGameFlagSet(FLAG_DJINNI(djinni));
		}
	}
	gmPartyInventoryRemove(ITEM_MYTHRIL_BAG_MARS_STAR);
	gmPartyAddMember(ISAAC);
	gmPartyAddMember(GARET);
	gmPartyAddMember(IVAN);
	gmPartyAddMember(MIA);
	// Called with an argument it does not take.
	((void (*)(Sint32))gmPartyBalanceDjinn)(0);
	gmDjinnSetAllForParty();
	for (i = 0; i <= PIERS; i++)
		gmUnitCalculate(i);
	gmPartyRestoreHpPp(1);
	syGameFlagSet(FLAG_REUNION);

	// One of The Broken Seal's Djinn of each element the party lacks, picked at random, plus one; 0
	// when it has them all.
	for (i = 0; i < ELEMENT_COUNT; i++)
	{
		count = 0;
		for (djinni = 0; djinni < TRANSFERRED_DJINN_PER_ELEMENT; djinni++)
		{
			if (!syGameFlagGet(FLAG_DJINNI(i * DJINN_PER_ELEMENT + djinni)))
				missing[count++] = djinni;
		}
		if (count != 0)
			djinni = missing[(syRandom() * count) >> 16] + 1;
		else
			djinni = 0;
		SceneLocalState[SCENE_STATE_CHOSEN_DJINN + i] = djinni;
	}
	// The Psynergy items Isaac's party carried in The Broken Seal.
	gmPartyGiveItemIfMissing(ITEM_FROST_JEWEL, MIA);
	gmPartyGiveItemIfMissing(ITEM_DOUSE_DROP, MIA);
	gmPartyGiveItemIfMissing(ITEM_LIFTING_GEM, GARET);
	gmPartyGiveItemIfMissing(ITEM_CARRY_STONE, ISAAC);
	gmPartyGiveItemIfMissing(ITEM_CATCH_BEADS, IVAN);
	MainRoutineStateWork.coins += MainRoutineStateWork.transferredCoins;
#if TRANSFERRED_COINS_CAPPED
	if (MainRoutineStateWork.coins > COIN_MAX)
		MainRoutineStateWork.coins = COIN_MAX;
#endif
}

// Does nothing; nothing calls gmUnitReservedNoOperationStub.
void gmUnitReservedNoOperation(void)
{
}

// Gives `owner` a Djinni on standby and sets its found flag, as if met in the field.
void gmDjinnAddFoundToOwner(Sint32 owner, Sint32 element, Sint32 djinn)
{
	Sint32 flag = element * DJINN_PER_ELEMENT + djinn;

	gmDjinnGive(owner, element, djinn);
	flag += FLAG_DJINN_FIRST;
	gmDjinnRecoveryAddStandby(owner, element, djinn);
	syGameFlagSet(flag);
}

// The Djinn of The Broken Seal handed out at random, besides Flint and Fizz, when the player
// transfers none.
#define DEFAULT_TRANSFER_DJINN 16

// Stands in for transferred data when the player transfers none: unless the transferred Djinn are
// already set, sets the party aside, makes Isaac's party the party, gives it Flint and Fizz, then
// hands out DEFAULT_TRANSFER_DJINN random Djinn of The Broken Seal's still unowned, keeping the
// elements' counts within one, each with its chance in DataTransferDjinnChances; then moves the
// Djinn flags to the transferred ones and puts the party back.
void gmDataTransferGiveDefaultDjinn(void)
{
	Sint8  saved[8];
	Sint8  counts[ELEMENT_COUNT];
	Sint32 count;
	Sint32 remaining;
	Sint32 element;
	Sint32 djinn;
	Sint32 flag;
	Sint32 i;
	Sint32 k;

	if (syGameFlagGet(FLAG_TRANSFERRED_DJINN_FIRST))
		return;

	count = gmPartyCountMembers();
	for (i = 0; i < count; i++)
		saved[i] = MainRoutineStateWork.party[i];
	for (i = 0; i < count; i++)
		gmPartyRemoveMember(saved[i]);
	for (i = ISAAC; i <= MIA; i++)
		gmPartyAddMember(i);
	// Isaac's Flint and Mia's Fizz, which The Broken Seal gives at its start.
	gmDjinnAddFoundToOwner(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT));
	gmDjinnAddFoundToOwner(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ));

	// Each element's Djinn so far: Flint and Fizz.
	counts[ELEMENT_VENUS]   = 1;
	counts[ELEMENT_MERCURY] = 1;
	counts[ELEMENT_MARS]    = 0;
	counts[ELEMENT_JUPITER] = 0;
	remaining               = DEFAULT_TRANSFER_DJINN;
	do
	{
		element = syRandomBelow(ELEMENT_COUNT);
		djinn   = syRandomBelow(TRANSFERRED_DJINN_PER_ELEMENT);
		if (!syGameFlagGet(FLAG_DJINNI(element * DJINN_PER_ELEMENT + djinn)))
		{
			Sint8 low;
			Sint8 high;

			// Would the elements' counts stay within one with this Djinni?
			counts[element]++;
			low  = 9; // above any element's count
			high = 0;
			for (i = 0; i < ELEMENT_COUNT; i++)
			{
				if (low > counts[i])
					low = counts[i];
				if (high < counts[i])
					high = counts[i];
			}
			counts[element]--;
			if (high - low <= 1 && (syRandomBelow(100)) < DataTransferDjinnChances[element][djinn])
			{
				gmDjinnGiveToParty(element, djinn);
				counts[element]++;
				remaining--;
			}
		}
	} while (remaining != 0);
	gmDjinnSetAllForParty();

	for (i = 0, k = 0; i < ELEMENT_COUNT; i++, k += TRANSFERRED_DJINN_PER_ELEMENT)
	{
		Sint32 j;

		for (j = 0; j < TRANSFERRED_DJINN_PER_ELEMENT; j++)
		{
			djinn = i * DJINN_PER_ELEMENT + j;
			flag  = FLAG_TRANSFERRED_DJINN_FIRST + k + j;
			if (syGameFlagGet(FLAG_DJINNI(djinn)))
				syGameFlagSet(flag);
			syGameFlagClear(FLAG_DJINNI(djinn));
		}
	}
	for (i = ISAAC; i <= MIA; i++)
		gmPartyRemoveMember(i);
	for (i = 0; i < count; i++)
		gmPartyAddMember(saved[i]);
}

// Recomputes the HP and PP gauges of `unitId` from its current HP and PP, so that gmUnitCalculate
// keeps the same fractions.
void gmUnitRecalculateGauges(Sint32 unitId)
{
	UNIT *unit;

	unit = syUnitGet(unitId);
	UNIT_UPDATE_GAUGES(unit);
}

// Clamps a new HP or PP value to 0 and the maximum.
static inline Sint32 gmUnitClampGaugeValue(Sint32 value, Sint32 maximum)
{
	Sint32 result;

	if (value > maximum)
		result = maximum;
	else
	{
		result = 0;
		if (value >= 0)
			result = value;
	}
	return result;
}

// Each sets the unit's HP or PP, clamped to 0 and its maximum, and recomputes its HP and PP gauges.
#define UNIT_DEFINE_SET(name, field, maximum)                                                      \
	void name(UNIT *unit, Sint32 value)                                                            \
	{                                                                                              \
		Sint32 clamped;                                                                            \
                                                                                                   \
		clamped     = gmUnitClampGaugeValue(value, unit->maximum);                                 \
		unit->field = clamped;                                                                     \
		UNIT_UPDATE_GAUGES(unit);                                                                  \
	}

UNIT_DEFINE_SET(gmUnitSetHp, hp, hpMax)
UNIT_DEFINE_SET(gmUnitSetPp, pp, ppMax)

// Each adds delta to the HP or PP of unit unitId, clamped to 0 and its maximum, recomputes its
// gauges and returns the new value, as `type`.
#define UNIT_DEFINE_ADD(type, name, field, maximum)                                                \
	type name(Sint32 unitId, Sint32 delta)                                                         \
	{                                                                                              \
		UNIT  *unit    = syUnitGet(unitId);                                                        \
		Sint32 current = unit->field;                                                              \
		Sint32 limit   = unit->maximum;                                                            \
		Sint32 value   = current + delta;                                                          \
		Sint32 result;                                                                             \
                                                                                                   \
		result      = gmUnitClampGaugeValue(value, limit);                                         \
		unit->field = result;                                                                      \
		gmUnitRecalculateGauges(unitId);                                                           \
		return unit->field;                                                                        \
	}

UNIT_DEFINE_ADD(Sint32, gmUnitAddHp, hp, hpMax)
UNIT_DEFINE_ADD(Sint16, gmUnitAddPp, pp, ppMax)

#undef UNIT_DEFINE_SET
#undef UNIT_DEFINE_ADD

#undef UNIT_STAT_DIFF
#undef SCALE_EFFECT
#undef BUILD_STAMP_MARK
#undef BUILD_STAMP_DEBUG
#undef BUILD_STAMP_SECONDS_PER_MINUTE
#undef BUILD_STAMP_MINUTES_PER_HOUR
#undef SCENE_STATE_WORDS
#undef NEW_GAME_COINS
#undef SCENE_STATE_SNAPSHOT_SLOTS
#undef SCENE_STATE_RANDOM_ROWS
#undef SCENE_STATE_RANDOM_ROW_COUNT
#undef SCENE_STATE_RANDOM_ROW_BYTES
#undef SCENE_STATE_TABLE_COPY
#undef DJINN_COUNT_ABOVE_ANY
#undef DEFAULT_TRANSFER_DJINN
