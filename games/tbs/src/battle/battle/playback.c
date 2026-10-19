/// @file
/// Part of battle.c, the battle object, compiled only through it.
/// Battle events played over frames, BattleEvent_Playback.

#include "../battle_internal.h"

/// Sets DMA3 three times over, stopping it, then calls the IRQ return
/// instruction's word as a routine.
void Dma_StopAllThenRunHook(void) {
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
  _call_via_r0(IwramIrqModeWord);
}

/// Resumes paused playback from target processing.
/// cue: hit sound effect; zero leaves it unchanged.
void BattleEventRuntime_BeginPhase(s32 cue) {
  struct BattleEventState *events;

  events = &gBattleWork->events;
  if (events->phase == BATTLE_PLAYBACK_IDLE) {
    events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    if (cue != 0) {
      events->pendingCue = cue;
    }
  }
}

/// Clears the event sequence and schedules playback.
/// frames: delay before the first event.
s32 BattleEventRuntime_SchedulePhase(s32 frames) {
  struct BattleEventState *events;

  events = &gBattleWork->events;
  events->queue.count = 0;
  events->eventIndex = 0;
  events->timer = frames;
  events->phase = BATTLE_PLAYBACK_DISPATCH;
  gBattleWork->plan.targetCount = 0;
  return Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
}

// BattleEvent_Playback's waits, in BattleEventState.phase beside the phases
// battle_event.h names.
#define PLAY_MESSAGE 3      ///< Wait for message display.
#define PLAY_ADVANCE 5      ///< Wait for the advance key.
#define PLAY_HIT 10         ///< Flash the unit that was hit.
#define PLAY_FELLED 11      ///< Remove a defeated enemy.
#define PLAY_MESSAGE_ADD 13 ///< Continue the message without waiting for a key.

#define FELLED_FLASH 0x400 ///< Timer base while changing colour.

/// Scheduler callback that runs BattleTargetExec's queued events in order.
void BattleEvent_Playback(void) {
  u16 selection[8];
  struct AnimationObject *records[4];
  struct BattleWork *work = gBattleWork;
  struct BattleEventState *events = &work->events;

  // Clear the character's tiles.
  void BattleSprite_ClearTiles(struct AnimationObject * record, s32 unused) {
    u8 *tiles = (u8 *)OBJ_VRAM0 + gVramBlockCache[record->slot].offset;

    Iwram_ClearWords(tiles, record->width * record->height);
  }

  if (work->events.phase == BATTLE_PLAYBACK_IDLE)
    return;
  for (;;) {
    if (events->phase == BATTLE_PLAYBACK_READY)
      return;
    if (events->phase == BATTLE_PLAYBACK_RESOLVE_TARGET) {
      // Calculate effects for the next target and enqueue its events.
      if (events->queue.targetIndex < work->plan.targetCount) {
        events->queue.count = 0;
        events->eventIndex = 0;
        events->timer = 0;
        BattleTargetExec(&work->plan, events->queue.targetIndex);
        events->queue.targetIndex++;
        events->phase = BATTLE_PLAYBACK_DISPATCH;
      } else {
        events->phase = BATTLE_PLAYBACK_READY;
      }
    } else if (events->phase == BATTLE_PLAYBACK_DISPATCH) {
      // Run the queued events in order.
      while (events->eventIndex < events->queue.count) {
        {
          s32 eventIndex = events->eventIndex;

          if (events->phase != BATTLE_PLAYBACK_DISPATCH)
            break;
          if (events->timer != 0) {
            events->timer--;
            return;
          }
          switch (events->queue.opcodes[eventIndex]) {
          case BATTLE_EVENT_SOUND: // Sound effect.
            AudioCommand_PlayFar(events->queue.operands[eventIndex]);
            break;
          case BATTLE_EVENT_DJINN_BONUS:
            BattleEv_GrantDjinnBonus(events,
                                     events->queue.operands[eventIndex]);
            break;
          case BATTLE_EVENT_UNIT_NAME: // Name inserted into the message.
            UiWork_PushValueSlotFar(events->queue.operands[eventIndex],
                                    VALUE_UNIT_NAME);
            break;
          case BATTLE_EVENT_NUMBER: // Number inserted into the message.
            UiWork_PushValueSlotFar(events->queue.operands[eventIndex],
                                    VALUE_NUMBER);
            break;
          case BATTLE_EVENT_ITEM_NAME: // Item name inserted into the message.
            UiWork_PushValueSlotFar(events->queue.operands[eventIndex] &
                                        ITEM_ID_MASK,
                                    VALUE_ITEM_NAME);
            break;
          case BATTLE_EVENT_ABILITY_NAME: // Ability name inserted into the
                                          // message.
            UiWork_PushValueSlotFar(events->queue.operands[eventIndex] &
                                        OWNER_ACTION_ID_MASK,
                                    VALUE_ABILITY_NAME);
            break;
          case BATTLE_EVENT_MARK:
            gBattleDisplayWork->marked = 1;
            break;
          case BATTLE_EVENT_MESSAGE: // Show the message and wait for advance.
            if ((s32)events->queue.operands[eventIndex] >= 0)
              UiText_PrepareMessageWorkFar(events->queue.operands[eventIndex]);
            events->phase = PLAY_MESSAGE;
            gKeysPressedLatch = 0;
            break;
          case BATTLE_EVENT_MESSAGE_NO_WAIT: // Show without waiting for
                                             // advance.
            if ((s32)events->queue.operands[eventIndex] >= 0)
              UiText_PrepareMessageWorkFar(events->queue.operands[eventIndex]);
            events->phase = PLAY_MESSAGE_ADD;
            break;
          case BATTLE_EVENT_CLEAR_ARGUMENTS: // Clear message arguments.
            UiWork_ClearValueNameTablesFar();
            break;
          case BATTLE_EVENT_REMOVE_UNIT:
            BattleActor_DestroyTemporaryObject(
                events->queue.operands[eventIndex]);
            break;
          case BATTLE_EVENT_HIT: // Hit.
            if (events->pendingCue > 0)
              AudioCommand_PlayFar(events->pendingCue);
            events->actorId = events->queue.operands[eventIndex];
            ObjectDispatch_SelectAnimationFar(
                BattleObjGet(events->actorId)->object, POSE_DAMAGE);
            events->phase = PLAY_HIT;
            events->timer = 0;
            break;
          case BATTLE_EVENT_DEFEAT: // Defeated.
          {
            struct BattleUnit *unit;
            struct AnimationObject *record;
            s32 recordIndex;

            events->actorId = events->queue.operands[eventIndex];
            BattleEnemy_RecordDefeat(events->actorId, events->flags);
            BattleActor_ResetRuntimeFields(events->actorId);
            unit = UnitGetFar(events->actorId);
            for (recordIndex = 0;
                 (record = GetMotionRecord(
                      BattleObjGet(events->actorId)->object, recordIndex)) != 0;
                 recordIndex++) {
              if (unit->side != BATTLE_UNIT_ENEMY)
                AnimationObjects_SelectAnimationFar(record, 4);
              else
                AnimationObjects_SelectAnimationFar(record, 5);
            }
            if (unit->side == BATTLE_UNIT_ENEMY) {
              events->phase = PLAY_FELLED;
              events->timer = 0;
            }
            break;
          }
          case BATTLE_EVENT_DRAW_STATUS: // Redraw party status.
            PartyStatusDrawFar(gBattleWork->partyStatus);
            break;
          case BATTLE_EVENT_REFRESH_UNIT: // Update status icons, colour and
                                          // pose.
          {
            struct BattleObj *slot;

            BattleUnit_BuildStatusFlags(
                events->queue.operands[eventIndex],
                BattleObjGet(events->queue.operands[eventIndex]));
            slot = BattleObjGet(events->queue.operands[eventIndex]);
            BattleMotion_SetRecordChildValues(
                slot->object, BattleMotion_GetSlotPalette(
                                  events->queue.operands[eventIndex]));
            BattlePres_SetActorModeAndAction(
                events->queue.operands[eventIndex]);
            break;
          }
          }
          events->eventIndex++;
        }
      }
      if (events->phase == BATTLE_PLAYBACK_DISPATCH)
        events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    } else if (events->phase == PLAY_MESSAGE ||
               events->phase == PLAY_MESSAGE_ADD) {
      // Wait until the message finishes appearing.
      if (UiWork_IsCompleteFar() == 0)
        return;
      if (events->phase == PLAY_MESSAGE_ADD) {
        events->phase = BATTLE_PLAYBACK_DISPATCH;
        events->timer = 0;
      } else {
        events->phase = PLAY_ADVANCE;
        events->displaySource = -1;
        events->timer = gFrameTick;
      }
    } else if (events->phase == PLAY_ADVANCE) {
      // Show the arrow and wait for advance. Holding B or pressing an advance
      // key proceeds; after ten ticks, holding an advance key also proceeds.
      struct BattlePromptSprite *prompt = &events->displayEntry;
      u8 *arrowTiles = gBattlePres_AdvanceArrowTiles;
      s32 frameTiles = (s32)arrowTiles + ((gFrameCount >> 2) & 7) * 128;
      struct BattleDisplayWork *display = gBattleDisplayWork;
      struct UiWindow *window = display->window;
      struct BattleDisplayOffset *offset = display->offset;
      s32 x = 0;

      if (events->displaySource == -1)
        events->displaySource = work->promptSlot;
      UiWork_ClearValueNameTablesFar();
      QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
      QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
      ((u32 *)prompt)[1] = 0xa000;
      ((u32 *)prompt)[2] = x;
      prompt->tile = VramCache_Reload(events->displaySource, frameTiles);
      x = window->x * 8 + (offset->x >> 8) + 4;
      prompt->x = x;
      prompt->y = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 +
                  (offset->y >> 8) + 6;
      if ((BATTLE_OPTIONS & KEY_B) || (gKeysPressedLatch & KEY_ADVANCE) ||
          ((u32)(gFrameTick - events->timer) > 10 &&
           (BATTLE_OPTIONS & KEY_ADVANCE))) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        events->phase = BATTLE_PLAYBACK_DISPATCH;
        events->timer = 0;
      } else {
        Runtime_PushSlotEntry(prompt, 240);
        return;
      }
    } else if (events->phase == PLAY_HIT) {
      // Change colour every two frames and restore it after nine frames.
      if (events->timer & 1) {
        if (events->timer & 2) {
          struct BattleObj *slot;

          selection[0] = BATTLE_UNIT_LIST_END;
          slot = BattleObjGet(events->actorId);
          BattleMotion_SetRecordChildValues(
              slot->object, BattleMotion_GetSlotPalette(events->actorId));
        } else {
          selection[0] = events->actorId;
          selection[1] = BATTLE_UNIT_LIST_END;
          BattleMotion_SetRecordChildValues(
              BattleObjGet(events->actorId)->object, SPRITE_COLOR_WHITE);
        }
        BattleLayout_HighlightPartyPanelsFar(selection);
      }
      events->timer++;
      if (events->timer <= 8)
        return;
      events->phase = BATTLE_PLAYBACK_DISPATCH;
      events->timer = 0;
    } else if (events->phase == PLAY_FELLED) {
      // With flags set, first change colour for 30 frames.
      if (events->timer == 0 || events->timer >= FELLED_FLASH) {
        s32 frame = 6;

        if (events->timer == 0 && events->flags != 0) {
          s32 kind = BattleClass_GetDefeatSoundKind(
              UnitGetFar(events->actorId)->classId);

          if (kind >= 0) {
            kind--;
            if (kind < 0)
              kind = 0;
            AudioCommand_PlayFar(SE_146 + kind);
          }
          events->timer = FELLED_FLASH;
        }
        if (events->timer > FELLED_FLASH + 29)
          events->timer = 0;
        if (events->timer == 0) {
          s32 kind = BattleClass_GetDefeatSoundKind(
              UnitGetFar(events->actorId)->classId);

          if (kind >= 0)
            AudioCommand_PlayFar(SE_146 + kind);
        }
        if (events->timer >= FELLED_FLASH)
          frame = (events->timer - FELLED_FLASH) / 8 % 5 + 1;
        if (frame == 6 || (events->timer & 7) == 0) {
          struct AnimationObject *record;
          s32 recordIndex;
          struct AnimationObject **slot;
          s32 lastFrameMask;

          for (recordIndex = 0, slot = records, lastFrameMask = 0xff;
               (record = GetMotionRecord(BattleObjGet(events->actorId)->object,
                                         recordIndex)) != 0;
               recordIndex++) {
            struct AnimationEntry *child;
            u8 previousFrame;

            *slot++ = record;
            child = record->entries[0];
            previousFrame = child->frame;
            child->param = frame;
            child->frame = previousFrame | lastFrameMask;
          }
        }
        events->timer++;
        return;
      }
      if (events->timer == 4) {
        BattleActor_RemoveFromLists(events->actorId);
        events->timer++;
        return;
      }
      if (events->timer > 4) {
        // Remove the image by replacing its tiles in sequence.
        struct BattleObj *slot;
        struct AnimationObject *record;
        s32 count;
        s32 frame;
        s32 base;

        slot = BattleObjGet(events->actorId);
        slot->fading = 1;
        for (count = 0; (record = GetMotionRecord(slot->object, count)) != 0;
             count++)
          records[count] = record;
        base = events->timer * 4;
        frame = base - 20;
        if (frame > 127) {
          s32 recordIndex;

          for (recordIndex = 0; recordIndex < count; recordIndex++)
            BattleSprite_ClearTiles(records[recordIndex], 0);
          BattleObj_MarkForRelease(events->actorId);
          events->phase = BATTLE_PLAYBACK_DISPATCH;
          events->timer = 0;
          return;
        }
        {
          s32 recordIndex;

          for (recordIndex = 0; recordIndex < count; recordIndex++) {
            render_animated_tile_frameFar((u8 *)records[recordIndex], frame);
            render_animated_tile_frameFar((u8 *)records[recordIndex],
                                          base - 19);
            render_animated_tile_frameFar((u8 *)records[recordIndex],
                                          base - 18);
            render_animated_tile_frameFar((u8 *)records[recordIndex],
                                          base - 17);
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

/// Clears playback, queued events, target progress and flags; restores the
/// default hit cue.
u32 BattleEventRuntime_Reset(void) {
  struct BattleEventState *events;

  events = &gBattleWork->events;
  events->phase = BATTLE_PLAYBACK_IDLE;
  events->queue.count = 0;
  events->eventIndex = 0;
  events->timer = 0;
  events->queue.targetIndex = 0;
  events->pendingCue = SE_HEAVY_IMPACT;
  events->flags = 0;
}

/// Starts idle playback, waits for all targets and events, then removes the
/// callback and resets it.
s32 BattleEventRuntime_WaitForReady(void) {
  s32 state;
  struct BattleEventState *events;

  events = &gBattleWork->events;
  state = events->phase;
  if (state == BATTLE_PLAYBACK_IDLE) {
    events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    state = BATTLE_PLAYBACK_RESOLVE_TARGET;
  }
  if (state != BATTLE_PLAYBACK_READY) {
    do {
      WaitFrames(1U);
    } while (events->phase != BATTLE_PLAYBACK_READY);
  }
  Scheduler_RemoveCallback(BattleEvent_Playback);
  return BattleEventRuntime_Reset();
}
