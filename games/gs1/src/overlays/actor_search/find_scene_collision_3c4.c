#define GetSceneViewActor Func_02003476
#define ActorKindProbeValues Data_0200b230
#define ActorFootprintBounds Data_0200b248
#define ActorDirectionSteps Data_0200b1f0
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
