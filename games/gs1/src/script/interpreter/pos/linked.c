#include "scene.h"
#include "abi/script/interpreter/pos/linked.h"
#include "object_commands.h"
#include "script_object_runtime.h"

s32 Script_ApplyLinkedObjectPosition(struct ScriptObjectRuntime *object)
{
    struct ScriptObjectRuntime *target;

    target = object->linked_object;
    Script_Run(object, target->x, target->y, target->z);
    object->script_cursor++;
    return 1;
}
