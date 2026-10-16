// differ 2: the native loads the object x before it copies the mask 64 for the
// first and.
#include "types.h"
#include "field_event.h"
#include "party_state.h"

struct MapTrigger {
  u8 unk00[6];
  s16 x;
  s16 z;
};

struct FieldActor *Object_GetById(s32 id);
void Battle_WaitMode0(s32 frames);
void Func_020021c0(void);
void Func_02002a40(s32 cue);
void Func_02002858(s32 x, s32 z, s32 width, s32 height, s32 destX, s32 destZ);

void Func_02002248(struct MapTrigger *trigger) {
  struct FieldActor *object = Object_GetById(gPartyState.currentOwner);
  s32 x;
  s32 z;

  x = object->x >> 20;
  x |= (s16)(trigger->x & 0x40);
  z = object->z >> 20;
  z |= (s16)(trigger->z & 0x40);

  Func_020021c0();
  Func_02002a40(161);
  Func_02002858(trigger->x, trigger->z + 1, 1, 1, x, z);
  Battle_WaitMode0(12);
}
