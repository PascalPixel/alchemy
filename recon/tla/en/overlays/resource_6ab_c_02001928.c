/* Near miss of Func_02001928 (30 instructions, 4 differ): the native builds the mask 3 before it
 * loads the sprite attributes and masks the priority before it loads their byte; the draft
 * loads both first. Also measured: the priority masked first (12), the byte
 * written by hand (12, 22), a one-pass block before or after the sprite's
 * load (4), five minutes of alchemy permute (4). Its copy group is led by
 * 6AA's Func_02001e1c in another lane. */
#include "types.h"
#include "object_runtime.h"
#include "call.h"

struct SpriteAttributes {
    u8 unk00[9];
    u8 unk09_0 : 2;
    u8 priority : 2;
};

extern const u8 gData_02004744[];

void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Func_020041cc(struct ObjectRuntime *object, s32 mode);
void Func_020041dc(struct ObjectRuntime *object, const void *script);
struct ObjectRuntime *Func_020041e4();
void Func_020015ec(void);

void Func_02001928(struct ObjectRuntime *object, s32 priority)
{
    object->motionFlags = 0;
    object->collisionFlags = 0;
    ((struct SpriteAttributes *)object->animation)->priority = priority;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_020041dc(object, gData_02004744);
    object->action = 0;
    object->speedLimit = 0;
    object->acceleration = 0;
}
