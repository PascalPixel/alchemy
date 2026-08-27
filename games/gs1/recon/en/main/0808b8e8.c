#include "battle_effect_runtime.h"
#include "types.h"

/*
 * Sibling of Func_0808b98c: that owner unconditionally clears every slot in
 * this same 58-entry array (Data_03001ebc + 0x34), while this one only
 * clears slots whose object has drifted outside the camera-relative bounds
 * built from the anchor object at Data_03001ebc + 0x1e0. Both share the
 * per-slot cleanup: mark object+0x54 done, clear bit 0 of
 * ((object+0x50) as u8*)[29], call Func_080090d0(object), then null the slot.
 */
void Func_080090d0(void *object);

#define BattleEffect_ClearOutOfBoundsObjects Func_0808b8e8

void BattleEffect_ClearOutOfBoundsObjects(void)
{
    struct BattleRuntime *runtime = Data_03001ebc;
    u8 *anchor = *(u8 **)((u8 *)runtime + 0x1e0);
    s32 x = *(s32 *)(anchor + 8);
    s32 z = *(s32 *)(anchor + 16);
    s32 left = x + 0xff600000;
    s32 right = x + 0x00a00000;
    s32 top = z + 0xff380000;
    s32 bottom = z + 0x00640000;
    void **slots = (void **)((u8 *)runtime + 0x34);
    s32 mask = -2;
    s32 i;

    for (i = 57; i >= 0; i--) {
        void *object = *slots;

        if (object != 0) {
            s32 objX = *(s32 *)((u8 *)object + 8);
            s32 objZ = *(s32 *)((u8 *)object + 16);

            if (objX != 0 || objZ != 0) {
                if (objX < left || objX > right || objZ < top || objZ > bottom) {
                    u8 *visual;

                    *(u8 *)((u8 *)object + 84) = 1;
                    visual = *(u8 **)((u8 *)object + 80);
                    visual[29] &= mask;
                    Func_080090d0(object);
                    *slots = 0;
                }
            }
        }

        slots++;
    }
}
