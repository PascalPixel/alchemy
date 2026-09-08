#define GetSceneViewActor Func_02002f8e
#define ActorKindProbeValues Data_0200ada8
#define ActorFootprintBounds Data_0200adc0
#define ActorDirectionSteps Data_0200ad68
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
