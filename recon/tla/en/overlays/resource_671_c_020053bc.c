// Func_020053bc is adopted from this draft (2026-11-01).
// Draft, differ 23 of 61, skeleton gap 3 (tla-en, 2026-11-01). Measure with
// --symbol: Func_020053bc matches, differ 0 of 51; the figures are
// Func_02005428's. Its skeleton lacks the entry's movs #255, and sl is copied
// first; object and id swap r6 and r7.
// Func_020053bc: the test on an int copy of the word (while ((value = *entry)
// != 0xffff)) is an SImode compare, whose constant is one insn that loop.c
// leaves inside the loop, as the native rebuilds 0xffff on every pass; compared
// directly the u16 test is narrowed to HImode, its constant is three insns
// (life 4, savings 2) and moves above the loop (differ 28). The id as a u16
// read with a post-increment keeps the body's own read and the parameter in r0
// for the first test; mode and flag read as entry[0] and entry[1] with the
// pointer stepped by 2 after the last call put the step after the call's
// arguments (stepped with *entry++ for all three, 2 differ).
// Func_02005428: the native's shape is a C loop whose copied exit test
// (movs #255; ldrh; lsls; adds; cmp) jump2 cross-jumped into the bottom test,
// leaving the entry's b and its movs r1, #255; jump2 runs after sched2 and
// needs the two tests' last insns equal, so no insn may be scheduled into the
// entry's test. while ((value = *entry) != 0xffff) with a u16 key = *entry in
// the body gets the bottom test, the reread and both copies of the test right
// (differ 36, gap 12), with two things in the way. gcse's PRE moves
// work->target above the loop, because the copied test gives the loop top a
// place to insert it and only the exit path holds calls and stores, so work
// dies early and sits in r0 where the native keeps it in r2. Then 0xffff takes
// r2, not r1, and mov r9, r1 lands inside the entry's test, which stops the
// cross-jump. break, return, for and if/else do not change the hoist; loading
// work inside the loop is worse (differ 62). The goto loop below has no copied
// test, so PRE leaves the read in the loop. XXX: what kept the native's read
// in the loop with a copied test; a kill or a condition before the read on the
// way to the loop top would settle it. As if ((value = *entry) != 0xffff) in
// the goto loop the gap is 2 of the same 61, but the reread is lost and differ
// is 30, so that form is not kept. Not yet a C loop and key is a temporary:
// both stay to be resolved before adoption.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void GameFlag_SetBit(s32 flag);
void Func_020054ac(s32 id, u32 mode, s32 flag, s32 layer);

struct EffectWork {
  u8 unk00[26];
  s16 target;
};

// Layer switch table: three words (id, mode, flag), terminated by 0xffff.


void Func_02005428(const u16 *entry, s32 set, s32 step) {
  struct EffectWork *work = Ram_HeapSlots->effectWork;
  struct Actor *object;
  s32 id;
  s32 mode;
  s32 flag;
  s32 key;

  // A goto loop: as while ((value = *entry) != 0xffff), 42 of 61 instructions
  // differ in tla-en.
  while (*entry != 0xffff) {
  key = entry[0];
  id = work->target;
  if (id == key) {
    object = Actor_GetById(id);
    mode = *++entry;
    flag = entry[1];

    if (step <= 7)
      object->scaleY -= 0x1f00;
    if (set == 1) {
      GameFlag_SetBit(flag);
      Func_020054ac(id, mode, flag, object->terrainId);
      object->scaleY = 0x10000;
    }
    return;
  }
  entry += 3;
  }
}
