/* A unit's part in the battle stagings: a beast called up, an attack, an item's
   use and a cast, the flash when a unit is hit, and a downed unit's removal
   from the battle and its graphics. Part of battle.c. */

void btObjectReset(BATTLE_OBJECT *battleObject, Sint32 keep);

/* The sprite slot a called beast borrows, after the six enemy slots. */
#define BEAST_SPRITE_SLOT 7

/* The ticks a called beast takes to drop in, and to rise away. */
#define BEAST_ARRIVAL_TICKS	  30
#define BEAST_DEPARTURE_TICKS 16

/* A critical hit's zoom on the attacker and its target, in percent: from
   CRITICAL_ZOOM_START, CRITICAL_ZOOM_GAIN more by the end of the run up. */
#define CRITICAL_ZOOM_START 100
#define CRITICAL_ZOOM_GAIN	30

/* The ticks the battle background darkens through before an item's effect, a
   BACKGROUND_LEVEL_STEP each: to two thirds of its brightness. */
#define ITEM_DARKEN_TICKS 20

/* The beasts called up in battle (ABILITY_WILD_WOLF to ABILITY_CALL_DULLAHAN):
   for a call that lands, saves the battle object it borrows and brings in the
   beast's enemy (ENEMY_WILD_WOLF for Wild Wolf and so on) to play the attack;
   returns the save, or NULL for any other ability. btMotionDismissBeast undoes
   it. */
SAVED_BATTLE_OBJECT *btMotionCallBeast(
	BATTLE_PLAN *plan, BATTLE_PRESENTATION_STATE *state)
{
	Sint32				 single;
	BATTLE_OBJECT		*battleObject;
	SAVED_BATTLE_OBJECT *saved;
	Uint16				 ids[UNIT_MAX];
	MOTION_OBJECT		*object;
	MOTION_OBJECT		*actor;
	Sint32				 classId;
	Sint32				 cnt;
	Sint32				 i;

	if((Uint32)(plan->staging - STAGING_BEAST_ATTACK) >
			STAGING_BEAST_PSYNERGY - STAGING_BEAST_ATTACK ||
		plan->failure != 0)
		return NULL;
	{
		Uint8 sides = (plan->unitId ^ plan->tgtIds[0]) & UNIT_ID_ENEMY_BIT;
		single = sides != 0;
	}
	saved = syHeapAllocEWRAM(sizeof(SAVED_BATTLE_OBJECT));
	saved->single = single;
	saved->reactivate = 0;
	switch(plan->abilityId) {
	case ABILITY_WILD_WOLF:
		classId = ENEMY_WILD_WOLF;
		break;
	case ABILITY_EMU:
		classId = ENEMY_EMU;
		break;
	case ABILITY_ROC:
		classId = ENEMY_ROC;
		break;
	case ABILITY_SALAMANDER:
		classId = ENEMY_SALAMANDER;
		break;
	case ABILITY_ORC:
		classId = ENEMY_ORC;
		break;
	case ABILITY_HARPY:
		classId = ENEMY_HARPY;
		break;
	case ABILITY_GRAND_GOLEM:
		classId = ENEMY_GRAND_GOLEM;
		break;
	case ABILITY_CERBERUS:
		classId = ENEMY_DREAD_HOUND;
		break;
	case ABILITY_WYVERN:
		classId = ENEMY_WYVERN;
		break;
	case ABILITY_PIXIE:
		classId = ENEMY_PIXIE;
		break;
	case ABILITY_DINOX:
		classId = ENEMY_DINOX;
		break;
	case ABILITY_GRYPHON:
		classId = ENEMY_GRYPHON;
		break;
	case ABILITY_LIVING_ARMOR:
		classId = ENEMY_LIVING_ARMOR;
		break;
	case ABILITY_CHIMERA:
		classId = ENEMY_CHIMERA;
		break;
	case ABILITY_BLUE_DRAGON:
		classId = ENEMY_BLUE_DRAGON;
		break;
	case ABILITY_FAERY:
		classId = ENEMY_FAERY;
		break;
	case ABILITY_ELDER_WOOD:
		classId = ENEMY_ELDER_WOOD;
		break;
	case ABILITY_LICH:
		classId = ENEMY_DOOMSAYER;
		break;
	case ABILITY_TROLL:
		classId = ENEMY_TROLL;
		break;
	case ABILITY_MINOTAUR:
		classId = ENEMY_MINOTAURUS;
		break;
	case ABILITY_GHOST_SOLDIER:
		classId = ENEMY_GHOST_ARMY;
		break;
	case ABILITY_MACETAIL:
		classId = ENEMY_MACETAIL;
		break;
	case ABILITY_FIRE_DRAGON:
		classId = ENEMY_FIRE_DRAGON;
		break;
	case ABILITY_WEIRD_NYMPH:
		classId = ENEMY_WEIRD_NYPMH;
		break;
	case ABILITY_SUCCUBUS:
		classId = ENEMY_SIREN;
		break;
	case ABILITY_ESTRE_WOOD:
		classId = ENEMY_ESTRE_WOOD;
		break;
	case ABILITY_MANTICORE:
		classId = ENEMY_MANTICORE;
		break;
	case ABILITY_PHOENIX:
		classId = ENEMY_PHOENIX;
		break;
	case ABILITY_CALL_ZOMBIE:
		classId = ENEMY_ZOMBIE;
		break;
	case ABILITY_CALL_DEMON:
		classId = ENEMY_RED_DEMON;
		break;
	case ABILITY_CALL_DULLAHAN:
		classId = ENEMY_DULLAHAN;
		break;
	default:
		/* No beast of its own: the first enemy row after ENEMY_NONE. */
		classId = ENEMY_VENUS_DJINNI_UNUSED_QUARTZ;
		break;
	}
	saved->classId = classId;
	if(!single) {
		saved->unitId = (plan->unitId ^ UNIT_ID_ENEMY_BIT) & UNIT_ID_ENEMY_BIT;
		if(btObjectGet(saved->unitId)->obj == NULL) {
			saved->reactivate = 1;
#if FIX_FUNCTION_SIGNATURES
			btObjectReset(btObjectGet(saved->unitId), FALSE);
#else
			/* UB: called without keep, so the function reads an argument it was
			   never given. */
			((void (*)())btObjectReset)(btObjectGet(saved->unitId));
#endif
			btObjectGet(saved->unitId)->scale = toFIXED(6) / 5;
		}
	} else {
		saved->unitId = plan->unitId;
	}
	state->actor = saved->unitId;
	actor = btObjectGet(plan->unitId)->obj;
	fdActorSelectAnimationStub(actor, POSE_CAST);
	fdActorSetAnimationSpeedStub((ACTOR *)actor, ANIM_STEP_NORMAL);
	syIOWriteQueueStore16(
		(Uint32)&REG_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
	sySndPlayStub(SE_CAST);
	sySynch(10);
	if(single) {
		btHighlightSetSprMode(saved->unitId, SPRITE_SEMI_TRANSPARENT);
	} else {
		cnt = btUnitListIds((saved->unitId != 0) + 1, ids);
		for(i = 0; i < cnt; i++)
			btHighlightSetSprMode(ids[i], SPRITE_SEMI_TRANSPARENT);
	}
	for(i = 0; i != BLEND_WEIGHT_MAX; i++) {
		REG_BLDALPHA = (i << 8) | (BLEND_WEIGHT_MAX - i);
		sySynch(1);
	}

	battleObject = btObjectGet(saved->unitId);
	object = battleObject->obj;
	if(single) {
		btObjectDestroySprs(saved->unitId);
	} else {
		cnt = btUnitListIds((saved->unitId != 0) + 1, ids);
		for(i = 0; i < cnt; i++) {
			btObjectDestroySprs(ids[i]);
			btObjectReleaseSprSlots(ids[i]);
		}
	}
	syWordsCopyEntry(saved, btObjectGet(saved->unitId), sizeof(BATTLE_OBJECT));
	btObjectReset(btObjectGet(saved->unitId), single);
	btObjectGet(saved->unitId)->scale = toFIXED(6) / 5;
	if(single)
		saved->buf = syHeapAllocEWRAM(32768);
	else
		saved->buf = ActorSpriteSlots;
	i = fdSprPictureCacheLoadStub(BEAST_SPRITE_SLOT, saved->buf,
		btSprGetSpr(saved->classId), btSprGetPalette(saved->classId));
	if(i != 0) {
		battleObject->resource = btSprGetSpr(saved->classId) +
			(BEAST_SPRITE_SLOT << RESOURCE_SLOT_SHIFT);
		if(!single)
			saved->buf = NULL;
	} else {
		battleObject->resource = btSprGetSpr(saved->classId);
		if(single)
			syHeapRelease(saved->buf);
		saved->buf = (Uint32 *)i;
	}
	ids[0] = saved->unitId;
	ids[1] = BATTLE_UNIT_LIST_END;
	btObjectSpawnObjectsForList((Sint16 *)ids, 1, 0);
	object->y = toFIXED(40);
	object->ang = plan->tgtIds[0] <= ALLY_MAX ? 0 : DEGtoANG(180);
	btHighlightSetSprMode(saved->unitId, SPRITE_SEMI_TRANSPARENT);
	for(i = 0; i < BEAST_ARRIVAL_TICKS; i++) {
		object->y = toFIXED((BEAST_ARRIVAL_TICKS - 1 - i) * 3);
		object->velY = 0;
		/* Four turns as it drops. */
		object->ang += TRIG_TURN * 4 / BEAST_ARRIVAL_TICKS;
		if(i < BLEND_WEIGHT_MAX)
			REG_BLDALPHA = ((BLEND_WEIGHT_MAX - i) << 8) | i;
		sySynch(1);
	}
	btHighlightSetSprMode(saved->unitId, SPRITE_OPAQUE);
	return saved;
}

/* Frees the borrowed buffer and releases the motion graphics slot. */
void btMotionReleaseBuffer(SAVED_BATTLE_OBJECT *saved)
{
	MOTION_OBJECT *object = btObjectGet(saved->unitId)->obj;

	if(saved->buf != NULL) {
		fdSprSkipToStopStub(btActorGetSpr(object, 0));
		sySynch(1);
		syHeapRelease(saved->buf);
		saved->buf = NULL;
		{
			SPR *record = btActorGetSpr(object, 0);

			record->flg = SPRITE_FLAG_KEEP_PICTURE | record->flg;
		}
		fdSprRemoveAnimationAtStub(btActorGetSpr(object, 0), 0);
	}
}

/* Sends the called beast away and puts back the battle object btMotionCallBeast
   saved, then frees the save. */
void btMotionDismissBeast(SAVED_BATTLE_OBJECT *saved)
{
	Uint16		   ids[UNIT_MAX];
	MOTION_OBJECT *object;
	Sint32		   unitId;
	Sint32		   cnt;
	Sint32		   i;

	unitId = saved->unitId;
	object = btObjectGet(unitId)->obj;
	syIOWriteQueueStore16(
		(Uint32)&REG_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
	btHighlightSetSprMode(unitId, SPRITE_SEMI_TRANSPARENT);
	for(i = 0; i != BEAST_DEPARTURE_TICKS; i++) {
		Sint32 fade;

		/* Two turns as it rises, fading out over the second half. */
		object->y = toFIXED(i * 4);
		object->ang += DEGtoANG(45);
		fade = i * 2 - BEAST_DEPARTURE_TICKS;
		if(fade < 0)
			fade = 0;
		if(fade < BLEND_WEIGHT_MAX)
			REG_BLDALPHA = (fade << 8) | (BLEND_WEIGHT_MAX - fade);
		sySynch(1);
	}
	btObjectDestroySprs(unitId);
	syWordsCopyEntry(btObjectGet(saved->unitId), saved, sizeof(BATTLE_OBJECT));
	if(saved->reactivate) {
		btObjectMarkForRelease(unitId);
		btMotionDestroyAllSlotObjects();
	}
	fdSprPictureCacheLoadStub(BEAST_SPRITE_SLOT, 0, 0, 0);
	if(saved->single) {
		ids[0] = unitId;
		ids[1] = BATTLE_UNIT_LIST_END;
		btObjectSpawnObjectsForList((Sint16 *)ids, 1, 0);
		btHighlightSetSprMode(unitId, SPRITE_SEMI_TRANSPARENT);
	} else {
		cnt = btUnitListIds((saved->unitId != 0) + 1, ids);
		for(i = 0; i < cnt; i++) {
			Sint32 resource = btEnemySprLoadIntoSlots(ids[i]);

			if(resource)
				btObjectGet(ids[i])->resource = resource;
		}
		btObjectSpawnObjectsForList((Sint16 *)ids, 1, 0);
		for(i = 0; i < cnt; i++)
			btHighlightSetSprMode(ids[i], SPRITE_SEMI_TRANSPARENT);
	}
	btMotionReturnToSpot(unitId);
	SET_POSITION(object, object->tgtX, object->tgtY, object->tgtZ);
	for(i = 0; i < BLEND_WEIGHT_MAX; i++) {
		object->y = 0;
		REG_BLDALPHA = ((BLEND_WEIGHT_MAX - i) << 8) | i;
		sySynch(1);
	}
	if(saved->single) {
		btHighlightSetSprMode(unitId, SPRITE_OPAQUE);
	} else {
		Sint32 total = btUnitListIds((saved->unitId != 0) + 1, ids);

		for(i = 0; i < total; i++)
			btHighlightSetSprMode(ids[i], SPRITE_OPAQUE);
	}
}

/* Plays an attack: the attacker runs up to its first target and the blow lands.
   A miss makes the target hop back ("nimbly dodges the blow"); a critical hit
   zooms in on the pair with only the two of them shown. */
Sint32 btStagingRunAttack(BATTLE_PLAN *plan, Sint32 mode)
{
	BATTLE_PRESENTATION_STATE state;
	void					 *object;
	SPR						 *anm;
	UNIT					 *unit;
	Sint16					  criticalPair[3];
	Sint32					  critical;
	BATTLE_BACKGROUND_STATE	 *background;
	void					 *saved;
	Sint32					  ang;
	Sint32					  adjusted;
	Sint32					  facingAng;
	Sint32					  ticks;
	Sint32					  missed;
	Sint32					  idx;
	Bool					  zoomOnCritical;
	Uint32					  actorId;
	BATTLE_OBJECT			 *battleObject;

	background = RamHeapSlots->battleBg;
	actorId = plan->unitId;
	battleObject = btObjectGet(actorId);
	/* The camera swings round to watch the run up from the side: it takes the
	   attacker's bearing from the battlefield's centre, an eighth of a turn
	   back for a party member and three eighths on for an enemy, within half a
	   turn, and turns halfway there from the party's action view. */
	{
		Sint32 x = battleObject->anchorX;
		FIXED  z;

		battleObject->obj->x = x;
		z = battleObject->anchorZ;
		battleObject->obj->z = z;
		ang = (Uint16)syAtan(x, z);
		adjusted = ang - DEGtoANG(45);
		if(actorId > ALLY_MAX)
			adjusted = ang + TRIG_TURN * 3 / 8;
	}
	adjusted &= DEGtoANG(180) - 1;
	facingAng = (adjusted - YAW_PARTY_ACTION) / 2 + YAW_PARTY_ACTION;
	if(background->tgtYaw == facingAng) {
		background->tgtYaw = facingAng;
		sySynch(1);
	} else {
		background->tgtYaw = facingAng;
		sySynch(1);
	}

	btHighlightSetModes(0, HIGHLIGHT_OFF);
	btStagingBuildTargetList(plan, &state);
	if(state.kind == EFFECT_ID_SEVERE_BLOW)
		mnPartyStatDrawStub(
			RamHeapSlots->battleState->partyStat & ~PARTY_STATUS_SHOW_PP);
	saved = btMotionCallBeast(plan, &state);
	unit = syUnitGet(state.actor);
	/* DEAD: syUnitGet's result is not used; the ROM keeps the call. */
	syUnitGet(state.actors[0]);
	critical = plan->tgtCritical[0];
	missed = plan->tgtMultiplier[0] == 0;

	object = btObjectGet(plan->unitId)->obj;
	anm = btActorGetSpr(object, 0);
	ticks = fdSprAnimationSumCommandLengthsStub(anm->entries[0]->anmId, 2, 1);
	/* A class whose attack is the burst leaps where it stands. */
	if(state.kind == ATTACK_EFFECT_BURST)
		fdActorSelectAnimationStub(object, POSE_LEAP);
	else
		btMotionApproachTarget(state.actor, state.actors[0], ticks,
			toFIXED(btSprGetAttackHop(unit->enemyId)));
	if((mode & ATTACK_WITH_PARTNER) && plan->unitId != plan->partnerId &&
		plan->partnerId != (state.actors[0] & UNIT_ID_ENEMY_BIT)) {
		object = btObjectGet(plan->partnerId);

		anm = btActorGetSpr(((BATTLE_OBJECT *)object)->obj, 0);
		ticks =
			fdSprAnimationSumCommandLengthsStub(anm->entries[0]->anmId, 2, 1);
		btMotionApproachTarget(plan->partnerId, state.actors[0], ticks,
			toFIXED(btSprGetAttackHop(syUnitGet(plan->partnerId)->enemyId)));
		fdActorSetAnimationSpeedStub(
			(ACTOR *)((BATTLE_OBJECT *)object)->obj, ANIM_STEP_NORMAL);
	}
	fdActorSetAnimationSpeedStub(
		(ACTOR *)btObjectGet(state.actor)->obj, ANIM_STEP_NORMAL);

	if((Uint16)state.actors[0] <= ALLY_MAX)
		state.side = EFFECT_SIDE_PARTY;
	else
		state.side = EFFECT_SIDE_ENEMIES;

	REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
	REG_WIN0V = WIN_RANGE(16, 136);
	REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
	REG_WIN1V = WIN_RANGE(16, 136);
	REG_WININ = WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 |
		WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
		WININ_WIN1_OBJ | WININ_WIN1_CLR;
	REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL;
	REG_DISPCNT |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;

	if(missed != 0) {
		sySynch(10);
		btMotionHopBack(state.actors[0]);
		sySynch(2);
		sySynch(4);
		sySynch(10);
		btPlaybackPush(PLAYBACK_UNIT_NAME, plan->tgtIds[0]);
		btPlaybackPush(PLAYBACK_MESSAGE, (Uint32)&MsgNimblyDodgesBlow);
		btPlaybackRunAll();
		btMotionReturnToSpot(state.actors[0]);
	} else {
		zoomOnCritical = FALSE;
		state.prelude = 0;
		if(plan->battleEff != 0)
			state.prelude = 1;
		if(critical != 0)
			state.kind += EFFECT_ID_CRITICAL_OFFSET;
		if(state.kind != EFFECT_ID_CRITICAL_OFFSET + ATTACK_EFFECT_BURST &&
			critical != 0) {
			zoomOnCritical = TRUE;
			background->keepRange = 1;
			criticalPair[0] = state.actor;
			criticalPair[1] = state.firstTgt;
			criticalPair[2] = BATTLE_UNIT_LIST_END;
			btActorSpawnObjectsForList(criticalPair, 0);
		}
		/* The battle effect starts 8 ticks before the run up ends, at least
		   one tick after it began; a critical hit zooms in on the pair
		   meanwhile. */
		ticks -= 8;
		if(ticks <= 0)
			ticks = 1;
		{
			Sint32 first;
			Sint32 second;

			for(idx = 0; idx != ticks; idx++) {
				if(zoomOnCritical != 0) {
					first = state.actor;
					second = state.firstTgt;
					btCameraSetViewAtPairMidpoint(first, second,
						idx * CRITICAL_ZOOM_GAIN / ticks + CRITICAL_ZOOM_START);
				}
				sySynch(1);
			}
		}
		sySetEvent(btPlaybackRun, EVENT_PASS_TICK);
		if(state.kind != 0) {
			if(saved)
				btMotionReleaseBuffer(saved);
			BATTLE_EFFECT_RUN(&state, plan->battleEff);
		}
		if(saved) {
			btMotionDismissBeast(saved);
			syHeapRelease(saved);
		}
		btPlaybackWaitForReady();
		if(critical != 0) {
			background->keepRange = 0;
			btObjectPutKeepingPoses();
			btCameraSetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
		}
		btMotionReturnToSpot(state.actors[0]);
	}
	if(!saved)
		btMotionReturnToSpot(state.actor);
	return 0;
}

/* Marks the item in `slot` broken; returns -1 for an empty slot. */
Sint32 gmInventoryBreakStub(Sint32 owner, Sint32 slot);

/* Plays an item's use: the camera turns to the user's side, the user raises the
   item and, for an item with a battle effect, the battle background dims to two
   thirds while the effect plays with its messages. Then the item is spent as
   its use type says: used up, broken one time in eight, or the Hermes' Water
   left an Empty Bottle. When a used-up item empties its slot, the user's later
   item commands this turn (`selection` is this one) follow their items to their
   new slots, or lose theirs (-1). */
Sint32 btStagingRunItem(
	BATTLE_PLAN *plan, Sint32 unused, BATTLE_ACTION_RECORD *selection)
{
	BATTLE_PRESENTATION_STATE state;
	MOTION_OBJECT			 *object;
	Sint32					  i;
	Sint32					  inventoryEntry;
	UNIT					 *unit;
	ITEM_DEFINITION			 *item;
	BATTLE_BACKGROUND_STATE	 *background = RamHeapSlots->battleBg;
	Sint32					  tgtYaw;

	if(plan->unitId > FELIX)
		tgtYaw = YAW_ENEMY_ACTION;
	else
		tgtYaw = YAW_PARTY_ACTION;
	if(background->tgtYaw != tgtYaw)
		background->tgtYaw = tgtYaw;
	btStagingBuildTargetList(plan, &state);
	btHighlightSetModes(0, HIGHLIGHT_OFF);
	{
		BATTLE_OBJECT *slot = btObjectGet(state.actor);
		BATTLE_STATE  *battle;
		Uint8		  *mode;

		battle = RamHeapSlots->battleState;
		mode = &battle->actingAnm;
		object = slot->obj;
		fdActorSelectAnimationStub(object, *mode);
	}
	fdActorSetAnimationSpeedStub((ACTOR *)object, ANIM_STEP_NORMAL);
	if(plan->tgtIds[0] <= ALLY_MAX)
		state.side = EFFECT_SIDE_PARTY;
	else
		state.side = EFFECT_SIDE_ENEMIES;

	for(i = 0; i != state.cnt; i++) {
		SPR	  *anm = btActorGetSpr(btObjectGet(state.actors[i])->obj, 0);
		Sint32 entryIdx;

		for(entryIdx = 0; entryIdx != anm->cnt - 1; entryIdx++)
			state.params[i][entryIdx] = anm->entries[entryIdx]->colMode;
	}

	sySetEvent(btPlaybackRun, EVENT_PASS_TICK);
	if(state.kind != 0) {
		FIXED		fade;
		HEAP_SLOTS *heap;

		i = 0;
		heap = RamHeapSlots;
		fade = 0;
		do {
			BATTLE_STATE *battle = heap->battleState;

			if(i < ITEM_DARKEN_TICKS) {
				Uint16 *palette = battle->pal;
				FIXED	value = toFIXED(1) - fade;

				battle->brightness = value;
				btColBufferScale(palette, (Uint32)BATTLE_BG_PALETTE, value,
					BATTLE_BACKGROUND_COLORS);
			}
			sySynch(1);
			i++;
			fade += BACKGROUND_LEVEL_STEP;
		} while(i < ITEM_DARKEN_TICKS);
		BATTLE_EFFECT_RUN(&state, plan->battleEff);
	} else {
		sySynch(60);
	}
	btPlaybackWaitForReady();
	fdActorSelectAnimationStub(object, POSE_STANDING);
	for(i = 0; i != state.cnt; i++)
		btMotionReturnToSpot(state.actors[i]);

	unit = syUnitGet(selection->unitId);
	inventoryEntry = unit->inventory[selection->prm];
	item = gmItemGetDirectStub(inventoryEntry);
	if(item->useType == ITEM_USE_CONSUMED) {
		Sint32 result =
			gmInventoryRemoveStub(selection->unitId, selection->prm);
		Sint32 idx = selection->prm;
		if(result == INVENTORY_REMOVE_LAST) {
			BATTLE_STATE *battle = RamHeapSlots->battleState;
			Uint32		  row;

			for(row = 0; row < BATTLE_ACTION_CAPACITY; row++) {
				if(battle->actions[row].command == COMMAND_ITEM &&
					battle->actions[row].unitId == selection->unitId) {
					if(battle->actions[row].prm == idx)
						battle->actions[row].prm = -1;
					else if(battle->actions[row].prm > idx)
						battle->actions[row].prm--;
				}
			}
		}
	} else if(item->useType == ITEM_USE_MAY_BREAK) {
		if((gmBattleRandomStub() & 7) == 0) {
			btPlaybackPush(PLAYBACK_ITEM_NAME, unit->inventory[selection->prm]);
			btPlaybackPush(PLAYBACK_MESSAGE, (Uint32)&MsgItemBroke);
			gmInventoryBreakStub(selection->unitId, selection->prm);
			btPlaybackRunAll();
		}
	} else if(item->useType == ITEM_USE_EMPTIES) {
		if((inventoryEntry & INVENTORY_ITEM_ID_MASK) == ITEM_HERMES_WATER)
			inventoryEntry = ITEM_EMPTY_BOTTLE;
		unit->inventory[selection->prm] = inventoryEntry;
	}
	return 0;
}

/* Sets the first child's value of each of the object's records and clears the
   others, hiding every child's tick. */
void btObjectSetColMode(MOTION_OBJECT *object, Sint32 value)
{
	Sint32			idx = 0;
	Sint32			childIdx;
	Sint32			cnt;
	SPR			   *record;
	SPR_ANIMATION  *child;
	SPR_ANIMATION **children;
	Uint8			mask = SPRITE_PICTURE_NONE;
	Uint8			frm;

	for(; (record = btActorGetSpr(object, idx)) != NULL; idx++) {
		children = &record->entries[1];
		child = record->entries[0];
		frm = child->picture;
		cnt = record->cnt;
		child->colMode = value;
		/* DEAD: each picture is read and ORed with SPRITE_PICTURE_NONE, which
		   stores SPRITE_PICTURE_NONE whatever it was; the ROM keeps the loads
		   and the ORs. */
		child->picture = frm | mask;
		for(childIdx = 1; childIdx < cnt; childIdx++) {
			child = *children++;
			child->colMode = 0;
			child->picture |= SPRITE_PICTURE_NONE;
		}
	}
}

/* Plays Psynergy, a Djinni or an enemy's cast. The camera turns toward the
   targets from the caster's side, the caster raises its hand and glows in its
   element (no glow for a Djinni); then the cast fails ("blocked" or too little
   PP) or its battle effect plays with its playback, and the targets return to
   their spots. The party status window is drawn without its PP row meanwhile.
   `mode` is an BATTLE_CAST_KIND: STAGING_CAST plays CAST_PSYNERGY,
   STAGING_DJINNI CAST_DJINNI and STAGING_ENEMY_CAST CAST_ENEMY. */
Sint32 btStagingRunPsynergyCast(BATTLE_PLAN *plan, Sint32 mode)
{
	BATTLE_PRESENTATION_STATE state;
	BATTLE_BACKGROUND_STATE	 *background = RamHeapSlots->battleBg;
	MOTION_OBJECT			 *object;
	HEAP_SLOTS				 *heap;
	void					 *saved;
	Sint32					  isDjinni;
	Sint32					  tgtIdx;

	if(plan->battleEff & EFFECT_VIEW_ACROSS) {
		if(plan->unitId <= ALLY_MAX)
			background->tgtYaw = YAW_ENEMY_ACTION;
		else
			background->tgtYaw = YAW_BEHIND_ENEMIES;
		background->ticks = 60;
	} else {
		MOTION_OBJECT *actor = btObjectGet(plan->unitId)->obj;
		Uint32		   tgt = plan->tgtIds[0];
		Uint32		   actorId = plan->unitId;
		Sint32		   ang = (Uint16)syAtan(actor->x, actor->z);
		Sint32		   tgtYaw;

		if(actorId <= ALLY_MAX)
			tgtYaw = ang - TRIG_TURN * 3 / 32;
		else
			tgtYaw = ang + TRIG_TURN * 3 / 32;
		tgtYaw = (Sint16)tgtYaw;
		tgtYaw += ((actorId <= ALLY_MAX ? YAW_PARTY_ACTION : YAW_ENEMY_ACTION) -
					  tgtYaw) *
			3 / 4;
		if(!((actorId <= ALLY_MAX) ^ (tgt <= ALLY_MAX)))
			tgtYaw = actorId <= ALLY_MAX ? TRIG_TURN * 9 / 64
										 : -(TRIG_TURN * 9 / 64);
		if(background->tgtYaw != tgtYaw)
			background->tgtYaw = tgtYaw;
	}
	if(plan->battleEff & EFFECT_VIEW_REVERSED) {
		if(plan->unitId <= ALLY_MAX)
			background->tgtYaw = YAW_ENEMY_ACTION;
		else
			background->tgtYaw = YAW_PARTY_ACTION;
		background->ticks = 60;
	}

	btStagingBuildTargetList(plan, &state);
	isDjinni = mode & CAST_DJINNI;
	if(isDjinni)
		state.prelude = 1;
	btHighlightSetModes(0, HIGHLIGHT_OFF);
	heap = RamHeapSlots;
	mnPartyStatDrawStub(heap->battleState->partyStat & ~PARTY_STATUS_SHOW_PP);
	saved = btMotionCallBeast(plan, &state);
	object = btObjectGet(state.actor)->obj;
	fdActorSelectAnimationStub(object, heap->battleState->actingAnm);
	fdActorSetAnimationSpeedStub((ACTOR *)object, ANIM_STEP_NORMAL);
	sySndPlayStub(SE_CAST);
	if(mode & CAST_ENEMY)
		btElementGlowPlay(state.actor, plan->element, GLOW_STREAKS, 0);
	else if(!isDjinni)
		btElementGlowPlay(state.actor, plan->element, GLOW_STAR_FIELD, 0);
	if(plan->tgtIds[0] <= ALLY_MAX)
		state.side = EFFECT_SIDE_PARTY;
	else
		state.side = EFFECT_SIDE_ENEMIES;

	for(tgtIdx = 0; tgtIdx != state.cnt; tgtIdx++) {
		SPR	  *anm = btActorGetSpr(btObjectGet(state.actors[tgtIdx])->obj, 0);
		Sint32 entryIdx;

		for(entryIdx = 0; entryIdx != anm->cnt - 1; entryIdx++)
			state.params[tgtIdx][entryIdx] = anm->entries[entryIdx]->colMode;
	}
	if(plan->failure != ACTION_FAILURE_NONE) {
		if(plan->failure == ACTION_FAILURE_SEALED) {
			btPlaybackPush(PLAYBACK_UNIT_NAME, plan->unitId);
			btPlaybackPush(PLAYBACK_MESSAGE, (Uint32)&MsgPsynergyBlocked);
		} else {
			btPlaybackPush(PLAYBACK_MESSAGE, (Uint32)&MsgCastLacksPp);
		}
		btPlaybackRunAll();
		btBackgroundRestorePalette();
	} else {
		/* The word after the plan in BATTLE_STATE is skipPlayback. */
		if(*(Sint32 *)(plan + 1) == 0)
			sySetEvent(btPlaybackRun, EVENT_PASS_TICK);
		if(state.kind) {
			if(saved)
				btMotionReleaseBuffer(saved);
			BATTLE_EFFECT_RUN(&state, plan->battleEff);
		} else {
			btBackgroundRestorePalette();
		}
		if(saved) {
			btMotionDismissBeast(saved);
			syHeapRelease(saved);
		}
		if(*(Sint32 *)(plan + 1) == 0)
			btPlaybackWaitForReady();
		fdActorSelectAnimationStub(object, POSE_STANDING);
		for(tgtIdx = 0; tgtIdx != state.cnt; tgtIdx++)
			btMotionReturnToSpot(state.actors[tgtIdx]);
	}
	return 0;
}

/* Flashes a unit as it is hit, white and back twice in its damage pose. */
void btMotionFlashHit(Sint32 id, Sint32 unusedA, Sint32 unusedB)
{
	Uint16 selection[2];
	Sint32 tgt;
	Sint32 flash;

	/* DEAD: syUnitGet's result is not used; the ROM keeps the call. */
	syUnitGet(id);
	fdActorSelectAnimationStub(btObjectGet(id)->obj, POSE_DAMAGE);
	for(flash = 0; flash < 2; flash++) {
		tgt = BATTLE_UNIT_LIST_END;
		selection[1] = tgt;
		selection[0] = id;
		mnPartyStatHighlightMembersStub(selection);
		btObjectSetColMode(btObjectGet(id)->obj, SPRITE_COLOR_WHITE);
		sySynch(2);

		selection[0] = id;
		mnPartyStatHighlightMembersStub(selection);
		btObjectSetColMode(btObjectGet(id)->obj, btMotionGetSlotPalette(id));
		sySynch(2);
	}

	mnPartyStatDrawStub(RamHeapSlots->battleState->partyStat);
}

/* Takes unit id out of the battle: off the party's or the enemies' list, its
   name suffix freed and its actions this turn struck off. Returns -1 when it is
   on neither list. */
Sint32 btObjectRemoveFromLists(Sint32 id)
{
	BATTLE_STATE *battle;
	Sint32		  i;
	Uint32		  j;
	Sint32		  unit;

	battle = RamHeapSlots->battleState;
	syUnitGet(id)->side = BATTLE_SIDE_NONE;
	for(i = 0;; i++) {
		unit = battle->partyUnits[i];
		if(unit == id) {
			battle->partyUnits[i] = BATTLE_UNIT_REMOVED;
			goto removed;
		}
		if(unit == BATTLE_UNIT_LIST_END)
			break;
	}

	for(j = 0;; j++) {
		unit = battle->enemyIds[j];
		if(unit == id) {
			battle->enemyIds[j] = BATTLE_UNIT_REMOVED;
			goto removed;
		}
		if(unit == BATTLE_UNIT_LIST_END)
			break;
	}
	return -1;

removed:
	btEnemyReleaseNameLetter(id);
	for(j = 0; j < BATTLE_ACTION_CAPACITY; j++) {
		if(battle->actions[j].unitId == id)
			battle->actions[j].unitId = BATTLE_UNIT_LIST_END;
	}
	return 0;
}

/* Shows unit id downed: a party member takes POSE_PARTY_HELD; an enemy takes
   POSE_ENEMY_DOWN, turns grey, and four ticks later leaves the battle and
   dissolves pixel by pixel. */
void btMotionShowDowned(Sint32 id)
{
	void		  *sprs[4];
	UNIT		  *unit;
	SPR			  *spr;
	SPR_ANIMATION *entry;
	Sint32		   cnt;

	unit = syUnitGet(id);
	for(cnt = 0; (spr = btActorGetSpr(btObjectGet(id)->obj, cnt)) != 0; cnt++) {
		if(unit->side != BATTLE_SIDE_ENEMY)
			fdSprStartAnimationStub(spr, POSE_PARTY_HELD);
		else
			fdSprStartAnimationStub(spr, POSE_ENEMY_DOWN);
	}

	if(unit->side == BATTLE_SIDE_ENEMY) {
		for(cnt = 0; (spr = btActorGetSpr(btObjectGet(id)->obj, cnt)) != 0;
			cnt++) {
			entry = spr->entries[0];
			sprs[cnt] = spr;
			entry->colMode = SPRITE_COLOR_GREY;
			entry->picture = SPRITE_PICTURE_NONE;
		}
		sySynch(4);
		btObjectRemoveFromLists(id);
		fdSprDissolveRunManyStub(sprs, cnt);
		btObjectMarkForRelease(id);
	}
}
