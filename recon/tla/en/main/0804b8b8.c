// Matches (tla-en, 2026-11-08, -mtune=gba).
// Lets the player pick which reserve member takes party slot `slot` in battle,
// previewing each choice in place (left and right cycle, A confirms through
// Func_08118140 and swaps the two in gMainRoutineWork.activeOwners, B restores
// the slot). The loop enters at its test, as the native does (a while loop's
// test gets copied ahead of the body); the keys are read through a volatile
// view of gInput, as the native reloads them. The locals after the reserve
// call are declared in a block that opens after it, so the owners address
// takes its spill slot before sel and reserve after result (from 19: 9). A
// once-only block (plain C under S2) after the first Sound_PlayFar, in place
// of the earlier one around it, takes the rest (9 to 0).
#include "types.h"
#include "battle_work.h"
#include "main_routine_work.h"
#include "input.h"
#include "ram_buffer.h"
#include "actor.h"

s32 BattleParty_PrepareReserveOwnersFar(u16 *owners);
s32 Func_08118140(s32, s32);
struct BattleObj *GetBattleObjectSlotFar(s32 objectId);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x, s32 y,
                                        s32 z);
void PartyStatus_Draw(s32 flags);
void BattleActor_CommitPlacementFar(void);
void BattleHighlight_SetModesFar(u16 *, s32);
s32 BattleLayout_HighlightPartyPanels(u16 *ids);
void wait(s32);

#define INPUT ((volatile struct InputState *)&gInput)

s32 Func_0804b8b8(s32 slot) {
  u16 owners[6];
  u16 selected[2];
  struct BattleWork *battle;
  s32 count;

  count = BattleParty_PrepareReserveOwnersFar(owners);
  {
    s32 sel;
    s32 redraw;
    s32 result;
    s32 *reserve;
    s32 current;
    s32 newId;
    s32 i;

    sel = 0;
    redraw = 1;
    battle = Ram_HeapSlots->battleWork;
    result = 0;
    reserve = Ram_HeapSlots->unke4[0];
    if (count <= 0)
      return -1;
    slot &= 0xff;
    current = battle->partyUnits[slot];
    Sound_PlayFar(112);
    // Once-only block: the native's code order after the first sound.
    do {
      goto test;
      for (;;) {
        if ((INPUT->pressed & 1) && !redraw) {
          if (Func_08118140(1, 0) == 0) {
            Sound_PlayFar(114);
            goto test;
          }
          Sound_PlayFar(110);
          reserve[65] = current;
          reserve[66] = slot;
          for (i = 0; i < 8; i++) {
            if (gMainRoutineWork.activeOwners[i] == current)
              gMainRoutineWork.activeOwners[i] = newId;
            else if (gMainRoutineWork.activeOwners[i] == newId)
              gMainRoutineWork.activeOwners[i] = current;
          }
          goto done;
        }
        if (INPUT->repeat & 0x20) {
          sel = (sel + count - 1) % count;
          Sound_PlayFar(112);
          redraw = 1;
        } else if (INPUT->repeat & 0x10) {
          sel = (sel + 1) % count;
          Sound_PlayFar(112);
          redraw = 1;
        }
        if (redraw) {
          struct BattleObj *from;
          struct BattleObj *to;
          struct Actor *object;
          struct Actor *place;

          redraw = 0;
          newId = battle->partyUnits[slot] = owners[sel];
          from = GetBattleObjectSlotFar(current);
          to = GetBattleObjectSlotFar(newId);
          to->anchorX = from->anchorX;
          place = (struct Actor *)from->object;
          object = (struct Actor *)to->object;
          to->anchorZ = from->anchorZ;
          Actor_SetPositionAndResetMotionFar(object, place->x, place->y,
                                             place->z);
          PartyStatus_Draw(Ram_HeapSlots->battleWork->partyStatus);
          BattleActor_CommitPlacementFar();
          selected[0] = newId;
          selected[1] = 0xff;
          BattleHighlight_SetModesFar(selected, 2);
          BattleLayout_HighlightPartyPanels(selected);
        }
        wait(1);
      test:
        newId = battle->partyUnits[slot] = owners[sel];
        if (INPUT->pressed & 2)
          break;
      }
      battle->partyUnits[slot] = current;
      result = -1;
      Sound_PlayFar(113);
    done:
      selected[0] = 0xff;
      BattleHighlight_SetModesFar(selected, 0);
      BattleLayout_HighlightPartyPanels(selected);
      PartyStatus_Draw(Ram_HeapSlots->battleWork->partyStatus);
      BattleActor_CommitPlacementFar();
      wait(1);
    } while (0);
    return result;
  }
}
