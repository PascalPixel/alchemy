// Draft, differ 65 of 191: Func_020002f8 with person 8 taking part: it steps
// into the pillar and back while the pillar flashes, or before the pillar
// sinks. The same function is overlay 68F's Func_02002764 and 691's
// Func_02003240.
// The motion flags go through one pointer and the pillar test takes the
// native's branch order, as in Func_020002f8's draft. Left: allocation (the
// native has the leader in r5, person 8 in r7, the pillar in r10 and the id
// in r8; pins of those alone make it worse, 69 to 91) and the store-then-load
// row before the search's 0 argument.
#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "call.h"

struct Actor *Actor_GetById(s32 id);
s32 Func_020014d8(void);
struct Actor *Func_02001530(s32 *position, s32 flags);
void Func_02001488(void);
s32 Func_02001490(void);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02001538(s32 cue);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);
void WaitFrames(s32 frames);
void Func_02001428(struct Actor *object, s32 mode);
void Func_020006ec(struct Actor *object, struct Actor *target);
void Func_02000974(s32 mode);
void Func_02001470(s32 x, s32 y, s32 z);
void Func_02000c4c(s32 level, s32 layer);
void Func_02000aa8(void);

void Func_02000708(u16 *list) {
  struct Actor *leader;
  struct Actor *helper;
  struct Actor *pillar;
  s32 position[3];

  if (Func_020014d8() != 0)
    return;
  leader = Actor_GetById(gAdventure.currentOwner);
  helper = Actor_GetById(8);
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

    if (Actor_GetById(id) != pillar) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Func_02001488();
      if (!GameFlag_Test(flag)) {
        Func_02001428(helper, 1);
        Func_020006ec(helper, pillar);
        WaitFrames(1);
        Func_02001538(125);
        Actor_SetPartAttribute(Actor_GetById(8), 7);
        WaitFrames(2);
        Func_02001428(helper, 0);
        ObjectVisual_CopyAttributes(id, 9);
        Actor_SetPartAttribute(Actor_GetById(8), 0);
        WaitFrames(2);
        Actor_SetPartAttribute(Actor_GetById(8), 7);
        WaitFrames(4);
        Actor_SetPartAttribute(Actor_GetById(8), 0);
        Func_02000974(0);
        Func_020006ec(pillar, helper);
        WaitFrames(1);
        GameFlag_SetBit(flag);
      } else if (!GameFlag_Test(flag + 1)) {
        u8 *flags;

        Func_02001428(helper, 0);
        Func_020006ec(helper, pillar);
        WaitFrames(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Func_02001538(185);
        Call3((void (*)())Func_02001470, 0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        WaitFrames(8);
        Func_02001428(helper, 2);
        Func_02000c4c(-2, pillar->terrainId);
        Func_02000974(1);
        WaitFrames(16);
        Func_02000c4c(-2, pillar->terrainId);
        WaitFrames(4);
        Call3((void (*)())Func_02001470, -1, -1, 0xe666);
        WaitFrames(8);
        *flags = 3;
        WaitFrames(5);
        Actor_SetPositionAndResetMotion(helper, 0, 0, 0);
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
