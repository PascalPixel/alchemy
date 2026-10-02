#include "TYPES.H"
#include "FIELD_SPRITE.H"

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
    struct FieldSprite *obj;

    obj = src;
    ret = NULL;
    if (obj != NULL) {
        if (alt == NULL) {
            *((u8 *)obj + 0x1d) = (u8)(*((u8 *)obj + 0x1d) | 1);
        } else {
            Resource_ResetEntry(obj->vram_block);
            obj->vram_block = (u8)((struct FieldSprite *)alt)->vram_block;
            *((u8 *)obj + 0x1d) = (u8)(*((u8 *)obj + 0x1d) | 1);
            obj = alt;
        }
        ret = obj;
    }
    return ret;
}
