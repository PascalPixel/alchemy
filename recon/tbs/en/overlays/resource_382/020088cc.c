/*
 * Draft: overlay 382 (KUUPUAPPU_MURA) at 0x020088cc, between
 * ACTOR_PRESENTATION.C and SCENE_STEP.C; its rows stay in the listing.
 *
 * Remaining difference: the game loads message 0x12c0 from the literal pool,
 * but a constant argument compiles to mov #150 / lsl #5, which is four bytes
 * shorter. A plain constant, a static const and a local all fold to the
 * shifted form; only a relocated symbol would load it, and message numbers
 * are not symbols.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneActor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay);

void FieldScene_RunActor21Sequence(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(0x12c0);
    SceneActor_ApplyActorCueThenWait(21, 0, 2);
    Actor_ShowEmote(21, 0x103, 0);
    Event_Wait(30);
    Event_OpenMessage(21, 0);
    Event_End();
}
