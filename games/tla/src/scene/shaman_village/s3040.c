#include "shaman_village.h"

extern u16 gData_0200515a[];

void Func_02002f20(s32 a, s32 b);
void Func_02002fb0(s32 a);

void Func_02003040(void) {
  gData_0200515a[15] = 180;
  FieldEvent_Wait(30);
  Import_SoundPlay(78);
  Func_02002fb0(5);
  Func_02002f20(2, 0);
  Import_SoundPlay(236);
  FieldEvent_Wait(60);
  Func_02002f20(2, 1);
  Import_SoundPlay(236);
  FieldEvent_Wait(60);
  Func_02002fb0(6);
  Func_02002f20(2, 0);
  Import_SoundPlay(236);
  FieldEvent_Wait(60);
  Func_02002fb0(7);
  Func_02002f20(4, 0);
  Import_SoundPlay(237);
  Import_SoundPlay(76);
}
