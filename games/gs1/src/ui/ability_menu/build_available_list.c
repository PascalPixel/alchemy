#include "types.h"
#include "scene.h"
#include "abi/ui/ability_menu/build_available_list.h"

s32 Ability_GetMaximum(s32, s32);

extern void *gIw;

s32 AbilityMenu_BuildAvailableList(void)
{
    u8 *state;
    s16 *output;
    s32 index;
    s32 count;
    s32 offset;
    s8 mode;

    state = gIw;
    count = 0;
    index = 0;
    output = (s16 *)(state + 0x26c);
    do {
        mode = *(s8 *)(state + 0x3a9);
        if (mode == Ui_Check(index)&&
            Ability_GetMaximum(index, 0) != 0) {
            *output = index;
            count++;
            output++;
        }
        index++;
    } while (index <= 0x1ff);
    offset = count << 1;
    offset += 0x26c;
    *(u16 *)(state + offset) = 0;
    *(u8 *)(state + 0x3a6) = count;
    return count;
}
