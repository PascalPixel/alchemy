// Func_0811edb0: The Broken Seal's BattleStaging_RunItem
// (games/tbs/src/battle/battle/staging.c), ported: plays an item's use, the
// camera turned to the user's side, the user raising the item and, for an item
// with an effect, the screen dimming while the effect plays with the events;
// then spends the item as its use type says. This game sets the user's mode
// from the battle work's byte 0x869. The dimming loop reads the battle work
// through a heap pointer, which keeps 0x03000000 in r11 as the native. Message
// 3176 is MsgItemBroke here, a name not yet in the catalogs.
// Func_0811ddd8 is void (r0 is set before r1 for the call, as the
// native).
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_work.h"
#include "battle_display.h"
#include "battle_event.h"
#include "battle_system.h"
#include "task.h"
#include "item.h"
#include "motobj.h"
#include "ram_buffer.h"

struct ItemAnimationEntry {
  u8 unk00[5];
  u8 param;
};

struct ItemAnimation {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct ItemAnimationEntry *entries[1];
};

extern char MsgItemBroke;

struct BattleUnit *Unit_Get(s32 unitId);
void Actor_SetMode(struct MotionObject *object, s32 mode);
void ColorBuffer_Scale(u16 *source, u32 destination, s32 value, s32 size);
void Func_0811ddd8(struct BattlePlan *plan,
                   struct BattlePresentationWork *work);
void Func_08138008(struct BattlePresentationWork *work);
void Func_08138018(struct BattlePresentationWork *work);
void BattlePlayback_WaitForReady(void);
void BattleEvent_Playback(void);
void ObjMotRst(s32 slot);
void WaitFrames(s32 frames);
s32 Inventory_BreakFar(s32 owner, s32 slot);

s32 Func_0811edb0(struct BattlePlan *plan, s32 unused,
                  struct BattleActionRecord *selection) {
  struct BattlePresentationWork work;
  struct MotionObject *object;
  s32 targetOrFadeIndex;
  s32 inventoryEntry;
  struct BattleUnit *unit;
  struct ItemDefinition *itemDefinition;
  struct BattlePresentationTransition *transition =
      (struct BattlePresentationTransition *)Ram_HeapSlots->transitionWork;
  s32 targetYaw;

  if (plan->actorId > 4)
    targetYaw = -0x2000;
  else
    targetYaw = 0x2000;
  if (transition->targetYaw != targetYaw)
    transition->targetYaw = targetYaw;
  Func_0811ddd8(plan, &work);
  BattleHighlight_SetModes(0, 0);
  {
    struct BattleObj *slot = BattleObject_Get(work.actor);
    struct BattleWork *battle;
    u8 *mode;

    /* FAKEMATCH(one-pass): the battle work is loaded before the mode's
     * offset is built; as Actor_SetMode(object, battle->byte 0x869) after the
     * object, 4 of 289 instructions differ in tla-en. */
    do {
      battle = Ram_HeapSlots->battleWork;
    } while (0);
    mode = (u8 *)battle + 0x869;
    object = slot->object;
    Actor_SetMode(object, *mode);
  }
  Actor_SetAnimationSpeedFar((struct ActorView *)object, 16);
  if (plan->targetIds[0] <= 7)
    work.side = 1;
  else
    work.side = 0;

  for (targetOrFadeIndex = 0; targetOrFadeIndex != work.count;
       targetOrFadeIndex++) {
    struct ItemAnimation *animation = GetMotionRecord(
        BattleObject_Get(work.actors[targetOrFadeIndex])->object, 0);
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      work.params[targetOrFadeIndex][entryIndex] =
          animation->entries[entryIndex]->param;
  }

  Task_Add(BattleEvent_Playback, TASK_PASS_VBLANK);
  if (work.kind != 0) {
    s32 fade;
    struct HeapSlots *heap;

    targetOrFadeIndex = 0;
    heap = Ram_HeapSlots;
    fade = 0;
    do {
      struct BattleWork *battle = heap->battleWork;

      if (targetOrFadeIndex < 20) {
        u16 *palette = battle->palette;
        s32 value = 0x10000 - fade;

        battle->brightness = value;
        ColorBuffer_Scale(palette, 0x050000c0, value, 128);
      }
      WaitFrames(1);
      targetOrFadeIndex++;
      fade += 0x444;
    } while (targetOrFadeIndex < 20);
    if (plan->presentationFlags & 0x4000)
      Func_08138008(&work);
    else
      Func_08138018(&work);
  } else {
    WaitFrames(60);
  }
  BattlePlayback_WaitForReady();
  Actor_SetMode(object, 1);
  for (targetOrFadeIndex = 0; targetOrFadeIndex != work.count;
       targetOrFadeIndex++)
    ObjMotRst(work.actors[targetOrFadeIndex]);

  unit = Unit_Get(selection->unitId);
  inventoryEntry = unit->inventory[selection->parameter];
  itemDefinition = Item_Get(inventoryEntry);
  if (itemDefinition->useType == 1) {
    s32 result = Inventory_RemoveFar(selection->unitId, selection->parameter);
    s32 index = selection->parameter;
    if (result == 2) {
      struct BattleWork *battle = Ram_HeapSlots->battleWork;
      u32 row;

      for (row = 0; row < 20; row++) {
        if (battle->actions[row].command == 2 &&
            battle->actions[row].unitId == selection->unitId) {
          if (battle->actions[row].parameter == index)
            battle->actions[row].parameter = -1;
          else if (battle->actions[row].parameter > index)
            battle->actions[row].parameter--;
        }
      }
    }
  } else if (itemDefinition->useType == 2) {
    if ((Battle_RandomFar() & 7) == 0) {
      BattleEvent_Push(2, unit->inventory[selection->parameter]);
      BattleEvent_Push(4, (u32)&MsgItemBroke);
      Inventory_BreakFar(selection->unitId, selection->parameter);
      BattleEvent_DispatchQueued();
    }
  } else if (itemDefinition->useType == 4) {
    if ((inventoryEntry & 0x1ff) == 184)
      inventoryEntry = 185;
    unit->inventory[selection->parameter] = inventoryEntry;
  }
  return 0;
}
