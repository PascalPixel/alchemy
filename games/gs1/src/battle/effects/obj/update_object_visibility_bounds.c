#include "object_lookup.h"
#include "types.h"

s32 Object_GetById(u32);

extern s32 Data_02000240[];
extern u8 Data_03001ebc[];

void BattleEffect_UpdateObjectVisibilityBounds(void)
{
    s32 object;
    s32 x;
    s32 y;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    u32 id;

    object = Data_02000240[125];
    Object_GetById(object);
    object = *(s32 *)(*(u8 **)Data_03001ebc + 480);
    x = *(s32 *)(object + 8);
    left = x + 0xFEC00000;
    right = x + 0x01400000;
    y = *(s32 *)(object + 16);
    top = y + 0xFDA80000;
    bottom = y + 0x01900000;

    id = 8;
    do {
        object = ObjectTable_Get(id);

        if (object != 0) {
            s32 ox = *(s32 *)(object + 8);
            s32 oy = *(s32 *)(object + 16);

            if (ox < left || ox > right ||
                oy < top || oy > bottom) {
                *(u8 *)(object + 84) = 0;
            } else {
                *(u8 *)(object + 84) = 1;
            }
        }
        id++;
    } while (id <= 65);
}
