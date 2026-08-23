#include "staged_actor_pair_scene.h"

/*
 * resource_3be owner at 0x020011d8, 32 bytes.
 *
 * Runs a step at most 40 times, stopping early once the caller's +12 field has
 * come down to the limit. Both the counter and the field test guard the loop.
 */

struct HeightTrackedObject {
    u8 pad00[12];
    s32 height;                 /* +12 */
};

void Func_02002674();

void StagedActorPairScene_WaitForHeight(struct HeightTrackedObject *object,
                                       s32 limit)
{
    s32 frames = 40;

    while (frames != 0) {
        Func_02002674(1);
        frames--;
        if (object->height <= limit) {
            break;
        }
    }
}
