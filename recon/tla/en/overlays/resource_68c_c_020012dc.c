/* Near miss, differ 6: the listing schedules the hoisted constants 0x80000
 * and 0x1c0000 after the first position loads; this draft builds both before. */
#include "types.h"
#include "field_event.h"

void Func_0200128c();
struct FieldActor *Object_GetById(s32 id);
void Func_02002924(s32 cue);
void Battle_WaitMode0(s32 frames);

struct SpriteFrames {
    u8 unknown_00[5];
    u8 frame;
};

struct AnimatedSprite {
    u8 unknown_00[0x12];
    s16 rotation;
    u8 unknown_14[5];
    u8 frame;
    u8 next_frame;
    u8 unknown_1b[0xd];
    struct SpriteFrames *frames;
};

void Func_020012dc(s32 id)
{
    struct FieldActor *object = Object_GetById(id);

    Func_0200128c(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000, 15);
    Func_02002924(151);
    Battle_WaitMode0(15);
    Func_0200128c(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000, 15);
    Func_02002924(151);
    Battle_WaitMode0(15);
    Func_0200128c(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000, 15);
    Func_02002924(151);
    Battle_WaitMode0(15);
    Func_0200128c(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000, 15);
    Func_02002924(151);
    Battle_WaitMode0(15);
}
