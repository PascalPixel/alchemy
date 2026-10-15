#include "types.h"
#include "motobj.h"

struct RecordChild
{
    u8 unk00[5];
    u8 param;
    u8 unk06[0x10];
    u8 frame;
};

struct ChildRecord
{
    u8 unk00[0x1b];
    u8 count;
    u8 unk1c[0xc];
    struct RecordChild *entries[4];
};

// Sets the first child's value of each of the object's records and clears the
// others, hiding every child's frame.
void BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 value)
{
    s32 index = 0;
    s32 child_index;
    s32 count;
    struct ChildRecord *record;
    struct RecordChild *child;
    struct RecordChild **children;
    /* FAKEMATCH: The Broken Seal's two: the mask is a value and the first frame is read into a temporary, so 255 stays in a register across the call and the frame is or-ed with it (child->frame |= 0xff differs 18, |= mask 28). */
    u8 mask = 0xff;
    u8 frame;
    /* FAKEMATCH: the count read through a pointer taken first loads it before the mask is copied for the or, as the native; read directly it loads after (one reordered row, differ 2). */
    u8 *countp;

    while ((record = GetMotionRecord(object, index)) != NULL)
    {
        countp = &record->count;
        children = &record->entries[1];
        child = record->entries[0];
        frame = child->frame;
        count = *countp;
        child->param = value;
        child->frame = frame | mask;
        for (child_index = 1; child_index < count; child_index++)
        {
            child = *children++;
            child->param = 0;
            child->frame |= 0xff;
        }
        index++;
    }
}
