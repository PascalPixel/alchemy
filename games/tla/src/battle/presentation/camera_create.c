#include "types.h"
#include "macros.h"
#include "mem.h"
#include "battle_display.h"

/// Allocates the battle camera (heap slot 48) and sets its starting view.
// The values are those The Broken Seal's battle start sets inline after its
// own allocation.
void BattleCamera_Create(void) {
  struct BattleCamera *camera =
      Runtime_AllocateBlock(48, sizeof(struct BattleCamera));
  s32 *focus = camera->pos;

  SET_VECTOR(focus, 0, 0x400000, 0);
  SET_VECTOR(camera->eye, 0, 0xb40000, 0x400000);
  camera->yaw = 0x2800;
  camera->pitch = 0x5000;
  camera->distance = 0x1000000;
}
