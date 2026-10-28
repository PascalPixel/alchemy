#include "room.h"

extern u16 gData_0200687c;
extern u16 gData_02006884;
extern u16 gData_02006898;
extern u16 gData_0200689c;
extern u16 gData_0200e888;
extern u16 *gData_0200e88c;

s32 Task_Add(void (*callback)(void), s32 priority);
s32 Func_02004794(u16 *list);
void Func_020047b8(void);

void Func_02004738(void) {
  gData_0200689c = 0;
  gData_02006898 = Func_02004794(gData_0200e88c);
  gData_0200e888 = 2;
  gData_02006884 = 2;
  gData_0200687c = 1;
  Task_Add(Func_020047b8, 0x480);
}
