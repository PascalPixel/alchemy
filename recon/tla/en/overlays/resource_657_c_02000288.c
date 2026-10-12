/* Draft of overlay 657's Func_02000288, differ 2 of 30: the native builds
 * the angle step 0x200 as movs r2, #128 / ldr r3, [r5, #12] / lsls r2, r2, #2,
 * with the layer word's load between the halves; with the step forced into a
 * register before the layer line (the empty asm barrier below) the halves
 * stay together (movs; lsls; ldr). Without the barrier the step is built
 * after the layer store (differ 4). Tried: a named step with and without the
 * barrier at each of the three statement gaps, one-pass blocks around either
 * statement, the angle read before the layer line. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "map.h"

#define LAYER_WORD(layer, offset) (*(s32 *)((u8 *)(layer) + (offset)))

extern s32 Data_020024b0;
s32 Math_Sine(s32 angle);
struct ObjectRuntime *Func_02002074(void);

void Func_02000288(void)
{
    struct MapLayer *layer = &Ram_HeapSlots->map_work->layers[0];
    struct ObjectRuntime *object = Func_02002074();
    s32 offset = Math_Sine(Data_020024b0) >> 2;
    s32 step;

    object->y += offset;
    step = 0x200;
    /* FAKEMATCH: the empty asm forces the step into a register before the layer line (differ 4 without it). */
    asm volatile("" : "+l"(step));
    LAYER_WORD(layer, 12) += offset;
    Data_020024b0 += step;
}
