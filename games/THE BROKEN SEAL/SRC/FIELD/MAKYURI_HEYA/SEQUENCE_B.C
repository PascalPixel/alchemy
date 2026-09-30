/* In the first room the leader walks to the stair, turns, and sinks out of
 * sight through the opened floor before the party leaves by exit 8; in the
 * others the leader only rises. */
/* The rising sequence goes through Call3, which sets r0 and r1 before
 * negating the third argument, as the game does; a direct call negates it
 * first. */
#include "PROBE.H"
#include "FIELD_SCENE.H"
#include "CALL.H"

void BattleFx_RunRisingObjectSequence();

void FieldScene_RunScene39cSequenceB(void)
{
    Event_Begin();
    if (gGameState.scene == (s32)&SceneId_MakyuriHeya1) {
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1d8, 0x258);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
        Camera_MoveTo(0x1d00000, -1, 0x2900000, 1);
        Actor_SetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
        OverlayObject_PrepareSpawnedObject(Object_GetById(ACTOR_PARTY_LEADER)->x.fixed, 0, 0x2be0000, 223);
        Map_CopyCellsTo(92, 46, 92, 40, 3, 2);
        *(s32 *)&Object_GetById(ACTOR_PARTY_LEADER)->unknown_44[4] = 0x8000;
        Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
        Call3(BattleFx_RunRisingObjectSequence, ACTOR_PARTY_LEADER, 6, -1);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
        Event_Wait(60);
        Event_RequestExit(8);
    } else {
        Call3(BattleFx_RunRisingObjectSequence, ACTOR_PARTY_LEADER, 6, -1);
    }
    Event_End();
}
