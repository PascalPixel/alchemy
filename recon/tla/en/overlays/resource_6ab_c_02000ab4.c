/* Near miss of Func_02000ab4 (20 instructions, 2 differ): the native loads the destination byte
 * before it builds the mask 12, the draft after the mask is applied. */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct SpriteAttributes {
    u8 unknown_00[9];
    u8 unknown_09_0 : 2;
    u8 priority : 2;
};

struct ObjectRuntime *Object_GetById(s32 id);

void Func_02000ab4(struct ObjectRuntime *object)
{
    struct ObjectRuntime *owner = Object_GetById(gPartyState.current_owner);

    ((struct SpriteAttributes *)object->animation)->priority
        = ((struct SpriteAttributes *)owner->animation)->priority;
}
