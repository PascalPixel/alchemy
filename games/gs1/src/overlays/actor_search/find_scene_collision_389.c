#define GetSceneViewActor Func_020017ba
#define ActorKindProbeValues Data_02009594
#define ActorFootprintBounds Data_020095ac
#define ActorDirectionSteps Data_02009554
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
