#include "types.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "battle.h"
#include "system.h"
#include "mem.h"
#include "resource.h"
#include "render_input.h"
#include "edition.h"

struct IconSprite {
  u8 unk00[4];
  struct RenderOam oam;
  s32 scale;
  u8 vramSlot;
  u8 unk11[3];
  u8 width;
  u8 height;
  u8 unk16;
  s8 unk17;
  u8 unk18;
  u8 unk19;
  u8 unk1a;
  u8 unk1b;
};

struct IconOwner {
  u8 unk00[80];
  struct IconSprite *sprite;
  u8 unk54[8];
  u8 unk5c;
  u8 unk5d[5];
  u8 iconId;
};

void Func_08038248(s32 id);

/// Shows icon id on its owner's 16 by 16 sprite: Func_08038248 prepares the
/// icon in heap slot 68, whose tiles at +0x400 go to the sprite's VRAM cache
/// slot, and the sprite is reset to scale 1.0, 16-colour palette 0.
// No ☀️ twin found.
void Func_0802372c(struct IconOwner *owner, s32 id) {
  struct IconSprite *sprite;
  u8 *buffer;
  s32 tile;

  if (owner == 0)
    return;
  owner->unk5c = 1;
  owner->iconId = id;
  sprite = owner->sprite;
  if (sprite == 0)
    return;
  buffer = Runtime_AllocateHeapBlock(68, 0x608);
  Func_08038248(id);
  tile = VramCache_Load(sprite->vramSlot, 128, buffer + 0x400);
  Runtime_ReleaseHeapBlock(68);
  sprite->unk1b = 0;
  sprite->unk19 = 0;
  sprite->width = 16;
  sprite->height = 16;
  sprite->unk16 = 0;
  sprite->unk17 = 4;
  sprite->scale = 0x10000;
  sprite->oam.index = tile;
  sprite->oam.palette256 = 0;
  sprite->oam.palette = 0;
}

s32 Func_080237c8(u8 *object) {
  u8 *track = *(u8 **)(object + 0x50);

  if (object != NULL && track != NULL && track != (u8 *)-0x28)
    return **(s16 **)(track + 0x28);
  return 0;
}

void Animation_WaitForTrackEnd(void *track);

void Func_080237e8(u8 *object) {
  if (object != NULL)
    Animation_WaitForTrackEnd(*(void **)(object + 0x50));
}

void Func_0802386c(void);
void ObjectSystem_UpdateCameraFixed(void);

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
// Spanish and Italian builds use different names for these two far calls,
// as in shop.c.
void Func_080c8368(s32, s32);
void BattleEffect_ApplyColorToSourceBufferFar(s32);
#endif

void Graphics_EnableObjLayerAndCallbacks(void) {
  u16 dispcnt;

  Scheduler_SuspendTasks(Func_0802386c);
  Scheduler_SuspendTasks(ObjectSystem_UpdateCameraFixed);
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
  Func_080c8368(0x10000, 1);
  BattleEffect_ApplyColorToSourceBufferFar(1);
#else
  BattleEffect_ApplyColorToTargetBufferFar(0x10000, 1);
  BattleEffect_StartBufferInterpolationFar(1);
#endif
  WaitFrames(1);
  dispcnt = REG_DISPCNT;
  /* FAKEMATCH: ⚓️ reads DISPCNT before it builds the layer mask; the plain
   * statement starts the mask first (one pair of instructions swapped). */
  asm volatile("");
  REG_DISPCNT = (dispcnt & 0xf1ff) | DISPCNT_OBJ_ON;
}

void ObjectDispatch_StopCallbacksAndHideLayers(void) {
  u16 dispcnt;

  Scheduler_ResumeTasks(Func_0802386c);
  Scheduler_ResumeTasks(ObjectSystem_UpdateCameraFixed);
  dispcnt = REG_DISPCNT;
  /* FAKEMATCH: ⚓️ reads DISPCNT before it builds the layer mask; the plain
   * statement starts the mask first (one pair of instructions swapped). */
  asm volatile("");
  REG_DISPCNT = dispcnt & 0xe1ff;
}

s32 ObjectDispatch_ReturnTrue(void) { return 1; }
