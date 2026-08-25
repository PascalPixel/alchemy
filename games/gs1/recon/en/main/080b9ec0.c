#include "types.h"

#define BattlePresentation_RunUnitTransition Func_080b9ec0

struct BattlePresentationSelection {
    u8 primary_unit;
    s8 unit_count;
    u8 units[0x4e];
    void *presentation_data;
    u8 unknown_54[4];
    u32 flags;
    s32 message_mode;
};

struct BattlePresentationUnitInfo {
    u8 unknown_00[0x27];
    u8 ability_count;
    void *abilities[1];
};

#define FIELD8(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define FIELD16(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define FIELD32(base, offset) (*(u32 *)((u8 *)(base) + (offset)))

void BattlePresentation_RunUnitTransition(
    struct BattlePresentationSelection *selection,
    s32 mode)
{
    u16 visible_units[14];
    u8 context[0x54];
    u32 primary_unit;
    u32 opposing_unit;
    u32 visible_count;
    u32 refreshed_count;
    u32 index;
    u32 kept_count;
    u32 primary_record;

    Func_080b9d34(selection, context);
    primary_unit = selection->primary_unit;
    opposing_unit = selection->units[0];

    if (selection->flags & 0x8000) {
        u32 *transition = *(u32 **)0x03001f00;
        transition[0] = primary_unit <= 7 ? 0x2000 : 0x5000;
        transition[1] = 60;
    } else {
        u32 *transition = *(u32 **)0x03001f00;
        u32 target = primary_unit <= 7 ? 0x2000 : 0xffffe000;
        if (transition[0] != target) {
            transition[0] = target;
        }
    }

    Func_080c10e8(0, 0);
    Func_08015130((FIELD8(*(void **)0x03001e74, 0x41)) & ~1);
    primary_record = *(u32 *)Func_080b7dd0(primary_unit);
    *(u16 *)0x04000050 = 0x3f40;
    visible_count = Func_080b6c08(3, visible_units);

    for (index = 0; index < visible_count; index++) {
        u16 unit = visible_units[index];
        if (unit != 0xfe) {
            if (unit == primary_unit) {
                Func_08009080(primary_record, 3);
            } else if ((opposing_unit <= 7) != (unit <= 7)) {
                Func_080c0f98(unit, 1);
            }
        }
    }

    Func_080f9010(0x9a);
    Func_080c1798(FIELD32(context, 8), selection->presentation_data, 0, 0);
    if (mode & 1) {
        Func_080c0f98(primary_unit, 1);
    }

    for (index = 0; index < 16; index++) {
        *(u16 *)0x04000052 = (16 - index) | 0x1000;
        Func_080030f8(1);
    }

    if (selection->message_mode != 0) {
        if (selection->message_mode == 1) {
            Func_080bbabc(0, primary_unit);
            Func_080bbabc(4, 0x856);
        } else {
            Func_080bbabc(4, 0x855);
        }
        Func_080bb938();
        Func_080c1a14();
    } else {
        kept_count = 0;
        for (index = 0; index < visible_count; index++) {
            u16 unit = visible_units[index];
            if (unit == primary_unit) {
                if (!(mode & 1)) {
                    visible_units[kept_count++] = primary_unit;
                }
            } else if ((opposing_unit > 7) != (unit > 7)) {
                visible_units[kept_count++] = unit;
            }
        }
        visible_units[kept_count] = 0xff;
        Func_080b7b6c(visible_units, 0);

        for (index = 0; index < selection->unit_count; index++) {
            visible_units[index] = selection->units[index];
        }
        visible_units[index] = 0xff;

        for (index = 0; index < FIELD32(context, 0x14); index++) {
            struct BattlePresentationUnitInfo *info;
            u32 ability;
            u32 ability_count;
            u32 unit = FIELD16(context, 0x24 + index * 2);
            info = Func_080b7f70(*(u32 *)Func_080b7dd0(unit), 0);
            ability_count = info->ability_count - 1;
            for (ability = 0; ability < ability_count; ability++) {
                FIELD8(context, 0x34 + index * 4 + ability) =
                    FIELD8(info->abilities[ability], 5);
            }
        }

        if (selection->flags & 0x8000) {
            FIELD32(context, 4) = opposing_unit > 7 ? 0 : 1;
        } else {
            FIELD32(context, 4) = selection->units[0] <= 7 ? 1 : 0;
        }
        if (selection->flags & 0x20000) {
            FIELD32(context, 4) ^= 1;
        }

        Func_080041d8(0x080bd899, 0xc80);
        if (selection->flags & 0x8000) {
            Func_080c9010(context);
        } else if (selection->flags & 0x4000) {
            Func_080c9008(context);
        } else {
            Func_080c9018(context);
        }
        Func_080be02c();
    }

    Func_080b6c90();
    refreshed_count = Func_080b6c08(3, visible_units);
    *(u16 *)0x04000050 = 0x3f40;
    for (index = 0; index < refreshed_count; index++) {
        u16 unit = visible_units[index];
        if (unit != 0xfe && unit != primary_unit &&
            ((opposing_unit <= 7) != (unit <= 7))) {
            Func_080c0f98(unit, 1);
        }
    }
    for (index = 0; index < 16; index++) {
        *(u16 *)0x04000052 = index | 0x1000;
        Func_080030f8(1);
    }
    for (index = 0; index < refreshed_count; index++) {
        Func_080c0f98(visible_units[index], 0);
    }
    Func_080c0cec(0, 0, 0, 0x64);
    Func_080030f8(1);
}
