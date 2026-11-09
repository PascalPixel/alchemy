#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"

struct JupiterLighthouseAerieWork {
  u8 unk00[2];
  s16 flag;
  u8 unk04[2];
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  u8 unk0e[18];
};

struct JupiterLighthouseAerieEntry {
  s16 x;
  s16 z;
  s16 flag;
  u8 unk06[2];
};

extern u8 Data_020068b4[];

void GameFlag_SetByte(s32 flag, s32 value);
void Func_020040f0(struct JupiterLighthouseAerieWork *work,
                   struct JupiterLighthouseAerieEntry *entry);
void Func_02004160(struct JupiterLighthouseAerieWork *work,
                   struct JupiterLighthouseAerieEntry *entry);

s32 Func_02004490(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct JupiterLighthouseAerieWork *work = (struct JupiterLighthouseAerieWork *)Data_020068b4;
  struct JupiterLighthouseAerieEntry *entry = (struct JupiterLighthouseAerieEntry *)(work + 1);
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        Func_020040f0(work, entry);
        Event_SetFlag(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_02004160(work, entry);
      GameFlag_SetByte(work->flag, x);
      GameFlag_SetByte(work->flag + 8, z);
      return 1;
    }
  }
  return 0;
}
