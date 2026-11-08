#include "types.h"

struct UiPanel {
  u8 unk00[0xc];
  s16 x;
  s16 y;
};

void Func_08101c7c(s32 x, s32 y, s32 width, s32 height, s32 palette);

void UiPanel_SetRectPalette(struct UiPanel *panel, s32 x, s32 y, s32 width,
                            s32 height, s32 palette) {
  Func_08101c7c(panel->x + x + 1, panel->y + y + 1, width, height, palette);
}
