/* The Suhara gate's scene start: once flag 0x89f is set the gate returns
 * the party to Lunpa's Suhara side at entrance 10; the first scene sets the
 * retreat point and the passage cells by the entrance used, and the second
 * poses the gate crew. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"

s32 SuharaGate_EnterScene(void)
{
    s32 scene;

    if (Engine_GameFlagIsSet(0x89f)) {
        gGameState.saved_scene = (s32)&SceneId_RunpaSuhara;
        gGameState.saved_entrance = 10;
    }
    scene = gGameState.scene;
    if (scene == (s32)&SceneId_SuharaGate1) {
        if (Engine_GameFlagIsSet(0x897))
            Engine_ActorSetPosition(10, 0, 0);
        if (gGameState.entrance == 3) {
            if (Engine_GameFlagIsSet(0x8fb)) {
                gGameState.retreat_scene = scene;
                gGameState.retreat_entrance = 1;
            }
            if (Engine_GameFlagIsSet(0x8fc)) {
                gGameState.retreat_scene = scene;
                gGameState.retreat_entrance = 5;
            }
            Engine_GameFlagClear(0x12f);
        }
        if (gGameState.entrance == 1) {
            Engine_GameFlagSet(0x8fb);
            if (!Engine_GameFlagIsSet(0x96f))
                Map_CopyCellAttributes(6, 0, 2, 1, 8, 27);
        }
        if (gGameState.entrance == 5)
            Engine_GameFlagSet(0x8fc);
    } else if (scene == (s32)&SceneId_SuharaGate2) {
        Engine_ActorSetAnimation(8, 4);
        Engine_ActorSetAnimation(9, 4);
        Engine_ActorSetAnimation(10, 3);
        Engine_ActorSetAnimation(11, 4);
        Engine_ActorSetAnimation(12, 3);
        Engine_ActorGet(15)->scale_y = 0x19999;
        Map_CopyCellAttributes(108, 38, 1, 1, 102, 56);
    }
    return 0;
}
