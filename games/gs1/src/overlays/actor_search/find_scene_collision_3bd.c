#define GetSceneViewActor Func_02004032
#define ActorKindProbeValues Data_0200bef4
#define ActorFootprintBounds Data_0200bf0c
#define ActorDirectionSteps Data_0200beb4
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
