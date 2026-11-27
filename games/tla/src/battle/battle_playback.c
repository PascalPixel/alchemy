/* Battle playback: starting it, scheduling its phases, resetting it and waiting
   until it is ready. Part of battle.c. */

#include "window/screens.h"

/* Starts playback of the action's targets when the effect's blow lands, with
   snd as the hit's sound (0 keeps the one set); later calls change nothing. */
void btPlaybackStart(Sint32 snd)
{
	BATTLE_PLAYBACK_STATE *playback;

	playback = &Ram_HeapSlots->battleState->events;
	if(playback->phase == BATTLE_PLAYBACK_PHASE_IDLE) {
		playback->phase = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
		if(snd != 0) {
			if(playback->keepPendingSnd == 0)
				playback->pendingSnd = snd;
		}
	}
}

/* Empties the playback queue and plays the commands queued next, after ticks
   ticks, with no targets to resolve. */
Sint32 btPlaybackSchedulePhase(Sint32 ticks)
{
	BATTLE_STATE		  *battle;
	BATTLE_PLAYBACK_STATE *playback;

	battle = Ram_HeapSlots->battleState;
	playback = &battle->events;
	playback->queue.cnt = 0;
	playback->eventIdx = 0;
	playback->tmr = ticks;
	playback->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
	battle->plan.tgtCnt = 0;
	return sySetEvent(btPlaybackRun, EVENT_PASS_TICK);
}

/* btPlaybackRun's waits, in BATTLE_PLAYBACK_STATE.phase beside the phases
   BATTLE_PLAYBACK_PHASE_ names. */
#define PLAY_MESSAGE	 3	/* waits for the message, then for the player */
#define PLAY_ADVANCE	 5	/* shows the arrow and waits for the player */
#define PLAY_HIT		 10 /* flashes the unit that was hit */
#define PLAY_FELLED		 11 /* greys a fallen enemy and dissolves it */
#define PLAY_MESSAGE_ADD 13 /* waits for the message, then goes straight on */

/* PLAY_FELLED's timer from here counts the Djinn bonus's colour cycle, kept
   apart from the counts of the fall itself (0 to 36). */
#define FELLED_FLASH			1024
/* The Djinn bonus's colour cycle lasts 30 ticks. */
#define FELLED_FLASH_TICKS		30
/* A felled enemy's sprite dissolves over field/field.h's DISSOLVE_STEPS, four a
   tick. */
#define DISSOLVE_STEPS_PER_TICK 4
/* PLAY_FELLED's timer at the tick the grey enemy leaves the lists, and at the
   dissolve's first tick. */
#define FELLED_LEAVES_LISTS		4
#define FELLED_DISSOLVE_START	5

/* The arrow btPlaybackRun shows at a message: The Broken Seal's advance arrow,
   opaque. */
#define PLAYBACK_ARROW_ATTR0 (OBJ_SIZE_8X16 | OBJ_256_COLORS)

/* Plays the action's targets one at a time: resolves each with
   btAbilityApplyToTarget, then plays what it queued over the ticks that
   follow, without waiting. */
void btPlaybackRun(void)
{
	Uint16				   selection[2];
	SPR					  *records[4];
	BATTLE_STATE		  *battle = Ram_HeapSlots->battleState;
	BATTLE_PLAYBACK_STATE *events = &battle->events;

	/* Clears a sprite's tiles in OBJ VRAM, at its VRAM cache entry's offset;
	   the last step of a fallen enemy's dissolve. */
	void btSprClearTiles(SPR * record, Sint32 unused)
	{
		Uint8 *tiles = (Uint8 *)OBJ_VRAM0 + VramCacheEntries[record->slot].ofs;

		syWordsClearEntry(tiles, record->width * record->height);
	}

	if(battle->events.phase == BATTLE_PLAYBACK_PHASE_IDLE)
		return;
	for(;;) {
		if(events->phase == BATTLE_PLAYBACK_PHASE_READY)
			return;
		if(events->phase == BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET) {
			if(events->queue.tgtIdx < battle->plan.tgtCnt) {
				events->queue.cnt = 0;
				events->eventIdx = 0;
				events->tmr = 0;
				btAbilityApplyToTarget(&battle->plan, events->queue.tgtIdx);
				events->queue.tgtIdx++;
				events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
			} else {
				events->phase = BATTLE_PLAYBACK_PHASE_READY;
			}
		} else if(events->phase == BATTLE_PLAYBACK_PHASE_DISPATCH) {
			while(events->eventIdx < events->queue.cnt) {
				{
					Sint32 eventIdx = events->eventIdx;

					if(events->phase != BATTLE_PLAYBACK_PHASE_DISPATCH)
						break;
					if(events->tmr != 0) {
						events->tmr--;
						return;
					}
					switch(events->queue.opcodes[eventIdx]) {
					case PLAYBACK_SOUND:
						sySndPlayStub(events->queue.operands[eventIdx]);
						break;
					case PLAYBACK_DJINN_BONUS:
						btPlaybackGrantDjinnBonus(
							events, events->queue.operands[eventIdx]);
						break;
					case PLAYBACK_UNIT_NAME:
						mnMessageArgumentPushStub(
							events->queue.operands[eventIdx],
							MESSAGE_ARGUMENT_ACTOR);
						break;
					case PLAYBACK_NUMBER:
						mnMessageArgumentPushStub(
							events->queue.operands[eventIdx],
							MESSAGE_ARGUMENT_NUMBER);
						break;
					case PLAYBACK_ITEM_NAME:
						mnMessageArgumentPushStub(
							events->queue.operands[eventIdx] &
								INVENTORY_ITEM_ID_MASK,
							MESSAGE_ARGUMENT_ITEM);
						break;
					case PLAYBACK_ABILITY_NAME:
						mnMessageArgumentPushStub(
							events->queue.operands[eventIdx] &
								PSYNERGY_ABILITY_MASK,
							MESSAGE_ARGUMENT_ABILITY);
						break;
					case PLAYBACK_MARK:
						Ram_HeapSlots->battleMsg->marked = 1;
						break;
					case PLAYBACK_MESSAGE:
						if((Sint32)events->queue.operands[eventIdx] >= 0)
							mnBattleMessageOpenStub(
								events->queue.operands[eventIdx]);
						events->phase = PLAY_MESSAGE;
						PerWork.pendingPush = 0;
						break;
					case PLAYBACK_MESSAGE_NO_WAIT:
						if((Sint32)events->queue.operands[eventIdx] >= 0)
							mnBattleMessageOpenStub(
								events->queue.operands[eventIdx]);
						events->phase = PLAY_MESSAGE_ADD;
						break;
					case PLAYBACK_CLEAR_ARGUMENTS:
						mnMessageArgumentClearAllStub();
						break;
					case PLAYBACK_REMOVE_UNIT:
						btUnitRemoveEnemy(events->queue.operands[eventIdx]);
						break;
					case PLAYBACK_HIT:
						if(events->pendingSnd > 0)
							sySndPlayStub(events->pendingSnd);
						events->unitId = events->queue.operands[eventIdx];
						fdActorSelectAnimationStub(
							btObjectGet(events->unitId)->obj, POSE_DAMAGE);
						events->phase = PLAY_HIT;
						events->tmr = 0;
						break;
					case PLAYBACK_DEFEAT: {
						UNIT  *unit;
						void  *record;
						Sint32 recordIdx;

						events->unitId = events->queue.operands[eventIdx];
						btEnemyRecordDefeat(events->unitId, events->djinnBonus);
						btUnitClearStatuses(events->unitId);
						unit = syUnitGet(events->unitId);
						for(recordIdx = 0;
							(record = btActorGetSpr(
								 btObjectGet(events->unitId)->obj,
								 recordIdx)) != 0;
							recordIdx++) {
							if(unit->side != BATTLE_SIDE_ENEMY)
								fdSprStartAnimationStub(
									record, POSE_PARTY_HELD);
							else
								fdSprStartAnimationStub(
									record, POSE_ENEMY_DOWN);
						}
						if(unit->side == BATTLE_SIDE_ENEMY) {
							events->phase = PLAY_FELLED;
							events->tmr = 0;
						}
						break;
					}
					case PLAYBACK_DRAW_STATUS:
						mnPartyStatDrawStub(
							Ram_HeapSlots->battleState->partyStat);
						break;
					case PLAYBACK_REFRESH_UNIT: {
						BATTLE_OBJECT *slot;

						btUnitBuildStatFlags(events->queue.operands[eventIdx],
							btObjectGet(events->queue.operands[eventIdx]));
						slot = btObjectGet(events->queue.operands[eventIdx]);
						btObjectSetColMode(slot->obj,
							btMotionGetSlotPalette(
								events->queue.operands[eventIdx]));
						btObjectUpdatePose(events->queue.operands[eventIdx]);
						break;
					}
					case PLAYBACK_BLOW_AWAY:
						btObjectRemoveFromLists(
							events->queue.operands[eventIdx]);
						break;
					}
					events->eventIdx++;
				}
			}
			if(events->phase == BATTLE_PLAYBACK_PHASE_DISPATCH)
				events->phase = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
		} else if(events->phase == PLAY_MESSAGE ||
			events->phase == PLAY_MESSAGE_ADD) {
			if(mnMessageAllTypedStub() == 0)
				return;
			if(events->phase == PLAY_MESSAGE_ADD) {
				events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
				events->tmr = 0;
			} else {
				events->phase = PLAY_ADVANCE;
				events->arrowResource = -1;
				events->tmr = FrameCount;
			}
		} else if(events->phase == PLAY_ADVANCE) {
			/* Show the arrow and wait for the player, as
			   btMessageWaitForAdvance does, but with an opaque arrow. */
			MENU_SPR *prompt = &events->arrow;
			Uint8	 *arrowTiles = BattleMessage_AdvanceArrowTiles;
			Sint32	  pictureTiles =
				(Sint32)arrowTiles + ((TickCount >> 2) & 7) * 128;
			BATTLE_MESSAGE_STATE *display = Ram_HeapSlots->battleMsg;
			WINDOW				 *win = display->win;
			MESSAGE				 *msg = display->msg;
			Sint32				  x = 0;

			if(events->arrowResource == -1)
				events->arrowResource = battle->arrowTiles;
			mnMessageArgumentClearAllStub();
			syIOWriteQueueClearBits16((Uint32)&REG_WINOUT, WINOUT_WIN01_BG2);
			syIOWriteQueueSetBits16((Uint32)&REG_WINOUT, WINOUT_WIN01_OBJ);
			prompt->oam.word.attr01 = PLAYBACK_ARROW_ATTR0;
			prompt->oam.word.attr23 = x;
			ASM_NOTHING();
			prompt->oam.f.tile =
				syVRAMCacheReload(events->arrowResource, pictureTiles);
			x = win->x * 8 + (msg->x >> 8) + 4;
			prompt->oam.f.x = x;
			prompt->oam.f.y = CURSOR_BOB_Y() + win->y * 8 + (msg->y >> 8) + 6;
			if((PerWork.data & KEY_B) || (PerWork.pendingPush & KEY_ADVANCE) ||
				((Uint32)(FrameCount - events->tmr) > 10 &&
					(PerWork.data & KEY_ADVANCE))) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
				events->tmr = 0;
			} else {
				syOAMAddSprAtDepth(prompt, OAM_DEPTH_WINDOW);
				return;
			}
		} else if(events->phase == PLAY_HIT) {
			/* A hit: the unit flashes white twice, its party panel lighting
			   with it. */
			if(LastTickFrames)
				events->tmr |= 1;
			if(events->tmr & 1) {
				if(events->tmr & 2) {
					BATTLE_OBJECT *slot;

					selection[0] = BATTLE_UNIT_LIST_END;
					slot = btObjectGet(events->unitId);
					btObjectSetColMode(
						slot->obj, btMotionGetSlotPalette(events->unitId));
				} else {
					selection[0] = events->unitId;
					selection[1] = BATTLE_UNIT_LIST_END;
					btObjectSetColMode(
						btObjectGet(events->unitId)->obj, SPRITE_COLOR_WHITE);
				}
				mnPartyStatHighlightMembersStub(selection);
			}
			events->tmr++;
			if(events->tmr <= 7)
				return;
			events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
			events->tmr = 0;
		} else if(events->phase == PLAY_FELLED) {
			/* A fallen enemy: with the Djinn bonus it first cycles its colours
			   for FELLED_FLASH_TICKS, then turns grey, leaves the lists and
			   dissolves. */
			if(events->tmr == 0 || events->tmr >= FELLED_FLASH) {
				Sint32 colMode = SPRITE_COLOR_GREY;

				if(events->tmr == 0 && events->djinnBonus != 0) {
					Sint32 kind =
						btSprGetDefeatSnd(syUnitGet(events->unitId)->enemyId);

					if(kind >= 0) {
						kind--;
						if(kind < 0)
							kind = 0;
						sySndPlayStub(SE_ENEMY_CRY + kind);
					}
					events->tmr = FELLED_FLASH;
				}
				if(events->tmr > FELLED_FLASH + FELLED_FLASH_TICKS - 1)
					events->tmr = 0;
				if(events->tmr == 0) {
					Sint32 kind =
						btSprGetDefeatSnd(syUnitGet(events->unitId)->enemyId);

					if(kind >= 0)
						sySndPlayStub(SE_ENEMY_CRY + kind);
				}
				if(events->tmr >= FELLED_FLASH)
					colMode =
						(events->tmr - FELLED_FLASH) / 8 % 5 + SPRITE_COLOR_GRB;
				/* Recolour every layer's first entry and have its picture
				   redrawn. */
				if(colMode == SPRITE_COLOR_GREY || (events->tmr & 7) == 0) {
					SPR	  *record;
					Sint32 recordIdx = 0;
					SPR	 **slot = records;
					Sint32 noPicture = SPRITE_PICTURE_NONE;

					for(;
						(record = btActorGetSpr(
							 btObjectGet(events->unitId)->obj, recordIdx)) != 0;
						recordIdx++) {
						SPR_ANIMATION *child;
						Uint8		   prevPicture;

						*slot++ = record;
						child = record->entries[0];
						prevPicture = child->picture;
						child->colMode = colMode;
						/* DEAD: the old picture is read and ORed with
						   SPRITE_PICTURE_NONE, which stores SPRITE_PICTURE_NONE
						   whatever it was; the ROM keeps the load and the
						   OR. */
						child->picture = prevPicture | noPicture;
					}
				}
				events->tmr++;
				return;
			}
			if(events->tmr == FELLED_LEAVES_LISTS) {
				btObjectRemoveFromLists(events->unitId);
				events->tmr++;
				return;
			}
			if(events->tmr > FELLED_LEAVES_LISTS) {
				BATTLE_OBJECT *slot;
				void		  *record;
				Sint32		   cnt;
				Sint32		   step;
				Sint32		   base;

				slot = btObjectGet(events->unitId);
				slot->fading = 1;
				for(cnt = 0; (record = btActorGetSpr(slot->obj, cnt)) != 0;
					cnt++)
					records[cnt] = record;
				base = events->tmr * DISSOLVE_STEPS_PER_TICK;
				step = base - FELLED_DISSOLVE_START * DISSOLVE_STEPS_PER_TICK;
				if(step > DISSOLVE_STEPS - 1) {
					Sint32 recordIdx;

					for(recordIdx = 0; recordIdx < cnt; recordIdx++)
						btSprClearTiles(records[recordIdx], 0);
					btObjectMarkForRelease(events->unitId);
					events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
					events->tmr = 0;
					return;
				}
				{
					Sint32 recordIdx;

					for(recordIdx = 0; recordIdx < cnt; recordIdx++) {
						fdSprDissolveStepStub(records[recordIdx], step);
						fdSprDissolveStepStub(records[recordIdx], step + 1);
						fdSprDissolveStepStub(records[recordIdx], step + 2);
						fdSprDissolveStepStub(records[recordIdx], step + 3);
					}
				}
				events->tmr++;
				return;
			}
			events->tmr++;
			return;
		}
	}
}

/* Stops playback and empties the playback queue. */
Uint32 btPlaybackReset(void)
{
	BATTLE_PLAYBACK_STATE *events;
	BATTLE_STATE		  *battle;

	battle = Ram_HeapSlots->battleState;
	events = &battle->events;
	events->phase = BATTLE_PLAYBACK_PHASE_IDLE;
	events->queue.cnt = 0;
	events->eventIdx = 0;
	events->tmr = 0;
	events->queue.tgtIdx = 0;
	events->pendingSnd = SE_HEAVY_IMPACT;
	events->keepPendingSnd = 0;
	events->djinnBonus = 0;
}

/* Waits until playback has played every target, starting it if the effect never
   showed the damage. */
Sint32 btPlaybackWaitForReady(void)
{
	Sint32				   state;
	BATTLE_PLAYBACK_STATE *events;

	events = &Ram_HeapSlots->battleState->events;
	state = events->phase;
	if(state == BATTLE_PLAYBACK_PHASE_IDLE) {
		events->phase = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
		state = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
	}
	if(state != BATTLE_PLAYBACK_PHASE_READY) {
		do {
			sySynch(1U);
		} while(events->phase != BATTLE_PLAYBACK_PHASE_READY);
	}
	syCloseEvent(btPlaybackRun);
	return btPlaybackReset();
}
