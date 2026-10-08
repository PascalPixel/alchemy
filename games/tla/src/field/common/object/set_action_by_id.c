#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "eventwrk.h"
#include "object_runtime.h"
#include "objdisp.h"
#include "system.h"

#define ObjectDispatch_ApplyValueToChildrenFar ((void (*)(struct ObjectRuntime*,s32))ObjectDispatch_ApplyValueToChildrenFar)

void ObjectMotion_SetHorizontalPositionWithTerrain(u32, s32, s32);


struct ObjectRuntime *Object_GetById(u32 object_id);
void ObjectMotion_SetSpeedParameters(u32 object_id, s32 speed_limit, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(u32 object_id, s32 action);
void ObjectMotion_EnableActionAndResetMotion(u32 object_id);
void Object_LinkObjectAndSetCallback(u32 object_id, u32 linked_object_id);
void Object_SetActionCallbackAndRefreshById(u32 object_id, s32 action);
void ObjectMotion_ResetAndSetPositionInMode2(u32 object_id, s32 x, s32 z);
void ObjectMotion_SetPositionAndReset(u32 object_id, s32 x, s32 z);
void ObjectMotion_OffsetPositionAndResetMotion(u32 object_id, s32 x_offset, s32 z_offset);
void Motion_LaunchFromFocusedObject(u32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ObjectMotion_CommitCurrentPositionAndActivate(u32 object_id);
void ObjectMotion_SetHorizontalPositionWithTerrain(u32 object_id, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(u32 object_id, s32 action);
void ObjectMotion_Launch(u32 object_id, s32 speed, s32 event_id);

void Object_SetActionById(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        ObjectDispatch_ApplyValueToChildrenFar(object, action);
}
