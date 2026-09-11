#include "types.h"
#include "scene.h"
#include "abi/object/table/clear_battle_slots.h"

extern u8 *gWork;

void ObjectTable_ClearBattleSlots(void)
{
    s32 *current;
    s32 offset;
    s32 zero;
    s32 count;
    offset = 0x8C;
    zero = 0;
    count = 0x41;
    current = (s32 *)(gWork + offset * 2);
    do {
        count--;
        *current = zero;
        current--;
    } while (count >= 0);
}
