#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "objdisp.h"
#include "battle_system.h"
#include "battle_session.h"
#include "owner_state.h"

struct BattleObj *BattleObjGet(s32 arg0);
s32 ActivateBattleObjectSlot(s32 arg0);

s32 BattleActor_DestroyTemporaryObject(s32 arg0) {
  /* FAKEMATCH: the shared discard-only result type preserves the epilogue
   * register choice. */
  struct BattleUnit *creature;
  struct BattleObj *runtime;

  creature = UnitGet(arg0);
  if (creature->side == 1) {
    Owner_UpdateRatioPairFar(creature, 0);
    BattleActor_RemoveFromLists(arg0);
    ActivateBattleObjectSlot(arg0);
    runtime = BattleObjGet(arg0);
    Object_Destroy((struct DispatchObject *)runtime->object);
    runtime->object = 0;
    runtime->active = 0;
  }
}

struct BattleEventFlagWork {
  u8 unk00[0x16c];
  u32 flags;
};

void Battle_SetRuntimeFlagBit0(struct BattleEventFlagWork *runtime,
                               u32 operand) {
  runtime->flags |= 1;
}
