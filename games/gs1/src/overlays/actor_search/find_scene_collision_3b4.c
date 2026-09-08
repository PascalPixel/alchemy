#define GetSceneViewActor Func_020028aa
#define ActorKindProbeValues Data_0200a684
#define ActorFootprintBounds Data_0200a69c
#define ActorDirectionSteps Data_0200a644
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
