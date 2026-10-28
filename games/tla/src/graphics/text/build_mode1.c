#include "types.h"

s32 Message_Decode(s32 message, s32 mode);

s32 Message_DecodeMode1(s32 message) {
  return Message_Decode(message, 1);
}
