/* NONMATCHING: Japanese live-coordinate probe, 2026-10-01.
 * The approved TBS flags match the full 60-byte function except one pool
 * byte: this attempt probes 24 pixels behind the actor, while the Japanese
 * game probes 32 pixels behind it. Production preserves the corrected C.
 */
#include "../../../../../../games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/MORI.H"
#include "FIELD_EVENT.H"
#include "STAGED_ACTOR.H"

void SceneActor_PassOffsetPointOfActorZero(void)
{
    s32 v[3];
    #if defined(TBS_EDITION_JA)
    v[0] = ((struct FieldActor *)Actor_Get(ACTOR_PARTY_LEADER))->x.fixed;
    v[1] = ((struct FieldActor *)Actor_Get(ACTOR_PARTY_LEADER))->y.fixed;
    v[2] = ((struct FieldActor *)Actor_Get(ACTOR_PARTY_LEADER))->z.fixed - 0x180000;
#else
    s32 *p = Actor_Get(ACTOR_PARTY_LEADER);

    v[0] = (p[2] & 0xfff00000) + 0x80000;
    v[1] = p[3];
    v[2] = (p[4] & 0xfff00000) + 0xffe80000;
#endif
    SceneActor_TryRunSlotZeroMoveStep(v);
}
