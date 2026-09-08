#define GetSceneViewActor Func_0200634e
#define ActorKindProbeValues Data_0200e1d0
#define ActorFootprintBounds Data_0200e1e8
#define ActorDirectionSteps Data_0200e190
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
