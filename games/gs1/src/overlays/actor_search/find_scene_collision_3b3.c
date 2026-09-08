#define GetSceneViewActor Func_0200316a
#define ActorKindProbeValues Data_0200ace0
#define ActorFootprintBounds Data_0200acf8
#define ActorDirectionSteps Data_0200aca0
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
