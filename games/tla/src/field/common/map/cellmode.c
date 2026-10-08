#include "types.h"
#include "ram_buffer.h"
#include "map.h"

s32 GetWorldMapCollisionFar(s32 *position);
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer);

s32 Func_080dbcd8(s32 x, s32 z, s32 layer)
{
    u8 *event = Ram_HeapSlots->event_work;
    s32 column;
    s32 result;

    {
        /* FAKEMATCH: ⚓️ keeps x in r0 until it has loaded the event work */
        register s32 incoming asm("r0") = x;

        /* FAKEMATCH: and copies it only then */
        asm("" : "+l"(incoming) : "l"(event));
        column = incoming;
    }
    if (*(s8 *)(event + 394) == 3) {
        s32 position[3];

        position[0] = column;
        position[1] = 0;
        position[2] = z;
        result = (u32)(GetWorldMapCollisionFar(position) - 5) <= 1;
    } else {
        s32 code = Func_080dbb78(column, z, 2)->collision_code;

        code ^= 232;
        result = (u32)(-code | code) >> 31;
        result = 1 - result;
    }
    if (*(s8 *)(event + 0xd37) != 0)
        result = 1 - ((u32)(-result | result) >> 31);
    return result;
}
