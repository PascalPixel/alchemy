/// @file
/// The battle camera each frame, the opening announcement, the unit lists,
/// and the party's battle state and Djinn as a battle starts and ends.
/// Part of battle.c, the battle object, compiled only through it.

#include "../battle_internal.h"

/// Runs each frame of a battle. In a link battle it first checks the link:
/// a connection lost for more than LINK_MAX_MISSED_FRAMES frames, or a
/// console whose link number changed, pauses the battle. Then it turns the
/// camera a sixteenth of the way toward a transition's yaw, places the camera
/// at its position (or at the one it follows) with its yaw, pitch and
/// distance, and, unless a transition owns the background, refreshes the
/// background's perspective.
void BattlePresentation_UpdateCamera(void) {
  // UB: indexing past scalar gCameraWork reaches other work slots. Named
  // slots add four bytes in the native EN trial (0xf8 to 0xfc).
#ifdef UBFIX
  struct BattleCamera *camera = gCameraWork;
  struct BattlePresentationTransition *transition =
      WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
  struct BattleWork *work = gBattleWork;
#else
  void **slot = (void **)&gCameraWork;
  struct BattleCamera *camera = slot[0];
  struct BattlePresentationTransition *transition =
      slot[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_CAMERA];
  struct BattleWork *work = slot[HEAP_SLOT_BATTLE - HEAP_SLOT_CAMERA];
#endif
  struct FixedPointPosition eyeOffset;
  s32 *focus;
  s16 delta;
  u32 linkId;

  // Treat a lost connection or changed console number as a paused battle.
  if (work->twoSided != 0) {
    if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
      work->linkMisses++;
      if (work->linkMisses > LINK_MAX_MISSED_FRAMES) {
        work->linkPaused = 1;
      }
    } else {
      linkId = SIO_MULTI_PLAYER_ID();
      if (work->linkSide != linkId) {
        work->linkPaused = 1;
      }
      work->linkMisses = 0;
    }
  }

  // Ease the yaw before building the view around its current focus.
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

  Render_ResetTransformState();
  SceneTransform_ApplyPosition(focus);
  SceneTransform_ApplyYaw((s16)camera->yaw);
  SceneTransform_ApplyPitch((s16)camera->pitch);

  // The eye sits distance behind the focus, along the turned view.
  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = camera->distance;
  Iwram_TransformVector(&eyeOffset.x, (s32 *)camera);

  if (transition->flag == 0) {
    BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
  }
}

/// Announces the enemies at the start of a battle, then whether the party
/// strikes first or is caught by surprise. Each of the first enemyCount enemies
/// gets `<enemy> appeared!`, the last one's message a different line.
void BattleIntro_AnnounceEncounter(s32 enemyCount) {
  s16 enemies[8];
  struct BattleWork *battle;
  s16 *enemy;
  s32 announced;

  battle = gBattleWork;
  UiWork_ClearValueNameTablesFar();
  BattleEnemyList(enemies);

  announced = 0;
  // Finish all appearance messages before announcing an opening advantage.
  if (enemyCount != 0) {
    enemy = enemies;
    do {
      UiWork_PushValueSlotFar((u16)*enemy++, VALUE_UNIT_NAME);
      if (announced == enemyCount - 1)
        UiText_ShowMessageAndWaitCoreFar(MsgLastEnemyAppeared);
      else
        UiText_ShowMessageAndWaitCoreFar(MsgEnemyAppeared);
      announced++;
      BattlePresentation_WaitForAdvance();
    } while (announced != enemyCount);
  }

  UiWork_FinalizeSharedSlotFar();
  if (battle->encounterMode == BATTLE_ENCOUNTER_PARTY_FIRST) {
    UiWork_ClearValueNameTablesFar();
    UiWork_PushValueSlotFar(0, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgPartyStrikesFirst);
    BattlePresentation_WaitForAdvance();
  } else if (battle->encounterMode == BATTLE_ENCOUNTER_ENEMIES_FIRST) {
    UiWork_ClearValueNameTablesFar();
    UiWork_PushValueSlotFar(0, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgPartySurprised);
    BattlePresentation_WaitForAdvance();
  }
}

/// Fills the battle's two unit lists, each ended by BATTLE_UNIT_LIST_END:
/// the party members in their order, and the enemies, left to right for a
/// random formation or, for a fixed one, alternating out from the centre
/// (gBattleParty_CenterOrderOffsets).
void BattleParty_CollectUnitList(void) {
  u16 unitIds[14];
  struct BattleWork *battle;
  s32 count;
  s32 i;
  s32 position;
  s32 formation;

  battle = gBattleWork;
  // The party list follows active-owner order regardless of enemy formation.
  count = BattleParty_PrepareActiveOwners(unitIds);
  for (i = 0; i < count; i++)
    battle->partyUnits[i] = unitIds[i];
  battle->partyUnits[count] = BATTLE_UNIT_LIST_END;

  count = BattleEnemyList(unitIds);
  formation = battle->battleType;
  if (formation >= FORMATION_SHUFFLED) {
    if (formation <= FORMATION_INTERLEAVED) {
      for (i = 0; i < count; i++)
        battle->enemyIds[i] = unitIds[i];
      goto done;
    }
  }
  for (i = 0; i < count; i++) {
    position = gBattleParty_CenterOrderOffsets[i] + count / 2;
    battle->enemyIds[position] = unitIds[i];
  }
done:
  battle->enemyIds[count] = BATTLE_UNIT_LIST_END;
}

/// Moves the 16 bytes at VRAM 0x06000290 down to 0x06000280 and clears the
/// 20 bytes from 0x0600028c.
s32 BattlePresentation_InitializeTilePattern(void) {
  Dma_Set((const void *)0x06000290, (void *)0x06000280, DMA_COPY_HALFWORDS(8),
          REG_DMA3);
  return Iwram_ClearWords((void *)0x0600028c, 20);
}

/// Sets BG0's vertical scroll back to 0.
void BattlePresentation_ClearBg0VerticalOffset(void) {
  u32 zero = 0;

  REG_BG0VOFS = zero;
}

/// Does nothing.
void Party_ReservedNoOp5B14(s32 unused) {}

/// Clears every battle-only state of the party members (stat stages and
/// their turns, statuses, Djinn element boosts, the countdowns) and
/// recalculates their stats, as a battle ends.
s32 BattleParty_ResetActiveRuntimeFields(void) {
  u16 owners[10];
  s32 count;
  s32 i;

  count = BattleParty_PrepareActiveOwners(owners);

  i = 0;
  if (i < count) {
    struct BattleUnit *unit;
    u8 *level;
    s32 remaining;

    do {
      unit = UnitGetFar(owners[i]);

      // The four element levels, cleared from the last.
      level = (u8 *)&unit->elementLevels[NUM_ELEMENTS - 1];
      remaining = NUM_ELEMENTS - 1;
      do {
        remaining--;
        *level-- = 0;
      } while (remaining >= 0);

      // Clear temporary battle effects before recalculating persistent stats.
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

      UnitCalcFar(owners[i]);
      i++;
    } while (i < count);
  }
}

/// Brings the Djinn recovery list up to date as a battle ends: every Djinni
/// a party member has on standby that the list lacks joins it. Then, outside
/// a link battle, each Djinni waiting for the battle to end whose owner is
/// no longer in the battle is Set again and leaves the list.
s32 BattleParty_ReconcileDjinnRecovery(void) {
  u16 owners[10];
  s32 count;
  s32 i;
  s32 owner;
  s32 element;
  s32 djinni;

  count = BattleParty_PrepareActiveOwners(owners);

  // Register standby Djinn that are missing from their side's recovery list.
  for (i = 0; i < count; i++) {
    owner = owners[i];
    for (element = 0; element < NUM_ELEMENTS; element++) {
      for (djinni = 0; djinni < DJINN_PER_ELEMENT; djinni++) {
        if (Djinn_IsReadyFar(owner, element, djinni) != 0) {
          struct DjinnRecoveryList *list =
              &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
                    (owner < 0 || owner > 7) ? DJINN_RECOVERY_OPPONENT
                                             : DJINN_RECOVERY_PARTY))
                   ->list;
          s32 j;

          for (j = 0; j < list->count; j++) {
            if (element == list->entries[j].element &&
                djinni == list->entries[j].index)
              break;
          }
          if (j == list->count)
            DjinnRecovery_AddStandbyFar(owner, element, djinni);
        }
      }
    }
  }

  // Link battles keep their recovery lists; only ordinary battles reset them.
  if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0)
    return;

  {
    struct DjinnRecoveryList *list =
        &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
              DJINN_RECOVERY_PARTY))
             ->list;
    struct DjinnRecoveryEntry *entry;

    i = 0;
    if (i < list->count) {
      s32 afterBattle = DJINN_RECOVERY_AFTER_BATTLE;

      entry = list->entries;
      do {
        if (entry->turns == afterBattle && BattleObjGet(entry->unitId) == 0) {
          u8 id = entry->unitId;
          u8 element = entry->element;
          u8 index = entry->index;

          Djinn_ActivateFar(id, element, index);
          DjinnRecovery_RemoveFar(id, element, index);
        }
        i++;
        entry++;
      } while (i < list->count);
    }
  }
}

#define NUM_SUMMONS 32

/// Works out which summons the party can call: one whose Djinn requirement
/// of every element is met by the party's Djinn of that element, all
/// members together. Stores the mask of them (bit n for summon n) and
/// returns the state that holds it.
struct BattleSummonState *BattleSummon_UpdateAvailability(void) {
  u8 totals[NUM_ELEMENTS];
  u16 partyMembers[10];
  s32 partySize;
  s32 element;
  u32 availableMask;

  partySize = BattleParty_PrepareActiveOwners(partyMembers);
  availableMask = 0;

  // The party's Djinn of each element.
  element = 0;
  do {
    totals[element] = 0;
    {
      s32 partySlot;

      for (partySlot = 0; partySlot < partySize; partySlot++) {
        struct BattleUnit *member = UnitGetFar(partyMembers[partySlot]);

        totals[element] += member->djinnOwnedCounts[element];
      }
    }
    element++;
  } while (element < NUM_ELEMENTS);

  // A summon is available when all four elements meet its requirement.
  // The element counter is reused here to walk the summon IDs.
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
          if (elementsMet >= NUM_ELEMENTS)
            break;
          total++;
          required++;
        } while (*total >= *required);
      }

      if (elementsMet == NUM_ELEMENTS)
        availableMask |= 1u << element;
    }
    element++;
  } while (element < NUM_SUMMONS);

  {
    struct BattleSummonState *state =
        Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY);
    state->availableMask = availableMask;
    return state;
  }
}

/// Does nothing.
void BattleSummon_ReservedNoOp(void) {}
