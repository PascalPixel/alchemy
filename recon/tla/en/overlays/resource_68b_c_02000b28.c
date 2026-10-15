/* Draft of overlay 68b's Func_02000b28, differ 35 of 65: the BG2 wave's scanline table for
 * Func_02000ad8. The native keeps the counter and the sine in stack slots (the counter's
 * address in r7 = sp + 4, reloaded after each call, the sine stored to sp and used from
 * its register), which this draft's array through a pointer approaches; the native also
 * loads the layer's offset with ldrsh. Twin SuharaMura_AnimateCells0 is a false match. */
#include "types.h"
#include "frame.h"
#include "ram_buffer.h"

extern u8 Data_02002590;
extern s16 Data_020025a0[2][160];

s32 Math_Sine(s32 angle);

struct Ov68bLayer {
    s16 unknown_0;
    s16 offset;
    s16 unknown_4;
    s16 phase;
};

void Func_02000b28(void)
{
    struct Ov68bLayer *layer = (struct Ov68bLayer *)((u8 *)Ram_HeapSlots->map_work + 0x140);
    s16 *lines = Data_020025a0[Data_02002590 ^ 1];
    s32 v[2];
    s32 *p = v;

    for (p[1] = 0; p[1] != 160; p[1]++) {
        p[0] = Math_Sine((p[1] + *(u8 *)&gFrameCount + layer->phase) << 9);
        *lines++ = layer->offset + (p[0] >> 15) + p[1] % 3 - 1;
    }
    Data_02002590 ^= 1;
}
