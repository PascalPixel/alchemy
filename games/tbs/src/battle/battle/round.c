/// @file
/// Part of battle.c, the battle object, compiled only through it.
/// A round's actions: the party's commands, the enemies' or the link
/// opponent's, and their turn order.

#include "../battle_internal.h"

/// Background palette 14, which takes palette 15 brightened.
#define BG_PALETTE_14 ((u16 *)&BG_PLTT_COLOR(14, 0))

/// Pulses BG palette 14's colours: each takes the matching colour of
/// palette 15 brightened by up to twelve steps, rising and falling with the
/// frame count.
void Palette_UpdatePulseBrightness(void) {
  u16 *palette;
  s32 remainingColors;
  s32 channelMask;

  palette = BG_PALETTE_14;
  channelMask = 31;
  remainingColors = 15;
  do {
    u32 color;
    s32 brightness;
    u32 frameCount;
    s32 angle;
    s32 blue;
    s32 green;
    s32 red;

    frameCount = gFrameCount;
    angle = (frameCount * 3U) << 10;
    color = palette[16];
    brightness = (0x10000 - Trig_Cos(angle)) / 0x2AAA;

    blue = (color >> 10) & channelMask;
    green = (color >> 5) & channelMask;
    red = color & channelMask;
    blue += brightness;
    green += brightness;
    red += brightness;

    if (blue < 0 || blue > 31)
      blue = 31;
    if (green < 0 || green > 31)
      green = 31;
    if (red < 0 || red > 31)
      red = 31;

    *palette++ = (blue << 10) | (green << 5) | red;
    remainingColors--;
  } while (remainingColors >= 0);
}

/// Sets the camera up for a scene drawn over the battle (the summons'):
/// a fixed view raised by pos pixels, with its projection and the
/// background's perspective to match.
void Camera_ConfigureScene(s32 verticalOffset) {
  // UB: indexing past scalar gCameraWork reaches another work slot. Named
  // slots add four bytes in the native EN trial (0xd8 to 0xdc).
#ifdef UBFIX
  struct BattleCamera *camera = gCameraWork;
  struct BattlePresentationTransition *transition =
      WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
#else
  void **slots = (void **)&gCameraWork;
  struct BattleCamera *camera = slots[0];
  struct BattlePresentationTransition *transition =
      slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_CAMERA];
#endif
  struct FixedPointPosition eyeOffset;
  u32 result;

  camera->pos[1] = 160 << 11;
  camera->pos[0] = 0;
  camera->pos[2] = 0;
  transition->targetYaw = 128 << 7;
  camera->yaw = 128 << 7;
  camera->pitch = 244 << 8;
  camera->followPos = 0;
  camera->distance = 0x02ee0000;
  camera->unk18 = 0;

  Render_ResetTransformState();
  SceneTransform_ApplyPosition(&camera->pos[0]);
  SceneTransform_ApplyYaw((s16)camera->yaw);
  SceneTransform_ApplyPitch((s16)camera->pitch);

  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = camera->distance;
  Iwram_TransformVector(&eyeOffset, (s32 *)camera);

  result = Iwram_RatioMulQ14(0x03c90000, 192 << 8);
  Projection_Set(0, result, 0x07920000);

  gProjection.centerY = verticalOffset + 120;
  transition->active = 1;
  BattleCamera_SetRange(240 << 15, (0x76 - verticalOffset) << 16, 0, 128 << 4,
                        128 << 10);
  transition->flag = 1;
  transition->active = 0;
}

/// Ends every unit's guard (Defend and the like) and recalculates its stats.
void BattleUnit_ResetGuardLevels(void) {
  u16 unitIds[14];
  s32 count;
  s32 unitIndex;

  count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, unitIds);
  for (unitIndex = 0; unitIndex < count; unitIndex++) {
    struct BattleUnit *unit;

    unit = UnitGetFar(unitIds[unitIndex]);
    unit->guardLevel = 0;
    UnitCalcFar(unitIds[unitIndex]);
  }
}

/// gGameState.battleStart of a battle the party cannot run from.
#define BATTLE_START_NO_ESCAPE 2

/// Rolls whether the party gets away. The chance, in hundredths of a
/// percent, is 5000 plus 2000 for each attempt that failed, plus 500 times
/// the party's average level, less 500 times the enemies' average level. A
/// party that struck first always gets away; no one gets away from a battle
/// that forbids it. Every attempt counts toward the next. Returns 1 when
/// the party gets away.
s32 BattleEscape_CheckSuccess(void) {
  s32 escaped;
  u8 *failedAttempts;
  s16 livingUnits[14];
  s32 livingCount;
  s32 levelTotal;
  s32 unitIndex;
  s32 chance;
  struct BattleWork *escapeState;

  escaped = 0;
  escapeState = gBattleWork;
  if (escapeState->encounterMode == BATTLE_ENCOUNTER_PARTY_FIRST) {
    escaped = 1;
  } else {
    failedAttempts = &escapeState->escapeFailures;
    chance = 5000 + (escapeState->escapeFailures * 2000);
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, livingUnits);
    levelTotal = 0;
    for (unitIndex = escaped; unitIndex < livingCount; unitIndex++) {
      levelTotal += UnitGetFar(livingUnits[unitIndex])->level;
    }
    chance += levelTotal * 500 / livingCount;
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, livingUnits);
    levelTotal = 0;
    for (unitIndex = 0; unitIndex < livingCount; unitIndex++) {
      levelTotal += UnitGetFar(livingUnits[unitIndex])->level;
    }
    chance -= levelTotal * 500 / livingCount;
    if ((chance > 0) && (((10000 * Random16()) >> 0x10) < chance)) {
      escaped = 1;
    }
    *failedAttempts += 1;
  }
  if (gGameState.battleStart == BATTLE_START_NO_ESCAPE) {
    escaped = 0;
  }
  return escaped;
}

/// The party's actions for the round, from the command menu: a member who
/// is asleep, confused, charmed or stunned loses each of their turns
/// (CMD_NONE); the others choose theirs (Battle_CollectPartyCommandsFar).
/// Returns how many actions there are, or -1 if the menu gave up.
s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *entries) {
  u16 *choosingUnits = Runtime_BumpAllocateAlternatePool(17);
  u16 *unitIds = Runtime_BumpAllocateAlternatePool(9);
  s32 unitCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, unitIds);
  s32 choosingCount = 0;
  s32 entryCount = 0;
  s32 unitIndex;

  for (unitIndex = 0; unitIndex < unitCount; unitIndex++) {
    struct BattleUnit *unit = UnitGetFar(unitIds[unitIndex]);
    s32 copyIndex;

    for (copyIndex = 0; copyIndex < unit->actionEntryCount; copyIndex++) {
      // These conditions suppress a turn rather than opening the command menu.
      if (unit->sleep != 0 || unit->confusion != 0 || unit->charm != 0 ||
          unit->stun != 0) {
        struct BattleActionRecord *entry = &entries[entryCount];
        entry->unitId = unitIds[unitIndex];
        entry->priority = unit->agi;
        entry->command = CMD_NONE;
        entry->parameter = 0;
        entry->target = TARGET_NONE_PARTY;
        entryCount++;
      } else {
        choosingUnits[choosingCount++] = unitIds[unitIndex];
      }
    }
  }

  {
    s32 appended;

    entries += entryCount;
    appended =
        Battle_CollectPartyCommandsFar(entries, choosingUnits, choosingCount);
    if (appended < 0) {
      unitCount = -1;
    } else {
      unitCount = entryCount + appended;
    }
    MemFree(unitIds);
    MemFree(choosingUnits);
    return unitCount;
  }
}

/// The enemies' actions for the round, in a shuffled order, each chosen by
/// BattleCommand_SelectAutomatic; a sleeping or stunned enemy loses its
/// turns. An enemy acting more than once a turn acts later at half its
/// Agility. None act when the party struck first; when the party is caught
/// by surprise, only two to five enemies act, once each. Returns how many
/// actions there are.
s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *entries) {
  u16 unitIds[14];
  s32 entryCount = 0;
  struct BattleWork *battle = gBattleWork;
  s32 unitCount;
  s32 shuffleOrUnitIndex;

  if (battle->encounterMode == BATTLE_ENCOUNTER_PARTY_FIRST) {
    return 0;
  }

  unitCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, unitIds);
  if (unitCount == 0) {
    return 0;
  }

  for (shuffleOrUnitIndex = 31; shuffleOrUnitIndex >= 0; shuffleOrUnitIndex--) {
    u32 first = (unitCount * Random16()) >> 16;
    u32 second = (unitCount * Random16()) >> 16;
    s32 swap = unitIds[first];
    unitIds[first] = unitIds[second];
    unitIds[second] = swap;
  }

  if (battle->encounterMode == BATTLE_ENCOUNTER_ENEMIES_FIRST) {
    s32 limit = ((Random16() * 5) >> 16) + 1;

    if (limit <= 1) {
      limit = 2;
    }
    if (limit < unitCount) {
      unitCount = limit;
    }
  }

  for (shuffleOrUnitIndex = 0; shuffleOrUnitIndex < unitCount;
       shuffleOrUnitIndex++) {
    s32 unitId = unitIds[shuffleOrUnitIndex];
    struct BattleUnit *unit = UnitGetFar(unitId);
    s32 copyIndex;

    for (copyIndex = 0; copyIndex < unit->actionEntryCount; copyIndex++) {
      struct BattleActionRecord *entry = &entries[entryCount];
      s32 agility;

      entry->unitId = unitId;
      agility = unit->agi;
      entry->priority = agility;
      if (copyIndex != 0) {
        entry->priority = (s16)agility / 2;
      }

      if (unit->sleep != 0 || unit->stun != 0) {
        entry->command = CMD_NONE;
        entry->parameter = 0;
        entry->target = TARGET_NONE_ENEMY;
      } else {
        BattleCommand_SelectAutomatic(entry, 0);
      }

      entryCount++;

      if (battle->encounterMode == BATTLE_ENCOUNTER_ENEMIES_FIRST) {
        break;
      }
    }
  }

  return entryCount;
}

/// A Djinni whose effect is a guard or Ground acts before everyone else.
#define DJINN_FIRST_PRIORITY 10000

/// Sorts the round's actions into turn order, fastest first, after moving
/// the guarding and grounding Djinn to the front.
void BattleQueue_SortByPriority(struct BattleActionRecord *entries, s32 count) {
  s32 actionIndex;
  s32 sortIndex;
  s32 swapCount;

  for (actionIndex = 0; actionIndex < count; actionIndex++) {
    struct BattleActionRecord *entry = &entries[actionIndex];

    if (entry->command == CMD_DJINN) {
      struct BattleAct *action;

      // parameter: the Djinni's element in bits 8 to 11, its number below.
      UnitGetFar(entry->unitId);
      action = BattleActGet(Djinn_GetAbilityIdFar(entry->parameter >> 8 & 15,
                                                  entry->parameter & 0xff));
      if (action->effect == ABILITY_EFFECT_DEFEND ||
          action->effect == ABILITY_EFFECT_TOTAL_DEFENSE ||
          action->effect == ABILITY_EFFECT_GROUND) {
        entry->priority += DJINN_FIRST_PRIORITY;
      }
    }
  }

  do {
    swapCount = 0;
    for (sortIndex = count - 1; sortIndex > 0; sortIndex--) {
      if ((s16)entries[sortIndex].priority >
          (s16)entries[sortIndex - 1].priority) {
        struct BattleActionRecord temporary;

        Iwram_CopyWords(&temporary, &entries[sortIndex],
                        sizeof(struct BattleActionRecord));
        Iwram_CopyWords(&entries[sortIndex], &entries[sortIndex - 1],
                        sizeof(struct BattleActionRecord));
        Iwram_CopyWords(&entries[sortIndex - 1], &temporary,
                        sizeof(struct BattleActionRecord));
        swapCount++;
      }
    }
  } while (swapCount != 0);
}

/// The header a link battle sends before its actions: how many there are,
/// the next random number to check the consoles agree, and the parent's
/// random seed for the round.
struct BattleLinkedActionState {
  s32 count;
  s32 check;
  s32 seed;
  u8 unused[28];
};

/// The link sends in packets of 20 bytes (the header is one), and a transfer
/// may take 300 frames.
#define LINK_PACKET_BYTES 20
#define LINK_TRANSFER_TIMEOUT 300
/// The bytes of count actions, in whole packets.
#define LINK_ACTIONS_BYTES(count)                                              \
  ((u32)((count) * 16 + LINK_PACKET_BYTES - 1) / LINK_PACKET_BYTES *           \
   LINK_PACKET_BYTES)

/// Link battle: exchanges the round's actions with the other console and
/// appends the opponent's after the count of this console's. Each console
/// sees its own party as the party: the received actions' units take the
/// ids they have here and their targets swap sides. The parent sends first
/// and gives the round its random seed; the child checks both consoles
/// agree on the next random number. A party member's priority is made even
/// on the parent and odd on the child, so equal speeds sort alike on both.
/// Returns how many actions were received, or -1 when the exchange fails.
s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions,
                                           s32 count) {
  struct BattleWork *battle = gBattleWork;
  s32 result = 0;
  s32 allocationSize = LINK_ACTIONS_BYTES(count);
  struct BattleLinkedActionState *state =
      (struct BattleLinkedActionState *)Runtime_BumpAllocateAlternatePool(40);
  s32 index;

  // Sends the header, then this console's actions.
  s32 BattleLink_SendActions(void) {
    s32 timeout;
    s32 disconnected;
    s32 status = SerialRuntime_BeginSend(state, LINK_PACKET_BYTES);

    disconnected = 0;
    timeout = LINK_TRANSFER_TIMEOUT;
    if (status == -1)
      return status;
    while (SerialRuntime_GetActiveTransfers()) {
      WaitFrames(1);
      if (--timeout < 0)
        return -1;
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++disconnected > LINK_MAX_MISSED_FRAMES)
          return -1;
      } else {
        disconnected = 0;
      }
    }
    if (allocationSize) {
      status = SerialRuntime_BeginSend(actions, allocationSize);
      if (status == -1)
        return status;
      while (SerialRuntime_GetActiveTransfers()) {
        WaitFrames(1);
        if (--timeout < 0)
          return -1;
        if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
          if (++disconnected > LINK_MAX_MISSED_FRAMES)
            return -1;
        } else {
          disconnected = 0;
        }
      }
    }
    return 0;
  }

  // Receives the header, then the other console's actions after ours.
  s32 BattleLink_ReceiveActions(void) {
    // gSerialReceivedSize changes under the serial interrupt and is volatile
    // (serial_runtime.h), so every read here loads it again.
    s32 timeout;
    s32 disconnected;
    s32 status = SerialRuntime_BeginReceive(state);

    disconnected = 0;
    timeout = LINK_TRANSFER_TIMEOUT;
    if (status == -1)
      return status;
    while (SerialRuntime_GetActiveTransfers()) {
      if (gSerialReceivedSize > LINK_PACKET_BYTES)
        return -1;
      WaitFrames(1);
      if (--timeout < 0)
        return -1;
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++disconnected > LINK_MAX_MISSED_FRAMES)
          return -1;
      } else {
        disconnected = 0;
      }
    }
    if (gSerialReceivedSize != LINK_PACKET_BYTES)
      return -1;
    result = state->count;
    if (state->count) {
      status = SerialRuntime_BeginReceive(actions + count);
      if (status == -1)
        return status;
      while (SerialRuntime_GetActiveTransfers()) {
        if (gSerialReceivedSize > LINK_ACTIONS_BYTES(result))
          return -1;
        WaitFrames(1);
        if (--timeout < 0)
          return -1;
        if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
          if (++disconnected > LINK_MAX_MISSED_FRAMES)
            return -1;
        } else {
          disconnected = 0;
        }
      }
      if (gSerialReceivedSize != LINK_ACTIONS_BYTES(result))
        return -1;
    }
    return 0;
  }

  // Each action carries its unit's id as the opponent knows it.
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
      state->check = BattleRandomFar();
      IRQ_LOCK(saved);
      gBattleRandomSeed = state->seed = gRandomSeed;
      IRQ_UNLOCK(saved);
      if (BattleLink_SendActions() < 0 || BattleLink_ReceiveActions() < 0)
        goto fail;
      result = state->count;
    } else {
      if (BattleLink_ReceiveActions() < 0)
        goto fail;
      result = state->count;
      state->count = count;
      if (BattleLink_SendActions() < 0 || BattleRandomFar() != state->check)
        goto fail;
      gBattleRandomSeed = state->seed;
    }

    // The received actions, seen from this side.
    for (index = 0; index < result; index++) {
      struct BattleActionRecord *action = &actions[count + index];

      action->unitId = action->slot;
      action->target ^= FIRST_ENEMY_UNIT;
    }

    MemFree(state);
    return result;
  }

fail:
  BattleLink_ResetTransferState();
  SerialRuntime_RemoveIrqHandlers();
  MemFree(state);
  return -1;
}

/// Copies BG palette 15 to 14 and 5 to 4, brightened or darkened.
/// offset : added to each of the red, green and blue values (held in 0 to 31)
void Palette_CopyBanksWithBrightnessOffset(s32 offset) {
  s32 bankIndex;
  s32 sourceBank;
  s32 colorIndex;
  s32 channelMask;
  s32 paletteIndex;

  sourceBank = 15;
  bankIndex = 0;
  channelMask = 31;
  do {
    for (colorIndex = 0; colorIndex <= 15; colorIndex++) {
      u32 color;
      s32 blue;
      s32 green;
      s32 red;
      s32 adjustedColor;

      paletteIndex = sourceBank * 16 + colorIndex;
      color = ((u16 *)BG_PLTT)[paletteIndex];
      blue = (color >> 10) & channelMask;
      green = (color >> 5) & channelMask;
      red = color & channelMask;
      blue += offset;
      green += offset;
      red += offset;
      if (blue > 31)
        blue = 31;
      if (green > 31)
        green = 31;
      if (red > 31)
        red = 31;
      if (blue < 0)
        blue = 0;
      if (green < 0)
        green = 0;
      if (red < 0)
        red = 0;
      adjustedColor = blue << 10;
      adjustedColor |= green << 5;
      adjustedColor |= red;
      // Into the palette before the source: 14 and 4.
      ((s16 *)0x04ffffe0)[paletteIndex] = adjustedColor;
    }
    bankIndex++;
    sourceBank = 5;
  } while (bankIndex <= 1);
}

/// Builds the turn's actions and sorts them into turn order: the party's
/// commands, then the link opponent's or the enemies' actions.
/// Returns how many actions there are; negative to abort (a link failure).
/// actions : the actions (20 at most)
s32 BattlePresentation_BuildActions(struct BattleActionRecord *actions) {
  struct BattleUnit *unit;
  struct BattleWork *battle;
  u8 *encounterMode;
  s32 count;
  s32 received;
  s32 remainingActions;

  battle = gBattleWork;
  {
    struct BattleActionRecord *record = battle->actions;
    u32 actionIndex;

    for (actionIndex = 0; actionIndex < BATTLE_ACTION_CAPACITY; actionIndex++) {
      record[actionIndex].unitId = BATTLE_UNIT_LIST_END;
      // The lowest priority.
      record[actionIndex].priority = 0x8000;
    }
  }

  BattleUnit_ResetGuardLevels();
  Palette_CopyBanksWithBrightnessOffset(8);
  // Set while the commands are chosen.
  GameFlag_SetBitFar(FLAG_CHOOSING_COMMANDS);
  encounterMode = &battle->encounterMode;
  Camera_ConfigureScene(0);
  UiWork_FinalizeSharedSlotFar();

  // The party's commands (none when the enemies strike first).
  if (*encounterMode != BATTLE_ENCOUNTER_ENEMIES_FIRST) {
    count = BattlePres_BuildUnitEntries(actions);
    if (count < 0)
      goto finish;
    // A failed escape turns the round into one the enemies began.
    if (count != 0 && actions[0].command == CMD_FLEE &&
        BattleEscape_CheckSuccess() == 0)
      *encounterMode = BATTLE_ENCOUNTER_ENEMIES_FIRST;
  } else {
    count = 0;
  }

  // The link opponent's actions, or the enemies'.
  if (gBattleWork->twoSided != 0) {
    received = BattlePresentation_AppendLinkedActions(actions, count);
    if (BattlePres_WaitSync() < 0) {
      count = -1;
      goto finish;
    }
    count += received;
    if (received < 0) {
      count = -1;
      goto finish;
    }
  } else {
    count += BattlePres_BuildOpponentEntries(actions + count);
  }

  // Sort by priority, and raise the guards of Defend and Total Defense at
  // once, before anyone actions.
  BattleQueue_SortByPriority(actions, count);
  if (count > 0) {
    struct BattleActionRecord *action = actions;

    remainingActions = count;
    do {
      if (action->command == CMD_DEFEND || action->command == CMD_DEFEND2) {
        unit = UnitGetFar(action->unitId);
        // Half the damage, or a tenth.
        unit->guardLevel =
            action->command == CMD_DEFEND ? GUARD_DEFEND : GUARD_TOTAL_DEFENSE;
      }
      action++;
      remainingActions--;
    } while (remainingActions != 0);
  }

finish:
  GameFlag_ClearBitFar(FLAG_CHOOSING_COMMANDS);
  Camera_InitDefaultTransform();
  gTransitionWork->targetYaw = YAW_PARTY_TURN;
  return count;
}

/// Returns the unit id at a list place.
/// taggedPlace: the place in the low four bits; 0x80 set for the enemies' list
s32 Battle_GetTaggedSlotValue(s32 taggedPlace) {
  struct BattleWork *battle = gBattleWork;

  if ((taggedPlace & 0x80) != 0)
    return battle->enemyIds[taggedPlace & 0xf];
  return battle->partyUnits[taggedPlace & 0xf];
}

/// Returns the list place of unit id, or -1 if it is in neither list: the
/// party's as 0x100 | place, the enemies' as 0x180 | place.
/// unitId: 0 to 7 the party, any other an enemy
s32 Battle_FindTaggedSlotByValue(u32 unitId) {
  struct BattleWork *battle = gBattleWork;
  s32 placeIndex;
  s16 listedUnit;

  if (unitId <= 7) {
    for (placeIndex = 0;; placeIndex++) {
      listedUnit = battle->partyUnits[placeIndex];
      if (listedUnit == BATTLE_UNIT_LIST_END)
        return -1;
      if (listedUnit != BATTLE_UNIT_REMOVED && listedUnit == unitId)
        return placeIndex | 0x100;
    }
  }
  for (placeIndex = 0;; placeIndex++) {
    listedUnit = battle->enemyIds[placeIndex];
    if (listedUnit == BATTLE_UNIT_LIST_END)
      return -1;
    if (listedUnit != BATTLE_UNIT_REMOVED && listedUnit == unitId)
      return placeIndex | 0x180;
  }
}

/// Turns the camera while L or R is held (a second copy).
DEFINE_ADJUST_CAMERA(BattlePres_AdjustCameraByShoulderKeysAlt)

/// Does nothing; Battle_RunEncounter calls it at the start of each round.
void Battle_ReservedNoOp9B2C(void) {}
