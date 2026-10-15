/* Near miss: Func_0200215c differs 4 (one reordered pair): the listing builds the part mask before it loads the
 * sprite. Overlay 6aa's Func_02001e1c is the same function. */
#include "../../../../games/tla/src/scene/mars_room_f/room.h"

#define ABS(value) ((value) < 0 ? -(value) : (value))

struct SpriteParts {
    u8 unknown_00[9];
    u8 unknown_09_low : 2;
    u8 part : 2;
    u8 unknown_0a[8];
    u16 unknown_12;
};

extern const u8 Data_02005d8c[];

void Func_02001e20(void);
void Func_02001ffc(void);
void Func_020022d0(struct ObjectRuntime *object);

void Func_0200215c(struct ObjectRuntime *object, s32 part)
{
    object->motion_flags = 0;
    object->collision_flags = 0;
    ((struct SpriteParts *)object->animation)->part = part;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02005490(object, Data_02005d8c);
    object->action = 0;
    object->speed_limit = 0;
    object->acceleration = 0;
}

