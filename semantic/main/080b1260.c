#include "layout_guard.h"
#include "types.h"

typedef struct ActorState_080b1260 {
    u8 padding00[0x3c];
    u16 stat_3c;
    u16 stat_3e;
    u16 stat_40;
    u8 stat_42;
    u8 padding43[0x95];
    u16 slots[15];
} ActorState_080b1260;

typedef struct ItemDefinition_080b1260 {
    s16 value;
    u8 kind;
} ItemDefinition_080b1260;

typedef struct BattleUiState_080b1260 {
    u8 padding000[0x398];
    u16 comparison_down_resource;
    u16 comparison_up_resource;
} BattleUiState_080b1260;

typedef struct Indicator_080b1260 {
    u8 padding00[4];
    u8 state;
} Indicator_080b1260;

LAYOUT_OFFSET_GUARD(
    ActorState080b1260_Stats,
    ActorState_080b1260,
    stat_3c,
    0x3c);
LAYOUT_OFFSET_GUARD(
    ActorState080b1260_Slots,
    ActorState_080b1260,
    slots,
    0xd8);
LAYOUT_OFFSET_GUARD(
    BattleUiState080b1260_ComparisonDown,
    BattleUiState_080b1260,
    comparison_down_resource,
    0x398);

void Func_08015060(s32);
void Func_08015070(s32, s32, s32, s32, s32);
void Func_08015078(s32, s32, s32, s32);
void Func_08015080(s32, s32, s32, s32);
void Func_080150b0(s32, s32, s32, s32, s32);
Indicator_080b1260 *Func_080150c8(u16, s32, s32, s32, s32);
ActorState_080b1260 *Func_08077008(s32);
void Func_08077010(s32);
ItemDefinition_080b1260 *Func_08077018(u16);
s32 Func_08077218(s32, u16);
s32 Func_08077228(s32, u8);

/*
 * Temporarily equip an item, snapshot the resulting actor values, restore the
 * original slot, and render three rows comparing the candidate with the
 * baseline state.
 */
void Func_080b1260(s32 window, s32 actor, u16 item_id)
{
    BattleUiState_080b1260 *ui =
        *(BattleUiState_080b1260 **)0x03001f2c;
    ActorState_080b1260 *actor_state = Func_08077008(actor);
    ItemDefinition_080b1260 *item = Func_08077018(item_id);
    s32 candidate[4];
    s32 baseline[4];
    s32 saved_item = -1;
    s32 slot;
    u16 original_slot;
    s32 i;

    if (window == 0)
        return;

    Func_08015060(window);
    if (Func_08077218(actor, item_id) == 0) {
        Func_08015078(0x0c8e, window, 8, 24);
        return;
    }

    slot = Func_08077228(actor, item->kind);
    if (slot != -1) {
        saved_item = actor_state->slots[slot] & 0x01ff;
    } else {
        for (slot = 0; slot < 15; slot++) {
            if ((actor_state->slots[slot] & 0x0200) == 0)
                break;
        }
        if (slot == 15) {
            for (slot = 0; slot < 15; slot++) {
                if (Func_08077018(actor_state->slots[slot])->kind == 6)
                    break;
            }
            if (slot == 15)
                slot = 0;
        }
    }

    original_slot = actor_state->slots[slot];
    actor_state->slots[slot] = item_id | 0x0200;
    Func_08077010(actor);
    candidate[0] = actor_state->stat_3c;
    candidate[1] = actor_state->stat_3e;
    candidate[2] = actor_state->stat_40;
    candidate[3] = actor_state->stat_42;

    actor_state->slots[slot] = original_slot;
    Func_08077010(actor);
    baseline[0] = actor_state->stat_3c;
    baseline[1] = actor_state->stat_3e;
    baseline[2] = actor_state->stat_40;
    baseline[3] = actor_state->stat_42;

    for (i = 0; i < 3; i++) {
        s32 row = i * 16;
        s32 comparison_row = row;
        u16 indicator_resource = 0;
        s32 changed = 0;

        if (candidate[i] > baseline[i]) {
            indicator_resource = ui->comparison_up_resource;
            changed = 1;
        } else if (candidate[i] < baseline[i]) {
            indicator_resource = ui->comparison_down_resource;
            changed = 1;
        }
        if (changed != 0) {
            Indicator_080b1260 *indicator = Func_080150c8(
                indicator_resource,
                0x40000000,
                window,
                56,
                row - 4);
            indicator->state = 0;
        }

        Func_080150b0(candidate[i], 3, window, 32, row);
        if (changed != 0)
            Func_080150b0(baseline[i], 3, window, 72, comparison_row);
        Func_08015080(0x0c98 + i, window, 0, comparison_row);
        Func_08015070(window, 0, 2 + i * 2, 13, 2 + i * 2);
    }

    if (saved_item != -1)
        Func_08015080(saved_item + 0x182, window, 0, 48);
}
