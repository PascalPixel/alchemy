#include "types.h"

/*
 * One symbol per call site.  These names stand for loader-relocated call
 * words in the overlay's veneer table, not runtime addresses.
 */
extern u8 *Func_02007168(s32 no);
extern void Func_02006f74(void);
extern void Func_02006f5c(u8 *obj, s32 x);
extern void Func_02006f98(u8 *obj, s32 x, s32 z, s32 arg3);
extern void Func_02006fa6(u8 *obj);
extern void Func_02006f76(u8 *obj, s32 x);

/*
 * resource_3bb owner at 0x020030e8, 80 bytes plus one alignment halfword:
 * spawn an object and run a short fixed setup sequence on it.  `no' is
 * forwarded unchanged -- it is never freshly loaded before the first call.
 * The object's fields (0x30 and 0x34 taking the fixed 0x14000/0xa000 pair,
 * 0x5b cleared, 0xc read back for the position call) are named by position
 * from call shape alone and are not verified.
 */
void SceneActor_PlaceWithScale14000(s32 no, s32 x, s32 z)
{
    u8 *obj = Func_02007168(no);

    if (obj == 0) {
        return;
    }

    {
        s32 scale = 0x14000;
        u8 *p = obj;
        u8 zero = 0;
        *(s32 *)(obj + 0x30) = scale;
        *(s32 *)(obj + 0x34) = scale >> 1;
        p += 0x5b;
        *p = zero;
    }

    Func_02006f74();
    Func_02006f5c(obj, 5);
    Func_02006f98(obj, x << 16, *(s32 *)(obj + 12), z << 16);
    Func_02006fa6(obj);
    Func_02006f76(obj, 1);
}
