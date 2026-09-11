#include "scene.h"
#include "abi/map/locations/heidia/village/scene/move_redraw.h"

#include "staged_actor_movement.h"

void Map_Run(
    StagedActorMovementRequest request)
{
#include "run_staged_actor_movement_and_redraw_body.inc"
}
