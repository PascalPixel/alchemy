/* Near miss, differ 4 of 95 (2026-10-15), ported from ☀️'s UpdateRisingParticleBurst
 * with ⚓️'s object 673 (☀️'s 285). Two loads come one slot later than the
 * listing's: the 0xfffff800 literal before the first Audio_PlayCue argument, and
 * the angle (ldrh r3, [r7, #6]) before the 0x2000 step is started. ☀️'s block-local
 * 1.0 is still needed. Single and paired barriers after each statement measured no
 * better. */
#include "types.h"
#include "system.h"
#include "objdisp.h"
#include "object_runtime.h"

extern const s32 Data_080f0e78[];

struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(struct ObjectRuntime *object, s32 magnitude, s32 angle);
void Object_Destroy(struct ObjectRuntime *object);

void UpdateRisingParticleBurst(void *object)
{
    struct ObjectRuntime *src = object;
    struct ObjectRuntime *child;
    s32 i;
    s32 dist;
    u32 spread;

    Audio_PlayCue(154);
    for (i = 30; i >= 0; i--) {
        src->y += 1 << 16;
        src->angle += 0x2000;
        src->scaleX += -0x800;
        src->scaleY += -0x800;
        WaitFrames(1);
    }
    for (i = 7; i >= 0; i--) {
        child = Object_Spawn(673, src->x, src->y, src->z);
        if (child != 0) {
            s32 speed;
            /* FAKEMATCH: a block-local 1.0, set before the block's calls, keeps 0x10000 in one register that loop.c hoists for both uses, as in ☀️'s UpdateRisingParticleBurst. */
            s32 unit = 0x10000;

            ObjectDispatch_InitializeFar((struct DispatchObject *)child, Data_080f0e78);
            speed = Random16();
            child->acceleration = unit;
            child->speedLimit = speed + unit;
            child->motionFlags = 2;
            child->gravity = 0xa3d;
            spread = Random16();
            child->velocityY = spread - Random16();
            dist = Random16() * 24;
            dist += 8 << 16;
            Motion_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
        }
    }
    Audio_PlayCue(131);
    Object_Destroy(src);
}
