#include "types.h"
#include "call.h"
#include "actor.h"
#include "vram_tab.h"
#include "event_inline.h"

extern u32 gInput;
extern s16 gData_02003fc6;
extern s16 gData_02003fc8;
extern u32 Data_02003fcc[3];

void Func_0200200c(s32 value);
s32 Func_02002014(void);
void Func_02001be0(s32 progress);
void Render_ProjectPoint(s32 *position, s32 *screen);
void Func_02001f7c(u32 *oam, s32 priority);

// The same cursor as the world map's, in this overlay: counts down while A or B
// is held, redraws the gauge and queues its sprite over the leader.
void Func_02001d90(void) {
  u32 *oam = Data_02003fcc;
  s32 tile = gResourceTableEntries[gData_02003fc6].offset >> 5;
  struct Actor *leader = Actor_Get(FieldParty_GetLeaderActor());
  s32 screen[3];
  u32 attributes;

  if ((gInput & 3) != 0 && Event_GetFlag(0x106) == 0 && leader->y > 0x40000) {
    if (--gData_02003fc8 <= 0) {
      Func_0200200c(1);
      gData_02003fc8 = 10;
    }
  }
  Func_02001be0(Func_02002014());
  attributes = 0x80004000;
  Call2((void (*)())Render_ProjectPoint, (s32)(&leader->x), (s32)(screen));
  {
    s32 x = screen[0] - 16;
    s32 y = screen[1] - 32;

    *oam++ = 0;
    *oam++ = (y | (x << 16)) | attributes;
  }
  *oam = tile | 0x400;
  Func_02001f7c(Data_02003fcc, 255);
}
