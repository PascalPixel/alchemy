#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

struct MagmaRockWork {
  s32 unk00;
  s32 x;
  s32 y;
  s32 z;
  s32 unk10;
  struct Actor *target;
};

struct Actor *Actor_GetById(s32 id);
s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);

s32 Func_02001084(u16 *table) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  s32 result = 0;
  s32 code = MapCell_GetAttributes(0, work->x >> 20, work->z >> 20) >> 8;
  s32 id;

  goto check;
next:
  result = *table++;
check:
  if (*table == 0xffff)
    goto done;
  id = *table++;
  if (code != *table++)
    goto next;
  work->target = Actor_GetById(id);
  result = *table;
done:
  return result;
}
