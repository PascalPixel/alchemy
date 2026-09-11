#include "scene.h"
#include "abi/overlays/scene/story/selector_effect/move_and_redraw.h"
#define Value Value2

#include "staged_actor_movement.h"

void Story_Run(StagedActorMovementRequest request)
{
#include "run_staged_actor_movement_and_redraw_body.inc"
}
