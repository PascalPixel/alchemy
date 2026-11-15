/// @file
/// The battle camera each frame, the opening announcement, the unit lists, the
/// tile pattern, and the party's battle state and Djinn as a battle starts.
// Part of battle.c.

#include "battle_internal.h"
#include "gamepak/far_call/game.h"
#include "battle/battle.h"
#include "system/system_bank.h"
#include "system/transform.h"
#include "macros.h"

void Transform_SetPosition(s32 *position);

/// Runs each frame of a battle: pauses a link battle whose connection is lost,
/// turns the camera a sixteenth of the way toward its target yaw, places it,
/// and refreshes the background's zoom.
void BattleCamera_Update(void) {
  struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
  struct BattlePresentationTransition *transition =
      Ram_HeapSlots->battleBackground;
  struct FixedPointPosition eyeOffset;
  s32 *focus;
  s16 delta;
  u32 linkId;

  if (Ram_HeapSlots->battleWork->isLink != 0) {
    if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
      Ram_HeapSlots->battleWork->linkMisses++;
      if (Ram_HeapSlots->battleWork->linkMisses > LINK_MAX_MISSED_FRAMES) {
        Ram_HeapSlots->battleWork->linkPaused = 1;
      }
    } else {
      linkId = (REG_SIOCNT << 26) >> 30;
      if (Ram_HeapSlots->battleWork->linkSide != linkId) {
        Ram_HeapSlots->battleWork->linkPaused = 1;
      }
      Ram_HeapSlots->battleWork->linkMisses = 0;
    }
  }

  if (transition->frames != 0) {
    delta = transition->targetYaw - camera->yaw;
    delta /= 16;
    camera->yaw += delta;
    transition->frames--;
  }

  focus = camera->focus;
  if (camera->followPosition != 0) {
    focus = camera->followPosition;
  }

  Transform_RewindStack();
  Transform_SetYaw((s16)camera->yaw);
  Transform_ApplyPitch((s16)camera->pitch);
  Transform_SetPosition(focus);

  SET_POSITION(&eyeOffset, 0, 0, camera->distance);
  Iwram_TransformVector(&eyeOffset.x, (s32 *)camera);

  if (transition->flag == 0) {
    BattleCamera_SetRange(px(120), px(120), 0, 0, Q16_ONE);
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
      MessageArgument_PushFar((u16)*enemy++, MESSAGE_ARGUMENT_ACTOR);
      if (announced == enemyCount - 1)
        BattleMessage_ShowAndWaitFar((s32)MsgLastEnemyAppeared);
      else
        BattleMessage_ShowAndWaitFar((s32)MsgEnemyAppeared);
      announced++;
      BattleMessage_WaitForAdvance();
    } while (announced != enemyCount);
  }

  BattleMessage_CloseFar();
  if (battle->firstStrike == FIRST_STRIKE_PARTY) {
    MessageArgument_ClearAllFar();
    BattleMessage_ShowAndWaitFar((s32)MsgPartyStrikesFirst);
    BattleMessage_WaitForAdvance();
  } else if (battle->firstStrike == FIRST_STRIKE_ENEMIES) {
    MessageArgument_ClearAllFar();
    BattleMessage_ShowAndWaitFar((s32)MsgPartySurprised);
    BattleMessage_WaitForAdvance();
  }
}

/// Fills the battle's two unit lists: the party in its order, and the enemies
/// left to right, or for a fixed formation out from the centre.
void BattleUnit_Collect(void) {
  u16 unitIds[UNIT_MAX];
  struct BattleWork *battle;
  s32 count;
  s32 i;
  s32 position;
  s32 formation;

  battle = Ram_HeapSlots->battleWork;
  count = BattleParty_PrepareMembers(unitIds);
  for (i = 0; i < count; i++)
    battle->partyUnits[i] = unitIds[i];
  battle->partyUnits[count] = BATTLE_UNIT_LIST_END;

  // The shuffled and interleaved formations keep the enemies in unit order; the
  // others put the first in the centre and the rest either side of it.
  count = BattleEnemy_List(unitIds);
  formation = battle->layout;
  if (formation >= FORMATION_LAYOUT_SHUFFLED) {
    if (formation <= FORMATION_LAYOUT_INTERLEAVED) {
      for (i = 0; i < count; i++)
        battle->enemyIds[i] = unitIds[i];
      goto placed;
    }
  }
  for (i = 0; i < count; i++) {
    position = gBattleParty_CenterOrderOffsets[i] + count / 2;
    battle->enemyIds[position] = unitIds[i];
  }
placed:
  battle->enemyIds[count] = BATTLE_UNIT_LIST_END;
}

/// Moves the 16 bytes at VRAM 0x06000290 down to 0x06000280 and clears the
/// 20 bytes from 0x0600028c.
s32 Battle_ShiftVramTiles(void) {
  Dma_Set((const void *)0x06000290, (void *)0x06000280, DMA_COPY_HALFWORDS(8),
          REG_DMA3);
  return Iwram_ClearWordsEntry((void *)0x0600028c, 20);
}

/// An HBlank handler: clears BG0's vertical scroll from the line it fires on.
void BattleTransition_ClearBg0Vofs(void) {
  u32 zero = 0;

  REG_BG0VOFS = zero;
}

/// Does nothing.
void Party_ReservedNoOp5B14(s32 value) {}

/// Clears the party's battle-only states, and works their stats out again, as a
/// battle ends.
s32 BattleParty_ResetActiveRuntimeFields(void) {
  u16 owners[10];
  s32 count;
  s32 i;

  count = BattleParty_PrepareMembers(owners);
  count += BattleParty_PrepareReserveOwners(owners + count);

  i = 0;
  if (i < count) {
    struct Unit *unit;
    u8 *cursor;
    s32 remaining;

    do {
      unit = Unit_Get(owners[i]);
      remaining = 3;
      cursor = (u8 *)&unit->summonBoost[3];

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
      unit->regenerate = 0;
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
  count += BattleUnit_ListIds(2, ids + count);
  if (count > 0) {
    do {
      struct Unit *unit = Unit_Get(*id++);

      unit->defendLevel = 0;
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
          list = &(Djinn_GetRecoveryTableFar(owner > ALLY_MAX))->list;
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
  u8 totals[ELEMENT_COUNT];
  u16 partyMembers[10];
  s32 partySize;
  s32 element;
  u32 availableMask;

  partySize = BattleParty_PrepareMembers(partyMembers);
  availableMask = Djinn_GetRecoveryTableFar(0)->summonMask & 0xffff0000;

  element = 0;
  do {
    totals[element] = 0;
    {
      s32 partySlot;

      for (partySlot = 0; partySlot < partySize; partySlot++) {
        struct Unit *member = Unit_Get(partyMembers[partySlot]);

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

      if (elementsMet == ELEMENT_COUNT)
        availableMask |= 1u << element;
    }
    element++;
  } while (element <= 15);

  Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY)->summonMask = availableMask;
}

/// An empty function, right after BattleSummon_UpdateAvailability.
void BattleSummon_ReservedNoOp(void) {}
