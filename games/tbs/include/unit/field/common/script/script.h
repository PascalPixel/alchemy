#ifndef ALCHEMY_UNIT_FIELD_COMMON_SCRIPT_SCRIPT_H
#define ALCHEMY_UNIT_FIELD_COMMON_SCRIPT_SCRIPT_H

#include "types.h"

struct ObjectRuntime;
struct ScriptInterpreter;
struct ScriptObjectEntry;
struct ScriptObjectRuntime;
struct ScriptOperands;

s32 Script_AdvancePastCursor(struct ScriptInterpreter *sc);
s32 Script_ClearByte54(struct ScriptInterpreter *sc);
s32 Script_SetByte54(struct ScriptInterpreter *sc);
void Script_WaitForEventTimeout(s32 arg0);
s32 Object_IsTargetUnset(struct ObjectRuntime *object);
void Object_UpdateAllThumb(void);
void Object_SetPositionAndResetMotion(struct ObjectRuntime *obj, s32 x, s32 y, s32 z);
void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Object_RunUpdateAllFromHeap(void);
void Object_UpdateAllMotion(void);
s32 Script_StoreLookupResult(struct ScriptInterpreter *interpreter);
s32 Script_WaitForEvent(struct ScriptInterpreter *interpreter);
s32 Script_InvokeCallback(struct ScriptInterpreter *interpreter);
s32 Script_FindLabel(struct ScriptInterpreter *interpreter, u32 key);
s32 Script_RepeatOrJump(struct ScriptInterpreter *interpreter);
s32 Script_JumpToLabel(struct ScriptInterpreter *interpreter);
u32 Script_JumpIfTrue(struct ScriptInterpreter *interpreter);
u32 Script_JumpIfFalse(struct ScriptInterpreter *interpreter);
s32 Script_LoadMainScript(struct ScriptInterpreter *interpreter);
s32 Script_TestFlag(struct ScriptInterpreter *interpreter);
s32 Script_ToggleFlagAndTest(struct ScriptInterpreter *interpreter);
s32 Script_ApplyObjectArgument(struct ScriptInterpreter *interpreter);
s32 ObjectDispatch_RunHookAndReturnZero(struct ScriptInterpreter *interpreter);
s32 Script_SkipCommand(struct ScriptInterpreter *interpreter);
s32 Script_PlayAudioCue(struct ScriptInterpreter *interpreter);
s32 ScriptObject_CheckOverlap(struct ScriptObjectEntry *object, s32 *values);
struct ScriptObjectEntry *ScriptObject_FindOverlappingEntry(struct ScriptObjectEntry *object, s32 *values);
s32 Script_SetPositionAndResetMotion(struct ScriptInterpreter *interpreter);
s32 Script_ApplyAbsolutePosition(struct ScriptInterpreter *interpreter);
s32 Script_ApplyRelativePosition(struct ScriptObjectRuntime *object);
s32 Script_StoreAngleToLinkedObject(struct ScriptObjectRuntime *object);
s32 Script_ApplyLinkedObjectPosition(struct ScriptObjectRuntime *object);
s32 Script_ApplyLocalOffsetPosition(struct ScriptObjectRuntime *object);
s32 Camera_FollowLeaderInBounds(struct ObjectRuntime *obj);
s32 Object_ApproachLinkedObject(struct ObjectRuntime *object);
s32 Object_Wander(struct ScriptObjectRuntime *object);
s32 ScriptObject_WanderNearHome(struct ScriptObjectRuntime *object);

#endif
