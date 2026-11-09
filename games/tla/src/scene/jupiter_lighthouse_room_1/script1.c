#include "types.h"
#include "field_event.h"
#include "flag_call.h"
#include "event_inline.h"

void Func_02000060(s32 animate) {
  if (animate == 1) {
    Sound_Play(133);
    Func_0802b450(81, 20, 2, 3, 79, 29);
    Func_0802b450(81, 20, 2, 3, 79, 37);
    wait(6);
  }
  Func_0802b450(84, 20, 2, 3, 79, 29);
  Func_0802b450(84, 20, 2, 3, 79, 37);
  Map_CopyCellAttributeRect(20, 22, 2, 1, 15, 31);
  Map_CopyCellAttributeRect(20, 22, 2, 1, 15, 39);
}

void Func_020000ec(void) {
  Sound_Play(134);
  Func_0802b450(81, 20, 2, 3, 79, 29);
  Func_0802b450(81, 20, 2, 3, 79, 37);
  wait(6);
  Func_0802b450(87, 20, 2, 3, 79, 29);
  Func_0802b450(87, 20, 2, 3, 79, 37);
  Map_CopyCellAttributeRect(23, 22, 2, 1, 15, 31);
  Map_CopyCellAttributeRect(23, 22, 2, 1, 15, 39);
}

void Func_02000178(void) {
  struct FieldActor *object = ((struct FieldActor *)Actor_GetById(10));

  if (!Flag_IsSet(0xa01) && object->x >> 20 == 17 && object->z >> 20 == 33) {
    Flag_Set(0xa01);
    Func_02000060(1);
  }
}

void Func_020001b4(void) {
  if (!Flag_IsSet(0x200)) {
    Flag_Set(0x200);
    Func_02000060(1);
  }
}

void Func_020001d4(void) {
  if (Flag_IsSet(0x200) == 1) {
    Flag_Clear(0x200);
    Func_020000ec();
  }
  Func_02000178();
}
