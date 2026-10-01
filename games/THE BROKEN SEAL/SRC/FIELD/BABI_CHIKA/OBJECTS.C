/* Overlay object setup. */
#include "BABI.H"

void OverlayObject_SetOwnerMode(u8 *object, s32 mode)
{
    struct Owner *owner = *(struct Owner **)(object + 80);
    owner->mode = mode;
}

/*
 * Create an object, clear the low bits of its owner record, and initialise it.
 * The four callees are declared at their in-overlay entry points, not at
 * veneers.  The +9 flag clear takes its mask from a local that is initialised
 * first and only afterwards ANDed with the loaded byte; written as one
 * expression it swaps which register holds the mask and which the byte.
 */
u8 *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Object_Create(kind, x, y, z);

    if (obj != 0) {
        u8 *owner = *(u8 **)(obj + 80);
        u32 mask = ~12;
        mask = mask & owner[9];
        owner[9] = (u8)mask;
        obj[85] = 0;
        obj[89] = 8;
        Engine_ActorSetSpriteFlags(obj, 0);
        ObjectGroup_SetChildValue(obj, 14);
        Engine_ObjectSetBlendMode(obj, 1);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}

/*
 * Spawn an object and prepare its owner record and flags.  The three callees
 * are declared at their in-overlay entry points, not at veneers.  Each
 * read-modify-write takes its mask from a local that is initialised first and
 * ANDed with the loaded byte afterwards, which is what puts the mask in r3 and
 * the byte in r2.  The +0x23 mask stays byte-wide as 0xfe rather than ~1,
 * which would widen it to a negated 2.
 */
u8 *OverlayObject_PrepareSpawnedObjectMode4(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Object_Create(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        u32 mask = ~12;
        u32 lo;
        mask = mask & rec[9];
        rec[9] = (u8)(mask | 4);
        obj[85] = 0;
        obj[89] = 8;
        Engine_ActorSetSpriteFlags(obj, 0);
        ObjectGroup_SetChildValue(obj, 15);
        lo = 0xfe;
        lo = lo & obj[35];
        obj[35] = (u8)(lo | 2);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}
