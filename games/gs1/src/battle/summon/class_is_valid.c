#include "types.h"
#include "scene.h"
#include "abi/battle/summon/class_is_valid.h"
#include "global_cells.h"

struct Layout {
    u8 pad[4];
    s16 field[6];
};

s32 Summon_ClassValid(s32 arg0)
{
    struct Layout *ptr;
    s32 retval;
    s32 i;

    retval = Battle_Check(arg0);
    ptr = *(struct Layout **)ADDR_03001E74;
    for (i = 0; i <= 5; i++) {
        if (ptr->field[i] != 0)
            continue;
        if (retval != 0)
            break;
        if (i <= 4 && ptr->field[i + 1] == 0)
            break;
    }
    return i != 6;
}
