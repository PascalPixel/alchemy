// Vale, its home, storm and Babi scenes, Sol Sanctum's eruption and Venus
// Lighthouse's aerie: Psynergy shown in a cutscene, such as the villager's
// "Behold, the power of Psynergy!" in Vale.
#include "types.h"
#include "field_event.h"
#include "ability_ids.h"

// Prepare the effect work and spark tiles of Move (ability 140) for the
// cutscene's Psynergy.
void SceneState_SetValue140Mode0(void) {
  Engine_PsynergyBegin(ABILITY_MOVE, 0);
}

// End the cutscene's Psynergy and release its effect work.
void SceneEffect_CleanupSceneObjects(void) {
  BattleEffect_CleanupSceneObjects();
}
