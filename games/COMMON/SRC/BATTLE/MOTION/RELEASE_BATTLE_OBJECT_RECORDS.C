#include "TYPES.H"
#include "MOTION_OBJECT.H"

void ResourceObject_ReleaseFar(void *);
#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
void Func_08020018(void *);
#endif
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
            slot->runtime_word_20 = 0;
            slot->runtime_word_24 = 0;
            record_index = 0;
            while ((record = GetMotionRecord(object, record_index)) != NULL) {
#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
                /* These editions release records through this owned entry. */
                Func_08020018(record);
#else
                ResourceObject_ReleaseFar(record);
#endif
                record_index += 1;
            }
            object->record_storage_kind = (s8)record;
            object->records = record;
        }
    }
}
