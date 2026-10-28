#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "render_input.h"
#include "types.h"
#include "ram_buffer.h"
#include "resource.h"

struct WindowSprite *WindowSprite_CreateLoaded(s32 arg0, s32 arg1,
                                               struct RenderInput *arg2,
                                               s32 arg3, s32 arg4) {
  s32 no;
  struct WindowSprite *result;

  no = Resource_FindFreeEntry();
  result = NULL;
  if (no != 0x60) {
    Icon_CopyResourceToSlot(arg0, arg1, no);
    result = WindowSprite_Create(no, OUTPUT_SIZE_16X16, arg2, arg3, arg4);
  }
  return result;
}
