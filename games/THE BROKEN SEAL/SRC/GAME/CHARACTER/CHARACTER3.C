#include "GAME_FLAGS.H"
#include "PARTY_STATE.H"
#include "TYPES.H"
#include "GLOBAL_PROGRESS.H"
#include "BATTLE_SUMMON.H"
#include "RUNTIME_INTERFACES.H"
#include "PRESET_TABLE.H"
#include "FIXED_MATH.H"
#include "OWNER_STATE.H"

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
        result = Math_Div(values[index], 10);
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
        ones = Math_Mod(value, 10);
        tens = Math_Div(value, 10);

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
