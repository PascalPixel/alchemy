#include "layout_guard.h"
#include "types.h"

struct MenuControl_080aa768 {
    u8 unknown_00[5];
    u8 mode;
    u8 unknown_06[6];
    u16 value;
};

struct BattleMenu_080aa768 {
    u8 unknown_000[8];
    s32 selection;
    u8 unknown_00c[8];
    struct MenuControl_080aa768 *control;
    u8 unknown_018[4];
    s8 primary_index;
    s8 secondary_index;
    u8 unknown_01e[0x12];
    s32 display;
    u8 unknown_034[0x110];
    u16 lines[24];
    u16 chosen_entry;
    u16 encoded_selection;
    u8 selected_kind;
    u8 unknown_179[0x0b];
    u8 *database;
    u8 unknown_188[0x80];
    u16 member_ids[8];
    u8 has_secondary;
    u8 line_count;
    u8 primary;
    u8 secondary;
    u8 unknown_21c[4];
    u16 action_flags;
    u8 unknown_222[0x32];
    u8 primary_arg;
    u8 secondary_arg;
    u8 primary_value;
    u8 secondary_value;
};

LAYOUT_OFFSET_GUARD(
    BattleMenu080aa768_Control,
    struct BattleMenu_080aa768,
    control,
    0x14);
LAYOUT_OFFSET_GUARD(
    BattleMenu080aa768_Lines,
    struct BattleMenu_080aa768,
    lines,
    0x144);
LAYOUT_OFFSET_GUARD(
    BattleMenu080aa768_Database,
    struct BattleMenu_080aa768,
    database,
    0x184);
LAYOUT_OFFSET_GUARD(
    BattleMenu080aa768_Primary,
    struct BattleMenu_080aa768,
    primary,
    0x21a);
LAYOUT_OFFSET_GUARD(
    BattleMenu080aa768_PrimaryArg,
    struct BattleMenu_080aa768,
    primary_arg,
    0x254);

extern struct BattleMenu_080aa768 *Data_03001f2c;

u16 Func_08002304(u16, s32);
void Func_080030f8(s32);
void Func_08015278(s32);
void Func_08077010(u8);
void Func_080771b0(u8, u8, u8);
void Func_080771b8(u8, u8, u8);
s32 Func_080771c0(u8, u8, u8);
s32 Func_080771c8(u8, u8, u8);
s32 Func_080771d0(u8, u8, u8, u8);
void Func_080aa544(s32);
void Func_080aad10(void);
void Func_080aaf58(void *);
void Func_080aafb8(void *);
s32 Func_080ab314(void);
s32 Func_080ab5e4(s32);
void Func_080ad5b4(s32, s32, s32, s32);
s32 Func_080ad6d4(s32);
s32 Func_080ae2f4(void);
void Func_080f9010(s32);

static void SelectDatabaseEntry_080aa768(
    struct BattleMenu_080aa768 *menu)
{
    u16 group = Func_08002304(menu->encoded_selection, 10);
    s32 selected = 0;
    s32 index;
    s32 count = *(s8 *)(menu->database + group + 0xa0);

    for (index = 0; index < count; index++) {
        if (menu->database[((group * 10 + index) * 2)] ==
            menu->selected_kind) {
            selected = index;
            break;
        }
    }
    menu->chosen_entry = group + selected * 10;
}

static void FinishSingleAction_080aa768(
    struct BattleMenu_080aa768 *menu,
    s32 result)
{
    Func_08077010(menu->primary);
    menu->control->mode = 13;
    Func_08015278(menu->display);
    menu->control->mode = 1;
    (void)result;
}

s32 Func_080aa768(void)
{
    struct BattleMenu_080aa768 *menu = Data_03001f2c;
    s32 return_code = 0;
    s32 result = 0;
    s32 state = 2;
    s32 done = 0;
    s32 index;

    menu->control->mode = 13;
    menu->control->value = 0;
    Func_080aad10();
    Func_080030f8(1);

    while (!done) {
        switch (state) {
        case 0:
            if (result < 0) {
                return_code = -1;
                done = 1;
            }
            state = 2;
            break;

        case 2:
            Func_080aa544(0);
            Func_080ad5b4(1, 0, 200, 0);
            result = Func_080ab5e4(0);
            if (result == 10) {
                state = 15;
            } else if (result < 0) {
                state = 0;
            } else {
                menu->encoded_selection = menu->primary_index;
                state = result == 7 ? 10 : 3;
            }
            break;

        case 3:
            Func_080aafb8(menu->database);
            Func_080aa544(-8);
            menu->selection = menu->member_ids[menu->primary_index];
            menu->primary = menu->member_ids[menu->primary_index];
            Func_080ad5b4(
                0,
                menu->primary_index * 56 + 48,
                54,
                0);
            result = Func_080ab5e4(1);
            for (index = 0; index < menu->line_count; index++)
                menu->lines[index] += 8;
            if (result == -2)
                done = 1;
            if (result < 0) {
                state = 2;
                break;
            }
            if (result == 3 || result == 4 || result == 8 ||
                result == 9) {
                menu->secondary =
                    menu->member_ids[menu->secondary_index];
            }
            switch (result) {
            case 1:
                state = 5;
                break;
            case 2:
                state = 6;
                break;
            case 3:
                menu->action_flags = 2;
                state = 7;
                break;
            case 4:
                menu->action_flags = 2;
                state = 9;
                break;
            case 5:
                state = 11;
                break;
            case 6:
                state = 12;
                break;
            case 8:
                menu->action_flags = 2;
                state = 13;
                break;
            case 9:
                menu->action_flags = 2;
                state = 14;
                break;
            default:
                state = 3;
                break;
            }
            break;

        case 4:
            if (result == -1) {
                return_code = -1;
                state = 2;
            } else if (menu->action_flags & 1) {
                state = 8;
            } else if (menu->action_flags & 2) {
                state = 7;
            }
            break;

        case 5:
            result = Func_080ad6d4(3);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
            /* Fall through to state 11. */
        case 11:
            Func_080f9010(139);
            Func_080771b0(
                menu->primary,
                menu->primary_value,
                menu->primary_arg);
            result = Func_080771c0(
                menu->primary,
                menu->primary_value,
                menu->primary_arg);
            FinishSingleAction_080aa768(menu, result);
            state = 2;
            break;

        case 6:
            result = Func_080ad6d4(2);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
            /* Fall through to state 12. */
        case 12:
            Func_080f9010(175);
            Func_080771b8(
                menu->primary,
                menu->primary_value,
                menu->primary_arg);
            result = Func_080771c8(
                menu->primary,
                menu->primary_value,
                menu->primary_arg);
            FinishSingleAction_080aa768(menu, result);
            state = 2;
            break;

        case 7:
            result = Func_080ad6d4(1);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
            /* Fall through to state 13. */
        case 13:
            Func_080f9010(126);
            result = Func_080771d0(
                menu->primary,
                menu->primary_value,
                menu->primary_arg,
                menu->secondary);
            Func_08077010(menu->primary);
            Func_08077010(menu->secondary);
            menu->control->mode = 13;
            Func_08015278(menu->display);
            Func_080aaf58(menu->database);
            SelectDatabaseEntry_080aa768(menu);
            menu->control->mode = 1;
            state = 0;
            break;

        case 8:
            if (menu->has_secondary == 0) {
                state = 0;
                break;
            }
            result = Func_080ab5e4(1);
            if (result == -2)
                done = 1;
            state = result < 0 ? 4 : 9;
            break;

        case 9:
            result = Func_080ad6d4(0);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
            /* Fall through to state 14. */
        case 14:
            Func_080f9010(126);
            Func_080771d0(
                menu->primary,
                menu->primary_value,
                menu->primary_arg,
                menu->secondary);
            result = Func_080771d0(
                menu->secondary,
                menu->secondary_value,
                menu->secondary_arg,
                menu->primary);
            Func_08077010(menu->primary);
            Func_08077010(menu->secondary);
            Func_080aaf58(menu->database);
            SelectDatabaseEntry_080aa768(menu);
            menu->control->mode = 1;
            state = 2;
            break;

        case 10:
            menu->selection = menu->member_ids[menu->primary_index];
            menu->primary = menu->member_ids[menu->primary_index];
            result = Func_080ae2f4();
            if (result == -2)
                done = 1;
            state = 2;
            break;

        case 15:
            result = Func_080ab314();
            if (result == -2)
                done = 1;
            state = 2;
            break;

        default:
            done = 1;
            break;
        }
    }

    return return_code;
}
