#define GetSceneViewActor Func_020033f2
#define ActorKindProbeValues Data_0200b19c
#define ActorFootprintBounds Data_0200b1b4
#define ActorDirectionSteps Data_0200b15c
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
