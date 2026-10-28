/// @file
/// The spoils: coins, experience, items and level ups.
// Part of battle.c.

#include "../battle_internal.h"

/// Returns an item's rank among the spoils, higher for a better item and 0
/// for none: an indispensable item first, then a rare item, then the higher
/// item id.
/// The rank's bits: 10 ITEM_INDISPENSABLE | 9 ITEM_RARE | 8..0 the item id.
/// Called by BattleEnemy_RecordDefeat, to keep the best four drops, and by
/// Battle_AwardSpoils, to hand them out best first.
u32 BattleSpoils_RankItem(u32 inventoryEntry) {
  u32 priority = 0;
  u32 itemId = inventoryEntry & INVENTORY_ITEM_ID_MASK;
  if (itemId == 0)
    return 0;
  {
    u8 flags = Item_GetDirectFar(itemId)->flags;

    if (flags & ITEM_INDISPENSABLE)
      priority = 1;
    priority <<= 1;
    if (flags & ITEM_RARE)
      priority++;
    priority <<= 9;
    priority += itemId;
  }
  return priority;
}

/// Empties the battle's spoils (BattleWork.spoils) and
/// gAdventure.pendingItem. Called once by Battle_Run as the battle opens.
void BattleSpoils_Clear(void) {
  struct BattleWork *battle = gBattleWork;
  struct BattleSpoils *spoils = &battle->spoils;
  u16 *items;
  s32 index;

  gAdventure.pendingItem = 0;
  spoils->coins = 0;
  spoils->experience = 0;
  spoils->defeated = 0;
  items = battle->spoils.items;
  for (index = 3; index >= 0; index--)
    items[index] = 0;
}

/// Returns an enemy's coins with the Djinn bonus: its own, plus a six-sided
/// die for every ten levels and one more, at least 30 percent of its own.
/// Used by BattleEnemy_RecordDefeat.
static __inline__ s32 BattleEnemy_CalculateCoinReward(struct Unit *unit,
                                                      u16 *reward) {
  // These reward rolls retain separate inline bodies: a die-size parameter adds
  // eight bytes to BattleEnemy_RecordDefeat in EN.
  s32 rollIndex;
  s32 bonus = 0;
  s32 base;
  s32 minimumBonus;

  for (rollIndex = 0; rollIndex < (u8)(unit->level / 10U) + 1; rollIndex++)
    bonus += ((Random16() * 6) >> 16) + 1;
  base = *reward;
  minimumBonus = base * 3 / 10;
  if (bonus < minimumBonus)
    bonus = minimumBonus;
  return bonus + base;
}

/// Returns an enemy's experience with the Djinn bonus: its own, plus a
/// four-sided die for every ten levels and one more, at least 30 percent of
/// its own. Used by BattleEnemy_RecordDefeat.
static __inline__ s32 BattleEnemy_CalculateExperienceReward(struct Unit *unit,
                                                            u16 *reward) {
  s32 rollIndex;
  s32 bonus = 0;
  s32 base;
  s32 minimumBonus;

  for (rollIndex = 0; rollIndex < (u8)(unit->level / 10U) + 1; rollIndex++)
    bonus += ((Random16() * 4) >> 16) + 1;
  base = *reward;
  minimumBonus = base * 3 / 10;
  if (bonus < minimumBonus)
    bonus = minimumBonus;
  return bonus + base;
}

/// Counts a fallen enemy and adds its coins, experience and perhaps its item
/// to the spoils.
/// Returns -1 for a party unit and -2 for a party member fought as an enemy,
/// else 0. It notes the class for the victory message (firstDefeated,
/// defeatState). In a monster battle it adds nothing more; otherwise it sets
/// the class's ENEMY_DEFEATED_FLAG, adds the rewards (with the Djinn bonus,
/// djinnBonus nonzero, the rolls above) and rolls the item: 1 in
/// 2^(itemChance - 1), two powers likelier with the bonus. An item not yet
/// among the four kept replaces the lowest ranked, if it ranks higher.
/// Called as each enemy falls by BattlePlayback_RunAll and
/// BattlePlayback_Run.
s32 BattleEnemy_RecordDefeat(s32 unitId, s32 djinnBonus) {
  struct Unit *unit;
  struct EnemyDefinition *enemy;
  struct BattleWork *battle;
  struct BattleSpoils *spoils;
  s32 itemOrClassIndex;
  s32 dropShift;
  s32 classSlot;
  s32 lowestItemPriority;
  s32 lowestSlot;
  s32 itemPriority;

  unit = Unit_GetFar(unitId);
  battle = gBattleWork;
  spoils = &battle->spoils;
  if (unitId >= 0 && unitId <= ALLY_MAX)
    return -1;
  if (unit->classId != CLASS_NONE)
    return -2;
  classSlot = 0;
  itemOrClassIndex = 0;
  if (battle->enemyKinds[0] != unit->id) {
    do {
      if (++itemOrClassIndex > BATTLE_ENEMY_CAPACITY - 1)
        break;
    } while (battle->enemyKinds[itemOrClassIndex] != unit->id);
  }
  if (itemOrClassIndex != BATTLE_ENEMY_CAPACITY)
    classSlot = itemOrClassIndex;
  // The earliest listed class to fall names the victory message, and a
  // second defeat makes it DEFEAT_SEVERAL.
  // No defeat state is 2, so this test never fails.
  // XXX (no evidence): whether DEFEAT_BY_PARTY_MEMBER was meant; the code
  // behaves the same for every state the game sets, so only the author's
  // intent would say.
  if (battle->defeatState != 2) {
    if (classSlot < battle->firstDefeated)
      battle->firstDefeated = classSlot;
    if (spoils->defeated)
      battle->defeatState = DEFEAT_SEVERAL;
  }
  spoils->defeated++;
  if (GameFlag_GetFar(FLAG_MONSTER_BATTLE))
    return 0;
  GameFlag_SetFar(ENEMY_DEFEATED_FLAG(unit->id));
  enemy = (struct EnemyDefinition *)Enemy_GetDefinitionFar(unit->id);
  if (djinnBonus != 0) {
    if (enemy->coins != 0)
      spoils->coins += BattleEnemy_CalculateCoinReward(unit, &enemy->coins);
    if (enemy->experience != 0)
      spoils->experience +=
          BattleEnemy_CalculateExperienceReward(unit, &enemy->experience);
  } else {
    spoils->coins += enemy->coins;
    spoils->experience += enemy->experience;
  }
  if (enemy->item == 0 || enemy->itemChance == 0)
    return 0;
  itemOrClassIndex = 0;
  if (spoils->items[0] != enemy->item) {
    do {
      if (++itemOrClassIndex > 3)
        break;
    } while (spoils->items[itemOrClassIndex] != enemy->item);
  }
  if (itemOrClassIndex != 4)
    return 0;
  dropShift = enemy->itemChance;
  if (djinnBonus != 0)
    dropShift -= 2;
  if (dropShift < 0)
    dropShift = 0;
  // 0x20000 >> n against a 16-bit random number: 1 in 2^(n - 1).
  if ((0x20000 >> dropShift) > (Battle_RandomFar() & 0xffff)) {
    lowestSlot = -1;
    lowestItemPriority = 0x40000000;
    for (itemOrClassIndex = 0; itemOrClassIndex <= 3; itemOrClassIndex++) {
      itemPriority = BattleSpoils_RankItem(spoils->items[itemOrClassIndex]);
      if (itemPriority < lowestItemPriority) {
        lowestItemPriority = itemPriority;
        lowestSlot = itemOrClassIndex;
      }
    }
    if ((s32)BattleSpoils_RankItem(enemy->item) > lowestItemPriority)
      spoils->items[lowestSlot] = enemy->item;
  }
  return 0;
}

/// Hands out the spoils after a victory: the experience to each party member
/// still standing, with every level gained and what it brings, then the
/// coins, then the items best first.
/// Each level up shows its message, every Psynergy newly learned and each
/// stat that rose. An item the inventory cannot take goes to gOverflowItem
/// and ends the handing out. Called by Battle_Run once the battle is
/// won. Changes the party members' experience, levels and stats, the party's
/// coins and inventory.
void Battle_AwardSpoils(void) {
  struct BattleSpoils *spoils;
  s32 count;
  u16 *unitIds;
  s32 unitIndex;
  s32 unitId;
  struct Unit *unit;
  struct Unit *beforeLevelUp;
  s32 abilityIndex;
  u32 encodedAbility;
  s32 oldAbilityIndex;
  s32 highestItemPriority;
  s32 highestItemSlot;
  s32 slot;
  u16 *overflowItem;
  s32 itemPriority;
  struct LevelUpResult gains; // Read stat increases as signed values.
  u16 units[8];

  spoils = &gBattleWork->spoils;
  if (spoils->experience != 0) {
    MessageArgument_PushFar(spoils->experience, MESSAGE_ARGUMENT_NUMBER);
    BattleMessage_ShowAndWaitFar(MsgExpGained);
    BattleMessage_WaitForAdvance();
  }
  // Experience and level ups: each party member is copied before every level it
  // may gain, so that what the level brought can be read against the copy.
  unitIds = units;
  count = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, unitIds);
  beforeLevelUp = AllocEwram(sizeof(struct Unit));
  for (unitIndex = 0; unitIndex < count; unitIndex++) {
    unitId = unitIds[unitIndex];
    unit = Unit_GetFar(unitId);
    unit->experience += spoils->experience;
    while (Iwram_CopyWords(beforeLevelUp, unit, sizeof(struct Unit)),
           Unit_TryLevelUpFar(unitId, &gains) != 0) {
      Sound_PlaySongFar(MUS_LEVEL_UP);
      MessageArgument_ClearAllFar();
      MessageArgument_PushFar(unit->classId, MESSAGE_ARGUMENT_CLASS);
      MessageArgument_PushFar(unitIds[unitIndex], MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(unit->level, MESSAGE_ARGUMENT_NUMBER);
      BattleMessage_ShowAndWaitFar(MsgLevelUp);
      BattleMessage_WaitForAdvance();
      // A class ability (bit 15) the copy lacks is newly learned.
      for (abilityIndex = 0; abilityIndex < 32; abilityIndex++) {
        encodedAbility = unit->psynergy[abilityIndex].encodedAction;
        if ((encodedAbility & PSYNERGY_ABILITY_MASK) &&
            (encodedAbility >> 15)) {
          for (oldAbilityIndex = 0; oldAbilityIndex < 32; oldAbilityIndex++) {
            if (encodedAbility ==
                beforeLevelUp->psynergy[oldAbilityIndex].encodedAction)
              break;
          }
          if (oldAbilityIndex == 32) {
            MessageArgument_ClearAllFar();
            MessageArgument_PushFar(unit->classId, MESSAGE_ARGUMENT_CLASS);
            MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
            MessageArgument_PushFar(encodedAbility & PSYNERGY_ABILITY_MASK,
                                    MESSAGE_ARGUMENT_ABILITY);
            Sound_PlaySongFar(SE_CAST);
            BattleMessage_ShowAndWaitFar(MsgAbilityMastered);
            BattleMessage_WaitForAdvance();
          }
        }
      }
      if ((s16)gains.hp != 0) {
        MessageArgument_PushFar((s16)gains.hp, MESSAGE_ARGUMENT_NUMBER);
        BattleMessage_ShowAndWaitFar(MsgMaxHpRises);
        BattleMessage_WaitForAdvance();
      }
      if ((s16)gains.pp != 0) {
        MessageArgument_PushFar((s16)gains.pp, MESSAGE_ARGUMENT_NUMBER);
        BattleMessage_ShowAndWaitFar(MsgMaxPpRises);
        BattleMessage_WaitForAdvance();
      }
      if ((s16)gains.atk != 0) {
        MessageArgument_PushFar((s16)gains.atk, MESSAGE_ARGUMENT_NUMBER);
        BattleMessage_ShowAndWaitFar(MsgAttackRises);
        BattleMessage_WaitForAdvance();
      }
      if ((s16)gains.def != 0) {
        MessageArgument_PushFar((s16)gains.def, MESSAGE_ARGUMENT_NUMBER);
        BattleMessage_ShowAndWaitFar(MsgDefenseRises);
        BattleMessage_WaitForAdvance();
      }
      if ((s16)gains.agi != 0) {
        MessageArgument_PushFar((s16)gains.agi, MESSAGE_ARGUMENT_NUMBER);
        BattleMessage_ShowAndWaitFar(MsgAgilityRises);
        BattleMessage_WaitForAdvance();
      }
      if ((s16)gains.luck != 0) {
        MessageArgument_PushFar((s16)gains.luck, MESSAGE_ARGUMENT_NUMBER);
        BattleMessage_ShowAndWaitFar(MsgLuckRises);
        BattleMessage_WaitForAdvance();
      }
    }
  }
  Free(beforeLevelUp);
  if (spoils->coins != 0) {
    MessageArgument_PushFar(spoils->coins, MESSAGE_ARGUMENT_NUMBER);
    BattleMessage_ShowAndWaitFar(MsgCoinsGained);
    Party_AdjustCoinsFar(spoils->coins);
    BattleMessage_WaitForAdvance();
  }
  // The items, the highest ranked first; the first the inventory cannot
  // take is left in gOverflowItem.
  overflowItem = &gOverflowItem;
  for (;;) {
    highestItemSlot = -1;
    highestItemPriority = -1;
    for (slot = 0; slot <= 3; slot++) {
      if (spoils->items[slot] != 0) {
        itemPriority = BattleSpoils_RankItem(spoils->items[slot]);
        if (itemPriority >= highestItemPriority) {
          highestItemPriority = itemPriority;
          highestItemSlot = slot;
        }
      }
    }
    if (highestItemSlot == -1)
      break;
    MessageArgument_PushFar(spoils->items[highestItemSlot],
                            MESSAGE_ARGUMENT_ITEM);
    BattleMessage_ShowAndWaitFar(MsgItemGained);
    BattleMessage_WaitForAdvance();
    if (PartyInventory_AddFar(spoils->items[highestItemSlot]) == -1) {
      *overflowItem = spoils->items[highestItemSlot];
      break;
    }
    spoils->items[highestItemSlot] = 0;
  }
}

/// Does nothing; the debug battle menu (DebugBattle_RunMenu) calls it.
void Battle_DebugSelectNoOp(void) {}
