// Matches (tla-en, 2026-11-07). Overlay 69d's Func_02001c68 (698's
// Func_020018bc and 6ad's Func_02003fc8 are the same routine), written as
// jupiter_lighthouse_entrance's matched Func_020018bc: the puzzle's flag and
// the event work read through structs. GameFlag_GetByte must return a word
// here: as u8 the results are masked.
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"

struct Puzzle {
  s16 unk00;
  s16 flag;
};

struct PuzzleEventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
  u8 unk1b0[0x1c];
  struct Actor *marker;
};

extern struct Puzzle gData_02002864;

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void Func_02001dd0(void);
void Func_02001a20(void);

void Func_02001c68(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct PuzzleEventWork *work = Ram_HeapSlots->eventWork;
  s32 dx = GameFlag_GetByte(gData_02002864.flag);
  s32 dz = GameFlag_GetByte(gData_02002864.flag + 8);

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(gData_02002864.flag, 0);
    GameFlag_SetByte(gData_02002864.flag + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = work->marker;
    marker->x = leader->x;
    marker->z = leader->z;
    Func_02001dd0();
    Func_02001a20();
    ((struct PuzzleEventWork *)Ram_HeapSlots->eventWork)->screenEffect = effect;
  }
}
