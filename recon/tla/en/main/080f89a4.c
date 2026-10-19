// First draft of Func_080f89a4: scheduler callback that places every party
// member figure of the menu at its position and scale each frame, through the
// projected placement (Func_08020018, with a fifth argument 250). The twin of
// ☀️'s Menu_UpdateEntryObjectTransforms (games/tbs/src/menu/menu.c); here the
// height is 114.5 less half the row, in 16.16. Near miss, differ 2 of 77,
// instruction order only: the native copies &scale.y's base (mov r2, r8)
// before the first scale load; here the load comes first.
// 2026-10-19: the scale through a local pointer, pinned or behind a barrier, is
// 31 to 47; four minutes of permuter found nothing.
#include "types.h"
#include "ram_buffer.h"
#include "animspr.h"
#include "render_placement.h"

struct OwnerMenuState {
  u8 unk000[0xf8];
  struct AnimationObject *tabObjects[8];
  s16 tabX[8];
  s16 rowPositions[8];
  s32 ownerScale[8];
};

s32 Party_CountActiveOwnersFar(void);
void Func_08020018(struct AnimationObject *object,
                   struct SpritePlacement *position, struct SpriteScale *scale,
                   s32 depth, s32 mode);

void Func_080f89a4(void) {
  struct OwnerMenuState *menu =
      (struct OwnerMenuState *)Ram_HeapSlots->menuRuntime;
  struct SpriteScale scale;
  struct SpritePlacement position;
  s32 i;
  s32 count;

  count = (u16)Party_CountActiveOwnersFar();
  for (i = 0; i < count; i++) {
    struct AnimationObject *figure = menu->tabObjects[i];
    s32 placeY = 0x00728000 - (menu->rowPositions[i] << 15);

    if (figure != 0) {
      figure->image.priority = 0;
      scale.x = menu->ownerScale[i];
      scale.y = menu->ownerScale[i];
      position.x = menu->tabX[i] << 16;
      position.height = placeY;
      position.y = (menu->rowPositions[i] << 16) + placeY;
      position.groundHeight = 0;
      Func_08020018(figure, &position, &scale, 0x4000, 250);
    }
  }
}
