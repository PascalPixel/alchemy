#include "types.h"

struct MenuPanel {
  u8 unk00[12];
  s16 x;
  s16 y;
};

extern char MsgStatusNameNormal[];
extern char MsgStatusNameDown[];

void MenuIcon_SetSlot(s32 index, u32 x, s32 mode);
void Text_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);

// Lists usable entries with icons and names, or "none" when no entries qualify.
void Func_081054e0(struct MenuPanel *panel, s32 x, s32 y, const u8 *enabled) {
  s32 count = 0;
  s32 icon_x = panel->x * 8 + x + 8;
  s32 icon_y = panel->y * 8 + y + 8;
  s32 i;

  for (i = 0; i <= 4; i++) {
    if (enabled[i] != 0) {
      MenuIcon_SetSlot(i, icon_x, icon_y);
      Text_DrawCharacterAtOffsetFar((s32)MsgStatusNameDown + i, panel, x + 16,
                                    y);
      y += 16;
      icon_y += 16;
      count++;
    } else {
      MenuIcon_SetSlot(i, -16, -16);
    }
  }
  if (count == 0)
    Text_DrawCharacterAtOffsetFar((s32)MsgStatusNameNormal, panel, x, y);
}
