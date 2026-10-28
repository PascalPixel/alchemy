#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"

#define Actor_SetPartAttribute ((void (*)(void *, s32))Actor_SetPartAttribute)

extern u32 gFrameCount;

s32 OverlayObject_UpdateEveryFourFrames(void *obj) {
  if ((gFrameCount & 3) == 0)
    Actor_SetPartAttribute(obj, 7);
  else
    Actor_SetPartAttribute(obj, 0);

  if ((gFrameCount & 7) == 0)
    Import_SoundPlay(138);
  return 0;
}
