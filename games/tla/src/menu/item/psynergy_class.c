#include "types.h"
#include "ram_buffer.h"
#include "edition.h"
#include "static_assert.h"

struct BattleAct {
  u8 targetMode;
  u8 unk01[5];
  u8 type;
  u8 unk07;
  u8 range;
  u8 ppCost;
};

/// The Psynergy menu's work (the menu heap slot) as these functions read it.
struct PsynergyMenuState {
  u8 unk000[0x28];
  void *statusWindow;
  u8 unk02c[4];
  void *infoWindow;
  u8 unk034[0x134];
  u16 paneRow[2];    ///< each pane's Psynergy row chosen
  u16 paneAction[2]; ///< each pane's Psynergy chosen, as a member action entry
  u8 unk170[0xa4];
  u8 psynergyCount; ///< the chosen member's Psynergy listed
  u8 memberCount;
  /// The user and the target: [1] is MENU_TARGET_WHOLE_PARTY for Psynergy on
  /// the whole party.
  u8 memberIds[2];
  u8 unk218[4];
  /// bits: 0 set after a use or a cancelled target
  u16 flags;
  u16 skipSlide; ///< 1 skips the cursor's next slide once
  u8 unk220[0x3e];
  s16 useResult; ///< the use's result message, after MsgItemUseResult
  u8 unk260[0xc];
  u8 mode; ///< 0 to use Psynergy; 1 or 2 to set the L or R shortcut
};

STATIC_ASSERT(offsetof(struct PsynergyMenuState, paneAction) == 0x16c);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, memberIds) == 0x216);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, useResult) == 0x25e);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, mode) == 0x26c);

struct BattleAct *BattleAction_Get(s32 action);
s32 BattleEffect_HasMatchingEvent5Far(s32 type);

#define MENU_TARGET_WHOLE_PARTY 9 ///< memberIds[1] for the whole party
#define PSYNERGY_ABILITY_MASK 0x3fff
#define FLAG_MENU_START_PRESSED 0x150

/// The steps of the Psynergy menu (PsynergyMenu_RunSteps).
enum PsynergyMenuStep {
  PSYNERGY_STEP_MEMBER,
  PSYNERGY_STEP_LIST,
  /// how the Psynergy is used (Func_080fcf14)
  PSYNERGY_STEP_CLASSIFY,
  PSYNERGY_STEP_TARGET,
  PSYNERGY_STEP_APPLY,
};

/// The Psynergy shortcut a choice sets (Party_SetPackedEvent).
enum PsynergyShortcut {
  PSYNERGY_SHORTCUT_L,
  PSYNERGY_SHORTCUT_R,
};

/// What the Psynergy menu is open for (PsynergyMenuState's mode).
enum PsynergyMenuMode {
  PSYNERGY_MODE_USE,
  PSYNERGY_MODE_SET_L,
  PSYNERGY_MODE_SET_R,
};

extern u8 MsgPsynergyChooseOwner[];
extern u8 MsgPsynergyChooseAbility[];
extern u8 MsgPsynergyChooseForL[];
extern u8 MsgPsynergyChooseForR[];
extern u8 MsgShortcutSetL[];
extern u8 MsgShortcutSetR[];
extern u8 MsgPsynergyChooseTarget[];
extern u8 MsgItemUseResult[];

s32 GameFlag_Test(s32 flag);
void WaitFrames(s32 frames);
void Sound_PlayFar(s32 cue);
void *Unit_Get(s32 member);
void Unit_CalculateFar(s32 member);
s16 Unit_AddPpFar(s32 unit, s32 amount);
void ItemMenu_DrawMessage(s32 window, s32 message);
void ItemMenu_PositionCategory(void);
void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y);
void RenderOutput_ClearListFar(void *window);
void RenderOutput_RedrawSavedRectFar(void *window);
void Party_SetPackedEvent(s32 member, s32 action, s32 shortcut);
s32 BattleEffect_ApplyToTargets(s32 entry, s32 member, s32 target, s32 mode);
void Func_080f8170(void *window, s32 member, s32 mode, s32 kind);
s32 Func_080fcf14(void);
s32 Func_080fcf5c(s32 paneIndex);
s32 Func_080fd2e8(s32 paneIndex);
s32 Func_080fdad4(s32 paneIndex);
void Func_08100d58(s32 action);

/// Runs the Psynergy menu, The Broken Seal's PsynergyMenu_RunSteps: choose a
/// party member and one of their Psynergy, then use it on a member (or the
/// whole party), or, in the shortcut modes, set it as the L or R shortcut.
/// Returns 1 with the member and Psynergy in the out parameters when the
/// chosen Psynergy works on the field, to be used once the menu closes; -1
/// when the player backs out or presses Start; otherwise the result of the
/// last use.
s32 PsynergyMenu_RunSteps(s32 *outMember, s32 *unused, s32 *outAction) {
  struct PsynergyMenuState *menu;
  s32 useResult;
  s32 step;
  s32 done;
  s32 chosenAction;
  s32 targetMode;
  s32 actionEntry;
  s32 chosenTarget;
  struct BattleAct *action;

  step = PSYNERGY_STEP_MEMBER;
  done = 0;
  /* FAKEMATCH(barrier): the menu's address is built after the parameters and
   * done are stored; without it, 4 of 344 instructions differ in tla-en. */
  asm volatile("");
  menu = Ram_HeapSlots->menuRuntime;
  useResult = 0;

  while (done == 0 && GameFlag_Test(FLAG_MENU_START_PRESSED) == 0) {
    switch (step) {
    // Choose whose Psynergy to look at.
    case PSYNERGY_STEP_MEMBER: {
      s32 cancelled;
      s32 chosen;

      menu->paneRow[0] = 0;
      ItemMenu_DrawMessage(0, (s32)MsgPsynergyChooseOwner);
      chosen = Func_080fcf5c(0);
      cancelled = -1;
      /* FAKEMATCH(dead): a redundant copy of -1 and an XOR test give -1 its
       * own register, as The Broken Seal's twin; as if (chosen == -1) setting
       * done and useResult, 12 of 344 instructions differ in tla-en. */
      if (chosen == -1)
        cancelled = chosen;
      if (((u32)chosen ^ (u32)cancelled) == 0) {
        done = 1;
        useResult = cancelled;
      }
      RenderOutput_RedrawSavedRectFar(menu->infoWindow);
      step = PSYNERGY_STEP_LIST;
      break;
    }

    // Choose one of their Psynergy; in the shortcut modes, set it as the L or
    // R shortcut and start again.
    case PSYNERGY_STEP_LIST:
      WaitFrames(1);
      Unit_Get(menu->memberIds[0]);
      step = PSYNERGY_STEP_MEMBER;
      if (menu->psynergyCount != 0) {
        {
          struct PsynergyMenuState *savedWork = menu;

          /* FAKEMATCH(dead): menu cleared and restored under a false test, so
           * the mode's offset is built from its own constant, as The Broken
           * Seal's twin; without the two tests, 45 of 344 instructions differ
           * in tla-en. */
          if (done)
            menu = NULL;
          if (done)
            menu = savedWork;
        }
        switch (menu->mode) {
        case PSYNERGY_MODE_USE:
          ItemMenu_DrawMessage(0, (s32)MsgPsynergyChooseAbility);
          break;
        case PSYNERGY_MODE_SET_L:
          ItemMenu_DrawMessage(0, (s32)MsgPsynergyChooseForL);
          break;
        case PSYNERGY_MODE_SET_R:
          ItemMenu_DrawMessage(0, (s32)MsgPsynergyChooseForR);
          break;
        }
        ItemMenu_PositionCategory();
        Func_080f8170(menu->statusWindow, menu->memberIds[0], 0, 0);
        chosenAction = Func_080fdad4(0);
        step = PSYNERGY_STEP_MEMBER;
        if (chosenAction != -1) {
          step = PSYNERGY_STEP_CLASSIFY;
          if (menu->mode != PSYNERGY_MODE_USE) {
            if (menu->mode == PSYNERGY_MODE_SET_L) {
              Party_SetPackedEvent(menu->memberIds[0], chosenAction,
                                   PSYNERGY_SHORTCUT_L);
              RenderOutput_ClearListFar(menu->infoWindow);
              InventoryMenu_ShowModalMessage((s32)MsgShortcutSetL, -1, -1);
              RenderOutput_RedrawSavedRectFar(menu->infoWindow);
            } else {
              Party_SetPackedEvent(menu->memberIds[0], chosenAction,
                                   PSYNERGY_SHORTCUT_R);
              RenderOutput_ClearListFar(menu->infoWindow);
              InventoryMenu_ShowModalMessage((s32)MsgShortcutSetR, -1, -1);
              RenderOutput_RedrawSavedRectFar(menu->infoWindow);
            }
            step = PSYNERGY_STEP_MEMBER;
          }
        }
      }
      break;

    // Choose the member to use it on.
    case PSYNERGY_STEP_TARGET:
      ItemMenu_DrawMessage(0, (s32)MsgPsynergyChooseTarget);
      chosenTarget = Func_080fd2e8(0);
      step = PSYNERGY_STEP_APPLY;
      if (chosenTarget == -1) {
        menu->flags |= 1;
        step = PSYNERGY_STEP_LIST;
      }
      break;

    // How is it used? On one member, on the whole party, or on the field
    // once the menu closes.
    case PSYNERGY_STEP_CLASSIFY:
      targetMode = Func_080fcf14();
      if (targetMode == 1) {
        step = PSYNERGY_STEP_TARGET;
        break;
      }
      if (targetMode == 2) {
        menu->memberIds[1] = MENU_TARGET_WHOLE_PARTY;
        step = PSYNERGY_STEP_APPLY;
        break;
      }
      done = 1;
      useResult = 1;
      *outMember = menu->memberIds[0];
      *outAction = menu->paneAction[0] & PSYNERGY_ABILITY_MASK;
      break;

    // Use it: pay its PP, show the result and go back to the list, or to the
    // target when it failed on one member.
    case PSYNERGY_STEP_APPLY:
      chosenTarget = 0;
      actionEntry = menu->paneAction[0];
      useResult = BattleEffect_ApplyToTargets(actionEntry, menu->memberIds[0],
                                              menu->memberIds[1], 0);
      if (menu->memberIds[1] == MENU_TARGET_WHOLE_PARTY) {
        menu->memberIds[1] = menu->memberIds[0];
        chosenTarget = MENU_TARGET_WHOLE_PARTY;
      }
      if (useResult != -1) {
        u32 entry = menu->paneAction[0];
        u32 mask;

        /* FAKEMATCH(barrier): the entry's load before the mask is built;
         * without it, 2 of 344 instructions differ in tla-en. */
        asm volatile("");
        mask = PSYNERGY_ABILITY_MASK;
        mask &= entry;
        action = BattleAction_Get(mask);
        Unit_AddPpFar(menu->memberIds[0], -action->ppCost);
      }
      Unit_CalculateFar(menu->memberIds[0]);
      if (useResult != -1) {
        void *window;
        s32 target = menu->memberIds[1];

        /* FAKEMATCH(temporary): the target read before the window, as the
         * native orders the call's arguments; as Func_080f8170(menu->
         * statusWindow, menu->memberIds[1], 0, 0), 2 of 344 instructions
         * differ in tla-en. */
        window = menu->statusWindow;
        Func_080f8170(window, target, 0, 0);
        {
          u32 entry = menu->paneAction[0];
          u32 mask;

          /* FAKEMATCH(barrier): the entry's load before the mask is built;
           * without it, 2 of 344 instructions differ in tla-en. */
          asm volatile("");
          mask = PSYNERGY_ABILITY_MASK;
          mask &= entry;
          Func_08100d58(mask);
        }
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage(menu->useResult + (s32)MsgItemUseResult,
                                       0, -1);
      } else {
        Sound_PlayFar(114);
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage(menu->useResult + (s32)MsgItemUseResult,
                                       useResult, useResult);
      }
      if (useResult == -1) {
        menu->skipSlide = 1;
        if (chosenTarget == MENU_TARGET_WHOLE_PARTY) {
          menu->flags |= 1;
          step = PSYNERGY_STEP_LIST;
        } else {
          step = PSYNERGY_STEP_TARGET;
        }
      } else {
        useResult = 1;
        menu->flags |= 1;
        step = PSYNERGY_STEP_LIST;
      }
      break;

    default:
      done = 1;
      break;
    }
  }

  if (GameFlag_Test(FLAG_MENU_START_PRESSED) != 0)
    useResult = -1;
  return useResult;
}

/// How the selected psynergy is used: 0 acts at once, 1 picks a target,
/// 2 cannot be used here.
// The Broken Seal's PsynergyMenu_ClassifySelectedPsynergy.
s32 Func_080fcf14(void) {
  struct PsynergyMenuState *menu;
  struct BattleAct *psynergy;
  s32 diff;
  s32 ret;
  /* FAKEMATCH: the barrier keeps the action's load ahead of the 0x3fff mask,
   * and the pin keeps it in r3; as plain C, 2 instructions differ in
   * tla-en. */
  register u32 action asm("r3");

  menu = Ram_HeapSlots->menuRuntime;
  action = menu->paneAction[0];
  asm volatile("");
  psynergy = BattleAction_Get(0x3fff & action);
  if (BattleEffect_HasMatchingEvent5Far(psynergy->type) != 0)
    return 0;
  ret = 2;
  if (psynergy->range != 0xff) {
    u8 kind = psynergy->targetMode;

    diff = kind ^ 2;
    ret = (0 - diff) | diff;
    ret = (u32)ret >> 31;
    ret = 1 - ret;
  }
  return ret;
}
