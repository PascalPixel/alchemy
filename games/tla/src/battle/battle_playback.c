/// @file
/// Battle playback: starting it, scheduling its phases, resetting it and
/// waiting until it is ready.
// Part of battle.c.

#include "battle_internal.h"
#include "sounds.h"
#include "battle/presentation.h"
#include "battle/presentation.h"
#include "gamepak/far_call/field.h"
#include "window/screens.h"

/// Starts playback of the action's targets when the effect's blow lands, with
/// cue as the hit's sound (0 keeps the one set); later calls change nothing.
void BattlePlayback_Start(s32 cue) {
  struct BattlePlaybackWork *ev;

  ev = &Ram_HeapSlots->battleWork->events;
  if (ev->phase == BATTLE_PLAYBACK_PHASE_IDLE) {
    ev->phase = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
    if (cue != 0) {
      if (ev->unk16a == 0)
        ev->pendingCue = cue;
    }
  }
}

/// Empties the playback queue and plays the commands queued next, after frames
/// frames, with no targets to resolve.
s32 BattlePlayback_SchedulePhase(s32 frames) {
  struct BattleWork *work;
  struct BattlePlaybackWork *ev;

  work = Ram_HeapSlots->battleWork;
  ev = &work->events;
  ev->queue.count = 0;
  ev->eventIndex = 0;
  ev->timer = frames;
  ev->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
  work->plan.targetCount = 0;
  return Task_Add(BattlePlayback_Run, TASK_PASS_VBLANK);
}

#define OBJ_VRAM 0x06010000

// BattlePlayback_Run's waits, in BattlePlaybackWork.phase beside the phases
// BATTLE_PLAYBACK_PHASE_ names.
#define PLAY_MESSAGE 3      ///< waits for the message, then for the player
#define PLAY_ADVANCE 5      ///< shows the arrow and waits for the player
#define PLAY_HIT 10         ///< flashes the unit that was hit
#define PLAY_FELLED 11      ///< greys a fallen enemy and dissolves it
#define PLAY_MESSAGE_ADD 13 ///< waits for the message, then goes straight on

/// PLAY_FELLED's timer from here counts the Djinn bonus's colour cycle, kept
/// apart from the counts of the fall itself (0 to 36).
#define FELLED_FLASH 1024
/// The Djinn bonus's colour cycle lasts 30 frames.
#define FELLED_FLASH_FRAMES 30

/// Plays the action's targets one at a time: resolves each with
/// BattleAbility_ApplyToTarget, then plays what it queued over the frames that
/// follow, without waiting.
void BattlePlayback_Run(void) {
  u16 selection[2];
  struct Sprite *records[4];
  struct BattleMessageWork *display;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  struct BattlePlaybackWork *events = &battle->events;

  void BattleSprite_ClearTiles(struct Sprite * record, s32 unused) {
    u8 *tiles = (u8 *)OBJ_VRAM + gVramCacheEntries[record->slot].offset;

    Iwram_ClearWordsEntry(tiles, record->width * record->height);
  }

  if (battle->events.phase == BATTLE_PLAYBACK_PHASE_IDLE)
    return;
  for (;;) {
    if (events->phase == BATTLE_PLAYBACK_PHASE_READY)
      return;
    if (events->phase == BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET) {
      if (events->queue.targetIndex < battle->plan.targetCount) {
        events->queue.count = 0;
        events->eventIndex = 0;
        events->timer = 0;
        BattleAbility_ApplyToTarget(&battle->plan, events->queue.targetIndex);
        events->queue.targetIndex++;
        events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
      } else {
        events->phase = BATTLE_PLAYBACK_PHASE_READY;
      }
    } else if (events->phase == BATTLE_PLAYBACK_PHASE_DISPATCH) {
      while (events->eventIndex < events->queue.count) {
        {
          s32 eventIndex = events->eventIndex;

          if (events->phase != BATTLE_PLAYBACK_PHASE_DISPATCH)
            break;
          if (events->timer != 0) {
            events->timer--;
            return;
          }
          switch (events->queue.opcodes[eventIndex]) {
          case PLAYBACK_SOUND:
            Sound_PlayFar(events->queue.operands[eventIndex]);
            break;
          case PLAYBACK_DJINN_BONUS:
            BattlePlayback_GrantDjinnBonus(events,
                                           events->queue.operands[eventIndex]);
            break;
          case PLAYBACK_UNIT_NAME:
            MessageArgument_PushFar(events->queue.operands[eventIndex],
                                    MESSAGE_ARGUMENT_ACTOR);
            break;
          case PLAYBACK_NUMBER:
            MessageArgument_PushFar(events->queue.operands[eventIndex],
                                    MESSAGE_ARGUMENT_NUMBER);
            break;
          case PLAYBACK_ITEM_NAME:
            MessageArgument_PushFar(events->queue.operands[eventIndex] &
                                        INVENTORY_ITEM_ID_MASK,
                                    MESSAGE_ARGUMENT_ITEM);
            break;
          case PLAYBACK_ABILITY_NAME:
            MessageArgument_PushFar(events->queue.operands[eventIndex] &
                                        PSYNERGY_ABILITY_MASK,
                                    MESSAGE_ARGUMENT_ABILITY);
            break;
          case PLAYBACK_MARK:
            ((struct BattleMessageWork *)Ram_HeapSlots->battleMessage)->marked =
                1;
            break;
          case PLAYBACK_MESSAGE:
            if ((s32)events->queue.operands[eventIndex] >= 0)
              BattleMessage_OpenFar(events->queue.operands[eventIndex]);
            events->phase = PLAY_MESSAGE;
            gInput.pendingPressed = 0;
            break;
          case PLAYBACK_MESSAGE_NO_WAIT:
            if ((s32)events->queue.operands[eventIndex] >= 0)
              BattleMessage_OpenFar(events->queue.operands[eventIndex]);
            events->phase = PLAY_MESSAGE_ADD;
            break;
          case PLAYBACK_CLEAR_ARGUMENTS:
            MessageArgument_ClearAllFar();
            break;
          case PLAYBACK_REMOVE_UNIT:
            BattleObject_DestroyTemporaryObject(
                events->queue.operands[eventIndex]);
            break;
          case PLAYBACK_HIT:
            if (events->pendingCue > 0)
              Sound_PlayFar(events->pendingCue);
            events->unitId = events->queue.operands[eventIndex];
            Actor_SelectAnimationFar(BattleObject_Get(events->unitId)->object,
                                     POSE_DAMAGE);
            events->phase = PLAY_HIT;
            events->timer = 0;
            break;
          case PLAYBACK_DEFEAT: {
            struct Unit *unit;
            void *record;
            s32 recordIndex;

            events->unitId = events->queue.operands[eventIndex];
            BattleEnemy_RecordDefeat(events->unitId, events->flags);
            BattleUnit_ClearStatuses(events->unitId);
            unit = Unit_Get(events->unitId);
            for (recordIndex = 0; (record = GetMotionRecord(
                                       BattleObject_Get(events->unitId)->object,
                                       recordIndex)) != 0;
                 recordIndex++) {
              if (unit->side != BATTLE_UNIT_ENEMY)
                Sprite_StartAnimationFar(record, POSE_PARTY_HELD);
              else
                Sprite_StartAnimationFar(record, POSE_ENEMY_DOWN);
            }
            if (unit->side == BATTLE_UNIT_ENEMY) {
              events->phase = PLAY_FELLED;
              events->timer = 0;
            }
            break;
          }
          case PLAYBACK_DRAW_STATUS:
            PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
            break;
          case PLAYBACK_REFRESH_UNIT: {
            struct BattleObject *slot;

            BattleUnit_BuildStatusFlags(
                events->queue.operands[eventIndex],
                BattleObject_Get(events->queue.operands[eventIndex]));
            slot = BattleObject_Get(events->queue.operands[eventIndex]);
            BattleObject_SetColorMode(slot->object,
                                      BattleMotion_GetSlotPalette(
                                          events->queue.operands[eventIndex]));
            BattleObject_UpdatePose(events->queue.operands[eventIndex]);
            break;
          }
          case PLAYBACK_BLOW_AWAY:
            BattleObject_RemoveFromLists(events->queue.operands[eventIndex]);
            break;
          }
          events->eventIndex++;
        }
      }
      if (events->phase == BATTLE_PLAYBACK_PHASE_DISPATCH)
        events->phase = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
    } else if (events->phase == PLAY_MESSAGE ||
               events->phase == PLAY_MESSAGE_ADD) {
      if (Message_AllTypedFar() == 0)
        return;
      if (events->phase == PLAY_MESSAGE_ADD) {
        events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
        events->timer = 0;
      } else {
        events->phase = PLAY_ADVANCE;
        events->arrowResource = -1;
        events->timer = gVBlankCount;
      }
    } else if (events->phase == PLAY_ADVANCE) {
      struct AdvanceArrowSprite *prompt = &events->arrow;
      u8 *arrowTiles = gBattleMessage_AdvanceArrowTiles;
      s32 pictureTiles = (s32)arrowTiles + ((gFrameCount >> 2) & 7) * 128;
      struct BattleMessageWork *display =
          (struct BattleMessageWork *)Ram_HeapSlots->battleMessage;
      struct Window *window = display->window;
      struct BattleMessageOffset *offset = display->offset;
      s32 x = 0;

      if (events->arrowResource == -1)
        events->arrowResource = battle->arrowTiles;
      MessageArgument_ClearAllFar();
      IoWriteQueue_ClearBits16((u32)&REG_WINOUT, 4);
      IoWriteQueue_SetBits16((u32)&REG_WINOUT, 16);
      ((u32 *)prompt)[1] = 0xa000;
      ((u32 *)prompt)[2] = x;
      /* FAKEMATCH(barrier): an empty asm after x is stored. */
      asm("");
      prompt->tile = VramCache_Reload(events->arrowResource, pictureTiles);
      x = (s16)window->x * 8 + (offset->x >> 8) + 4;
      prompt->x = x;
      prompt->y = Trig_Sin(gFrameCount << 12) / 32768 + (s16)window->y * 8 +
                  (offset->y >> 8) + 6;
      if ((gInput.held & KEY_B) || (gInput.pendingPressed & KEY_ADVANCE) ||
          ((u32)(gVBlankCount - events->timer) > 10 &&
           (gInput.held & KEY_ADVANCE))) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
        events->timer = 0;
      } else {
        Oam_AddSpriteAtDepth(prompt, OAM_DEPTH_WINDOW);
        return;
      }
    } else if (events->phase == PLAY_HIT) {
      if (gLastFrameVBlanks)
        events->timer |= 1;
      if (events->timer & 1) {
        if (events->timer & 2) {
          struct BattleObject *slot;

          selection[0] = BATTLE_UNIT_LIST_END;
          slot = BattleObject_Get(events->unitId);
          BattleObject_SetColorMode(
              slot->object, BattleMotion_GetSlotPalette(events->unitId));
        } else {
          selection[0] = events->unitId;
          selection[1] = BATTLE_UNIT_LIST_END;
          BattleObject_SetColorMode(BattleObject_Get(events->unitId)->object,
                                    SPRITE_COLOR_WHITE);
        }
        PartyStatus_HighlightMembersFar(selection);
      }
      events->timer++;
      if (events->timer <= 7)
        return;
      events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
      events->timer = 0;
    } else if (events->phase == PLAY_FELLED) {
      if (events->timer == 0 || events->timer >= FELLED_FLASH) {
        s32 colorMode = SPRITE_COLOR_GREY;

        if (events->timer == 0 && events->flags != 0) {
          s32 kind =
              BattleSprite_GetDefeatSound(Unit_Get(events->unitId)->enemyId);

          if (kind >= 0) {
            kind--;
            if (kind < 0)
              kind = 0;
            Sound_PlayFar(SE_ENEMY_CRY + kind);
          }
          events->timer = FELLED_FLASH;
        }
        if (events->timer > FELLED_FLASH + FELLED_FLASH_FRAMES - 1)
          events->timer = 0;
        if (events->timer == 0) {
          s32 kind =
              BattleSprite_GetDefeatSound(Unit_Get(events->unitId)->enemyId);

          if (kind >= 0)
            Sound_PlayFar(SE_ENEMY_CRY + kind);
        }
        if (events->timer >= FELLED_FLASH)
          colorMode = (events->timer - FELLED_FLASH) / 8 % 5 + SPRITE_COLOR_GRB;
        if (colorMode == SPRITE_COLOR_GREY || (events->timer & 7) == 0) {
          struct Sprite *record;
          s32 recordIndex;
          struct Sprite **slot;
          s32 lastFrameMask;

          for (recordIndex = 0, slot = (struct Sprite **)records,
              lastFrameMask = 0xff;
               (record =
                    GetMotionRecord(BattleObject_Get(events->unitId)->object,
                                    recordIndex)) != 0;
               recordIndex++) {
            struct SpriteAnimation *child;
            u8 previousFrame;

            *slot++ = record;
            child = record->entries[0];
            previousFrame = child->picture;
            child->colorMode = colorMode;
            child->picture = previousFrame | lastFrameMask;
          }
        }
        events->timer++;
        return;
      }
      if (events->timer == 4) {
        BattleObject_RemoveFromLists(events->unitId);
        events->timer++;
        return;
      }
      if (events->timer > 4) {
        struct BattleObject *slot;
        void *record;
        s32 count;
        s32 step;
        s32 base;

        slot = BattleObject_Get(events->unitId);
        slot->fading = 1;
        for (count = 0; (record = GetMotionRecord(slot->object, count)) != 0;
             count++)
          records[count] = record;
        base = events->timer * 4;
        step = base - 20;
        if (step > 127) {
          s32 recordIndex;

          for (recordIndex = 0; recordIndex < count; recordIndex++)
            BattleSprite_ClearTiles(records[recordIndex], 0);
          BattleObject_MarkForRelease(events->unitId);
          events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
          events->timer = 0;
          return;
        }
        {
          s32 recordIndex;

          for (recordIndex = 0; recordIndex < count; recordIndex++) {
            SpriteDissolve_StepFar(records[recordIndex], step);
            SpriteDissolve_StepFar(records[recordIndex], base - 19);
            SpriteDissolve_StepFar(records[recordIndex], base - 18);
            SpriteDissolve_StepFar(records[recordIndex], base - 17);
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

/// Stops playback and empties the playback queue.
u32 BattlePlayback_Reset(void) {
  struct BattlePlaybackWork *events;
  struct BattleWork *work;

  work = Ram_HeapSlots->battleWork;
  events = &work->events;
  events->phase = BATTLE_PLAYBACK_PHASE_IDLE;
  events->queue.count = 0;
  events->eventIndex = 0;
  events->timer = 0;
  events->queue.targetIndex = 0;
  events->pendingCue = 0x86;
  events->unk16a = 0;
  events->flags = 0;
}

/// Waits until playback has played every target, starting it if the effect
/// never showed the damage.
s32 BattlePlayback_WaitForReady(void) {
  s32 state;
  struct BattlePlaybackWork *events;

  events = &Ram_HeapSlots->battleWork->events;
  state = events->phase;
  if (state == BATTLE_PLAYBACK_PHASE_IDLE) {
    events->phase = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
    state = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
  }
  if (state != BATTLE_PLAYBACK_PHASE_READY) {
    do {
      wait(1U);
    } while (events->phase != BATTLE_PLAYBACK_PHASE_READY);
  }
  Task_Remove((u32)BattlePlayback_Run);
  return BattlePlayback_Reset();
}
