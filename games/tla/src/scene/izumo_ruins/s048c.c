#include "../trial_road/trial_road.h"
#include "call.h"

struct TrapEntry {
  u16 actor;
  u16 flag;
};

void wait(s32 frames);
void Actor_SetPositionAndResetMotion(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02001428();
void Func_02001488(void);
void Func_02001490(void);
void Func_02001470(s32 a, s32 b, s32 c);
void Func_02000974(s32 value);
void Func_02000aa8(void);
void Func_02000bc0(s32 a, s32 b, s32 c);

static __inline__ void Func_02005838_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02001470(a, b, c);
}

void Func_0200048c(const void *data, s32 id) {
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
      Func_02001488();
      if (!GameFlag_Test(flag + 1)) {
        Func_02001538(185);
        Func_02005838_(0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        wait(8);
        Call2(Func_02001428, (s32)object, 2);
        Func_02000bc0(-4, OBJECT_BYTE(object, 34), id);
        Func_02000974(1);
        wait(16);
        Func_02005838_(-1, -1, 0xe666);
        wait(8);
        Actor_SetPositionAndResetMotion(object, 0, 0, 0);
        wait(2);
        Func_02001538(188);
        Func_02000aa8();
        wait(20);
        GameFlag_SetBit(flag);
        GameFlag_SetBit(flag + 1);
      }
      Func_02001490();
      break;
    }
  }
}
