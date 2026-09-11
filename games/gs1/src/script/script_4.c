#include "scene.h"
#include "script_interpreter.h"
#include "script_object_entry.h"
#include "global_cells.h"
#include "object_commands.h"
#include "script_object_runtime.h"
#include "fixed_math.h"
#include "types.h"

/* script/interpreter/cmd/skip.c */
s32 Script_SkipCommand(struct ScriptInterpreter *interpreter)
{
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

/* script/interpreter/cmd/audio.c */
s32 Audio_PlayCue(s32);

s32 Script_PlayAudioCue(struct ScriptInterpreter *interpreter)
{
    Audio_PlayCue(interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

/* script/object/check_overlap.c */
s32 ScriptObject_CheckOverlap(struct ScriptObjectEntry *object, s32 *values)
{
    s32 tmp;
    s32 index;
    u8 *flags;
    struct ScriptObjectEntry *entry;

    entry = *(struct ScriptObjectEntry **)ADDR_03001E64;
    index = 0;
    flags = &entry->flags_59;
loop_1:
    if (entry->data != NULL && (1 & *flags) && entry != object) {
        tmp = index;
        if (Script_SetMode(entry->values_08, entry->value_20 - 2,
                          values, object->value_20 - 2) >= 0) {
            return -1;
        }
    }
    index += 1;
    flags += 0x70;
    entry++;
    if (index > 0x3F) {
        return 0;
    }
    goto loop_1;
}

/* script/object/find_overlapping_entry.c */
struct ScriptObjectEntry *ScriptObject_FindOverlappingEntry(
    struct ScriptObjectEntry *object, s32 *values)
{
    s32 tmp;
    s32 index;
    u8 *flags;
    struct ScriptObjectEntry *entry;

    entry = *(struct ScriptObjectEntry **)ADDR_03001E64;
    index = 0;
    flags = &entry->flags_59;
loop_1:
    if (entry->data != NULL && (1 & *flags) && entry != object) {
        tmp = index;
        if (Script_SetMode(entry->values_08, entry->value_20 - 2,
                          values, object->value_20 - 2) >= 0) {
            return entry;
        }
    }
    index += 1;
    flags += 0x70;
    entry++;
    if (index > 0x3F) {
        return NULL;
    }
    goto loop_1;
}

/* script/interpreter/pos/set_reset.c */
s32 Script_SetPositionAndResetMotion(struct ScriptInterpreter *interpreter)
{
    s32 first;
    s32 second;
    s32 third;
    const s32 *argument;

    argument = interpreter->script + interpreter->cursor;
    argument++;
    first = *argument;
    argument++;
    second = *argument;
    argument++;
    third = *argument;
    Script_SetMode(interpreter, first, second, third);
    interpreter->cursor = (u16)interpreter->cursor + 4;
    return 1;
}

/* script/interpreter/pos/absolute.c */
s32 Script_ApplyAbsolutePosition(struct ScriptInterpreter *interpreter)
{
    s32 first;
    s32 second;
    s32 third;
    const s32 *argument;

    argument = interpreter->script + interpreter->cursor;
    argument++;
    first = *argument;
    argument++;
    second = *argument;
    argument++;
    third = *argument;
    Script_Run(interpreter, first, second, third);
    interpreter->cursor = (u16)interpreter->cursor + 4;
    return 1;
}

/* script/interpreter/pos/relative.c */
s32 Script_ApplyRelativePosition(struct ScriptObjectRuntime *object)
{
    u8 *entry = (u8 *)(object->script + (s16)object->script_cursor);
    s32 *cursor = (s32 *)(entry + 4);
    s32 first = *cursor++;
    s32 second = *cursor++;
    s32 third = *cursor;

    Script_Run(object, object->x + first,
        object->y + second, object->z + third);
    object->script_cursor += 4;
    return 1;
}

/* script/interpreter/pos/store_angle.c */
s16 ArcTan2(s32, s32);

s32 Script_StoreAngleToLinkedObject(struct ScriptObjectRuntime *object)
{
    struct ScriptObjectRuntime *target;

    target = object->linked_object;
    object->script_value = ArcTan2(
        (s32)((u32)target->z - (u32)object->z),
        (s32)((u32)target->x - (u32)object->x));
    object->script_cursor++;
    return 1;
}

/* script/interpreter/pos/linked.c */
s32 Script_ApplyLinkedObjectPosition(struct ScriptObjectRuntime *object)
{
    struct ScriptObjectRuntime *target;

    target = object->linked_object;
    Script_Run(object, target->x, target->y, target->z);
    object->script_cursor++;
    return 1;
}

/* script/interpreter/pos/local.c */
s32 Script_ApplyLocalOffsetPosition(u8 *arg0)
{
    s32 offset[3];

    Script_Run(
        arg0,
        *(s32 *)(arg0 + 8) + offset[0],
        *(s32 *)(arg0 + 12) + offset[1],
        *(s32 *)(arg0 + 16) + offset[2]
    );
    *(u16 *)(arg0 + 4) += 3;
    return 1;
}
