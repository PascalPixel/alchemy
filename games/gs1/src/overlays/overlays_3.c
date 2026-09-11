#include "scene.h"
#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"
#include "types.h"

/* overlays/scene/actor/map_init/move_and_redraw.c */
void Actor_Run(
    StagedActorMovementRequest request)
{
}

/* overlays/shared/copy_mode_to_owner.c */
/* Copy the low two mode bits into the object's owner record. */

struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

    owner->mode = mode;
}
