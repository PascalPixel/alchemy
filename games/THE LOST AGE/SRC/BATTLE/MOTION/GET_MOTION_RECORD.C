#include "TYPES.H"
#include "SCENE.H"
#include "MOTION_OBJECT.H"

/* ☀️'s, but ⚓️ also refuses a missing object. */
void *GetMotionRecord(struct MotionObject *object, s32 record_index)
{
    s32 storage_kind;

    if (object == NULL)
        return NULL;
    storage_kind = object->record_storage_kind & 0xF;
    if (storage_kind == 1) {
        if (record_index == 0) {
            return object->records;
        }
    } else if (storage_kind == 2) {
        return ((void **)object->records)[record_index];
    }
    return NULL;
}
