/// @file
/// Flickering an object and the arc sprites it sheds.
// Part of radial.c.

#include "../radial_internal.h"
#include "module/field/object.h"
#include "module/field/radial.h"

void BattleEffect_WaitForArcGroups(void) {
  struct ArcEffectWork *work = Ram_HeapSlots->block62;
  s32 i = 0;

  while (work->age[0] <= 31 || work->age[1] <= 31) {
    wait(1);
    i++;
    if (i > 89)
      return;
  }
}

void BattleEffect_FlickerObjectAndSpawnArcs(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Animation_ApplyChildValuesFar(object, 7);
  } else {
    Animation_ApplyChildValuesFar(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    BattleEffect_SpawnScaledArcObjects(object);
  }
}

void BattleEffect_CycleObjectAndSpawnArcs(struct Actor *object) {
  if (gFrameCount & 1)
    Animation_ApplyChildValuesFar(object, (gFrameCount >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    BattleEffect_SpawnScaledArcObjects(object);
}

void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode) {
  if (mode == 0) {
    object->update = 0;
    Animation_ApplyChildValuesFar(object, 0);
  } else if (mode == 1) {
    object->update = (void (*)(void))BattleEffect_FlickerObjectAndSpawnArcs;
  } else {
    object->update = (void (*)(void))BattleEffect_CycleObjectAndSpawnArcs;
  }
}

// The arc effect's next pair of scaled sprites.

/// Sets up the current group's two arc sprites from the effect's tiles, at the
/// target's priority, then restarts that group's age, records the target and
/// switches to the other group.
void BattleEffect_SpawnScaledArcObjects(struct Actor *target) {
  struct ArcWork *work = Ram_HeapSlots->block62;
  s32 tiles = gResourceTableEntries[work->resource].offset >> 5;
  s32 group = work->group;
  struct EffectSprite *sprite;
  s32 i;

  for (i = 0; i < 2; i++) {
    sprite = &work->sprites[group][i];
    Func_080eaf98(sprite, 16, 16, 0x80004000, tiles);
    sprite->oam.palette256 = 0;
    sprite->oam.palette = 0;
    sprite->oam.priority = BattleEffect_GetObjectPriority(target);
  }
  work->age[group] = 0;
  work->target = target;
  work->group ^= 1;
}
