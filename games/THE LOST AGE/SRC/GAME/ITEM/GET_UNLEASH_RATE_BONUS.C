#include "TYPES.H"
#include "BATTLE_UNIT.H"
#include "ITEM.H"
#include "INVENTORY.H"
#include "BATTLE_RANDOM.H"

s32 Equipment_GetUnleashRateBonus(struct BattleUnit *owner)
{
    /* FAKEMATCH: retain the existing inventory offset and byte-effect cursors;
       direct array cursors remove the four-byte call spill and reorder operands. */
    s32 sum;
    s32 offset;
    s32 index;
    u8 *data;
    s32 j;
    s32 mask;
    u16 v;

    sum = 0;
    offset = (u32)&((struct BattleUnit *)0)->inventory;
    mask = 0x200;
    index = 15;
    while (--index >= 0) {
        v = *(u16 *)((u8 *)offset + (u32)owner);
        if (v & mask) {
            data = (u8 *)Item_GetDirect(
                *(u16 *)((u8 *)offset + (u32)owner)) + (u32)&((struct ItemDefinition *)0)->effects;
            j = 4;
            while (--j >= 0) {
                if (((struct ItemEffect *)data)->kind == 23)
                    sum += ((struct ItemEffect *)data)->amount;
                data += sizeof(struct ItemEffect);
            }
        }
        offset += sizeof(owner->inventory[0]);
    }
    if (sum < 0)
        sum = 0;
    return sum;
}

/* Rolls the equipped weapon's unleash: its rate in fifths of a percent plus
   the equipment's bonus against a 16-bit random number. Returns the
   unleash's message, or 1 when there is none or the roll fails. */
s32 RollWeaponUnleash(struct BattleUnit *owner)
{
    struct ItemDefinition *item;
    s32 rate;

    if (owner->class_index == 0) {
        return 1;
    }
    item = Inventory_GetEquippedDefinition(owner, 1);
    if (item == NULL) {
        return 1;
    }
    if (item->description_message == 0) {
        return 1;
    }
    rate = ((Equipment_GetUnleashRateBonus(owner) +
         (item->secondary_flags * 5)) << 0x10) / 100;
    if (rate > (s32)(BattleRandom16() & 0xFFFF)) {
        return item->description_message;
    }
    return 1;
}
