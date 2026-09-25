#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/* Branching dialogue and actor presentation sequences. */

void Func_02005b7e();    /* main 0x080091e0 */
void Scene_WaitFrames();    /* main 0x080000c0 */
s32 Func_02005c00();     /* main 0x0808a080, record lookup */
void Scene_PauseScript();    /* main 0x0808a010 */
void Func_02005d5c();    /* main 0x0808a300 */
void Func_02005d82();    /* main 0x0808a360 */
void Func_02005d8e();    /* main 0x0808a370 */
void Value_0200c488();   /* main 0x0808a148 */
void Func_02005ed0();    /* main 0x0808a178 */
void Func_020063a2();    /* main 0x0808a0b8 */

/* FAKEMATCH: Inline calls retain the original argument evaluation order. */

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The work-record cells are written as a union, not as bare halfwords: the
 * halfword store and the scene-pointer read that follows are only ordered
 * against each other when they can alias. */
union WorkCell {
    s32 w;
    u16 h;
};

static __inline__ void SetHalf(u16 *at, s32 value)
{
    ((union WorkCell *)at)->h = value;
}

extern u8 *Data_03001ebc[];
extern s32 Data_03001ae8;

void Func_020063fe();
s32 Func_020064bc();
s32 Func_0200650e();
s32 Func_02006518();
void Func_0200651a();
void Func_02006524();
s32 Func_02006562();
s32 Func_0200656a();
void Func_0200657e();
void Func_0200660e();
void Func_0200661c();
void Func_0200662a();
void Func_02006634();
void Func_020067ac();
void Func_02006840();
s32 Func_02006e16();
s32 Func_020077a2();
s32 Func_020077de();
s32 Func_020078d6();
s32 Func_0200790a();
s32 Func_0200791e();
s32 Func_02007928();
void Func_02007f9c();
void Func_02007fa4();
void Func_02007fac();
void Func_02007fb4();
s16 *Func_02008090();
void Func_02008096();
void Func_0200809e();
void Func_020080a6();
void Func_020080ae();
void Func_020080ca();
s32 Func_020080ea();
s16 *Func_020080f0();
void Func_020080fa();
s32 Func_02008108();
void Func_02008120();
void Scene_UpdateSlot();
void Func_0200812a();
s32 Func_0200814a();
s32 Func_02008182();
s32 Func_0200824c();

/* FAKEMATCH: Call scopes retain the argument lifetimes of the exact build. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void AdvanceMessage(s32 amount)
{
    u8 *scene = *(u8 **)0x03001ebc;

    *(u16 *)(scene + 0x1d8) = (u16)(*(u16 *)(scene + 0x1d8) + amount);
}
static __inline__ void AdvanceMessageAt(
    u8 **scene_address, s32 amount)
{
    u8 *scene = *scene_address;

    *(u16 *)(scene + 0x1d8) = (u16)(*(u16 *)(scene + 0x1d8) + amount);
}

static __inline__ void AdvanceMessageCursor(s32 amount)
{
    AdvanceMessageAt((u8 **)Data_03001ebc, amount);
}

/* Three identical tails: an optional record whose signed halfwords at +10 and
 * +18 are forwarded to a per-channel import. */
static __inline__ void follow_record(
    s16 *record, s32 channel, void (*apply)())
{
    if (record != 0)
        apply(channel, record[5], record[9]);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

extern u8 Value_00002233;

/* Supplemental scene sequence of the resource_3b8 overlay. */

void Func_020081ee();
void Func_02008356();

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

extern u8 Value_0000008b;
extern u8 Data_0200ca1c[];
extern u8 Data_0200c614[];
extern u8 Data_0200ca7c[];
extern u8 Data_0200ca8c[];
extern u8 Data_0200cb3c[];
extern u8 Data_0200ce6c[];
extern u8 Data_0200cd64[];
extern u8 Data_0200cb84[];

/* Contiguous unnamed leaf-owner run for resource_3b8. */

extern u8 Value_00001ff1;
extern u8 Value_00002006;
extern u8 Value_00000105;
extern s16 Data_02000240[];
extern u8 Value_0000008b;
extern u8 Data_0200dad8[];
extern u8 Data_0200da48[];
extern u8 Data_0200d9e8[];
extern u8 Data_0200d688[];
extern u8 Data_0200d394[];
extern u8 Data_0200d004[];

/* Contiguous unnamed leaf-owner run for resource_3b8. */

extern u8 Value_00001ff8;
extern u8 Data_0200c570[];

void Func_02004630();
void Func_0200550e();

/* Primary effect sequence of the resource_3b8 overlay: two hundred and fifty scene calls with constant arguments. */

static __inline__ void Call1_scene_effect_sequence_head(void (*f)(), s32 a0)
{
    void Func_020047bc();

    f(a0);
}

static __inline__ s32 Value1_scene_effect_sequence_head(s32 (*f)(), s32 a0)
{
    void Func_020047bc();

    return f(a0);
}

static __inline__ void Call3_scene_effect_sequence_head(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Func_020047bc();

    f(a0, a1, a2);
}

extern u8 Value_00002280[];
void Func_02004a62();
s32 Func_02004a84_effect();
s32 Func_02004a90();
s32 Func_02004ab6();
s32 Func_02004ac2();
s32 Func_02004ad4();
s32 Func_02004ba0();
s32 Func_02004bae();
void Func_02004c34();
void Func_02004c40();
void Func_02004d48();
s32 Func_02004dca();
s32 Func_02004e46();
s32 Func_02004e76();
void Func_02004e80();
void Func_02004eb0();
void Func_02004ee0();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

extern u8 Value_00001ffb[];
s32 Func_02004efe();
void Func_02004fca();
void Func_0200501e();
void Func_02005044();
void Func_0200509a();
void Func_020050bc();
void Func_020050cc();
void Func_02005146();
void Func_020051be();
void Func_02005326();
s32 Func_02005354();
void Func_0200538e();
s32 Func_020053b4();
void Func_020053be();
void Func_020053ee();

extern u8 LinkedMessage_YouAllCameRestUp[];
extern u8 Value_000022ab;
extern u8 Value_00002352;
extern u8 Value_00000f31;

void Func_020046a6_scene_primary_script();
void Func_020046d0_scene_primary_script();
void Func_0200475a_scene_primary_script();
void Func_02004776_scene_primary_script();
void Func_020047bc_scene_primary_script();
void Func_0200484c_scene_primary_script();
void Func_02004ab4();
void Func_020080e0();
void Func_020080fe();
void Func_02008118();
void Func_0200814c();
void Func_02008152();
void Func_02008158();

extern u8 Value_000022a8;
extern u8 Value_000022a3;

s32 Func_02004a00_dialogue_layout(s32, s32);

enum {
    /* Message 0x182 + 205. */
    ITEM_CLOAK_BALL = 205
};

enum ChoiceMessage {
    MSG_ROBIN_PEERED_INTO = 0x947,
    MSG_WE_HAVE_JUST_ENOUGH_EXTRA = 0x1ff7,
    MSG_CANT_FIGHT_BECAUSE_LITTLE_INDIGESTION = 0x2009,
    MSG_EVEN_IF_ESCAPED_BABI_PALACE = 0x2052,
    MSG_ROBIN_ID_REALLY_LIKE_THANK = 0x2239,
    MSG_BABI_WAITING_FOR_AT_COLOSSEUM = 0x223a,
    MSG_ITS_FILLED_WITH_FRESH_CLEAN = 0x29e0
};

s32 SceneData_SelectTablec614ByState(void)
{
    if (gGameState.scene == (s32)&Value_0000008b) {
        return (s32)Data_0200ca1c;
    }
    return (s32)Data_0200c614;
}

s32 SceneData_GetTableca7c(void)
{
    return (s32)Data_0200ca7c;
}

s32 SceneData_GetTableca8c(void)
{
    return (s32)Data_0200ca8c;
}

s32 SceneData_SelectDataBySelectorAndFlags(void)
{

    s16 *tbl = (s16 *)Data_02000240;

    if (tbl[0xe0] == (s32)&Value_0000008b)
        return (s32)Data_0200cb3c;
    if (GameFlag_IsSet(0x950) != 0)
        return (s32)Data_0200ce6c;
    if (GameFlag_IsSet(0x962) != 0)
        return (s32)Data_0200cd64;
    return (s32)Data_0200cb84;
}

void FieldScene_RunBranchedSteps1FF1(s32 a)
{
    s32 k = (s32)&Value_00001ff1;

    Event_SetMessage(k);
    Event_OpenMessage(a, 0);
    if (Event_ChooseYesNo(0, 0) == 0)
        Event_SetMessage(k + 1);
    else
        Event_SetMessage(k + 2);
    Event_ShowMessage(a, 0);
}

void RunOpeningAuxiliarySequence(s32 a)
{
    void Func_020047bc();

    u8 *ret;
    s16 v;
    s32 c;
    s32 t;

    ret = Actor_Get(ACTOR_PARTY_LEADER);
    v = (*(u16 *)(ret + 6) + 0x2000) & 0xc000;
    Event_Begin();
    Func_02004630();
    if (Value1_scene_effect_sequence_head(Engine_GameFlagIsSet, 512) == 0) {
        Call1_scene_effect_sequence_head(Engine_GameFlagSet, 512);
        GameFlag_Clear(0x969);
        Event_SetMessage(MSG_WE_HAVE_JUST_ENOUGH_EXTRA);
        Event_ShowMessage(a, 0);
        Event_Wait(10);
        t = v << 16;
        c = 0x4000;
        if (t == (0x4000 << 16)) {
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 40, 104);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        }
        Call3_scene_effect_sequence_head(Engine_ActorSetSpeed, a, 0x10000, 0x8000);
        Call3_scene_effect_sequence_head(Engine_ActorWalkByAndWait, a, 0, -48);
        Actor_WalkByAndWait(a, 64, 0);
        Actor_FaceDirection(a, c, 0);
    } else {
        Call1_scene_effect_sequence_head(Engine_GameFlagClear, 512);
        GameFlag_Set(0x969);
        Call3_scene_effect_sequence_head(Engine_ActorFaceDirection, a, 0x4000, 0);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Event_Wait(20);
        c = (s32)&Value_00001ff8;
        Event_SetMessage(c);
        Event_OpenMessage(a, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage(c + 1);
            Event_ShowMessage(a, 0);
        } else {
            Event_SetMessage(c + 2);
            Event_ShowMessage(a, 0);
        }
        Event_Wait(10);
        Actor_SetAnimationAndWait(a, 3);
        Event_Wait(20);
        Call3_scene_effect_sequence_head(Engine_ActorWalkByAndWait, a, -64, 0);
        Actor_WalkByAndWait(a, 0, 48);
    }
    Event_End();
}

void FieldScene_RunScene3b8_02000264(s32 a0)
{
    u32 i;
    s32 record;
    s32 base6_2241;

    Event_Begin();
    Func_02004776_scene_primary_script();
    if (GameFlag_IsSet(0x966) == 0) {
        GameFlag_Set(0x966);
        GameFlag_Set(0x967);
        Actor_FaceDirection(a0, 0x4000, 0);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Event_Wait(20);
        base6_2241 = (s32)LinkedMessage_YouAllCameRestUp;
        Event_SetMessage(base6_2241);
        Event_OpenMessage(a0, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Func_020046a6_scene_primary_script(10);
            Event_SetMessage((base6_2241 + 1));
        } else {
            Event_SetMessage((base6_2241 + 2));
        }
        Event_ShowMessage(a0, 0);
        Event_Wait(10);
        Func_0200475a_scene_primary_script(a0, 3);
        Func_020046d0_scene_primary_script(20);
        Actor_SetSpeed(a0, 0x10000, 0x8000);
        Actor_WalkByAndWait(a0, -64, 0);
        Func_0200484c_scene_primary_script(a0, 0, 48);
    } else {
        Call1(Func_020047bc_scene_primary_script, 0x2245);
        Event_OpenMessage(a0, 0);
    }
    Event_End();
}

void FieldScene_RunBranchedSteps2006(s32 a)
{
    s32 k = (s32)&Value_00002006;

    Event_SetMessage(k);
    Event_OpenMessage(a, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Actor_ShowEmote(a, 0x102, 0x28);
        Event_SetMessage(k + 1);
    } else {
        Event_Wait(10);
        Actor_ShowEmote(a, (s32)&Value_00000105, 0x28);
        Event_SetMessage(k + 2);
    }
    Event_ShowMessage(a, 0);
}

void RunMiddleAuxiliarySequence(s32 a)
{
    u8 *obj;
    u8 *q;

    obj = Func_020047bc();
    Event_Begin();
    q = Data_0200c570;
    Actor_EnableActionCallback(a, q);
    Event_SetMessage(MSG_CANT_FIGHT_BECAUSE_LITTLE_INDIGESTION);
    Event_ShowMessage(a, 0);
    Actor_Stop(a);
    *(s32 *)(obj + 28) = 0x10000;
    *(s32 *)(obj + 24) = 0x10000;
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(60);
    Event_ShowMessage(a, 0);
    Event_Wait(20);
    Actor_ShowEmote(a, 258, 60);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_EnableActionCallback(a, q);
    Event_ShowMessage(a, 0);
    Call3_scene_effect_sequence_head(Engine_ActorFaceDirection, a, 0xe000, 0);
    Event_Wait(10);
    *(s32 *)(obj + 28) = 0x10000;
    *(s32 *)(obj + 24) = 0x10000;
    Actor_EnableActionCallback(a, q);
    Event_End();
}

void FieldScene_RunScene3b8_0200049c(s32 unused0, s32 a1)
{
    Event_Begin();
    Event_SetMessage(MSG_EVEN_IF_ESCAPED_BABI_PALACE);
    Event_ShowMessage(a1, 0);
    if (GameFlag_IsSet(0x968) == 0) {
        GameFlag_Set(0x968);
        Psynergy_Cancel();
        Event_Wait(50);
        Actor_ShowEmote(a1, 0x100, 70);
        Actor_FaceActor(a1, ACTOR_PARTY_LEADER, 40);
        Event_ShowMessage(a1, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(a1, 4);
        Event_Wait(20);
        Event_ShowMessage(a1, 0);
        Actor_FaceDirection(a1, 0x8000, 0);
    }
    Event_End();
}

void SceneDialogue_ShowMessage22a8Branch(s32 a)
{
    s32 k = (s32)&Value_000022a8;

    Event_SetMessage(k);
    Event_OpenMessage(a, 0);
    if (Event_ChooseYesNo(0, 0) == 0)
        Event_SetMessage(k + 1);
    else
        Event_SetMessage(k + 2);
    Func_02004a00_dialogue_layout(a, 0);
}

void SceneDialogue_RunChoiceSequence22ab(s32 no)
{
    s32 msg = (s32)&Value_000022ab;

    Event_SetMessage(msg);
    Event_OpenMessage(no, 0);
    if (Event_ChooseYesNo(0, 0) == 0)
        Event_SetMessage(msg + 1);
    else
        Event_SetMessage(msg + 2);
    Event_ShowMessage(no, 0);
}

void SceneDialogue_RunChoiceSequence2352(void)
{
    s32 msg;

    Event_Begin();
    Func_02004ab4();
    msg = (s32)&Value_00002352;
    Event_SetMessage(msg);
    Event_ShowMessage(-1, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 30);
    Event_OpenMessage(14, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        Event_SetMessage(msg + 2);
        Event_ShowMessage(14, 0);
    } else {
        Event_Wait(20);
        Event_SetMessage(msg + 3);
        Event_ShowMessage(14, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Event_Wait(30);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
        Event_Wait(30);
        Actor_SetPosition(16, 0, 0);
        Item_ShowFound(ITEM_CLOAK_BALL, 3);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
        Party_GiveItem(ITEM_CLOAK_BALL, 0);
        GameFlag_Set((s32)&Value_00000f31);
    }
}

void FieldScene_RunScene3b8SequenceB(void)
{
    u32 i;
    s32 record;
    s32 v5;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)&Value_00002280);
    v5 = 0;
    *(u8 *)(Func_02004a84_effect(0) + 84) = v5;
    *(u8 *)(Func_02004a90(10) + 84) = v5;
    Engine_TaskWait(1);
    *(volatile u16 *)0x04000000 = 0x1140;
    Call2(Engine_EventShowMessage, -1, 0);
    *(volatile u16 *)0x04000000 = 0x140;
    v5 = 1;
    *(u8 *)(Func_02004ab6(0) + 84) = v5;
    *(u8 *)(Func_02004ac2(10) + 84) = v5;
    Engine_ActorSetAnimation(0, 31);
    record = Func_02004ad4(0);
    Func_02004a62(record, 0);
    Call3(Engine_ActorSetPosition, 1, 0x780000, 0x680000);
    Call3(Engine_ActorSetPosition, 3, 0x680000, 0x500000);
    Call3(Engine_ActorSetPosition, 2, 0x780000, 0x780000);
    Engine_ActorFaceDirection(1, 0, 0);
    Engine_ActorFaceDirection(3, 0, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xe000, 0);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c8) = 60;
    Func_02004c34();
    Func_02004c40();
    Engine_EventWait(20);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c8) = 24;
    Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    Actor_WalkByAndWait(ACTOR_MIA, 16, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x2000, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(3, 2);
    Engine_EventWait(30);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    record = Func_02004ba0(0);
    *(s32 *)(record + 16) += -0x30000;
    record = Value1(Func_02004bae, 0);
    *(s32 *)(record + 64) += -0x30000;
    Engine_ActorSetAnimation(0, 32);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(0, 34);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(0, 33);
    Engine_EventWait(50);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 0, 0x105, 60);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 1, 0x102, 60);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(1, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 0, 0x102, 80);
    Call3(Engine_ActorShowEmote, 2, 0x106, 60);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(2, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 1, 0x4000, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(45);
    Engine_ActorFaceDirection(1, 0, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xe000, 0);
    Engine_EventWait(30);
    Value2(Engine_EventOpenMessage, 1, 0);
    if (Value2(Engine_EventChooseYesNo, -1, 0) != 0) {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 34);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(1, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(1, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 33);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(1, 3);
        Engine_EventWait(20);
        Event_ShowMessage(ACTOR_GERALD, 0);
        AdvanceMessageCursor(1);
    } else {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 33);
        AdvanceMessageCursor(2);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(1, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(1, 0);
    }
    Engine_EventWait(10);
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3(Engine_ActorWalkByAndWait, 1, -16, 0);
    Engine_ActorFaceDirection(1, 0, 0);
    Engine_EventWait(35);
    Engine_ActorJump(0, 6, 0);
    Call3(Engine_ActorSetSpeed, 0, 0x1e666, 0xf333);
    Call3(Engine_ActorWalkByAndWait, 0, -32, 0);
    record = Func_02004dca(0);
    Func_02004d48(record, 1);
    Call3(Engine_ActorFaceDirection, 3, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(0, 3);
    Event_Wait(30);
    Engine_ActorSetAnimation(2, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(30);
    Call3(Engine_ActorSetSpeed, 1, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 3, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 2, 0x13333, 0x9999);
    Engine_ActorSetAnimation(1, 2);
    record = Value1(Func_02004e46, 0);
    if (record != 0) {
        Func_02004e80(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(1);
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetAnimation(3, 2);
    record = Value1(Func_02004e76, 0);
    if (record != 0) {
        Func_02004eb0(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(3);
    Engine_ActorSetPosition(3, 0, 0);
    Engine_ActorSetAnimation(2, 2);
    record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    if (record != 0) {
        Func_02004ee0(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(2);
    Engine_ActorSetPosition(2, 0, 0);
    ((void (*)())Engine_EventWait)(10);
    Engine_EventEnd();
}

void FieldScene_RunScene3b8SequenceA(void)
{
    s32 record;
    s16 dir;
    u16 facing;

    record = Func_02004efe(0);
    dir = (*(u16 *)(record + 6) + 0x2000) & -0x4000;
    Engine_EventBegin();
    Func_0200501e();
    Engine_EventSetMessage((s32)&Value_00001ffb);
    Value2(Engine_EventOpenMessage, -1, 0);
    if (Value2(Engine_EventChooseYesNo, 0, 0) != 0) {
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x96a) != 0) {
            Engine_EventWait(20);
            Func_0200509a(0);
            goto L_02000fe2;
        }
        Engine_EventWait(20);
        facing = dir;
        if (facing == 0) {
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 120);
        }
        if (facing == 0x8000) {
            Engine_ActorWalkToAndWait(0, 240, 120);
        }
        Engine_ActorWalkToAndWait(0, 184, 120);
        Engine_ActorFaceDirection(0, 0, 0);
        Engine_EventWait(10);
        Func_020050cc(1, 16, 0, 0x8000);
        Func_02004fca(1);
        Engine_EventWait(10);
        Engine_EventShowMessage(1, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 3);
        Event_Wait(20);
        Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
        Actor_WalkByAndWait(ACTOR_GERALD, 40, 0);
        Call3(Engine_ActorWalkByAndWait, 1, 0, -32);
        Call3(Engine_ActorFaceDirection, 1, 0x6000, 0);
        Engine_EventWait(20);
        Engine_ActorFaceDirection(0, 0x8000, 0);
        Engine_EventWait(20);
        Call4(Func_02005146, 2, -16, 0, 0);
        Func_02005044(2);
        Engine_EventWait(10);
        Engine_EventShowMessage(2, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_EventWait(20);
        Call3(Engine_ActorSetSpeed, 2, 0x10000, 0x8000);
        Call3(Engine_ActorWalkByAndWait, 2, -40, 0);
        Engine_ActorWalkByAndWait(2, 0, 40);
        Call3(Engine_ActorFaceDirection, 2, 0xe000, 0);
        Engine_EventWait(20);
        Engine_ActorFaceDirection(0, 0, 0);
        Engine_EventWait(20);
        Func_020051be(3, 16, 0, 0x8000);
        Func_020050bc(3);
        Engine_EventWait(10);
        Engine_EventShowMessage(3, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_EventWait(20);
        Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
        Engine_ActorWalkByAndWait(3, 40, 0);
        Engine_ActorWalkByAndWait(3, 0, 40);
        Call3(Engine_ActorFaceDirection, 3, 0xa000, 0);
        Engine_EventWait(20);
        Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
        Call3(Engine_ActorWalkByAndWait, 0, -56, 0);
        Call3(Engine_ActorWalkByAndWait, 0, 0, -32);
        Call3(Engine_ActorFaceDirection, 0, 0x2000, 0);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(3, 3);
        Event_Wait(10);
        Engine_EventShowMessage(3, 0);
        Engine_ActorSetAnimationAndWait(1, 3);
        ((void (*)())Engine_EventWait)(10);
        if (Value1(Engine_GameFlagIsSet, 0x96a) == 0) {
            Engine_EventShowMessage(1, 0);
        } else {
            AdvanceMessageCursor(1);
        }
        Engine_ActorSetAnimationAndWait(2, 3);
        Engine_EventWait(10);
        if (GameFlag_IsSet(0x96a) == 0) {
            Engine_EventShowMessage(2, 0);
        } else {
            AdvanceMessageCursor(1);
        }
        Engine_EventWait(20);
        Engine_ActorFaceDirection(0, 0, 0);
        Call3(Engine_ActorFaceDirection, 1, 0x8000, 0);
        Call3(Engine_ActorFaceDirection, 3, 0x8000, 0);
        Engine_ActorFaceDirection(2, 0, 0);
        Func_02005326(0);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0x6000, 0);
        Call3(Engine_ActorFaceDirection, 3, 0xa000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0xe000, 0);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(1, 3);
        Engine_EventWait(10);
        Engine_EventShowMessage(1, 0);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        ((void (*)())Engine_EventWait)(10);
        if (Value1(Engine_GameFlagIsSet, 0x96a) == 0) {
            Engine_EventShowMessage(2, 0);
        } else {
            AdvanceMessageCursor(1);
        }
        Engine_ActorSetAnimationAndWait(3, 3);
        Engine_EventWait(10);
        if (GameFlag_IsSet(0x96a) == 0) {
            Event_ShowMessage(ACTOR_MIA, 0);
        } else {
            AdvanceMessageCursor(1);
        }
        Engine_ActorWalkByAndWait(1, 0, 32);
        Call3(Engine_ActorWalkBy, 1, -112, 0);
        Call3(Engine_ActorWalkByAndWait, 3, 0, -40);
        Actor_WalkBy(ACTOR_MIA, -112, 0);
        Event_Wait(50);
        Call3(Engine_ActorWalkByAndWait, 2, 0, -24);
        Actor_WaitForMove(ACTOR_GERALD);
        Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
        Call3(Engine_ActorWalkByAndWait, 1, 0, -16);
        Engine_ActorWaitForMove(3);
        Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
        Engine_EventWait(20);
        Event_ShowMessage(ACTOR_GERALD, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_EventWait(20);
        Call3(Engine_ActorSetSpeed, 1, 0x13333, 0x9999);
        Call3(Engine_ActorSetSpeed, 2, 0x13333, 0x9999);
        Call3(Engine_ActorSetSpeed, 3, 0x13333, 0x9999);
        Engine_ActorSetAnimation(1, 2);
        record = Value1(Func_02005354, 0);
        if (record != 0) {
            Func_0200538e(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(ACTOR_GERALD);
        Engine_ActorSetPosition(1, 0, 0);
        Engine_ActorSetAnimation(3, 2);
        record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
        if (record != 0) {
            Func_020053be(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(ACTOR_MIA);
        Engine_ActorSetPosition(3, 0, 0);
        Engine_ActorSetAnimation(2, 2);
        record = Value1(Func_020053b4, 0);
        if (record != 0) {
            Func_020053ee(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(2);
        Engine_ActorSetPosition(2, 0, 0);
        GameFlag_Set(0x96a);
    }
    Engine_EventEnd();
    L_02000fe2:;
}

void Func_02000ff8(void)
{
    Call1_scene_effect_sequence_head(Engine_GameFlagSet, 2411);
    Call0(Engine_EventBegin);
    Call0(Func_0200550e);
    Call1_scene_effect_sequence_head(Engine_EventSetMessage, 8225);
    Call3_scene_effect_sequence_head(Engine_ActorWalkToAndWait, 0, 520, 424);
    Call3_scene_effect_sequence_head(Engine_ActorFaceDirection, 0, 57344, 0);
    Camera_MoveTo(36700160, -1, 24117248, 1);
    Call0(Engine_CameraWaitForMove);
    Call3_scene_effect_sequence_head(Engine_ActorSetSpeed, 20, 65536, 32768);
    Call3_scene_effect_sequence_head(Engine_ActorWalkByAndWait, 20, 40, 0);
    Call3_scene_effect_sequence_head(Engine_ActorWalkToAndWait, 20, 584, 360);
    Event_ShowMessage(21, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 20, 257, 40);
    Call3_scene_effect_sequence_head(Engine_ActorFaceDirection, 20, 32768, 0);
    Call3_scene_effect_sequence_head(Engine_ActorSetSpeed, 21, 131072, 65536);
    Call3_scene_effect_sequence_head(Engine_ActorSetSpeed, 22, 131072, 65536);
    Call3_scene_effect_sequence_head(Engine_ActorWalkTo, 21, 528, 352);
    Call3_scene_effect_sequence_head(Engine_ActorWalkToAndWait, 22, 528, 368);
    Call1_scene_effect_sequence_head(Engine_TaskWait, 3);
    Actor_SetAnimation(21, 1);
    Call1_scene_effect_sequence_head(Engine_EventWait, 30);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(22, 3);
    Call1_scene_effect_sequence_head(Engine_EventWait, 30);
    Event_ShowMessage(22, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_RunRepeatedMotion(20, 2);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Call3_scene_effect_sequence_head(Engine_ActorWalkByAndWait, 20, -16, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimation(21, 4);
    Actor_SetAnimationAndWait(22, 4);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 20, 261, 40);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 21, 258, 40);
    Call3_scene_effect_sequence_head(Engine_ActorSetSpeed, 21, 65536, 32768);
    Call3_scene_effect_sequence_head(Engine_ActorWalkByAndWait, 21, 8, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(21, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 20, 257, 80);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 22, 258, 40);
    Call3_scene_effect_sequence_head(Engine_ActorSetSpeed, 22, 65536, 32768);
    Call3_scene_effect_sequence_head(Engine_ActorWalkByAndWait, 22, 8, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(22, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(20, 4);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimation(21, 3);
    Actor_SetAnimationAndWait(22, 3);
    Call1_scene_effect_sequence_head(Engine_EventWait, 30);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_RunRepeatedMotion(21, 2);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(21, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(22, 4);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(22, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 20, 261, 40);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 21, 257, 40);
    Event_ShowMessage(21, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 20, 258, 40);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_RunRepeatedMotion(22, 2);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(22, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(20, 3);
    Call1_scene_effect_sequence_head(Engine_EventWait, 30);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_RunRepeatedMotion(21, 2);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(21, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 20, 258, 40);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Call3_scene_effect_sequence_head(Engine_ActorFaceEachOther, 21, 22, 60);
    Call3_scene_effect_sequence_head(Engine_ActorFaceActor, 21, 20, 0);
    Call3_scene_effect_sequence_head(Engine_ActorFaceActor, 22, 20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_RunRepeatedMotion(20, 2);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 21, 256, 40);
    Event_ShowMessage(21, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(22, 3);
    Call1_scene_effect_sequence_head(Engine_EventWait, 30);
    Event_ShowMessage(22, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAttachedEffect(20, 258);
    Actor_RunRepeatedMotion(20, 2);
    Call1_scene_effect_sequence_head(Engine_EventWait, 30);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 21, 257, 40);
    Event_ShowMessage(21, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(20, 3);
    Call1_scene_effect_sequence_head(Engine_EventWait, 30);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 22, 257, 40);
    Event_ShowMessage(22, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(20, 4);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Call3_scene_effect_sequence_head(Engine_ActorFaceEachOther, 21, 22, 60);
    Call3_scene_effect_sequence_head(Engine_ActorFaceActor, 21, 20, 0);
    Call3_scene_effect_sequence_head(Engine_ActorFaceActor, 22, 20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 20, 261, 40);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_RunRepeatedMotion(21, 2);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(21, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(22, 3);
    Call1_scene_effect_sequence_head(Engine_EventWait, 30);
    Event_ShowMessage(22, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(20, 3);
    Call1_scene_effect_sequence_head(Engine_EventWait, 30);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_RunRepeatedMotion(22, 2);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(22, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 20, 258, 40);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_RunRepeatedMotion(21, 2);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(21, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(20, 4);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 22, 256, 40);
    Event_ShowMessage(22, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 20, 258, 40);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 21, 257, 40);
    Event_ShowMessage(21, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(20, 4);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Call3_scene_effect_sequence_head(Engine_ActorFaceEachOther, 21, 22, 60);
    Call3_scene_effect_sequence_head(Engine_ActorFaceActor, 21, 20, 0);
    Call3_scene_effect_sequence_head(Engine_ActorFaceActor, 22, 20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_RunRepeatedMotion(20, 2);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAttachedEffect(21, 258);
    Actor_SetAttachedEffect(22, 258);
    Actor_StartRepeatedMotion(21, 2);
    Actor_RunRepeatedMotion(22, 2);
    Call1_scene_effect_sequence_head(Engine_EventWait, 30);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Call3_scene_effect_sequence_head(Engine_ActorFaceDirection, 20, 0, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(21, 4);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(21, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(22, 4);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(22, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_RunRepeatedMotion(20, 2);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Call3_scene_effect_sequence_head(Engine_ActorFaceDirection, 20, 32768, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(21, 3);
    Call1_scene_effect_sequence_head(Engine_EventWait, 30);
    Event_ShowMessage(21, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Call3_scene_effect_sequence_head(Engine_ActorShowEmote, 20, 261, 40);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(22, 3);
    Call1_scene_effect_sequence_head(Engine_EventWait, 30);
    Event_ShowMessage(22, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(20, 3);
    Call1_scene_effect_sequence_head(Engine_EventWait, 40);
    Call3_scene_effect_sequence_head(Engine_ActorFaceEachOther, 21, 22, 60);
    Actor_SetAnimationAndWait(21, 3);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Actor_SetAnimationAndWait(22, 3);
    Call1_scene_effect_sequence_head(Engine_EventWait, 20);
    Call3_scene_effect_sequence_head(Engine_ActorFaceDirection, 22, 16384, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 40);
    Call3_scene_effect_sequence_head(Engine_ActorSetSpeed, 21, 85196, 42598);
    Call3_scene_effect_sequence_head(Engine_ActorSetSpeed, 22, 85196, 42598);
    Call3_scene_effect_sequence_head(Engine_ActorWalkBy, 21, 0, 120);
    Call3_scene_effect_sequence_head(Engine_ActorWalkByAndWait, 22, 0, 120);
    Call3_scene_effect_sequence_head(Engine_ActorSetPosition, 21, 0, 0);
    Call3_scene_effect_sequence_head(Engine_ActorSetPosition, 22, 0, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_RunRepeatedMotion(20, 2);
    Call1_scene_effect_sequence_head(Engine_EventWait, 40);
    Call3_scene_effect_sequence_head(Engine_ActorSetSpeed, 20, 52428, 26214);
    Call3_scene_effect_sequence_head(Engine_ActorWalkByAndWait, 20, -16, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 30);
    Event_ShowMessage(20, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 10);
    Actor_SetAnimationAndWait(20, 4);
    Call1_scene_effect_sequence_head(Engine_EventWait, 30);
    Call3_scene_effect_sequence_head(Engine_ActorFaceDirection, 20, 0, 0);
    Call1_scene_effect_sequence_head(Engine_EventWait, 30);
    Call3_scene_effect_sequence_head(Engine_ActorSetSpeed, 20, 65536, 32768);
    Call3_scene_effect_sequence_head(Engine_ActorWalkByAndWait, 20, 120, 0);
    Call3_scene_effect_sequence_head(Engine_ActorWalkByAndWait, 20, 60, 0);
    Call3_scene_effect_sequence_head(Engine_ActorSetPosition, 20, 0, 0);
    Call0(Engine_EventEnd);
}

void Func_020017e8(void)
{
    s32 record;
    s32 step;
    s32 pick;
    s32 state;
    u8 *work;

    Engine_EventBegin();
    Engine_EventSetMessage(0x228c);
    Engine_ActorSetAnimation(0, 31);
    record = Func_02005c00(0);
    Func_02005b7e(record, 0);
    Call3(Engine_ActorSetPosition, 1, 0x680000, 0x680000);
    Call3(Engine_ActorSetPosition, 3, 0x580000, 0x780000);
    Call3(Engine_ActorSetPosition, 2, 0x780000, 0x780000);
    Call3(Engine_ActorFaceDirection, 1, 0x4000, 0);
    Engine_ActorFaceDirection(3, 0, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
    state = (s32)Data_03001ebc[0];
    *(s32 *)(state + 0x1c0) = 0x100;
    *(s32 *)(state + 0x1c8) = 12;
    Scene_WaitFrames(1);
    Func_02005d5c(9);
    work = Data_03001ebc[4];
    SetHalf((u16 *)(work + 0x52a), 0);
    SetHalf((u16 *)(work + 0x534), 0x1f1f);
    SetHalf((u16 *)(work + 0x536), 1);
    Func_02005d82();
    Func_02005d8e();
    for (step = 1; step <= 5; step++) {
        Scene_WaitFrames(3);
        *(u16 *)(work + 0x52a) = step;
    }
    Scene_PauseScript(40);
    Engine_ActorRunRepeatedMotion(0, 2);
    Scene_PauseScript(30);
    for (step = 5; step <= 31; step++) {
        Scene_WaitFrames(3);
        *(u16 *)(work + 0x52a) = step;
    }
    SetHalf((u16 *)(work + 0x536), 31);
    state = (s32)Data_03001ebc[0];
    *(s32 *)(state + 0x1c0) = 0x209;
    *(s32 *)(state + 0x1c8) = 24;
    Scene_PauseScript(20);
    Call3(Engine_ActorShowEmote, 1, 0x100, 50);
    Value_0200c488(1, 0, 40);
    Engine_ActorRunRepeatedMotion(1, 2);
    Scene_PauseScript(30);
    Scene_PauseScript(10);
    Call3(Engine_ActorShowEmote, 2, 0x101, 40);
    Call3(Engine_ActorFaceDirection, 2, 0xe000, 0);
    Scene_PauseScript(30);
    Engine_EventShowMessage(2, 0);
    Scene_PauseScript(10);
    Engine_EventShowMessage(3, 0);
    Engine_ActorFaceDirection(3, 0xe000, 0);
    Scene_PauseScript(30);
    Engine_ActorRunRepeatedMotion(3, 2);
    Scene_PauseScript(30);
    Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    Call3(Engine_ActorWalkByAndWait, 3, 0, -40);
    Engine_ActorWalkByAndWait(3, 32, 0);
    Engine_ActorFaceDirection(3, 0x2000, 0);
    Scene_PauseScript(10);
    Engine_EventShowMessage(3, 0);
    record = Func_02005c00(0);
    *(s32 *)(record + 16) += -0x30000;
    record = Func_02005c00(0);
    *(s32 *)(record + 64) += -0x30000;
    Engine_ActorSetAnimation(0, 32);
    Scene_PauseScript(40);
    Engine_ActorSetAnimationAndWait(0, 34);
    Scene_PauseScript(30);
    Engine_ActorSetAnimation(0, 33);
    Scene_PauseScript(40);
    Engine_ActorSetAnimationAndWait(1, 4);
    Scene_PauseScript(20);
    Engine_EventShowMessage(1, 0);
    Scene_PauseScript(10);
    Call3(Engine_ActorShowEmote, 0, 0x102, 80);
    Call3(Engine_ActorShowEmote, 2, 0x100, 50);
    Value2(Func_02005ed0, 2, 0);
    if (Engine_EventChooseYesNo(-1, 0) == 0) {
        Scene_PauseScript(10);
        Engine_ActorSetAnimationAndWait(0, 33);
        Scene_PauseScript(20);
        Call3(Engine_ActorShowEmote, 1, 0x103, 40);
        Engine_ActorJump(1, 4, 13);
        Engine_ActorJump(1, 4, 30);
        Engine_EventShowMessage(1, 0);
        *(u16 *)(Data_03001ebc[0] + 0x1d8) += 1;
    } else {
        Scene_PauseScript(10);
        Engine_ActorSetAnimationAndWait(0, 34);
        Scene_PauseScript(20);
        Call3(Engine_ActorShowEmote, 1, 0x103, 40);
        Engine_ActorJump(1, 4, 13);
        Engine_ActorJump(1, 4, 30);
        *(u16 *)(Data_03001ebc[0] + 0x1d8) += 1;
        Engine_EventShowMessage(1, 0);
    }
    Scene_PauseScript(10);
    Engine_ActorFaceDirection(2, 0xa000, 0);
    Scene_PauseScript(30);
    Engine_ActorSetAnimationAndWait(2, 4);
    Scene_PauseScript(20);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Scene_PauseScript(30);
    Engine_ActorRunRepeatedMotion(1, 2);
    Scene_PauseScript(40);
    Call3(Engine_ActorFaceDirection, 1, 0xe000, 0);
    Engine_ActorFaceDirection(2, 0xe000, 0);
    Scene_PauseScript(30);
    Engine_ActorSetAnimationAndWait(1, 3);
    Scene_PauseScript(30);
    Engine_EventShowMessage(1, 0);
    Scene_PauseScript(10);
    Engine_ActorFaceDirection(3, 0x6000, 0);
    Scene_PauseScript(50);
    Engine_ActorFaceDirection(3, 0x2000, 0);
    Scene_PauseScript(35);
    Engine_ActorShowEmote(3, 0x108, 50);
    Scene_PauseScript(10);
    Engine_ActorSetAnimationAndWait(3, 3);
    Scene_PauseScript(30);
    Engine_EventShowMessage(3, 0);
    Scene_PauseScript(10);
    Engine_ActorRunRepeatedMotion(2, 2);
    Scene_PauseScript(20);
    Value2(Func_02005ed0, 2, 0);
    if (Engine_EventChooseYesNo(-1, 0) == 0) {
        Scene_PauseScript(10);
        Engine_ActorSetAnimationAndWait(0, 33);
        Scene_PauseScript(20);
        Call3(Engine_ActorShowEmote, 1, 0x107, 40);
        Engine_EventShowMessage(1, 0);
        pick = 0;
        *(u16 *)(Data_03001ebc[0] + 0x1d8) += 1;
    } else {
        Scene_PauseScript(10);
        Engine_ActorSetAnimationAndWait(0, 34);
        Scene_PauseScript(20);
        Call3(Engine_ActorShowEmote, 1, 0x107, 40);
        *(u16 *)(Data_03001ebc[0] + 0x1d8) += 1;
        pick = 1;
        Engine_EventShowMessage(1, 0);
    }
    Scene_PauseScript(10);
    Engine_ActorFaceDirection(2, 0xa000, 0);
    Scene_PauseScript(30);
    Engine_ActorSetAnimationAndWait(2, 4);
    Scene_PauseScript(20);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Scene_PauseScript(30);
    Engine_ActorRunRepeatedMotion(1, 2);
    Scene_PauseScript(40);
    Call3(Engine_ActorFaceDirection, 1, 0xe000, 0);
    Engine_ActorFaceDirection(2, 0xe000, 0);
    Scene_PauseScript(30);
    Engine_ActorSetAnimationAndWait(1, 3);
    Scene_PauseScript(30);
    Engine_EventShowMessage(1, 0);
    Scene_PauseScript(10);
    Engine_ActorFaceDirection(3, 0x6000, 0);
    Scene_PauseScript(50);
    Engine_ActorFaceDirection(3, 0x2000, 0);
    Scene_PauseScript(35);
    Engine_ActorShowEmote(3, 0x108, 50);
    Scene_PauseScript(10);
    Engine_ActorSetAnimationAndWait(3, 3);
    Scene_PauseScript(30);
    Engine_EventShowMessage(3, 0);
    if (pick == 0) {
        Scene_PauseScript(10);
        Engine_ActorSetAnimationAndWait(2, 3);
        Scene_PauseScript(30);
        Engine_EventShowMessage(2, 0);
        Scene_PauseScript(10);
        Engine_ActorRunRepeatedMotion(1, 2);
        Scene_PauseScript(30);
        Engine_EventShowMessage(1, 0);
        *(u16 *)(Data_03001ebc[0] + 0x1d8) += 2;
    } else {
        *(u16 *)(Data_03001ebc[0] + 0x1d8) += 2;
        Scene_PauseScript(10);
        Engine_ActorSetAnimationAndWait(2, 4);
        Scene_PauseScript(20);
        Engine_EventShowMessage(2, 0);
        Scene_PauseScript(10);
        Engine_ActorRunRepeatedMotion(1, 2);
        Scene_PauseScript(20);
        Engine_EventShowMessage(1, 0);
    }
    Scene_PauseScript(10);
    Engine_ActorShowEmote(0, 0x102, 60);
    Scene_PauseScript(10);
    Engine_ActorSetAnimationAndWait(1, 3);
    Scene_PauseScript(30);
    Engine_EventShowMessage(1, 0);
    Scene_PauseScript(20);
    Engine_ActorFaceEachOther(2, 3, 40);
    Call3(Engine_ActorFaceDirection, 3, 0x2000, 0);
    Engine_ActorFaceDirection(2, 0xe000, 0);
    Scene_PauseScript(30);
    while ((Data_03001ae8 & 240) == 0) {
        Scene_WaitFrames(1);
    }
    Engine_ActorJump(0, 6, 0);
    Call3(Engine_ActorSetSpeed, 0, 0x1e666, 0xf333);
    Engine_ActorWalkByAndWait(0, -32, -8);
    record = Func_02005c00(0);
    Func_02005b7e(record, 1);
    Scene_PauseScript(20);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x4000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Scene_PauseScript(20);
    Scene_PauseScript(10);
    Engine_ActorRunRepeatedMotion(3, 2);
    Scene_PauseScript(20);
    Engine_EventShowMessage(3, 0);
    Scene_PauseScript(10);
    Engine_ActorRunRepeatedMotion(1, 2);
    Scene_PauseScript(20);
    Engine_EventShowMessage(1, 0);
    Scene_PauseScript(20);
    Engine_ActorSetAnimationAndWait(2, 3);
    Scene_PauseScript(30);
    Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
    Engine_ActorFaceDirection(1, 0x2000, 0);
    Scene_PauseScript(30);
    Engine_EventShowMessage(2, 0);
    Scene_PauseScript(20);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(3, 3);
    Scene_PauseScript(30);
    Call3(Engine_ActorSetSpeed, 1, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 3, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 2, 0x13333, 0x9999);
    Engine_ActorSetAnimation(1, 2);
    record = Func_02005c00(0);
    if (record != 0) {
        Func_020063a2(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(1);
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetAnimation(3, 2);
    record = Func_02005c00(0);
    if (record != 0) {
        Func_020063a2(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(3);
    Engine_ActorSetPosition(3, 0, 0);
    Engine_ActorSetAnimation(2, 2);
    record = Func_02005c00(0);
    if (record != 0) {
        Func_020063a2(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(2);
    Engine_ActorSetPosition(2, 0, 0);
    Scene_PauseScript(10);
    Engine_EventEnd();
}

void FieldScene_RunMainCutsceneSequence(void)
{
    s32 carry;
    s16 *position;

    carry = Func_02006562(30);
    Func_020063fe(carry);
    carry = Value1(Func_020064bc, 0x22c4);
    carry = Func_02006518(carry);
    Func_02006524(carry);
    carry = Value4(Func_0200650e, 0xd80000, -1, 0x2e00000, 1);
    Func_0200651a(carry);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorSetSpeed, 14, 0xcccc, 0x6666);
    Engine_ActorWalkByAndWait(14, 0, 16);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    carry = Func_0200656a(0, 1);
    Func_0200657e(carry);
    Engine_EventWait(40);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Value3(Engine_ActorWalkToAndWait, 0, 208, 0x2f8);
    Engine_EventWait(10);
    Camera_MoveTo(0xd80000, -1, 0x2e00000, 1);
    Call4(Func_0200660e, 1, -16, 16, 0xc000);
    Call4(Func_0200661c, 3, 0, 24, 0xc000);
    Value4(Func_0200662a, 2, 16, 16, 0xc000);
    Engine_ActorWaitForMove(1);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);

    if (Value1(Engine_GameFlagIsSet, 0x951) != 0) {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(20, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(20, 0);
        Engine_EventWait(10);
        Actor_FaceDirection(14, 0xa000, 0);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(14, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(14, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(20, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(20, 0);
        Call3(Engine_ActorFaceDirection, 20, 0xc000, 0);
        Engine_EventWait(20);
        Call3(Engine_ActorSetSpeed, 20, 0x10000, 0x8000);
        Value3(Engine_ActorWalkByAndWait, 20, 0, -16);
        Event_Wait(40);
        Engine_ActorSetAnimationAndWait(20, 3);
        Engine_EventWait(40);
        Value3(Engine_ActorFaceDirection, 20, 0x4000, 0);
        Engine_EventWait(20);
        Engine_ActorWalkByAndWait(20, 0, 32);
        Engine_ActorFaceDirection(14, 0x8000, 0);
        Engine_EventWait(10);
        Engine_ActorFaceDirection(20, 0, 0);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(20, 3);
        Engine_EventWait(30);
        Value3(Engine_ActorFaceDirection, 20, 0x4000, 0);
        Engine_EventWait(20);
        Value3(Engine_ActorSetSpeed, 20, 0xcccc, 0x6666);
        Engine_ActorGet(20)->unknown_5a &= 0xfe;
        Value3(Engine_ActorWalkByAndWait, 20, 0, -16);
        Engine_ActorGet(20)->unknown_5a |= 1;
        Value3(Engine_ActorFaceDirection, 14, 0x4000, 0);
        Engine_EventWait(40);
        Call3(Engine_ActorSetSpeed, 14, 0xcccc, 0x6666);
        Engine_ActorWalkByAndWait(14, 0, 16);
        Engine_EventWait(40);
        Func_02006634(164, 2);
        Engine_EventShowMessage(-1, 0);
        Engine_ItemShowFound(164, 3);
        Engine_PartyGiveItem(164, 0);
        Engine_ActorFaceDirection(0, 0xc000, 0);
        Engine_EventWait(30);
        Call3(Engine_ActorWalkByAndWait, 14, 0, -16);
        Actor_FaceDirection(14, 0x4000, 0);
        Engine_EventWait(30);
        Engine_ActorRunRepeatedMotion(14, 2);
        Engine_EventWait(20);
        Func_020067ac(14, 0);
        AdvanceMessage(2);
    } else {
        AdvanceMessage(5);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(20, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(20, 0);
        Engine_ActorFaceDirection(14, 0xa000, 0);
        Event_Wait(40);
        Actor_SetAnimationAndWait(14, 3);
        Engine_EventWait(20);
        Actor_SetAnimationAndWait(20, 3);
        Engine_EventWait(30);
        Actor_FaceDirection(14, 0x4000, 0);
        Engine_EventWait(30);
        Engine_ActorRunRepeatedMotion(14, 2);
        Engine_EventWait(20);
        Func_02006840(14, 0);
    }

    if (Event_ChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(20, 3);
        Event_Wait(20);
        Engine_EventShowMessage(20, 0);
        AdvanceMessage(1);
    } else {
        Event_Wait(10);
        Engine_ActorSetAnimationAndWait(20, 3);
        Event_Wait(20);
        AdvanceMessage(1);
        Engine_EventShowMessage(20, 0);
    }

    Event_Wait(10);
    Actor_FaceDirection(14, 0xa000, 0);
    Engine_EventWait(20);
    Actor_SetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Engine_EventWait(10);
    Actor_Jump(ACTOR_GERALD, 4, 13);
    Engine_ActorJump(1, 4, 30);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 14, 0x4000, 0);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Actor_SetSpeed(14, 0x19999, 0xcccc);
    Engine_ActorWalkByAndWait(20, 0, 16);
    Task_Wait(2);
    Actor_FaceDirection(20, 0x2000, 0);
    Event_Wait(10);
    Engine_EventShowMessage(20, 0);
    Event_Wait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Event_Wait(30);
    Engine_ActorFaceActor(14, 20, 30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Actor_FaceEachOther(14, ACTOR_GERALD, 0);
    Engine_EventWait(40);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(30);
    Engine_ActorFaceActor(1, 2, 30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Engine_ActorFaceActor(2, 1, 30);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorShowEmote(1, 0x106, 50);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Event_Wait(10);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Value3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Event_Wait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 0);
    Engine_ActorFaceEachOther(3, 2, 50);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(20, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 2, 0x101, 40);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Engine_EventOpenMessage(14, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(14, 3);
        Engine_EventWait(30);
        Engine_EventShowMessage(14, 0);
        AdvanceMessage(1);
    } else {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(14, 4);
        Engine_EventWait(20);
        AdvanceMessage(1);
        Event_ShowMessage(14, 0);
    }

    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x102, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(0, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(3, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(50);
    Engine_ActorFaceActor(14, 20, 60);
    Engine_ActorFaceDirection(14, 0x4000, 0);
    Engine_EventWait(20);
    Event_ShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x102, 40);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(20, 4);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Value3(Engine_ActorShowEmote, 0, 0x102, 40);
    Event_Wait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 50);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Value3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(1, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Call3(Engine_ActorShowEmote, 3, 0x100, 40);
    Engine_EventShowMessage(3, 0);
    Event_Wait(10);
    Call3(Engine_ActorShowEmote, 14, 0x102, 50);
    Engine_ActorFaceActor(20, 14, 50);
    Event_Wait(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Event_ShowMessage(14, 0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(1, 0, 0);
    Engine_ActorFaceEachOther(3, 2, 50);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Value3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(50);
    Engine_ActorFaceDirection(20, 0x2000, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(1, 2);
    Event_Wait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(14, 0x8000, 0);
    Engine_EventWait(40);
    Call3(Engine_ActorShowEmote, 14, 0x105, 60);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 2, 0x100, 40);
    Engine_EventShowMessage(2, 0);
    Event_Wait(10);
    Engine_ActorFaceEachOther(14, 2, 40);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 0);
    Engine_ActorFaceEachOther(3, 2, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Engine_EventWait(50);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 40);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Engine_EventOpenMessage(1, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_EventShowMessage(1, 0);
        AdvanceMessage(3);
    } else {
        do {
            Func_02006e16(10);
        } while (0);
        {
            u8 **scene_address = (u8 **)0x03001ebc;

            AdvanceMessageAt(scene_address, 1);
            Engine_EventOpenMessage(1, 0);
            if (Event_ChooseYesNo(0, 0) == 0) {
                Engine_EventWait(20);
                Engine_EventShowMessage(1, 0);
                AdvanceMessageAt(scene_address, 1);
            } else {
                AdvanceMessageAt(scene_address, 1);
                Engine_EventShowMessage(1, 0);
            }
        }
    }

    Event_Wait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Event_ShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Value3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Event_ShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 3, 0x101, 40);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Actor_FaceActor(20, 14, 40);
    Actor_RunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 14, 0x105, 70);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Call3(Engine_ActorShowEmote, 1, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Value3(Engine_ActorShowEmote, 2, 0x101, 40);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Event_Wait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 40);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(40);
    Engine_ActorFaceActor(0, 2, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Engine_ActorFaceActor(3, 2, 0);
    Engine_ActorFaceActor(20, 2, 0);
    Event_Wait(50);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(2, 2);
    Event_Wait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(14, 0x101, 40);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(2, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(20);
    Actor_SetAnimationAndWait(20, 3);
    Engine_EventWait(30);
    Engine_ActorFaceActor(20, 14, 30);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Value3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Event_Wait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(1, 2);
    Engine_ActorStartRepeatedMotion(3, 2);
    Engine_ActorRunRepeatedMotion(2, 2);
    Event_Wait(30);
    Event_Wait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Value3(Engine_ActorFaceDirection, 14, 0x8000, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 0);
    Engine_ActorFaceEachOther(3, 2, 50);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Value3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(40);
    Value3(Engine_ActorFaceDirection, 14, 0x4000, 0);
    Engine_EventWait(30);
    Call3(Engine_ActorShowEmote, 14, 0x102, 40);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x102, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(14, 20, 40);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Event_Wait(10);
    Call2(Engine_ActorSetAttachedEffect, 20, 0x102);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Actor_SetSpeed(20, 0x19999, 0xcccc);
    Engine_ActorWalkByAndWait(20, 0, 24);
    Engine_ActorFaceActor(0, 20, 0);
    Engine_ActorFaceActor(1, 20, 0);
    Engine_ActorFaceActor(3, 20, 0);
    Engine_ActorFaceActor(2, 20, 0);
    Actor_FaceDirection(14, 0x4000, 0);
    Event_ShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x100, 60);
    Engine_ActorFaceActor(20, 14, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Value3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Event_Wait(30);
    Actor_ShowEmote(14, 0x105, 40);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x101, 60);
    Call3(Engine_ActorSetSpeed, 20, 0x13333, 0x9999);
    Call3(Engine_ActorWalkByAndWait, 20, 0, -24);
    Engine_ActorFaceDirection(20, 0, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Value3(Engine_ActorFaceDirection, 14, 0x8000, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x102, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Event_Wait(10);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Value3(Engine_ActorFaceDirection, 14, 0x4000, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 3, 0x102, 40);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 0);
    Engine_ActorFaceEachOther(3, 2, 50);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Value3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 0, 0x101, 0);
    Call3(Engine_ActorShowEmote, 1, 0x101, 0);
    Call3(Engine_ActorShowEmote, 3, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 50);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Event_ShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x102, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(14, 20, 30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Event_Wait(10);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Value3(Engine_ActorFaceDirection, 14, 0x4000, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x101, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Event_Wait(30);
    Actor_FaceDirection(14, 0x8000, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 2, 0x101, 40);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 14, 0x4000, 0);
    Engine_ActorFaceDirection(20, 0x2000, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(3, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 14, 0x102, 40);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimation(3, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(30);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 14, 0x105, 60);
    Engine_EventShowMessage(14, 0);
    Event_Wait(10);
    Call3(Engine_ActorShowEmote, 1, 0x101, 40);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 14, 0x100, 40);
    Engine_EventShowMessage(14, 0);
    Func_020077a2(10);
    Call3(Engine_ActorFaceEachOther, 1, 0, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Value3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Func_020077de(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Func_020078d6, 3, 0x101, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Event_ShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Func_0200790a, 0, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Call3(Func_0200791e, 2, 0x101, 0);
    Value3(Func_02007928, 3, 0x101, 40);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 2, 0x100, 40);
    Engine_EventOpenMessage(2, 0);
    if (Engine_EventChooseYesNo(14, 0) == 0) {
        Event_Wait(10);
        Engine_EventShowMessage(2, 0);
        AdvanceMessage(1);
    } else {
        Engine_EventWait(10);
        AdvanceMessage(1);
        Engine_EventShowMessage(2, 0);
    }

    Event_Wait(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Event_Wait(20);
    Engine_ActorFaceDirection(20, 0, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorFaceActor(14, 20, 40);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 40);
    Engine_EventShowMessage(1, 0);
    Actor_FaceDirection(14, 0x4000, 0);
    Value3(Engine_ActorFaceDirection, 20, 0x2000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Value3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Event_Wait(30);
    Engine_ActorFaceEachOther(3, 2, 60);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(2, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(20, 14, 60);
    Call3(Engine_ActorFaceDirection, 20, 0x2000, 0);
    Value3(Engine_ActorFaceDirection, 14, 0x4000, 0);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(14, 0x4000, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Event_Wait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Engine_EventOpenMessage(20, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Call3(Engine_ActorShowEmote, 1, 0x102, 40);
        Engine_EventShowMessage(1, 0);
        AdvanceMessage(1);
    } else {
        Engine_EventWait(10);
        AdvanceMessage(1);
        Engine_EventShowMessage(1, 0);
    }

    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x100, 40);
    Engine_EventShowMessage(20, 0);
    Event_Wait(20);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimation(3, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 14, 0x100, 40);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorFaceActor(20, 14, 40);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x102, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorFaceActor(14, 20, 40);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_ActorFaceDirection(14, 0x4000, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 80);
    Call3(Engine_ActorShowEmote, 20, 0x102, 70);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 0);
    Engine_ActorFaceEachOther(3, 2, 50);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x106, 50);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Actor_FaceDirection(20, 0x2000, 0);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 14, 0x108, 40);
    Engine_EventOpenMessage(14, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Call3(Engine_ActorShowEmote, 14, 0x100, 40);
        Event_ShowMessage(14, 0);
        AdvanceMessage(1);
    } else {
        Engine_EventWait(10);
        Call3(Engine_ActorShowEmote, 14, 0x100, 40);
        AdvanceMessage(1);
        Event_ShowMessage(14, 0);
    }

    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorFaceActor(14, 20, 40);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorFaceActor(20, 14, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 14, 0x100, 40);
    Value3(Engine_ActorFaceDirection, 14, 0x8000, 0);
    Engine_EventWait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Call3(Engine_ActorShowEmote, 20, 0x100, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Event_Wait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(20, 3);
    Event_Wait(30);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(20, 0x2000, 0);
    Engine_EventWait(20);
    Func_02007f9c(0, 20);
    Func_02007fa4(1, 20);
    Func_02007fac(3, 20);
    Func_02007fb4(2, 20);
    Call3(Engine_ActorSetSpeed, 20, 0x10000, 0x8000);
    Engine_ActorWalkByAndWait(20, 0, 32);
    Engine_ActorFaceDirection(20, 0, 0);
    Value3(Engine_ActorFaceDirection, 14, 0x4000, 0);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(20, 3);
    Engine_EventWait(30);
    Engine_ActorWalkByAndWait(1, 16, 0);
    Engine_ActorWalkBy(20, 0, 80);
    Engine_EventWait(40);
    Actor_WalkByAndWait(ACTOR_GERALD, -16, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x4000, 0);
    Value3(Engine_ActorFaceDirection, 3, 0x4000, 0);
    Actor_WaitForMove(20);
    Engine_EventWait(80);
    Engine_ActorSetPosition(20, 0, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Event_Wait(30);
    Func_02008096(0, 14);
    Func_0200809e(1, 14);
    Func_020080a6(3, 14);
    Func_020080ae(2, 14);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Actor_SetSpeed(14, 0xcccc, 0x6666);
    Engine_ActorWalkByAndWait(14, 0, 24);
    Engine_ActorWalkByAndWait(14, -80, 0);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(14, 0, 0);
    Engine_EventWait(20);
    Event_ShowMessage(14, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(14, 0x4000, 0);
    Engine_EventWait(20);
    Actor_WalkByAndWait(14, 0, 48);
    Engine_ActorWalkByAndWait(14, -64, 0);
    Engine_ActorStop(0);
    Actor_Stop(ACTOR_GERALD);
    Engine_ActorStop(3);
    Engine_ActorStop(2);
    Actor_SetPosition(14, 0, 0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(0, 3, 0);
    Engine_ActorFaceEachOther(1, 2, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimation(3, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(30);
    Engine_AudioPlayCue(17);
    Call3(Engine_ActorSetSpeed, 1, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 2, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
    Func_020080ea(1, 2);
    position = Func_02008090(0);
    if (position != 0)
        Func_020080ca(1, position[5], position[9]);
    Engine_ActorWaitForMove(1);
    Engine_ActorSetPosition(1, 0, 0);
    Actor_SetAnimation(ACTOR_IVAN, 2);
    position = Actor_Get(ACTOR_PARTY_LEADER);
    if (position != 0)
        Func_020080fa(2, position[5], position[9]);
    Scene_UpdateSlot(2);
    Engine_ActorSetPosition(2, 0, 0);
    Func_0200814a(3, 2);
    position = Func_020080f0(0);
    if (position != 0)
        Func_0200812a(3, position[5], position[9]);
    Engine_ActorWaitForMove(3);
    Func_02008182(3, 0, 0);
    carry = Func_02008108(10);
    carry = Func_0200824c(carry);
    Func_02008120(carry);
}

void FieldScene_RunScene3b8_02003d40(void)
{
    struct EventWork *work;

    work = (struct EventWork *)Data_03001ebc[0];
    Event_Begin();
    Audio_PlayCue(158);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    if (work->touched_trigger == 32) {
        Func_020080e0(1);
        Event_Wait(10);
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    } else {
        if (work->touched_trigger == 30) {
            Func_020080fe(4);
            Event_Wait(10);
            Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
        } else {
            Func_02008118(2);
            Event_Wait(10);
            Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
        }
    }
    Event_Wait(16);
    Event_RequestExit(work->touched_trigger);
    Func_0200814c(1);
    Func_02008152(2);
    Func_02008158(4);
    Event_End();
}

static __inline__ void SetSceneEffectOffset(s32 actor, s32 axis, s32 offset)
{
    Actor_CenterAndWalk(actor, axis, offset);
}

void RunSceneEffectSetup(void)
{
    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384);
    Audio_PlayCue(158);
    Map_AnimateCells(33604944, 36, 10);
    SetSceneEffectOffset(0, 2, -16);
    Event_Wait(16);
    Event_RequestExit(2);
    Event_End();
}

void RunSupplementalSequenceOne(void)
{
    s32 p;
    Call1(Func_020081ee, 2412);
    Call0(Engine_EventBegin);
    Call0(Func_02008356);
    Call3(Engine_ActorFaceDirection, 8, 20480, 0);
    Call3(Engine_ActorFaceDirection, 9, 12288, 0);
    Call3(Engine_ActorWalkToAndWait, 0, 200, 272);
    Call3(Engine_ActorFaceDirection, 0, 49152, 0);
    Call1(Engine_EventWait, 20);
    p = (s32)&Value_00002233;
    Engine_EventSetMessage(p);
    Value2(Engine_EventOpenMessage, 8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Call1(Engine_EventWait, 20);
        Engine_EventSetMessage(p + 1);
        Call2(Engine_EventShowMessage, 8, 0);
    } else {
        Value1(Engine_EventWait, 20);
        Event_SetMessage(p + 2);
        Call2(Engine_EventShowMessage, 8, 0);
        Call1(Engine_EventWait, 20);
        Call3(Engine_ActorFaceEachOther, 8, 9, 60);
        Call3(Engine_ActorFaceDirection, 9, 12288, 0);
        Call1(Engine_EventWait, 40);
        Actor_RunRepeatedMotion(9, 2);
        Call1(Engine_EventWait, 30);
        Call3(Engine_ActorFaceEachOther, 8, 9, 30);
        Actor_SetAnimationAndWait(9, 3);
        Call1(Engine_EventWait, 30);
        Actor_ShowEmote(8, 258, 50);
        Call3(Engine_ActorFaceDirection, 8, 20480, 0);
        Call3(Engine_ActorFaceDirection, 9, 12288, 0);
        Call1(Engine_EventWait, 20);
        Call2(Engine_ActorSetAnimationAndWait, 8, 4);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 8, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 8, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 8, 0);
    }
    Call0(Engine_EventEnd);
}

void SceneDialogue_ShowLine2239Or223A(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x96d) == 0) {
        GameFlag_Set(0x96d);
        Event_SetMessage(MSG_ROBIN_ID_REALLY_LIKE_THANK);
        Event_ShowMessage(9, 0);
    } else {
        Event_SetMessage(MSG_BABI_WAITING_FOR_AT_COLOSSEUM);
        Event_ShowMessage(9, 0);
    }
}

void SceneDialogue_ShowMessage22a3Branch(s32 a)
{
    s32 k = (s32)&Value_000022a3;

    Event_SetMessage(k);
    Event_OpenMessage(a, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage(k + 1);
        Event_ShowMessage(a, 0);
    } else {
        Event_SetMessage(k + 2);
        Event_ShowMessage(a, 0);
    }
}

void FieldScene_RunStepWithValue29e0(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_ROBIN_PEERED_INTO, 1);
    Message_ShowCentered(MSG_ITS_FILLED_WITH_FRESH_CLEAN, 1);
    Event_End();
}

s32 SceneData_SelectTableD004ByStateAndFlags(void)
{
    if (gGameState.scene == (s32)&Value_0000008b) {
        if (GameFlag_IsSet(0x950) != 0) {
            return (s32)Data_0200dad8;
        }
        if (GameFlag_IsSet(0x962) != 0) {
            return (s32)Data_0200da48;
        }
        return (s32)Data_0200d9e8;
    }
    if (GameFlag_IsSet(0x950) != 0) {
        return (s32)Data_0200d688;
    }
    if (GameFlag_IsSet(0x962) != 0) {
        return (s32)Data_0200d394;
    }
    return (s32)Data_0200d004;
}
