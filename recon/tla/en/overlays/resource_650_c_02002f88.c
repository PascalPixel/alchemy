/* Near miss, differ 2 of 32 (6 plain): the x load and the sprite pointer now
 * straddle their constants as in ⚓️ (an operand asm each, the pointer pinned to
 * r2). Left: the native loads the pooled zero between the halves of 0x4000
 * (movs r3, #128; ldr r2, =0; lsls r3, r3, #7); sched2 ranks the zero 44 against
 * 45 for the second half, and an asm can only lower a load's chain. */
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);

struct ActorSprite {
    u8 unk00[18];
    u16 rotation;
};

void Func_02002f88(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    ObjectDispatch_SetSingleChildFlags(Object_GetById(id), 0);
    object->angle = 0x4000;
    object->motionFlags = 0;
    {
        s32 x = object->x;

        asm("" : "+l"(x));
        object->x = x + 0xc0000;
    }
    object->y = 0x100000;
    object->collisionFlags = 0;
    {
        register struct ActorSprite *sprite asm("r2") = (struct ActorSprite *)object->animation;

        asm("" : "+l"(sprite));
        sprite->rotation = 0xc000;
    }
}
