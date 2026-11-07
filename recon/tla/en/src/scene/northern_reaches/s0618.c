#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "flag_call.h"

struct SceneWork {
  u8 unk000[0x1ac];
  s32 unk1ac;
};

extern char SceneId_Ov6a7Room1;

struct Actor *Actor_GetById(s32 id);
void Func_02000064(void);
void Func_02000af8(s32 scene, s32 entrance);
void Func_02000b08(void);

static __inline__ void SetFlag(s32 flag) {
  /* FAKEMATCH(wrapper): the flag set forwarded inline; as a direct call, 12 of 43
   * instructions differ in tla-en. */
  GameFlag_SetBit(flag);
}

s32 Func_02000618(void) {
  struct MainRoutineWork *state;

  ((struct SceneWork *)Ram_HeapSlots->eventWork)->unk1ac = 256;
  state = &gMainRoutineWork;
  Actor_GetById(state->currentOwner)->priorityFlags |= 32;
  if (state->entrance == 90) {
    SetFlag(0x90a);
    state->ownerOverride = 1;
    Func_02000af8((s32)&SceneId_Ov6a7Room1, 4);
  }
  Func_02000b08();
  return 0;
}
