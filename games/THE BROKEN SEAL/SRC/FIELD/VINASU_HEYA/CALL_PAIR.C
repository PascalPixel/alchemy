#include "ENTRY_SETUP.H"

/*
 * Two rubble-fall task callbacks: each spawns the random particles over its
 * own band.  The 16-byte owners load no literal, so they carry no pool word
 * and no alignment halfword.  432 is built from a shifted immediate and
 * passed straight to the callee as a value, not used as a displacement.
 */
void SceneState_CallWith432And32(void)
{
    VinasuHeya_SpawnRandomParticles(432, 32);
}

void FieldScene_CallWith560And44(void)
{
    VinasuHeya_SpawnRandomParticles(0x230, 44);
}
