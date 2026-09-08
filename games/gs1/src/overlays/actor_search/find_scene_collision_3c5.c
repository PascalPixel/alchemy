#define GetSceneViewActor Func_020031ba
#define ActorKindProbeValues Data_0200b014
#define ActorFootprintBounds Data_0200b02c
#define ActorDirectionSteps Data_0200afd4
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
