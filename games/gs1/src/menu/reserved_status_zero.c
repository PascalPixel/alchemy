#include "types.h"
#include "scene.h"

/* menu/status/reserved_status_zero.c */
s32 Menu_ReservedStatusZero(void)
{
    return 0;
}

/* menu/core/get_modulo_of_sum.c */
s32 Modulo(s32);

s32 Menu_GetModuloOfSum(s32 arg0, s32 arg1)
{
    return Modulo(arg0 + arg1);
}

/* menu/entry/set_first_object_row_coordinates.c */
extern u8 *gIw;

s32 Menu_SetFirstObjectRowCoordinates(s32 arg0)
{
    u8 *current;
    s32 value;
    s32 count;
    current = gIw + 0x134;
    arg0 += 0x3D;
    value = 0x20;
    count = 3;
    do {
        count--;
        *(s16 *)current = value;
        *(s16 *)(current + 0x10) = arg0;
        value += 0x38;
        current += 2;
    } while (count >= 0);
    return arg0;
}
