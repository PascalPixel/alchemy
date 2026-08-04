#include "types.h"

#define OFFSET_OF(type, member) ((u32)&(((type *)0)->member))

struct CharacterState_080b1f4c {
    u8 unknown_000[0xd8];
    u16 inventory_0d8[1];
};

struct ItemMetadata_080b1f4c {
    u8 unknown_00[3];
    u8 flags_03;
};

struct MenuState_080b1f4c {
    u8 unknown_00[0x20];
    s32 primary_window_20;
};

typedef char Assert_080b1f4c_inventory[
    OFFSET_OF(struct CharacterState_080b1f4c, inventory_0d8) == 0xd8
        ? 1 : -1];
typedef char Assert_080b1f4c_metadata_flags[
    OFFSET_OF(struct ItemMetadata_080b1f4c, flags_03) == 3 ? 1 : -1];
typedef char Assert_080b1f4c_primary_window[
    OFFSET_OF(struct MenuState_080b1f4c, primary_window_20) == 0x20
        ? 1 : -1];

extern struct MenuState_080b1f4c *Data_03001f2c;

void Func_08015120(s32 value, s32 slot);
struct CharacterState_080b1f4c *Func_08077008(s32 character);
struct ItemMetadata_080b1f4c *Func_08077018(u16 item);
void Func_08077230(s32 value);
void Func_080772b0(s32 character, s32 inventory_slot);
void Func_080b0574(s32 message);
s32 Func_080b0634(s32 mode);
void Func_080b10cc(void);
s32 Func_080b19cc(u16 raw_item);
void Func_080b1dec(s32 window, s32 character);
void Func_080f9010(s32 sound);

/*
 * Confirm and perform a sale from one inventory slot. A requested quantity of
 * -1 selects the special single-item wording used by the direct-use caller.
 */
void Func_080b1f4c(
    s32 character, s32 inventory_slot, s32 requested_quantity)
{
    struct CharacterState_080b1f4c *character_state =
        Func_08077008(character);
    u16 raw_item = character_state->inventory_0d8[inventory_slot];
    u16 item = raw_item & 0x01ff;
    struct ItemMetadata_080b1f4c *metadata = Func_08077018(item);
    s32 special = 0;
    s32 quantity = requested_quantity;
    s32 total;
    s32 prompt;

    if (quantity == -1) {
        special = 1;
        quantity = 1;
    }

    total = quantity * Func_080b19cc(raw_item);
    if (total == 0) {
        Func_08015120(item, 2);
        Func_080b0574(0x0cac);
        return;
    }

    if ((raw_item & 0x0200) != 0 &&
        (metadata->flags_03 & 2) != 0) {
        Func_08015120(item, 2);
        Func_080b0574(0x0cab);
        return;
    }

    if (special != 0)
        prompt = 0x0cb2;
    else if ((raw_item & 0x0400) != 0)
        prompt = 0x0cb1;
    else if (quantity > 1)
        prompt = 0x0cb0;
    else if ((metadata->flags_03 & 4) != 0)
        prompt = 0x0caf;
    else
        prompt = 0x0cae;

    Func_08015120(item, 2);
    Func_08015120(total, 5);
    Func_080b0574(prompt);

    if (Func_080b0634(0) != 0) {
        s32 cancelled_message;

        if ((metadata->flags_03 & 4) != 0 || special != 0)
            cancelled_message = 0x0cb6;
        else
            cancelled_message = 0x0cb4;
        Func_080b0574(cancelled_message);
        return;
    }

    Func_080f9010(0x66);
    while (quantity > 0) {
        Func_080772b0(character, inventory_slot);
        quantity--;
    }

    Func_08077230(total);
    Func_080b10cc();
    Func_080b1dec(Data_03001f2c->primary_window_20, character);

    {
        s32 completed_message;

        if ((metadata->flags_03 & 4) != 0 || special != 0)
            completed_message = 0x0cb5;
        else
            completed_message = 0x0cb3;
        Func_080b0574(completed_message);
    }
}
