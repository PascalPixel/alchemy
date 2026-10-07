#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "MAP.H"
#include "OBJECT_RUNTIME.H"

void WaitFrames(s32);

void Object_AttachWorkTargetToObject(s32 id, s32 flag)
{
    struct ObjectRuntime *obj;
    struct ObjectRuntime *target;
    struct EventWork *work;
    struct MapWork *map;

    obj = ObjectTable_Get(id);
    work = Ram_HeapSlots->event_work;
    target = work->view_center;
    map = Ram_HeapSlots->map_work;
    if (obj != 0) {
        map->origin = &target->x;
        ObjectDispatch_InitFromTable4WithArgumentFar(target, obj);
        if (flag == 0) {
            target->x = obj->x;
            target->y = obj->y;
            target->z = obj->z;
            WaitFrames(1);
            if (work->control_mode != 3)
                Map_ApplyWorkOriginAndSpanFar();
        }
    }
}

void ObjectTable_AllocateAndSetObjectSpeed(s32 first, s32 second)
{
    struct EventWork *work = Runtime_AllocateBlock(108, 0xd50);

    work->view_center->speed_limit = first;
    work->view_center->acceleration = second;
}
