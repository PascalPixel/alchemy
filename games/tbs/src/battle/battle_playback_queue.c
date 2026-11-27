// Battle playback: a downed unit's reset, waiting for the player, and the playback queue, filled by
// btPlaybackPush and played at once by btPlaybackRunAll. Part of battle.c.

#include "battle_internal.h"

// Clears every status and stat change of a unit that goes down and works out its stats again.
UNREAD_RESULT btUnitClearStatuses(Sint32 unitId)
{
	UNIT  *unit;
	Sint32 element;

	unit = gmUnitGetStub(unitId);
	for (element = 0; element < ELEMENT_COUNT; element++)
		unit->summonBoost[element] = 0;

	unit->poison          = 0;
	unit->attackTurns     = 0;
	unit->attackStage     = 0;
	unit->defenseTurns    = 0;
	unit->defenseStage    = 0;
	unit->resistanceTurns = 0;
	unit->resistanceStage = 0;
	unit->delusion        = 0;
	unit->confusion       = 0;
	unit->charm           = 0;
	unit->stun            = 0;
	unit->sleep           = 0;
	unit->psySeal         = 0;
	unit->regenerate      = 0;
	unit->reflect         = 0;
	unit->haunt           = 0;
	unit->grimReaper      = 0;
	unit->equipBonus[0]   = 0;
	unit->equipBonus[1]   = 0;
	unit->ready           = 0;
	unit->immobile        = 0;
	unit->agilityTurns    = 0;
	unit->agilityStage    = 0;
	unit->challenge       = 0;

	gmUnitCalculateStub(unitId);
	btUnitBuildStatusFlags(unitId, btObjectGet(unitId));
}

// Shows the bobbing arrow at the message window and waits for the player to go on.
UNREAD_RESULT btMessageWaitForAdvance(void)
{
	MENU_SPR              *prompt;
	MENU_SPR               promptStorage;
	Sint32                 frame;
	Sint32                 tileSlot;
	Sint32                 pictureTiles;
	WINDOW                *window;
	BATTLE_MESSAGE_OFFSET *offset;

	while (!mnMessageAllTypedStub())
		sySynch(1);
	// FAKEMATCH(temporary): prompt holds &promptStorage; as promptStorage's
	// * members at its uses.
	prompt   = &promptStorage;
	tileSlot = syVRAMCacheAllocateEntry(128);
	for (frame = 0;; frame++)
	{
		// Cycle the eight pictures, one every four ticks.
		pictureTiles = ((TickCount >> 2) & 7) * 128 + (Sint32)BattleMessageAdvanceArrowTiles;
		window       = BattleMessageStateWork->window;
		offset       = BattleMessageStateWork->offset;
		// Outside the windows, sprites show and BG2 does not.
		syIOWriteQueueClearBits16((Uint32)&REG_WINOUT, WINOUT_WIN01_BG2);
		syIOWriteQueueSetBits16((Uint32)&REG_WINOUT, WINOUT_WIN01_OBJ);
		IO_WRITE16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);
		// Both attribute words are set whole, as PLAY_ADVANCE sets them: an 8 by 16 arrow of 256
		// colours, blended with what is behind it.
		prompt->oam.word.attr01 =
			OBJ_SIZE_8X16 | OBJ_256_COLORS | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		prompt->oam.word.attr23 = 0;
		prompt->oam.f.tile      = syVRAMCacheReload(tileSlot, pictureTiles);
		prompt->oam.f.x         = window->x * 8 + (offset->x >> 8) + 4;
		prompt->oam.f.y         = CURSOR_BOB_Y() + window->y * 8 + (offset->y >> 8) + 6;
		syOAMAddSpriteAtDepth(prompt, OAM_DEPTH_WINDOW);
		// A held advance key counts only after 16 ticks, so one still held from the previous
		// message does not skip this one; B held goes on at once.
		if ((PerData & KEY_B) || (PerPush & KEY_ADVANCE) || (frame > 15 && (PerData & KEY_ADVANCE)))
			break;
		sySynch(1);
	}
	sySoundPlayStub(SE_MENU_CURSOR_MOVE);
	syVRAMCacheFreeEntry(tileSlot);
	sySynch(1);
}

// Shows the cursor bobbing at (x, y) and waits for the player to go on.
UNREAD_RESULT btMessageWaitForAdvanceAt(Sint32 x, Sint32 y)
{
	MENU_POINT pos;
	Sint32     ptrTiles;
	MENU_SPR   promptStorage;
	MENU_SPR  *prompt;
	Sint32     tileSlot;

	ptrTiles = mnWindowSpriteGetPointerTilesStub(0);
	pos.x    = x;
	pos.y    = y;
	while (!mnMessageAllTypedStub())
		sySynch(1);
	// FAKEMATCH(temporary): prompt holds &promptStorage; as promptStorage's
	// * members at its uses.
	prompt   = &promptStorage;
	tileSlot = syVRAMCacheAllocateEntry(128);
	while (1)
	{
		syIOWriteQueueClearBits16((Uint32)&REG_WINOUT, WINOUT_WIN01_BG2);
		syIOWriteQueueSetBits16((Uint32)&REG_WINOUT, WINOUT_WIN01_OBJ);
		IO_WRITE16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);
		prompt->oam.word.attr01 = OBJ_SIZE_16X16;
		prompt->oam.word.attr23 = 0;
		prompt->oam.f.tile      = syVRAMCacheReload(tileSlot, ptrTiles);
		// Nudge it up and right every four ticks.
		prompt->oam.f.x         = pos.x + ((TickCount & 4) >> 1) - 4;
		prompt->oam.f.y         = pos.y - ((TickCount & 4) >> 2) - 8;
		syOAMAddSpriteAtDepth(prompt, OAM_DEPTH_WINDOW);
		if (PerPush & KEY_ADVANCE)
			break;
		sySynch(1);
	}
	syVRAMCacheFreeEntry(tileSlot);
	sySynch(1);
}

// Marks the next message, as PLAYBACK_MARK does.
void btPlaybackMarkMessage(void)
{
	BattleMessageStateWork->marked = 1;
}

// Takes an enemy out of the battle at once, one that leaves rather than falls.
UNREAD_RESULT btUnitRemoveEnemy(Sint32 unitId)
{
	UNIT          *enemy;
	BATTLE_OBJECT *slot;

	enemy = gmUnitGetStub(unitId);
	if (enemy->side == BATTLE_SIDE_ENEMY)
	{
		gmUnitSetHpStub(enemy, 0);
		btObjectRemoveFromLists(unitId);
		btObjectMarkForRelease(unitId);
		slot = btObjectGet(unitId);
		fdActorReleaseStub(slot->actor);
		slot->actor  = 0;
		slot->active = 0;
	}
}

// BATTLE_PLAYBACK_STATE.djinnBonus: a Djinni struck a weakness, so enemies that fall pay the Djinn
// bonus (btEnemyRecordDefeat).
#define PLAYBACK_FLAG_DJINN_BONUS 1

// Grants the Djinn bonus for the rest of the action.
void btPlaybackGrantDjinnBonus(BATTLE_PLAYBACK_STATE *runtime, Uint32 unused)
{
	runtime->djinnBonus |= PLAYBACK_FLAG_DJINN_BONUS;
}

// Plays the queued playback at once, in order, where btPlaybackRun plays it over ticks.
#if FIX_FUNCTION_SIGNATURES
void btPlaybackRunAll(void)
{
#else
Uint32 btPlaybackRunAll(void)
{
#endif
	BATTLE_PLAYBACK_STATE *runtime = &BattleStateWork->playback;
	BATTLE_PLAYBACK_QUEUE *queue   = &runtime->queue;
	Sint32                 eventIndex;

	for (eventIndex = 0; eventIndex < queue->count; eventIndex++)
	{
		Uint8 opcode = queue->opcodes[eventIndex];
		// A queue played at once makes no sounds but a hit's.
		if (opcode <= PLAYBACK_DJINN_BONUS)
			switch (opcode)
			{
				case PLAYBACK_DJINN_BONUS:
					btPlaybackGrantDjinnBonus(runtime, queue->operands[eventIndex]);
					break;
				case PLAYBACK_REMOVE_UNIT: btUnitRemoveEnemy(queue->operands[eventIndex]); break;
				case PLAYBACK_UNIT_NAME:
					mnMessageArgumentPushStub(queue->operands[eventIndex], MESSAGE_ARGUMENT_ACTOR);
					break;
				case PLAYBACK_NUMBER:
					mnMessageArgumentPushStub(queue->operands[eventIndex], MESSAGE_ARGUMENT_NUMBER);
					break;
				case PLAYBACK_ITEM_NAME:
					mnMessageArgumentPushStub(queue->operands[eventIndex] & INVENTORY_ITEM_ID_MASK,
					                          MESSAGE_ARGUMENT_ITEM);
					break;
				case PLAYBACK_ABILITY_NAME:
					mnMessageArgumentPushStub(queue->operands[eventIndex] & PSYNERGY_ABILITY_MASK,
					                          MESSAGE_ARGUMENT_ABILITY);
					break;
				case PLAYBACK_MARK           : BattleMessageStateWork->marked = 1; break;
				case PLAYBACK_CLEAR_ARGUMENTS: mnMessageArgumentClearAllStub(); break;
				// A message of MESSAGE_NONE only waits for the player.
				case PLAYBACK_MESSAGE:
					if ((Sint32)queue->operands[eventIndex] >= 0)
						mnBattleMessageShowAndWaitStub(queue->operands[eventIndex]);
					btMessageWaitForAdvance();
					mnMessageArgumentClearAllStub();
					break;
				case PLAYBACK_MESSAGE_NO_WAIT:
					if ((Sint32)queue->operands[eventIndex] >= 0)
						mnBattleMessageShowAndWaitStub(queue->operands[eventIndex]);
					mnMessageArgumentClearAllStub();
					break;
				case PLAYBACK_HIT:
					if (runtime->pendingSound > 0)
						sySoundPlayStub(runtime->pendingSound);
					btMotionFlashHit(queue->operands[eventIndex], 0, 0);
					break;
				case PLAYBACK_DEFEAT:
					btEnemyRecordDefeat(queue->operands[eventIndex], runtime->djinnBonus);
					btUnitClearStatuses(queue->operands[eventIndex]);
					btMotionShowDowned(queue->operands[eventIndex]);
					break;
				case PLAYBACK_DRAW_STATUS:
					mnPartyStatusDrawStub(BattleStateWork->partyStatus);
					break;
				case PLAYBACK_REFRESH_UNIT:
					btUnitBuildStatusFlags(queue->operands[eventIndex],
					                       btObjectGet(queue->operands[eventIndex]));
					btObjectUpdatePose(queue->operands[eventIndex]);
					break;
			}
	}
	btPlaybackReset();
}

// Queues one playback command with its operand, and returns the opcode. Nothing checks the queue's
// capacity.
Uint32 btPlaybackPush(Uint32 opcode, Uint32 operand)
{
	BATTLE_PLAYBACK_QUEUE *queue = &BattleStateWork->playback.queue;
	Uint32                 index = queue->count;

	queue->opcodes[index]        = opcode;
	queue->operands[index]       = operand;
	queue->count                 = index + 1;
	return opcode;
}
