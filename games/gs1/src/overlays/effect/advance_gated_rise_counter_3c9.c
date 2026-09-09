/* The +98 byte is read directly in both tests and incremented in place rather
 * than cached in a local; that shape is what the reference holds. */
#include "types.h"

/*
 * Gated by the byte flag at +99: recompute a fixed-point offset from the byte
 * counter at +98 into the u32 field at +12, call a leaf helper, then advance
 * that counter while it stays in (0, 31]. The 56-byte owner has no literal
 * pool. obj is read before it is overwritten, so it is a real argument. The
 * layout behind +12, +76, +98 and +99 is not established.
 */

extern void Func_0200b096(void *record);
void SceneEffect_AdvanceGatedRiseCounter(u8 *obj)
{
    if (*(u8 *)(obj + 99) != 0) {
        u8 counter = *(u8 *)(obj + 98);

        *(u32 *)(obj + 12) = *(u32 *)(obj + 76) + ((u32)(counter >> 2) << 16);

        Func_0200b096(obj);

        {
            if (*(u8 *)(obj + 98) != 0) {
                if (*(u8 *)(obj + 98) <= 31) {
                    ++*(u8 *)(obj + 98);
                }
            }
        }
    }
}
