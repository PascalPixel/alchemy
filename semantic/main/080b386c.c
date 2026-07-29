#include "layout_guard.h"
#include "types.h"

typedef struct OwnerState_080b386c {
    u8 padding00[0xd8];
    u16 inventory[15];
} OwnerState_080b386c;

LAYOUT_OFFSET_GUARD(
    OwnerState080b386c_Inventory,
    OwnerState_080b386c,
    inventory,
    0xd8);

void Func_08015080(s32, s32, s32, s32);
void Func_080150b0(s32, s32, s32, s32, s32);
void Func_08015270(s32);
OwnerState_080b386c *Func_08077008(s32);
s32 Func_080772a8(s32, s32);
s32 Func_080b19cc(s32);

/*
 * Render the selected inventory entry, or the reason that entry cannot be
 * acted on, followed by its quantity-adjusted value.
 */
void Func_080b386c(s32 window, s32 owner, s32 slot_index)
{
    OwnerState_080b386c *state = Func_08077008(owner);
    u16 encoded = state->inventory[slot_index];
    s32 item_id = encoded & 0x01ff;
    s32 quantity = (encoded >> 11) + 1;
    s32 status;
    s32 status_resource;
    s32 total;

    if (window == 0)
        return;

    Func_08015270(window);
    Func_08015080(item_id + 0x182, window, 0, 0);
    status = Func_080772a8(owner, slot_index);
    if (status == -4) {
        status_resource = 0x0c94;
    } else if (status == -3) {
        status_resource = 0x0c95;
    } else {
        status_resource = 0;
    }
    if (status_resource != 0) {
        Func_08015080(status_resource, window, 0, 8);
        return;
    }

    total = quantity * Func_080b19cc(encoded);
    Func_08015080(0x0c8d, window, 8, 8);
    Func_080150b0(total, 5, window, 40, 8);
    Func_08015080(0x0c88, window, 80, 8);
}
