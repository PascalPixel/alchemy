#include "types.h"
#include "scene.h"

/* ability/play_use_animation.c */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Ability_GetData();

void Ability_PlayUseAnimation(void)
{
    s32 animation_type;
    u32 target_type;
    void *ability;

    ability = Ability_GetData();
    animation_type = 0xf & FIELD(ability, u8 *, 1);
    switch (animation_type) {
    case 1:
        Sys_Do(0x7e);

    case 11:
        Sys_Do(0x7e);
        return;
    default:
        target_type = FIELD(ability, u8 *, 3) - 1;
        switch (target_type) {
        case 4:
            Sys_Do(0x52);
            return;
        case 2:
            Sys_Do(0x54);
            return;
        default:
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
            Sys_Do(0x5b);

        case 0:
        case 1:
        case 30:
        case 31:
            return;
        }
        break;
    }
}

/* menu/reserved_status_zero.c */
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
