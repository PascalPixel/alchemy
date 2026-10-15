#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "map.h"

#define LAYER_WORD(layer, offset) (*(s32 *)((u8 *)(layer) + (offset)))

extern s32 gData_020024b4;
extern s32 gData_020024b0;

s32 Math_Sine(s32 angle);
struct ObjectRuntime *Func_02002074(void);

void Func_02000258(void)
{
    s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x108);
    s32 phase = gData_020024b4;

    layer[2] = phase;
    phase += 0x8000;
    gData_020024b4 = phase;
    if (phase == 0x200000)
        gData_020024b4 = 0;
}

// Bobs the object returned by Func_02002074 and the first map layer's word 12
// together on a sine wave, its phase advancing 0x200 a frame.
void Func_02000288(void)
{
    struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[0];
    struct ObjectRuntime *object = Func_02002074();
    s32 offset = Math_Sine(gData_020024b0) >> 2;
    s32 step;

    object->y += offset;
    /* FAKEMATCH: a one-pass block around the last three statements and an empty asm that takes the step after them: ⚓️ loads the layer word between the halves of 0x200 (permute; without the block or the asm differ 4, plain differ 4). */
    do {
        step = 0x200;
        LAYER_WORD(layer, 12) += offset;
        gData_020024b0 += step;
    } while (0);
    asm("" : : "l"(step));
}
