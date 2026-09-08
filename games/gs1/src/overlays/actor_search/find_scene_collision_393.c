#define GetSceneViewActor Func_020011ee
#define ActorKindProbeValues Data_02008f50
#define ActorFootprintBounds Data_02008f68
#define ActorDirectionSteps Data_02008f10
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
