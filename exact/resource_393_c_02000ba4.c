#include "resource_393.h"

#define GetResource393Object Func_02001a38
#define ApplyResource393Position Func_0200168a
#define Resource393SharedWork Data_02000240
#define SubmitResource393ObjectPosition Func_02000ba4

void SubmitResource393ObjectPosition(void)
{
    struct Resource393Position position;
    struct Resource393Object *object = GetResource393Object(Resource393SharedWork.object_id);
    u32 x_base = object->position_x & 0xfff00000;

    position.x = x_base + 0x80000;
    position.y = object->position_y;
    position.z = (object->position_z & 0xfff00000) + 0x80000;
    position.x = x_base + 0x280000;
    ApplyResource393Position(&position);
}
