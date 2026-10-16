#include "mars_lighthouse_aerie.h"

void Func_02004fac(void) {
  gData_02006cf0 = 0;
  gData_02006cec = Func_0200508c(gData_02006ce0);
  gData_02006cdc = 15;
  gData_02006cd8 = 16;
  gData_02006cd0 = 1;
  Scheduler_AddOrUpdateCallback(Func_020050b0, 0x480);
}

void Func_02005010(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  Scheduler_RemoveCallbackFar(Func_020050b0);
}

void Func_02005030(void) {
  gData_02006cf0 = 0;
  gData_02006cec = Func_0200508c(gData_02006ce0);
  gData_02006cdc = 2;
  gData_02006cd8 = 2;
  gData_02006cd0 = 1;
  Scheduler_AddOrUpdateCallback(Func_020050b0, 0x480);
}
