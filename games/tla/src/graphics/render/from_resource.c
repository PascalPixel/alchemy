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

struct RenderOutput *RenderOutput_CreateFromResource(s32 arg0, s32 arg1,
                                                     struct RenderInput *arg2,
                                                     s32 arg3, s32 arg4) {
  s32 no;

  no = Resource_LoadByMode(arg0, arg1);
  if (no < 0) {
    return NULL;
  }
  return RenderOutput_Create(no, 0x40000000, arg2, arg3, arg4);
}
