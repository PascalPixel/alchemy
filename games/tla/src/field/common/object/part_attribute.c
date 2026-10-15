#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "object_runtime.h"

struct ObjectPart {
    u8 unk00[5];
    u8 attribute;
    u8 unk06[0xa];
    void *graphics;
};

struct ObjectPartList {
    u8 unk00[0x19];
    u8 dirty;
    u8 unk1a;
    u8 count;
    u8 unk1c[0xc];
    struct ObjectPart *parts[1];
};

extern u32 gFrameCount;

void ObjectGroup_ApplyIndexedChildValue(struct ObjectRuntime *object)
{
    struct ObjectPartList *list;
    struct ObjectPart *part;
    s32 attribute;
    s32 i;

    if ((object->animationKind & 0xf) == 1) {
        list = object->animation;
        attribute = gObjectGroup_BlinkChildValues[(gFrameCount >> 1) & 3];
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

    if ((object->animationKind & 0xf) == 1) {
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
