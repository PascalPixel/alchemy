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

extern u8 Data_0200354c[];

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02002248(struct JupiterLighthouseAerieWork *work,
                   struct JupiterLighthouseAerieEntry *entry);
void Func_020022b8(struct JupiterLighthouseAerieWork *work,
                   struct JupiterLighthouseAerieEntry *entry);

s32 Func_020025e8(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct JupiterLighthouseAerieWork *work = (struct JupiterLighthouseAerieWork *)Data_0200354c;
  struct JupiterLighthouseAerieEntry *entry = (struct JupiterLighthouseAerieEntry *)(work + 1);
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (GameFlag_Test(entry->flag) == 0) {
        Func_02002248(work, entry);
        GameFlag_SetBit(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_020022b8(work, entry);
      GameFlag_SetByte(work->flag, x);
      GameFlag_SetByte(work->flag + 8, z);
      return 1;
    }
  }
  return 0;
}
