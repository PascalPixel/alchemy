// Matches: differ 0 (tla-en, 2026-11-01)
// The five functions here share their names with resource_6ba, so measure each
// with --symbol; all five are differ 0. Func_02000054 needs each form below;
// each was measured, the passes named are inferred, not read from RTL dumps:
// - The outer loop and the two waits are built from labels: as for or while
//   loops, loop.c moves the input address and the masks out of them.
// - The message wait is a loop entered at its test from outside, which stops
//   loop.c hoisting and leaves the listing's branch into the test.
// - The hold test is written twice; cross-jumping merges the copies.
// - frames++ follows wait(1); sched2 moves it ahead of the call.
// INPUT_HELD and the volatile input pointer are the same volatile device, not
// yet tagged in the S2 form; tag and measure it at adoption. Only tla-en was
// measured: the other editions are not checked.
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

#define INPUT_HELD (*(volatile u32 *)&gInput.held)

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000054(s32 message, s32 count) {
  s32 i;
  s32 frames;
  void *window;
  s32 typed;
  volatile struct InputState *input;

  window = Func_02000d00(206, 0, 0, 0);
  i = 0;
  if (i >= count)
    goto done;
loop:
  Func_02000d08(1, 1);
  Func_02000d08(5, 2);
  Func_02000d08(0x1e240, 5);
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
