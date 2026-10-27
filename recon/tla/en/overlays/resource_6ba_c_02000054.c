// Near miss of Func_02000054 (94 native instructions, 97 differ in alignment):
// the same flow, calls and constants. The draft moves the input address and the
// masks 3 and 1 out of the message loop into r6, r7 and r11 and keeps the
// window on the stack; the native builds them at each test and keeps message,
// count, index and window in r6, r8, r7 and r10. As for loops the two waits
// measured worse (108 differ).
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"
#include "input.h"
#include "text/msg_ids.h"

void *Func_02000d00(s32, s32, s32, s32);
void Func_02000d08(s32, s32);
void UiWork_FinalizePendingCore(void);
void UiText_OpenMessageWindow(s32, s32, s32, s32);
s32 UiWork_IsComplete(void);

#define INPUT_HELD (*(volatile u32 *)&gInput.held)

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  return gScenePlacements;
}

void Func_02000054(s32 message, s32 count) {
  s32 i;
  s32 frames;
  void *window;

  window = Func_02000d00(206, 0, 0, 0);
  for (i = 0; i < count; i++) {
    Func_02000d08(1, 1);
    Func_02000d08(5, 2);
    Func_02000d08(0x1e240, 5);
    UiWork_FinalizePendingCore();
    UiText_OpenMessageWindow(message, 5, 0, 0x22);
    /* FAKEMATCH: as for loops, loop.c moves the input address and masks out of
     * the waits; ⚓️ builds them at each test. */
    frames = 0;
    if (INPUT_HELD & 3)
      goto shown;
  wait:
    frames++;
    WaitTicks(1);
    if (frames > 59)
      goto shown;
    if (!(INPUT_HELD & 3))
      goto wait;
    goto shown;
  skip:
    gInput.held = 1;
    gInput.pressed = 1;
    gInput.repeat = 1;
    WaitTicks(1);
  shown:
    if (UiWork_IsComplete() == 0)
      goto skip;
    frames = 0;
    goto check;
  hold:
    frames++;
    WaitTicks(1);
    if (frames > 9)
      goto next;
  check:
    if (INPUT_HELD & 2)
      goto done;
    if (!(INPUT_HELD & 1))
      goto hold;
  next:
    message++;
  }
done:
  UiWork_FinalizePendingCore();
  UiWork_Finalize(window, 2);
}
