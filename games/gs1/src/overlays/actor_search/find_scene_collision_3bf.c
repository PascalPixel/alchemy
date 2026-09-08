#define GetSceneViewActor Func_020059ba
#define ActorKindProbeValues Data_0200df18
#define ActorFootprintBounds Data_0200df30
#define ActorDirectionSteps Data_0200ded8
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
