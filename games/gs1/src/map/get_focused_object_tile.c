#include "types.h"
#include "map.h"

struct Object_0808bd24 {
    u8 padding00[6];
    u16 kind;
    s32 x;
    s32 unknown_0c;
    s32 y;
    u8 padding14[14];
    u8 terrain_id;
};

struct Runtime_0808bd24 {
    u8 padding000[0x19e];
    s16 mode;
};

struct Global_0808bd24 {
    u8 padding000[0x1f4];
    u32 object_id;
};

struct Position_0808bd24 {
    s32 x;
    s32 unknown_04;
    s32 y;
};

void RotateVectorByMagnitude(s32, u32, struct Position_0808bd24 *);
s32 Func_080091a8(s32, s32, s32);
s32 Func_0808d48c(s32, s32);
void *ObjectTable_Get(u32);

extern struct Global_0808bd24 Data_02000240;
extern struct Runtime_0808bd24 *Data_03001ebc;

s32 Func_0808ce74(void)
{
    u32 runtime_slot_address;
    struct Runtime_0808bd24 *runtime;
    struct Object_0808bd24 *object;
    struct MapState *map;
    struct Position_0808bd24 position;
    u8 *tile;
    s32 x;
    s32 y;
    s32 result;
    u8 tile_value;

    result = 0;
    object = ObjectTable_Get(Data_02000240.object_id);
    runtime_slot_address = (u32)&Data_03001ebc;
    runtime = Data_03001ebc;
    map = *(struct MapState **)(runtime_slot_address - 76);

    if (object != 0) {
        position.x = object->x;
        position.unknown_04 = object->unknown_0c;
        position.y = object->y;
        RotateVectorByMagnitude(0x100000, object->kind, &position);

        if (runtime->mode == 3) {
            u32 tile_x;
            u32 tile_y;

            x = position.x;
            if (x < 0)
                x += 0x1fffff;
            tile_x = (x >> 21) & 31;

            y = position.y;
            if (y < 0)
                y += 0x1fffff;
            tile_y = (y >> 21) & 31;

            tile = (u8 *)(0x02020000 + ((tile_x + (tile_y << 5)) << 2));
        } else {
            tile = (u8 *)map->layers[0].cells;

            x = position.x;
            if (x < 0)
                x += 0xfffff;
            {
                u32 tile_x = x >> 20;

                y = position.y;
                if (y < 0)
                    y += 0xfffff;

                tile = (u8 *)((u32)tile +
                    ((tile_x + ((u32)(y >> 20) << 7)) << 2));
            }
        }

        tile_value = tile[2];
        if ((u32)(tile_value - 242) <= 5) {
            s32 height;
            s32 base;

            height = Func_080091a8(object->terrain_id, position.x, position.y);
            base = object->unknown_0c;
            if (height >= base && height <= base + 0x400000)
                result = tile_value;
        } else if (Func_0808d48c(3, tile_value) != 0) {
            result = tile_value;
        }
    }

    return result;
}
