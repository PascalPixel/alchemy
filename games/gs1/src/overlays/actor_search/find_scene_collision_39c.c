#define GetSceneViewActor Func_02006122
#define ActorKindProbeValues Data_0200dd90
#define ActorFootprintBounds Data_0200dda8
#define ActorDirectionSteps Data_0200dd50
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
