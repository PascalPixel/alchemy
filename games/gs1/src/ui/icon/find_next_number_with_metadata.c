#include "metadata_lookup.h"
#include "types.h"
#include "scene.h"
#include "abi/ui/icon/find_next_number_with_metadata.h"

s32 Ui_FindNextNumberWithMetadata(s32 start, s32 step)
{
    s32 value = start;
    s32 delta = step + value - value;

    for (;;) {
        value += delta;
        if (value < 0) {
            value = 0x200;
            continue;
        }
        {
            s32 limit = 0x200;
            if (value >= limit) {
                value = -1;
                continue;
            }
        }
        if (*(u8 *)Ui_Run(value)) {
            return value;
        }
    }
}
