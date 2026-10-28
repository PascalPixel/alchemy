#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "mem.h"
#include "iwram_call.h"
#include "resource.h"
#include "task.h"

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

void BattleEffect_DrawSceneSprite(void);
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

  mainObject = scene->mainObject;
  if (scene->mode == 0xc000) {
    scene->active = 1;
    buffer = MemAlloc(0x200);
    /* FAKEMATCH(one-pass): an empty once-only block, so the frame's zero is
     * not the active flag's; as plain statements, 12 of 103 instructions
     * differ in tla-en. */
    do {
    } while (0);
    Iwram_FillWords(buffer, 0x200, 0);
    slot = Resource_FindFreeEntry();
    tiles = VramCache_Load(slot, 0x200, buffer);
    scene->vramSlot = slot;
    MemFree(buffer);
    sprite = &scene->sprite;
    Func_080eaf98(sprite, 16, 31, 0x80000000, tiles);
    sprite->frame = 0;
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
    Task_Add(BattleEffect_DrawSceneSprite, TASK_PASS_VBLANK);
  }
}
