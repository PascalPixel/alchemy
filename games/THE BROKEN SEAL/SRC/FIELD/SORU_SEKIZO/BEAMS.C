#include "STATUE_HALL.H"

void Scene_ShineLeftBeam(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x818) == 0) {
        if (GameFlag_IsSet(FLAG_LEFT_BEAM_SHINING) == 0) {
            Camera_SetSpeed(0x20000, 0x4000);
            Camera_MoveTo(0x11e0000, -1, 0x920000, 1);
            Camera_WaitForMove();
            Audio_PlayCue(186);
            Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
            if (GameFlag_IsSet(FLAG_RIGHT_BEAM_SHINING) != 0) {
                Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
            }
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
            Event_Wait(30);
            GameFlag_Set(FLAG_LEFT_BEAM_SHINING);
            if (GameFlag_IsSet(FLAG_RIGHT_BEAM_SHINING) != 0) {
                Scene_OpenTheHole();
            }
        }
    }
    Event_End();
}

void Scene_ShineRightBeam(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x818) == 0) {
        if (GameFlag_IsSet(FLAG_RIGHT_BEAM_SHINING) == 0) {
            Camera_SetSpeed(0x20000, 0x4000);
            Camera_MoveTo(0x11e0000, -1, 0x920000, 1);
            Camera_WaitForMove();
            Audio_PlayCue(186);
            Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
            if (GameFlag_IsSet(FLAG_LEFT_BEAM_SHINING) != 0) {
                Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
            }
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
            Event_Wait(30);
            GameFlag_Set(FLAG_RIGHT_BEAM_SHINING);
            if (GameFlag_IsSet(FLAG_LEFT_BEAM_SHINING) != 0) {
                Scene_OpenTheHole();
            }
        }
    }
    Event_End();
}

/* Shows the next line of dialogue, then holds the scene for a moment. */
void Event_SayThenWait(s32 speaker, s32 frames)
{
    Event_ShowMessage(speaker, 0);
    Event_Wait(frames);
}
