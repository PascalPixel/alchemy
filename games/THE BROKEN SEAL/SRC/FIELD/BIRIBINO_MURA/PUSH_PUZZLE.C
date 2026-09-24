#include "TYPES.H"

#define NULL ((void *)0)
#define CalculateFacingAngle Func_0200107a
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_0000001e
#define SecondaryRuntimeSelector Value_00000023
#define TertiaryRuntimeSelector Value_00000020
#define PrimaryOverlayData Data_020091ec
#define SecondaryOverlayData Data_0200930c
#define TertiaryOverlayData Data_0200936c
#define DefaultOverlayData Data_020091d4
#define PrimaryOverlayData_0200011c Data_02009498
#define SecondaryOverlayData_0200011c Data_02009600
#define TertiaryOverlayData_0200011c Data_020096f0
#define DefaultOverlayData_0200011c Data_02009480
#define PrimaryOverlayData_02000198 Data_02009744
#define SecondaryOverlayData_02000198 Data_02009a2c
#define TertiaryOverlayData_02000198 Data_02009bc4
#define DefaultOverlayData_02000198 Data_02009738

#include "FACING_OBJECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

enum {
    /* Message 0x182 + 194. */
    ITEM_HARD_NUT = 194
};

enum PushPuzzleMessage {
    MSG_ROBIN_PEERED_INTO = 0x947,
    MSG_ITS_TREE_BUT_ALMOST_LOOKS = 0x13ae,
    MSG_DID_SEE_TREE_AT_ENTRANCE = 0x13b3,
    MSG_HAVE_TRIED_HEADING_SOUTHEAST_FROM = 0x13b7,
    MSG_MCCOYS_HIDDEN_WAREHOUSE_DO_NOT = 0x146e,
    MSG_THERE_TREE_LOOKS_LIKE_PERSON = 0x1470,
    MSG_AREA_OFF_LIMITS_THOSE_WITHOUT = 0x1472,
    MSG_WAS_TURNED_INTO_TREE_FOR = 0x16bf,
    MSG_CURSE_WAS_BROKEN_THANKS_EFFORTS = 0x16c8,
    MSG_HAVE_EVER_BEEN_VILLAGE_IMIL = 0x16cc,
    MSG_THANK_SAVED_ME_FROM_BEING = 0x1774,
    MSG_YOURE_GUY = 0x1775,
    MSG_JILL_GAVE_ROBIN_SPECIAL_GIFT = 0x177a,
    MSG_BOTTOM_NOT_VISIBLE_LOOKS_VERY = 0x29dd
};

extern s16 Data_02000240[];
extern u8 Value_00000020;
extern u8 Data_020093fc[];
extern u8 Data_020091c0[];
extern u8 Value_0000001e;
extern u8 Value_00000023;
extern u8 Data_02009d28[];
extern u8 Data_02009d7c[];

s32 Func_0200107a(s32, s32);
void Func_020012c6(s32, s32, s32);
s32 Func_02001460();
void Func_020016ca();
s32 Func_02001804();
s32 Func_02001828();
s32 Func_020018a2();
void Func_02001960();
void Func_0200196c();
void Func_02001978();
s32 Func_02001cb2();
s32 Func_02001cc2();
s32 Func_02001cd0();
void Func_020018e2();
u8 *Func_02001f90();
void Func_02001fa8();

/* Per-site raw names preserve the stock relocated branch operands; they
 * resolve to Func_080090c8, Object_SetCallback and Object_InitializeMode respectively. */

/* Per-site raw names: the first resolves to Scene_GetRecord and the four
 * renderer sites resolve to Func_080091c0. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Shared cross-overlay scene-record block; +450 is the scene sub-state. */

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ u16 ReadU16Elem(u16 *base, s32 idx)
{

    return *(u16 *)(base + idx);
}

static __inline__ void bump_step_020001ec(s32 amount)
{
    gEventWork->message += amount;
}

s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 tgt;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        tgt = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(tgt - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

s32 SceneData_SelectOverlayDataByRuntimeSelector(void)
{
    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData;
    }
    return (s32)DefaultOverlayData;
}

s32 SceneData_GetTable93FCWhenState20(void)
{
    if (gGameState.scene == (s32)&Value_00000020) {
        return (s32)Data_020093fc;
    }
    return 0;
}

/*
 * Table getter for resource_38b. The owner at 0x02000104 is eight bytes and
 * includes its one pool word at 0x02000108: the pc-relative load reads that
 * word, so the word belongs to this owner. The word is an address returned
 * without being dereferenced. Many getters share this body, but each returns
 * a different address.
 */
u8 *SceneData_GetTable942c(void)
{
    return (u8 *)0x0200942c;
}

void SceneState_SetValues9_3_0(void)
{
    Func_020012c6(9, 3, 0);
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    extern u8 PrimaryOverlayData_0200011c[];
    extern u8 SecondaryOverlayData_0200011c[];
    extern u8 TertiaryOverlayData_0200011c[];
    extern u8 DefaultOverlayData_0200011c[];
    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData_0200011c;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData_0200011c;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData_0200011c;
    }
    return (s32)DefaultOverlayData_0200011c;
}

void FieldScene_RunScriptedSteps947And29DD(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_ROBIN_PEERED_INTO, 1);
    Message_ShowCentered(MSG_BOTTOM_NOT_VISIBLE_LOOKS_VERY, 1);
    Event_End();
}

s32 SceneData_SelectDataByRuntimeSelectorB(void)
{
    extern u8 PrimaryOverlayData_02000198[];
    extern u8 SecondaryOverlayData_02000198[];
    extern u8 TertiaryOverlayData_02000198[];
    extern u8 DefaultOverlayData_02000198[];
    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData_02000198;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData_02000198;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData_02000198;
    }
    return (s32)DefaultOverlayData_02000198;
}

void FieldScene_RunScriptedStep1472(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_AREA_OFF_LIMITS_THOSE_WITHOUT, 1);
    Event_End();
}

void FieldScene_RunScriptedStep146E(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_MCCOYS_HIDDEN_WAREHOUSE_DO_NOT, 1);
    Event_End();
}

void SceneDialogue_RunLine1470(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_THERE_TREE_LOOKS_LIKE_PERSON, 1);
    Event_End();
}

void FieldScene_RunScene38b_02000240(void)
{
    Event_Begin();
    Event_SetMessage(MSG_ITS_TREE_BUT_ALMOST_LOOKS);
    if (GameFlag_IsSet(0x301) != 0) {
        bump_step_020001ec(1);
    }
    Event_ShowMessage(9, 0);
    GameFlag_Set(0x301);
    Event_End();
}

void SceneDialogue_RunActorTwelveDialogue(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DID_SEE_TREE_AT_ENTRANCE);
    Event_AskYesNo(12, 0);
    Event_End();
}

void SceneDialogue_RunActorFourteenDialogue(void)
{
    Event_Begin();
    Event_SetMessage(MSG_HAVE_TRIED_HEADING_SOUTHEAST_FROM);
    Event_AskYesNo(14, 0);
    Event_End();
}

void SceneDialogue_ShowLine16BF(void)
{
    Event_Begin();
    Event_SetMessage(MSG_WAS_TURNED_INTO_TREE_FOR);
    Event_AskYesNo(21, 0);
    Event_End();
}

void SceneDialogue_RunActorSixteenDialogue(void)
{
    Event_Begin();
    Event_SetMessage(MSG_CURSE_WAS_BROKEN_THANKS_EFFORTS);
    Event_AskYesNo(16, 0);
    Event_End();
}

void SceneDialogue_ShowLine16CC(void)
{
    Event_Begin();
    Event_SetMessage(MSG_HAVE_EVER_BEEN_VILLAGE_IMIL);
    Event_AskYesNo(18, 0);
    Event_End();
}

void FieldScene_RunEarlySequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *record;
    s32 v5;
    u8 *tbl;
    u8 *tbl2;
    s32 off;
    s32 off2;
    s32 a1;
    s32 a2;
    u8 *p7;

    p7 = *(u8 **)Data_03001ebc;
    Event_Begin();
    for (i = 8; i < 66; i++) {
        record = (u8 *)Value1(Engine_ActorGet, i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    v5 = (s32)((s32)(*(u16 *)(p7 + 0x16c) - 3) << 16) >> 16;
    if (v5 == 6) {
        Audio_PlayCue(188);
    } else {
        Call1((void (*)())Engine_AudioPlayCue, 158);
    }
    off = v5 << 2;
    tbl = (u8 *)0x02009d0c;
    a1 = *(s16 *)(tbl + off);
    off2 = off + 2;
    a2 = *(s16 *)(tbl + off2);
    tbl2 = (u8 *)0x02009cf0;
    Value3(Engine_MapAnimateCells, *(s32 *)(tbl2 + off), a1, a2);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    *(u8 *)(Func_02001460(0) + 85) = 0;
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x100;
    if (v5 == 6) {
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
    } else {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    }
    if (v5 == 4) {
        Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    } else {
        Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    }
    Event_Wait(16);
    Event_RequestExit(v5 + 3);
    Event_End();
}

void FieldScene_RunScene38bSequenceC(void)
{
    struct FieldActor *rec;
    struct FieldActor *rec7;

    rec = (struct FieldActor *)Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    rec7 = (struct FieldActor *)Value1(Engine_ActorGet, 11);
    if ((rec7->x.fixed >> 20) == 6) {
        Event_Begin();
        Actor_SetSpritePriority(11, 1);
        Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Event_Wait(20);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
        Actor_SetSpeed(11, 0x3333, 0x1999);
        Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a &= ~1;
        rec7->motion_flags = 0;
        rec->scale_x = -0x10000;
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 16);
        Actor_MoveToAndWait(11, 111, 196);
        rec->scale_x = 0x10000;
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 185);
        Event_Wait(20);
        rec->scale_x = -0x10000;
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 16);
        Actor_MoveToAndWait(11, 121, 190);
        rec->scale_x = 0x10000;
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 141, 189);
        Event_Wait(20);
        rec->scale_x = -0x10000;
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 16);
        Actor_MoveToAndWait(11, 132, 186);
        rec->scale_x = 0x10000;
        Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a |= 1;
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 166, 185);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
        Actor_SetSpritePriority(11, 2);
        Func_020016ca(0, 11);
        Task_Wait(10);
        Event_SetMessage(MSG_THANK_SAVED_ME_FROM_BEING);
        Event_ShowMessage(11, 0);
        Psynergy_Cancel();
        Task_Wait(10);
        GameFlag_Set(0x848);
        Event_End();
    }
}

void FieldScene_CallHelper170c(void)
{
    Leader_CheckAhead();
}

void FieldScene_RunScene38b_02000584(void)
{
    u32 i;
    struct FieldActor *rec7;
    struct FieldActor *record;
    s32 base5_20091c0;

    rec7 = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    if (GameFlag_IsSet(0x845) == 0) {
    } else {
        if (GameFlag_IsSet(0x848) == 0) {
        } else {
            Event_Begin();
            Camera_SetSpeed(0x26666, 0x4ccc);
            Camera_MoveTo(0x1070000, -1, 0xad0000, 1);
            Camera_WaitForMove();
            record = Actor_Get(12);
            if (record->x.fixed > rec7->x.fixed) {
                Actor_FaceDirection(13, 0x5000, 20);
                Actor_ShowEmote(13, 0x100, 20);
                Event_SetMessage(MSG_YOURE_GUY);
                Event_ShowMessageAndWait(13, 0, 10);
                Actor_ShowEmote(12, 0x100, 0);
            } else {
                Actor_FaceDirection(12, 0x3000, 20);
                Actor_ShowEmote(12, 0x100, 20);
                Event_SetMessage(MSG_YOURE_GUY);
                Event_ShowMessageAndWait(12, 0, 10);
                Actor_ShowEmote(13, 0x100, 0);
            }
            Actor_ShowEmote(14, 0x100, 0);
            Actor_FaceDirection(14, 0x3000, 0);
            Actor_FaceDirection(12, 0x5000, 0);
            Actor_FaceDirection(13, 0x3000, 0);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x10c, 184);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 40);
            Actor_RunRepeatedMotion(13, 2);
            Event_ShowMessageAndWait(13, 0, 10);
            Actor_FaceDirection(13, 0, 0);
            Actor_FaceDirection(14, 0x3000, 20);
            Actor_FaceDirection(12, 0x8000, 20);
            Actor_SetAnimationAndWait(12, 3);
            Actor_SetAttachedEffect(14, 0x102);
            Event_Wait(40);
            Actor_FaceDirection(14, 0x3000, 10);
            Actor_FaceDirection(12, 0x5000, 0);
            Actor_FaceDirection(13, 0x3000, 10);
            Actor_RunRepeatedMotion(14, 1);
            Event_ShowMessageAndWait(14, 0, 10);
            Actor_SetAnimation(12, 3);
            Actor_SetAnimationAndWait(13, 3);
            Event_Wait(20);
            Event_ShowMessage(14, 0);
            Actor_SetSpeed(14, 0x9999, 0x4ccc);
            *(u8 *)(Func_02001804(14) + 90) &= 254;
            Actor_WalkToAndWait(14, 0x10a, 172);
            Event_Wait(1);
            *(u8 *)(Func_02001828(14) + 90) |= 1;
            Event_Wait(10);
            Actor_SetAnimationAndWait(14, 3);
            Event_ShowMessageAndWait(14, 0, 10);
            Message_ShowCentered(MSG_JILL_GAVE_ROBIN_SPECIAL_GIFT, 1);
            bump_step(1);
            Item_ShowFound(ITEM_HARD_NUT, 3);
            Party_GiveItem(ITEM_HARD_NUT, 0);
            Actor_SetAnimationAndWait(14, 3);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
            Actor_SetSpeed(14, 0x10000, 0x8000);
            *(u8 *)(Func_020018a2(14) + 90) &= 254;
            Actor_WalkToAndWait(14, 0x106, 156);
            Event_Wait(1);
            {
                u8 *record = Actor_Get(14);
                u8 value = record[90] | 1;

                record[90] = value;
            }
            Event_Wait(20);
            Actor_RunRepeatedMotion(12, 2);
            Event_ShowMessageAndWait(12, 0, 10);
            Actor_SetAnimation(12, 3);
            Actor_SetAnimation(13, 3);
            Actor_SetAnimationAndWait(14, 3);
            base5_20091c0 = (s32)Data_020091c0;
            Call3(Func_02001960, 12, 0x10000, base5_20091c0);
            Call3(Func_0200196c, 13, 0x10000, base5_20091c0);
            Call3(Func_02001978, 14, 0x10000, base5_20091c0);
            GameFlag_Set(0x849);
            Event_End();
        }
    }
}

s32 Scene_DispatchPuzzleEvent(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    if (gGameState.scene == (s32)&Value_0000001e) {
        FieldScene_RunScene38b_020008f0();
    } else {
        if (gGameState.scene == (s32)&Value_00000023) {
            FieldScene_RunScene38bSequenceA();
            Call2(Func_020018e2, 0x2008ed9, 0xc80);
        } else {
            if (gGameState.scene == (s32)&Value_00000020) {
                FieldScene_RunScene38b_02000d10();
            }
        }
    }
    return 0;
}

void FieldScene_RunScene38b_020008f0(void)
{

    struct FieldActor *record;
    s16 sub_state;

    if (GameFlag_IsSet(0x845) != 0) {
        ((void (*)())Engine_ActorSetPosition)(9, 0, 0);
        Actor_FaceDirection(14, 0x3000, 0);
        Actor_FaceDirection(15, 0x5000, 0);
    } else {
        record = Actor_Get(9);
        Actor_SetSpriteFlags(record, 0);
        Actor_SetPosition(21, 0, 0);
    }
    record = Actor_Get(8);
    record->scale_y = 0x18000;
    {
        s32 off = 450;
        sub_state = *(s16 *)((u8 *)Data_02000240 + off);
    }
    if (sub_state == 10) {
        Actor_SetPosition(8, 0, 0);
    } else {
        if (sub_state == 9) {
            GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
        }
    }
    if (GameFlag_IsSet(0x109) == 0) {
        {
            s32 off = 450;
            sub_state = *(s16 *)((u8 *)Data_02000240 + off);
        }
        if (sub_state == 11) {
            Actor_SetPosition(20, 0xf80000, 0xd80000);
        }
    }
    Scene_UpdatePuzzleActors();
    if (GameFlag_IsSet(0x84a) != 0) {
        if (GameFlag_IsSet(0x84b) == 0) {
            GameFlag_Set(0x304);
        }
    }
}

void Scene_UpdatePuzzleActors(void)
{
    s32 p10;
    s32 p9;
    s32 rec7;
    s32 record;
    s32 p6;
    s32 row;

    rec7 = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    record = Value1(Engine_ActorGet, 20);
    row = *(s32 *)(record + 16) >> 20;
    p9 = (*(s32 *)(rec7 + 8) >> 20);
    p10 = (*(s32 *)(rec7 + 16) >> 20);
    p6 = *(s32 *)(record + 8);
    Map_CopyCellAttributes(15, 11, 3, 1, 15, 12);
    Map_CopyCellAttributes(15, 11, 3, 1, 15, 13);
    Map_CopyCellAttributes(15, 11, 3, 1, 15, 14);
    Map_CopyCellAttributes(1, 0, 1, 1, (p6 >> 20), row);
    if (((s32)p6 >> 20) == 16) {
        if (row == 13) {
            goto L_02000a60;
        }
    }
    Map_CopyCellAttributes(0, 0, 1, 1, 16, 13);
    L_02000a60:;
    if (p9 == 16) {
        if (p10 == 13) {
            Event_Begin();
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 20);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
            Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
            if (row == 13) {
                Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x106, 196);
                Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 20);
            } else {
                Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x11e, 218);
                Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
            }
            Event_End();
        }
    }
}

void FieldScene_RunScene38bSequenceA(void)
{
    u32 i;
    struct FieldActor *rec;
    s32 rec7;
    struct FieldActor *rec8;
    s32 record;

    rec8 = Value1(Engine_ActorGet, 10);
    rec = Value1(Engine_ActorGet, 11);
    record = Actor_Get(8);
    Actor_SetSpriteFlags(record, 0);
    rec7 = GameFlag_IsSet(0x845);
    if (rec7 != 0) {
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(10, 0, 0);
        Actor_SetPosition(11, 0, 0);
        Map_CopyCellsTo(56, 15, 40, 15, 1, 2);
        Map_CopyCellAttributes(26, 15, 1, 3, 10, 15);
        if (GameFlag_IsSet(0x849) == 0) {
            if (GameFlag_IsSet(0x848) != 0) {
                goto L_02000c92;
            }
            ((void (*)())Engine_ActorSetPosition)(14, 0, 0);
        }
        Actor_FaceDirection(12, 0xd000, 0);
        Actor_FaceDirection(13, 0xb000, 0);
    } else {
        Actor_SetPosition(12, 0, 0);
        Actor_SetPosition(13, 0, 0);
        Actor_SetPosition(14, 0, 0);
        record = Actor_Get(9);
        Actor_SetSpriteFlags(record, 0);
        record = Actor_Get(10);
        Actor_SetSpriteFlags(record, 0);
        record = Actor_Get(11);
        Actor_SetSpriteFlags(record, 0);
        rec8->motion_flags = rec7;
        record = GameFlag_IsSet(0x881);
        if (record != 0) {
            *(u8 *)(Func_02001cb2(9) + 89) |= 16;
            *(u8 *)(Func_02001cc2(16) + 89) |= 16;
            *(u8 *)(Func_02001cd0(11) + 89) |= 16;
            Actor_SetPosition(16, 0x8e0000, 0x9c0000);
            record = Actor_Get(16);
            Actor_SetSpriteFlags(record, 0);
            Actor_SetPosition(10, 0x8e0000, 0x9c0000);
            rec8->sprite->rotation = 0x4000;
            rec8->y.fixed += -0x80000;
            if (GameFlag_IsSet(0x848) != 0) {
                Actor_SetPosition(11, 0x840000, 0xba0000);
                goto L_02000c92;
            }
            Actor_SetPosition(11, 0x580000, 0xc40000);
            Actor_SetSpritePriority(11, 3);
            rec->collision_flags |= 4;
        } else {
            rec8->y.fixed = 0x200000;
            rec->motion_flags = record;
            rec->y.fixed = 0x300000;
        }
    }
    L_02000c92:;
    ActorPresentation_RepaintTenCellsAndActorEightCell();
}

/*
 * Ten (x, z) tile pairs, held in the overlay's own writable image.  Overlay
 * data lives in EWRAM and is deliberately not const.
 */

/*
 * Slot accessor: Engine_ActorGet(slot) returns the actor record, or NULL.
 * Typed as a byte pointer so the +0x08 and +0x10 field reads are explicit.
 */

/*
 * The six-argument renderer ABI: four register arguments plus two stack
 * words, here the tile x and tile z of the cell being repainted.  The two
 * names are separate per-site call words that reach the same renderer.
 */

/*
 * Repaint ten fixed collision cells and then actor 8's own cell.  The
 * 92-byte owner includes the alignment halfword and the single pool word
 * that follows the code; that word holds the address of Data_02009d28, which
 * is in-image data rather than a RAM global.  The two renderer calls must
 * keep their separate call words -- naming one renderer for both changes the
 * displacement emitted at each site.
 */
void ActorPresentation_RepaintTenCellsAndActorEightCell(void)
{
    u8 *actor;
    s32 tx;
    s32 tz;
    u32 i;

    /*
     * Slot 8 is the scene's own actor; the accessor result is not
     * null-checked here.  The 20-bit shift is one signed arithmetic shift:
     * 16 takes the fixed-point coordinate to pixels, the further 4 take it
     * to the 16-pixel tile grid.
     */
    actor = Actor_Get(8);
    tx = *(s32 *)(actor + 0x08) >> 20;
    tz = *(s32 *)(actor + 0x10) >> 20;

    /*
     * Ten fixed cells from the table, then the actor's own cell.  The table
     * is walked by the byte index itself rather than by a 0..9 counter
     * scaled by two, so the loop steps the byte offset directly.
     */
    for (i = 0; i < 20; i += 2) {
        s32 x = (s32)Data_02009d28[i];
        s32 z = (s32)Data_02009d28[i + 1];
        Map_CopyCellAttributes(1, 0, 1, 1, x, z);
    }

    /*
     * The same repaint with 0 rather than 1 in the first argument.  What
     * that selector chooses is not established.
     */
    Map_CopyCellAttributes(0, 0, 1, 1, tx, tz);
}

void FieldScene_RunScene38b_02000d10(void)
{
    s32 arg0;
    s32 rec7;
    s32 record;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    FieldScene_DrawTilesByActor8Row();
    record = ReadU16Elem((u16 *)Data_02000240, 225);
    if ((u32)((record - 3) << 16) <= 0x10000) {
        if (GameFlag_IsSet(0x109) == 0) {
            rec7 = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
            Event_Begin();
            arg0 = *(s32 *)(rec7 + 8);
            *(s32 *)(rec7 + 12) = 0x100000;
            Camera_MoveTo(arg0, 0x100000, *(s32 *)(rec7 + 16), 0);
            Map_Redraw();
            Event_End();
            Task_Wait(1);
        }
    }
}

s32 *SceneActor_FindAtTileXZ(s32 x, s32 z)
{
    extern u8 *Data_03001ebc;

    s32 **tbl = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = tbl[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void OverlayObject_SpawnKind24AtActor(u8 *actor)
{
    u8 *obj;
    u8 *child;
    s32 mask;

    obj = Func_02001f90(24,
                            *(s32 *)(actor + 0x08),
                            *(s32 *)(actor + 0x0c),
                            *(s32 *)(actor + 0x10));
    if (obj == 0)
        return;

    child = *(u8 **)(obj + 0x50);
    Object_SetScript(obj, Data_02009d7c);
    *(u8 *)(obj + 0x55) = 0;
    *(u8 *)(obj + 0x22) = 1;
    *(u8 *)(obj + 0x23) = 2;
    if (child == 0)
        return;

    Func_02001fa8(child, 2);
    *(u8 *)(child + 0x26) = 0;
    mask = 13;
    mask = -mask;
    *(u8 *)(child + 5) = (u8)((*(u8 *)(child + 5) & mask) | 4);
    *(u8 *)(child + 9) |= 12;
}

void FieldScene_DrawTilesByActor8Row(void)
{
    u8 *actor;
    actor = Actor_Get(8);
    if (actor == 0)
        return;

    {
        s32 tile_z = *(s32 *)(actor + 0x10) >> 20;
        if (tile_z == 6)
            Map_CopyCellAttributes(2, 0, 1, 1, 14, 6);
        else {
            s32 x = 14;
            s32 z = 6;
            Map_CopyCellAttributes(0, 0, 1, 1, x, z);
        }
    }

    {
        s32 tile_z = *(s32 *)(actor + 0x10) >> 20;
        if (tile_z == 9)
            Map_CopyCellAttributes(2, 0, 1, 1, 14, 9);
        else {
            s32 x = 14;
            s32 z = 9;
            Map_CopyCellAttributes(1, 0, 1, 1, x, z);
        }
    }
}
