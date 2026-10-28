// Draft, differ 77 of 191: the trap table's step check for maps with object 8
// (shared with overlay 697 and the other trap maps), Func_0200406c with object
// 8 moved onto the trap and back (Func_02004460) around each effect: a trap
// whose first flag is clear flashes object 8, copies object 9's look and sets
// the flag; one whose second flag is clear drops the leader through with the
// camera shaken and sets that flag.
#include "types.h"
#include "actor.h"
#include "adventure.h"

struct Actor *Actor_GetById(s32 id);
s32 Func_02004e38(void);
struct Actor *Func_02004ef8(s32 *position, s32 mode);
void Func_02004d40(void);
void Func_02004d48(void);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02004f30(s32 sound);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void wait(s32 frames);
void Func_02004c98(struct Actor *object, s32 mode);
void Func_02004460(struct Actor *object, struct Actor *to);
void ObjectVisual_CopyAttributes(s32 id, s32 from);
void Func_020046e8(s32 value);
void Func_02004d10(s32 a, s32 b, s32 c);
void Func_020049c0(s32 offset, s32 layer);
void Func_0200481c(void);

static __inline__ void Shake(s32 a, s32 b, s32 c) { Func_02004d10(a, b, c); }

void Func_0200447c(const u16 *table) {
  struct Actor *leader;
  struct Actor *partner;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;

  if (Func_02004e38() != 0)
    return;
  leader = Actor_GetById(gAdventure.currentOwner);
  partner = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Func_02004ef8(position, 0);
  if (trap == 0)
    return;
  goto test;
next:
  id = table[0];
  if (Actor_GetById(id) != trap) {
    table += 2;
    goto test;
  }
  {
    flag = table[1];
    Func_02004d40();
    if (!GameFlag_Test(flag)) {
      Func_02004c98(partner, 1);
      Func_02004460(partner, trap);
      wait(1);
      Func_02004f30(125);
      Actor_SetPartAttribute(Actor_GetById(8), 7);
      wait(2);
      Func_02004c98(partner, 0);
      ObjectVisual_CopyAttributes(id, 9);
      Actor_SetPartAttribute(Actor_GetById(8), 0);
      wait(2);
      Actor_SetPartAttribute(Actor_GetById(8), 7);
      wait(4);
      Actor_SetPartAttribute(Actor_GetById(8), 0);
      Func_020046e8(0);
      Func_02004460(trap, partner);
      wait(1);
      GameFlag_SetBit(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Test(second)) {
        Func_02004c98(partner, 0);
        Func_02004460(partner, trap);
        wait(1);
        leader->motionFlags = 0;
        Func_02004f30(185);
        Shake(0x10000, 0x40000, 0x10000);
        Func_020046e8(0);
        wait(8);
        Func_02004c98(partner, 2);
        Func_020049c0(-2, trap->terrainId);
        Func_020046e8(1);
        wait(16);
        Func_020049c0(-2, trap->terrainId);
        wait(4);
        Shake(-1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(partner, 0, 0, 0);
        wait(2);
        Func_02004f30(188);
        Func_0200481c();
        wait(20);
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
