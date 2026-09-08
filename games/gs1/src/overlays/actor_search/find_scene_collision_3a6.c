#define GetSceneViewActor Func_0200230a
#define ActorKindProbeValues Data_02009e88
#define ActorFootprintBounds Data_02009ea0
#define ActorDirectionSteps Data_02009e48
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
