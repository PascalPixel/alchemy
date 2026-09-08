#define GetSceneViewActor Func_02003126
#define ActorKindProbeValues Data_0200af38
#define ActorFootprintBounds Data_0200af50
#define ActorDirectionSteps Data_0200aef8
#include "find_scene_collision.h"

struct StagedActor *SceneActor_FindCollision(
    s32 *direction_out, s32 *slot_out, s32 *footprint_out)
{
#include "find_scene_collision_body.inc"
}
