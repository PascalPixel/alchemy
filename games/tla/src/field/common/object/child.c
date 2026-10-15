#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "object_lookup.h"
#include "object_dispatch.h"

#define Object_SetPartAttribute ((void (*)(struct DispatchObject *, s32))Object_SetPartAttribute)

void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *);

void ObjectGroup_ConfigureChildValue(s32 objectId, s32 value)
{
    struct DispatchObject *object;

    object = ObjectTable_Get(objectId);
    if (object != NULL)
    {
        if (value & 0x100)
        {
            object->update = ObjectGroup_ApplyIndexedChildValue;
            return;
        }
        object->update = NULL;
        Object_SetPartAttribute(object, value);
    }
}
