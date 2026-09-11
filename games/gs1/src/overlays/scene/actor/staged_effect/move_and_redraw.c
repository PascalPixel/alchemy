#include "scene.h"
#include "abi/overlays/scene/actor/staged_effect/move_and_redraw.h"
#define Value Value2

#include "staged_actor_movement.h"

void Actor_Run(
    StagedActorMovementRequest request)
{
#include "run_staged_actor_movement_and_redraw_body.inc"
}
