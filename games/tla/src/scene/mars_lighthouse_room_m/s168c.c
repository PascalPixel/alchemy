#include "mars_lighthouse_room_m.h"

void Func_0200168c(void) {
  gData_02003938 = 0;
  gData_02003934 = Func_020016e8(gData_02003928);
  gData_02003924 = 2;
  gData_02003920 = 2;
  gData_02003918 = 1;
  Value2((s32 (*)())Scheduler_AddOrUpdateCallback, (s32)Func_0200170c, 0x480);
}
