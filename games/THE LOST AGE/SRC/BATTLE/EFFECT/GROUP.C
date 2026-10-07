#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "OBJECT_RUNTIME.H"

#define ObjectDispatch_ApplyValueToChildrenFar ((void (*)(struct ObjectRuntime*,s32))ObjectDispatch_ApplyValueToChildrenFar)


void ObjectGroup_SetActionForOthers(struct ObjectRuntime *excluded_object,
                                  s32 group_mode, s32 action)
{
    s16 *active_object_id;
    struct ObjectRuntime *object;
    s32 object_id;

    object_id = 0;
    active_object_id = (s16 *)gPlayerObjectId;
    do {
        object = ObjectTable_Get(object_id);
        if (object_id != *active_object_id && object != NULL && object != excluded_object) {
            object->movement_state = group_mode;
            ObjectDispatch_ApplyValueToChildrenFar(object, action);
        }
        object_id++;
    } while (object_id <= 80);
}
