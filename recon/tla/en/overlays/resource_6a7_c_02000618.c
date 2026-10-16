// Near miss: Func_02000618 differs 14. With the first store and the party state
// address in a one-pass block (alchemy permute's), the address is loaded one
// row early instead of late; the owner override store swaps r2 and r3 (offset
// and value in r3, address in r2). Tried for the store: the field through the
// state pointer, a field pointer, a value temporary, a value pinned to r2
// (differ 14 to 25); 3 minutes of alchemy permute.
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "flag_call.h"

struct SceneWork {
  u8 unk000[0x1ac];
  s32 unk1ac;
};

extern char SceneId_Ov6a7Room1;

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02000064(void);
void Func_02000af8(s32 scene, s32 entrance);
void Func_02000b08(void);

s32 Func_02000618(void) {
  struct PartyState *state;

  do {
    ((struct SceneWork *)Ram_HeapSlots->eventWork)->unk1ac = 256;
    state = &gPartyState;
  } while (0);
  Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
  if (state->entrance == 90) {
    GameFlag_SetBit(0x90a);
    gPartyState.ownerOverride = 1;
    Func_02000af8((s32)&SceneId_Ov6a7Room1, 4);
  }
  Func_02000b08();
  return 0;
}
