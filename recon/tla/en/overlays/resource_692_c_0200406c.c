// Draft, differ 68 of 158: the trap table's step check (shared with overlay 697 and
// the other trap maps): when the leader stands on a trap, a trap whose first
// flag is clear flashes, takes mode 0 and sets the flag; one whose first flag is
// set and second clear drops the leader through with the camera shaken, then
// sets the second flag.
#include "types.h"
#include "object_runtime.h"
#include "adventure.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_02004e38(void);
struct ObjectRuntime *Func_02004ef8(s32 *position, s32 mode);
void Func_02004d40(void);
void Func_02004d48(void);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02004f30(s32 sound);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void WaitFrames(s32 frames);
void Func_02004c98(struct ObjectRuntime *object, s32 mode);
void Func_020046e8(s32 value);
void Func_02004d10(s32 a, s32 b, s32 c);
void Func_020049c0(s32 offset, s32 layer);
void Func_0200481c(void);

static __inline__ void Shake(s32 a, s32 b, s32 c) { Func_02004d10(a, b, c); }

void Func_0200406c(const u16 *table) {
  struct ObjectRuntime *leader;
  struct ObjectRuntime *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;

  if (Func_02004e38() != 0)
    return;
  leader = Object_GetById(gAdventure.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Func_02004ef8(position, 0);
  if (trap == 0)
    return;
  goto test;
next:
  id = table[0];
  if (Object_GetById(id) != trap) {
    table += 2;
    goto test;
  }
  {
    flag = table[1];
    Func_02004d40();
    if (!GameFlag_Test(flag)) {
      Func_02004f30(125);
      Object_SetPartAttribute(Object_GetById(id), 7);
      WaitFrames(2);
      Func_02004c98(trap, 0);
      Object_SetPartAttribute(Object_GetById(id), 0);
      WaitFrames(2);
      Object_SetPartAttribute(Object_GetById(id), 7);
      WaitFrames(4);
      Object_SetPartAttribute(Object_GetById(id), 0);
      Func_020046e8(0);
      GameFlag_SetBit(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Test(second)) {
        leader->motionFlags = 0;
        Func_02004f30(185);
        Shake(0x10000, 0x40000, 0x10000);
        Func_020046e8(0);
        WaitFrames(8);
        Func_02004c98(trap, 2);
        Func_020049c0(-2, trap->terrainId);
        Func_020046e8(1);
        WaitFrames(16);
        Func_020049c0(-2, trap->terrainId);
        WaitFrames(4);
        Shake(-1, -1, 0xe666);
        WaitFrames(8);
        leader->motionFlags = 3;
        WaitFrames(5);
        Object_SetPositionAndResetMotion(trap, 0, 0, 0);
        WaitFrames(2);
        Func_02004f30(188);
        Func_0200481c();
        WaitFrames(20);
        GameFlag_SetBit(second);
      }
    }
    Func_02004d48();
    return;
  }
test:
  if (*table != 0xffff)
    goto next;
}
