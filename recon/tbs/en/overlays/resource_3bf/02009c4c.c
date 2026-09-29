/* Draft of resource_3bf 0x02009c4c (RunActorScriptedSequenceB): it matches
 * the ROM byte for byte now that the message it loads from the literal pool
 * has a catalogue name (MsgRunpaIntruder). The listing keeps these rows until
 * the draft is adopted. */
#include "FORTRESS.H"
extern u8 MsgRunpaIntruder[];

/*
 * The resource run is taken as the address of 0x241e rather than as
 * an integer constant, which preserves its pointer identity and materialises
 * it after the first call.
 */
void RunActorScriptedSequenceB(s32 handle)
{
    u8 *id;

    Actor_RunRepeatedMotion(handle, 1);
    id = (s32)MsgRunpaIntruder;
    Event_SetMessage((s32)id);
    Event_ShowMessage(handle, 0);
    Actor_ShowEmoteAt(handle);
    Event_SetMessage((s32)(id + 1));
    Event_ShowMessage(handle, 0);
    id += 2;
    Actor_SetAnimationAndWait(handle, 4);
    Event_SetMessage((s32)id);
    Event_ShowMessage(handle, 0);
}
