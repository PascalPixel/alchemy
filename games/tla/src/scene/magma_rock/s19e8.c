#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"
#include "magma_rock.h"

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void Func_020014bc(s32 mode, s32 cue);

// In rooms 1 and 2, targets actor 15 or 14, runs Func_020014bc and sets flag
// 0x200.
void Func_020019e8(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  struct PartyState *party = &gPartyState;

  /* FAKEMATCH(one-pass): an empty one-pass block, so the party state's address
   * loads before the scene offset is built; without it, 4 of 31 instructions
   * differ in tla-en. */
  do {
  } while (0);
  {
    s32 scene = party->scene;
    /* FAKEMATCH(pin): the mode copied into r6 after the scene is read, as
     * native; as mode passed on directly, 2 of 31 instructions differ in
     * tla-en. */
    register s32 savedMode asm("r6") = mode;

    if (scene == (s32)&SceneId_Ov691Room1)
      work->target = Object_GetById(15);
    else if (scene == (s32)&SceneId_Ov691Room2)
      work->target = Object_GetById(14);
    Func_020014bc(savedMode, cue);
  }
  GameFlag_SetBit(0x200);
}
