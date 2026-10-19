// Part of battle.c, the battle object, compiled only through it.
// The spoils: coins, experience, items and level ups.

#include "../battle_internal.h"

// Ranks a spoil item by its indispensable flag, flag 4 and item ID; an empty
// entry ranks zero.
u32 Item_EncodeBankedId(u32 inventoryEntry) {
  u32 priority = 0;
  u32 itemId = inventoryEntry & ITEM_ID_MASK;
  if (itemId == 0)
    return 0;
  {
    u8 flags = Item_GetDirectFar(itemId)->flags;

    if (flags & 8)
      priority = 1;
    priority <<= 1;
    if (flags & 4)
      priority++;
    priority <<= 9;
    priority += itemId;
  }
  return priority;
}

// Clears pending items and the battle's coin, experience, defeat and item
// spoils.
void Summon_ClearWorkFields(void) {
  struct BattleWork *battle = gBattleWork;
  struct BattleSpoils *spoils = &battle->spoils;
  u16 *items;
  s32 index;

  gGameState.pendingItem = 0;
  spoils->coins = 0;
  spoils->experience = 0;
  spoils->defeated = 0;
  items = battle->spoils.items;
  for (index = 3; index >= 0; index--)
    items[index] = 0;
}

// These reward rolls retain separate inline bodies: a die-size parameter
// adds eight bytes to BattleEnemy_RecordDefeat in EN.
// Adds one six-sided roll per ten levels plus one, with a bonus floor of 30
// percent of the base coins.
static __inline__ s32 BattleEnemy_CalculateCoinReward(struct BattleUnit *unit,
                                                      u16 *reward) {
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

// Adds one four-sided roll per ten levels plus one, with a bonus floor of 30
// percent of base experience.
static __inline__ s32
BattleEnemy_CalculateExperienceReward(struct BattleUnit *unit, u16 *reward) {
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

// Records an enemy defeat and its coins and experience, applying the Djinn
// bonus when earned and retaining higher-ranked distinct item drops.
s32 BattleEnemy_RecordDefeat(s32 unitId, s32 djinnBonus) {
  struct BattleUnit *unit;
  struct EnemyDefinition *enemy;
  struct BattleWork *battle;
  struct BattleSpoils *spoils;
  s32 itemOrClassIndex;
  s32 dropShift;
  s32 classSlot;
  s32 lowestItemPriority;
  s32 lowestSlot;
  s32 itemPriority;

  unit = UnitGetFar(unitId);
  battle = gBattleWork;
  spoils = &battle->spoils;
  if (unitId >= 0 && unitId < 8)
    return -1;
  if (unit->classIndex != 0)
    return -2;
  classSlot = 0;
  itemOrClassIndex = 0;
  if (battle->enemyClasses[0] != unit->classId) {
    do {
      if (++itemOrClassIndex > 5)
        break;
    } while (battle->enemyClasses[itemOrClassIndex] != unit->classId);
  }
  if (itemOrClassIndex != 6)
    classSlot = itemOrClassIndex;
  if (battle->defeatState != 2) {
    if (classSlot < battle->firstDefeated)
      battle->firstDefeated = classSlot;
    if (spoils->defeated)
      battle->defeatState = 1;
  }
  spoils->defeated++;
  if (GameFlag_TestFar(FLAG_MONSTER_BATTLE))
    return 0;
  GameFlag_SetBitFar(unit->classId + 0x600);
  enemy = Enemy_GetDefinitionFar(unit->classId);
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
  if ((0x20000 >> dropShift) > (BattleRandomFar() & 0xffff)) {
    lowestSlot = -1;
    lowestItemPriority = 0x40000000;
    for (itemOrClassIndex = 0; itemOrClassIndex <= 3; itemOrClassIndex++) {
      itemPriority = Item_EncodeBankedId(spoils->items[itemOrClassIndex]);
      if (itemPriority < lowestItemPriority) {
        lowestItemPriority = itemPriority;
        lowestSlot = itemOrClassIndex;
      }
    }
    if ((s32)Item_EncodeBankedId(enemy->item) > lowestItemPriority)
      spoils->items[lowestSlot] = enemy->item;
  }
  return 0;
}

// Awards living party members experience and level gains, reports newly learned
// Psynergy, gives coins and distributes ranked item drops until inventory
// overflow.
void Battle_AwardSpoils(void) {
  struct BattleSpoils *spoils;
  s32 count;
  u16 *unitIds;
  s32 unitIndex;
  s32 unitId;
  struct BattleUnit *unit;
  struct BattleUnit *beforeLevelUp;
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
    UiWork_PushValueSlotFar(spoils->experience, VALUE_NUMBER);
    UiText_ShowMessageAndWaitCoreFar(MsgExpGained);
    BattlePresentation_WaitForAdvance();
  }
  unitIds = units;
  count = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, unitIds);
  beforeLevelUp = Runtime_BumpAllocateAlternatePool(sizeof(struct BattleUnit));
  for (unitIndex = 0; unitIndex < count; unitIndex++) {
    unitId = unitIds[unitIndex];
    unit = UnitGetFar(unitId);
    unit->experience += spoils->experience;
    while (Iwram_CopyWords(beforeLevelUp, unit, sizeof(struct BattleUnit)),
           Unit_TryLevelUpFar(unitId, &gains) != 0) {
      AudioCommand_PlayFar(MUS_089);
      UiWork_ClearValueNameTablesFar();
      UiWork_PushValueSlotFar(unit->classIndex, VALUE_CLASS_NAME);
      UiWork_PushValueSlotFar(unitIds[unitIndex], VALUE_UNIT_NAME);
      UiWork_PushValueSlotFar(unit->level, VALUE_NUMBER);
      UiText_ShowMessageAndWaitCoreFar(MsgLevelUp);
      BattlePresentation_WaitForAdvance();
      for (abilityIndex = 0; abilityIndex < 32; abilityIndex++) {
        encodedAbility = unit->actionSlots[abilityIndex].encodedAction;
        if ((encodedAbility & OWNER_ACTION_ID_MASK) && (encodedAbility >> 15)) {
          for (oldAbilityIndex = 0; oldAbilityIndex < 32; oldAbilityIndex++) {
            if (encodedAbility ==
                beforeLevelUp->actionSlots[oldAbilityIndex].encodedAction)
              break;
          }
          if (oldAbilityIndex == 32) {
            UiWork_ClearValueNameTablesFar();
            UiWork_PushValueSlotFar(unit->classIndex, VALUE_CLASS_NAME);
            UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
            UiWork_PushValueSlotFar(encodedAbility & OWNER_ACTION_ID_MASK,
                                    VALUE_ABILITY_NAME);
            AudioCommand_PlayFar(SE_154);
            UiText_ShowMessageAndWaitCoreFar(MsgAbilityMastered);
            BattlePresentation_WaitForAdvance();
          }
        }
      }
      if ((s16)gains.hp != 0) {
        UiWork_PushValueSlotFar((s16)gains.hp, VALUE_NUMBER);
        UiText_ShowMessageAndWaitCoreFar(MsgMaxHpRises);
        BattlePresentation_WaitForAdvance();
      }
      if ((s16)gains.pp != 0) {
        UiWork_PushValueSlotFar((s16)gains.pp, VALUE_NUMBER);
        UiText_ShowMessageAndWaitCoreFar(MsgMaxPpRises);
        BattlePresentation_WaitForAdvance();
      }
      if ((s16)gains.atk != 0) {
        UiWork_PushValueSlotFar((s16)gains.atk, VALUE_NUMBER);
        UiText_ShowMessageAndWaitCoreFar(MsgAttackRises);
        BattlePresentation_WaitForAdvance();
      }
      if ((s16)gains.def != 0) {
        UiWork_PushValueSlotFar((s16)gains.def, VALUE_NUMBER);
        UiText_ShowMessageAndWaitCoreFar(MsgDefenseRises);
        BattlePresentation_WaitForAdvance();
      }
      if ((s16)gains.agi != 0) {
        UiWork_PushValueSlotFar((s16)gains.agi, VALUE_NUMBER);
        UiText_ShowMessageAndWaitCoreFar(MsgAgilityRises);
        BattlePresentation_WaitForAdvance();
      }
      if ((s16)gains.luck != 0) {
        UiWork_PushValueSlotFar((s16)gains.luck, VALUE_NUMBER);
        UiText_ShowMessageAndWaitCoreFar(MsgLuckRises);
        BattlePresentation_WaitForAdvance();
      }
    }
  }
  MemFree(beforeLevelUp);
  if (spoils->coins != 0) {
    UiWork_PushValueSlotFar(spoils->coins, VALUE_NUMBER);
    UiText_ShowMessageAndWaitCoreFar(MsgCoinsGained);
    Party_AdjustCoinsFar(spoils->coins);
    BattlePresentation_WaitForAdvance();
  }
  overflowItem = &gOverflowItem;
  for (;;) {
    highestItemSlot = -1;
    highestItemPriority = -1;
    for (slot = 0; slot <= 3; slot++) {
      if (spoils->items[slot] != 0) {
        itemPriority = Item_EncodeBankedId(spoils->items[slot]);
        if (itemPriority >= highestItemPriority) {
          highestItemPriority = itemPriority;
          highestItemSlot = slot;
        }
      }
    }
    if (highestItemSlot == -1)
      break;
    UiWork_PushValueSlotFar(spoils->items[highestItemSlot], VALUE_ITEM_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgItemGained);
    BattlePresentation_WaitForAdvance();
    if (PartyInventory_AddFar(spoils->items[highestItemSlot]) == -1) {
      *overflowItem = spoils->items[highestItemSlot];
      break;
    }
    spoils->items[highestItemSlot] = 0;
  }
}

// Reserved empty routine used by the debug battle menu.
void Battle_ReservedNoOp2A08(void) {}
