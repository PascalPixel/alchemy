#include "layout_guard.h"
#include "types.h"

typedef struct MenuState_080a7d68 {
    u8 padding000[8];
    s32 selected_actor_id;
    u8 padding00c[0x10];
    s8 selection;
    u8 padding01d;
    s8 actor_count;
    u8 padding01f[0xed];
    s32 preview_window;
    u8 padding110[0x34];
    u16 markers[8];
    u8 padding154[0x74];
    u8 preview_workspace[0x40];
    u16 actor_ids[8];
    u8 preview_result;
    u8 padding219;
    u8 selected_actor_low;
    u8 padding21b[5];
    u16 preview_variant;
} MenuState_080a7d68;

LAYOUT_OFFSET_GUARD(
    MenuState080a7d68_SelectedActor,
    MenuState_080a7d68,
    selected_actor_id,
    8);
LAYOUT_OFFSET_GUARD(
    MenuState080a7d68_Selection,
    MenuState_080a7d68,
    selection,
    0x1c);
LAYOUT_OFFSET_GUARD(
    MenuState080a7d68_ActorCount,
    MenuState_080a7d68,
    actor_count,
    0x1e);
LAYOUT_OFFSET_GUARD(
    MenuState080a7d68_PreviewWindow,
    MenuState_080a7d68,
    preview_window,
    0x10c);
LAYOUT_OFFSET_GUARD(
    MenuState080a7d68_Markers,
    MenuState_080a7d68,
    markers,
    0x144);
LAYOUT_OFFSET_GUARD(
    MenuState080a7d68_Workspace,
    MenuState_080a7d68,
    preview_workspace,
    0x1c8);
LAYOUT_OFFSET_GUARD(
    MenuState080a7d68_ActorIds,
    MenuState_080a7d68,
    actor_ids,
    0x208);
LAYOUT_OFFSET_GUARD(
    MenuState080a7d68_PreviewVariant,
    MenuState_080a7d68,
    preview_variant,
    0x220);

s32 Func_080022fc(s32, s32);
void Func_080030f8(s32);
void Func_08015080(s32, s32, s32, s32);
void Func_08015270(s32);
void *Func_08077008(u16);
void Func_080a1804(MenuState_080a7d68 *, u16);
void Func_080a1a40(s32, s32);
void Func_080a3d24(void *);
void Func_080a68a8(void *);
s8 Func_080a68ec(void *, void *, s32);
void Func_080a8088(u16, s32);
void Func_080a9b94(s32, s32, s32);
void Func_080f9010(s32);

/*
 * Select an actor while rebuilding an inline preview for that actor on every
 * movement.  The three-state preview variant remains local to this modal.
 */
s32 Func_080a7d68(void)
{
    MenuState_080a7d68 *state =
        *(MenuState_080a7d68 **)0x03001f2c;
    s8 selection = state->selection;
    s8 count = state->actor_count;
    s32 preview_variant = state->preview_variant;
    s32 redraw = 1;
    s32 result;
    u32 i;

    Func_08077008(state->actor_ids[selection]);
    Func_08015270(state->preview_window);
    Func_08015080(0x0c05, state->preview_window, 0, 0);
    Func_08015080(0x0c06, state->preview_window, 0, 16);

    for (;;) {
        if (redraw != 0) {
            u16 actor_id;
            void *actor_state;

            redraw = 0;
            selection = (s8)Func_080022fc(
                selection + count,
                count);
            actor_id = state->actor_ids[selection];
            Func_08077008(actor_id);
            preview_variant = Func_080022fc(preview_variant + 3, 3);
            Func_080a8088(actor_id, preview_variant);
            Func_080a1804(state, actor_id);

            for (i = 0; i < 8; i++)
                state->markers[i] = 0x1e;
            state->markers[selection] = 0x1a;

            actor_state = Func_08077008(actor_id);
            state->preview_result = Func_080a68ec(
                actor_state,
                state->preview_workspace,
                0);
            Func_080a68a8(state->preview_workspace);
            Func_080a9b94(96, 96, 8);
            Func_080a3d24(state->preview_workspace);
        }

        Func_080a1a40(selection * 24 - 10, 16);
        Func_080030f8(1);

        if ((*(volatile u32 *)0x03001c94 & 1) != 0) {
            Func_080f9010(0x70);
            result = 1;
            break;
        }
        if ((*(volatile u32 *)0x03001c94 & 2) != 0) {
            Func_080f9010(0x71);
            result = -1;
            break;
        }
        if ((*(volatile u32 *)0x03001b04 & 0x20) != 0) {
            Func_080f9010(0x6f);
            if (count > 1) {
                selection--;
                redraw = 1;
            }
        }
        if ((*(volatile u32 *)0x03001b04 & 0x10) != 0) {
            Func_080f9010(0x6f);
            if (count > 1) {
                selection++;
                redraw = 1;
            }
        }
    }

    state->selection = selection;
    state->selected_actor_id = state->actor_ids[selection];
    state->selected_actor_low = state->actor_ids[selection];
    return result;
}
