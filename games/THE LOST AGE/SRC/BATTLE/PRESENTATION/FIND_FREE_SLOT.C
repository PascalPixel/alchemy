#include "TYPES.H"

extern u16 Resource_SlotAssignments[];

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
            /* FAKEMATCH: rereads the entry as [table, index]; cse otherwise swaps the operands */
            asm("ldrh %0, [%1, %2]" : "=r"(value) : "r"(Resource_SlotAssignments), "r"(index * 2));
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
