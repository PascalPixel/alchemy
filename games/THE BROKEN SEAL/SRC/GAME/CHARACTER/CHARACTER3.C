#include "EDITION.H"
#include "GAME_FLAGS.H"
#include "PARTY_STATE.H"
#include "TYPES.H"
#include "CHARACTER.H"
#include "BATTLE_SUMMON.H"
#include "RUNTIME_INTERFACES.H"
#include "PRESET_TABLE.H"
#include "FIXED_MATH.H"
#include "OWNER_STATE.H"
#include "IWRAM_CALL.H"

s32 GameFlag_SetBit(s32);
void GameFlag_ClearBit(s32);

struct TradeFlags {
    u32 flags;
};

extern u8 Summon_OrderList[16];
extern const struct SummonDefinition Summon_DefinitionTable[];

extern s32 Data_08088db8[];
extern struct PresetValues Enemy_ElementPresetTable[];

s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 *output);

struct ElementLevelBonus {
    u16 power;
    u16 resist;
};

extern const struct ElementLevelBonus Element_PowerResistByLevel[16];
s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 output[4]);

s32 GameFlag_Test(s32 flag);
s32 Owner_LookupFourColumnTable(s32 primary, s32 j);

s32 Owner_RefreshClassActions(s32);
u32 Owner_BuildDigitTiles(s32 owner, s16 destination[4][2]);
s32 Owner_DetermineClass(s32 character, const u8 *djinn);

/* The mark after a repeated enemy's name: a letter from A in Japanese, a
   digit from 1 in the localizations. */
#if EDITION_INTERNATIONAL
#define ENEMY_SUFFIX_FIRST '1'
#define ENEMY_SUFFIX_LAST 8
#else
#define ENEMY_SUFFIX_FIRST 'A'
#define ENEMY_SUFFIX_LAST 25
#endif

extern const u8 Data_08080ec8[];
extern u8 MsgEnemyName;

void UiText_DecodeMessageFar(s32 message, u16 *buffer, s32 length);
void Owner_RecalculateStats(s32 owner);

typedef s32 (*ClearWordsFn)(void *destination, s32 size);

static __inline__ void ClearWords(ClearWordsFn clear, void *destination, s32 size)
{
    /* FAKEMATCH: a direct call puts the size in its register before the routine's address; the ROM loads the address between the two halves of the size. */
    clear(destination, size);
}

/* Fill battle unit 128..134 from an enemy's definition and name it, with a
   mark after the name when the battle holds the same enemy more than
   once. The enemy carries each item of its definition as many times as the
   definition counts, and enemies 158..171 fight on the second side. */
s32 BattleUnit_Assign(s32 unit_id, s32 enemy_id, s32 suffix)
{
    struct BattleUnit *unit;
    const struct EnemyDefinition *enemy;
    u16 name[15];
    s32 i;
    u32 index;
    s32 q;
    s32 count;

    index = enemy_id - 8;
    if (unit_id <= 127)
        return 0;
    if (unit_id > 134)
        return 0;
    if (index > 242)
        return 0;
    unit = Owner_GetState(unit_id);
    ClearWords(Iwram_ClearWords, unit, BATTLE_UNIT_SIZE);
    if (index > 164)
        index = 0;
    enemy = &((const struct EnemyDefinition *)Data_08080ec8)[index];
    unit->level = enemy->level;
    unit->base_hp = enemy->hp;
    unit->hp = enemy->hp;
    unit->max_hp = enemy->hp;
    unit->base_pp = enemy->pp;
    unit->pp = enemy->pp;
    unit->max_pp = enemy->pp;
    unit->hp_gauge = 0x4000;
    unit->pp_gauge = 0x4000;
    unit->base_attack = enemy->attack;
    unit->base_defense = enemy->defense;
    unit->base_agility = enemy->agility;
    unit->base_luck = enemy->luck;
    unit->base_action_count = enemy->turns;
    unit->base_hp_regen = enemy->hp_regen;
    unit->base_pp_regen = enemy->pp_regen;
    UiText_DecodeMessageFar(index + (s32)&MsgEnemyName, name, 15);
    for (i = 0; i < 14 && name[i] != 0; i++)
        unit->name[i] = name[i];
    if (suffix <= ENEMY_SUFFIX_LAST) {
        unit->name[i] = ENEMY_SUFFIX_FIRST + suffix;
        i++;
    }
    unit->name[i] = 0;
    unit->name[14] = 0;
    count = 0;
    for (i = 0; i < 4; i++) {
        if (enemy->items[i] != 0) {
            for (q = 0; q < enemy->counts[i]; q++) {
                if (count < 15)
                    unit->inventory[count++] = enemy->items[i];
            }
        }
    }
    unit->rewards = enemy->rewards;
    unit->class_index = 0;
    unit->class_id = enemy_id;
    Owner_BuildDigitTiles(unit_id, (s16 (*)[2])unit->base_elements);
    Owner_RecalculateStats(unit_id);
    unit->status_12a = 1;
    switch (unit->class_id) {
    case 158:
    case 159:
    case 160:
    case 161:
    case 162:
    case 163:
    case 164:
    case 165:
    case 166:
    case 167:
    case 168:
    case 169:
    case 170:
    case 171:
        unit->status_12a = 2;
        break;
    }
    return 1;
}

s32 Party_CountActiveOwners(void)
{
    s32 owner;
    s32 count;

    count = 0;
    owner = 0;
    do {
        if (GameFlag_Test(owner) != 0)
            count++;
        owner++;
    } while (owner <= 7);
    return count;
}

s32 Party_AddActiveOwner(s32 value)
{
    s32 count = Party_CountActiveOwners();
    s32 index;

    GameFlag_SetBit(value);
    index = 0;
    while (index < count) {
        if (gGameState.active_owners[index] == value)
            return count;
        index++;
    }
    gGameState.active_owners[index] = value;
    return count + 1;
}

s32 Party_RemoveActiveOwner(s32 value)
{
    s32 count = Party_CountActiveOwners();
    s32 i;
    s32 j;

    GameFlag_ClearBit(value);
    for (i = 0; i < count; i++) {
        if (gGameState.active_owners[i] == value)
            break;
    }
    for (j = i; j < count - 1; j++)
        gGameState.active_owners[j] = gGameState.active_owners[j + 1];
    return Party_CountActiveOwners();
}

s32 Party_ListActiveOwners(s16 *owners)
{
    s32 count = 0;

    if (owners != NULL) {
        s32 index;

        count = Party_CountActiveOwners();
        index = 0;
        if (count != 0) {
            do {
                *owners++ = gGameState.active_owners[index];
                index++;
            } while (index != count);
        }
        *owners = 0xff;
    }
    return count;
}

s32 Party_AdjustSixDigitCounterA(s32 amount)
{
    s32 value;
    struct GameState *work;
    struct GameState *store;

    work = &gGameState;
    value = work->coins;
    value = (s32)((u32)value + (u32)amount);
    store = work;
    if (value > 0xF423F) {
        value = 0xF423F;
    }
    if (value < 0) {
        value = 0;
    }
    work = store;
    work->coins = value;
    return value;
}

s32 Party_AdjustSixDigitCounterB(s32 amount)
{
    s32 value;

    struct GameState *progress = &gGameState;

    value = progress->shop_credit;
    value += amount;
    if (value > 0xf423f)
        value = 0xf423f;
    if (value < 0)
        value = 0;
    progress->shop_credit = value;
    return value;
}

s32 Party_AdjustCounterCappedAt28(s32 amount)
{
    s32 value;

    struct GameState *progress = &gGameState;

    value = progress->shop_gifts;
    value += amount;
    if (value > 28)
        value = 28;
    if (value < 0)
        value = 0;
    progress->shop_gifts = (s8)value;
    return value;
}

s32 Trade_ListFlaggedEntries(u8 *output)
{
    u8 *source = Summon_OrderList;
    u8 *end = Summon_OrderList + 15;
    s32 count = 0;

    do {
        u8 value = *source++;
        if ((((struct TradeFlags *)Trade_GetOfferState(0))->flags & (1 << value)) != 0) {
            *output++ = value;
            count++;
        }
    } while (source <= end);
    *output = 32;
    return count;
}

const struct SummonDefinition *SummonDefinition_GetNear(u32 summon_id)
{
    if (summon_id > 15)
        return NULL;
    return &Summon_DefinitionTable[summon_id];
}

s32 Owner_LookupFourColumnTable(s32 row, s32 column)
{
    return Data_08088db8[(row * 4) + column];
}

s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 output[4])
{
    s32 i;
    s32 *cursor;

    if (record > 7) {
        u32 index;

        index = ((const struct EnemyDefinition *)Owner_GetRecord(record))->element_preset;
        if (index > 43)
            index = 0;

        i = 0;
        cursor = output;
        for (; i <= 3; i++)
            *cursor++ = Enemy_ElementPresetTable[index].digits[i] * 10;
    } else {
        cursor = output;
        source += (u8 *)&((struct BattleUnit *)0)->djinn_active_counts -
                  (u8 *)&((struct BattleUnit *)0)->djinn_available;
        for (i = 3; i >= 0; i--) {
            u32 value = *source;
            source++;
            *cursor++ = value * 10;
        }

        if (record <= 7) {
            for (i = 0; i <= 3; i++) {
                *output += Owner_GetRecordStride180(record)->element_bonus[i];
                output++;
            }
        }
    }

    return 0;
}

s32 Owner_GetResistanceValue(s32 owner, s32 index)
{
    struct BattleUnit *state = (struct BattleUnit *)Owner_GetState(owner);
    s32 values[4];
    s32 result = 0;

    if (index <= 3) {
        Owner_GetDigitValues(state->class_id, (u8 *)state->djinn_available, values);
        result = values[index] / 10;
    }
    return result;
}

s32 Owner_GetDefaultElement(struct BattleUnit *state)
{
    const struct EnemyDefinition *record =
        (const struct EnemyDefinition *)Owner_GetRecord(state->class_id);
    u8 value = record->element_preset;

    if ((u32)value > 43)
        value = 0;
    return Enemy_ElementPresetTable[value].first;
}

u32 Owner_BuildDigitTiles(s32 owner, s16 destination[4][2])
{
    struct BattleUnit *state = (struct BattleUnit *)Owner_GetState(owner);
    u32 index;
    u32 result;
    s32 values[4];
    s32 i;

    if (state->class_index == 0) {
        index = ((const struct EnemyDefinition *)Owner_GetRecord(state->class_id))->element_preset;
        if (index > 43)
            index = 0;

        i = 0;
        for (;;) {
            ((s32 *)destination)[i] =
                Enemy_ElementPresetTable[index].values[i];
            i++;
            if (i > 3)
                goto copied;
        }
copied:
        return index;
    }

    Owner_GetDigitValues(state->class_id, (u8 *)state->djinn_available, values);
    i = 0;
    do {
        s32 value;
        s32 ones;
        s32 tens;

        result = (u32)Element_PowerResistByLevel;
        value = values[i];
        ones = value % 10;
        tens = value / 10;

        if (tens > 15)
            tens = 15;
        if (tens < 0)
            tens = 0;

        destination[i][0] = Element_PowerResistByLevel[tens].power + ones;
        destination[i][1] =
            ((volatile const struct ElementLevelBonus *)Element_PowerResistByLevel)[tens].resist + ones;
        i++;
    } while (i < 4);
    return result;
}

/* The class a character holds with the given set Djinn: the two strongest
   elements pick the class series, and the last class of that series whose
   element requirements are met wins. Some characters have fixed classes. */
s32 Owner_DetermineClass(s32 character, const u8 *djinn)
{
    s32 levels[4];
    s32 primary;
    s32 best;
    s32 series;
    s32 result;
    s32 j; /* the second element, then each requirement */
    s32 i;

    result = -1;
    if (character > 7)
        return 0;
    Owner_GetDigitValues(character, djinn, levels);
    if (GameFlag_Test(32)) {
        if (character == 0)
            return 200;
        if (character == 1)
            return 201;
    }
    if (character == 5)
        return 202;
    if (result != -1)
        return result;
    best = result;
    primary = result;
    for (i = 0; i < 4; i++) {
        if (best < levels[i]) {
            best = levels[i];
            primary = i;
        }
    }
    j = -1;
    best = -1;
    for (i = 0; i < 4; i++) {
        if (i != primary && best < levels[i]) {
            best = levels[i];
            j = i;
        }
    }
    series = Owner_LookupFourColumnTable(primary, levels[j] > 9 ? j : primary);
    for (i = 202; i >= 0; i--) {
        if (Class_DefinitionTable[i].series != series)
            continue;
        for (j = 0; j < 4; j++) {
            if (levels[j] < Class_DefinitionTable[i].required[j] * 10)
                break;
        }
        if (j == 4) {
            result = i;
            break;
        }
    }
    if (result == -1)
        result = 0;
    return result;
}

struct ClassDefinition *Owner_GetRecordStride84(s32 arg0)
{
    return &Class_DefinitionTable[arg0];
}

void Owner_RefreshDerivedData(s32 owner_no)
{
    struct BattleUnit *owner = Owner_GetState(owner_no);

    owner->class_index = Owner_DetermineClass(owner->class_id, (u8 *)owner->djinn_available);
    Owner_RefreshClassActions(owner_no);
    Owner_BuildDigitTiles(owner_no, (s16 (*)[2])owner->base_elements);
}
