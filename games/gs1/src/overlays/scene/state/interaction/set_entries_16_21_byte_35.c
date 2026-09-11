#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/state/interaction/set_entries_16_21_byte_35.h"

extern u8 *State_Run(s32);

void State_SetEntries16To21Byte35(void)
{
    s32 index = 16;
    s32 flag = 1;
    s32 remaining = 5;

    do {
        u8 *entry = State_Run(index);

        remaining--;
        entry[35] = flag;
        index++;
    } while (remaining >= 0);
}
