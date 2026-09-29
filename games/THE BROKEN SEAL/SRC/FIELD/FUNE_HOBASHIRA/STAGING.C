#include "FUNE.H"

/* The IWRAM field globals: the map work first, the event work at +0x4c. */
struct FieldGlobals {
    s32 **map;
    u8 unknown_04[0x48];
    struct EventWork *event;
};

extern struct FieldGlobals gMapWork;

/* Places the staging objects and starts the lookout's actions, saves the
 * camera position as the origin actor 9's camera-following actions measure
 * from, grows actors 10 to 12 in and walks them to their places, then the
 * lookout walks and leaps before the scene exits. */
void Scene_RunFourActorStagingSequence(void)
{
    s32 *placement = *gMapWork.map;
    struct FieldActor *actor;
    struct EventWork **event;
    s32 x;
    s32 zero = 0;
    s32 scale;
    s32 update;

    Engine_EventBegin();
    Event_CallWithLastActiveObjectId((s32)FuneHobashira_StagingObjects);
    Engine_TaskWait(1);
    Engine_ActorSetChildValue(0, 15);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(0), 0);
    Engine_ActorEnableActionCallback(8, FuneHobashira_LookoutActions);
    event = &gMapWork.event;
    (*event)->start_transition = 0x203;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    FuneHobashira_CameraOrigin[0] = *placement++;
    FuneHobashira_CameraOrigin[1] = *placement;
    Call3((void (*)())Engine_ActorSetPosition, 9, 0x500000, 0xd20000);
    x = 0x500000;
    Engine_ActorGet(9)->motion_flags = zero;
    FuneHobashira_CameraCenter[0] = x;
    FuneHobashira_CameraCenter[1] = zero;
    Engine_ActorEnableActionCallback(9, FuneHobashira_FollowCameraActions);
    Engine_EventWait(20);
    Engine_AudioPlayCue(29);
    Engine_GameFlagSet(0x8f0);
    Engine_ActorStop(8);
    Engine_TaskWait(1);
    Call3((void (*)())Engine_ActorShowEmote, 8, 0x100, 0);
    Engine_ActorFaceDirection(8, 0xb000, 0);
    Engine_EventSetMessage(0x1e3e);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Call3((void (*)())Engine_ActorSetPosition, 10, x, 0xd20000);
    Call3((void (*)())Engine_ActorSetPosition, 11, x, 0xd20000);
    Call3((void (*)())Engine_ActorSetPosition, 12, x, 0xd20000);
    Engine_ActorSetSpritePriority(10, 3);
    Engine_ActorSetSpritePriority(11, 3);
    Engine_ActorSetSpritePriority(12, 3);
    Engine_ActorSetChildValue(10, 3);
    Engine_ActorSetChildValue(11, 3);
    Engine_ActorSetChildValue(12, 3);
    actor = Engine_ActorGet(10);
    scale = 0x8000;
    update = (s32)OverlayObject_Add160ToFields18And1c;
    actor->scale_y = scale;
    actor->scale_x = scale;
    actor->update = (void (*)(union FieldObject *))update;
    actor = Engine_ActorGet(11);
    actor->scale_y = scale;
    actor->scale_x = scale;
    actor->update = (void (*)(union FieldObject *))update;
    actor = Engine_ActorGet(12);
    actor->scale_y = scale;
    actor->scale_x = scale;
    actor->update = (void (*)(union FieldObject *))update;
    Engine_TaskWait(1);
    Call3((void (*)())Engine_ActorSetSpeed, 10, 0x851e, 0x428f);
    Call3((void (*)())Engine_ActorSetSpeed, 11, 0x7333, 0x3999);
    Call3((void (*)())Engine_ActorSetSpeed, 12, 0x9999, 0x4ccc);
    Call3((void (*)())Engine_ActorSetDestination, 10, 128, 345);
    Call3((void (*)())Engine_ActorSetDestination, 11, 136, 330);
    Call3((void (*)())Engine_ActorSetDestination, 12, 156, 340);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(8, 2);
    Call3((void (*)())Engine_ActorWalkToAndWait, 8, 164, 344);
    Engine_ActorJump(8, 4, 10);
    Engine_ActorJump(8, 6, 40);
    Engine_ActorStartRepeatedMotion(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 20);
    (*event)->start_transition = 0x202;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(11);
    Engine_EventEnd();
}
