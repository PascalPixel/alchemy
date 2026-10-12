/* Near miss, differ 6 of 89, three reordered pairs: the native reads state->page before it finishes the heap
 * address (mov r8, r2; movs r3, #192; ldr r2, [r2, #8]; lsls r3, r3, #18) and loads the message base before it
 * finishes the mask 0x1ff (movs r0, #128; ldr r3; lsls r0, r0, #1); this form finishes each constant first.
 * Reading the page before the menu is differ 20; the message as pointer arithmetic changes nothing. */
#include "types.h"
#include "ram_buffer.h"

// エナジーメニューの作業領域（わかっている欄だけ）
struct PsynergyMenuState {
    u8 unknown_000[0x24];
    s32 list_window;
    u8 unknown_028[8];
    s32 info_window;
    u8 unknown_034[0x190];
    u16 psynergies[32];
    u8 unknown_204[0x10];
    u8 psynergy_count;
    u8 unknown_215;
    u8 pane_owner[2];
    u8 unknown_218[0x4c];
    s8 selected_index_by_owner[8];
};

struct MenuResult {
    s32 owner_state;
    s32 unknown_04;
    s32 page;
    s32 page_count;
    s32 row;
    s32 entry_count;
    s32 selected_index;
};

extern char MsgAbilityDescription[];

s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 WaitFrames(s32 frames);
void RenderOutput_RedrawSavedRectFar(s32 window);
s32 UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
s32 Render_SetTilemapFlagRect(s32 window, s32 x, s32 y, s32 width, s32 height, s32 value);

// 選んだ行の説明を出し、5 行の地の色を塗り分ける
s32 PsynergyMenu_DrawDetailPage(s32 window, s32 *work, struct MenuResult *state)
{
    struct PsynergyMenuState *menu;
    s32 combined;
    s32 row;

    menu = Ram_HeapSlots->menu_runtime;
    combined = state->page * 5;
    combined += state->row;
    state->selected_index = combined;

    if (GameFlag_Test(0x151) == 0) {
        RenderOutput_RedrawSavedRectFar(menu->info_window);
        WaitFrames(1);

        combined = state->selected_index;
        if (menu->psynergies[combined] != 0) {
            s32 masked = (s32)(MsgAbilityDescription + (menu->psynergies[combined] & 0x1ff));

            UiText_DrawCharacterAtOffsetFar(masked, menu->info_window, 0, 0);
        }
    } else {
        GameFlag_ClearBit(0x2ff);
    }

    row = 0;
    do {
        if (row == state->row) {
            Render_SetTilemapFlagRect(menu->list_window, 1, row * 2 + 1, 14, 1, 14);
        } else {
            Render_SetTilemapFlagRect(menu->list_window, 1, row * 2 + 1, 14, 1, 15);
        }
        row++;
    } while (row <= 4);

    WaitFrames(1);
    return 1;
}
