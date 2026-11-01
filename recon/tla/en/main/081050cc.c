// Draft, differ 12 of 108, skeleton gap 6 (tla-en, 2026-11-01)
// MenuIcon_UpdateTransforms: task that, while the menu's byte at 0x50a is
// set, places its four figures each frame through the projected placement
// (Func_08020018, depth from a table and mode 250), easing each figure's scale
// a quarter of the way to 1.0 per frame (a negative scale is used negated and
// kept). Twin of ☀️'s DjinnFigures_UpdateBottomRow: same work layout (figures
// at 0x220, then x, y, scale phases), same (16, 200) and 1.0 start in
// Func_081051a8, same negative-phase rule; here a third becomes a quarter, the
// height 482 becomes 458, and the page arrows and facing are gone.
// Left: instruction order in the loop's setup only. The native builds the
// walking offsets 0x230 and 0x220 and the spill of the second (str r1,
// [sp, #4], reloaded at once) as one chain ahead of the other setup, the load
// and store pattern AGENTS.md records for ⚓️.
// Tried: the twin's whole form (differ 25: scale.y = scale.x reloads, so the
// new scale stays in its own local, stored to x and then y; differ 18 with
// x = y = value); do-while and while (1) loops, i < 4, guard wrapping the
// loop, the twin's declaration order, function-scope locals, u32 height (all
// 12); placeY before the null test (18); explicit walking pointers (47);
// shared scale writes after the if (97).
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
