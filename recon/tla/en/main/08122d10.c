// Under -mthumb-slow-loads (26 October 2026): differ 46 of 844 in tla-en.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped, each call argument copied into its
// argument register (pinned) at the call or up to three statements before it;
// none reaches 0. One barrier (after line 221 of the draft as it stood)
// lowers it to 42, and no second one lowers it further.
// Draft of BattleEvent_Playback with its nested Func_08122cd0 (compare both:
// alchemy-devtools diff 08122d10.c 08122cd0,08122d10): ☀️'s BattlePlayback_Run
// (games/tbs/src/battle/battle/playback.c), ported; the nested function is
// ☀️'s BattleSprite_ClearTiles. ⚓️ adds event 15 (BattleActor_RemoveFromLists),
// lets gLagFramesShown push a hit's flash on a frame, flashes for 8 frames
// rather than 9, and keeps the message window in heap slot 0x94.
// Near miss, differ 44 of 870 (2026-10-24), instruction order only: 22 pairs,
// a load or a constant one slot from the native, as in ⚓️'s other near misses.
// Forms measured: the held keys read through a volatile view (as ☀️'s
// gKeysHeld is volatile; read plainly, the second test reuses the first read:
// 58), and the fading byte stored as a u8 (tla's battle object splits ☀️'s
// halfword, as reset_actor.c's TlaBattleObj shows; 98 as the s16).
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_type.h"
#include "battle_work.h"
#include "battle_display.h"
#include "battle_event.h"
#include "battle_party.h"
#include "motobj.h"
#include "iwram_call.h"
#include "vram_tab.h"
#include "input.h"
#include "frame.h"
#include "ram_buffer.h"

#define OBJ_VRAM 0x06010000
#define KEYS_HELD (*(volatile u32 *)&gInput.held)

struct AnimationRecord {
  u8 unk00[0x10];
  u8 slot;
  u8 unk11[3];
  u8 width;
  u8 height;
};

struct ArrowSprite {
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
  struct ArrowSprite arrow;
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

struct MessageWindow {
  u8 unk00[0x0c];
  s16 x;
  s16 y;
};

struct MessageOffset {
  u8 unk00[4];
  u16 x;
  u16 y;
};

struct BattleMessageDisplay {
  struct MessageWindow *window;
  struct MessageOffset *offset;
  s32 marked;
};

struct ChildEntry {
  u8 unk00[5];
  u8 colorMode;
  u8 unk06[16];
  u8 frame;
};

struct MotionRecord {
  u8 unk00[40];
  struct ChildEntry *entries[1];
};

extern u32 gFrameTick;
extern u8 Data_0812996c[];

struct BattleUnit *Unit_Get(s32 unitId);
s32 BattleTarget_Execute(struct BattlePlan *plan, s32 slot);
void BattleEvent_GrantDjinnBonus(struct PlaybackEvents *events, u32 value);
void Audio_PlayCue(s32 cue);
void Func_080381d0(s32 message);
s32 UiWork_IsCompleteFar(void);
void Object_SetMode(struct MotionObject *object, s32 mode);
void Func_0812824c(s32 unitId, s32 flags);
void BattleActor_ResetRuntimeFields(s32 unitId);
void Animation_ApplyChildArgumentFar(void *record, s32 value);
void PartyStatus_DrawFar(s32 flags);
s32 BattleUnit_BuildStatusFlags(s32 id, struct BattleObj *slot);
s32 BattleMotion_GetSlotPalette(s32 id);
void BattlePres_SetActorModeAndAction(s32 id);
s32 VramCache_Reload(s32 slot, s32 source);
s32 Trig_Sin(s32 angle);
void Runtime_PushSlotEntry(void *entry, s32 priority);
s32 BattleClass_GetDefeatSoundKind(s32 classId);
void Map_RenderAnimatedTileFrameFar(void *record, s32 step);

void BattleEvent_Playback(void) {
  u16 selection[2];
  struct AnimationRecord *records[4];
  struct BattleMessageDisplay *display;
  struct PlaybackBattleWork *battle =
      (struct PlaybackBattleWork *)Ram_HeapSlots->battleWork;
  struct PlaybackEvents *events = &battle->events;

  void Func_08122cd0(struct AnimationRecord * record, s32 unused) {
    u8 *tiles = (u8 *)OBJ_VRAM + gResourceTableEntries[record->slot].offset;

    Iwram_ClearWords(tiles, record->width * record->height);
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
            Audio_PlayCue(events->queue.operands[eventIndex]);
            break;
          case BATTLE_EVENT_DJINN_BONUS:
            BattleEvent_GrantDjinnBonus(events,
                                        events->queue.operands[eventIndex]);
            break;
          case BATTLE_EVENT_UNIT_NAME:
            UiText_DrawQuantity(events->queue.operands[eventIndex], 1);
            break;
          case BATTLE_EVENT_NUMBER:
            UiText_DrawQuantity(events->queue.operands[eventIndex], 5);
            break;
          case BATTLE_EVENT_ITEM_NAME:
            UiText_DrawQuantity(events->queue.operands[eventIndex] & 0x1ff, 2);
            break;
          case BATTLE_EVENT_ABILITY_NAME:
            UiText_DrawQuantity(events->queue.operands[eventIndex] & 0x3fff, 4);
            break;
          case BATTLE_EVENT_MARK:
            ((struct BattleMessageDisplay *)Ram_HeapSlots->unk84[4])->marked =
                1;
            break;
          case BATTLE_EVENT_MESSAGE:
            if ((s32)events->queue.operands[eventIndex] >= 0)
              Func_080381d0(events->queue.operands[eventIndex]);
            events->phase = 3;
            gInput.pendingPressed = 0;
            break;
          case BATTLE_EVENT_MESSAGE_NO_WAIT:
            if ((s32)events->queue.operands[eventIndex] >= 0)
              Func_080381d0(events->queue.operands[eventIndex]);
            events->phase = 13;
            break;
          case BATTLE_EVENT_CLEAR_ARGUMENTS:
            UiWork_ClearValueNameTablesFar();
            break;
          case BATTLE_EVENT_REMOVE_UNIT:
            BattleActor_DestroyTemporaryObject(
                events->queue.operands[eventIndex]);
            break;
          case BATTLE_EVENT_HIT:
            if (events->pendingCue > 0)
              Audio_PlayCue(events->pendingCue);
            events->actorId = events->queue.operands[eventIndex];
            Object_SetMode(BattleObject_Get(events->actorId)->object, 5);
            events->phase = 10;
            events->timer = 0;
            break;
          case BATTLE_EVENT_DEFEAT: {
            struct BattleUnit *unit;
            void *record;
            s32 recordIndex;

            events->actorId = events->queue.operands[eventIndex];
            Func_0812824c(events->actorId, events->flags);
            BattleActor_ResetRuntimeFields(events->actorId);
            unit = Unit_Get(events->actorId);
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
            BattleMotion_SetRecordChildValues(
                slot->object, BattleMotion_GetSlotPalette(
                                  events->queue.operands[eventIndex]));
            BattlePres_SetActorModeAndAction(
                events->queue.operands[eventIndex]);
            break;
          }
          case 15:
            BattleActor_RemoveFromLists(events->queue.operands[eventIndex]);
            break;
          }
          events->eventIndex++;
        }
      }
      if (events->phase == BATTLE_PLAYBACK_DISPATCH)
        events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    } else if (events->phase == 3 || events->phase == 13) {
      if (UiWork_IsCompleteFar() == 0)
        return;
      if (events->phase == 13) {
        events->phase = BATTLE_PLAYBACK_DISPATCH;
        events->timer = 0;
      } else {
        events->phase = 5;
        events->arrowResource = -1;
        events->timer = gFrameTick;
      }
    } else if (events->phase == 5) {
      struct ArrowSprite *prompt = &events->arrow;
      u8 *arrowTiles = Data_0812996c;
      s32 frameTiles = (s32)arrowTiles + ((gFrameCount >> 2) & 7) * 128;
      struct BattleMessageDisplay *display =
          (struct BattleMessageDisplay *)Ram_HeapSlots->unk84[4];
      struct MessageWindow *window = display->window;
      struct MessageOffset *offset = display->offset;
      s32 x = 0;

      if (events->arrowResource == -1)
        events->arrowResource = battle->arrowTiles;
      UiWork_ClearValueNameTablesFar();
      QueueIoWriteDelay10(0x0400004a, 4);
      QueueIoWriteDelay6(0x0400004a, 16);
      ((u32 *)prompt)[1] = 0xa000;
      ((u32 *)prompt)[2] = x;
      prompt->tile = VramCache_Reload(events->arrowResource, frameTiles);
      x = window->x * 8 + (offset->x >> 8) + 4;
      prompt->x = x;
      prompt->y = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 +
                  (offset->y >> 8) + 6;
      if ((KEYS_HELD & 2) || (gInput.pendingPressed & 0x303) ||
          ((u32)(gFrameTick - events->timer) > 10 && (KEYS_HELD & 0x303))) {
        Audio_PlayCue(111);
        events->phase = BATTLE_PLAYBACK_DISPATCH;
        events->timer = 0;
      } else {
        Runtime_PushSlotEntry(prompt, 240);
        return;
      }
    } else if (events->phase == 10) {
      if (gLagFramesShown)
        events->timer |= 1;
      if (events->timer & 1) {
        if (events->timer & 2) {
          struct BattleObj *slot;

          selection[0] = 255;
          slot = BattleObject_Get(events->actorId);
          BattleMotion_SetRecordChildValues(
              slot->object, BattleMotion_GetSlotPalette(events->actorId));
        } else {
          selection[0] = events->actorId;
          selection[1] = 255;
          BattleMotion_SetRecordChildValues(
              BattleObject_Get(events->actorId)->object, 7);
        }
        BattleLayout_HighlightPartyPanelsFar(selection);
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
          s32 kind = BattleClass_GetDefeatSoundKind(
              Unit_Get(events->actorId)->enemyId);

          if (kind >= 0) {
            kind--;
            if (kind < 0)
              kind = 0;
            Audio_PlayCue(146 + kind);
          }
          events->timer = 0x400;
        }
        if (events->timer > 0x400 + 30 - 1)
          events->timer = 0;
        if (events->timer == 0) {
          s32 kind = BattleClass_GetDefeatSoundKind(
              Unit_Get(events->actorId)->enemyId);

          if (kind >= 0)
            Audio_PlayCue(146 + kind);
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
        BattleActor_RemoveFromLists(events->actorId);
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
        *(u8 *)&slot->fading = 1;
        for (count = 0; (record = GetMotionRecord(slot->object, count)) != 0;
             count++)
          records[count] = record;
        base = events->timer * 4;
        step = base - 20;
        if (step > 127) {
          s32 recordIndex;

          for (recordIndex = 0; recordIndex < count; recordIndex++)
            Func_08122cd0(records[recordIndex], 0);
          BattleObject_MarkForRelease(events->actorId);
          events->phase = BATTLE_PLAYBACK_DISPATCH;
          events->timer = 0;
          return;
        }
        {
          s32 recordIndex;

          for (recordIndex = 0; recordIndex < count; recordIndex++) {
            Map_RenderAnimatedTileFrameFar(records[recordIndex], step);
            Map_RenderAnimatedTileFrameFar(records[recordIndex], base - 19);
            Map_RenderAnimatedTileFrameFar(records[recordIndex], base - 18);
            Map_RenderAnimatedTileFrameFar(records[recordIndex], base - 17);
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
