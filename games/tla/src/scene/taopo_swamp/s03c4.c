#include "local.h"

struct SequenceParentWork254 {
  u8 unk000[0x254];
  u16 value;
};

void Func_020003c4(void) {
  Func_02002990_(76, 70, 14, 8, 14, 37, 0);
  if (!GameFlag_Test(0x9b6)) {
    Func_020029a8_(249, 344, 648);
    ((struct SequenceParentWork254 *)&gSequenceParentWork)->value = 0x19b6;
    gSequenceParentWork.battleRule24b = 2;
    Func_02002938(106, 1);
  }
}
