#include "task.h"
#include "types.h"
#include "ram_buffer.h"
#include "scene.h"
#include "fixed_math.h"
void Camera_ConfigureSceneFar(s32 offset);
void Link_DrawShiftedTilePair(s32 destination);
s32 Link_CreateCountdownLabelWindow(void);

#define COUNTDOWN_START_FRAMES 900
#define FRAMES_PER_SECOND 60

struct CountdownDisplayEntry {
  u8 data[12];
};

struct LinkSignature {
  u8 padding000[8];
  u16 e;
  u16 d;
  u16 v;
  u16 s;
  u8 padding010[8];
};

struct LinkCountdownState {
  struct CountdownDisplayEntry entries[3];
  u8 entryActive[3];
  u8 padding027;
  s32 targetOffset;
  s32 currentOffset;
  u8 padding030[20];
  s32 displayHandle;
  s32 secondaryHandle;
  s32 timer;
  s32 enabled;
};

struct LinkRuntimeState {
  u8 padding000[80];
  u8 side;
  u8 padding051;
  u8 paused;
};

extern struct LinkCountdownState *gLinkCountdownWork;
extern struct LinkRuntimeState *gBattleWork;
extern struct LinkSignature gLinkPeerSignatures[];

void Text_DrawNumberInWindow(s32 value, s32 width, s32 handle, s32 arg3,
                             s32 arg4);

void Sound_PlayFar(s32 soundId);

void UpdateLinkSessionCountdown(void) {
  struct LinkCountdownState *state =
      ((struct LinkCountdownState **)Ram_HeapSlots)[57];
  struct LinkRuntimeState *runtime;
  struct CountdownDisplayEntry *entry;
  u8 *active;
  struct LinkSignature *signature;
  s32 target;
  s32 current;
  s32 difference;
  s32 step;
  s32 newOffset;
  s32 timer;
  s32 nextTimer;
  s32 seconds;
  s32 i;

  if (state != 0) {
    target = state->targetOffset;
    current = state->currentOffset;
    if (target != current) {
      difference = target - current;
      step = __divsi3(difference, 3);
      if (step == 0) {
        step--;
        if (difference >= 0)
          step = 1;
      }
      newOffset = current + step;
      state->currentOffset = newOffset;
      Camera_ConfigureSceneFar(newOffset);
    }

    entry = state->entries;
    active = state->entryActive;
    i = 2;
    do {
      if (*active++ != 0)
        Oam_AddSpriteAtDepth(entry, 240);
      i--;
      entry++;
    } while (i >= 0);
    Link_DrawShiftedTilePair(0x06006680);

    if (state->enabled == 0)
      goto done;

    runtime = (struct LinkRuntimeState *)Ram_HeapSlots->battleWork;
    if (runtime->paused == 0)
      goto load_timer;
    state->timer = 0;
    goto done;

  signature_tail:
    if (signature->v != 'V')
      goto signature_done;
    if (signature->s != 'S')
      goto signature_done;
    state->timer = COUNTDOWN_START_FRAMES;
    timer = state->timer;
    goto signature_done;

  load_timer:
    timer = state->timer;
    if (timer >= 0)
      goto timer_ready;
    signature = &gLinkPeerSignatures[runtime->side ^ 1];
    if (signature->e != 'E')
      goto signature_done;
    if (signature->d == 'D')
      goto signature_tail;

  signature_done:
    if (timer < 0)
      goto done;

  timer_ready:
    if (state->displayHandle == 0 && state->secondaryHandle == 0) {
      state->displayHandle = Link_CreateCountdownLabelWindow();
      timer = state->timer;
    }

    if (timer > 0) {
      nextTimer = timer - 1;
      state->timer = nextTimer;
      timer = nextTimer;
    }
    if (timer < 0)
      goto done;

    seconds = __divsi3(timer + FRAMES_PER_SECOND - 1, FRAMES_PER_SECOND);
    if (seconds != 0 && seconds * FRAMES_PER_SECOND == timer)
      Sound_PlayFar(108);

    if (state->displayHandle != 0)
      Text_DrawNumberInWindow(seconds, 2, state->displayHandle, 16, 8);
  }

done:
  return;
}

#include "edition.h"
#include "input.h"
#include "io_reg.h"
#include "system.h"
#include "main_routine_work.h"
unsigned char Vram_FillWindowBlockPattern(void);
void Text_DrawWideStringAtOffset(s16 *, s32, s32, s32);

// The Japanese edition decodes the message (Message_DecodeRaw) into at most 32
// halfwords; the others copy its text (Message_CopyExpanded) into at most 52.
#if LANGUAGE != LANGUAGE_JAPANESE
#define BATTLE_MENU_MESSAGE_CAPACITY 52
#define BATTLE_MENU_MESSAGE_COPY Message_CopyExpanded
#else
#define BATTLE_MENU_MESSAGE_CAPACITY 32
#define BATTLE_MENU_MESSAGE_COPY Message_DecodeRaw
#endif

s32 Window_Create(s32, s32, s32, s32, s32);
void Window_Close(struct Work *work, s32 release);

s32 Message_CopyExpanded(s32, s16 *, s32);
void Message_DecodeRaw(s32, s16 *, s32);

extern s32 gKeyState;
extern char MsgNoTimeToRun;

s32 Message_ShowLocalizedAndWait(void) {
  s16 buffer[64];
  s32 work;
  s32 result;
  void *state;
  s32 mode;
  s32 attrs;

  state = Ram_HeapSlots->battleWork;
  mode = gMainRoutineWork.battleRule24b;
  if (gMainRoutineWork.battleRule24b == 2 || mode <= 2 ||
      (result = 1, mode > 4)) {
    result = 0;
  }
  if (result == 0) {
    if (*(s8 *)((u8 *)state + 0x43) != 0) {
      result = 1;
    }
  }
  if (result != 0) {
    attrs = 42;
    work = Window_Create(0, 7, 30, 4, attrs);
    Vram_FillWindowBlockPattern();
    BATTLE_MENU_MESSAGE_COPY((s32)&MsgNoTimeToRun, buffer,
                             BATTLE_MENU_MESSAGE_CAPACITY);
    Text_DrawWideStringAtOffset(buffer, work, 0, 4);
    do {
      wait(1);
    } while ((gInput.pressed & (KEY_A | KEY_B)) == 0 &&
             *(s32 *)((u8 *)((void **)Ram_HeapSlots)[57] + 0x4C) != 0);
    Window_Close(work, 1);
  }
  return result;
}

#include "battle_system.h"

/// Classifies an ability's effect for target selection.
/// 0 other, 1 restore HP, 2 restore PP, 3 cure poison, 4 cure an ailment,
/// 5 revive, 6 cure all ailments.
/// act: ability data.
s32 Battle_ClassifyEntryKind(const struct BattleAct *act) {
  s32 val;
  s32 type;
  s32 kind;
  u8 effect;

  kind = 0;
  type = act->flags & 15;
  if (type == 1) // Restore HP.
    kind = 1;
  if (type == 11) // Restore PP.
    kind = 2;
  val = (s8)act->effect;
  effect = val;
  if (effect == 3) // Cure poison.
    kind = 3;
  if (effect == 4) // Cure an ailment.
    kind = 4;
  if (effect == 64) // Cure all ailments.
    kind = 6;
  if (Battle_IsReviveEffectFar(act->effect) != 0) // Revive.
    kind = 5;
  return kind;
}

#include "battle_work.h"
#include "actor.h"
#include "version.h"

s32 BattleParty_PrepareReserveOwnersFar(u16 *owners);
s32 Func_08118140(s32, s32);
struct BattleObj *GetBattleObjectSlotFar(s32 objectId);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x, s32 y,
                                        s32 z);
void PartyStatus_Draw(s32 flags);
void BattleActor_CommitPlacementFar(void);
void BattleHighlight_SetModesFar(u16 *, s32);
s32 BattleLayout_HighlightPartyPanels(u16 *ids);

// The keys, read through a volatile view: the loop reloads them on each test.
#define INPUT ((volatile struct InputState *)&gInput)

/// Lets the player choose which reserve member takes a party slot in battle,
/// previewing each choice in place: left and right cycle through the reserve,
/// A confirms and swaps the two in gMainRoutineWork.activeOwners, and B puts
/// the slot's member back.
s32 Func_0804b8b8(s32 slot) {
  u16 owners[6];
  u16 selected[2];
  struct BattleWork *battle;
  s32 count;

  count = BattleParty_PrepareReserveOwnersFar(owners);
  {
    s32 sel;
    s32 redraw;
    s32 result;
    s32 *reserve;
    s32 current;
    s32 newId;
    s32 i;

    sel = 0;
    redraw = 1;
    battle = Ram_HeapSlots->battleWork;
    result = 0;
    reserve = Ram_HeapSlots->unke4[0];
    if (count <= 0)
      return -1;
    slot &= 0xff;
    current = battle->partyUnits[slot];
    Sound_PlayFar(112);
    // Once-only block: the native's code order after the first sound.
    do {
      goto test;
      for (;;) {
        // Japanese also takes A on the first frame, before the first preview
        // has been drawn.
#if TLA_PATCH_1
        if ((INPUT->pressed & 1) && !redraw) {
#else
        if (INPUT->pressed & 1) {
#endif
          if (Func_08118140(1, 0) == 0) {
            Sound_PlayFar(114);
            goto test;
          }
          Sound_PlayFar(110);
          reserve[65] = current;
          reserve[66] = slot;
          for (i = 0; i < 8; i++) {
            if (gMainRoutineWork.activeOwners[i] == current)
              gMainRoutineWork.activeOwners[i] = newId;
            else if (gMainRoutineWork.activeOwners[i] == newId)
              gMainRoutineWork.activeOwners[i] = current;
          }
          goto done;
        }
        if (INPUT->repeat & 0x20) {
          sel = (sel + count - 1) % count;
          Sound_PlayFar(112);
          redraw = 1;
        } else if (INPUT->repeat & 0x10) {
          sel = (sel + 1) % count;
          Sound_PlayFar(112);
          redraw = 1;
        }
        if (redraw) {
          struct BattleObj *from;
          struct BattleObj *to;
          struct Actor *object;
          struct Actor *place;

          redraw = 0;
          newId = battle->partyUnits[slot] = owners[sel];
          from = GetBattleObjectSlotFar(current);
          to = GetBattleObjectSlotFar(newId);
          to->anchorX = from->anchorX;
          place = (struct Actor *)from->object;
          object = (struct Actor *)to->object;
          to->anchorZ = from->anchorZ;
          Actor_SetPositionAndResetMotionFar(object, place->x, place->y,
                                             place->z);
          PartyStatus_Draw(Ram_HeapSlots->battleWork->partyStatus);
          BattleActor_CommitPlacementFar();
          selected[0] = newId;
          selected[1] = 0xff;
          BattleHighlight_SetModesFar(selected, 2);
          BattleLayout_HighlightPartyPanels(selected);
        }
        wait(1);
      test:
        newId = battle->partyUnits[slot] = owners[sel];
        if (INPUT->pressed & 2)
          break;
      }
      battle->partyUnits[slot] = current;
      result = -1;
      Sound_PlayFar(113);
    done:
      selected[0] = 0xff;
      BattleHighlight_SetModesFar(selected, 0);
      BattleLayout_HighlightPartyPanels(selected);
      PartyStatus_Draw(Ram_HeapSlots->battleWork->partyStatus);
      BattleActor_CommitPlacementFar();
      wait(1);
    } while (0);
    return result;
  }
}

#include "battle_unit.h"
#include "battle_action.h"

s32 BattleParty_ListLivingUnitsFar(s32 sideMask, u16 *unitIds);
void *Unit_Get(s32 owner);

s32 Func_0804babc(u16 *ready, struct BattleActionRecord **cursor,
                  s32 *forcedCount) {
  u16 *unitIds = Heap_AllocEwram(9);
  struct BattleActionRecord *entries = *cursor;
  s32 count = BattleParty_ListLivingUnitsFar(1, unitIds);
  s32 readyCount = 0;
  s32 forced = 0;
  s32 i;
  s32 action;

  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = Unit_Get(unitIds[i]);

    for (action = 0; action < unit->actionEntryCount; action++) {
      if (unit->sleep != 0 || unit->confusion != 0 || unit->charm != 0 ||
          unit->stun != 0) {
        struct BattleActionRecord *entry = &entries[forced];

        entry->unitId = unitIds[i];
        entry->priority = unit->agi;
        entry->command = 8;
        entry->parameter = 0;
        entry->target = 384;
        forced++;
      } else {
        ready[readyCount++] = unitIds[i];
      }
    }
  }
  *forcedCount = forced;
  entries += forced;
  *cursor = entries;
  return readyCount;
}
