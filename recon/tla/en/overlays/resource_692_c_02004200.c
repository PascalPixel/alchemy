// Draft, differ 16 of 91: overlay 692's copy of the function drafted in
// resource_697/0200c5ac.c (overlay 697's Func_020045ac), whose
// header says what it is, what still differs and what was tried.
#include "../../../../../games/tla/src/scene/trial_road/trial_road.h"

struct TrapEntry {
  u16 actor;
  u16 flag;
};

void WaitFrames(s32 frames);
void Actor_SetPositionAndResetMotion(struct Actor *object, s32 x,
                                      s32 y, s32 z);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004d10(s32 a, s32 b, s32 c);
void Func_020046e8(s32 value);
void Func_0200481c(void);
void Func_02004934(s32 a, s32 b, s32 c);

static __inline__ void Func_02005838_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02004d10(a, b, c);
}

void Func_02004200(const void *data, s32 id) {
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
      Func_02004d40();
      if (!GameFlag_Test(flag + 1)) {
        Func_02004f30(185);
        Func_02005838_(0x10000, 0x40000, 0x10000);
        Func_020046e8(0);
        WaitFrames(8);
        Func_02004c98(object, 2);
        Func_02004934(-4, OBJECT_BYTE(object, 34), id);
        Func_020046e8(1);
        WaitFrames(16);
        Func_02005838_(-1, -1, 0xe666);
        WaitFrames(8);
        Actor_SetPositionAndResetMotion(object, 0, 0, 0);
        WaitFrames(2);
        Func_02004f30(188);
        Func_0200481c();
        WaitFrames(20);
        GameFlag_SetBit(flag);
        GameFlag_SetBit(flag + 1);
      }
      Func_02004d48();
      break;
    }
  }
}
