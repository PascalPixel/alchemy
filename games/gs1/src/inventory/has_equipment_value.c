#include "scene.h"
#include "inventory.h"
#include "types.h"
#include "owner_state.h"
#include "game_flags.h"
#include "party_state.h"
#include "global_progress.h"
#include "battle_summon.h"
#include "runtime_interfaces.h"

/* inventory/has_equipment_value.c */
/* inventory/has_equipment_value.c */
/* inventory/has_equipment_value.c */
s32 Equipment_HasValue(s32 owner, s32 value)
{
    u8 *entry = OwnerState_Get(owner);
    s32 mask = 0x3fff;
    s32 index = 0;

    entry += 88;
    do {
        s32 current = *(u16 *)entry;

        current &= mask;
        entry += 4;
        if (current == value) {
            return 1;
        }
        index++;
    } while (index <= 31);
    return 0;
}

/* owner/action/add.c */
struct Slot08078e28 {
    u16 value;
    u16 padding;
};

struct State08078e28 {
    u8 padding00[0x58];
    struct Slot08078e28 slots[32];
};

void *Owner_GetState(s32 owner);

s32 OwnerAction_Add(s32 state_index, s32 value)
{
    struct State08078e28 *state = (struct State08078e28 *)Owner_GetState(state_index);
    s32 key = value & 0x3fff;
    s32 found = -1;
    s32 index;

    for (index = 0; index <= 30; index++) {
        s32 masked = state->slots[index].value & 0x3fff;

        if ((masked ^ key) == 0) {
            state->slots[index].value = masked;
            found = index;
            break;
        }
    }

    if (found < 0) {
        for (index = 0; index <= 30; index++) {
            s32 offset = (index * 4) + 0x58;
            if (*(u16 *)((u8 *)state + offset) == 0) {
                *(u16 *)((u8 *)state + offset) = key;
                found = index;
                break;
            }
        }
        if (found < 0) {
            return -1;
        }
    }

    Sys_Do(state_index);
    for (index = 0; index <= 31; index++) {
        if (state->slots[index].value == key) {
            break;
        }
    }
    return index;
}

/* owner/action/check_level_threshold.c */
s32 OwnerAction_CheckLevelThreshold(void)
{
    return Sys_Check();
}

/* owner/get_record_stride180.c */
typedef struct {
    u8 bytes[0xB4];
} gRom;

extern gRom gRom2[];

gRom *Owner_GetRecordStride180(s32 index)
{
    gRom *base;

    base = gRom2;
    return &base[index];
}

/* owner/reserved_no_op_b.c */
/* owner/reserved_no_op_b.c */
void Owner_ReservedNoOp79004(void)
{
}

/* owner/get_level_threshold.c */
struct State_08079008 {
    u8 padding[0x128];
    u8 type;
    u8 enabled;
};

extern u32 gRomNoOpB[];

u32 Owner_GetLevelThreshold(s32 owner, s32 level)
{
    struct State_08079008 *state = (struct State_08079008 *)Owner_GetState(owner);

    if (state->enabled != 0) {
        if (level <= 0) {
            return 0;
        }
        if (level <= 99 && state->type <= 7) {
            return gRomNoOpB[state->type * 99 + level - 1];
        }
    }
    return (u32)-1;
}

/* owner/get_value_if_level_threshold_reached.c */
struct Owner_080792c4 {
    u8 unknown_000[0x0f];
    u8 level;
    u8 unknown_010[0x114];
    u32 value_124;
};

u32 Owner_GetLevelThreshold(s32 owner, s32 level);

s32 Owner_GetValueIfLevelThresholdReached(s32 owner_no, s32 value)
{
    struct Owner_080792c4 *owner;

    owner = (struct Owner_080792c4 *)Owner_GetState(owner_no);
    if ((owner->value_124 >= Owner_GetLevelThreshold(owner_no, owner->level + 1)) &&
        (Sys_CheckNoOpB(owner_no, value) != 0)) {
        return value;
    }
    return 0;
}

/* party/advance_owner_count_to_target.c */
struct State_080792fc {
    u8 padding[15];
    u8 count;
};


void Party_AdvanceOwnerCountToTarget(s32 owner, s32 target)
{
    u8 buf[16];
    s32 count = ((struct State_080792fc *)Owner_GetState(owner))->count;

    while (count < target) {
        Party_Check(owner, buf);
        count++;
    }
    Party_Do(owner);
}

/* party/count_active_owners.c */
/* party/count_active_owners.c */
s32 Party_CountActiveOwners()
{
    s32 owner;
    s32 count;

    count = 0;
    owner = 0;
    do {
        if (GameFlag_Test(owner)!= 0) {
            count += 1;
        }
        owner += 1;
    } while (owner <= 7);
    return count;
}

/* party/add_active_owner.c */
s32 Party_AddActiveOwner(s32 value)
{
    s32 count = Party_CountActiveOwners();
    s32 index;

    GameFlag_Set(value);
    index = 0;
    while (index < count) {
        if (gCell.active_owners[index] == value) {
            return count;
        }
        index++;
    }
    gCell.active_owners[index] = value;
    return count + 1;
}

/* party/remove_active_owner.c */
s32 Party_RemoveActiveOwner(s32 owner_id)
{
    s32 active_count;
    s32 owner_index;
    s32 last_index;

    active_count = Party_CountActiveOwners(owner_id);
    GameFlag_Clear(owner_id);

    owner_index = 0;
    while (owner_index < active_count
        && gCell.active_owners[owner_index] != owner_id) {
        owner_index++;
    }

    last_index = active_count - 1;
    if (owner_index < last_index) {
        s32 remaining_count;
        u8 *owner_base;
        u8 *owner_cursor;

        owner_base = (u8 *)&gCell;
        owner_base += owner_index;
        owner_cursor = owner_base + 0x1f8;
        remaining_count = last_index - owner_index;
        do {
            remaining_count--;
            *owner_cursor = owner_cursor[1];
            owner_cursor++;
        } while (remaining_count != 0);
    }

    return Party_CountActiveOwners(last_index);
}

/* party/list_active_owners.c */
s32 Party_ListActiveOwners(s16 *owners)
{
    s32 count = 0;

    if (owners != 0) {
        s32 index;

        count = Party_CountActiveOwners();
        index = 0;
        if (count != 0) {
            do {
                *owners++ = gCell.active_owners[index];
                index++;
            } while (index != count);
        }
        *owners = 0xff;
    }
    return count;
}

/* party/adjust_six_digit_counter_a.c */
struct Work_08079700 {
    u8 unknown_00[0x10];
    s32 value;
};

s32 Party_AdjustSixDigitCounterA(s32 amount)
{
    s32 value;
    struct Work_08079700 *work;
    struct Work_08079700 *store;

    work = (struct Work_08079700 *)0x02000240;
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

/* party/adjust_six_digit_counter_b.c */
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

/* party/adjust_counter_capped_at28.c */
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

/* trade/list_flagged_entries.c */
struct State_0807977c {
    u32 flags;
};

extern u8 gRomCountActiveOwners[16];
struct State_0807977c *Trade_GetOfferState(s32);

s32 Trade_ListFlaggedEntries(u8 *output)
{
    u8 *source = gRomCountActiveOwners;
    u8 *end = gRomCountActiveOwners + 15;
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

/* summon/get_definition.c */
extern const u8 gRomGetDefinition[];

s32 SummonDefinition_GetNear(u32 summon_id)
{
    if (summon_id > 0xFU) {
        return 0;
    }
    return (s32)(gRomGetDefinition + summon_id * 8);
}

/* owner/lookup_four_column_table.c */
extern s32 gRomFourColumnTable[];

s32 Owner_LookupFourColumnTable(s32 row, s32 column)
{
    return gRomFourColumnTable[(row * 4) + column];
}

/* owner/get_digit_values.c */
struct Record_080797fc {
    u8 padding[2];
    u8 values[148];
};

struct Record_080797fc *Sys_Run(s32);
#include "preset_table.h"

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
            *cursor++ = gRomGetDigitValues[index].digits[i] * 10;
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
                *output += Sys_Run(record)->values[144 + i];
                output++;
            }
        }
    }

    return 0;
}

/* owner/get_resistance_value.c */
struct State_0807987c {
    u8 padding[0xf8];
    u8 source[0x30];
    u8 record;
};

s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 output[4]);
s32 FixedPoint_Ratio(s32 value, s32 divisor);

s32 Owner_GetResistanceValue(s32 owner, s32 index)
{
    struct State_0807987c *state = (struct State_0807987c *)Owner_GetState(owner);
    s32 values[4];
    s32 result = 0;

    if (index <= 3) {
        Owner_GetDigitValues(state->record, state->source, values);
        result = FixedPoint_Ratio(values[index], 10);
    }
    return result;
}

/* owner/get_default_element.c */
struct OwnerElementState {
    u8 padding[0x128];
    u8 record;
};

struct OwnerElementRecord {
    u8 padding[0x34];
    u8 value;
};

#include "preset_table.h"

s32 Owner_GetDefaultElement(struct OwnerElementState *state)
{
    const struct OwnerElementRecord *record =
        (const struct OwnerElementRecord *)Owner_GetRecord(state->record);
    u8 value = record->value;

    if ((u32)value > 43)
        value = 0;
    return gRomGetDefaultElement[value].first;
}

/* owner/build_digit_tiles.c */
/* owner/build_digit_tiles.c */
struct OwnerState {
    u8 unknown_000[0xf8];
    u8 source_f8[0x30];
    u8 record_128;
    u8 use_source_129;
};

#include "preset_table.h"

struct DigitOffsets {
    u16 first;
    u16 second;
};

extern const struct DigitOffsets gRomBuildDigitTiles[16];

const u8 *Owner_GetRecord(s32 record);
s32 Modulo(s32 value, s32 divisor);

u32 Owner_BuildDigitTiles(s32 owner, s16 destination[4][2])
{
    struct OwnerState *state = (struct OwnerState *)Owner_GetState(owner);
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
                gRom2BuildDigitTiles[index].values[i];
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

        result = (u32)gRomBuildDigitTiles;
        value = values[i];
        ones = Modulo(value, 10);
        tens = FixedPoint_Ratio(value, 10);

        if (tens > 15)
            tens = 15;
        if (tens < 0)
            tens = 0;

        destination[i][0] = gRomBuildDigitTiles[tens].first + ones;
        destination[i][1] =
            ((volatile const struct DigitOffsets *)gRomBuildDigitTiles)[tens].second + ones;
        i++;
    } while (i < 4);
    return result;
}

/* owner/get_record_stride84.c */
/* owner/get_record_stride84.c */
extern const u8 gRomGetRecordStride84[];

s32 Owner_GetRecordStride84(s32 arg0)
{
    return (s32)(gRomGetRecordStride84 + arg0 * 0x54);
}

/* owner/refresh_derived_data.c */
struct OwnerDerivedState {
    u8 unknown_000[0x24];
    u8 data_024[0xd4];
    u8 values_f8[0x30];
    u8 value_128;
    s8 value_129;
};

u32 Owner_BuildDigitTiles(s32, void *);

void Owner_RefreshDerivedData(s32 owner_no)
{
    struct OwnerDerivedState *owner;

    owner = Owner_GetState(owner_no);
    owner->value_129 = Sys_Apply(owner->value_128, owner->values_f8);
    Sys_CheckBuildDigitTiles(owner_no);
    Owner_BuildDigitTiles(owner_no, owner->data_024);
}
