#include "types.h"

extern u8 *Data_03001e70;

void *Func_0808ba1c(s32 object_id);
void *Func_08185000(s16 object_id);

s32 Func_08094154(s32 object_id, s32 *distance)
{
    u8 *object = Func_0808ba1c(object_id);
    u8 *state;
    s32 mask;
    s32 camera_x;
    s32 camera_z;
    s32 x;
    s32 z;

    if (object == 0) {
        return -1;
    }
    state = Data_03001e70;
    state += 228;
    mask = 0xffff0000;
    camera_x = *(s32 *)(state + 0) & mask;
    camera_z = *(s32 *)(state + 4) & mask;
    z = *(s32 *)(object + 16) - camera_z;
    x = *(s32 *)(object + 8) - camera_x;
    z -= *(s32 *)(object + 12);
    if (x < 0) {
        x += 0xffff;
    }
    *distance = x >> 16;
    distance++;
    if (z < 0) {
        z += 0xffff;
    }
    *distance = z >> 16;
    if ((*(u8 *)(object + 84) & 15) == 1) {
        u8 *table = *(u8 **)(object + 80);
        u8 *entry = *(u8 **)(table + 40);
        u8 *record = Func_08185000(*(s16 *)entry);
        *distance -= *(s8 *)(record + 8);
    }
    return 0;
}
