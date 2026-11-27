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
   sound as the hit's sound (0 keeps the one set); later calls change nothing.
 */
void btPlaybackStart(Sint32 sound)
{
	BATTLE_PLAYBACK_STATE *events;

	events = &BattleStateWork->playback;
	if(events->phase == BATTLE_PLAYBACK_PHASE_IDLE) {
		events->phase = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
		if(sound != 0) {
			events->pendingSound = sound;
		}
	}
}

/* Empties the playback queue and plays the commands queued next, after ticks
   ticks, with no targets to resolve. */
Sint32 btPlaybackSchedulePhase(Sint32 ticks)
{
	BATTLE_PLAYBACK_STATE *events;

	events = &BattleStateWork->playback;
	events->queue.count = 0;
	events->eventIndex = 0;
	events->timer = ticks;
	events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
	BattleStateWork->actionRun.targetCount = 0;
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
   leaves the lists; it takes fdSpriteDissolveStep's 128 steps, four a tick. */
#define FELLED_DISSOLVE_FIRST_TICK 5
#define FELLED_DISSOLVE_STEPS	   128

/* Plays the action's targets one at a time: resolves each with
   btAbilityApplyToTarget, then plays what it queued over the ticks that
   follow, without waiting. */
void btPlaybackRun(void)
{
	Uint16				   selection[8];
	SPR					  *sprites[4];
	BATTLE_STATE		  *battle = BattleStateWork;
	BATTLE_PLAYBACK_STATE *events = &battle->playback;

	/* Clears a sprite's tiles in OBJ VRAM, at its VRAM cache entry's offset;
	   the last step of a fallen enemy's dissolve. */
	void btSpriteClearTiles(SPR * sprite, Sint32 unused)
	{
		Uint8 *tiles =
			(Uint8 *)OBJ_VRAM0 + VramCacheEntries[sprite->slot].offset;

		syWordsClear(tiles, sprite->width * sprite->height);
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
			if(events->queue.targetIndex < battle->actionRun.targetCount) {
				events->queue.count = 0;
				events->eventIndex = 0;
				events->timer = 0;
				btAbilityApplyToTarget(
					&battle->actionRun, events->queue.targetIndex);
				events->queue.targetIndex++;
				events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
			} else {
				events->phase = BATTLE_PLAYBACK_PHASE_READY;
			}
		} else if(events->phase == BATTLE_PLAYBACK_PHASE_DISPATCH) {
			/* Play the queued events in order; one that has to wait for later
			   ticks ends the loop. */
			while(events->eventIndex < events->queue.count) {
				{
					Sint32 eventIndex = events->eventIndex;

					if(events->phase != BATTLE_PLAYBACK_PHASE_DISPATCH)
						break;
					if(events->timer != 0) {
						events->timer--;
						return;
					}
					switch(events->queue.opcodes[eventIndex]) {
					case PLAYBACK_SOUND:
						sySoundPlayStub(events->queue.operands[eventIndex]);
						break;
					case PLAYBACK_DJINN_BONUS:
						btPlaybackGrantDjinnBonus(
							events, events->queue.operands[eventIndex]);
						break;
					case PLAYBACK_UNIT_NAME:
						mnMessageArgumentPushStub(
							events->queue.operands[eventIndex],
							MESSAGE_ARGUMENT_ACTOR);
						break;
					case PLAYBACK_NUMBER:
						mnMessageArgumentPushStub(
							events->queue.operands[eventIndex],
							MESSAGE_ARGUMENT_NUMBER);
						break;
					case PLAYBACK_ITEM_NAME:
						mnMessageArgumentPushStub(
							events->queue.operands[eventIndex] &
								INVENTORY_ITEM_ID_MASK,
							MESSAGE_ARGUMENT_ITEM);
						break;
					case PLAYBACK_ABILITY_NAME:
						mnMessageArgumentPushStub(
							events->queue.operands[eventIndex] &
								PSYNERGY_ABILITY_MASK,
							MESSAGE_ARGUMENT_ABILITY);
						break;
					case PLAYBACK_MARK:
						BattleMessageStateWork->marked = 1;
						break;
					/* A message of MESSAGE_NONE only waits for the player, and
					   only a press after the message goes on. */
					case PLAYBACK_MESSAGE:
						if((Sint32)events->queue.operands[eventIndex] >= 0)
							mnBattleMessageOpenStub(
								events->queue.operands[eventIndex]);
						events->phase = PLAY_MESSAGE;
						PerPushLatch = 0;
						break;
					case PLAYBACK_MESSAGE_NO_WAIT:
						if((Sint32)events->queue.operands[eventIndex] >= 0)
							mnBattleMessageOpenStub(
								events->queue.operands[eventIndex]);
						events->phase = PLAY_MESSAGE_ADD;
						break;
					case PLAYBACK_CLEAR_ARGUMENTS:
						mnMessageArgumentClearAllStub();
						break;
					case PLAYBACK_REMOVE_UNIT:
						btUnitRemoveEnemy(events->queue.operands[eventIndex]);
						break;
					/* The hit's sound, then the damage pose and PLAY_HIT's
					   flashes. */
					case PLAYBACK_HIT:
						if(events->pendingSound > 0)
							sySoundPlayStub(events->pendingSound);
						events->unitId = events->queue.operands[eventIndex];
						fdActorSelectAnimationStub(
							btObjectGet(events->unitId)->actor, POSE_DAMAGE);
						events->phase = PLAY_HIT;
						events->timer = 0;
						break;
					/* The spoils are counted and the unit reset; a party member
					   lies down, an enemy keeps its damage pose and falls in
					   PLAY_FELLED. */
					case PLAYBACK_DEFEAT: {
						UNIT  *unit;
						SPR	  *sprite;
						Sint32 spriteIndex;

						events->unitId = events->queue.operands[eventIndex];
						btEnemyRecordDefeat(events->unitId, events->djinnBonus);
						btUnitClearStatuses(events->unitId);
						unit = gmUnitGetStub(events->unitId);
						for(spriteIndex = 0;
							(sprite = btActorGetSprite(
								 btObjectGet(events->unitId)->actor,
								 spriteIndex)) != 0;
							spriteIndex++) {
							if(unit->side != BATTLE_SIDE_ENEMY)
								fdSpriteStartAnimationStub(
									sprite, POSE_PARTY_HELD);
							else
								fdSpriteStartAnimationStub(
									sprite, POSE_ENEMY_DOWN);
						}
						if(unit->side == BATTLE_SIDE_ENEMY) {
							events->phase = PLAY_FELLED;
							events->timer = 0;
						}
						break;
					}
					case PLAYBACK_DRAW_STATUS:
						mnPartyStatusDrawStub(BattleStateWork->partyStatus);
						break;
					case PLAYBACK_REFRESH_UNIT: {
						BATTLE_OBJECT *slot;

						btUnitBuildStatusFlags(
							events->queue.operands[eventIndex],
							btObjectGet(events->queue.operands[eventIndex]));
						slot = btObjectGet(events->queue.operands[eventIndex]);
						btObjectSetColorMode(slot->actor,
							btObjectGetPalette(
								events->queue.operands[eventIndex]));
						btObjectUpdatePose(events->queue.operands[eventIndex]);
						break;
					}
					}
					events->eventIndex++;
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
				events->timer = 0;
			} else {
				events->phase = PLAY_ADVANCE;
				events->arrowResource = -1;
				events->timer = FrameCount;
			}
		} else if(events->phase == PLAY_ADVANCE) {
			/* Show the arrow and wait for the player, as
			   btMessageWaitForAdvance does, but with an opaque arrow. */
			ADVANCE_ARROW_SPR *prompt = &events->arrow;
			Uint8			  *arrowTiles = BattleMessageAdvanceArrowTiles;
			Sint32			   pictureTiles =
				(Sint32)arrowTiles + ((TickCount >> 2) & 7) * 128;
			BATTLE_MESSAGE_STATE  *display = BattleMessageStateWork;
			WINDOW				  *window = display->window;
			BATTLE_MESSAGE_OFFSET *offset = display->offset;
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
			x = window->x * 8 + (offset->x >> 8) + 4;
			prompt->x = x;
			prompt->y = CURSOR_BOB_Y() + window->y * 8 + (offset->y >> 8) + 6;
			if((PerData & KEY_B) || (PerPushLatch & KEY_ADVANCE) ||
				((Uint32)(FrameCount - events->timer) > 10 &&
					(PerData & KEY_ADVANCE))) {
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
				events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
				events->timer = 0;
			} else {
				syOAMAddSpriteAtDepth(prompt, OAM_DEPTH_WINDOW);
				return;
			}
		} else if(events->phase == PLAY_HIT) {
			/* A hit: the unit flashes white twice, its party panel lighting
			   with it. */
			if(events->timer & 1) {
				if(events->timer & 2) {
					BATTLE_OBJECT *slot;

					selection[0] = BATTLE_UNIT_LIST_END;
					slot = btObjectGet(events->unitId);
					btObjectSetColorMode(
						slot->actor, btObjectGetPalette(events->unitId));
				} else {
					selection[0] = events->unitId;
					selection[1] = BATTLE_UNIT_LIST_END;
					btObjectSetColorMode(
						btObjectGet(events->unitId)->actor, SPRITE_COLOR_WHITE);
				}
				mnPartyStatusHighlightMembersStub(selection);
			}
			events->timer++;
			if(events->timer <= HIT_FLASH_TICKS)
				return;
			events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
			events->timer = 0;
		} else if(events->phase == PLAY_FELLED) {
			/* A fallen enemy: with the Djinn bonus it first cycles its colours
			   for 30 ticks, then turns grey, leaves the lists and
			   dissolves. */
			if(events->timer == 0 || events->timer >= FELLED_FLASH) {
				Sint32 colorMode = SPRITE_COLOR_GREY;

				if(events->timer == 0 && events->djinnBonus != 0) {
					Sint32 kind = btSpriteGetDefeatSound(
						gmUnitGetStub(events->unitId)->id);

					if(kind >= 0) {
						kind--;
						if(kind < 0)
							kind = 0;
						sySoundPlayStub(SE_ENEMY_CRY + kind);
					}
					events->timer = FELLED_FLASH;
				}
				if(events->timer > FELLED_FLASH + FELLED_FLASH_TICKS - 1)
					events->timer = 0;
				if(events->timer == 0) {
					Sint32 kind = btSpriteGetDefeatSound(
						gmUnitGetStub(events->unitId)->id);

					if(kind >= 0)
						sySoundPlayStub(SE_ENEMY_CRY + kind);
				}
				if(events->timer >= FELLED_FLASH)
					colorMode = (events->timer - FELLED_FLASH) / 8 % 5 +
						SPRITE_COLOR_GRB;
				/* Recolour every layer's first entry and have its picture drawn
				   again. */
				if(colorMode == SPRITE_COLOR_GREY || (events->timer & 7) == 0) {
					SPR	  *sprite;
					Sint32 spriteIndex;
					SPR	 **slot;
					Sint32 lastPictureMask;

					for(spriteIndex = 0, slot = sprites,
					lastPictureMask = SPRITE_PICTURE_NONE;
						(sprite = btActorGetSprite(
							 btObjectGet(events->unitId)->actor,
							 spriteIndex)) != 0;
						spriteIndex++) {
						SPR_ANIMATION *child;
						Uint8		   prevPicture;

						*slot++ = sprite;
						child = sprite->entries[0];
						prevPicture = child->picture;
						child->colorMode = colorMode;
						child->picture = prevPicture | lastPictureMask;
					}
				}
				events->timer++;
				return;
			}
			if(events->timer == FELLED_DISSOLVE_FIRST_TICK - 1) {
				btObjectRemoveFromLists(events->unitId);
				events->timer++;
				return;
			}
			if(events->timer > FELLED_DISSOLVE_FIRST_TICK - 1) {
				/* The dissolve, four steps a tick. */
				BATTLE_OBJECT *slot;
				SPR			  *sprite;
				Sint32		   count;
				Sint32		   step;
				Sint32		   base;

				slot = btObjectGet(events->unitId);
				slot->fading = 1;
				for(count = 0;
					(sprite = btActorGetSprite(slot->actor, count)) != 0;
					count++)
					sprites[count] = sprite;
				base = events->timer * 4;
				step = base - FELLED_DISSOLVE_FIRST_TICK * 4;
				if(step > FELLED_DISSOLVE_STEPS - 1) {
					Sint32 spriteIndex;

					for(spriteIndex = 0; spriteIndex < count; spriteIndex++)
						btSpriteClearTiles(sprites[spriteIndex], 0);
					btObjectMarkForRelease(events->unitId);
					events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
					events->timer = 0;
					return;
				}
				{
					Sint32 spriteIndex;

					for(spriteIndex = 0; spriteIndex < count; spriteIndex++) {
						fdSpriteDissolveStepStub(
							(Uint8 *)sprites[spriteIndex], step);
						fdSpriteDissolveStepStub(
							(Uint8 *)sprites[spriteIndex], step + 1);
						fdSpriteDissolveStepStub(
							(Uint8 *)sprites[spriteIndex], step + 2);
						fdSpriteDissolveStepStub(
							(Uint8 *)sprites[spriteIndex], step + 3);
					}
				}
				events->timer++;
				return;
			}
			events->timer++;
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
	events->queue.count = 0;
	events->eventIndex = 0;
	events->timer = 0;
	events->queue.targetIndex = 0;
	events->pendingSound = SE_HEAVY_IMPACT;
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
