/* An action played: its targets, and how an attack, a cast, an item, a summon
   or a flight is staged. Part of battle.c. */

#include "battle_internal.h"

/* A critical hit's zoom on the attacker and its target, in percent: from
   CRITICAL_ZOOM_START, CRITICAL_ZOOM_GAIN more by the end of the run up. */
#define CRITICAL_ZOOM_START 100
#define CRITICAL_ZOOM_GAIN	30

/* btStagingRunPlainAttack's flag for a miss: the target hops back and no effect
   plays. Nothing calls the function, so its other flags are unknown. */
#define PLAIN_ATTACK_MISSED 2

/* Plays one action of the turn, after delay ticks facing the party: the action
   is built and staged, its end effects run and every unit returns to its spot.
   Returns 1 when the party got away, -1 when the unit is already down, else
   0. */
Sint32 btStagingDispatchAction(BATTLE_ACTION *request, Sint32 delay)
{
	BATTLE_BACKGROUND_STATE *view;
	BATTLE_STATE			*battle;
	BATTLE_CAMERA			*camera;
	UNIT					*unit;
	Uint16					 actingUnit[2];
	Uint16					 unitIds[UNIT_MAX];
	Bool					 fled;
	Sint32					 result;
	Sint32					 unitCnt;
	Sint32					 unitIdx;

	fled = FALSE;
	if(request->unitId == BATTLE_UNIT_LIST_END)
		return 0;
	unit = gmUnitGetStub(request->unitId);
	if(unit->hp == 0)
		return -1;
	/* An enemy settles its ability now, as it acts. */
	if(unit->classId == CLASS_NONE)
		btAIChooseAction(request, 1);

	/* The camera eases back over 60 ticks, the background at full brightness,
	   the far depth well past the battlefield. */
	view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
	view->ticks = 60;
	battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
	view->keepRange = 0;
	battle->brightness = toFIXED(1);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	syInitMatrix();
	syLookAtLoad(camera->eye, camera->focus);
	sySetScreenDist(BATTLE_CAMERA_DISTANCE,
		syDivFX(BATTLE_CAMERA_DISTANCE, toFIXED(3) / 4),
		PROJECTION_FAR_DEPTH_MAX);
	if(delay != 0) {
		view->tgtYaw = YAW_PARTY_ACTION;
		sySynch(delay);
	}
	/* Build the action, with the acting unit highlighted, and stage it. */
	actingUnit[0] = request->unitId;
	actingUnit[1] = BATTLE_UNIT_LIST_END;
	btHighlightSetModes(actingUnit, HIGHLIGHT_SELECTED);
	result = btActionRunBuild(request, &battle->actionRun);
	if(result == 0) {
		switch(battle->actionRun.staging) {
		case STAGING_MESSAGES:
			btStagingRunMessages(&battle->actionRun, 0);
			break;
		case STAGING_ATTACK:
			btStagingRunAttack(&battle->actionRun, 0);
			break;
		case STAGING_SUMMON:
			btStagingRunFullScreen(&battle->actionRun, STAGING_HIDE_ACTOR);
			break;
		case STAGING_FULL_SCREEN:
			btStagingRunFullScreen(&battle->actionRun, 0);
			break;
		case STAGING_CAST:
			btStagingRunPsynergyCast(&battle->actionRun, CAST_PSYNERGY);
			break;
		case STAGING_DJINNI:
			btStagingRunPsynergyCast(&battle->actionRun, CAST_DJINNI);
			break;
		case STAGING_ENEMY_CAST:
			btStagingRunPsynergyCast(&battle->actionRun, CAST_ENEMY);
			break;
		case STAGING_ITEM:
			btStagingRunItem(&battle->actionRun, 0, request);
			break;
		case STAGING_FLEE:
			if(btStagingRunFlee(&battle->actionRun) != 0)
				fled = TRUE;
			if(fled != 0)
				goto finish;
			break;
		}
	} else {
		if(result == ACTION_REPORTED) {
			btMessageWaitForAdvance();
			sySynch(3);
		}
		btHighlightSetModes(0, HIGHLIGHT_OFF);
	}

	/* The action's end, then every unit back to its spot. A party that fled
	   skips this and the action's mark. */
	btObjectDestroyAll();
	btUnitProcessActionEnd(&battle->actionRun);
	btObjectPut();
	unitCnt = btUnitListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
	for(unitIdx = 0; unitIdx < unitCnt; unitIdx++)
		btMotionReturnToSpot(unitIds[unitIdx]);
	request->unitId = BATTLE_UNIT_LIST_END;
finish:
	btBackgroundSetMode(BATTLE_BACKGROUND_ZOOMED, BattleStateWork->bg, 0);
	return fled;
}

/* Fills a battle effect command from an action run: its kind and strength, the
   user and the targets still standing, or all of them for an effect that
   revives. */
UNREAD_RESULT btStagingBuildTargetList(
	BATTLE_ACTION_RUN *plan, BATTLE_EFFECT_PARM *args)
{
	Sint32 cnt = 0;
	Sint32 i;

	args->prelude = 0;
	args->kind = plan->battleEff & EFFECT_ID_MASK;
	args->variant =
		(plan->battleEff & EFFECT_VARIANT_MASK) >> EFFECT_VARIANT_SHIFT;
	args->user = plan->unitId;
	for(i = 0; i < plan->tgtCnt; i++) {
		if(gmUnitGetStub(plan->tgtIds[i])->hp != 0 ||
			(plan->battleEff & EFFECT_ON_DOWNED))
			args->tgts[cnt++] = plan->tgtIds[i];
	}
	if(cnt == 0) {
		args->tgts[0] = plan->tgtIds[0];
		cnt = 1;
	}
	args->firstTgt = plan->tgtIds[0];
	args->tgtCnt = cnt;
	args->range = 1;
}

/* Plays a unit's attempt to run: the party runs off one by one unless the roll
   failed ("But there's no escape!"); an enemy gets away 7 times in 10. Returns
   1 when the party got away. */
Sint32 btStagingRunFlee(BATTLE_ACTION_RUN *plan)
{
	BATTLE_BACKGROUND_STATE *view =
		HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
	BATTLE_STATE *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
	Bool		  completed = FALSE;
	Sint32		  rollSucceeded = 0;

	view->tgtYaw = YAW_PARTY_ACTION;
	view->busy = 1;
	btHighlightSetModes(0, HIGHLIGHT_OFF);

	/* The party: each party member that can move runs, the last listed first,
	   eight ticks apart. */
	if(plan->unitId <= ALLY_MAX) {
		rollSucceeded = completed;

		/* A failed roll (btFleeCheckSuccess) gave the enemies the turn. */
		if(battle->firstStrike != FIRST_STRIKE_ENEMIES) {
			rollSucceeded = 1;
		}

		if(!rollSucceeded) {
			mnBattleMessageShowAndWaitStub(MsgNoEscape);
			btMessageWaitForAdvance();
		} else {
			Sint16 unitIds[UNIT_MAX];
			Sint32 idx =
				btUnitListLiving(BATTLE_SIDE_MASK_PARTY, (Uint16 *)unitIds) - 1;

			while(idx != -1) {
				UNIT *unit = gmUnitGetStub(unitIds[idx]);

				if(unit->stun == 0 && unit->sleep == 0) {
					btMotionSetupEscapeObject(unitIds[idx]);
					sySynch(8);
				}
				idx--;
			}
			sySynch(22);
			completed = TRUE;
		}
	} else if(syRandom16Below(10) <= 6) {
		/* An enemy: a roll of 0 to 9, 7 in 10 away. */
		Sint16 fleeing[2];

		fleeing[0] = plan->unitId;
		fleeing[1] = BATTLE_UNIT_LIST_END;
		btMotionSetupEscapeObject(fleeing[0]);
		sySynch(8);
		btObjectRemoveFromLists(plan->unitId);
		btObjectMarkForRelease(plan->unitId);
	} else {
		mnBattleMessageShowAndWaitStub(MsgNoEscape);
		btMessageWaitForAdvance();
	}

	view->busy = 0;
	return completed;
}

/* Plays a summon or a full-screen Psynergy: the unit glows in its element and
   the other side fades out while the effect plays, then everyone fades back in.
   STAGING_HIDE_ACTOR hides the unit too, as the summons do. */
Sint32 btStagingRunFullScreen(BATTLE_ACTION_RUN *plan, Sint32 mode)
{
	Uint16				 units[UNIT_MAX];
	BATTLE_STAGING_STATE state;
	Uint32				 userId;
	Uint32				 tgtId;
	Uint32				 cnt;
	Sint32				 i;
	Sint32				 n;
	Uint32				 kept;
	ACTOR				*userActor;
	/* UB: indexing past scalar BattleStateWork reaches another heap slot. */
#if !FIX_POINTER_OUTSIDE_OBJECT
	void **slots = (void **)&BattleStateWork;
#endif

	/* The camera turns to the acting unit's side for a staged effect, else to
	   the usual view. */
	btStagingBuildTargetList(plan, &state.eff);
	userId = plan->unitId;
	tgtId = plan->tgtIds[0];
	if(plan->battleEff & EFFECT_SUMMON) {
		BATTLE_BACKGROUND_STATE *view;
		Sint32					 yaw;
#if FIX_POINTER_OUTSIDE_OBJECT
		view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
#else
		view = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
#endif
		yaw = YAW_PARTY_ACTION;
		if(userId > ALLY_MAX)
			yaw = YAW_BEHIND_ENEMIES;
		view->tgtYaw = yaw;
		view->ticks = 60;
	} else {
		BATTLE_BACKGROUND_STATE *view;
		Sint32					 yaw;
#if FIX_POINTER_OUTSIDE_OBJECT
		view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
#else
		view = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
#endif
		yaw = YAW_ENEMY_ACTION;
		if(userId <= ALLY_MAX)
			yaw = YAW_PARTY_ACTION;
		if(view->tgtYaw != yaw)
			view->tgtYaw = yaw;
	}

	/* The unit raises its hand and glows; the side not targeted fades out. */
	btHighlightSetModes(0, HIGHLIGHT_OFF);
	mnPartyStatDrawStub(BattleStateWork->partyStat & ~PARTY_STATUS_SHOW_PP);
	userActor = btObjectGet(userId)->actor;
	REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
	cnt = btUnitListIds(BATTLE_SIDE_MASK_BOTH, units);
	for(i = 0; i != cnt; i++) {
		if(units[i] == BATTLE_UNIT_REMOVED)
			continue;
		if(units[i] != userId) {
			if((tgtId <= ALLY_MAX) != (units[i] <= ALLY_MAX))
				btHighlightSetSprMode(units[i], SPRITE_SEMI_TRANSPARENT);
		} else {
			fdActorSelectAnimationStub(userActor, POSE_CAST);
		}
	}
	/* The sound of a cast, as the caster raises its hand. */
	sySndPlayStub(SE_CAST);
	btElementGlowPlay(state.eff.user, plan->element, GLOW_STAR_FIELD, 0);
	if(mode & STAGING_HIDE_ACTOR)
		btHighlightSetSprMode(userId, SPRITE_SEMI_TRANSPARENT);
	for(i = 0; i != BLEND_WEIGHT_MAX; i++) {
		REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - i, BLEND_WEIGHT_MAX);
		sySynch(1);
	}
	/* A failed cast: "blocked" (sealed) or too little PP, shown at once. */
	if(plan->failure != ACTION_FAILURE_NONE) {
		if(plan->failure == ACTION_FAILURE_SEALED) {
			btPlaybackPush(PLAYBACK_UNIT_NAME, plan->unitId);
			btPlaybackPush(PLAYBACK_MESSAGE, MsgPsynergyBlocked);
		} else {
			btPlaybackPush(PLAYBACK_MESSAGE, MsgCastLacksPp);
		}
		btPlaybackRunAll();
		btBackgroundRestorePalette();
	} else {
		/* Only the target side stays on the field. */
		kept = 0;
		for(i = 0; i < cnt; i++) {
			Uint32 unit = units[i];
			if(unit == userId) {
				if(!(mode & STAGING_HIDE_ACTOR))
					units[kept++] = userId;
			} else if((tgtId <= ALLY_MAX) == (unit <= ALLY_MAX)) {
				units[kept++] = unit;
			}
		}
		units[kept] = BATTLE_UNIT_LIST_END;
		btObjectSpawnObjectsForList(units, 0);
		for(n = 0; n < plan->tgtCnt; n++)
			units[n] = plan->tgtIds[n];
		units[n] = BATTLE_UNIT_LIST_END;
		/* Each target's colour modes are saved beside the effect command. XXX
		   (no evidence): what for; nothing reads them back. */
		for(i = 0; i != state.eff.tgtCnt; i++) {
			SPR	  *anm;
			Uint32 child;

			anm = btActorGetSpr(btObjectGet(state.eff.tgts[i])->actor, 0);
			for(child = 0; child != anm->cnt - 1; child++)
				state.savedAnmPrms[i][child] = anm->entries[child]->colMode;
		}
		/* The side the battle effect plays on: the targets'. */
		if(plan->battleEff & EFFECT_SUMMON) {
			if(tgtId > ALLY_MAX)
				state.eff.side = EFFECT_SIDE_ENEMIES;
			else
				state.eff.side = EFFECT_SIDE_PARTY;
		} else {
			if(plan->tgtIds[0] <= ALLY_MAX)
				state.eff.side = EFFECT_SIDE_PARTY;
			else
				state.eff.side = EFFECT_SIDE_ENEMIES;
		}
		if(plan->battleEff & EFFECT_OTHER_SIDE)
			state.eff.side ^= 1;
		/* The battle effect runs to its end while playback, an event, plays
		   each target's outcome from the tick the blow lands. */
		sySetEvent(btPlaybackRun, EVENT_PASS_TICK);
		if(plan->battleEff & EFFECT_SUMMON)
			btEffectRunSummonStub(&state.eff);
		else
			BATTLE_EFFECT_RUN(&state.eff, plan->battleEff);
		btPlaybackWaitForReady();
	}

	/* Everyone back: the faded units fade in over 16 ticks and every colour
	   mode is cleared. */
	btObjectPut();
	cnt = btUnitListIds(BATTLE_SIDE_MASK_BOTH, units);
	REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
	for(i = 0; i != cnt; i++) {
		if(units[i] == BATTLE_UNIT_REMOVED || units[i] == userId)
			continue;
		if((tgtId <= ALLY_MAX) != (units[i] <= ALLY_MAX))
			btHighlightSetSprMode(units[i], SPRITE_SEMI_TRANSPARENT);
	}
	for(i = 0; i != BLEND_WEIGHT_MAX; i++) {
		REG_BLDALPHA = BLEND_ALPHA(i, BLEND_WEIGHT_MAX);
		sySynch(1);
	}
	for(i = 0; i != cnt; i++)
		btHighlightSetSprMode(units[i], SPRITE_OPAQUE);
	btCameraSetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
	sySynch(1);
	return 0;
}

/* Clears the highlights, then applies the action to each target in turn and
   plays its events; with no target, only waits for the player. */
UNREAD_RESULT btStagingRunMessages(BATTLE_ACTION_RUN *plan, Sint32 unused)
{
	Sint32 tgtIdx;

	btHighlightSetModes(0, HIGHLIGHT_OFF);
	if(plan->tgtCnt == 0) {
		btMessageWaitForAdvance();
	} else {
		for(tgtIdx = 0; tgtIdx < plan->tgtCnt; tgtIdx++) {
			btAbilityApplyToTarget(plan, tgtIdx);
			btPlaybackRunAll();
		}
	}
	sySynch(1);
}

/* Plays an attack: the attacker runs up to its first target and the blow lands.
   A miss makes the target hop back ("nimbly dodges the blow"); a critical hit
   zooms in on the pair with only the two of them shown. */
Sint32 btStagingRunAttack(BATTLE_ACTION_RUN *plan, Sint32 mode)
{
	BATTLE_STAGING_STATE	 state;
	ACTOR					*actor;
	SPR						*anm;
	UNIT					*unit;
	Sint16					 criticalPair[3];
	Sint32					 critical;
	BATTLE_BACKGROUND_STATE *view;
	Sint32					 ang;
	Sint32					 adjusted;
	Sint32					 facingAng;
	FIXED					 x;
	FIXED					 z;
	Sint32					 ticks;
	Sint32					 missed;
	Sint32					 idx;
	Bool					 zoomOnCritical;

	view = BattleBackgroundStateWork;
	{
		BATTLE_ACTION_RUN *savedPlan = plan;

		/* FAKEMATCH(dead): plan cleared and restored under tests that do
		 * nothing. */
		if(view)
			plan = NULL;
		if(view)
			plan = savedPlan;
	}

	/* The camera turns halfway to the attacker's view. */
	actor = btObjectGet(plan->unitId)->actor;
	z = actor->z.fixed;
	x = actor->x.fixed;
	ang = (Uint16)syAtan(x, z);
	adjusted = ang - DEGtoANG(45);
	if(plan->unitId > ALLY_MAX)
		adjusted = ang + TRIG_TURN * 3 / 8;
	adjusted &= TRIG_TURN / 2 - 1;
	facingAng = (adjusted - YAW_PARTY_ACTION) / 2 + YAW_PARTY_ACTION;

	if(view->tgtYaw == facingAng) {
		view->tgtYaw = facingAng;
		sySynch(5);
	} else {
		view->tgtYaw = facingAng;
		sySynch(10);
	}

	btHighlightSetModes(0, HIGHLIGHT_OFF);
	btStagingBuildTargetList(plan, &state.eff);
	/* Severe Blow drops the status window's PP row while it plays. */
	if(state.eff.kind == EFFECT_ID_SEVERE_BLOW)
		mnPartyStatDrawStub(BattleStateWork->partyStat & ~PARTY_STATUS_SHOW_PP);

	unit = gmUnitGetStub(state.eff.user);
	/* DEAD: the first target, fetched and not used; the ROM keeps the call. */
	gmUnitGetStub(state.eff.tgts[0]);
	critical = plan->tgtCritical[0];
	missed = plan->tgtMultiplier[0] == 0;

	/* The run up: as long as the attacker's POSE_LEAP, with its class's hop
	   height. */
	anm = btActorGetSpr(btObjectGet(plan->unitId)->actor, 0);
	ticks = fdSprAnimationSumCommandLengthsStub(anm->entries[0]->anmId, 2, 1);
	btMotionApproachTarget(state.eff.user, state.eff.tgts[0], ticks,
		toFIXED(btSprGetAttackHop(unit->id)));
	fdActorSetAnimationSpeedStub(
		btObjectGet(state.eff.user)->actor, ANIM_STEP_NORMAL);
	/* DEAD: btObjectGet on the target, unused; the ROM keeps the call. */
	btObjectGet(state.eff.tgts[0]);

	if((Uint16)state.eff.tgts[0] <= ALLY_MAX)
		state.eff.side = EFFECT_SIDE_PARTY;
	else
		state.eff.side = EFFECT_SIDE_ENEMIES;

	REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
	REG_WIN0V = WIN_RANGE(16, 136);
	REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
	REG_WIN1V = WIN_RANGE(16, 136);
	REG_WININ = BATTLE_WININ;
	REG_WINOUT = BATTLE_WINOUT;
	REG_DISPCNT |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;

	/* A miss: after 10 ticks the target hops back; 16 ticks later the dodge
	   is shown at once. */
	if(missed != 0) {
		sySynch(10);
		btMotionHopBack(state.eff.tgts[0]);
		sySynch(2);
		sySynch(4);
		sySynch(10);
		btPlaybackPush(PLAYBACK_UNIT_NAME, plan->tgtIds[0]);
		btPlaybackPush(PLAYBACK_MESSAGE, MsgNimblyDodgesBlow);
		btPlaybackRunAll();
		btMotionReturnToSpot(state.eff.tgts[0]);
	} else {
		/* A hit; a critical one zooms in on the pair over the rest of the run
		   up. */
		zoomOnCritical = FALSE;
		state.eff.prelude = 0;
		if(plan->battleEff != 0)
			state.eff.prelude = 1;

		if(critical != 0) {
			state.eff.kind += EFFECT_ID_CRITICAL_OFFSET;
			zoomOnCritical = TRUE;
			view->keepRange = 1;
			criticalPair[0] = state.eff.user;
			criticalPair[1] = state.eff.firstTgt;
			criticalPair[2] = BATTLE_UNIT_LIST_END;
			btObjectSpawnObjectsForList(criticalPair, 0);
		}

		/* The battle effect starts 8 ticks before the run up ends, at least
		   one tick after it began. */
		ticks -= 8;
		if(ticks <= 0)
			ticks = 1;
		{
			Sint32 first;
			Sint32 second;

			for(idx = 0; idx != ticks; idx++) {
				if(zoomOnCritical != 0) {
					first = state.eff.user;
					second = state.eff.firstTgt;
					btCameraSetViewAtPairMidpoint(first, second,
						idx * CRITICAL_ZOOM_GAIN / ticks + CRITICAL_ZOOM_START);
				}
				sySynch(1);
			}
		}

		sySetEvent(btPlaybackRun, EVENT_PASS_TICK);
		if(state.eff.kind != 0) {
			BATTLE_EFFECT_RUN(&state.eff, plan->battleEff);
		}
		btPlaybackWaitForReady();
		/* After a critical hit every unit is shown again and the zoom ends. */
		if(critical != 0) {
			view->keepRange = 0;
			btObjectPutKeepingPoses();
			btCameraSetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
		}
		btMotionReturnToSpot(state.eff.tgts[0]);
	}
	btMotionReturnToSpot(state.eff.user);
	return 0;
}

/* Unused: a plainer attack run, with no hop, no critical zoom and no dodge
   message. */
UNUSED Sint32 btStagingRunPlainAttack(BATTLE_ACTION_RUN *input, Sint32 flg)
{
	BATTLE_STAGING_STATE	 state;
	BATTLE_ACTION_RUN		*savedInput;
	ACTOR					*actor;
	SPR						*anm;
	BATTLE_BACKGROUND_STATE *view;
	Sint32					 ang;
	Sint32					 adjusted;
	Sint32					 facingAng;
	FIXED					 x;
	FIXED					 z;
	Sint32					 ticks;
	Sint32					 missed;

	view = BattleBackgroundStateWork;
	savedInput = input;
	actor = btObjectGet(savedInput->unitId)->actor;
	z = actor->z.fixed;
	x = actor->x.fixed;
	ang = (Uint16)syAtan(x, z);
	adjusted = ang - DEGtoANG(45);
	if(savedInput->unitId > ALLY_MAX)
		adjusted = ang + TRIG_TURN * 3 / 8;
	adjusted &= TRIG_TURN / 2 - 1;
	facingAng = (adjusted - YAW_PARTY_ACTION) / 2 + YAW_PARTY_ACTION;

	if(view->tgtYaw == facingAng) {
		view->tgtYaw = facingAng;
		sySynch(5);
	} else {
		view->tgtYaw = facingAng;
		sySynch(20);
	}

	btHighlightSetModes(0, HIGHLIGHT_OFF);
	btStagingBuildTargetList(savedInput, &state.eff);
	gmUnitGetStub(state.eff.user);
	gmUnitGetStub(savedInput->tgtIds[0]);

	missed = flg & PLAIN_ATTACK_MISSED;
	anm = btActorGetSpr(btObjectGet(savedInput->unitId)->actor, 0);
	ticks = fdSprAnimationSumCommandLengthsStub(anm->entries[0]->anmId, 2, 1);
	btMotionApproachTarget(state.eff.user, savedInput->tgtIds[0], ticks, 0);
	fdActorSetAnimationSpeedStub(
		btObjectGet(state.eff.user)->actor, ANIM_STEP_NORMAL);
	btObjectGet(savedInput->tgtIds[0]);

	if(savedInput->tgtIds[0] <= ALLY_MAX)
		state.eff.side = EFFECT_SIDE_PARTY;
	else
		state.eff.side = EFFECT_SIDE_ENEMIES;
	if(missed != 0) {
		sySynch(10);
		btMotionHopBack(savedInput->tgtIds[0]);
		sySynch(2);
		sySynch(4);
		sySynch(10);
		btMotionReturnToSpot(savedInput->tgtIds[0]);
	} else {
		btEffectRunByIdRangeStub(&state.eff);
		btPlaybackRunAll();
		btMotionReturnToSpot(savedInput->tgtIds[0]);
	}
	btMotionReturnToSpot(state.eff.user);
	return 0;
}

/* Plays an item's use: the user raises the item and, for one with a battle
   effect, the screen dims while it plays. Then the item is spent as its use
   type says, and later item commands of the same party member this turn follow
   their items to their new slots. */
Sint32 btStagingRunItem(
	BATTLE_ACTION_RUN *plan, Sint32 unused, BATTLE_ACTION *selection)
{
	BATTLE_STAGING_STATE	 state;
	ACTOR					*actor;
	Sint32					 i;
	Sint32					 inventoryEntry;
	UNIT					*unit;
	ITEM_DEFINITION			*item;
	BATTLE_BACKGROUND_STATE *view = BattleBackgroundStateWork;
	Sint32					 tgtYaw;

	/* The camera turns to the user's side, and the user raises the item. XXX
	   (no evidence): whether > 4 is a slip for > ALLY_MAX; only Jenna, Sheba
	   and Piers would take the enemies' view, and none of them fights here. */
	if(plan->unitId > 4)
		tgtYaw = YAW_ENEMY_ACTION;
	else
		tgtYaw = YAW_PARTY_ACTION;
	if(view->tgtYaw != tgtYaw)
		view->tgtYaw = tgtYaw;
	btStagingBuildTargetList(plan, &state.eff);
	btHighlightSetModes(0, HIGHLIGHT_OFF);
	actor = btObjectGet(state.eff.user)->actor;
	fdActorSelectAnimationStub(actor, POSE_CAST);
	fdActorSetAnimationSpeedStub(actor, ANIM_STEP_NORMAL);
	if(plan->tgtIds[0] <= ALLY_MAX)
		state.eff.side = EFFECT_SIDE_PARTY;
	else
		state.eff.side = EFFECT_SIDE_ENEMIES;

	for(i = 0; i != state.eff.tgtCnt; i++) {
		SPR	  *anm = btActorGetSpr(btObjectGet(state.eff.tgts[i])->actor, 0);
		Sint32 entryIdx;

		for(entryIdx = 0; entryIdx != anm->cnt - 1; entryIdx++)
			state.savedAnmPrms[i][entryIdx] = anm->entries[entryIdx]->colMode;
	}

	/* An item with a battle effect dims the background, then plays it; one with
	   none waits a second. */
	sySetEvent(btPlaybackRun, EVENT_PASS_TICK);
	if(state.eff.kind != 0) {
		FIXED fade;

		i = 0;
		fade = 0;
		do {
			BATTLE_STATE *battle = BattleStateWork;

			if(i < 20) {
				Uint16 *palette = battle->pal;
				FIXED	value = toFIXED(1) - fade;

				battle->brightness = value;
				btGraphicsScaleRGB555Clamped(palette, BATTLE_BG_PALETTE, value,
					BATTLE_BACKGROUND_COLORS);
			}
			sySynch(1);
			i++;
			fade += toFIXED(1) / 60;
		} while(i < 20);
		BATTLE_EFFECT_RUN(&state.eff, plan->battleEff);
	} else {
		sySynch(60);
	}
	btPlaybackWaitForReady();
	fdActorSelectAnimationStub(actor, POSE_STANDING);
	for(i = 0; i != state.eff.tgtCnt; i++)
		btMotionReturnToSpot(state.eff.tgts[i]);

	/* Spend the item, by its use type. */
	unit = gmUnitGetStub(selection->unitId);
	inventoryEntry = unit->inventory[selection->prm];
	item = gmItemGetDirectStub(inventoryEntry);
	if(item->useType == ITEM_USE_CONSUMED) {
		Sint32 result =
			gmInventoryRemoveStub(selection->unitId, selection->prm);
		Sint32 idx = selection->prm;
		if(result == INVENTORY_REMOVE_LAST) {
			BATTLE_STATE *battle = BattleStateWork;
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
		/* One time in eight it breaks, shown at once. */
		if((gmBattleRandomStub() & 7) == 0) {
			btPlaybackPush(PLAYBACK_ITEM_NAME, unit->inventory[selection->prm]);
			btPlaybackPush(PLAYBACK_MESSAGE, MsgUsedItemBroke);
			gmInventoryBreakStub(selection->unitId, selection->prm);
			btPlaybackRunAll();
		}
	} else if(item->useType == ITEM_USE_EMPTIES) {
		/* Hermes' Water becomes one Empty Bottle, the slot's other bits
		   dropped; any other such item stays as it was. */
		if((inventoryEntry & INVENTORY_ITEM_ID_MASK) == ITEM_HERMES_WATER)
			inventoryEntry = ITEM_EMPTY_BOTTLE;
		unit->inventory[selection->prm] = inventoryEntry;
	}
	return 0;
}

/* Draws every sprite of an actor in a colour mode, its first layer in colMode
   and the others plain. */
UNREAD_RESULT btObjectSetColMode(ACTOR *actor, Sint32 colMode)
{
	Sint32			recordIdx = 0;
	Sint32			childIdx;
	Sint32			entryCnt;
	SPR			   *record;
	SPR_ANIMATION  *child;
	SPR_ANIMATION **children;
	Uint8			lastPictureMask = SPRITE_PICTURE_NONE;
	Uint8			prevPicture;

	for(; (record = btActorGetSpr(actor, recordIdx)) != NULL; recordIdx++) {
		children = &record->entries[1];
		child = record->entries[0];
		prevPicture = child->picture;
		child->picture = prevPicture | lastPictureMask;
		entryCnt = record->cnt;
		child->colMode = colMode;
		for(childIdx = 1; childIdx < entryCnt; childIdx++) {
			child = *children++;
			child->colMode = 0;
			child->picture = SPRITE_PICTURE_NONE;
		}
	}
}

/* Plays Psynergy, a Djinni or an enemy's cast: the caster raises its hand and
   glows in its element, then the cast fails or its battle effect plays, and the
   targets return to their spots. */
Sint32 btStagingRunPsynergyCast(BATTLE_ACTION_RUN *plan, Sint32 mode)
{
	BATTLE_STAGING_STATE	 state;
	BATTLE_BACKGROUND_STATE *view = BattleBackgroundStateWork;
	ACTOR					*object;
	Sint32					 isDjinni;
	Sint32					 tgtIdx;

	/* The camera turns toward the targets from the caster's side, or across. */
	if(plan->battleEff & EFFECT_VIEW_ACROSS) {
		view->tgtYaw =
			plan->unitId <= ALLY_MAX ? YAW_ENEMY_ACTION : YAW_BEHIND_ENEMIES;
		view->ticks = 60;
	} else {
		ACTOR *actor = btObjectGet(plan->unitId)->actor;
		Sint32 ang = (Uint16)syAtan(actor->x.fixed, actor->z.fixed);
		Sint32 tgtYaw;

		if(plan->unitId <= ALLY_MAX)
			tgtYaw = ang - TRIG_TURN * 3 / 32;
		else
			tgtYaw = ang + TRIG_TURN * 3 / 32;
		tgtYaw = (Sint16)tgtYaw;
		tgtYaw +=
			((plan->unitId <= ALLY_MAX ? YAW_PARTY_ACTION : YAW_ENEMY_ACTION) -
				tgtYaw) *
			3 / 4;
		if(!((plan->unitId <= ALLY_MAX) ^ (plan->tgtIds[0] <= ALLY_MAX)))
			tgtYaw = plan->unitId <= ALLY_MAX ? TRIG_TURN * 9 / 64
											  : -(TRIG_TURN * 9 / 64);
		if(view->tgtYaw != tgtYaw)
			view->tgtYaw = tgtYaw;
	}
	if(plan->battleEff & EFFECT_VIEW_REVERSED) {
		view->tgtYaw =
			plan->unitId <= ALLY_MAX ? YAW_ENEMY_ACTION : YAW_PARTY_ACTION;
		view->ticks = 60;
	}

	/* The caster raises its hand and glows in its element; a Djinni plays its
	   prelude instead. */
	btStagingBuildTargetList(plan, &state.eff);
	isDjinni = mode & CAST_DJINNI;
	if(isDjinni)
		state.eff.prelude = 1;
	btHighlightSetModes(0, HIGHLIGHT_OFF);
	mnPartyStatDrawStub(BattleStateWork->partyStat & ~PARTY_STATUS_SHOW_PP);
	object = btObjectGet(state.eff.user)->actor;
	fdActorSelectAnimationStub(object, POSE_CAST);
	fdActorSetAnimationSpeedStub(object, ANIM_STEP_NORMAL);
	sySndPlayStub(SE_CAST);
	if(mode & CAST_ENEMY)
		btElementGlowPlay(state.eff.user, plan->element, GLOW_STREAKS, 0);
	else if(!isDjinni)
		btElementGlowPlay(state.eff.user, plan->element, GLOW_STAR_FIELD, 0);
	if(plan->tgtIds[0] <= ALLY_MAX)
		state.eff.side = EFFECT_SIDE_PARTY;
	else
		state.eff.side = EFFECT_SIDE_ENEMIES;

	for(tgtIdx = 0; tgtIdx != state.eff.tgtCnt; tgtIdx++) {
		SPR *anm = btActorGetSpr(btObjectGet(state.eff.tgts[tgtIdx])->actor, 0);
		Sint32 entryIdx;

		for(entryIdx = 0; entryIdx != anm->cnt - 1; entryIdx++)
			state.savedAnmPrms[tgtIdx][entryIdx] =
				anm->entries[entryIdx]->colMode;
	}

	/* A failed cast is shown at once; otherwise the battle effect plays during
	   playback, and an ability with no battle effect only restores the
	   palette. */
	if(plan->failure != ACTION_FAILURE_NONE) {
		if(plan->failure == ACTION_FAILURE_SEALED) {
			btPlaybackPush(PLAYBACK_UNIT_NAME, plan->unitId);
			btPlaybackPush(PLAYBACK_MESSAGE, MsgPsynergyBlocked);
		} else {
			btPlaybackPush(PLAYBACK_MESSAGE, MsgCastLacksPp);
		}
		btPlaybackRunAll();
		btBackgroundRestorePalette();
	} else {
		sySetEvent(btPlaybackRun, EVENT_PASS_TICK);
		if(state.eff.kind) {
			BATTLE_EFFECT_RUN(&state.eff, plan->battleEff);
		} else {
			btBackgroundRestorePalette();
		}
		btPlaybackWaitForReady();
		fdActorSelectAnimationStub(object, POSE_STANDING);
		for(tgtIdx = 0; tgtIdx != state.eff.tgtCnt; tgtIdx++)
			btMotionReturnToSpot(state.eff.tgts[tgtIdx]);
	}
	return 0;
}

/* Flashes a unit as it is hit, white and back twice in its damage pose. */
void btMotionFlashHit(Sint32 unitId, Sint32 unusedA, Sint32 unusedB)
{
	Uint16 selection[2];
	Sint32 flash;

	/* DEAD: the unit, fetched and not used; the ROM keeps the call. */
	gmUnitGetStub(unitId);
	fdActorSelectAnimationStub(btObjectGet(unitId)->actor, POSE_DAMAGE);
	for(flash = 0; flash < 2; flash++) {
		selection[0] = unitId;
		selection[1] = BATTLE_UNIT_LIST_END;
		mnPartyStatHighlightMembersStub(selection);
		btObjectSetColMode(btObjectGet(unitId)->actor, SPRITE_COLOR_WHITE);
		sySynch(2);

		selection[0] = unitId;
		mnPartyStatHighlightMembersStub(selection);
		btObjectSetColMode(
			btObjectGet(unitId)->actor, btObjectGetPalette(unitId));
		sySynch(2);
	}
	mnPartyStatDrawStub(BattleStateWork->partyStat);
}

/* Takes a unit out of its side's list and this turn's actions, and frees its
   name letter. */
void btObjectRemoveFromLists(Sint32 unitId)
{
	BATTLE_STATE *battle;
	Sint32		  partyIdx;
	Uint32		  i;
	Sint32		  listedUnit;

	/* The party's list. */
	battle = BattleStateWork;
	gmUnitGetStub(unitId)->side = BATTLE_SIDE_NONE;
	for(partyIdx = 0;; partyIdx++) {
		if(battle->partyUnits[partyIdx] == unitId) {
			battle->partyUnits[partyIdx] = BATTLE_UNIT_REMOVED;
			goto removed;
		}
		if(battle->partyUnits[partyIdx] == BATTLE_UNIT_LIST_END)
			break;
	}

	/* The enemies' list. */
	for(i = 0;; i++) {
		listedUnit = battle->enemyIds[i];
		if(listedUnit == unitId) {
			battle->enemyIds[i] = BATTLE_UNIT_REMOVED;
			goto removed;
		}
		if(listedUnit == BATTLE_UNIT_LIST_END)
			break;
	}
	return;

removed:
	/* Free the letter that told the enemy apart from others of its class. */
	btEnemyReleaseNameLetter(unitId);
	for(i = 0; i < BATTLE_ACTION_CAPACITY; i++) {
		if(battle->actions[i].unitId == unitId)
			battle->actions[i].unitId = BATTLE_UNIT_LIST_END;
	}
}

/* Shows a downed unit: a party member lies down; an enemy turns grey and
   dissolves a dot at a time. */
void btMotionShowDowned(Sint32 unitId)
{
	void		  *records[4];
	UNIT		  *unit;
	SPR			  *record;
	SPR_ANIMATION *entry;
	Sint32		   recordCnt;

	unit = gmUnitGetStub(unitId);
	for(recordCnt = 0;
		(record = btActorGetSpr(btObjectGet(unitId)->actor, recordCnt)) != 0;
		recordCnt++) {
		if(unit->side != BATTLE_SIDE_ENEMY)
			fdSprStartAnimationStub(record, POSE_PARTY_HELD);
		else
			fdSprStartAnimationStub(record, POSE_ENEMY_DOWN);
	}

	if(unit->side == BATTLE_SIDE_ENEMY) {
		for(recordCnt = 0; (record = btActorGetSpr(
								btObjectGet(unitId)->actor, recordCnt)) != 0;
			recordCnt++) {
			entry = record->entries[0];
			records[recordCnt] = record;
			entry->colMode = SPRITE_COLOR_GREY;
			entry->picture = SPRITE_PICTURE_NONE;
		}
		sySynch(4);
		btObjectRemoveFromLists(unitId);
		/* The whole dissolve, over 32 ticks. */
		fdSprDissolveRunManyStub((Uint8 **)records, recordCnt);
		btObjectMarkForRelease(unitId);
	}
}

#undef CRITICAL_ZOOM_START
#undef CRITICAL_ZOOM_GAIN
