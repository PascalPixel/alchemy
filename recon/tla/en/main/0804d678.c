// Draft of Menu_DrawModeLabel: differ 0 of 97 (tla-en, 2026-10-18), ready to
// adopt. Its ☀️ twin is games/tbs/src/window/battle_command.c's function
// of the same name: when the selected mode changes, it clears the lower
// window's label area and writes the mode's lines of text. The Lost Age's
// area is 128 wide at x 8, and a mode past 2 draws nothing.
// The listing's message 0x117c (4476) is MsgPasswordLevel in en.po.
#include "types.h"
#include "ram_buffer.h"

struct MenuModeLabelState {
  u8 unk00[0x7c];
  void *window;
  u8 unk80[0x0c];
  s16 mode;
  u8 unk8e[8];
  s16 previousMode;
};

extern u8 MsgPasswordLevel;

void UiWindow_ClearInteriorTiles(void *window, s32 x, s32 y, s32 right,
                                 s32 bottom);
void UiText_DrawCharacterAtOffset(s32 message, void *window, s32 x, s32 y);

void Menu_DrawModeLabel(void) {
  struct MenuModeLabelState *state = Ram_HeapSlots->menuSelectWork;

  if (state->previousMode != state->mode) {
    state->previousMode = state->mode;
    UiWindow_ClearInteriorTiles(state->window, 8, 40, 128, 80);

    switch (state->mode) {
    case 0: {
      s32 text = (s32)&MsgPasswordLevel;

      UiText_DrawCharacterAtOffset(text, state->window, 8, 40);
      UiText_DrawCharacterAtOffset(text + 1, state->window, 8, 48);
      UiText_DrawCharacterAtOffset(text + 2, state->window, 8, 56);
      UiText_DrawCharacterAtOffset(text + 3, state->window, 8, 64);
      text += 4;
      UiText_DrawCharacterAtOffset(text, state->window, 8, 72);
      break;
    }
    case 1: {
      s32 text = (s32)&MsgPasswordLevel;

      UiText_DrawCharacterAtOffset(text, state->window, 8, 40);
      UiText_DrawCharacterAtOffset(text + 1, state->window, 8, 48);
      text += 2;
      UiText_DrawCharacterAtOffset(text, state->window, 8, 56);
      break;
    }
    case 2: {
      s32 text = (s32)&MsgPasswordLevel;

      UiText_DrawCharacterAtOffset(text++, state->window, 8, 40);
      UiText_DrawCharacterAtOffset(text, state->window, 8, 48);
      break;
    }
    }
  }
}
