#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "ANIMSPR.H"

/*
 * Walks every motion record of an object: the first child of each record
 * takes the value, the others are cleared, and every child's frame
 * is reset to the selected-frame sentinel. The result is unused by every caller.
 */
s32 BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 value)
{
    s32 index = 0;
    s32 child_index;
    s32 count;
    struct AnimationObject *record;
    struct AnimationEntry *child;
    struct AnimationEntry **children;
    /* Attempts: direct stores or |= 0xff shrink this extent from 94 to 82 bytes. */
    /* FAKEMATCH: cached compound assignment keeps 94 bytes but changes
       registers and store order. The cached byte read preserves allocation. */
    u8 mask = 0xff;
    u8 frame;

    while ((record = GetMotionRecord(object, index)) != NULL) {
        children = &record->entries[1];
        child = record->entries[0];
        frame = child->frame;
        child->frame = frame | mask;
        count = record->count;
        child->param = value;
        for (child_index = 1; child_index < count; child_index++) {
            child = *children++;
            child->param = 0;
            child->frame = 0xff;
        }
        index++;
    }
}
