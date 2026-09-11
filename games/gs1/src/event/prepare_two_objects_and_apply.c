#include "types.h"
#include "scene.h"
#include "abi/event/prepare_two_objects_and_apply.h"

void Event_PrepareTwoObjectsAndApply(void *first, void *second)
{
    Sys_Check(first);
    Sys_Check(second);
    Sys_Apply(first, second);
}
