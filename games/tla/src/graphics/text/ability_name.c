#include "types.h"
#include "text/msg_ids.h"

extern const u8 gData_0805eb50[];

void UiText_DrawCharacterAtOffset();
void UiText_DrawStringInWindow();

/// Draw the ability name; index zero displays "----".
void Func_08041268(s32 ability, s32 arg1, s32 arg2, s32 arg3) {
  if (ability != 0)
    UiText_DrawCharacterAtOffset(ability + (s32)MsgAbilityNothing, arg1, arg2,
                                 arg3);
  else
    UiText_DrawStringInWindow(gData_0805eb50, arg1, arg2, arg3);
}
