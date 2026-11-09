#include "types.h"
#include "call.h"
#include "actor.h"
#include "vram_tab.h"
#include "event_inline.h"

extern u32 gInput;
extern s16 gData_02006e0e;
extern s16 gData_02006e10;
extern u32 Data_02006e14[3];

void Func_02004c08(s32 value);
s32 Func_02004c10(void);
void Func_02004744(s32 progress);
void Render_ProjectPoint(s32 *position, s32 *screen);
void Func_02004ad0(u32 *oam, s32 priority);

// The world map cursor: counts down while A or B is held, redraws the gauge and
// queues its sprite over the leader.
void Func_020048f4(void) {
  u32 *oam = Data_02006e14;
  s32 tile = gResourceTableEntries[gData_02006e0e].offset >> 5;
  struct Actor *leader = Actor_Get(FieldParty_GetLeaderActor());
  s32 screen[3];
  u32 attributes;

  if ((gInput & 3) != 0 && Event_GetFlag(0x106) == 0 && leader->y > 0x40000) {
    if (--gData_02006e10 <= 0) {
      Func_02004c08(1);
      gData_02006e10 = 10;
    }
  }
  Func_02004744(Func_02004c10());
  attributes = 0x80004000;
  Call2((void (*)())Render_ProjectPoint, (s32)(&leader->x), (s32)(screen));
  {
    s32 x = screen[0] - 16;
    s32 y = screen[1] - 32;

    *oam++ = 0;
    *oam++ = (y | (x << 16)) | attributes;
  }
  *oam = tile | 0x400;
  Func_02004ad0(Data_02006e14, 255);
}
