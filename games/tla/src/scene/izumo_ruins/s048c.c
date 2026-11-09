#include "../trial_road/trial_road.h"
#include "event_inline.h"

struct TrapEntry {
  u16 actor;
  u16 flag;
};

void Func_02001428();
void Func_02001488(void);
void Func_02001490(void);
void Func_02000974(s32 value);
void Func_02000aa8(void);
void Func_02000bc0(s32 a, s32 b, s32 c);

void Func_0200048c(const void *data, s32 id) {
  const struct TrapEntry *entry = data;
  struct Actor *object = Actor_Get(id);
  s32 flag;

  while (entry->actor != 0xffff) {
    if (entry->actor != id) {
      entry++;
      continue;
    }
    {
      flag = entry->flag;
      Func_02001488();
      if (!Event_GetFlag(flag + 1)) {
        Event_PlaySound(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        System_Wait(8);
        Func_02001428(object, 2);
        Func_02000bc0(-4, OBJECT_BYTE(object, 34), id);
        Func_02000974(1);
        System_Wait(16);
        MapGrid_SetShake(-1, -1, 0xe666);
        System_Wait(8);
        FieldActor_SetPositionAndResetMotion(object, 0, 0, 0);
        System_Wait(2);
        Event_PlaySound(188);
        Func_02000aa8();
        System_Wait(20);
        Event_SetFlag(flag);
        Event_SetFlag(flag + 1);
      }
      Func_02001490();
      break;
    }
  }
}
