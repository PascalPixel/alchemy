// Draft, differ 65 of 158, skeleton gap 13 (8 removed, 13 added; tla-en, 2026-11-01)
// Calls, branches, loop and fields are the native's; every remaining line is
// one allocation. The native has object r6, id r7, target r8; here id r6,
// target r7, object r8, and the listing's moves and the terrainId add follow.
// Ranks from the lreg dump (floor_log2(refs) * refs / live): row .91, id 24/41,
// target 24/75, object 10/34. Object prefers r6 because the motionFlags address
// is local-allocated there, but that only holds in find_reg's pass 0: r7 has
// not been used yet, so pass 1 gives id r6. The native needs object ranked
// above id, or id in conflict with r6. No plain source found does either:
// object would need 8 refs or id 7 refs and object 6, and the refs are fixed by
// the listing's calls. local-alloc never takes r7, so the terrainId address
// cannot mark r7 used first.
// Left out on purpose: one local for id and the terrainId address (a reused
// local, a device) gives id r7 and differ 52, but target then evicts the
// local-allocated address from r6 and object stays in r8, so it does not close.
// Call3 is call.h's forwarding, as adopted scenes write this same veneer
// (ankohl_ruins/s104c.c); a direct call shares the constants, differ 77.
// Tried, no gain: u16 id (76); a copy of id in the arm (identical); layer set
// after wait(8) or after Func_02002f80, with or without -2 in a local (66, 67);
// a local for the motionFlags address or -2 (identical); no local for the
// terrainId address (64, but skeleton 9 removed); a {id, flag} struct (identical);
// break for return, goto end, the whole body nested in the first test
// (identical); a real while loop (103).
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 Func_020030f8(void);
struct Actor *Func_02003188(s32 *position, s32 mode);
void Func_02003048(void);
void Func_02003050(void);
void Func_020031a8(s32 sound);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void wait(s32 frames);
void Func_02002f80(struct Actor *object, s32 mode);
void Func_020029d0(s32 mode);
void Func_02003000(s32 a, s32 b, s32 c);
void Func_02002ca8(s32 a, s32 layer);
void Func_02002b04(void);

void Func_02002354(u16 *row) {
  struct Actor *object;
  s32 position[3];
  struct Actor *target;
  s32 id;
  s32 flag;
  s32 nextFlag;
  u8 *layer;

  if (Func_020030f8() != 0)
    return;
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  target = Func_02003188(position, 0);
  if (target != NULL) {
  goto test;
  do {
    id = row[0];
    if (Actor_GetById(id) != target) {
      row += 2;
      continue;
    }
    flag = row[1];
    Func_02003048();
    if (!GameFlag_Test(flag)) {
      Func_020031a8(125);
      Actor_SetPartAttribute(Actor_GetById(id), 7);
      wait(2);
      Func_02002f80(target, 0);
      Actor_SetPartAttribute(Actor_GetById(id), 0);
      wait(2);
      Actor_SetPartAttribute(Actor_GetById(id), 7);
      wait(4);
      Actor_SetPartAttribute(Actor_GetById(id), 0);
      Func_020029d0(0);
      GameFlag_SetBit(flag);
    } else {
      nextFlag = flag + 1;
      if (!GameFlag_Test(nextFlag)) {
        object->motionFlags = 0;
        Func_020031a8(185);
        Call3((void (*)())Func_02003000, 0x10000, 0x40000, 0x10000);
        Func_020029d0(0);
        layer = &target->terrainId;
        wait(8);
        Func_02002f80(target, 2);
        Func_02002ca8(-2, *layer);
        Func_020029d0(1);
        wait(16);
        Func_02002ca8(-2, *layer);
        wait(4);
        Call3((void (*)())Func_02003000, -1, -1, 0xe666);
        wait(8);
        object->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(target, 0, 0, 0);
        wait(2);
        Func_020031a8(188);
        Func_02002b04();
        wait(20);
        GameFlag_SetBit(nextFlag);
      }
    }
    Func_02003050();
    return;
  test:;
  } while (*row != 0xffff);
  }
}
