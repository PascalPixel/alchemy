// Near miss, differ 5 of 61 (tla-en, 2026-10-31): only the entry block
// differs; the native starts the end mark 0xffff (movs r1, #255) before the
// entry jump and copies the count to r10 after the mode, both left by the
// first test described below. Overlay 6a0's Func_02001a48 is the same
// function with Func_02001acc for Func_020054ac.
// Entered at its test through a goto into the do-while, as the native enters
// it, and with the object pinned to r6 (as a plain local, 23), the loop has the
// native's shape and registers; reading the list word into a local before the
// id gives the native's load order (7 before). An end mark pinned to r1 and
// set before the jump and at the end of the body loses the first set and swaps
// r2 and r3 (18). The native's entry is a first test (if (*list == 0xffff)
// return; before a goto loop, or a loop with its exit test copied) that
// jump2's cross-jumping merged into the loop's own test after sched2 had moved
// the table pointer load between its first two rows: written so, this draft
// gets the same entry jump, but gcse's PRE then hoists the id load onto the
// edge into the loop (ldrh before it, the sign extension apart, 21 to 27), and
// as a while or for loop loop.c also hoists 0xffff into a register (38); with
// the id read through a volatile pointer it stays inside (25, its extension
// still apart). Reading the effect work through Ram_HeapSlots inside the loop
// keeps the id inside but leaves the work load there too (32); an endless loop
// with the end test at its top keeps the id inside but hoists 0xffff and puts
// the test first (26 with return, 21 with break).
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

struct GabombaStatueEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void Func_020054ac(s32 id, s32 a, s32 flag, s32 terrain);

// Finds the current target's table row and shrinks it.
void Func_02005428(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  while (*list != 0xffff) {
    u32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
      struct Actor *object = Actor_GetById(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        GameFlag_SetBit(flag);
        Func_020054ac(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      break;
    }
    list += 3;
  }
}
