#include "types.h"
#include "ram_buffer.h"
#include "sequence_parent_work.h"
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
    /* FAKEMATCH: the native code reads the held keys again for the second test.
     */
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
    struct SequenceParentWork *party = &gSequenceParentWork;
    /* FAKEMATCH: the pressed keys are read again for each test; through a
     * plain pointer one read serves all, in both branches (differ 141). */
    volatile struct InputState *input = (volatile struct InputState *)&gInput;

    /* FAKEMATCH: the native loads gSequenceParentWork before the input; without
     * the barrier the input comes first (differ 2). */
    asm volatile("" : "+l"(party));
    if ((input->pressed & party->keys234) ||
        (input->pressed & party->keys230) ||
        (input->pressed & party->keys236) ||
        (input->pressed & party->keys238) ||
        (input->pressed & party->keys23a)) {
      work->choice[0] = 0x209b;
      return 1;
    }
    return 0;
  } else {
    struct SequenceParentWork *party = &gSequenceParentWork;
    /* FAKEMATCH: the pressed keys are read again for each test, as above. */
    volatile struct InputState *input = (volatile struct InputState *)&gInput;

    /* FAKEMATCH: the barrier of gSequenceParentWork's load, as above (differ 2
     * without it). */
    asm volatile("" : "+l"(party));
    if (input->pressed & party->keys234) {
      work->menu = 1;
      result = 1;
    } else if (input->pressed & party->keys230) {
      work->examine = 1;
      result = 1;
    } else if (input->pressed & party->keys236) {
      work->start = 1;
      result = 1;
    } else if (input->pressed & party->keys238) {
      result = Func_08026dd0(party->packedEvents[0]);
    } else if (input->pressed & party->keys23a) {
      result = Func_08026dd0(party->packedEvents[1]);
    }
  }
  return result;
}
