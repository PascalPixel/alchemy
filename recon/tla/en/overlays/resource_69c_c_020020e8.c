// Draft, differ 32 of 76, skeleton gap 0 (tla-en, 2026-11-01): Func_020020e8;
// Func_02002198 matches, differ 0, in the form of the adopted
// games/tla/src/scene/jupiter_lighthouse_room_3/s2198.c. Overlay 6A3 has the same
// two functions at 0200c4dc and 0200c58c.
// Func_020020e8: all 32 rows are one register exchange, work in r7 and i in r6
// where the native has work in r6 and i in r7. Global allocation takes the entry
// pointer, then i (11 refs across 60 insns), then work (11 across 84), and i is
// dead on the exit paths, so moving its initializer does not reorder them (70
// against 82 at the earliest place after the call). Tried without effect: i and
// entry declared and set in every order, an inner block for i, entry through the
// global, a copy of i inside the match. Reading the global directly in place of
// work is worse (86), and so is any of the count reads through the global (67).
// The loop is built from a label and goto with the count read twice: loop notes
// (for, while, do, for (;;)) let loop.c and CSE join the two reads, and only this
// form keeps the native's ldrsh at entry, ldrh beside it and the sign extension
// on every pass. An s16 count local loses the ldrh and the shifts (47).
#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_3/local.h"

struct PushEntry {
  s16 x;
  s16 z;
  s16 flag;
  s16 unk06;
};

struct PushWork {
  s16 unk00;
  s16 flag;
  s16 unk04;
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  u16 busy;
  u8 unk10[16];
  struct PushEntry entries[1];
};

struct PushEventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
  u8 unk1b0[0x1c];
  struct Actor *marker;
};

extern struct PushWork gData_02002e34;

void Func_02001d48(struct PushWork *work, struct PushEntry *entry);
void Func_02001db8(struct PushWork *work, struct PushEntry *entry);
void Func_02001f50(void);
void Func_02002308(void);
void GameFlag_SetByte(s32 flag, s32 value);
s32 GameFlag_GetByte(s32 flag);

s32 Func_020020e8(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  struct PushWork *work = &gData_02002e34;
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct PushEntry *entry;
  s32 i = 0;

  entry = work->entries;
  if (i < work->count) {
  next:
    if (entry->x == x && entry->z == z) {
      if (GameFlag_Test_(entry->flag) == 0) {
        Func_02001d48(work, entry);
        GameFlag_SetBit_(entry->flag);
        work->current = i;
        goto done;
      }
      if (i == work->current)
        goto done;
      work->current = i;
      Func_02001db8(work, entry);
      GameFlag_SetByte(work->flag, x);
      GameFlag_SetByte(work->flag + 8, z);
      return 1;
    }
    i++;
    entry++;
    if (i < work->count)
      goto next;
  }
done:
  return 0;
}

void Func_02002198(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct PushEventWork *eventWork = Ram_HeapSlots->eventWork;
  s32 dx = GameFlag_GetByte(gData_02002e34.flag);
  s32 dz = GameFlag_GetByte(gData_02002e34.flag + 8);

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(gData_02002e34.flag, 0);
    GameFlag_SetByte(gData_02002e34.flag + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = eventWork->marker;
    marker->x = leader->x;
    marker->z = leader->z;
    Func_02002308();
    Func_02001f50();
    ((struct PushEventWork *)Ram_HeapSlots->eventWork)->screenEffect = effect;
  }
}
