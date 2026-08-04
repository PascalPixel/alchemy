#include "layout_guard.h"
#include "types.h"

typedef struct OwnerState_080b1868 {
    u8 padding_000[0xd8];
    u16 inventory_0d8[15];
} OwnerState_080b1868;

typedef struct ItemMetadata_080b1868 {
    s16 value_00;
    u8 kind_02;
    u8 flags_03;
} ItemMetadata_080b1868;

typedef struct MenuState_080b1868 {
    u8 padding_000[0x20];
    s32 detail_window_20;
} MenuState_080b1868;

LAYOUT_OFFSET_GUARD(
    OwnerState080b1868_Inventory,
    OwnerState_080b1868,
    inventory_0d8,
    0xd8);
LAYOUT_OFFSET_GUARD(
    ItemMetadata080b1868_Kind,
    ItemMetadata_080b1868,
    kind_02,
    2);
LAYOUT_OFFSET_GUARD(
    ItemMetadata080b1868_Flags,
    ItemMetadata_080b1868,
    flags_03,
    3);
LAYOUT_OFFSET_GUARD(
    MenuState080b1868_DetailWindow,
    MenuState_080b1868,
    detail_window_20,
    0x20);

extern MenuState_080b1868 *Data_03001f2c;

void Func_080030f8(s32);
void Func_08015038(s32, s32, s32, s32);
s32 Func_08015048(void);
void Func_08015120(s32, s32);
void Func_08015140(void);
OwnerState_080b1868 *Func_08077008(s32);
ItemMetadata_080b1868 *Func_08077018(s32);
void Func_08077050(s32, s32);
s32 Func_08077218(s32, u16);
s32 Func_08077228(s32, u8);
void Func_080b04dc(s32);
void Func_080b0574(s32);
s32 Func_080b0634(s32);
void Func_080b1dec(s32, s32);
void Func_080f9010(s32);

/*
 * Confirm and apply the selected inventory entry.  Entries already marked as
 * active, entries rejected for this owner, and protected replacements leave
 * the inventory unchanged.
 */
s32 Func_080b1868(s32 owner, s32 slot_index)
{
    MenuState_080b1868 *menu = Data_03001f2c;
    OwnerState_080b1868 *owner_state = Func_08077008(owner);
    u16 encoded_item = owner_state->inventory_0d8[slot_index];
    u16 item_id = encoded_item & 0x01ff;
    ItemMetadata_080b1868 *item = Func_08077018(item_id);
    s32 matching_slot;

    if ((encoded_item & 0x0200) != 0)
        return 0;

    if (Func_08077218(owner, item_id) == 0)
        return 0;

    matching_slot = Func_08077228(owner, item->kind_02);
    if (matching_slot != -1) {
        u16 replacement = owner_state->inventory_0d8[matching_slot];
        ItemMetadata_080b1868 *replacement_item =
            Func_08077018(replacement);

        if ((replacement_item->flags_03 & 2) != 0)
            return 0;
    }

    Func_08015120(owner, 1);
    Func_080b04dc(0x0ca2);
    if (Func_080b0634(0) != 0)
        return 0;

    Func_08077050(owner, slot_index);
    if (menu->detail_window_20 != 0)
        Func_080b1dec(menu->detail_window_20, owner);

    if ((item->flags_03 & 1) != 0) {
        Func_080f9010(0x67);
        Func_08015140();
        Func_08015038(0x0ad0, 8, 4, 2);
        while (Func_08015048() == 0)
            Func_080030f8(1);
    }

    Func_080b0574(0x0ca3);
    return 1;
}
