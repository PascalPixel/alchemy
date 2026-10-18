// First draft of MenuIcon_UpdateTransforms: scheduler callback that, while
// the menu's byte at 0x50a is set, places its four figures each frame through
// the projected placement (Func_08020018, depth from a table and mode 250),
// easing each figure's scale a quarter of the way to 1.0 per frame (a
// negative scale is used negated and kept). Built as Func_080f89a4
// (recon/tla/en/main/080f89a4.c), ☀️'s Menu_UpdateEntryObjectTransforms.
// Differ 18 of 108, instruction order only: the setup of the walking
// offsets 0x230 and 0x220 is interleaved differently, and the native stores
// scale.x before the move to scale.y's base where this draft stores scale.y
// first (writing scale.y = scale.x gives 25).
#include "types.h"
#include "ram_buffer.h"
#include "animspr.h"
#include "render_placement.h"

struct FigureMenuState {
  u8 unk000[0x128];
  s16 rows[4];
  u8 unk130[0xf0];
  struct AnimationObject *figures[4];
  s16 tabX[4];
  s16 tabY[4];
  s32 scales[4];
  u16 depths[4];
  u8 unk258[0x2b2];
  s8 active;
};

void Func_08020018(struct AnimationObject *object,
                   struct SpritePlacement *position, struct SpriteScale *scale,
                   s32 depth, s32 mode);

void MenuIcon_UpdateTransforms(void) {
  struct FigureMenuState *menu =
      (struct FigureMenuState *)Ram_HeapSlots->menuRuntime;
  struct SpriteScale scale;
  struct SpritePlacement position;
  s32 i;

  if (menu->active == 0)
    return;
  for (i = 0; i <= 3; i++) {
    struct AnimationObject *figure = menu->figures[i];

    if (figure != NULL) {
      s32 placeY = 0x00728000 - (menu->rows[i] << 15);
      s32 value = menu->scales[i];

      if (value < 0) {
        scale.x = scale.y = -value;
      } else {
        scale.x = scale.y = value + (0x10000 - value) / 4;
        menu->scales[i] = scale.x;
      }
      position.x = menu->tabX[i] << 16;
      position.height = placeY;
      position.y = (menu->tabY[i] << 16) + placeY;
      position.groundHeight = 0;
      Func_08020018(figure, &position, &scale, menu->depths[i], 250);
    }
  }
}
