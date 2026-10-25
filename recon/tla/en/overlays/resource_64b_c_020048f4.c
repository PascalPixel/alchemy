// Draft, differ 49 of 75: the world map cursor: counts down while A or B is
// held, redraws the gauge and queues its sprite over the leader. Left: the
// sprite word order and register choice around the stmia stores.
#include "types.h"
#include "object_runtime.h"
#include "vram_tab.h"

extern u32 gInput;
extern s16 gData_02006e0e;
extern s16 gData_02006e10;
extern u32 Data_02006e14[3];

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
s32 Func_02004e10(void);
void Func_02004c08(s32 value);
s32 Func_02004c10(void);
void Func_02004744(s32 progress);
void Render_ProjectPoint(s32 *position, s32 *screen);
void Func_02004ad0(u32 *oam, s32 priority);

void Func_020048f4(void) {
  u32 *oam = Data_02006e14;
  s32 tile = gResourceTableEntries[gData_02006e0e].offset >> 5;
  struct ObjectRuntime *leader = Object_GetById(Func_02004e10());
  s32 screen[3];

  if ((gInput & 3) != 0 && GameFlag_Test(0x106) == 0 && leader->y > 0x40000) {
    if (--gData_02006e10 <= 0) {
      Func_02004c08(1);
      gData_02006e10 = 10;
    }
  }
  Func_02004744(Func_02004c10());
  Render_ProjectPoint(&leader->x, screen);
  *oam++ = 0;
  *oam++ = 0x80004000 | ((screen[0] - 16) << 16) | (screen[1] - 32);
  *oam = tile | 0x400;
  Func_02004ad0(Data_02006e14, 255);
}
