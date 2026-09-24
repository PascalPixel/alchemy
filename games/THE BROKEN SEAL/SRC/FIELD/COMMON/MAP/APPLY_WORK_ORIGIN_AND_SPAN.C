#include "TYPES.H"
#include "GLOBAL_CELLS.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Map_UpdateLayerScroll();
s32 Map_SetCameraCenter(s32, s32);

void Map_ApplyWorkOriginAndSpan(void)
{
    s32 first;
    s32 third;
    s32 second;
    s32 *p;

    p = **(s32 ***)ADDR_03001E70;
    first = 0;
    second = 0;
    third = 0;
    if (p != NULL) {
        first = *p++;
        second = *p++;
        third = *p;
    }
    Map_SetCameraCenter(first, (s32)((u32)third - (u32)second));
    Map_UpdateLayerScroll();
}
