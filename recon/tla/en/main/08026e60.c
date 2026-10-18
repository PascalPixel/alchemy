// Draft, differ 128 of 138: reads the party's button bindings on the
// field: while the event work is locked any bound key asks for event
// 0x209b; otherwise the first three bindings raise a request flag and the
// last two run their stored event through Func_08026dd0. Returns 1 when a
// key was taken. No ☀️ twin found. Left: the native rereads gInput.pressed
// before every test, as if the input record were volatile when this was
// compiled (see 0801399c.c); the draft reads it once. Through a volatile
// cast of gInput, differ 29, then only register choice (the native puts
// gInput in r0 in the locked branch and sets the 0 result after the last
// load).
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "party_state.h"

struct FieldEventWork {
  u8 unk000[0x15a];
  u16 requests[3];
  u8 unk160[6];
  u16 menuRequest;
  u8 unk168[0xb44];
  s8 locked;
};

#define KEYS238 (*(u16 *)&gPartyState.unk238[0])
#define KEYS23A (*(u16 *)&gPartyState.unk238[2])

s32 Func_08026dd0(s32 id);

s32 Func_08026e60(void) {
  struct FieldEventWork *work = Ram_HeapSlots->eventWork;
  s32 result = 0;

  if (work == 0)
    return result;
  if (work->locked != 0) {
    if ((gInput.pressed & gPartyState.keys234) ||
        (gInput.pressed & gPartyState.keys230) ||
        (gInput.pressed & gPartyState.keys236) ||
        (gInput.pressed & KEYS238) || (gInput.pressed & KEYS23A)) {
      work->menuRequest = 0x209b;
      result = 1;
    }
  } else if (gInput.pressed & gPartyState.keys234) {
    work->requests[0] = 1;
    result = 1;
  } else if (gInput.pressed & gPartyState.keys230) {
    work->requests[1] = 1;
    result = 1;
  } else if (gInput.pressed & gPartyState.keys236) {
    work->requests[2] = 1;
    result = 1;
  } else if (gInput.pressed & KEYS238) {
    result = Func_08026dd0(gPartyState.packedEvents[0]);
  } else if (gInput.pressed & KEYS23A) {
    result = Func_08026dd0(gPartyState.packedEvents[1]);
  }
  return result;
}
