#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgShianWarriorWelcome[];

/*
 * The Xian room's entry: the warrior's welcome, the scene table for the
 * entrance, and the objects the entrance sets up.
 */


/* The scene's tables, in the overlay's read-only data. */
extern u8 ShianHeya_SceneTable3[];
extern u8 ShianHeya_SceneTable3Entrance8[];

void *OverlayObject_CreateConfigured(s32 x, s32 y, s32 z, s32 type);

void SceneDialogue_RunActor9MotionDialogue(void)
{
    void Event_SetMessage(s32);

    Event_Begin();
    Event_SetMessage((s32)MsgShianWarriorWelcome);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceActor(9, 10, 0);
    Event_Wait(60);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Event_ShowMessage(9, 0);
    Event_End();
}

s32 SceneData_SelectTable8d4cOr8a28(void)
{
    if (gGameState.entrance == 8) {
        return (s32)ShianHeya_SceneTable3Entrance8;
    }
    return (s32)ShianHeya_SceneTable3;
}

s32 SceneState_SetRuntimeWord448To521(void)
{

    s16 scene;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    scene = gGameState.entrance;
    if (scene == 4 || scene == 7) {
        OverlayObject_CreateConfigured(0x00f80000, 0, 0x01a10000, 20);
    } else if (scene == 6) {
        OverlayObject_CreateConfigured(0x01cc0000, 0, 0x02380000, 20);
        OverlayObject_CreateConfigured(0x01e40000, 0, 0x02380000, 20);
    } else if (scene == 8) {
        GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
        Actor_SetAnimation(10, 6);
    }
    return 0;
}
