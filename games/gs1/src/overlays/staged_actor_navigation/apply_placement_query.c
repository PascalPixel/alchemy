/* The callees are declared at their in-overlay entry points. Declaring them at
 * their veneer addresses in ROM space would route every call through a
 * veneer. */
#include "types.h"

/*
 * Apply a placement query to an actor. Func_02001d54 is an out-param helper:
 * it fills out20 and out16, a 24-byte record of which only rec[2] and rec[4]
 * are read back, and out12 and out8 passed on the stack. Its field semantics
 * are not established. On success the values thread unchanged into the two
 * finishing calls in that shape; on failure the object is left untouched.
 */

u8 *Func_02003a7c();
s32 Func_02001d54();
void Func_02003a5c();
void Func_020014d4();
void Func_02003a18();

s32 SceneActor_ApplyPlacementQuery(u8 *no)
{
    u8 *obj = Func_02003a7c(no);
    s32 out20, out16, out12, out8;
    s32 rec[6];

    if (Func_02001d54(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    {
        s32 x = out12 + rec[2];
        s32 z = out8 + rec[4];

        Func_02003a5c(x, z, out20, out16, rec[2], rec[4]);
        Func_020014d4(0, rec[2], rec[4], out20, out16, 255);
    }

    Func_02003a18(obj, 1);
    obj[0x23] &= 0xfd;

    return 1;
}
