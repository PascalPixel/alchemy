#define GetSceneViewActor Func_02002b3a
#define ActorKindProbeValues Data_0200a740
#define ActorFootprintBounds Data_0200a758
#define ActorDirectionSteps Data_0200a700
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
