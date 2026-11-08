#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"

extern char SceneId_Ov675Room1, SceneId_Ov675Room2, SceneId_Ov675Room3;
extern u16 gData_0200134c[], gData_0200135c[], gData_02001388[],
    gData_020013b4[];
extern u8 gData_02001454[];
void Func_0200131c(s32 mode);
void Func_020008c8(void);
void Func_02000038(u16 *slots, u16 *colours, u16 *a, u16 *b);
struct Actor *Actor_GetById(u32 id);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_0200129c(s32 id, s32 x, s32 z);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Actor_SetMovementTypeById(s32 id, const u8 *script);
void Func_02000580(s32 scale);
void Func_0200133c(void);
void Func_020012fc(s32 mode);
void Event_WaitTransitionFrames(void);

s32 Func_0200069c(void) {
  s32 scene, dim;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = SPLIT_CONSTANT(0x10a, 255);
  Func_0200131c(0);
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov675Room1) {
    Func_020008c8();
    Func_02000038(gData_0200134c, gData_0200135c, gData_02001388,
                  gData_020013b4);
  } else if (scene == (s32)&SceneId_Ov675Room3) {
    Actor_SetSingleChildFlags(Actor_GetById(9), 0);
    if (gMainRoutineWork.entrance == 2) {
      Func_0200129c(8, 0, 0);
      GameFlag_SetBit(SPLIT_CONSTANT(0x8d0, 255));
    }
  } else if (scene == (s32)&SceneId_Ov675Room2) {
    if (GameFlag_Test(85) || GameFlag_Test(165)) {
      GameFlag_SetBit(0x230);
    } else if (!GameFlag_Test(0x230)) {
      Func_0200129c(9, 0, 0);
      Actor_SetMovementTypeById(8, gData_02001454);
    }
  } else {
    goto other;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov675Room2)
    goto fade;
other:
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov675Room3) {
  fade:
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov675Room2)
      dim = (gMainRoutineWork.entrance - 3) / 2;
    else
      dim = 3;
    Func_02000580(0x400 - (dim << 7));
    Func_0200133c();
    Func_020012fc(1);
    Event_WaitTransitionFrames();
  }
  return 0;
}
