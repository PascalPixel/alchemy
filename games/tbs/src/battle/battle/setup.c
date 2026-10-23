/// @file
/// The battle camera each frame, the opening announcement, the unit lists,
/// and the party's battle state and Djinn as a battle starts and ends.
// Part of battle.c.

#include "../battle_internal.h"

/// Runs each frame of a battle. In a link battle it first checks the link:
/// a connection lost for more than LINK_MAX_MISSED_FRAMES frames, or a
/// console whose link number changed, pauses the battle. Then it turns the
/// camera a sixteenth of the way toward a transition's yaw, places the camera
/// at its position (or at the one it follows) with its yaw, pitch and
/// distance, and, unless a transition owns the background, refreshes the
/// background's perspective. Runs as a scheduler callback that
/// Battle_RunEncounter adds with order 0xc7f: in the main-loop pass, after
/// the tasks of order 0xc80 (the objects' camera, the sprite priorities and
/// event playback). Changes battle->linkMisses and linkPaused,
/// the camera's yaw and the transition's frame count, the transform state,
/// and through BattleCamera_SetRange the background's affine page.
void BattlePresentation_UpdateCamera(void) {
  // UB: indexing past scalar gCameraWork reaches other work slots. Named
  // slots add four bytes in the native EN trial (0xf8 to 0xfc).
#ifdef UBFIX
  struct BattleCamera *camera = gCameraWork;
  struct BattlePresentationTransition *transition =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  struct BattleWork *battle = gBattleWork;
#else
  void **slot = (void **)&gCameraWork;
  struct BattleCamera *camera = slot[0];
  struct BattlePresentationTransition *transition =
      slot[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_CAMERA];
  struct BattleWork *battle = slot[HEAP_SLOT_BATTLE - HEAP_SLOT_CAMERA];
#endif
  struct FixedPointPosition eyeOffset;
  s32 *focus;
  s16 delta;
  u32 linkId;

  // The link check: a missed frame counts only while the link is down, and
  // any frame with it up resets the count.
  if (battle->twoSided != 0) {
    if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
      battle->linkMisses++;
      if (battle->linkMisses > LINK_MAX_MISSED_FRAMES) {
        battle->linkPaused = 1;
      }
    } else {
      linkId = SIO_MULTI_PLAYER_ID();
      if (battle->linkSide != linkId) {
        battle->linkPaused = 1;
      }
      battle->linkMisses = 0;
    }
  }

  // Each frame of a turn closes a sixteenth of the gap that remains, so the
  // camera slows as it nears targetYaw.
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

  Render_ResetTransformState();
  Transform_ApplyPosition(focus);
  Transform_ApplyYaw((s16)camera->yaw);
  Transform_ApplyPitch((s16)camera->pitch);

  // The eye sits distance behind the focus, along the turned view.
  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = camera->distance;
  Iwram_TransformVector(&eyeOffset.x, (s32 *)camera);

  // The view centred at (120, 120) in 16.16 pixels, at scale 1.
  if (transition->flag == 0) {
    BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
  }
}

/// Announces the enemies at the start of a battle, then whether the party
/// strikes first or is caught by surprise. Each of the first enemyCount enemies
/// gets `<enemy> appeared!`; the last one's comes from a message of its own,
/// MsgLastEnemyAppeared, whose text matches MsgEnemyAppeared's in every
/// edition. Called by BattlePresentation_RunEntryTransition. Rewrites the
/// message argument tables.
void BattleIntro_AnnounceEncounter(s32 enemyCount) {
  s16 enemies[8];
  struct BattleWork *battle;
  s16 *enemy;
  s32 announced;

  battle = gBattleWork;
  UiWork_ClearValueNameTablesFar();
  BattleEnemy_List(enemies);

  announced = 0;
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
  // Unit 0's name stands for the party in either message.
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
/// (gBattleParty_CenterOrderOffsets). Called by Battle_RunEncounter as the
/// battle is set up. Writes gBattleWork->partyUnits and enemyIds, and marks
/// each party member's side.
void BattleParty_CollectUnitList(void) {
  u16 unitIds[14];
  struct BattleWork *battle;
  s32 count;
  s32 i;
  s32 position;
  s32 formation;

  battle = gBattleWork;
  count = BattleParty_PrepareActiveOwners(unitIds);
  for (i = 0; i < count; i++)
    battle->partyUnits[i] = unitIds[i];
  battle->partyUnits[count] = BATTLE_UNIT_LIST_END;

  // The shuffled and interleaved formations (a link battle's included)
  // keep the enemies in unit order; the others place the first enemy in
  // the centre and the rest alternately either side of it.
  count = BattleEnemy_List(unitIds);
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
/// 20 bytes from 0x0600028c. Nothing in the source calls it.
/// XXX: what the tiles at 0x06000280 are; a caller in another image, or the
/// picture loaded there, would settle it.
s32 BattlePresentation_InitializeTilePattern(void) {
  Dma_Set((const void *)0x06000290, (void *)0x06000280, DMA_COPY_HALFWORDS(8),
          REG_DMA3);
  return Iwram_ClearWords((void *)0x0600028c, 20);
}

/// Nothing in the source calls it.
void BattlePresentation_ClearBg0VerticalOffset(void) {
  u32 zero = 0;

  REG_BG0VOFS = zero;
}

/// Does nothing; Battle_RunEncounter calls it with 1 as the battle is set
/// up.
void Party_ReservedNoOp5B14(s32 unused) {}

/// Clears every battle-only state of the party members (stat stages and
/// their turns, statuses, Djinn element boosts, the countdowns) and
/// recalculates their stats, as a battle ends. Called by Battle_RunEncounter
/// on every way out of the battle.
#ifdef UBFIX
void BattleParty_ResetActiveRuntimeFields(void) {
#else
s32 BattleParty_ResetActiveRuntimeFields(void) {
#endif
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
      unit = Unit_GetFar(owners[i]);

      // The four element levels, cleared from the last.
      level = (u8 *)&unit->elementLevels[ELEMENT_COUNT - 1];
      remaining = ELEMENT_COUNT - 1;
      do {
        remaining--;
        *level-- = 0;
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

/// Brings the Djinn recovery list up to date as a battle starts: every
/// Djinni a party member has on standby that the list lacks joins it. Then,
/// outside a link battle, each Djinni waiting for a battle to end whose
/// owner has no battle object is Set again and leaves the list. Called by
/// Battle_RunEncounter once the unit lists and battle objects are in place.
/// Changes the party's and the opponent's recovery lists and Djinn states.
#ifdef UBFIX
void BattleParty_ReconcileDjinnRecovery(void) {
#else
s32 BattleParty_ReconcileDjinnRecovery(void) {
#endif
  u16 owners[10];
  s32 count;
  s32 i;
  s32 owner;
  s32 element;
  s32 djinni;

  count = BattleParty_PrepareActiveOwners(owners);

  // Owners 0 to 7 use the party's list, any other the opponent's.
  for (i = 0; i < count; i++) {
    owner = owners[i];
    for (element = 0; element < ELEMENT_COUNT; element++) {
      for (djinni = 0; djinni < DJINN_PER_ELEMENT; djinni++) {
        if (Djinn_IsReadyFar(owner, element, djinni) != 0) {
          struct DjinnRecoveryList *list =
              &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
                    (owner < 0 || owner > ALLY_MAX) ? DJINN_RECOVERY_OPPONENT
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

  // Link battles leave the Djinn waiting on the list.
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
        if (entry->turns == afterBattle &&
            BattleObject_Get(entry->unitId) == 0) {
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

/// The summon ids the mask covers, one bit each.
#define SUMMON_COUNT 32

/// Works out which summons the party can call: one whose Djinn requirement
/// of every element is met by the Djinn the party holds of that element
/// (djinnOwnedCounts, set or not), all members together. Whether enough of
/// them are on standby is checked as the summon is chosen
/// (SummonMenu_SelectSummon, Battle_CollectPartyCommands) and again when it
/// is planned (BattleCommand_BuildPlan). Stores the mask of them (bit n for
/// summon n) and returns the state that holds it. Called by
/// Battle_RunEncounter as the battle is set up and at the start of each
/// round, which asks for the party status window's standby Djinn column
/// (PARTY_STATUS_SUMMONS) while the mask is not 0; PartyStatus_Draw shows it
/// only while a Djinni is on standby. Writes the party recovery table's
/// availableMask.
struct BattleSummonState *BattleSummon_UpdateAvailability(void) {
  u8 totals[ELEMENT_COUNT];
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
        struct BattleUnit *member = Unit_GetFar(partyMembers[partySlot]);

        totals[element] += member->djinnOwnedCounts[element];
      }
    }
    element++;
  } while (element < ELEMENT_COUNT);

  // element walks the summon ids here. The inner loop stops at the first
  // element whose requirement is not met.
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
          if (elementsMet >= ELEMENT_COUNT)
            break;
          total++;
          required++;
        } while (*total >= *required);
      }

      if (elementsMet == ELEMENT_COUNT)
        availableMask |= 1u << element;
    }
    element++;
  } while (element < SUMMON_COUNT);

  {
    struct BattleSummonState *state =
        Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY);
    state->availableMask = availableMask;
    return state;
  }
}

/// Does nothing; nothing in the source calls it.
void BattleSummon_ReservedNoOp(void) {}
