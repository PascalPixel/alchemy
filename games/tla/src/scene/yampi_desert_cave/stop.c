#include "yampi_desert_cave.h"
#include "task.h"

void Task_RemoveFar(void (*callback)(void));

void Func_02000898(void) {
  gData_02001fe8 = 0;
  gData_02001fe4 = Func_02000978(gData_02001fd8);
  gData_02001fd4 = 15;
  gData_02001fd0 = 16;
  gData_02001fc8 = 1;
  Task_Add(Func_0200099c, TASK_PASS_VBLANK);
}

void Func_020008fc(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  Task_RemoveFar(Func_0200099c);
}

void Func_0200091c(void) {
  gData_02001fe8 = 0;
  gData_02001fe4 = Func_02000978(gData_02001fd8);
  gData_02001fd4 = 2;
  gData_02001fd0 = 2;
  gData_02001fc8 = 1;
  Task_Add(Func_0200099c, TASK_PASS_VBLANK);
}
