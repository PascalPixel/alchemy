#include "types.h"
#include "actor.h"
#include "call.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000bc0(s32 a, s32 b, s32 owner);
void Func_02003ba0(struct Actor *object, s32 value);
void Func_02003c00(s32 a, s32 b, s32 c);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003df8(s32 cue);

/// Runs the event of the listed object `id` once: the list holds (object id,
/// flag) pairs ended by 0xffff, and unless the flag after the entry's own is
/// set, cues 185 and 188 play around Func_02000bc0 and both flags are set. The
/// twin of overlay 67f's Func_0200048c.
void Func_0200048c(u16 *list, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 flag;

  if (list[0] == 0xffff)
    return;
loop:
  if (list[0] != id) {
    list += 2;
    goto test;
  }
  flag = list[1];
  Func_02003c28();
  if (!GameFlag_Test(flag + 1)) {
    Func_02003df8(185);
    Call3((void (*)())Func_02003c00, 0x10000, 0x40000, 0x10000);
    Func_02000974(0);
    wait(8);
    Func_02003ba0(object, 2);
    Func_02000bc0(-4, object->terrainId, id);
    Func_02000974(1);
    wait(16);
    Call3((void (*)())Func_02003c00, -1, -1, 0xe666);
    wait(8);
    Actor_SetPositionAndResetMotion(object, 0, 0, 0);
    wait(2);
    Func_02003df8(188);
    Func_02000aa8();
    wait(20);
    GameFlag_SetBit(flag);
    GameFlag_SetBit(flag + 1);
  }
  Func_02003c30();
  return;
test:
  if (list[0] != 0xffff)
    goto loop;
}
