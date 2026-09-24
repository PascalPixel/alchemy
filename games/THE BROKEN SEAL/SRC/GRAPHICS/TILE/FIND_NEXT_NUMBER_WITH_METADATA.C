#include "METADATA_LOOKUP.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

/* ui/icon/find_next_number_with_metadata.c */
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
        if (*(u8 *)Resource_GetMetadataRecordFar(value)) {
            return value;
        }
    }
}
