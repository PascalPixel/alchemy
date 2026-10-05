
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "MOTION_OBJECT.H"

extern s32 Summon_IsEntryFlagged(s32 index);

/* The palette of a battle object's slot, as ☀️ reads it. */
s32 BattleMotion_GetSlotPalette(s32 id)
{
    return GetBattleObjectSlot(id)->palette;
}

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
