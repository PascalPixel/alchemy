#include "event_inline.h"
#include "scene.h"
#include "task.h"

struct ChampaCliffCavesWork {
  u8 unk0000[0x1000];
  s32 field1000;
  s32 field1004;
  s32 field1008;
  u8 unk100c[0x58];
  s32 field1064;
  u8 unk1068[4];
  s32 field106c;
  s32 field1070;
  u16 field1074;
  u8 unk1076[2];
};

void *HeapSlot_AllocFar(s32 slot, s32 size);
void Func_02002d34(void);
void Func_02002de4(void);

void Func_0200320c(void) {
  struct ChampaCliffCavesWork *work =
      HeapSlot_AllocFar(220, sizeof(struct ChampaCliffCavesWork));
  s32 state;

  Func_02002d34();
  {
    s32 priority = 0x480;

    Task_Add(Func_02002de4, priority);
  }
  Sound_Play(144);
  state = 0;
  do {
    switch (state) {
    case 0:
      work->field106c += 0x1000;
      if (work->field106c > 0x6665) {
        work->field106c = 0x6666;
        state = 1;
      }
      break;
    case 1:
      work->field106c += 0x3000;
      if (work->field106c > 0x14ccb) {
        work->field106c = 0x14ccc;
        state = 999;
      }
      break;
    }
    wait(1);
  } while (state != 999);
}

#include "types.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Resource_ResetEntry(u32 slot);
void HeapSlot_Release(s32 slot);

void Func_020032a4(void) {
  u8 *work = Ram_HeapSlots->menuRuntime;
  s32 state = 0;
  s32 time = 0;

  do {
    switch (state) {
    case 0:
      if ((FIELD_AT(work, s32, 4204) += 0x1000) > 0x1ffff) {
        FIELD_AT(work, s32, 4204) = 0x20000;
        state = 1;
        time = -1;
      }
      break;
    case 1:
      if ((FIELD_AT(work, s32, 4204) -= 0x1000) <= 0x14ccb) {
        FIELD_AT(work, s32, 4204) = 0x14ccc;
        state = 2;
        time = -1;
      }
      /* fall through */
    case 2:
      if ((FIELD_AT(work, s32, 4204) -= 0x1000) < 0)
        FIELD_AT(work, s32, 4204) = 0;
      if ((FIELD_AT(work, s32, 4196) -= 0x1000) < 0) {
        FIELD_AT(work, s32, 4196) = 0;
        state++;
        time = -1;
      }
      break;
    case 3:
      if (time == 1)
        state = 999;
      break;
    }
    wait(1);
    time++;
  } while (state != 999);
  Task_RemoveFar(Func_02002de4);
  Resource_ResetEntry(FIELD_AT(work, s16, 4188));
  Resource_ResetEntry(FIELD_AT(work, s16, 4192));
  HeapSlot_Release(220);
}

#include "scene_tables.h"

u32 gData_02003634[] = {
    439,
    440,
    441,
};
