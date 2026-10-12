/* Near miss, differ 16: the native starts the 0x100000 radius (movs r0, #128) before it copies y, builds the mode and cell-table offsets in r0 where this uses r2 and r1, and keeps the cell address in r1. */
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "object_runtime.h"

struct DisplayBlock {
    u8 unknown_000[0x18a];
    s8 mode;
    s8 pending;
    u8 unknown_18c[0x40];
    u8 *object;
};

extern struct MapCell gMapBlocks[];

struct ObjectRuntime *Func_080cb8e8(void);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);

s32 Func_080cb09c(void)
{
    struct DisplayBlock *block = Ram_HeapSlots->event_work;
    struct ObjectRuntime *object = Func_080cb8e8();
    struct MapWork *map = Ram_HeapSlots->map_work;
    s32 position[3];
    struct MapCell *cell;

    if (object == NULL)
        return 0;
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    Vector_AddPolarOffset(0x100000, object->angle, position);
    if (block->mode == 3) {
        s32 x = position[0];
        s32 z = position[2];

        cell = &gMapBlocks[((x / 0x200000) & 31) + ((z / 0x200000) & 31) * 32];
    } else {
        s32 x = position[0];
        struct MapCell *cells = map->layers[0].cells;
        s32 z = position[2];

        cell = &cells[x / 0x100000 + z / 0x100000 * 128];
    }
    return cell->collision_code;
}
