#include "types.h"
#include "ram_buffer.h"
#include "sprite.h"
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

      s32 next;

      if (value < 0) {
        next = -value;
        scale.x = next;
        scale.y = next;
      } else {
        next = value + (0x10000 - value) / 4;
        scale.x = next;
        scale.y = next;
        menu->scales[i] = next;
      }
      position.x = menu->tabX[i] << 16;
      position.height = placeY;
      position.y = (menu->tabY[i] << 16) + placeY;
      position.groundHeight = 0;
      Func_08020018(figure, &position, &scale, menu->depths[i], 250);
    }
  }
}
