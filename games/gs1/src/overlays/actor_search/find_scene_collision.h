#ifndef GUARD_OVERLAYS_ACTOR_SEARCH_FIND_SCENE_COLLISION_H
#define GUARD_OVERLAYS_ACTOR_SEARCH_FIND_SCENE_COLLISION_H

#include "types.h"
#include "staged_actor.h"
#include "staged_actor_probe_state.h"

/* The x/z bounds swept for one staged actor kind. Declared here rather than
 * taken from staged_actor_probe.h so that every overlay in this family sees
 * the same element type; some overlays already declare their own table under
 * a different width for other callers. */
struct SceneCollisionFootprint {
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;
};

#ifndef SceneCollisionWork
#define SceneCollisionWork Data_03001ebc
#endif

extern u8 *SceneCollisionWork;
extern struct StagedActor *GetSceneViewActor(s32 which);
extern s32 ActorKindProbeValues[];
extern struct SceneCollisionFootprint ActorFootprintBounds[];
extern s32 ActorDirectionSteps[];

#endif
