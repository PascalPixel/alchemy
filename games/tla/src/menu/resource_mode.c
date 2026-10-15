#include "tla_edition.h"
#include "types.h"
#include "ram_buffer.h"

extern char MsgPasswordTransferHelp[];
extern char MsgCableTransferHelp[];
extern char MsgClearDataTransferHelp[];

void RenderOutput_PrepareForRedraw(void *window);
void UiText_DrawResource(s32 message, void *window, s32 x, s32 y);

struct ResourceMenuWork {
  u8 unk00[124];
  void *lowerWindow;
  u8 unk80[12];
  s16 selection;
  u8 unk8e[8];
  s16 previousMode;
};

// えらんだ方法の説明を下のウィンドウに描きなおす
void Menu_DrawModeIndicator(void) {
  struct ResourceMenuWork *state = Ram_HeapSlots->menuSelectWork;
  s16 *shown = &state->previousMode;
  s16 *current = &state->selection;

  if (*shown != *current) {
    *shown = *current;
    RenderOutput_PrepareForRedraw(state->lowerWindow);
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
  }
}
