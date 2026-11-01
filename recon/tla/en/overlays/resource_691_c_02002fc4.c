// Matches: differ 0 (tla-en, 2026-11-01)
// The search is a goto loop entered by a test of its own: as for or while, loop.c
// hoists 0xffff into ip (differ 12). Call3 keeps the constants of each
// Func_02003ac4 call apart; it is the one device, tagged in the function.
#include "types.h"
#include "actor.h"
#include "call.h"

struct Func_02002fc4_Entry {
  u16 id;
  u16 flag;
};

struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Actor_SetPositionAndResetMotion(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02003aec(void);
void Func_02003af4(void);
void Func_02003c54(s32 cue);
void Func_02003a54(struct Actor *object, s32 a);
void Func_020034ac(s32 a);
void Func_02003ac4(s32 a, s32 b, s32 c);
void Func_020036f8(s32 a, s32 b, s32 c);
void Func_020035e0(void);

void Func_02002fc4(struct Func_02002fc4_Entry *entry, s32 id) {
  struct Actor *target;
  s32 flag;
  s32 nextFlag;

  /* FAKEMATCH(wrapper): Call3 kept; as direct Func_02003ac4 calls, 20 of 91 instructions differ in tla-en. */
  target = Actor_GetById(id);
  if (entry->id == 0xffff)
    return;
body:
  if (entry->id != id) {
    entry++;
    goto test;
  }
  flag = entry->flag;
  Func_02003aec();
  nextFlag = flag + 1;
  if (GameFlag_Test(nextFlag) == 0) {
    Func_02003c54(185);
    Call3((void (*)())Func_02003ac4, 0x10000, 0x40000, 0x10000);
    Func_020034ac(0);
    wait(8);
    Func_02003a54(target, 2);
    Func_020036f8(-4, target->terrainId, id);
    Func_020034ac(1);
    wait(16);
    Call3((void (*)())Func_02003ac4, -1, -1, 0xe666);
    wait(8);
    Actor_SetPositionAndResetMotion(target, 0, 0, 0);
    wait(2);
    Func_02003c54(188);
    Func_020035e0();
    wait(20);
    GameFlag_SetBit(flag);
    GameFlag_SetBit(nextFlag);
  }
  Func_02003af4();
  return;
test:
  if (entry->id != 0xffff)
    goto body;
}
