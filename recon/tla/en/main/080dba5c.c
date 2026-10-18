// Near miss, differ 20 of 103 (2026-10-18; 46 before, counted with
// Func_080dbb40, adopted as depthend.c and now dropped here). Order only. The
// native builds the frame's zero in r2 after VramCache_Load (movs r2, #0; mov
// sl, r2); a plain zero local is set at the top of the block instead, and
// behind an empty asm (volatile or not) it measured 22. Also: the active flag's
// address before the size, the fill's arguments, the frame store before
// the bitfield loads and the last pair round the callback's literal.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "mem.h"
#include "iwram_call.h"
#include "resource.h"
#include "callback_scheduler.h"

struct DepthSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 hidden : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 layer : 4;
  u8 unk0a[2];
  s32 x;
  s32 y;
  u8 unk14[4];
  s32 scale;
  u8 unk1c[2];
  u16 frame;
  u8 unk20[8];
};

struct DepthScene {
  u16 mode;
  u8 unk02[2];
  s32 x;
  u8 unk08[8];
  struct ObjectRuntime *mainObject;
  u8 unk14[0x7b4];
  struct DepthSprite sprite;
  s16 vramSlot;
  s16 active;
};

void BattleFx_DrawSceneSprite(void);
void Func_080eaf98(struct DepthSprite *sprite, s32 a, s32 b, u32 c, s32 tiles);
void Camera_WorldToScreen(s32 *position);

void Func_080dba5c(void) {
  struct DepthScene *scene = Ram_HeapSlots->effectWork;
  struct ObjectRuntime *mainObject;
  struct DepthSprite *sprite;
  s32 position[3];
  void *buffer;
  s32 slot;
  s32 tiles;
  s32 zero;

  mainObject = scene->mainObject;
  if (scene->mode == 0xc000) {
    scene->active = 1;
    buffer = MemAlloc(0x200);
    Iwram_FillWords(buffer, 0x200, 0);
    slot = Resource_FindFreeEntry();
    tiles = VramCache_Load(slot, 0x200, buffer);
    zero = 0;
    scene->vramSlot = slot;
    MemFree(buffer);
    sprite = &scene->sprite;
    Func_080eaf98(sprite, 16, 31, 0x80000000, tiles);
    sprite->frame = zero;
    sprite->priority = 0;
    sprite->hidden = 0;
    sprite->layer = 0;
    sprite->scale = 0x20000;
    position[0] = scene->x;
    position[1] = mainObject->y + 0x1c0000;
    position[2] = mainObject->z;
    Camera_WorldToScreen(position);
    sprite->x = position[0];
    sprite->y = position[2];
    Scheduler_AddOrUpdateCallback(BattleFx_DrawSceneSprite, 0x480);
  }
}
