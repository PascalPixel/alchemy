// Enemy formations: the enemies chosen, levelled and told apart by name suffixes, and each enemy
// class's sprite, palette, size and attack effect. Part of battle.c.

// The most formations a monster battle chooses among.
#define FORMATION_POOL_SIZE       32
// How far above the party's level a monster battle's formation may be.
#define FORMATION_LEVEL_MARGIN    3
// Above any formation's average level: where the search for the pool's lowest entry starts.
#define FORMATION_SCORE_ABOVE_ALL 999

// Chooses a monster battle's formation for the party's level plus its winning streak, at random
// among those at most FORMATION_LEVEL_MARGIN above it, and returns it with the levels its enemies
// must gain in outMargin.
Sint32 btFormationSelectLevelMatchedCandidate(Sint32 *outMargin)
{
    Sint32               matchCount = 0;
    FORMATION_CANDIDATE *pool       = syHeapAllocateEWRAM(FORMATION_POOL_SIZE * sizeof *pool);
    Uint16               party[8];
    Sint32               targetLevel = 0;
    Sint32               unitCount   = btPartyPrepareMembers(party);
    Sint32               i;
    Sint32               poolIndex;
    Sint32               result;

    if (unitCount > 0)
    {
        Uint16 *unitCursor = party;

        i                  = unitCount;
        do
        {
            Uint8 level = syUnitGet(*unitCursor)->level;

            i--;
            unitCursor++;
            targetLevel += level;
        } while (i != 0);
    }

    // The flag byte is -1 once a series has ended, which takes a level off.
    targetLevel  = targetLevel / unitCount;
    targetLevel += (Sint8)syGameFlagGetByte(FLAG_MONSTER_BATTLE_WINS);
    if (targetLevel <= 0)
        targetLevel = 1;
    if (targetLevel > UNIT_LEVEL_MAX)
        targetLevel = UNIT_LEVEL_MAX;

    for (i = 0; i < FORMATION_POOL_SIZE; i++)
        pool[i].score = -1;

    // A boss beaten in another of its forms marks one form beaten: the Serpent's four-beam one, the
    // Knight's second, Moapa's third, and the Doom Dragon's first only once its last has fallen.
    if (syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT)))
        syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_FOUR_BEAMS));
    if (syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_ONE_BEAM)))
        syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_FOUR_BEAMS));
    if (syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_TWO_BEAMS)))
        syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_FOUR_BEAMS));
    if (syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_THREE_BEAMS)))
        syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_FOUR_BEAMS));
    if (syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_KNIGHT)))
        syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_KNIGHT_B));
    if (syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_MOAPA)))
        syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_MOAPA_C));
    if (syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_MOAPA_B)))
        syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_MOAPA_C));
    syGameFlagClear(ENEMY_DEFEATED_FLAG(ENEMY_DOOM_DRAGON_A));
    if (syGameFlagGet(ENEMY_DEFEATED_FLAG(ENEMY_DOOM_DRAGON_H)))
        syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_DOOM_DRAGON_A));

    // Forget the defeated classes the monster battles exclude.
    for (i = 0; i < ARRAY_COUNT(BattleFormationExcludedEnemies); i++)
        syGameFlagClear(ENEMY_DEFEATED_FLAG(BattleFormationExcludedEnemies[i]));
    // BUG: the three enemies formations 241 to 248 are made of are set by their enemy ids, not
    // their defeated flags, so they stay unavailable unless beaten, and FLAG_DJINNI(DJINNI_SPRITZ),
    // FLAG_DJINNI(DJINNI_ETHER) and flag 0xe6 are set instead.
#if FIX_MONSTER_BATTLE_DEFEATED_FLAGS
    syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_PUNCH_ANT));
    syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_GIANT_BAT));
    syGameFlagSet(ENEMY_DEFEATED_FLAG(ENEMY_WILD_WOLF));
#else
    syGameFlagSet(ENEMY_PUNCH_ANT);
    syGameFlagSet(ENEMY_GIANT_BAT);
    syGameFlagSet(ENEMY_WILD_WOLF);
#endif

    // Each qualifying formation replaces the pool's lowest entry; empty entries score -1, so the
    // pool fills first.
    for (i = 0; (Uint32)i < FORMATION_COUNT; i++)
    {
        Sint32 score = btFormationAverageLevel(i);

        if (score >= 0 && score <= targetLevel + FORMATION_LEVEL_MARGIN)
        {
            Sint32 lowestIndex = -1;
            Sint32 lowestLevel;

            for (lowestLevel = FORMATION_SCORE_ABOVE_ALL, poolIndex = 0;
                 poolIndex < FORMATION_POOL_SIZE;
                 poolIndex++)
            {
                if (pool[poolIndex].score < lowestLevel)
                {
                    lowestLevel = pool[poolIndex].score;
                    lowestIndex = poolIndex;
                }
            }

            if (lowestIndex >= 0)
            {
                pool[lowestIndex].score    = score;
                pool[lowestIndex].recordId = i;
                matchCount++;
            }
        }
    }

    // matchCount includes formations evicted from the pool.
    if (matchCount > FORMATION_POOL_SIZE)
        matchCount = FORMATION_POOL_SIZE;

    if (matchCount != 0)
    {
        FORMATION_CANDIDATE *chosen = &pool[(matchCount * syRandom()) >> 16];

        result                      = chosen->recordId;
        *outMargin                  = targetLevel - chosen->score;
    }
    else
    {
        *outMargin = matchCount;
        result     = 1;
    }

    syHeapRelease(pool);
    return result;
}

// What an enemy gains a level (btEnemyAdjustLevel), each stat in tenths of a point: 9.7 HP, 1.5
// PP, 12.3 Attack, 3.3 Defense and 5.1 Agility; and its element power in whole points.
#define HP_GAIN_TENTHS      97
#define PP_GAIN_TENTHS      15
#define ATTACK_GAIN_TENTHS  123
#define DEFENSE_GAIN_TENTHS 33
#define AGILITY_GAIN_TENTHS 51
#define ELEMENT_POWER_GAIN  15

// Raises an enemy by levels, or lowers it for a negative count, each stat by a fixed amount a level
// and no lower than 70 percent of what it was.
UNREAD_RESULT btEnemyAdjustLevel(Sint32 unitId, Sint32 levels)
{
    BATTLE_BASE_STATS *base = syHeapAllocateEWRAM(sizeof *base);
    UNIT              *unit = syUnitGet(unitId);
    Sint32             value;
    Sint32             element;

    syWordsCopyEntry(base, &unit->baseHp, sizeof *base);

    value = unit->baseHp + levels * HP_GAIN_TENTHS / 10;
    CLAMP(value, base->baseHp * 7 / 10, UNIT_ENEMY_HP_MAX);
    unit->baseHp = value;

    value        = unit->basePp + levels * PP_GAIN_TENTHS / 10;
    CLAMP(value, base->basePp * 7 / 10, UNIT_ENEMY_PP_MAX);
    unit->basePp = value;

    value        = unit->baseAttack + levels * ATTACK_GAIN_TENTHS / 10;
    CLAMP(value, base->baseAttack * 7 / 10, UNIT_STAT_MAX);
    unit->baseAttack = value;

    value            = unit->baseDefense + levels * DEFENSE_GAIN_TENTHS / 10;
    CLAMP(value, base->baseDefense * 7 / 10, UNIT_STAT_MAX);
    unit->baseDefense = value;

    value             = unit->baseAgility + levels * AGILITY_GAIN_TENTHS / 10;
    CLAMP(value, base->baseAgility * 7 / 10, UNIT_STAT_MAX);
    unit->baseAgility = value;

    for (element = 0; element < ELEMENT_COUNT; element++)
    {
        value = unit->baseElement[element].power + levels * ELEMENT_POWER_GAIN;
        CLAMP(value, base->element[element].power * 7 / 10, UNIT_ELEMENT_MAX);
        unit->baseElement[element].power = value;
    }

    unit->level += levels;
    gmUnitCalculateStub(unitId);
    syHeapRelease(base);
}

// Raises a unit's base HP by half and base Attack and Defense by a quarter, at least 70 percent of
// the old values and at most UNIT_ENEMY_HP_MAX and UNIT_STAT_MAX: an enemy on the HARD level
// (FLAG_HARD_LEVEL).
void btUnitStrengthen(Sint32 unitId)
{
    BATTLE_BASE_STATS *saved = syHeapAllocateEWRAM(sizeof *saved);
    UNIT              *unit  = syUnitGet(unitId);
    Sint32             value;

    syWordsCopyEntry(saved, &unit->baseHp, sizeof *saved);
    value = unit->baseHp * 3 / 2;
    CLAMP(value, saved->baseHp * 7 / 10, UNIT_ENEMY_HP_MAX);
    unit->baseHp = value;
    value        = unit->baseAttack * 5 / 4;
    CLAMP(value, saved->baseAttack * 7 / 10, UNIT_STAT_MAX);
    unit->baseAttack = value;
    value            = unit->baseDefense * 5 / 4;
    CLAMP(value, saved->baseDefense * 7 / 10, UNIT_STAT_MAX);
    unit->baseDefense = value;
    gmUnitCalculateStub(unitId);
    syHeapRelease(saved);
}

// The suffixes a class's mask holds, one bit each.
#define NAME_SUFFIX_MASK_BITS 32

// Gives an enemy of the class its name suffix, which tells it from others of its kind: digits
// abroad and the alphabet in Japanese. Returns -1 when five classes are already listed.
Sint32 btEnemyClaimNameLetter(Sint32 classId, Sint32 unused)
{
    BATTLE_STATE *battle;
    Sint32        classCount;
    Sint32        classIndex;
    Sint32        attempt;

    battle     = RamHeapSlots->battleState;
    classCount = battle->enemyKindCount;
    for (classIndex = 0; classIndex < classCount; classIndex++)
    {
        if (battle->enemyKinds[classIndex] == classId)
            break;
    }
    if (classIndex != classCount)
    {
        attempt = 0;
        // The second of its class: suffixes 0 and 1 both taken.
        if (battle->lastNameLetter[classIndex] < 0)
        {
            battle->lastNameLetter[classIndex]  = 1;
            battle->nameLetterMasks[classIndex] = 3;
            return ENEMY_LETTER_FIRST_TOO | 1;
        }
        // The next suffix after the last given that is free; after 32 tries the last one tried is
        // reused.
        for (; attempt < NAME_SUFFIX_MASK_BITS; attempt++)
        {
            battle->lastNameLetter[classIndex] =
                (battle->lastNameLetter[classIndex] + 1) % NAME_SUFFIX_COUNT;
            if ((battle->nameLetterMasks[classIndex] & (1 << battle->lastNameLetter[classIndex]))
                == 0)
                break;
        }
        battle->nameLetterMasks[classIndex] |= 1 << battle->lastNameLetter[classIndex];
        return battle->lastNameLetter[classIndex];
    }
    if (classCount <= 4)
    {
        battle->lastNameLetter[classCount]  = -1;
        battle->enemyKinds[classCount]      = classId;
        battle->nameLetterMasks[classCount] = 0;
        battle->enemyKindCount              = classCount + 1;
        return NAME_SUFFIX_COUNT;
    }
    return -1;
}

// Frees a departing enemy's name suffix for the next of its class.
UNREAD_RESULT btEnemyReleaseNameLetter(Sint32 unitId)
{
    BATTLE_STATE *battle = RamHeapSlots->battleState;
    UNIT         *unit;
    Sint32        count = battle->enemyKindCount;
    Sint32        index;
    Sint32        nameLength;
    Sint32        suffix;
    Sint32        classId;

    unit = syUnitGet(unitId);
    if (unit->classId != CLASS_NONE)
        return;

    classId = unit->enemyId;
    for (index = 0; index < count; index++)
    {
        if (battle->enemyKinds[index] == classId)
            break;
    }
    if (index == count || battle->nameLetterMasks[index] == 0)
        return;

    for (nameLength = 0; nameLength < UNIT_NAME_SIZE - 1; nameLength++)
    {
        if (unit->name[nameLength] == 0)
            break;
    }

    // UB: an empty name shifts by NAME_SUFFIX_MASK_BITS, which the ARM turns into 0, so no suffix
    // is freed. A name with no suffix instead uses its last character as one.
    suffix = NAME_SUFFIX_MASK_BITS;
#if FIX_UNDEFINED_SHIFT
    if (nameLength == 0)
        return;
#endif
    if (nameLength > 0)
        suffix = unit->name[nameLength - 1] - ENEMY_NAME_LETTER_FIRST;
    battle->nameLetterMasks[index] &= ~(1 << suffix);
}

// Gives the first enemy of the class on the field the first suffix (0), appended to its name, once
// a second of its class arrives.
Sint32 btEnemyLetterFirstOfKind(Sint32 enemyId)
{
    UNIT  *unit;
    Sint32 enemyIndex;
    Sint32 nameIndex;
    Sint32 terminatorIndex;
    Uint8  nameByte;

    for (enemyIndex = 0; enemyIndex < ENEMY_BATTLE_SIZE; enemyIndex++)
    {
        unit     = syUnitGet(enemyIndex + FIRST_ENEMY_UNIT);
        nameByte = unit->side;
        if (nameByte != BATTLE_SIDE_ENEMY)
            continue;
        if (unit->enemyId != enemyId)
            continue;
        // nameByte still holds BATTLE_SIDE_ENEMY here, used as the index after the suffix;
        // nameIndex holds the terminator, 0.
        nameIndex = 0;
        if (unit->name[0] == 0)
        {
            unit->name[0]        = ENEMY_NAME_LETTER_FIRST;
            unit->name[nameByte] = nameIndex;
            return;
        }
        // The name's end, then the suffix and a new terminator after it.
    scan_slots:
        nameIndex++;
        if (nameIndex > UNIT_NAME_SIZE - 2)
            return;
        nameByte = unit->name[nameIndex];
        if (nameByte != 0)
            goto scan_slots;
        terminatorIndex             = nameIndex + 1;
        unit->name[nameIndex]       = ENEMY_NAME_LETTER_FIRST;
        unit->name[terminatorIndex] = nameByte;
        return;
    }
}

// Returns the class of a random member of the formation that may appear.
Sint32 btFormationGetRandomMember(Sint32 recordId)
{
    Sint32    *choicesBase;
    Sint32     choices[FORMATION_MEMBERS];
    FORMATION *record;
    Sint32    *choiceCursor;
    Uint8     *maximumCount;
    Uint16    *memberId;
    Sint32     remaining;
    Uint32     count;

    count = 0;
    if ((Uint32)recordId >= FORMATION_COUNT)
        recordId = 1;

    record       = &Formations[recordId];
    choicesBase  = choices;
    memberId     = record->memberIds;
    maximumCount = record->maxCounts;
    choiceCursor = choicesBase;
    for (remaining = 0; remaining < FORMATION_MEMBERS; remaining++)
    {
        if (*maximumCount++ != 0)
        {
            Sint32 id = *memberId;

            count++;
            *choiceCursor++ = id + ENEMY_ID_FIRST;
        }
        memberId++;
    }

    return choices[(syRandom() * count) >> 16];
}

// The enemy sprite slots an enemy of the class takes: one for a small enemy, two for any other.
#define ENEMY_SLOTS_TAKEN(classId) (2 - (btSpriteIsSmall(classId) != 0))

// Builds the battle's enemies from a formation and returns how many there are: each member its
// fewest times and at random up to its most while the sprite slots hold them, laid out as the
// formation says.
Sint32 btFormationBuildEnemyList(Sint32 recordId)
{
    Uint16       *enemyClasses;
    BATTLE_STATE *battle;
    FORMATION    *record;
    Sint32        freeSpriteSlots;
    Sint32        enemyCount;
    Sint32        levelAdjustment;
    Uint16        listBuf[UNIT_MAX];
    Sint32        extraCounts[FORMATION_MEMBERS];
    Sint32        memberCounts[FORMATION_MEMBERS];
    Sint32        memberOrder[FORMATION_MEMBERS];
    Bool          changed;
    Sint32        additionalRoom;
    Sint32        spriteSlots;
    Sint32        fittingCount;
    Sint32        classId;
    Sint32        remainingKinds;
    Sint32        copies;
    Uint32        i;
    Sint32        memberIndex;

    battle                 = (BATTLE_STATE *)RamHeapSlots->battleState;
    levelAdjustment        = 0;
    battle->enemyKindCount = 0;
    enemyClasses           = listBuf;
    if (syGameFlagGet(FLAG_MONSTER_BATTLE))
        recordId = btFormationSelectLevelMatchedCandidate(&levelAdjustment);
    if ((Uint32)recordId >= FORMATION_COUNT)
        recordId = 1;
    record = &Formations[recordId];
    for (memberIndex = 0; memberIndex < FORMATION_MEMBERS && record->minCounts[memberIndex] == 0;
         memberIndex++)
        ;
    if (memberIndex == FORMATION_MEMBERS)
        record = &Formations[1];

    freeSpriteSlots = ENEMY_SPRITE_SLOTS;
    enemyCount      = 0;
    for (i = 0; i < FORMATION_MEMBERS; i++)
    {
        if (record->minCounts[i] != 0)
        {
            Sint32 requiredSlots  = ENEMY_SLOTS_TAKEN(record->memberIds[i] + ENEMY_ID_FIRST);

            freeSpriteSlots      -= requiredSlots * record->minCounts[i];
        }
    }

    for (i = 0; i < FORMATION_MEMBERS; i++)
    {
        Sint32 minimum;
        Sint32 maximum;

        memberCounts[i] = minimum = record->minCounts[i];
        maximum                   = record->maxCounts[i];
        additionalRoom            = maximum - minimum;
        if (additionalRoom > 0)
        {
            Sint32 memberSlots = ENEMY_SLOTS_TAKEN(record->memberIds[i] + ENEMY_ID_FIRST);

            fittingCount       = freeSpriteSlots / memberSlots;
            if (fittingCount < additionalRoom)
                additionalRoom = fittingCount;
            extraCounts[i] = ((additionalRoom + 1) * syRandom()) >> 16;
        }
        else
        {
            extraCounts[i] = 0;
        }
    }

    do
    {
        changed = FALSE;
        for (i = 0; i < FORMATION_MEMBERS; i++)
        {
            classId = record->memberIds[i] + ENEMY_ID_FIRST;
            if (extraCounts[i] != 0)
            {
                spriteSlots = ENEMY_SLOTS_TAKEN(classId);
                if (spriteSlots > freeSpriteSlots)
                {
                    extraCounts[i] = 0;
                }
                else
                {
                    memberCounts[i]++;
                    extraCounts[i]--;
                    freeSpriteSlots -= spriteSlots;
                    changed          = TRUE;
                }
            }
        }
    } while (changed);

    battle->layout = record->layout;
    switch (battle->layout)
    {
        case FORMATION_LAYOUT_SHUFFLED:
            for (i = 0; i < FORMATION_MEMBERS; i++)
                memberOrder[i] = i;
            for (i = 0; i <= 9; i++)
            {
                Sint32 first        = syRandomBelow(FORMATION_MEMBERS);
                Sint32 second       = syRandomBelow(FORMATION_MEMBERS);
                Sint32 swap         = memberOrder[first];

                memberOrder[first]  = memberOrder[second];
                memberOrder[second] = swap;
            }
            for (i = 0; i < FORMATION_MEMBERS; i++)
            {
                Sint32 copyIndex;
                Sint32 slot = memberOrder[i];

                for (copyIndex = 0; copyIndex < memberCounts[slot]; copyIndex++)
                    enemyClasses[enemyCount++] = record->memberIds[slot] + ENEMY_ID_FIRST;
            }
            break;
        case FORMATION_LAYOUT_INTERLEAVED:
            for (;;)
            {
                Sint32 member;

                remainingKinds = 0;
                for (i = 0; i < FORMATION_MEMBERS; i++)
                {
                    if (memberCounts[i] != 0)
                        memberOrder[remainingKinds++] = i;
                }
                if (remainingKinds == 0)
                    break;
                // One of the members still owed a copy, at random.
                member                     = memberOrder[(syRandom() * remainingKinds) >> 16];
                enemyClasses[enemyCount++] = record->memberIds[member] + ENEMY_ID_FIRST;
                memberCounts[member]--;
            }
            break;
        default:
            for (i = 0; i < FORMATION_MEMBERS; i++)
            {
                for (copies = 0; copies < memberCounts[i]; copies++)
                    enemyClasses[enemyCount++] = record->memberIds[i] + ENEMY_ID_FIRST;
            }
            break;
    }
    enemyClasses[enemyCount] = 0;
    battle->firstDefeated    = ENEMY_BATTLE_SIZE;
    battle->defeatState      = DEFEAT_ONE;
    for (i = FIRST_ENEMY_UNIT; i < (FIRST_ENEMY_UNIT + ENEMY_BATTLE_SIZE); i++)
        syWordsClearEntry(syUnitGet(i), BATTLE_UNIT_SIZE);

    i = 0;
    if (enemyClasses[0] != 0)
    {
        for (;;)
        {
            // Repeated enemy classes get distinct suffixes within this encounter.
            Sint32 suffix = btEnemyClaimNameLetter(enemyClasses[i], 1);
            UNIT  *unit;
            Sint32 enemyId;

            if (suffix & ENEMY_LETTER_FIRST_TOO)
                btEnemyLetterFirstOfKind(enemyClasses[i]);
            gmBattleUnitSetStub(i + FIRST_ENEMY_UNIT, enemyClasses[i], suffix & ENEMY_LETTER_MASK);
            unit = syUnitGet(i + FIRST_ENEMY_UNIT);
            if (levelAdjustment != 0)
                btEnemyAdjustLevel(i + FIRST_ENEMY_UNIT, levelAdjustment);
            else if (!syGameFlagGet(FLAG_MONSTER_BATTLE) && syGameFlagGet(FLAG_HARD_LEVEL))
                btUnitStrengthen(i + FIRST_ENEMY_UNIT);
            battle->forceField = 0;
            enemyId            = unit->enemyId;
            switch (enemyId)
            {
                case ENEMY_POSEIDON:
                case ENEMY_POSEIDON_SECOND_ACTION:
                case ENEMY_POSEIDON_3:
                    if (!syGameFlagGet(FLAG_MONSTER_BATTLE))
                        battle->forceField = 1;
                    break;
            }
            i++;
            if (i >= ENEMY_BATTLE_SIZE || enemyClasses[i] == 0)
                break;
        }
    }
    return i;
}

// Returns the class's defeat-sound kind, 0 to 4, or -1 for none; a fallen enemy's sound is
// SE_ENEMY_CRY plus it. Reads any index unchecked.
Uint32 btSpriteGetDefeatSound(Sint32 index)
{
    Uint32 kind = BattleSprites[index].defeatSound;

    if ((Sint32)kind > 4)
        kind = -1U;
    return kind;
}

// The last enemy with a row in BattleSprites (battle_sprites.c).
#define ENEMY_CLASS_LAST 386

// Returns the class's battle sprite, row 0's for a class out of range.
Sint32 btSpriteGetSprite(Sint32 index)
{
    if ((Uint32)index > ENEMY_CLASS_LAST)
        return BattleSprites[0].animation;
    return BattleSprites[index].animation;
}

// Returns the class's sprite palette. For a class out of range it returns row 0's sprite, not a
// palette.
Sint32 btSpriteGetPalette(Sint32 index)
{
    if ((Uint32)index > ENEMY_CLASS_LAST)
        return BattleSprites[0].animation;
    return BattleSprites[index].palette;
}

// Returns 1 for a small class, which takes one enemy sprite slot instead of two and stands closer
// to its neighbours; 0 out of range.
Sint32 btSpriteIsSmall(Sint32 index)
{

    Bool   result;
    Uint32 flags;

    if ((Uint32)index > ENEMY_CLASS_LAST)
        return 0;
    result = FALSE;
    flags  = BattleSprites[index].isSmall;
    if (flags)
        result = TRUE;
    return result;
}

// Returns the battle effect a class's plain attack shows, 1 for none or out of range.
Uint32 btSpriteGetAttackEffect(Uint32 index)
{
    Uint16 value;

    if (index > ENEMY_CLASS_LAST)
        return 1;
    value = BattleSprites[index].attackEffect;
    return value ? value : 1;
}

// Returns how high a class hops as it runs up to attack, in pixels; 0 out of range.
Uint32 btSpriteGetAttackHop(Uint32 index)
{
    Uint16 value;

    if (index > ENEMY_CLASS_LAST)
        return 0;
    value = BattleSprites[index].attackHop;
    return value ? value : 0;
}

// Returns 1 for a class whose blow lands with a heavier impact; 0 out of range.
Sint32 btSpriteHasHeavyImpact(Sint32 index)
{
    if ((Uint32)index > ENEMY_CLASS_LAST)
        return 0;
    return BattleSprites[index].heavyImpact;
}

// Returns the height an animation is drawn at over a class, in pixels, or 0 for its size's default;
// 0 out of range.
Uint32 btSpriteGetEffectHeight(Uint32 index)
{
    if (index > ENEMY_CLASS_LAST)
        return 0;
    return BattleSprites[index].effectHeight;
}
