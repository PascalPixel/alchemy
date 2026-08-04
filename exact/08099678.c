#include "object_lookup.h"
#include "types.h"
#include "map.h"

struct Object_08099678 {
    u8 padding00[8];
    s32 x;
    u8 padding0c[4];
    s32 y;
    u8 padding14[14];
    u8 map_layer;
};

struct Runtime_08099678 {
    u8 padding000[0x17e];
    s16 field_17e;
    u8 padding180[0x1e];
    s16 mode;
};

struct Global_08099678 {
    u8 padding000[0x1f4];
    u32 object_id;
};

extern struct Global_08099678 Data_02000240;
extern struct Runtime_08099678 *Data_03001ebc;

void Func_08099678(void)
{
    u32 runtime_slot_address;
    struct Runtime_08099678 *runtime;
    struct Object_08099678 *object;
    u8 *tile;
    s32 x;
    s32 y;

    runtime_slot_address = (u32)&Data_03001ebc;
    runtime = Data_03001ebc;
    object = Func_0808ba1c(Data_02000240.object_id);
    /* The map-state pointer slot is 19 words before the runtime pointer slot. */
    tile = (u8 *)*(struct MapState **)(runtime_slot_address - 76);

    if (runtime->mode == 3) {
        u32 tile_x;
        u32 tile_y;

        x = object->x;
        if (x < 0)
            x += 0x1fffff;
        tile_x = (x >> 21) & 31;

        y = object->y;
        if (y < 0)
            y += 0x1fffff;
        tile_y = (y >> 21) & 31;

        tile = (u8 *)(0x02020000 +
            ((tile_x + (tile_y << 5)) << 2));
    } else {
        if (object->map_layer <= 2) {
            tile = (u8 *)((struct MapState *)tile)
                ->layers[object->map_layer].cells;
        } else
            tile = (u8 *)0x02010000;

        x = object->x;
        if (x < 0)
            x += 0xfffff;
        {
            u32 tile_x = x >> 20;

            y = object->y;
            if (y < 0)
                y += 0xfffff;

            tile = (u8 *)((u32)tile +
                ((tile_x + ((u32)(y >> 20) << 7)) << 2));
        }
    }

    if (tile[2] != 0xfb)
        runtime->field_17e = 0x2092;
}
