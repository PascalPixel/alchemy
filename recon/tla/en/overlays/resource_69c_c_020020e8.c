// Drafts of overlay 69C Func_020020e8 and Func_02002198 (first written
// 2026-10-12); overlay 6A3 has the same two functions at 0200c4dc and 0200c58c.
// Func_020020e8: differ 76 of 76 rows counted, nearly all one register exchange
// (the work in r6 and the index in r7 in the native) that follows from the loop
// bound: the native loads the count once before the loop as an unsigned
// halfword (ldrh r2, [r6, #10]) and sign-extends it on every pass, where loop.c
// here moves the load and both shifts out (insn 45 and 46 in the loop dump, 54
// insns) and CSE joins them with the entry test. Measured: an unsigned field
// with a cast, the count through an inline, returns in place of breaks, the
// flag services through the inline layer (all the same). Func_02002198: differ
// 59 of 81: the native keeps dx in r9 and dz in r11, x in r7 and the object in
// r6, and adds x + dx before z + dz though it stores z first; adding in place
// or into dx first is worse (85, 83).
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

extern struct PushWork gData_02002e34;

void Func_02001d48(struct PushWork *work, struct PushEntry *entry);
void Func_02001db8(struct PushWork *work, struct PushEntry *entry);
void Func_02001f50(void);
void Func_02002308(void);
void GameFlag_SetByte(s32 flag, s32 value);
s32 GameFlag_GetByte(s32 flag);

static __inline__ void SetByte(s32 flag, s32 value) {
  GameFlag_SetByte(flag, value);
}

s32 Func_020020e8(void) {
  struct ObjectRuntime *object = Object_GetById(gAdventure.currentOwner);
  struct PushWork *work = &gData_02002e34;
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct PushEntry *entry;
  s32 i;

  for (i = 0, entry = work->entries; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (GameFlag_Test_(entry->flag) == 0) {
        Func_02001d48(work, entry);
        GameFlag_SetBit_(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_02001db8(work, entry);
      SetByte(work->flag, x);
      SetByte(work->flag + 8, z);
      return 1;
    }
  }
  return 0;
}

void Func_02002198(s32 dx, s32 dz, s32 route) {
  struct ObjectRuntime *object = Object_GetById(gAdventure.currentOwner);
  struct PushWork *work = &gData_02002e34;
  u8 *eventWork = Ram_HeapSlots->eventWork;
  s32 x = GameFlag_GetByte(work->flag);
  s32 z = GameFlag_GetByte(work->flag + 8);

  if (x != 0 || z != 0) {
    struct ObjectRuntime *other;

    GameFlag_SetByte(work->flag, 0);
    GameFlag_SetByte(work->flag + 8, 0);
    dx += x;
    object->z = ((z + dz) << 20) + 0x80000;
    object->x = (dx << 20) + 0x80000;
    other = *(struct ObjectRuntime **)(eventWork + 0x1cc);
    other->x = object->x;
    other->z = object->z;
    Func_02002308();
    Func_02001f50();
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = route;
  }
}
