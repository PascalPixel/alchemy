#include "types.h"
#include "scene.h"

/* object/replace_resource_entry.c */
s32 Resource_ResetEntry(u32 index);

void *Object_ReplaceResourceEntry(void *src, void *alt)
{
    void *ret;
    void *obj;

    obj = src;
    ret = NULL;
    if (obj != NULL) {
        if (alt == NULL) {
            FIELD_AT_OFFSET(obj, u8 *, 0x1D) = (u8)(FIELD_AT_OFFSET(obj, u8 *, 0x1D) | 1);
        } else {
            Resource_ResetEntry(FIELD_AT_OFFSET(obj, u8 *, 0x1C));
            FIELD_AT_OFFSET(obj, u8 *, 0x1C) = (u8)FIELD_AT_OFFSET(alt, u8 *, 0x1C);
            FIELD_AT_OFFSET(obj, u8 *, 0x1D) = (u8)(FIELD_AT_OFFSET(obj, u8 *, 0x1D) | 1);
            obj = alt;
        }
        ret = obj;
    }
    return ret;
}
