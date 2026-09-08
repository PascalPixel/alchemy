#define GetSceneViewActor Func_02001b82
#define ActorKindProbeValues Data_02009700
#define ActorFootprintBounds Data_02009718
#define ActorDirectionSteps Data_020096c0
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
