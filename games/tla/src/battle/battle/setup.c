/// @file
/// The battle camera each frame, the opening announcement, the unit lists, the
/// tile pattern, and the party's battle state and Djinn as a battle starts.
// Part of battle.c.

#include "../battle_internal.h"
#include "gamepak/far_call/game.h"
#include "module/battle/battle.h"
#include "module/system/system.h"
#include "transform.h"

void SceneTransform_SetPosition(s32 *position);

void BattleCamera_Update(void) {
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
  Transform_SetYaw((s16)camera->yaw);
  Transform_ApplyPitch((s16)camera->pitch);
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
void BattleIntro_AnnounceEncounter(s32 enemyCount) {
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
        BattleMessage_ShowAndWaitFar((s32)MsgLastEnemyAppeared);
      else
        BattleMessage_ShowAndWaitFar((s32)MsgEnemyAppeared);
      announced++;
      BattleMessage_WaitForAdvance();
    } while (announced != enemyCount);
  }

  BattleMessage_CloseFar();
  if (battle->encounterMode == 1) {
    MessageArgument_ClearAllFar();
    BattleMessage_ShowAndWaitFar((s32)MsgPartyStrikesFirst);
    BattleMessage_WaitForAdvance();
  } else if (battle->encounterMode == 2) {
    MessageArgument_ClearAllFar();
    BattleMessage_ShowAndWaitFar((s32)MsgPartySurprised);
    BattleMessage_WaitForAdvance();
  }
}

// BattleUnit_Collect: The Broken Seal's BattleParty_CollectUnitList.

void BattleUnit_Collect(void) {
  u16 buf[14];
  struct BattleWork *battle;
  s32 count;
  s32 i;
  s32 index;
  s32 kind;

  battle = Ram_HeapSlots->battleWork;
  count = BattleParty_PrepareMembers(buf);
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
    index = gBattleParty_CenterOrderOffsets[i] + count / 2;
    battle->enemyIds[index] = buf[i];
  }
done:
  battle->enemyIds[count] = 0xff;
}

/// Moves the 16 bytes at VRAM 0x06000290 down to 0x06000280 and clears the
/// 20 bytes from 0x0600028c.
s32 Battle_ShiftVramTiles(void) {
  Dma_Set((const void *)0x06000290, (void *)0x06000280, DMA_COPY_HALFWORDS(8),
          REG_DMA3);
  return Iwram_ClearWordsEntry((void *)0x0600028c, 20);
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

  count = BattleParty_PrepareMembers(owners);
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

      Unit_CalculateFar(owners[i]);
      i++;
    } while (i < count);
  }
}

/// Clears every unit's guard and Ground's hold at the end of a turn: the
/// party's members and reserves and the opponents. Battle_Run calls it once
/// the turn's actions have played.
void BattleParty_ClearGuards(void) {
  u16 ids[16];
  u16 *id = ids;
  s32 count;

  count = BattleParty_PrepareMembers(ids);
  count += BattleParty_PrepareReserveOwners(ids + count);
  count += BattleParty_ListActorIds(2, ids + count);
  if (count > 0) {
    do {
      struct BattleUnit *unit = Unit_Get(*id++);

      unit->guardLevel = 0;
      unit->immobile = 0;
      count--;
    } while (count != 0);
  }
}

/// Adds to the trade offers every djinn an active owner can offer that is
/// not offered yet; owners above 7 offer on the second side.
void DjinnRecovery_Reconcile(void) {
  u16 owners[10];
  s32 count;
  s32 n;
  u32 owner;
  s32 element;
  s32 djinn;
  s32 i;
  struct DjinnRecoveryList *list;

  count = BattleParty_PrepareMembers(owners);
  for (n = 0; n < count; n++) {
    owner = owners[n];
    for (element = 0; element <= 3; element++) {
      for (djinn = 0; djinn <= 19; djinn++) {
        if (Djinn_CanSetFar(owner, element, djinn) != 0) {
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

// BattleSummon_UpdateAvailability: The Broken Seal's
// BattleSummon_UpdateAvailability; this game keeps the high half of the offer
// state's mask, tests 16 summons and writes the mask back.

/// Works out which summons the party can call: one whose Djinn requirement
/// of every element is met by the Djinn the party holds of that element
/// (djinnHeldCounts, set or not), all party members together. Whether enough of
/// them are on standby is checked as the summon is chosen
/// (BattleSummonMenu_Run, Battle_CollectPartyCommands) and again when it
/// is built (BattleActionRun_Build). Stores the mask of them (bit n for
/// summon n) and returns the state that holds it. Called by
/// Battle_Run as the battle is set up and at the start of each
/// turn, which asks for the party status window's standby Djinn column
/// (PARTY_STATUS_SUMMONS) while the mask is not 0; PartyStatus_Draw shows it
/// only while a Djinni is on standby. Writes the party recovery table's
/// availableMask.
void BattleSummon_UpdateAvailability(void) {
  u8 totals[4];
  u16 partyMembers[10];
  s32 partySize;
  s32 element;
  u32 availableMask;

  partySize = BattleParty_PrepareMembers(partyMembers);
  availableMask = ((struct BattleSummonState *)Djinn_GetRecoveryTableFar(0))
                      ->availableMask &
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

/// An empty function, right after BattleSummon_UpdateAvailability.
void BattleSummon_ReservedNoOp(void) {}
