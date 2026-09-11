#include "object_lookup.h"
#include "types.h"
#include "scene.h"
#include "abi/object/group/configure_child_value.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern u8 gRom;
void ObjectGroup_SetChildValue(void *, s32);

void ObjectGroup_ConfigureChildValue(s32 arg0, s32 arg1)
{
    s32 mode_flags;
    void *object;

    object = ObjectTable_Get();
    if (object != NULL) {
        mode_flags = 0x100 & arg1;
        if (mode_flags != 0) {
            FIELD_AT_OFFSET(object, s32 *, 0x6C) = (s32)&gRom;
            return;
        }
        FIELD_AT_OFFSET(object, s32 *, 0x6C) = mode_flags;
        ObjectGroup_SetChildValue(object, arg1);
    }
}
