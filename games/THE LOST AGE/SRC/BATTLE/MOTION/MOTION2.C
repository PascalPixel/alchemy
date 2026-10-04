#include "TYPES.H"

void Func_0811b75c(s16 *list, s32 refresh, s32 spawn);

/* ⚓️ spawns the list's objects through a three-argument routine that ☀️
   does not have; this keeps ☀️'s two-argument entry. */
void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh)
{
    Func_0811b75c(list, refresh, 1);
}

struct MotionRecordState {
    u8 unknown_00[0x10];
    s32 field10;
};

void ResetMotionRecordGroup(void *owner)
{
    s32 remaining;
    s32 zero;
    struct MotionRecordState **items;

    if (owner != NULL) {
        zero = 0;
        items = (struct MotionRecordState **)((u8 *)owner + 0x28);
        for (remaining = 3; remaining >= 0; remaining--) {
            struct MotionRecordState *item = *items++;
            if (item != NULL) {
                item->field10 = zero;
            }
        }
    }
}
