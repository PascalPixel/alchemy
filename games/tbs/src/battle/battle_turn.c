/* A turn's actions: the party's commands, the enemies' or the link opponent's,
   and their action order. Part of battle.c. */

#include "battle_internal.h"

/* The odds of fleeing in hundredths of a percent, against a roll below
   FLEE_ROLL_RANGE: FLEE_BASE_CHANCE, FLEE_CHANCE_PER_FAILURE more for each
   failed attempt, and FLEE_CHANCE_PER_LEVEL for each level of the party's
   average above the enemies'. */
typedef enum FLEE_CHANCE {
	FLEE_ROLL_RANGE = 10000,
	FLEE_BASE_CHANCE = 5000,
	FLEE_CHANCE_PER_FAILURE = 2000,
	FLEE_CHANCE_PER_LEVEL = 500
} FLEE_CHANCE;

/* The random swaps that shuffle the enemies' order each turn. */
#define TURN_ORDER_SWAPS 32

/* An empty action's priority, below every other. */
#define PRIORITY_LOWEST -32768

/* Unused: fills HIGHLIGHT_PALETTE with WINDOW_PALETTE's colours, brightened by
   a pulse that rises and falls with the tick count. */
void btPaletteUpdatePulseBrightness(void)
{
	Uint16 *palette = &BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0);
	Sint32	i;

	for(i = 0; i < PALETTE_COLORS; i++) {
		/* A pulse about every 21 ticks (3/64 of a turn a tick), brightening
		   each channel by up to 12 levels. */
		Uint32 angle = TickCount * (TRIG_TURN * 3 / 64);
		Uint32 color = palette[PALETTE_COLORS];
		Sint32 brightness = (toFIXED(1) - syCos(angle)) / (toFIXED(1) / 6);
		Sint32 blue = RGB_BLUE(color);
		Sint32 green = RGB_GREEN(color);
		Sint32 red = RGB_RED(color);

		blue += brightness;
		green += brightness;
		red += brightness;
		if(blue < 0 || blue > RGB_LEVEL_MAX)
			blue = RGB_LEVEL_MAX;
		if(green < 0 || green > RGB_LEVEL_MAX)
			green = RGB_LEVEL_MAX;
		if(red < 0 || red > RGB_LEVEL_MAX)
			red = RGB_LEVEL_MAX;
		*palette++ = RGB(red, green, blue);
	}
}

/* The command view's projection has no focal length, so every unit is drawn at
   one scale (syConvert3Dto2D's 1.32) whatever its depth, out to twice this
   depth; its near depth, as btCameraSetView's, is a fraction of a pixel. */
#define COMMAND_VIEW_DEPTH toFIXED(969)

/* Sets the camera up for the fixed view the battle shows while commands are
   chosen, raised by verticalOffset pixels. */
void btCameraSetCommandView(Sint32 verticalOffset)
{
	void				   **slots = &HeapSlots[HEAP_SLOT_CAMERA];
	BATTLE_CAMERA			*camera = slots[0];
	BATTLE_BACKGROUND_STATE *view =
		HEAP_SLOT_FROM(slots, HEAP_SLOT_CAMERA, HEAP_SLOT_BATTLE_BACKGROUND);
	POINT  eyeOffset;
	Uint32 result;

	/* The focus 5 pixels up, a quarter turn of yaw, a slight downward pitch,
	   750 pixels away. */
	camera->focus[1] = toFIXED(5);
	camera->focus[0] = 0;
	camera->focus[2] = 0;
	view->targetYaw = DEGtoANG(90);
	camera->yaw = DEGtoANG(90);
	camera->pitch = TRIG_TURN - TRIG_TURN * 3 / 64;
	camera->followPos = 0;
	camera->distance = toFIXED(750);
	camera->followEye = 0;

	syInitMatrix();
	syMatrixTranslateVector(&camera->focus[0]);
	syRotY(camera->yaw);
	syRotX(camera->pitch);

	eyeOffset[X] = 0;
	eyeOffset[Y] = 0;
	eyeOffset[Z] = camera->distance;
	syCalcPoint(eyeOffset, (Sint32 *)camera);

	result = syDivFX(COMMAND_VIEW_DEPTH, toFIXED(3) / 4);
	sySetScreenDist(0, result, COMMAND_VIEW_DEPTH * 2);

	/* The background zoomed twofold and raised 8 of its pixels (16 on screen);
	   while the view is busy, btCameraSetRange leaves BG1's scroll alone. */
	ProjectionWork.centerY = verticalOffset + BATTLE_PROJECTION_CENTER_Y;
	view->busy = 1;
	btCameraSetRange(
		toFIXED(120), toFIXED(118 - verticalOffset), 0, Q_8_8(8), toFIXED(2));
	view->keepRange = 1;
	view->busy = 0;
}

/* Ends every unit's guard (Defend and the like) and recalculates its stats. */
void btUnitResetGuardLevels(void)
{
	Uint16 unitIds[UNIT_MAX];
	Sint32 count;
	Sint32 unitIndex;

	count = btUnitListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
	for(unitIndex = 0; unitIndex < count; unitIndex++) {
		UNIT *unit;

		unit = gmUnitGetStub(unitIds[unitIndex]);
		unit->defendLevel = 0;
		gmUnitCalculateStub(unitIds[unitIndex]);
	}
}

/* Rolls whether the party gets away: 50 percent, 20 more for each failed
   attempt, and 5 for each level the party's average is above the enemies'. A
   party that struck first always gets away, and no one from a battle that
   forbids it. Returns 1 when the party gets away. */
Sint32 btFleeCheckSuccess(void)
{
	Bool		  escaped;
	Uint8		 *failedAttempts;
	Sint16		  livingUnits[UNIT_MAX];
	Sint32		  livingCount;
	Sint32		  levelTotal;
	Sint32		  unitIndex;
	Sint32		  chance;
	BATTLE_STATE *battle;

	escaped = FALSE;
	battle = BattleStateWork;
	if(battle->firstStrike == FIRST_STRIKE_PARTY) {
		escaped = TRUE;
	} else {
		failedAttempts = &battle->fleeFailures;
		chance =
			FLEE_BASE_CHANCE + (battle->fleeFailures * FLEE_CHANCE_PER_FAILURE);
		livingCount = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, livingUnits);
		levelTotal = 0;
		for(unitIndex = escaped; unitIndex < livingCount; unitIndex++) {
			levelTotal += gmUnitGetStub(livingUnits[unitIndex])->level;
		}
		chance += levelTotal * FLEE_CHANCE_PER_LEVEL / livingCount;
		livingCount = btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, livingUnits);
		levelTotal = 0;
		for(unitIndex = 0; unitIndex < livingCount; unitIndex++) {
			levelTotal += gmUnitGetStub(livingUnits[unitIndex])->level;
		}
		chance -= levelTotal * FLEE_CHANCE_PER_LEVEL / livingCount;
		if((chance > 0) && (syRandom16Below(FLEE_ROLL_RANGE) < chance)) {
			escaped = TRUE;
		}
		*failedAttempts += 1;
	}
	if(MainRoutineStateWork.battleType == BATTLE_TYPE_NO_ESCAPE) {
		escaped = FALSE;
	}
	return escaped;
}

/* The party's actions for the turn, from the command menu; one who is asleep,
   confused, charmed or stunned loses theirs. Returns how many there are, or -1
   if the menu gave up. */
Sint32 btBuildPartyActions(BATTLE_ACTION *entries)
{
	/* Up to eight choosers, and up to four party members with their end
	   mark. */
	Uint16 *choosingUnits = syHeapAllocateEWRAM(17);
	Uint16 *unitIds = syHeapAllocateEWRAM(9);
	Sint32	unitCount = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, unitIds);
	Sint32	choosingCount = 0;
	Sint32	entryCount = 0;
	Sint32	unitIndex;

	for(unitIndex = 0; unitIndex < unitCount; unitIndex++) {
		UNIT  *unit = gmUnitGetStub(unitIds[unitIndex]);
		Sint32 copyIndex;

		/* Once for each of the party member's actions this turn
		   (actionsPerTurn):
		   a party member with two is listed twice and chooses twice. */
		for(copyIndex = 0; copyIndex < unit->actionsPerTurn; copyIndex++) {
			if(unit->sleep != 0 || unit->confusion != 0 || unit->charm != 0 ||
				unit->stun != 0) {
				BATTLE_ACTION *entry = &entries[entryCount];
				entry->unitId = unitIds[unitIndex];
				entry->priority = unit->agility;
				entry->command = COMMAND_NONE;
				entry->param = 0;
				entry->target = TARGET_NONE_PARTY;
				entryCount++;
			} else {
				choosingUnits[choosingCount++] = unitIds[unitIndex];
			}
		}
	}

	/* The command menu appends the choosing party members' actions. */
	{
		Sint32 appended;

		entries += entryCount;
		appended = mnBattleCollectPartyCommandsStub(
			entries, choosingUnits, choosingCount);
		if(appended < 0) {
			unitCount = -1;
		} else {
			unitCount = entryCount + appended;
		}
		syHeapRelease(unitIds);
		syHeapRelease(choosingUnits);
		return unitCount;
	}
}

/* The enemies' actions for the turn, in a shuffled order. An enemy acting more
   than once acts later at half its Agility; none act when the party struck
   first, and when the party is caught by surprise two to five act once each.
   Returns how many there are. */
Sint32 btBuildEnemyActions(BATTLE_ACTION *entries)
{
	Uint16		  unitIds[UNIT_MAX];
	Sint32		  entryCount = 0;
	BATTLE_STATE *battle = BattleStateWork;
	Sint32		  unitCount;
	Sint32		  i;

	if(battle->firstStrike == FIRST_STRIKE_PARTY) {
		return 0;
	}

	unitCount = btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, unitIds);
	if(unitCount == 0) {
		return 0;
	}

	for(i = 0; i < TURN_ORDER_SWAPS; i++) {
		Uint32 first = syRandom16Below(unitCount);
		Uint32 second = syRandom16Below(unitCount);
		Sint32 swap = unitIds[first];
		unitIds[first] = unitIds[second];
		unitIds[second] = swap;
	}

	/* A roll of 1 to 5, raised to at least 2. */
	if(battle->firstStrike == FIRST_STRIKE_ENEMIES) {
		Sint32 limit = syRandom16Below(5) + 1;

		if(limit <= 1) {
			limit = 2;
		}
		if(limit < unitCount) {
			unitCount = limit;
		}
	}

	for(i = 0; i < unitCount; i++) {
		Sint32 unitId = unitIds[i];
		UNIT  *unit = gmUnitGetStub(unitId);
		Sint32 copyIndex;

		for(copyIndex = 0; copyIndex < unit->actionsPerTurn; copyIndex++) {
			BATTLE_ACTION *entry = &entries[entryCount];
			Sint32		   agility;

			entry->unitId = unitId;
			agility = unit->agility;
			entry->priority = agility;
			if(copyIndex != 0) {
				entry->priority = (Sint16)agility / 2;
			}

			if(unit->sleep != 0 || unit->stun != 0) {
				entry->command = COMMAND_NONE;
				entry->param = 0;
				entry->target = TARGET_NONE_ENEMY;
			} else {
				btAIChooseAction(entry, 0);
			}

			entryCount++;

			if(battle->firstStrike == FIRST_STRIKE_ENEMIES) {
				break;
			}
		}
	}

	return entryCount;
}

/* A Djinni whose effect is a guard or Ground acts before everyone else: its
   priority is raised by this, past any Agility (999 at most). */
#define DJINN_FIRST_PRIORITY 10000

/* Sorts the turn's actions into action order, fastest first, after moving the
   guarding and grounding Djinn to the front. */
void btActionOrderSort(BATTLE_ACTION *entries, Sint32 count)
{
	Sint32 actionIndex;
	Sint32 sortIndex;
	Sint32 swapCount;

	for(actionIndex = 0; actionIndex < count; actionIndex++) {
		BATTLE_ACTION *entry = &entries[actionIndex];

		if(entry->command == COMMAND_DJINN) {
			ABILITY_DEFINITION *action;

			/* DEAD: the unit, fetched and not used; the ROM keeps the call. */
			gmUnitGetStub(entry->unitId);
			action = gmAbilityGetDirectStub(
				gmDjinnGetAbilityIdStub(DJINN_COMMAND_ELEMENT(entry->param),
					DJINN_COMMAND_INDEX(entry->param)));
			if(action->effect == ABILITY_EFFECT_DEFEND ||
				action->effect == ABILITY_EFFECT_TOTAL_DEFENSE ||
				action->effect == ABILITY_EFFECT_GROUND) {
				entry->priority += DJINN_FIRST_PRIORITY;
			}
		}
	}

	/* Equal priorities keep their order, including the shuffle of enemies. */
	do {
		swapCount = 0;
		for(sortIndex = count - 1; sortIndex > 0; sortIndex--) {
			if(entries[sortIndex].priority > entries[sortIndex - 1].priority) {
				BATTLE_ACTION temporary;

				syWordsCopy(
					&temporary, &entries[sortIndex], sizeof(BATTLE_ACTION));
				syWordsCopy(&entries[sortIndex], &entries[sortIndex - 1],
					sizeof(BATTLE_ACTION));
				syWordsCopy(
					&entries[sortIndex - 1], &temporary, sizeof(BATTLE_ACTION));
				swapCount++;
			}
		}
	} while(swapCount != 0);
}

/* The header a link battle sends before its actions: how many, the next random
   number to check the consoles agree, and the parent's seed for the turn. */
typedef struct BATTLE_LINK_HEADER {
	Sint32 count; /* actions that follow the header */
	/* the parent's next gmBattleRandomStub; the child checks its own */
	Sint32 check;
	Sint32 seed; /* the parent's RandomSeed, the turn's battle seed */
	/* padding; a header packet carries only the first 20 bytes */
	Uint8 unused[28];
} BATTLE_LINK_HEADER;

/* The link sends in packets of 20 bytes (the header is one), and a transfer may
   take 300 ticks. */
#define LINK_PACKET_BYTES	  20
#define LINK_TRANSFER_TIMEOUT 300
/* The bytes of count actions (16 each, a BATTLE_ACTION), in whole packets. */
#define LINK_ACTIONS_BYTES(count) \
	((Uint32)((count) * 16 + LINK_PACKET_BYTES - 1) / LINK_PACKET_BYTES * \
		LINK_PACKET_BYTES)

/* Link battle: exchanges the turn's actions with the other console and appends
   the opponent's, their units and targets seen from this side. Returns how many
   were received, or -1 when the exchange fails. */
Sint32 btAppendLinkedActions(BATTLE_ACTION *actions, Sint32 count)
{
	BATTLE_STATE	   *battle = BattleStateWork;
	Sint32				result = 0;
	Sint32				allocationSize = LINK_ACTIONS_BYTES(count);
	BATTLE_LINK_HEADER *header =
		syHeapAllocateEWRAM(sizeof(BATTLE_LINK_HEADER));
	Sint32 index;

	/* Sends the header, then this console's actions. Both transfers share one
	   timeout of LINK_TRANSFER_TIMEOUT ticks. */
	Sint32 btLinkBattleSendActions(void)
	{
		Sint32 timeout;
		Sint32 disconnected;
		Sint32 status = sySerialStartSend(header, LINK_PACKET_BYTES);

		disconnected = 0;
		timeout = LINK_TRANSFER_TIMEOUT;
		if(status == -1)
			return status;
		while(sySerialGetActiveTransfers()) {
			sySynch(1);
			if(--timeout < 0)
				return -1;
			if((LinkStatusWork & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
				if(++disconnected > LINK_MAX_MISSED_TICKS)
					return -1;
			} else {
				disconnected = 0;
			}
		}
		if(allocationSize) {
			status = sySerialStartSend(actions, allocationSize);
			if(status == -1)
				return status;
			while(sySerialGetActiveTransfers()) {
				sySynch(1);
				if(--timeout < 0)
					return -1;
				if((LinkStatusWork & LINK_BOTH_CONNECTED) !=
					LINK_BOTH_CONNECTED) {
					if(++disconnected > LINK_MAX_MISSED_TICKS)
						return -1;
				} else {
					disconnected = 0;
				}
			}
		}
		return 0;
	}

	/* Receives the header, then the other console's actions after ours. A
	   transfer that grows past its expected size, or ends short of it,
	   fails. */
	Sint32 btLinkBattleReceiveActions(void)
	{
		/* SerialReceivedSize changes under the serial interrupt and is volatile
		   (system/globals.h), so every read here loads it again. */
		Sint32 timeout;
		Sint32 disconnected;
		Sint32 status = sySerialStartReceive(header);

		disconnected = 0;
		timeout = LINK_TRANSFER_TIMEOUT;
		if(status == -1)
			return status;
		while(sySerialGetActiveTransfers()) {
			if(SerialReceivedSize > LINK_PACKET_BYTES)
				return -1;
			sySynch(1);
			if(--timeout < 0)
				return -1;
			if((LinkStatusWork & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
				if(++disconnected > LINK_MAX_MISSED_TICKS)
					return -1;
			} else {
				disconnected = 0;
			}
		}
		if(SerialReceivedSize != LINK_PACKET_BYTES)
			return -1;
		result = header->count;
		if(header->count) {
			status = sySerialStartReceive(actions + count);
			if(status == -1)
				return status;
			while(sySerialGetActiveTransfers()) {
				if(SerialReceivedSize > LINK_ACTIONS_BYTES(result))
					return -1;
				sySynch(1);
				if(--timeout < 0)
					return -1;
				if((LinkStatusWork & LINK_BOTH_CONNECTED) !=
					LINK_BOTH_CONNECTED) {
					if(++disconnected > LINK_MAX_MISSED_TICKS)
						return -1;
				} else {
					disconnected = 0;
				}
			}
			if(SerialReceivedSize != LINK_ACTIONS_BYTES(result))
				return -1;
		}
		return 0;
	}

	/* Each action carries its unit's id as the opponent knows it. */
	for(index = 0; index < count; index++) {
		BATTLE_ACTION *action = &actions[index];

		action->slot = battle->opponentSlots[action->unitId];

		if(battle->linkSide == 0) {
			if(action->priority & 1)
				action->priority++;
		} else {
			action->priority |= 1;
		}
	}

	/* The parent (link side 0) sends, then receives; the child receives, then
	   sends and checks the parent's next random number against its own. */
	if(battle->linkPaused == 0) {
		if(battle->linkSide == 0) {
			Uint32 saved;

			header->count = count;
			header->check = gmBattleRandomStub();
			IRQ_LOCK(saved);
			SET_RANDOM_SEED(header->seed = RandomSeed);
			IRQ_UNLOCK(saved);
			if(btLinkBattleSendActions() < 0 ||
				btLinkBattleReceiveActions() < 0)
				goto fail;
			result = header->count;
		} else {
			if(btLinkBattleReceiveActions() < 0)
				goto fail;
			result = header->count;
			header->count = count;
			if(btLinkBattleSendActions() < 0 ||
				gmBattleRandomStub() != header->check)
				goto fail;
			SET_RANDOM_SEED(header->seed);
		}

		/* The received actions, seen from this side: the xor with
		   FIRST_ENEMY_UNIT moves a target between the party's ids and the
		   enemies'. */
		for(index = 0; index < result; index++) {
			BATTLE_ACTION *action = &actions[count + index];

			action->unitId = action->slot;
			action->target ^= FIRST_ENEMY_UNIT;
		}

		syHeapRelease(header);
		return result;
	}

fail:
	syLinkBattleResetTransferState();
	sySerialRemoveIRQHandlers();
	syHeapRelease(header);
	return -1;
}

/* Copies WINDOW_PALETTE into HIGHLIGHT_PALETTE and BASIC_COLORS_PALETTE into
   the palette before it, each channel moved by offset levels. */
void btPaletteCopyWithBrightnessOffset(Sint32 offset)
{
	Sint32 bank;
	Sint32 source = WINDOW_PALETTE;
	Sint32 i;

	for(bank = 0; bank < 2; bank++) {
		for(i = 0; i < PALETTE_COLORS; i++) {
			Sint32 index = source * PALETTE_COLORS + i;
			Uint32 color = ((Uint16 *)BG_PLTT)[index];
			Sint32 blue = RGB_BLUE(color);
			Sint32 green = RGB_GREEN(color);
			Sint32 red = RGB_RED(color);
			Sint32 adjusted;

			blue += offset;
			green += offset;
			red += offset;
			if(blue > RGB_LEVEL_MAX)
				blue = RGB_LEVEL_MAX;
			if(green > RGB_LEVEL_MAX)
				green = RGB_LEVEL_MAX;
			if(red > RGB_LEVEL_MAX)
				red = RGB_LEVEL_MAX;
			if(blue < 0)
				blue = 0;
			if(green < 0)
				green = 0;
			if(red < 0)
				red = 0;
			adjusted = RGB(red, green, blue);
			((Uint16 *)BG_PLTT)[index - PALETTE_COLORS] = adjusted;
		}
		source = BASIC_COLORS_PALETTE;
	}
}

/* Builds the turn's actions and sorts them: the party's commands, then the link
   opponent's or the enemies' actions. Returns how many there are, negative when
   the link fails. */
Sint32 btGatherActions(BATTLE_ACTION *actions)
{
	UNIT		 *unit;
	BATTLE_STATE *battle;
	Sint32		  count;
	Sint32		  received;
	Sint32		  i;

	battle = BattleStateWork;
	/* Empty every action slot of the battle state. */
	{
		BATTLE_ACTION *record = battle->actions;
		Uint32		   actionIndex;

		for(actionIndex = 0; actionIndex < BATTLE_ACTION_CAPACITY;
			actionIndex++) {
			record[actionIndex].unitId = BATTLE_UNIT_LIST_END;
			record[actionIndex].priority = PRIORITY_LOWEST;
		}
	}

	/* Last turn's guards end; the highlight palettes take the window's and the
	   basic colours eight levels brighter; the camera takes the command
	   view. */
	btUnitResetGuardLevels();
	btPaletteCopyWithBrightnessOffset(8);
	gmFlagSetStub(FLAG_CHOOSING_COMMANDS);
	btCameraSetCommandView(0);
	mnBattleMessageCloseStub();

	/* The party's commands (none when the enemies strike first). */
	if(battle->firstStrike != FIRST_STRIKE_ENEMIES) {
		count = btBuildPartyActions(actions);
		if(count < 0)
			goto finish;
		/* A failed attempt to flee makes the turn one the enemies began. */
		if(count != 0 && actions[0].command == COMMAND_FLEE &&
			btFleeCheckSuccess() == 0)
			battle->firstStrike = FIRST_STRIKE_ENEMIES;
	} else {
		count = 0;
	}

	/* The link opponent's actions, or the enemies'. A link battle syncs even
	   after a failed exchange, before giving up. */
	if(BattleStateWork->isLink != 0) {
		received = btAppendLinkedActions(actions, count);
		if(btLinkBattleWaitSync() < 0) {
			count = -1;
			goto finish;
		}
		count += received;
		if(received < 0) {
			count = -1;
			goto finish;
		}
	} else {
		count += btBuildEnemyActions(actions + count);
	}

	/* Sort by priority, and raise the guards of Defend and Total Defense at
	   once, before anyone acts. */
	btActionOrderSort(actions, count);
	for(i = 0; i < count; i++) {
		BATTLE_ACTION *action = &actions[i];

		if(action->command == COMMAND_DEFEND ||
			action->command == COMMAND_TOTAL_DEFENSE) {
			unit = gmUnitGetStub(action->unitId);
			/* Half the damage, or a tenth. */
			unit->defendLevel = action->command == COMMAND_DEFEND
				? DEFEND_NORMAL
				: DEFEND_TOTAL;
		}
	}

finish:
	/* The battle view again, turned toward the party's side. */
	gmFlagClearStub(FLAG_CHOOSING_COMMANDS);
	btCameraSetDefaultView();
	BattleBackgroundStateWork->targetYaw = YAW_PARTY_ACTION;
	return count;
}

/* Returns the unit id at a list place, PLACE_ENEMY_SIDE set for the enemies'
   list. */
Sint32 btGetUnitAtPlace(Sint32 taggedPlace)
{
	BATTLE_STATE *battle = BattleStateWork;

	if((taggedPlace & PLACE_ENEMY_SIDE) != 0)
		return battle->enemyIds[taggedPlace & PLACE_INDEX_MASK];
	return battle->partyUnits[taggedPlace & PLACE_INDEX_MASK];
}

/* Returns the list place of a unit, PLACE_PARTY or PLACE_ENEMY with its place,
   or -1 when it is in neither list. */
Sint32 btFindUnitPlace(Uint32 unitId)
{
	BATTLE_STATE *battle = BattleStateWork;
	Sint32		  placeIndex;
	Sint16		  listedUnit;

	if(unitId <= ALLY_MAX) {
		for(placeIndex = 0;; placeIndex++) {
			listedUnit = battle->partyUnits[placeIndex];
			if(listedUnit == BATTLE_UNIT_LIST_END)
				return -1;
			if(listedUnit != BATTLE_UNIT_REMOVED && listedUnit == unitId)
				return placeIndex | PLACE_PARTY;
		}
	}
	for(placeIndex = 0;; placeIndex++) {
		listedUnit = battle->enemyIds[placeIndex];
		if(listedUnit == BATTLE_UNIT_LIST_END)
			return -1;
		if(listedUnit != BATTLE_UNIT_REMOVED && listedUnit == unitId)
			return placeIndex | PLACE_ENEMY;
	}
}

/* Unused: a second copy of the camera turn while L or R is held. */
DEFINE_ADJUST_CAMERA(btCameraAdjustByShoulderKeysAlternate)

/* Does nothing; btRun calls it at the start of each turn. */
void btTurnStartNoOperation(void)
{
}
