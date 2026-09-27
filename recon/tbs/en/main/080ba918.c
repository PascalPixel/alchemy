/* NONMATCHING: child values and flags are updated for every motion record.
 * Omitting the unused return removes the redundant zero. Byte mask types
 * do not alter allocation; volatile flags add duplicate reads and are not
 * retained. Remaining: value and mask consume an extra high register;
 * the first flag store moves after the record count load (50 vs 46 insns). */
/* Upper-main H1, complete [080ba918,080ba978), 96 bytes:
 * baseline 102/96 bytes, 46 differing halfwords, 38 aligned edits.
 * Exact EVENT_PLAYBACK/RUN_VALUE_SEQUENCE ignore the result. GetMotionRecord
 * returns an untyped object record. Test a shared union view for record and
 * child storage so the first flag store may alias the following count load.
 * Prior mask-width, volatile and unused-return axes are not repeated.
 * H1 result: 102/96 bytes, 49 differing halfwords, 37 aligned edits.
 * The union restores the reference first flag-store/count-load order.
 * Extra high register and inner flag temporary remain; save count is wrong.
 */
#include "TYPES.H"
#include "MOTION_OBJECT.H"

union MotionRecordView {
    struct {
        u8 filler_00[5];
        s32 value : 8;
        u8 filler_06[16];
        u32 flags : 8;
    } child;
    struct {
        u8 filler_00[39];
        u8 child_count;
        union MotionRecordView *first_child;
        union MotionRecordView *children[1];
    } record;
};

union MotionRecordView *BattleMotion_SetRecordChildValues(
    struct MotionObject *object, s32 value)
{
    s32 object_index;
    union MotionRecordView *record;
    u8 first_mask;

    object_index = 0;
    first_mask = 0xff;
    while ((record = GetMotionRecord(object, object_index)) != NULL) {
        union MotionRecordView *child;
        union MotionRecordView **children;
        s32 child_count;
        s32 zero;
        u8 inner_mask;
        s32 remaining;

        child = record->record.first_child;
        children = record->record.children;
        child->child.flags |= first_mask;
        child_count = record->record.child_count;
        child->child.value = value;
        if (child_count > 1) {
            zero = 0;
            inner_mask = 0xff;
            remaining = child_count - 1;
            do {
                child = *children++;
                child->child.value = zero;
                child->child.flags |= inner_mask;
                remaining--;
            } while (remaining != 0);
        }
        object_index++;
    }
    /* FAKEMATCH: the unused result keeps the final lookup's r0 live. */
}
