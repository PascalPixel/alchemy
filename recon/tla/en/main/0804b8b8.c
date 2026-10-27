// Near miss, differ 29 of 231 (31 before; 2026-10-26, result cleared after the
// battle work is read): lets the player pick which
// reserve member takes party slot `slot` in battle, previewing each choice
// in place (left and right cycle, A confirms through Func_08118140 and
// swaps the two in gPartyState.activeOwners, B restores the slot). The
// loop enters at its test, as the native does (a while loop's test gets
// copied ahead of the body: differ 84 more); the keys are read through a
// volatile view of gInput, as the native reloads them (plain: differ 82).
// Remaining: the spill slots of the owners address and reserve swap
// (native: owners [sp, #24], reserve [sp, #8]), selected's address is
// built in two steps (movs, add sp) against one, and a few reordered pairs
// in the confirm and redraw paths. A pointer to owners declared first puts
// the owners slot right but re-reads the new id with ldrsh (differ 79);
// reserve declared after result moves the other slots (differ 59).
// Devices: a once-only block after the first Audio_PlayCue and an empty asm
// after the target object's read reach 14 (instruction order in the redraw).
#include "types.h"
#include "battle_work.h"
#include "party_state.h"
#include "input.h"
#include "ram_buffer.h"
#include "object_runtime.h"

s32 BattleParty_PrepareReserveOwnersFar(u16 *owners);
s32 Func_08118140(s32, s32);
struct BattleObj *GetBattleObjectSlotFar(s32 objectId);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x,
                                         s32 y, s32 z);
void PartyStatus_Draw(s32 flags);
void BattleActor_CommitPlacementFar(void);
void BattlePres_SetActorModesFar(u16 *, s32);
s32 BattleLayout_HighlightPartyPanels(u16 *ids);
void WaitFrames(s32);

#define INPUT ((volatile struct InputState *)&gInput)

s32 Func_0804b8b8(s32 slot) {
  u16 owners[6];
  u16 selected[2];
  struct BattleWork *battle;
  s32 *reserve;
  s32 count;
  s32 sel;
  s32 redraw;
  s32 result;
  s32 current;
  s32 newId;
  s32 i;

  count = BattleParty_PrepareReserveOwnersFar(owners);
  sel = 0;
  redraw = 1;
  battle = Ram_HeapSlots->battleWork;
  result = 0;
  reserve = Ram_HeapSlots->unke4[0];
  if (count <= 0)
    return -1;
  slot &= 0xff;
  current = battle->partyUnits[slot];
  Audio_PlayCue(112);
  goto test;
  for (;;) {
    if ((INPUT->pressed & 1) && !redraw) {
      if (Func_08118140(1, 0) == 0) {
        Audio_PlayCue(114);
        goto test;
      }
      Audio_PlayCue(110);
      reserve[65] = current;
      reserve[66] = slot;
      for (i = 0; i < 8; i++) {
        if (gPartyState.activeOwners[i] == current)
          gPartyState.activeOwners[i] = newId;
        else if (gPartyState.activeOwners[i] == newId)
          gPartyState.activeOwners[i] = current;
      }
      goto done;
    }
    if (INPUT->repeat & 0x20) {
      sel = (sel + count - 1) % count;
      Audio_PlayCue(112);
      redraw = 1;
    } else if (INPUT->repeat & 0x10) {
      sel = (sel + 1) % count;
      Audio_PlayCue(112);
      redraw = 1;
    }
    if (redraw) {
      struct BattleObj *from;
      struct BattleObj *to;
      struct ObjectRuntime *object;
      struct ObjectRuntime *place;

      redraw = 0;
      newId = battle->partyUnits[slot] = owners[sel];
      from = GetBattleObjectSlotFar(current);
      to = GetBattleObjectSlotFar(newId);
      to->anchorX = from->anchorX;
      place = (struct ObjectRuntime *)from->object;
      object = (struct ObjectRuntime *)to->object;
      to->anchorZ = from->anchorZ;
      Object_SetPositionAndResetMotionFar(object, place->x, place->y, place->z);
      PartyStatus_Draw(Ram_HeapSlots->battleWork->partyStatus);
      BattleActor_CommitPlacementFar();
      selected[0] = newId;
      selected[1] = 0xff;
      BattlePres_SetActorModesFar(selected, 2);
      BattleLayout_HighlightPartyPanels(selected);
    }
    WaitFrames(1);
  test:
    newId = battle->partyUnits[slot] = owners[sel];
    if (INPUT->pressed & 2)
      break;
  }
  battle->partyUnits[slot] = current;
  result = -1;
  Audio_PlayCue(113);
done:
  selected[0] = 0xff;
  BattlePres_SetActorModesFar(selected, 0);
  BattleLayout_HighlightPartyPanels(selected);
  PartyStatus_Draw(Ram_HeapSlots->battleWork->partyStatus);
  BattleActor_CommitPlacementFar();
  WaitFrames(1);
  return result;
}
