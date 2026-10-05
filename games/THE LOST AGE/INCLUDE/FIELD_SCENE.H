#ifndef TLA_FIELD_SCENE_H
#define TLA_FIELD_SCENE_H

#include "TYPES.H"
#include "PARTY_STATE.H"

struct SceneRecord {
    s16 resource_id;
    s8 group;
    s8 variant;
    u16 map_index;
    s16 attributes;
};

extern const struct SceneRecord Field_SceneTable[];

struct SceneEntrance;
struct SceneRegion;
struct ScenePlacement;
struct SceneEvent;

#endif
