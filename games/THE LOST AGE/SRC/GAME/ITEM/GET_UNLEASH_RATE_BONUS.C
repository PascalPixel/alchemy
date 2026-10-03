#include "TYPES.H"
#include "BATTLE_UNIT.H"
#include "ITEM.H"

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
