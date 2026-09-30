/* NONMATCHING: 3 halfwords; the reference sets i = 0 before the first
   field load, here it is scheduled after. Declaration order and loop forms
   do not move it. */
#include "TYPES.H"
#include "RAM_BUFFER.H"

extern s32 Summon_IsEntryFlagged(s32 index);

struct Layout {
    u8 pad[4];
    s16 field[6];
};

s32 Summon_ClassValid(s32 arg0)
{
    s32 retval;
    s32 i;
    struct Layout *ptr;

    retval = Summon_IsEntryFlagged(arg0);
    ptr = (struct Layout *)Ram_HeapSlots->battle_work;
    i = 0;
    asm volatile("" ::: "memory"); /* FAKEMATCH: i is cleared before the first field load */
    for (; i < 6; i++) {
        if (ptr->field[i] != 0)
            continue;
        if (retval != 0)
            break;
        if (i <= 4 && ptr->field[i + 1] == 0)
            break;
    }
    return i != 6;
}
