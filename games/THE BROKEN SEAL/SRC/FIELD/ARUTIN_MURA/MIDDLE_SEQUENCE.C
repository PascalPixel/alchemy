#include "ARUTIN.H"

void FieldScene_RunMiddleSequence(void)
{
    s32 scene;
    s32 rec5;
    s32 rec6;
    s32 rec0;
    s32 kind;

    scene = Value1(Engine_ActorGet, 0);
    rec5 = GameFlag_IsSet(0x242);
    if (rec5 != 0) {
        Map_CopyCellsTo(64, 32, 0, 32, 32, 32);
        Map_CopyCellAttributes(64, 32, 32, 32, 0, 0);
        kind = 20;
    } else {
        rec6 = GameFlag_IsSet(0x241);
        if (rec6 != 0) {
            Map_CopyCellsTo(64, 0, 0, 32, 32, 32);
            Value6(Engine_MapCopyCellAttributes, 64, 0, 32, 32, rec5, rec5);
            Actor_Destroy(17);
            kind = 20;
        } else {
            rec0 = GameFlag_IsSet(0x240);
            if (rec0 == 0) {
                goto L_020009b8;
            }
            Map_CopyCellsTo(0, 64, 0, 32, 32, 32);
            Value6(Engine_MapCopyCellAttributes, 0, 64, 32, 32, rec6, rec6);
            Actor_Destroy(16);
            kind = 17;
        }
    }
    Actor_Destroy(kind);
    Actor_Destroy(21);
    goto L_020009da;
L_020009b8:
    Map_CopyCellAttributes(0, 32, 32, 32, rec0, rec0);
    Actor_Destroy(15);
    Actor_Destroy(16);
    Actor_Destroy(17);
L_020009da:
    if (GameFlag_IsSet(0x8ff) != 0) {
        Actor_Destroy(18);
    } else {
        BattleFx_SetQueuedSoundAndPlay(170);
        Actor_SetChildValue(18, 2);
        Actor_SetAnimation(18, 3);
        Call2((void (*)())Engine_TaskAddCallback, (s32)SceneEffect_SpawnDriftingParticle, 0xc80);
    }
    if (gGameState.entrance == 3) {
        GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    }
    Map_CopyCellAttributes(0, 33, 4, 3, 20, 41);
    if (GameFlag_IsSet(0x906) != 0) {
        Actor_SetPosition(19, 0x1680000, 0xa80000);
    }
    Actor_SetSpriteFlags((s32)Engine_ActorGet(19), 0);
    Actor_SetChildValue(22, 15);
    Call2((void (*)())Engine_ActorSetChildValue, 23, 15);
    Actor_SetChildValue(24, 15);
    {
        u8 bits = 8;
        u8 *flags = (u8 *)Engine_ActorGet(22) + 89;
        u8 value = *flags;

        value |= bits;
        *flags = value;
        flags = (u8 *)Engine_ActorGet(23) + 89;
        value = *flags;
        value |= bits;
        *flags = value;
        flags = (u8 *)Engine_ActorGet(24) + 89;
        bits |= *flags;
        *flags = bits;
    }
    {
        u8 bits = 2;
        u8 *flags = (u8 *)Engine_ActorGet(22) + 35;
        u8 value = *flags;

        value |= bits;
        *flags = value;
        flags = (u8 *)Engine_ActorGet(23) + 35;
        value = *flags;
        value |= bits;
        *flags = value;
        flags = (u8 *)Engine_ActorGet(24) + 35;
        bits |= *flags;
        *flags = bits;
    }
    Actor_SetSpritePriority(22, 1);
    Actor_SetSpritePriority(23, 1);
    Actor_SetSpritePriority(24, 1);
    Task_Wait(1);
    Event_Begin();
    Camera_MoveTo(*(s32 *)(scene + 8), *(s32 *)(scene + 12), *(s32 *)(scene + 16), 0);
    Map_Redraw();
    Event_End();
    Task_Wait(1);
}

void FieldScene_RunScene3a3SequenceD(void)
{
    u8 *actor;
    s32 facing;

    if (GameFlag_IsSet(0x240) == 0) {
        Actor_SetPosition(8, 0x3280000, 0x2d70000);
        actor = (u8 *)Engine_ActorGet(8);
        facing = 0x3000;
        *(u16 *)(actor + 6) = facing;
        Actor_SetPosition(9, 0x31a0000, 0x3390000);
    }
    if (GameFlag_IsSet(0x241) == 0) {
        Actor_SetPosition(10, 0x2300000, 0x2c60000);
        actor = (u8 *)Engine_ActorGet(10);
        facing = 0x1000;
        *(u16 *)(actor + 6) = facing;
        Actor_SetPosition(11, 0x2400000, 0x2c60000);
    }
    if (GameFlag_IsSet(0x242) == 0) {
        Actor_SetPosition(15, 0x1270000, 0x2e80000);
        actor = (u8 *)Engine_ActorGet(15);
        facing = 0xb000;
        *(u16 *)(actor + 6) = facing;
    } else {
        u8 flags;

        actor = (u8 *)Value1(Engine_ActorGet, 15);
        flags = 4;
        flags |= actor[89];
        actor[89] = flags;
    }
    actor = (u8 *)Value1(Engine_ActorGet, 17);
    if (actor != 0) {
        u8 flags = 4;

        flags |= actor[89];
        actor[89] = flags;
    }
    actor = (u8 *)Value1(Engine_ActorGet, 16);
    if (actor != 0) {
        u8 flags = 4;

        flags |= actor[89];
        actor[89] = flags;
    }
}

void SceneActor_ResetStateAndSpan(struct Actor02000c0c *actor)
{

    u8 *state = &actor->state;
    s32 clear = 0;
    u8 *attached;

    *state = (u8)clear;
    attached = actor->attached;
    clear -= 13;
    attached[9] = (clear & attached[9]) | 4;
    Object_SetPalette(actor, 3);
    Engine_ActorSetSpriteFlags((struct FieldActor *)actor, 0);
    actor->span = 0x4CCC;
    actor->reach = 0x4CCC;
}

void SceneEffect_UpdateDriftingParticle(struct SceneMotion *work)
{
    work->x += (work->timer << 12) +
        ((s16)((s32)((Random16() * 2) >> 16) - 1) << 15);
    if (work->timer <= 3) {
        work->z += -((Random16() * 0x8000) >> 16) - 0x10000;
        work->scale_x += 0x2666;
        work->scale_y += -0xa3d;
    } else {
        work->z += 0x20000;
        work->scale_x += 0x7ae;
        work->scale_y += 0x7ae;
    }
    if ((Random16() * work->timer) >> 16 == 0)
        Object_SetPalette(work, 7);
    if (work->timer != 0)
        work->timer--;
    else
        work->timer = ((Random16() * 5) >> 16) * 2 + 2;
    if (--work->active == 0) {
        work->callback = 0;
        Engine_ObjectDispatchRelease(work);
    }
}

void SceneEffect_SpawnDriftingParticle(void)
{
    struct SceneMotion *work;
    if ((gFrameCount & 3) == 0) {
        work = CreateActor((struct SceneMotion *(*)(s32, s32, s32, s32))Engine_ObjectCreate, 222, 0x400000, 0, 0x1900000);
        if (work != 0) {
            work->timer = 20;
            work->delay = 0;
            work->active = 20;
            SceneActor_ResetStateAndSpan(work);
            work->callback = SceneEffect_UpdateDriftingParticle;
            Object_SetAnimation(work, 1);
        }
    }
}
