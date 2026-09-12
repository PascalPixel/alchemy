#include "object_lookup.h"
#include "types.h"
#include "scene.h"
#include "map.h"

/* map/shared/events/check_object_tile.c */
struct MapObject {
    u8 padding00[8];
    s32 x;
    u8 padding0c[4];
    s32 y;
    u8 padding14[14];
    u8 map_layer;
};

struct MapEventRuntime {
    u8 padding000[0x17e];
    s16 event_code;
    u8 padding180[0x1e];
    s16 mode;
};

struct Global_08099738 {
    u8 pad_000[0x1F4];
    u32 object_id;
    u8 pad_1f8[0x54];
    u16 field_24c;
};

extern struct Global_08099738 gCell;
extern struct MapEventRuntime *gWork;

void CheckObjectMapTile(void)
{
    u32 runtime_slot_address;
    struct MapEventRuntime *runtime;
    struct MapObject *object;
    u8 *tile;
    s32 x;
    s32 y;

    runtime_slot_address = (u32)&gWork;
    runtime = gWork;
    object = ObjectTable_Get(gCell.object_id);
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
        runtime->event_code = 0x2092;
}
