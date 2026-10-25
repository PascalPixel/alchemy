// Draft, differ 49 of 75: the credits cursor: counts down while A or B is
// held, redraws the gauge and queues its sprite over the leader. Left: the
// sprite word order and register choice around the stmia stores.
#include "types.h"
#include "object_runtime.h"
#include "vram_tab.h"

extern u32 gInput;
extern s16 gData_02003fc6;
extern s16 gData_02003fc8;
extern u32 gData_02003fcc[3];

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
s32 Func_0200209c(void);
void Func_0200200c(s32 value);
s32 Func_02002014(void);
void Func_02001be0(s32 progress);
void Render_ProjectPoint(s32 *position, s32 *screen);
void Func_02001f7c(u32 *oam, s32 priority);

void Func_02001d90(void) {
  u32 *oam = gData_02003fcc;
  s32 tile = gResourceTableEntries[gData_02003fc6].offset >> 5;
  struct ObjectRuntime *leader = Object_GetById(Func_0200209c());
  s32 screen[3];

  if ((gInput & 3) != 0 && GameFlag_Test(0x106) == 0 && leader->y > 0x40000) {
    if (--gData_02003fc8 <= 0) {
      Func_0200200c(1);
      gData_02003fc8 = 10;
    }
  }
  Func_02001be0(Func_02002014());
  Render_ProjectPoint(&leader->x, screen);
  *oam++ = 0;
  *oam++ = 0x80004000 | ((screen[0] - 16) << 16) | (screen[1] - 32);
  *oam = tile | 0x400;
  Func_02001f7c(gData_02003fcc, 255);
}
