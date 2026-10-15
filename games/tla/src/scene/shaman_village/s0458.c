#include "shaman_village.h"

void Func_02000458(struct ShamanVillageObject *object) {
  s32 bit = (((u32)gFrameCount & 3) >> *((u8 *)object + 99)) & 1;

  Object_SetPartAttribute(object, (u8)(bit * *((u8 *)object + 98)));
}
