#ifndef FIELD_SERVICE_H
#define FIELD_SERVICE_H

#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field2.h"
#include "unit/field/common/script/field3.h"
#include "unit/field/common/script/field5.h"
#include "unit/field/common/script/field6.h"
#include "unit/field/common/script/field7.h"
#include "unit/field/common/script/field8.h"
#include "unit/field/common/script/field9.h"
#include "unit/field/common/script/field10.h"
#include "unit/field/common/script/field11.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "field_event.h"
#include "farcall.h"

static inline void Actor_EnableActionCallback(s32 actor, const s32 *table)
{
	Engine_ActorEnableActionCallback(actor, table);
}

static inline void Actor_FaceEachOther(s32 actor, s32 other, s32 frames)
{
	Engine_ActorFaceEachOther(actor, other, frames);
}

static inline void Actor_Jump(s32 actor, s32 height, s32 frames)
{
	Engine_ActorJump(actor, height, frames);
}

static inline void Actor_RunRepeatedMotion(s32 actor, s32 repeats)
{
	Engine_ActorRunRepeatedMotion(actor, repeats);
}

static inline void Actor_SetAnimation(s32 actor, s32 animation)
{
	Engine_ActorSetAnimation(actor, animation);
}

static inline void Actor_SetAnimationAndWait(s32 actor, s32 animation)
{
	Engine_ActorSetAnimationAndWait(actor, animation);
}

static inline void Actor_SetSpriteFlags(struct FieldActor *actor, s32 flags)
{
	Engine_ActorSetSpriteFlags(actor, flags);
}

static inline void Actor_SetSpritePriority(s32 actor, s32 priority)
{
	Engine_ActorSetSpritePriority(actor, priority);
}

static inline void Actor_StartRepeatedMotion(s32 actor, s32 repeats)
{
	Engine_ActorStartRepeatedMotion(actor, repeats);
}

static inline void Actor_Stop(s32 actor)
{
	Engine_ActorStop(actor);
}

static inline void Actor_WaitForMove(s32 actor)
{
	Engine_ActorWaitForMove(actor);
}

static inline void Camera_WaitForMove(void)
{
	Engine_CameraWaitForMove();
}

static inline void ColorBuffer_Interpolate(s32 frames)
{
	Engine_ColorBufferInterpolate(frames);
}

static inline void Event_Begin(void)
{
	Engine_EventBegin();
}

static inline s32 Event_ChooseYesNo(s32 actor, s32 flags)
{
	return Engine_EventChooseYesNo(actor, flags);
}

static inline void Event_End(void)
{
	Engine_EventEnd();
}

static inline struct FieldActor *Event_GetViewCenter(void)
{
	return Engine_EventGetViewCenter();
}

static inline void Event_OpenScreen(void)
{
	Engine_EventOpenScreen();
}

static inline void Event_RequestExit(s32 exit)
{
	Engine_EventRequestExit(exit);
}

static inline void Event_SetMessage(s32 message)
{
	Engine_EventSetMessage(message);
}

static inline void Event_Wait(s32 frames)
{
	Engine_EventWait(frames);
}

static inline void Event_WaitForScreen(void)
{
	Engine_EventWaitForScreen();
}

static inline void FieldObject_CommitPosition(struct FieldActor *object)
{
	Engine_ObjectCommitPosition(object);
}

static inline void FieldObject_SetPosition(
	struct FieldActor *object, s32 fixed_x, s32 fixed_y, s32 fixed_z)
{
	Engine_ObjectSetPosition(object, fixed_x, fixed_y, fixed_z);
}

static inline void *Heap_Allocate(s32 slot, s32 size)
{
	return Engine_HeapAllocate(slot, size);
}

static inline void Heap_Release(s32 slot)
{
	Engine_HeapRelease(slot);
}

static inline void Item_LoadIcon(s32 item)
{
	Engine_ItemLoadIcon(item);
}

static inline void Item_ShowFound(s32 item, s32 height)
{
	Engine_ItemShowFound(item, height);
}

static inline void Map_Redraw(void)
{
	Engine_MapRedraw();
}

static inline s32 Math_Cos(s32 angle)
{
	return Engine_MathCos(angle);
}

static inline s32 Math_Sin(s32 angle)
{
	return Engine_MathSin(angle);
}

static inline void Message_ShowCentered(s32 message, s32 flags)
{
	Engine_MessageShowCentered(message, flags);
}

static inline s32 Party_GiveItem(s32 item, s32 flags)
{
	return Engine_PartyGiveItem(item, flags);
}

static inline void Psynergy_Cancel(void)
{
	Engine_PsynergyCancel();
}

static inline s32 Task_AddCallback(void (*callback)(void), s32 priority)
{
	return Engine_TaskAddCallback(callback, priority);
}

static inline void Task_Wait(s32 frames)
{
	Engine_TaskWait(frames);
}

static inline s32 Vram_Load(s32 block, s32 size, const void *data)
{
	return Engine_VramLoad(block, size, data);
}

static inline void FieldObjectMotion_CommitPositionAndActivate(s32 a0, s32 a1, s32 a2)
{
	ObjectMotion_CommitPositionAndActivate(a0, a1, a2);
}

static inline void FieldObjectMotion_OffsetPositionAndResetMotion(s32 a0, s32 a1, s32 a2)
{
	Engine_ActorSetDestinationOffset(a0, a1, a2);
}

static inline void FieldObjectMotion_CenterOnColumn(s32 a0, s32 a1, s32 a2)
{
	ObjectMotion_CenterOnColumn(a0, a1, a2);
}

static inline void FieldObjectMotion_ResetAndSetPosition(s32 a0, s32 a1, s32 a2)
{
	Engine_ActorSetDestination(a0, a1, a2);
}

static inline void FieldObject_MotionSetPositionAndCommit(s32 actor, s32 x, s32 z)
{
	Engine_ObjectMotionSetPositionAndCommit(actor, x, z);
}

static inline s32 FieldScheduler_AddOrUpdateCallback(s32 a0, s32 a1)
{
	return Engine_TaskAddCallback(a0, a1);
}

static inline void FieldObject_SetMoveTarget(s32 a0, s32 a1, s32 a2, s32 a3)
{
	Engine_ObjectSetPosition(a0, a1, a2, a3);
}

static inline void Field_SetMapCellCollision(s32 a0, s32 a1, s32 a2, s32 a3)
{
	SetMapCellCollision(a0, a1, a2, a3);
}

static inline void FieldMap_SetWindowCellTile(s32 a0, s32 a1, s32 a2, s32 a3)
{
	Map_SetWindowCellTile(a0, a1, a2, a3);
}

static inline void FieldMotion_LaunchFromFocusedObject(s32 actor, s32 dx, s32 dz, s32 facing)
{
	Motion_LaunchFromFocusedObject(actor, dx, dz, facing);
}

static inline void FieldObject_SetTargetAndCallback(s32 object, s32 target, const s32 *actions)
{
	Object_SetTargetAndCallback(object, target, actions);
}

static inline void FieldVector_AddPolarOffset(s32 radius, s32 heading, struct FieldPosition *probe)
{
	Vector_AddPolarOffset(radius, heading, probe);
}

#endif
