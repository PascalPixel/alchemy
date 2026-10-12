/* Near miss, differ 2: the listing copies the index's zero from the result's register (adds r1, r5, #0); this
 * draft builds it (movs r1, #0). Overlay 680's Func_020005f0 and Func_02000630 open with the same search. */
#include "worka0.h"

struct WorkA4Slot *Func_080dae70(s32 id)
{
    struct WorkA4 *work = WORK_A4;
    struct WorkA4Slot *found = NULL;
    s32 index;

    for (index = 0; index <= 7; index++) {
        if (work->slots[index].id == id) {
            found = &work->slots[index];
            break;
        }
    }
    return found;
}
