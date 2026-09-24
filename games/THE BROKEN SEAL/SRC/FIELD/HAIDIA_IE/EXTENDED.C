#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define CalculateFacingAngle Func_02002656

#include "FACING_OBJECT.H"

enum ValeMessage {
    MSG_PARTY_PP_RESTORED = 0x974,
    MSG_CAN_I_USE_PSYNERGY = 0xea8,
    MSG_I_HAVE_SOME_PSYNERGY_LEFT = 0xeae,
    MSG_BE_SURE_TO_HELP_GARCIA = 0xeb1,
    MSG_MEDITATE_ON_MT_ALEPH_DAILY = 0xf3c,
    MSG_A_DIFFICULT_TIME_THREE_YEARS_AGO = 0xf3f,
    MSG_DID_THE_TRAVELERS_MEET_THE_MAYOR = 0xf44,
    MSG_HAVE_I_SHOWN_YOU_MY_ABILITY = 0xf48,
    MSG_BEHOLD_THE_POWER_OF_PSYNERGY = 0xf4b,
    MSG_CHECKED_THE_PSYNERGY_STONE = 0x111f,
    MSG_YOU_SAW_THE_WISE_ONE = 0x1191,
    MSG_THE_STONE_FELL_ON_THE_HUT = 0x11c7,
    MSG_YOU_CAME_BACK_HOME = 0x1be3,
    MSG_THIS_IS_VALE = 0x1be4,
    MSG_ANYTHING_INTERESTING_ON_YOUR_TRIP = 0x1be8,
    MSG_THE_PSYNERGY_STONE_IS_GONE = 0x1c94
};

struct Obj {
    u8 pad00[6];
    u16 f06;
    u8 pad08[0x30];
    s32 f38;
    s32 f3c;
    s32 f40;
};

struct VerticalEffectAnchor {
    u8 pad00[8];
    s32 x;
    u8 pad0c[4];
    s32 z;
};

struct SceneVerticalEffect {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[4];
    s32 amplitude_x;
    s32 amplitude_y;
    u8 pad20[0x44];
    u16 frame;
    u8 pad66[2];
    struct VerticalEffectAnchor *anchor;
};

extern u8 Data_0200b144[];
extern u8 Data_0200b108[];
extern u8 Data_0200b380[];
extern u8 Data_0200b560[];
extern u8 Data_0200b7d0[];
extern u8 Data_0200b170[];
extern u8 Data_0200bcec[];
extern u8 Data_0200bb3c[];
extern u8 Data_0200bb30[];
extern u8 Data_0200ba64[];
extern u8 Data_0200b938[];
extern u8 LinkedMessage_ArentYouWorriedAboutCrossing[];
extern u8 Value_0200beb4;
extern u8 LinkedMessage_JasmineWentOffWay;
extern u8 Data_0200ae34[];

s32 Func_02002656(s32, s32);
void Func_02002962(s32, s32);
void Func_020027a4(void);
s32 Func_0200290c();
s32 Func_0200290c_a();
void Func_0200295a();
void Func_020029c2_a();
void Func_0200273a(s32, s32);
void Func_02002778(s32, s32);
void Func_020027c8();
s32 Func_02002b36();
s32 Func_02002b40();
void Func_02002b8a();
void Func_02002b90();
s32 Func_02002bec_a();
s32 Func_02002c22();
void Func_02004c4a(s32);
void Func_020048be(void);
void Func_02004c5a(s32);
void Func_020048ce(void);
void Func_02004c6a(s32);
void Func_0200491a(void);
struct Obj *Func_02002d3e(s32);
void Func_02002f20(void);
void Func_02004dac(void);
void Func_02004768(s32);
s32 Func_02004930(s32, s32);
void Func_020047a4(s32);
s32 Func_0200496c(s32, s32);

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1_02000750(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call1_02000780(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call1_020007ec(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2_0200081c(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call1_0200081c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2_0200084c(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value2_0200087c(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value2_020008ac(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call1_020001c4(void (*f)(), s32 a0)
{
    f(a0);
}

s32 Object_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 facing_delta;
    u16 old_facing;
    s32 target_facing;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        target_facing = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old_facing = object->facing;
        facing_delta = (s16)(target_facing - old_facing);
        if (facing_delta != 0) {
            if (facing_delta > 0x1000) {
                facing_delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (facing_delta < -0x1000) {
                facing_delta = -0x1000;
            }
            object->facing = (u16)(old_facing + facing_delta);
        }
    }
    return 1;
}

/*
 * Table getter for resource_374. The owner at 0x02000088 is eight bytes and
 * includes its one pool word at 0x0200008c: the pc-relative load reads that
 * word, so the word belongs to this owner. The word is an address returned
 * without being dereferenced. Many getters share this body, but each returns
 * a different address.
 */
u8 *SceneData_GetTableAfa0(void)
{
    return (u8 *)0x0200afa0;
}

s32 Func_02000090(void)
{
    return 0;
}

void *SceneData_SelectTableByFlag834(void)
{
    if (GameFlag_IsSet(0x834) != 0) {
        return Data_0200b144;
    }
    return Data_0200b108;
}

void *SceneData_SelectTableByFlags834And87a(void)
{
    if (GameFlag_IsSet(0x834) != 0) {
        return Data_0200b380;
    }
    if (gGameState.entrance == 12) {
        return Data_0200b560;
    }
    if (GameFlag_IsSet(0x87a) != 0) {
        return Data_0200b7d0;
    }
    return Data_0200b170;
}

void Scene_CheckPsynergyStone(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_CHECKED_THE_PSYNERGY_STONE, 1);
    Audio_PlayCue(126);
    Func_02002962(0x3e7, 0);
    Event_Wait(10);
    Message_ShowCentered(MSG_PARTY_PP_RESTORED, 1);
    Func_020027a4();
    GameFlag_Clear(322);
    Event_End();
}

void *SceneData_SelectTableByFlags87a_815_834(void)
{
    if (GameFlag_IsSet(0x87a) != 0) {
        return Data_0200bcec;
    }
    if (GameFlag_IsSet(0x815) != 0) {
        return Data_0200bb3c;
    }
    if (gGameState.entrance == 12) {
        return Data_0200bb30;
    }
    if (GameFlag_IsSet(0x834) != 0) {
        return Data_0200ba64;
    }
    return Data_0200b938;
}

void Villager_AskAboutMeditation(void)
{
    Event_Begin();
    Event_SetMessage(MSG_MEDITATE_ON_MT_ALEPH_DAILY);
    Actor_FaceEachOther(23, ACTOR_PARTY_LEADER, 2);
    Event_AskYesNo(23, 0);
    Event_End();
}

void Villager_RecallThreeYearsAgo(void)
{
    Event_Begin();
    Event_SetMessage(MSG_A_DIFFICULT_TIME_THREE_YEARS_AGO);
    Actor_FaceEachOther(24, ACTOR_PARTY_LEADER, 2);
    Event_AskYesNo(24, 0);
    Event_End();
}

void Villager_AskAboutTheTravelers(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DID_THE_TRAVELERS_MEET_THE_MAYOR);
    Actor_FaceEachOther(15, ACTOR_PARTY_LEADER, 2);
    Event_AskYesNo(15, 0);
    Event_End();
}

void Villager_ShowOffPsynergy(void)
{
    u32 i;
    s32 base5_1197;
    s32 base7_0;
    u8 *p6;

    Event_Begin();
    if (GameFlag_IsSet(0x815) != 0) {
        base5_1197 = (s32)LinkedMessage_ArentYouWorriedAboutCrossing;
        Event_SetMessage(base5_1197);
        if (GameFlag_IsSet(2) != 0) {
            bump_step(1);
        }
        if (GameFlag_IsSet(3) != 0) {
            bump_step(1);
        }
        Event_OpenMessage(17, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage((base5_1197 + 3));
        } else {
            Event_SetMessage((base5_1197 + 4));
        }
        Event_ShowMessage(17, 0);
    } else {
        p6 = *(volatile s32 *)(*(volatile s32 *)0x03001e70);
        Event_SetMessage(MSG_HAVE_I_SHOWN_YOU_MY_ABILITY);
        Actor_FaceEachOther(17, ACTOR_PARTY_LEADER, 0);
        Event_AskYesNo(17, 0);
        Event_Wait(20);
        Actor_StartRepeatedMotion(17, 2);
        Event_Wait(15);
        SceneState_ApplyPair140And0();
        base7_0 = 0;
        for (i = 0; i < 40; i++) {
            OverlayObject_UpdateOnFrameBit1(((s32 (*)())Func_020029c2_a)(17));
            Task_Wait(1);
        }
        Value2(Func_0200290c, 0x200a591, 0xc80);
        Audio_PlayCue(107);
        for (i = 0; i != 180; i++) {
            if (Value2(Func_0200290c_a, i, 10) == 0) {
                if ((1 & base7_0) != 0) {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 - 0x10000;
                } else {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 + 0x10000;
                }
                base7_0 = (base7_0 + 1);
            }
            Event_Wait(1);
        }
        Audio_PlayCue(0x121);
        Call1(Func_0200295a, 0x200a591);
        Task_Wait(1);
        FieldScene_Forward4dac();
        Actor_SetChildValue(17, 0);
        Event_Wait(40);
        Event_SetMessage(MSG_BEHOLD_THE_POWER_OF_PSYNERGY);
        Event_ShowMessage(17, 0);
    }
    Event_End();
}

void SceneDialogue_RunFlagGatedMessageStep(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x87a) != 0) {
        Event_SetMessage(MSG_ANYTHING_INTERESTING_ON_YOUR_TRIP);
        Event_OpenMessage(15, 0);
        if (Event_ChooseYesNo(0, 0) == 1) {
            Event_ShowMessage(15, 0);
        } else {
            u8 *p = *(u8 **)0x03001ebc;
            *(u16 *)(p + 472) = *(u16 *)(p + 472) + 1;
            Event_AskYesNo(15, 0);
        }
    } else if (GameFlag_IsSet(0x815) != 0) {
        Event_SetMessage(MSG_YOU_SAW_THE_WISE_ONE);
        Event_AskYesNo(11, 0);
    } else {
        Event_SetMessage(MSG_CAN_I_USE_PSYNERGY);
        Event_AskYesNo(11, 0);
    }
    Event_End();
}

void Scene_StoneFellOnTheHut(void)
{
    Event_Begin();
    Actor_SetAnimation(26, 1);
    Actor_FaceActor(26, ACTOR_PARTY_LEADER, 20);
    Actor_FaceActor(26, 21, 40);
    Event_SetMessage(MSG_THE_STONE_FELL_ON_THE_HUT);
    Func_0200273a(26, 20);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x1510000, -1, 0x1100000, 1);
    Event_Wait(20);
    Actor_RunRepeatedMotion(26, 2);
    Event_Wait(20);
    Actor_FaceActor(26, ACTOR_PARTY_LEADER, 10);
    Func_02002778(26, 40);
    Actor_EnableActionCallback(26, 2);
    Event_End();
}

void FieldScene_RunMiddleAuxiliarySequence(void)
{
    u32 i;
    s32 p8;
    u8 *rec8;
    s32 record;
    s32 v2;

    Event_Begin();
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 82, 0x2f8);
    Actor_FaceEachOther(15, ACTOR_PARTY_LEADER, 30);
    Event_SetMessage(MSG_I_HAVE_SOME_PSYNERGY_LEFT);
    Func_020027c8(15, 20);
    Value3(SceneActor_SetPairZeroAndValue, 15, 0xa000, 20);
    Actor_SetAttachedEffect(15, 0x102);
    Event_Wait(20);
    SceneState_ApplyPair140And0();
    for (i = 0; i < 40; i++) {
        OverlayObject_UpdateOnFrameBit1(Func_02002bec_a(15));
        Task_Wait(1);
    }
    Value2(Func_02002b36, 0x200a581, 0xc80);
    Value2(Func_02002b40, 0x200a5a1, 0xc80);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 10);
    rec8 = Value1(Func_02002c22, 20);
    v2 = rec8[85];
    rec8[85] = 0;
    p8 = v2;
    for (i = 0; i < 40; i++) {
        *(s32 *)(rec8 + 12) += 0x1800;
        Task_Wait(1);
    }
    rec8[85] = p8;
    Call1(Func_02002b8a, 0x200a581);
    Call1(Func_02002b90, 0x200a5a1);
    Task_Wait(1);
    Audio_PlayCue(161);
    Actor_SetChildValue(15, 0);
    Actor_SetChildValue(20, 0);
    Event_Wait(40);
    FieldScene_Forward4dac();
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 15, 30);
    Event_ShowMessage(15, 0);
    Event_End();
}

void SceneDialogue_ShowLineEB1OrEB0(void)
{
    Event_Begin();
    Actor_FaceEachOther(16, ACTOR_PARTY_LEADER, 10);
    if (GameFlag_IsSet(0x840) != 0) {
        Event_SetMessage(MSG_BE_SURE_TO_HELP_GARCIA);
        Event_ShowMessage(16, 0);
    } else {
        Event_SetMessage((s32)&LinkedMessage_JasmineWentOffWay);
        Event_ShowMessage(16, 0);
    }
    Event_End();
}

void Villager_WelcomeBack(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x302) != 0) {
        Event_SetMessage(MSG_THIS_IS_VALE);
    } else {
        Event_SetMessage(MSG_YOU_CAME_BACK_HOME);
        GameFlag_Set(0x302);
    }
    Event_ShowMessage(11, 0);
    Event_End();
}

void Scene_PsynergyStoneIsGone(void)
{
    struct Obj *p = Func_02002d3e(21);
    Event_Begin();
    p->f38 = 0x80000000;
    p->f3c = 0x80000000;
    p->f40 = 0x80000000;
    Actor_SetAnimation(21, 1);
    Actor_Stop(21);
    Actor_ShowEmote(21, 256, 40);
    p->f06 = 0xb000;
    Event_Wait(20);
    Actor_StartRepeatedMotion(21, 2);
    Event_SetMessage(MSG_THE_PSYNERGY_STONE_IS_GONE);
    Event_ShowMessageAndWait(21, 0, 40);
    Actor_FaceActor(21, ACTOR_PARTY_LEADER, 20);
    Actor_StartRepeatedMotion(21, 2);
    Event_ShowMessage(21, 0);
    GameFlag_Set(0x306);
    Actor_Stop(21);
    Task_Wait(1);
    Actor_EnableActionCallback(21, Data_0200ae34);
    Event_End();
}

void SceneState_SetWork1c0AndRun(s32 no)
{
    u8 *p;
    if (GameFlag_IsSet(0x834) != 0) {
        Func_02002f20();
    }
    p = *(u8 **)0x03001ebc;
    *(s32 *)(p + 0x1c0) = 0x100;
    *(s32 *)(p + 0x1c8) = 16;
    Event_RequestExit(no);
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value_0200beb4 as its first argument. */
void FieldScene_RunSupplementalSequenceOne(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 44, 7);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 0x117);
    Call1_02000750(SceneState_SetWork1c0AndRun, 1);
}

/* Sets step 188, then runs a pair of 6-argument setup calls for indices 0
 * and 2 sharing the same trailing four values, followed by a pair of
 * 3-argument calls sharing the same leading two arguments, and a closing
 * 1-argument call. */
void FieldScene_RunSupplementalSequenceTwo(void)
{
    Audio_PlayCue(188);
    Map_CopyCellsTo(0, 63, 51, 8, 2, 2);
    Task_Wait(10);
    Map_CopyCellsTo(2, 63, 51, 8, 2, 2);
    Task_Wait(10);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 306);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 296);
    Call1_02000780(SceneState_SetWork1c0AndRun, 2);
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value_0200beb4 as its first argument. */
void FieldScene_RunSupplementalSequenceThree(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 43, 15); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 230, 0x197);
    Call1_020007ec(SceneState_SetWork1c0AndRun, 3);
}

/* Runs four scene calls in sequence: a single-argument call, a call that
 * passes the address of Value_0200beb4 with two more values, a call that
 * passes 0, 374, and 0x1a3, and a final single-argument call. */
void FieldScene_RunSupplementalSequenceFour(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 52, 18); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 374, 0x1a3); /* object_id 0, x 374, z 0x1a3 */
    Call1_0200081c(SceneState_SetWork1c0AndRun, 4);
}

/* Runs a fixed sequence of four scripted calls: one keyed off Value_0200beb4
 * with two small numeric arguments, one with a 0x222 argument, and two plain
 * single-argument calls. */
void FieldScene_RunSupplementalSequenceFive(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 41, 32); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 200, 0x222);
    Call1_0200081c(SceneState_SetWork1c0AndRun, 5);
}

/* Runs four scripted calls with fixed literal arguments: a single-argument
 * call, a 3-argument call whose first argument is the address of
 * Value_0200beb4, another 3-argument call, and a closing single-argument
 * call. */
void FieldScene_RunSupplementalSequenceSix(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 35, 36); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 0x263); /* object_id 0, x 102, z 611 */
    Call1_0200081c(SceneState_SetWork1c0AndRun, 6);
}

/* Runs four scripted scene calls in sequence, passing a byte's address and a
 * handful of small immediate constants to each. */
void FieldScene_RunSupplementalSequenceSeven(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 51, 39); /* main:08009178 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 358, 0x29e);
    Call1_0200081c(SceneState_SetWork1c0AndRun, 7);
}

void FieldScene_RunStep7BThen8(void)
{
    Audio_PlayCue(123);
    SceneState_SetWork1c0AndRun(8);
}

void SceneState_ApplyFlag815Branch(void)
{
    if (GameFlag_IsSet(0x815) != 0) {
        Audio_PlayCue(123);
        SceneState_SetWork1c0AndRun(10);
    }
}

void SceneState_ApplyFlag90b(void)
{
    GameFlag_Set(0x90b);
}

void SceneState_ApplyFlag90c(void)
{
    GameFlag_Set(0x90c);
}

void SceneState_ApplyFlag90d(void)
{
    GameFlag_Set(0x90d);
}

/* Shows the next line of dialogue, then holds the scene for a moment. */
void Event_SayThenWait(s32 speaker, s32 frames)
{
    Event_ShowMessage(speaker, 0);
    Event_Wait(frames);
}

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Actor_FaceDirection(a, b, 0);
    Event_Wait(c);
}

void OverlayObject_UpdateOnFrameBit1(s32 obj)
{
    if ((*(volatile s32 *)0x03001e40 & 2) != 0) {
        Object_SetPartPalettes(obj, 7);
    } else {
        Object_SetPartPalettes(obj, 0);
    }
    if ((*(volatile s32 *)0x03001e40 & 15) == 0) {
        Func_02004768(obj);
    }
}

void SceneEffect_UpdateByFrameBits(s32 no)
{
    volatile s32 *p = (volatile s32 *)0x03001e40;
    if ((*p & 1) != 0) {
        s32 t = Func_02004930((u32)*p >> 1, 6);
        Object_SetPartPalettes(no, t);
    }
    if ((*p & 15) == 0) {
        Func_020047a4(no);
    }
}

void SceneEffect_UpdateByFrameBit(s32 no)
{
    volatile s32 *p = (volatile s32 *)0x03001e40;
    if ((*p & 1) != 0) {
        s32 t = Func_0200496c((u32)*p >> 1, 6);
        Object_SetPartPalettes(no, t);
    }
}

void SceneEffect_AnimateVerticalPositive(struct SceneVerticalEffect *effect)
{
    struct VerticalEffectAnchor *anchor;
    s32 frame;
    s32 amplitude;
    s32 offset;

    anchor = effect->anchor;
    effect->frame = effect->frame + 1;
    frame = (s16)effect->frame;
    if (frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }
    amplitude = Math_Sin(frame << 10);
    effect->amplitude_x = amplitude;
    effect->amplitude_y = amplitude;
    effect->x = anchor->x;
    offset = 0x10000;
    effect->y = effect->y + offset;
    offset = offset - amplitude;
    effect->z = anchor->z + (offset * 4 + offset) + 0x80000;
}

void SceneEffect_AnimateVerticalNegative(struct SceneVerticalEffect *effect)
{
    struct VerticalEffectAnchor *anchor;
    s32 frame;
    s32 amplitude;
    s32 offset;

    anchor = effect->anchor;
    effect->frame = effect->frame + 1;
    frame = (s16)effect->frame;
    if (frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }
    amplitude = Math_Sin(frame << 10);
    effect->amplitude_x = amplitude;
    effect->amplitude_y = -amplitude;
    effect->x = anchor->x;
    offset = 0x10000;
    effect->y = effect->y + offset;
    offset = offset - amplitude;
    effect->z = anchor->z - (offset * 4 + offset) + 0x100000;
}

void SceneState_ApplyPair140And0(void)
{
    Psynergy_Begin(140, 0);
}

void FieldScene_Forward4dac(void)
{
    Func_02004dac();
}

void FieldScene_RunStep15(void)
{
    Func_02004c4a(15);
    Func_020048be();
}

void FieldScene_RunStep17(void)
{
    Func_02004c5a(17);
    Func_020048ce();
}

void FieldScene_RunStep20(void)
{
    Func_02004c6a(20);
    Func_0200491a();
}

void SceneState_SetValues352_365_2116_2117_40(void)
{
    GameFlag_Set(352);
    GameFlag_Set(0x16d);
    GameFlag_Set(0x844);
    GameFlag_Set(0x845);
    Event_RequestExit(40);
}
