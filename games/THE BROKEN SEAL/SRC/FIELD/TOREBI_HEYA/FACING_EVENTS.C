#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum FacingEventsMessage {
    MSG_IF_CAN_MAKE_NAME_FOR = 0x1fd9,
    MSG_WASTE_STUCK_HERE_WHEN_SUCH = 0x1feb,
    MSG_HEY_WHATS_THIS = 0x2394,
    MSG_OH_HELLO = 0x239c,
    MSG_HO_HUM_IM_FINE_ITS = 0x239d,
    MSG_GRRR_CHEF_IN_BAD_MOOD = 0x23a1,
    MSG_HE_WONT_SAIL_SHIP_EVEN = 0x23a4,
    MSG_GRRR_SCAM_WHY_WONT_THEY = 0x23a8,
    MSG_HEH_HEH_SHE_JUST_HID = 0x23b3,
    MSG_THING_FOUND_DEFINITELY_SAME_AS = 0x23b4
};


struct Ent { s32 a; u16 b; u16 c; };

extern u8 Data_020099d0[];
extern u8 Data_02009670[];
extern u8 Data_02009310[];
extern struct Ent Data_02009dcc[];
extern s16 Data_02000240_t[][1];
extern u8 LinkedMessage_YourFirstTimeTolbi[];
extern u8 LinkedMessage_DidYouComeAllWay[];
extern u8 Value_0000239e;
extern u8 LinkedMessage_DoYouWantStayWith;
extern u8 LinkedMessage_HeeHeeDoLookLike[];
extern u8 Value_000023ac;

void Func_02000b16(void);
s32 Func_02000ac6();
s32 Func_02000b0e();
s32 Func_02000afe();
u8 *Func_02000e0e();
u8 *Func_02000e32();
u8 *Func_02000f00(s32 no);
u8 *Func_02000fa8(s32 no);
u8 *Func_0200106c(s32 no);
u8 *Func_0200131c(s32 no);
void Func_02001122(s32);

/* Contiguous unnamed leaf-owner run for resource_3b6. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* 0x02000d6a serves two imports: the one-argument message at +0x04 and the
 * three-argument setter at +0x0e. `_b` is the same address with its own
 * prototype. */

/* Pool-forced message base: a SYMBOL_REF gcc cannot fold, so the two
 * sibling ids stay `adds r0, r5, #1` / `#2` off one pooled word. */

/* Old-style declarations: interfaces vary by call site across this overlay. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void SceneInit_Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 SceneInit_Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void SceneInit_Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void SceneInit_Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Scene_Call1(void (*f)(s32), s32 a) { f(a); }

static __inline__ s32 Scene_Value2(s32 (*f)(s32, s32), s32 a, s32 b) { return f(a, b); }

static __inline__ void Scene_Call3(void (*f)(s32, s32, s32), s32 a, s32 b, s32 c) { f(a, b, c); }

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

u8 *SceneData_GetTable8BB4(void)
{
    return (u8 *)0x02008bb4;
}

s32 Func_02000038(void)
{
    return 0;
}

u8 *SceneData_GetTable8dac(void)
{
    return (u8 *)0x02008dac;
}

s32 SceneData_SelectTable8e00ByFlag(void)
{
    if (GameFlag_IsSet(0x950) != 0) {
        return 0x02009040;
    }
    return 0x02008E00;
}

u8 *SceneData_SelectTable9310ByFlags(void)
{
    if (GameFlag_IsSet(0x950) != 0) {
        return Data_020099d0;
    }
    if (GameFlag_IsSet(0x962) != 0) {
        return Data_02009670;
    }
    return Data_02009310;
}

void SceneState_SetWork1c0AndRun(void)
{
    extern u8 *Data_03001ebc;

    u8 *state = Data_03001ebc;

    *(s32 *)(state + 0x1C0) = 0x201;
    *(s32 *)(state + 0x1C8) = 24;
    Func_02000b16();
}

void FieldScene_RunPrimarySequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    u8 *p6;
    s32 n;

    p6 = *(u8 **)Data_03001ebc;
    for (i = 8; i < 66; i++) {
        record = Value1(Func_02000ac6, i);
        if (record != 0) {
            *(u8 *)(record + 85) = 0;
        }
    }
    p6 = p6 + 0x16c;
    n = *(s16 *)p6 - 14;
    Audio_PlayCue(158);
    Value3(Engine_MapAnimateCells,
           Data_02009dcc[n].a,
           Data_02009dcc[n].b,
           Data_02009dcc[n].c);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    *(u8 *)(Func_02000b0e(0) + 85) = 0;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Event_RequestExit(*(s16 *)p6);
}

void FieldScene_RunScene3b6SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Event_Begin();
    Event_SetMessage(MSG_HEY_WHATS_THIS);
    Event_Wait(40);
    rec7 = Value4(Func_02000afe, 0x11c, 0x2580000, 0, 0x3380000);
    Actor_SetSpriteFlags(rec7, 0);
    Object_SetAnimation(rec7, 6);
    Event_Wait(10);
    Object_SetAnimation(rec7, 1);
    Event_Wait(40);
    Engine_ObjectDispatchRelease(rec7);
    Event_Wait(2);
    Actor_ShowEmote(25, 0x100, 50);
    Actor_SetSpeed(25, 0x10000, 0x8000);
    Actor_WalkToAndWait(25, 0x258, 0x350);
    Actor_FaceDirection(25, 0xc000, 0);
    Event_Wait(40);
    Event_ShowMessage(25, 0);
    Actor_RunRepeatedMotion(25, 2);
    Event_Wait(30);
    Actor_WalkToAndWait(25, 0x238, 0x350);
    Actor_FaceDirection(25, 0xc000, 0);
    Event_Wait(30);
    Actor_ShowEmote(25, 0x108, 50);
    Event_Wait(20);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
    Event_Wait(20);
    Actor_FaceDirection(25, 0x3000, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(25, 2);
    Event_Wait(20);
    Event_ShowMessage(25, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 50);
    Event_Wait(20);
    Actor_SetAnimationAndWait(25, 4);
    Event_Wait(20);
    Event_ShowMessage(25, 0);
    Event_Wait(30);
    Actor_ShowEmote(25, 0x102, 50);
    Event_ShowMessage(25, 0);
    Actor_SetSpeed(25, 0x16666, 0xb333);
    Actor_WalkByAndWait(25, 16, 0);
    Actor_WalkByAndWait(25, 0, 32);
    Event_Wait(20);
    Actor_SetAnimationAndWait(25, 3);
    Event_Wait(20);
    Event_ShowMessage(25, 0);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 16, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Event_Wait(20);
    Actor_SetSpeed(25, 0x1cccc, 0xe666);
    Actor_WalkByAndWait(25, 0, 48);
    Actor_SetPosition(25, 0, 0);
    Event_End();
}

void FieldScene_RunActorsThirtyOneToThirtyThreeChoreography(void)
{
    void Event_Wait(s32);
    void Event_SetMessage(s32);

    Event_Begin();
    Event_SetMessage(MSG_HE_WONT_SAIL_SHIP_EVEN);
    Event_Wait(30);
    /* Same import, same first two arguments, differing only in the third.
     * Two call sites, not a loop. */
    Actor_Jump(31, 4, 13);
    Actor_Jump(31, 4, 30);
    Event_ShowMessage(31, 0);
    Event_Wait(10);
    /* r1 = 129 << 1 = 0x102. Argument registers are set r1, r2, r0. */
    Actor_ShowEmote(32, 0x102, 50);
    Event_Wait(10);
    Actor_SetAnimationAndWait(32, 3);
    Event_Wait(30);
    Event_ShowMessage(32, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(33, 4);
    Event_Wait(20);
    Event_ShowMessage(33, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(31, 2);
    Event_Wait(20);
    Event_ShowMessage(31, 0);
    Event_Wait(10);
    /* Repeats the (32, 3) call made above; a second site, deliberately not
     * folded with the first. */
    Actor_SetAnimationAndWait(32, 3);
    Event_Wait(30);
    Event_End();
}

/* Keep the first byte store and zero initialization as one assignment. */
s32 Scene_InitFacingActors(void)
{
    u8 *record;
    s32 none;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (SceneInit_Value1(Engine_GameFlagIsSet, 0x950) != 0) {
        SceneInit_Call6(Engine_MapCopyCellAttributes, 51, 47, 3, 1, 51, 45);
        record = Func_02000e0e(31);
        record[35] = none = 0;
        (*(s8 **)(record + 80))[9] = ((-13 & (*(s8 **)(record + 80))[9]) | 8);
        record = Func_02000e32(32);
        record[35] = none;
        (*(s8 **)(record + 80))[9] = ((-13 & (*(s8 **)(record + 80))[9]) | 8);
        if (SceneInit_Value1(Engine_GameFlagIsSet, 0x8bc) != 0) {
            SceneInit_Call3(Engine_ActorSetPosition, 25, 0x2300000, 0x2a80000);
            SceneInit_Call3(Engine_ActorFaceDirection, 25, 0x8000, 0);
        }
        if (Data_02000240_t[225][0] == 19) {
            if (SceneInit_Value1(Engine_GameFlagIsSet, 0x8bc) == 0) {
                SceneInit_Call1(Engine_GameFlagSet, 0x8bc);
                Event_OpenScreen();
                FieldScene_RunScene3b6SequenceA();
            }
        }
        if (Data_02000240_t[225][0] == 16) {
            if (SceneInit_Value1(Engine_GameFlagIsSet, 0x300) == 0) {
                SceneInit_Call1(Engine_GameFlagSet, 0x300);
                Event_OpenScreen();
                FieldScene_RunActorsThirtyOneToThirtyThreeChoreography();
            }
        }
        if (SceneInit_Value1(Engine_GameFlagIsSet, 0x8ab) != 0) {
            Actor_SetPosition(35, 0, 0);
            Actor_SetPosition(36, 0, 0);
        }
    }
    return 0;
}

void SceneDialogue_RunFacingPrompt(s32 no)
{
    u8 *actor = Func_02000f00(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0x8000) {
        Shop_Open(28, no);
    } else if (GameFlag_IsSet(0x950) != 0) {
        Scene_Call1(Engine_EventSetMessage, 0x238d);
        Event_ShowMessage(no, 0);
    } else if (GameFlag_IsSet(0x962) != 0) {
        Scene_Call1(Engine_EventSetMessage, 0x221b);
        Event_ShowMessage(no, 0);
    } else {
        msg = (s32)LinkedMessage_YourFirstTimeTolbi;
        Event_SetMessage(msg);
        Scene_Value2(Engine_EventOpenMessage, no, 0);
        if (Scene_Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage(msg + 1);
        } else {
            Event_SetMessage(msg + 2);
        }
        Event_ShowMessage(no, 0);
    }
}

void SceneDialogue_RunFacingActionPrompt(s32 no)
{
    u8 *actor = Func_02000fa8(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Shop_Open(26, no);
    } else if (GameFlag_IsSet(0x950) != 0) {
        msg = (s32)LinkedMessage_DidYouComeAllWay;
        Event_SetMessage(msg);
        Scene_Value2(Engine_EventOpenMessage, no, 0);
        if (Scene_Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage(msg + 1);
        } else {
            Event_SetMessage(msg + 2);
        }
        Event_ShowMessage(no, 0);
    } else if (GameFlag_IsSet(0x962) != 0) {
        Scene_Call1(Engine_EventSetMessage, 0x2219);
        Event_ShowMessage(no, 0);
    } else {
        Scene_Call1(Engine_EventSetMessage, 0x1fd2);
        Event_ShowMessage(no, 0);
        Scene_Call3(Engine_ActorShowEmote, no, 0x106, 0);
        Event_Wait(40);
        Event_ShowMessage(no, 0);
    }
}

void SceneDialogue_RunFacingAction(s32 no)
{
    u8 *actor = Func_0200106c(0);
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Shop_Open(27, no);
    } else {
        if (GameFlag_IsSet(0x950) != 0) {
            Scene_Call1(Engine_EventSetMessage, 0x238f);
            Event_ShowMessage(no, 0);
        } else if (GameFlag_IsSet(0x962) != 0) {
            Scene_Call1(Engine_EventSetMessage, 0x221d);
            Event_ShowMessage(no, 0);
        } else {
            Event_SetMessage(MSG_IF_CAN_MAKE_NAME_FOR);
            Event_ShowMessage(no, 0);
        }
    }
}

void SceneDialogue_RunMessage239eStep(s32 subject)
{
    s32 message;

    Event_Begin();

    message = (s32)&Value_0000239e;
    Event_SetMessage(message);
    Event_OpenMessage(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(message + 1);
    } else {
        Event_SetMessage(message + 2);
    }

    Event_ShowMessage(subject, 0);
    Event_End();
}

void SceneDialogue_RunActorLine23a1(s32 no)
{
    Func_02001122(no);
    Event_SetMessage(MSG_GRRR_CHEF_IN_BAD_MOOD);
    Event_ShowMessage(no, 0);
    Event_End();
}

void SceneDialogue_RunMessage1FBBStep(s32 subject)
{
    s32 msg;

    Event_Begin();

    msg = (s32)&LinkedMessage_DoYouWantStayWith;
    Event_SetMessage(msg);
    Event_OpenMessage(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(msg + 1);
    } else {
        Event_SetMessage(msg + 2);
    }

    Event_ShowMessage(subject, 0);
    Event_End();
}

void FieldScene_RunScene3b6_020007b0(s32 a0)
{
    void Event_ShowMessage();

    u32 i;
    s32 record;
    s32 base5_2399;

    Event_Begin();
    if (GameFlag_IsSet(0x8bd) == 0) {
        base5_2399 = (s32)LinkedMessage_HeeHeeDoLookLike;
        Event_SetMessage(base5_2399);
        Event_OpenMessage(a0, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage((base5_2399 + 1));
        } else {
            Event_SetMessage((base5_2399 + 2));
        }
        Event_ShowMessage(a0, 0);
    } else {
        if (GameFlag_IsSet(0x8be) == 0) {
            GameFlag_Set(0x8be);
            Event_SetMessage(MSG_OH_HELLO);
            Event_ShowMessage(a0, 0);
            Event_Wait(10);
            Actor_RunRepeatedMotion(a0, 2);
            Event_Wait(20);
        }
        Event_SetMessage(MSG_HO_HUM_IM_FINE_ITS);
        Event_ShowMessage(a0, 0);
    }
    Event_End();
}

void SceneDialogue_RunActor25FlaggedLine(void)
{
    void Event_Begin(void);

    Event_Begin();
    if (GameFlag_IsSet(0x8BE) == 0) {
        Event_SetMessage(MSG_HEH_HEH_SHE_JUST_HID);
    } else {
        Event_SetMessage(MSG_THING_FOUND_DEFINITELY_SAME_AS);
    }
    Event_ShowMessage(25, 0);
    Event_End();
}

void FieldScene_RunScene3b6_02000898(s32 a0)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(MSG_GRRR_SCAM_WHY_WONT_THEY);
    Actor_ShowEmote(31, 0x103, 40);
    Event_ShowMessage(a0, 0);
    Event_End();
}

void SceneDialogue_RunMessage23acStep(s32 subject)
{
    s32 message;

    Event_Begin();

    message = (s32)&Value_000023ac;
    Event_SetMessage(message);
    Event_OpenMessage(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(message + 1);
    } else {
        Event_SetMessage(message + 2);
    }

    Event_ShowMessage(subject, 0);
    Event_End();
}

void SceneDialogue_RunFacingMessage(s32 no)
{
    s32 GameFlag_IsSet(s32 flag);

    u8 *actor = Func_0200131c(0);
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Sanctum_Open(no);
    } else {
        if (GameFlag_IsSet(0x950) != 0) {
            Scene_Call1(Engine_EventSetMessage, 0x23bf);
            Event_ShowMessage(no, 0);
        } else if (GameFlag_IsSet(0x962) != 0) {
            Scene_Call1(Engine_EventSetMessage, 0x2231);
            Event_ShowMessage(no, 0);
        } else {
            Event_SetMessage(MSG_WASTE_STUCK_HERE_WHEN_SUCH);
            Event_ShowMessage(no, 0);
        }
    }
}
