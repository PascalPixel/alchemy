/* The villager who cannot find the man meant to be fixing the roof. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgKuupuappuFixingRoofCant[];

void SceneActor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay);

void Villager_LookForRoofer(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgKuupuappuFixingRoofCant);
    SceneActor_ApplyActorCueThenWait(21, 0, 2);
    Actor_ShowEmote(21, 0x103, 0);
    Event_Wait(30);
    Event_OpenMessage(21, 0);
    Event_End();
}
