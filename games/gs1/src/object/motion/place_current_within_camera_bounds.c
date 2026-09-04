#include "object_lookup.h"
#include "types.h"

#define Object_PlaceCurrentWithinCameraBounds Func_08093500

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Runtime_AllocateBlock(s32, s32);
void ObjectMotion_PlaceWithinCameraBounds(s32, s32, s32, s32);

void Object_PlaceCurrentWithinCameraBounds(s32 arg0, s32 arg1) {
    void *obj;

    obj = ObjectTable_Get();
    Runtime_AllocateBlock(0x1B, 0xCCC);
    if (obj != NULL) {
        ObjectMotion_PlaceWithinCameraBounds(FIELD_AT_OFFSET(obj, s32 *, 8), -1, FIELD_AT_OFFSET(obj, s32 *, 0x10), arg1);
    }
}
