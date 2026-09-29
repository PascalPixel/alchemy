/* Actor 14's first talk: "Hey, it's that warrior guy!", with its opening
 * lines only until flag 0x300 is set. It sets flag 0x307. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgKuupuappuWarriorGuy[];

struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    u8 unk_08[92];
    u16 state_flags;
};

void SceneDialogue_RunActorFourteenFlagDialogue(void)
{
    struct SceneActor *actor = Actor_Get(14);
    u16 facing = actor->facing;
    s32 text;

    actor->state_flags |= 2;
    Event_Begin();
    text = (s32)MsgKuupuappuWarriorGuy;
    Event_SetMessage(text);
    Actor_SetAnimation(14, 0);
    Actor_FaceEachOther(14, ACTOR_PARTY_LEADER, 2);
    if (GameFlag_IsSet(0x300) == 0) {
        Actor_ShowEmote(14, 256, 60);
        Event_ShowMessageAndWait(14, 0, 10);
        Event_ShowMessageAndWait(14, 0, 10);
        GameFlag_Set(0x300);
    }
    Event_SetMessage(text + 2);
    Event_ShowMessageAndWait(14, 0, 10);
    actor->facing = facing;
    Task_Wait(1);
    Event_End();
    actor->state_flags = 1;
    GameFlag_Set(0x307);
}
