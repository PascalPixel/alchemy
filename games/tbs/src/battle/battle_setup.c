/// @file
/// The battle camera each frame, the opening announcement, the unit lists,
/// and the party's battle state and Djinn as a battle starts and ends.
// Part of battle.c.

#include "battle_internal.h"

/// Each frame of a turn the camera closes this share of the gap to its target
/// yaw, one sixteenth, so it slows as it nears it.
#define CAMERA_YAW_EASE 16

/// Runs each frame of a battle: pauses a link battle whose connection is lost,
/// turns the camera a sixteenth of the way toward its target yaw, places it,
/// and refreshes the background's zoom.
void BattleCamera_Update(void) {
  void **slots = &gHeapSlots[HEAP_SLOT_CAMERA];
  struct BattleCamera *camera = slots[0];
  struct BattleBackgroundState *view =
      HEAP_SLOT_FROM(slots, HEAP_SLOT_CAMERA, HEAP_SLOT_BATTLE_BACKGROUND);
  struct BattleState *battle =
      HEAP_SLOT_FROM(slots, HEAP_SLOT_CAMERA, HEAP_SLOT_BATTLE);
  struct Point eyeOffset;
  s32 *focus;
  s16 delta;

  // The link check: a missed frame counts only while the link is down, and
  // any frame with it up resets the count.
  if (battle->isLink != 0) {
    if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
      battle->linkMisses++;
      if (battle->linkMisses > LINK_MAX_MISSED_FRAMES) {
        battle->linkPaused = 1;
      }
    } else {
      u32 linkId = SIO_MULTI_PLAYER_ID();
      if (battle->linkSide != linkId) {
        battle->linkPaused = 1;
      }
      battle->linkMisses = 0;
    }
  }

  if (view->frames != 0) {
    delta = view->targetYaw - camera->yaw;
    delta /= CAMERA_YAW_EASE;
    camera->yaw += delta;
    view->frames--;
  }

  focus = camera->focus;
  if (camera->followPosition != 0) {
    focus = camera->followPosition;
  }

  Matrix_Initialize();
  Matrix_TranslateVector(focus);
  Matrix_RotateY((s16)camera->yaw);
  Matrix_RotateX((s16)camera->pitch);

  // The eye sits distance behind the focus, along the turned view.
  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = camera->distance;
  Iwram_TransformVector(&eyeOffset.x, (s32 *)camera);

  // The plane centred on the projection's centre, at a zoom of 1.
  if (view->keepRange == 0) {
    BattleCamera_SetRange(toFIXED(BATTLE_PROJECTION_CENTER_X),
                          toFIXED(BATTLE_PROJECTION_CENTER_Y), 0, 0, FIXED_ONE);
  }
}

/// Announces the enemies at the start of a battle, "<enemy> appeared!" for
/// each, then whether the party strikes first or is caught by surprise.
void BattleIntro_AnnounceEncounter(s32 enemyCount) {
  s16 enemies[8];
  struct BattleState *battle = gBattleState;
  s32 i;

  MessageArgument_ClearAllFar();
  BattleEnemy_List(enemies);
  for (i = 0; i != enemyCount; i++) {
    MessageArgument_PushFar((u16)enemies[i], MESSAGE_ARGUMENT_ACTOR);
    if (i == enemyCount - 1)
      BattleMessage_ShowAndWaitFar(MsgLastEnemyAppeared);
    else
      BattleMessage_ShowAndWaitFar(MsgEnemyAppeared);
    BattleMessage_WaitForAdvance();
  }

  BattleMessage_CloseFar();
  // Unit 0's name stands for the party in either message.
  if (battle->firstStrike == FIRST_STRIKE_PARTY) {
    MessageArgument_ClearAllFar();
    MessageArgument_PushFar(0, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar(MsgPartyStrikesFirst);
    BattleMessage_WaitForAdvance();
  } else if (battle->firstStrike == FIRST_STRIKE_ENEMIES) {
    MessageArgument_ClearAllFar();
    MessageArgument_PushFar(0, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar(MsgPartySurprised);
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

  battle = gBattleState;
  count = BattleParty_PrepareMembers(unitIds);
  for (i = 0; i < count; i++)
    battle->partyUnits[i] = unitIds[i];
  battle->partyUnits[count] = BATTLE_UNIT_LIST_END;

  // The shuffled and interleaved formations keep the enemies in unit order; the
  // others put the first in the centre and the rest either side of it.
  count = BattleEnemy_List(unitIds);
  formation = battle->layout;
  switch (formation) {
  case FORMATION_LAYOUT_SHUFFLED:
  case FORMATION_LAYOUT_INTERLEAVED:
    for (i = 0; i < count; i++)
      battle->enemyIds[i] = unitIds[i];
    break;
  case FORMATION_LAYOUT_CENTERED:
  default:
    for (i = 0; i < count; i++) {
      position = gBattleFormation_CenteredOffsets[i] + count / 2;
      battle->enemyIds[position] = unitIds[i];
    }
    break;
  }
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

/// Clears the party's battle-only states, and works their stats out again, as a
/// battle ends.
UNREAD_RESULT BattleParty_ClearBattleStatuses(void) {
  u16 unitIds[10];
  s32 count;
  s32 i;

  count = BattleParty_PrepareMembers(unitIds);

  for (i = 0; i < count; i++) {
    struct Unit *unit = Unit_GetFar(unitIds[i]);
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

    Unit_CalculateFar(unitIds[i]);
  }
}

/// Brings the Djinn recovery list up to date as a battle starts, and outside a
/// link battle sets again each Djinni waiting for a battle's end.
UNREAD_RESULT DjinnRecovery_Reconcile(void) {
  u16 unitIds[10];
  s32 count;
  s32 i;
  s32 unitId;
  s32 element;
  s32 djinni;

  count = BattleParty_PrepareMembers(unitIds);

  // A party unit (0 to ALLY_MAX) uses the party's list, any other the
  // opponent's.
  for (i = 0; i < count; i++) {
    unitId = unitIds[i];
    for (element = 0; element < ELEMENT_COUNT; element++) {
      for (djinni = 0; djinni < DJINN_PER_ELEMENT; djinni++) {
        if (Djinn_CanSetFar(unitId, element, djinni) != 0) {
          struct DjinnRecoveryList *list =
              &Djinn_GetRecoveryTableFar((unitId < 0 || unitId > ALLY_MAX)
                                             ? DJINN_RECOVERY_OPPONENT
                                             : DJINN_RECOVERY_PARTY)
                   ->list;
          s32 j;

          for (j = 0; j < list->count; j++) {
            if (element == list->entries[j].element &&
                djinni == list->entries[j].index)
              break;
          }
          if (j == list->count)
            DjinnRecovery_AddStandbyFar(unitId, element, djinni);
        }
      }
    }
  }

  // Link battles leave the Djinn waiting on the list.
  if (GameFlag_GetFar(FLAG_LINK_BATTLE) != 0)
    return;

  {
    struct DjinnRecoveryList *list =
        &Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY)->list;

    for (i = 0; i < list->count; i++) {
      struct DjinnRecoveryEntry *entry = &list->entries[i];

      if (entry->turns == DJINN_RECOVERY_AFTER_BATTLE &&
          BattleObject_Get(entry->unitId) == 0) {
        u8 id = entry->unitId;
        u8 element = entry->element;
        u8 index = entry->index;

        Djinn_SetFar(id, element, index);
        DjinnRecovery_RemoveFar(id, element, index);
      }
    }
  }
}

/// The summon mask's bits, one a summon id; SummonDefinition_Get gives NULL for
/// the ids past the last summon.
#define SUMMON_MASK_BITS 32

/// Works out which summons the party can call, by the Djinn it holds of each
/// element, set or not; whether enough are on standby is checked when one is
/// chosen.
struct DjinnRecoveryTable *BattleSummon_UpdateAvailability(void) {
  u8 totals[ELEMENT_COUNT];
  u16 partyMembers[10];
  s32 partySize;
  s32 element;
  u32 availableMask;

  partySize = BattleParty_PrepareMembers(partyMembers);
  availableMask = 0;

  // The party's Djinn of each element.
  for (element = 0; element < ELEMENT_COUNT; element++) {
    totals[element] = 0;
    {
      s32 partySlot;

      for (partySlot = 0; partySlot < partySize; partySlot++) {
        struct Unit *member = Unit_GetFar(partyMembers[partySlot]);

        totals[element] += member->djinnHeldCounts[element];
      }
    }
  }

  // Then element counts the summons; the inner loop stops at the first element
  // whose requirement is not met.
  for (element = 0; element < SUMMON_MASK_BITS; element++) {
    const struct SummonDefinition *summon = SummonDefinition_Get(element);
    s32 elementsMet;

    if (summon != 0) {
      const u8 *required = summon->djinnRequired;

      elementsMet = 0;
      if (totals[0] >= required[0]) {
        const u8 *total = totals;

        do {
          if (++elementsMet >= ELEMENT_COUNT)
            break;
          total++;
          required++;
        } while (*total >= *required);
      }

      if (elementsMet == ELEMENT_COUNT)
        availableMask |= 1u << element;
    }
  }

  {
    struct DjinnRecoveryTable *table =
        Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY);
    table->summonMask = availableMask;
    return table;
  }
}

/// Unused: does nothing.
UNUSED void BattleSummon_ReservedNoOp(void) {}

#undef CAMERA_YAW_EASE
#undef SUMMON_MASK_BITS
