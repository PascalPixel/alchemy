#include "types.h"
#include "actor.h"
#include "event_inline.h"

struct Func_02002fc4_Entry {
  u16 id;
  u16 flag;
};

void Func_02003aec(void);
void Func_02003af4(void);
void Func_02003a54(struct Actor *object, s32 a);
void Func_020034ac(s32 a);
void Func_020036f8(s32 a, s32 b, s32 c);
void Func_020035e0(void);

void Func_02002fc4(struct Func_02002fc4_Entry *entry, s32 id) {
  struct Actor *target;
  s32 flag;
  s32 nextFlag;

  /* FAKEMATCH(wrapper): Call3 kept; as direct Map_SetScreenShake calls, 20 of
   * 91 instructions differ in tla-en. */
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
    Sound_Play(185);
    MapGrid_SetShake(0x10000, 0x40000, 0x10000);
    Func_020034ac(0);
    wait(8);
    Func_02003a54(target, 2);
    Func_020036f8(-4, target->terrainId, id);
    Func_020034ac(1);
    wait(16);
    MapGrid_SetShake(-1, -1, 0xe666);
    wait(8);
    Actor_SetPositionAndResetMotion(target, 0, 0, 0);
    wait(2);
    Sound_Play(188);
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
