#include "types.h"

/*
 * Spawns a kind-24 object from three fields of the caller's object, sets
 * three of its byte fields, then ORs two low bits into its owner record's
 * flag byte -- a sibling routine in this overlay clears those bits instead,
 * and the two are deliberately not unified. The 88-byte owner includes its
 * one pool word, 0x0200de08, which is never dereferenced here and so stays
 * a raw literal. The field offsets are named by position only.
 */

extern u8 *Func_0200ab38(s32 kind, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0200ab3e(u8 *src, void *table);
extern void Func_0200ab50(u8 *rec, s32 arg1);

void OverlayObject_SpawnKind24AtObject(u8 *src)
{
    u8 *obj;
    u8 *rec;

    obj = Func_0200ab38(24, *(s32 *)(src + 8), *(s32 *)(src + 12), *(s32 *)(src + 16));
    if (obj == 0) {
        return;
    }

    rec = *(u8 **)(obj + 0x50);
    Func_0200ab3e(obj, (void *)0x0200de08);

    obj[0x55] = 0;
    obj[0x22] = 1;
    obj[0x23] = 2;

    if (rec != 0) {
        Func_0200ab50(rec, 2);
        rec[0x26] = 0;
        rec[9] |= 0x0C;
    }
}
