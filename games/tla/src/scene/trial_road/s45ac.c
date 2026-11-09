#include "../../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "event_inline.h"

struct TrapEntry {
  u16 actor;
  u16 flag;
};

void Func_02005880(void);
void Func_02005888(void);
void Func_02004a94(s32 value);
void Func_02004bc8(void);
void Func_02004ce0(s32 a, s32 b, s32 c);

void Func_020045ac(const void *data, s32 id) {
  const struct TrapEntry *entry = data;
  struct Actor *object = Actor_GetById(id);
  s32 flag;

  while (entry->actor != 0xffff) {
    if (entry->actor != id) {
      entry++;
      continue;
    }
    {
      flag = entry->flag;
      Func_02005880();
      if (!GameFlag_Test(flag + 1)) {
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02004a94(0);
        wait(8);
        Func_020057c8(object, 2);
        Func_02004ce0(-4, OBJECT_BYTE(object, 34), id);
        Func_02004a94(1);
        wait(16);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        Actor_SetPositionAndResetMotion(object, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_02004bc8();
        wait(20);
        GameFlag_SetBit(flag);
        GameFlag_SetBit(flag + 1);
      }
      Func_02005888();
      break;
    }
  }
}
