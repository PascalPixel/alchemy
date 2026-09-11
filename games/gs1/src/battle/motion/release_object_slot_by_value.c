#include "types.h"
#include "scene.h"
#include "abi/battle/motion/release_object_slot_by_value.h"
#include "global_cells.h"

s32 BattleMotion_ReleaseObjectSlotByValue(s32 value)
{
    u8 *base;
    s32 offset;
    s32 index;
    s16 item;

    base = *(u8 **)ADDR_03001E74;
    index = 0;
    do {
        offset = index * 2 + 4;
        item = *(s16 *)(offset + (u32)base);
        if (item == value) {
            Battle_SetMode(index, 0, 0, 0);
            *(s16 *)(offset + (u32)base) = 0;
        }
        index++;
    } while (index <= 5);
}
