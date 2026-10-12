/* Near miss of Func_02001928 (30 instructions, 4 differ): the native builds the mask 3 before it
 * loads the sprite attributes and masks the priority before it loads their byte; the draft
 * loads both first. */
#include "types.h"
#include "object_runtime.h"
#include "call.h"

struct SpriteAttributes {
    u8 unknown_00[9];
    u8 unknown_09_0 : 2;
    u8 priority : 2;
};

extern const u8 Data_02004744[];

void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Func_020041cc(struct ObjectRuntime *object, s32 mode);
void Func_020041dc(struct ObjectRuntime *object, const void *script);
struct ObjectRuntime *Func_020041e4();
void Func_020015ec(void);

void Func_02001928(struct ObjectRuntime *object, s32 priority)
{
    object->motion_flags = 0;
    object->collision_flags = 0;
    ((struct SpriteAttributes *)object->animation)->priority = priority;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_020041dc(object, Data_02004744);
    object->action = 0;
    object->speed_limit = 0;
    object->acceleration = 0;
}
