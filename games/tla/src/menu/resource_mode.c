#include "tla_edition.h"
#include "types.h"
#include "ram_buffer.h"

extern char MsgPasswordTransferHelp[];
extern char MsgCableTransferHelp[];
extern char MsgClearDataTransferHelp[];

void RenderOutput_PrepareForRedraw(void *window);
void RenderOutput_ClearList(void *work);
void UiText_DrawResource(s32 message, void *window, s32 x, s32 y);

// The Spanish descriptions are indented.
#if LANGUAGE == LANGUAGE_SPANISH
#define HELP_X 6
#else
#define HELP_X 0
#endif

struct ResourceMenuWork {
  u8 unk00[124];
  void *lowerWindow;
  u8 unk80[12];
  s16 selection;
  u8 unk8e[8];
  s16 previousMode;
};

/// Redraws the selected method's description in the lower window. Spanish,
/// French and Italian describe two methods, the password and everything else
/// as the cable; French clears the window with RenderOutput_ClearList and
/// redraws only when the description changes.
void Menu_DrawModeIndicator(void) {
  struct ResourceMenuWork *state = Ram_HeapSlots->menuSelectWork;
  s16 *shown = &state->previousMode;
  s16 *current = &state->selection;

#if LANGUAGE == LANGUAGE_FRENCH
  if (*shown != *current) {
    if (*current == 0) {
      RenderOutput_ClearList(state->lowerWindow);
      UiText_DrawResource((s32)MsgPasswordTransferHelp, state->lowerWindow, 8,
                          4);
      UiText_DrawResource((s32)MsgPasswordTransferHelp + 1, state->lowerWindow,
                          0, 16);
    } else if (*shown == 0) {
      RenderOutput_ClearList(state->lowerWindow);
      UiText_DrawResource((s32)MsgCableTransferHelp, state->lowerWindow, 0, 4);
      UiText_DrawResource((s32)MsgCableTransferHelp + 1, state->lowerWindow, 0,
                          16);
      UiText_DrawResource((s32)MsgCableTransferHelp + 2, state->lowerWindow, 0,
                          28);
    }
    // Through the pointers, the selection would be loaded once at the top.
    state->previousMode = state->selection;
  }
#else
  if (*shown != *current) {
    *shown = *current;
    RenderOutput_PrepareForRedraw(state->lowerWindow);
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
    if (*current == 0) {
      UiText_DrawResource((s32)MsgPasswordTransferHelp, state->lowerWindow,
                          HELP_X, 4);
      UiText_DrawResource((s32)MsgPasswordTransferHelp + 1, state->lowerWindow,
                          HELP_X, 16);
    } else {
      UiText_DrawResource((s32)MsgCableTransferHelp, state->lowerWindow, HELP_X,
                          4);
      UiText_DrawResource((s32)MsgCableTransferHelp + 1, state->lowerWindow,
                          HELP_X, 16);
      UiText_DrawResource((s32)MsgCableTransferHelp + 2, state->lowerWindow,
                          HELP_X, 28);
    }
#else
    switch (*current) {
    case 0:
      UiText_DrawResource((s32)MsgPasswordTransferHelp, state->lowerWindow, 0,
                          4);
      UiText_DrawResource((s32)MsgPasswordTransferHelp + 1, state->lowerWindow,
                          0, 16);
      break;
    case 1:
      UiText_DrawResource((s32)MsgCableTransferHelp, state->lowerWindow, 0, 4);
      UiText_DrawResource((s32)MsgCableTransferHelp + 1, state->lowerWindow, 0,
                          16);
      UiText_DrawResource((s32)MsgCableTransferHelp + 2, state->lowerWindow, 0,
                          28);
      break;
    default:
      UiText_DrawResource((s32)MsgClearDataTransferHelp, state->lowerWindow, 0,
                          4);
      UiText_DrawResource((s32)MsgClearDataTransferHelp + 1, state->lowerWindow,
                          0, 16);
      break;
    }
#endif
  }
#endif
}
