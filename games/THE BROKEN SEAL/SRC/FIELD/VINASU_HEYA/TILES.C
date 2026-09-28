#include "ENTRY_SETUP.H"

void FieldScene_DrawTilesWhenCheckClear(void)
{

    Event_Begin();
    if (SceneActor_TryMoveActorZeroTwoTilesAhead() == 0) {
        { s32 k5 = 5, k6 = 48; Map_CopyCellAttributes(69, 48, 4, 2, k5, k6); }
        { s32 j5 = 9, j6 = 37; Map_CopyCellAttributes(73, 37, 9, 13, j5, j6); }
        RunStagedActorTransition();
    }
    Event_End();
    Scene_RunScene3c8SequenceA();
}

/* Runs a fixed sequence of setup calls with literal parameters; most share
 * a leading 0 argument. */
void FieldScene_RunApproachAndSpawnEffect(void)
{
    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x208, 0x2c8);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
    (void)OverlayObject_SpawnWithMode14(0x2080000, 0, 0x3100000, 223);
    BattleFx_RunRisingObjectSequence(0, 6, 0);
    Event_Wait(60);
    Event_RequestExit(20); /* main:0808a248 */
    Event_End();
}
