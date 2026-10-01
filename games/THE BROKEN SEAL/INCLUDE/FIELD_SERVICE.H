#ifndef FIELD_SERVICE_H
#define FIELD_SERVICE_H

/*
 * The inline services for the engine imports FIELD_EVENT.H declares without
 * one. A scene written against this header reaches every engine service
 * through its inline and names no import directly: one rule for every call
 * site, in place of a direct call here and a forwarding wrapper there.
 */

#include "FIELD_EVENT.H"

static inline void Actor_EnableActionCallback(s32 actor, const u8 *table)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_ActorEnableActionCallback(actor, table);
}

static inline void Actor_FaceEachOther(s32 actor, s32 other, s32 frames)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_ActorFaceEachOther(actor, other, frames);
}

static inline void Actor_Jump(s32 actor, s32 height, s32 frames)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_ActorJump(actor, height, frames);
}

static inline void Actor_RunRepeatedMotion(s32 actor, s32 repeats)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_ActorRunRepeatedMotion(actor, repeats);
}

static inline void Actor_SetAnimation(s32 actor, s32 animation)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_ActorSetAnimation(actor, animation);
}

static inline void Actor_SetAnimationAndWait(s32 actor, s32 animation)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_ActorSetAnimationAndWait(actor, animation);
}

static inline void Actor_SetSpriteFlags(struct FieldActor *actor, s32 flags)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_ActorSetSpriteFlags(actor, flags);
}

static inline void Actor_StartRepeatedMotion(s32 actor, s32 repeats)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_ActorStartRepeatedMotion(actor, repeats);
}

static inline void Actor_Stop(s32 actor)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_ActorStop(actor);
}

static inline void Actor_WaitForMove(s32 actor)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_ActorWaitForMove(actor);
}

static inline void Camera_WaitForMove(void)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_CameraWaitForMove();
}

static inline void Event_Begin(void)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_EventBegin();
}

static inline s32 Event_ChooseYesNo(s32 actor, s32 flags)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    return Engine_EventChooseYesNo(actor, flags);
}

static inline void Event_End(void)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_EventEnd();
}

static inline void Event_RequestExit(s32 exit)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_EventRequestExit(exit);
}

static inline void Event_SetMessage(s32 message)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_EventSetMessage(message);
}

static inline void Event_Wait(s32 frames)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_EventWait(frames);
}

static inline void *Heap_Allocate(s32 slot, s32 size)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    return Engine_HeapAllocate(slot, size);
}

static inline void Heap_Release(s32 slot)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_HeapRelease(slot);
}

static inline void Item_LoadIcon(s32 item)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_ItemLoadIcon(item);
}

static inline void Item_ShowFound(s32 item, s32 height)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_ItemShowFound(item, height);
}

static inline void Message_ShowCentered(s32 message, s32 flags)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_MessageShowCentered(message, flags);
}

static inline s32 Party_GiveItem(s32 item, s32 flags)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    return Engine_PartyGiveItem(item, flags);
}

static inline void Psynergy_Cancel(void)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_PsynergyCancel();
}

static inline void Task_Wait(s32 frames)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    Engine_TaskWait(frames);
}

static inline s32 Vram_Load(s32 block, s32 size, const void *data)
{
    /* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
    return Engine_VramLoad(block, size, data);
}

#endif
