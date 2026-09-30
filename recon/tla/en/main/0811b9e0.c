#include "TYPES.H"
#include "MOTION_OBJECT.H"
extern u8 Data_03001e74[];

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
