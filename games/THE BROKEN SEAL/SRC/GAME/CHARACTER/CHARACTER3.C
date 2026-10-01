#include "EDITION.H"
#include "GAME_FLAGS.H"
#include "PARTY_STATE.H"
#include "TYPES.H"
#include "GLOBAL_PROGRESS.H"
#include "BATTLE_SUMMON.H"
#include "RUNTIME_INTERFACES.H"
#include "PRESET_TABLE.H"
#include "FIXED_MATH.H"
#include "OWNER_STATE.H"
#include "IWRAM_CALL.H"

s32 GameFlag_SetBit(s32);
void GameFlag_ClearBit(s32);

struct PartyCounterWork {
    u8 unknown_00[0x10];
    s32 value;
};

struct State_0807977c {
    u32 flags;
};

extern u8 Summon_OrderList[16];
struct State_0807977c *Trade_GetOfferState(s32);
extern const struct SummonDefinition Summon_DefinitionTable[];

extern s32 Data_08088db8[];
extern struct PresetValues Enemy_ElementPresetTable[];

struct OwnerBonusValues {
    u8 unknown[2];
    u8 values[148];
};

struct OwnerBonusValues *Owner_GetRecordStride180(s32);

struct OwnerResistanceState {
    u8 unknown[0xf8];
    u8 source[0x30];
    u8 record;
};

void *Owner_GetState(s32);
s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 *output);

struct OwnerElementState {
    u8 unknown[0x128];
    u8 record;
};

struct OwnerElementRecord {
    u8 unknown[0x34];
    u8 value;
};

struct OwnerDigitState {
    u8 unknown_000[0xf8];
    u8 source_f8[0x30];
    u8 record_128;
    u8 use_source_129;
};

struct DigitOffsets {
    u16 first;
    u16 second;
};

extern const struct DigitOffsets Element_PowerResistByLevel[16];
void *Owner_GetState(s32 owner);
const u8 *Owner_GetRecord(s32 record);
s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 output[4]);

/* An 84-byte class record: the class series it belongs to and the Djinn
   level each element needs, in tens. */
struct ClassDefinition {
    s32 series;                 /* 0x00 */
    u8 required[4];             /* 0x04 */
    u8 unknown_08[0x4c];
};

extern struct ClassDefinition Class_DefinitionTable[];
s32 GameFlag_Test(s32 flag);
s32 Owner_LookupFourColumnTable(s32 primary, s32 j);

struct OwnerDerivedState {
    u8 unknown_000[0x24];
    u8 data_024[0xd4];
    u8 values_f8[0x30];
    u8 value_128;
    s8 value_129;
};

s32 Owner_RefreshClassActions(s32);
u32 Owner_BuildDigitTiles(s32 owner, s16 destination[4][2]);
s32 Owner_DetermineClass(s32 character, const u8 *djinn);

/* An 84-byte enemy definition. */
struct EnemyDefinition {
    u8 unknown_00[15];
    u8 level;                   /* 0x0f */
    u16 hp;                     /* 0x10 */
    u16 pp;                     /* 0x12 */
    u16 attack;                 /* 0x14 */
    u16 defense;                /* 0x16 */
    u16 agility;                /* 0x18 */
    u8 luck;                    /* 0x1a */
    u8 turns;                   /* 0x1b */
    u8 hp_regen;                /* 0x1c */
    u8 pp_regen;                /* 0x1d */
    u8 unknown_1e[2];
    u32 rewards;                /* 0x20 */
    u8 unknown_24[4];
    /* What the enemy carries, and how many of each. */
    u16 items[4];               /* 0x28 */
    u8 counts[4];               /* 0x30 */
    u8 unknown_34[0x20];
};

/* The fields of a battle unit that an enemy's definition fills. */
struct EnemyUnit {
    u8 name[14];                /* 0x00 */
    u8 name_end;                /* 0x0e */
    u8 level;                   /* 0x0f */
    u16 base_hp;                /* 0x10 */
    u16 base_pp;                /* 0x12 */
    u16 hp_ratio;               /* 0x14 current/maximum HP, Q14 */
    u16 pp_ratio;               /* 0x16 */
    u16 attack;                 /* 0x18 */
    u16 defense;                /* 0x1a */
    u16 agility;                /* 0x1c */
    u8 luck;                    /* 0x1e */
    u8 turns;                   /* 0x1f */
    u8 hp_regen;                /* 0x20 */
    u8 pp_regen;                /* 0x21 */
    u8 unknown_22[2];
    s16 element_levels[4][2];   /* 0x24 */
    u16 max_hp;                 /* 0x34 */
    u16 max_pp;                 /* 0x36 */
    u16 hp;                     /* 0x38 */
    u16 pp;                     /* 0x3a */
    u8 unknown_3c[0x9c];
    u16 inventory[15];          /* 0xd8 */
    u8 unknown_0f6[0x2a];
    u32 rewards;                /* 0x120 */
    u8 unknown_124[4];
    u8 enemy;                   /* 0x128 */
    u8 class_index;             /* 0x129 */
    u8 side;                    /* 0x12a */
};

enum {
    BATTLE_UNIT_BYTES = 0x14c
};

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
    struct EnemyUnit *unit;
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
    ClearWords(Iwram_ClearWords, unit, BATTLE_UNIT_BYTES);
    if (index > 164)
        index = 0;
    enemy = (const struct EnemyDefinition *)(Data_08080ec8 + index * 84);
    unit->level = enemy->level;
    unit->base_hp = enemy->hp;
    unit->hp = enemy->hp;
    unit->max_hp = enemy->hp;
    unit->base_pp = enemy->pp;
    unit->pp = enemy->pp;
    unit->max_pp = enemy->pp;
    unit->hp_ratio = 0x4000;
    unit->pp_ratio = 0x4000;
    unit->attack = enemy->attack;
    unit->defense = enemy->defense;
    unit->agility = enemy->agility;
    unit->luck = enemy->luck;
    unit->turns = enemy->turns;
    unit->hp_regen = enemy->hp_regen;
    unit->pp_regen = enemy->pp_regen;
    UiText_DecodeMessageFar(index + (s32)&MsgEnemyName, name, 15);
    for (i = 0; i < 14 && name[i] != 0; i++)
        unit->name[i] = name[i];
    if (suffix <= ENEMY_SUFFIX_LAST) {
        unit->name[i] = ENEMY_SUFFIX_FIRST + suffix;
        i++;
    }
    unit->name[i] = 0;
    unit->name_end = 0;
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
    unit->enemy = enemy_id;
    Owner_BuildDigitTiles(unit_id, unit->element_levels);
    Owner_RecalculateStats(unit_id);
    unit->side = 1;
    switch (unit->enemy) {
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
        unit->side = 2;
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
    struct PartyCounterWork *work;
    struct PartyCounterWork *store;

    work = (struct PartyCounterWork *)&gGameState;
    value = work->value;
    value = (s32)((u32)value + (u32)amount);
    store = work;
    if (value > 0xF423F) {
        value = 0xF423F;
    }
    if (value < 0) {
        value = 0;
    }
    work = store;
    work->value = value;
    return value;
}

s32 Party_AdjustSixDigitCounterB(s32 amount)
{
    s32 value;

    struct GlobalProgressPartialView *progress = GlobalProgress_Get();

    value = progress->value_118;
    value += amount;
    if (value > 0xf423f)
        value = 0xf423f;
    if (value < 0)
        value = 0;
    progress->value_118 = value;
    return value;
}

s32 Party_AdjustCounterCappedAt28(s32 amount)
{
    s32 value;

    struct GlobalProgressPartialView *progress = GlobalProgress_Get();

    value = progress->value_11c;
    value += amount;
    if (value > 28)
        value = 28;
    if (value < 0)
        value = 0;
    progress->value_11c = (s8)value;
    return value;
}

s32 Trade_ListFlaggedEntries(u8 *output)
{
    u8 *source = Summon_OrderList;
    u8 *end = Summon_OrderList + 15;
    s32 count = 0;

    do {
        u8 value = *source++;
        if ((Trade_GetOfferState(0)->flags & (1 << value)) != 0) {
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

        index = Owner_GetRecord(record)[52];
        if (index > 43)
            index = 0;

        i = 0;
        cursor = output;
        for (; i <= 3; i++)
            *cursor++ = Enemy_ElementPresetTable[index].digits[i] * 10;
    } else {
        cursor = output;
        source += 36;
        for (i = 3; i >= 0; i--) {
            u32 value = *source;
            source++;
            *cursor++ = value * 10;
        }

        if (record <= 7) {
            for (i = 0; i <= 3; i++) {
                *output += Owner_GetRecordStride180(record)->values[144 + i];
                output++;
            }
        }
    }

    return 0;
}

s32 Owner_GetResistanceValue(s32 owner, s32 index)
{
    struct OwnerResistanceState *state = (struct OwnerResistanceState *)Owner_GetState(owner);
    s32 values[4];
    s32 result = 0;

    if (index <= 3) {
        Owner_GetDigitValues(state->record, state->source, values);
        result = values[index] / 10;
    }
    return result;
}

s32 Owner_GetDefaultElement(struct OwnerElementState *state)
{
    const struct OwnerElementRecord *record =
        (const struct OwnerElementRecord *)Owner_GetRecord(state->record);
    u8 value = record->value;

    if ((u32)value > 43)
        value = 0;
    return Enemy_ElementPresetTable[value].first;
}

u32 Owner_BuildDigitTiles(s32 owner, s16 destination[4][2])
{
    struct OwnerDigitState *state = (struct OwnerDigitState *)Owner_GetState(owner);
    u32 index;
    u32 result;
    s32 values[4];
    s32 i;

    if (state->use_source_129 == 0) {
        index = Owner_GetRecord(state->record_128)[52];
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

    Owner_GetDigitValues(state->record_128, state->source_f8, values);
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

        destination[i][0] = Element_PowerResistByLevel[tens].first + ones;
        destination[i][1] =
            ((volatile const struct DigitOffsets *)Element_PowerResistByLevel)[tens].second + ones;
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

s32 Owner_GetRecordStride84(s32 arg0)
{
    return (s32)(((const u8 *)Class_DefinitionTable) + arg0 * 0x54);
}

void Owner_RefreshDerivedData(s32 owner_no)
{
    struct OwnerDerivedState *owner = Owner_GetState(owner_no);

    owner->value_129 = Owner_DetermineClass(owner->value_128, owner->values_f8);
    Owner_RefreshClassActions(owner_no);
    ((u32 (*)(s32, void *))Owner_BuildDigitTiles)(owner_no, owner->data_024);
}
