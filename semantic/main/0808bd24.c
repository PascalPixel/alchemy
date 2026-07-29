#include "types.h"

struct Object_0808bd24 {
    u8 filler[6];
    u16 value;
    s32 position[3];
};

struct Runtime_0808bd24 {
    u8 filler_000[20];
    struct Object_0808bd24 *objects[98];
    u8 filler_19c[2];
    s16 mode;
};

struct MapWork_0808bd24 {
    u8 filler[304];
    u8 *tiles;
};

extern u32 Data_02000240[];
extern struct Runtime_0808bd24 *Data_03001ebc;
extern u8 Data_02020000[];

void Func_0800447c(s32, s32, s32 *);

u8 Func_0808bd24(void)
{
    u32 index = Data_02000240[125];
    struct Runtime_0808bd24 *runtime = Data_03001ebc;
    struct Object_0808bd24 *object = runtime->objects[index];
    struct MapWork_0808bd24 *map =
        *(struct MapWork_0808bd24 **)((u8 *)&Data_03001ebc - 76);
    s32 position[3];
    u8 *tiles;

    if (object == 0)
        return 0;

    position[0] = object->position[0];
    position[1] = object->position[1];
    position[2] = object->position[2];
    Func_0800447c(0x100000, object->value, position);

    if (runtime->mode == 3) {
        s32 x = position[0];
        s32 z;

        if (x < 0)
            x += 0x1fffff;
        x = (x >> 21) & 31;
        z = position[2];
        if (z < 0)
            z += 0x1fffff;
        z = (z >> 21) & 31;
        tiles = Data_02020000;
        tiles += ((z << 5) + x) * 4;
    } else {
        s32 x;
        s32 z;

        tiles = map->tiles;
        x = position[0];
        if (x < 0)
            x += 0xfffff;
        x >>= 20;
        z = position[2];
        if (z < 0)
            z += 0xfffff;
        z >>= 20;
        tiles += ((z << 7) + x) * 4;
    }
    return tiles[2];
}
