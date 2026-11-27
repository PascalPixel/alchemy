// The spoils: coins, experience, items and level ups. Part of battle.c.

// What a level up raised, as places in the halfwords gmUnitTryLevelUp fills (LEVEL_UP_RESULT's
// fields, read here as an array): each is shown when it is not 0.
typedef enum LEVEL_UP_GAIN
{
    GAIN_HP = 2,
    GAIN_PP,
    GAIN_ATTACK,
    GAIN_DEFENSE,
    GAIN_AGILITY,
    GAIN_LUCK,
    GAIN_COUNT
} LEVEL_UP_GAIN;

// Returns an item's rank among the spoils, by which the four kept are chosen and handed out best
// first: its item id (INVENTORY_ITEM_ID_MASK's nine bits), with ITEM_RARE's bit above it and
// ITEM_INDISPENSABLE's above that; 0 for none.
Uint32 btSpoilsRankItem(Uint32 inventoryEntry)
{
    Uint32 priority = 0;
    Uint32 itemId   = inventoryEntry & INVENTORY_ITEM_ID_MASK;
    if (itemId == 0)
        return 0;
    {
        Uint8 flags = gmItemGetDirectStub(itemId)->flags;

        if (flags & ITEM_INDISPENSABLE)
            priority = 1;
        priority <<= 1;
        if (flags & ITEM_RARE)
            priority++;
        priority <<= 9;
        priority  += itemId;
    }
    return priority;
}

// Above every item's rank: the search for the lowest of the spoils' four starts here.
#define SPOILS_RANK_CEILING (1 << 30)

// Empties the battle's spoils and the party's pending item. Called by btRun.
void btSpoilsClear(void)
{
    BATTLE_STATE  *battle = RamHeapSlots->battleState;
    BATTLE_SPOILS *spoils = &battle->spoils;
    Uint16        *items;
    Sint32         index;

    MainRoutineStateWork.pendingItem = 0;
    spoils->coins                    = 0;
    spoils->experience               = 0;
    spoils->defeated                 = 0;
    items                            = battle->spoils.items;
    for (index = 3; index >= 0; index--)
        items[index] = 0;
}

// The name suffixes the scan reads back (NAME_SUFFIX_COUNT of them): the digits 1 to 9 outside
// Japan, and the letters A to Z in the Japanese release.
#define FIRST_DIGIT_BASE  JAPANESE_OR('A', '1')
#define FIRST_DIGIT_COUNT JAPANESE_OR(26, 9)

// Returns an enemy name's suffix: its first digit less one (its first letter's place in the
// alphabet in Japanese), or FIRST_DIGIT_COUNT when it has none.
Sint32 btStringFindFirstDigit(Uint8 *name)
{
    Sint32 i;

    for (i = 0; i < UNIT_NAME_SIZE - 1; i++)
    {
        if ((Uint8)(name[i] - FIRST_DIGIT_BASE) <= FIRST_DIGIT_COUNT - 1)
            return name[i] - FIRST_DIGIT_BASE;
    }
    return FIRST_DIGIT_COUNT;
}

// Returns an enemy's coins with the Djinn bonus: its own, plus a six-sided die for every ten levels
// and one more, at least 30 percent of its own.
static __inline__ Sint32 btEnemyCalculateCoinReward(UNIT *unit, Uint16 *reward)
{
    Sint32 rollIndex;
    Sint32 bonus = 0;
    Sint32 base;
    Sint32 minimumBonus;

    for (rollIndex = 0; rollIndex < (Uint8)(unit->level / 10U) + 1; rollIndex++)
        bonus += (syRandomBelow(6)) + 1;
    base         = *reward;
    minimumBonus = base * 3 / 10;
    if (bonus < minimumBonus)
        bonus = minimumBonus;
    return bonus + base;
}

// Returns an enemy's experience with the Djinn bonus: its own plus a four-sided die for every ten
// levels and one more, at least 30 percent more.
static __inline__ Sint32 btEnemyCalculateExperienceReward(UNIT *unit, Uint16 *reward)
{
    Sint32 rollIndex;
    Sint32 bonus = 0;
    Sint32 base;
    Sint32 minimumBonus;

    for (rollIndex = 0; rollIndex < (Uint8)(unit->level / 10U) + 1; rollIndex++)
        bonus += (syRandomBelow(4)) + 1;
    base         = *reward;
    minimumBonus = base * 3 / 10;
    if (bonus < minimumBonus)
        bonus = minimumBonus;
    return bonus + base;
}

// Counts a fallen enemy and adds its spoils, The Broken Seal's btEnemyRecordDefeat with the battle
// state from its heap slot. This game also keeps the last two defeated enemies (their ids and
// btStringFindFirstDigit's first digit of their names, alternating slots, with a count up to 2) and
// sets the defeated flag only for the enemies that have one, below ENEMY_DEFEATED_FLAG_COUNT.
Sint32 btEnemyRecordDefeat(Sint32 unitId, Sint32 djinnBonus)
{
    UNIT                   *unit;
    const ENEMY_DEFINITION *enemy;
    BATTLE_STATE           *battle;
    BATTLE_SPOILS          *spoils;
    Sint32                  itemIndex;
    Sint32                  dropShift;
    Sint32                  classSlot;
    Sint32                  lowestItemPriority;
    Sint32                  lowestSlot;
    Sint32                  itemPriority;
    Sint32                  slot;
    Sint32                  classIndex;

    unit   = syUnitGet(unitId);
    battle = RamHeapSlots->battleState;
    spoils = &battle->spoils;
    if (unitId >= 0 && unitId <= ALLY_MAX)
        return -1;
    if (unit->classId != CLASS_NONE)
        return -2;
    // Queued for Regen Dance: the name letter is the first digit of the name.
    slot = spoils->queueHead    = (spoils->queueHead + 1) % 2;
    spoils->queuedSummons[slot] = unit->enemyId;
    spoils->queuedLetters[slot] = btStringFindFirstDigit((Uint8 *)unit);
    if (spoils->queueCount <= 1)
        spoils->queueCount++;
    classSlot  = 0;
    classIndex = 0;
    // UB: the search runs to a sixth kind, one past enemyKinds: it reads kindsEnd, the halfword
    // after them.
    if (battle->enemyKinds[0] != unit->enemyId)
    {
        do
        {
            if (++classIndex > (ENEMY_BATTLE_SIZE - 1))
                break;
        } while (battle->enemyKinds[classIndex] != unit->enemyId);
    }
    if (classIndex != ENEMY_BATTLE_SIZE)
        classSlot = classIndex;
    // The first class to fall names the victory message. XXX (no evidence): whether
    // DEFEAT_BY_PARTY_MEMBER was meant; no state is 2, so the test never fails.
    if (battle->defeatState != 2)
    {
        if (classSlot < battle->firstDefeated)
            battle->firstDefeated = classSlot;
        if (spoils->defeated)
            battle->defeatState = DEFEAT_SEVERAL;
    }
    spoils->defeated++;
    if (syGameFlagGet(FLAG_MONSTER_BATTLE))
        return 0;
    if (unit->enemyId < ENEMY_DEFEATED_FLAG_COUNT)
        syGameFlagSet(ENEMY_DEFEATED_FLAG(unit->enemyId));
    enemy = gmEnemyGetDefinitionStub(unit->enemyId);
    if (djinnBonus != 0)
    {
        if (enemy->coins != 0)
            spoils->coins += btEnemyCalculateCoinReward(unit, &enemy->coins);
        if (enemy->experience != 0)
            spoils->experience += btEnemyCalculateExperienceReward(unit, &enemy->experience);
    }
    else
    {
        spoils->coins      += enemy->coins;
        spoils->experience += enemy->experience;
    }
    if (enemy->item == 0 || enemy->itemChance == 0)
        return 0;
    itemIndex = 0;
    if (spoils->items[0] != enemy->item)
    {
        do
        {
            if (++itemIndex > 3)
                break;
        } while (spoils->items[itemIndex] != enemy->item);
    }
    if (itemIndex == 4)
    {
        dropShift = enemy->itemChance;
        if (djinnBonus != 0)
            dropShift -= 2;
        if (dropShift < 0)
            dropShift = 0;
        // Twice toFIXED(1) >> n against a 16-bit random number: 1 in 2^(n - 1).
        if ((toFIXED(2) >> dropShift) > (gmBattleRandomStub() & 0xffff))
        {
            lowestSlot         = -1;
            lowestItemPriority = SPOILS_RANK_CEILING;
            for (itemIndex = 0; itemIndex <= 3; itemIndex++)
            {
                itemPriority = btSpoilsRankItem(spoils->items[itemIndex]);
                if (itemPriority < lowestItemPriority)
                {
                    lowestItemPriority = itemPriority;
                    lowestSlot         = itemIndex;
                }
            }
            if ((Sint32)btSpoilsRankItem(enemy->item) > lowestItemPriority)
                spoils->items[lowestSlot] = enemy->item;
        }
    }
    return 0;
}

// Hands out the spoils after a victory: experience to each party member still standing, with every
// level gained, then the coins, then the items best first.
void btAwardSpoils(void)
{
    BATTLE_SPOILS *spoils;
    Sint32         living;
    Uint16        *list;
    Sint32         count;
    Sint32         i;
    Sint32         unitId;
    UNIT          *unit;
    UNIT          *backup;
    Sint32         entry;
    Uint32         learned;
    Sint32         j;
    Sint32         best;
    Sint32         bestSlot;
    Sint32         slot;
    Uint16        *found;
    Sint32         value;
    Sint16         gains[GAIN_COUNT];
    Uint16         units[10];

    spoils = &RamHeapSlots->battleState->spoils;
    if (spoils->experience != 0)
    {
        mnMessageArgumentPushStub(spoils->experience, MESSAGE_ARGUMENT_NUMBER);
        // "You got N experience points."
        mnBattleMessageShowAndWaitStub((Sint32)MsgGotExperience);
        btMessageWaitForAdvance();
    }
    list   = units;
    living = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, list);
    count  = btUnitListLivingWithReserve(BATTLE_SIDE_MASK_PARTY, list);
    backup = syHeapAllocateEWRAM(sizeof(UNIT));
    for (i = 0; i < count; i++)
    {
        unitId = list[i];
        unit   = syUnitGet(unitId);
        if (i >= living)
            unit->experience += (Uint32)spoils->experience >> 1;
        else
            unit->experience += spoils->experience;
        while (syWordsCopyEntry(backup, unit, sizeof(UNIT)),
               gmUnitTryLevelUpStub(unitId, gains) != 0)
        {
            sySoundPlayStub(MUS_LEVEL_UP);
            mnMessageArgumentClearAllStub();
            mnMessageArgumentPushStub(unit->classId, MESSAGE_ARGUMENT_CLASS);
            mnMessageArgumentPushStub(list[i], MESSAGE_ARGUMENT_ACTOR);
            mnMessageArgumentPushStub(unit->level, MESSAGE_ARGUMENT_NUMBER);
            // "<name> is now a level N ..."
            mnBattleMessageShowAndWaitStub((Sint32)MsgLevelUp);
            btMessageWaitForAdvance();
            for (entry = 0; entry < PSYNERGY_SLOTS; entry++)
            {
                learned = unit->psynergy[entry].encodedAction;
                // A class ability the copy lacks is newly learned.
                if ((learned & ACTION_ID_MASK) && (learned >> PSYNERGY_FROM_CLASS_SHIFT))
                {
                    for (j = 0; j < PSYNERGY_SLOTS; j++)
                    {
                        if (learned == backup->psynergy[j].encodedAction)
                            break;
                    }
                    if (j == PSYNERGY_SLOTS)
                    {
                        mnMessageArgumentClearAllStub();
                        mnMessageArgumentPushStub(unit->classId, MESSAGE_ARGUMENT_CLASS);
                        mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
                        mnMessageArgumentPushStub(learned & ACTION_ID_MASK,
                                                  MESSAGE_ARGUMENT_ABILITY);
                        sySoundPlayStub(SE_CAST);
                        // "Mastered the <class>'s <ability>"
                        mnBattleMessageShowAndWaitStub((Sint32)MsgMasteredAbility);
                        btMessageWaitForAdvance();
                    }
                }
            }
            if (gains[GAIN_HP] != 0)
            {
                mnMessageArgumentPushStub(gains[GAIN_HP], MESSAGE_ARGUMENT_NUMBER);
                // "Maximum HP rises by N!"
                mnBattleMessageShowAndWaitStub((Sint32)MsgMaxHpRises);
                btMessageWaitForAdvance();
            }
            if (gains[GAIN_PP] != 0)
            {
                mnMessageArgumentPushStub(gains[GAIN_PP], MESSAGE_ARGUMENT_NUMBER);
                mnBattleMessageShowAndWaitStub((Sint32)MsgMaxPpRises);
                btMessageWaitForAdvance();
            }
            if (gains[GAIN_ATTACK] != 0)
            {
                mnMessageArgumentPushStub(gains[GAIN_ATTACK], MESSAGE_ARGUMENT_NUMBER);
                mnBattleMessageShowAndWaitStub((Sint32)MsgAttackRises);
                btMessageWaitForAdvance();
            }
            if (gains[GAIN_DEFENSE] != 0)
            {
                mnMessageArgumentPushStub(gains[GAIN_DEFENSE], MESSAGE_ARGUMENT_NUMBER);
                mnBattleMessageShowAndWaitStub((Sint32)MsgDefenseRises);
                btMessageWaitForAdvance();
            }
            if (gains[GAIN_AGILITY] != 0)
            {
                mnMessageArgumentPushStub(gains[GAIN_AGILITY], MESSAGE_ARGUMENT_NUMBER);
                mnBattleMessageShowAndWaitStub((Sint32)MsgAgilityRises);
                btMessageWaitForAdvance();
            }
            if (gains[GAIN_LUCK] != 0)
            {
                mnMessageArgumentPushStub(gains[GAIN_LUCK], MESSAGE_ARGUMENT_NUMBER);
                // "Luck rises by N!"
                mnBattleMessageShowAndWaitStub((Sint32)MsgLuckRises);
                btMessageWaitForAdvance();
            }
        }
    }
    syHeapRelease(backup);
    if (spoils->coins != 0)
    {
        mnMessageArgumentPushStub(spoils->coins, MESSAGE_ARGUMENT_NUMBER);
        // "You got N coins."
        mnBattleMessageShowAndWaitStub((Sint32)MsgGotCoins);
        gmPartyAdjustCoinsStub(spoils->coins);
        btMessageWaitForAdvance();
    }
    found = &MainRoutineStateWork.pendingItem;
    for (;;)
    {
        bestSlot = -1;
        best     = -1;
        for (slot = 0; slot <= 3; slot++)
        {
            if (spoils->items[slot] != 0)
            {
                value = btSpoilsRankItem(spoils->items[slot]);
                if (value >= best)
                {
                    best     = value;
                    bestSlot = slot;
                }
            }
        }
        if (bestSlot == -1)
            break;
        mnMessageArgumentPushStub(spoils->items[bestSlot], MESSAGE_ARGUMENT_ITEM);
        // "You got <item>."
        mnBattleMessageShowAndWaitStub((Sint32)MsgGotItem);
        btMessageWaitForAdvance();
        if (gmPartyInventoryAddStub(spoils->items[bestSlot]) == -1)
        {
            *found = spoils->items[bestSlot];
            break;
        }
        spoils->items[bestSlot] = 0;
    }
}

#undef FIRST_DIGIT_BASE
#undef FIRST_DIGIT_COUNT
