/// @file
/// Battle events played over frames, BattleEvent_Playback.
/// Part of battle.c, the battle object, compiled only through it.

#include "../battle_internal.h"

/// Sets DMA3 three times over, stopping it, then calls the IRQ return
/// instruction's word as a routine.
/// Nothing in this game calls it.
/// XXX: _call_via_r0 is given the value of IwramIrqModeWord, the instruction
/// `orr r3, r3, #0x92` in runtime.s, not its address, so it would branch to
/// 0xe3833092; a caller in another edition, or the same function in The Lost
/// Age, would show what it was meant to run.
void Dma_StopAllThenRunHook(void) {
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
  _call_via_r0(IwramIrqModeWord);
}

/// Starts playback of the plan's targets when the effect's blow lands.
/// Reached as EFFECT_SHOW_DAMAGE(hitSound), through
/// BattleEventRuntime_BeginPhaseFar, from the effects (see the folder README).
/// Only while playback is idle: sets the phase to
/// BATTLE_PLAYBACK_RESOLVE_TARGET, and a nonzero cue to pendingCue.
/// cue : the hit's sound (an SE_ id); zero keeps the one set
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

/// Empties the event queue and schedules playback of the events queued next,
/// with no targets to resolve.
/// Called before a Djinni is set (BattleCommand_BuildPlan) and for the Djinn
/// the round's and a turn's end show (Battle_ProcessRoundEnd,
/// BattleUnit_ProcessTurnEnd), each followed by
/// BattleEventRuntime_WaitForReady. Sets the phase to BATTLE_PLAYBACK_DISPATCH
/// and gBattleWork->plan's targetCount to 0, so playback goes to
/// BATTLE_PLAYBACK_READY once the queue is played; adds BattleEvent_Playback as
/// a scheduler callback and returns the scheduler's result. frames : the delay
/// before the first event
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
#define PLAY_MESSAGE 3      ///< waits for the message, then for the player
#define PLAY_ADVANCE 5      ///< shows the arrow and waits for the player
#define PLAY_HIT 10         ///< flashes the unit that was hit
#define PLAY_FELLED 11      ///< greys a fallen enemy and dissolves it
#define PLAY_MESSAGE_ADD 13 ///< waits for the message, then goes straight on

/// PLAY_FELLED's timer from here counts the Djinn bonus's colour cycle, kept
/// apart from the counts of the fall itself (0 to 36).
#define FELLED_FLASH 0x400
/// The Djinn bonus's colour cycle lasts 30 frames.
#define FELLED_FLASH_FRAMES 30

/// Plays the plan's targets one at a time: resolves the next target with
/// BattleTargetExec, then plays the events it queued over the frames that
/// follow, until every target is done.
/// Runs each frame as a scheduler callback (priority 0xc80), added by the
/// staging runners in staging.c and by BattleEventRuntime_SchedulePhase, and
/// removed by BattleEventRuntime_WaitForReady. Idle until
/// BattleEventRuntime_BeginPhase or the wait starts it. Advances
/// gBattleWork->events (phase, eventIndex, timer, queue.targetIndex, actorId,
/// displaySource, displayEntry, flags); plays the sounds, message arguments,
/// messages, flashes, falls and redraws as BattleEv_DispatchQueued does,
/// without waiting. Each pass of its loop handles one phase; a phase that has
/// to wait for a later frame returns.
void BattleEvent_Playback(void) {
  u16 selection[8];
  struct AnimationObject *records[4];
  struct BattleWork *work = gBattleWork;
  struct BattleEventState *events = &work->events;

  /// Clears a sprite's tiles in OBJ VRAM, at its VRAM cache entry's offset;
  /// the last step of a fallen enemy's dissolve.
  void BattleSprite_ClearTiles(struct AnimationObject * record, s32 unused) {
    u8 *tiles = (u8 *)OBJ_VRAM0 + gVramBlockCache[record->slot].offset;

    Iwram_ClearWords(tiles, record->width * record->height);
  }

  // Idle until the effect's blow lands; ready once every target is played.
  if (work->events.phase == BATTLE_PLAYBACK_IDLE)
    return;
  for (;;) {
    if (events->phase == BATTLE_PLAYBACK_READY)
      return;
    if (events->phase == BATTLE_PLAYBACK_RESOLVE_TARGET) {
      // Resolve the next target, which queues its events, then play them;
      // with none left, playback is ready.
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
      // Play the queued events in order. The timer delays the next one; an
      // event that has to wait (a message, a hit, a fall) changes the phase,
      // which ends the loop, and its phase comes back here when done.
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
          case BATTLE_EVENT_SOUND:
            AudioCommand_PlayFar(events->queue.operands[eventIndex]);
            break;
          case BATTLE_EVENT_DJINN_BONUS:
            BattleEv_GrantDjinnBonus(events,
                                     events->queue.operands[eventIndex]);
            break;
          case BATTLE_EVENT_UNIT_NAME:
            UiWork_PushValueSlotFar(events->queue.operands[eventIndex],
                                    VALUE_UNIT_NAME);
            break;
          case BATTLE_EVENT_NUMBER:
            UiWork_PushValueSlotFar(events->queue.operands[eventIndex],
                                    VALUE_NUMBER);
            break;
          case BATTLE_EVENT_ITEM_NAME:
            UiWork_PushValueSlotFar(events->queue.operands[eventIndex] &
                                        ITEM_ID_MASK,
                                    VALUE_ITEM_NAME);
            break;
          case BATTLE_EVENT_ABILITY_NAME:
            UiWork_PushValueSlotFar(events->queue.operands[eventIndex] &
                                        OWNER_ACTION_ID_MASK,
                                    VALUE_ABILITY_NAME);
            break;
          case BATTLE_EVENT_MARK:
            gBattleDisplayWork->marked = 1;
            break;
          // A negative message shows nothing and only waits for the player.
          // The key latch is cleared so that only a press after the message
          // goes on.
          case BATTLE_EVENT_MESSAGE:
            if ((s32)events->queue.operands[eventIndex] >= 0)
              UiText_PrepareMessageWorkFar(events->queue.operands[eventIndex]);
            events->phase = PLAY_MESSAGE;
            gKeysPressedLatch = 0;
            break;
          case BATTLE_EVENT_MESSAGE_NO_WAIT:
            if ((s32)events->queue.operands[eventIndex] >= 0)
              UiText_PrepareMessageWorkFar(events->queue.operands[eventIndex]);
            events->phase = PLAY_MESSAGE_ADD;
            break;
          case BATTLE_EVENT_CLEAR_ARGUMENTS:
            UiWork_ClearValueNameTablesFar();
            break;
          case BATTLE_EVENT_REMOVE_UNIT:
            BattleActor_DestroyTemporaryObject(
                events->queue.operands[eventIndex]);
            break;
          // The hit's sound, then the damage pose and PLAY_HIT's flashes.
          case BATTLE_EVENT_HIT:
            if (events->pendingCue > 0)
              AudioCommand_PlayFar(events->pendingCue);
            events->actorId = events->queue.operands[eventIndex];
            ObjectDispatch_SelectAnimationFar(
                BattleObjGet(events->actorId)->object, POSE_DAMAGE);
            events->phase = PLAY_HIT;
            events->timer = 0;
            break;
          // The spoils are counted and the unit reset; a party member lies
          // down, an enemy keeps its damage pose and falls in PLAY_FELLED.
          case BATTLE_EVENT_DEFEAT: {
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
                AnimationObjects_SelectAnimationFar(record, POSE_PARTY_HELD);
              else
                AnimationObjects_SelectAnimationFar(record, POSE_ENEMY_DOWN);
            }
            if (unit->side == BATTLE_UNIT_ENEMY) {
              events->phase = PLAY_FELLED;
              events->timer = 0;
            }
            break;
          }
          case BATTLE_EVENT_DRAW_STATUS:
            PartyStatusDrawFar(gBattleWork->partyStatus);
            break;
          case BATTLE_EVENT_REFRESH_UNIT: {
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
      // The whole queue played: on to the next target.
      if (events->phase == BATTLE_PLAYBACK_DISPATCH)
        events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    } else if (events->phase == PLAY_MESSAGE ||
               events->phase == PLAY_MESSAGE_ADD) {
      // Wait until the message finishes appearing. The arrow's resource entry
      // is fetched again (-1) and its wait timed from now.
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
      // Show the arrow, as BattlePresentation_WaitForAdvance does, and wait
      // for the player: B held or an advance key pressed goes on; after ten
      // ticks, a held advance key does too. The arrow is an 8 by 16,
      // 256-colour sprite (attribute 0 0xa000) cycling eight frames of
      // gBattlePres_AdvanceArrowTiles, one every four frames.
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
      // Nine frames, counted by the timer from 0 to 8: at 1 and 5 the unit
      // turns white and its party panel lights; at 3 and 7 it takes back its
      // own colours and the panel goes dark.
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
      // A fallen enemy. Its timer runs: with the Djinn bonus first, from
      // FELLED_FLASH, 30 frames cycling colour modes 1 to 5 every 8 frames,
      // with the defeat sound numbered one before its own; then from 0: grey
      // (mode 6) and its own defeat sound, out of the lists at 4, and from 5
      // the dissolve, four steps a frame, until its tiles are cleared and its
      // object released.
      if (events->timer == 0 || events->timer >= FELLED_FLASH) {
        s32 colorMode = SPRITE_COLOR_GREY;

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
        if (events->timer > FELLED_FLASH + FELLED_FLASH_FRAMES - 1)
          events->timer = 0;
        if (events->timer == 0) {
          s32 kind = BattleClass_GetDefeatSoundKind(
              UnitGetFar(events->actorId)->classId);

          if (kind >= 0)
            AudioCommand_PlayFar(SE_146 + kind);
        }
        if (events->timer >= FELLED_FLASH)
          colorMode = (events->timer - FELLED_FLASH) / 8 % 5 + 1;
        // Recolour every layer's first entry and redraw its frame (255).
        if (colorMode == SPRITE_COLOR_GREY || (events->timer & 7) == 0) {
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
            child->param = colorMode;
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
        // The dissolve: steps 0 to 127 of render_animated_tile_frame, four a
        // frame from timer 5 to 36; at 37 the tiles are cleared, the object
        // released, and the queue goes on.
        struct BattleObj *slot;
        struct AnimationObject *record;
        s32 count;
        s32 step;
        s32 base;

        slot = BattleObjGet(events->actorId);
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
          BattleObj_MarkForRelease(events->actorId);
          events->phase = BATTLE_PLAYBACK_DISPATCH;
          events->timer = 0;
          return;
        }
        {
          s32 recordIndex;

          for (recordIndex = 0; recordIndex < count; recordIndex++) {
            render_animated_tile_frameFar((u8 *)records[recordIndex], step);
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

/// Stops playback and empties the event queue.
/// Called when an action is planned (BattleCommand_BuildPlan), after a queue
/// is played (BattleEv_DispatchQueued, BattleEventRuntime_WaitForReady), and
/// before playback is scheduled (BattleCommand_BuildPlan, turn_end.c). Sets
/// gBattleWork->events' phase to BATTLE_PLAYBACK_IDLE, clears the queue, the
/// event index, the timer, the target index and the flags (the Djinn bonus),
/// and sets the hit sound back to SE_HEAVY_IMPACT. Declared to return u32,
/// but returns nothing.
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

/// Waits, a frame at a time, until BattleEvent_Playback has played every
/// target, then removes the callback and resets the events.
/// Called after the effect by the staging runners in staging.c, and after a
/// Djinn's effect by BattleCommand_BuildPlan and turn_end.c. Playback still
/// idle (the effect never called EFFECT_SHOW_DAMAGE) is started here. Returns
/// BattleEventRuntime_Reset's result.
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

#undef FELLED_FLASH_FRAMES
