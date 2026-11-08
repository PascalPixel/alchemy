#include "types.h"
#include "task.h"

extern u16 gData_02004434;
extern u16 gData_0200443c;
extern u16 gData_02004440;
extern u16 *gData_02004444;
extern u16 gData_02004450;
extern u16 gData_02004454;

s32 Func_020001d4(u16 *list);
void Func_020001f8(void);

void Func_020000f4(void) {
  gData_02004454 = 0;
  gData_02004450 = Func_020001d4(gData_02004444);
  gData_02004440 = 15;
  gData_0200443c = 16;
  gData_02004434 = 1;
  Task_Add(Func_020001f8, TASK_PASS_VBLANK);
}
