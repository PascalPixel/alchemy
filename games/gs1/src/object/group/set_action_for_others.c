#include "object_lookup.h"
#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Object_SetAction(s32, s32);
extern u8 Data_0200048a[];

void ObjectGroup_SetActionForOthers(s32 excluded_object, s32 group_mode, s32 action) {
    s16 *active_object_id;
    s32 object;
    s32 object_id;

    object_id = 0;
    active_object_id = (s16 *)Data_0200048a;
    do {
        object = ObjectTable_Get(object_id);
        if ((object_id != *active_object_id) && (object != 0) && (object != excluded_object)) {
            FIELD_AT_OFFSET(object, s8, 0x5B) = group_mode;
            Object_SetAction(object, action);
        }
        object_id += 1;
    } while (object_id <= 0x42);
}
