/* Draft, differ 2 of 24: the listing loads the effect work pointer before it builds 0x7c4; this draft builds the offset first. Tried: the flag through a local, an s16 index, a one-pass block after the load or around it, a minute of alchemy permute. Also tried: the struct view of overlay 655 (EffectWork7c4, the same 2); an empty asm after the load, or a one-pass block around it, orders the load and the offset as the native but lifts the frame's sub sp above them (2); 3 more minutes of alchemy permute (nothing better). In sched2 the offset's three-insn chain outranks the load. */
#include "../../../../games/tla/src/scene/yampi_desert/scene.h"

void Func_02002c6c(void)
{
    if (*(s16 *)((u8 *)Ram_HeapSlots->effect_work + 0x7c4) != 0)
        Func_02003ff0(73, 76, 78, 11, 1, 1);
}
