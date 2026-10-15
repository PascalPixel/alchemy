#include "types.h"
#include "field_obj.h"

static void SceneActor_WaitObjectBelowHeight(struct FieldActor *object, s32 height)
{
    s32 frames = 60;

    while (frames != 0)
    {
        Engine_TaskWait(1);
        frames--;
        if (object->y.fixed <= height)
            break;
    }
}
