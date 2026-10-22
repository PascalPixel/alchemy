#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct DepthPart {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 unk09_4 : 4;
};

struct DepthMapBase {
  u8 unk00[4];
  s32 base;
};

struct DepthScroll {
  s32 unk00;
  s32 offset;
};

struct DepthMap {
  struct DepthMapBase *base;
  u8 unk04[0xe0];
  struct DepthScroll scroll;
};

s32 Render_ProjectPoint(const void *point, s32 *screen);
void Func_080eb01c(void *work);

/// Per-frame callback: draws the effect scene's sprite (effect work +0x7c8).
void BattleEffect_DrawSceneSprite(void) {
  Func_080eb01c((u8 *)Ram_HeapSlots->effectWork + 0x7c8);
}
