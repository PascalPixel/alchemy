// Draft, differ 41 of 124: rewritten from the listing, with the loop as a goto
// loop (as a for loop: 73). The aerie's setup: marks the leader and objects 10
// to 16, and under flag 0xa3c places objects 8 and 9, starts their actions and
// schedules Func_020004a4. Func_0200567c goes through a forwarding inline
// (called directly the two calls share 0x2000000). Left: GCSE gives the closing
// store's zero a register set inside the loop (the native sets it in object 8's
// block), which reorders the callee-saved constants: 0x700000, 4, 0x20 and 0
// take r8, r5, r6 and r10 where the native has r5, r6, r8 and r10. The test of
// flag 0xa3c as an enclosing if changes nothing.
#include "types.h"
#include "actor.h"
#include "adventure.h"

extern u32 Data_0200eb60;
extern const u8 gData_02005aa4[];
extern const u8 gData_02005ab4[];
extern const u8 gData_02005ae0[];
extern const u8 gData_02005b0c[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void WaitFrames(s32 frames);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Task_Add(void (*callback)(void), s32 priority);
void Func_02004ef0(const u8 *a, const u8 *b, const u8 *c, const u8 *d);
void Func_0200567c(s32 id, s32 a, s32 b);
void Func_020004a4(void);

static __inline__ void PlaceObject(s32 id, s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded inline; see scene_call.h. */
  Func_0200567c(id, a, b);
}

void Func_020001d8(void) {
  struct Actor *object;
  s32 i;

  Func_02004ef0(gData_02005aa4, gData_02005ab4, gData_02005ae0,
                gData_02005b0c);
  Actor_GetById(gAdventure.currentOwner)->priorityFlags |= 0x40;
  i = 10;
next:
  object = Actor_GetById(i);
  object->priorityFlags |= 0x80;
  Animation_ApplyChildValues(object, 5);
  if (++i <= 16)
    goto next;
  if (!GameFlag_Test(0xa3c))
    return;
  PlaceObject(8, 0x2000000, 0x1080000);
  PlaceObject(9, 0x2000000, 0x10f0000);
  WaitFrames(1);
  Actor_SetSpritePriority(8, 1);
  object = Actor_GetById(8);
  object->motionFlags = 4;
  object->y = 0x700000;
  Animation_ApplyChildValues(object, 5);
  Actor_SetSingleChildFlags(object, 0);
  object->priorityFlags |= 0x20;
  Actor_SetSpritePriority(9, 2);
  object = Actor_GetById(9);
  object->motionFlags = 4;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  object->y = 0x700000;
  Animation_ApplyChildValues(object, 5);
  Actor_SetSingleChildFlags(object, 0);
  object->priorityFlags |= 0x20;
  object->priorityFlags |= 2;
  Animation_SetStateFlags(object, 1);
  Data_0200eb60 = 0;
  Task_Add(Func_020004a4, 0x480);
}
