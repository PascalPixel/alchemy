// Func_020053bc matches: differ 0 of 51 (tla-en, 2026-10-31). Func_02005428:
// near miss, differ 23 of 61 (tla-en, 2026-10-31): object and id swap r6 and
// r7, and the native builds the first byte of 0xffff before entering the loop.
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
// leaving the entry's b and its movs r1, #255; the native also rereads the
// word in the body and reads work->target on every pass. As a while loop the
// value test's read is reused in the body (while ((value = *entry) != 0xffff),
// differ 42), and while (*entry != 0xffff) keeps the reread but hoists 0xffff
// and work->target above the loop (differ 38). The goto loop below rereads
// both; key, read before the target, gives the native's load order (with id
// == entry[0], differ 25). Not yet a C loop and key is a temporary: both stay
// to be resolved before adoption.
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
void Func_020053bc(const u16 *entry) {
  s32 value;

  while ((value = *entry) != 0xffff) {
    u16 id = *entry++;
    struct Actor *object = Actor_GetById(id);
    s16 mode = entry[0];
    s16 flag = entry[1];

    object->collisionFlags &= 0xfe;
    Actor_SetSingleChildFlags(object, 0);
    Func_020054ac(id, (u16)mode, (u16)flag, object->terrainId);
    entry += 2;
  }
}

void Func_02005428(const u16 *entry, s32 set, s32 step) {
  struct EffectWork *work = Ram_HeapSlots->effectWork;
  struct Actor *object;
  s32 id;
  s32 mode;
  s32 flag;
  s32 key;

  // A goto loop: as while ((value = *entry) != 0xffff), 42 of 61 instructions
  // differ in tla-en.
  goto test;
next:
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
test:
  if (*entry != 0xffff)
    goto next;
}
