#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "global_cells.h"
#include "battle_party.h"
#include "battle_escape.h"

/* battle/actor/clear_field_12b_for_group.c */
struct ActorState_080b90ac {
    u8 padding_000[0x12b];
    u8 field_12b;
};

struct ActorState_080b90ac *Runtime_GetObject(u16 id);
void BattleUnit_Recalculate(u16 id);

void BattleUnit_ClearField12bForGroup(void)
{
    u16 ids[14];
    s32 count;
    s32 index;

    count = Actor_Apply(3, ids);
    for (index = 0; index < count; index++) {
        struct ActorState_080b90ac *actor;

        actor = Runtime_GetObject(ids[index]);
        actor->field_12b = 0;
        BattleUnit_Recalculate(ids[index]);
    }
}

/* battle/escape/check_success.c */
struct BattleEscapeState {
    u8 reserved_00[0x45];
    u8 guaranteed;
    u8 failed_attempts;
};

struct BattleUnitLevel {
    u8 reserved_00[0x0f];
    u8 level;
};

struct BattleUnitLevel *Runtime_GetObject(s32);
s32 FixedPoint_Ratio(s32, s32);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
extern u8 gCell[];

s32 BattleEscape_CheckSuccess(void)
{
    s32 escaped;
    u8 *failed_attempts;
    s16 living_units[14];
    s32 living_count;
    s32 level_total;
    s32 unit_index;
    s32 chance;
    struct BattleEscapeState *escape_state;

    escaped = 0;
    escape_state = *(struct BattleEscapeState **)ADDR_03001E74;
    if (escape_state->guaranteed == 1) {
        escaped = 1;
    } else {
        failed_attempts = &escape_state->failed_attempts;
        chance = 0x1388 + (escape_state->failed_attempts * 0x7D0);
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_PARTY,
            living_units);
        level_total = 0;
        for (unit_index = escaped; unit_index < living_count; unit_index++) {
            level_total += Runtime_GetObject(
                (s32)living_units[unit_index])->level;
        }
        chance += FixedPoint_Ratio(level_total * 0x1F4, living_count);
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_ENEMIES,
            living_units);
        level_total = 0;
        for (unit_index = 0; unit_index < living_count; unit_index++) {
            level_total += Runtime_GetObject(
                (s32)living_units[unit_index])->level;
        }
        chance -= FixedPoint_Ratio(level_total * 0x1F4, living_count);
        if ((chance > 0) &&
            ((u32)((u32)(0x2710 * Rand()) >> 0x10) < (u32)chance)) {
            escaped = 1;
        }
        *failed_attempts += 1;
    }
    if (gCell[0x22B] == 2) {
        escaped = 0;
    }
    return escaped;
}

/* battle/presentation/list/units.c */
u16 *Battle_Run(s32 size);

u8 *random_16(s32 unit_id);

struct BattlePresentationUnitEntry {
    u16 unit_id;
    u16 unknown_02;
    u16 value;
    s16 width;
    s16 mode;
    s16 height;
    u8 unknown_0c[4];
};

s32 BattlePres_BuildUnitEntries(
    struct BattlePresentationUnitEntry *entries)
{
    u16 *excluded_units = Battle_Run(17);
    u16 *unit_ids = Battle_Run(9);
    s32 unit_count = Battle_Apply(1, unit_ids);
    s32 excluded_count = 0;
    s32 entry_count = 0;
    s32 unit_index;

    for (unit_index = 0; unit_index < unit_count; unit_index++) {
        u8 *unit = random_16(unit_ids[unit_index]);
        s32 copy_index;

        for (copy_index = 0; copy_index < unit[0x43]; copy_index++) {
            if (unit[0x13c] != 0 || (*(u32 *)(unit + 0x138) & 0xffffff00)) {
                struct BattlePresentationUnitEntry *entry =
                    &entries[entry_count];
                entry->unit_id = unit_ids[unit_index];
                entry->value = *(u16 *)(unit + 0x40);
                entry->width = 8;
                entry->mode = 0;
                entry->height = 0x180;
                entry_count++;
            } else {
                excluded_units[excluded_count++] = unit_ids[unit_index];
            }
        }
    }

    {
        s32 appended;

        entries += entry_count;
        appended = Battle_Place(entries, excluded_units, excluded_count);
        if (appended < 0) {
            unit_count = -1;
        } else {
            unit_count = entry_count + appended;
        }
        Battle_Do(unit_ids);
        Battle_Do(excluded_units);
        return unit_count;
    }
}

/* battle/presentation/list/opponent.c */
u8 *Battle_Run(s32 unit_id);

struct BattlePresentationOpponentEntry {
    u16 unit_id;
    u16 unknown_02;
    u16 value;
    s16 width;
    s16 mode;
    s16 height;
    u8 unknown_0c[4];
};

s32 BattlePres_BuildOpponentEntries(
    struct BattlePresentationOpponentEntry *entries)
{
    u16 unit_ids[14];
    s32 entry_count = 0;
    u8 *battle = *(u8 **)0x03001e74;
    s32 unit_count;
    s32 i;

    if (battle[0x45] == 1) {
        return 0;
    }

    unit_count = Battle_Apply(2, unit_ids);
    if (unit_count == 0) {
        return 0;
    }

    for (i = 31; i >= 0; i--) {
        u32 first = (u32)(unit_count *random_16()) >> 16;
        u32 second = (u32)(unit_count *random_16()) >> 16;
        s32 swap = unit_ids[first];
        unit_ids[first] = unit_ids[second];
        unit_ids[second] = swap;
    }

    if (battle[0x45] == 2) {
        s32 limit = ((u32)(random_16() * 5) >> 16) + 1;

        if (limit <= 1) {
            limit = 2;
        }
        if (limit < unit_count) {
            unit_count = limit;
        }
    }

    for (i = 0; i < unit_count; i++) {
        s32 unit_id = unit_ids[i];
        u8 *unit = Battle_Run(unit_id);
        s32 copy_index;

        for (copy_index = 0; copy_index < unit[0x43]; copy_index++) {
            struct BattlePresentationOpponentEntry *entry =
                &entries[entry_count];
            s32 value;

            entry->unit_id = unit_id;
            value = *(u16 *)(unit + 0x40);
            entry->value = value;
            if (copy_index != 0) {
                entry->value = (s16)value / 2;
            }

            if (unit[0x13c] != 0 || unit[0x13b] != 0) {
                entry->width = 8;
                entry->mode = 0;
                entry->height = 0x100;
            } else {
                Battle_Apply2(entry, 0);
            }

            entry_count++;

            if (battle[0x45] == 2) {
                break;
            }
        }
    }

    return entry_count;
}
