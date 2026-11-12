/// @file
/// The battle camera each frame, the opening announcement, the unit lists, the
/// tile pattern, and the party's battle state and Djinn as a battle starts.
// Part of battle.c.

#include "../battle_internal.h"

#define REG_SIOCNT (*(volatile u32 *)0x04000128)

void SceneTransform_RewindStack(void);
void SceneTransform_SetPosition(s32 *position);
void Func_080150ac(s32 yaw);
void SceneTransform_ApplyPitch(s32);

void Func_08118adc(void) {
  struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
  struct BattlePresentationTransition *transition =
      Ram_HeapSlots->transitionWork;
  struct FixedPointPosition eyeOffset;
  s32 *focus;
  s16 delta;
  u32 linkId;

  if (Ram_HeapSlots->battleWork->twoSided != 0) {
    if ((gLinkStatus & 3) != 3) {
      Ram_HeapSlots->battleWork->unk051++;
      if (Ram_HeapSlots->battleWork->unk051 > 24) {
        Ram_HeapSlots->battleWork->linkPaused = 1;
      }
    } else {
      linkId = (REG_SIOCNT << 26) >> 30;
      if (Ram_HeapSlots->battleWork->linkSide != linkId) {
        Ram_HeapSlots->battleWork->linkPaused = 1;
      }
      Ram_HeapSlots->battleWork->unk051 = 0;
    }
  }

  if (transition->frames != 0) {
    delta = transition->targetYaw - camera->yaw;
    delta /= 16;
    camera->yaw += delta;
    transition->frames--;
  }

  focus = camera->pos;
  if (camera->followPos != 0) {
    focus = camera->followPos;
  }

  SceneTransform_RewindStack();
  Func_080150ac((s16)camera->yaw);
  SceneTransform_ApplyPitch((s16)camera->pitch);
  SceneTransform_SetPosition(focus);

  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = camera->distance;
  Iwram_TransformVector(&eyeOffset.x, (s32 *)camera);

  if (transition->flag == 0) {
    BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
  }
}

/// Announces each enemy that appears, then who strikes first. The Broken
/// Seal's BattleIntro_AnnounceEncounter.
void Func_08118bcc(s32 enemyCount) {
  s16 enemies[8];
  struct BattleWork *battle;
  s16 *enemy;
  s32 announced;

  battle = Ram_HeapSlots->battleWork;
  MessageArgument_ClearAllFar();
  BattleEnemy_List(enemies);

  announced = 0;
  if (enemyCount != 0) {
    enemy = enemies;
    do {
      MessageArgument_PushFar((u16)*enemy++, 1);
      if (announced == enemyCount - 1)
        Message_ShowAndWaitCoreFar((s32)MsgLastEnemyAppeared);
      else
        Message_ShowAndWaitCoreFar((s32)MsgEnemyAppeared);
      announced++;
      BattleMessage_WaitForAdvance();
    } while (announced != enemyCount);
  }

  Window_CloseSharedFar();
  if (battle->encounterMode == 1) {
    MessageArgument_ClearAllFar();
    Message_ShowAndWaitCoreFar((s32)MsgPartyStrikesFirst);
    BattleMessage_WaitForAdvance();
  } else if (battle->encounterMode == 2) {
    MessageArgument_ClearAllFar();
    Message_ShowAndWaitCoreFar((s32)MsgPartySurprised);
    BattleMessage_WaitForAdvance();
  }
}

// Func_08118c68: The Broken Seal's BattleParty_CollectUnitList.

extern const s8 gData_081287c8[];

void Func_08118c68(void) {
  u16 buf[14];
  struct BattleWork *battle;
  s32 count;
  s32 i;
  s32 index;
  s32 kind;

  battle = Ram_HeapSlots->battleWork;
  count = BattleParty_PrepareActiveOwners(buf);
  for (i = 0; i < count; i++)
    battle->partyUnits[i] = buf[i];
  battle->partyUnits[count] = 0xff;

  count = BattleEnemy_List(buf);
  kind = battle->unk042;
  if (kind >= 0) {
    if (kind <= 1) {
      for (i = 0; i < count; i++)
        battle->enemyIds[i] = buf[i];
      goto done;
    }
  }
  for (i = 0; i < count; i++) {
    index = gData_081287c8[i] + count / 2;
    battle->enemyIds[index] = buf[i];
  }
done:
  battle->enemyIds[count] = 0xff;
}

/// Moves the 16 bytes at VRAM 0x06000290 down to 0x06000280 and clears the
/// 20 bytes from 0x0600028c.
s32 BattleStaging_InitializeTilePattern(void) {
  Dma_Set((const void *)0x06000290, (void *)0x06000280, DMA_COPY_HALFWORDS(8),
          REG_DMA3);
  return Iwram_ClearWords((void *)0x0600028c, 20);
}

void BattleTransition_ClearBg0Vofs(void) {
  u32 zero = 0;

  REG_BG0VOFS = zero;
}

void Party_ReservedNoOp5B14(s32 value) {}

s32 BattleParty_ResetActiveRuntimeFields(void) {
  u16 owners[10];
  s32 count;
  s32 i;

  count = BattleParty_PrepareActiveOwners(owners);
  count += BattleParty_PrepareReserveOwners(owners + count);

  i = 0;
  if (i < count) {
    struct BattleUnit *unit;
    u8 *cursor;
    s32 remaining;

    do {
      unit = Unit_Get(owners[i]);
      remaining = 3;
      cursor = (u8 *)&unit->elementLevels[3];

      do {
        remaining--;
        *cursor-- = 0;
      } while (remaining >= 0);

      unit->attackTurns = 0;
      unit->attackStage = 0;
      unit->defenseTurns = 0;
      unit->defenseStage = 0;
      unit->resistanceTurns = 0;
      unit->resistanceStage = 0;
      unit->delusion = 0;
      unit->confusion = 0;
      unit->charm = 0;
      unit->stun = 0;
      unit->sleep = 0;
      unit->psySeal = 0;
      unit->refrain = 0;
      unit->reflect = 0;
      unit->grimReaper = 0;
      unit->equipBonus[0] = 0;
      unit->equipBonus[1] = 0;
      unit->ready = 0;
      unit->immobile = 0;
      unit->agilityTurns = 0;
      unit->agilityStage = 0;
      unit->challenge = 0;

      BattleUnit_Recalculate(owners[i]);
      i++;
    } while (i < count);
  }
}

void Func_08118e64(void) {
  u16 ids[16];
  u16 *id = ids;
  s32 count;

  count = BattleParty_PrepareActiveOwners(ids);
  count += BattleParty_PrepareReserveOwners(ids + count);
  count += BattleParty_ListActorIds(2, ids + count);
  if (count > 0) {
    do {
      u8 *unit = Unit_Get(*id++);

      unit[299] = 0;
      unit[325] = 0;
      count--;
    } while (count != 0);
  }
}

s32 Djinn_IsReadyFar(s32 owner, s32 element, s32 djinn);
void DjinnRecovery_AddStandbyFar(s32 owner, s32 element, s32 djinn);

/// Adds to the trade offers every djinn an active owner can offer that is
/// not offered yet; owners above 7 offer on the second side.
void Func_08118eb0(void) {
  u16 owners[10];
  s32 count;
  s32 n;
  u32 owner;
  s32 element;
  s32 djinn;
  s32 i;
  struct DjinnRecoveryList *list;

  count = BattleParty_PrepareActiveOwners(owners);
  for (n = 0; n < count; n++) {
    owner = owners[n];
    for (element = 0; element <= 3; element++) {
      for (djinn = 0; djinn <= 19; djinn++) {
        if (Djinn_IsReadyFar(owner, element, djinn) != 0) {
          list = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
                      owner > 7))
                      ->list;
          for (i = 0; i < list->count; i++) {
            if (element == list->entries[i].element &&
                djinn == list->entries[i].index)
              break;
          }
          if (i == list->count)
            DjinnRecovery_AddStandbyFar(owner, element, djinn);
        }
      }
    }
  }
}

// Func_08118f6c: The Broken Seal's BattleSummon_UpdateAvailability; this game
// keeps the high half of the offer state's mask, tests 16 summons and writes
// the mask back.

void Func_08118f6c(void) {
  u8 totals[4];
  u16 partyMembers[10];
  s32 partySize;
  s32 element;
  u32 availableMask;

  partySize = BattleParty_PrepareActiveOwners(partyMembers);
  availableMask =
      ((struct BattleSummonState *)Djinn_GetRecoveryTableFar(0))->availableMask &
      0xffff0000;

  element = 0;
  do {
    totals[element] = 0;
    {
      s32 partySlot;

      for (partySlot = 0; partySlot < partySize; partySlot++) {
        struct BattleUnit *member = Unit_Get(partyMembers[partySlot]);

        totals[element] += member->djinnOwnedCounts[element];
      }
    }
    element++;
  } while (element <= 3);

  element = 0;
  do {
    const struct SummonDefinition *summon = SummonDefinition_Get(element);
    s32 elementsMet;

    if (summon != 0) {
      const u8 *required = summon->djinnRequired;

      elementsMet = 0;
      if (totals[0] >= required[0]) {
        u8 *total = totals;

        do {
          elementsMet++;
          if (elementsMet > 3)
            break;
          total++;
          required++;
        } while (*total >= *required);
      }

      if (elementsMet == 4)
        availableMask |= 1u << element;
    }
    element++;
  } while (element <= 15);

  ((struct BattleSummonState *)Djinn_GetRecoveryTableFar(0))->availableMask =
      availableMask;
}

/// An empty function, right after Func_08118f6c.
void Func_08119050(void) {}
