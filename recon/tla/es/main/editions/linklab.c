#include "types.h"

s32 Window_Create(s32, s32, s32, s32, s32);
void Text_DrawStringInWindow(u8 *s, s32 arg1, u32 arg2, u32 arg3);
void Text_DrawStringAtOffset(u8 *s, s32 window, s32 x, s32 y);
extern u8 gLink_TimeLabelString[];
s32 Link_CreateCountdownLabelWindow(void) {
  s32 handle = Window_Create(0, 0, 6, 4, 6);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
  Text_DrawStringAtOffset(gLink_TimeLabelString, handle, 0, 0);
#else
  Text_DrawStringInWindow(gLink_TimeLabelString, handle, 0, 0);
#endif
  return handle;
}
