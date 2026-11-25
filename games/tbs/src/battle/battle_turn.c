/// @file
/// A turn's actions: the party's commands, the enemies' or the link
/// opponent's, and their action order.
// Part of battle.c.

#include "battle_internal.h"

/// The odds of fleeing in hundredths of a percent, against a roll below
/// FLEE_ROLL_RANGE: FLEE_BASE_CHANCE, FLEE_CHANCE_PER_FAILURE more for each
/// failed attempt, and FLEE_CHANCE_PER_LEVEL for each level of the party's
/// average above the enemies'.
enum FleeChance {
  FLEE_ROLL_RANGE = 10000,
  FLEE_BASE_CHANCE = 5000,
  FLEE_CHANCE_PER_FAILURE = 2000,
  FLEE_CHANCE_PER_LEVEL = 500
};

/// The random swaps that shuffle the enemies' order each turn.
#define TURN_ORDER_SWAPS 32

/// An empty action's priority, below every other.
#define PRIORITY_LOWEST -32768

/// Unused: fills HIGHLIGHT_PALETTE with WINDOW_PALETTE's colours, brightened by
/// a pulse that rises and falls with the frame count.
void Palette_UpdatePulseBrightness(void) {
  u16 *palette = &BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0);
  s32 i;

  for (i = 0; i < PALETTE_COLORS; i++) {
    // A pulse about every 21 frames (3/64 of a turn a frame), brightening each
    // channel by up to 12 levels.
    u32 angle = gFrameCount * (TRIG_TURN * 3 / 64);
    u32 color = palette[PALETTE_COLORS];
    s32 brightness = (FIXED_ONE - Trig_Cos(angle)) / (FIXED_ONE / 6);
    s32 blue = RGB_BLUE(color);
    s32 green = RGB_GREEN(color);
    s32 red = RGB_RED(color);

    blue += brightness;
    green += brightness;
    red += brightness;
    if (blue < 0 || blue > RGB_LEVEL_MAX)
      blue = RGB_LEVEL_MAX;
    if (green < 0 || green > RGB_LEVEL_MAX)
      green = RGB_LEVEL_MAX;
    if (red < 0 || red > RGB_LEVEL_MAX)
      red = RGB_LEVEL_MAX;
    *palette++ = RGB(red, green, blue);
  }
}

/// The command view's projection has no focal length, so every unit is drawn at
/// one scale (Matrix_ProjectPoint's 1.32) whatever its depth, out to twice this
/// depth; its near depth, as BattleCamera_SetView's, is a fraction of a pixel.
#define COMMAND_VIEW_DEPTH toFIXED(969)

/// Sets the camera up for the fixed view the battle shows while commands are
/// chosen, raised by verticalOffset pixels.
void BattleCamera_SetCommandView(s32 verticalOffset) {
  void **slots = &gHeapSlots[HEAP_SLOT_CAMERA];
  struct BattleCamera *camera = slots[0];
  struct BattleBackgroundState *view =
      HEAP_SLOT_FROM(slots, HEAP_SLOT_CAMERA, HEAP_SLOT_BATTLE_BACKGROUND);
  struct Point eyeOffset;
  u32 result;

  // The focus 5 pixels up, a quarter turn of yaw, a slight downward pitch, 750
  // pixels away.
  camera->focus[1] = toFIXED(5);
  camera->focus[0] = 0;
  camera->focus[2] = 0;
  view->targetYaw = DEGtoANG(90);
  camera->yaw = DEGtoANG(90);
  camera->pitch = TRIG_TURN - TRIG_TURN * 3 / 64;
  camera->followPosition = 0;
  camera->distance = toFIXED(750);
  camera->followEye = 0;

  Matrix_Initialize();
  Matrix_TranslateVector(&camera->focus[0]);
  Matrix_RotateY(camera->yaw);
  Matrix_RotateX(camera->pitch);

  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = camera->distance;
  Iwram_TransformVector(&eyeOffset, (s32 *)camera);

  result = Iwram_DivFX(COMMAND_VIEW_DEPTH, FIXED_ONE * 3 / 4);
  Projection_Set(0, result, COMMAND_VIEW_DEPTH * 2);

  // The background zoomed twofold and raised 8 of its pixels (16 on screen);
  // while the view is busy, BattleCamera_SetRange leaves BG1's scroll alone.
  gProjection.centerY = verticalOffset + BATTLE_PROJECTION_CENTER_Y;
  view->busy = 1;
  BattleCamera_SetRange(toFIXED(120), toFIXED(118 - verticalOffset), 0,
                        Q_8_8(8), FIXED_ONE * 2);
  view->keepRange = 1;
  view->busy = 0;
}

/// Ends every unit's guard (Defend and the like) and recalculates its stats.
void BattleUnit_ResetGuardLevels(void) {
  u16 unitIds[UNIT_MAX];
  s32 count;
  s32 unitIndex;

  count = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
  for (unitIndex = 0; unitIndex < count; unitIndex++) {
    struct Unit *unit;

    unit = Unit_GetFar(unitIds[unitIndex]);
    unit->defendLevel = 0;
    Unit_CalculateFar(unitIds[unitIndex]);
  }
}

/// Rolls whether the party gets away: 50 percent, 20 more for each failed
/// attempt, and 5 for each level the party's average is above the enemies'. A
/// party that struck first always gets away, and no one from a battle that
/// forbids it. Returns 1 when the party gets away.
s32 BattleFlee_CheckSuccess(void) {
  s32 escaped;
  u8 *failedAttempts;
  s16 livingUnits[UNIT_MAX];
  s32 livingCount;
  s32 levelTotal;
  s32 unitIndex;
  s32 chance;
  struct BattleState *battle;

  escaped = 0;
  battle = gBattleState;
  if (battle->firstStrike == FIRST_STRIKE_PARTY) {
    escaped = 1;
  } else {
    failedAttempts = &battle->fleeFailures;
    chance =
        FLEE_BASE_CHANCE + (battle->fleeFailures * FLEE_CHANCE_PER_FAILURE);
    livingCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, livingUnits);
    levelTotal = 0;
    for (unitIndex = escaped; unitIndex < livingCount; unitIndex++) {
      levelTotal += Unit_GetFar(livingUnits[unitIndex])->level;
    }
    chance += levelTotal * FLEE_CHANCE_PER_LEVEL / livingCount;
    livingCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, livingUnits);
    levelTotal = 0;
    for (unitIndex = 0; unitIndex < livingCount; unitIndex++) {
      levelTotal += Unit_GetFar(livingUnits[unitIndex])->level;
    }
    chance -= levelTotal * FLEE_CHANCE_PER_LEVEL / livingCount;
    if ((chance > 0) && (Random16_Below(FLEE_ROLL_RANGE) < chance)) {
      escaped = 1;
    }
    *failedAttempts += 1;
  }
  if (gMainRoutineState.battleType == BATTLE_TYPE_NO_ESCAPE) {
    escaped = 0;
  }
  return escaped;
}

/// The party's actions for the turn, from the command menu; one who is asleep,
/// confused, charmed or stunned loses theirs. Returns how many there are, or -1
/// if the menu gave up.
s32 Battle_BuildPartyActions(struct BattleAction *entries) {
  // Up to eight choosers, and up to four party members with their end mark.
  u16 *choosingUnits = Heap_AllocEwram(17);
  u16 *unitIds = Heap_AllocEwram(9);
  s32 unitCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, unitIds);
  s32 choosingCount = 0;
  s32 entryCount = 0;
  s32 unitIndex;

  for (unitIndex = 0; unitIndex < unitCount; unitIndex++) {
    struct Unit *unit = Unit_GetFar(unitIds[unitIndex]);
    s32 copyIndex;

    // Once for each of the party member's actions this turn (actionsPerTurn):
    // a party member with two is listed twice and chooses twice.
    for (copyIndex = 0; copyIndex < unit->actionsPerTurn; copyIndex++) {
      if (unit->sleep != 0 || unit->confusion != 0 || unit->charm != 0 ||
          unit->stun != 0) {
        struct BattleAction *entry = &entries[entryCount];
        entry->unitId = unitIds[unitIndex];
        entry->priority = unit->agility;
        entry->command = COMMAND_NONE;
        entry->parameter = 0;
        entry->target = TARGET_NONE_PARTY;
        entryCount++;
      } else {
        choosingUnits[choosingCount++] = unitIds[unitIndex];
      }
    }
  }

  // The command menu appends the choosing party members' actions.
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
    Heap_Release(unitIds);
    Heap_Release(choosingUnits);
    return unitCount;
  }
}

/// The enemies' actions for the turn, in a shuffled order. An enemy acting more
/// than once acts later at half its Agility; none act when the party struck
/// first, and when the party is caught by surprise two to five act once each.
/// Returns how many there are.
s32 Battle_BuildEnemyActions(struct BattleAction *entries) {
  u16 unitIds[UNIT_MAX];
  s32 entryCount = 0;
  struct BattleState *battle = gBattleState;
  s32 unitCount;
  s32 i;

  if (battle->firstStrike == FIRST_STRIKE_PARTY) {
    return 0;
  }

  unitCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, unitIds);
  if (unitCount == 0) {
    return 0;
  }

  for (i = 0; i < TURN_ORDER_SWAPS; i++) {
    u32 first = Random16_Below(unitCount);
    u32 second = Random16_Below(unitCount);
    s32 swap = unitIds[first];
    unitIds[first] = unitIds[second];
    unitIds[second] = swap;
  }

  // A roll of 1 to 5, raised to at least 2.
  if (battle->firstStrike == FIRST_STRIKE_ENEMIES) {
    s32 limit = Random16_Below(5) + 1;

    if (limit <= 1) {
      limit = 2;
    }
    if (limit < unitCount) {
      unitCount = limit;
    }
  }

  for (i = 0; i < unitCount; i++) {
    s32 unitId = unitIds[i];
    struct Unit *unit = Unit_GetFar(unitId);
    s32 copyIndex;

    for (copyIndex = 0; copyIndex < unit->actionsPerTurn; copyIndex++) {
      struct BattleAction *entry = &entries[entryCount];
      s32 agility;

      entry->unitId = unitId;
      agility = unit->agility;
      entry->priority = agility;
      if (copyIndex != 0) {
        entry->priority = (s16)agility / 2;
      }

      if (unit->sleep != 0 || unit->stun != 0) {
        entry->command = COMMAND_NONE;
        entry->parameter = 0;
        entry->target = TARGET_NONE_ENEMY;
      } else {
        BattleAI_ChooseAction(entry, 0);
      }

      entryCount++;

      if (battle->firstStrike == FIRST_STRIKE_ENEMIES) {
        break;
      }
    }
  }

  return entryCount;
}

/// A Djinni whose effect is a guard or Ground acts before everyone else: its
/// priority is raised by this, past any Agility (999 at most).
#define DJINN_FIRST_PRIORITY 10000

/// Sorts the turn's actions into action order, fastest first, after moving the
/// guarding and grounding Djinn to the front.
void BattleActionOrder_Sort(struct BattleAction *entries, s32 count) {
  s32 actionIndex;
  s32 sortIndex;
  s32 swapCount;

  for (actionIndex = 0; actionIndex < count; actionIndex++) {
    struct BattleAction *entry = &entries[actionIndex];

    if (entry->command == COMMAND_DJINN) {
      struct AbilityDefinition *action;

      // DEAD: the unit, fetched and not used; the ROM keeps the call.
      Unit_GetFar(entry->unitId);
      action = Ability_Get(
          Djinn_GetAbilityIdFar(DJINN_COMMAND_ELEMENT(entry->parameter),
                                DJINN_COMMAND_INDEX(entry->parameter)));
      if (action->effect == ABILITY_EFFECT_DEFEND ||
          action->effect == ABILITY_EFFECT_TOTAL_DEFENSE ||
          action->effect == ABILITY_EFFECT_GROUND) {
        entry->priority += DJINN_FIRST_PRIORITY;
      }
    }
  }

  // Equal priorities keep their order, including the shuffle of enemies.
  do {
    swapCount = 0;
    for (sortIndex = count - 1; sortIndex > 0; sortIndex--) {
      if (entries[sortIndex].priority > entries[sortIndex - 1].priority) {
        struct BattleAction temporary;

        Iwram_CopyWords(&temporary, &entries[sortIndex],
                        sizeof(struct BattleAction));
        Iwram_CopyWords(&entries[sortIndex], &entries[sortIndex - 1],
                        sizeof(struct BattleAction));
        Iwram_CopyWords(&entries[sortIndex - 1], &temporary,
                        sizeof(struct BattleAction));
        swapCount++;
      }
    }
  } while (swapCount != 0);
}

/// The header a link battle sends before its actions: how many, the next random
/// number to check the consoles agree, and the parent's seed for the turn.
struct BattleLinkHeader {
  s32 count; ///< actions that follow the header
  s32 check; ///< the parent's next Battle_RandomFar; the child checks its own
  s32 seed;  ///< the parent's gRandomSeed, the turn's battle seed
  u8 unused[28]; ///< padding; a header packet carries only the first 20 bytes
};

/// The link sends in packets of 20 bytes (the header is one), and a transfer
/// may take 300 frames.
#define LINK_PACKET_BYTES 20
#define LINK_TRANSFER_TIMEOUT 300
/// The bytes of count actions (16 each, a struct BattleAction), in
/// whole packets.
#define LINK_ACTIONS_BYTES(count)                                              \
  ((u32)((count) * 16 + LINK_PACKET_BYTES - 1) / LINK_PACKET_BYTES *           \
   LINK_PACKET_BYTES)

/// Link battle: exchanges the turn's actions with the other console and appends
/// the opponent's, their units and targets seen from this side. Returns how
/// many were received, or -1 when the exchange fails.
s32 Battle_AppendLinkedActions(struct BattleAction *actions, s32 count) {
  struct BattleState *battle = gBattleState;
  s32 result = 0;
  s32 allocationSize = LINK_ACTIONS_BYTES(count);
  struct BattleLinkHeader *header =
      Heap_AllocEwram(sizeof(struct BattleLinkHeader));
  s32 index;

  // Sends the header, then this console's actions. Both transfers share one
  // timeout of LINK_TRANSFER_TIMEOUT frames.
  s32 LinkBattle_SendActions(void) {
    s32 timeout;
    s32 disconnected;
    s32 status = Serial_StartSend(header, LINK_PACKET_BYTES);

    disconnected = 0;
    timeout = LINK_TRANSFER_TIMEOUT;
    if (status == -1)
      return status;
    while (Serial_GetActiveTransfers()) {
      wait(1);
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
      status = Serial_StartSend(actions, allocationSize);
      if (status == -1)
        return status;
      while (Serial_GetActiveTransfers()) {
        wait(1);
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

  // Receives the header, then the other console's actions after ours. A
  // transfer that grows past its expected size, or ends short of it, fails.
  s32 LinkBattle_ReceiveActions(void) {
    // gSerialReceivedSize changes under the serial interrupt and is volatile
    // (serial.h), so every read here loads it again.
    s32 timeout;
    s32 disconnected;
    s32 status = Serial_StartReceive(header);

    disconnected = 0;
    timeout = LINK_TRANSFER_TIMEOUT;
    if (status == -1)
      return status;
    while (Serial_GetActiveTransfers()) {
      if (gSerialReceivedSize > LINK_PACKET_BYTES)
        return -1;
      wait(1);
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
    result = header->count;
    if (header->count) {
      status = Serial_StartReceive(actions + count);
      if (status == -1)
        return status;
      while (Serial_GetActiveTransfers()) {
        if (gSerialReceivedSize > LINK_ACTIONS_BYTES(result))
          return -1;
        wait(1);
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
    struct BattleAction *action = &actions[index];

    action->slot = battle->opponentSlots[action->unitId];

    if (battle->linkSide == 0) {
      if (action->priority & 1)
        action->priority++;
    } else {
      action->priority |= 1;
    }
  }

  // The parent (link side 0) sends, then receives; the child receives,
  // then sends and checks the parent's next random number against its own.
  if (battle->linkPaused == 0) {
    if (battle->linkSide == 0) {
      u32 saved;

      header->count = count;
      header->check = Battle_RandomFar();
      IRQ_LOCK(saved);
      Battle_SetRandomSeed(header->seed = gRandomSeed);
      IRQ_UNLOCK(saved);
      if (LinkBattle_SendActions() < 0 || LinkBattle_ReceiveActions() < 0)
        goto fail;
      result = header->count;
    } else {
      if (LinkBattle_ReceiveActions() < 0)
        goto fail;
      result = header->count;
      header->count = count;
      if (LinkBattle_SendActions() < 0 || Battle_RandomFar() != header->check)
        goto fail;
      Battle_SetRandomSeed(header->seed);
    }

    // The received actions, seen from this side: the xor with FIRST_ENEMY_UNIT
    // moves a target between the party's ids and the enemies'.
    for (index = 0; index < result; index++) {
      struct BattleAction *action = &actions[count + index];

      action->unitId = action->slot;
      action->target ^= FIRST_ENEMY_UNIT;
    }

    Heap_Release(header);
    return result;
  }

fail:
  LinkBattle_ResetTransferState();
  Serial_RemoveIrqHandlers();
  Heap_Release(header);
  return -1;
}

/// Copies WINDOW_PALETTE into HIGHLIGHT_PALETTE and BASIC_COLORS_PALETTE into
/// the palette before it, each channel moved by offset levels.
void Palette_CopyWithBrightnessOffset(s32 offset) {
  s32 bank;
  s32 source = WINDOW_PALETTE;
  s32 i;

  for (bank = 0; bank < 2; bank++) {
    for (i = 0; i < PALETTE_COLORS; i++) {
      s32 index = source * PALETTE_COLORS + i;
      u32 color = ((u16 *)BG_PLTT)[index];
      s32 blue = RGB_BLUE(color);
      s32 green = RGB_GREEN(color);
      s32 red = RGB_RED(color);
      s32 adjusted;

      blue += offset;
      green += offset;
      red += offset;
      if (blue > RGB_LEVEL_MAX)
        blue = RGB_LEVEL_MAX;
      if (green > RGB_LEVEL_MAX)
        green = RGB_LEVEL_MAX;
      if (red > RGB_LEVEL_MAX)
        red = RGB_LEVEL_MAX;
      if (blue < 0)
        blue = 0;
      if (green < 0)
        green = 0;
      if (red < 0)
        red = 0;
      adjusted = RGB(red, green, blue);
      ((u16 *)BG_PLTT)[index - PALETTE_COLORS] = adjusted;
    }
    source = BASIC_COLORS_PALETTE;
  }
}

/// Builds the turn's actions and sorts them: the party's commands, then the
/// link opponent's or the enemies' actions. Returns how many there are,
/// negative when the link fails.
s32 Battle_GatherActions(struct BattleAction *actions) {
  struct Unit *unit;
  struct BattleState *battle;
  s32 count;
  s32 received;
  s32 i;

  battle = gBattleState;
  // Empty every action slot of the battle state.
  {
    struct BattleAction *record = battle->actions;
    u32 actionIndex;

    for (actionIndex = 0; actionIndex < BATTLE_ACTION_CAPACITY; actionIndex++) {
      record[actionIndex].unitId = BATTLE_UNIT_LIST_END;
      record[actionIndex].priority = PRIORITY_LOWEST;
    }
  }

  // Last turn's guards end; the highlight palettes take the window's and the
  // basic colours eight levels brighter; the camera takes the command view.
  BattleUnit_ResetGuardLevels();
  Palette_CopyWithBrightnessOffset(8);
  GameFlag_SetFar(FLAG_CHOOSING_COMMANDS);
  BattleCamera_SetCommandView(0);
  BattleMessage_CloseFar();

  // The party's commands (none when the enemies strike first).
  if (battle->firstStrike != FIRST_STRIKE_ENEMIES) {
    count = Battle_BuildPartyActions(actions);
    if (count < 0)
      goto finish;
    // A failed attempt to flee makes the turn one the enemies began.
    if (count != 0 && actions[0].command == COMMAND_FLEE &&
        BattleFlee_CheckSuccess() == 0)
      battle->firstStrike = FIRST_STRIKE_ENEMIES;
  } else {
    count = 0;
  }

  // The link opponent's actions, or the enemies'.
  // A link battle syncs even after a failed exchange, before giving up.
  if (gBattleState->isLink != 0) {
    received = Battle_AppendLinkedActions(actions, count);
    if (LinkBattle_WaitSync() < 0) {
      count = -1;
      goto finish;
    }
    count += received;
    if (received < 0) {
      count = -1;
      goto finish;
    }
  } else {
    count += Battle_BuildEnemyActions(actions + count);
  }

  // Sort by priority, and raise the guards of Defend and Total Defense at
  // once, before anyone acts.
  BattleActionOrder_Sort(actions, count);
  for (i = 0; i < count; i++) {
    struct BattleAction *action = &actions[i];

    if (action->command == COMMAND_DEFEND ||
        action->command == COMMAND_TOTAL_DEFENSE) {
      unit = Unit_GetFar(action->unitId);
      // Half the damage, or a tenth.
      unit->defendLevel =
          action->command == COMMAND_DEFEND ? DEFEND_NORMAL : DEFEND_TOTAL;
    }
  }

finish:
  // The battle view again, turned toward the party's side.
  GameFlag_ClearFar(FLAG_CHOOSING_COMMANDS);
  BattleCamera_SetDefaultView();
  gBattleBackgroundState->targetYaw = YAW_PARTY_ACTION;
  return count;
}

/// Returns the unit id at a list place, PLACE_ENEMY_SIDE set for the enemies'
/// list.
s32 Battle_GetUnitAtPlace(s32 taggedPlace) {
  struct BattleState *battle = gBattleState;

  if ((taggedPlace & PLACE_ENEMY_SIDE) != 0)
    return battle->enemyIds[taggedPlace & PLACE_INDEX_MASK];
  return battle->partyUnits[taggedPlace & PLACE_INDEX_MASK];
}

/// Returns the list place of a unit, PLACE_PARTY or PLACE_ENEMY with its place,
/// or -1 when it is in neither list.
s32 Battle_FindUnitPlace(u32 unitId) {
  struct BattleState *battle = gBattleState;
  s32 placeIndex;
  s16 listedUnit;

  if (unitId <= ALLY_MAX) {
    for (placeIndex = 0;; placeIndex++) {
      listedUnit = battle->partyUnits[placeIndex];
      if (listedUnit == BATTLE_UNIT_LIST_END)
        return -1;
      if (listedUnit != BATTLE_UNIT_REMOVED && listedUnit == unitId)
        return placeIndex | PLACE_PARTY;
    }
  }
  for (placeIndex = 0;; placeIndex++) {
    listedUnit = battle->enemyIds[placeIndex];
    if (listedUnit == BATTLE_UNIT_LIST_END)
      return -1;
    if (listedUnit != BATTLE_UNIT_REMOVED && listedUnit == unitId)
      return placeIndex | PLACE_ENEMY;
  }
}

/// Unused: a second copy of the camera turn while L or R is held.
DEFINE_ADJUST_CAMERA(BattleCamera_AdjustByShoulderKeysAlt)

/// Does nothing; Battle_Run calls it at the start of each turn.
void Battle_TurnStartNoOp(void) {}
