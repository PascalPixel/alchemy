#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "input.h"

/// The requests the configured keys raise.
// Named as in ☀️'s FieldWork.
struct EventPrompt {
  u8 unk000[0x15a];
  u16 menu;
  u16 examine;
  u16 start;
  u8 unk160[6];
  u16 choice[2];
  u16 result;
  u8 unk16c[30];
  s8 mode;
  u8 unk18b[0xb21];
  s8 pendingSecond;
};

s32 GameFlag_Test(s32 flag);

s32 Func_08026dd0(u32 value) {
  struct EventPrompt *event = Ram_HeapSlots->eventWork;
  u32 kind = value >> 14;
  s32 code = value & 0x3fff;

  if (GameFlag_Test(0x107) != 0) {
    event->result = 250;
  } else if (event->mode == 3) {
    // The held keys, read again for the second test through a volatile
    // pointer; through a plain pointer, 14 of 69 instructions differ in
    // tla-en.
    volatile u32 *held = &gInput.held;

    if (*held & 0x100)
      event->result = 0xfc88;
    else if (*held & 0x200)
      event->result = 0xfc87;
  } else {
    switch (kind) {
    case 0:
      event->choice[0] = code;
      break;
    case 1:
      event->choice[1] = code;
      break;
    }
  }
  return code;
}

/// Checks the keys the player has configured: the menu, examine and start
/// keys set their request and return 1, the two shortcut keys pass on their
/// assignment (Func_08026dd0). While a second event is pending, any of the
/// five requests command 0x209b instead. Returns 0 when none is pressed.
// As ☀️'s function of the same name.
s32 Field_CheckConfiguredKeys(void) {
  struct EventPrompt *work = Ram_HeapSlots->eventWork;
  s32 result = 0;

  if (work == NULL)
    return 0;
  if (work->pendingSecond != 0) {
    if ((gInput.pressed & gMainRoutineWork.keys234) ||
        (gInput.pressed & gMainRoutineWork.keys230) ||
        (gInput.pressed & gMainRoutineWork.keys236) ||
        (gInput.pressed & gMainRoutineWork.keys238) ||
        (gInput.pressed & gMainRoutineWork.keys23a)) {
      work->choice[0] = 0x209b;
      return 1;
    }
    return 0;
  } else {
    if (gInput.pressed & gMainRoutineWork.keys234) {
      work->menu = 1;
      result = 1;
    } else if (gInput.pressed & gMainRoutineWork.keys230) {
      work->examine = 1;
      result = 1;
    } else if (gInput.pressed & gMainRoutineWork.keys236) {
      work->start = 1;
      result = 1;
    } else if (gInput.pressed & gMainRoutineWork.keys238) {
      result = Func_08026dd0(gMainRoutineWork.packedEvents[0]);
    } else if (gInput.pressed & gMainRoutineWork.keys23a) {
      result = Func_08026dd0(gMainRoutineWork.packedEvents[1]);
    }
  }
  return result;
}
