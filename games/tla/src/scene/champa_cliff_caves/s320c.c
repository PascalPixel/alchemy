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
void wait(s32 frames);
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
