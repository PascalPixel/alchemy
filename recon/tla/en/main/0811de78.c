// Near miss: The Broken Seal source without its last wait, the actor read
// before the mode call. Left: the transition work and the result swap r8 and
// r10 once the result is the twin union (as a plain word they are right, but
// then partyMode takes an immediate zero where the native copies r10), and the
// actor byte is loaded two rows late. Message 3228 (no escape, English
// numbering) has no name in the catalogs yet: MsgNoEscape here is undefined.
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
void WaitFrames(s32 frames);
u32 Random16(void);
void BattleActor_RemoveFromLists(s32 unitId);
void BattleObject_MarkForRelease(s32 objectId);
struct BattleUnit *Unit_Get(s32 id);

s32 Func_0811de78(struct BattlePlan *plan) {
  /* FAKEMATCH: as in The Broken Seal, the halfword initialization preserves the
   * word copy into partyMode instead of folding it into an immediate zero. */
  union {
    s32 value;
    u16 halves[2];
  } completed;
  struct EscapeTransition *presentation =
      (struct EscapeTransition *)Ram_HeapSlots->transitionWork;
  u32 aid = plan->actorId;
  struct BattleWork *scene = Ram_HeapSlots->battleWork;
  s32 partyMode;

  completed.halves[0] = 0;
  completed.halves[1] = 0;
  presentation->targetYaw = 0x2000;
  presentation->active = 1;
  BattlePres_SetActorModes(0, 0);

  if (aid <= 7) {
    partyMode = completed.value;
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
          WaitFrames(8);
        }
        index--;
      }
      completed.value = 1;
    }
  } else if (((Random16() * 10) >> 16) <= 6) {
    s16 event[2];

    event[0] = plan->actorId;
    event[1] = 0xff;
    BattleMotion_SetupEscapeObject(event[0]);
    WaitFrames(8);
    BattleActor_RemoveFromLists(plan->actorId);
    BattleObject_MarkForRelease(plan->actorId);
  } else {
    UiText_ShowMessageAndWaitCoreFar((s32)MsgNoEscape);
    BattlePresentation_WaitForAdvance();
  }

  presentation->active = 0;
  return completed.value;
}
