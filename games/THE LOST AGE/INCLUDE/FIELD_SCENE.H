#ifndef TLA_FIELD_SCENE_H
#define TLA_FIELD_SCENE_H

#include "TYPES.H"
#include "PARTY_STATE.H"

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
