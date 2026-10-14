/* Draft, differ 2 of 24: the listing loads the effect work pointer before it builds 0x7c4; this draft builds the offset first. Tried: the flag through a local, an s16 index, a one-pass block after the load or around it, a minute of alchemy permute. */
#include "../../../../games/tla/src/field/overlay_661/scene.h"

void Func_02002c6c(void)
{
    if (*(s16 *)((u8 *)Ram_HeapSlots->effect_work + 0x7c4) != 0)
        Func_02003ff0(73, 76, 78, 11, 1, 1);
}
