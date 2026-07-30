#include "types.h"

struct MapPosition_0808cf78 {
    s16 map_id;
    s16 condition;
    s16 x;
    s16 y;
    s16 z;
    u16 direction;
    s16 unused_0c;
    s16 camera_x;
    s16 camera_y;
    s16 camera_z;
    s16 camera_w;
    s16 unused_16;
};

struct MapState_0808cf78 {
    u8 padding_00[0xec];
    s32 camera_x;
    s32 camera_y;
    s32 camera_z;
    s32 camera_w;
};

struct MapPosition_0808cf78 *Func_080072e4(const void *table);
s32 Func_080770c0(s32 flag);

void Func_0808cf78(void)
{
    struct MapState_0808cf78 *state =
        *(struct MapState_0808cf78 **)0x03001e70;
    s16 map_id = *(s16 *)0x02000402;
    const void *table = *(const void **)0x0200800c;
    struct MapPosition_0808cf78 *entry = Func_080072e4(table);
    s32 found = 0;

    while (entry->map_id != -1) {
        if (entry->map_id == map_id &&
            (entry->condition == -1 ||
             Func_080770c0(entry->condition) != 0)) {
            found = 1;
            break;
        }
        entry++;
    }
    if (!found)
        entry = Func_080072e4(table);

    if (Func_080770c0(0x109) == 0) {
        *(s32 *)0x0200041c = entry->x << 16;
        *(s32 *)0x02000420 = entry->y << 16;
        *(s32 *)0x02000424 = entry->z << 16;
        *(u32 *)0x02000428 = entry->direction;
        *(u16 *)0x0200042c = 0;
    }

    if (entry->camera_x != -1)
        state->camera_x = entry->camera_x << 16;
    if (entry->camera_y != -1)
        state->camera_y = entry->camera_y << 16;
    if (entry->camera_z != -1)
        state->camera_z = entry->camera_z << 16;
    if (entry->camera_w != -1)
        state->camera_w = entry->camera_w << 16;

    if (state->camera_x + 0x00f00000 > state->camera_z)
        state->camera_x = state->camera_z - 0x00f00000;
    if (state->camera_y + 0x00a00000 > state->camera_w)
        state->camera_y = state->camera_w - 0x00a00000;
}
