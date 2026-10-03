#ifndef FIELD_SERVICE_H
#define FIELD_SERVICE_H

/*
 * Inline calls retained where direct engine calls change GCC 2.96 scheduling.
 * Other scene calls use the imports declared in FIELD_EVENT.H directly.
 */

#include "FIELD_EVENT.H"

static inline void Actor_EnableActionCallback(s32 actor, const u8 *table)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ActorEnableActionCallback(actor, table);
}

static inline void Actor_FaceEachOther(s32 actor, s32 other, s32 frames)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ActorFaceEachOther(actor, other, frames);
}

static inline void Actor_Jump(s32 actor, s32 height, s32 frames)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ActorJump(actor, height, frames);
}

static inline void Actor_RunRepeatedMotion(s32 actor, s32 repeats)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ActorRunRepeatedMotion(actor, repeats);
}

static inline void Actor_SetAnimation(s32 actor, s32 animation)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ActorSetAnimation(actor, animation);
}

static inline void Actor_SetAnimationAndWait(s32 actor, s32 animation)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ActorSetAnimationAndWait(actor, animation);
}

static inline void Actor_SetSpriteFlags(struct FieldActor *actor, s32 flags)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ActorSetSpriteFlags(actor, flags);
}

static inline void Actor_SetSpritePriority(s32 actor, s32 priority)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ActorSetSpritePriority(actor, priority);
}

static inline void Actor_StartRepeatedMotion(s32 actor, s32 repeats)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ActorStartRepeatedMotion(actor, repeats);
}

static inline void Actor_Stop(s32 actor)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ActorStop(actor);
}

static inline void Actor_WaitForMove(s32 actor)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ActorWaitForMove(actor);
}

static inline void Camera_WaitForMove(void)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_CameraWaitForMove();
}

static inline void ColorBuffer_Interpolate(s32 frames)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ColorBufferInterpolate(frames);
}

static inline void Event_Begin(void)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_EventBegin();
}

static inline s32 Event_ChooseYesNo(s32 actor, s32 flags)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    return Engine_EventChooseYesNo(actor, flags);
}

static inline void Event_End(void)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_EventEnd();
}

static inline struct FieldActor *Event_GetViewCenter(void)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    return Engine_EventGetViewCenter();
}

static inline void Event_OpenScreen(void)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_EventOpenScreen();
}

static inline void Event_RequestExit(s32 exit)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_EventRequestExit(exit);
}

static inline void Event_SetMessage(s32 message)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_EventSetMessage(message);
}

static inline void Event_Wait(s32 frames)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_EventWait(frames);
}

static inline void Event_WaitForScreen(void)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_EventWaitForScreen();
}

/* Carries out the move FieldObject_SetPosition set, and waits for it. */
static inline void FieldObject_CommitPosition(struct FieldActor *object)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ObjectCommitPosition(object);
}

/* Moves an object to a fixed-point position; committing it follows. */
static inline void FieldObject_SetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y,
                                           s32 fixed_z)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ObjectSetPosition(object, fixed_x, fixed_y, fixed_z);
}

static inline void *Heap_Allocate(s32 slot, s32 size)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    return Engine_HeapAllocate(slot, size);
}

static inline void Heap_Release(s32 slot)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_HeapRelease(slot);
}

static inline void Item_LoadIcon(s32 item)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ItemLoadIcon(item);
}

static inline void Item_ShowFound(s32 item, s32 height)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ItemShowFound(item, height);
}

static inline void Map_Redraw(void)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_MapRedraw();
}

static inline s32 Math_Cos(s32 angle)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    return Engine_MathCos(angle);
}

static inline s32 Math_Sin(s32 angle)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    return Engine_MathSin(angle);
}

static inline void Message_ShowCentered(s32 message, s32 flags)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_MessageShowCentered(message, flags);
}

static inline s32 Party_GiveItem(s32 item, s32 flags)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    return Engine_PartyGiveItem(item, flags);
}

static inline void Psynergy_Cancel(void)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_PsynergyCancel();
}

static inline void Shop_Open(s32 shop, s32 keeper)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_ShopOpen(shop, keeper);
}

static inline s32 Task_AddCallback(void (*callback)(void), s32 priority)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    return Engine_TaskAddCallback(callback, priority);
}

static inline void Task_Wait(s32 frames)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    Engine_TaskWait(frames);
}

static inline s32 Vram_Load(s32 block, s32 size, const void *data)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments between calls, changing instruction order. */
    return Engine_VramLoad(block, size, data);
}

#endif
