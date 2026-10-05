#ifndef TLA_FIELD_SCENE_H
#define TLA_FIELD_SCENE_H

#include "TYPES.H"
#include "PARTY_STATE.H"

/* A scene's row: its overlay's resource id, group, variant and map, then
   packed attributes the field reads four bits at a time. */
struct SceneRecord {
    s16 resource_id;
    s8 group;
    s8 variant;
    u16 map_index;
    s16 attributes;
};

extern const struct SceneRecord Field_SceneTable[];

/*
 * The tables a field scene hands to the map engine when it loads. The
 * scene's entry veneers give the engine, in order, the scene setup, the
 * entrances, the exits, the placed actors, the events, the regions and a
 * last hook that returns 0.
 */
struct SceneEntrance;
struct SceneRegion;
struct ScenePlacement;
struct SceneEvent;

#endif
