#include "TYPES.H"
#include "MOTION_OBJECT.H"

struct MotionRecordChild {
    u8 unknown_00[5];
    u8 value;
    u8 unknown_06[16];
    u8 flags;
};

struct MotionRecordNode {
    u8 unknown_00[39];
    u8 child_count;
    struct MotionRecordChild *children[1];
};

/*
 * Walks every motion record of an object: the first child of each record
 * takes the value, the others are cleared, and every child's flags are
 * filled. The result is unused by every caller.
 */
s32 BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 value)
{
    s32 index = 0;
    s32 child_index;
    s32 count;
    struct MotionRecordNode *record;
    struct MotionRecordChild *child;
    struct MotionRecordChild **children;
    u8 mask = 0xff;
    u8 flags;

    while ((record = GetMotionRecord(object, index)) != NULL) {
        children = &record->children[1];
        child = record->children[0];
        flags = child->flags;
        child->flags = flags | mask;
        count = record->child_count;
        child->value = value;
        for (child_index = 1; child_index < count; child_index++) {
            child = *children++;
            child->value = 0;
            child->flags |= 0xff;
        }
        index++;
    }
}
