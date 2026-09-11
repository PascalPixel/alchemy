#include "scene.h"
/* Begin a scene, attempt the forward transition, and fall back to pushing the
 * obstructing actor when the transition cannot run.  Complete 28-byte owner
 * from the prologue at 0x02001528 through return/alignment at 0x02001543. */

void Scene_RunTransitionOrFallback(void)
{
    Actor_Run();
    if (Actor_unk2_4() == 0)
        Actor_unk3_4();
    Actor_unk4_4();
}
