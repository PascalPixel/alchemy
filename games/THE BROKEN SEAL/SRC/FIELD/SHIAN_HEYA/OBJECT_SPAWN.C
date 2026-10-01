#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/*
 * The Xian room's object helpers: set an entry sprite's two-bit field, and
 * create an object with its sprite flags, palette and blend configured.
 */

#define NULL ((void *)0)

typedef struct {
    u8 pad[9];
    u8 lo:2;
    u8 field:2;
    u8 hi:4;
} EntrySprite;

void OverlayObject_SetEntryField(void *arg0, s32 arg1)
{
    EntrySprite *obj = *(EntrySprite **)((u8 *)arg0 + 0x50);

    obj->field = arg1;
}

void *OverlayObject_SpawnWithMode14(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{

    u8 *ret = Object_Create(arg3, arg0, arg1, arg2);

    if (ret != NULL) {
        u8 *obj = *(u8 **)(ret + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = obj[9];
        mask = -mask;
        mask &= flags;
        obj[9] = mask;
        ret[0x55] = 0;
        ret[0x59] = 8;
        Engine_ActorSetSpriteFlags(ret, 0);
        ObjectGroup_SetChildValue(ret, 14);
        Engine_ObjectSetBlendMode(ret, 1);
        return ret;
    }
    return NULL;
}

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *ret = Object_Create(arg3, arg0, arg1, arg2);

    if (ret != NULL) {
        u8 *obj = *(u8 **)(ret + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = obj[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        obj[9] = mask;
        ret[0x55] = 0;
        ret[0x59] = 8;
        Engine_ActorSetSpriteFlags(ret, 0);
        ObjectGroup_SetChildValue(ret, 15);
        ret[0x23] = (ret[0x23] & 0xfe) | 2;
        return ret;
    }
    return NULL;
}
