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
void WaitFrames(s32 frames);
void Func_02001428(struct Actor *object, s32 mode);
void Func_02000974(s32 mode);
void Func_02001470(s32 x, s32 y, s32 z);
void Func_02000c4c(s32 level, s32 layer);
void Func_02000aa8(void);

void Func_020002f8(u16 *list) {
  /* FAKEMATCH(pin): the leader held in r6; as a plain local, 64 of 158
   * instructions differ in tla-en. */
  register struct Actor *leader asm("r6");
  struct Actor *pillar;
  s32 position[3];

  if (Func_020014d8() != 0)
    return;
  leader = Actor_GetById(gAdventure.currentOwner);
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
        Func_02001538(125);
        Actor_SetPartAttribute(Actor_GetById(id), 7);
        WaitFrames(2);
        Func_02001428(pillar, 0);
        Actor_SetPartAttribute(Actor_GetById(id), 0);
        WaitFrames(2);
        Actor_SetPartAttribute(Actor_GetById(id), 7);
        WaitFrames(4);
        Actor_SetPartAttribute(Actor_GetById(id), 0);
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
        Actor_SetPositionAndResetMotion(pillar, 0, 0, 0);
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
