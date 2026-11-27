/* A turn's actions: the party's commands, the enemies' or the link opponent's,
   and their action order. Part of battle.c. */

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

/* An empty action's priority, below every other as the sort reads it
   (Sint16). */
#define PRIORITY_LOWEST 32768

/* Sets the camera up for the fixed view the battle shows while commands are
   chosen, raised by verticalOfs pixels. */
void btCameraSetCommandView(Sint32 verticalOfs)
{
	BATTLE_CAMERA			*camera = RamHeapSlots->cameraState;
	BATTLE_BACKGROUND_STATE *view = RamHeapSlots->battleBg;
	POINT					 eyeOfs;
	Uint32					 result;

	/* The focus 5 pixels up, a quarter turn of yaw, a slight downward pitch,
	   750 pixels away. */
	camera->focus[1] = toFIXED(5);
	camera->focus[0] = 0;
	camera->focus[2] = 0;
	view->tgtYaw = DEGtoANG(90);
	camera->yaw = DEGtoANG(90);
	camera->pitch = TRIG_TURN - TRIG_TURN * 3 / 64;
	camera->followPos = 0;
	camera->distance = toFIXED(750);
	camera->followEye = 0;

	syInitMatrix();
	syMatrixTranslateVector(&camera->focus[0]);
	syRotY(camera->yaw);
	syRotX(camera->pitch);

	SET_VECTOR(eyeOfs, 0, 0, camera->distance);
	syCalcPoint(eyeOfs, (Sint32 *)camera);

	result = syDivFX(toFIXED(969), toFIXED(3) / 4);
	sySetScreenDist(0, result, toFIXED(1938));

	/* While the view is busy, btCameraSetRange leaves BG1's scroll alone. */
	ProjectionWork.centerY = verticalOfs + BATTLE_PROJECTION_CENTER_Y;
	view->busy = 1;
	btCameraSetRange(
		toFIXED(120), toFIXED(118 - verticalOfs), 0, 8 << 8, toFIXED(2));
	view->keepRange = 1;
	view->busy = 0;
}

/* Ends every unit's guard (Defend and the like) and recalculates its stats. */
void btUnitResetGuardLevels(void)
{
	Uint16 unitIds[UNIT_MAX];
	Sint32 cnt;
	Sint32 unitIdx;

	cnt = btUnitListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
	for(unitIdx = 0; unitIdx < cnt; unitIdx++) {
		UNIT *unit;

		unit = syUnitGet(unitIds[unitIdx]);
		unit->defendLevel = 0;
		gmUnitCalcStub(unitIds[unitIdx]);
	}
}

/* Rolls whether the party gets away: 50 percent, 20 more for each failed
   attempt, and 5 for each level the party's average is above the enemies'. A
   party that struck first always gets away, and no one from a battle that
   forbids it. Returns 1 when the party gets away. */
Sint32 btFleeCheckSuccess(void)
{
	Bool		  escaped;
	Sint16		  livingUnits[UNIT_MAX];
	Sint32		  livingCnt;
	Sint32		  levelTotal;
	Sint32		  unitIdx;
	Sint32		  chance;
	BATTLE_STATE *battle;

	escaped = FALSE;
	battle = RamHeapSlots->battleState;
	if(battle->firstStrike == FIRST_STRIKE_PARTY) {
		escaped = TRUE;
	} else {
		levelTotal = 0;
		chance =
			FLEE_BASE_CHANCE + (battle->fleeFailures * FLEE_CHANCE_PER_FAILURE);
		livingCnt = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, livingUnits);
		for(unitIdx = 0; unitIdx < livingCnt; unitIdx++) {
			levelTotal += syUnitGet((Sint32)livingUnits[unitIdx])->level;
		}
		chance += levelTotal * FLEE_CHANCE_PER_LEVEL / livingCnt;
		livingCnt = btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, livingUnits);
		levelTotal = 0;
		for(unitIdx = 0; unitIdx < livingCnt; unitIdx++) {
			levelTotal += syUnitGet((Sint32)livingUnits[unitIdx])->level;
		}
		chance -= levelTotal * FLEE_CHANCE_PER_LEVEL / livingCnt;
		if((chance > 0) &&
			((Uint32)((Uint32)(FLEE_ROLL_RANGE * syRandom()) >> 16) <
				(Uint32)chance)) {
			escaped = TRUE;
		}
		battle->fleeFailures++;
	}
	if(MainRoutineStateWork.battleType == BATTLE_TYPE_NO_ESCAPE) {
		escaped = FALSE;
	}
	return escaped;
}

/* The list mnBattleCollectPartyCommands fills with the unit id of each action
   the party is still to choose (mnBattlePartySplitChoosers): eight halfwords,
   and a byte over. */
#define CHOOSER_LIST_BYTES (8 * sizeof(Uint16) + 1)

/* Builds the party's commands for the turn into entries and returns how many
   there are, or -1 when none could be chosen. */
Sint32 btBuildUnitActions(BATTLE_ACTION_RECORD *entries)
{
	void  *buf = syHeapAllocEWRAM(CHOOSER_LIST_BYTES);
	Sint32 cnt = mnBattleCollectPartyCommandsStub(entries, buf, 0);
	Sint32 result = -1;

	if(cnt >= 0)
		result = cnt;
	syHeapRelease(buf);
	return result;
}

/* The enemies' actions for the turn, in a shuffled order; none act when the
   party struck first. Returns how many there are. */
Sint32 btBuildEnemyActions(BATTLE_ACTION_RECORD *entries)
{
	Uint16		  unitIds[UNIT_MAX];
	Sint32		  entryCnt = 0;
	BATTLE_STATE *battle = RamHeapSlots->battleState;
	Sint32		  unitCnt;
	Sint32		  i;

	if(battle->firstStrike == FIRST_STRIKE_PARTY)
		return 0;

	unitCnt = btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, unitIds);
	if(unitCnt == 0)
		return 0;

	for(i = 0; i < TURN_ORDER_SWAPS; i++) {
		Uint32 first = (unitCnt * syRandom()) >> 16;
		Uint32 second = (unitCnt * syRandom()) >> 16;
		Sint32 swap = unitIds[first];
		unitIds[first] = unitIds[second];
		unitIds[second] = swap;
	}

	/* Striking first, only two to five of the enemies act, each once. */
	if(battle->firstStrike == FIRST_STRIKE_ENEMIES) {
		Sint32 limit = (syRandomBelow(5)) + 1;

		if(limit <= 1)
			limit = 2;
		if(limit < unitCnt)
			unitCnt = limit;
	}

	for(i = 0; i < unitCnt; i++) {
		Sint32 unitId = unitIds[i];
		UNIT  *unit = syUnitGet(unitId);
		Sint32 copyIdx;

		for(copyIdx = 0; copyIdx < unit->actionsPerTurn; copyIdx++) {
			BATTLE_ACTION_RECORD *entry = &entries[entryCnt];
			Sint32				  agility;

			entry->unitId = unitId;
			agility = unit->agility;
			entry->pri = agility;
			entry->turnAction = copyIdx;
			if(unit->actionsPerTurn == 4) {
				if(copyIdx == 0)
					;
				else if(copyIdx == 1)
					entry->pri = (Sint16)agility * 5 / 6;
				else if(copyIdx == 2)
					entry->pri = (Sint16)agility * 4 / 6;
				else
					entry->pri = (Sint16)agility / 2;
			} else if(unit->actionsPerTurn == 3) {
				if(copyIdx == 0)
					;
				else if(copyIdx == 1)
					entry->pri = (Sint16)agility * 3 / 4;
				else
					entry->pri = (Sint16)agility / 2;
			} else if(copyIdx != 0) {
				entry->pri = (Sint16)agility / 2;
			}

			if(unit->sleep != 0 || unit->stun != 0) {
				entry->command = COMMAND_NONE;
				entry->prm = 0;
				entry->tgt = TARGET_NONE_ENEMY;
			} else {
				btAIChooseAction(entry, 0);
			}

			entryCnt++;

			if(battle->firstStrike == FIRST_STRIKE_ENEMIES)
				break;
		}
	}

	return entryCnt;
}

/* Swaps two actions of the turn's order. */
static inline void btActionSwap(
	BATTLE_ACTION_RECORD *a, BATTLE_ACTION_RECORD *b)
{
	BATTLE_ACTION_RECORD temporary;

	syWordsCopyEntry(&temporary, a, sizeof(BATTLE_ACTION_RECORD));
	syWordsCopyEntry(a, b, sizeof(BATTLE_ACTION_RECORD));
	syWordsCopyEntry(b, &temporary, sizeof(BATTLE_ACTION_RECORD));
}

/* Sorts the actions by descending priority, after putting the Djinn that
   shield, guard or defend ahead of the rest. */
void btActionOrderSort(BATTLE_ACTION_RECORD *entries, Sint32 cnt)
{
	Sint32 i;
	Sint32 j;
	Sint32 swapped;

	for(i = 0; i < cnt; i++) {
		BATTLE_ACTION_RECORD *entry = &entries[i];

		if(entry->command == COMMAND_DJINN) {
			ABILITY_DEFINITION *action;

			/* DEAD: syUnitGet's result is not used; the ROM keeps the call. */
			syUnitGet(entry->unitId);
			action = gmAbilityGetDirectStub(
				gmDjinnGetAbilityIdStub(DJINN_COMMAND_ELEMENT(entry->prm),
					DJINN_COMMAND_INDEX(entry->prm)));
			/* These Djinn act ahead of the turn order. */
			if(action->eff == ABILITY_EFFECT_SHIELD ||
				action->eff == ABILITY_EFFECT_BREATH ||
				action->eff == ABILITY_EFFECT_VANISH ||
				action->eff == ABILITY_EFFECT_COUNTER ||
				action->eff == ABILITY_EFFECT_DEFEND ||
				action->eff == ABILITY_EFFECT_TOTAL_DEFENSE ||
				action->eff == ABILITY_EFFECT_GUARD_AURA ||
				action->eff == ABILITY_EFFECT_GROUND) {
				Uint32 pri = entry->pri;

				entry->pri = pri + PRIORITY_FIRST_IN_TURN;
			}
		}
	}

	do {
		swapped = 0;
		for(j = cnt - 1; j > 0; j--) {
			if((Sint16)entries[j].pri > (Sint16)entries[j - 1].pri) {
				btActionSwap(&entries[j], &entries[j - 1]);
				swapped++;
			}
		}
	} while(swapped != 0);
}

/* The link sends in packets of 20 bytes (the header is one), and a transfer may
   take 300 ticks. */
#define LINK_PACKET_BYTES	  20
#define LINK_TRANSFER_TIMEOUT 300
/* The bytes of count actions (16 each, a BATTLE_ACTION_RECORD), in whole
   packets. */
#define LINK_ACTIONS_BYTES(count) \
	((Uint32)((count) * 16 + LINK_PACKET_BYTES - 1) / LINK_PACKET_BYTES * \
		LINK_PACKET_BYTES)

/* Link battle: exchanges the turn's actions with the other console and appends
   the opponent's, their units and targets seen from this side. Returns how many
   were received, or -1 when the exchange fails. */
Sint32 btAppendLinkedActions(BATTLE_ACTION_RECORD *actions, Sint32 cnt)
{
	BATTLE_STATE	   *battle = RamHeapSlots->battleState;
	Sint32				result = 0;
	Sint32				allocationSize = LINK_ACTIONS_BYTES(cnt);
	BATTLE_LINK_HEADER *state = syHeapAllocEWRAM(sizeof(BATTLE_LINK_HEADER));
	Sint32				idx;

	Sint32 btLinkBattleSendActions(void)
	{
		Sint32 timeout;
		Sint32 disconnected;
		Sint32 status = sySerialStartSend(state, LINK_PACKET_BYTES);

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

	Sint32 btLinkBattleReceiveActions(void)
	{
		Sint32 timeout;
		Sint32 disconnected;
		Sint32 status = sySerialStartReceive(state);

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
		result = state->cnt;
		if(state->cnt) {
			status = sySerialStartReceive(actions + cnt);
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

	for(idx = 0; idx < cnt; idx++) {
		BATTLE_ACTION_RECORD *action = &actions[idx];

		action->slot = battle->opponentSlots[action->unitId];

		if(battle->linkSide == 0) {
			if(action->pri & 1)
				action->pri++;
		} else {
			action->pri |= 1;
		}
	}

	if(battle->linkPaused == 0) {
		if(battle->linkSide == 0) {
			Uint32 saved;

			state->cnt = cnt;
			state->check = gmBattleRandomStub();
			IRQ_LOCK(saved);
			SET_RANDOM_SEED(state->seed = RandomSeed);
			IRQ_UNLOCK(saved);
			if(btLinkBattleSendActions() < 0 ||
				btLinkBattleReceiveActions() < 0)
				goto fail;
			result = state->cnt;
		} else {
			if(btLinkBattleReceiveActions() < 0)
				goto fail;
			result = state->cnt;
			state->cnt = cnt;
			if(btLinkBattleSendActions() < 0)
				goto fail;
			if(gmBattleRandomStub() != state->check)
				goto fail;
			SET_RANDOM_SEED(state->seed);
		}

		for(idx = 0; idx < result; idx++) {
			BATTLE_ACTION_RECORD *action = &actions[cnt + idx];

			action->unitId = action->slot;
			action->tgt = action->tgt ^ PLACE_ENEMY_SIDE;
		}

		syHeapRelease(state);
		return result;
	}

fail:
	syLinkBattleResetTransferState();
	sySerialRemoveIRQHandlers();
	syHeapRelease(state);
	return -1;
}

/* Fills BG palettes 14 and 4 with palettes 15 and 5 (the windows' and the basic
   colours) with offset added to each colour channel, held within 0 to
   RGB_LEVEL_MAX. */
void btPaletteCopyWithBrightnessOffset(Sint32 ofs)
{
	Sint32 bank;
	Sint32 source = WINDOW_PALETTE;
	Sint32 i;

	/* Palettes 15 and 5, each into the palette before it, 14 and 4. */
	for(bank = 0; bank < 2; bank++) {
		for(i = 0; i < PALETTE_COLORS; i++) {
			Sint32 idx = source * PALETTE_COLORS + i;
			Uint32 col = ((Uint16 *)BG_PLTT)[idx];
			Sint32 blue = RGB_BLUE(col);
			Sint32 green = RGB_GREEN(col);
			Sint32 red = RGB_RED(col);
			Sint32 adjusted;

			blue += ofs;
			green += ofs;
			red += ofs;
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
			((Uint16 *)BG_PLTT)[idx - PALETTE_COLORS] = adjusted;
		}
		source = BASIC_COLORS_PALETTE;
	}
}

/* Builds this turn's actions and sorts them by descending priority: the allies'
   commands and either the link opponent's or the enemies' actions. Returns the
   action count, or a negative value on cancellation or link failure. */
Sint32 btGatherActions(BATTLE_ACTION_RECORD *actions)
{
	UNIT		 *unit;
	BATTLE_STATE *battle;
	Uint8		 *firstStrike;
	Sint32		  cnt;
	Sint32		  received;
	Sint32		  i;

	battle = RamHeapSlots->battleState;
	{
		BATTLE_ACTION_RECORD *records = battle->actions;
		Uint32				  n;

		for(n = 0; n < BATTLE_ACTION_CAPACITY; n++) {
			records[n].unitId = BATTLE_UNIT_LIST_END;
			records[n].pri = PRIORITY_LOWEST;
		}
	}

	btUnitResetGuardLevels();
	btPaletteCopyWithBrightnessOffset(8);
	syGameFlagSet(FLAG_CHOOSING_COMMANDS);
	firstStrike = &battle->firstStrike;
	btCameraSetCommandView(0);
	mnBattleMessageCloseStub();

	/* Ally commands (unavailable when enemies strike first). */
	if(*firstStrike != FIRST_STRIKE_ENEMIES) {
		cnt = btBuildUnitActions(actions);
		if(cnt < 0)
			goto finish;
		/* A failed escape gives enemies the same advantage as striking
		   first. */
		if(cnt != 0 && actions[0].command == COMMAND_FLEE &&
			btFleeCheckSuccess() == 0)
			*firstStrike = FIRST_STRIKE_ENEMIES;
	} else {
		cnt = 0;
	}

	/* Link opponent or enemy actions. */
	if(RamHeapSlots->battleState->isLink != 0) {
		received = btAppendLinkedActions(actions, cnt);
		if(btLinkBattleWaitSync() < 0) {
			cnt = -1;
			goto finish;
		}
		cnt += received;
		if(received < 0) {
			cnt = -1;
			goto finish;
		}
	} else {
		cnt += btBuildEnemyActions(actions + cnt);
	}

	/* Sorted, and every unit that defends this turn guards from its start. */
	btActionOrderSort(actions, cnt);
	if(cnt > 0) {
		BATTLE_ACTION_RECORD *action = actions;

		i = cnt;
		do {
			if(action->command == COMMAND_DEFEND ||
				action->command == COMMAND_TOTAL_DEFENSE) {
				unit = syUnitGet(action->unitId);
				unit->defendLevel = action->command == COMMAND_DEFEND
					? DEFEND_NORMAL
					: DEFEND_TOTAL;
			}
			action++;
			i--;
		} while(i != 0);
	}

finish:
	syGameFlagClear(FLAG_CHOOSING_COMMANDS);
	btCameraSetDefaultView();
	RamHeapSlots->battleBg->tgtYaw = YAW_PARTY_ACTION;
	return cnt;
}

/* Returns the unit id at a list place, PLACE_ENEMY_SIDE set for the enemies'
   list. */
Sint32 btGetUnitAtPlace(Sint32 taggedPlace)
{
	BATTLE_STATE *battle = RamHeapSlots->battleState;

	if((taggedPlace & PLACE_ENEMY_SIDE) != 0)
		return battle->enemyIds[taggedPlace & PLACE_INDEX_MASK];
	return battle->partyUnits[taggedPlace & PLACE_INDEX_MASK];
}

/* Returns the list place of a unit, PLACE_PARTY or PLACE_ENEMY with its place,
   or -1 when it is in neither list. */
Sint32 btFindUnitPlace(Uint32 unitId)
{
	BATTLE_STATE *battle = RamHeapSlots->battleState;
	Sint32		  placeIdx;
	Sint16		  listedUnit;

	if(unitId <= ALLY_MAX) {
		for(placeIdx = 0;; placeIdx++) {
			listedUnit = battle->partyUnits[placeIdx];
			if(listedUnit == BATTLE_UNIT_LIST_END)
				return -1;
			if(listedUnit != BATTLE_UNIT_REMOVED && listedUnit == unitId)
				return placeIdx | PLACE_PARTY;
		}
	}
	for(placeIdx = 0;; placeIdx++) {
		listedUnit = battle->enemyIds[placeIdx];
		if(listedUnit == BATTLE_UNIT_LIST_END)
			return -1;
		if(listedUnit != BATTLE_UNIT_REMOVED && listedUnit == unitId)
			return placeIdx | PLACE_ENEMY;
	}
}
