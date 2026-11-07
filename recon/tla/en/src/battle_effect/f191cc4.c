#include "types.h"
#include "ram_buffer.h"
#include "motobj.h"
#include "battle_display.h"
#include "battle_work.h"
#include "battle_effect_work.h"

extern s32 gCameraSceneParameters[];

struct BattleObj *GetBattleObjectSlotFar(s32 objectId);
void Func_08118088(s32 actor, s32 mode);
void Palette_LoadBasicObjColors(void);
void BattleBackground_LoadFar(s32 mode, s32 background, s32 level);

// Summon 25's teardown: sets each target back on the ground at the battle
// origin with the usual bounce (The Broken Seal's 0xab85 motion strength)
// and restarts its idle motion, ends the presentation transition, centres the
// camera at (120, 120), reloads the battle palette and redraws the battle's
// own backdrop over the summon's.
void Func_08191cc4(void) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattlePresentationTransition *transition =
      Ram_HeapSlots->transitionWork;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 i;

  for (i = 0; i != work->effect->count; i++) {
    struct MotionObject *object =
        GetBattleObjectSlotFar(work->effect->actors[i])->object;

    object->x = 0;
    object->y = 120 << 16;
    object->z = 0;
    object->verticalMotionStrength = 0xab85;
    Func_08118088(work->effect->actors[i], 1);
  }
  transition->active = 0;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
  Palette_LoadBasicObjColors();
  // The battle's background (The Broken Seal's battle->background).
  BattleBackground_LoadFar(1, *(u16 *)battle->unk670, 24);
}
