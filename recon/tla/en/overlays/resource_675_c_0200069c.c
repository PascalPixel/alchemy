// Draft, differ 70 of 123: Islet Cave's setup: in its first room it starts the
// palette lists (Func_02000038); in the third it frees actor 9 and, arriving
// by entrance 2, places actor 8 and sets flag 0x9cf; in the second it sets
// flag 0x230 when flag 85 or 165 is set, else (until 0x230) places actor 9
// and starts actor 8's action script. The second and third rooms then dim
// the palette by the entrance (Func_02000580) and wait for the transition.
// Returns 0. Left: the native's block order after the three rooms (its
// tests of the scene again before the dimming) and the register holding
// gSequenceParentWork.
#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"

extern char SceneId_Ov675Room1, SceneId_Ov675Room2, SceneId_Ov675Room3;
extern u16 gData_0200134c[], gData_0200135c[], gData_02001388[],
    gData_020013b4[];
extern u8 gData_02001454[];
void Func_0200131c(s32 mode);
void Func_020008c8(void);
void Func_02000038(u16 *slots, u16 *colours, u16 *a, u16 *b);
struct Actor *Actor_GetById(u32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
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

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
  Func_0200131c(0);
  scene = gSequenceParentWork.scene;
  if (scene == (s32)&SceneId_Ov675Room1) {
    Func_020008c8();
    Func_02000038(gData_0200134c, gData_0200135c, gData_02001388,
                  gData_020013b4);
  } else if (scene == (s32)&SceneId_Ov675Room3) {
    Actor_SetSingleChildFlags(Actor_GetById(9), 0);
    if (gSequenceParentWork.entrance == 2) {
      Func_0200129c(8, 0, 0);
      GameFlag_SetBit(0x9cf);
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
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov675Room2) {
    dim = (gSequenceParentWork.entrance - 3) / 2;
    goto fade;
  }
other:
  if (gSequenceParentWork.scene != (s32)&SceneId_Ov675Room3)
    return 0;
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov675Room2)
    dim = (gSequenceParentWork.entrance - 3) / 2;
  else
    dim = 3;
fade:
  Func_02000580(0x400 - (dim << 7));
  Func_0200133c();
  Func_020012fc(1);
  Event_WaitTransitionFrames();
  return 0;
}
