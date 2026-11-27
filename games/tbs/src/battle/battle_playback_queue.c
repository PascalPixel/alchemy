/* @file */
/* Battle playback: a downed unit's reset, waiting for the player, and the */
/* playback queue, filled by btPlaybackPush and played at once by */
/* btPlaybackRunAll. */
/* Part of battle.c. */

#include "battle_internal.h"

/* Clears every status and stat change of a unit that goes down and works out */
/* its stats again. */
UNREAD_RESULT btUnitClearStatuses(s32 unitId) {
	UNIT *unit;
	s32 element;

	unit = gmUnitGetStub(unitId);
	for(element = 0; element < ELEMENT_COUNT; element++)
		unit->summonBoost[element] = 0;

	unit->poison = 0;
	unit->attackTurns = 0;
	unit->attackStage = 0;
	unit->defenseTurns = 0;
	unit->defenseStage = 0;
	unit->resistanceTurns = 0;
	unit->resistanceStage = 0;
	unit->delusion = 0;
	unit->confusion = 0;
	unit->charm = 0;
	unit->stun = 0;
	unit->sleep = 0;
	unit->psySeal = 0;
	unit->regenerate = 0;
	unit->reflect = 0;
	unit->haunt = 0;
	unit->grimReaper = 0;
	unit->equipBonus[0] = 0;
	unit->equipBonus[1] = 0;
	unit->ready = 0;
	unit->immobile = 0;
	unit->agilityTurns = 0;
	unit->agilityStage = 0;
	unit->challenge = 0;

	gmUnitCalcStub(unitId);
	btUnitBuildStatFlags(unitId, btObjectGet(unitId));
}

/* Shows the bobbing arrow at the message window and waits for the player to go
 */
/* on. */
UNREAD_RESULT btMessageWaitForAdvance(void) {
	MENU_SPR *prompt;
	MENU_SPR promptStorage;
	s32 frame;
	s32 tileSlot;
	s32 pictureTiles;
	WINDOW *window;
	BATTLE_MESSAGE_OFFSET *offset;

	while(!mnMessageAllTypedStub())
		sySynch(1);
	/* FAKEMATCH(temporary): prompt holds &promptStorage; as promptStorage's
	 * members at its uses. */
	prompt = &promptStorage;
	tileSlot = syVramCacheAllocEntry(128);
	for(frame = 0;; frame++) {
		/* Cycle the eight pictures, one every four frames. */
		pictureTiles = ((FrameCount >> 2) & 7) * 128 +
					   (s32)BattleMessage_AdvanceArrowTiles;
		window = BattleMessageStateWork->win;
		offset = BattleMessageStateWork->ofs;
		/* Outside the windows, sprites show and BG2 does not. */
		syIoWriteQueueClearBits16((u32)&REG_WINOUT, WINOUT_WIN01_BG2);
		syIoWriteQueueSetBits16((u32)&REG_WINOUT, WINOUT_WIN01_OBJ);
		scIoWrite16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);
		/* Both attribute words are set whole, as PLAY_ADVANCE sets them: an 8
		 * by */
		/* 16 arrow of 256 colours, blended with what is behind it. */
		prompt->oam.word.attr01 = OBJ_SIZE_8X16 | OBJ_256_COLORS |
								  OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		prompt->oam.word.attr23 = 0;
		prompt->oam.f.tile = syVramCacheReload(tileSlot, pictureTiles);
		prompt->oam.f.x = window->x * 8 + (offset->x >> 8) + 4;
		prompt->oam.f.y = CURSOR_BOB_Y() + window->y * 8 + (offset->y >> 8) + 6;
		syOamAddSprAtDepth(prompt, OAM_DEPTH_WINDOW);
		/* A held advance key counts only after 16 frames, so one still held
		 * from */
		/* the previous message does not skip this one; B held goes on at once.
		 */
		if((KeysHeld & KEY_B) || (KeysPressed & KEY_ADVANCE) ||
		   (frame > 15 && (KeysHeld & KEY_ADVANCE)))
			break;
		sySynch(1);
	}
	sySndPlayStub(SE_MENU_CURSOR_MOVE);
	syVramCacheFreeEntry(tileSlot);
	sySynch(1);
}

/* Shows the cursor bobbing at (x, y) and waits for the player to go on. */
UNREAD_RESULT btMessageWaitForAdvanceAt(s32 x, s32 y) {
	MENU_POINT position;
	s32 pointerTiles;
	MENU_SPR promptStorage;
	MENU_SPR *prompt;
	s32 tileSlot;

	pointerTiles = mnWindowSprGetPointerTilesStub(0);
	position.x = x;
	position.y = y;
	while(!mnMessageAllTypedStub())
		sySynch(1);
	/* FAKEMATCH(temporary): prompt holds &promptStorage; as promptStorage's
	 * members at its uses. */
	prompt = &promptStorage;
	tileSlot = syVramCacheAllocEntry(128);
	while(1) {
		syIoWriteQueueClearBits16((u32)&REG_WINOUT, WINOUT_WIN01_BG2);
		syIoWriteQueueSetBits16((u32)&REG_WINOUT, WINOUT_WIN01_OBJ);
		scIoWrite16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);
		prompt->oam.word.attr01 = OBJ_SIZE_16X16;
		prompt->oam.word.attr23 = 0;
		prompt->oam.f.tile = syVramCacheReload(tileSlot, pointerTiles);
		/* Nudge it up and right every four frames. */
		prompt->oam.f.x = position.x + ((FrameCount & 4) >> 1) - 4;
		prompt->oam.f.y = position.y - ((FrameCount & 4) >> 2) - 8;
		syOamAddSprAtDepth(prompt, OAM_DEPTH_WINDOW);
		if(KeysPressed & KEY_ADVANCE)
			break;
		sySynch(1);
	}
	syVramCacheFreeEntry(tileSlot);
	sySynch(1);
}

/* Marks the next message, as PLAYBACK_MARK does. */
void btPlaybackMarkMessage(void) { BattleMessageStateWork->marked = 1; }

/* Takes an enemy out of the battle at once, one that leaves rather than falls.
 */
UNREAD_RESULT btUnitRemoveEnemy(s32 unitId) {
	UNIT *enemy;
	BATTLE_OBJECT *slot;

	enemy = gmUnitGetStub(unitId);
	if(enemy->side == BATTLE_SIDE_ENEMY) {
		gmUnitSetHpStub(enemy, 0);
		btObjectRemoveFromLists(unitId);
		btObjectMarkForRelease(unitId);
		slot = btObjectGet(unitId);
		fdActorReleaseStub(slot->actor);
		slot->actor = 0;
		slot->active = 0;
	}
}

/* BATTLE_PLAYBACK_STATE.djinnBonus: a Djinni struck a weakness, so enemies that
 */
/* fall pay the Djinn bonus (btEnemyRecordDefeat). */
#define PLAYBACK_FLAG_DJINN_BONUS 1

/* Grants the Djinn bonus for the rest of the action. */
void btPlaybackGrantDjinnBonus(BATTLE_PLAYBACK_STATE *runtime, u32 unused) {
	runtime->djinnBonus |= PLAYBACK_FLAG_DJINN_BONUS;
}

/* Plays the queued playback at once, in order, where btPlaybackRun plays */
/* it over frames. */
#ifdef UBFIX
void btPlaybackRunAll(void) {
#else
u32 btPlaybackRunAll(void) {
#endif
	BATTLE_PLAYBACK_STATE *runtime = &BattleStateWork->playback;
	BATTLE_PLAYBACK_QUEUE *queue = &runtime->queue;
	s32 eventIndex;

	for(eventIndex = 0; eventIndex < queue->cnt; eventIndex++) {
		u8 opcode = queue->opcodes[eventIndex];
		/* A queue played at once makes no sounds but a hit's. */
		if(opcode <= PLAYBACK_DJINN_BONUS)
			switch(opcode) {
			case PLAYBACK_DJINN_BONUS:
				btPlaybackGrantDjinnBonus(runtime, queue->operands[eventIndex]);
				break;
			case PLAYBACK_REMOVE_UNIT:
				btUnitRemoveEnemy(queue->operands[eventIndex]);
				break;
			case PLAYBACK_UNIT_NAME:
				mnMessageArgumentPushStub(queue->operands[eventIndex],
										  MESSAGE_ARGUMENT_ACTOR);
				break;
			case PLAYBACK_NUMBER:
				mnMessageArgumentPushStub(queue->operands[eventIndex],
										  MESSAGE_ARGUMENT_NUMBER);
				break;
			case PLAYBACK_ITEM_NAME:
				mnMessageArgumentPushStub(queue->operands[eventIndex] &
											  INVENTORY_ITEM_ID_MASK,
										  MESSAGE_ARGUMENT_ITEM);
				break;
			case PLAYBACK_ABILITY_NAME:
				mnMessageArgumentPushStub(queue->operands[eventIndex] &
											  PSYNERGY_ABILITY_MASK,
										  MESSAGE_ARGUMENT_ABILITY);
				break;
			case PLAYBACK_MARK:
				BattleMessageStateWork->marked = 1;
				break;
			case PLAYBACK_CLEAR_ARGUMENTS:
				mnMessageArgumentClearAllStub();
				break;
			/* A message of MESSAGE_NONE only waits for the player. */
			case PLAYBACK_MESSAGE:
				if((s32)queue->operands[eventIndex] >= 0)
					mnBattleMessageShowAndWaitStub(queue->operands[eventIndex]);
				btMessageWaitForAdvance();
				mnMessageArgumentClearAllStub();
				break;
			case PLAYBACK_MESSAGE_NO_WAIT:
				if((s32)queue->operands[eventIndex] >= 0)
					mnBattleMessageShowAndWaitStub(queue->operands[eventIndex]);
				mnMessageArgumentClearAllStub();
				break;
			case PLAYBACK_HIT:
				if(runtime->pendingSnd > 0)
					sySndPlayStub(runtime->pendingSnd);
				btMotionFlashHit(queue->operands[eventIndex], 0, 0);
				break;
			case PLAYBACK_DEFEAT:
				btEnemyRecordDefeat(queue->operands[eventIndex],
									runtime->djinnBonus);
				btUnitClearStatuses(queue->operands[eventIndex]);
				btMotionShowDowned(queue->operands[eventIndex]);
				break;
			case PLAYBACK_DRAW_STATUS:
				mnPartyStatDrawStub(BattleStateWork->partyStat);
				break;
			case PLAYBACK_REFRESH_UNIT:
				btUnitBuildStatFlags(queue->operands[eventIndex],
									 btObjectGet(queue->operands[eventIndex]));
				btObjectUpdatePose(queue->operands[eventIndex]);
				break;
			}
	}
	btPlaybackReset();
}

/* Queues one playback command with its operand, and returns the opcode. */
/* Nothing checks the queue's capacity. */
u32 btPlaybackPush(u32 opcode, u32 operand) {
	BATTLE_PLAYBACK_QUEUE *queue = &BattleStateWork->playback.queue;
	u32 index = queue->cnt;

	queue->opcodes[index] = opcode;
	queue->operands[index] = operand;
	queue->cnt = index + 1;
	return opcode;
}
