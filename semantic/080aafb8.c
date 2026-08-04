#include "layout_guard.h"
#include "types.h"

enum {
    PARTY_CAPACITY_080AAFB8 = 8,
    ABILITIES_PER_MEMBER_080AAFB8 = 10,
    ABILITY_CATEGORY_COUNT_080AAFB8 = 4,
};

typedef struct AbilityGrid_080aafb8 {
    u16 abilities[PARTY_CAPACITY_080AAFB8]
                 [ABILITIES_PER_MEMBER_080AAFB8];
    s8 ability_counts[PARTY_CAPACITY_080AAFB8];
} AbilityGrid_080aafb8;

typedef struct BattleMenu_080aafb8 {
    u8 padding000[0x30];
    s32 display;
    u8 padding034[0x1d4];
    u16 member_ids[PARTY_CAPACITY_080AAFB8];
    u8 has_secondary;
    u8 active_count;
} BattleMenu_080aafb8;

typedef struct MenuRenderer_080aafb8 {
    u8 padding000[0xea3];
    s8 redraw_requested;
    u8 paddingea4[2];
    s8 drawing;
} MenuRenderer_080aafb8;

LAYOUT_OFFSET_GUARD(
    AbilityGrid080aafb8_Counts,
    AbilityGrid_080aafb8,
    ability_counts,
    0xa0);
LAYOUT_OFFSET_GUARD(
    BattleMenu080aafb8_Display,
    BattleMenu_080aafb8,
    display,
    0x30);
LAYOUT_OFFSET_GUARD(
    BattleMenu080aafb8_MemberIds,
    BattleMenu_080aafb8,
    member_ids,
    0x208);
LAYOUT_OFFSET_GUARD(
    BattleMenu080aafb8_ActiveCount,
    BattleMenu_080aafb8,
    active_count,
    0x219);
LAYOUT_OFFSET_GUARD(
    MenuRenderer080aafb8_RedrawRequested,
    MenuRenderer_080aafb8,
    redraw_requested,
    0xea3);
LAYOUT_OFFSET_GUARD(
    MenuRenderer080aafb8_Drawing,
    MenuRenderer_080aafb8,
    drawing,
    0xea6);

extern BattleMenu_080aafb8 *Data_03001f2c;
extern MenuRenderer_080aafb8 *Data_03001e8c;

void Func_08015070(s32, s32, s32, s32, s32);
void Func_08015080(s32, s32, s32, s32);
void Func_080150b8(s32);
void Func_08015270(s32);
void Func_08015280(s32, s32, s32, s32, s32);
s32 Func_08077208(u32, u32, s32);
s32 Func_08077210(u32, u32, s32);
s8 Func_080ac8fc(u16 *, u16, s32);

static u32 AbilityCategory_080aafb8(u16 ability)
{
    return (ability & 0x00e0) >> 5;
}

static u32 AbilityGroup_080aafb8(u16 ability)
{
    return (ability & 0x0f00) >> 8;
}

static u32 AbilityIndex_080aafb8(u16 ability)
{
    return ability & 0x001f;
}

static s32 AbilityIsAvailable_080aafb8(u16 ability)
{
    u32 group = AbilityGroup_080aafb8(ability);
    u32 category = AbilityCategory_080aafb8(ability);
    u32 index = AbilityIndex_080aafb8(ability);

    return Func_08077210(group, category, index) != 0 ||
           Func_08077208(group, category, index) != 0;
}

/* Build, categorize, and render the ability grid for every active member. */
void Func_080aafb8(AbilityGrid_080aafb8 *grid)
{
    BattleMenu_080aafb8 *menu = Data_03001f2c;
    MenuRenderer_080aafb8 *renderer = Data_03001e8c;
    s32 member;

    renderer->drawing = 1;

    for (member = 0; member < menu->active_count; member++) {
        grid->ability_counts[member] = Func_080ac8fc(
            grid->abilities[member],
            menu->member_ids[member],
            -1);
    }

    Func_08015270(menu->display);
    Func_08015080(0x0bad, menu->display, 0, 0x50);

    for (member = 0; member < menu->active_count; member++) {
        s32 category;
        s32 rendered_row = 0;

        for (category = 0;
             category < ABILITY_CATEGORY_COUNT_080AAFB8;
             category++) {
            s32 ability_index;

            for (ability_index = 0;
                 ability_index < grid->ability_counts[member];
                 ability_index++) {
                u16 ability = grid->abilities[member][ability_index];
                u32 ability_category = AbilityCategory_080aafb8(ability);

                if (category != ability_category)
                    continue;

                if ((ability & 0x8000) == 0)
                    Func_080150b8(2);
                if (!AbilityIsAvailable_080aafb8(ability))
                    Func_080150b8(4);

                Func_08015280(
                    menu->display,
                    ability_category + 0x5001,
                    member * 7 + 1,
                    rendered_row + 2,
                    0);
                Func_08015080(
                    ability_category * 0x14 +
                        AbilityIndex_080aafb8(ability) + 0x45f,
                    menu->display,
                    member * 0x38 + 0x10,
                    rendered_row * 8 + 0x10);
                rendered_row++;
                Func_080150b8(0x0f);
            }
        }
    }

    Func_08015070(menu->display, 0, 0x0a, 0x1c, 0x0a);
    Data_03001e8c->redraw_requested = 1;
    renderer->drawing = 0;
}
