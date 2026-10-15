#include "types.h"
#include "motobj.h"

void ResourceObject_ReleaseFar(void *);
void ReleaseBattleObjectRecords(s32 object_id)
{
    struct BattleObj *slot;
    struct MotionObject *object;
    s32 record_index;
    void *record;

    slot = BattleObjGet(object_id);
    if (slot != NULL) {
        object = slot->object;
        if (object != NULL) {
            slot->runtime_word_20 = 0;
            slot->runtime_word_24 = 0;
            record_index = 0;
            while ((record = GetMotionRecord(object, record_index)) != NULL) {
                ResourceObject_ReleaseFar(record);
                record_index += 1;
            }
            object->record_storage_kind = (s8)record;
            object->records = record;
        }
    }
}
