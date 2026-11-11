/// @file
/// A turn's actions: the party's commands, the enemies' or the link
/// opponent's, and their action order.
// Part of battle.c.

#include "../battle_internal.h"

void SceneTransform_ApplyYaw(s32 yaw);
void SceneTransform_ApplyPitch(s32 pitch);

void Camera_ConfigureScene(s32 pos) {
  struct BattleCamera *state = Ram_HeapSlots->cameraWork;
  struct BattlePresentationTransition *secondary =
      Ram_HeapSlots->transitionWork;
  struct FixedPointPosition local;
  u32 result;
  s32 one;

  state->pos[1] = 160 << 11;
  state->pos[0] = 0;
  state->pos[2] = 0;
  secondary->targetYaw = 128 << 7;
  state->yaw = 128 << 7;
  state->pitch = 244 << 8;
  state->followPos = 0;
  state->distance = 0x02ee0000;
  state->unk18 = 0;

  Render_ResetTransformState();
  SceneTransform_ApplyPosition(&state->pos[0]);
  SceneTransform_ApplyYaw((s16)state->yaw);
  SceneTransform_ApplyPitch((s16)state->pitch);

  local.x = 0;
  local.y = 0;
  local.z = state->distance;
  Iwram_TransformVector(&local, (s32 *)state);
  one = 1;

  result = Iwram_RatioMulQ14(0x03c90000, 192 << 8);
  Projection_Set(0, result, 0x07920000);

  gCameraSceneParameters[4] = pos + 120;
  secondary->active = one;
  BattleCamera_SetRange(240 << 15, (0x76 - pos) << 16, 0, 128 << 4, 128 << 10);
  secondary->flag = one;
  secondary->active = 0;
}

#define Unit_CalculateFar ((void (*)(u16))Unit_CalculateFar)

void BattleUnit_ResetGuardLevels(void) {
  u16 ids[14];
  s32 count;
  s32 index;

  count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, (u16 *)ids);
  for (index = 0; index < count; index++) {
    struct BattleUnit *actor;

    actor = Unit_Get(ids[index]);
    actor->guardLevel = 0;
    Unit_CalculateFar(ids[index]);
  }
}

s32 __divsi3(s32, s32);
u32 Random16(void);

struct BattleEscapeState {
  u8 reserved00[0x45];
  u8 guaranteed;
  u8 failedAttempts;
};

s32 BattleEscape_CheckSuccess(void) {
  s32 escaped;
  s16 livingUnits[14];
  s32 livingCount;
  s32 levelTotal;
  s32 unitIndex;
  s32 chance;
  struct BattleEscapeState *escapeState;

  escaped = 0;
  escapeState = (struct BattleEscapeState *)Ram_HeapSlots->battleWork;
  if (escapeState->guaranteed == 1) {
    escaped = 1;
  } else {
    levelTotal = 0;
    chance = 0x1388 + (escapeState->failedAttempts * 0x7D0);
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, livingUnits);
    for (unitIndex = 0; unitIndex < livingCount; unitIndex++) {
      levelTotal += ((u8 *)Unit_Get((s32)livingUnits[unitIndex]))[0x0f];
    }
    chance += __divsi3(levelTotal * 0x1F4, livingCount);
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, livingUnits);
    levelTotal = 0;
    for (unitIndex = 0; unitIndex < livingCount; unitIndex++) {
      levelTotal += ((u8 *)Unit_Get((s32)livingUnits[unitIndex]))[0x0f];
    }
    chance -= __divsi3(levelTotal * 0x1F4, livingCount);
    if ((chance > 0) &&
        ((u32)((u32)(0x2710 * Random16()) >> 0x10) < (u32)chance)) {
      escaped = 1;
    }
    escapeState->failedAttempts++;
  }
  if (gMainRoutineWork.battleRule24b == 2) {
    escaped = 0;
  }
  return escaped;
}

s32 Func_080381c0(struct BattleActionRecord *entries, void *buffer, s32 mode);

s32 Battle_BuildUnitActions(struct BattleActionRecord *entries) {
  void *buffer = Heap_AllocEwram(17);
  s32 count = Func_080381c0(entries, buffer, 0);
  s32 result = -1;

  if (count >= 0)
    result = count;
  Heap_Release(buffer);
  return result;
}

s32 Func_08122514(struct BattleActionRecord *request, s32 retry);

s32 Battle_BuildOpponentActions(struct BattleActionRecord *entries) {
  u16 unitIds[14];
  s32 entryCount = 0;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 unitCount;
  s32 shuffleOrUnitIndex;

  if (battle->encounterMode == 1)
    return 0;

  unitCount = BattleParty_ListLivingUnits(2, unitIds);
  if (unitCount == 0)
    return 0;

  for (shuffleOrUnitIndex = 0; shuffleOrUnitIndex < 32; shuffleOrUnitIndex++) {
    u32 first = (unitCount * Random16()) >> 16;
    u32 second = (unitCount * Random16()) >> 16;
    s32 swap = unitIds[first];
    unitIds[first] = unitIds[second];
    unitIds[second] = swap;
  }

  if (battle->encounterMode == 2) {
    s32 limit = ((Random16() * 5) >> 16) + 1;

    if (limit <= 1)
      limit = 2;
    if (limit < unitCount)
      unitCount = limit;
  }

  for (shuffleOrUnitIndex = 0; shuffleOrUnitIndex < unitCount;
       shuffleOrUnitIndex++) {
    s32 unitId = unitIds[shuffleOrUnitIndex];
    struct BattleUnit *unit = Unit_Get(unitId);
    s32 copyIndex;

    for (copyIndex = 0; copyIndex < unit->actionEntryCount; copyIndex++) {
      struct BattleActionRecord *entry = &entries[entryCount];
      s32 agility;

      entry->unitId = unitId;
      agility = unit->agi;
      entry->priority = agility;
      entry->unk0e = copyIndex;
      if (unit->actionEntryCount == 4) {
        if (copyIndex == 0)
          ;
        else if (copyIndex == 1)
          entry->priority = (s16)agility * 5 / 6;
        else if (copyIndex == 2)
          entry->priority = (s16)agility * 4 / 6;
        else
          entry->priority = (s16)agility / 2;
      } else if (unit->actionEntryCount == 3) {
        if (copyIndex == 0)
          ;
        else if (copyIndex == 1)
          entry->priority = (s16)agility * 3 / 4;
        else
          entry->priority = (s16)agility / 2;
      } else if (copyIndex != 0) {
        entry->priority = (s16)agility / 2;
      }

      if (unit->sleep != 0 || unit->stun != 0) {
        entry->command = 8;
        entry->parameter = 0;
        entry->target = 0x100;
      } else {
        Func_08122514(entry, 0);
      }

      entryCount++;

      if (battle->encounterMode == 2)
        break;
    }
  }

  return entryCount;
}

static inline void BattleAction_Swap(struct BattleActionRecord *a,
                                     struct BattleActionRecord *b) {
  struct BattleActionRecord temporary;

  // The swap as an inline takes the temporary's address afresh at each copy;
  // written in the sort's loop, 44 of 110 instructions of
  // BattleQueue_SortByPriority differ in tla-en.
  Iwram_CopyWords(&temporary, a, sizeof(struct BattleActionRecord));
  Iwram_CopyWords(a, b, sizeof(struct BattleActionRecord));
  Iwram_CopyWords(b, &temporary, sizeof(struct BattleActionRecord));
}

/// Sorts the actions by descending priority, after raising the priority of
/// the Djinn with effects 46, 47, 53, 72, 74, 76, 79 and 88 by 10000.
void BattleQueue_SortByPriority(struct BattleActionRecord *entries, s32 count) {
  s32 i;
  s32 j;
  s32 swapped;

  for (i = 0; i < count; i++) {
    struct BattleActionRecord *entry = &entries[i];

    if (entry->command == 5) {
      struct BattleAct *action;

      Unit_Get(entry->unitId);
      action = BattleAction_Get(Djinn_GetDefinitionHeaderFar(
          (entry->parameter) >> 8 & 15, entry->parameter & 0xff));
      if (action->effect == 72 || action->effect == 76 ||
          action->effect == 79 || action->effect == 74 ||
          action->effect == 46 || action->effect == 47 ||
          action->effect == 88 || action->effect == 53) {
        u32 priority = entry->priority;

        entry->priority = priority + 10000;
      }
    }
  }

  do {
    swapped = 0;
    for (j = count - 1; j > 0; j--) {
      if ((s16)entries[j].priority > (s16)entries[j - 1].priority) {
        BattleAction_Swap(&entries[j], &entries[j - 1]);
        swapped++;
      }
    }
  } while (swapped != 0);
}

s32 Serial_StartSend(void *source, s32 size);
s32 Serial_GetActiveTransfers(void);
void Heap_Release(void *block);

extern volatile u16 gLinkStatus;

struct BattleLinkedActionState {
  s32 count;
  s32 check;
  s32 seed;
  u8 unk0c[28];
};

s32 Battle_AppendLinkedActions(struct BattleActionRecord *actions, s32 count) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 result = 0;
  s32 allocationSize = (u32)(count * 16 + 19) / 20 * 20;
  struct BattleLinkedActionState *state =
      (struct BattleLinkedActionState *)Heap_AllocEwram(40);
  s32 index;

  s32 BattleLink_SendActions(void) {
    s32 timeout;
    s32 disconnected;
    s32 status = Serial_StartSend(state, 20);

    disconnected = 0;
    timeout = 300;
    if (status == -1)
      return status;
    while (Serial_GetActiveTransfers()) {
      wait(1);
      if (--timeout < 0)
        return -1;
      if ((gLinkStatus & 3) != 3) {
        if (++disconnected > 24)
          return -1;
      } else {
        disconnected = 0;
      }
    }
    if (allocationSize) {
      status = Serial_StartSend(actions, allocationSize);
      if (status == -1)
        return status;
      while (Serial_GetActiveTransfers()) {
        wait(1);
        if (--timeout < 0)
          return -1;
        if ((gLinkStatus & 3) != 3) {
          if (++disconnected > 24)
            return -1;
        } else {
          disconnected = 0;
        }
      }
    }
    return 0;
  }

  s32 BattleLink_ReceiveActions(void) {
    s32 timeout;
    s32 disconnected;
    s32 status = Serial_StartReceive(state);

    disconnected = 0;
    timeout = 300;
    if (status == -1)
      return status;
    while (Serial_GetActiveTransfers()) {
      if (gSerialReceivedSize > 20)
        return -1;
      wait(1);
      if (--timeout < 0)
        return -1;
      if ((gLinkStatus & 3) != 3) {
        if (++disconnected > 24)
          return -1;
      } else {
        disconnected = 0;
      }
    }
    if (gSerialReceivedSize != 20)
      return -1;
    result = state->count;
    if (state->count) {
      status = Serial_StartReceive(actions + count);
      if (status == -1)
        return status;
      while (Serial_GetActiveTransfers()) {
        if (gSerialReceivedSize > (u32)(result * 16 + 19) / 20 * 20)
          return -1;
        wait(1);
        if (--timeout < 0)
          return -1;
        if ((gLinkStatus & 3) != 3) {
          if (++disconnected > 24)
            return -1;
        } else {
          disconnected = 0;
        }
      }
      if (gSerialReceivedSize != (u32)(result * 16 + 19) / 20 * 20)
        return -1;
    }
    return 0;
  }

  for (index = 0; index < count; index++) {
    struct BattleActionRecord *action = &actions[index];

    action->slot = battle->ownerSlots[action->unitId];

    if (battle->linkSide == 0) {
      if (action->priority & 1)
        action->priority++;
    } else {
      action->priority |= 1;
    }
  }

  if (battle->linkPaused == 0) {
    if (battle->linkSide == 0) {
      u32 saved;

      state->count = count;
      state->check = Battle_RandomFar();
      IRQ_LOCK(saved);
      gBattleRandomSeed = state->seed = gRandomState;
      IRQ_UNLOCK(saved);
      if (BattleLink_SendActions() < 0 || BattleLink_ReceiveActions() < 0)
        goto fail;
      result = state->count;
    } else {
      if (BattleLink_ReceiveActions() < 0)
        goto fail;
      result = state->count;
      state->count = count;
      if (BattleLink_SendActions() < 0)
        goto fail;
      if (Battle_RandomFar() != state->check)
        goto fail;
      gBattleRandomSeed = state->seed;
    }

    for (index = 0; index < result; index++) {
      struct BattleActionRecord *action = &actions[count + index];

      action->unitId = action->slot;
      action->target = action->target ^ 0x80;
    }

    Heap_Release(state);
    return result;
  }

fail:
  BattleLink_ResetTransferState();
  Serial_RemoveIrqHandlers();
  Heap_Release(state);
  return -1;
}

extern struct BattlePresentationTransition *gTransitionWork;

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);

/// Copies BG palettes 15 to 14 and 5 to 4, adjusting brightness.
/// offset: amount added to each RGB component, clamped to 0-31.
void Palette_CopyWithBrightnessOffset(s32 offset) {
  s32 n;
  s32 paletteNumber;
  s32 col;
  s32 mask;
  s32 idx;

  paletteNumber = 15;
  n = 0;
  mask = 31;
  do {
    for (col = 0; col <= 15; col++) {
      u32 color;
      s32 b;
      s32 g;
      s32 r;
      s32 val;

      idx = paletteNumber * 16 + col;
      color = ((u16 *)0x05000000)[idx];
      b = (color >> 10) & mask;
      g = (color >> 5) & mask;
      r = color & mask;
      b += offset;
      g += offset;
      r += offset;
      if (b > 31)
        b = 31;
      if (g > 31)
        g = 31;
      if (r > 31)
        r = 31;
      if (b < 0)
        b = 0;
      if (g < 0)
        g = 0;
      if (r < 0)
        r = 0;
      val = b << 10;
      val |= g << 5;
      val |= r;
      ((s16 *)0x04ffffe0)[idx] = val; // Into the preceding palette.
    }
    n++;
    paletteNumber = 5;
  } while (n <= 1);
}

/// Builds this turn's actions and sorts them by descending priority: the
/// allies' commands and either the link opponent's or the enemies' actions.
/// Returns the action count, or a negative value on cancellation or link
/// failure.
/// acts: action list, up to 20 entries.
s32 Battle_GatherActions(struct BattleActionRecord *acts) {
  struct BattleUnit *unit;
  struct BattleWork *work;
  u8 *mode;
  s32 cnt;
  s32 add;
  s32 i;

  work = Ram_HeapSlots->battleWork;
  {
    struct BattleActionRecord *rec = work->actions;
    u32 n;

    for (n = 0; n < 20; n++) {
      rec[n].unitId = BATTLE_UNIT_LIST_END;
      rec[n].priority = 0x8000; // Lowest priority.
    }
  }

  BattleUnit_ResetGuardLevels();
  Palette_CopyWithBrightnessOffset(8);
  // Flag set while commands are being selected.
  GameFlag_SetBit(363);
  mode = &work->encounterMode;
  Camera_ConfigureScene(0);
  Window_CloseSharedFar();

  // Ally commands (unavailable when enemies strike first).
  if (*mode != 2) {
    cnt = Battle_BuildUnitActions(acts);
    if (cnt < 0)
      goto finish;
    // A failed escape gives enemies the same advantage as striking first.
    if (cnt != 0 && acts[0].command == 99 && BattleEscape_CheckSuccess() == 0)
      *mode = 2;
  } else {
    cnt = 0;
  }

  // Link opponent or enemy actions.
  if (Ram_HeapSlots->battleWork->twoSided != 0) {
    add = Battle_AppendLinkedActions(acts, cnt);
    if (LinkBattle_WaitSync() < 0) {
      cnt = -1;
      goto finish;
    }
    cnt += add;
    if (add < 0) {
      cnt = -1;
      goto finish;
    }
  } else {
    cnt += Battle_BuildOpponentActions(acts + cnt);
  }

  // Sort by priority and set guards for Defend (3 and 7) first.
  BattleQueue_SortByPriority(acts, cnt);
  if (cnt > 0) {
    struct BattleActionRecord *act = acts;

    i = cnt;
    do {
      if (act->command == 3 || act->command == 7) {
        unit = Unit_Get(act->unitId);
        unit->guardLevel = act->command == 3 ? 1 : 3;
      }
      act++;
      i--;
    } while (i != 0);
  }

finish:
  GameFlag_ClearBit(363);
  Camera_InitializeDefaultTransform();
  Ram_HeapSlots->transitionWork->targetYaw = 0x2000;
  return cnt;
}

/// Returns the unit ID at a formation position.
/// slot: low 4 bits are the position; 0x80 selects the enemy formation.
s16 Battle_GetTaggedSlotValue(s32 slot) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;

  if ((slot & 0x80) != 0)
    return work->enemyIds[slot & 0xf];
  return work->partyUnits[slot & 0xf];
}

/// Returns a unit's formation position, or -1 if absent.
/// Allies use 0x100 | position; enemies use 0x180 | position.
/// id: unit ID (0-7 allies, all others enemies).
s32 Battle_FindTaggedSlotByValue(u32 id) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  s32 i;
  s16 unit;

  if (id <= 7) {
    for (i = 0;; i++) {
      unit = work->partyUnits[i];
      if (unit == BATTLE_UNIT_LIST_END)
        return -1;
      if (unit != BATTLE_UNIT_REMOVED && unit == id)
        return i | 0x100;
    }
  }
  for (i = 0;; i++) {
    unit = work->enemyIds[i];
    if (unit == BATTLE_UNIT_LIST_END)
      return -1;
    if (unit != BATTLE_UNIT_REMOVED && unit == id)
      return i | 0x180;
  }
}
