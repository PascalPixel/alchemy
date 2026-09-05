#include "types.h"
#include "runtime_interfaces.h"

typedef void (*InterruptHandler)(void);

u32 Func_08002dd8(s32);
void Func_0800307c(s32, s32, InterruptHandler);
u32 SaveState_ReleaseWorkspace(void)
{
  int fn;
  long long id;
  long long tmp;
  int arg;
  unsigned int no;
  fn = 0;
  id = (tmp = (no = 0x33));
  arg = 0;
  Func_0800307c(5, arg, (InterruptHandler)fn);
 return Func_08002dd8(id);
}
