#include "types.h"
#include "display_transition.h"
#include "event_work.h"
#include "io_reg.h"
#include "iowrite.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

void WaitTicks(s32 ticks);

/// The Broken Seal's Scene_FadeColorFromWhite, playing cue 298 where that
/// plays 288: turns the backdrop (on the world map, a colour of palette 15)
/// white, finishes the display transition and fades that colour to black
/// over 16 frames.
void Func_080d5c70(s32 unused) {
  struct EventWork *work;
  s32 i;
  s32 c;

  work = Ram_HeapSlots->eventWork;
  Audio_PlayCue(gPartyState.sceneCue);
  Audio_PlayCue(298);
  Audio_PlayCue(147);
  if (work->controlMode == 3) {
    Io_Write16(0x7fff, (u16 *)0x050001e6);
    DisplayTransition_Finish(0x401, 16);
    work->screenStatus = 0;
    WaitTicks(16);
    for (i = 0; i < 16; i++) {
      c = 30 - i * 2;
      Io_Write16((c << 10) | (c << 5) | c, (u16 *)0x050001e6);
      WaitTicks(1);
    }
  } else {
    Io_Write16(0x7fff, &PLTT_BACKDROP);
    DisplayTransition_Finish(0x207, 16);
    work->screenStatus = 0;
    WaitTicks(16);
    for (i = 0; i < 16; i++) {
      c = 30 - i * 2;
      Io_Write16((c << 10) | (c << 5) | c, &PLTT_BACKDROP);
      WaitTicks(1);
    }
  }
}
