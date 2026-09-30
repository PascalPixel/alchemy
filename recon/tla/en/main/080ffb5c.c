#include "TYPES.H"
#include "BATTLE_TYPES.H"
void UiWindow_Commit(s32 window);

void UiWindow_SetTilemapEntryFar(s32, s32, s32, s32, s32);

s32 PsynergyMenu_DrawListPage(
    s32 window, s32 unused, const struct MenuResult *res)
{
    struct PsynergyMenuState *menu = gMenuWork;
    u8 *owner;
    u32 first;
    u32 rows;
    u8 row;
    s32 ofs;

    (void)unused;

    owner = Owner_GetStateFar(menu->owner_ids[0]);

    UiWindow_Commit(window);

    first = res->page * 5;
    rows = (u8)(res->entry_count - first);
    if (rows > 5) {
        rows = 5;
    }

    Menu_SetPageIcons(5, first, window, 80, 58);
    Menu_DrawPageIndicator(window, res->entry_count, 5, res->page, 28);

    UiText_DrawAt((s32)&MsgPsynergyPp, window, 176, 0);

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
                (s32)&MsgAbilityName;
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
        UiText_DrawAt((s32)&MsgNoPsynergy, window, 96, 17);
    }

    UiText_DrawStringAtOffsetFar(owner, (void *)window, 40, 0);
    UiText_DrawAt(
        owner[OWNER_CLASS_MSG_OFS] + (s32)&MsgClassName, window, 0, 32);
    UiText_DrawStringInWindowFar(&Menu_LvString, window, 0, 48);
    UiText_DrawNumberInWindowFar(owner[OWNER_LEVEL_OFS], 2, window, 24, 48);

    return 1;
}
