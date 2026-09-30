#include "TYPES.H"
#include "SCENE.H"

extern u8 ResourceBlockOwners[];

/* object/replace_resource_entry.c */
s32 Resource_ResetEntry(u32 index);

s32 ResourceTable_CountFreeBlocks(void)
{
    u8 *marker = ResourceBlockOwners;
    s32 free_count = 0;
    s32 remaining = 0x200;

    do {
        if (*marker++ == 0xff) {
            free_count++;
        }
        remaining--;
    } while (remaining != 0);
    return free_count;
}

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
