#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "RESOURCE.H"

void ReleaseBattleObjectRecords(s32 object_id)
{
    struct BattleObjectSlot *slot;
    struct MotionObject *object;
    s32 record_index;
    void *record;

    slot = GetBattleObjectSlot(object_id);
    if (slot != NULL) {
        object = slot->object;
        if (object != NULL) {
            slot->animation_entry = 0;
            slot->effect_entry = 0;
            record_index = 0;
            while ((record = GetMotionRecord(object, record_index)) != NULL) {
                ResourceObject_ReleaseFar((struct ResourceObjectWork *)record);
                record_index += 1;
            }
            object->record_storage_kind = (s8)record;
            object->records = record;
        }
    }
}
