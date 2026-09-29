#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "EVENT_RUNTIME.H"
extern u8 MsgKuupuappuImSurrounded[];
extern u8 MsgKuupuappuNowIvan[];
extern u8 MsgKuupuappuTheresNowhereRun[];


extern struct EventRuntime *Data_03001ebc;

void Scheduler_RemoveCallback();
void SceneActor_SetModeZeroAndValue();
void Ui_SetBank15PaletteAndClearRenderMode();
void SceneState_SetValue2ThenFinish();
u8 *Object_GetById();
void KuupuappuHeya_RunScene021C8();
void Engine_ScheduleCallback();

/* Constant-bearing scene calls use shared inline argument helpers. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

struct SceneObjectFlags {
    u8 unknown_000[9];
    unsigned char unk_low : 2;
    unsigned char mode : 2;
    unsigned char unk_high : 4;
};

void FieldScene_SelectActorPair(void)
{
    u8 *record;
    s32 actor;
    u8 *work;

    work = (u8 *)Data_03001ebc;
    Event_Begin();
    Call1(Scheduler_RemoveCallback, 0x200c8c9);
    GameFlag_Clear(0x107);
    GameFlag_Clear(0x250);
    Actor_SetAnimation(24, 1);
    Actor_SetAnimation(25, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(24, ACTOR_IVAN, 0);
    Actor_FaceActor(25, ACTOR_IVAN, 0);
    Event_Wait(10);
    actor = 24;
    switch (*(s16 *)(((s32)work + 0x182))) {
    case 202:
    case 203:
        Event_SetMessage((s32)MsgKuupuappuImSurrounded);
        Actor_SetAttachedEffect(25, 0x102);
        Actor_RunRepeatedMotion(25, 2);
        SceneActor_SetModeZeroAndValue(25, 20);
        if (*(s16 *)(((s32)work + 0x182)) == 202) {
            actor = 25;
            break;
        }
        /* fall through */
    case 201:
        Event_SetMessage((s32)MsgKuupuappuTheresNowhereRun);
        Actor_SetAttachedEffect(24, 0x102);
        Actor_RunRepeatedMotion(24, 2);
        actor = 24;
        SceneActor_SetModeZeroAndValue(24, 20);
        break;
    }
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_FaceActor(ACTOR_IVAN, actor, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Event_SetMessage((s32)MsgKuupuappuNowIvan);
    SceneActor_SetModeZeroAndValue(1, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(2, 20);
    SceneEffect_ApplyPairWithValue141(2, actor);
    Ui_SetBank15PaletteAndClearRenderMode();
    Event_Wait(60);
    Actor_RunRepeatedMotion(24, 2);
    SceneActor_SetModeZeroAndValue(24, 20);
    Actor_RunRepeatedMotion(25, 2);
    SceneActor_SetModeZeroAndValue(25, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 60);
    SceneState_SetValue2ThenFinish();
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(1, 20);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_GERALD, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(40);
    record = Object_GetById(0);
    (*(struct SceneObjectFlags **)(record + 80))->mode = 1;
    record = Object_GetById(1);
    (*(struct SceneObjectFlags **)(record + 80))->mode = 1;
    record = Object_GetById(2);

    (*(struct SceneObjectFlags **)(record + 80))->mode = 1;
    Data_03001ebc->value_1c8 = 24;
    Data_03001ebc->value_1c0 = 0x201;
    Event_CloseScreen();
    Event_WaitForScreen();
    KuupuappuHeya_RunScene021C8();
    Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
    GameFlag_Set(0x853);
    {
        u8 *record = Object_GetById(24);
        s32 shown = 5;

        *(u16 *)((s32)record + 100) = shown;
    }
    {
        u8 *record = Object_GetById(25);
        s32 shown = 4;

        *(u16 *)((s32)record + 100) = shown;
    }
    Call2(Engine_ScheduleCallback, 0x200aba1, 0xc80);
    Data_03001ebc->value_1c0 = 0x209;
    Event_End();
}
