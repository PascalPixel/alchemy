/* The Lunpa fortress: a guard turns to the party and says one of his lines. */
#include "FORTRESS.H"

void TurnActorToSceneDirection(s32 actor_id)
{

    Actor_FaceActor(actor_id, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, actor_id, 0);
    switch ((s32)gRunpaJoRandomPick & 3) {
    case 0:
        RunActorScriptedSequenceB(actor_id);
        break;
    case 1:
        RunActorScriptedSequenceC(actor_id);
        break;
    case 2:
        FieldScene_RunScene3bf_02001cf0(actor_id);
        break;
    case 3:
        RunActorScriptedSequenceD(actor_id);
        break;
    default:
        RunActorScriptedSequenceC(actor_id);
        break;
    }
}
