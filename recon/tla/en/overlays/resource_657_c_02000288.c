/* Near miss, differ 4 of 30: the native starts the 0x200 step (movs r2, #128) before it loads the layer offset and finishes it (lsls) right after; this form starts it after the load and finishes it after the phase load. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

extern s32 Data_020024b0;

struct ObjectRuntime *Func_02002074(void);
s32 Math_Sine(s32 angle);

void Func_02000288(void)
{
    s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->map_work + 0x108);
    struct ObjectRuntime *object = Func_02002074();
    s32 offset = Math_Sine(Data_020024b0) >> 2;

    object->y += offset;
    layer[3] += offset;
    Data_020024b0 += 0x200;
}
