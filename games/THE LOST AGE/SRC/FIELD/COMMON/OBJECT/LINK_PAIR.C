#include "EVENTWRK.H"
#include "TYPES.H"

void FacingObject_TurnPairToFaceEachOther(struct FacingObject *first, struct FacingObject *second);

void Object_LinkPair(s32 first_id, s32 second_id, s32 wait)
{
    void *first = ObjectTable_Get(first_id);
    void *second = ObjectTable_Get(second_id);

    if (first != NULL && second != NULL) {
        FacingObject_TurnPairToFaceEachOther(first, second);
        EventRuntime_Wait(wait);
    }
}
