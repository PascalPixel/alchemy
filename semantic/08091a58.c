#include "layout_guard.h"
#include "types.h"

typedef struct State_08091a58 {
    u8 padding000[0x1d8];
    s16 saved_value;
} State_08091a58;

typedef struct ItemDefinition_08091a58 {
    u8 padding00[3];
    u8 flags;
} ItemDefinition_08091a58;

LAYOUT_OFFSET_GUARD(
    State08091a58_SavedValue,
    State_08091a58,
    saved_value,
    0x1d8);
LAYOUT_OFFSET_GUARD(
    ItemDefinition08091a58_Flags,
    ItemDefinition_08091a58,
    flags,
    3);

void Func_08015040(s32, s32);
void Func_08015120(s32, s32);
void Func_08015140(void);
void *Func_08077008(s32);
ItemDefinition_08091a58 *Func_08077018(u16);
s32 Func_08077020(s32, s32);
s32 Func_08077030(s32);
void Func_08077240(u16, s32);
void Func_080772b0(s32, s32);
s32 Func_08091d84(s32);
s32 Func_080b0058(s32 *, s32 *);
void Func_080f9010(s32);

/*
 * Resolve an item's current assignment, optionally ask for a source slot, and
 * report the resulting owner while preserving the caller's saved menu value.
 */
s32 Func_08091a58(u16 item_id)
{
    State_08091a58 *state = *(State_08091a58 **)0x03001ebc;
    s32 saved_value = state->saved_value;
    s32 selected_actor;
    s32 selected_slot;
    s32 current;

    current = Func_08077030(saved_value);
    if (current != -1) {
        Func_080f9010(0x53);
        Func_08015120(item_id, 2);
        Func_08015040(0x096a, 3);
        if (current != *(s32 *)0x02000434) {
            Func_08015120(item_id, 2);
            Func_08015120(current, 1);
            Func_08015040(0x096b, 3);
        }
        state->saved_value = saved_value;
        return current;
    }

    Func_08015120(item_id, 2);
    Func_08015040(0x096a, 1);
    Func_08015040(0x0977, 1);

    for (;;) {
        Func_08015040(0x0978, 1);
        if (Func_080b0058(&selected_actor, &selected_slot) == -1) {
            ItemDefinition_08091a58 *item = Func_08077018(item_id);

            if ((item->flags & 8) != 0) {
                Func_08015120(item_id, 2);
                Func_08015040(0x097c, 1);
                continue;
            }

            Func_08015120(item_id, 2);
            Func_08015040(0x0979, 5);
            current = Func_08091d84(1);
            Func_08015140();
            if (current != 0)
                continue;

            Func_08077240(item_id, 1);
            Func_08015120(item_id, 2);
            Func_08015040(0x097a, 1);
            state->saved_value = saved_value;
            return -1;
        } else {
            s32 count;

            Func_08077008(selected_actor);
            count = Func_08077020(selected_actor, selected_slot);
            while (count > 0) {
                count--;
                Func_080772b0(selected_actor, selected_slot);
            }
        }

        current = Func_08077030(item_id);
        Func_080f9010(0x53);
        if (current == *(s32 *)0x02000434) {
            Func_08015120(item_id, 2);
            Func_08015040(0x096a, 3);
        } else {
            Func_08015120(item_id, 2);
            Func_08015120(current, 1);
            Func_08015040(0x096b, 3);
        }
        state->saved_value = saved_value;
        return current;
    }
}
