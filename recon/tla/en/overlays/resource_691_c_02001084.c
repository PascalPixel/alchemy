/* Near miss: differ 2 of 34. The sentinel's shift is scheduled before the table load; the listing loads
 * first (movs, ldrh, lsls, adds). Func_020010cc has the same remaining difference.
 * In sched2 the load and the shift tie at priority 3, and the shift has two
 * dependents to the load's one. A for loop hoists the 0xffff out (differ 8),
 * while loops are laid out differently (36), and reading the id before the
 * sentinel test costs 15. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct Ov691Work {
    s32 unknown_00;
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_10;
    struct ObjectRuntime *target;
};

struct ObjectRuntime *Object_GetById(s32 id);
s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);

s32 Func_02001084(u16 *table)
{
    struct Ov691Work *work = Ram_HeapSlots->effect_work;
    s32 result = 0;
    s32 code = MapCell_GetAttributes(0, work->x >> 20, work->z >> 20) >> 8;
    s32 id;

    goto check;
next:
    result = *table++;
check:
    if (*table == 0xffff)
        goto done;
    id = *table++;
    if (code != *table++)
        goto next;
    work->target = Object_GetById(id);
    result = *table;
done:
    return result;
}
