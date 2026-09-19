#ifndef TLA_FIELD_SCENE_H
#define TLA_FIELD_SCENE_H

#include "TYPES.H"

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

/* The part of the saved game that records where the party is. */
struct GameState {
    u8 unknown_000[0x1e0];
    s16 scene;
    s16 entrance;
    u8 unknown_1e4[0x67];
    u8 battle_rule_24b;
};

extern struct GameState gGameState;

#endif
