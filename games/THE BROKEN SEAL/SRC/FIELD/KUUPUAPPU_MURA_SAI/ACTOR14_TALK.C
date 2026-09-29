/* Talking to actor 14: the first talk until flag 0x307 is set, then the line
 * about feeling brave. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgKuupuappuWatchingGuysMakes[];

struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    u8 unk_08[92];
    u16 state_flags;
};

void ActorPresentation_RunActorModeOneThenZero(s32 actor);
void SceneDialogue_RunActorFourteenFlagDialogue(void);

void KuupuappuMuraSai_RunActor14Talk(void)
{
    {
        struct SceneActor *actor = (struct SceneActor *)Actor_Get(14);

        actor->state_flags |= 2;
    }
    Event_Begin();
    if (GameFlag_IsSet(0x307) != 0) {
        Event_SetMessage((s32)MsgKuupuappuWatchingGuysMakes);
        ActorPresentation_RunActorModeOneThenZero(14);
    } else {
        SceneDialogue_RunActorFourteenFlagDialogue();
        GameFlag_Set(0x307);
    }
    Event_End();
    {
        u8 *record = (u8 *)Actor_Get(14);
        s32 shown = 1;

        *(volatile u16 *)((s32)record + 100) = shown;
    }
}
