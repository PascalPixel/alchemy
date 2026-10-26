// Draft, differ 2 of 158: the pillar the leader faces, when listed, either
// flashes once and sets its flag or, once that flag is set, sinks into the
// floor with a shake and sets the second. The same function is overlay 68F's
// Func_02002354 and 691's Func_02002e30.
// With the leader pinned to r6 (a pin device to tag on adoption; without it the
// leader takes r8 and 64 differ), the motion flags written through one pointer
// (the native reuses leader + 85 for both stores) and the pillar test in the
// native's branch order. Left: the known store-then-load row: the native sets
// the search's 0 argument between the first position store and the second
// load. Tried: empty asm and one-pass blocks around the copies, the 0 in an
// asm-held local (all 2 or worse).
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_020014d8(void);
struct ObjectRuntime *Func_02001530(s32 *position, s32 flags);
void Func_02001488(void);
s32 Func_02001490(void);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02001538(s32 cue);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
void WaitFrames(s32 frames);
void Func_02001428(struct ObjectRuntime *object, s32 mode);
void Func_02000974(s32 mode);
void Func_02001470(s32 x, s32 y, s32 z);
void Func_02000c4c(s32 level, s32 layer);
void Func_02000aa8(void);

void Func_020002f8(u16 *list) {
  /* FAKEMATCH(pin): the leader held in r6; as a plain local, 64 of 158
   * instructions differ in tla-en. */
  register struct ObjectRuntime *leader asm("r6");
  struct ObjectRuntime *pillar;
  s32 position[3];

  if (Func_020014d8() != 0)
    return;
  leader = Object_GetById(gPartyState.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  pillar = Func_02001530(position, 0);
  if (pillar != 0)
    goto test;
  return;
body:
  {
    u32 id = list[0];

    if (Object_GetById(id) != pillar) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Func_02001488();
      if (!GameFlag_Test(flag)) {
        Func_02001538(125);
        Object_SetPartAttribute(Object_GetById(id), 7);
        WaitFrames(2);
        Func_02001428(pillar, 0);
        Object_SetPartAttribute(Object_GetById(id), 0);
        WaitFrames(2);
        Object_SetPartAttribute(Object_GetById(id), 7);
        WaitFrames(4);
        Object_SetPartAttribute(Object_GetById(id), 0);
        Func_02000974(0);
        GameFlag_SetBit(flag);
      } else if (!GameFlag_Test(flag + 1)) {
        u8 *flags = &leader->motionFlags;

        *flags = 0;
        Func_02001538(185);
        Call3((void (*)())Func_02001470, 0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        WaitFrames(8);
        Func_02001428(pillar, 2);
        Func_02000c4c(-2, pillar->terrainId);
        Func_02000974(1);
        WaitFrames(16);
        Func_02000c4c(-2, pillar->terrainId);
        WaitFrames(4);
        Call3((void (*)())Func_02001470, -1, -1, 0xe666);
        WaitFrames(8);
        *flags = 3;
        WaitFrames(5);
        Object_SetPositionAndResetMotion(pillar, 0, 0, 0);
        WaitFrames(2);
        Func_02001538(188);
        Func_02000aa8();
        WaitFrames(20);
        GameFlag_SetBit(flag + 1);
      }
      Func_02001490();
      return;
    }
  }
test:
  if (list[0] != 0xffff)
    goto body;
}
