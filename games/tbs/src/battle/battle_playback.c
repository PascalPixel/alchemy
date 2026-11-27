/* Playback over ticks, btPlaybackRun. Part of battle.c. */

#include "battle_internal.h"

void _call_via_r0(Uint32 routine);

/* Unused: stops DMA3, then calls a function that is not one. */
void btDMAStopDMA3(void)
{
	/* XXX (no evidence): what the call was meant to run; it calls the word of
	   the IRQ return instruction, which would branch into nonsense, and nothing
	   calls this in either game. */
	syDMAXCopy(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
	syDMAXCopy(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
	syDMAXCopy(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
	_call_via_r0(IwramIrqModeWord);
}

/* Starts playback of the action's targets when the effect's blow lands, with
   snd as the hit's sound (0 keeps the one set); later calls change nothing. */
void btPlaybackStart(Sint32 snd)
{
	BATTLE_PLAYBACK_STATE *events;

	events = &BattleStateWork->playback;
	if(events->phase == BATTLE_PLAYBACK_PHASE_IDLE) {
		events->phase = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
		if(snd != 0) {
			events->pendingSnd = snd;
		}
	}
}

/* Empties the playback queue and plays the commands queued next, after ticks
   ticks, with no targets to resolve. */
Sint32 btPlaybackSchedulePhase(Sint32 ticks)
{
	BATTLE_PLAYBACK_STATE *events;

	events = &BattleStateWork->playback;
	events->queue.cnt = 0;
	events->eventIdx = 0;
	events->tmr = ticks;
	events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
	BattleStateWork->actionRun.tgtCnt = 0;
	return sySetEvent(btPlaybackRun, EVENT_PASS_TICK);
}

/* btPlaybackRun's waits, in BATTLE_PLAYBACK_STATE.phase beside the phases
   playback_queue.h names. */
#define PLAY_MESSAGE	 3	/* waits for the message, then for the player */
#define PLAY_ADVANCE	 5	/* shows the arrow and waits for the player */
#define PLAY_HIT		 10 /* flashes the unit that was hit */
#define PLAY_FELLED		 11 /* greys a fallen enemy and dissolves it */
#define PLAY_MESSAGE_ADD 13 /* waits for the message, then goes straight on */

/* PLAY_HIT's ticks: the unit white on ticks 1 and 5, its own colours again on
   3 and 7. */
#define HIT_FLASH_TICKS 8

/* PLAY_FELLED's timer from here counts the Djinn bonus's colour cycle, kept
   apart from the counts of the fall itself (0 to 36). */
#define FELLED_FLASH			   1024
/* The Djinn bonus's colour cycle lasts 30 ticks. */
#define FELLED_FLASH_TICKS		   30
/* PLAY_FELLED's timer at which the dissolve starts, a tick after the enemy
   leaves the lists; it takes fdSprDissolveStep's 128 steps, four a tick. */
#define FELLED_DISSOLVE_FIRST_TICK 5
#define FELLED_DISSOLVE_STEPS	   128

/* Plays the action's targets one at a time: resolves each with
   btAbilityApplyToTarget, then plays what it queued over the ticks that
   follow, without waiting. */
void btPlaybackRun(void)
{
	Uint16				   selection[8];
	SPR					  *sprs[4];
	BATTLE_STATE		  *battle = BattleStateWork;
	BATTLE_PLAYBACK_STATE *events = &battle->playback;

	/* Clears a sprite's tiles in OBJ VRAM, at its VRAM cache entry's offset;
	   the last step of a fallen enemy's dissolve. */
	void btSprClearTiles(SPR * spr, Sint32 unused)
	{
		Uint8 *tiles = (Uint8 *)OBJ_VRAM0 + VramCacheEntries[spr->slot].ofs;

		syWordsClear(tiles, spr->width * spr->height);
	}

	/* Idle until the effect's blow lands; ready once every target is played. */
	if(battle->playback.phase == BATTLE_PLAYBACK_PHASE_IDLE)
		return;
	for(;;) {
		if(events->phase == BATTLE_PLAYBACK_PHASE_READY)
			return;
		if(events->phase == BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET) {
			/* Resolve the next target, which queues its events, then play them;
			   with none left, playback is ready. */
			if(events->queue.tgtIdx < battle->actionRun.tgtCnt) {
				events->queue.cnt = 0;
				events->eventIdx = 0;
				events->tmr = 0;
				btAbilityApplyToTarget(
					&battle->actionRun, events->queue.tgtIdx);
				events->queue.tgtIdx++;
				events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
			} else {
				events->phase = BATTLE_PLAYBACK_PHASE_READY;
			}
		} else if(events->phase == BATTLE_PLAYBACK_PHASE_DISPATCH) {
			/* Play the queued events in order; one that has to wait for later
			   ticks ends the loop. */
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
						BattleMessageStateWork->marked = 1;
						break;
					/* A message of MESSAGE_NONE only waits for the player, and
					   only a press after the message goes on. */
					case PLAYBACK_MESSAGE:
						if((Sint32)events->queue.operands[eventIdx] >= 0)
							mnBattleMessageOpenStub(
								events->queue.operands[eventIdx]);
						events->phase = PLAY_MESSAGE;
						PerPushLatch = 0;
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
					/* The hit's sound, then the damage pose and PLAY_HIT's
					   flashes. */
					case PLAYBACK_HIT:
						if(events->pendingSnd > 0)
							sySndPlayStub(events->pendingSnd);
						events->unitId = events->queue.operands[eventIdx];
						fdActorSelectAnimationStub(
							btObjectGet(events->unitId)->actor, POSE_DAMAGE);
						events->phase = PLAY_HIT;
						events->tmr = 0;
						break;
					/* The spoils are counted and the unit reset; a party member
					   lies down, an enemy keeps its damage pose and falls in
					   PLAY_FELLED. */
					case PLAYBACK_DEFEAT: {
						UNIT  *unit;
						SPR	  *spr;
						Sint32 sprIdx;

						events->unitId = events->queue.operands[eventIdx];
						btEnemyRecordDefeat(events->unitId, events->djinnBonus);
						btUnitClearStatuses(events->unitId);
						unit = gmUnitGetStub(events->unitId);
						for(sprIdx = 0; (spr = btActorGetSpr(
											 btObjectGet(events->unitId)->actor,
											 sprIdx)) != 0;
							sprIdx++) {
							if(unit->side != BATTLE_SIDE_ENEMY)
								fdSprStartAnimationStub(spr, POSE_PARTY_HELD);
							else
								fdSprStartAnimationStub(spr, POSE_ENEMY_DOWN);
						}
						if(unit->side == BATTLE_SIDE_ENEMY) {
							events->phase = PLAY_FELLED;
							events->tmr = 0;
						}
						break;
					}
					case PLAYBACK_DRAW_STATUS:
						mnPartyStatDrawStub(BattleStateWork->partyStat);
						break;
					case PLAYBACK_REFRESH_UNIT: {
						BATTLE_OBJECT *slot;

						btUnitBuildStatFlags(events->queue.operands[eventIdx],
							btObjectGet(events->queue.operands[eventIdx]));
						slot = btObjectGet(events->queue.operands[eventIdx]);
						btObjectSetColMode(slot->actor,
							btObjectGetPalette(
								events->queue.operands[eventIdx]));
						btObjectUpdatePose(events->queue.operands[eventIdx]);
						break;
					}
					}
					events->eventIdx++;
				}
			}
			/* The whole queue played: on to the next target. */
			if(events->phase == BATTLE_PLAYBACK_PHASE_DISPATCH)
				events->phase = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
		} else if(events->phase == PLAY_MESSAGE ||
			events->phase == PLAY_MESSAGE_ADD) {
			/* Wait until the message finishes appearing. The arrow's resource
			   entry is fetched again (-1) and its wait timed from now. */
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
			ADVANCE_ARROW_SPR *prompt = &events->arrow;
			Uint8			  *arrowTiles = BattleMessageAdvanceArrowTiles;
			Sint32			   pictureTiles =
				(Sint32)arrowTiles + ((TickCount >> 2) & 7) * 128;
			BATTLE_MESSAGE_STATE  *display = BattleMessageStateWork;
			WINDOW				  *win = display->win;
			BATTLE_MESSAGE_OFFSET *ofs = display->ofs;
			Sint32				   x = 0;

			if(events->arrowResource == -1)
				events->arrowResource = battle->arrowTiles;
			mnMessageArgumentClearAllStub();
			syIOWriteQueueClearBits16((Uint32)&REG_WINOUT, WINOUT_WIN01_BG2);
			syIOWriteQueueSetBits16((Uint32)&REG_WINOUT, WINOUT_WIN01_OBJ);
			/* OAM attributes 0 and 1 set as one word, an 8 by 16 arrow of 256
			   colours, and attributes 2 and 3 as the next, cleared (x is 0
			   still). */
			((Uint32 *)prompt)[1] = OBJ_SIZE_8X16 | OBJ_256_COLORS;
			((Uint32 *)prompt)[2] = x;
			prompt->tile =
				syVRAMCacheReload(events->arrowResource, pictureTiles);
			x = win->x * 8 + (ofs->x >> 8) + 4;
			prompt->x = x;
			prompt->y = CURSOR_BOB_Y() + win->y * 8 + (ofs->y >> 8) + 6;
			if((PerData & KEY_B) || (PerPushLatch & KEY_ADVANCE) ||
				((Uint32)(FrameCount - events->tmr) > 10 &&
					(PerData & KEY_ADVANCE))) {
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
			if(events->tmr & 1) {
				if(events->tmr & 2) {
					BATTLE_OBJECT *slot;

					selection[0] = BATTLE_UNIT_LIST_END;
					slot = btObjectGet(events->unitId);
					btObjectSetColMode(
						slot->actor, btObjectGetPalette(events->unitId));
				} else {
					selection[0] = events->unitId;
					selection[1] = BATTLE_UNIT_LIST_END;
					btObjectSetColMode(
						btObjectGet(events->unitId)->actor, SPRITE_COLOR_WHITE);
				}
				mnPartyStatHighlightMembersStub(selection);
			}
			events->tmr++;
			if(events->tmr <= HIT_FLASH_TICKS)
				return;
			events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
			events->tmr = 0;
		} else if(events->phase == PLAY_FELLED) {
			/* A fallen enemy: with the Djinn bonus it first cycles its colours
			   for 30 ticks, then turns grey, leaves the lists and
			   dissolves. */
			if(events->tmr == 0 || events->tmr >= FELLED_FLASH) {
				Sint32 colMode = SPRITE_COLOR_GREY;

				if(events->tmr == 0 && events->djinnBonus != 0) {
					Sint32 kind =
						btSprGetDefeatSnd(gmUnitGetStub(events->unitId)->id);

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
						btSprGetDefeatSnd(gmUnitGetStub(events->unitId)->id);

					if(kind >= 0)
						sySndPlayStub(SE_ENEMY_CRY + kind);
				}
				if(events->tmr >= FELLED_FLASH)
					colMode =
						(events->tmr - FELLED_FLASH) / 8 % 5 + SPRITE_COLOR_GRB;
				/* Recolour every layer's first entry and have its picture drawn
				   again. */
				if(colMode == SPRITE_COLOR_GREY || (events->tmr & 7) == 0) {
					SPR	  *spr;
					Sint32 sprIdx;
					SPR	 **slot;
					Sint32 lastPictureMask;

					for(sprIdx = 0, slot = sprs,
					lastPictureMask = SPRITE_PICTURE_NONE;
						(spr = btActorGetSpr(
							 btObjectGet(events->unitId)->actor, sprIdx)) != 0;
						sprIdx++) {
						SPR_ANIMATION *child;
						Uint8		   prevPicture;

						*slot++ = spr;
						child = spr->entries[0];
						prevPicture = child->picture;
						child->colMode = colMode;
						child->picture = prevPicture | lastPictureMask;
					}
				}
				events->tmr++;
				return;
			}
			if(events->tmr == FELLED_DISSOLVE_FIRST_TICK - 1) {
				btObjectRemoveFromLists(events->unitId);
				events->tmr++;
				return;
			}
			if(events->tmr > FELLED_DISSOLVE_FIRST_TICK - 1) {
				/* The dissolve, four steps a tick. */
				BATTLE_OBJECT *slot;
				SPR			  *spr;
				Sint32		   cnt;
				Sint32		   step;
				Sint32		   base;

				slot = btObjectGet(events->unitId);
				slot->fading = 1;
				for(cnt = 0; (spr = btActorGetSpr(slot->actor, cnt)) != 0;
					cnt++)
					sprs[cnt] = spr;
				base = events->tmr * 4;
				step = base - FELLED_DISSOLVE_FIRST_TICK * 4;
				if(step > FELLED_DISSOLVE_STEPS - 1) {
					Sint32 sprIdx;

					for(sprIdx = 0; sprIdx < cnt; sprIdx++)
						btSprClearTiles(sprs[sprIdx], 0);
					btObjectMarkForRelease(events->unitId);
					events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
					events->tmr = 0;
					return;
				}
				{
					Sint32 sprIdx;

					for(sprIdx = 0; sprIdx < cnt; sprIdx++) {
						fdSprDissolveStepStub((Uint8 *)sprs[sprIdx], step);
						fdSprDissolveStepStub((Uint8 *)sprs[sprIdx], step + 1);
						fdSprDissolveStepStub((Uint8 *)sprs[sprIdx], step + 2);
						fdSprDissolveStepStub((Uint8 *)sprs[sprIdx], step + 3);
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
void btPlaybackReset(void)
{
	BATTLE_PLAYBACK_STATE *events;

	events = &BattleStateWork->playback;
	events->phase = BATTLE_PLAYBACK_PHASE_IDLE;
	events->queue.cnt = 0;
	events->eventIdx = 0;
	events->tmr = 0;
	events->queue.tgtIdx = 0;
	events->pendingSnd = SE_HEAVY_IMPACT;
	events->djinnBonus = 0;
}

/* Waits until playback has played every target, starting it if the effect never
   showed the damage. */
UNREAD_RESULT btPlaybackWaitForReady(void)
{
	Sint32				   state;
	BATTLE_PLAYBACK_STATE *events;

	events = &BattleStateWork->playback;
	state = events->phase;
	if(state == BATTLE_PLAYBACK_PHASE_IDLE) {
		events->phase = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
		state = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
	}
	while(events->phase != BATTLE_PLAYBACK_PHASE_READY)
		sySynch(1U);
	syCloseEvent(btPlaybackRun);
	btPlaybackReset();
}

#undef HIT_FLASH_TICKS
#undef FELLED_FLASH_TICKS
#undef FELLED_DISSOLVE_FIRST_TICK
#undef FELLED_DISSOLVE_STEPS
