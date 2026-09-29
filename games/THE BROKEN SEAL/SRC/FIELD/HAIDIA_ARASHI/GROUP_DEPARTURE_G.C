#include "GROUP_DEPARTURE.H"
extern u8 MsgHaidiaICantMoveGetHelp[];
extern u8 MsgHaidiaIllGo[];
extern u8 MsgHaidiaRobin[];

void ActorPresentation_SelectActorTwentySevenState(void)
{
    struct Actor *actor = Actor_Get(27);
    u32 flags = gFrameCount;
    u8 *presentation = actor->presentation;

    if (flags & 1) {
        u8 *state = presentation + 35;
        *state = 2;
    } else {
        u8 *state = presentation + 35;
        *state = 64;
    }
}

void FieldScene_RunScene372_02003e48(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec8 = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    rec7 = Value1(Engine_ActorGet, 8);
    Event_Begin();
    if (GameFlag_IsSet(0x305) != 0) {
        Actor_Stop(8);
        Event_Wait(10);
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Actor_SetAnimation(8, 7);
        } else {
            Actor_SetAnimation(8, 8);
        }
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(20);
        Event_SetMessage((s32)MsgHaidiaICantMoveGetHelp);
        Event_ShowMessage(8, 0);
        Value2(Engine_ActorEnableActionCallback, 8, (s32)HaidiaArashi_ActorEightScript);
        Actor_SetAnimation(8, 6);
    } else {
        Actor_Stop(8);
        *(s32 *)(rec7 + 24) = 0x10000;
        *(s32 *)(rec7 + 28) = 0x10000;
        Actor_FaceDirection(8, 0x1000, 0);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Actor_SetAnimation(8, 7);
        } else {
            Actor_SetAnimation(8, 8);
        }
        Event_Wait(20);
        Event_SetMessage((s32)MsgHaidiaIllGo);
        Event_ShowMessageAndWait(8, 0, 20);
        Actor_SetAnimation(8, 1);
        Actor_Jump(8, 4, 0);
        Event_Wait(80);
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Actor_SetAnimation(8, 7);
        } else {
            Actor_SetAnimation(8, 8);
        }
        Event_Wait(2);
        Actor_Jump(8, 2, 0);
        Event_Wait(60);
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(20);
        Event_ShowMessage(8, 0);
        Value2(Engine_ActorEnableActionCallback, 8, (s32)HaidiaArashi_ActorEightScript);
        Actor_SetAnimation(8, 6);
        GameFlag_Set(0x305);
    }
    Event_End();
}

void FieldScene_ConfigureActorTwentyTwoScene(void)
{
    u32 i;
    u8 *record;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Actor_Stop(ACTOR_ID);
    Call1(Scheduler_RemoveCallback, (s32)OverlayObject_CopyRecordField1ToSlots22And8);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1e0, 0x570);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_ID, 0x3000, 20);
    Actor_Get(ACTOR_ID)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetPosition(ACTOR_ID, 0xf90000, 0x4d80000);
    Task_Wait(1);
    Event_SetMessage((s32)MsgHaidiaRobin);
    Event_ShowMessage(0x1016, 0);
    Actor_SetPosition(ACTOR_ID, 0xac0000, 0x4fe0000);
    Task_Wait(1);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0xa20000, 0, 0x5050000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_ID, 4);
    Event_ShowMessageAndWait(0x1016, 0, 10);
    Actor_FaceDirection(ACTOR_ID, 0xc000, 20);
    Actor_RunRepeatedMotion(ACTOR_ID, 2);
    Event_ShowMessageAndWait(0x1016, 0, 10);
    Actor_FaceDirection(ACTOR_ID, 0x1000, 20);
    Actor_SetAnimationAndWait(ACTOR_ID, 3);
    Actor_SetSpeed(ACTOR_ID, 0x20000, 0x10000);
    Actor_WalkToAndWait(ACTOR_ID, 165, 0x514);
    Actor_WalkToAndWait(ACTOR_ID, 195, 0x598);
    GameFlag_Set(0x842);
}

/* Each of the 15 placement calls below takes the same 6-argument shape:
 * two coordinate-like values, two more coordinate-like values, and a
 * trailing pair of small counts. The final call takes no arguments. */
void FieldScene_BuildPlacementGrid(void)
{
    u32 i;
    u8 *record;

    Map_CopyCellsTo(16, 96, 11, 73, 6, 3); /* main:08009180 */
    Map_CopyCellsTo(16, 96, 34, 68, 14, 10); /* main:08009180 */
    Map_CopyCellsTo(16, 96, 64, 68, 7, 7); /* main:08009180 */
    Map_CopyCellsTo(9, 95, 11, 73, 6, 3); /* main:08009180 */
    Map_CopyCellsTo(40, 94, 34, 68, 14, 10); /* main:08009180 */
    Map_CopyCellsTo(54, 94, 64, 68, 8, 7); /* main:08009180 */
    Map_CopyCellsTo(72, 75, 72, 76, 1, 1); /* main:08009180 */
    Map_CopyCellsTo(72, 75, 74, 76, 1, 1); /* main:08009180 */
    Map_CopyCellAttributes(7, 75, 1, 1, 6, 75); /* main:080091c0 */
    Map_CopyCellAttributes(8, 70, 3, 1, 8, 71); /* main:080091c0 */
    Map_CopyCellAttributes(8, 70, 2, 1, 9, 72); /* main:080091c0 */
    Map_CopyCellAttributes(8, 70, 2, 1, 9, 73); /* main:080091c0 */
    Map_CopyCellAttributes(11, 66, 1, 1, 8, 73); /* main:080091c0 */
    Map_CopyCellAttributes(12, 66, 1, 4, 11, 73); /* main:080091c0 */
    Map_CopyCellAttributes(25, 0, 1, 1, 6, 74); /* main:080091c0 */
    /* No-argument call that closes out the sequence started above. */
    Map_Redraw(); /* main:08009128 */
}

void SceneState_SetWords1c0And1c8AndRun(void)
{

    u8 *state;

    Event_Begin();
    state = (u8 *)gEventWork;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    GameFlag_Set(0x87c);
    BattleFx_SetWeightedResult(12, 2);
    GameFlag_Set(0x900);
    Event_End();
}

void SceneState_SetWorkWordsAndFlag87f(void)
{

    u8 *state;

    Event_Begin();
    state = (u8 *)gEventWork;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    GameFlag_Set(0x87f);
    BattleFx_SetWeightedResult(12, 3);
    GameFlag_Set(0x900);
    Event_End();
}

void SceneActor_SetModeByFrameBit1(s32 o)
{
    s32 v;

    if ((*(volatile s32 *)&gFrameCount & 2) != 0) {
        Object_SetPartPalettes(o, 7);
    } else {
        Object_SetPartPalettes(o, 0);
    }
    {
        volatile s32 *q = (volatile s32 *)&gFrameCount;
        v = (HaidiaArashi_ShakeShift << 3) + 16;
        if (IwramUnsignedRemainder(*q, v) == 0) {
            HaidiaArashi_SpawnEffectPair(o);
        }
    }
}

void OverlayObject_UpdateRandomSlotByFrame(s32 obj)
{
    volatile s32 *fc = (volatile s32 *)&gFrameCount;
    s32 t;
    s32 n;

    if ((*fc & 1) != 0) {
        t = (s32)((u32)*fc >> 1);
        Object_SetPartPalettes(obj, IwramUnsignedRemainder(t, 6));
    }
    n = (HaidiaArashi_ShakeShift << 3) + 16;
    if (IwramUnsignedRemainder(*fc, n) == 0) {
        HaidiaArashi_SpawnEffectPair(obj);
    }
}

void OverlayObject_ApplyIwramWord1e40(s32 o)
{
    volatile s32 *p = (s32 *)&gFrameCount;
    s32 t;

    if ((*p & 1) != 0) {
        t = (s32)((u32)*p >> 1);
        Object_SetPartPalettes(o, IwramUnsignedRemainder(t, 6));
    }
}

void SceneEffect_UpdateArcOverAnchor(Obj *o)
{
    Obj *b;
    s32 t;
    s32 d;
    s32 k;

    b = o->f68;
    o->f64++;
    t = (s16)o->f64;
    t = (s16)o->f64;
    if (t > 31) {
        Engine_ObjectDispatchRelease(o);
    } else {
        d = Math_Sin(t << 10);
        o->f18 = d;
        o->f1c = d;
        o->f08 = b->f08;
        k = 0x10000;
        o->f0c += k;
        k -= d;
        o->f10 = b->f10 + ((k << 2) + k) + 0x80000;
    }
}

void OverlayObject_UpdateArcFromParent(Obj *o)
{
    Obj *b;
    s32 t;
    s32 d;
    s32 k;

    b = o->f68;
    o->f64++;
    t = (s16)o->f64;
    t = (s16)o->f64;
    if (t > 31) {
        Engine_ObjectDispatchRelease(o);
    } else {
        d = Math_Sin(t << 10);
        o->f18 = d;
        o->f1c = -d;
        o->f08 = b->f08;
        k = 0x10000;
        o->f0c += k;
        k -= d;
        o->f10 = b->f10 - ((k << 2) + k) + 0x100000;
    }
}
