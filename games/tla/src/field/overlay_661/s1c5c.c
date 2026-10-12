#include "scene.h"

void Func_02001c5c(void)
{
    struct FieldActor *object;
    s16 scene;

    ((struct SceneEventWork *)Ram_HeapSlots->event_work)->unknown_16a = Data_02004808;
    if (Data_0200480c != 0) {
        scene = gPartyState.scene;
        if (scene == (s32)&SceneId_Ov661Room4)
            object = Object_GetById(16);
        else if (scene == (s32)&SceneId_Ov661Room5)
            object = Object_GetById(21);
        else
            return;
        Data_0200480c->x = object->x;
        Data_0200480c->z = object->z;
        Data_0200480c->facing = object->facing;
    }
}
