/* Draft, differ 6 of 59 (three reordered pairs): the native copies the object
 * before the scaleY store, builds the party state offset after its pool load
 * and loads its own sprite byte before the mask 12. Tried: the sprite read
 * earlier or inline, a u16 action, the leader as a local (12), a minute of
 * alchemy permute (4, with a one-pass block). In 655 661 67f 68f 691 692 too. */
#include "../../../../games/tla/include/types.h"
#include "../../../../games/tla/include/object_runtime.h"
#include "../../../../games/tla/include/party_state.h"
#include "../../../../games/tla/include/field_sprite.h"

struct ObjectRuntime *Object_GetById(u32 id);

void Func_02000774(struct ObjectRuntime *object)
{
    s32 dx = object->bounce;
    s32 dy = object->gravity;
    s32 dz = object->unk4c;
    struct FieldSprite *sprite;

    object->x += dx;
    object->y += dy;
    object->z += dz;
    object->bounce = dx - dx / 18;
    object->unk4c = dz - dz / 16;
    object->scaleX += object->speedLimit;
    object->scaleY += object->acceleration;
    sprite = object->animation;
    sprite->rotation += object->action;
    ((struct FieldSprite *)object->animation)->priority =
        ((struct FieldSprite *)Object_GetById(gPartyState.currentOwner)->animation)->priority;
}
