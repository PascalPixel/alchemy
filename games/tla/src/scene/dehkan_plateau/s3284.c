#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"
#include "task.h"

extern const u8 gData_02003f80[];
extern const u8 gData_02003fc4[];
extern const u8 gData_02003fc8[];
extern s32 gData_02005678;
extern s32 gData_0200567c;

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Func_020001d0(const void *data);
void Func_02000c64(const void *data);
void Func_020013b0(void);
void Func_0200172c(void);
void Func_02001930(void);
void Func_02001ee8(void);
void Func_02001fc4(void);
void Func_02003be0(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_02003bf0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003c70(s32 id, s32 x, s32 z, s32 angle);
void Func_02003cc0(s32 id);
void Func_02003d90(void);
s32 Func_02003d98(void);
s32 Func_02003da0(s32 a, s32 b, s32 c);
void Func_02003da8(s32 a, s32 b, s32 c, s32 d);
s32 Func_02003db0(void);
void Func_02003db8(const void *data);

// The room entry written in the scene layer of Func_02003164 and Func_0200390c:
// sets actors' variants, links, cells and flags for the room.
void Func_02003284(void) {
  struct Actor *object;
  s32 zero, value;

  if (GameFlag_Test(265) == 0) {
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  }
  Actor_SetSpritePriority(8, 1);
  Actor_SetSpritePriority(9, 1);
  Actor_SetSpritePriority(10, 1);
  Actor_SetSpritePriority(11, 1);
  /* FAKEMATCH(one-pass): an empty block ends the argument loads' tie; without it, 2
   * of 276 instructions differ in tla-en. */
  do {
  } while (0);
  Func_02003d98();
  Func_02003da0(0, 8, 9);
  Func_02003da0(1, 10, 11);
  Actor_SetSpritePriority(12, 1);
  Actor_SetSpritePriority(13, 1);
  Func_02003d90();
  Func_02003da8(0, 520, 12, 13);
  Func_02003db8(gData_02003fc4);
  if (Actor_GetById(19)->y > (s32)0xfff80000)
    Actor_SetSpritePriority(19, 1);
  else
    Actor_SetSpritePriority(19, 2);
  Func_02003cc0(20);
  Actor_GetById(20)->priorityFlags |= 32;
  Actor_SetSpritePriority(20, 2);
  if (GameFlag_Test(0x841) != 0)
    Func_0200172c();
  else
    /* FAKEMATCH(wrapper): forwarded, so each call builds its own constants; as direct
     * calls, 8 of 276 instructions differ in tla-en. */
    Call6((void (*)())Func_02003bf0, 38, 32, 1, 1, 38, 33);
  Actor_GetById(14)->priorityFlags &= ~1;
  Actor_GetById(15)->priorityFlags &= ~1;
  Actor_GetById(16)->priorityFlags &= ~1;
  Actor_GetById(17)->priorityFlags &= ~1;
  Func_02000c64(gData_02003fc8);
  value = Actor_GetById(17)->x;
  zero = 0;
  if ((value >> 20) == 54) {
    struct MapCell *cell = Ram_HeapSlots->mapWork->layers[0].cells;

    cell += Func_02003db0();
    cell->attributeB &= 0x7f;
    cell->collisionCode = 11;
  }
  Actor_SetSpritePriority(14, 1);
  Actor_SetSpritePriority(15, 1);
  Func_02001930();
  if (GameFlag_Test(0x86e) != 0) {
    s32 angle = 0x4000;

    /* FAKEMATCH(wrapper): forwarded, so the angle loads ahead of the coordinates; as
     * direct calls, 4 of 276 instructions differ in tla-en. */
    Call4((void (*)())Func_02003c70, 25, 212 << 17, 208 << 15, angle);
  }
  Actor_SetModeById(18, 3);
  if (GameFlag_Test(0x86f) != 0) {
    object = Actor_GetById(18);
    Call6((void (*)())Func_02003bf0, 27, 8, 1, 1, 27, 9);
    Actor_SetModeById(18, 4);
    object->priorityFlags |= 2;
    *((u8 *)object + 89) &= ~1;
    GameFlag_SetBit(517);
    GameFlag_SetBit(577);
    Func_02003be0(69, 51, 92, 11, 1, 1);
    {
      s32 angle = 0x4000;

      Call4((void (*)())Func_02003c70, 25, 154 << 18, 140 << 17, angle);
    }
  }
  if (GameFlag_Test(0x870) != 0) {
    GameFlag_SetBit(528);
    GameFlag_SetBit(534);
    Func_02003c68(25, 0, 0);
  }
  Func_020001d0(gData_02003f80);
  Func_020013b0();
  Func_02001fc4();
  {
    struct Actor *last = Actor_GetById(27);

    *((u8 *)last + 85) = zero;
    last->y = zero;
  }
  gData_0200567c = zero;
  gData_02005678 = zero;
  Task_Add(Func_02001ee8, TASK_PASS_VBLANK);
}
