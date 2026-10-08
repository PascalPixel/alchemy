#include "object_lookup.h"
#include "types.h"
#include "fixed_math.h"
#include "scene_ids.h"
#include "scene.h"

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
