#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "OBJECT_LOOKUP.H"
#include "OBJECT_DISPATCH.H"

#define Object_SetPartAttribute ((void (*)(struct DispatchObject*,s32))Object_SetPartAttribute)

void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *);

void ObjectGroup_ConfigureChildValue(s32 object_id, s32 value)
{
    struct DispatchObject *object;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        if (value & 0x100) {
            object->update = ObjectGroup_ApplyIndexedChildValue;
            return;
        }
        object->update = NULL;
        Object_SetPartAttribute(object, value);
    }
}
