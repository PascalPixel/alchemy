// Draft, differ 32 of 76, skeleton gap 0 (tla-en, 2026-11-01): Func_02003f18;
// Func_02003fc8 in the same file matches, differ 0 of 81. Overlays 6aa, 69e and
// 6a4 hold the same pair and can take this loop.
// Left in Func_02003f18: work and index swap r6 and r7, nothing else. The
// priorities are 3 * refs / length: index has 11 refs (its bottom block counts
// double) over 58, work 11 over 76, and the listing wants work first. Work needs
// 15 refs or index 8. Declaration order, a cast for entries, reads through the
// global and a reversed current test leave the swap or make it worse.
// The loop is built from labels because a for or do-while lets loop.c hoist the
// count's two shifts (threshold 15 * 2 * 2 >= 54 insns); the listing keeps them at
// the bottom on the halfword gcse left in r2. A for with 61 or more insns would
// also stop the hoist, but indexing the array to get them moves entry's setup
// after the test, and two return 0s leave a movs before the compare.
#include "../../../../games/tla/src/scene/mars_lighthouse_room_f/room.h"

struct WarpEntry {
  s16 x;
  s16 z;
  s16 flag;
  s16 unk6;
};

struct WarpWork {
  s16 unk00;
  s16 positionFlag;
  s16 unk04;
  s16 sourceX;
  s16 sourceZ;
  s16 count;
  s16 current;
  s16 busy;
  u8 unk10[0x10];
  struct WarpEntry entries[1];
};

extern struct WarpWork gData_020070a8;

void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
struct Actor *Func_020055e8(void);
void Func_020054b0(void);
void Func_02003af0(void);
void Func_02003b78(struct WarpWork *work, struct WarpEntry *entry);
void Func_02003be8(struct WarpWork *work, struct WarpEntry *entry);
void Func_02003d80(void);

s32 Func_02003f18(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct WarpWork *work = &gData_020070a8;
  struct WarpEntry *entry = work->entries;
  s32 index;

  index = 0;
  if (index >= work->count)
    goto end;
top:
  if (entry->x == x && entry->z == z) {
    if (!GameFlag_Test(entry->flag)) {
      Func_02003b78(work, entry);
      GameFlag_SetBit(entry->flag);
      work->current = index;
      goto end;
    }
    if (index == work->current)
      goto end;
    work->current = index;
    Func_02003be8(work, entry);
    GameFlag_SetByte(work->positionFlag, x);
    GameFlag_SetByte(work->positionFlag + 8, z);
    return 1;
  }
  index++;
  entry++;
  if (index < work->count)
    goto top;
end:
  return 0;
}

void Func_02003fc8(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  u8 *event = Ram_HeapSlots->eventWork;
  struct WarpWork *work = &gData_020070a8;
  s32 x = GameFlag_GetByte(work->positionFlag);
  s32 z = GameFlag_GetByte(work->positionFlag + 8);

  if (x != 0 || z != 0) {
    struct Actor *other;

    GameFlag_SetByte(work->positionFlag, 0);
    GameFlag_SetByte(work->positionFlag + 8, 0);
    object->x = ((x + dx) << 20) + 0x80000;
    object->z = ((z + dz) << 20) + 0x80000;
    other = *(struct Actor **)(event + 460);
    other->x = object->x;
    other->z = object->z;
    Func_020054b0();
    Func_02003d80();
    WORD_AT(Ram_HeapSlots->eventWork, 428) = value;
  }
}
