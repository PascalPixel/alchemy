#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02003df8(s32 cue);
void Func_02003be0(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);

struct EffectWork7c4 {
  u8 unk000[0x7c4];
  s16 active;
  s16 timer;
};

void Func_02002b68(void) {
  if (((struct EffectWork7c4 *)Ram_HeapSlots->effectWork)->active != 0) {
    Func_02003df8(229);
    Func_02003be0(64, 64, 15, 8, 1, 1);
    Func_02003be0(64, 66, 15, 72, 1, 2);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                       0);
  }
}
