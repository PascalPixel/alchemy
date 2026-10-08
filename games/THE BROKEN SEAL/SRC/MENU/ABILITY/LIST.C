#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EDITION.H"
#include "BTLTYPE.H"
#include "PSYNERGY_MENU.H"
#include "TBS_EDITION.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "GLOBAL_CELLS.H"
#include "MENU_RESULT.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "UI.H"
#include "BTLSYS.H"
#include "text/MSG_IDS.H"
s32 GameFlag_TestFar(s32 flag);
struct BtlAct * BtlActGet(s32 action_id);
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *state, s32 flags);

#define RenderOutput_RedrawSavedRectFar ((void (*)(s32))RenderOutput_RedrawSavedRectFar)
#define gKeyState (*(volatile u32 *)&gKeyState)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)

#define UiWindow_ClearInteriorTilesFar ((void (*)(s32,s32,s32,s32,s32))UiWindow_ClearInteriorTilesFar)

void UiText_DrawNumberAtOffsetFar(s32 value, s32 digits, s32 layer, s32 x, s32 y);

struct AnimationObject;
extern volatile u32 gKeysHeld;

void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 unused0, s32 unused1);
void AudioCommand_PlayFar(s32 cue);
#define ACTION_ID_MASK 0x3fff
#define LIST_PAGE_SIZE 5

s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused, const struct MenuResult *state)
{
    u32 first_entry;
    u32 visible_count;
    u8 row;
    s32 cursor;
    struct BtlUnit *owner;
    struct BtlAct *ability;
    struct PsynergyMenuState *menu = gMenuWork;

    (void)unused;

    RenderOutput_RedrawSavedRectFar(window);
    UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

    if (2 & menu->flags) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutHelp, window, 0, 88);
    } else {
        UiText_DrawCharacterAtOffsetFar((s32)MsgChangeCharacterHelp, window, HELP_TEXT_X, 88);
    }

    first_entry = state->page * 5;
    visible_count = (u8)(state->entry_count - first_entry);
    if (visible_count > 5) {
        visible_count = 5;
    }

#if EDITION_INTERNATIONAL
    Menu_SetPageIcons(5, first_entry, window, 0x70, 0x22);
#else
    Menu_SetPageIcons(5, first_entry, window, 0x78, 0x22);
#endif
    Menu_DrawPageIndicator(window, state->entry_count, 5, state->page, 15);
#if EDITION_INTERNATIONAL
    UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window, 0x60, 0);
#else
    UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window, 0x48, 0);
#endif

    row = 0;
    if (visible_count > row) {
        cursor = first_entry;
        do {
            owner = UnitGetFar(menu->owner_ids[0]);
            ability = BtlActGet(0x3fff & menu->psynergies[cursor]);

            if (ability->pp_cost > owner->ep) {
                UiWork_SetParamNibbleFar(2);
            } else if (PsynergyMenu_IsActionRestricted(0x3fff & menu->psynergies[cursor]) != 0) {
                UiWork_SetParamNibbleFar(4);
            } else {
                UiWork_SetParamNibbleFar(15);
            }

            UiText_DrawCharacterAtOffsetFar(
                (0x3fff & menu->psynergies[cursor]) + (s32)MsgAbilityName,
#if EDITION_INTERNATIONAL
                window, 16, row * 16 + 8);
#else
                window, 32, row * 16 + 8);
#endif
            UiText_DrawNumberAtOffsetFar(ability->pp_cost, 2, window, 104, row * 16 + 8);
            UiWork_SetParamNibbleFar(15);

            row++;
            cursor++;
        } while (visible_count > row);
    }

    return 1;
}

s32 PsynergyMenu_RunList(s32 pane)
{
    struct PsynergyMenuState *menu;
    struct BtlAct *ability;
    struct RenderOutput *icon;
    s32 window;
    s32 changed;
    s32 nav;
    s8 mode;
    s32 tab;
    u8 i;
    s32 result;
    s32 redraw;
    s32 done;
    struct BtlUnit *owner;
    s32 prev;
    s32 prompt;
    s32 work[5];
    struct MenuResult state;

    menu = ((struct PsynergyMenuState *)gMenuWork);
    result = 0;
    prev = 0;
    prompt = 0;
    menu->pane_icon[pane]->active = 13;
    UiWindow_UpdateOrCreate(&menu->list_window, 13, 3, 17, 14, 2);
    window = menu->list_window;
    done = 0;

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        owner = UnitGetFar(menu->owner_ids[pane]);
        if (menu->mode != 0) {
            menu->psynergy_count =
                PsynergyMenu_CollectActions(owner, menu->psynergies, 1);
        } else {
            menu->psynergy_count =
                PsynergyMenu_CollectActions(owner, menu->psynergies, 2);
        }
        PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
        PsynergyMenu_BuildPageResult(&state, pane);
        redraw = 1;
        changed = 1;
        menu->pane_icon[pane]->active = 1;

        while (GameFlag_TestFar(0x150) == 0) {
#if EDITION_INTERNATIONAL
            UiMenu_PositionCursor(88, state.row * 16 + 36);
#else
            UiMenu_PositionCursor(98, state.row * 16 + 36);
#endif

            if (changed != 0) {
                changed = 0;
                if (menu->psynergies[prev] != 0) {
                    icon = menu->entry_icons[prev];
                    UiIcon_PrepareObject(icon);
                }
                if (redraw != 0) {
                    redraw = 0;
                    WaitFrames(1);
                    PsynergyMenu_DrawActionPage(window, 0, &state);
                }
                PsynergyMenu_DrawDetailPage(window, work, &state);
                menu->pane_action[pane] =
                    menu->psynergies[state.selected_index];
                menu->cursor_icon->active = 13;
                if (menu->psynergies[state.selected_index] != 0) {
                    icon = menu->entry_icons[state.selected_index];
                    icon->active = 9;
                    icon->anim_step = 0;
                    icon->sentinel = 250;
                }
                for (i = 0; i < menu->owner_count; i++) {
                    AnimationObjects_SelectAnimationFar(menu->tab_objects[i], 1);
                }
            }

            WaitFrames(1);
            prev = state.selected_index;

            if ((gKeysHeld & KEY_SELECT) == 0) {
                nav = Menu_HandlePageInput(
                    0, state.entry_count, LIST_PAGE_SIZE,
                    &state.row, &state.page);
            } else {
                nav = -1;
            }
            if (nav == 1) {
                redraw = 1;
                changed = 1;
            }
            if (nav == 0) {
                changed = 1;
            }
            if (nav == -1) {
                changed = 0;
            }

            if (menu->mode == 0) {
                if ((gKeyState & KEY_SELECT) != 0 && prompt == 0) {
                    ability = BtlActGet(
                        menu->psynergies[state.selected_index] &
                        ACTION_ID_MASK);
                    if (ability->type_0c == 0) {
                        AudioCommand_PlayFar(114);
                    } else {
                        AudioCommand_PlayFar(174);
                        prompt = 1;
                        menu->flags |= 2;
                        UiWindow_ClearInteriorTilesFar(window, 0, 88, 120, 96);
                        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutHelp, window, 0, 88);
                    }
                }
                if ((gKeysHeld & KEY_SELECT) == 0 && prompt == 1) {
                    prompt = 0;
                    menu->flags &= 0xfffd;
                    UiWindow_ClearInteriorTilesFar(window, 0, 88, 120, 96);
                    UiText_DrawCharacterAtOffsetFar((s32)MsgChangeCharacterHelp, window, HELP_TEXT_X, 88);
                }
            }

            if ((gKeyState & KEY_A) != 0) {
                if (menu->mode != 0) {
                    AudioCommand_PlayFar(130);
                    result = menu->psynergies[state.selected_index];
                    done = 1;
                    break;
                }
                if (menu->psynergies[state.selected_index] == 0) {
                    goto no_accept;
                }
                if (PsynergyMenu_IsActionRestricted(
                        menu->psynergies[state.selected_index]) != 0) {
                    AudioCommand_PlayFar(114);
                    continue;
                }
                ability = BtlActGet(
                    menu->psynergies[state.selected_index] & ACTION_ID_MASK);
                if (ability->pp_cost > owner->ep) {
                    AudioCommand_PlayFar(114);
                    goto no_accept;
                }
                AudioCommand_PlayFar(173);
                result = menu->psynergies[state.selected_index];
                done = 1;
                break;
            }

        no_accept:
            if ((gKeyState & KEY_B) != 0) {
                AudioCommand_PlayFar(113);
                result = -1;
                done = 1;
                break;
            }

            if (((gKeysRepeat & KEY_R) != 0 ||
                 (gKeysRepeat & KEY_L) != 0) &&
                (gKeysHeld & KEY_SELECT) == 0) {
                mode = menu->mode != 0 ? 1 : 2;
                AudioCommand_PlayFar(111);
                menu->selected_index_by_owner[menu->owner_ids[pane]] =
                    state.selected_index;
                tab = menu->tab_index[pane];
                do {
                    if ((gKeysRepeat & KEY_R) != 0) {
                        tab = tab + 1;
                    } else {
                        tab = tab - 1;
                    }
                    tab = (tab + menu->owner_count) % menu->owner_count;
                    menu->selected_owner = menu->owner_table[tab];
                    menu->owner_ids[0] = menu->owner_table[tab];
                    menu->psynergy_count = PsynergyMenu_CollectActions(
                        UnitGetFar(menu->owner_ids[0]),
                        menu->psynergies, mode);
                } while (menu->psynergy_count == 0);
                menu->tab_index[pane] = tab;
                for (i = 0; i < 4; i++) {
                    menu->row_positions[i] = 30;
                }
                menu->row_positions[tab] = 26;
                Menu_DrawOwnerStatusPanel(menu->status_window, menu->owner_table[tab], 0, 0);
                PsynergyMenu_CallIconRoutineWithValue((s32)menu, menu->owner_table[tab]);
                break;
            }

            if ((gKeyState & KEY_L) != 0 &&
                (gKeysHeld & KEY_SELECT) != 0) {
                ability = BtlActGet(
                    menu->psynergies[state.selected_index] & ACTION_ID_MASK);
                if (ability->type_0c == 0) {
                    AudioCommand_PlayFar(114);
                } else {
                    AudioCommand_PlayFar(130);
                    if (PsynergyMenu_SetShortcut(
                            menu->owner_ids[pane],
                            menu->psynergies[state.selected_index],
                            0) != 0) {
                        result = menu->psynergies[state.selected_index];
                        menu->mode = 1;
                        done = 1;
                        break;
                    }
                }
            }

            if ((gKeyState & KEY_R) != 0 &&
                (gKeysHeld & KEY_SELECT) != 0) {
                ability = BtlActGet(
                    menu->psynergies[state.selected_index] & ACTION_ID_MASK);
                if (ability->type_0c == 0) {
                    AudioCommand_PlayFar(114);
                } else {
                    if (PsynergyMenu_SetShortcut(
                            menu->owner_ids[pane],
                            menu->psynergies[state.selected_index],
                            1) != 0) {
                        AudioCommand_PlayFar(130);
                        result = menu->psynergies[state.selected_index];
                        menu->mode = 2;
                        done = 1;
                        break;
                    }
                }
            }
        }
    }

    menu->flags &= 0xfffd;
    UiIcon_PrepareObject(menu->entry_grid_cursor);
    menu->pane_row[pane] = state.selected_index;
    menu->selected_index_by_owner[menu->owner_ids[pane]] =
        state.selected_index;
    menu->pane_action[pane] = result;
    if (GameFlag_TestFar(0x150) != 0) {
        result = -1;
    }
    WaitFrames(1);
    return result;
}

s32 PsynergyMenu_IsActionRestricted(s32 no)
{
    struct BtlAct *action = BtlActGet((u32)(no << 18) >> 18);
    u32 flags;

    if (action->type_0c != 0)
        goto restricted;
    flags = action->tgt_flg & 0xc0;
    no = 1;
    if (flags != 0xc0)
        goto done;
restricted:
    no = 0;
done:
    return no;
}
