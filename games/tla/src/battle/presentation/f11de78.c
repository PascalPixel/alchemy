#include "types.h"
#include "ram_buffer.h"
#include "battle_work.h"
#include "battle_type.h"
#include "battle_unit.h"

struct EscapeTransition {
  s32 targetYaw;
  s32 unk04[3];
  s32 active;
};

extern u8 MsgNoEscape[];

void BattlePres_SetActorModes(u16 *actors, s32 mode);
void BattlePresentation_WaitForAdvance(void);
void UiText_ShowMessageAndWaitCoreFar(s32 messageId);
s32 BattleParty_ListLivingUnits(s32 side_mask, u16 *unit_ids);
void BattleMotion_SetupEscapeObject(s32 objectId);
void WaitTicks(s32 ticks);
u32 Random16(void);
void BattleActor_RemoveFromLists(s32 unitId);
void BattleObject_MarkForRelease(s32 objectId);
struct BattleUnit *Unit_Get(s32 id);

/// Plays a flee action, The Broken Seal's BattleStaging_RunFlee without its
/// last wait: the party runs unless encounterMode is 2, and an enemy gets away
/// 7 times in 10. Returns 1 when the party got away.
s32 Func_0811de78(struct BattlePlan *plan) {
  s32 completed;
  struct EscapeTransition *presentation =
      (struct EscapeTransition *)Ram_HeapSlots->transitionWork;
  u32 aid = plan->actorId;
  struct BattleWork *scene = Ram_HeapSlots->battleWork;
  s32 partyMode;

  completed = 0;
  presentation->targetYaw = 0x2000;
  presentation->active = 1;
  BattlePres_SetActorModes(0, 0);

  if (aid <= 7) {
    partyMode = completed;
    /* FAKEMATCH(barrier): an empty asm on partyMode keeps it a copy of
     * completed (mov r2, r10); as plain C it folds to movs r2, #0, and 2 of
     * 108 instructions differ in tla-en. */
    __asm__("" : "=r"(partyMode) : "0"(partyMode));
    if (scene->encounterMode != 2)
      partyMode = 1;

    if (!partyMode) {
      UiText_ShowMessageAndWaitCoreFar((s32)MsgNoEscape);
      BattlePresentation_WaitForAdvance();
    } else {
      s16 unitIds[14];
      s32 index = BattleParty_ListLivingUnits(1, (u16 *)unitIds) - 1;

      while (index != -1) {
        struct BattleUnit *unit = Unit_Get(unitIds[index]);

        if (unit->stun == 0 && unit->sleep == 0) {
          BattleMotion_SetupEscapeObject(unitIds[index]);
          WaitTicks(8);
        }
        index--;
      }
      completed = 1;
    }
  } else if (((Random16() * 10) >> 16) <= 6) {
    s16 event[2];

    event[0] = plan->actorId;
    event[1] = 0xff;
    BattleMotion_SetupEscapeObject(event[0]);
    WaitTicks(8);
    BattleActor_RemoveFromLists(plan->actorId);
    BattleObject_MarkForRelease(plan->actorId);
  } else {
    UiText_ShowMessageAndWaitCoreFar((s32)MsgNoEscape);
    BattlePresentation_WaitForAdvance();
  }

  presentation->active = 0;
  return completed;
}
