#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"

struct ObjectPart {
    u8 unknown_00[5];
    u8 attribute;
    u8 unknown_06[10];
    void *graphics;
};

struct ObjectPartList {
    u8 unknown_00[25];
    u8 dirty;
    u8 unknown_1a;
    u8 count;
    u8 unknown_1c[12];
    struct ObjectPart *parts[1];
};

extern u32 gFrameCount;

void ObjectGroup_ApplyIndexedChildValue(struct ObjectRuntime *object)
{
    struct ObjectPartList *list;
    struct ObjectPart *part;
    s32 attribute;
    s32 i;

    if ((object->animation_kind & 0xf) == 1) {
        list = object->animation;
        attribute = ObjectGroup_BlinkChildValues[(gFrameCount >> 1) & 3];
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
