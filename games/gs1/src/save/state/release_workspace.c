#include "types.h"
#include "scene.h"
#include "runtime_interfaces.h"

typedef void (*InterruptHandler)(void);

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
  State_Place(5, arg, (InterruptHandler)fn);
 return State_Do(id);
}
