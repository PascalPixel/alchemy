#include "SHIAN.H"

void FieldScene_RunScene3a0_02000de8(s32 a0)
{
    u32 i;
    s32 record;

    *(u8 *)((u8 *)Engine_ActorGet(0) + 85) = 0;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    if (a0 == 6) {
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    } else {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
    }
    gEventWork->transition_frames = 16;
    Event_RequestExit(a0);
}

/*
 * The selector is halfword [182] of the event work,
 * guarded to the range 1..7.  The pointer is loaded before the first call and
 * held across all of them, so it is a function-top local.  Cases 2 and 3 set
 * the two shared arguments and jump into the middle of case 6 to share its
 * final call; the goto and the two locals are what reproduce that.
 */
void SceneEffect_DispatchStep(void)
{
    s16 *scene = (s16 *)gEventWork;
    u8 *shared0;
    s32 shared1;

    Event_Begin();

    switch (scene[182]) {
    case 1:
        Audio_PlayCue(158);
        Map_AnimateCells(ShianMura_GateSteps1, 81, 18);
        break;
    case 2:
        Audio_PlayCue(158);
        shared0 = ShianMura_GateSteps2;
        shared1 = 83;
        goto shared;
    case 3:
        Audio_PlayCue(158);
        shared0 = ShianMura_GateSteps2;
        shared1 = 86;
        goto shared;
    case 4:
        Audio_PlayCue(158);
        Map_AnimateCells(ShianMura_GateSteps3, 84, 24);
        break;
    case 5:
        Audio_PlayCue(158);
        Map_AnimateCells(ShianMura_GateSteps3, 72, 7);
        break;
    case 6:
        Audio_PlayCue(188);
        shared0 = ShianMura_GateSteps4;
        shared1 = 69;
    shared:
        Map_AnimateCells(shared0, shared1, 11);
        break;
    case 7:
        Audio_PlayCue(158);
        Map_AnimateCells(ShianMura_GateSteps5, 83, 7);
        break;
    default:
        break;
    }

    FieldScene_RunScene3a0_02000de8(scene[182]);
    Event_End();
}
