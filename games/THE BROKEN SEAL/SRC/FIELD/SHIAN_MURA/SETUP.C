#include "TYPES.H"
#include "FIELD_EVENT.H"

void BattleFx_SetQueuedSoundAndPlay(s32 music);


void Actor_UpdatePresentationFlag(void);
void ShianMura_WatchGateTrigger(union FieldObject *object);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 ShianMura_SetupScene(void)
{
    struct FieldActor *actor;
    u32 n;
    s32 x;

    gEventWork->start_transition = 0x100;
    BattleFx_SetQueuedSoundAndPlay(169);
    if (gGameState.entrance > 9) {
        Call1((void (*)())Engine_GameFlagClear, 0x12f);
    }
    if (Value1((s32 (*)())Engine_GameFlagIsSet, 0x895)) {
        Call3((void (*)())Engine_ActorFaceDirection, 13, 0x8000, 0);
        Call3((void (*)())Engine_ActorSetPosition, 14, 0x920000, 0x1380000);
        Engine_ActorFaceDirection(14, 0, 0);
        if (Value1((s32 (*)())Engine_GameFlagIsSet, 0x89a)) {
            Engine_ActorSetPosition(17, 0, 0);
        }
    }
    if (Engine_GameFlagIsSet(0x8b0)) {
        Engine_ActorSetPosition(17, 0, 0);
    }
    for (n = 0; n <= 2; n++) {
        struct FieldActor *actor;
        actor = Engine_ActorGet(n + 23);
        /* FAKEMATCH: one value spans priority and motion setup so the
         * motion zero stays inside the loop and feeds the later actors. */
        x = 1;
        actor->sprite->priority = x;
        x = 0;
        actor->motion_flags = x;
        actor->collision_flags = 8;
        Engine_ActorSetSpriteFlags(actor, 0);
        ObjectGroup_SetChildValue(actor, 15);
        actor->priority_flags = (actor->priority_flags & 254) | 2;
    }
    if (Value1((s32 (*)())Engine_GameFlagIsSet, 0x202)) {
        Call3((void (*)())Engine_ActorSetPosition, 14, 0x920000, 0x1380000);
        Engine_ActorFaceDirection(14, 0, 0);
    }
    if (Value1((s32 (*)())Engine_GameFlagIsSet, 0x201)) {
        Engine_ActorSetAnimation(20, 5);
        {
            s32 px = Engine_ActorGet(20)->x.fixed;

            Call6((void (*)())Engine_MapCopyCellAttributes, 3, 17, 1, 1, px >> 20, Engine_ActorGet(20)->z.fixed >> 20);
        }
        Call2((void (*)())Engine_TaskAddCallback, (s32)Actor_UpdatePresentationFlag, 0xc80);
    }
    Engine_ActorSetChildValue(18, 2);
    Engine_ActorGet(18)->update = ShianMura_WatchGateTrigger;
    actor = Engine_ActorGet(19);
    actor->motion_flags = x;
    actor->y.fixed = 0x100000;
    actor->target_y = 0x100000;
    actor->scale_x = 0x8ccc;
    actor->scale_y = 0x6666;
    actor->sprite->rotation = 0x8000;
    Engine_ActorSetSpriteFlags(Engine_ActorGet(21), 0);
    {
        /* FAKEMATCH: narrow local retains the short-range zero pool load. */
        u8 shown = 0;

        Engine_ActorGet(21)->motion_flags = shown;
    }
    Engine_ActorGet(21)->y.fixed = x;
    Engine_ActorGet(21)->target_y = -0x80000000;
    return 0;
}
