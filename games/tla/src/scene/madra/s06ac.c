#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "event_work.h"
#include "field_sprite.h"
#include "flag_call.h"
#include "call.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_0200068c(void);
void Func_02000f80(void);
void Func_020019d8(void);
void Func_02002b88(s32 id, s32 x, s32 z);

/// Madra's prepare step: sets up actor 26 (larger, its sprite flags set)
/// and places actors 26 to 28, 33 and 66 by the story flags, then runs
/// Func_02000f80 on entrance 9 and Func_020019d8 once on entrance 7.
s32 Func_020006ac(void) {
  struct ObjectRuntime *object;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  if (Flag_IsSet(0x8ff)) {
    Func_0200068c();
  } else {
    ObjectDispatch_SetSingleChildFlags(Object_GetById(26), 0);
    object = Object_GetById(26);
    object->radius = Object_GetById(26)->radius * 4 / 3;
    ((struct FieldSprite *)Object_GetById(26)->animation)->nextFrame |= 8;
    {
      struct FieldSprite *sprite = Object_GetById(26)->animation;

      sprite->unk11 = (sprite->unk11 & 3) | 32;
    }
    ((struct FieldSprite *)Object_GetById(26)->animation)->unk19 = 1;
    if (Flag_IsSet(0x8ff) == 0 && Flag_IsSet(0x8ab) != 0) {
      Call3((void (*)())Func_02002b88, 27, 0, 0);
      Call3((void (*)())Func_02002b88, 28, 0, 0);
      if (Flag_IsSet(0x280) == 0)
        Call3((void (*)())Func_02002b88, 26, 0, 0);
    } else if (Flag_IsSet(0x8ff) != 0 || Flag_IsSet(0x910) != 0 ||
               Flag_IsSet(0x280) == 0) {
      Call3((void (*)())Func_02002b88, 26, 0, 0);
    }
  }
  if (gPartyState.entrance == 9 && Flag_IsSet(0x913) == 0)
    Func_02000f80();
  if (Flag_IsSet(0x8ff) != 0 && Flag_IsSet(0x989) != 0) {
    if (gPartyState.entrance == 7 && Flag_IsSet(0x98e) == 0) {
      Func_020019d8();
      Flag_Set(0x98e);
    }
    if (Flag_IsSet(0x98e))
      Call3((void (*)())Func_02002b88, 33, 0, 0);
  } else {
    Call3((void (*)())Func_02002b88, 66, 0, 0);
  }
  return 0;
}
