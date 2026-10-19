// Draft, differ 2 of 92 (English and Japanese; the other four editions place
// the commands at other columns and need their own branch): The Broken Seal's
// ItemMenu_DrawCommands. The native builds the disabled marker -1 around the first
// state load (movs r2, #1; movs r3, #0; ldrsb r3, [r5, r3]; negs r2, r2); this
// form negates before the load. Assigning the marker first, a one-pass block or
// an empty asm between the load and the marker, and comparing with -1 directly
// measured 2 or 6; three minutes of alchemy permute found nothing better.
#include "types.h"

extern char MsgItemCommandUse[];

s32 UiWork_SetParamNibbleFar(s32 color);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

// Command order (indices into command_states).
enum {
  ITEM_COMMAND_USE,
  ITEM_COMMAND_EQUIP,
  ITEM_COMMAND_INSPECT,
  ITEM_COMMAND_GIVE,
  ITEM_COMMAND_REMOVE,
  ITEM_COMMAND_DROP
};

#define ITEM_TEXT_X 40

// Draw six item commands; unavailable commands use color 14.
void ItemMenu_DrawCommands(const s8 *command_states, s32 window) {
  s32 disabled;
  s32 value;
  u32 message;

  UiWork_SetParamNibbleFar(0xf);
  value = command_states[ITEM_COMMAND_USE];
  disabled = -1;
  if (value == disabled)
    UiWork_SetParamNibbleFar(0xe);

  message = (u32)MsgItemCommandUse;
  UiText_DrawCharacterAtOffsetFar(message, window, 0, 0x18);
  UiWork_SetParamNibbleFar(0xf);
  if (command_states[ITEM_COMMAND_EQUIP] == disabled)
    UiWork_SetParamNibbleFar(0xe);

  UiText_DrawCharacterAtOffsetFar(message + 1, window, ITEM_TEXT_X, 0x18);
  UiWork_SetParamNibbleFar(0xf);
  if (command_states[ITEM_COMMAND_GIVE] == disabled)
    UiWork_SetParamNibbleFar(0xe);

  UiText_DrawCharacterAtOffsetFar(message + 2, window, 0, 0x20);
  UiWork_SetParamNibbleFar(0xf);
  if (command_states[ITEM_COMMAND_DROP] == disabled)
    UiWork_SetParamNibbleFar(0xe);

  UiText_DrawCharacterAtOffsetFar(message + 3, window, 0x50, 0x20);
  UiWork_SetParamNibbleFar(0xf);
  if (command_states[ITEM_COMMAND_INSPECT] == disabled)
    UiWork_SetParamNibbleFar(0xe);

  UiText_DrawCharacterAtOffsetFar(message + 4, window, 0x50, 0x18);
  UiWork_SetParamNibbleFar(0xf);
  if (command_states[ITEM_COMMAND_REMOVE] == disabled)
    UiWork_SetParamNibbleFar(0xe);

  UiText_DrawCharacterAtOffsetFar(message + 5, window, ITEM_TEXT_X, 0x20);
  UiWork_SetParamNibbleFar(0xf);
}
