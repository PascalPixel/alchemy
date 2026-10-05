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

extern u32 gFrameCount;
extern const u8 Data_080f088c[4];

/* The display hook of a multi-part object (animation kind 1): every other
   frame, gives each part that has graphics the next of four attributes, and
   marks the part list dirty. */
void ObjectGroup_ApplyIndexedChildValue(struct ObjectRuntime *object)
{
    struct ObjectPartList *list;
    struct ObjectPart *part;
    s32 attribute;
    s32 i;

    if ((object->animation_kind & 0xf) == 1) {
        list = object->animation;
        attribute = Data_080f088c[(gFrameCount >> 1) & 3];
        for (i = 0; i < list->count; i++) {
            part = list->parts[i];
            if (part != 0 && part->graphics != 0) {
                part->attribute = attribute;
                /* FAKEMATCH: the ARM9 scheduling would reload the part count before this store. */
                __asm__("");
            }
        }
        list->dirty = 1;
    }
}

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
            if (part != 0 && part->graphics != 0) {
                part->attribute = attribute;
                /* FAKEMATCH: the ARM9 scheduling would reload the part count before this store. */
                __asm__("");
            }
        }
        list->dirty = 1;
    }
}
