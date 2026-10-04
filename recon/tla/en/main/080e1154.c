/* Ported from ☀️ games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/EFFECT42.C. */
#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "FIXED_MATH.H"
#include "SCENE_IDS.H"
#include "SCENE.H"

/* Spawns the descending arc particles every seventh frame in the Clear
   scene and every fifth frame elsewhere; while the halfword at 0x64 is 1,
   also adds 0xc00 to the one at 6. */
void BattleFx_UpdatePairedArcSpawner(void *object)
{
    s16 field64;
    s16 counter;

    field64 = *(s16 *)((u8 *)object + 0x64);
    counter = (*(u16 *)((u8 *)object + 0x66))++;

    if (gGameState.scene == (s32)&SceneId_Clear) {
        if (__modsi3(counter, 7) == 0)
            BattleFx_SpawnDescendingArcParticles(object);
    } else if (__modsi3(counter, 5) == 0) {
        BattleFx_SpawnDescendingArcParticles(object);
    }

    if (field64 == 1)
        *(u16 *)((u8 *)object + 6) += 0xC00;
}
