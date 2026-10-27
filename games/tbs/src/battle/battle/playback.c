/// @file
/// Playback over frames, BattlePlayback_Run.
// Part of battle.c.

#include "../battle_internal.h"

// _call_via_r0 is given the value of IwramIrqModeWord, the instruction
// `orr r3, r3, #0x92` in runtime.s, not its address, so it would branch to
// 0xe3833092. The Lost Age's twin, between the same neighbours, calls its ARM
// high-multiply helper with 5 and 0 before the three stops and nothing after
// them, and nothing in either game calls the function.
// XXX (no evidence): what the call was meant to run; neither game's copy
// shows it, and no caller exists to say.
/// Sets DMA3 three times over, stopping it, then calls the IRQ return
/// instruction's word as a routine. Nothing in this game calls it.
void Dma_StopAllThenRunHook(void) {
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
  _call_via_r0(IwramIrqModeWord);
}

/// Starts playback of the turn's targets when the animation's blow lands.
/// Reached as ANIM_SHOW_DAMAGE(hitSound), through
/// BattlePlayback_StartFar, from the battle animations (see the folder README).
/// Only while playback is idle: sets the phase to
/// BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET, and a nonzero cue to pendingSong; a
/// later call before the reset does nothing.
/// @param cue the hit's sound (an SE_ id); zero keeps the one set
void BattlePlayback_Start(s32 cue) {
  struct BattlePlaybackWork *events;

  events = &gBattleWork->playback;
  if (events->phase == BATTLE_PLAYBACK_PHASE_IDLE) {
    events->phase = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
    if (cue != 0) {
      events->pendingSong = cue;
    }
  }
}

/// Empties the playback queue and schedules playback of the commands queued
/// next, with no targets to resolve. Called before a Djinni is set
/// (BattleTurn_Build) and for the Djinn the round's and a turn's end show
/// (Battle_ProcessRoundEnd, BattleUnit_ProcessTurnEnd), each followed by
/// BattlePlayback_WaitForReady. Sets the phase to
/// BATTLE_PLAYBACK_PHASE_DISPATCH and gBattleWork->turn's targetCount to 0, so
/// playback goes to BATTLE_PLAYBACK_PHASE_READY once the queue is played; adds
/// BattlePlayback_Run as a task and returns its task index (-1
/// when the task table is full), which no caller reads.
/// @param frames the delay before the first command
s32 BattlePlayback_SchedulePhase(s32 frames) {
  struct BattlePlaybackWork *events;

  events = &gBattleWork->playback;
  events->queue.count = 0;
  events->eventIndex = 0;
  events->timer = frames;
  events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
  gBattleWork->turn.targetCount = 0;
  return Task_Add(BattlePlayback_Run, TASK_PASS_FRAME);
}

// BattlePlayback_Run's waits, in BattlePlaybackWork.phase beside the phases
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

/// Plays the turn's targets one at a time: resolves the next target with
/// BattleAbility_ApplyToTarget, then plays the events it queued over the frames
/// that follow, until every target is done. Runs each frame as a task
/// (priority TASK_PASS_FRAME), added by the staging runners in
/// staging.c and by BattlePlayback_SchedulePhase, and removed by
/// BattlePlayback_WaitForReady. Idle until BattlePlayback_Start or the wait
/// starts it. Advances gBattleWork->events (phase, eventIndex, timer,
/// queue.targetIndex, actorId, arrowResource, arrow, flags); plays the sounds,
/// message arguments, messages, flashes, falls and redraws as
/// BattlePlayback_RunAll does, without waiting. Each pass of its loop handles
/// one phase; a phase that has to wait for a later frame returns.
void BattlePlayback_Run(void) {
  u16 selection[8];
  struct Sprite *records[4];
  struct BattleWork *battle = gBattleWork;
  struct BattlePlaybackWork *events = &battle->playback;

  /// Clears a sprite's tiles in OBJ VRAM, at its VRAM cache entry's offset;
  /// the last step of a fallen enemy's dissolve.
  void BattleSprite_ClearTiles(struct Sprite * record, s32 unused) {
    u8 *tiles = (u8 *)OBJ_VRAM0 + gVramCacheEntries[record->slot].offset;

    Iwram_ClearWords(tiles, record->width * record->height);
  }

  // Idle until the animation's blow lands; ready once every target is played.
  if (battle->playback.phase == BATTLE_PLAYBACK_PHASE_IDLE)
    return;
  for (;;) {
    if (events->phase == BATTLE_PLAYBACK_PHASE_READY)
      return;
    if (events->phase == BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET) {
      // Resolve the next target, which queues its events, then play them;
      // with none left, playback is ready.
      if (events->queue.targetIndex < battle->turn.targetCount) {
        events->queue.count = 0;
        events->eventIndex = 0;
        events->timer = 0;
        BattleAbility_ApplyToTarget(&battle->turn, events->queue.targetIndex);
        events->queue.targetIndex++;
        events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
      } else {
        events->phase = BATTLE_PLAYBACK_PHASE_READY;
      }
    } else if (events->phase == BATTLE_PLAYBACK_PHASE_DISPATCH) {
      // Play the queued events in order. The timer delays the next one; an
      // event that has to wait (a message, a hit, a fall) changes the phase,
      // which ends the loop, and its phase comes back here when done.
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
            Sound_PlaySongFar(events->queue.operands[eventIndex]);
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
            gBattleMessageWork->marked = 1;
            break;
          // A negative message shows nothing and only waits for the player.
          // The key latch is cleared so that only a press after the message
          // goes on.
          case PLAYBACK_MESSAGE:
            if ((s32)events->queue.operands[eventIndex] >= 0)
              BattleMessage_OpenFar(events->queue.operands[eventIndex]);
            events->phase = PLAY_MESSAGE;
            gKeysPressedLatch = 0;
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
          // The hit's sound, then the damage pose and PLAY_HIT's flashes.
          case PLAYBACK_HIT:
            if (events->pendingSong > 0)
              Sound_PlaySongFar(events->pendingSong);
            events->actorId = events->queue.operands[eventIndex];
            Object_SelectAnimationFar(BattleObject_Get(events->actorId)->object,
                                      POSE_DAMAGE);
            events->phase = PLAY_HIT;
            events->timer = 0;
            break;
          // The spoils are counted and the unit reset; a party member lies
          // down, an enemy keeps its damage pose and falls in PLAY_FELLED.
          case PLAYBACK_DEFEAT: {
            struct Unit *unit;
            struct Sprite *record;
            s32 recordIndex;

            events->actorId = events->queue.operands[eventIndex];
            BattleEnemy_RecordDefeat(events->actorId, events->flags);
            BattleObject_ResetRuntimeFields(events->actorId);
            unit = Unit_GetFar(events->actorId);
            for (recordIndex = 0;
                 (record =
                      GetMotionRecord(BattleObject_Get(events->actorId)->object,
                                      recordIndex)) != 0;
                 recordIndex++) {
              if (unit->side != BATTLE_SIDE_ENEMY)
                Sprite_StartAnimFar(record, POSE_PARTY_HELD);
              else
                Sprite_StartAnimFar(record, POSE_ENEMY_DOWN);
            }
            if (unit->side == BATTLE_SIDE_ENEMY) {
              events->phase = PLAY_FELLED;
              events->timer = 0;
            }
            break;
          }
          case PLAYBACK_DRAW_STATUS:
            PartyStatus_DrawFar(gBattleWork->partyStatus);
            break;
          case PLAYBACK_REFRESH_UNIT: {
            struct BattleObject *slot;

            BattleUnit_BuildStatusFlags(
                events->queue.operands[eventIndex],
                BattleObject_Get(events->queue.operands[eventIndex]));
            slot = BattleObject_Get(events->queue.operands[eventIndex]);
            BattleObject_SetColorMode(
                slot->object,
                BattleObject_GetPalette(events->queue.operands[eventIndex]));
            BattleObject_UpdatePose(events->queue.operands[eventIndex]);
            break;
          }
          }
          events->eventIndex++;
        }
      }
      // The whole queue played: on to the next target.
      if (events->phase == BATTLE_PLAYBACK_PHASE_DISPATCH)
        events->phase = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
    } else if (events->phase == PLAY_MESSAGE ||
               events->phase == PLAY_MESSAGE_ADD) {
      // Wait until the message finishes appearing. The arrow's resource entry
      // is fetched again (-1) and its wait timed from now.
      if (TextPrinter_AllFinishedFar() == 0)
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
      // Show the arrow, as BattleMessage_WaitForAdvance does, and wait
      // for the player: B held or an advance key pressed goes on; after ten
      // ticks, a held advance key does too. The arrow is an 8 by 16,
      // 256-colour sprite (attribute 0 0xa000, which unlike that function's
      // is not semi-transparent) cycling eight frames of
      // gBattleMessage_AdvanceArrowTiles, one every four frames, in the
      // VRAM cache entry BattleWork.arrowTiles.
      struct AdvanceArrowSprite *prompt = &events->arrow;
      u8 *arrowTiles = gBattleMessage_AdvanceArrowTiles;
      s32 frameTiles = (s32)arrowTiles + ((gFrameCount >> 2) & 7) * 128;
      struct BattleMessageWork *display = gBattleMessageWork;
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
      prompt->tile = VramCache_Reload(events->arrowResource, frameTiles);
      x = window->x * 8 + (offset->x >> 8) + 4;
      prompt->x = x;
      prompt->y = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 +
                  (offset->y >> 8) + 6;
      if ((BATTLE_OPTIONS & KEY_B) || (gKeysPressedLatch & KEY_ADVANCE) ||
          ((u32)(gVBlankCount - events->timer) > 10 &&
           (BATTLE_OPTIONS & KEY_ADVANCE))) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
        events->timer = 0;
      } else {
        Oam_AddSpriteAtDepth(prompt, 240);
        return;
      }
    } else if (events->phase == PLAY_HIT) {
      // Nine frames, counted by the timer from 0 to 8: at 1 and 5 the unit
      // turns white and its party panel lights; at 3 and 7 it takes back its
      // own colours and the panel goes dark.
      if (events->timer & 1) {
        if (events->timer & 2) {
          struct BattleObject *slot;

          selection[0] = BATTLE_UNIT_LIST_END;
          slot = BattleObject_Get(events->actorId);
          BattleObject_SetColorMode(slot->object,
                                    BattleObject_GetPalette(events->actorId));
        } else {
          selection[0] = events->actorId;
          selection[1] = BATTLE_UNIT_LIST_END;
          BattleObject_SetColorMode(BattleObject_Get(events->actorId)->object,
                                    SPRITE_COLOR_WHITE);
        }
        PartyStatus_HighlightMembersFar(selection);
      }
      events->timer++;
      if (events->timer <= 8)
        return;
      events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
      events->timer = 0;
    } else if (events->phase == PLAY_FELLED) {
      // A fallen enemy. Its timer runs: with the Djinn bonus first, from
      // FELLED_FLASH, 30 frames cycling the colour modes SPRITE_COLOR_GRB to
      // SPRITE_COLOR_BRG every 8 frames,
      // with the defeat sound numbered one before its own; then from 0:
      // SPRITE_COLOR_GREY and its own defeat sound, out of the lists at 4, and
      // from 5 the dissolve, four steps a frame, until its tiles are cleared
      // and its object released.
      if (events->timer == 0 || events->timer >= FELLED_FLASH) {
        s32 colorMode = SPRITE_COLOR_GREY;

        if (events->timer == 0 && events->flags != 0) {
          s32 kind =
              BattleSprite_GetDefeatSound(Unit_GetFar(events->actorId)->id);

          if (kind >= 0) {
            kind--;
            if (kind < 0)
              kind = 0;
            Sound_PlaySongFar(SE_ENEMY_CRY + kind);
          }
          events->timer = FELLED_FLASH;
        }
        if (events->timer > FELLED_FLASH + FELLED_FLASH_FRAMES - 1)
          events->timer = 0;
        if (events->timer == 0) {
          s32 kind =
              BattleSprite_GetDefeatSound(Unit_GetFar(events->actorId)->id);

          if (kind >= 0)
            Sound_PlaySongFar(SE_ENEMY_CRY + kind);
        }
        if (events->timer >= FELLED_FLASH)
          colorMode = (events->timer - FELLED_FLASH) / 8 % 5 + SPRITE_COLOR_GRB;
        // Recolour every layer's first entry and redraw its frame (255).
        if (colorMode == SPRITE_COLOR_GREY || (events->timer & 7) == 0) {
          struct Sprite *record;
          s32 recordIndex;
          struct Sprite **slot;
          s32 lastFrameMask;

          for (recordIndex = 0, slot = records, lastFrameMask = 0xff;
               (record =
                    GetMotionRecord(BattleObject_Get(events->actorId)->object,
                                    recordIndex)) != 0;
               recordIndex++) {
            struct SpriteAnim *child;
            u8 previousFrame;

            *slot++ = record;
            child = record->entries[0];
            previousFrame = child->frame;
            child->colorMode = colorMode;
            child->frame = previousFrame | lastFrameMask;
          }
        }
        events->timer++;
        return;
      }
      if (events->timer == 4) {
        BattleObject_RemoveFromLists(events->actorId);
        events->timer++;
        return;
      }
      if (events->timer > 4) {
        // The dissolve: steps 0 to 127 of render_animated_tile_frame, four a
        // frame from timer 5 to 36; at 37 the tiles are cleared, the object
        // released, and the queue goes on.
        struct BattleObject *slot;
        struct Sprite *record;
        s32 count;
        s32 step;
        s32 base;

        slot = BattleObject_Get(events->actorId);
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
          BattleObject_MarkForRelease(events->actorId);
          events->phase = BATTLE_PLAYBACK_PHASE_DISPATCH;
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

/// Stops playback and empties the playback queue.
/// Called when an action is built into a turn (BattleTurn_Build), after a queue
/// is played (BattlePlayback_RunAll, BattlePlayback_WaitForReady), and
/// before playback is scheduled (BattleTurn_Build, turn_end.c). Sets
/// gBattleWork->events' phase to BATTLE_PLAYBACK_PHASE_IDLE, clears the queue,
/// the event index, the timer, the target index and the flags (the Djinn
/// bonus), and sets the hit sound back to SE_HEAVY_IMPACT.
void BattlePlayback_Reset(void) {
  struct BattlePlaybackWork *events;

  events = &gBattleWork->playback;
  events->phase = BATTLE_PLAYBACK_PHASE_IDLE;
  events->queue.count = 0;
  events->eventIndex = 0;
  events->timer = 0;
  events->queue.targetIndex = 0;
  events->pendingSong = SE_HEAVY_IMPACT;
  events->flags = 0;
}

/// Waits, a frame at a time, until BattlePlayback_Run has played every
/// target, then removes the callback and resets the playback.
/// Called after the animation by the staging runners in staging.c, and after a
/// Djinn's animation by BattleTurn_Build and turn_end.c. Playback still
/// idle (the animation never called ANIM_SHOW_DAMAGE) is started here.
#ifdef UBFIX
void BattlePlayback_WaitForReady(void) {
#else
s32 BattlePlayback_WaitForReady(void) {
#endif
  s32 state;
  struct BattlePlaybackWork *events;

  events = &gBattleWork->playback;
  state = events->phase;
  if (state == BATTLE_PLAYBACK_PHASE_IDLE) {
    events->phase = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
    state = BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET;
  }
  if (state != BATTLE_PLAYBACK_PHASE_READY) {
    do {
      WaitFrames(1U);
    } while (events->phase != BATTLE_PLAYBACK_PHASE_READY);
  }
  Task_Remove(BattlePlayback_Run);
  BattlePlayback_Reset();
}

#undef FELLED_FLASH_FRAMES
