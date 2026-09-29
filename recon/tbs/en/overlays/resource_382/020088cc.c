/* Draft of resource_382 0x020088cc (FieldScene_RunActor21Sequence): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgKuupuappuFixingRoofCant). The listing
 * keeps these rows until the draft is adopted. */

#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgKuupuappuFixingRoofCant[];

void SceneActor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay);

void FieldScene_RunActor21Sequence(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)MsgKuupuappuFixingRoofCant);
    SceneActor_ApplyActorCueThenWait(21, 0, 2);
    Actor_ShowEmote(21, 0x103, 0);
    Event_Wait(30);
    Event_OpenMessage(21, 0);
    Event_End();
}
