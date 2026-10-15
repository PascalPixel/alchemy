#include "types.h"
#include "ram_buffer.h"
#include "map.h"

s32 GetWorldMapCollisionFar(s32 *position);
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer);

s32 Func_080dbd5c(s32 x, s32 z, s32 layer)
{
    u8 *event = Ram_HeapSlots->event_work;
    s32 column = x;

    /* FAKEMATCH: reuse the dead x parameter for the mode constant; an ordinary
     * constant copies x before the event load (two reordered rows). Loading
     * the mode into x instead changes the signed byte-load form (seven rows). */
    x = 3;
    if (*(s8 *)(event + 394) == x) {
        s32 position[3];
        s32 collision;

        position[1] = 0;
        position[0] = column;
        position[2] = z;
        collision = GetWorldMapCollisionFar(position);
        return (u32)(collision - 2) <= 2;
    }
    return (u8)(Func_080dbb78(column, z, layer)->attribute_b & 16);
}
