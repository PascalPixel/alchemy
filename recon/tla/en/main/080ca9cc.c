// Near miss, differ 4 of 43 (tla-en, 2026-10-31): sched2 order only. Both key
// loads are zero-extended ldrb's straight after a store to the cursor's slot.
// The native waits a cycle before each one (adds r6, r1 at the top, adds r3, #5
// in the loop go first). Here sched2 readies the load one cycle after the
// store (cost 1), because -mthumb-slow-loads' store rule skips extended loads.
// A whole QImode load would wait, but only a sign-extending read gives one,
// since a char local is promoted and loaded zero-extended: s32 key =
// (s8)*cursor++ waits in the loop but becomes ldrsb at the top and drops the
// per-compare asrs (differ 31). The FIXED_ADDRESS read with this loop's asm
// and no top barrier: differ 6. Every loop shape tried (while with the read
// in the condition, for (;;) with break, a for with the step, key =
// *(cursor += 4)) measures the same as the plain form, symbol or constant.
// Read through the constant address (FIXED_ADDRESS((u8 *),
// 0x0202e008, gData_0202e008), as the Ram_ buffers are), plain `key = *cursor;
// cursor++;` keeps both of the cursor's stores with no device: differ 8 of 43,
// the two key loads one slot early.
// Draft, differ 4 of 43 (6 before the loop's barriers): the native stores the
// cursor to its stack slot before the first key read (str r3, [sp]; ldrb r2,
// [r3]); flow drops that store as dead unless something stands between it
// and the step's store, and the empty asm at the top keeps it (differ 11
// without it; a one-pass block there differs 7). In the loop, a second
// volatile asm keeps the store of cursor + 4 and an asm reading the stepped
// cursor makes the key's load wait for its step (adds r3, #5) as the
// native's does. What remains: the top asm lets the copy of position (adds
// r6, r1) move ahead of the store, and in the loop the cursor + 5 store comes
// before the key's load (the native loads first). The store after the load
// (next = at + 1; key = *at; cursor = next) gives the native's order but
// moves the key into r1 (differ 20). A for loop is the same; a reader inline
// (s8 Stream_ReadS8(u8 **)) sign-extends twice (differ 37), a u8 one changes
// nothing.
#include "types.h"

extern u8 gData_0202e008[];

u32 Stream_ReadU16(u8 **cursor);

s32 Func_080ca9cc(s32 id, s32 *position) {
  u8 *cursor = gData_0202e008;
  s8 key;

  asm volatile("");
  key = *cursor++;

  while (key != -1) {
    if (key == id) {
      position[0] = Stream_ReadU16(&cursor) << 16;
      position[2] = Stream_ReadU16(&cursor) << 16;
      return 0;
    }
    cursor += 4;
    {
      u8 *at = cursor;
      asm volatile("");
      cursor = at + 1;
      __asm__("" : "+l"(at) : "l"(cursor));
      key = *at;
    }
  }
  return -1;
}
