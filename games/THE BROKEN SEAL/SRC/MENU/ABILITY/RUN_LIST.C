#include "TYPES.H"
#include "BATTLE_TYPES.H"
#include "GLOBAL_CELLS.H"
#include "MENU_RESULT.H"

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
 * Data_03001f2c is the polymorphic menu-runtime cell (compare item_menu.h
 * and psynergy_menu.h).  Field names shared with PsynergyMenuState keep that
 * header's spellings (entry_grid_cursor 0x044, entry_icons 0x048, psynergies
 * 0x1c8, psynergy_count 0x218, owner_ids 0x21a, selected_index_by_owner
 * 0x260).  A local view is used instead of that header for two reasons: this
 * owner touches ranges the header does not describe (a per-pane icon table at
 * 0x014, per-pane tab indices at 0x01c, the owner tab objects at 0x114, the
 * tab colour words at 0x144, the per-pane selection pair at 0x174/0x178 and
 * the encoded owner table at 0x208), and it reads 0x21c as a word pointer and
 * 0x220 as a u16 flags word, both of which fall inside that header's
 * owner_ids[8].  080a5cc0.c witnesses the same 0x21c/0x220 split and uses a
 * local view for the same reason; resolving owner_ids[8] belongs to the
 * shared header, not to this owner.
 *
 * Uncertain: the element counts of pane_icon/tab_index/pane_row/pane_action
 * are inferred from the 0x14/0x1c and 0x174/0x178 spacing (two panes); only
 * pane 0 is witnessed at a call site.  The role of field_008, field_024 and
 * the 20-byte buffer handed to PsynergyMenu_DrawDetailPage is unresolved.
 *
 * PsynergyMenu_DrawDetailPage returns a value its caller discards: its epilogue returns
 * through r1, and the call sets r0 last.
 */

struct MenuEntryIcon {
    u8 unknown_00[5];
    u8 state;                    /* 0x05 */
    u8 unknown_06[6];
    u16 field_0c;                /* 0x0c */
    u8 unknown_0e;
    u8 field_0f;                 /* 0x0f */
};

struct PsynergyListWork {
    u8 unknown_000[8];
    s32 field_008;                            /* 0x008 */
    u8 unknown_00c[8];
    struct MenuEntryIcon *pane_icon[2];       /* 0x014 */
    s8 tab_index[2];                          /* 0x01c */
    u8 unknown_01e[6];
    s32 field_024;                            /* 0x024 */
    u8 unknown_028[0x0c];
    s32 list_window;                          /* 0x034 */
    u8 unknown_038[0x0c];
    struct MenuEntryIcon *entry_grid_cursor;  /* 0x044 */
    struct MenuEntryIcon *entry_icons[32];    /* 0x048 */
    u8 unknown_0c8[0x4c];
    s32 tab_objects[4];                       /* 0x114 */
    u8 unknown_124[0x20];
    u16 tab_colors[4];                        /* 0x144 */
    u8 unknown_14c[0x28];
    u16 pane_row[2];                          /* 0x174 */
    u16 pane_action[2];                       /* 0x178 */
    u8 unknown_17c[0x4c];
    u16 psynergies[32];                       /* 0x1c8 */
    u16 owner_table[8];                       /* 0x208 */
    u8 psynergy_count;                        /* 0x218 */
    u8 owner_count;                           /* 0x219 */
    u8 owner_ids[2];                          /* 0x21a */
    struct MenuEntryIcon *cursor_icon;        /* 0x21c */
    u16 flags;                                /* 0x220 */
    u8 unknown_222[0x3e];
    s8 selected_index_by_owner[8];            /* 0x260 */
    u8 mode;                                  /* 0x268 */
};

extern struct PsynergyListWork *Data_03001f2c;

void WaitFrames(s32 frames);
s32 Math_Mod(s32 value, s32 divisor);
void AnimationObjects_SelectAnimationFar(s32 object, s32 mode);
void Func_08015068(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
struct BattleUnit *Owner_GetStateFar(s32 owner);
struct BattleAction *BattleAction_Get(s32 action);
s32 GameFlag_TestFar(s32 message);
void UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void Func_080a112c(s32 window, s32 owner, s32 unused0, s32 unused1);
void UiIcon_PrepareObject(struct MenuEntryIcon *icon);
void PsynergyMenu_CallIconRoutineWithValue(void *work, s32 value);
void UiMenu_PositionCursor(s32 x, s32 y);
s32 Unnamed_080a1fd4(s32 mode, s32 count, s32 page_size, s32 *row, s32 *page);
s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut);
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies);
u8 PsynergyMenu_CollectActions(struct BattleUnit *owner, u16 *actions, s32 mode);
s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 pane);
s32 PsynergyMenu_DrawDetailPage(s32 window, s32 *work, struct MenuResult *result);
s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused, struct MenuResult *result);
s32 PsynergyMenu_IsActionRestricted(s32 encoded_action);
void Audio_PlayCue(s32 cue);

/*
 * types.h already supplies WaitFrames, Modulo, Audio_PlayCue, GameFlag_IsSet,
 * Ability_GetData, UiText_DrawAt, UiIcon_PrepareObject and
 * Object_InitializeMode; only the names it does not carry are declared here.
 */

#define INPUT_NEW_KEYS (*(volatile u32 *)ADDR_03001C94)
#define INPUT_HELD_KEYS (*(volatile u32 *)ADDR_03001AE8)
#define INPUT_REPEAT_KEYS (*(volatile u32 *)ADDR_03001B04)

#define KEY_A 1
#define KEY_B 2
#define KEY_SELECT 4
#define KEY_R 0x100
#define KEY_L 0x200

#define ACTION_ID_MASK 0x3fff
#define LIST_PAGE_SIZE 5

s32 PsynergyMenu_RunList(s32 pane)
{
    struct PsynergyListWork *menu;
    struct BattleAction *ability;
    struct MenuEntryIcon *icon;
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

    menu = Data_03001f2c;
    result = 0;
    prev = 0;
    prompt = 0;
    menu->pane_icon[pane]->state = 13;
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
        menu->pane_icon[pane]->state = 1;

        while (GameFlag_IsSet(0x150) == 0) {
            UiMenu_PositionCursor(88, state.row * 16 + 36);

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
                menu->cursor_icon->state = 13;
                if (menu->psynergies[state.selected_index] != 0) {
                    icon = menu->entry_icons[state.selected_index];
                    icon->state = 9;
                    icon->field_0c = 0;
                    icon->field_0f = 250;
                }
                for (i = 0; i < menu->owner_count; i++) {
                    Object_InitializeMode(menu->tab_objects[i], 1);
                }
            }

            WaitFrames(1);
            prev = state.selected_index;

            if ((INPUT_HELD_KEYS & KEY_SELECT) == 0) {
                nav = Unnamed_080a1fd4(
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
                if ((INPUT_NEW_KEYS & KEY_SELECT) != 0 && prompt == 0) {
                    ability = Ability_GetData(
                        menu->psynergies[state.selected_index] &
                        ACTION_ID_MASK);
                    if (ability->type_0c == 0) {
                        Audio_PlayCue(114);
                    } else {
                        Audio_PlayCue(174);
                        prompt = 1;
                        menu->flags |= 2;
                        Func_08015068(window, 0, 88, 120, 96);
                        UiText_DrawAt(0xae1, window, 0, 88);
                    }
                }
                if ((INPUT_HELD_KEYS & KEY_SELECT) == 0 && prompt == 1) {
                    prompt = 0;
                    menu->flags &= 0xfffd;
                    Func_08015068(window, 0, 88, 120, 96);
                    UiText_DrawAt(0xb89, window, 0, 88);
                }
            }

            /*
             * Confirming in shortcut-assignment mode (mode != 0) reports the
             * highlighted action straight back with cue 130; plain selection
             * mode first rejects an empty slot, a restricted action and an
             * action the owner cannot pay for.  Both arms leave through the
             * one exit the reference shares.
             */
            if ((INPUT_NEW_KEYS & KEY_A) != 0) {
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
            if ((INPUT_NEW_KEYS & KEY_B) != 0) {
                Audio_PlayCue(113);
                result = -1;
                done = 1;
                break;
            }

            if (((INPUT_REPEAT_KEYS & KEY_R) != 0 ||
                 (INPUT_REPEAT_KEYS & KEY_L) != 0) &&
                (INPUT_HELD_KEYS & KEY_SELECT) == 0) {
                mode = menu->mode != 0 ? 1 : 2;
                Audio_PlayCue(111);
                menu->selected_index_by_owner[menu->owner_ids[pane]] =
                    state.selected_index;
                tab = menu->tab_index[pane];
                do {
                    if ((INPUT_REPEAT_KEYS & KEY_R) != 0) {
                        tab = tab + 1;
                    } else {
                        tab = tab - 1;
                    }
                    tab = Modulo(tab + menu->owner_count, menu->owner_count);
                    menu->field_008 = menu->owner_table[tab];
                    menu->owner_ids[0] = menu->owner_table[tab];
                    menu->psynergy_count = PsynergyMenu_CollectActions(
                        Owner_GetStateFar(menu->owner_ids[0]),
                        menu->psynergies, mode);
                } while (menu->psynergy_count == 0);
                menu->tab_index[pane] = tab;
                for (i = 0; i < 4; i++) {
                    menu->tab_colors[i] = 30;
                }
                menu->tab_colors[tab] = 26;
                Func_080a112c(menu->field_024, menu->owner_table[tab], 0, 0);
                PsynergyMenu_CallIconRoutineWithValue(
                    menu, menu->owner_table[tab]);
                break;
            }

            if ((INPUT_NEW_KEYS & KEY_L) != 0 &&
                (INPUT_HELD_KEYS & KEY_SELECT) != 0) {
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

            if ((INPUT_NEW_KEYS & KEY_R) != 0 &&
                (INPUT_HELD_KEYS & KEY_SELECT) != 0) {
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
