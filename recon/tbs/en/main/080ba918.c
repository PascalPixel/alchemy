/* NONMATCHING: child values and flags are updated for every motion record.
 * Omitting the unused return removes the redundant zero. Byte mask types
 * do not alter allocation; volatile flags add duplicate reads and are not
 * retained. Remaining: value and mask consume an extra high register;
 * the first flag store moves after the record count load (50 vs 46 insns). */
#include "TYPES.H"
#include "MOTION_OBJECT.H"

struct Child_080ba918 {
    u8 filler_00[5];
    s32 value : 8;
    u8 filler_06[16];
    u32 flags : 8;
};

struct Record_080ba918 {
    u8 filler_00[39];
    u8 child_count;
    struct Child_080ba918 *first_child;
    struct Child_080ba918 *children[1];
};

struct Record_080ba918 *BattleMotion_SetRecordChildValues(
    struct MotionObject *object, s32 value)
{
    s32 object_index;
    struct Record_080ba918 *record;
    u8 first_mask;

    object_index = 0;
    first_mask = 0xff;
    while ((record = GetMotionRecord(object, object_index)) != NULL) {
        struct Child_080ba918 *child;
        struct Child_080ba918 **children;
        s32 child_count;
        s32 zero;
        u8 inner_mask;
        s32 remaining;

        child = record->first_child;
        children = record->children;
        child->flags |= first_mask;
        child_count = record->child_count;
        child->value = value;
        if (child_count > 1) {
            zero = 0;
            inner_mask = 0xff;
            remaining = child_count - 1;
            do {
                child = *children++;
                child->value = zero;
                child->flags |= inner_mask;
                remaining--;
            } while (remaining != 0);
        }
        object_index++;
    }
    /* FAKEMATCH: the unused result keeps the final lookup's r0 live. */
}
