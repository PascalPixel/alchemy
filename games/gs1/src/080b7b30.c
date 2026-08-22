#include "types.h"
#include "motion_object.h"

void Func_08009038(void *);
void ReleaseBattleObjectRecords(s32 object_id) {
    struct BattleObjectSlot *slot;
    struct MotionObject *object;
    s32 record_index;
    void *record;

    slot = GetBattleObjectSlot(object_id);
    if (slot != NULL) {
        object = slot->object;
        if (object != NULL) {
            slot->runtime_word_20 = 0;
            slot->runtime_word_24 = 0;
            record_index = 0;
            while ((record = GetMotionRecord(object, record_index)) != NULL) {
                Func_08009038(record);
                record_index += 1;
            }
            object->record_storage_kind = (s8) record;
            object->records = record;
        }
    }
}
