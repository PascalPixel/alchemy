#include "EDITION.H"
#include "BATTLE_TYPES.H"
#include "PSYNERGY_MENU.H"
#include "TBS_EDITION.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "GLOBAL_CELLS.H"
#include "MENU_RESULT.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "UI.H"

void RenderOutput_RedrawSavedRectFar(s32 window);
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 width, s32 height, s32 style);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Menu_SetPageIcons(s32 page_size, s32 first_entry, s32 window, s32 x, s32 y);
void Menu_DrawPageIndicator(s32 window, s32 count, s32 page_size, s32 page, s32 style);
s32 UiWork_SetParamNibbleFar(s32 color);
void UiText_DrawNumberAtOffsetFar(s32 value, s32 digits, s32 layer, s32 x, s32 y);
struct BattleUnit *Owner_GetStateFar(s32 owner);
struct BattleAction *BattleAction_Get(s32 action);
extern u8 MsgAbilityName;
extern u8 MsgShortcutHelp[];
extern u8 MsgChangeCharacterHelp[];
extern u8 MsgPsynergyPp[];

s32 GameFlag_IsSet(s32 message);
struct AnimationObject;
s32 Object_InitializeMode(struct AnimationObject *object, s32 mode);
struct BattleAction *Ability_GetData(s32 action);
extern volatile u32 gKeysHeld;
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;

s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *object, s32 mode);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
s32 GameFlag_TestFar(s32 message);
s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 unused0, s32 unused1);
void UiIcon_PrepareObject(struct RenderOutput *icon);
void PsynergyMenu_CallIconRoutineWithValue(s32 menu, s32 owner);
void UiMenu_PositionCursor(s32 x, s32 y);
s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut);
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies);
s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 pane);
s32 PsynergyMenu_DrawDetailPage(s32 window, s32 *work, struct MenuResult *result);
s32 PsynergyMenu_IsActionRestricted(s32 encoded_action);
void Audio_PlayCue(s32 cue);
#define ACTION_ID_MASK 0x3fff
#define LIST_PAGE_SIZE 5

s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused, const struct MenuResult *state);

s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused, const struct MenuResult *state)
{
    u32 first_entry;
    u32 visible_count;
    u8 row;
    s32 cursor;
    struct BattleUnit *owner;
    struct BattleAction *ability;
    struct PsynergyMenuState *menu = gMenuWork;

    /* FAKEMATCH: retain the original byte-offset list cursor; the direct u16 pointer spills the menu base and changes this page by eight allocated bytes. */
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
        cursor = first_entry * sizeof(menu->psynergies[0]) +
                 ((u8 *)menu->psynergies - (u8 *)menu);
        do {
            owner = Owner_GetStateFar(menu->owner_ids[0]);
            ability = BattleAction_Get(0x3fff & *(const u16 *)(cursor + (s32)menu));

            if (ability->pp_cost > owner->pp) {
                UiWork_SetParamNibbleFar(2);
            } else if (PsynergyMenu_IsActionRestricted(0x3fff & *(const u16 *)(cursor + (s32)menu)) != 0) {
                UiWork_SetParamNibbleFar(4);
            } else {
                UiWork_SetParamNibbleFar(15);
            }

            UiText_DrawCharacterAtOffsetFar(
                (0x3fff & *(const u16 *)(cursor + (s32)menu)) + (s32)&MsgAbilityName,
#if EDITION_INTERNATIONAL
                window, 16, row * 16 + 8);
#else
                window, 32, row * 16 + 8);
#endif
            UiText_DrawNumberAtOffsetFar(ability->pp_cost, 2, window, 104, row * 16 + 8);
            UiWork_SetParamNibbleFar(15);

            row++;
            cursor += sizeof(menu->psynergies[0]);
        } while (visible_count > row);
    }

    return 1;
}

/*
 * Psynergy / action list selection loop.
 *
 * Called from Menu_ResolveSelectedAction (0x080a5cc0) with a pane
 * index of 0.  It opens the list window, collects the current owner's usable
 * actions through PsynergyMenu_CollectActions, and then runs an input loop:
 *
 *   A          confirm the highlighted entry (denied with cue 114 when the
 *              action is restricted or costs more PP than the owner has)
 *   B          cancel, returning -1
 *   L / R      rotate to the previous / next party owner, re-collecting the
 *              list until an owner with at least one entry is found
 *   Select     hold to show the shortcut prompt; Select + L / R assigns the
 *              highlighted action to shortcut slot 0 / 1
 *
 * The shared Psynergy menu record holds both panes, the owner tabs and
 * the remembered selection for each party member. The detail-page scratch
 * buffer remains private to this caller.
 *
 * PsynergyMenu_DrawDetailPage returns a value its caller discards: its epilogue returns
 * through r1, and the call sets r0 last.
 */

/*
 * types.h already supplies WaitFrames, __modsi3, Audio_PlayCue, GameFlag_IsSet,
 * Ability_GetData, UiText_DrawCharacterAtOffsetFar, UiIcon_PrepareObject and
 * Object_InitializeMode; only the names it does not carry are declared here.
 */
s32 PsynergyMenu_RunList(s32 pane)
{
    struct PsynergyMenuState *menu;
    struct BattleAction *ability;
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
    struct BattleUnit *owner;
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

    while (done == 0 && GameFlag_IsSet(0x150) == 0) {
        owner = Owner_GetStateFar(menu->owner_ids[pane]);
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

        while (GameFlag_IsSet(0x150) == 0) {
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
                    icon->unknown_0c = 0;
                    icon->sentinel = 250;
                }
                for (i = 0; i < menu->owner_count; i++) {
                    Object_InitializeMode(menu->tab_objects[i], 1);
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

            /* The shortcut prompt only appears in plain selection mode. */
            if (menu->mode == 0) {
                if ((gKeyState & KEY_SELECT) != 0 && prompt == 0) {
                    ability = Ability_GetData(
                        menu->psynergies[state.selected_index] &
                        ACTION_ID_MASK);
                    if (ability->type_0c == 0) {
                        Audio_PlayCue(114);
                    } else {
                        Audio_PlayCue(174);
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

            /*
             * Confirming in shortcut-assignment mode (mode != 0) reports the
             * highlighted action straight back with cue 130; plain selection
             * mode first rejects an empty slot, a restricted action and an
             * action the owner cannot pay for.  Both arms leave through the
             * one exit the reference shares.
             */
            if ((gKeyState & KEY_A) != 0) {
                if (menu->mode != 0) {
                    Audio_PlayCue(130);
                    result = menu->psynergies[state.selected_index];
                    done = 1;
                    break;
                }
                if (menu->psynergies[state.selected_index] == 0) {
                    goto no_accept;
                }
                if (PsynergyMenu_IsActionRestricted(
                        menu->psynergies[state.selected_index]) != 0) {
                    Audio_PlayCue(114);
                    continue;
                }
                ability = Ability_GetData(
                    menu->psynergies[state.selected_index] & ACTION_ID_MASK);
                if (ability->pp_cost > owner->pp) {
                    Audio_PlayCue(114);
                    goto no_accept;
                }
                Audio_PlayCue(173);
                result = menu->psynergies[state.selected_index];
                done = 1;
                break;
            }

        no_accept:
            if ((gKeyState & KEY_B) != 0) {
                Audio_PlayCue(113);
                result = -1;
                done = 1;
                break;
            }

            if (((gKeysRepeat & KEY_R) != 0 ||
                 (gKeysRepeat & KEY_L) != 0) &&
                (gKeysHeld & KEY_SELECT) == 0) {
                mode = menu->mode != 0 ? 1 : 2;
                Audio_PlayCue(111);
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
                        Owner_GetStateFar(menu->owner_ids[0]),
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
                ability = Ability_GetData(
                    menu->psynergies[state.selected_index] & ACTION_ID_MASK);
                if (ability->type_0c == 0) {
                    Audio_PlayCue(114);
                } else {
                    Audio_PlayCue(130);
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
                ability = Ability_GetData(
                    menu->psynergies[state.selected_index] & ACTION_ID_MASK);
                if (ability->type_0c == 0) {
                    Audio_PlayCue(114);
                } else {
                    if (PsynergyMenu_SetShortcut(
                            menu->owner_ids[pane],
                            menu->psynergies[state.selected_index],
                            1) != 0) {
                        Audio_PlayCue(130);
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
    if (GameFlag_IsSet(0x150) != 0) {
        result = -1;
    }
    WaitFrames(1);
    return result;
}

s32 PsynergyMenu_IsActionRestricted(s32 no)
{
    struct BattleAction *action = BattleAction_Get((u32)(no << 18) >> 18);
    u32 flags;

    if (action->type_0c != 0)
        goto restricted;
    flags = action->target_flags & 0xc0;
    no = 1;
    if (flags != 0xc0)
        goto done;
restricted:
    no = 0;
done:
    return no;
}
