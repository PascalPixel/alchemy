#include "types.h"
#include "text/msg_ids.h"
#include "edition.h"

void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
s32 Djinn_AddToLeastLoadedOwner(s32 element, s32 djinn);
void Func_02000dc0(void);

void GameFlag_SetBit(s32 flag);
void Djinn_AddToOwner(s32 owner, s32 element, s32 djinn);
void Djinn_Activate(s32 owner, s32 element, s32 djinn);

void Func_020005fc(s32 owner, s32 element, s32 djinn, s32 activate) {
  GameFlag_SetBit(element * 20 + djinn + 48);
  Djinn_AddToOwner(owner, element, djinn);
  if (activate == 1)
    Djinn_Activate(owner, element, djinn);
}

#if (LANGUAGE == LANGUAGE_JAPANESE)
/// Japanese edition: give each of four characters a Djinni set starting at 7.
void Func_02000638(void) {
  s32 djinn;

  UiText_ShowPositionedMessageAndWait((s32)MsgYouGotA, 1);
  for (djinn = 7; djinn < 13; djinn++)
    Func_020005fc(4, 0, djinn, 1);
  for (djinn = 7; djinn < 14; djinn++)
    Func_020005fc(5, 2, djinn, 1);
  for (djinn = 7; djinn < 13; djinn++)
    Func_020005fc(6, 3, djinn, 1);
  for (djinn = 7; djinn < 14; djinn++)
    Func_020005fc(7, 1, djinn, 1);
  Func_02000dc0();
}
#else
void Func_02000638(void) {
  s32 element;
  s32 djinn;

  UiText_ShowPositionedMessageAndWait((s32)MsgYouGotA, 1);
  for (element = 0; element < 4; element++) {
    for (djinn = 0; djinn < 18; djinn++)
      Djinn_AddToLeastLoadedOwner(element, djinn);
  }
  Func_02000dc0();
}
#endif

void Func_02000664(void) {}
