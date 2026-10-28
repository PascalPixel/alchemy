#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "adventure.h"
#include "battle_unit.h"
#include "render_input.h"

extern const s16 gData_080aa170[4][9];

void Func_0804121c(void);
void Func_08041254(void);
struct RenderInput *Func_0804128c(void);
void Func_08108048(void *cursor, s32 x, s32 y);
void Func_08108040(void *cursor, s32 x, s32 y, s32 mode);
void WaitFrames(s32 frames);
void Sound_PlayFar(s32 cue);
void UiWork_Finalize(struct RenderInput *window, s32 release);
s32 Party_FindOwnerWithActionFar(s32 action);
void *Unit_Get(s32 owner);

/// The pressed keys, read again at each test.
#define KEYS_PRESSED (((volatile struct InputState *)&gInput)->pressed)

/// The debug ability grid, drawn by Func_0804128c: the cursor moves over four
/// columns of nine abilities (gData_080aa170); A, L and R give the chosen
/// ability, packed with its owner, to slot 0, 1 or 2 and set units 4 to 7 to
/// 500 PP; B cancels. Returns the slot, or -1.
s32 Func_08041350(void) {
  s32 row = 0;
  u8 *work;
  s16 left;
  struct Adventure *party;
  struct RenderInput *window;
  s32 result;
  s32 column;
  s32 ability;
  s32 owner;
  u8 *events;
  s32 x;
  s32 y;
  s32 pp;
  /* FAKEMATCH: the column count as a variable makes the wrap a __modsi3 call,
   * as the native has it; a constant 4 is reduced to shifts (differ 84). */
  s32 columns = 4;

  /* FAKEMATCH: KEYS_PRESSED reads the pressed keys through a volatile view,
   * so that each test loads them again; read plainly they are loaded once
   * (differ 33). */
  Func_0804121c();
  work = Ram_HeapSlots->unkd0[0];
  WaitFrames(1);
  result = -1;
  column = 0;
  window = Func_0804128c();
  do {
    if (result != 0) {
      column = (column + columns) % columns;
      row = (row + 9) % 9;
      /* FAKEMATCH: the window's x through a halfword temporary copies the
       * window out of r10 for each field; read in place it is copied once
       * (differ 23). */
      left = window->x;
      x = (column * 7 + left) * 8 - 4;
      y = ((s16)window->y << 3) + (row << 4) + 12;
      if (result == -1)
        Func_08108048(work + 0x5a4, x, y);
      else
        Func_08108040(work + 0x5a4, x, y, 3);
      result = 0;
    }
    WaitFrames(1);
    if (KEYS_PRESSED & 2) {
      Sound_PlayFar(113);
      result = -1;
      goto done;
    }
    if (KEYS_PRESSED & 1) {
      Sound_PlayFar(112);
      result = 0;
      goto done;
    }
    if (KEYS_PRESSED & 0x200) {
      Sound_PlayFar(112);
      result = 1;
      goto done;
    }
    if (KEYS_PRESSED & 0x100) {
      Sound_PlayFar(112);
      result = 2;
      goto done;
    }
    if (gInput.repeat & 0x20) {
      column--;
      result = 1;
      Sound_PlayFar(111);
    }
    if (gInput.repeat & 0x10) {
      column++;
      result = 1;
      Sound_PlayFar(111);
    }
    if (gInput.repeat & 0x40) {
      row--;
      result = 1;
      Sound_PlayFar(111);
    }
    if (gInput.repeat & 0x80) {
      row++;
      result = 1;
      Sound_PlayFar(111);
    }
  } while (1);
done:
  UiWork_Finalize(window, 2);
  Func_08041254();
  WaitFrames(1);
  if (result != -1) {
    events = Ram_HeapSlots->eventWork;
    ability = gData_080aa170[column][row];
    owner = Party_FindOwnerWithActionFar(ability);
    if (owner == -1)
      owner = gAdventure.currentOwner;
    if (ability == 0) {
      if (result == 0)
        *(u16 *)(events + 0x166) = 0;
      if (result == 1)
        gAdventure.packedEvents[0] = 0;
      if (result == 2) {
        /* FAKEMATCH: the one-pass block loads gAdventure's address before
         * it starts the field's offset; without it the offset comes first
         * (differ 2). */
        do {
          party = &gAdventure;
        } while (0);
        party->packedEvents[1] = 0;
      }
    } else {
      owner = (owner << 10) | (ability & 0x3fff);
      if (result == 0)
        *(u16 *)(events + 0x166) = owner;
      if (result == 1)
        gAdventure.packedEvents[0] = owner;
      if (result == 2) {
        /* FAKEMATCH: as above (differ 2 without it). */
        do {
          party = &gAdventure;
        } while (0);
        party->packedEvents[1] = owner;
      }
    }
    pp = 500;
    ((struct BattleUnit *)Unit_Get(4))->pp = pp;
    ((struct BattleUnit *)Unit_Get(5))->pp = pp;
    ((struct BattleUnit *)Unit_Get(6))->pp = pp;
    ((struct BattleUnit *)Unit_Get(7))->pp = pp;
    ((struct BattleUnit *)Unit_Get(4))->maxPp = pp;
    ((struct BattleUnit *)Unit_Get(5))->maxPp = pp;
    ((struct BattleUnit *)Unit_Get(6))->maxPp = pp;
    ((struct BattleUnit *)Unit_Get(7))->maxPp = pp;
  }
  return result;
}
