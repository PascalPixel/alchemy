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

static __inline__ s32 Scene_QueryFlag(s32 (*func)(s32), s32 flag)
{
    return func(flag);
}

static __inline__ void Scene_SetFlag(void (*func)(s32), s32 flag)
{
    func(flag);
}

static __inline__ void Scene_Call3(void (*func)(s32, s32, s32), s32 a, s32 b, s32 c)
{
    func(a, b, c);
}

void SceneDialogue_RunActorFourteenFlagDialogue(void)
{
    struct SceneActor *actor = Actor_Get(14);
    s16 facing = (s16)actor->facing;
    s32 text;

    actor->state_flags |= 2;
    Event_Begin();
    text = (s32)MsgKuupuappuWarriorGuy;
    Event_SetMessage(text);
    Actor_SetAnimation(14, 0);
    Actor_FaceEachOther(14, ACTOR_PARTY_LEADER, 2);
    if (Scene_QueryFlag(Engine_GameFlagIsSet, 0x300) == 0) {
        Scene_Call3(Engine_ActorShowEmote, 14, 256, 60);
        Event_ShowMessageAndWait(14, 0, 10);
        Event_ShowMessageAndWait(14, 0, 10);
        Scene_SetFlag(Engine_GameFlagSet, 0x300);
    }
    Event_SetMessage(text + 2);
    Event_ShowMessageAndWait(14, 0, 10);
    actor->facing = facing;
    Task_Wait(1);
    Event_End();
    {
        s32 shown = 1;
        actor->state_flags = shown;
    }
    GameFlag_Set(0x307);
}
