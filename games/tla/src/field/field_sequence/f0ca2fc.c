#include "types.h"
#include "main_routine_work.h"

extern const s16 gParty_PairResolveRules[];

/// Sets the scene the party returns to after a defeat
/// (gMainRoutineWork.savedScene and savedEntrance), from the first rule of
/// gParty_PairResolveRules that matches the current scene and entrance: rows
/// of eight halfwords that open {scene, entrance (-1 for any), return scene,
/// return entrance}, ended by scene -1. Without a match the restart point stays
/// as it was.
void Scene_SetRestartPoint(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  s16 first;
  s16 second;
  const s16 *entry;

  first = party->scene;
  second = party->entrance;
  entry = gParty_PairResolveRules;

  while (entry[0] != -1) {
    if (entry[0] == first && (entry[1] == -1 || entry[1] == second)) {
      party->savedScene = entry[2];
      party->savedEntrance = entry[3];
      return;
    }
    entry += 8;
  }
}
