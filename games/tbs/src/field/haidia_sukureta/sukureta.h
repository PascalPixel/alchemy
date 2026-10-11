#ifndef SUKURETA_H
#define SUKURETA_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "facing_object.h"

enum CottageActor {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED + 5,
    ACTOR_SATUROS,
    ACTOR_MENARDI
};

enum CottageFlag {
    FLAG_SANCTUM_VISIT_PLANNED = 0x801,
    FLAG_MET_SATUROS_AND_MENARDI = 0x808
};

#define NULL ((void *)0)

extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement Placement_Actors[];
extern struct SceneEvent Placement_Effects[];
extern struct SceneEvent Placement_Effects834[];
extern struct SceneEvent Placement_Effects87a[];
extern u16 Sukureta_GateCells[];

s32 ArcTan2(s32, s32);
void BattleFx_SetWeightedResult(s32, s32);

#endif
