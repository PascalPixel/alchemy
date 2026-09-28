/* The gated rise counter. */
#include "CHOJO.H"

void SceneEffect_AdvanceGatedRiseCounter(u8 *obj)
{
    if (*(u8 *)(obj + 99) != 0) {
        u8 counter = *(u8 *)(obj + 98);

        *(u32 *)(obj + 12) = *(u32 *)(obj + 76) + ((u32)(counter >> 2) << 16);

        SceneActor_ParkRecord(obj);

        {
            if (*(u8 *)(obj + 98) != 0) {
                if (*(u8 *)(obj + 98) <= 31) {
                    ++*(u8 *)(obj + 98);
                }
            }
        }
    }
}
