#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "task.h"

extern const u8 gData_02003fb0[];
extern struct Actor *gData_02004420;
extern s32 gData_02005678;
extern s32 gData_0200567c;

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Func_02000038(const void *data);
void Func_02001804(void);
void Func_020019b8(void);
void Func_02001ee8(void);
void Func_02002c14(void);
void Func_020035ec(void);
void Func_02003764(void);
void Func_02003a30(void);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 Flag_Test(s32 flag) {
  return GameFlag_Test(flag);
}

static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

static __inline__ void Actor_SetPosition(struct Actor *object, s32 x,
                                         s32 y, s32 z) {
  Actor_SetPositionAndResetMotion(object, x, y, z);
}

void Func_0200390c(void) {
  struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 entrance;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  owner->priorityFlags |= 32;
  if (GameFlag_Test(0x878) == 0) {
    Func_02003a30();
    if (GameFlag_Test(0x871) != 0) {
      Map_CopyAttributes(0, 0, 1, 1, 15, 5);
      if (gData_02004420 != NULL) {
        Actor_SetPosition(gData_02004420, 248 << 16, 0, 176 << 15);
        wait(1);
      }
    }
  }
  Func_02000038(gData_02003fb0);
  Func_020019b8();
  entrance = gMainRoutineWork.entrance;
  if (entrance > 10) {
    Func_02001804();
    GameFlag_SetBit(entrance + 789);
  }
  if (Flag_Test(0x871) != 0)
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Func_020035ec();
  if (GameFlag_Test(265) == 0) {
    if (gMainRoutineWork.entrance > 16) {
      if (gMainRoutineWork.entrance == 37 && GameFlag_Test(0x871) == 0)
        Func_02002c14();
      else
        Func_02003764();
    }
  }
  gData_0200567c = 0;
  gData_02005678 = 0;
  Task_Add(Func_02001ee8, TASK_PASS_VBLANK);
}
