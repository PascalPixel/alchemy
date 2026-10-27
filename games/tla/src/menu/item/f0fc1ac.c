#include "types.h"
#include "system.h"
#include "input.h"
#include "edition.h"

// The French question starts at the window's left edge.
#if LANGUAGE == LANGUAGE_FRENCH
#define ITEM_DROP_QUESTION_X 0
#else
#define ITEM_DROP_QUESTION_X 8
#endif

extern u8 MsgItemName[];
extern u8 MsgConfirmDrop[];
extern u8 MsgYes[];

s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
void UiWork_FinalizeFar(s32, s32);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Item_Get(s32);
s32 GameFlag_Test(s32);
void Func_080f8a44(s32, s32);
void Func_080f8ab4(s32, s32);
void Audio_PlayCue(s32 cue);

/// Asks whether to drop item (an inventory entry): its name, the question, and
/// Yes or No, No first. Returns 0 for Yes and 1 for No, B or Start. The twin of
/// The Broken Seal's ItemMenu_ConfirmDrop.
s32 Func_080fc1ac(s32 item) {
  s32 window;
  s32 itemId;
  s32 question;
  s32 answer;
  s32 rows = 2;
  s32 selection = 1;
  s32 changed = 1;

  window = UiWindow_CreateFar(13, 3, 17, 10, 2);
  itemId = item & 0x1ff;
  Item_Get(itemId);
  UiText_DrawCharacterAtOffsetFar(itemId + (s32)MsgItemName, window, 24, 0);
  question = (s32)MsgConfirmDrop;
  UiText_DrawCharacterAtOffsetFar(question, window, ITEM_DROP_QUESTION_X, 16);
  question++;
  UiText_DrawCharacterAtOffsetFar(question, window, ITEM_DROP_QUESTION_X, 24);
  answer = (s32)MsgYes;
  UiText_DrawCharacterAtOffsetFar(answer, window, 24, 40);
  answer++;
  UiText_DrawCharacterAtOffsetFar(answer, window, 24, 56);
  Func_080f8ab4(104, 86);
  for (;;) {
    if (GameFlag_Test(336))
      break;
    if (changed) {
      changed = 0;
      selection = (selection + rows) % rows;
    }
    if (gInput.pressed & 1) {
      Audio_PlayCue(112);
      break;
    }
    if (gInput.pressed & 2) {
      Audio_PlayCue(113);
      selection = 1;
      break;
    }
    Func_080f8a44(104, (selection << 4) + 70);
    if (gInput.repeat & 0x40) {
      selection--;
      changed = 1;
      Audio_PlayCue(111);
    }
    if (gInput.repeat & 0x80) {
      selection++;
      changed = 1;
      Audio_PlayCue(111);
    }
    WaitTicks(1);
  }
  if (GameFlag_Test(336)) {
    selection = 1;
  }
  UiWork_FinalizeFar(window, 1);
  return selection;
}
