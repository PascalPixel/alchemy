// Draft, differ 4 of 32: the native loads the party state address before it
// saves the second argument (adds r7, r1) and starts the scene offset after;
// this form saves the argument first.
#include "../../../../games/tla/src/scene/magma_rock/magma_rock.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "scene_ids.h"
#include "flag_call.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_020014bc(s32 a, s32 b);

void Func_02001a38(s32 a, s32 b) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  if (gPartyState.scene == (s32)&SceneId_Ov691Room1)
    work->target = Object_GetById(16);
  else if (gPartyState.scene == (s32)&SceneId_Ov691Room2)
    work->target = Object_GetById(10);
  Func_020014bc(a, b);
  Flag_Set(0x201);
}
