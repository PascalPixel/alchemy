/*
 * Draft: Inventory_TryAddAndReturnOwner does not yet match; ported from its ☀️ twin.
 * Differing ranges: length 1000015 vs 1000000; 0x8013308..0x801330b (3 bytes); 0x80136c4..0x80136c5 (1 bytes); 0x8013864..0x8013865 (1 bytes); 0x8016be4..0x8016be5 (1 bytes); 0x8021b88..0x8021b89 (1 bytes); 0x8024ef2..0x8024ef3 (1 bytes); 0x8026786..0x8026787 (1 bytes); 0x8027002..0x8027003 (1 bytes); 0x8029892..0x8029893 (1 bytes); 0x8029bda..0x8029bdb (1 bytes); 0x802a0b6..0x802a0b7 (1 bytes); 0x8039e4a..0x8039e4b (1 bytes)
 * Links as its recon/tla/raw listing.
 */
#include "TYPES.H"
#include "SCENE.H"

s32 Inventory_AddItemFar(s32, s32);

s32 Inventory_TryAddAndReturnOwner(
    s32 item_id, s32 unused, s32 owner_id)
{
    if (Inventory_AddItemFar(owner_id, item_id) >= 0)
        return owner_id;
    return -1;
}

void Inventory_NoOpCallback(void)
{
}
