#include "types.h"
#include "motion_object.h"

struct BattleObjectSlot *Func_080b7dd0(s32 object_id);
void Func_08009038(void *);
void *Func_080b7f70(struct MotionObject *object, s32 record_index);
void Func_080b7b30(s32 object_id) {
    struct BattleObjectSlot *slot;
    struct MotionObject *object;
    s32 record_index;
    void *record;

    slot = Func_080b7dd0(object_id);
    if (slot != NULL) {
        object = slot->object;
        if (object != NULL) {
            slot->runtime_word_20 = 0;
            slot->runtime_word_24 = 0;
            record_index = 0;
            while ((record = Func_080b7f70(object, record_index)) != NULL) {
                Func_08009038(record);
                record_index += 1;
            }
            object->record_storage_kind = (s8) record;
            object->records = record;
        }
    }
}
