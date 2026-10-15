#include "types.h"
#include "field_event.h"

void SceneState_SetValue140Mode0(void) { Engine_PsynergyBegin(140, 0); }

void SceneEffect_CleanupSceneObjects(void) {
  BattleEffect_CleanupSceneObjects();
}
