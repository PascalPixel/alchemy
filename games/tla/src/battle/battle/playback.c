/// @file
/// Battle playback: starting it, scheduling its phases, resetting it and
/// waiting until it is ready.
// Part of battle.c.

#include "../battle_internal.h"
#include "sounds.h"
#include "module/battle/presentation.h"
#include "module/battle/presentation.h"
#include "gamepak/far_call/field.h"
#include "module/window/screens.h"

void BattlePlayback_Start(s32 cue) {
  struct BattleEventState *ev;

  ev = &Ram_HeapSlots->battleWork->events;
  if (ev->phase == BATTLE_PLAYBACK_IDLE) {
    ev->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    if (cue != 0) {
      if (ev->unk16a == 0)
        ev->pendingCue = cue;
    }
  }
}

s32 BattlePlayback_SchedulePhase(s32 frames) {
  struct BattleWork *work;
  struct BattleEventState *ev;

  work = Ram_HeapSlots->battleWork;
  ev = &work->events;
  ev->queue.count = 0;
  ev->eventIndex = 0;
  ev->timer = frames;
  ev->phase = BATTLE_PLAYBACK_DISPATCH;
  work->unk670[0x0d] = 0;
  return Task_Add(BattlePlayback_Run, TASK_PASS_VBLANK);
}

#define OBJ_VRAM 0x06010000
#define KEYS_HELD (*(volatile u32 *)&gInput.held)

/* VIEW(f11e3ac): tag struct ArrowSprite as struct ArrowSprite__presentation;
 * module/tla.h has another definition; not tried: struct ArrowSprite__f11e3ac
 * was not found. */
struct ArrowSprite__presentation {
  void *next;
  u16 y : 8;
  u16 flags : 8;
  u16 x : 9;
  u16 size : 7;
  u16 tile : 10;
  u16 palette : 6;
  u16 affineParam;
};

struct PlaybackEvents {
  struct BattleEventQueue queue;
  s32 phase;
  s32 eventIndex;
  s32 timer;
  struct ArrowSprite__presentation arrow;
  s32 arrowResource;
  s32 actorId;
  s16 pendingCue;
  s16 unk16a;
  s32 flags;
};

struct PlaybackBattleWork {
  u8 unk000[0x54];
  s32 arrowTiles;
  u8 unk058[0x67c - 0x58];
  struct BattlePlan plan;
  u8 unk6e0[4];
  struct PlaybackEvents events;
};

void BattlePlayback_Run(void) {
  u16 selection[2];
  struct AnimationRecord *records[4];
  struct BattleMessageDisplay *display;
  struct PlaybackBattleWork *battle =
      (struct PlaybackBattleWork *)Ram_HeapSlots->battleWork;
  struct PlaybackEvents *events = &battle->events;

  void BattleSprite_ClearTiles(struct AnimationRecord * record, s32 unused) {
    u8 *tiles = (u8 *)OBJ_VRAM + gResourceTableEntries[record->slot].offset;

    Iwram_ClearWordsEntry(tiles, record->width * record->height);
  }

  if (battle->events.phase == BATTLE_PLAYBACK_IDLE)
    return;
  for (;;) {
    if (events->phase == BATTLE_PLAYBACK_READY)
      return;
    if (events->phase == BATTLE_PLAYBACK_RESOLVE_TARGET) {
      if (events->queue.targetIndex < battle->plan.targetCount) {
        events->queue.count = 0;
        events->eventIndex = 0;
        events->timer = 0;
        BattleTarget_Execute(&battle->plan, events->queue.targetIndex);
        events->queue.targetIndex++;
        events->phase = BATTLE_PLAYBACK_DISPATCH;
      } else {
        events->phase = BATTLE_PLAYBACK_READY;
      }
    } else if (events->phase == BATTLE_PLAYBACK_DISPATCH) {
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
            Sound_PlayFar(events->queue.operands[eventIndex]);
            break;
          case BATTLE_EVENT_DJINN_BONUS:
            BattlePlayback_GrantDjinnBonus(events,
                                           events->queue.operands[eventIndex]);
            break;
          case BATTLE_EVENT_UNIT_NAME:
            MessageArgument_PushFar(events->queue.operands[eventIndex], 1);
            break;
          case BATTLE_EVENT_NUMBER:
            MessageArgument_PushFar(events->queue.operands[eventIndex], 5);
            break;
          case BATTLE_EVENT_ITEM_NAME:
            MessageArgument_PushFar(events->queue.operands[eventIndex] & 0x1ff,
                                    2);
            break;
          case BATTLE_EVENT_ABILITY_NAME:
            MessageArgument_PushFar(events->queue.operands[eventIndex] & 0x3fff,
                                    4);
            break;
          case BATTLE_EVENT_MARK:
            ((struct BattleMessageDisplay *)Ram_HeapSlots->unk84[4])->marked =
                1;
            break;
          case BATTLE_EVENT_MESSAGE:
            if ((s32)events->queue.operands[eventIndex] >= 0)
              BattleMessage_OpenFar(events->queue.operands[eventIndex]);
            events->phase = 3;
            gInput.pendingPressed = 0;
            break;
          case BATTLE_EVENT_MESSAGE_NO_WAIT:
            if ((s32)events->queue.operands[eventIndex] >= 0)
              BattleMessage_OpenFar(events->queue.operands[eventIndex]);
            events->phase = 13;
            break;
          case BATTLE_EVENT_CLEAR_ARGUMENTS:
            MessageArgument_ClearAllFar();
            break;
          case BATTLE_EVENT_REMOVE_UNIT:
            BattleObject_DestroyTemporaryObject(
                events->queue.operands[eventIndex]);
            break;
          case BATTLE_EVENT_HIT:
            if (events->pendingCue > 0)
              Sound_PlayFar(events->pendingCue);
            events->actorId = events->queue.operands[eventIndex];
            Actor_SelectAnimationFar(BattleObject_Get(events->actorId)->object,
                                     5);
            events->phase = 10;
            events->timer = 0;
            break;
          case BATTLE_EVENT_DEFEAT: {
            struct BattleUnit *unit;
            void *record;
            s32 recordIndex;

            events->actorId = events->queue.operands[eventIndex];
            BattleEnemy_RecordDefeat(events->actorId, events->flags);
            BattleUnit_ClearStatuses(events->actorId);
            unit = Unit_Get__f11f66c(events->actorId);
            for (recordIndex = 0;
                 (record =
                      GetMotionRecord(BattleObject_Get(events->actorId)->object,
                                      recordIndex)) != 0;
                 recordIndex++) {
              if (unit->side != 1)
                Animation_ApplyChildArgumentFar(record, 4);
              else
                Animation_ApplyChildArgumentFar(record, 5);
            }
            if (unit->side == 1) {
              events->phase = 11;
              events->timer = 0;
            }
            break;
          }
          case BATTLE_EVENT_DRAW_STATUS:
            PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
            break;
          case BATTLE_EVENT_REFRESH_UNIT: {
            struct BattleObj *slot;

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
          case 15:
            BattleObject_RemoveFromLists(events->queue.operands[eventIndex]);
            break;
          }
          events->eventIndex++;
        }
      }
      if (events->phase == BATTLE_PLAYBACK_DISPATCH)
        events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    } else if (events->phase == 3 || events->phase == 13) {
      if (Message_AllTypedFar() == 0)
        return;
      if (events->phase == 13) {
        events->phase = BATTLE_PLAYBACK_DISPATCH;
        events->timer = 0;
      } else {
        events->phase = 5;
        events->arrowResource = -1;
        events->timer = gVBlankCount;
      }
    } else if (events->phase == 5) {
      struct ArrowSprite__presentation *prompt = &events->arrow;
      u8 *arrowTiles = gBattleMessage_AdvanceArrowTiles;
      s32 pictureTiles = (s32)arrowTiles + ((gFrameCount >> 2) & 7) * 128;
      struct BattleMessageDisplay *display =
          (struct BattleMessageDisplay *)Ram_HeapSlots->unk84[4];
      struct MessageWindow *window = display->window;
      struct MessageOffset *offset = display->offset;
      s32 x = 0;

      if (events->arrowResource == -1)
        events->arrowResource = battle->arrowTiles;
      MessageArgument_ClearAllFar();
      IoWriteQueue_ClearBits16(0x0400004a, 4);
      IoWriteQueue_SetBits16(0x0400004a, 16);
      ((u32 *)prompt)[1] = 0xa000;
      ((u32 *)prompt)[2] = x;
      /* FAKEMATCH(barrier): an empty asm after x is stored. */
      asm("");
      prompt->tile = VramCache_Reload(events->arrowResource, pictureTiles);
      x = window->x * 8 + (offset->x >> 8) + 4;
      prompt->x = x;
      prompt->y = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 +
                  (offset->y >> 8) + 6;
      // The held keys reread through a volatile pointer for the last test, a
      // load apart from the B test's; as gInput.held, 16 of 844 instructions
      // differ in tla-en.
      if ((gInput.held & 2) || (gInput.pendingPressed & 0x303) ||
          ((u32)(gVBlankCount - events->timer) > 10 && (KEYS_HELD & 0x303))) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        events->phase = BATTLE_PLAYBACK_DISPATCH;
        events->timer = 0;
      } else {
        Oam_AddSpriteAtDepth(prompt, 240);
        return;
      }
    } else if (events->phase == 10) {
      if (gLastFrameVBlanks)
        events->timer |= 1;
      if (events->timer & 1) {
        if (events->timer & 2) {
          struct BattleObj *slot;

          selection[0] = 255;
          slot = BattleObject_Get(events->actorId);
          BattleObject_SetColorMode(
              slot->object, BattleMotion_GetSlotPalette(events->actorId));
        } else {
          selection[0] = events->actorId;
          selection[1] = 255;
          BattleObject_SetColorMode(BattleObject_Get(events->actorId)->object,
                                    7);
        }
        PartyStatus_HighlightMembersFar(selection);
      }
      events->timer++;
      if (events->timer <= 7)
        return;
      events->phase = BATTLE_PLAYBACK_DISPATCH;
      events->timer = 0;
    } else if (events->phase == 11) {
      if (events->timer == 0 || events->timer >= 0x400) {
        s32 colorMode = 6;

        if (events->timer == 0 && events->flags != 0) {
          s32 kind = BattleSprite_GetDefeatSound(
              ((struct BattleUnit *)Unit_Get__f11f66c(events->actorId))
                  ->enemyId);

          if (kind >= 0) {
            kind--;
            if (kind < 0)
              kind = 0;
            Sound_PlayFar(146 + kind);
          }
          events->timer = 0x400;
        }
        if (events->timer > 0x400 + 30 - 1)
          events->timer = 0;
        if (events->timer == 0) {
          s32 kind = BattleSprite_GetDefeatSound(
              ((struct BattleUnit *)Unit_Get__f11f66c(events->actorId))
                  ->enemyId);

          if (kind >= 0)
            Sound_PlayFar(146 + kind);
        }
        if (events->timer >= 0x400)
          colorMode = (events->timer - 0x400) / 8 % 5 + 1;
        if (colorMode == 6 || (events->timer & 7) == 0) {
          struct MotionRecord *record;
          s32 recordIndex;
          struct MotionRecord **slot;
          s32 lastFrameMask;

          for (recordIndex = 0, slot = (struct MotionRecord **)records,
              lastFrameMask = 0xff;
               (record =
                    GetMotionRecord(BattleObject_Get(events->actorId)->object,
                                    recordIndex)) != 0;
               recordIndex++) {
            struct ChildEntry *child;
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
        struct BattleObj *slot;
        void *record;
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
          events->phase = BATTLE_PLAYBACK_DISPATCH;
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

u32 BattlePlayback_Reset(void) {
  struct BattleEventState *events;
  struct BattleWork *work;

  work = Ram_HeapSlots->battleWork;
  events = &work->events;
  events->phase = BATTLE_PLAYBACK_IDLE;
  events->queue.count = 0;
  events->eventIndex = 0;
  events->timer = 0;
  events->queue.targetIndex = 0;
  events->pendingCue = 0x86;
  events->unk16a = 0;
  events->flags = 0;
}

s32 BattlePlayback_WaitForReady(void) {
  s32 state;
  struct BattleEventState *events;

  events = &Ram_HeapSlots->battleWork->events;
  state = events->phase;
  if (state == BATTLE_PLAYBACK_IDLE) {
    events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    state = BATTLE_PLAYBACK_RESOLVE_TARGET;
  }
  if (state != BATTLE_PLAYBACK_READY) {
    do {
      wait(1U);
    } while (events->phase != BATTLE_PLAYBACK_READY);
  }
  Task_Remove((u32)BattlePlayback_Run);
  return BattlePlayback_Reset();
}
