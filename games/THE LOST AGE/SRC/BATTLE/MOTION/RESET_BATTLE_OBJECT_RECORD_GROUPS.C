#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "RAM_BUFFER.H"

struct BattleObjectSlot *GetBattleObjectSlot(s32 object_id);
void ResetMotionRecordGroup(void *owner);
s32 ActivateBattleObjectSlot(s32 object_id);
void *BattleMotion_DestroyAllSlotObjects(void);

void ResetBattleObjectRecordGroups(struct MotionObject *object)
{
    s32 storage_kind;
    s32 remaining;
    void **record_groups;
    void *record_group;

    if (object != NULL) {
        storage_kind = object->record_storage_kind & 0xF;
        switch (storage_kind) {
        case 1:
            ResetMotionRecordGroup(object->records);
            return;
        case 2:
            record_groups = object->records;
            remaining = 3;
            do {
                record_group = *record_groups++;
                if (record_group != NULL) {
                    ResetMotionRecordGroup(record_group);
                }
                remaining -= 1;
            } while (remaining >= 0);
            break;
        }
    }
}
