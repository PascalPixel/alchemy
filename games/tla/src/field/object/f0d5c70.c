#include "types.h"
#include "display_transition.h"
#include "event_work.h"
#include "io_reg.h"
#include "io_write.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

void wait(s32 frames);

/// The Broken Seal's Scene_FadeColorFromWhite, playing cue 298 where that
/// plays 288: turns the backdrop (on the world map, a colour of palette 15)
/// white, finishes the screen transition and fades that colour to black
/// over 16 frames.
void Func_080d5c70(s32 unused) {
  struct EventWork *work;
  s32 i;
  s32 c;

  work = Ram_HeapSlots->eventWork;
  Sound_PlayFar(gMainRoutineWork.sceneCue);
  Sound_PlayFar(298);
  Sound_PlayFar(147);
  if (work->controlMode == 3) {
    Io_Write16(RGB(31, 31, 31), (u16 *)&BG_PLTT_COLOR(15, 3));
    ScreenTransition_Finish(0x401, 16);
    work->screenStatus = 0;
    wait(16);
    for (i = 0; i < 16; i++) {
      c = 30 - i * 2;
      Io_Write16(RGB(c, c, c), (u16 *)&BG_PLTT_COLOR(15, 3));
      wait(1);
    }
  } else {
    Io_Write16(RGB(31, 31, 31), &PLTT_BACKDROP);
    ScreenTransition_Finish(0x207, 16);
    work->screenStatus = 0;
    wait(16);
    for (i = 0; i < 16; i++) {
      c = 30 - i * 2;
      Io_Write16(RGB(c, c, c), &PLTT_BACKDROP);
      wait(1);
    }
  }
}
