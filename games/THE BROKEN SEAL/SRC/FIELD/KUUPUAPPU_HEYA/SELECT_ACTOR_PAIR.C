#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "EVENT_RUNTIME.H"

enum SelectActorPairMessage {
    MSG_THERES_NOWHERE_RUN = 0x12a3,
    MSG_IM_SURROUNDED = 0x12a4,
    MSG_NOW_IVAN = 0x12a5
};

extern struct EventRuntime *Data_03001ebc;

void Func_0200cc7c();
void Func_0200c5f4();
void Func_0200cefc();
void Func_0200c684();
u8 *Func_0200cd7c();
void Func_0200a1c8();
void Func_0200cc74();

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
    Call1(Func_0200cc7c, 0x200c8c9);
    GameFlag_Clear(0x107);
    GameFlag_Clear(0x250);
    Actor_SetAnimation(24, 1);
    Actor_SetAnimation(25, 1);
    Actor_SetAnimation(2, 1);
    Actor_SetAnimation(0, 1);
    Actor_SetAnimation(1, 1);
    Actor_FaceActor(1, 2, 0);
    Actor_FaceEachOther(0, 2, 0);
    Actor_FaceActor(24, 2, 0);
    Actor_FaceActor(25, 2, 0);
    Event_Wait(10);
    actor = 24;
    switch (*(s16 *)(((s32)work + 0x182))) {
    case 202:
    case 203:
        Event_SetMessage(MSG_IM_SURROUNDED);
        Actor_SetAttachedEffect(25, 0x102);
        Actor_RunRepeatedMotion(25, 2);
        Func_0200c5f4(25, 20);
        if (*(s16 *)(((s32)work + 0x182)) == 202) {
            actor = 25;
            break;
        }
        /* fall through */
    case 201:
        Event_SetMessage(MSG_THERES_NOWHERE_RUN);
        Actor_SetAttachedEffect(24, 0x102);
        Actor_RunRepeatedMotion(24, 2);
        actor = 24;
        Func_0200c5f4(24, 20);
        break;
    }
    Actor_SetAnimation(2, 1);
    Actor_FaceActor(2, actor, 0);
    Actor_FaceActor(1, 2, 0);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Event_SetMessage(MSG_NOW_IVAN);
    Func_0200c5f4(1, 20);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Func_0200c5f4(2, 20);
    SceneEffect_ApplyPairWithValue141(2, actor);
    Func_0200cefc();
    Event_Wait(60);
    Actor_RunRepeatedMotion(24, 2);
    Func_0200c5f4(24, 20);
    Actor_RunRepeatedMotion(25, 2);
    Func_0200c5f4(25, 20);
    Actor_ShowEmote(2, 0x100, 60);
    Func_0200c684();
    Actor_StartRepeatedMotion(1, 2);
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(20);
    Func_0200c5f4(1, 20);
    Actor_FaceActor(2, 1, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(40);
    record = Func_0200cd7c(0);
    (*(struct SceneObjectFlags **)(record + 80))->mode = 1;
    record = Func_0200cd7c(1);
    (*(struct SceneObjectFlags **)(record + 80))->mode = 1;
    record = Func_0200cd7c(2);

    (*(struct SceneObjectFlags **)(record + 80))->mode = 1;
    Data_03001ebc->value_1c8 = 24;
    Data_03001ebc->value_1c0 = 0x201;
    Event_CloseScreen();
    Event_WaitForScreen();
    Func_0200a1c8();
    Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
    GameFlag_Set(0x853);
    {
        u8 *record = Func_0200cd7c(24);
        s32 shown = 5;

        *(u16 *)((s32)record + 100) = shown;
    }
    {
        u8 *record = Func_0200cd7c(25);
        s32 shown = 4;

        *(u16 *)((s32)record + 100) = shown;
    }
    Call2(Func_0200cc74, 0x200aba1, 0xc80);
    Data_03001ebc->value_1c0 = 0x209;
    Event_End();
}
