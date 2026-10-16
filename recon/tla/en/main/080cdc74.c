/* Near miss: differ 2 of 122 (one reordered row). The native starts the 0x40000 before it loads the
 * owner height (str r3, [r2]; movs r2, #128; ldr r3, [r0, #12]; lsls r2, r2, #11); the draft loads first.
 * Tried: the sum in either order, the constant in a variable added to, three minutes of the permuter.
 * The quarter turn needs one variable assigned twice. Func_080cdd80 after it opens the same way. */
#include "types.h"
#include "party_state.h"
#include "object_runtime.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

struct ObjectRuntime *Object_GetById(s32 id);
void Vector_AddPolarOffset(s32 length, s32 angle, s32 *position);
struct ObjectRuntime *Func_080eaf28(s32 *position, s32 mode);
s32 Func_08020210(struct ObjectRuntime *object, s32 *position);

s32 Func_080cdc74(void)
{
    s32 position[3];
    s32 above[3];
    struct ObjectRuntime *owner = Object_GetById(gPartyState.currentOwner);
    struct ObjectRuntime *target;
    struct ObjectRuntime *hit;
    s32 id;
    s32 terrain;
    s32 result;
    s32 angle = owner->angle >> 12;

    angle = ((angle + 2) & 12) << 12;
    position[0] = owner->x;
    position[1] = (owner->y + 0x40000) & 0xfff80000;
    position[2] = owner->z;
    Vector_AddPolarOffset(0x100000, angle, position);
    target = Func_080eaf28(position, 1);
    if (target == 0)
        return 0;
    if (BYTE_AT(target, 98) != 0)
        return 0;
    id = gPartyState.pausedObject;
    if (id != 1 && target == Object_GetById(id))
        return 0;
    position[0] = target->x;
    position[1] = target->y;
    position[2] = target->z;
    Vector_AddPolarOffset(0x100000, angle, position);
    hit = Func_080eaf28(position, 1);
    if (hit != 0 && (hit->collisionFlags & 1))
        return 0;
    above[0] = target->x;
    above[1] = target->y + 0x100000;
    above[2] = target->z;
    hit = Func_080eaf28(above, 1);
    if (hit != 0 && (hit->collisionFlags & 1))
        return 0;
    terrain = target->terrainId;
    target->terrainId = 2;
    result = Func_08020210(target, position);
    target->terrainId = terrain;
    if (result > 0)
        return 0;
    return 1;
}
