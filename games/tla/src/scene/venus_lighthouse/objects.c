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

#define ObjectDispatch_SetSingleChildFlags ((void (*)(struct FieldActor*,s32))ObjectDispatch_SetSingleChildFlags)

#define Object_SetPartAttribute ((void (*)(struct FieldActor*,s32))Object_SetPartAttribute)


void ConfigureOverlayObject(struct FieldActor *object, s32 parameter)
{
    object->motionFlags = 0;
    object->collisionFlags = 8;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Object_SetPartAttribute(object, parameter);
}


struct FieldActor *OverlayObject_SpawnWithMode14(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *object = Engine_ObjectCreate(kind, x, y, z);

    if (object != 0) {
        Object_SetSpritePriority(object, 0);
        ConfigureOverlayObject(object, 14);
        Animation_SetStateFlags(object, 1);
        return object;
    }
    return 0;
}

struct FieldActor *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *object = Engine_ObjectCreate(kind, x, y, z);

    if (object != 0) {
        Object_SetSpritePriority(object, 1);
        ConfigureOverlayObject(object, 15);
        object->priorityFlags |= 2;
        return object;
    }
    return 0;
}
