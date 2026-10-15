/* Near miss, differ 12 of 51 (2026-10-15). The facing is masked once and shifted at
 * each call, which gives the listing's r2 and r7. Left: in both position copies the
 * listing starts the call's 0x100000 or 0x200000 between the stores and the next
 * loads; this loads and stores the three words first. */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
struct ObjectRuntime *Func_080eaf28(s32 *position, s32 check_y);

struct ObjectRuntime *Func_080eaeb4(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
    s32 position[3];
    s32 angle = ((leader->angle >> 12) + 2) & 12;
    struct ObjectRuntime *found;

    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z;
    Vector_AddPolarOffset(0x100000, angle << 12, position);
    found = Func_080eaf28(position, 1);
    if (found != NULL)
        return found;
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z;
    Vector_AddPolarOffset(0x200000, angle << 12, position);
    found = Func_080eaf28(position, 1);
    if (found != NULL)
        return found;
    return NULL;
}
