#include "OBJECT_RUNTIME.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "SCRIPT.H"
#include "SCRIPT_OBJECT_ENTRY.H"
#include "GLOBAL_CELLS.H"
#include "OBJDISP.H"
#include "SCRIPT_OBJECT_RUNTIME.H"

s32 FixedSqrt(s32 value);

/* Object_UpdateAll is ARM code that runs from a heap copy of itself. */
extern u8 Object_UpdateAll[];
extern u8 Object_UpdateAllCodeSize[];

extern u8 gObjectSlots[];
#define TARGET_UNSET ((s32)0x80000000)

struct MotionObject {
    s32 active;          /* 0x00 */
    u16 unk_04;
    u16 angle;           /* 0x06 */
    s32 x;               /* 0x08 */
    s32 y;               /* 0x0c */
    s32 z;               /* 0x10 */
    s32 floor;           /* 0x14 */
    u8 unk_18[0x0c];
    s32 vx;              /* 0x24 */
    s32 vy;              /* 0x28 */
    s32 vz;              /* 0x2c */
    s32 speed_limit;     /* 0x30 */
    s32 acceleration;    /* 0x34 */
    s32 target_x;        /* 0x38 */
    s32 target_y;        /* 0x3c */
    s32 target_z;        /* 0x40 */
    s32 bounce;          /* 0x44 */
    s32 gravity;         /* 0x48 */
    u8 unk_4c[0x09];
    u8 flags;            /* 0x55 */
    u8 arrive_axis;      /* 0x56 */
    u8 unk_57;
    u8 snap;             /* 0x58 */
    u8 unk_59;
    u8 turn;             /* 0x5a */
    u8 unk_5b[0x06];
    u8 frozen;           /* 0x61 */
    u8 unk_62[0x0e];
};

s32 ArcTan2(s32, s32);

s32 GameFlag_TestFar(s32);
s32 GameFlag_SetBitFar(s32);
void GameFlag_ClearBitFar(s32);
void ObjectDispatch_ApplyArgumentToChildren(void *, s32);
s32 Audio_PlayCue(s32);
s32 Object_IsTargetUnset(void *);
extern const s32 Script_MainScript[];
s32 Runtime_CheckRadiusOverlap(s32 *a, s32 arg1, s32 *b, s32 arg3);
void Object_SetPositionAndResetMotion(struct ObjectRuntime *, s32, s32, s32);

void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);

/*
 * Aims an object at a point. A point closer than one unit is taken at once;
 * otherwise, unless the object moves without easing, the target is pulled
 * in to where the object must start braking at its speed and acceleration.
 * The dominant axis of the move is kept at +0x56 (16 x, 17 y, 18 z).
 */
void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z)
{
    s32 dx;
    s32 dist;
    s32 dz;
    s32 total;
    u8 *axis;

    dx = (x - object->x) / 0x10000;
    dist = (y - object->y) / 0x10000;
    dz = (z - object->z) / 0x10000;
    total = dz * dz;
    dist = Iwram_Sqrt(dx * dx + dist * dist + total) << 16;
    if (dist < 0x100000) {
        dx = x - object->x;
        dist = y - object->y;
        dz = z - object->z;
        dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dist, dist) + Iwram_MulQ16(dz, dz));
    }
    if (dist < 0x10000) {
        object->x = x;
        object->y = y;
        object->z = z;
        object->target_x = 0x80000000;
        object->target_y = 0x80000000;
        object->target_z = 0x80000000;
        return;
    }
    if (object->unknown_56[2] == 0) {
        s32 brake;

        brake = Iwram_RatioMulQ14(object->acceleration,
                             Iwram_MulQ16(object->speed_limit, object->speed_limit));
        if (dist > brake)
            brake = dist - brake / 2;
        else
            brake = dist / 2;
        dist = Iwram_RatioMulQ14(dist, brake);
        x = object->x + Iwram_MulQ16(x - object->x, dist);
        y = object->y + Iwram_MulQ16(y - object->y, dist);
        z = object->z + Iwram_MulQ16(z - object->z, dist);
    }
    object->target_x = x;
    object->target_y = y;
    object->target_z = z;
    dx = x - object->x;
    dist = y - object->y;
    dz = z - object->z;
    axis = &object->unknown_56[0];
    *axis = 16;
    if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
        *axis = 18;
        dx = dz;
    }
    if (object->flags == 0) {
        if (dx < 0)
            dx = -dx;
        if (dx < (dist < 0 ? -dist : dist))
            *axis = 17;
    }
}

void Object_RunUpdateAllFromHeap(void)
{
    void (*routine)(void);

    routine = (void (*)(void))Runtime_BumpAllocate((s32)Object_UpdateAllCodeSize);
    Dma_Set(Object_UpdateAll, routine, 0x84000000 | ((u32)Object_UpdateAllCodeSize >> 2),
        (volatile u32 *)0x040000d4);
    routine();
    Sys_Free(routine);
}

/* Moves every active object by its velocity each frame: position, then
   gravity and the bounce off the ground, through the IWRAM Q16 multiply. */

/* Moves each of the fourteen entries of the object table toward its target,
   applies gravity with bounce, notices when an axis target was passed and
   turns the entry to face its motion. */
void Object_UpdateAllMotion(void)
{
    struct MotionObject *obj;
    s32 cnt;
    s32 y;
    s32 arrived;
    s32 x;
    s32 z;
    s32 dx;
    s32 dz;
    s32 vx;
    s32 dist;
    s32 ratio;
    s32 vy;
    s32 turn;

    obj = *(struct MotionObject **)gObjectSlots;
    for (cnt = 13; cnt >= 0; cnt--, obj++) {
        if (obj->active == 0)
            continue;
        x = obj->x;
        y = obj->y;
        z = obj->z;
        if (obj->frozen == 0) {
            arrived = 0;
            if (obj->target_x != TARGET_UNSET) {
                dx = (obj->target_x - x) / 65536;
                dz = (obj->target_z - z) / 65536;
                dist = Iwram_Sqrt(dx * dx + dz * dz) << 16;
                if (dist <= 0xffffff) {
                    dx = obj->target_x - x;
                    dz = obj->target_z - z;
                    dist = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz)) << 8;
                }
                if (dist == 0) {
                    x = obj->target_x;
                    z = obj->target_z;
                } else {
                    ratio = Iwram_RatioMulQ14(dist, obj->acceleration);
                    vx = obj->vx + Iwram_MulQ16(dx, ratio);
                    obj->vx = vx;
                    dx = obj->vz + Iwram_MulQ16(dz, ratio);
                    obj->vz = dx;
                    dist = Iwram_Sqrt(Iwram_MulQ16(vx, vx) + Iwram_MulQ16(dx, dx)) << 8;
                    if (dist > obj->speed_limit) {
                        ratio = Iwram_RatioMulQ14(dist, obj->speed_limit);
                        obj->vx = Iwram_MulQ16(vx, ratio);
                        obj->vz = Iwram_MulQ16(dx, ratio);
                    }
                }
            } else {
                dx = obj->vx;
                dz = obj->vz;
                if ((dx | dz) != 0) {
                    dist = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz)) << 8;
                    if (dist != 0) {
                        if (dist - obj->acceleration < 0) {
                            obj->vx = 0;
                            obj->vz = 0;
                        } else {
                            ratio = Iwram_RatioMulQ14(dist, dist - obj->acceleration);
                            obj->vx = Iwram_MulQ16(dx, ratio);
                            obj->vz = Iwram_MulQ16(dz, ratio);
                        }
                    } else {
                        obj->vx = 0;
                        obj->vz = 0;
                    }
                }
            }
            if (obj->flags & 2) {
                if (y > obj->floor) {
                    obj->vy -= obj->gravity;
                } else if (obj->vy < 0) {
                    y = obj->floor;
                    obj->vy = -Iwram_MulQ16(obj->vy, obj->bounce);
                    if ((obj->vy < 0 ? -obj->vy : obj->vy) <= obj->gravity)
                        obj->vy = 0;
                }
            }
        }
        x += obj->vx;
        y += obj->vy;
        z += obj->vz;
        if (obj->arrive_axis != 0) switch (obj->arrive_axis) {
        case 16:
            if (x == obj->target_x || ((obj->x - obj->target_x) ^ (x - obj->target_x)) < 0)
                arrived = 1;
            break;
        case 17:
            if (y == obj->target_y || ((obj->y - obj->target_y) ^ (y - obj->target_y)) < 0)
                arrived = 1;
            break;
        case 18:
            if (z == obj->target_z || ((obj->z - obj->target_z) ^ (z - obj->target_z)) < 0)
                arrived = 1;
            break;
        }
        if (arrived) {
            if (obj->snap) {
                obj->vx = 0;
                obj->vz = 0;
                x = obj->target_x;
                z = obj->target_z;
                if (obj->flags == 0) {
                    y = obj->target_y;
                    obj->vy = 0;
                }
            }
            obj->target_x = TARGET_UNSET;
            obj->target_y = TARGET_UNSET;
            obj->target_z = TARGET_UNSET;
            obj->arrive_axis = 0;
        }
        obj->x = x;
        obj->y = y;
        obj->z = z;
        if (obj->turn & 1) {
            x = obj->vx;
            z = obj->vz;
            if (x != 0 || z != 0) {
                turn = (s16)(ArcTan2(z, x) - obj->angle);
                if (turn > 0x1000)
                    turn = 0x1000;
                if (turn < -0x1000)
                    turn = -0x1000;
                obj->angle += turn;
            }
        }
    }
}

s32 Script_StoreLookupResult(struct ScriptInterpreter *interpreter)
{
    s16 cursor = interpreter->cursor;

    interpreter->lookup_result = interpreter->script[cursor + 1] - 1;
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 0;
}

s32 Script_WaitForEvent(struct ScriptInterpreter *interpreter)
{
    if ((u32)interpreter->delay > 0x3B) {
        interpreter->delay = 0;
        goto block_3;
    }
    if (Object_IsTargetUnset(interpreter)!= 0) {
block_3:
        interpreter->cursor = (u16)interpreter->cursor + 1;
        return 1;
    }
    return 0;
}

s32 Script_InvokeCallback(struct ScriptInterpreter *interpreter)
{
    s16 initial = interpreter->cursor;
    ScriptCommand callback =
        (ScriptCommand)interpreter->script[initial + 1];

    if (callback(interpreter)!= 0)
        return 0;
    if (interpreter->cursor == initial)
        interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 Script_FindLabel(struct ScriptInterpreter *interpreter, u32 key)
{
    u32 *entries;
    s32 index;
    s16 *field = &interpreter->lookup_result;
    s32 zero = 0;

    *field = zero;
    if (key == 0) {
        return 0;
    }

    key &= 0xBFFFFFFF;
    entries = (u32 *)interpreter->script;
    index = 0;
    do {
        if (*entries++ == key) {
            return index + 1;
        }
        index++;
    } while (index <= 0x3FF);
    return 0;
}

s32 Script_RepeatOrJump(struct ScriptInterpreter *interpreter)
{
    const s32 *arguments;
    s32 repeat_limit;
    s32 jump_key;
    u8 *repeat_count;
    s32 next_count;

    arguments = &interpreter->script[interpreter->cursor + 1];
    repeat_limit = *arguments++;
    jump_key = *arguments;
    if (repeat_limit == 0xFFFF) {
        interpreter->cursor = Script_FindLabel(interpreter, jump_key);
    } else {
        repeat_count = &interpreter->repeat_count;
        next_count = *repeat_count + 1;
        *repeat_count = next_count;
        if ((s32)(u8)next_count < (s32)(s16)repeat_limit) {
            interpreter->cursor = Script_FindLabel(interpreter, jump_key);
        } else {
            *repeat_count = 0;
            interpreter->cursor = interpreter->cursor + 3;
        }
    }
    return 1;
}

s32 Script_JumpToLabel(struct ScriptInterpreter *interpreter)
{
    interpreter->cursor =
        Script_FindLabel(interpreter, interpreter->script[interpreter->cursor + 1]);
    return 1;
}

u32 Script_JumpIfTrue(struct ScriptInterpreter *interpreter)
{
    s32 index = interpreter->cursor;
    const s32 *table = interpreter->script;
    u32 value = table[index + 1];

    if (interpreter->condition_result != 0) {
        interpreter->cursor = Script_FindLabel(interpreter, value);
    } else {
        interpreter->cursor = (u16)interpreter->cursor + 2;
    }
    return 1;
}

u32 Script_JumpIfFalse(struct ScriptInterpreter *interpreter)
{
    s32 index = interpreter->cursor;
    const s32 *table = interpreter->script;
    u32 value = table[index + 1];

    if (interpreter->condition_result == 0) {
        interpreter->cursor = Script_FindLabel(interpreter, value);
    } else {
        interpreter->cursor = (u16)interpreter->cursor + 2;
    }
    return 1;
}

s32 Script_LoadMainScript(struct ScriptInterpreter *interpreter)
{
    s32 result;

    interpreter->script = Script_MainScript;
    interpreter->cursor = (result = 0);
    return result;
}

s32 Script_TestFlag(struct ScriptInterpreter *interpreter)
{
    interpreter->condition_result =
        GameFlag_TestFar(interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 Script_SetFlagAndTest(struct ScriptInterpreter *interpreter)
{
    s32 value;

    value = interpreter->script[interpreter->cursor + 1];
    interpreter->condition_result = GameFlag_TestFar(value);
    GameFlag_SetBitFar(value);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 Script_ClearFlagAndTest(struct ScriptInterpreter *interpreter)
{
    s32 value;

    value = interpreter->script[interpreter->cursor + 1];
    interpreter->condition_result = GameFlag_TestFar(value);
    GameFlag_ClearBitFar(value);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 Script_ToggleFlagAndTest(struct ScriptInterpreter *interpreter)
{
    s32 value;
    s32 result;

    value = interpreter->script[interpreter->cursor + 1];
    result = GameFlag_TestFar(value);
    interpreter->condition_result = result;
    if (((u32)result << 0x18) == 0x01000000) {
        GameFlag_ClearBitFar(value);
    } else {
        GameFlag_SetBitFar(value);
    }
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 Script_ApplyObjectArgument(struct ScriptInterpreter *interpreter)
{
    ObjectDispatch_ApplyArgumentToChildren(interpreter,
        interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 ObjectDispatch_RunHookAndReturnZero(struct ScriptInterpreter *interpreter)
{
    ObjectDispatch_Release((struct DispatchObject *)interpreter);
    return 0;
}

s32 Script_SkipCommand(struct ScriptInterpreter *interpreter)
{
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 Script_PlayAudioCue(struct ScriptInterpreter *interpreter)
{
    Audio_PlayCue(interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 ScriptObject_CheckOverlap(struct ScriptObjectEntry *object, s32 *values)
{
    s32 tmp;
    s32 index;
    u8 *flags;
    struct ScriptObjectEntry *entry;

    entry = *(struct ScriptObjectEntry **)((u32)&gObjectSlots);
    index = 0;
    flags = &entry->flags_59;
loop_1:
    if (entry->data != NULL && (1 & *flags) && entry != object) {
        tmp = index;
        if (Runtime_CheckRadiusOverlap(entry->values_08, entry->value_20 - 2,
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

struct ScriptObjectEntry *ScriptObject_FindOverlappingEntry(
    struct ScriptObjectEntry *object, s32 *values)
{
    s32 tmp;
    s32 index;
    u8 *flags;
    struct ScriptObjectEntry *entry;

    entry = *(struct ScriptObjectEntry **)((u32)&gObjectSlots);
    index = 0;
    flags = &entry->flags_59;
loop_1:
    if (entry->data != NULL && (1 & *flags) && entry != object) {
        tmp = index;
        if (Runtime_CheckRadiusOverlap(entry->values_08, entry->value_20 - 2,
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
    Object_SetPositionAndResetMotion((struct ObjectRuntime *)interpreter, first, second, third);
    interpreter->cursor = (u16)interpreter->cursor + 4;
    return 1;
}

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
    Object_SetMoveTarget(interpreter, first, second, third);
    interpreter->cursor = (u16)interpreter->cursor + 4;
    return 1;
}

s32 Script_ApplyRelativePosition(struct ScriptObjectRuntime *object)
{
    u8 *entry = (u8 *)(object->script + (s16)object->script_cursor);
    s32 *cursor = (s32 *)(entry + 4);
    s32 first = *cursor++;
    s32 second = *cursor++;
    s32 third = *cursor;

    Object_SetMoveTarget(object, object->x + first,
        object->y + second, object->z + third);
    object->script_cursor += 4;
    return 1;
}

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

s32 Script_ApplyLinkedObjectPosition(struct ScriptObjectRuntime *object)
{
    struct ScriptObjectRuntime *target;

    target = object->linked_object;
    Object_SetMoveTarget(object, target->x, target->y, target->z);
    object->script_cursor++;
    return 1;
}

s32 Script_ApplyLocalOffsetPosition(u8 *arg0)
{
    s32 offset[3];

    Object_SetMoveTarget(
        arg0,
        *(s32 *)(arg0 + 8) + offset[0],
        *(s32 *)(arg0 + 12) + offset[1],
        *(s32 *)(arg0 + 16) + offset[2]
    );
    *(u16 *)(arg0 + 4) += 3;
    return 1;
}
