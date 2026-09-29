/* NONMATCHING: resource_398 at 0x0200846c (72 bytes with its pool), between
 * FIELD/BIRIBINO_DOU/REGION.C and SETUP.C, stays listing. It is the
 * overlay's exported entry and returns 0.
 *
 * Remaining difference: the reference loads the scene numbers 0x31, 0x30
 * and 0x2f from its literal pool and compares registers, as link-time scene
 * symbols do; plain constants compile to cmp with an immediate.
 */

#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/REGION.H"

s32 FieldScene_DispatchByScenarioId(void)
{
    s16 variant = gGameState.scene;

    if (variant == 0x31) {
        RunGuardedSceneSetup();
    } else if (variant == 0x30) {
        SceneState_SetRuntimeWord448To516();
    } else if (variant == 0x2f) {
        FieldScene_RunScene398SequenceC();
    }
    return 0;
}
