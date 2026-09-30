#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"
extern u8 FuneKanpan_LeadActionsB[];
extern u8 FuneKanpan_LeadActionsC[];
extern u8 MsgFuneCanSeeLand[];
extern u8 MsgFuneThankRobinDidGoodAgainst[];
extern u8 FuneKanpan_CrewScript[];
/* FAKEMATCH: calls that cast Object_GetById to another return type keep their original register order. */
s32 Object_GetById();
/* FAKEMATCH: calls that cast FieldScene_RunStepThen10 to another return type keep their original register order. */
void FieldScene_RunStepThen10();
/* FAKEMATCH: calls that cast FieldScene_CallPairWith10 to another return type keep their original register order. */
void FieldScene_CallPairWith10();

#define SCENE_PHASE (*(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0))

union Slot {
    s32 w;
    s16 h[2];
};

s32 BuildMotionCountdown(s32, s16);

void Event_CallWithLastActiveObjectId();

void FieldScene_RunScene3af_02000bb8(void);

/* Configures actors 20, 21, 22 and 23 (position, pose, and movement/sprite
 * flags) and advances the shared scene phase before the scene runs. */
void FieldScene_ConfigureLeadActors(void)
{
    u8 *record;

    Event_Begin();
    Call1(Event_CallWithLastActiveObjectId, (u32)FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetPosition(20, 0xb60000, 0x26a0000);
    Actor_SetPosition(23, 0xee0000, 0x2720000);
    Actor_SetPosition(22, 0x10c0000, 0x2a60000);
    record = ((u8 *(*)())Object_GetById)(22);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Actor_EnableActionCallback(22, FuneKanpan_LeadActionsC);
    {
        /* Set the high bit of the flag byte at +89. */
        u8 *record = ((u8 *(*)())Object_GetById)(21);
        u8 bits = 128;

        bits |= record[89];
        record[89] = bits;
    }
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_EnableActionCallback(21, FuneKanpan_LeadActionsB);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetSpeed(20, 0x19999, 0xcccc);
    Actor_WalkToAndWait(20, 182, 0x224);
    FieldScene_CallPairWith10(20, 0);
    Value2(FieldScene_CallPairWith10, 0, 0x8000);
    Actor_RunRepeatedMotion(20, 1);
    Event_SetMessage((s32)MsgFuneThankRobinDidGoodAgainst);
    ((void (*)())FieldScene_RunStepThen10)(20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(40);
    Actor_FaceDirection(20, 0x5000, 20);
    Actor_ShowEmote(20, 0x105, 60);
    Event_ShowMessageAndWait(20, 0, 40);
    FieldScene_CallPairWith10(20, 0);
    FieldScene_RunStepThen10(20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(20, 3);
    Actor_WalkToAndWait(20, 182, 0x258);
    Actor_WalkToAndWait(20, 216, 0x258);
    Call2(FieldScene_CallPairWith10, 20, 0xc000);
    FieldScene_RunScene3af_02000bb8();
    Event_Wait(10);
    Actor_WalkToAndWait(20, 216, 0x244);
    Actor_SetPosition(20, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    GameFlag_Set(0x92b);
    GameFlag_Clear(0x302);
    Event_End();
}

/* Configures actors 20, 21 and 22 (position and movement/sprite flags) and
 * advances the shared scene phase before the scene runs. */
void FieldScene_ConfigureThreeActors(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = Object_GetById(0);
    Actor_SetSpriteFlags(record, 0);
    Call1(Event_CallWithLastActiveObjectId, (u32)FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetPosition(20, 0xc40000, 0x1f60000);
    record = Object_GetById(20);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xa000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Actor_SetPosition(22, 0xb80000, 0x20c0000);
    record = Object_GetById(22);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Value2(Engine_ActorSetSpritePriority, 21, 1);
    Actor_SetPosition(21, 0xb80000, 0x2780000);
    record = Object_GetById(21);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    SCENE_PHASE = 0x202;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_Jump(22, 4, 10);
    Actor_Jump(22, 6, 20);
    Event_SetMessage((s32)MsgFuneCanSeeLand);
    FieldScene_RunStepThen10(22);
    Actor_SetAnimationAndWait(20, 3);
    Actor_SetSpeed(21, 0x30000, 0x18000);
    Actor_WalkToAndWait(21, 180, 0x222);
    Value3(Engine_ActorFaceDirection, 21, 0xb000, 40);
    Actor_RunRepeatedMotion(21, 1);
    FieldScene_RunStepThen10(21);
    Event_RequestExit(15);
}
