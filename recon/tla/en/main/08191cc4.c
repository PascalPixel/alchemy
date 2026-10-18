// Draft, differ 2 of 72 (no device): summon 25's closing step, which puts
// the targets back and restores the battle's camera, palette and backdrop.
// Left: the native loads the battle work (ldr r3, [r3, #36]) between the two
// halves of the effect field's offset 0x78f0 (movs r2, #240 / lsls); agscc
// finishes the constant first (one reordered row).
// Tried: the three heap reads in all six declaration orders (work first:
// differ 2; transition first: 20; battle first: 29-41), assigned in
// statements in other orders than declared (2 or 20), the work read in a
// one-pass block (20), all three through one HeapSlots pointer (2 or 20), a
// pointer to the effect field (20); alchemy permute 4 minutes, nothing below
// 2.
// 2026-10-18: each callee declared s32 in place of void, one at a time (2 or
// 4).
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
