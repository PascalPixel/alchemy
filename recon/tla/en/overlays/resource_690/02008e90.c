/* Draft, differ 14 of 24: the native builds the 376 offset in r1 and loads
 * x through `movs r1, #2; ldrsh r2, [r2, r1]` after the first store; the
 * draft builds the offset in r3 and schedules the x load earlier. A struct
 * view of the map work (fields at 0x17a and 0x17e) is further away. */
#include "types.h"
#include "ram_buffer.h"
#include "frame.h"

extern s32 gData_02002ce0;
extern u16 gData_02002ce4;
extern u16 gData_02002ce6;

void Func_02000e90(void)
{
    s16 *view = (s16 *)((u8 *)Ram_HeapSlots->mapWork + 376);
    s32 x;

    gData_02002ce0 = 1448 - view[3];
    x = view[1];
    gData_02002ce4 = x;
    gData_02002ce6 = x - (gFrameCount >> 2);
}
