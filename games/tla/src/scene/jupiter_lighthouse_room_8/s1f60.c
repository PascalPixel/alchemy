#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"

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

extern u8 gData_02002bf8[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void Func_02001bc0(struct JupiterLighthouseAerieWork *work,
                   struct JupiterLighthouseAerieEntry *entry);
void Func_02001c30(struct JupiterLighthouseAerieWork *work,
                   struct JupiterLighthouseAerieEntry *entry);

s32 Func_02001f60(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct JupiterLighthouseAerieWork *work = (struct JupiterLighthouseAerieWork *)gData_02002bf8;
  struct JupiterLighthouseAerieEntry *entry = (struct JupiterLighthouseAerieEntry *)(work + 1);
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (GameFlag_Test(entry->flag) == 0) {
        Func_02001bc0(work, entry);
        GameFlag_SetBit(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_02001c30(work, entry);
      GameFlag_SetByte(work->flag, x);
      GameFlag_SetByte(work->flag + 8, z);
      return 1;
    }
  }
  return 0;
}
