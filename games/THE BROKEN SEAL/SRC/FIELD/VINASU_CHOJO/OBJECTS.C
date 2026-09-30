/* Overlay object setup. */
#include "CHOJO.H"


void OverlayObject_SetRecordField1(Obj *o, u32 v)
{
    o->sub->f1 = v;
}

/* The +98 byte is read directly in both tests and incremented in place rather
 * than cached in a local; that shape is what the reference holds. */

/*
 * Gated by the byte flag at +99: recompute a fixed-point offset from the byte
 * counter at +98 into the u32 field at +12, call a leaf helper, then advance
 * that counter while it stays in (0, 31]. The 56-byte owner has no literal
 * pool. obj is read before it is overwritten, so it is a real argument. The
 * layout behind +12, +76, +98 and +99 is not established.
 */
void *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret = Object_Create(kind, x, y, z);

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
        OverlayObject_SetValue1(ret, 1);
        return ret;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *result = Engine_ObjectCreate(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        u8 *object = *(u8 **)(result + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object[9] = mask;
        result[0x55] = 0;
        result[0x59] = 8;
        Engine_ActorSetSpriteFlags(result, 0);
        ObjectGroup_SetChildValue(result, 15);
        result[0x23] = (result[0x23] & 0xfe) | 2;
        return result;
    }
    return NULL;
}
