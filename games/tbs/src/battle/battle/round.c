/// @file
/// A round's actions: the party's commands, the enemies' or the link
/// opponent's, and their turn order.
// Part of battle.c.

#include "../battle_internal.h"

/// Background palette 14, which takes palette 15 brightened.
#define BG_PALETTE_14 ((u16 *)&BG_PLTT_COLOR(14, 0))

/// Pulses BG palette 14's colours: each takes the matching colour of
/// palette 15 brightened by up to twelve steps, rising and falling with the
/// frame count. Nothing in the source calls it. Writes BG palette RAM.
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

    // 3/64 of a turn a frame, a pulse about every 21 frames; 1 - cos runs
    // from 0 to 2.0 (16.16), and a sixth of 1.0 (0x2aaa) is one step.
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

/// Sets the camera up for the fixed view the battle shows while commands are
/// chosen, raised by verticalOffset pixels, with its projection and the
/// background's perspective to match. Called by
/// Battle_GatherActions, and through its far stub by
/// Battle_CollectPartyCommands and UpdateLinkSessionCountdown. Writes the
/// camera, gBattleBackgroundWork's targetYaw and flag (left 1, so the camera
/// callback leaves the background alone), gProjection and the background's
/// affine page.
void BattleCamera_SetCommandView(s32 verticalOffset) {
  // UB: indexing past scalar gCameraWork reaches another heap slot. Named
  // slots add four bytes in the native EN trial (0xd8 to 0xdc).
#ifdef UBFIX
  struct BattleCamera *camera = gCameraWork;
  struct BattleBackgroundWork *view =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
#else
  void **slots = (void **)&gCameraWork;
  struct BattleCamera *camera = slots[0];
  struct BattleBackgroundWork *view =
      slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_CAMERA];
#endif
  struct FixedPointPosition eyeOffset;
  u32 result;

  // Positions are 16.16 pixels and angles 1/65536 turns: the focus 5
  // pixels up, a quarter turn of yaw, 750 pixels away.
  camera->focus[1] = 160 << 11;
  camera->focus[0] = 0;
  camera->focus[2] = 0;
  view->targetYaw = 128 << 7;
  camera->yaw = 128 << 7;
  camera->pitch = 244 << 8;
  camera->followPosition = 0;
  camera->distance = 0x02ee0000;
  camera->unk18 = 0;

  Transform_Reset();
  Transform_ApplyPosition(&camera->focus[0]);
  Transform_ApplyYaw((s16)camera->yaw);
  Transform_ApplyPitch((s16)camera->pitch);

  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = camera->distance;
  Iwram_TransformVector(&eyeOffset, (s32 *)camera);

  result = Iwram_RatioMulQ14(0x03c90000, 192 << 8);
  Projection_Set(0, result, 0x07920000);

  // While active (the view's busy) is set, BattleCamera_SetRange leaves
  // BG1's scroll alone. The range: centre (120, 118 less verticalOffset),
  // offset 8 pixels in y (0x800 in 8.8), scale 2.0.
  gProjection.centerY = verticalOffset + 120;
  view->busy = 1;
  BattleCamera_SetRange(240 << 15, (0x76 - verticalOffset) << 16, 0, 128 << 4,
                        128 << 10);
  view->flag = 1;
  view->busy = 0;
}

/// Ends every unit's guard (Defend and the like) and recalculates its stats.
/// Called by Battle_GatherActions as each round begins. Clears
/// every unit's guardLevel.
void BattleUnit_ResetGuardLevels(void) {
  u16 unitIds[14];
  s32 count;
  s32 unitIndex;

  count = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
  for (unitIndex = 0; unitIndex < count; unitIndex++) {
    struct Unit *unit;

    unit = Unit_GetFar(unitIds[unitIndex]);
    unit->guardLevel = 0;
    Unit_CalculateFar(unitIds[unitIndex]);
  }
}

/// Rolls whether the party gets away. The chance, in hundredths of a
/// percent, is 5000 plus 2000 for each attempt that failed, plus 500 times
/// the party's average level, less 500 times the enemies' average level. A
/// party that struck first always gets away; no one gets away from a battle
/// that forbids it. Every attempt counts toward the next. Returns 1 when
/// the party gets away. Called by Battle_GatherActions when the
/// first action is COMMAND_FLEE. Each roll, even a successful one, raises
/// gBattleWork->fleeFailures.
s32 BattleFlee_CheckSuccess(void) {
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
  if (escapeState->firstStrike == FIRST_STRIKE_PARTY) {
    escaped = 1;
  } else {
    failedAttempts = &escapeState->fleeFailures;
    chance = 5000 + (escapeState->fleeFailures * 2000);
    livingCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, livingUnits);
    levelTotal = 0;
    for (unitIndex = escaped; unitIndex < livingCount; unitIndex++) {
      levelTotal += Unit_GetFar(livingUnits[unitIndex])->level;
    }
    chance += levelTotal * 500 / livingCount;
    livingCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, livingUnits);
    levelTotal = 0;
    for (unitIndex = 0; unitIndex < livingCount; unitIndex++) {
      levelTotal += Unit_GetFar(livingUnits[unitIndex])->level;
    }
    chance -= levelTotal * 500 / livingCount;
    // A roll from 0 to 9999 against the chance.
    if ((chance > 0) && (((10000 * Random16()) >> 0x10) < chance)) {
      escaped = 1;
    }
    *failedAttempts += 1;
  }
  if (gSaveBlock.battleType == BATTLE_TYPE_NO_ESCAPE) {
    escaped = 0;
  }
  return escaped;
}

/// The party's actions for the round, from the command menu: a member who
/// is asleep, confused, charmed or stunned loses each of their turns
/// (COMMAND_NONE); the others choose theirs (Battle_CollectPartyCommandsFar).
/// Returns how many actions there are, or -1 if the menu gave up. Called by
/// Battle_GatherActions. Fills entries.
s32 Battle_BuildUnitActions(struct BattleAction *entries) {
  // Two lists, freed before the return; the allocator rounds them up to 20
  // and 12 bytes: up to eight choosers and the up to four members with their
  // end mark.
  u16 *choosingUnits = AllocEwram(17);
  u16 *unitIds = AllocEwram(9);
  s32 unitCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, unitIds);
  s32 choosingCount = 0;
  s32 entryCount = 0;
  s32 unitIndex;

  for (unitIndex = 0; unitIndex < unitCount; unitIndex++) {
    struct Unit *unit = Unit_GetFar(unitIds[unitIndex]);
    s32 copyIndex;

    // Once for each of the member's actions this round (actionsPerRound):
    // a member with two is listed twice and chooses twice.
    for (copyIndex = 0; copyIndex < unit->actionsPerRound; copyIndex++) {
      if (unit->sleep != 0 || unit->confusion != 0 || unit->charm != 0 ||
          unit->stun != 0) {
        struct BattleAction *entry = &entries[entryCount];
        entry->unitId = unitIds[unitIndex];
        entry->priority = unit->agi;
        entry->command = COMMAND_NONE;
        entry->parameter = 0;
        entry->target = TARGET_NONE_PARTY;
        entryCount++;
      } else {
        choosingUnits[choosingCount++] = unitIds[unitIndex];
      }
    }
  }

  // The command menu appends the choosing members' actions.
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
    Free(unitIds);
    Free(choosingUnits);
    return unitCount;
  }
}

/// The enemies' actions for the round, in a shuffled order, each chosen by
/// BattleAI_ChooseAction; a sleeping or stunned enemy loses its
/// turns. An enemy acting more than once a turn acts later at half its
/// Agility. None act when the party struck first; when the party is caught
/// by surprise, only two to five enemies act, once each. Returns how many
/// actions there are. Called by Battle_GatherActions outside a
/// link battle. Fills entries.
s32 Battle_BuildOpponentActions(struct BattleAction *entries) {
  u16 unitIds[14];
  s32 entryCount = 0;
  struct BattleWork *battle = gBattleWork;
  s32 unitCount;
  s32 shuffleOrUnitIndex;

  if (battle->firstStrike == FIRST_STRIKE_PARTY) {
    return 0;
  }

  unitCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, unitIds);
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

  // A roll of 1 to 5, raised to at least 2.
  if (battle->firstStrike == FIRST_STRIKE_ENEMIES) {
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
    struct Unit *unit = Unit_GetFar(unitId);
    s32 copyIndex;

    for (copyIndex = 0; copyIndex < unit->actionsPerRound; copyIndex++) {
      struct BattleAction *entry = &entries[entryCount];
      s32 agility;

      entry->unitId = unitId;
      agility = unit->agi;
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

/// Sorts the round's actions into turn order, fastest first, after moving
/// the guarding and grounding Djinn to the front. Called by
/// Battle_GatherActions. Raises those Djinn actions' priority
/// and reorders entries in place.
void BattleTurnOrder_Sort(struct BattleAction *entries, s32 count) {
  s32 actionIndex;
  s32 sortIndex;
  s32 swapCount;

  for (actionIndex = 0; actionIndex < count; actionIndex++) {
    struct BattleAction *entry = &entries[actionIndex];

    if (entry->command == COMMAND_DJINN) {
      struct AbilityDefinition *action;

      // parameter: the Djinni's element in bits 8 to 11, its number below.
      // The unit lookup's result is unused.
      Unit_GetFar(entry->unitId);
      action = Ability_Get(Djinn_GetAbilityIdFar(entry->parameter >> 8 & 15,
                                                 entry->parameter & 0xff));
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
      if ((s16)entries[sortIndex].priority >
          (s16)entries[sortIndex - 1].priority) {
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

/// The header a link battle sends before its actions: how many there are,
/// the next random number to check the consoles agree, and the parent's
/// random seed for the round. Battle_AppendLinkedActions takes it
/// from the EWRAM pool (40 bytes) for one exchange and frees it before
/// returning. One header serves both ways: the parent's received header
/// overwrites the one it sent, and the child receives first, then writes
/// its count over the received header and sends it back.
struct BattleLinkedActionState {
  s32 count; ///< actions that follow the header
  s32 check; ///< the parent's next Battle_RandomFar; the child checks its own
  s32 seed;  ///< the parent's gRandomSeed, the round's battle seed
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

/// Link battle: exchanges the round's actions with the other console and
/// appends the opponent's after the count of this console's. Each console
/// sees its own party as the party: the received actions' units take the
/// ids they have here and their targets swap sides. The parent sends first
/// and gives the round its random seed; the child checks both consoles
/// agree on the next random number. A party member's priority is made even
/// on the parent and odd on the child, so equal speeds sort alike on both.
/// Returns how many actions were received, or -1 when the exchange fails.
/// Called by Battle_GatherActions in a link battle. Sets
/// gBattleRandomSeed, writes each action's slot and priority, and on a
/// failure resets the link transfer and removes its interrupt handlers.
s32 Battle_AppendLinkedActions(struct BattleAction *actions, s32 count) {
  struct BattleWork *battle = gBattleWork;
  s32 result = 0;
  s32 allocationSize = LINK_ACTIONS_BYTES(count);
  struct BattleLinkedActionState *state =
      (struct BattleLinkedActionState *)AllocEwram(40);
  s32 index;

  // Sends the header, then this console's actions. Both transfers share one
  // timeout of LINK_TRANSFER_TIMEOUT frames.
  s32 LinkBattle_SendActions(void) {
    s32 timeout;
    s32 disconnected;
    s32 status = Serial_StartSend(state, LINK_PACKET_BYTES);

    disconnected = 0;
    timeout = LINK_TRANSFER_TIMEOUT;
    if (status == -1)
      return status;
    while (Serial_GetActiveTransfers()) {
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
      status = Serial_StartSend(actions, allocationSize);
      if (status == -1)
        return status;
      while (Serial_GetActiveTransfers()) {
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

  // Receives the header, then the other console's actions after ours. A
  // transfer that grows past its expected size, or ends short of it, fails.
  s32 LinkBattle_ReceiveActions(void) {
    // gSerialReceivedSize changes under the serial interrupt and is volatile
    // (serial_runtime.h), so every read here loads it again.
    s32 timeout;
    s32 disconnected;
    s32 status = Serial_StartReceive(state);

    disconnected = 0;
    timeout = LINK_TRANSFER_TIMEOUT;
    if (status == -1)
      return status;
    while (Serial_GetActiveTransfers()) {
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
      status = Serial_StartReceive(actions + count);
      if (status == -1)
        return status;
      while (Serial_GetActiveTransfers()) {
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

      state->count = count;
      state->check = Battle_RandomFar();
      IRQ_LOCK(saved);
      gBattleRandomSeed = state->seed = gRandomSeed;
      IRQ_UNLOCK(saved);
      if (LinkBattle_SendActions() < 0 || LinkBattle_ReceiveActions() < 0)
        goto fail;
      result = state->count;
    } else {
      if (LinkBattle_ReceiveActions() < 0)
        goto fail;
      result = state->count;
      state->count = count;
      if (LinkBattle_SendActions() < 0 || Battle_RandomFar() != state->check)
        goto fail;
      gBattleRandomSeed = state->seed;
    }

    // The received actions, seen from this side: the xor with 0x80 moves a
    // target between the party's ids and the enemies'.
    for (index = 0; index < result; index++) {
      struct BattleAction *action = &actions[count + index];

      action->unitId = action->slot;
      action->target ^= FIRST_ENEMY_UNIT;
    }

    Free(state);
    return result;
  }

fail:
  LinkBattle_ResetTransferState();
  Serial_RemoveIrqHandlers();
  Free(state);
  return -1;
}

/// Copies BG palette 15 to 14 and 5 to 4, brightened or darkened. Called by
/// Battle_GatherActions with 8, as the round's commands are
/// chosen.
/// @param offset added to each of the red, green and blue values (held in 0
/// to 31)
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
      // Into the palette before the source, 14 and 4: 0x04ffffe0 is BG
      // palette RAM (0x05000000) less one palette's 32 bytes.
      ((s16 *)0x04ffffe0)[paletteIndex] = adjustedColor;
    }
    bankIndex++;
    sourceBank = 5;
  } while (bankIndex <= 1);
}

/// Builds the turn's actions and sorts them into turn order: the party's
/// commands, then the link opponent's or the enemies' actions.
/// Returns how many actions there are; negative to abort (a link failure).
/// Called by Battle_Run at the start of each round outside auto
/// battle. Clears gBattleWork->actions, sets every defender's guardLevel,
/// may set firstStrike to FIRST_STRIKE_ENEMIES after a failed
/// attempt to flee, and holds FLAG_CHOOSING_COMMANDS while the commands are
/// chosen.
/// @param actions the actions (20 at most)
s32 Battle_GatherActions(struct BattleAction *actions) {
  struct Unit *unit;
  struct BattleWork *battle;
  u8 *firstStrike;
  s32 count;
  s32 received;
  s32 remainingActions;

  battle = gBattleWork;
  // Empty every action slot of the battle work.
  {
    struct BattleAction *record = battle->actions;
    u32 actionIndex;

    for (actionIndex = 0; actionIndex < BATTLE_ACTION_CAPACITY; actionIndex++) {
      record[actionIndex].unitId = BATTLE_UNIT_LIST_END;
      // The lowest priority.
      record[actionIndex].priority = 0x8000;
    }
  }

  // Last round's guards end; palettes 14 and 4 take 15 and 5 eight steps
  // brighter; the camera takes the command view.
  BattleUnit_ResetGuardLevels();
  Palette_CopyBanksWithBrightnessOffset(8);
  GameFlag_SetFar(FLAG_CHOOSING_COMMANDS);
  firstStrike = &battle->firstStrike;
  BattleCamera_SetCommandView(0);
  BattleMessage_CloseFar();

  // The party's commands (none when the enemies strike first).
  if (*firstStrike != FIRST_STRIKE_ENEMIES) {
    count = Battle_BuildUnitActions(actions);
    if (count < 0)
      goto finish;
    // A failed attempt to flee turns the round into one the enemies began.
    if (count != 0 && actions[0].command == COMMAND_FLEE &&
        BattleFlee_CheckSuccess() == 0)
      *firstStrike = FIRST_STRIKE_ENEMIES;
  } else {
    count = 0;
  }

  // The link opponent's actions, or the enemies'.
  // A link battle syncs even after a failed exchange, before giving up.
  if (gBattleWork->isLink != 0) {
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
    count += Battle_BuildOpponentActions(actions + count);
  }

  // Sort by priority, and raise the guards of Defend and Total Defense at
  // once, before anyone acts.
  BattleTurnOrder_Sort(actions, count);
  if (count > 0) {
    struct BattleAction *action = actions;

    remainingActions = count;
    do {
      if (action->command == COMMAND_DEFEND ||
          action->command == COMMAND_TOTAL_DEFENSE) {
        unit = Unit_GetFar(action->unitId);
        // Half the damage, or a tenth.
        unit->guardLevel = action->command == COMMAND_DEFEND
                               ? GUARD_DEFEND
                               : GUARD_TOTAL_DEFENSE;
      }
      action++;
      remainingActions--;
    } while (remainingActions != 0);
  }

finish:
  // The battle view again, turned toward the party's turn.
  GameFlag_ClearFar(FLAG_CHOOSING_COMMANDS);
  BattleCamera_SetDefaultView();
  gBattleBackgroundWork->targetYaw = YAW_PARTY_TURN;
  return count;
}

/// Returns the unit id at a list place. Called by BattleTurn_Build
/// for a command's target.
/// @param taggedPlace the place in the low four bits; 0x80 set for the
/// enemies' list
s32 Battle_GetTaggedSlotValue(s32 taggedPlace) {
  struct BattleWork *battle = gBattleWork;

  if ((taggedPlace & 0x80) != 0)
    return battle->enemyIds[taggedPlace & 0xf];
  return battle->partyUnits[taggedPlace & 0xf];
}

/// Returns the list place of unit id, or -1 if it is in neither list: the
/// party's as 0x100 | place, the enemies' as 0x180 | place (PLACE_PARTY,
/// PLACE_ENEMY). Called by the enemies' command choice (enemy_ai.c) to aim
/// at a unit.
/// @param unitId 0 to 7 the party, any other an enemy
s32 Battle_FindTaggedSlotByValue(u32 unitId) {
  struct BattleWork *battle = gBattleWork;
  s32 placeIndex;
  s16 listedUnit;

  if (unitId <= ALLY_MAX) {
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

/// Turns the camera while L or R is held (a second copy). Nothing in the
/// source calls it.
DEFINE_ADJUST_CAMERA(BattleCamera_AdjustByShoulderKeysAlt)

/// Does nothing; Battle_Run calls it at the start of each round.
void Battle_ReservedNoOp9B2C(void) {}
