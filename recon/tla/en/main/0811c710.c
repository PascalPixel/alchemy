// Near miss, differ 4 of 276 (tla-en, 2026-10-30): one swap at entry. The
// native reloads the action from its stack slot before setting livingCount's
// zero; sched2 queues that reload two cycles behind the parameter's spill
// store, so at the next cycle only the zero is ready and goes first. Ready a
// cycle earlier, the reload's priority (40 to the zero's 35) would give the
// native order; no source order reaches it (scheduler cost model).
// The Broken Seal's twin is AutoBattle_RunPsynergy (battle/auto_battle.c),
// with this game's heap slots.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped, a register tie on each local after
// each statement, each local pinned to each register, each call argument
// copied into its argument register (pinned) at the call or up to three
// statements before it, five minutes of permute; none reaches 0. Also
// (30 October): livingCount set after actorId (22 differ) or before its loops
// (83), the else arm's sideStart before its call (23), the count temporary in
// both arms (78), four minutes of permute.
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_display.h"
#include "battle_side.h"
#include "motobj.h"
#include "io_reg.h"
#include "owner_state.h"
#include "ram_buffer.h"
#include "system.h"

void Actor_SetMode(struct MotionObject *object, s32 mode);

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
  wait(10);
  Random16();
  actorRecord = BattleObject_Get(actorId)->object;

  if (targetId >= 0 && targetId <= 7) {
    sideCount = BattleParty_ListLivingUnits(2, visibleUnits);
    sideStart = 0x80;
  } else {
    /* FAKEMATCH(temporary): the count held until sideStart is stored, so the
     * spill store comes before sideCount's copy; as sideCount =
     * BattleParty_ListLivingUnits(1, visibleUnits), 6 of 276 instructions
     * differ in tla-en. */
    u32 count = BattleParty_ListLivingUnits(1, visibleUnits);
    sideStart = 0;
    sideCount = count;
  }

  for (index = 0; index != sideCount; index++) {
    if (index + sideStart == actorId)
      Actor_SetMode(actorRecord, 3);
  }

  wait(30);
  REG_BLDCNT = 0x3f40;
  for (index = 0; index != sideCount; index++)
    BattleHighlight_SetSpriteMode(index + sideStart, 1);
  for (index = 0; index != 16; index++) {
    REG_BLDALPHA = (16 - index) | 0x1000;
    wait(1);
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

  wait(10);
  BattleObject_Put();
  REG_BLDCNT = 0x3f40;
  for (index = 0; index != sideCount; index++)
    BattleHighlight_SetSpriteMode(index + sideStart, 1);
  for (index = 0; index != 16; index++) {
    REG_BLDALPHA = index | 0x1000;
    wait(1);
  }
  for (index = 0; index != sideCount; index++)
    BattleHighlight_SetSpriteMode(index + sideStart, 0);
  BattleCamera_SetView(0, 0, 0, 0x64);
  wait(3);
  return 0;
}
