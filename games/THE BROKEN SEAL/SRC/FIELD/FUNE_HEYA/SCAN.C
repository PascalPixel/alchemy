#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "HEYA.H"

#include "STAGED_ACTOR.H"
extern u8 MsgFuneSorryEveryoneButWeNeed[];

struct SceneActor {
    u8 pad00[99];
    u8 mode;
};

struct EffectRecord {
    u8 pad00[6];
    u16 angle;
    u8 pad08[83];
    u8 state;
    u8 pad5c[6];
    u8 active;
};
s32 FuneHeya_FindFirstSetFlag();

/* Scene-step dispatcher for overlay resource 0x3b1.
 *
 * The owner takes a step selector plus two step parameters and jumps through a
 * 26-entry table into one bounded block of scene setup calls per step. Step 18
 * finishes by re-entering the dispatcher with step 14, which the compiler turns
 * into a jump back to the range check.
 *
 * Uncertain: the roles of the two parameters differ per step (actor slot,
 * count, flag, upper loop bound), so they keep neutral names here. The record
 * fields written at +6 (halfword) and +24 (word) are the same scene-object
 * fields the neighbouring scene sources touch; their meaning is not recovered.
 */

/*
 * Field scene beat for overlay resource_3b1.  Each callee is named for its own
 * call site: every call reaches its target through its own local veneer, even
 * where the same logical callee is used from more than one site.
 */

void FieldScene_CallPairWith10(s32 a, u16 b);

void SceneState_ScanTwoArraysAndCrossNotify(u8 *a, u8 *b);

void FieldScene_RunStepThen10(s32 a);
void OverlayObject_SetPositionAndHeading(void *a, s32 b, s32 c, s32 d);
void FieldScene_RunSceneStep(s32 step, u32 arg, u32 opt);

void FieldScene_RunScene3b1_02006110(void)
{
    s32 rec2;
    s32 rec4;
    s32 rec7;
    s32 rec8;

    rec2 = FuneHeya_FindFirstSetFlag(0, 0);
    rec8 = FuneHeya_FindFirstSetFlag(1, 0);
    rec7 = FuneHeya_FindFirstSetFlag(2, 0);
    rec4 = FuneHeya_FindFirstSetFlag(3, 0);
    Event_Begin();
    FieldScene_RunSceneStep(10, 0, 0);
    OverlayObject_SetPositionAndHeading(8, 0x1d8, 144, 0x5000);
    OverlayObject_SetPositionAndHeading(27, 0x198, 142, 0x3000);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(27, 1);
    Event_SetMessage((s32)MsgFuneSorryEveryoneButWeNeed);
    FieldScene_RunStepThen10(27);
    Actor_StartRepeatedMotion(rec2, 2);
    Actor_StartRepeatedMotion(rec8, 2);
    Actor_StartRepeatedMotion(rec7, 2);
    Actor_RunRepeatedMotion(rec4, 2);
    Event_Wait(20);
    Actor_FaceDirection(rec2, 0, 0);
    Actor_FaceDirection(rec8, 0x8000, 0);
    Actor_FaceDirection(rec7, 0, 0);
    Actor_FaceDirection(rec4, 0x8000, 40);
    Actor_SetSpeed(rec2, 0x10000, 0x8000);
    Actor_SetSpeed(rec8, 0x10000, 0x8000);
    Actor_SetSpeed(rec7, 0x10000, 0x8000);
    Actor_SetSpeed(rec4, 0x10000, 0x8000);
    Actor_WalkTo(rec2, 0x1d6, 172);
    Actor_WalkTo(rec8, 0x19a, 172);
    Actor_WalkTo(rec7, 0x1d6, 204);
    Actor_WalkToAndWait(rec4, 0x19a, 204);
    Actor_SetAnimation(rec2, 1);
    Actor_SetAnimation(rec8, 1);
    Actor_SetAnimation(rec7, 1);
    Actor_FaceDirection(rec8, 0xd000, 0);
    Actor_FaceDirection(rec2, 0xb000, 0);
    Actor_FaceDirection(rec4, 0xd000, 0);
    Actor_FaceDirection(rec7, 0xb000, 20);
    Actor_RunRepeatedMotion(27, 1);
    FieldScene_RunStepThen10(27);
    Actor_SetAnimation(rec2, 3);
    Actor_SetAnimation(rec8, 3);
    Actor_SetAnimation(rec7, 3);
    Actor_SetAnimationAndWait(rec4, 3);
    FieldScene_RunStepThen10(27);
    Actor_SetAnimation(rec2, 3);
    Actor_SetAnimation(rec8, 3);
    Actor_SetAnimation(rec7, 3);
    Actor_SetAnimationAndWait(rec4, 3);
    Actor_FaceDirection(27, 0, 0);
    FieldScene_CallPairWith10(0, 0x8000);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(27, 3);
    Actor_SetSpeed(27, 0x10000, 0x8000);
    Actor_WalkToAndWait(27, 0x198, 132);
    Actor_WalkToAndWait(27, 0x1bc, 132);
    Actor_SetPosition(27, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Event_CloseScreen();
    Event_WaitForScreen();
    SceneState_ScanTwoArraysAndCrossNotify(0x92c, 0x935);
    SceneState_ScanTwoArraysAndCrossNotify(0x917, 0x990);
    GameFlag_Clear(0x8a0);
    Event_RequestExit(10);
}

/*
 * Runs two first-match linear scans over indices 0 to 8, each breaking on its
 * first hit and calling a per-element handler, then cross-pairs the miss
 * counts: the count from scanning `a` indexes into `b`, and the count from
 * scanning `b` indexes into `a`.  Each callee is named for its own call site,
 * because every call reaches its target through its own local veneer and two
 * of the sites share one veneer.
 */
void SceneState_ScanTwoArraysAndCrossNotify(u8 *a, u8 *b)
{
    s32 cnt_a = 0;
    s32 cnt_b = 0;
    u32 i;

    for (i = 0; i <= 8; i++) {
        u8 *p = a + i;
        if (GameFlag_IsSet(p)!= 0) {
            GameFlag_Clear(p);
            break;
        }
        cnt_a++;
    }

    for (i = 0; i <= 8; i++) {
        u8 *p = b + i;
        if (GameFlag_IsSet(p)!= 0) {
            GameFlag_Clear(p);
            break;
        }
        cnt_b++;
    }

    GameFlag_Set(b + cnt_a);
    GameFlag_Set(a + cnt_b);
}
