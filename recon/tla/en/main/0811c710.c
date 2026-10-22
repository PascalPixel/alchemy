// Near miss, differ 14 of 276 (seven reordered rows, 28 before): The Broken
// Seal's BattlePresentation_RunPairedUnitTransition with this game's heap
// slots. Left are load-before-constant swaps of the ARM9 schedule: the
// transition work's heap base after the stacked parameter, fp after the zero
// store, and three BLDCNT/BLDALPHA blocks where the native loads the pool value
// before or between the register address rows. Not yet tried: f18284c.c's
// pinned value and one-pass blocks on those stores.
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_display.h"
#include "battle_party.h"
#include "motobj.h"
#include "io_reg.h"
#include "owner_state.h"
#include "ram_buffer.h"
#include "system.h"

void Object_SetMode(struct MotionObject *object, s32 mode);

s32 Func_0811c710(struct BattleActionRecord *action) {
  u16 visibleUnits[14];
  struct BattlePresentationWork context;
  s32 actorId;
  s32 targetId;
  u32 sideStart;
  u32 sideCount;
  u32 livingCount;
  struct MotionObject *actorRecord;
  u32 index;

  livingCount = 0;
  actorId = action->unitId;
  if (BattleObject_IsValidId(actorId) < 0)
    return -1;
  targetId = action->target;
  if (BattleObject_IsValidId(targetId) < 0)
    return -1;

  {
    struct BattlePresentationTransition *transition =
        Ram_HeapSlots->transitionWork;
    transition->targetYaw = action->unitId > 4 ? 0x5000 : 0x2000;
    transition->frames = 60;
  }
  WaitFrames(10);
  Random16();
  actorRecord = BattleObject_Get(actorId)->object;

  if (targetId >= 0 && targetId <= 7) {
    sideCount = BattleParty_ListLivingUnits(2, visibleUnits);
    sideStart = 0x80;
  } else {
    sideCount = BattleParty_ListLivingUnits(1, visibleUnits);
    sideStart = 0;
  }

  for (index = 0; index != sideCount; index++) {
    if (index + sideStart == actorId)
      Object_SetMode(actorRecord, 3);
  }

  WaitFrames(30);
  REG_BLDCNT = 0x3f40;
  for (index = 0; index != sideCount; index++)
    BattlePres_SetActorRecordMode(index + sideStart, 1);
  for (index = 0; index != 16; index++) {
    REG_BLDALPHA = (16 - index) | 0x1000;
    WaitFrames(1);
  }
  PartyStatus_DrawFar(9);

  if (targetId > 0x7f) {
    u32 count = BattleParty_ListLivingUnits(2, visibleUnits);
    for (index = 0; livingCount != count; index++) {
      u32 unit = index + 0x80;
      if (((struct BattleUnit *)Unit_Get(unit))->hp > 0)
        visibleUnits[livingCount++] = unit;
    }
  } else {
    u32 count = BattleParty_ListLivingUnits(1, visibleUnits);
    for (index = 0; livingCount != count; index++) {
      if (((struct BattleUnit *)Unit_Get(index))->hp > 0)
        visibleUnits[livingCount++] = index;
    }
  }
  visibleUnits[livingCount] = 0xff;
  BattleActor_SpawnObjectsForList((s16 *)visibleUnits, 0);

  context.kind = action->parameter;
  context.actor = actorId;
  for (index = 0; index != livingCount; index++)
    context.actors[index] = visibleUnits[index];
  context.count = livingCount;
  if (targetId >= 0 && targetId <= 7)
    context.side = 1;
  else
    context.side = 0;
  BattleEffect_RunSceneEffectFar((s32 *)&context);

  WaitFrames(10);
  BattleObject_Put();
  REG_BLDCNT = 0x3f40;
  for (index = 0; index != sideCount; index++)
    BattlePres_SetActorRecordMode(index + sideStart, 1);
  for (index = 0; index != 16; index++) {
    REG_BLDALPHA = index | 0x1000;
    WaitFrames(1);
  }
  for (index = 0; index != sideCount; index++)
    BattlePres_SetActorRecordMode(index + sideStart, 0);
  BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
  WaitFrames(3);
  return 0;
}
