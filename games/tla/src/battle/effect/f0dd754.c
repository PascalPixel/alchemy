#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "iwram_call.h"

/// The Broken Seal's FieldEffect_WatchLeaderDistance with this game's
/// offsets: counts the timer down and, once the leader is 60 pixels from the
/// centre or the timer has run out, requests the effect's end.
void Func_080dd754(void) {
  u8 *work;
  struct Actor *actor;
  s16 *timer;
  s32 dx;
  s32 dz;
  struct MainRoutineWork *party;

  work = Ram_HeapSlots->eventWork;
  party = &gMainRoutineWork;
  actor = ObjectTable_Get(party->currentOwner);
  if (*(s8 *)(work + 0xca5) != 0) {
    timer = (s16 *)(work + 0xca6);
    if (*timer != 0)
      (*timer)--;
  }

  dx = Iwram_MulQ16(*(s16 *)(work + 0xca8) - actor->x / 0x10000, 0xd105);
  dz = *(s16 *)(work + 0xcaa) - (actor->z - actor->y) / 0x10000;
  if (dx * dx + dz * dz >= 3600 || *(s16 *)(work + 0xca6) == 0) {
    u16 *slot = (u16 *)(work + 358);
    u32 request = 0x2090;

    *slot = request;
  }
}
