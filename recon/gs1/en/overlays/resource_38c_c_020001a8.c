#include "resource_38c.h"

#include "types.h"

extern void Func_020006c4(void);
extern void Func_02000702(s32);
extern void Func_02000704(s32, s32, s32);
extern void Func_02000724(s32, s32);
extern void Func_02000748(s32, s32, s32);
extern void Func_020006f4(void);

/* resource_38c owner at 0x020001a8: twin of 0x02000430 over the low veneer
 * bank. */
void Func_020001a8(void) {
  s32 scale;

  Func_020006c4();
  Func_02000702(0x13ed);
  Func_02000704(21, 0, 0);
  Func_02000724(21, 0);
  scale = 192;
  Func_02000748(21, scale << 8, 10);
  Func_020006f4();
}
