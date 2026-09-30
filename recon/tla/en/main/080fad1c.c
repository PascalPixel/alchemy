/*
 * Draft: ItemMenu_Count does not yet match; 3 halfwords differ from ☀️'s C, first at +0xc (adds r4, #255).
 * Links as recon/tla/raw/080fad1c.s.
 */
#include "OWNER_STATE.H"

s32 ItemMenu_Count(s32 owner_id)
{
    s32 item_id;
    s32 remaining;
    s32 count;
    u16 *slots;

    count = 0;
    slots = Owner_GetStateFar(owner_id)->inventory;
    remaining = 0xE;
    do {
        item_id = 0x1FF & *slots;
        slots += 1;
        if (item_id != 0) {
            count += 1;
        }
        remaining -= 1;
    } while (remaining >= 0);
    return count;
}
