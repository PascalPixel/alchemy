/* Near miss: Func_020005f0 and Func_02000630, each differ 26 of 31 (registers only, one row moved).
 * The native zeroes the found slot in r4 and then copies that register into the index
 * (adds r0, r4, #0) after the first id load; every form tried builds the index with its own
 * movs #0 and numbers work, index and offset r0, r1, r2 where the native has r2, r0, r1.
 * Tried: found = NULL before or in the declaration, the index initialised from found
 * (CSE folds the copy back to a constant), a do-while, the lookup inline or in the body.
 * The permuter (4 minutes, 9603 candidates) found nothing nearer. */
#include "types.h"
#include "worka0.h"

struct A4SlotValues {
    s16 id;
    s16 unk02;
    s32 value04;
    u8 unk08[8];
    s32 value10;
    u8 unk14[8];
};

static inline struct A4SlotValues *FindSlot(s32 id)
{
    struct WorkA4 *work = WORK_A4;
    struct A4SlotValues *found = NULL;
    s32 i;

    for (i = 0; i < 8; i++) {
        if (work->slots[i].id == id) {
            found = (struct A4SlotValues *)&work->slots[i];
            break;
        }
    }
    return found;
}

void Func_020005f0(void)
{
    struct A4SlotValues *slot = FindSlot(8);

    slot->value10 = 0x4000;
    slot->value04 = 64;
}

void Func_02000630(void)
{
    struct A4SlotValues *slot = FindSlot(8);

    slot->value10 = 0x4000;
    slot->value04 += 128;
}
