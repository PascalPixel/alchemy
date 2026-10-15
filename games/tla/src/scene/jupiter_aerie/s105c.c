#include "jupiter_aerie.h"

void Func_0200105c(struct ObjectRuntime *object) {
  BYTE_AT(object, 99) = 0;
  WORD_AT(object, 108) = (s32)Func_02001010;
}

void Func_02001070(struct ObjectRuntime *object) {
  WORD_AT(object, 108) = 0;
  Animation_ApplyChildValues(object, 4);
}
