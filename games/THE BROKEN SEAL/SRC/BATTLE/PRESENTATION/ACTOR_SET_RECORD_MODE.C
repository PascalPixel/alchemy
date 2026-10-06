#include "TYPES.H"
#include "SCENE.H"
#include "MOTION_OBJECT.H"
#include "ANIMSPR.H"

void BattlePres_SetActorRecordMode(s32 actor_id, s32 mode)
{
    struct BattleObjectSlot *slot = GetBattleObjectSlot(actor_id);
    struct MotionObject *object;
    struct AnimationObject *record;
    struct AnimationObject **records;
    s32 i;

    if (slot == 0)
        return;
    object = slot->object;
    if (object == 0)
        return;
    switch (object->record_storage_kind & 15) {
    case 1:
    {
        record = object->records;
        record->part[0].object_mode = mode;
        record->part[1].object_mode = mode;
        break;
    }
    case 2:
    {
        records = (struct AnimationObject **)object->records;
        i = 0;
        do {
            record = *records++;
            if (record == 0)
                break;
            record->part[0].object_mode = mode;
            record->part[1].object_mode = mode;
            i++;
        } while (i <= 3);
        break;
    }
    }
}
