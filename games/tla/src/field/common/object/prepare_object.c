#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"

#define ObjectDispatch_SetSingleChildFlags \
    ((void (*)(struct FieldActor *, s32))ObjectDispatch_SetSingleChildFlags)

#define Object_SetPartAttribute ((void (*)(struct FieldActor *, s32))Object_SetPartAttribute)

struct FieldActor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type)
{
    struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

    if (object != NULL)
    {
        Object_SetSpritePriority(object, 0);
        object->motionFlags = 0;
        object->collisionFlags = 8;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Object_SetPartAttribute(object, 14);
        Animation_SetStateFlags(object, 1);
        return object;
    }
    return NULL;
}
