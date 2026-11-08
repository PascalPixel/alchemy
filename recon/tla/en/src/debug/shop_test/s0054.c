#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "input.h"
#include "text/msg_ids.h"

void *Func_02000d00(s32, s32, s32, s32);
void Func_02000d08(s32, s32);
void Message_CloseFinishedWindows(void);
void Message_OpenWindow(s32, s32, s32, s32);
s32 Message_AllTyped(void);

#define INPUT_HELD (gInput.held)

void Func_02000054(s32 message, s32 count) {
  s32 i;
  s32 frames;
  void *window;
  s32 typed;
  volatile struct InputState *input;

  /* FAKEMATCH(volatile): the input read through a volatile pointer; as a plain
   * pointer, 31 of 94 instructions differ in tla-en. */
  window = Func_02000d00(206, 0, 0, 0);
  i = 0;
  if (i >= count)
    goto done;
loop:
  Func_02000d08(1, 1);
  Func_02000d08(5, 2);
  Func_02000d08(SPLIT_CONSTANT(SPLIT_CONSTANT(0x1e200, 64)), 5);
  Message_CloseFinishedWindows();
  Message_OpenWindow(message, 5, 0, 0x22);
  frames = 0;
  if (INPUT_HELD & 3)
    goto shown;
wait:
  wait(1);
  frames++;
  if (frames > 59)
    goto shown;
  if (!(INPUT_HELD & 3))
    goto wait;
  for (;;) {
  shown:
    typed = Message_AllTyped();
    input = (volatile struct InputState *)&gInput;
    if (typed != 0)
      break;
    input->held = 1;
    input->pressed = 1;
    input->repeat = 1;
    wait(1);
  }
  frames = 0;
  if (input->held & 2)
    goto done;
  if (!(input->held & 1))
    goto hold;
  goto next;
hold:
  wait(1);
  frames++;
  if (frames > 9)
    goto next;
  input = (volatile struct InputState *)&gInput;
  if (input->held & 2)
    goto done;
  if (!(input->held & 1))
    goto hold;
next:
  message++;
  i++;
  if (i < count)
    goto loop;
done:
  Message_CloseFinishedWindows();
  Window_Close(window, 2);
}
