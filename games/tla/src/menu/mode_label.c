#include "types.h"
#include "ram_buffer.h"
#include "edition.h"

// The label area is narrower in Japanese and wider in French.
#if LANGUAGE == LANGUAGE_JAPANESE
#define LABEL_RIGHT 80
#elif LANGUAGE == LANGUAGE_FRENCH
#define LABEL_RIGHT 160
#else
#define LABEL_RIGHT 128
#endif

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

/// When the selected mode changes, clears the lower window's label area and
/// writes the mode's lines of text. The area starts at x 8 and ends at
/// LABEL_RIGHT; a mode past 2 draws nothing.
// ☀️'s function of the same name.
void Menu_DrawModeLabel(void) {
  struct MenuModeLabelState *state = Ram_HeapSlots->menuSelectWork;

  if (state->previousMode != state->mode) {
    state->previousMode = state->mode;
    UiWindow_ClearInteriorTiles(state->window, 8, 40, LABEL_RIGHT, 80);

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
