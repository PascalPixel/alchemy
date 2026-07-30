#include "layout_guard.h"
#include "menu_result.h"
#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct ActionInfo_080a6ccc {
    u8 padding00[9];
    u8 value;
    u8 padding0a[2];
    u8 type;
};

struct Owner_080a6ccc {
    u8 padding00[0x3a];
    s16 threshold;
};

struct Actor_080a6ccc {
    u8 padding00[5];
    u8 display_mode;
    u8 padding06[6];
    s16 cursor_x;
    u8 padding0e;
    u8 cursor_style;
};

struct MenuState_080a6ccc {
    u8 padding000[8];
    s32 selected_actor;
    u8 padding00c[8];
    s32 result_windows[2];
    s8 actor_selection[4];
    s32 padding020;
    s32 window;
    u8 padding028[0x0c];
    struct MenuResult page;
    u8 padding050[0xf4];
    u16 actor_markers[8];
    u8 padding154[0x20];
    s16 selected_rows[2];
    u16 selected_actions[2];
    u8 padding17c[0x4c];
    u16 action_list[40];
    u8 action_count;
    u8 actor_count;
    u8 owner_ids[2];
    s32 description_window;
    u16 flags;
    u8 padding222[0x3e];
    s8 row_by_owner[8];
    u8 mode;
};

LAYOUT_OFFSET_GUARD(
    MenuState080a6ccc_ResultWindows,
    struct MenuState_080a6ccc,
    result_windows,
    0x14);
LAYOUT_OFFSET_GUARD(
    MenuState080a6ccc_ActorSelection,
    struct MenuState_080a6ccc,
    actor_selection,
    0x1c);
LAYOUT_OFFSET_GUARD(
    MenuState080a6ccc_Window,
    struct MenuState_080a6ccc,
    window,
    0x24);
LAYOUT_OFFSET_GUARD(
    MenuState080a6ccc_Page,
    struct MenuState_080a6ccc,
    page,
    0x34);
LAYOUT_OFFSET_GUARD(
    MenuState080a6ccc_ActorMarkers,
    struct MenuState_080a6ccc,
    actor_markers,
    0x144);
LAYOUT_OFFSET_GUARD(
    MenuState080a6ccc_ActionList,
    struct MenuState_080a6ccc,
    action_list,
    0x1c8);
LAYOUT_OFFSET_GUARD(
    MenuState080a6ccc_OwnerIds,
    struct MenuState_080a6ccc,
    owner_ids,
    0x21a);
LAYOUT_OFFSET_GUARD(
    MenuState080a6ccc_Flags,
    struct MenuState_080a6ccc,
    flags,
    0x220);
LAYOUT_OFFSET_GUARD(
    MenuState080a6ccc_RowByOwner,
    struct MenuState_080a6ccc,
    row_by_owner,
    0x260);
LAYOUT_OFFSET_GUARD(
    MenuState080a6ccc_Mode,
    struct MenuState_080a6ccc,
    mode,
    0x268);

void Func_080030f8(u32);
s32 Func_080022fc(s32, s32);
void Func_08009020(s32, s32);
void Func_08015068(s32, s32, s32, s32, s32);
void Func_08015080(s32, s32, s32, s32);
void *Func_08077008(s32);
struct ActionInfo_080a6ccc *Func_08077080(s32);
s32 Func_080770c0(s32);
void Func_080a10d0(void *, s32, s32, s32, s32, s32);
void Func_080a112c(s32, u16, s32, s32);
void Func_080a17c4(s32);
void Func_080a1804(void *, u16);
void Func_080a1a40(s32, s32);
s32 Func_080a1fd4(s32, s32, s32, void *, void *);
s32 Func_080a65e4(s32, s32, s32);
void Func_080a68a8(u16 *);
u8 Func_080a68ec(void *, u16 *, s32);
s32 Func_080a6a98(s32, s32, void *);
s32 Func_080a6b64(s32, s32, const struct MenuResult *);
s32 Func_080a735c(s32);
void Func_080f9010(s32);

/*
 * Select an action from one actor's action list.  The modal also supports
 * moving between actors, and the two shoulder-button modes immediately apply
 * the selected action before returning it to the caller.
 */
s32 Func_080a6ccc(s32 result_index)
{
    struct MenuState_080a6ccc *state =
        *(struct MenuState_080a6ccc **)0x03001f2c;
    struct Owner_080a6ccc *owner;
    struct MenuResult *page = &state->page;
    void *selected_info;
    s32 result = 0;
    s32 finished = 0;
    s32 first_draw = 1;
    s32 help_open = 0;
    s32 redraw;
    s32 selected_row = 0;
    u8 preview_scratch[20];
    u8 poll_scratch[16];
    u8 i;

    FIELD(state->result_windows[result_index], u8, 5) = 13;
    Func_080a10d0(page, 13, 3, 17, 14, 2);
    selected_info = (void *)page->value0;
    redraw = 0;

    while (!finished && Func_080770c0(0x150) == 0) {
        u16 selected_action;
        s32 poll_result;

        owner = Func_08077008(state->owner_ids[result_index]);
        state->action_count = Func_080a68ec(
            owner,
            state->action_list,
            state->mode != 0 ? 1 : 2);
        Func_080a68a8(state->action_list);
        Func_080a6a00(page, result_index);
        redraw = 1;
        first_draw = 1;
        FIELD(state->result_windows[result_index], u8, 5) = 1;

        while (Func_080770c0(0x150) == 0) {
            Func_080a1a40(88, page->value10 * 16 + 36);

            if (first_draw) {
                first_draw = 0;
                if (state->action_list[selected_row] != 0)
                    Func_080a17c4(FIELD(state, s32, 0x48 + selected_row * 4));

                if (redraw) {
                    redraw = 0;
                    Func_080030f8(1);
                    Func_080a6b64((s32)selected_info, 0, page);
                }

                Func_080a6a98((s32)selected_info, (s32)preview_scratch, page);
                state->selected_actions[result_index] =
                    state->action_list[page->value18];
                FIELD(state->description_window, u8, 5) = 13;

                selected_action = state->action_list[page->value18];
                if (selected_action != 0) {
                    struct Actor_080a6ccc *selected_actor =
                        (struct Actor_080a6ccc *)FIELD(
                            state, s32, 0x48 + page->value18 * 4);
                    selected_actor->display_mode = 9;
                    selected_actor->cursor_x = 0;
                    selected_actor->cursor_style = 250;
                }

                for (i = 0; i < state->actor_count; i++)
                    Func_08009020(FIELD(state, s32, 0x114 + i * 4), 1);
            }

            Func_080030f8(1);
            selected_row = page->value18;
            if ((*(volatile u32 *)0x03001ae8 & 4) == 0)
                poll_result = Func_080a1fd4(
                    0, page->value14, 5,
                    poll_scratch + 8,
                    poll_scratch);
            else
                poll_result = -1;

            if (poll_result == 1) {
                redraw = 1;
                first_draw = 1;
            } else if (poll_result == 0) {
                first_draw = 1;
            } else if (poll_result == -1) {
                first_draw = 0;
            }

            selected_action = state->action_list[page->value18];

            if (state->mode == 0) {
                if ((*(volatile u32 *)0x03001c94 & 4) && !help_open) {
                    if (Func_08077080(selected_action & 0x3fff)->type == 0) {
                        Func_080f9010(0x72);
                    } else {
                        Func_080f9010(0xae);
                        help_open = 1;
                        state->flags |= 2;
                        Func_08015068(
                            (s32)selected_info, 0, 88, 120, 96);
                        Func_08015080(
                            0xae1, (s32)selected_info, 0, 88);
                    }
                }
                if ((*(volatile u32 *)0x03001ae8 & 4) == 0 &&
                    help_open == 1) {
                    help_open = 0;
                    state->flags &= 0xfffd;
                    Func_08015068(
                        (s32)selected_info, 0, 88, 120, 96);
                    Func_08015080(
                        0xb89, (s32)selected_info, 0, 88);
                }
            }

            if (*(volatile u32 *)0x03001c94 & 1) {
                if (state->mode != 0) {
                    Func_080f9010(0x82);
                    result = selected_action;
                    finished = 1;
                    break;
                }
                if (selected_action != 0) {
                    if (Func_080a735c(selected_action) != 0) {
                        Func_080f9010(0x72);
                        continue;
                    }
                    if (Func_08077080(selected_action & 0x3fff)->value >
                        owner->threshold) {
                        Func_080f9010(0x72);
                        continue;
                    }
                    Func_080f9010(0xad);
                    result = selected_action;
                    finished = 1;
                    break;
                }
            } else if (*(volatile u32 *)0x03001c94 & 2) {
                Func_080f9010(0x71);
                result = -1;
                finished = 1;
                break;
            } else if (((*(volatile u32 *)0x03001b04 & 0x100) ||
                        (*(volatile u32 *)0x03001b04 & 0x200)) &&
                       ((*(volatile u32 *)0x03001ae8 & 4) == 0)) {
                s8 direction_mode = state->mode == 0 ? 2 : 1;
                s8 next_actor = state->actor_selection[result_index];

                Func_080f9010(0x6f);
                state->row_by_owner[state->owner_ids[result_index]] =
                    page->value18;
                do {
                    if (*(volatile u32 *)0x03001b04 & 0x100)
                        next_actor++;
                    else
                        next_actor--;
                    next_actor = (s8)Func_080022fc(
                        next_actor + state->actor_count,
                        state->actor_count);
                    state->selected_actor =
                        state->action_list[next_actor + 32];
                    state->owner_ids[0] =
                        state->action_list[next_actor + 32];
                    owner = Func_08077008(state->owner_ids[0]);
                    state->action_count = Func_080a68ec(
                        owner, state->action_list, direction_mode);
                } while ((s8)state->action_count == 0);

                state->actor_selection[result_index] = next_actor;
                for (i = 0; i < 4; i++)
                    state->actor_markers[i] = 30;
                state->actor_markers[next_actor] = 26;
                Func_080a112c(
                    state->window,
                    state->action_list[next_actor + 32],
                    0, 0);
                Func_080a1804(
                    state,
                    state->action_list[next_actor + 32]);
                break;
            } else if ((*(volatile u32 *)0x03001c94 & 0x200) &&
                       (*(volatile u32 *)0x03001ae8 & 4)) {
                if (Func_08077080(selected_action & 0x3fff)->type == 0) {
                    Func_080f9010(0x72);
                } else {
                    Func_080f9010(0x82);
                    if (Func_080a65e4(
                            state->owner_ids[result_index],
                            selected_action, 0)) {
                        result = selected_action;
                        state->mode = 1;
                        finished = 1;
                        break;
                    }
                }
            } else if ((*(volatile u32 *)0x03001c94 & 0x100) &&
                       (*(volatile u32 *)0x03001ae8 & 4)) {
                if (Func_08077080(selected_action & 0x3fff)->type == 0) {
                    Func_080f9010(0x72);
                } else if (Func_080a65e4(
                               state->owner_ids[result_index],
                               selected_action, 1)) {
                    Func_080f9010(0x82);
                    result = selected_action;
                    state->mode = 2;
                    finished = 1;
                    break;
                }
            }
        }
    }

    state->flags &= 0xfffd;
    Func_080a17c4(page->value10);
    state->selected_rows[result_index] = page->value18;
    state->row_by_owner[state->owner_ids[result_index]] = page->value18;
    state->selected_actions[result_index] = result;
    if (Func_080770c0(0x150) != 0)
        result = -1;
    Func_080030f8(1);
    return result;
}
