#include "types.h"
#include "event_inline.h"

s32 Random16Far();

s32 Func_02000ad0(void) {
  switch (Random16Far() & 7) {
  case 0:
    Sound_Play(126);
    break;
  case 1:
    Sound_Play(212);
    break;
  case 2:
    Sound_Play(134);
    break;
  case 3:
  case 5:
  case 6:
  default:
    Sound_Play(133);
    break;
  case 4:
  case 7:
    Sound_Play(154);
    break;
  }
  return 0;
}

s32 Func_02000b38(u8 *object) {
  object[90] &= 0xfe;
  return 0;
}
