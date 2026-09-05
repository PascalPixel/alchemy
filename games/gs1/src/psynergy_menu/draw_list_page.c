#include "battle_types.h"
#include "psynergy_menu.h"

extern u8 Value_00000aed;
extern u8 Value_00000aef;
extern u8 Value_00000333;
extern u8 Value_00000741;
extern u8 Data_080af22c;

#define MsgPP Value_00000aed
#define MsgNoPsy Value_00000aef
#define MsgAction Value_00000333
#define MsgClass Value_00000741
#define StrLv Data_080af22c
#define PsyMenuWork Data_03001f2c

#define PSY_LIST_OFS 0x1c8
#define ACT_ID_MASK 0x3fff
#define OWNER_LEVEL_OFS 15
#define OWNER_CLASS_MSG_OFS 0x129
#define Menu_SetPageIcons Func_080a2324
#define UiText_DrawStringAtOffsetFar Func_08015090
#define UiText_DrawStringInWindowFar Func_08015098
#define UiText_DrawNumberAtOffsetFar Func_080150a8
#define PsynergyMenu_DrawRange Func_080a8cc0

void Menu_SetPageIcons(s32 page_size, s32 first, s32 window, s32 x, s32 y);
void Menu_DrawPageIndicator(
    s32 window, s32 item_count, s32 page_size, s32 selected_page, s32 right_edge);
void UiText_DrawStringAtOffsetFar(u8 *, void *, s32, s32);
void UiText_DrawStringInWindowFar(u8 *, s32, s32, s32);
void UiText_DrawNumberAtOffsetFar(s32, s32, s32, s32, s32);
void PsynergyMenu_DrawRange(s32, s32, s32, s32, s32);
u8 *Runtime_GetObject(s32 owner);
struct BattleAction *Ability_GetData(s32 action);

#define PsynergyMenu_DrawListPage Func_080a8f40

s32 PsynergyMenu_DrawListPage(
    s32 window, s32 unused, const struct MenuResult *res)
{
    struct PsynergyMenuState *menu = PsyMenuWork;
    u8 *owner;
    u32 first;
    u32 rows;
    u8 row;
    s32 ofs;

    (void)unused;

    owner = Runtime_GetObject(menu->owner_ids[0]);

    UiWindow_Commit(window);

    first = res->page * 5;
    rows = (u8)(res->entry_count - first);
    if (rows > 5) {
        rows = 5;
    }

    Menu_SetPageIcons(5, first, window, 80, 58);
    Menu_DrawPageIndicator(window, res->entry_count, 5, res->page, 28);

    UiText_DrawAt((s32)&MsgPP, window, 176, 0);

    row = 0;
    if (rows > row) {
        ofs = (s32)(first * 2) + PSY_LIST_OFS;
        do {
            struct BattleAction *act;
            s32 msg;
            s32 y;
            s32 range;
            act = Ability_GetData(
                ACT_ID_MASK & *(u16 *)(ofs + (s32)menu));
            msg = (*(u16 *)(ofs + (s32)menu) & ACT_ID_MASK) +
                (s32)&MsgAction;
            y = row * 16 + 16;

            UiText_DrawAt(msg, window, 88, y);
            UiText_DrawNumberAtOffsetFar(act->pp_cost, 2, window, 176, y);

            range = act->range;
            if (range == 0xff) {
                range = 11;
            } else {
                range--;
            }
            PsynergyMenu_DrawRange(window, 25, row * 2 + 2, range, 0);

            row++;
            ofs += 2;
        } while (rows > row);
    }

    if (menu->psynergy_count == 0) {
        UiText_DrawAt((s32)&MsgNoPsy, window, 96, 17);
    }

    UiText_DrawStringAtOffsetFar(owner, (void *)window, 40, 0);
    UiText_DrawAt(
        owner[OWNER_CLASS_MSG_OFS] + (s32)&MsgClass, window, 0, 32);
    UiText_DrawStringInWindowFar(&StrLv, window, 0, 48);
    UiNumber_DrawAt(owner[OWNER_LEVEL_OFS], 2, window, 24, 48);

    return 1;
}
