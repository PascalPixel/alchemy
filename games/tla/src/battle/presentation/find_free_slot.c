#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"


struct BtlSpriteSlots
{
	s32 enm_grp;
	s16 sprite_slots[6];
};

s32 ResourceSlot_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);

void BattleMotion_ReleaseObjectSlotByValue(s32 value)
{
	struct BtlSpriteSlots *work = (struct BtlSpriteSlots *)Ram_HeapSlots->battle_work;
	s32					   index;

	for (index = 0; index <= 5; index++)
	{
		if (work->sprite_slots[index] == value)
		{
			ResourceSlot_LoadFar(index, 0, 0, 0);
			work->sprite_slots[index] = 0;
		}
	}
}

s32 Resource_FindFreeSlot(s32 key)
{
    s32 index;
    s32 entry;
    s32 result;
    u32 value;

    for (index = 0;; index++) {
        entry = Resource_SlotAssignments[index];
        /* FAKEMATCH: the plain order builds the 0x1ff mask before the load. */
        __asm__("");
        if (key == (entry & 0x1ff)) {
#ifdef MODERN
            value = Resource_SlotAssignments[index];
#else
            /* FAKEMATCH: rereads the entry as [table, index]; cse otherwise swaps the operands */
            asm("ldrh %0, [%1, %2]" : "=r"(value) : "r"(Resource_SlotAssignments), "r"(index * 2));
#endif
            result = value >> 9;
            goto done;
        }
        if ((s16)entry == -1)
            break;
    }
    result = 6;
done:
    return result;
}
