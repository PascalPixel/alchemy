#include "types.h"

void Func_080ab21c(s32 x, s32 y, s32 width, s32 height, u32 palette);
void UiWindow_ApplyRectAtObjectOrigin(void *obj, s32 x, s32 y, s32 width, s32 height, s32 palette)
{
  int ofs;
  ofs = 0xC;
  Func_080ab21c(((*((u16 *) (((u8 *) obj) + ofs))) + x) + 1, ((*((u16 *) (((u8 *) obj) + 0xE))) + y) + 1, width, height, palette);
}
