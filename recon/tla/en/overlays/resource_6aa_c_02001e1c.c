/* Near miss: Func_02001e1c differs 4, as overlay 6ad's Func_0200215c does: the part mask is built before the
 * sprite is loaded. */
#include "../../../../games/tla/src/field/overlay_6aa/room.h"

extern const u8 Data_020056a0[];

void Func_02001ae0(void);
void Func_02001cbc(void);
void Func_02001f90(struct ObjectRuntime *object);

void Func_02001e1c(struct ObjectRuntime *object, s32 part)
{
    object->motion_flags = 0;
    object->collision_flags = 0;
    ((struct SpriteParts *)object->animation)->part = part;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02004bd0(object, Data_020056a0);
    object->action = 0;
    object->speed_limit = 0;
    object->acceleration = 0;
}

