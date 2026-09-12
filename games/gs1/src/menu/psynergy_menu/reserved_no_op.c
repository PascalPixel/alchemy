#include "types.h"
#include "scene.h"
#include "m7_interfaces.h"
#include "global_cells.h"

/* psynergy_menu/psynergy_menu_reserved_no_op.c */
/* psynergy_menu/reserved_no_op.c */
void PsynergyMenu_ReservedNoOp1BC8(void)
{
}

/* psynergy_menu/refresh_owner_entries_default.c */
void PsynergyMenu_RefreshOwnerEntriesDefault(void)
{
  int val;
  val = 0x28;
  PsynergyMenu_RefreshOwnerEntries(0x6C, val, 8);
}

/* psynergy_menu/refresh_owner_entries.c */
void PsynergyMenu_RefreshOwnerEntries(s32 origin_x, s32 origin_y, s32 phase)
{
    s32 _v1d16;
    s32 _v1t36;
    s32 _v1d75;
    struct Object080a1c **scan;
    struct Object080a1c **slot;
    struct Object080a1c *object;
    s32 index;

    index = 0;
    _v1t36 = (*(s32 *)ADDR_03001F2C);
    slot = (struct Object080a1c **)(_v1t36 + 0x48);
    scan = slot;
    do {
        object = *scan++;
        if (object != NULL) {
            PsynergyMenu_PositionOwnerEntry(slot, index, origin_x, origin_y, phase);
        }
        _v1d75 = 0; index += 1;
        _v1d16 = 0; slot++;
    } while (index <= 0x1F);
}

/* psynergy_menu/position_owner_entry.c */
s32 FixedPoint_Ratio(s32, s32);
s32 Modulo(s32, s32);

void PsynergyMenu_PositionOwnerEntry(struct Object080a1c **slot, s32 index,
    s32 origin_x, s32 origin_y,
    s32 phase) {
    struct Object080a1c *object;
    s32 wave;
    s32 stable_phase;
    s32 wrapped_index;

    wrapped_index = index;
    if (wrapped_index > 0x1F) {
        wrapped_index = 0;
    }
    stable_phase = *(volatile s32 *)&phase;
    object = *(struct Object080a1c *volatile *)slot;
    wave = FixedPoint_Ratio(wrapped_index, stable_phase);
    object->y = (s16)((wave * 0x10) + origin_y);
    object->x =
        (s16)((Modulo(wrapped_index, phase) * 0x10) + origin_x);
    UiIcon_PrepareObject(object);
}

/* item_menu/item_menu_pos_owner.c */
/* item_menu/pos_owner.c */
s32 FixedPoint_Ratio(s32, s32);
s32 Modulo(s32, s32);

void ItemMenu_PosOwner(struct Object080a1c **slot, s32 index,
    s32 origin_x, s32 origin_y,
    s32 phase) {
    struct Object080a1c *object;
    s32 wave;
    s32 stable_phase;
    s32 wrapped_index;

    wrapped_index = index;
    if (wrapped_index > 0xF) {
        wrapped_index = 0;
    }
    stable_phase = *(volatile s32 *)&phase;
    object = *(struct Object080a1c *volatile *)slot;
    wave = FixedPoint_Ratio(wrapped_index, stable_phase);
    object->y = (s16)((wave * 0x10) + origin_y);
    object->x =
        (s16)((Modulo(wrapped_index, phase) * 0x18) + origin_x);
    UiIcon_PrepareObject(object);
}

/* item_menu/refresh_entry.c */
void ItemMenu_RefreshEntry(s32 layout)
{
    struct Object080a1c **slot;
    struct Object080a1c **scan;
    struct Object080a1c *object;
    s32 index;
    s32 origin_y;
    s32 base;

    base = *(s32 *)ADDR_03001F2C;
    origin_y = 0x38;
    if (layout != 1) {
        origin_y = 0x28;
    }
    slot = (struct Object080a1c **)(base + 0x48);
    index = 0;
    scan = slot;
    do {
        object = *scan++;
        if (object != NULL) {
            ItemMenu_PosOwner(slot, index, 0x74, origin_y, 5);
        }
        index++;
        slot++;
    } while (index <= 0xE);
}
