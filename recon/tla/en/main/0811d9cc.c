// Under -mthumb-slow-loads (26 October 2026): differ 12 of 421 in tla-en.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped, each local pinned to each register,
// each call argument copied into its argument register (pinned) at the call
// or up to three statements before it, five minutes of permute; none reaches
// 0. Barriers after lines 102 and 113 of the draft as it stood lower it to 8,
// and no third single barrier lowers it further.
// Draft of Func_0811d9cc: ☀️'s BattleStaging_DispatchAction
// (games/tbs/src/battle/battle/staging.c), ported: plays one action of the
// round. An enemy picks its ability now (Func_08122514 with retry 1), the
// camera resets, command 10 builds its plan with Func_0811d7e8 and any other
// with Func_0812381c (☀️'s BattleActionRun_Build), and the plan's staging kind
// chooses the runner. ⚓️ adds: the battle work's actor mode byte (0x869) set
// to 3, a confused unit's attack on an ally (kind 10: the actor and its plan's
// second unit trade places for a cast and an attack, "attacks an ally!" or
// "stop hitting yourself!"), kind 11 (a cast that speeds up the Djinn's
// recovery, "Djinn are recovering more quickly!", or "But no Djinn are in
// Recovery!" when Func_08124cc4 finds none), kinds 12 and 13 (Func_0811d89c,
// Func_0811d914), and the turn's end through Func_0812561c and Func_0811d9a4.
// The message names are ours, from their text (3213, 3214, 3297; en.po has no
// names for them yet). Near miss, differ 20 of 421 (2026-10-24), instruction
// order only: nine loads the native issues one or two slots earlier and one
// argument copy. Forms measured: case 0 written (else the table starts at 1),
// the projection's 0x1fe0000 rebuilt for each argument as camera.c writes it
// (else the constant is kept across the call), the message symbols (pool
// loads), and one u8 for the saved target and both swaps (210 with a u8 for
// each: those take r10 and leave the battle work r9).
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_type.h"
#include "battle_work.h"
#include "battle_display.h"
#include "battle_session.h"
#include "battle_party.h"
#include "iwram_call.h"
#include "ram_buffer.h"

struct DispatchBattleWork {
  u8 unk000[0x66c];
  s32 brightness;
  u16 background;
  u8 unk672[0x67c - 0x672];
  struct BattlePlan plan;
  s32 skipPlayback;
  u8 unk6e4[0x84e - 0x6e4];
  u16 unk84e;
  u8 unk850[0x869 - 0x850];
  u8 actorMode;
};

struct BattleUnit *Unit_Get(s32 unitId);
s32 Func_08122514(struct BattleActionRecord *request, s32 retry);
void Projection_Set(u32 value0, u32 value1, u32 value2);
void Func_0811d7e8(struct BattleActionRecord *request, struct BattlePlan *plan);
s32 Func_0812381c(struct BattleActionRecord *request, struct BattlePlan *plan);
s32 Func_0811ea0c(struct BattlePlan *plan, s32 mode);
s32 Func_0811df70(struct BattlePlan *plan, s32 mode);
s32 Func_0811f088(struct BattlePlan *plan, s32 mode);
s32 Func_0811edb0(struct BattlePlan *plan, s32 unused,
                  struct BattleActionRecord *selection);
s32 Func_0811de78(struct BattlePlan *plan);
s32 Func_0811d79c(s32 classId);
s32 Func_08124cc4(s32 party);
void Func_0811d89c(struct BattlePlan *plan);
void Func_0811d914(struct BattlePlan *plan);
void Func_0812561c(struct BattlePlan *plan);
void Func_0811d9a4(void);
void Func_0812628c(s32 mode, s32 background, s32 value);
void Sound_PlayFar(s32 cue);

extern char MsgAttacksAlly;
extern char MsgStopHittingYourself;
extern char MsgDjinnRecoverFaster;

s32 Func_0811d9cc(struct BattleActionRecord *request, s32 delay) {
  struct BattlePresentationTransition *view;
  struct DispatchBattleWork *battle;
  struct BattleCamera *camera;
  struct BattleUnit *unit;
  u16 actingUnit[2];
  s32 fled;
  s32 result;

  battle = (struct DispatchBattleWork *)Ram_HeapSlots->battleWork;
  fled = 0;
  if (request->unitId == 255)
    return 0;
  unit = Unit_Get(request->unitId);
  if (unit->hp == 0)
    return -1;
  battle->actorMode = 3;
  if (unit->classIndex == 0)
    Func_08122514(request, 1);

  view = Ram_HeapSlots->transitionWork;
  view->frames = 60;
  view->flag = 0;
  camera = Ram_HeapSlots->cameraWork;
  battle->brightness = 0x10000;
  Render_ResetTransformState();
  Transform_SetLookAt(camera->eye, camera->pos);
  {
    s32 param;
    s32 ratio;

    param = 255;
    param <<= 17;
    ratio = Iwram_RatioMulQ14(param, 192 << 8);
    param = 255;
    param <<= 17;
    Projection_Set(param, ratio, 0x7fff0000);
  }
  if (delay != 0) {
    view->targetYaw = 0x2000;
    WaitFrames(delay);
  }
  if (request->command == 10) {
    Func_0811d7e8(request, &battle->plan);
    result = 0;
  } else {
    actingUnit[0] = request->unitId;
    actingUnit[1] = 255;
    BattlePres_SetActorModes(actingUnit, 1);
    result = Func_0812381c(request, &battle->plan);
  }
  if (result == 0) {
    switch (battle->plan.outcome) {
    case 0:
      break;
    case 1:
      BattlePres_RunActorEntries(&battle->plan, 0);
      break;
    case 2:
      Func_0811ea0c(&battle->plan, 0);
      break;
    case 15:
      Func_0811ea0c(&battle->plan, 2);
      break;
    case 5:
      Func_0811df70(&battle->plan, 1);
      break;
    case 9:
      Func_0811df70(&battle->plan, 0);
      break;
    case 14:
      Func_0811df70(&battle->plan, 3);
      break;
    case 3:
    case 17:
      Func_0811f088(&battle->plan, 0);
      break;
    case 6:
      Func_0811f088(&battle->plan, 1);
      break;
    case 8:
    case 16:
      Func_0811f088(&battle->plan, 2);
      break;
    case 4:
      Func_0811edb0(&battle->plan, 0, request);
      break;
    case 10: {
      u8 swap = battle->plan.targetIds[0];

      battle->unk84e = 1;
      battle->plan.targetIds[0] = battle->plan.actorId2;
      battle->skipPlayback = 1;
      Func_0811f088(&battle->plan, 1);
      battle->skipPlayback = 0;
      battle->plan.targetIds[0] = swap;
      swap = battle->plan.actorId;
      battle->plan.actorId = battle->plan.actorId2;
      battle->plan.actorId2 = swap;
      battle->plan.presentationFlags =
          Func_0811d79c(Unit_Get(battle->plan.actorId)->enemyId);
      if (((battle->plan.actorId ^ battle->plan.targetIds[0]) >> 7) == 0) {
        UiText_DrawQuantity(battle->plan.actorId, 1);
        if (battle->plan.actorId == battle->plan.targetIds[0])
          UiText_ShowMessageAndWaitCoreFar((s32)&MsgStopHittingYourself);
        else
          UiText_ShowMessageAndWaitCoreFar((s32)&MsgAttacksAlly);
      }
      Func_0811ea0c(&battle->plan, battle->plan.abilityId == 312 ? 32 : 0);
      swap = battle->plan.actorId;
      battle->plan.actorId = battle->plan.actorId2;
      battle->plan.actorId2 = swap;
      break;
    }
    case 11: {
      s32 kept;

      battle->skipPlayback = 1;
      Func_0811f088(&battle->plan, 1);
      battle->skipPlayback = 0;
      UiText_ShowMessageAndWaitCoreFar((s32)&MsgDjinnRecoverFaster);
      kept = Func_08124cc4(battle->plan.targetIds[0] > 7);
      if (kept == 0)
        UiText_ShowMessageAndWaitCoreFar((s32)(&MsgDjinnRecoverFaster + 1));
      if (kept <= 1)
        BattlePresentation_WaitForAdvance();
      break;
    }
    case 12:
      Func_0811df70(&battle->plan, 1);
      if (BattleParty_ListLivingUnits(1, 0) == 0)
        break;
      if (BattleParty_ListLivingUnits(2, 0) == 0)
        break;
      Func_0811d89c(&battle->plan);
      Func_0811f088(&battle->plan, 1);
      break;
    case 13:
      Func_0811df70(&battle->plan, 1);
      Func_0811d914(&battle->plan);
      Func_0811f088(&battle->plan, 1);
      break;
    case 7:
      if (Func_0811de78(&battle->plan) != 0)
        fled = 1;
      if (fled != 0)
        goto finish;
      break;
    }
  } else {
    if (result == -1) {
      BattlePresentation_WaitForAdvance();
      WaitFrames(3);
    }
    BattlePres_SetActorModes(0, 0);
  }

  BattleMotion_DestroyAllSlotObjects();
  Func_0812561c(&battle->plan);
  BattleObject_Put();
  Enemy_Update();
  Func_0811d9a4();
  request->unitId = 255;
finish:
  Func_0812628c(
      2, ((struct DispatchBattleWork *)Ram_HeapSlots->battleWork)->background,
      0);
  Sound_PlayFar(390);
  return fled;
}
