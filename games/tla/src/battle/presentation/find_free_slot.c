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
#include "owner_state.h"
#include "character_ids.h"


struct BattleSpriteSlots
{
	s32 enemy_group;
	s16 sprite_slots[6];
};

s32 ResourceSlot_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);

void BattleMotion_ReleaseObjectSlotByValue(s32 value)
{
	struct BattleSpriteSlots *work = (struct BattleSpriteSlots *)Ram_HeapSlots->battle_work;
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

extern const u16 Data_081287d4[];
extern const u16 Data_081287e2[];
extern const u16 Data_081287f0[];
extern const u16 Data_081287fe[];
extern const u16 Data_0812880c[];
extern const u16 Data_0812881a[];
extern const u16 Data_08128828[];
extern const u16 Data_08128836[];

s32 Inventory_FindEquippedFar(s32 owner, s32 kind);

// The value of the owner's equipped weapon from its class's table, or 0.
s32 BattleUnit_LookupWeaponValueByClass(s32 owner)
{
    struct OwnerInventoryState *unit = UnitGet(owner);
    s32 slot = Inventory_FindEquippedFar(owner, 1);
    s32 result = 0;

    if (slot >= 0)
    {
        /* FAKEMATCH: the native loads the equipped item before it starts the id mask 0x1ff; plainly the mask is started first (one reordered row, differ 2). The item is pinned to the r3 it takes there and the barrier holds the mask until it is loaded; the barrier alone gives the item r0 and the mask r3 (differ 8), as does a one-pass block, and the pin alone or with an operand barrier differs 2 and 4. */
        register u32 item __asm__("r3") = unit->inventory[slot];
        s32 index;

        /* FAKEMATCH: the barrier of the pin above. */
        __asm__ volatile("");
        index = Resource_FindFreeSlot(item & 0x1ff);
        switch (unit->class_id)
        {
        case ISAAC:
            result = Data_081287d4[index];
            break;
        case GARET:
            result = Data_081287e2[index];
            break;
        case IVAN:
            result = Data_081287f0[index];
            break;
        case MIA:
            result = Data_081287fe[index];
            break;
        case JENNA:
            result = Data_0812880c[index];
            break;
        case FELIX:
            result = Data_0812881a[index];
            break;
        case SHEBA:
            result = Data_08128828[index];
            break;
        case PIERS:
            result = Data_08128836[index];
            break;
        }
    }
    return result;
}
