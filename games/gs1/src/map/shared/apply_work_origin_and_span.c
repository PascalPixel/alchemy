#include "types.h"
#include "scene.h"
#include "abi/map/shared/apply_work_origin_and_span.h"
#include "global_cells.h"


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
    Map_Apply(first, (s32)((u32)third - (u32)second));
    Map_Check();
}
