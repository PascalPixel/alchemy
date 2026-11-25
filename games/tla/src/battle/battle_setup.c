/// @file
/// The battle camera each frame, the opening announcement, the unit lists, the
/// tile pattern, and the party's battle state and Djinn as a battle starts.
// Part of battle.c.

#include "battle_internal.h"
#include "gamepak/far_call/game.h"
#include "battle/battle.h"
#include "system/system.h"
#include "system/matrix.h"
#include "macros.h"

void Matrix_SetTranslation(FIXED *position);

/// Each frame of a turn the camera closes this share of the gap to its target
/// yaw, one sixteenth, so it slows as it nears it.
#define CAMERA_YAW_EASE 16

/// Runs each frame of a battle: pauses a link battle whose connection is lost,
/// turns the camera a sixteenth of the way toward its target yaw, places it,
/// and refreshes the background's zoom.
void BattleCamera_Update(void) {
  struct BattleCamera *camera = Ram_HeapSlots->cameraState;
  struct BattleBackgroundState *background = Ram_HeapSlots->battleBackground;
  struct Point eyeOffset;
  s32 *focus;
  s16 delta;
  u32 linkId;

  // The link check: a missed frame counts only while the link is down, and
  // any frame with it up resets the count.
  if (Ram_HeapSlots->battleState->isLink != 0) {
    if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
      Ram_HeapSlots->battleState->linkMisses++;
      if (Ram_HeapSlots->battleState->linkMisses > LINK_MAX_MISSED_FRAMES) {
        Ram_HeapSlots->battleState->linkPaused = 1;
      }
    } else {
      linkId = SIO_ID_OF(REG_SIOCNT);
      if (Ram_HeapSlots->battleState->linkSide != linkId) {
        Ram_HeapSlots->battleState->linkPaused = 1;
      }
      Ram_HeapSlots->battleState->linkMisses = 0;
    }
  }

  if (background->frames != 0) {
    delta = background->targetYaw - camera->yaw;
    delta /= CAMERA_YAW_EASE;
    // The step is added to the yaw read as an unsigned halfword.
    camera->yaw = (u16)camera->yaw + delta;
    background->frames--;
  }

  focus = camera->focus;
  if (camera->followPosition != 0) {
    focus = camera->followPosition;
  }

  Matrix_RewindStack();
  Matrix_SetRotateY(camera->yaw);
  Matrix_RotateX(camera->pitch);
  Matrix_SetTranslation(focus);

  // The eye sits distance behind the focus, along the turned view.
  SET_POSITION(&eyeOffset, 0, 0, camera->distance);
  Iwram_TransformVector(&eyeOffset.x, (s32 *)camera);

  if (background->keepRange == 0) {
    BattleCamera_SetRange(toFIXED(120), toFIXED(120), 0, 0, FIXED_ONE);
  }
}

/// Announces the enemies at the start of a battle, "<enemy> appeared!" for
/// each, then whether the party strikes first or is caught by surprise.
void BattleIntro_AnnounceEncounter(s32 enemyCount) {
  s16 enemies[8];
  struct BattleState *battle;
  s16 *enemy;
  s32 announced;

  battle = Ram_HeapSlots->battleState;
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
  struct BattleState *battle;
  s32 count;
  s32 i;
  s32 position;
  s32 formation;

  battle = Ram_HeapSlots->battleState;
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
    position = gBattleFormation_CenteredOffsets[i] + count / 2;
    battle->enemyIds[position] = unitIds[i];
  }
placed:
  battle->enemyIds[count] = BATTLE_UNIT_LIST_END;
}

/// Unused: moves 16 bytes of VRAM down by 16 and clears the 20 from 12 bytes
/// past where they land.
UNUSED s32 Battle_ShiftVramTiles(void) {
  // XXX (no evidence): what the bytes moved are, tiles of character block 0 or
  // entries of screen block 0; nothing calls this in either game.
  Dma_Set((const void *)0x06000290, (void *)0x06000280, DMA_COPY_HALFWORDS(8),
          REG_DMA3);
  return Iwram_ClearWords((void *)0x0600028c, 20);
}

/// Unused: clears BG0's vertical scroll.
UNUSED void Battle_ClearBg0Vofs(void) { Io_Write16(0, &REG_BG0VOFS); }

/// Does nothing; Battle_Run calls it with 1 as the battle is set up.
void Party_BattleSetupNoOp(s32 unused) {}

/// Clears the party's battle-only statuses, and works their stats out again, as
/// a battle ends.
UNREAD_RESULT BattleParty_ClearBattleStatuses(void) {
  u16 owners[10];
  s32 count;
  s32 i;

  count = BattleParty_PrepareMembers(owners);
  count += BattleParty_PrepareReserveOwners(owners + count);

  for (i = 0; i < count; i++) {
    struct Unit *unit = Unit_Get(owners[i]);
    s32 element;

    for (element = 0; element < ELEMENT_COUNT; element++)
      unit->summonBoost[element] = 0;

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
  count += BattleUnit_ListIds(BATTLE_SIDE_MASK_ENEMIES, ids + count);
  if (count > 0) {
    do {
      struct Unit *unit = Unit_Get(*id++);

      unit->defendLevel = 0;
      unit->immobile = 0;
      count--;
    } while (count != 0);
  }
}

/// As a battle starts, lists every Djinni a fighting party member holds unset
/// that its side's Djinn recovery list lacks, on standby: the party's on the
/// party's list, a link opponent's (past ALLY_MAX) on the opponent's.
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
    for (element = 0; element < ELEMENT_COUNT; element++) {
      for (djinn = 0; djinn < DJINN_PER_ELEMENT; djinn++) {
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

/// The summon mask's bits of the summons from SUMMON_ZAGAN on, which the party
/// learns ("can now summon") rather than earns by its Djinn:
/// BattleSummon_UpdateAvailability keeps them as they are.
#define SUMMON_MASK_LEARNED (~0u << SUMMON_ZAGAN)

/// Works out which of the summons up to SUMMON_THOR the party can call: those
/// whose Djinn of every element the party holds (djinnHeldCounts, set or not),
/// all members together, and stores them in the party's summon mask beside
/// the ones learned. Whether enough are on standby is checked as
/// a summon is chosen (BattleSummonMenu_Run) and built
/// (BattleActionRun_Build). Battle_Run calls it as the battle is set up and at
/// the start of each turn.
void BattleSummon_UpdateAvailability(void) {
  u8 totals[ELEMENT_COUNT];
  u16 partyMembers[10];
  s32 partySize;
  s32 i; // an element, then a summon
  u32 availableMask;

  partySize = BattleParty_PrepareMembers(partyMembers);
  availableMask = Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY)->summonMask &
                  SUMMON_MASK_LEARNED;

  for (i = 0; i < ELEMENT_COUNT; i++) {
    totals[i] = 0;
    {
      s32 partySlot;

      for (partySlot = 0; partySlot < partySize; partySlot++) {
        struct Unit *member = Unit_Get(partyMembers[partySlot]);

        totals[i] += member->djinnHeldCounts[i];
      }
    }
  }

  for (i = 0; i <= SUMMON_THOR; i++) {
    const struct SummonDefinition *summon = SummonDefinition_Get(i);
    s32 elementsMet;

    if (summon != 0) {
      const u8 *required = summon->djinnRequired;

      elementsMet = 0;
      if (totals[0] >= required[0]) {
        u8 *total = totals;

        do {
          elementsMet++;
          if (elementsMet >= ELEMENT_COUNT)
            break;
          total++;
          required++;
        } while (*total >= *required);
      }

      if (elementsMet == ELEMENT_COUNT)
        availableMask |= 1u << i;
    }
  }

  Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY)->summonMask = availableMask;
}

/// Unused: does nothing.
UNUSED void BattleSummon_ReservedNoOp(void) {}
