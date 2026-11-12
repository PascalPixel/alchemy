#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"
#include "cell_target.h"

extern struct CellTarget Data_02004458;

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02002d40(struct CellTarget *work,
                   struct CellTargetEntry *entry);
void Func_02002db0(struct CellTarget *work,
                   struct CellTargetEntry *entry);

s32 Func_020030e0(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *work = &Data_02004458;
  struct CellTargetEntry *entry = work->entries;
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        Func_02002d40(work, entry);
        Event_SetFlag(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_02002db0(work, entry);
      GameFlag_SetByte(work->flagByte, x);
      GameFlag_SetByte(work->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}
