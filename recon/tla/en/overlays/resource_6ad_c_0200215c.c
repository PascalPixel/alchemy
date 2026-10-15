/* Near miss: Func_0200215c differs 4 (one reordered pair): the listing builds the part mask before it loads the
 * sprite. Overlay 6aa's Func_02001e1c is the same function. */
#include "../../../../games/tla/src/scene/mars_room_f/room.h"

#define ABS(value) ((value) < 0 ? -(value) : (value))

struct SpriteParts {
    u8 unk00[9];
    u8 unk09Low : 2;
    u8 part : 2;
    u8 unk0a[8];
    u16 unk12;
};

extern const u8 gData_02005d8c[];

void Func_02001e20(void);
void Func_02001ffc(void);
void Func_020022d0(struct ObjectRuntime *object);

void Func_0200215c(struct ObjectRuntime *object, s32 part)
{
    object->motionFlags = 0;
    object->collisionFlags = 0;
    ((struct SpriteParts *)object->animation)->part = part;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02005490(object, gData_02005d8c);
    object->action = 0;
    object->speedLimit = 0;
    object->acceleration = 0;
}

