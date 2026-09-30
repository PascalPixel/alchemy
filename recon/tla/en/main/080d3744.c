/*
 * Draft: Object_SetPartAttribute does not yet match; the reference clears
 * the index before loading the part count, here after (1 swap). Permuting
 * 90s and an asm pin did not place it.
 * Links as recon/tla/raw/080d3600.s.
 */
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"

struct ObjectPart {
    u8 unknown_00[5];
    u8 attribute;
    u8 unknown_06[0xa];
    void *graphics;
};

struct ObjectPartList {
    u8 unknown_00[0x19];
    u8 dirty;
    u8 unknown_1a;
    u8 count;
    u8 unknown_1c[0xc];
    struct ObjectPart *parts[1];
};

/* For a multi-part object (animation kind 1), sets the attribute byte of
   every part that has graphics and marks the part list dirty. */
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute)
{
    struct ObjectPartList *list;
    struct ObjectPart *part;
    s32 i;

    if ((object->animation_kind & 0xf) == 1) {
        list = object->animation;
        for (i = 0; i < list->count; i++) {
            part = list->parts[i];
            if (part != 0 && part->graphics != 0)
                part->attribute = attribute;
        }
        list->dirty = 1;
    }
}
