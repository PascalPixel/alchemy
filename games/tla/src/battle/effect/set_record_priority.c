#include "types.h"

struct FxSlot {
    void *object;
    u8 unknown_04[0x26];
    u8 hidden;
};

struct FxRecord {
    u8 unknown_00[9];
    u8 low : 2;
    u8 priority : 2;
    u8 high : 4;
};

struct FxSlot *GetBattleObjectSlotFar(s32 object_id);
struct FxRecord *GetMotionRecordFar(void *object, s32 record_index);

void Func_0817b970(s32 object_id, s32 priority)
{
    struct FxSlot *slot = GetBattleObjectSlotFar(object_id);
    struct FxRecord *record;
    s32 i;

    for (i = 0; (record = GetMotionRecordFar(slot->object, i)) != NULL; i++) {
        if (slot->hidden == 0)
            record->priority = priority;
    }
}
