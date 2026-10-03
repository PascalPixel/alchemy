#include "BATTLE_RUNTIME.H"
#include "PSYNERGY_MENU.H"
#include "GLOBAL_CELLS.H"
#include "FIXED_MATH.H"


s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 index)
{
    s32 owner_state;
    struct PsynergyMenuState *menu = gMenuWork;
    s32 entry_count;
    s32 row;
    s32 page;
    s32 page_count;
    s32 selected_index;
    s32 owner;

    owner_state = (s32)Owner_GetStateFar(menu->owner_ids[index]);
    entry_count = menu->psynergy_count;
    owner = menu->owner_ids[index];
    selected_index = menu->selected_index_by_owner[owner];
    if ((s32)(selected_index + 1) > entry_count) {
        selected_index = entry_count - 1;
    }
    page = selected_index / 5;
    row = selected_index % 5;
    page_count = entry_count / 5;
    if (entry_count % 5 != 0) {
        page_count++;
    }
    result->owner_state = owner_state;
    result->page = page;
    result->page_count = page_count;
    result->row = row;
    result->entry_count = entry_count;
    result->selected_index = selected_index;
    return 1;
}
